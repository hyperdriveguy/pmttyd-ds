#!/usr/bin/env python3
"""Find undocumented functions in the Paper Mario ISO."""

import json
import struct
from pathlib import Path


def find_small_functions(iso_path, min_size=1, max_size=256):
    """Find small binary functions in the ISO."""
    iso = Path(iso_path)
    data = iso.read_bytes()
    inventory = json.load(open('artifacts/iso-inventory.json'))
    files = inventory['files']

    # Find .bin files and small m/ files
    results = []

    # Look at .bin files
    bin_files = [f for f in files if f['path'].endswith('.bin') or '/bin/' in f['path']]
    for fe in bin_files:
        offset = fe['offset']
        size = fe['size']
        if min_size <= size <= max_size:
            chunk = data[offset:offset+size]
            results.append({
                'path': fe['path'],
                'offset': offset,
                'size': size,
                'type': 'bin',
                'content': chunk.hex(),
                'strings': extract_strings(chunk)
            })

    # Look at m/*/s files (small scripts)
    m_files = [f for f in files if f['path'].startswith('m/')]
    s_files = [f for f in m_files if '/s/' in f['path']]
    for fe in s_files:
        offset = fe['offset']
        size = fe['size']
        if min_size <= size <= max_size:
            chunk = data[offset:offset+size]
            results.append({
                'path': fe['path'],
                'offset': offset,
                'size': size,
                'type': 'm_s_script',
                'content': chunk.hex(),
                'strings': extract_strings(chunk)
            })

    # Look at very small files anywhere
    small_all = [f for f in files if min_size <= f['size'] <= max_size]
    for fe in small_all:
        # Skip if already covered
        if any(r['path'] == fe['path'] for r in results):
            continue
        offset = fe['offset']
        size = fe['size']
        chunk = data[offset:offset+size]
        results.append({
            'path': fe['path'],
            'offset': offset,
            'size': size,
            'type': 'other',
            'content': chunk.hex(),
            'strings': extract_strings(chunk)
        })

    return results


def extract_strings(chunk):
    """Extract printable strings from binary data."""
    strings = []
    current = b''
    for byte in chunk:
        if 32 <= byte < 127:
            current += bytes([byte])
        else:
            if len(current) >= 2:
                strings.append(current.decode('ascii', errors='replace'))
            current = b''
    if len(current) >= 2:
        strings.append(current.decode('ascii', errors='replace'))
    return strings


if __name__ == '__main__':
    import sys
    iso_path = sys.argv[1] if len(sys.argv) > 1 else '/home/carson/games/roms/G8ME01.iso'
    
    functions = find_small_functions(iso_path, min_size=1, max_size=64)
    
    print(f"=== Found {len(functions)} small functions (1-64 bytes) ===\n")
    
    # Group by type
    by_type = {}
    for f in functions:
        t = f['type']
        if t not in by_type:
            by_type[t] = []
        by_type[t].append(f)
    
    for t, items in by_type.items():
        print(f"\n--- {t} ({len(items)} files) ---")
        for item in items[:10]:  # Show first 10 of each type
            print(f"\n  {item['path']} (offset=0x{item['offset']:08x}, size={item['size']})")
            print(f"  Strings: {item['strings']}")
            print(f"  Hex: {item['content'][:64]}...")