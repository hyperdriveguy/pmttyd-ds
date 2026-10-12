#!/usr/bin/env python3
"""Correct Gekko paired-single instructions in PowerPC objdump text.

Independent field decoder; ordinary instructions remain objdump's responsibility.
Encoding/operand references: GNU opcodes ppc-opc.c (PPCPS) and Dolphin's
GekkoDisassembler.cpp/PPCTables.cpp. See docs/GEKKO_DISASSEMBLY.md.
No original instruction executes here; addresses and displayed bytes are retained.
"""
import argparse
import json
from pathlib import Path
import re
import sys

PAIRED_MAJORS = {4, 56, 57, 60, 61}
INSTRUCTION = re.compile(
    r'^(?P<prefix>[ \t]*[0-9a-fA-F]+:[ \t]+'
    r'(?P<bytes>(?:[0-9a-fA-F]{2}[ \t]+){3}[0-9a-fA-F]{2}|[0-9a-fA-F]{8})'
    r'[ \t]+)(?P<body>[^\r\n]*)(?P<newline>\r?\n)?$')


def decode_paired(word):
    """Return Gekko assembly, .long for unrecognized/reserved, or None outside scope."""
    major = word >> 26
    if major not in PAIRED_MAJORS:
        return None
    d, a, b, c = (word >> shift & 31 for shift in (21, 16, 11, 6))
    rc = '.' if word & 1 else ''
    unknown = f'.long 0x{word:08x}'
    if major != 4:
        name = {56: 'psq_l', 57: 'psq_lu', 60: 'psq_st', 61: 'psq_stu'}[major]
        # DQ displacement is signed 12-bit, not ordinary D-form's 16-bit.
        displacement = (word & 0xfff) - (0x1000 if word & 0x800 else 0)
        w, i = word >> 15 & 1, word >> 12 & 7
        if major in (57, 61) and a == 0:
            return unknown
        return f'{name} f{d}, {displacement}(r{a}), {w}, {i}'
    xo = word >> 1 & 1023
    short = xo & 31
    # Indexed quantized forms use a 6-bit XO: remaining bits encode W and I.
    if short in (6, 7):
        update = bool(word & 0x40)
        if word & 1 or (update and a == 0):
            return unknown
        name = ('psq_l' if short == 6 else 'psq_st') + ('u' if update else '') + 'x'
        return f'{name} f{d}, r{a}, r{b}, {word >> 10 & 1}, {word >> 7 & 7}'
    unary = {40: 'ps_neg', 72: 'ps_mr', 136: 'ps_nabs', 264: 'ps_abs'}
    if xo in unary:
        return unknown if a else f'{unary[xo]}{rc} f{d}, f{b}'
    compares = {0: 'ps_cmpu0', 32: 'ps_cmpo0', 64: 'ps_cmpu1', 96: 'ps_cmpo1'}
    if xo in compares:
        if word & 0x00600001:
            return unknown
        return f'{compares[xo]} cr{word >> 23 & 7}, f{a}, f{b}'
    merges = {528: 'ps_merge00', 560: 'ps_merge01', 592: 'ps_merge10', 624: 'ps_merge11'}
    if xo in merges:
        return f'{merges[xo]}{rc} f{d}, f{a}, f{b}'
    if xo == 1014:
        return unknown if d or word & 1 else f'dcbz_l r{a}, r{b}'
    # Standard assembler order for A-form four operands is D,A,C,B.
    four = {10: 'ps_sum0', 11: 'ps_sum1', 14: 'ps_madds0', 15: 'ps_madds1',
            23: 'ps_sel', 28: 'ps_msub', 29: 'ps_madd', 30: 'ps_nmsub', 31: 'ps_nmadd'}
    if short in four:
        return f'{four[short]}{rc} f{d}, f{a}, f{c}, f{b}'
    binary = {18: 'ps_div', 20: 'ps_sub', 21: 'ps_add'}
    if short in binary:
        return unknown if c else f'{binary[short]}{rc} f{d}, f{a}, f{b}'
    multiply = {12: 'ps_muls0', 13: 'ps_muls1', 25: 'ps_mul'}
    if short in multiply:
        return unknown if b else f'{multiply[short]}{rc} f{d}, f{a}, f{c}'
    reciprocal = {24: 'ps_res', 26: 'ps_rsqrte'}
    if short in reciprocal:
        return unknown if a or c else f'{reciprocal[short]}{rc} f{d}, f{b}'
    return unknown


def correct_disassembly(text):
    """Return corrected text and counts. Preserve human #/; comments and line endings."""
    counts = dict(instructions=0, paired=0, changed=0, unrecognized=0)
    lines = []
    for line in text.splitlines(keepends=True):
        match = INSTRUCTION.fullmatch(line)
        if not match:
            lines.append(line)
            continue
        counts['instructions'] += 1
        word = int(re.sub(r'\s', '', match['bytes']), 16)
        decoded = decode_paired(word)
        if decoded is None:
            lines.append(line)
            continue
        counts['paired'] += 1
        counts['unrecognized'] += decoded.startswith('.long')
        comment = re.search(r'[ \t]+[#;].*$', match['body'])
        if comment:
            decoded += comment[0]
        corrected = match['prefix'] + decoded + (match['newline'] or '')
        counts['changed'] += corrected != line
        lines.append(corrected)
    return ''.join(lines), counts


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('paths', nargs='+', type=Path, help='Listing files or directories to scan recursively')
    mode = p.add_mutually_exclusive_group()
    mode.add_argument('--in-place', action='store_true', help='Repair listings in place')
    mode.add_argument('--check', action='store_true', help='Exit 1 if any correction is needed; never write')
    a = p.parse_args()
    paths = set()
    for path in a.paths:
        if path.is_dir():
            # Only PowerPC listings; do not rewrite unrelated source/reference text.
            for candidate in path.rglob('*'):
                if candidate.is_file() and candidate.suffix in ('.disasm', '.annotated', '.txt'):
                    data = candidate.read_bytes()
                    if b'file format elf32-powerpc' in data:
                        paths.add(candidate.resolve())
        elif path.is_file():
            paths.add(path.resolve())
        else:
            p.error(f'No such listing: {path}')
    if not a.in_place and not a.check and len(paths) != 1:
        p.error('Use --in-place or --check for multiple listings')
    totals = dict(files=0, changed_files=0, instructions=0, paired=0, changed=0, unrecognized=0)
    for path in sorted(paths):
        if path.suffix == '.elf' or b'\0' in path.read_bytes():
            p.error(f'Refusing binary input: {path}')
        original = path.read_bytes().decode('utf-8')
        corrected, counts = correct_disassembly(original)
        totals['files'] += 1
        totals['changed_files'] += bool(counts['changed'])
        for key, value in counts.items():
            totals[key] += value
        if a.in_place and counts['changed']:
            path.write_bytes(corrected.encode('utf-8'))
        elif not a.in_place and not a.check:
            sys.stdout.write(corrected)
    print(json.dumps(totals, sort_keys=True), file=sys.stderr)
    if a.check and totals['changed']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
