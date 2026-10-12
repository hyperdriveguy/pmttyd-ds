#!/usr/bin/env python3
"""Check enemy sampling, choice and RNG contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {
    'irand': (0x80005da0,0x44,'0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf'),
    'BattleChoiceSamplingEnemy': (0x800febc8,0x9d8,'b6c211c3d0863f6ebab540adbe8c6ed5c825f94919ded9ee8651537ec00e7190'),
    'BattleSamplingEnemyUpdate': (0x800ff5a0,0x20,'c380ce54146ede6785eb5ae4509ffa69f4b595a64ecd73ee08219a9dd429d43e'),
    'BattleSamplingEnemy': (0x800ff5c0,0xc4,'9c0761564dcca42b319ce87e4a6323e8dbb3520ebb94e2393bd8ca41c904a421'),
    '_btlSamplingEnemy': (0x800ff684,0xbd4,'8276dd99cd4c9e1572b903ed72fe3d20a8700be749ee33bdbdbf078cc4ffb15b'),
    'BtlCompForwardLv': (0x801254a0,0x54,'d54a07ed89d09032b834f9a5a6761906f56c6310f69643401f22fd9ca56b3ca3'),
    'rand': (0x8026c324,0x20,'6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5'),
}
ANCHORS = {2147507636: '4082000c', 2147507652: '7c03fbd6', 2148527088: '90050000', 2148527096: '90060000', 2148527108: '40810988', 2148527164: '90d20000', 2148527172: '90d30000', 2148527252: '3803000a', 2148527344: '7c1ef92e', 2148527620: 'fc00001e', 2148528252: 'ec2408bc', 2148529236: 'ec2208fa', 2148529348: '4082005c', 2148529388: '4081000c', 2148529476: '4bf0685d', 2148529504: '7c601851', 2148529524: '1c060024', 2148529624: '90830000', 2148529636: '98c30abc', 2148529648: '99230ac8', 2148529696: '3be00001', 2148529736: '801e0ac0', 2148529772: '7fe3fb78', 2148529844: '981e0a6c', 2148529880: '4802813d', 2148529896: '54000673', 2148529932: '4182000c', 2148530004: 'd001000c', 2148530084: 'ec01203a', 2148530216: '907a0018', 2148530220: '901a0020', 2148530268: '901a0018', 2148530384: '909a000c', 2148530424: '48025ba9', 2148530512: '9b7e0a6c', 2148531044: '41820028', 2148531088: '41820038', 2148531236: '3b39ffff', 2148531704: '40820058', 2148531936: '3b000000', 2148532292: '3ae00000', 2148532440: '9b7e0a6c', 2148532612: '4081000c', 2148532764: '4181000c', 2148685008: '2c000030', 2148685024: '20600030', 2150023992: '900d14a0', 2150023996: '5403847e'}
CONSTANTS = {2151817712: '3fc00000', 2151817716: '3dcccccd', 2151817720: '3ecccccd', 2151817724: '3f000000', 2151817728: '40000000', 2151817732: '3f800000', 2151817736: '3d4ccccd', 2151817740: '40a00000', 2151818856: 'c4480000', 2151818860: '44480000', 2151818864: '42200000', 2150556568: '4330000080000000', 2147505176: '3c40804260427440', 2147505184: '3da0804161adcf20'}

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
    return dict(schema=1,scope='Static US sampling/choice/RNG; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),constants=len(CONSTANTS))


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
