/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KSU_HOOK_H
#define _LINUX_KSU_HOOK_H

#include <linux/cache.h>
#include <linux/types.h>

#ifdef CONFIG_KSU
struct filename;
struct file;

extern bool ksu_execveat_hook __read_mostly;
extern bool ksu_init_rc_hook __read_mostly;
extern bool ksu_input_hook __read_mostly;
int ksu_handle_execveat(int *fd, struct filename **filename, void *argv,
		       void *envp, int *flags);
int ksu_handle_execveat_sucompat(int *fd, struct filename **filename, void *argv,
				void *envp, int *flags);
int ksu_handle_faccessat(int *dfd, const char __user **filename, int *mode,
		       int *flags);
int ksu_handle_stat(int *dfd, const char __user **filename, int *flags);
int ksu_handle_vfs_read(struct file **file, char __user **buf, size_t *count,
		      loff_t **pos);
int ksu_handle_input_handle_event(unsigned int *type, unsigned int *code,
				int *value);
void ksu_handle_init_rc_stat(struct file *file, loff_t *size);
int ksu_handle_sys_reboot(int magic1, int magic2, unsigned int cmd,
			void __user **arg);
int ksu_handle_slow_avc_audit(u32 *tsid);
#endif

#endif /* _LINUX_KSU_HOOK_H */
