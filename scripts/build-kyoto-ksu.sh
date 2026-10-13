#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0
set -euo pipefail
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$ROOT"
OUT=$(realpath -m -- "${OUT:-$ROOT/out-kyoto-ksunext-susfs}")
JOBS=${JOBS:-$(nproc)}
mkdir -p "$OUT"
BUILD=(make "O=$OUT" ARCH=arm64 LLVM=1 LLVM_IAS=1)
"${BUILD[@]}" k6853v1_64_6360_k419_defconfig
sh scripts/kconfig/merge_config.sh -m -O "$OUT" "$OUT/.config" \
  arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419.config \
  arch/arm64/configs/ext_config/moto-k6853v1_64_6360_k419-kyoto.config \
  kernel/configs/ksu.config kernel/configs/susfs.config
"${BUILD[@]}" olddefconfig
for cfg in CONFIG_KSU=y CONFIG_KSU_MANUAL_HOOK=y CONFIG_KSU_SUSFS=y CONFIG_OVERLAY_FS=y CONFIG_SECURITY_SELINUX=y \
           CONFIG_LTO_CLANG=y CONFIG_CFI_CLANG=y \
           'CONFIG_ARCH_MOTO_PROJECT="kyoto"'; do
  grep -qxF "$cfg" "$OUT/.config" || { echo "Required config missing: $cfg" >&2; exit 1; }
done
if grep -qE '^CONFIG_(KPROBES|KRETPROBES|KPROBE_EVENTS|KSU_KPROBES_HOOK|KSU_SYSCALL_TABLE_HOOK)=y$|^CONFIG_KSU=m$' "$OUT/.config"; then
  echo 'Invalid configuration: this integration requires built-in manual hooks.' >&2
  exit 1
fi
"${BUILD[@]}" -j"$JOBS" Image.gz dtbs modules
printf '\nBuilt kernel: %s/arch/arm64/boot/Image.gz\n' "$OUT"
printf 'Built device trees: %s/arch/arm64/boot/dts/mediatek/\n' "$OUT"
printf 'Modules: find %s -name "*.ko"\n' "$OUT"
printf 'These are build artifacts, not a packaged or device-tested boot image.\n'
