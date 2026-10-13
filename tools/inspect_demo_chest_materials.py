#!/usr/bin/env python3
"""Reproduce original golden-chest conversion and inspect native cropped UVs.

ISO is read-only; all extracted/compiled reference assets use a temporary dir.
Reports geometry/texture evidence, never original dialogue or event payloads.
"""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
import tempfile
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract
from character_surface_mesh import compile_surface_mesh
from map_convert import convert

ROOT=Path(__file__).resolve().parents[1]
SAMPLE=r'''
#include "actor_animation.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
using namespace ttyd;
int main(int argc,char** argv){assert(argc==2);ActorAnimationPack p;assert(p.load(argv[1]));std::cout<<std::setprecision(9)<<"[";bool comma=false;
for(auto name:{"S_1","A_1"})for(float time:{0.f,1.f,15.f,60.f,300.f}){
 auto pose=p.pose(name);ActorAnimationSample s;assert(pose && pose->evaluate(time,s));float ulo=1,uhi=0,vlo=1,vhi=0;size_t nonwhite=0;
 for(size_t i=0;i<s.vertices.size();++i){auto v=s.vertices[i];ulo=std::min(ulo,v.u);uhi=std::max(uhi,v.u);const float cropv=1-(1-v.v)*8;vlo=std::min(vlo,cropv);vhi=std::max(vhi,cropv);assert(v.u>=0 && v.u<=1 && cropv>=0 && cropv<=1);if(s.colors[i]!=ActorColor{1,1,1,1})++nonwhite;}
 if(comma)std::cout<<",";comma=true;std::cout<<"{\"pose\":\""<<name<<"\",\"time\":"<<time<<",\"vertices\":"<<s.vertices.size()<<",\"u\":["<<ulo<<","<<uhi<<"],\"cropped_v\":["<<vlo<<","<<vhi<<"],\"nonwhite_vertices\":"<<nonwhite<<"}";
}std::cout<<"]\n";}
'''


def inspect(iso,romfs):
    iso=Path(iso);romfs=Path(romfs)
    with iso.open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
    with tempfile.TemporaryDirectory(prefix='pm-demo-chest-') as directory:
        work=Path(directory);model=work/'MOBJ_GoldenTreasureBox';tpl=work/'chest.tpl'
        extract(iso,'a/MOBJ_GoldenTreasureBox',model);extract(iso,'a/MOBJ_GoldenTreasureBox-',tpl)
        raw=model.read_bytes();u=lambda off:struct.unpack_from('>I',raw,off)[0]
        owner=raw[0x44:0x84].split(bytes(1))[0].decode('ascii')
        if owner!='MOBJ_GoldenTreasureBox':raise ValueError('Original texture-bank ownership changed')
        descriptors=[]
        for i in range(u(0x134)):
            offset=u(0x198)+i*64
            descriptors.append(dict(index=i,tpl_index=u(offset+4),kind=u(offset+8),source_name=raw[offset+12:offset+64].split(bytes(1))[0].decode('ascii')))
        animation=work/'chest.pmanim';atlas=work/'atlas.png'
        compiled=compile_surface_mesh([(model,tpl,['S_1','A_1'])],animation,atlas)
        if compiled['uv_scale']!=8:raise ValueError('Scoped original golden-chest crop changed')
        tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
        texture=work/'chest.t3x';subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(texture),str(atlas)],check=True,stdout=subprocess.DEVNULL)
        candidates=[('pmanim',animation.read_bytes()),('t3x',texture.read_bytes()),('uvscale',struct.pack('<f',compiled['uv_scale']))]
        hashes={}
        for suffix,reference in candidates:
            actual=(romfs/('demo-chest.'+suffix)).read_bytes()
            if actual!=reference:raise ValueError('Native chest '+suffix+' disagrees with read-only original conversion')
            hashes[suffix]=hashlib.sha256(actual).hexdigest()
        image=Image.open(atlas).convert('RGBA')
        pixels=image.tobytes();cool=sum(bool(pixels[i+3] and pixels[i+2]>pixels[i]*1.2+10) for i in range(0,len(pixels),4))
        source=work/'inspect.cpp';source.write_text(SAMPLE);binary=work/'inspect'
        subprocess.run([shutil.which('c++') or 'c++','-std=c++17','-I',str(ROOT/'include'),str(source),'-o',str(binary)],check=True)
        samples=json.loads(subprocess.run([str(binary),str(animation)],check=True,capture_output=True,text=True).stdout)
        map_model=work/'gor_01.d';extract(iso,'m/gor_01/d',map_model)
        stock=convert(map_model,work/'stock',scene_root='S_roten')
        used={texture for _,_,texture,_,_,_ in struct.iter_unpack('<IIiIII',(work/'stock'/'map.batches').read_bytes()[8:]) if texture>=0}
        cells=[{k:v for k,v in row.items() if k!='source'} for row in compiled['texture_cells']]
        return dict(source_owner=owner,texture_descriptors=descriptors,atlas=dict(width=image.width,height=image.height,uv_scale=compiled['uv_scale'],texture_cells=cells,blue_dominant_pixels=cool),native_hashes=hashes,samples=samples,original_stock=dict(subtree='S_roten',vertices=stock['vertices'],triangles=stock['triangles'],bounds=stock['bounds'],textures=[stock['textures'][i] for i in sorted(used)]),limitation='Exact read-only original conversion and portable native UV evaluation, not GPU texture-state inspection or original-runtime/hardware image parity. S_roten stock is original map geometry distinct from the golden chest actor.')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--romfs',default='romfs');p.add_argument('--output');a=p.parse_args();report=inspect(a.iso,a.romfs)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():raise ValueError('Cannot overwrite source ISO')
        output.parent.mkdir(parents=True,exist_ok=True);output.write_text(json.dumps(report,indent=2)+'\n')
    print('Original golden chest model/texture ownership, PMANIM4/T3X/crop bytes and ten native UV samples agree')
    print(f'Atlas {report["atlas"]["width"]}x{report["atlas"]["height"]}, scale{report["atlas"]["uv_scale"]}, blue-dominant pixels{report["atlas"]["blue_dominant_pixels"]}')
    print(f'Original S_roten stock bounds: {report["original_stock"]["bounds"]}')
if __name__=='__main__':main()
