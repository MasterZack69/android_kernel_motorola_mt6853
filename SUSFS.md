# Kyoto SUSFS port

This experimental branch is a copy of `ksu-linage-22.2` at
`b6d8b2f43eff37453e1e7f84e9ae634c1795cc70`. It retains the upstream Kyoto
FocalTech gesture commit and does not change that driver's timer implementation.

## Versions and requirements

- Linux 4.19.325, Motorola Edge 20 Fusion/Lite (`kyoto`).
- Official KernelSU v0.9.5, kernel version identifier 11872, built-in manual hooks.
- Legacy SUSFS v1.5.5 from simonpunk/susfs4ksu's `kernel-4.19` branch, pinned at
  `001e69919c6271f690fd00b17e4c721c9e599152`.
- **Not** current GKI SUSFS 2.x or KernelSU 3.3.0.

No special hardware feature is required. SUSFS modifies kernel filesystem,
mount and process behavior. Compatibility depends on the kernel port and
userspace tool, not on a specific touchscreen/SoC feature. It does not alter
hardware-backed attestation, relock the bootloader, or guarantee that apps
will accept a rooted device.

## What is integrated

The port includes filesystem and proc hooks, SUSFS control commands through
KernelSU, SELinux domain tracking, and explicit `path_umount`/`get_cred_rcu`
backports. New-mount hooks use this tree's `vfs_create_mount` and preserve its
vendor mount-data callbacks. Task state uses free KABI slots 7/8; the futex
backport's slots 3-6 and Motorola task-turbo fields remain intact. Mount state
uses vfsmount KABI slot 4.

Kprobes remain disabled. KernelSU remains built-in. CFI/LTO remain enabled.
The imported SUSFS Makefile's build-time source rewriting is intentionally
not used. Upstream provenance/license details are in SUSFS-UPSTREAM.md.
Review licensing before redistributing a combined source or binary product.

## Configuration

Apply device fragments, then `kernel/configs/ksu.config`, then
`kernel/configs/susfs.config`. This branch's Kyoto helper does that:

```sh
OUT="$PWD/out-kyoto-susfs" JOBS=2 bash scripts/build-kyoto-ksu.sh
```

The helper performs a full build **when you run it**; no full kernel image
was built for this port during its source integration.

Default enabled features:
- Suspicious mount handling and automatic KSU/bind-mount registration.
- kstat support and legacy OverlayFS handling.
- TRY_UMOUNT and automatic bind-mount unmount registration.
- KSU/SUSFS symbol filtering.

Default disabled / opt-in: path hiding, open redirection, uname/cmdline
spoofing, and SUSFS logging. These features can affect normal filesystem
behavior; enable them deliberately rather than blindly copying a config.
Magic Mount is disabled because this KernelSU version uses OverlayFS.
Deprecated SUS_SU is neither imported nor enabled.

To disable SUSFS, omit its config fragment and ensure `CONFIG_KSU_SUSFS=n`.
The feature defaults to n in Kconfig; the SUSFS fragment explicitly enables it.

## Userspace and testing

Use an ARM64 `ksu_susfs` tool/module compatible with legacy v1.5.5, and the
matching official KernelSU v0.9.5 Manager. A modern SUSFS module is not assumed
compatible merely because it installs. The userspace binaries/module are not
built or installed by this source port. Upstream pinned tool sources:
https://gitlab.com/simonpunk/susfs4ksu/-/tree/001e69919c6271f690fd00b17e4c721c9e599152/ksu_susfs

See SUSFS-VALIDATION.md for source/config/targeted compilation evidence.
Before deployment, perform a complete target build, package for the installed
ROM, and test boot, Manager/root access, safe mode, module mounting/unmounting,
SELinux, filesystem operations, and normal app behavior on the device.
Keep a known-good boot image and recovery path.

The earlier KSU-only downloadable build does **not** contain this port.
No flashable archive or boot-tested binary is provided here.
