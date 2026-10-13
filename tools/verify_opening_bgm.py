#!/usr/bin/env python3
"""Verify original US demo BGM requests and ordinary stream fade branch read-only."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import inventory_events
from verify_opening_book_requests import verify as verify_book

SPANS=[
    (0x80152450,0x4c,'93fe06ad052adf30c8f470b38c3fc760438ef34b1fcb2bafd1fed2ba87f7aef6'),
    (0x801525a4,0x30,'df23b6f55d45bc19acd0080bcde36c49e4e472e8fe0d2452f6b2b929e4ae23f9'),
    (0x800dabac,0x350,'38f527b79c239c86be3d5c7f82d3f69213bae597de4b7cb7ae03aca5b5bffc52'),
    (0x800db7a0,0x2c,'11f3c2b6faaa4fc845ccc82f465a6720bd89347e9b0e77c14cea5d99633570f9'),
    (0x800daf24,0x854,'7b3fe9b3bb5a74e1cb174d0a14e460621d4c922d55d95884a27926f01eb3bf0e'),
    (0x800db7cc,0x494,'8cc019f59869a9d519e40598474d996f1d9ebbb0688ac2f3c5e92c88c32cbdba'),
    (0x800ddf54,0xa4,'93458b146a157baacf6b66520b935de3a401ed49ae842168c02a8497e860afd2'),
    (0x800dd2e0,0x638,'93f13bd7f43bcc570277a16e59963012648c2e2ed631754e6a3105feed04e88c'),
    (0x800ddc1c,0x198,'29116a3cc5090f2de4aac7cb61a9203e6241422b7ae6f7150c026c0410cf8625'),
    (0x800e22c0,0x14,'c56592cf2eeabd1373e9cd24e8455ff1cded8e8ef2e4a02fe3869c9e1c38648f'),
    (0x80315b08,32,'a178b8fd05553dbca6e932482bac7a19755ffe74a3b2bb5ca4a6014c4962b1e8'),
    (0x804218cc,4,'2412b43a42f11c1cb3ef7956d3e293016e671a9061fbb76a4282c8b15139cc48'),
    (0x80421910,8,'609e0f8a545be527828c9f59530893aee9e6074deb89cd76c7887419e73c1156'),
]

def verify(iso,symbols):
    count=verify_book(iso,symbols)
    entry=next(f for f in inventory(iso)['files'] if f['path']=='rel/dmo.rel')
    with Path(iso).open('rb') as source:
        source.seek(entry['offset']);raw=source.read(entry['size'])
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
        for address,size,digest in SPANS:
            for i in range(18):
                offset,start,length=(struct.unpack_from('>I',header,b+4*i)[0] for b in [0,0x48,0x90])
                if start<=address and address+size<=start+length:
                    source.seek(dol+offset+address-start)
                    if hashlib.sha256(source.read(size)).hexdigest()!=digest:raise ValueError(f'Original BGM source changed at {address:x}')
                    break
            else:raise ValueError('BGM source outside loaded DOL')
    commands={c['address']:c for c in inventory_events(raw,symbols,'dmo',['demo_evt'])['events'][0]['commands']}
    for address,name,identifier in [(0x23f8,'evt_snd_bgmon','BGM_DEMO1'),(0x2fa8,'evt_snd_bgmon','BGM_DEMO2')]:
        operands=commands[address]['operands']
        if operands[0].get('annotation')!=name or operands[1]['raw']!=513 or operands[2].get('identifier')!=identifier:raise ValueError('Original demo BGM on request changed')
    operands=commands[0x2ef8]['operands']
    if operands[0].get('annotation')!='evt_snd_bgmoff_f' or [o['raw'] for o in operands[1:]]!=[513,6000]:raise ValueError('Original BGM fadeoff request changed')
    if commands[0x2fa0]['opcode']!=10 or commands[0x2fa0]['operands'][0]['raw']!=1000:raise ValueError('Original post-adjust BGM wait changed')
    return count+len(SPANS)+4

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);parser.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');args=parser.parse_args()
    print(f'{verify(args.iso,args.symbols)} original book/BGM request/body/table anchors verified read-only')
