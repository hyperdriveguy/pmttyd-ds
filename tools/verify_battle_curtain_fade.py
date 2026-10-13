#!/usr/bin/env python3
"""Read-only US fade23/curtain provider anchors; static evidence only."""
import argparse,csv,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
HASHES={
 'fadeEntry':'34375b105173c33a535a1d3057bc51f8f7ef1a0b677970e5576df7ff495878ff',
 'fadeMain':'369db5633483e4b397234036dae4080d6e8849d68ea747daec990b41fd891437',
 'disp_maku':'75fbec4b1248b4c8f24a4c93269f01e98bbf990fa60235378ad1cf6cf35c850b',
 'fadeIsFinish':'62d8e88e6c6ebb205a2b428a54a443bb13161d1ee3a84adeb7ba1816f1593557',
 'animPoseGetLoopTimes':'576896038164c06448527e5b6afcf816074cc06c198fcc77658e28a653c4b838',
 'animPoseMain':'9083ebe5e58f3c2bd56914adfa318296291b5ca0ddc779d2bca7e6e3cbeebd26'}
def verify(iso,symbols):
    rows=list(csv.DictReader(Path(symbols).open()))
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);header=f.read(0x100)
        sections=[tuple(struct.unpack_from('>I',header,o+4*i)[0]for o in [0,0x48,0x90])for i in range(18)]
        def read(address,size):
            offset,base,length=next(s for s in sections if s[1]<=address and address+size<=s[1]+s[2]);f.seek(dol+offset+address-base);raw=f.read(size)
            if len(raw)!=size:raise ValueError('Truncated original DOL')
            return raw
        for name,digest in HASHES.items():
            r=next(r for r in rows if r['area']=='_main'and r['name']==name)
            if hashlib.sha256(read(int(r['ram_addr'],16),int(r['size'],16))).hexdigest()!=digest:raise ValueError('Original '+name+' changed')
        for address,value in [(0x8030468c+23*4,0x80012a88),(0x80304790+4,0x80013864),(0x8041f794,0xffffffff),(0x8041f810,0x3f7ae148)]:
            if struct.unpack('>I',read(address,4))[0]!=value:raise ValueError('Original fade23 constant/dispatch changed')
        if read(0x802bf668+0xa8,len(b'OFF_d_maku_left_right\0'))!=b'OFF_d_maku_left_right\0' or read(0x8041f80c,4)!=b'A_2\0':raise ValueError('Original curtain owner/pose changed')
    return 12
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print(f'{verify(a.iso,a.symbols)} original fade23/provider hashes and anchors verified read-only')
