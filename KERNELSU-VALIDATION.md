# Kyoto KernelSU-Next full-build validation

**PASS: complete standalone vendor-config build, exit status 0.**

- Device target: Motorola Edge 20 Fusion/Lite, codename `kyoto`.
- Built source commit: `e419e0f142195cfd6f817f627c4aa0d34cb98aec` on `ksu-next-lineage-22.2`.
- Includes nift4 upstream merge `992a36d4d27f4a60784e4b396dfacaf9f10369da`.
- Kernel release: `4.19.325-cip135-st19+`.
- Next: v3.4.0-legacy at `8af3d4fec33be32fa5d3f8dae4f380fac45bf2cd`.
- Reported Next version: 33294; Manager UAPI 4.
- Toolchain: host Clang/LLVM/LLD 15.0.7, Amazon Linux 2023.
- Build completed/verified at UTC: `2026-10-11T13:01:43.509225+00:00`.
- No source fixes, global warning suppressions or CFI/LTO disablement were
  needed to complete this build. Vendor-source warnings remain; this is not
  a warning-free build or a source-security audit.

## Reproduction

The existing helper ran with `JOBS=2`:

```sh
OUT=/absolute/path/to/out-kyoto-ksunext JOBS=2 bash scripts/build-kyoto-ksu.sh
```

Configuration sequence:

1. `k6853v1_64_6360_k419_defconfig`
2. `arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419.config`
3. `arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419-kyoto.config`
4. `kernel/configs/ksu.config`
5. `olddefconfig`

Build command invoked by the helper:

```sh
make -j2 O=/absolute/path/to/out-kyoto-ksunext ARCH=arm64 LLVM=1 LLVM_IAS=1 \
  Image.gz dtbs modules
```

## Verified outputs

- Full `vmlinux` linked as ELF64 AArch64 with debug information.
- ARM64 `Image` and `Image.gz` generated; `gzip -t Image.gz` passed.
- `mt6853.dtb` and Kyoto EVB, EVT, DVT1 `.dtbo` overlays generated.
- All 20 configured loadable modules built.
- Linked-symbol inspection verified Next initialization, exec, access/stat,
  init.rc read/size hooks, reboot/Manager hook, setuid LSM, input safe mode,
  audit hook and explicit `path_umount` backport in the final kernel.
- Final config keeps `CONFIG_KSU=y`, `CONFIG_KSU_MANUAL_HOOK=y`,
  `CONFIG_LTO_CLANG=y`, `CONFIG_CFI_CLANG=y`, `CONFIG_OVERLAY_FS=y` and SELinux.
- Probes, Next probe hooking and syscall-table patching are disabled.
- Source checkout remained clean through the build; no build-time source
  rewrites were observed.

Config SHA-256: `7845d2613232ad192b3c014e2333544a8bca77f4d2044372a74fa74b8b3d1197`

Image.gz SHA-256: `f3cb9178056337b4b9b0d2d1e3e4090591f860584aa9672f8936442cddfc344d`

## Limits — read before using artifacts

This is the repository's standalone vendor-config build, **not a full LineageOS
ROM build**. The matching ROM device-tree settings, compiler, boot packaging,
vendor-module ABI and firmware state must be checked independently.

- No physical-device boot, Manager recognition, root grant, daemon bootstrap,
  safe-mode, SELinux runtime, module loading or banking-app tests performed.
- No SUSFS included.
- No `boot.img`, `vendor_boot.img`, packaged `dtbo.img`, AVB/signing work,
  AnyKernel installer or flashable ZIP supplied.
- Raw modules are supplied as build outputs, not instructions to replace the
  phone's vendor modules. Compatibility with installed firmware is unverified.
- The inherited gesture commit is unchanged; its previously identified
  softirq timer/mutex concern remains unresolved and is not cleared by building.
- The reported older-firmware A/B-slot hard-brick risk remains unresolved.
  Do not switch slots or flash these artifacts based on build success alone.

The previous source-only report is superseded for compilation status only.
The earlier official KernelSU v0.9.5 build is separate historical validation.
