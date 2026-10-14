#!/usr/bin/env python3
"""Keep the original icon directory/TPL and lossless native cells privately."""
import argparse, hashlib, json, shutil, struct, subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, extract, convert_tpl

SPANS=((0x80415040,24),(0x80415080,32),(0x8041f864,0x3c))

def padded(bitmap, wrap_s, wrap_t):
    width,height=bitmap.size
    size=lambda n: max(8,1<<(n+3).bit_length())
    output=Image.new('RGBA',(size(width),size(height)))
    def source(n,length,wrap):
        if wrap==0:return max(0,min(length-1,n))
        if wrap==1:return n%length
        if wrap==2:
            v=n%(2*length)
            return v if v<length else 2*length-1-v
        raise ValueError('Invalid original wrap')
    for y in range(-2,height+2):
        for x in range(-2,width+2):
            output.putpixel((x+2,y+2),bitmap.getpixel((source(x,width,wrap_s),source(y,height,wrap_t))))
    return output

def build(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US G8ME01 disc0 revision0 required')
    out=Path('romfs/source-icons');work=Path('local-assets/source-icons');out.mkdir(parents=True,exist_ok=True);work.mkdir(parents=True,exist_ok=True)
    for name in ('icon.bin','icon.tpl'):extract(iso,name,out/name)
    with Path(iso).open('rb') as f:
        f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);header=f.read(256)
        def read(address,size):
            for i in range(18):
                offset,start,length=[struct.unpack_from('>I',header,k+4*i)[0] for k in (0,0x48,0x90)]
                if offset and start<=address and address+size<=start+length:
                    f.seek(dol+offset+address-start);return f.read(size)
            raise ValueError('Unmapped original icon constants')
        constants=b''.join(read(a,n) for a,n in SPANS)
    (out/'constants.bin').write_bytes(constants)
    raw=(out/'icon.tpl').read_bytes();magic,count,table=struct.unpack_from('>3I',raw)
    if magic!=0x20af30 or count!=613:raise ValueError('Original US icon TPL required')
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds';rows=[]
    for index in range(count):
        h=struct.unpack_from('>I',raw,table+8*index)[0];height,width,fmt=struct.unpack_from('>2HI',raw,h);ws,wt=struct.unpack_from('>2I',raw,h+12)
        png=work/f'{index}.png';convert_tpl(out/'icon.tpl',png,index)
        image=Image.open(png).convert('RGBA');cell=padded(image,ws,wt);path=work/f'{index}-padded.png';cell.save(path)
        target=out/f'{index}.t3x';subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(target),str(path)],check=True,stdout=subprocess.DEVNULL)
        rows.append(dict(index=index,width=width,height=height,format=fmt,wrap=[ws,wt],native_size=cell.size,rgba_sha256=hashlib.sha256(image.tobytes()).hexdigest(),t3x_sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
    manifest=dict(source=['icon.bin','icon.tpl'],sha256={n:hashlib.sha256((out/n).read_bytes()).hexdigest() for n in ('icon.bin','icon.tpl','constants.bin')},constant_spans=SPANS,images=rows,conversion='Original decoded pixels, no resizing;2px source-wrap-aware gutter; power-of-two RGBA8 cell, lossless tex3ds compression',license='Nintendo/Intelligent Systems; private original assets, no redistribution rights')
    (out/'sources.json').write_text(json.dumps(manifest,indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
