#!/usr/bin/env python3
"""
Document binary code under artifacts/rom/*.iso using Python and CLI tools.
Records findings under docs/.

Analyzes the GameCube ISO (G8ME01.iso) and produces a structured documentation
of the binary code, file types, and organization found within the ISO.
"""

import json
import struct
from pathlib import Path


def inventory(iso_path):
    """Read GameCube ISO FST inventory."""
    path = Path(iso_path)
    size = path.stat().st_size
    with path.open('rb') as f:
        header = f.read(0x440)
        if len(header) != 0x440:
            raise ValueError('Not a GameCube disc image')
        magic = struct.unpack_from('>I', header, 0x1c)[0]
        if magic != 0xc2339f3d:
            raise ValueError(f'Not a GameCube disc image (magic=0x{magic:08x})')
        # offset and length are both 32-bit unsigned ints at 0x424 and 0x428
        offset_raw = struct.unpack_from('>I', header, 0x424)[0]
        length_raw = struct.unpack_from('>I', header, 0x428)[0]
        if length_raw < 12 or length_raw > 32 * 1024 * 1024 or offset_raw + length_raw > size:
            raise ValueError('Invalid filesystem table bounds')
        offset = offset_raw
        length = length_raw
        f.seek(offset)
        fst = f.read(length)
    count = struct.unpack_from('>I', fst, 8)[0]
    if not 1 <= count <= length // 12 or fst[0] != 1:
        raise ValueError('Invalid FST root')
    strings = fst[count * 12:]
    files = []
    stack = [(count, '')]
    for index in range(1, count):
        while stack and index >= stack[-1][0]:
            stack.pop()
        if not stack:
            raise ValueError('Invalid directory range')
        word, start, end = struct.unpack_from('>III', fst, index * 12)
        kind, nameoff = word >> 24, word & 0xffffff
        if kind not in (0, 1) or nameoff >= len(strings):
            raise ValueError('Invalid FST entry')
        terminator = strings.find(b'\0', nameoff)
        if terminator < 0:
            raise ValueError('Unterminated FST name')
        name = strings[nameoff:terminator].decode('ascii', errors='strict')
        if not name or '/' in name or '\\' in name or name in ('.', '..'):
            raise ValueError('Unsafe FST name')
        rel = str(Path(stack[-1][1]) / name) if stack[-1][1] else name
        if kind:
            if not index < end <= stack[-1][0]:
                raise ValueError('Invalid FST directory')
            stack.append((end, rel))
        else:
            if start + end > size:
                raise ValueError('File outside ISO')
            files.append({'path': rel, 'offset': start, 'size': end})
    return {'disc_id': header[:6].decode('ascii'),
            'disc_size': size,
            'title': header[0x20:0x400].split(b'\0')[0].decode('ascii'),
            'fst_offset': offset,
            'files': files}


def doc_binary_iso(iso_path, output_path):
    """Analyze ISO and write binary code documentation."""
    iso = Path(iso_path)
    out = Path(output_path)
    out.parent.mkdir(parents=True, exist_ok=True)

    info = inventory(iso)

    with open(out, 'w') as f:
        f.write("# Binary Code Documentation for ISO\n\n")
        f.write(f"* Disc ID: {info['disc_id']}\n")
        f.write(f"* Title: {info['title']}\n")
        f.write(f"* ISO size: {info['disc_size']} bytes\n")
        f.write(f"* FST offset: 0x{info['fst_offset']:08x}\n\n")

        files = info['files']
        total_files = len(files)
        total_size = sum(f['size'] for f in files)

        # Categorize files
        categories = {}
        for fe in files:
            path_parts = fe['path'].split('/')
            prefix = path_parts[0] if path_parts else ''
            if prefix not in categories:
                categories[prefix] = {'count': 0, 'size': 0, 'examples': []}
            categories[prefix]['count'] += 1
            categories[prefix]['size'] += fe['size']
            if len(categories[prefix]['examples']) < 5:
                categories[prefix]['examples'].append(
                    f"{fe['path']} offset=0x{fe['offset']:08x} size={fe['size']}")

        f.write("## File Type Summary\n\n")
        for cat_name in sorted(categories.keys()):
            cat = categories[cat_name]
            f.write(f"### {cat_name}: {cat['count']} files, {cat['size']} bytes\n")
        f.write(f"*Total file sizes: {total_size} bytes*\n")
        f.write(f"*Unallocated space: {info['disc_size'] - total_size} bytes*\n\n")

        # Detailed analysis by type
        f.write("## REL/Aggregate Files\n")
        rel_files = [f for f in files if f['path'].startswith('rel/')]
        f.write(f"Count: {len(rel_files)}\n")
        f.write(f"Total size: {sum(f['size'] for f in rel_files)} bytes\n")
        if rel_files:
            largest = max(rel_files, key=lambda x: x['size'])
            f.write(f"Largest: {largest['path']} (0x{largest['offset']:08x}, {largest['size']} bytes)\n")
        f.write("\n")

        f.write("## Template (.tpl) Files\n")
        tpl_files = [f for f in files if f['path'].endswith('.tpl') or '/tpl/' in f['path']]
        f.write(f"Count: {len(tpl_files)}\n")
        f.write(f"Total size: {sum(f['size'] for f in tpl_files)} bytes\n")
        if tpl_files:
            largest = max(tpl_files, key=lambda x: x['size'])
            f.write(f"Largest: {largest['path']} (0x{largest['offset']:08x}, {largest['size']} bytes)\n")
        f.write("\n")

        f.write("## Binary (.bin) Files\n")
        bin_files = [f for f in files if f['path'].endswith('.bin') or '/bin/' in f['path']]
        f.write(f"Count: {len(bin_files)}\n")
        f.write(f"Total size: {sum(f['size'] for f in bin_files)} bytes\n")
        if bin_files:
            largest = max(bin_files, key=lambda x: x['size'])
            f.write(f"Largest: {largest['path']} (0x{largest['offset']:08x}, {largest['size']} bytes)\n")
        f.write("\n")

        f.write("## Message (msg/US) Files\n")
        msg_files = [f for f in files if f['path'].startswith('msg/US/')]
        f.write(f"Count: {len(msg_files)}\n")
        msg_sizes = [f['size'] for f in msg_files]
        f.write(f"Size range: {min(msg_sizes)} - {max(msg_sizes)} bytes\n")
        f.write(f"Total message bytes: {sum(msg_sizes)}\n")
        if msg_files:
            f.write("\nFirst 20 message files:\n")
            for fe in sorted(msg_files, key=lambda x: x['path'])[:20]:
                f.write(f"  - {fe['path']} (offset=0x{fe['offset']:08x}, size={fe['size']})\n")
        f.write("\n")

        f.write("## Overview of File Prefixes\n")
        prefix_counts = {}
        for fe in files:
            path_parts = fe['path'].split('/')
            prefix = path_parts[0] if path_parts else ''
            prefix_counts[prefix] = prefix_counts.get(prefix, 0) + 1
        for prefix in sorted(prefix_counts.keys()):
            f.write(f"- **{prefix}**: {prefix_counts[prefix]} files\n")
        f.write("\n")

        # Key binary regions
        f.write("## Key Binary Regions (by offset)\n")
        sorted_files = sorted(files, key=lambda x: x['offset'])
        # Show first and last files, and some notable ones
        f.write(f"\n- First file: {sorted_files[0]['path']} at offset 0x{sorted_files[0]['offset']:08x}\n")
        f.write(f"- Last file: {sorted_files[-1]['path']} at offset 0x{sorted_files[-1]['offset']:08x}\n")

        # Files near FST offset
        fst_files = [f for f in files if f['offset'] >= info['fst_offset']]
        if fst_files:
            f.write(f"- First file at/after FST offset (0x{info['fst_offset']:08x}): {fst_files[0]['path']}\n")

        # Messages
        f.write("\n## Message ID Ranges\n")
        # Message files are sorted, so we can get ID ranges
        msg_sorted = sorted(msg_files, key=lambda x: x['offset'])
        if msg_sorted:
            f.write(f"- First message: {msg_sorted[0]['path']} at offset 0x{msg_sorted[0]['offset']:08x}\n")
            f.write(f"- Last message: {msg_sorted[-1]['path']} at offset 0x{msg_sorted[-1]['offset']:08x}\n")
            # Extract potential ID numbers from filenames
            import re
            id_nums = []
            for fe in msg_sorted[:50]:
                m = re.search(r'(\d+)', fe['path'].split('/')[-1])
                if m:
                    id_nums.append((int(m.group(1)), fe['path']))
            if id_nums:
                f.write(f"- Lowest message ID: {id_nums[0][0]} ({id_nums[0][1]})\n")
                f.write(f"- Highest message ID (first 50): {id_nums[-1][0]} ({id_nums[-1][1]})\n")

    print(f"Documentation written to {out}")


if __name__ == '__main__':
    iso_path = '/home/carson/games/roms/G8ME01.iso'
    output_path = 'docs/BINARY_DOCUMENTATION.md'
    doc_binary_iso(iso_path, output_path)