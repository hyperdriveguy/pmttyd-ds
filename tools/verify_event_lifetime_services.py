#!/usr/bin/env python3
"""Check deletion and stop/start lifetime contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtDelete': (2147706624, 464, 'ceae94ec9700b3a757e1611c3905d5c71c39ba06a9090943c40976cb926d5940'),
 'evtDeleteID': (2147706468,
                 156,
                 '6fa62ffd7b14955f741809ee989270265ccd4bfbccd47f42696e0cc6d80a32d3'),
 'evtStop': (2147705360, 980, 'cd93f55afa69c7ad1a57626848d18530200b5dc277caeccdedd48335d5acef2d'),
 'evtStart': (2147704380, 980, 'ce40808d70befb54b5100243dbf3696238fd9e50efaa99f8b25d4b8b8330576d'),
 'evtStopAll': (2147703912,
                148,
                'af4f97d2a90bab2e2687497985b60f98c05b6283b648813c95ae54a9c47746ac'),
 'evtStartAll': (2147703764,
                 148,
                 '2ddaa33356c2750613ed464e9c9d392ca3973462492f00b411b76ee5fb55ec4a')}

ANCHORS = {2147706692: '4182016c',
 2147706708: '4bffffad',
 2147706712: '83be0090',
 2147706744: '4082000c',
 2147706752: '4bffff81',
 2147706764: '801e0000',
 2147706776: '809f006c',
 2147706784: '418200e8',
 2147706796: '54630734',
 2147706800: '98640008',
 2147706804: '90040070',
 2147706812: '9004009c',
 2147706932: '900400d8',
 2147706940: '900400dc',
 2147706956: '900400e4',
 2147706964: '9004017c',
 2147706972: '90040178',
 2147706980: '90040180',
 2147706992: '90640184',
 2147706996: '90040188',
 2147707004: '9004018c',
 2147707012: '90040190',
 2147707036: '981f0008',
 2147707040: '4bfce815',
 2147707052: '900d1940',
 2147706548: '41820018',
 2147706560: '4082000c',
 2147706568: '48000039',
 2147706580: '801f0000',
 2147705412: '831e0070',
 2147705444: '83380070',
 2147705476: '80790070',
 2147705492: '4bffff7d',
 2147705540: '4bffff4d',
 2147705552: '80160000',
 2147705564: '8819000c',
 2147705584: '98190008',
 2147705636: '806d90b0',
 2147705684: '4bfffebd',
 2147705732: '4bfffe8d',
 2147705776: '981a0008',
 2147705820: '98180008',
 2147705872: '806d90b0',
 2147705940: '80780070',
 2147705956: '4bfffdad',
 2147706004: '4bfffd7d',
 2147706048: '98180008',
 2147706088: '806d90b0',
 2147706136: '4bfffcf9',
 2147706184: '4bfffcc9',
 2147706228: '981b0008',
 2147706272: '98170008',
 2147706284: '801c0000',
 2147706296: '881e000c',
 2147706316: '981e0008',
 2147704512: '4bffff7d',
 2147704560: '4bffff4d',
 2147704604: '98190008',
 2147704796: '981a0008',
 2147704840: '98180008',
 2147705068: '98180008',
 2147705204: '4bfffcc9',
 2147705248: '981b0008',
 2147705292: '98170008',
 2147705316: '881e000c',
 2147705336: '981e0008',
 2147704004: '4800054d',
 2147704016: '801f0000',
 2147703856: '4800020d',
 2147703868: '801f0000'}

CONSTANTS = {2147505184: '3da0804161adcf20'}

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol_offset=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_offset)
        header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    matches.append(dol_offset+off+address-va)
            if len(matches)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
        stop=read(0x80036210,0x3d4)
        start=read(0x80035e3c,0x3d4)
        changed={0xdc,0x19c,0x1c8,0x2ac,0x360,0x38c,0x3b8}
        for offset in range(0,0x3d4,4):
            lhs,rhs=stop[offset:offset+4],start[offset:offset+4]
            if offset in changed:
                if (lhs.hex(),rhs.hex())!=('60000002','540007fa'):
                    raise ValueError('Stop/start flag mutation mismatch')
            elif lhs!=rhs:
                raise ValueError('Stop/start paired traversal mismatch')
    return dict(schema=1,scope='Complete deletion and stop/start traversal; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),paired_instructions=245,flag_mutation_sites=7)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output')
    a=p.parse_args();result=verify(a.iso)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
