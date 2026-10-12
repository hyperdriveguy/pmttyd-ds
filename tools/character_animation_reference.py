#!/usr/bin/env python3
"""Generate ignored original-animation test samples with the independent verifier.
Reads local original models/manifest only; does not execute or change the ISO.
"""
import argparse
import json
from pathlib import Path
import struct
from character_bake import geometry
from verify_overworld_animation_updates import animation,evaluate


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',required=True)
    args=parser.parse_args()
    manifest=json.loads(Path('romfs/manifest.json').read_text())['character_mesh']
    cells={(r['source'],r['tpl_index']):r['cell'] for r in manifest['texture_cells']}
    cases=[]
    for entry in manifest['poses']:
        model=Path(entry['model']);name=entry['animation'];tpl=entry['texture_source']
        parsed=animation(model.read_bytes(),name)
        times={0,.25,1.5,parsed['end']/2,parsed['end']-.25,parsed['end'],parsed['end']+.25}
        # Texture steps include fractional original rear-run boundaries.
        for key in parsed['frames']:
            if key['texture']:
                times.update((max(0,key['time']-.01),key['time'],min(parsed['end'],key['time']+.01)))
        for time in sorted(times):
            time=struct.unpack('<f',struct.pack('<f',time))[0]
            vertices=[]
            for tid,tri in geometry(model,evaluated=evaluate(parsed,time)):
                cell=cells[(tpl,tid)]
                for point,uv in tri:
                    vertices.append((*point,(cell%8*128+2+uv[0]*124)/1024,1-(cell//8*128+2+uv[1]*124)/1024))
            cases.append((name,time,vertices))
    output=Path(args.output);output.parent.mkdir(parents=True,exist_ok=True)
    with output.open('wb') as file:
        file.write(struct.pack('<8sI',b'PMREF1\0\0',len(cases)))
        for name,time,vertices in cases:
            file.write(struct.pack('<32sfI',name.encode(),time,len(vertices)))
            for vertex in vertices:file.write(struct.pack('<5f',*vertex))
    print(f'Wrote {len(cases)} independent original-animation samples to {output}')


if __name__=='__main__':main()
