#!/usr/bin/env python3
"""Export the used original OFF_d_meku geometry and dynamic material owners read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from character_animation_pack import compile_animation_pack
from character_animation_data import animation
from character_bake import geometry,u

def build(iso=DEFAULT_ISO,out='romfs'):
    out=Path(out);out.mkdir(exist_ok=True,parents=True);work=Path('local-assets/opening-map-transition');work.mkdir(exist_ok=True,parents=True)
    model=work/'OFF_d_meku';tpl=work/'OFF_d_meku.tpl';extract(iso,'a/OFF_d_meku',model);extract(iso,'a/OFF_d_meku-',tpl)
    raw=model.read_bytes();digest=hashlib.sha256(raw).hexdigest()
    if digest!='8235cf8c1e9f368e0a46d9b81f824cb126aee1b483f3f1ae02f24379bc6729aa':raise ValueError('Original map paper changed')
    for name,end,keys in [('Z_1',0,1),('A_1',80,45)]:
        a=animation(raw,name)
        if (a['start'],a['end'],len(a['frames']),a['loop'])!=(0,end,keys,False) or any(k['texture'] or k['visibility'] for k in a['frames']):raise ValueError('Used original map paper pose changed')
    a=animation(raw,'A_1');kinds=[u(raw,u(raw,0x198)+(u(raw,u(raw,0x194)+b*8)+v)*64+8)for b,v in enumerate(a['texture_indices'])]
    if kinds!=[2,1]:raise ValueError('Map dynamic material ownership changed')
    # Ordinary sparse geometry encoding, with an explicit raw companion. No
    # fallback raster is admitted as the original dynamic capture resource.
    compile_animation_pack([(model,tpl,['Z_1','A_1'])],out/'opening-map-turn.pmanim',lambda m,t,index:index)
    records=[]
    for binding,triangle in geometry(model,'Z_1',bindings=True,surfaces=True):
        mode,cull=triangle[0][5:7]
        if mode!=2 or cull!=1 or any(v[3]!=binding or v[4]!=(255,255,255,255) or v[5:7]!=(mode,cull)for v in triangle):raise ValueError('Map paper surface changed')
        records.append((kinds[binding],mode,cull))
    if [r[0]for r in records]!=[1]*44+[2]*44:raise ValueError('Used inherited sampler draw order changed')
    with (out/'opening-map-turn.bin').open('wb') as f:
        f.write(struct.pack('<8sI',b'PMMAP001',len(records)))
        for row in records:f.write(struct.pack('<3I',*row))
    (out/'opening-map-turn.json').write_text(json.dumps(dict(source='a/OFF_d_meku',texture_descriptor='a/OFF_d_meku-',sha256=digest,poses=['Z_1','A_1'],end=[0,80],keys=[1,45],context=2,capture=[0,0,608,480],materials=kinds,draw_order='44kind1 capture0,44kind2 absent capture1 inherits actual sampler0',conversion='Original sparse hierarchy geometry, UV, RGBA, pass/cull and order; no raster replacement or second capture admission',license='Nintendo/Intelligent Systems resources; local use only, no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
