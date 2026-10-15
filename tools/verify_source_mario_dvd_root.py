#!/usr/bin/env python3
"""Pin original DVD-root register leaf, initialized data and UI callers."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODY_SHA = '20e17ad249044e215aba5f071125d98aee73da26094bbb4ff323cfcdf3574f7d'
LITERALS = ((0x8041f494, b'.\0'),
            (0x802c3094, b'%s/f/msgWindow.tpl\0'),
            (0x802c2ba0, b'%s/f/%s.bfn\0'),
            (0x802c2bac, b'%s/font/%s.bfn\0'))

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision-0 disc required')
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        def read(address, size):
            offsets = []
            for i in range(18):
                off, base, length = [struct.unpack_from('>I', header, table+4*i)[0]
                                     for table in (0, 0x48, 0x90)]
                if base <= address and address+size <= base+length:
                    offsets.append(dol+off+address-base)
            if len(offsets) != 1:
                raise ValueError('Unmapped initialized source span')
            disc.seek(offsets[0])
            return disc.read(size)
        body = read(0x80007388, 8)
        if body != bytes.fromhex('386280544e800020') or hashlib.sha256(body).hexdigest() != BODY_SHA:
            raise ValueError('Whole DVD-root getter differs')
        if read(0x80005418, 8) != bytes.fromhex('3c40804260427440'):
            raise ValueError('Original r2 ABI initializer differs')
        for address, expected in LITERALS:
            if read(address, len(expected)) != expected:
                raise ValueError('Original root/format bytes differ')
        # Actual source UI calls; no cache-state or successful-import gate.
        for site, target in ((0x80075024, 0x80007388), (0x8007507c, 0x80007388),
                             (0x8007fc18, 0x80007388), (0x80075048, 0x8026a0b8),
                             (0x80075094, 0x8026a0b8), (0x8007fc30, 0x8026a0b8)):
            op, = struct.unpack('>I', read(site, 4))
            displacement = op & 0x3fffffc
            if displacement & 0x2000000:
                displacement -= 0x4000000
            if op >> 26 != 18 or op & 3 != 1 or (site+displacement)&0xffffffff != target:
                raise ValueError('Original root/formatter caller differs')
        if hashlib.sha256(read(0x8026a0b8, 0xe0)).hexdigest() != '50511d2156f10f1f673ae32bf84f42cce8e35f4b3264040db35daa0666534c30':
            raise ValueError('Original sprintf wrapper differs')
    return dict(root_whole_body=1, root_source_bytes=2, actual_r2_initializer=1,
                original_ui_formats=3, original_ui_callsites=6,
                formatter_wrapper_pinned_not_implemented=1,
                scope='Exact pointer arithmetic only; real source string ownership/formatting remain separate')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
