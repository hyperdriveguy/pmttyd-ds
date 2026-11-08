#!/usr/bin/env python3
"""Import original fukidashi update data into an empty editable destination."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
# US DOL initialized spans, independently verified in the effect contract.
SPANS = (
    (0x802f3684, 12, '15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b'),
    (0x8036e860, 63, 'd04a053a109fa6d6e15464c75fb09420931de031671bcab2e2b22c7c38596948'),
    (0x804171c0, 39, '370874ebcf7f79265b4d0871a507519ecfa8af8ca2cb6398843d83a8cb2e27ee'),
    (0x804171e8, 38, 'bd4f0361dc4b7d72fe1eab4c119283df79fc5f4ee86fd94206ae196f534d1c0f'),
    (0x80417210, 2, '6775ad8bf1895ff2d867c9008219752f8f42c78b5fd101ace5255283b35145c9'),
    (0x80417214, 9, '99be3593e7e3ee47a759e76c6cd383e977716bf09a9a4f4b31be4585382c20e4'),
    (0x80417220, 28, '6d7aa2c9edf9823d1d116cdfc8ef7307698614f4d8f1b9b7ec5407e8cc11fc46'),
    (0x8041723c, 10, '2308c71d1eef4b2721b6709eb4882d1ab205d484cc30e9b3d7a8bfdfbe0d4f4c'),
    (0x80417248, 9, 'ea83d48d2ca491fadac4823b8be6705e770fd27bd49c9f705ea7046d7fcafd83'),
    (0x80417254, 34, '527f4abff3f67971d8c0365d0a3aaecf19466cd1fa19b475cd7a353e8f2dca28'),
    (0x80417278, 28, '2bd7a2ee404c6279445ad2db52aab400dd77bc2d59443946850acbac49a42729'),
    (0x804228c0, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'),
    (0x80422940, 4, '910c511d81fd1104bac6c142a47454f4a19a9ca60fee521aa67e0dd63ccecb2b'),
    (0x80422944, 4, '5609ee5b02f84f13232459c97c2a78ba4300a9b8fb5a2088e10ffa2843f7a9dd'),
    (0x80422948, 4, '2e4b88c5b36065e9192f27e51a58ea9293d498848552a5ddb361a4e2336f7fae'),
)

def build(iso, destination):
    destination = Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('Import destination must be empty')
    segments, provenance = [], []
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol = struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I',header,field+4*i)[0]
                          for field in (0,0x48,0x90)) for i in range(18)]
        for address, length, expected in SPANS:
            padded = (length + 3) & ~3
            offsets = [dol+off+address-va for off,va,size in sections
                       if va <= address and address+padded <= va+size]
            if len(offsets) != 1:
                raise ValueError('Ambiguous original data span')
            disc.seek(offsets[0]); data = disc.read(padded)
            if len(data) != padded or hashlib.sha256(data[:length]).hexdigest() != expected:
                raise ValueError('Original fukidashi data differs')
            segments.append(dict(address=address,bytes=data.hex()))
            provenance.append(dict(address=hex(address),original_bytes=length,sha256=expected,
                                   packaged_bytes=padded,iso_offset=offsets[0],
                                   packaged_sha256=hashlib.sha256(data).hexdigest(),license=LICENSE))
    destination.mkdir(parents=True,exist_ok=True)
    (destination/'event.json').write_text(json.dumps(dict(schema=1,segments=sorted(segments,key=lambda s:s['address']),license=LICENSE),indent=2)+'\n')
    (destination/'provenance.json').write_text(json.dumps(dict(source='sys/main.dol',spans=provenance,conversion='Original initialized bytes; word alignment retains original adjacent bytes, not synthesized zero padding.',license=LICENSE),indent=2)+'\n')

if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True)
    a=p.parse_args();build(a.iso,a.destination)
