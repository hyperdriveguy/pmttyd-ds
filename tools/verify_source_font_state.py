#!/usr/bin/env python3
"""Pin fourteen complete original US font-state bodies and all linked calls."""
import argparse
import hashlib
import struct
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO

BODIES = (
    ('FontDrawScaleVec', 0x8007773c, 0x1c, '26113f2ec520df74bc87e94b527935382c23a8f58eb72aa6b23f6d28b266d23c'),
    ('FontDrawScale', 0x80077758, 0x14, '6e8e27a420b24457feacd7299af528cbad0ce3191ec75fa256ade04db143fd9a'),
    ('FontGetDrawColor', 0x8007776c, 0x8, 'c6f1958d8f31377120b808421aa0b0834111615bde754102fd4818bbc33d2495'),
    ('FontDrawColor_', 0x80077774, 0x6c, 'b0db2bab9e8a6a7abb39015a52e081fcb688a45880bd0de3115cf892c73a115f'),
    ('FontDrawColor', 0x800777e0, 0x64, 'bdebd4849d9237cbf2e464aaafb635a197607bf7f3578b9a67a6e68545d163f7'),
    ('FontDrawColorIDX', 0x80077844, 0x70, '89d8b0d1ec95ec86d01200686e247447d483293170879b826a3ff5c0642f753c'),
    ('FontDrawNoiseOff', 0x800778b4, 0x2c, 'c50e400bc6405f84f6ee6f1e67b5098b60e7650ca9ab19e47b9f799e3d5b7182'),
    ('FontDrawNoise', 0x800778e0, 0x104, '20c9d9bc8851e4e5ad7fd1e6c6e3e7b0d4528763f98554f3f6c7bd6e60ab45b8'),
    ('FontDrawRainbowColorOff', 0x800779e4, 0x24, '7a229b23830af1506ecd9f67b035008ed6fa86d7574096fa4955f115799e09cb'),
    ('FontDrawRainbowColor', 0x80077a08, 0xa0, 'fdef6842ae6ee85d751514728a5ff9d7f9530623ed8943525b04989dca198cbd'),
    ('FontDrawEdgeOff', 0x80077aa8, 0xc, 'b8441d04c0b6949553b1de30bde90b4909fd0ce7269054a157985b8e9dee380b'),
    ('FontDrawEdge', 0x80077ab4, 0xc, '82b735dffab4a742775ba34eb501133f3e61bd4f170ac4fb123c2e38dfd85bd7'),
    ('FontDrawStart_alpha', 0x80077ac0, 0x8c, '08a26149fd52bb225b6525ccab2808031aa0c3fd4618fa602fecc570a9503fb5'),
    ('FontDrawStart', 0x80077b4c, 0x90, '9cd10925bd73263fdf1feb2e122e030aa6ce00a54e86ac67f46cca6a6bf1e8e3'),
)
CALLS = (
    (0x800777cc, 0x802b8ab8),
    (0x80077830, 0x802b8ab8),
    (0x800778a0, 0x802b8ab8),
    (0x800778c4, 0x802b5084),
    (0x800778cc, 0x802b8fcc),
    (0x800778f0, 0x802b5084),
    (0x800778f8, 0x802b8fcc),
    (0x80077904, 0x802b8c3c),
    (0x80077918, 0x802b8e30),
    (0x80077934, 0x802b89e8),
    (0x80077950, 0x802b8a50),
    (0x80077968, 0x802b8960),
    (0x80077980, 0x802b89a4),
    (0x80077990, 0x802b7880),
    (0x800779a4, 0x8029e2b0),
    (0x800779b4, 0x802ba3d0),
    (0x800779d0, 0x802b4e04),
    (0x800779f4, 0x802b8fcc),
    (0x80077a18, 0x802b8fcc),
    (0x80077a2c, 0x802b8e30),
    (0x80077a48, 0x802b89e8),
    (0x80077a64, 0x802b8a50),
    (0x80077a7c, 0x802b8960),
    (0x80077a94, 0x802b89a4),
    (0x80077b38, 0x80074bfc),
    (0x80077bc8, 0x80074bfc),
)
LITERALS = (
    (0x80005418, '3c408042604274403da0804161adcf20'),
    (0x802c2b6c, '3f8000003f8000003f800000'),
    (0x80420438, '3c800000'),
    (0x804203e4, '3f800000'),
)

def verify(iso=DEFAULT_ISO):
    source = Dol(iso)
    try:
        actual = []
        for name, address, size, digest in BODIES:
            body = source.read(address, size)
            if hashlib.sha256(body).hexdigest() != digest:
                raise ValueError("Original body mismatch: " + name)
            for offset in range(0, size, 4):
                word = struct.unpack_from(">I", body, offset)[0]
                if word >> 26 == 18 and word & 1:
                    delta = word & 0x03fffffc
                    if delta & 0x02000000:
                        delta -= 0x04000000
                    target = delta if word & 2 else address + offset + delta
                    actual.append((address + offset, target & 0xffffffff))
        if tuple(actual) != CALLS:
            raise ValueError("Original linked-call graph mismatch")
        for address, raw in LITERALS:
            expected = bytes.fromhex(raw)
            if source.read(address, len(expected)) != expected:
                raise ValueError(f"Original literal mismatch at {address:#x}")
    finally:
        source.close()
    return {"whole_bodies": len(BODIES), "linked_calls": len(CALLS),
            "literal_spans": len(LITERALS),
            "scope": "Static source, not original execution or native GX/font completion"}

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
