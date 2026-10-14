#!/usr/bin/env python3
"""Import original audience file payloads unchanged; no synthetic crowd artwork."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
def build(iso=DEFAULT_ISO):
    root=Path('local-assets/audience');root.mkdir(parents=True,exist_ok=True)
    out=Path('romfs');out.mkdir(exist_ok=True)
    banks=[]
    for name,target,size,count in [('audience_normal.tpl','audience-normal.tpl',428896,205),('audience_luigi.tpl','audience-luigi.tpl',39776,19)]:
        source='battle/audience/'+name;path=root/name
        extract(iso,source,path);data=path.read_bytes()
        magic,n,table=struct.unpack_from('>III',data)
        if len(data)!=size or magic!=0x20af30 or n!=count or table+n*8>len(data):raise ValueError('Original audience TPL changed')
        for i in range(n):
            image,palette=struct.unpack_from('>II',data,table+8*i)
            h,w,fmt,pixels=struct.unpack_from('>HHII',data,image)
            if image+36>len(data) or palette or (w,h,fmt)!=(64,64,14) or pixels+2048>len(data):raise ValueError('Invalid original audience image')
        product=out/target
        if not product.exists() or product.read_bytes()!=data:product.write_bytes(data)
        banks.append(dict(source=source,path=str(product),bytes=len(data),images=n,sha256=hashlib.sha256(data).hexdigest(),format='Original TPL/CMPR64x64'))
    (out/'audience-resources.json').write_text(json.dumps(dict(banks=banks,conversion='Byte-for-byte import, original image indices/GX payloads retained; no resampling or replacement art',license='Nintendo/Intelligent Systems original assets, local use only; no redistribution rights',limitations='Typed native file availability; active crowd GX/Citro3D conversion/drawing remains separate'),indent=2)+'\n')
    print('Original audience TPL banks:',sum(b['images'] for b in banks),'images')
if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--iso',default=DEFAULT_ISO);build(parser.parse_args().iso)
