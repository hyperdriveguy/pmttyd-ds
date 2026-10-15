#!/usr/bin/env python3
"""Verify complete US revision-0 scalar font parents and shared source helpers."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = (
    ("FontDrawString", 0x800773D0, 0x68, "ef1f387006006d59d37f2cb505442bfd24bc69433426d29031ec585047b69d25"),
    ("FontDrawStringVecPitch", 0x80077034, 0x334, "6c6436ee7392b72db4ec1c54d24ac8030a1ee32994159f1798b93768488b306b"),
    ("msgIsLead", 0x80081B40, 0x4C, "6b1cc7be1b9b25190052866295ae20aafce435db8822851694bbe3efb5cf4a39"),
    ("CodeToGlyph", 0x80074E90, 0x174, "c5c62aec2542e0ef775597654d52cb05d0afb5f15614f32eaa546a8abb4c8e30"),
    ("msgGetCommand", 0x80081AA0, 0xA0, "9849799a9225a52cc9a1cd0c950302ddc4403f5deb17bb9590fecc64b63d9484"),
)
LITERALS = (
    (0x802C2B78, "000000000000000000000000"),
    (0x802C2B90, "43300000000000004330000080000000"),
    (0x804203F8, "00000000"), (0x8042041C, "3f000000"),
    (0x80420430, "bf800000"), (0x80005418, "3c40804260427440"),
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
                scope="Whole scalar/vector-pitch font parents, shared lead/map/command source bodies, actual source vector/biases/zero/half/pitch and r2")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
