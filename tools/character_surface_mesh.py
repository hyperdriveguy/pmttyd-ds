"""Retain original single-texture/vertex-color surfaces in PMANIM4.

Sampler-aware atlas borders support original clamp/repeat/mirror coordinates.
CPU surface subdivision is a native atlas adapter, not original geometry data.
"""
from pathlib import Path
import numpy as np
from PIL import Image
from iso_assets import convert_tpl
from character_animation_pack import compile_animation_pack


def border_indices(size,wrap):
    indices=np.arange(-2,size+2)
    if wrap==0:return np.clip(indices,0,size-1)
    if wrap==1:return indices%size
    if wrap==2:
        q=indices%(2*size)
        return np.where(q<size,q,2*size-1-q)
    raise ValueError('Unsupported original wrap mode')


def compile_surface_mesh(models,output,atlas_path):
    atlas=Image.new('RGBA',(1024,1024));cells={}
    def texture_cell(model,tpl,tid,wrap):
        key=(str(tpl),tid,tuple(wrap))
        if key not in cells:
            cell=len(cells)
            if cell>=64:raise ValueError('Surface texture atlas capacity exceeded')
            cells[key]=cell
            if tid<0:image=Image.new('RGBA',(124,124),(255,255,255,255))
            else:
                path=Path(atlas_path).parent/f'surface-{Path(model).name}-{tid}.png'
                convert_tpl(tpl,path,tid)
                image=Image.open(path).convert('RGBA').resize((124,124),Image.Resampling.LANCZOS)
            pixels=np.asarray(image)
            x=border_indices(124,wrap[0]);y=border_indices(124,wrap[1])
            tile=Image.fromarray(pixels[y[:,None],x[None,:]],'RGBA')
            atlas.paste(tile,((cell%8)*128,(cell//8)*128))
        return cells[key]
    compile_animation_pack(models,output,texture_cell,surfaces=True)
    height=128
    while height<((len(cells)+7)//8)*128:height*=2
    atlas.crop((0,0,1024,height)).save(atlas_path)
    return dict(textures=len(cells),uv_scale=1024/height,atlas_height=height,
                poses=[dict(model=str(model),texture_source=str(tpl),animation=pose) for model,tpl,poses in models for pose in poses],
                texture_cells=[dict(source=tpl,tpl_index=tid,wrap_s=wrap[0],wrap_t=wrap[1],cell=cell) for (tpl,tid,wrap),cell in cells.items()],
                conversion='Original geometry/sparse states/RGBA/pass/cull retained; decoded textures resized124px; opposite-edge repeat/mirror or clamp atlas gutters. Untextured source draws use neutral white; no authored art replacement')
