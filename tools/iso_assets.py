#!/usr/bin/env python3
"""Read-only GameCube ISO inventory/extraction and standalone TPL conversion.
No game code is imported or executed. Output is always separate from the ISO.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import struct

DEFAULT_ISO = '/home/carson/games/roms/G8ME01.iso'

def u32(data, offset):
    return struct.unpack_from('>I', data, offset)[0]

def inventory(path):
    path = Path(path)
    size = path.stat().st_size
    with path.open('rb') as f:
        header = f.read(0x440)
        if len(header) != 0x440 or u32(header, 0x1c) != 0xc2339f3d:
            raise ValueError('Not a GameCube disc image')
        offset, length = u32(header,0x424), u32(header,0x428)
        if length < 12 or length > 32*1024*1024 or offset+length > size:
            raise ValueError('Invalid filesystem table bounds')
        f.seek(offset)
        fst = f.read(length)
    count = u32(fst,8)
    if not 1 <= count <= length//12 or fst[0] != 1:
        raise ValueError('Invalid FST root')
    strings = fst[count*12:]
    files, stack = [], [(count, '')]
    for index in range(1,count):
        while stack and index >= stack[-1][0]:
            stack.pop()
        if not stack:
            raise ValueError('Invalid directory range')
        word, start, end = struct.unpack_from('>III',fst,index*12)
        kind, nameoff = word>>24, word&0xffffff
        if kind not in (0,1) or nameoff >= len(strings):
            raise ValueError('Invalid FST entry')
        terminator = strings.find(b'\0',nameoff)
        if terminator < 0:
            raise ValueError('Unterminated FST name')
        name = strings[nameoff:terminator].decode('ascii',errors='strict')
        if not name or '/' in name or '\\' in name or name in ('.','..'):
            raise ValueError('Unsafe FST name')
        rel = str(PurePosixPath(stack[-1][1])/name)
        if kind:
            if not index < end <= stack[-1][0]:
                raise ValueError('Invalid FST directory')
            stack.append((end,rel))
        else:
            if start+end > size:
                raise ValueError('File outside ISO')
            files.append({'path':rel,'offset':start,'size':end})
    return {'disc_id':header[:6].decode('ascii'), 'disc_size':size,
            'title':header[0x20:0x400].split(b'\0')[0].decode('ascii'),
            'fst_offset':offset,'files':files}

def extract(iso, name, output):
    info = inventory(iso)
    matches = [x for x in info['files'] if x['path'] == name]
    if len(matches) != 1:
        raise ValueError('Choose one exact inventory path')
    output = Path(output)
    if output.resolve() == Path(iso).resolve():
        raise ValueError('Output must not overwrite ISO')
    item = matches[0]
    output.parent.mkdir(parents=True,exist_ok=True)
    digest = hashlib.sha256()
    with Path(iso).open('rb') as f, output.open('wb') as out:
        f.seek(item['offset'])
        remaining = item['size']
        while remaining:
            block = f.read(min(remaining,1024*1024))
            if not block:
                raise ValueError('Truncated ISO')
            out.write(block)
            digest.update(block)
            remaining -= len(block)
    manifest = {'source_disc':info['disc_id'],'source_path':name,
                'source_offset':item['offset'],'size':item['size'],
                'sha256':digest.hexdigest(),'license':'Local original-game asset; not a replacement license'}
    output.with_suffix(output.suffix+'.provenance.json').write_text(json.dumps(manifest,indent=2)+'\n')

def rgb565(v):
    return ((v>>11)*255//31, ((v>>5)&63)*255//63, (v&31)*255//31,255)

def rgb5a3(v):
    if v&0x8000:
        return (((v>>10)&31)*255//31,((v>>5)&31)*255//31,(v&31)*255//31,255)
    return (((v>>8)&15)*17,((v>>4)&15)*17,(v&15)*17,((v>>12)&7)*255//7)

def decode_gx(data, width, height, fmt, palette=None):
    """Decode tiled GX formats, including CMPR, into RGBA pixels."""
    blocks={0:(8,8,32),1:(8,4,32),2:(8,4,32),3:(4,4,32),4:(4,4,32),
            5:(4,4,32),6:(4,4,64),8:(8,8,32),9:(8,4,32),10:(4,4,32),14:(8,8,32)}
    if fmt not in blocks:
        raise ValueError(f'Unsupported GX format {fmt}')
    if width < 1 or height < 1 or width > 4096 or height > 4096:
        raise ValueError('Invalid texture dimensions')
    bw,bh,bs=blocks[fmt]
    required=((width+bw-1)//bw)*((height+bh-1)//bh)*bs
    if len(data)<required:
        raise ValueError('Truncated texture')
    result=[(0,0,0,0)]*(width*height)
    offset=0
    for by in range(0,height,bh):
        for bx in range(0,width,bw):
            block=data[offset:offset+bs]
            offset+=bs
            for y in range(bh):
                for x in range(bw):
                    n=y*bw+x
                    if fmt in (0,8):
                        val=(block[n//2]>>(4 if n%2==0 else 0))&15
                        pixel=(val*17,)*3+(255,) if fmt==0 else palette_pixel(palette,val)
                    elif fmt in (1,9):
                        val=block[n]
                        pixel=(val,val,val,255) if fmt==1 else palette_pixel(palette,val)
                    elif fmt==2:
                        v=block[n];pixel=((v&15)*17,)*3+((v>>4)*17,)
                    elif fmt==3:
                        a,i=block[n*2:n*2+2];pixel=(i,i,i,a)
                    elif fmt in (4,5,10):
                        v=struct.unpack_from('>H',block,n*2)[0]
                        pixel=rgb565(v) if fmt==4 else rgb5a3(v) if fmt==5 else palette_pixel(palette,v&0x3fff)
                    elif fmt==6:
                        a,r=block[n*2:n*2+2];g,b=block[32+n*2:34+n*2];pixel=(r,g,b,a)
                    else:
                        sub=((y//4)*2+x//4)*8
                        c0,c1=struct.unpack_from('>HH',block,sub)
                        a,b=rgb565(c0),rgb565(c1)
                        if c0>c1:
                            colors=[a,b,tuple((2*a[i]+b[i])//3 for i in range(3))+(255,),tuple((a[i]+2*b[i])//3 for i in range(3))+(255,)]
                        else:
                            colors=[a,b,tuple((a[i]+b[i])//2 for i in range(3))+(255,),(0,0,0,0)]
                        idx=(block[sub+4+y%4]>>(6-2*(x%4)))&3
                        pixel=colors[idx]
                    if bx+x<width and by+y<height:
                        result[(by+y)*width+bx+x]=pixel
    return result

def palette_pixel(palette,index):
    if palette is None or index>=len(palette):
        raise ValueError('Missing or invalid palette')
    return palette[index]

def convert_tpl(path, output, index=0):
    from PIL import Image
    raw=Path(path).read_bytes()
    if len(raw)<12 or u32(raw,0)!=0x0020af30:
        raise ValueError('Not a standalone TPL container')
    count,table=u32(raw,4),u32(raw,8)
    if index<0 or index>=count or table+count*8>len(raw):
        raise ValueError('Invalid TPL table/index')
    imageoff,paloff=struct.unpack_from('>II',raw,table+index*8)
    if imageoff+36>len(raw):
        raise ValueError('Invalid TPL image header')
    h,w,fmt,offset=struct.unpack_from('>HHII',raw,imageoff)
    palette=None
    if paloff:
        if paloff+12>len(raw): raise ValueError('Invalid palette header')
        entries=struct.unpack_from('>H',raw,paloff)[0]
        pf,po=struct.unpack_from('>II',raw,paloff+4)
        if po+entries*2>len(raw) or pf>2: raise ValueError('Invalid palette')
        palette=[]
        for n in range(entries):
            v=struct.unpack_from('>H',raw,po+n*2)[0]
            palette.append(((v&255,)*3+(v>>8,)) if pf==0 else rgb565(v) if pf==1 else rgb5a3(v))
    pixels=decode_gx(raw[offset:],w,h,fmt,palette)
    out=Path(output);out.parent.mkdir(parents=True,exist_ok=True)
    image=Image.new('RGBA',(w,h));image.putdata(pixels);image.save(out)
    out.with_suffix(out.suffix+'.provenance.json').write_text(json.dumps({
        'source':str(path),'sha256':hashlib.sha256(raw).hexdigest(),'index':index,
        'gx_format':fmt,'dimensions':[w,h],'conversion':'GX tiled to RGBA; no resize',
        'license':'Inherited from source; local original assets excluded from source history'},indent=2)+'\n')

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    sub=parser.add_subparsers(dest='command',required=True)
    inv=sub.add_parser('inventory');inv.add_argument('iso',nargs='?',default=DEFAULT_ISO);inv.add_argument('--output',required=True)
    ext=sub.add_parser('extract');ext.add_argument('iso');ext.add_argument('path');ext.add_argument('--output',required=True)
    tpl=sub.add_parser('tpl');tpl.add_argument('input');tpl.add_argument('--output',required=True);tpl.add_argument('--index',type=int,default=0)
    args=parser.parse_args()
    try:
        if args.command=='inventory':
            out=Path(args.output)
            if out.resolve()==Path(args.iso).resolve(): raise ValueError('Output must not overwrite ISO')
            result=inventory(args.iso);out.parent.mkdir(parents=True,exist_ok=True)
            out.write_text(json.dumps(result,indent=2)+'\n');print(f"{result['disc_id']}: {len(result['files'])} files -> {out}")
        elif args.command=='extract': extract(args.iso,args.path,args.output)
        else: convert_tpl(args.input,args.output,args.index)
    except (OSError,ValueError,struct.error) as exc:
        parser.exit(1,f'Asset error: {exc}\n')
if __name__=='__main__': main()
