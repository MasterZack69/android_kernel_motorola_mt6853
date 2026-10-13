# SUSFS v2.3.0 source-only validation

**NOT BUILT. NOT BOOTED. NOT RUNTIME-TESTED.**

Copied Next parent: `837a9e3873282d1f7b25e84778679822bf86dadd`.
Canonical SUSFS: `9892175b4acec7ee844e113b8d02c0f4d12cdfac`.

Completed checks:

- All reference patch rejects resolved; no .rej files retained.
- `git diff --cached --check`: pass after whitespace cleanup.
- `bash -n scripts/build-kyoto-ksu.sh`: pass (syntax only; script not run).
- `python3 scripts/check-kyoto-susfs-source.py`: 723 limited source/provenance
  assertions passed. These verify expected text/ABI markers, manifest hashes,
  ARM64/header bit availability, selected wiring and balanced conditionals.
  They do NOT compile or type-check C, resolve Kconfig, link symbols, test
  feature-off combinations or validate runtime behavior.
- SUSFS call-name/prototype text scan: no unexplained called SUSFS name;
  commented-out upstream susfs_exit is not a runtime reference.
- Manual review/adaptation of mount API and allocation cleanup, setresuid
  placement and non-duplicate LSM handling, root/domain command authorization,
  fsnotify callback and mark destruction, FUSE stat markers, map_files counts,
  pagemap index preservation, redirect ownership/readlink and log cleanup.

No make, kernel compiler, linker, ROM build, boot-image packaging or flashing
was run for this port. Existing KERNELSU-VALIDATION.md describes a historical
Next-only build, NOT this SUSFS branch.

The provenance manifests make the reviewed source reproducible, not proven
correct. A clean build and controlled device tests remain mandatory before
considering this a usable kernel. See SUSFS.md for constraints, userspace ABI,
licensing and the A/B safety warning.
