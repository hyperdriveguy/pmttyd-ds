#!/usr/bin/env python3
"""Pin the complete original success-feedback parent, visual references and data, ISO read-only.
Native A_1 playback adapts the effect pool/camera; this verifier never executes original code.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
SPANS = [{'name': 'ACSuccessEffect', 'address': 2148560800, 'size': 836, 'sha256': '70af9dac90e140fbd392993137fbd0daf82951df21855dae6e9a4a4642a40a7c'}, {'name': 'NiceEntry', 'address': 2148813516, 'size': 504, 'sha256': '4b0048598b21dd405ee4a645cd278b1577917c512d79285b9280a3c5e7f89a6e'}, {'name': 'NiceDraw', 'address': 2148812604, 'size': 372, 'sha256': '6323d3ea9d407539f97f05166038700b0d59416c34b356aaeb07d78ae493a6ca'}, {'name': 'NiceMain', 'address': 2148812976, 'size': 540, 'sha256': 'fa6226ca27ed3ed6247f9c05da823281bcdaf8b7c8df22ab22add79b324a6780'}, {'name': 'WidthsAndDispatch', 'address': 2150943560, 'size': 280, 'sha256': '82733f58bf9f80deef67f6cbcbea8a50b4b379c01b8ea209bda4e71a96f45416'}, {'name': 'SignedConversionBias', 'address': 2150557192, 'size': 8, 'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'}, {'name': 'SoundNames', 'address': 2150557208, 'size': 160, 'sha256': '2008e604e56985f150beecf1115709ac0f3f455f10e20224cb471653d457b7a5'}, {'name': 'SuccessScalars', 'address': 2151817872, 'size': 36, 'sha256': '2e190487b2a36cd60b8301763ec41da92662b2d0e02f36908687754a707825ea'}, {'name': 'NiceScalarsAndAnimation', 'address': 2151820928, 'size': 84, 'sha256': '3800e5a1daebb52e9874cdeda272eae53138ae516b5ee9c3c0a2fbb1e487cfdd'}, {'name': 'BankLanguageTable', 'address': 2151774968, 'size': 28, 'sha256': '604b909610467e32c77c17c3fb3e5e3f0959ac3f7f5ca61f406daed68bd91ea6'}, {'name': 'USBankNames', 'address': 2151774828, 'size': 32, 'sha256': 'b868c59fbda1ba46d5914d54229b181c2cdd03ced16d71aadae7a5738632c44a'}, {'name': 'FadeTable', 'address': 2150579872, 'size': 168, 'sha256': 'c2b95bc7c3654e7e3d87e63649554df549c0b6b598d18ad35623e21a63897a8e'}]

def verify(iso=DEFAULT_ISO):
    rows = {row['path']: row for row in inventory(iso)['files']}
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US revision zero required')
        source.seek(0x420)
        dol, = struct.unpack('>I', source.read(4))
        source.seek(dol)
        header = source.read(256)
        for span in SPANS:
            if 'file' in span:
                offset = rows[span['file']]['offset'] + span['offset']
            else:
                address, size = span['address'], span['size']
                for i in range(18):
                    off, base, count = [struct.unpack_from('>I', header, table + 4*i)[0] for table in (0, 0x48, 0x90)]
                    if base <= address and address + size <= base + count:
                        offset = dol + off + address - base
                        break
                else:
                    raise ValueError('Source DOL span unavailable')
            source.seek(offset)
            raw = source.read(span['size'])
            if hashlib.sha256(raw).hexdigest() != span['sha256']:
                raise ValueError('Original source changed: ' + span['name'])
    return dict(spans=len(SPANS), source='G8ME01 revision0', original_executed=False,
                policy='Exact344 parent decisions with a typed native visual policy; no whole source effect pool/GX receipt')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso), indent=2))
