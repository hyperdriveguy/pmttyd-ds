#!/usr/bin/env python3
"""Read-only US source pins for packed recovery and its exact reached children."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = {
    'BtlUnit_CheckRecoveryStatus': (0x80127890, 0x2c8, '47d904e6084c1438de661cd76bd1a6866bc5595a8dd531837eedcc4f80cdf302'),
    'BtlUnit_SetStatus': (0x80127bac, 0x388, '30d8aa59d2ae6b40dec269bc06d5e748da33249ad871f033c7bad2f04c604dd5'),
    'BtlUnit_CheckStatus': (0x80127814, 0x7c, '55635675e06c5fe3840551f6b46808ad814e2bafc47ff3f64442fd7310a09ebe'),
    'BtlUnit_GetStatus': (0x80127f34, 0x1c4, '1844c5989153ff06fa1639a54f08af95913427b8b88d4dd7ac31f66867702322'),
    '_CheckMoveCount': (0x801280f8, 0x4c, '613b924aab9281626a544a7be56f28b591e1c6c325c78223c38585033d44c794'),
    'BtlUnit_ResetMoveStatus': (0x801266d4, 0xf8, '0a575a196a1a6035c1673c0d2884b56519c92f410ecaccb07d317e3bd153615e'),
}
RECOVERY_JUMPS = (
    0x80127970, 0x8012792c, 0x80127970, 0x80127970, 0x80127adc,
    0x80127970, 0x80127970, 0x80127970, 0x80127970, 0x80127970,
    0x801279f4, 0x801279f4, 0x80127a68, 0x80127a68, 0x80127adc,
    0x80127adc, 0x80127b40, 0x80127b40, 0x80127970, 0x80127970,
    0x80127970, 0x80127970, 0x80127970, 0x80127adc, 0x80127adc,
)


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol = struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        if len(header) != 256:
            raise ValueError('Truncated DOL header')
        sections = [tuple(struct.unpack_from('>I', header, base + 4*i)[0]
                          for base in (0, 0x48, 0x90)) for i in range(18)]

        def read(address, size):
            matches = [offset + address - va for offset, va, length in sections
                       if offset and va <= address and address + size <= va + length]
            if len(matches) != 1:
                raise ValueError('Ambiguous/unmapped source address')
            disc.seek(dol + matches[0])
            raw = disc.read(size)
            if len(raw) != size:
                raise ValueError('Truncated source body')
            return raw

        for name, (address, size, digest) in BODIES.items():
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError('Original recovery source changed: ' + name)
        jumps = struct.unpack('>25I', read(0x80364204, 100))
        if jumps != RECOVERY_JUMPS:
            raise ValueError('Recovery signed-low8 dispatch table changed')
    return dict(bodies=len(BODIES), instructions=sum(n//4 for _, n, _ in BODIES.values()),
                recovery_dispatch_entries=len(jumps),
                limitation='Static original source pins; not runtime, hardware or whole Turn proof')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args = parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))


if __name__ == '__main__':
    main()
