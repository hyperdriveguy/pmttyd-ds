#!/usr/bin/env python3
"""Compile exact original dmo marketplace visibility subtrees, read-only.

Scene event group1 is the late wall reveal, not a persistent story group.
"""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from map_convert import convert

PERMANENT_HIDDEN=('S_ie_01_naka','S_ie_02_f1_naka','S_ie_02_f2_naka','S_ie_03_naka','S_ie_04_naka')
WALL_ROOTS=('S_gai_kabe_k','S_gai_kabe_k_01')
EVENT_GROUPS={name:1 for name in WALL_ROOTS}
PREFIX='opening-demo-market'


def nodes(path):
    raw=Path(path).read_bytes();base=32
    def u(offset):return struct.unpack_from('>I',raw,offset)[0]
    def string(offset):return raw[offset:raw.index(bytes(1),offset)].decode('ascii')
    table=base+u(4)+u(8)*4;names=table+u(12)*8
    chunks={string(names+u(table+n*8+4)):base+u(table+n*8) for n in range(u(12))}
    result={}
    def walk(offset,parent=None,depth=0):
        if depth>100 or offset-base in result:raise ValueError('Invalid source map hierarchy')
        result[offset-base]=dict(name=string(base+u(offset)),parent=parent,parts=u(offset+92))
        if u(offset+12):walk(base+u(offset+12),offset-base,depth+1)
        if u(offset+16):walk(base+u(offset+16),parent,depth+1)
    walk(base+u(chunks['information']+4))
    return result


def bindings(path):
    graph=nodes(path);result={}
    for name in ('S',*PERMANENT_HIDDEN,*WALL_ROOTS):
        matches=[offset for offset,row in graph.items() if row['name']==name]
        if len(matches)!=1:raise ValueError('Missing/ambiguous original demo subtree: '+name)
        root=matches[0];descendants=[]
        for offset,row in graph.items():
            ancestor=offset
            while ancestor is not None and ancestor!=root:ancestor=graph[ancestor]['parent']
            if ancestor==root:descendants.append(offset)
        result[name]=dict(offset=root,descendants=descendants,mesh_nodes=sum(bool(graph[n]['parts']) for n in descendants))
    return result


def complete(out):
    files=[out/(PREFIX+'.'+suffix) for suffix in ['vertices','batches','uvanim','meshanim']]
    if not all(p.is_file() for p in files):return False
    data=files[1].read_bytes()
    if len(data)<8:return False
    count,textures=struct.unpack_from('<II',data)
    return len(data)==8+count*24 and textures<=256 and all((out/f'{PREFIX}-{i}.t3x').is_file() for i in range(textures))


def build(iso,out):
    out=Path(out);out.mkdir(parents=True,exist_ok=True);work=Path('local-assets/demo-map');work.mkdir(parents=True,exist_ok=True)
    source=Path(iso);helpers=['map_convert.py','map_animation.py','character_bake.py','character_node.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',h).read_bytes() for h in helpers)+str((source.stat().st_size,source.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/(PREFIX+'.json')
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and complete(out):return
    model=work/'gor_01.d';extract(iso,'m/gor_01/d',model)
    bound=bindings(model)
    expected={'S':0x158128,'S_ie_01_naka':0x15c938,'S_ie_02_f1_naka':0x161fa0,'S_ie_02_f2_naka':0x165648,'S_ie_03_naka':0x1687b0,'S_ie_04_naka':0x16e600,'S_gai_kabe_k':0x159440,'S_gai_kabe_k_01':0x159bf8}
    if {name:row['offset'] for name,row in bound.items()}!=expected:raise ValueError('Original demo map source bindings changed')
    info=convert(model,work/'map',hidden=PERMANENT_HIDDEN,event_groups=EVENT_GROUPS)
    for suffix in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(work/'map'/('map.'+suffix),out/(PREFIX+'.'+suffix))
    tpl=work/'gor_01.tpl';extract(iso,'m/gor_01/t',tpl)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for i,_ in enumerate(info['textures']):
        image_path=work/f'texture-{i}.png';convert_tpl(tpl,image_path,i)
        image=Image.open(image_path).convert('RGBA');w,h=image.size
        image.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(image_path)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'{PREFIX}-{i}.t3x'),str(image_path)],check=True,stdout=subprocess.DEVNULL)
    batches=(out/(PREFIX+'.batches')).read_bytes();count=struct.unpack_from('<I',batches)[0]
    walls=[i for i in range(count) if struct.unpack_from('<I',batches,8+i*24+12)[0]>>26==1]
    if not walls:raise ValueError('Source wall subtree has no packed geometry')
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,sources=[json.loads(model.with_suffix('.d.provenance.json').read_text()),json.loads(tpl.with_suffix('.tpl.provenance.json').read_text())],source_bindings=bound,wall_batches=walls,vertices=info['vertices'],textures=len(info['textures']),groups={'0':'ordinary demo world','1':'late source wall reveal'},permanent_hidden=list(PERMANENT_HIDDEN),limitation='Original requested subtrees and event batch partition; renderer must use frontend scene predicate rather than field story group policy. Original paper/effects/background/material/runtime remain separate.',license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights'),indent=2)+'\n')
    print(f'Original demo visibility pack: {info["vertices"]} vertices, {len(walls)} wall batches, {len(info["textures"])} textures')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
if __name__=='__main__':main()
