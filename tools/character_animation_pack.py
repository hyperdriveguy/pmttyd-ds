"""Local original ordinary-animation pack; no source payload is tracked."""
from pathlib import Path
import struct
from character_bake import geometry,u
from character_animation_data import animation


def compile_animation_pack(models,output,texture_cell,surfaces=False):
    with open(output,'wb') as file:
        file.write(struct.pack('<8sI',b'PMANIM4\0' if surfaces else b'PMANIM3\0',sum(len(names) for _,_,names in models)))
        for model,tpl,names in models:
            raw=Path(model).read_bytes()
            for draw in range(u(raw,0x138)):
                offset=u(raw,0x19c)+draw*108
                count=u(raw,offset)
                if (count not in (0,1) if surfaces else count!=1) or (count and raw[offset+48]!=0) or (surfaces and u(raw,offset+8)!=0):
                    raise ValueError('Ordinary draw needs explicit coordinate-selector services')
            for name in names:
                anim=animation(raw,name)
                if not surfaces and any(s or t for key in anim['frames'] for _,_,s,t in key['texture']):
                    raise ValueError('Ordinary animated texture transform domain changed')
                if not surfaces and any(row!=[0,0,1,1,0] for row in anim['textures']):
                    raise ValueError('Ordinary base texture transform domain changed')
                # Keep shapes hidden at frame0: later original keys may show them.
                vertices=[v for _,tri in geometry(model,name,bindings=True,include_hidden=bool(anim['stats']['visibility']),surfaces=surfaces) for v in tri]
                used={v[3] for v in vertices}
                if -1 in used and not surfaces:raise ValueError('Untextured animated draw needs color services')
                if surfaces and -1 in used:
                    white=len(anim['texture_indices']);anim['texture_indices'].append(0);anim['textures'].append([0,0,1,1,0])
                    vertices=[(*v[:3],white,*v[4:]) if v[3]==-1 else v for v in vertices]
                    used.remove(-1);used.add(white)
                else:white=None
                palettes=[[0xffff]*256 for _ in anim['texture_indices']]
                states=list(anim['texture_indices'])
                wraps=[None for _ in states]
                tpl_raw=Path(tpl).read_bytes() if surfaces else b''
                def bind():
                    for binding in used:
                        if binding==white:
                            wraps[binding]=(0,0);palettes[binding][0]=texture_cell(model,tpl,-1,(0,0));continue
                        index=u(raw,u(raw,0x194)+binding*8)+states[binding]
                        if index>=u(raw,0x134):index=0
                        tid=u(raw,u(raw,0x198)+index*64+4)
                        if surfaces:
                            if u(raw,u(raw,0x198)+index*64+8)!=0:raise ValueError('Surface texture kind requires another material service')
                            override=struct.unpack_from('>i',raw,u(raw,0x194)+binding*8+4)[0]
                            if override==-1:
                                image=u(tpl_raw,u(tpl_raw,8)+tid*8);wrap=struct.unpack_from('>II',tpl_raw,image+12)
                            else:wrap=(2 if override&4 else override&1,2 if override&8 else (override>>1)&1)
                            if any(v>2 for v in wrap) or (wraps[binding] is not None and wraps[binding]!=wrap):raise ValueError('Changing source wrap modes require per-state sampler ownership')
                            wraps[binding]=wrap
                            palettes[binding][states[binding]]=texture_cell(model,tpl,tid,wrap)
                        else:palettes[binding][states[binding]]=texture_cell(model,tpl,tid)
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
                for vertex in vertices:
                    group,pos,uv,binding=vertex[:4]
                    if not surfaces and not all(0<=float(v)<=1 for v in uv):raise ValueError('Animated UV wrap service required')
                    file.write(struct.pack('<IIIff',group,pos,binding,*uv))
                for state,palette in zip(anim['texture_indices'],palettes):
                    file.write(struct.pack('<I256H',state,*palette))
                if surfaces:
                    for transform,wrap in zip(anim['textures'],wraps):file.write(struct.pack('<5f2I',*transform,*(wrap or (0,0))))
                    for vertex in vertices:file.write(bytes(vertex[4]))
                    for i in range(0,len(vertices),3):
                        group=vertices[i][0];binding=vertices[i][3];mode,cull=vertices[i][5:7]
                        if mode not in (0,2,3) or cull>3:raise ValueError('Unsupported original surface pass/cull')
                        if any(v[0]!=group or v[3]!=binding or v[5:7]!=(mode,cull) for v in vertices[i:i+3]):raise ValueError('Inconsistent triangle material owner')
                        file.write(struct.pack('<2I',2 if mode==0 else mode,cull))
                for key in anim['frames']:
                    file.write(struct.pack('<f5I',key['time'],len(key['position']),len(key['node']),len(key['texture']),len(key['normal']),len(key['visibility'])))
                    for index,delta in key['position']:file.write(struct.pack('<I3bB',index,*delta,0))
                    for index,delta in key['normal']:file.write(struct.pack('<I3bB',index,*delta,0))
                    for record in key['node']:file.write(struct.pack('<I3bB',*record,0))
                    for index,delta,_,_ in key['texture']:file.write(struct.pack('<IB3x',index,delta))
                    for index,delta in key['visibility']:file.write(struct.pack('<IB3x',index,delta))
                    if surfaces:
                        for _,_,s,t in key['texture']:file.write(struct.pack('<2f',s,t))
