# SUSFS source-port validation

**PASS: targeted ARM64 object compilation. NOT a complete kernel build.**

Base: `ksu-linage-22.2` at `b6d8b2f43eff37453e1e7f84e9ae634c1795cc70`.
Target: Kyoto vendor defconfig plus platform/device fragments, `ksu.config`,
then `susfs.config`. KernelSU v0.9.5 / 11872; legacy SUSFS v1.5.5 from
`001e69919c6271f690fd00b17e4c721c9e599152`.
Compiler: Clang/LLVM/LLD 15.0.7 (Amazon Linux 2023).

## Checks performed

- Kyoto configuration generated successfully with `CONFIG_KSU=y`,
  `CONFIG_KSU_SUSFS=y`, OverlayFS and SELinux enabled.
- Kprobes/kprobe events disabled; CFI/LTO enabled throughout compilation.
- SELinux prerequisites compiled, then the complete KernelSU archive,
  `fs/susfs.o`, and every filesystem/proc/kernel C file modified by the port.
- Existing exec/access/read/devpts/input manual-hook callsite objects compiled.
- Targeted compilation passed in four modes:
  1. Default conservative Kyoto SUSFS fragment.
  2. All implemented legacy optional features enabled (Magic Mount excluded).
  3. SUSFS disabled, KernelSU still enabled.
  4. TRY_UMOUNT enabled with SUS_MOUNT disabled.
- Default configuration restored and affected objects recompiled successfully
  after the final compatibility-guard changes.
- LLVM symbol inspection accounted for all 85 referenced `ksu_`/`susfs_`
  symbols across the checked objects/archive. This is cross-object name
  coverage, **not** full vmlinux linkage or runtime CFI verification.
- Original manual hook callsites preserved.
- Motorola's futex KABI reserve block (slots 3-6) verified unchanged.
  SUSFS task fields reuse slots 7/8; mount state reuses vfsmount slot 4.
- `git diff --check` and `bash -n scripts/build-kyoto-ksu.sh` passed.
- No patch rejects/backups or Makefile source-rewriting commands remain.

Default test-config SHA-256: `c2b8e3d58c81597c4854765c5639f59750d4f6fa0f9ad4623986bd0321e8d981`.

Targeted checks used:

```sh
make -j2 O=out-susfs-check ARCH=arm64 LLVM=1 LLVM_IAS=1 security/selinux/
make -j2 O=out-susfs-check ARCH=arm64 LLVM=1 LLVM_IAS=1 \
  drivers/kernelsu/ fs/susfs.o fs/dcache.o fs/namei.o fs/namespace.o \
  fs/notify/fdinfo.o fs/overlayfs/inode.o fs/overlayfs/readdir.o \
  fs/overlayfs/super.o fs/proc/cmdline.o fs/proc/fd.o fs/proc/task_mmu.o \
  fs/proc_namespace.o fs/readdir.o fs/stat.o fs/statfs.o \
  kernel/kallsyms.o kernel/sys.o fs/exec.o fs/open.o fs/read_write.o \
  fs/devpts/inode.o drivers/input/input.o
```

Existing vendor Kconfig warnings remain; the port is not claimed warning-free.

## Not validated

No full vmlinux link, Image/Image.gz, DTB/DTBO, or vendor-module rebuild.
No device boot, userspace tool/module build or installation, root access,
SUSFS hiding/unmount behavior, filesystem/performance regression testing,
or runtime CFI/SELinux testing. No hardware attestation changes are claimed.

The previously published KSU-only build/ZIP does not contain SUSFS or prove
this branch boots. Full target build and physical-device testing are required
before treating this source port as ready for deployment.
