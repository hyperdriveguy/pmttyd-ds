#!/usr/bin/env python3
"""Pin complete native-requested SetMoveSpeed/SetFallAccel wrappers and raw source tokens."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleGetUnitPtr': (2148501708,
                      60,
                      '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 'BattleTransID': (2148684780,
                   180,
                   '0ee29df5fb9888083ca20bcdab1043e488a773a48ae78ab053286b73351e21c3'),
 'BtlUnit_SetFallAccel': (2148696428,
                          8,
                          '85c10c5b991a02219eea9b7c7073a835992956d6566275713ec24a3389917d75'),
 'BtlUnit_SetMoveSpeed': (2148696412,
                          8,
                          '92a854e5d8f50ed65601a63f3b7e9e244c9d6ab89def3686c214c1e204fb1f76'),
 'btlevtcmd_SetFallAccel': (2148587376,
                            120,
                            '7c58dea9e3a0dd8cdf0759c920cc3d0ce17e886c156888c5e074615758c7220f'),
 'btlevtcmd_SetMoveSpeed': (2148587616,
                            120,
                            'e3d61e964f4aecca484b470146cf99f57385235959e2b60841e07d3655942a29'),
 'evtGetFloat': (2147682368,
                 1212,
                 'f714b2a7ddc29e31034b9c52f37dfcf8cb0f8b21cf73c73a81da2ccc67e8e3d8'),
 'evtGetValue': (2147684960,
                 1112,
                 'ad7dce301c927dbc422a143f3ffb023aa32af8d4f3bb81c752fddc1b9e21bd7a')}

ANCHORS = {2148587376: '9421ffe0',
 2148587380: '7c0802a6',
 2148587384: '90010024',
 2148587388: 'dbe10018',
 2148587392: 'bfc10010',
 2148587396: '7c7e1b78',
 2148587400: '83e30018',
 2148587404: '809f0000',
 2148587408: '4bf23ad1',
 2148587412: '7c601b78',
 2148587416: '7fc3f378',
 2148587420: '7c040378',
 2148587424: '48017c4d',
 2148587428: '809f0004',
 2148587432: '7c7f1b78',
 2148587436: '7fc3f378',
 2148587440: '4bf23091',
 2148587444: 'ffe00890',
 2148587448: '806d1c70',
 2148587452: '7fe4fb78',
 2148587456: '4bfeb10d',
 2148587460: 'fc20f890',
 2148587464: '4801a9a5',
 2148587468: 'cbe10018',
 2148587472: '38600002',
 2148587476: 'bbc10010',
 2148587480: '80010024',
 2148587484: '7c0803a6',
 2148587488: '38210020',
 2148587492: '4e800020',
 2148587616: '9421ffe0',
 2148587620: '7c0802a6',
 2148587624: '90010024',
 2148587628: 'dbe10018',
 2148587632: 'bfc10010',
 2148587636: '7c7e1b78',
 2148587640: '83e30018',
 2148587644: '809f0000',
 2148587648: '4bf239e1',
 2148587652: '7c601b78',
 2148587656: '7fc3f378',
 2148587660: '7c040378',
 2148587664: '48017b5d',
 2148587668: '809f0004',
 2148587672: '7c7f1b78',
 2148587676: '7fc3f378',
 2148587680: '4bf22fa1',
 2148587684: 'ffe00890',
 2148587688: '806d1c70',
 2148587692: '7fe4fb78',
 2148587696: '4bfeb01d',
 2148587700: 'fc20f890',
 2148587704: '4801a8a5',
 2148587708: 'cbe10018',
 2148587712: '38600002',
 2148587716: 'bbc10010',
 2148587720: '80010024',
 2148587724: '7c0803a6',
 2148587728: '38210020',
 2148587732: '4e800020',
 2148696412: 'd0230170',
 2148696416: '4e800020',
 2148696428: 'd0230174',
 2148696432: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20', 2150983304: '0003005b8010d770fffffffef24a7d4c', 2150983420: '0003005b8010d770fffffffef24a7ce6', 2150983948: '0003005b8010d770fffffffef24a7d4c', 2150984376: '0003005b8010d770fffffffef24a7ce6', 2150981440: '0003005b8010d860fffffffef24a8e80', 2150981756: '0003005b8010d860fffffffef24a8680', 2150984556: '0003005b8010d860fffffffef24a9280', 2151035960: '0003005b8010d770fffffffef24a7c80'}

EXPECTED_LITERAL_BITS = {0xf24a8e80:'40a00000',0xf24a8680:'40400000',0xf24a9280:'40c00000',0xf24a7d4c:'3f330000',0xf24a7ce6:'3f198000',0xf24a7c80:'3f000000'}
CONSTANTS[0x8041fa1c]='3a800000'

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
        decoded={}
        for address,value in CONSTANTS.items():
            if len(value)!=32:
                continue
            token=struct.unpack('>I',read(address+12,4))[0]
            signed=token if token<0x80000000 else token-0x100000000
            biased=(signed+230000000)&0xffffffff
            if biased>=0x80000000:
                biased-=0x100000000
            rounded=struct.unpack('>f',struct.pack('>f',biased))[0]
            bits=struct.pack('>f',rounded*struct.unpack('>f',read(0x8041fa1c,4))[0]).hex()
            if EXPECTED_LITERAL_BITS.get(token)!=bits:
                raise ValueError('Used original movement literal decoding differs')
            decoded[hex(token)]=bits
    return dict(schema=1,scope='Complete movement wrappers/leaves;operand/actor/lookup bodies linked;original seven Jump and one Majinai requests pinned;no source/Game/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),source_requests=8,decoded_literal_bits=decoded)


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
