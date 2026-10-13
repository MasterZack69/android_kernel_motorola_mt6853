# SUSFS v2.3.0 — Kyoto / KernelSU-Next legacy source port

## Status and provenance

Branch: `ksu-next-susfs-lineage-22.2`. Copied from `ksu-next-lineage-22.2`
at `837a9e3873282d1f7b25e84778679822bf86dadd`.

**Source port only: NOT BUILT, NOT BOOTED, NOT DEVICE-TESTED.** The parent
Next-only Image.gz/ZIP does not include SUSFS and is not validation of this
branch. No flashing, A/B slot, boot-image or AVB operation has been performed.

KernelSU-Next remains v3.4.0-legacy, commit
`8af3d4fec33be32fa5d3f8dae4f380fac45bf2cd`, version 33294 / UAPI 4.
Upstream nift4 synchronization through `c6f59923f3f93b6e7ec09aebf956999e5932a2a2`
is retained. The original Next branch is not modified by this port.

Canonical SUSFS source and userspace ABI:
https://github.com/simonpunk/susfs4ksu/tree/9892175b4acec7ee844e113b8d02c0f4d12cdfac
(`gki-android12-5.10`, actual `SUSFS_VERSION` v2.3.0). The official source
`fs/susfs.c` and headers are imported, then adapted to Kyoto APIs.
Filesystem backport starting point:
https://github.com/JackA1ltman/NonGKI_Kernel_Build_2nd/blob/b5d7e5be60398f040b9e9919186c410c01a228da/Patches/Patch/susfs_patch_to_4.19.patch
The community inline-hook script is NOT executed; Next interfaces are adapted
explicitly. The official KernelSU integration patch is a design/ABI reference,
not blindly applied over KernelSU-Next. This is a local port, not an upstream
SUSFS/Next-endorsed release.

## Integration choices

- Built-in ARM64/64-bit manual integration; no kprobes/syscall-table hooks.
  SUSFS requires THREAD_INFO_IN_TASK, FUSE_FS and FSNOTIFY. CFI/LTO and the
  existing blob-aware SELinux credential helpers remain intact.
- Mount tagging is placed in Kyoto's backported `vfs_create_mount()` API,
  not the old `vfs_kern_mount()` allocation point. MTK mount-data callbacks
  are preserved. Borrowed IDs are marked before fallible clone-data setup;
  normal clones do not inherit borrowed-ID ownership.
- Dedicated three-argument SUSFS setresuid adapter runs after UID/LSM checks,
  before commit_creds, with the old zygote SID still available. It handles
  equal UID/EUID/SUID specialization; partial UID changes are intentionally
  outside this adapter. Next's two-argument handler remains for SUSFS-off.
- Cached zygote/zygote_next/KSU/init/priv-app SIDs reject unresolved SID 0.
  Normal zygote children use Next's existing policy-checked task-work umount.
  **zygote_next children are never unmounted from the init namespace.**
  Extra SUS_PATH work is queued separately, not a synchronous unmount barrier.
- Real-file SUS_MAP hooks cover maps, smaps, rollup, map_files lookup/revalidate/
  links/enumeration, pagemap and remote-memory access. Pagemap outputs zero
  entries per hidden VMA, preserving unrelated entries and file offsets.
  Counts and emitted map_files entries use matching filters.
- OPEN_REDIRECT selects a replacement before opening/truncating the original
  file. It preserves normal open/LSM checks, uses a recursion barrier, and does
  not redirect O_TMPFILE creation. UID schemes remain explicit.
- SUS_KSTAT uses a distinct FUSE marker, never casting a non-FUSE inode as a
  fuse_inode. Internal statx marker bits are stripped from the result ABI.
- Dedicated reboot-magic command dispatcher preserves the SUSFS v2.3.0
  structures/opcodes, including bool is_statically. It requires UID 0 AND
  the KSU SELinux domain; unauthorized callers return EPERM. Unknown commands
  return EINVAL. Normal reboot and Next Manager descriptor routes remain.
- Next's existing SELinux-policy hiding feature is retained, not replaced
  with the official KernelSU-specific fake-policy implementation. SUSFS AVC
  log-spoof toggling owns the audit-log path in SUSFS mode; it does not change
  SELinux authorization decisions. The normal Next-only audit hook remains
  when SUSFS is disabled.
- Boot-completed starts the SD-card monitor once. The 4.19 fsnotify callback,
  free_mark callback, mark refs and failed-watch cleanup are adapted; cleanup
  remains deferred outside the fsnotify SRCU callback.

## Configuration and userspace

Apply `kernel/configs/susfs.config` after `kernel/configs/ksu.config` and the
Motorola fragments. `scripts/build-kyoto-ksu.sh` on this branch merges it and
uses a separate `out-kyoto-ksunext-susfs` output directory. It has NOT been run
for this port. The fragment enables the exported SUSFS features with kernel
logging off. Features still require userspace configuration; compiling them
in does not magically configure complete hiding. Mount hiding starts off,
matching the canonical v2.3.0 source, until userspace toggles it.

Use the normal signed Next v3.4.0 Manager APK (33294), installed separately:
https://github.com/KernelSU-Next/KernelSU-Next/releases/download/v3.4.0/KernelSU_Next_v3.4.0_33294-release.apk
SUSFS needs a compatible v2.3.0 arm64 `ksu_susfs` userspace tool/module. Source
for that ABI is pinned in the canonical SUSFS repository above. No userspace
binary/module is supplied or validated here; an old 1.5.5 module/tool is NOT
assumed compatible. Run the tool through Next's authorized su/ksud context.
This source work does not auto-install the Manager or a module on the device.

## Limits and next validation

No hardware-specific SUSFS capability is required, but kernel/API, ROM,
Manager, module and policy compatibility still need testing on this device.
SUS_MAP hides registered real-file mappings, not anonymous memory, injected
inline/PLT hooks or every side channel. Nothing here guarantees perfect root
hiding, banking-app compatibility or hardware-attestation success.

Next step: an explicitly authorized clean build, then controlled runtime
checks for Manager communication/root, safe mode, module mount/unmount,
SUSFS version/features/variant and individual commands. Also test SUSFS-off,
feature-off configurations and the SELinux-policy hiding interaction. A
source audit is not proof that these combinations compile or boot.

Do not switch to an older-firmware A/B slot, blindly flash both slots, or
relock the bootloader. No safety claim about the reported firmware brick
condition can be inferred from this source port.

## Licensing

Upstream SUSFS carries GPL-3.0 (retained verbatim in SUSFS-LICENSE); Next and
Linux retain their original notices. This port does not relicense SUSFS as
GPL-2.0. Combined Linux GPL-2.0-only / GPL-3.0 redistribution compatibility
is unresolved here: obtain a compatible licensing basis before distributing
combined binaries. License preservation is not a legal compatibility ruling.
