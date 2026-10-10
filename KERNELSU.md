# KernelSU manual-hook integration

This branch starts at lineage-22.2 commit
`cbbc069866f7b9957337153ff12c9807b8c46bef` (Linux 4.19.325).
It vendors the kernel portion of tiann/KernelSU v0.9.5 at
`b766b98513b5a7eb33bc1c4a76b5702bf1288f07`, release version 11872.
Use the official matching **KernelSU v0.9.5 Manager**. This is an older
non-GKI integration baseline, not a claim of current upstream support.

## Hooks

- `do_execveat_common`: Android init/bootstrap and su compatibility.
- `do_faccessat` and `vfs_statx`: su path compatibility.
- `vfs_read`: init.rc injection.
- `input_handle_event`: volume-down safe mode.
- `devpts_get_priv`: terminal SELinux compatibility.
- KernelSU's built-in LSM registers prctl, rename, and setuid hooks.

KernelSU is deliberately **built-in only**. Its Kconfig requires SELinux,
OverlayFS, and disabled kprobes. No kprobe runtime hooks are registered.
The vendored version is fixed; builds never fetch upstream or derive a
KernelSU version from the Motorola kernel's git history.

The upstream `embed_ksud.c` stub is retained. Install the matching Manager
and complete its userspace installation; an embedded standalone daemon
is not provided by this source integration.

## Configuration and build

First use the **correct device/ROM defconfig and Motorola fragments**.
Do not assume a generic MT6853 config is suitable for your phone.
After all device fragments, merge `kernel/configs/ksu.config` last:

```sh
# OUT must be your existing configured out-of-tree build directory.
# Keep your ROM's compiler/toolchain variables on all make commands.
sh scripts/kconfig/merge_config.sh -m -O "$OUT" \
  "$OUT/.config" kernel/configs/ksu.config
make O="$OUT" ARCH=arm64 LLVM=1 LLVM_IAS=1 olddefconfig
```

Before compiling, confirm `.config` contains `CONFIG_KSU=y`,
`CONFIG_OVERLAY_FS=y`, and `CONFIG_SECURITY_SELINUX=y`, and does NOT contain
`CONFIG_KPROBES=y` or `CONFIG_KSU=m`. If this fails, resolve dependencies
rather than claiming KernelSU is enabled.

Use the device/ROM's normal image, DTB/DTBO, and module build targets.
Do not disable CFI/LTO or other device security settings just to obtain a
successful build. The repository's MTK build configuration references an
Android Clang toolchain; device-image validation should use the matching
ROM toolchain rather than assuming any host Clang release is equivalent.

`path_umount` is not backported here; pre-5.9 KernelSU module-unmount
functionality is therefore not available. Compilation alone cannot verify
booting, Manager authorization, SELinux behavior, modules, or safe mode.
No flashable boot image is claimed without a device-target build and test.
