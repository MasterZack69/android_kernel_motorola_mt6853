# Kyoto full-build validation

**PASS: complete standalone vendor-config kernel build.**

Device: Motorola Edge 20 Fusion/Lite, codename `kyoto`.
Kernel release: `4.19.325-cip135-st19+`.
Base branch: lineage-22.2, commit `cbbc069866f7b9957337153ff12c9807b8c46bef`.
Integration source: commit `2572daf3f9dd9e1f2e156063e39144856c85df1a`.
KernelSU: pinned official v0.9.5, source
`b766b98513b5a7eb33bc1c4a76b5702bf1288f07`, version 11872.
Toolchain: host Clang/LLVM/LLD 15.0.7 (Amazon Linux 2023).

## Reproduction

The helper `scripts/build-kyoto-ksu.sh` reproduces the configuration and
build targets. Put the same Clang/LLVM toolchain in PATH, then run:

```sh
OUT=/absolute/path/to/out-kyoto JOBS=2 bash scripts/build-kyoto-ksu.sh
```

Configuration: `k6853v1_64_6360_k419_defconfig`, then, in order:

1. `arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419.config`
2. `arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419-kyoto.config`
3. `kernel/configs/ksu.config`

Ran `olddefconfig`, then:

```sh
make -j2 O=/absolute/path/to/out-kyoto ARCH=arm64 LLVM=1 LLVM_IAS=1 \
  Image.gz dtbs modules
```

Result: **exit status 0**. No vendor-code fixes, global warning suppressions,
or CFI/LTO disablement were required. Existing source/compiler/device-tree
warnings remain; success means the build completed, not a warning-free build.

## Verified outputs

- Full `vmlinux` linked as ELF64 AArch64.
- `Image` and `Image.gz` generated; `gzip -t Image.gz` passed.
- `mt6853.dtb` generated.
- Kyoto EVB, EVT, and DVT1 overlay `.dtbo` files generated.
- All 20 configured loadable modules built.
- `llvm-nm vmlinux` verified KernelSU initialization, exec, access/stat,
  read, input-safe-mode, devpts, and prctl hook definitions and hook flags.
- Final config: `CONFIG_KSU=y`, `CONFIG_OVERLAY_FS=y`,
  `CONFIG_SECURITY_SELINUX=y`, `CONFIG_LTO_CLANG=y`, `CONFIG_CFI_CLANG=y`;
  kprobes and kprobe events disabled.

Config SHA-256: `7c148da7afa980cbafc8819711fa61d2a73b25cf04bd63271a1ed5e64a1c58f2`.
Image.gz SHA-256: `37b1218d0fded71359455baa37cf77b5ae0dc2c0abfcf84f6804e6655d2f4195`.

## Not validated / not supplied

- No physical-device boot, root/Manager, safe-mode, SELinux, or module tests.
- Not a full LineageOS ROM build; no authoritative Kyoto LineageOS device-tree
  build configuration was found. Additional ROM-specific flags/fragments,
  ABI and vendor-module compatibility must be checked before deployment.
- No boot.img, AVB/signing work, or AnyKernel flashable ZIP is supplied.
- Raw DTBO files are not a packaged dtbo.img.
- No `path_umount` backport: the legacy module-unmount limitation remains.
- This is KernelSU v0.9.5, **not v3.3.0**. Upstream v3.3.0 requires kprobes;
  the archived non-GKI guide names v0.9.5 as the last supported release.

The earlier generic ARM64 smoke check also passed, but the full Kyoto
build above supersedes that limited compilation evidence.
