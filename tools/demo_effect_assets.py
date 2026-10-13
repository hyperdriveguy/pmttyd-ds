#!/usr/bin/env python3
"""Export used original dmo crystal/radiation data and masks read-only."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract,convert_tpl

EXPECTED_FUNCTIONS={'crystal_entry': '4457a8487c825bf8970b736a3c6806f0100f3b5431e6ba93da6760abb154489a', 'crystal_main': '2cd4c9b6d8e377401832600f88f89046e536262a4e06d9d94f87ed53de093feb', 'crystal_disp': '8cdc3a0911db5126ff34c14f0149ef16bd1b9793e9a95745efc6de516c957605', 'radiation_entry': '705205ba820355c4819a8b600eaed90d5fab304cc7db27775091ad8205c80c8a', 'radiation_main': 'e4ebac1d9b13715f84fe4f5246780e9bf4e8f0a6963dac45edf1c2a6819517ea', 'radiation_disp': '2b03742f389e67ba0e91859539e1ff040cb69d551aa1ef66417324d7732efcc7'}

SPANS=[('crystal_entry',0x801b9484,0x1c8),('crystal_main',0x801b8ea4,0x5e0),('crystal_disp',0x801b8a38,0x46c),('radiation_entry',0x801e1e0c,0x3c0),('radiation_main',0x801e1878,0x594),('radiation_disp',0x801e1280,0x5f8)]


def build(iso,out):
    iso=Path(iso);out=Path(out);out.mkdir(parents=True,exist_ok=True)
    work=Path('local-assets/demo-effects');work.mkdir(parents=True,exist_ok=True)
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+Path('tools/iso_assets.py').read_bytes()+str((iso.stat().st_size,iso.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/'demo-effects.json';required=['demo-effects.bin','demo-crystal-109.t3x','demo-crystal-110.t3x']
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all((out/name).is_file() for name in required):return
    with iso.open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
        def read(address,size):
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+i*4)[0] for b in [0,0x48,0x90])
                if va<=address and address+size<=va+length:
                    source.seek(dol+off+address-va);raw=source.read(size)
                    if len(raw)!=size:raise ValueError('Truncated source DOL data')
                    return raw
            raise ValueError('Data outside original DOL')
        arrays=[(0x8039f130,4),(0x8039f168,4),(0x80419b60,3)]
        vertices=[]
        for address,count in arrays:
            raw=read(address,count*14)
            for xyzst in struct.iter_unpack('>5h4B',raw):
                x,y,z,s,t,*rgba=xyzst;vertices.append(struct.pack('<5f4B',x,y,z,s/32,t/32,*rgba))
        rays=struct.unpack('>80i',read(0x803a7888,320))
        crystal=struct.unpack('>10f',read(0x80425040,40))
        radiation=struct.unpack('>15f',read(0x80425e18,60))
        colors=struct.unpack('>2I',read(0x80425e10,8))
        (out/'demo-effects.bin').write_bytes(b'PMDEFX01'+b''.join(vertices)+struct.pack('<80i10f15f2I',*rays,*crystal,*radiation,*colors))
        evidence=[dict(name=name,address=address,size=size,sha256=hashlib.sha256(read(address,size)).hexdigest()) for name,address,size in SPANS]
        if any(row['sha256']!=EXPECTED_FUNCTIONS[row['name']] for row in evidence):raise ValueError('Original effect source bodies changed')
        data=[dict(address=address,size=count*14,sha256=hashlib.sha256(read(address,count*14)).hexdigest()) for address,count in arrays]
        data+=[dict(address=0x803a7888,size=320,sha256=hashlib.sha256(read(0x803a7888,320)).hexdigest())]
    tpl=work/'effect_n64.tpl';extract(iso,'e/us/effect_n64.tpl',tpl)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for index in [109,110]:
        png=work/f'crystal-{index}.png';convert_tpl(tpl,png,index)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'demo-crystal-{index}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,functions=evidence,data=data,texture_source=json.loads(tpl.with_suffix('.tpl.provenance.json').read_text()),textures=[109,110],conversion='Original signed-short N64 XYZ and fraction5 ST retained; source RGBA/quad/triangle/ray rows/constants exported little-endian; original mask dimensions unchanged',limitation='Used demo subtypes2/6/3 only; shader/matrix/camera/manager numeric and hardware parity remain separate',license='Original Nintendo/Intelligent Systems resources, local use; no redistribution rights'),indent=2)+'\n')
    print('Original demo effect vertices11, radiation rows16, texture masks109/110 exported')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
if __name__=='__main__':main()
