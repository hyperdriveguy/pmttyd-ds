#!/usr/bin/env python3
"""Verify complete US revision-0 weapon cursor and its literal source inputs."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = (
    ("DrawCursor", 0x80117A6C, 0x1B0, "2964e46448e17f1514d4a97b5656a96de4b69e92128c864511f14e4d7a128ac7"),
    ("iconDispGx", 0x8001A918, 0x24, "140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d"),
)
LITERALS = (
    (0x802EF750, "c2700000420c000000000000"),  # caller's original XYZ copy
    (0x802EF780, "4330000080000000"),  # signed integer conversion bias
    (0x8042242C, "00000000"), (0x8042246C, "41200000"),
    (0x80422474, "41700000"), (0x80422470, "431b0000"),
    (0x80422478, "3f400000"), (0x8042244C, "41a00000"),
    (0x8042247C, "43240000"), (0x80422480, "41c00000"),
    (0x8042245C, "3f800000"), (0x80005418, "3c40804260427440"),
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
                scope="Whole DrawCursor and iconDispGx alpha wrapper; source r2, caller XYZ, conversion bias and all nine scalars")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
