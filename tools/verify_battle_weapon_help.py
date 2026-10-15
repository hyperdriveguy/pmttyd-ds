#!/usr/bin/env python3
"""Verify complete US revision-0 weapon-help parser and literal source inputs."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = (
    ("DrawHelp", 0x80117C1C, 0x1D4, "8b902f288bcc368e1f4351fb248dcb8d80a65b43effe9aa9e47e16614539681c"),
)
LITERALS = (
    (0x80422428, "ffffffcf"), (0x80422484, "43c80000"),
    (0x80422488, "42480000"), (0x8042246C, "41200000"),
    (0x8042248C, "3f4ccccd"), (0x8042244C, "41a00000"),
    (0x80422490, "40800000"), (0x80422494, "41e00000"),
    (0x80422498, "42500000"),
    (0x802EF788, "82b982c282df82a282cd814082c882a282cc82be8149814900"),
    (0x80005418, "3c40804260427440"),
)

def verify(iso=DEFAULT_ISO):
    with Path(iso).open("rb") as disc:
        if disc.read(8) != b"G8ME01\0\0":
            raise ValueError("US revision-0 disc required")
        disc.seek(0x420)
        dol = struct.unpack(">I", disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        def read(address, size):
            offsets = []
            for index in range(18):
                offset, base, length = (struct.unpack_from(">I", header, table + 4*index)[0]
                                        for table in (0, 0x48, 0x90))
                if base <= address and address + size <= base + length:
                    offsets.append(dol + offset + address - base)
            if len(offsets) != 1:
                raise ValueError(f"Unmapped/ambiguous source span {address:#x}")
            disc.seek(offsets[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError("Truncated source span")
            return data
        for name, address, size, digest in BODIES:
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError("Body mismatch: " + name)
        for address, literal in LITERALS:
            expected = bytes.fromhex(literal)
            if read(address, len(expected)) != expected:
                raise ValueError(f"Literal mismatch: {address:#x}")
    return dict(bodies=len(BODIES), literal_spans=len(LITERALS),
                scope="Whole DrawHelp parser/call order; source r2, original RGBA, all eight floats and exact original null-search fallback bytes")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
