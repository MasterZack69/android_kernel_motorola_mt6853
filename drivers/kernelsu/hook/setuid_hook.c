#include <linux/compiler.h>
#include <linux/version.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/task_work.h>
#include <linux/thread_info.h>
#include <linux/seccomp.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/uidgid.h>

#include "policy/allowlist.h"
#include "hook/setuid_hook.h"
#include "klog.h" // IWYU pragma: keep
#include "manager/manager_identity.h"
#include "infra/seccomp_cache.h"
#include "supercall/supercall.h"
#include "hook/hook_manager.h"
#include "feature/kernel_umount.h"
#include "compat/kernel_compat.h"

extern void disable_seccomp(void);

int ksu_handle_setresuid(uid_t old_uid, uid_t new_uid)
{
    // we rely on the fact that zygote always call setresuid(3) with same uids

    pr_info("handle_setresuid from %d to %d\n", old_uid, new_uid);

    if (unlikely(is_uid_manager(new_uid))) {

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
        if (current->seccomp.mode == SECCOMP_MODE_FILTER && current->seccomp.filter) {
            ksu_seccomp_allow_cache(current->seccomp.filter, __NR_reboot);
        }
#else
		disable_seccomp();
#endif

#ifdef KSU_KPROBES_HOOK
        ksu_set_task_tracepoint_flag(current);
#endif

        pr_info("install fd for manager: %d\n", new_uid);
        ksu_install_fd();
        return 0;
    }

    if (ksu_is_allow_uid_for_current(new_uid)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
        if (current->seccomp.mode == SECCOMP_MODE_FILTER && current->seccomp.filter) {
            ksu_seccomp_allow_cache(current->seccomp.filter, __NR_reboot);
        }
#else
		disable_seccomp();
#endif

#ifdef KSU_KPROBES_HOOK
		ksu_set_task_tracepoint_flag(current);
#endif
	} else {
#ifdef KSU_KPROBES_HOOK
		ksu_clear_task_tracepoint_flag_if_needed(current);
#endif
    }

    // Handle kernel umount
    ksu_handle_umount(old_uid, new_uid);

    return 0;
}

void __init ksu_setuid_hook_init(void)
{
	ksu_kernel_umount_init();
}

void __exit ksu_setuid_hook_exit(void)
{
	pr_info("ksu_core_exit\n");
	ksu_kernel_umount_exit();
}
#ifdef CONFIG_KSU_SUSFS
#include <linux/susfs_def.h>
#include "selinux/selinux.h"
extern struct work_struct susfs_extra_works;

/* Called only after UID and LSM validation, before commit_creds(), so the
 * zygote SID is still available. Never unmount zygote_next's init namespace. */
int ksu_susfs_handle_setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
    bool next;
    bool hide;

    if (current_uid().val != 0 || ruid == (uid_t)-1 ||
        ruid != euid || ruid != suid)
        return 0;
    next = susfs_is_sid_equal(current_cred(), susfs_zygote_next_sid);
    if (!next && !susfs_is_sid_equal(current_cred(), susfs_zygote_sid))
        return 0;

    /* Forks inherit flags; do not carry stale marks into a newly specialized
     * manager or root-allowed application. */
    susfs_clear_current_proc_no_su();
    susfs_clear_current_proc_umounted();
    susfs_clear_current_proc_umounted_for_zygote_next();

    if (ksu_is_manager_appid_valid() && is_uid_manager(ruid)) {
        disable_seccomp();
        ksu_install_fd();
        return 0;
    }
    hide = is_isolated_process(ruid) ||
           (is_appuid(ruid) && ksu_uid_should_umount(ruid));
    if (hide) {
        susfs_set_current_proc_no_su();
        susfs_set_current_proc_umounted();
        if (next)
            susfs_set_current_proc_umounted_for_zygote_next();
        else
            ksu_handle_umount(current_uid().val, ruid);
        queue_work(system_unbound_wq, &susfs_extra_works);
    } else if (ksu_is_allow_uid_for_current(ruid)) {
        disable_seccomp();
    } else {
        susfs_set_current_proc_no_su();
    }
    return 0;
}
#endif
