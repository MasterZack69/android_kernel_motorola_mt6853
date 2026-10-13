#!/usr/bin/env python3
"""Limited non-build source/provenance checks, NOT a C compiler or boot test."""
from pathlib import Path
import hashlib, json, re
R = Path(__file__).resolve().parents[1]
checks = []
def need(ok, label):
    if not ok:
        raise SystemExit('FAIL: ' + label)
    checks.append(label)
def text(p):
    return (R / p).read_text()
need('SUSFS_VERSION "v2.3.0"' in text('include/linux/susfs.h'), 'version is v2.3.0')
need(re.search(r'bool\s+is_statically;', text('include/linux/susfs.h')), 'v2.3.0 kstat bool ABI')
need('obj-$(CONFIG_KSU_SUSFS) += susfs.o' in text('fs/Makefile'), 'filesystem object wiring')
need('kernel/configs/ksu.config kernel/configs/susfs.config' in text('scripts/build-kyoto-ksu.sh'), 'fragment order')
need('CONFIG_KSU_SUSFS=y' in text('kernel/configs/susfs.config'), 'SUSFS fragment enabled')
need('# CONFIG_KPROBES is not set' in text('kernel/configs/ksu.config'), 'manual no-probe input')
need('current_uid().val != 0 || !susfs_is_current_ksu_domain()' in text('drivers/kernelsu/supercall/supercall.c'), 'SUSFS command authorization')
need('return sid2 != 0 && tsec->sid == sid2;' in text('drivers/kernelsu/selinux/selinux.c'), 'unresolved SID rejection')
need('ksu_susfs_handle_setresuid(new->uid.val, new->euid.val, new->suid.val)' in text('kernel/sys.c'), 'validated setresuid adapter')
need('#ifdef CONFIG_KSU_SUSFS\n\t/* The validated setresuid syscall adapter' in text('drivers/kernelsu/hook/lsm_hooks.c'), 'no duplicate LSM specialization')
need('.handle_event = susfs_handle_sdcard_inode_event' in text('fs/susfs.c') and '.free_mark = susfs_free_sdcard_mark' in text('fs/susfs.c'), 'legacy fsnotify ABI and destructor')
need('mnt = susfs_alloc_non_unshare_ksu_vfsmnt(fc->source' in text('fs/namespace.c'), 'backported mount API hook')
# Collision check against this 64-bit architecture's literal bit assignments.
existing_tif = {int(v) for v in re.findall(r'^#define TIF_\w+\s+(\d+)\b', text('arch/arm64/include/asm/thread_info.h'), re.M)}
need(not ({33,34,35} & existing_tif), 'SUSFS thread bits are free in ARM64 header')
existing_as = {int(v) for v in re.findall(r'\bAS_\w+\s*=\s*(\d+)', text('include/linux/pagemap.h'))}
need(not ({33,34,35,36,39} & existing_as), 'SUSFS mapping bits are free in pagemap enum')
for e in json.loads(text('drivers/kernelsu/SOURCE-MANIFEST.json'))['files']:
    need(hashlib.sha256((R/'drivers/kernelsu'/e['path']).read_bytes()).hexdigest() == e['sha256'], 'Next hash: '+e['path'])
manifest = json.loads(text('SUSFS-SOURCE-MANIFEST.json'))
for e in manifest['files']:
    need(hashlib.sha256((R/e['path']).read_bytes()).hexdigest() == e['sha256'], 'port hash: '+e['path'])
# Structural preprocessor checks; intentionally not called config compilation.
for e in manifest['files']:
    if not e['path'].endswith(('.c','.h')):
        continue
    stack = []
    for n, line in enumerate(text(e['path']).splitlines(), 1):
        directive = re.match(r'^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b', line)
        if not directive:
            continue
        op = directive[1]
        if op in ('if','ifdef','ifndef'):
            stack.append(n)
        elif op in ('else','elif'):
            need(bool(stack), 'conditional arm '+e['path']+':'+str(n))
        else:
            need(bool(stack), 'conditional end '+e['path']+':'+str(n))
            stack.pop()
    need(not stack, 'balanced conditionals: '+e['path'])
need(not list(R.glob('fs/**/*.rej')) and not list(R.glob('mm/*.rej')), 'no patch rejects')
print('PASS:',len(checks),'limited source/provenance assertions')
print('NOT BUILT; C syntax, linking, configuration resolution and runtime are NOT validated.')
