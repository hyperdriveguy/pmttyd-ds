#!/usr/bin/env python3
"""Import the original OFF_d_saku deformation; captured map materials stay native."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from character_mesh import compile_mesh
def build(iso):
    work=Path('local-assets/professor-fence');work.mkdir(parents=True,exist_ok=True)
    model=work/'OFF_d_saku';tpl=work/'OFF_d_saku.tpl'
    extract(iso,'a/OFF_d_saku',model);extract(iso,'a/OFF_d_saku-',tpl)
    meta=compile_mesh([(model,tpl,['A_1'])],'romfs/professor-fence.pmmesh',work/'atlas.png','romfs/professor-fence.pmanim')
    summary=json.loads(Path('local-assets/gor_02/map-summary.json').read_text())
    nodes=summary['collision_nodes'];root=next(n['source_node'] for n in nodes if n['name']=='A_saku_04')
    removed={root}
    while True:
        added={n['source_node'] for n in nodes if n['parent'] in removed}-removed
        if not added:break
        removed|=added
    Path('romfs/professor-fence.bin').write_bytes(struct.pack('<8sI',b'PMFENCE1',len(removed))+struct.pack('<'+'I'*len(removed),*sorted(removed)))
    meta.update(source='a/OFF_d_saku',sha256=hashlib.sha256(model.read_bytes()).hexdigest(),collision_nodes=sorted(removed),
                capture_policy='Original map group8 materials; handheld geometry deformation adapter, not the placeholder paper texture',
                license='Original Nintendo/Intelligent Systems resources; no redistribution rights')
    Path('romfs/professor-fence.json').write_text(json.dumps(meta,indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
