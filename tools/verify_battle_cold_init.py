#!/usr/bin/env python3
"""Verify the complete original US cold BattleInit and retained parent ordering."""
import argparse
import hashlib
import re
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BASE = 0x800f8f04
SIZE = 312
SHA256 = "616f72f3468ba55fb0ac5dc234c5b950eb85fed4c8caafc20cca55bb73aa48c7"


def verify(iso=DEFAULT_ISO):
    with Path(iso).open("rb") as disc:
        if disc.read(8) != b"G8ME01\0\0":
            raise ValueError("Original US revision-0 disc required")
        disc.seek(0x420)
        dol, = struct.unpack(">I", disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        locations = []
        for index in range(18):
            offset, address, size = [struct.unpack_from(">I", header, table + 4 * index)[0]
                                     for table in (0, 0x48, 0x90)]
            if address <= BASE and BASE + SIZE <= address + size:
                locations.append(dol + offset + BASE - address)
        if len(locations) != 1:
            raise ValueError("Unmapped or ambiguous original function")
        disc.seek(locations[0])
        body = disc.read(SIZE)
    if hashlib.sha256(body).hexdigest() != SHA256:
        raise ValueError("Original whole BattleInit changed")
    original = []
    for offset in range(0, SIZE, 4):
        instruction, = struct.unpack_from(">I", body, offset)
        pc = BASE + offset
        if instruction >> 26 == 18 and instruction & 1:
            displacement = instruction & 0x3fffffc
            if displacement & 0x2000000:
                displacement -= 0x4000000
            original.append((pc, "Call", (pc + displacement) & 0xffffffff))
        elif instruction >> 26 in (36, 44):
            register = (instruction >> 16) & 31
            if register == 1:  # source ABI stack save
                continue
            displacement = instruction & 0xffff
            if displacement & 0x8000:
                displacement -= 0x10000
            address = (0x8041eb90 if register == 13 else
                       displacement + (0x20000 if register == 3 else 0))
            original.append((pc, "StoreWord" if instruction >> 26 == 36 else "StoreHalf", address))
    source = (Path(__file__).resolve().parents[1] /
              "source/battle/battle_cold_init_continuation.cpp").read_text()
    retained = [(int(pc, 16), operation, 0x8041eb90 if target == "BattleColdPointerCell" else int(target, 0))
                for pc, operation, target in re.findall(
                    r"\{(0x[0-9a-f]+), Operation::(Call|StoreWord|StoreHalf), ([A-Za-z0-9]+),", source)]
    if retained != original:
        raise ValueError("Retained source call/store order or target changed")
    return {"whole_body_bytes": SIZE, "ordered_calls_and_stores": len(original),
            "scope": "Cold parent only; child services and native startup remain independently owned"}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
