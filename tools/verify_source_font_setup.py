#!/usr/bin/env python3
"""Verify complete US revision-0 font setup, source initializers and raw BFN."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = (
    ("JUTFontSetup", 0x80075004, 0x2F8, "9e7e9f2c64a36a45682515c125640992f54e71040fc7c87a7ea5285602849292"),
    ("fontmgrTexSetup", 0x80077BDC, 0x2C, "bb92b29cdba4d14b264319bf2a44e716b2b26cb222f4c3e669700aa7bc312736"),
    ("CodeToGlyph", 0x80074E90, 0x174, "c5c62aec2542e0ef775597654d52cb05d0afb5f15614f32eaa546a8abb4c8e30"),
)
LITERALS = (
    (0x802C2B30, "70617065726d6172696f7365745f4a504e00"),
    (0x802C2B44, "70617065726d6172696f7365745f555300"),
    (0x802C2B58, "70617065726d6172696f7365745f455500"),
    (0x80416038, "802c2b30802c2b44802c2b58802c2b58802c2b58802c2b58802c2b44"),
    (0x802C2BA0, "25732f662f25732e62666e00"),
    (0x802C2BAC, "25732f666f6e742f25732e62666e0000"),
    (0x80416010, "1e1414ffdcdcdcffb47814ff00000004000000020000000200000002000000020000000200000002"),
    (0x804203F8, "00000000"), (0x80005418, "3c40804260427440"),
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
        if struct.unpack_from(">II", header, 0xD8) != (0x803C5360, 0x645AC):
            raise ValueError("Original DOL BSS declaration changed")
        # Initialized sections win over the broad BSS extent in a cold image.
        for address in (0x8041E978, 0x8041E9BB, 0x80429578, 0x80429598):
            for index in range(18):
                base, length = (struct.unpack_from(">I", header, table + 4*index)[0]
                                for table in (0x48, 0x90))
                if base <= address < base + length:
                    raise ValueError("Expected actual BSS cell became initialized data")
        for name, address, size, digest in BODIES:
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError("Body mismatch: " + name)
        for address, literal in LITERALS:
            expected = bytes.fromhex(literal)
            if read(address, len(expected)) != expected:
                raise ValueError(f"Literal mismatch: {address:#x}")
    from source_font_resources_assets import SOURCE_PATH, SOURCE_SHA256, SOURCE_SIZE
    from iso_assets import inventory
    entries = [entry for entry in inventory(Path(iso))["files"] if entry["path"] == SOURCE_PATH]
    if len(entries) != 1 or entries[0]["size"] != SOURCE_SIZE:
        raise ValueError("Raw BFN extent changed")
    with Path(iso).open("rb") as disc:
        disc.seek(entries[0]["offset"])
        if hashlib.sha256(disc.read(SOURCE_SIZE)).hexdigest() != SOURCE_SHA256:
            raise ValueError("Raw BFN identity changed")
    return dict(bodies=len(BODIES), literal_spans=len(LITERALS), raw_bfn_sha256=SOURCE_SHA256,
                scope="Whole Setup/caller/map, original name/format/palette/spacing/zero/r2, true DOL BSS vs initialized cells and exact original US BFN")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
