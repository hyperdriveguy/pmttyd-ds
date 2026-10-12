#!/usr/bin/env python3
"""Read selected Switch relocatable data ELFs without executing game code.

Requires pyelftools and zstandard. Outputs ordered rows (including null IDs),
pointer relocation provenance and numeric words. Field meanings are not inferred.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import struct

from elftools.elf.elffile import ELFFile
import zstandard

TABLES = (
    ('data/battle/data_Parameter.elf.zst', '_ZN3wld3btl4data6s_DataE', 0x18, (0, 0x10)),
    ('data/param/data_param_player.elf.zst', '_ZN3wld3fld6player12s_playerDataE', 0x60, (0, 8, 16)),
    ('data/param/data_param_jump.elf.zst', '_ZN3wld3chr10s_jumpDataE', 0x48, (0,)),
    ('data/battle/weapon/data_battle_weaponac_mario_ac.elf.zst', '_ZN3wld3btl4data6s_dataE', 0x108, (0,)),
    ('data/battle/weapon/data_battle_weaponac_party_ac.elf.zst', '_ZN3wld3btl4data6s_dataE', 0x108, (0,)),
)


def read_table(path, symbol_name, stride, pointers):
    packed = path.read_bytes()
    raw = zstandard.ZstdDecompressor().decompress(packed, max_output_size=64 * 1024 * 1024)
    elf = ELFFile(io.BytesIO(raw))
    if elf.elfclass != 64 or not elf.little_endian or elf['e_type'] != 'ET_REL' or elf['e_machine'] != 'EM_AARCH64':
        raise ValueError('Expected little-endian AArch64 ELF64 relocatable object')
    symbols = elf.get_section_by_name('.symtab')
    candidates = [s for s in symbols.iter_symbols() if s['st_info']['type'] == 'STT_OBJECT'
                  and (s.name == symbol_name if symbol_name else s.name.endswith('6s_DataE'))]
    if len(candidates) != 1:
        raise ValueError(f'Expected unique table symbol in {path}: {[s.name for s in candidates]}')
    symbol = candidates[0]
    section_index = symbol['st_shndx']
    section = elf.get_section(section_index)
    data = section.data()
    base, size = symbol['st_value'], symbol['st_size']
    if size % stride or base + size > len(data):
        raise ValueError('Invalid table size/stride')
    relocations = {}
    for reloc_section in elf.iter_sections():
        if reloc_section['sh_type'] != 'SHT_RELA' or reloc_section['sh_info'] != section_index:
            continue
        linked_symbols = elf.get_section(reloc_section['sh_link'])
        for reloc in reloc_section.iter_relocations():
            offset = reloc['r_offset']
            if not base <= offset < base + size:
                continue
            if offset in relocations or reloc['r_info_type'] != 257:
                raise ValueError('Duplicate/unsupported relocation; expected R_AARCH64_ABS64')
            target = linked_symbols.get_symbol(reloc['r_info_sym'])
            if not isinstance(target['st_shndx'], int) or target['st_shndx'] == 0:
                raise ValueError('Unresolved pointer target')
            target_section = elf.get_section(target['st_shndx'])
            target_offset = target['st_value'] + reloc['r_addend']
            contents = target_section.data()
            end = contents.find(b'\0', target_offset)
            if target_offset < 0 or end < target_offset:
                raise ValueError('Invalid string target')
            relocations[offset] = dict(value=contents[target_offset:end].decode('utf-8'),
                                       section=target_section.name, offset=target_offset,
                                       elf_file_offset=target_section['sh_offset'] + target_offset)
    rows = []
    for index in range(size // stride):
        start = base + index * stride
        strings, words = {}, {}
        for field in pointers:
            offset = start + field
            if offset in relocations:
                strings[hex(field)] = relocations.pop(offset)
            elif struct.unpack_from('<Q', data, offset)[0] == 0:
                strings[hex(field)] = None
            else:
                raise ValueError('Nonzero pointer without relocation')
        for field in range(0, stride, 4):
            if any(p <= field < p + 8 for p in pointers):
                continue
            word = data[start+field:start+field+4]
            u, i, f = (struct.unpack('<' + fmt, word)[0] for fmt in ('I', 'i', 'f'))
            words[hex(field)] = dict(u32=u, i32=i, f32=f, bits=f'{u:08x}')
        rows.append(dict(index=index, section_offset=start,
                         elf_file_offset=section['sh_offset'] + start,
                         strings=strings, words=words))
    if relocations:
        raise ValueError('Layout missed pointer relocations')
    return dict(packed_sha256=hashlib.sha256(packed).hexdigest(),
                elf_sha256=hashlib.sha256(raw).hexdigest(), elf_size=len(raw),
                symbol=symbol.name, section=section.name, stride=stride,
                symbol_size=size, rows=rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--romfs', type=Path, default=Path('local-assets/switch/romfs'))
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    root, output = args.romfs.resolve(), args.output.resolve()
    if output.is_relative_to(root):
        parser.error('Keep reports outside source RomFS')
    tables = {}
    for rel, symbol, stride, pointers in TABLES:
        tables[rel] = read_table(root / rel, symbol, stride, pointers)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(dict(tables=tables), indent=2, allow_nan=False) + '\n')
    for name, table in tables.items():
        print(f"{name}: {len(table['rows'])} rows, stride {table['stride']:#x}")


if __name__ == '__main__':
    main()
