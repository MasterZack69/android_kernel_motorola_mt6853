# KernelSU-Next v3.4.0-legacy — Kyoto source port

## Provenance and status

This branch copies `ksu-linage-22.2` at
`b6d8b2f43eff37453e1e7f84e9ae634c1795cc70`, including its existing FocalTech
gesture update. It replaces the old official KernelSU v0.9.5 implementation
with KernelSU-Next **v3.4.0-legacy**, pinned at
`8af3d4fec33be32fa5d3f8dae4f380fac45bf2cd`.

**Full standalone vendor-config build passed** with Clang/LLVM 15.0.7 and
CFI/LTO enabled. See KERNELSU-VALIDATION.md for exact source, configuration
and hashes. This kernel is still **not booted or runtime-tested**; no flashable
boot image is supplied. The earlier v0.9.5 build is a separate validation.
This branch contains **no SUSFS**, no userspace binaries, no APK, and no
flashing, AVB or slot-switching changes.

`drivers/kernelsu` vendors upstream `kernel/`, with the upstream `uapi/`
symlink materialized as ordinary files in `include/uapi`. GPL-2.0 is retained.
See `drivers/kernelsu/UPSTREAM` and `SOURCE-MANIFEST.json` for exact provenance.

## Integration

- Built-in `CONFIG_KSU=y`, `CONFIG_KSU_MANUAL_HOOK=y`; probes and syscall-table
  patching are disabled. The local Kconfig deliberately restricts this branch
  to its audited integration mode, rather than advertising unported alternatives.
- `do_execveat_common`: existing native/compat exec bootstrap and su hooks.
- `do_faccessat`, `vfs_statx`: existing su path compatibility hooks.
- `vfs_read`: init.rc read proxy, gated by the actual Next init-rc lifecycle flag.
- `vfs_statx_fd`: kernel-side init.rc size adjustment shared by native fstat,
  compat fstat64 and fd-based statx. The proxy is installed before the first read,
  including read_iter; no userspace stat structure offset tricks are needed.
- `input_handle_event`: existing volume-down safe-mode hook.
- `reboot`: modern Manager descriptor installation hook before CAP_SYS_BOOT
  checking. Normal reboot authorization and magic checks remain in place.
- `slow_avc_audit`: Next's manual audit-log hook; this changes logged SID data,
  not the permission decision already made by SELinux.
- Upstream Next LSM hooks handle setuid transitions and Manager package tracking.
- Removed the obsolete v0.9.5 devpts hook; Next has no matching exported handler.

## Explicit compatibility changes

`path_umount`/`can_umount` are explicitly backported to Kyoto's namespace code.
The helper consumes both path references on success and failure. No unrelated
mount API replacement or struct layout change was made.

The upstream Kbuild's build-time `sed` backports and git/network version logic
are removed. Kyoto already has blob-aware `selinux_cred`/`selinux_inode` helpers;
they are preserved. Its native 4.19 `put_seccomp_filter` path is used, without
adding an unused `filter_count` field or modifying task layout. The stale
`disable_seccomp(current)` declaration/calls are corrected to the upstream
implementation's `disable_seccomp(void)` signature.

The Next SELinux context-write hook's 128-byte buffer gets an explicit terminator
and one extra byte, preventing string operations from reading beyond a full
128-byte input. Exec hooks reject NULL/error filenames before dereferencing.

## Manager and userspace

Use the **normal KernelSU-Next v3.4.0 Manager**, not the old official v0.9.5 APK:
https://github.com/KernelSU-Next/KernelSU-Next/releases/download/v3.4.0/KernelSU_Next_v3.4.0_33294-release.apk

The reported kernel version is fixed to the matching release's 33294;
UAPI version 4 and the upstream Next signing-certificate check are retained.
This is a real source replacement, not a version bump to disguise v0.9.5.
The Manager does not appear automatically; install it separately and complete
its userspace installation after a separately validated kernel is running.
Manager authorization, daemon bootstrap, root profiles, modules and safe mode
remain untested on Kyoto.

## Build (standalone vendor-config validation passed)

The existing `scripts/build-kyoto-ksu.sh` is updated for Next manual mode and
uses `out-kyoto-ksunext` by default. It merges the same Motorola/Kyoto fragments
followed by `kernel/configs/ksu.config`; CFI/LTO are preserved. It remains a
standalone vendor-config build helper, not proof of matching a particular
LineageOS ROM's device-tree build settings.

Do not flash or switch A/B slots based on this source port. Establish the
correct ROM toolchain/config, complete build and runtime validation, and
resolve the reported older-firmware-slot brick risk independently first.
