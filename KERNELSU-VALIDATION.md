# Kyoto KernelSU-Next validation status

**NOT BUILT. Source-level checks only.**

Kernel: Motorola Kyoto 4.19.325, source base
`b6d8b2f43eff37453e1e7f84e9ae634c1795cc70`.
Next: v3.4.0-legacy at `8af3d4fec33be32fa5d3f8dae4f380fac45bf2cd`.

The inherited v0.9.5 build-success report applied to source commit
`2572daf3f9dd9e1f2e156063e39144856c85df1a` on `ksu-linage-22.2`.
It is historical evidence for that implementation only and is not a build
result for this branch.

## Source-only checks

- Compared vendored files and materialized UAPI headers with the pinned tag;
  per-file hashes and local differences are recorded in SOURCE-MANIFEST.json.
- Reviewed manual hook definitions, shared declarations and call sites.
- Checked Kbuild object paths, local includes, Manager version/UAPI pairing,
  no build-time fetches/source rewrites, and configuration mode restrictions.
- Reviewed path_umount ownership against Kyoto's existing namespace helpers.
- Reviewed native 4.19 seccomp and blob-aware SELinux APIs without adding
  guessed struct fields.
- Confirmed no SUSFS code/config was imported and no FocalTech gesture files
  were changed relative to the branch copied for this port.
- Checked helper shell syntax and git diff whitespace. Some unchanged
  upstream Next lines retain trailing-whitespace warnings; these are not
  compiler results. No make, Kconfig
  generation, C/C++ compiler, linker, dtc, or image generation was run.

These checks are not a substitute for compilation or runtime tests. They
cannot prove there are no compiler/link errors or boot-time failures.

## Outstanding validation

- Correct ROM-specific configuration/toolchain; full Image.gz, DTB/DTBO and modules.
- CFI/LTO and vendor-module ABI compatibility.
- Boot, Manager recognition, userspace daemon initialization and root grants.
- Native/compat su, SELinux behavior, unmount profiles and safe mode.
- Device-specific A/B firmware safety. No flashable image is provided.
- Banking-app compatibility; perfect root hiding is not promised.

The inherited gesture commit is retained unchanged. Its previously identified
softirq timer/mutex concern is separate from this port and remains unresolved.
