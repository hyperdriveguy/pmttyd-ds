#!/usr/bin/env python3
"""Import used original message, selection and battle HUD icons unchanged."""
import argparse,json,struct,subprocess,shutil
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from battle_hud_assets import source_art
from battle_weapon_icon_assets import source_art as weapon_art

def build(iso):
    work=Path('local-assets/message-icons');work.mkdir(parents=True,exist_ok=True)
    directory=work/'icon.bin';tpl=work/'icon.tpl';extract(iso,'icon.bin',directory);extract(iso,'icon.tpl',tpl)
    raw=directory.read_bytes();hud=source_art(iso);weapons=weapon_art(iso);icons=[];bitmaps=[]
    identifiers=list(dict.fromkeys([108,109,129,504,524,134,136,115,404,403,307,308,445,446,447,314]+
        [row['icon'] for row in hud['rows']]+hud['large_digits']+hud['small_digits']+weapons['icons']))
    for identifier in identifiers:
        offset=struct.unpack_from('>H',raw,2*(identifier+1))[0];loop,_,_,count=struct.unpack_from('>4H',raw,offset)
        frames=[]
        for i in range(count):
            image,delay=struct.unpack_from('>2H',raw,offset+8+4*i);path=work/f'{image}.png';convert_tpl(tpl,path,image)
            bitmap=Image.open(path).convert('RGBA');w,h=bitmap.size
            frame=dict(tpl=image,delay=delay,width=w,height=h);frames.append(frame);bitmaps.append((frame,bitmap))
        icons.append(dict(id=identifier,loop=loop,descriptor=offset,frames=frames))
    # Resource namespace10000 separates shared effect IDs from actual icon IDs.
    effect=work/'effect_us.tpl';extract(iso,'e/us/effect_us.tpl',effect)
    for identifier in range(124,132):
        path=work/f'effect-{identifier}.png';convert_tpl(effect,path,identifier-118)
        bitmap=Image.open(path).convert('RGBA');w,h=bitmap.size
        frame=dict(tpl=identifier-118,delay=0,width=w,height=h);bitmaps.append((frame,bitmap))
        icons.append(dict(id=0x10000+identifier,source_effect_id=identifier,loop=0,frames=[frame]))
    # Several source animation cells have identical pixels. Share their atlas
    # storage while retaining every descriptor frame and its original delay.
    unique={};aliases={}
    for frame,bitmap in bitmaps:
        key=(bitmap.size,bitmap.tobytes())
        if key not in unique:unique[key]=(frame,bitmap);aliases[id(frame)]=[]
        aliases[id(unique[key][0])].append(frame)
    bitmaps=list(unique.values())
    # Pack the original cells into the existing1MiB allocation. Splitting
    # intersecting free rectangles avoids wasted shelf space for the wide bars.
    atlas=Image.new('RGBA',(512,512));free=[(0,0,512,512)]
    for frame,bitmap in sorted(bitmaps,key=lambda entry:-entry[1].width*entry[1].height):
        w,h=bitmap.size;pw,ph=w+4,h+4
        candidates=[r for r in free if r[2]>=pw and r[3]>=ph]
        if not candidates:raise ValueError('Used original UI atlas capacity exceeded')
        x,y,_,_=min(candidates,key=lambda r:(min(r[2]-pw,r[3]-ph),max(r[2]-pw,r[3]-ph),r[1],r[0]))
        next_free=[]
        for fx,fy,fw,fh in free:
            if x>=fx+fw or x+pw<=fx or y>=fy+fh or y+ph<=fy:
                next_free.append((fx,fy,fw,fh));continue
            if x>fx:next_free.append((fx,fy,x-fx,fh))
            if x+pw<fx+fw:next_free.append((x+pw,fy,fx+fw-x-pw,fh))
            if y>fy:next_free.append((fx,fy,fw,y-fy))
            if y+ph<fy+fh:next_free.append((fx,y+ph,fw,fy+fh-y-ph))
        unique=list(dict.fromkeys(next_free))
        free=[r for r in unique if not any(r!=q and q[0]<=r[0] and q[1]<=r[1] and
            r[0]+r[2]<=q[0]+q[2] and r[1]+r[3]<=q[1]+q[3] for q in unique)]
        atlas.paste(bitmap,(x+2,y+2))
        for alias in aliases[id(frame)]:alias['uv']=((x+2)/512,1-(y+2)/512,(x+2+w)/512,1-(y+2+h)/512)
    payload=bytearray()
    for icon in icons:
        payload+=struct.pack('<3I',icon['id'],icon['loop'],len(icon['frames']))
        for frame in icon['frames']:
            payload+=struct.pack('<3I4f',frame['delay'],frame['width'],frame['height'],*frame['uv'])
    atlas.save(work/'atlas.png');Path('romfs/message-icons.bin').write_bytes(struct.pack('<8sI',b'PMICON1',len(icons))+payload)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o','romfs/message-icons.t3x',str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    Path('romfs/message-icons.json').write_text(json.dumps(dict(source=['icon.bin','icon.tpl','e/us/effect_us.tpl'],icons=icons,battle_hud=hud,battle_weapons=weapons,
        atlas_dimensions=[512,512],conversion='Original pixels; identical pixel cells share storage; rectangle packing with2px transparent padding; no resampling',license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
