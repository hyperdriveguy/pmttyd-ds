#!/usr/bin/env python3
"""Import original setup2 foreground hierarchy once into an EMPTY editable path."""
import argparse
import hashlib
import json
from pathlib import Path
import tempfile
from asset_pipeline import write_json
from harbor_tutorial_stage_assets import selected_stage
from iso_assets import DEFAULT_ISO,extract
from map_convert import convert
from stage_hierarchy_assets import decode
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SOURCE_SHAS={'stg_00_0':'0c5d16504d645f05d2569b017c6c755bdad8d913482619a6c25aca7d89542b82','bti_01':'213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8'}

def import_hierarchy(iso,destination,template,bank="foreground"):
    destination=Path(destination);template=Path(template)
    if destination.exists() and any(destination.iterdir()):
        raise ValueError('Original hierarchy import requires an empty destination; preserve existing edits')
    with tempfile.TemporaryDirectory() as temporary:
        work=Path(temporary);extract(iso,'rel/gor.rel',work/'gor.rel');selected=selected_stage((work/'gor.rel').read_bytes())
        if bank not in ('foreground','base'):raise ValueError('Unknown original stage bank')
        room=selected['scenery' if bank=='foreground' else 'base'];model=work/(room+'.d')
        extract(iso,'m/'+room+'/d',model)
        if hashlib.sha256(model.read_bytes()).hexdigest()!=SOURCE_SHAS[room]:raise ValueError('Unexpected original stage data')
        roots=tuple(row['name'] for row in selected['objects']);offsets={row['name']:row['nativePosition'] for row in selected['objects']}
        if bank=='foreground':
            convert(model,work/'converted',scene_root=roots,scene_offsets=offsets,map_objects=True)
        else:
            hidden=['escape_wall']+(['if_body','if_wire'] if not any(row['layer']==6 for row in selected['objects']) else [])
            convert(model,work/'converted',hidden=hidden,map_objects=True)
        generated=(work/'converted/map.vertices').read_bytes()
        if generated!=template.read_bytes():raise ValueError('Original hierarchy must match the selected original template vertex count/order/bytes; never overwrite edited geometry')
        raw=(work/'converted/map.mapobj').read_bytes();source=decode(raw)
        names=[row['name'] for row in source['objects']]
        required=roots if bank=='foreground' else ('escape_wall','spotlight')
        if any(names.count(name)!=1 for name in required):raise ValueError('Selected original group missing or ambiguous')
        source['selection']=selected
        provenance=json.loads((work/(room+'.d.provenance.json')).read_text())
        provenance.update(license=LICENSE,selected_rel_sha256=hashlib.sha256((work/'gor.rel').read_bytes()).hexdigest(),
            template_sha256=hashlib.sha256(generated).hexdigest(),hierarchy_pack_sha256=hashlib.sha256(raw).hexdigest(),
            conversion='Original complete node hierarchy/parent order/local affine matrices and original template vertex bindings; exact matching original Harbor '+bank+' geometry selection, no texture or geometry replacement.',
            units='Original local node/vertex coordinates. Existing StageMapObjects applies its documented root*.1/native translation boundary.',
            native_limits='Existing PMMAP001 owner admitted domain: <=1000000 vertices, <=4096 nodes, <64-byte ASCII names, finite float32 matrices/points.',
            decoder='tools/map_convert.py: existing original map decoder adapted from noclip.website/world.ts (MIT); no new asset license')
        destination.mkdir(parents=True,exist_ok=True)
        write_json(destination/(room+'-hierarchy.json'),source)
        write_json(destination/'provenance.json',dict(schema=1,assets={room+'-hierarchy.json':provenance}))
        (destination/'LICENSE.txt').write_text(LICENSE+'.\nOriginal rights retained; tracking a source asset grants no new license.\n')
        return provenance
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);parser.add_argument('--destination',type=Path,required=True);parser.add_argument('--template',type=Path,required=True);parser.add_argument('--bank',choices=['foreground','base'],default='foreground');args=parser.parse_args()
    print(json.dumps(import_hierarchy(args.iso,args.destination,args.template,args.bank),indent=2))
