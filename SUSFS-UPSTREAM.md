# SUSFS upstream provenance

Repository: https://gitlab.com/simonpunk/susfs4ksu
Branch: kernel-4.19
Commit: 001e69919c6271f690fd00b17e4c721c9e599152
Kernel-side version: v1.5.5

Inputs:
- kernel_patches/50_add_susfs_in_kernel-4.19.patch
- kernel_patches/KernelSU/10_enable_susfs_for_ksu.patch
- kernel_patches/fs/susfs.c
- kernel_patches/include/linux/susfs.h
- kernel_patches/include/linux/susfs_def.h

The deprecated sus_su implementation is not imported or enabled.

Upstream's GPLv3 license is retained verbatim at
Documentation/susfs/LICENSE.upstream. The base Linux/KernelSU sources retain
their own licenses. This record does not resolve license compatibility;
review upstream licensing and obligations before redistributing a combined
source or binary product.

Local port adaptations:
- Manual-hook, built-in KernelSU v0.9.5; kprobes remain disabled.
- Adapt new-mount hooks to this tree's vfs_create_mount/fs_context backport.
- Preserve mount->data allocation/cloning hooks and Motorola task-turbo fields.
- Reuse free task_struct KABI slots 7/8, leaving the futex 3-6 block unchanged.
- Reuse vfsmount KABI slot 4; no extra field is appended.
- Preserve legacy SELinux compatibility and existing devpts domain.
- Add get_cred_rcu and path_umount explicitly, without build-time source edits.
- Fix path-reference leaks on early exits from the unmount helper.
- Correct helper declarations for independent TRY_UMOUNT configuration.
- OverlayFS configuration, not Magic Mount; riskier features are opt-in.
