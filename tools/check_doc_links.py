#!/usr/bin/env python3
"""Check relative Markdown links across the documentation tree.

Scans every ``.md`` file under ``docs/`` plus the repository root ``README.md``
and ``AGENTS.md`` for relative links (``[text](target)``) and reports any whose
target file or directory does not exist. Absolute URLs, in-page anchors and
mailto links are ignored.

Use this after moving or renaming documentation to confirm the link graph still
resolves:

    python3 tools/check_doc_links.py            # report broken links
    python3 tools/check_doc_links.py --quiet    # exit status only

Exit status is non-zero when a broken link is found.
"""
import argparse
import os
import re
import sys
from urllib.parse import unquote

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LINK = re.compile(r'!?\[[^\]]*\]\(([^)]+)\)')
SKIP_PARTS = {'build', 'build-host', 'artifacts', 'local-assets',
              'third_party', 'romfs', '.git'}


def doc_files():
    for base, dirs, files in os.walk(REPO):
        rel = os.path.relpath(base, REPO)
        if rel != '.' and set(rel.split(os.sep)) & SKIP_PARTS:
            continue
        for fn in files:
            if not fn.endswith('.md'):
                continue
            path = os.path.join(base, fn)
            if rel == '.' and fn not in ('README.md', 'AGENTS.md'):
                continue
            if rel != '.' and not rel.startswith('docs'):
                continue
            yield path


def check(path):
    text = open(path, encoding='utf-8', errors='replace').read()
    base = os.path.dirname(path)
    problems = []
    for m in LINK.finditer(text):
        raw = m.group(1).strip()
        if not raw or raw.startswith(('http://', 'https://', 'mailto:', '#', '<')):
            continue
        target = raw.split('#', 1)[0].split('?', 1)[0]
        if not target:
            continue
        target = unquote(target)
        dest = os.path.normpath(os.path.join(base, target))
        if not os.path.exists(dest):
            line = text.count('\n', 0, m.start()) + 1
            problems.append((line, raw))
    return problems


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--quiet', action='store_true', help='only set exit status')
    args = ap.parse_args()

    total = 0
    broken = 0
    for path in sorted(doc_files()):
        problems = check(path)
        if problems:
            broken += len(problems)
            if not args.quiet:
                rel = os.path.relpath(path, REPO)
                for line, raw in problems:
                    print(f'{rel}:{line}: broken link -> {raw}')
        total += 1
    if not args.quiet:
        print(f'checked {total} markdown file(s); {broken} broken link(s)')
    return 1 if broken else 0


if __name__ == '__main__':
    sys.exit(main())
