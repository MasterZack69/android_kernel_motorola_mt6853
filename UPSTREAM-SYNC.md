# Upstream sync — nift4 lineage-22.2

Merged upstream into `ksu-next-lineage-22.2` with merge commit
`992a36d4d27f4a60784e4b396dfacaf9f10369da` (PR #1).

- Previous Next branch head: `34464034eccc11ebd404efc89b7b18b976e2df62`.
- Upstream head: `c6f59923f3f93b6e7ec09aebf956999e5932a2a2`.
- Shared ancestor: `cbbc069866f7b9957337153ff12c9807b8c46bef`.
- User-provided checkpoint: `4a20476d1ed0e32fdcbf51c7ec7bda39fa610502`.
  The Next branch already included the following `cbbc0698` USB-toggle revert.

## Included changes

Git patch-equivalence checking found **33 missing patches**: Kyoto UVC config,
three KSM/do_wp_page changes, and 29 MM/MGLRU reverts. The normal merge retains
all original upstream commit IDs and authors.

The gesture commit `c6f59923` was already applied as `b6d8b2f4`. Its original
upstream commit is now also an ancestor, but the merge does not duplicate its
source changes. No FocalTech source files changed during this sync.

## Source-only verification

- Local three-way merge completed without conflicts; 54 source files changed.
- Published merge tree compared exactly with the reviewed local merged tree.
- Upstream head is an ancestor of the published branch.
- Next source/config, shared hook header, reboot/Manager hook, init.rc read/stat
  hooks, path_umount backport and SELinux audit hook are unchanged by this merge.
- Inspected overlap in fs/exec.c and include/linux/sched.h: only upstream MGLRU
  changes were applied; existing Next exec hooks remain present.
- Whitespace check of the upstream merge passed.

**No build, Kconfig generation, compilation, linking, device testing or flashing
was performed.** This sync does not validate the Next port or the inherited
gesture timer behavior. No SUSFS was added. Other branches were not modified.

## Applied upstream patch list (oldest first)

- `79e1d740f53fd428fe4ddca5806654e5646c2fa0` — Revert "UPSTREAM: mm: replace list_move_tail() with add_page_to_lru_list_tail()"
- `6370b33d1866b6e8c4051f4bd2d72ffb9dd10f87` — Revert "UPSTREAM: arm64: cpufeature: introduce helper cpu_has_hw_af()"
- `1f43948eaafc6dba19276c9845b6a58e56c1fa32` — Revert "UPSTREAM: mm/vmscan.c: use update_lru_size() in update_lru_sizes()"
- `ee1065400648d50b71bae166a4fed019e2481199` — Revert "BACKPORT: mm/swapcache: support to handle the shadow entries"
- `c1dd2ec76e89fc704496917386deb63779808e6b` — Revert "BACKPORT: mm: remove superfluous __ClearPageActive()"
- `7c39dd4a5c4cea383f602d6753e1213e922a74c2` — Revert "BACKPORT: mm: use self-explanatory macros rather than "2""
- `39c4e7b392887de620d3414cdf7fe6320f166f45` — Revert "BACKPORT: mm/vmscan.c: use add_page_to_lru_list()"
- `2394b516373fdf11ffa9c00d05f28132cd41ccc5` — Revert "BACKPORT: include/linux/mm_inline.h: shuffle lru list addition and deletion functions"
- `0e3f9faca4ddadcc15bcb7bf435e558674c1430c` — Revert "BACKPORT: mm: don't pass "enum lru_list" to lru list addition functions"
- `2655674844b2db46a34279e72b9e6f25a745b976` — Revert "UPSTREAM: mm/swap.c: don't pass "enum lru_list" to trace_mm_lru_insertion()"
- `4c72139e113e876c232ca1016ad889f2abf508b5` — Revert "BACKPORT: mm/swap.c: don't pass "enum lru_list" to del_page_from_lru_list()"
- `b5bd9c40705ef97cfa285319b020cdc68841784e` — Revert "BACKPORT: mm: add __clear_page_lru_flags() to replace page_off_lru()"
- `34515c1ae632f80d58fb631513e188a41dbb4221` — Revert "UPSTREAM: mm: VM_BUG_ON lru page flags"
- `c0c9d62efa5420bbf6ce463425f8520d725eb18c` — Revert "BACKPORT: include/linux/mm_inline.h: fold page_lru_base_type() into its sole caller"
- `f54ec37a787bbaf256a7f19e87162aebd0980b02` — Revert "FROMLIST: mm: x86, arm64: add arch_has_hw_pte_young()"
- `41c520c54dd54a56de5452489d748ef931665483` — Revert "FROMLIST: mm: x86: add CONFIG_ARCH_HAS_NONLEAF_PMD_YOUNG"
- `6adc912ab7e9261b08c052b903384cee6157c0af` — Revert "FROMLIST: mm: multi-gen LRU: groundwork"
- `cc9ef2b27b2e94086b18fe5d6d1f20f771602e9f` — Revert "FROMLIST: mm: multi-gen LRU: minimal implementation"
- `85b0de2f968856ed15696f8ae52aa313096b6ec9` — Revert "FROMLIST: mm: multi-gen LRU: exploit locality in rmap"
- `047c3e57c17e7ca7282c18ab847751bc28fa5ba3` — Revert "FROMLIST: mm: multi-gen LRU: support page table walks"
- `23c267a088f0efced7dd5909f31b02d30da17cfe` — Revert "FROMLIST: mm: multi-gen LRU: optimize multiple memcgs"
- `9e2ff60302f226b3e01fad3f06ec5f006d493d82` — Revert "FROMLIST: mm: multi-gen LRU: kill switch"
- `66184777cb085381a4007af4beacbb0a3e293859` — Revert "FROMLIST: mm: multi-gen LRU: thrashing prevention"
- `fb66a5d6a5bb37936d8f22fcc0da8d63cc24b647` — Revert "FROMLIST: mm: multi-gen LRU: debugfs interface"
- `742ddcd26a08ff920a7853dcded5c353cae6f962` — Revert "FROMLIST: mm: multi-gen LRU: admin guide"
- `e4295a2bfddf381c39cf3ef638beff4fa91b9688` — Revert "FROMLIST: mm: multi-gen LRU: design doc"
- `dbe8e89ab620daeaa643142e6dbd46e51f3e2c42` — Revert "mm: mglru fix on qcom baseline"
- `fbf6cb006b04fbc9368a69bf0cece419ea4df9f4` — Revert "Enable MGLRU on London"
- `253d0f2397983c91850eaeffdecdfcb50486bbde` — Revert "mm:vmscan:Update the page scan prioritiy"
- `1c0d6b6670aad89948c122502de8695ba412e533` — UPSTREAM: mm: fix misplaced unlock_page in do_wp_page()
- `4bb33022eff203af204f32ea70d86cda2c8acaab` — BACKPORT: mm: do_wp_page() simplification
- `c6cf1acea13f4ede73f101a577c9f07c12f0c7fd` — UPSTREAM: mm/ksm: Remove reuse_ksm_page()
- `de0b2f41d0d17c34cffb0dd5970f1f07d3846220` — configs: kyoto: enable UVC gadget function
