#!/usr/bin/env python3
"""Retain original frame0 character model coordinates in a local mesh pack.
Uses the existing original AnimGroup decoder. Full pose interpolation is separate.
"""
import json
from pathlib import Path
import struct
from PIL import Image
from character_bake import geometry
from iso_assets import convert_tpl


def compile_mesh(models, output, atlas_path):
    atlas=Image.new('RGBA',(1024,1024))
    textures={};poses=[];metadata=[]
    for model,tpl,names in models:
        for name in names:
            triangles=geometry(model,name,0)
            vertices=[]
            for tid,triangle in triangles:
                if tid<0:raise ValueError('Untextured draw needs original vertex-color conversion')
                key=(str(tpl),tid)
                if key not in textures:
                    cell=len(textures)
                    if cell>=64:raise ValueError('Character texture atlas capacity exceeded')
                    textures[key]=cell
                    image_path=Path(atlas_path).parent/f'mesh-{Path(model).name}-{tid}.png'
                    convert_tpl(tpl,image_path,tid)
                    image=Image.open(image_path).convert('RGBA').resize((124,124),Image.Resampling.LANCZOS)
                    x=(cell%8)*128;y=(cell//8)*128
                    # Duplicate edge texels into the2px border; no adjacent-cell
                    # sampling when original UV0/1 meets linear filtering.
                    atlas.paste(image,(x+2,y+2))
                    atlas.paste(image.crop((0,0,1,124)).resize((2,124)),(x,y+2))
                    atlas.paste(image.crop((123,0,124,124)).resize((2,124)),(x+126,y+2))
                    atlas.paste(atlas.crop((x,y+2,x+128,y+3)).resize((128,2)),(x,y))
                    atlas.paste(atlas.crop((x,y+125,x+128,y+126)).resize((128,2)),(x,y+126))
                cell=textures[key]
                for point,uv in triangle:
                    if not all(0<=float(v)<=1 for v in uv):raise ValueError('Original wrap domain requires separate texture service')
                    vertices.append((*map(float,point),(cell%8*128+2+float(uv[0])*124)/1024,
                                     1-(cell//8*128+2+float(uv[1])*124)/1024))
            poses.append((name,vertices))
            axes=list(zip(*(v[:3] for v in vertices)))
            metadata.append({'model':str(model),'animation':name,'frame':0,'triangles':len(triangles),
                             'bounds':[[min(v) for v in axes],[max(v) for v in axes]]})
    with open(output,'wb') as file:
        file.write(struct.pack('<8sI',b'PMCHAR1\0',len(poses)))
        for name,vertices in poses:
            encoded=name.encode('ascii')
            if len(encoded)>=32:raise ValueError('Pose name exceeds pack field')
            file.write(struct.pack('<32sI',encoded,len(vertices)))
            for vertex in vertices:file.write(struct.pack('<5f',*vertex))
    atlas.save(atlas_path)
    return {'poses':metadata,'textures':len(textures),'units':'unscaled original local model units',
            'conversion':'frame0 original indexed geometry; original textures resized to124px with2px border; interpolation unbound'}
