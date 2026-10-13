"""Local original ordinary-animation pack; no source payload is tracked."""
from pathlib import Path
import struct
from character_bake import geometry,u
from character_animation_data import animation


def compile_animation_pack(models,output,texture_cell):
    with open(output,'wb') as file:
        file.write(struct.pack('<8sI',b'PMANIM3\0',sum(len(names) for _,_,names in models)))
        for model,tpl,names in models:
            raw=Path(model).read_bytes()
            for draw in range(u(raw,0x138)):
                offset=u(raw,0x19c)+draw*108
                if u(raw,offset)!=1 or raw[offset+48]!=0:
                    raise ValueError('Ordinary draw needs explicit coordinate-selector services')
            for name in names:
                anim=animation(raw,name)
                if any(s or t for key in anim['frames'] for _,_,s,t in key['texture']):
                    raise ValueError('Ordinary animated texture transform domain changed')
                if any(row!=[0,0,1,1,0] for row in anim['textures']):
                    raise ValueError('Ordinary base texture transform domain changed')
                # Keep shapes hidden at frame0: later original keys may show them.
                vertices=[v for _,tri in geometry(model,name,bindings=True,include_hidden=bool(anim['stats']['visibility'])) for v in tri]
                used={v[3] for v in vertices}
                if -1 in used:raise ValueError('Untextured animated draw needs color services')
                palettes=[[0xffff]*256 for _ in anim['texture_indices']]
                states=list(anim['texture_indices'])
                def bind():
                    for binding in used:
                        index=u(raw,u(raw,0x194)+binding*8)+states[binding]
                        if index>=u(raw,0x134):index=0
                        tid=u(raw,u(raw,0x198)+index*64+4)
                        palettes[binding][states[binding]]=texture_cell(model,tpl,tid)
                bind()
                for key in anim['frames']:
                    for binding,delta,_,_ in key['texture']:
                        states[binding]=(states[binding]+delta)&255
                    bind()
                groups=[]
                for group in range(u(raw,0x144)):
                    g=u(raw,0x1a8)+group*88
                    sibling,child,_,visibility,node,joint=struct.unpack_from('>iiiIII',raw,g+64)
                    groups.append((sibling,child,node,joint,visibility))
                file.write(struct.pack('<32sIff8I',name.encode(),anim['loop'],anim['start'],anim['end'],
                                       len(anim['positions']),len(anim['nodes']),len(groups),len(vertices),
                                       len(palettes),len(anim['frames']),len(anim['normals']),len(anim['visibility'])))
                for pos in anim['positions']:file.write(struct.pack('<3f',*pos))
                for normal in anim['normals']:file.write(struct.pack('<3f',*normal))
                file.write(struct.pack('<'+'f'*len(anim['nodes']),*anim['nodes']))
                file.write(bytes(anim['visibility']))
                for group in groups:file.write(struct.pack('<iiIII',*group))
                for group,pos,uv,binding in vertices:
                    if not all(0<=float(v)<=1 for v in uv):raise ValueError('Animated UV wrap service required')
                    file.write(struct.pack('<IIIff',group,pos,binding,*uv))
                for state,palette in zip(anim['texture_indices'],palettes):
                    file.write(struct.pack('<I256H',state,*palette))
                for key in anim['frames']:
                    file.write(struct.pack('<f5I',key['time'],len(key['position']),len(key['node']),len(key['texture']),len(key['normal']),len(key['visibility'])))
                    for index,delta in key['position']:file.write(struct.pack('<I3bB',index,*delta,0))
                    for index,delta in key['normal']:file.write(struct.pack('<I3bB',index,*delta,0))
                    for record in key['node']:file.write(struct.pack('<I3bB',*record,0))
                    for index,delta,_,_ in key['texture']:file.write(struct.pack('<IB3x',index,delta))
                    for index,delta in key['visibility']:file.write(struct.pack('<IB3x',index,delta))
