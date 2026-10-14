#!/usr/bin/env python3
"""Pin used original Voyage sky light loader/normal requests, read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
MANIFEST=Path(__file__).with_name('opening_sky_light_evidence.json')
def verify(iso=DEFAULT_ISO):
    expected=json.loads(MANIFEST.read_text());info=inventory(iso)
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('Expected original US disc0 revision0')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
        def read(address,size):
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+i*4)[0]for b in(0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    source.seek(dol+off+address-va);return source.read(size)
            raise ValueError('Unmapped DOL evidence')
        for row in expected['bodies']:
            if hashlib.sha256(read(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('Original light/normal body changed')
        for row in expected['anchors']:
            if read(row['address'],len(row['hex'])//2).hex()!=row['hex']:raise ValueError('Original light/normal instruction changed')
        if read(0x802bf900,48).hex()!=expected['coefficients']:raise ValueError('Original light attenuation table changed')
        reset_sites=[]
        for i in range(7):
            off,va,length=(struct.unpack_from('>I',header,b+i*4)[0]for b in(0,0x48,0x90));source.seek(dol+off);text=source.read(length)
            for at in range(0,len(text)-3,4):
                word=struct.unpack_from('>I',text,at)[0]
                if word>>26!=18:continue
                delta=word&0x3fffffc;delta=delta-(1<<26)if delta&(1<<25)else delta
                target=delta if word&2 else va+at+delta
                if target==0x8001c1d8:reset_sites.append(va+at)
        if reset_sites!=expected['reset_direct_sites']:raise ValueError('Original light reset direct-call census changed')
        entry=next(e for e in info['files']if e['path']=='m/muj_20/d');source.seek(entry['offset']);raw=source.read(entry['size'])
        if hashlib.sha256(raw).hexdigest()!=expected['map_sha256']:raise ValueError('Original Voyage map changed')
        u=lambda o:struct.unpack_from('>I',raw,o)[0]
        animation=expected['light_animation'];entry=animation['entry'];color=animation['color_table']
        if struct.unpack_from('>f',raw,entry+8)[0]!=250 or u(entry+20)!=0 or 32+u(entry+24)!=animation['position_table'] or 32+u(entry+28)!=color or u(color)!=9:raise ValueError('Original day light-track table layout changed')
        scale=expected['rgb_scale_constant']
        if read(scale['address'],4).hex()!=scale['hex'] or struct.unpack('>f',bytes.fromhex(scale['hex']))[0]!=255:raise ValueError('Original light RGB scale changed')
        for track in animation['tracks']:
            at=track['offset'];name_at=32+u(at);name=raw[name_at:raw.index(b'\0',name_at)].decode('ascii');count=u(at+4)
            times=[struct.unpack_from('>f',raw,at+8+i*104)[0]for i in range(count)]
            if name!=track['name'] or times!=track['frames']:raise ValueError('Original day light color key times changed')
            final=at+8+(count-1)*104;values=[struct.unpack_from('>f',raw,final+4+i*20)[0]for i in range(5)]
            rgb=[int(struct.unpack('>f',struct.pack('>f',v*255))[0])&255 for v in values[:3]]+[255]
            if rgb!=track['final_rgba'] or values[3]!=track['final_intensity']:raise ValueError('Original day light endpoint publication changed')
        node=0x87500;control=32+u(node+88)
        if raw[32+u(node):32+u(node)+8]!=b'S_hai01\0' or raw[control+2]&8 or u(control+4)!=0x100:raise ValueError('Original named sky-light selection changed')
        light=0x884ec
        if raw[light+44:light+48]!=bytes.fromhex('141e57ff') or struct.unpack_from('>2f3I',raw,light+48)!=(0.,1.5,0,0,0x012d60d8):raise ValueError('Original pointLight9 fields changed')
        mesh=32+u(node+100);vcd=32+u(mesh+12);normal=32+u(vcd+4)+4;dl=32+u(mesh+16)
        indices=struct.unpack_from('>3H',raw,dl+3)
        if indices!=(7278,213,4) or struct.unpack_from('>3b',raw,normal+3*indices[1])!=(0,0,-64):raise ValueError('Original used sky normal changed')
        entry=next(e for e in info['files']if e['path']=='rel/muj.rel');source.seek(entry['offset']);rel=source.read(entry['size'])
        alpha=expected['alpha_rel']
        if hashlib.sha256(rel).hexdigest()!=alpha['sha256']:raise ValueError('Original muj REL changed')
        for row in alpha['bodies']:
            if hashlib.sha256(rel[row['offset']:row['offset']+row['size']]).hexdigest()!=row['sha256']:raise ValueError('Original alpha producer/attachment event changed')
        urel=lambda o:struct.unpack_from('>I',rel,o)[0]
        sections=[(urel(urel(16)+8*i)&~3,urel(urel(16)+8*i+4))for i in range(urel(12))]
        targets={(6,0x20),(1,0x5218),(1,0x5254),(1,0x5298),(5,0x1b778),(5,0x1b81c)};references=[]
        for p in range(urel(40),urel(40)+urel(44),8):
            module,pos=struct.unpack_from('>II',rel,p);section=None;offset=0
            while True:
                delta,kind,target,addend=struct.unpack_from('>HBBI',rel,pos);pos+=8
                if kind==203:break
                if kind==202:section=target;offset=0;continue
                offset+=delta
                if module==urel(0)and(target,addend)in targets:references.append(dict(site=sections[section][0]+offset,kind=kind,target=target,addend=addend))
        if references!=alpha['references']:raise ValueError('Original alpha BSS/callback/producer complete reference census changed')
    return dict(day_point9_rgba=animation['tracks'][1]['final_rgba'],day_point9_intensity=animation['tracks'][1]['final_intensity'],alpha_bodies=len(alpha['bodies']),alpha_references=len(references),bodies=len(expected['bodies']),anchors=len(expected['anchors']),normal_format='S8/64, stride3',point9_angular=[1.5,0,0],point9_distance=[1,0,0],scope='Read-only source requests and original resources; no original execution/hardware equation proof')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print(json.dumps(verify(a.iso),indent=2))
