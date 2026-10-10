# Validation status

Passed: targeted ARM64 object compilation with host Clang/LLVM 15.0.7,
CONFIG_KSU=y, CONFIG_SECURITY_SELINUX=y, CONFIG_OVERLAY_FS=y, and
CONFIG_KPROBES disabled. Base was arch/arm64/configs/defconfig, with
SELinux/network security and KernelSU enabled for this smoke check.

Commands (after configuring those prerequisites):

```sh
make -j2 O=out-hookcheck ARCH=arm64 LLVM=1 LLVM_IAS=1 security/selinux/
make -j2 O=out-hookcheck ARCH=arm64 LLVM=1 LLVM_IAS=1 \
  drivers/kernelsu/ fs/exec.o fs/open.o fs/read_write.o fs/stat.o \
  fs/devpts/inode.o drivers/input/input.o
```

All listed objects and drivers/kernelsu/built-in.a compiled successfully.
llvm-nm verified references from the exec/input objects to the manual
hooks and the matching definitions in the KernelSU archive.
git diff --check passed.

Test config SHA-256: `ca575d9b53ab1e5ab29ee9174699ab7baa7619fbb56b8c7252d7fc84f34f11e0`

NOT validated: full vmlinux linkage, device-specific Image/DTB/DTBO,
Motorola vendor modules, target CFI/LTO build, flashing or booting,
Manager operation, modules, and safe-mode behavior. Device model/codename,
ROM defconfig/fragments and toolchain are still required for a full build.
A targeted object check is NOT a successfully compiled device kernel.
