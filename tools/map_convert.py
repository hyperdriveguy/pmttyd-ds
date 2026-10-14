#!/usr/bin/env python3
"""Convert static TTYD map display lists to native 3DS triangle batches.
Format and transforms adapted from noclip.website/world.ts (MIT).
First-texture materials only; report multilayer materials for fidelity work.
"""
from pathlib import Path
import json
import math
import re
import struct
from collections import defaultdict
import numpy as np
from character_bake import u,i,f,trans,rotate
from map_animation import read_animations,sample,sample_values,sample_material,texture_matrix

def topology(command,verts):
    mode=command&0xf8
    if mode in (0x80,0x88):
        for n in range(0,len(verts)-3,4):
            yield [verts[n],verts[n+1],verts[n+2]];yield [verts[n],verts[n+2],verts[n+3]]
    elif mode==0x90:
        for n in range(0,len(verts)-2,3):yield verts[n:n+3]
    elif mode==0x98:
        for n in range(len(verts)-2):yield [verts[n+(n%2)],verts[n+1-(n%2)],verts[n+2]]
    elif mode==0xa0:
        for n in range(1,len(verts)-1):yield [verts[0],verts[n],verts[n+1]]
    else:raise ValueError(f'Unsupported GX primitive {command:#x}')

def batch_flags(layer,group,cull,event_group=0):
    """Low byte: layer; bits8–23: cutaway group; bits24–25: GX cull index; bits26–31: event visibility group."""
    if not 0<=layer<=255 or not 0<=group<=65535 or not 0<=cull<=3 or not 0<=event_group<=63:
        raise ValueError('Invalid map batch metadata')
    return layer | group<<8 | cull<<24 | event_group<<26

def hit_topology(verts):
    """hitCalcVtxPosition uses strip triples with opposite winding to GX rendering."""
    for n in range(len(verts)-2):
        yield [verts[n],verts[n+2-(n%2)],verts[n+1+(n%2)]]

def wall_segments(collision,metadata=None,with_triangles=False):
    if metadata is not None and len(metadata)!=len(collision):raise ValueError('Wall metadata count mismatch')
    segments=set()
    for index,tri in enumerate(collision):
        a=np.array(tri);normal=np.cross(a[1]-a[0],a[2]-a[0]);length=np.linalg.norm(normal)
        if length<1e-8 or abs(normal[1])/length>.2:continue
        points=a[:,[0,2]]
        p,q=max([(points[x],points[y]) for x,y in [(0,1),(1,2),(0,2)]],key=lambda pair:np.linalg.norm(pair[0]-pair[1]))
        if np.linalg.norm(p-q)>.01:
            p,q=sorted([tuple(np.round(p,4)),tuple(np.round(q,4))])
            tag=tuple(metadata[index]) if metadata is not None else ()
            geometry=tuple(a.flatten()) if with_triangles else ()
            segments.add((*p,*q,float(a[:,1].min()),float(a[:,1].max()),*tag,*geometry))
    return sorted(segments)

def convert(path,output,scale=.1,hidden=(),animation=None,frame=0,ambient=(),mesh_ambient=(),event_groups=None,scene_root=None,scene_offsets=None,object_bindings=(),initial_pose=None,local_offsets=None,named_lighting=(),map_objects=False):
    event_groups=event_groups or {}
    raw=Path(path).read_bytes();base=32
    table=32+u(raw,4)+u(raw,8)*4;names=table+u(raw,12)*8
    def string(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
    chunks={string(names+u(raw,table+n*8+4)):base+u(raw,table+n*8) for n in range(u(raw,12))}
    tex=chunks['texture_table'];textures=[string(base+u(raw,tex+4+n*4)) for n in range(u(raw,tex))]
    mtable=chunks['material_name_table'];materials={};warnings=[]
    for n in range(u(raw,mtable)):
        o=base+u(raw,mtable+8+n*8);name=string(base+u(raw,o));color=tuple(raw[o+4:o+8]);samplers=raw[o+11]
        tid=-1;uvmatrix=np.eye(3);wrap=(0,0)
        if samplers:
            # GX stores sampler entries in reverse texture order.
            s=base+u(raw,o+12+(samplers-1)*4);entry=base+u(raw,s)
            tid=textures.index(string(base+u(raw,entry)));wrap=(raw[s+8],raw[s+9])
            v=struct.unpack_from('>7f',raw,o+44+(samplers-1)*28)
            uvmatrix=texture_matrix(*v)
        if samplers>1:warnings.append({'material':name,'samplers':samplers,'limitation':'Only texture zero currently rendered'})
        materials[o]={'name':name,'color':color,'color_source':raw[o+8],'layer':raw[o+10],'texture':tid,'uv':uvmatrix,'wrap':wrap}
    info=chunks['information'];scene_name=scene_root or string(base+u(raw,info+8));scene_names={scene_name} if isinstance(scene_name,str) else set(scene_name);scene_offsets=scene_offsets or {};collision_name=string(base+u(raw,info+12))
    if initial_pose is not None:
        if animation is not None or len(initial_pose)!=2:raise ValueError('One explicit initial pose required')
        animation,frame=initial_pose
    elif animation is not None and (ambient or mesh_ambient):raise ValueError('Preview and runtime animation export are separate')
    local_offsets=local_offsets or {}
    animations=read_animations(path,light_colors=bool(named_lighting));selected={};selected_lights={}
    if animation is not None:
        matches=[a for a in animations if a['name']==animation]
        if len(matches)!=1:raise ValueError(f'Unknown or ambiguous map animation: {animation}')
        selected={t['node']:sample(t,frame) for t in matches[0]['tracks']}
        selected_lights={t['light']:sample_values(t,frame) for t in matches[0].get('light_color_tracks',[])}
        for track in matches[0]['material_tracks']:
            if track['texgen']!=0:
                warnings.append({'animation':animation,'material':track['material'],'limitation':'Animated texgen beyond texture zero unsupported'})
                continue
            targets=[mat for mat in materials.values() if track['material'] in (mat['name'],re.sub('_[vx]','',mat['name']))]
            if not targets:
                warnings.append({'animation':animation,'material':track['material'],'limitation':'Animation material absent from this map material table'})
                continue
            for mat in targets:mat['uv']=sample_material(track,frame)
    lighting_nodes=set(named_lighting);lighting_records=[]
    if lighting_nodes:
        lt=chunks['light_table']
        for index in range(u(raw,lt)):
            record=base+u(raw,lt+4+index*4)
            kind=string(base+u(raw,record+4))
            attenuation=u(raw,record+56)
            if kind!='pointLight' or attenuation not in (0,1,2,3):
                lighting_records.append(None);continue
            position=np.array(struct.unpack_from('>3f',raw,record+8),float)
            light_name=string(base+u(raw,record))
            if light_name in selected:position=(trans(position)@selected[light_name]@np.array([0,0,0,1.]))[:3]
            color=np.array(list(raw[record+44:record+47]))/255;intensity=f(raw,record+52)
            if light_name in selected_lights:
                values=selected_lights[light_name]
                # mapMain: fmuls255, fctiwz, stb RGB; E50 receives component3.
                color=np.trunc(np.asarray(values[:3],dtype=np.float32)*np.float32(255)).astype(int)%256/255
                intensity=values[3]
            lighting_records.append(dict(name=light_name,position=position,color=color,
                intensity=intensity,attenuation=((1,0,0),(0,.1,0),(0,0,.01),(0,0,.01))[attenuation]))
    vertices=defaultdict(list);nodes=[];collision=[];collision_meta=[];head_geometry=[];collision_nodes={};markers=[];cutaway_nodes={};cull_counts=defaultdict(int);owners=defaultdict(list);node_info={}
    def walk(o,parent,visible=False,collidable=False,depth=0,exterior=False,exterior_group=0,suppressed=False,parent_node=None,event_group=0,scene_offset=(0,0,0)):
        if depth>100:raise ValueError('Invalid map graph')
        name=string(base+u(raw,o));visible=visible or name in scene_names;collidable=collidable or name==collision_name
        node_offset=np.array(scene_offsets.get(name,scene_offset),float)
        values=struct.unpack_from('>9f',raw,o+24);local_matrix=trans(values[6:9])@rotate(values[3:6])@np.diag([*values[:3],1]);m=parent@local_matrix
        if visible and name in selected:
            local_matrix=local_matrix@selected[name];m=parent@local_matrix
        if visible and name in local_offsets:
            local_matrix=local_matrix@trans(local_offsets[name]);m=parent@local_matrix
        node_info[o]={'name':name,'parent':parent_node,'local':local_matrix,'world':m.copy(),'scene_offset':node_offset.copy()}
        node_exterior=exterior or name.startswith('S_gai_kabe')
        node_group=exterior_group
        if node_exterior and not node_group and u(raw,o+92):
            node_group=o;cutaway_nodes[node_group]=name
        node_event_group=event_groups.get(name,event_group)
        node_suppressed=suppressed or name in hidden
        nodes.append(name)
        draw_pointer=u(raw,o+88)
        attributes=u(raw,base+draw_pointer+8) if draw_pointer else 0
        if collidable:
            # _hitEnt stores bbox midpoint for mesh owners, local translation
            # for hierarchy-only nodes; hitObjGetPos transforms leaf centers.
            bbox=struct.unpack_from('>6f',raw,o+60)
            center=(np.array(bbox[:3])+np.array(bbox[3:]))*.5 if u(raw,o+92) else np.array(values[6:9])
            world_center=(m@np.array([*center,1]))[:3]*scale+node_offset
            collision_nodes[o-base]={'name':name,'attributes':attributes,
                'parent':parent_node-base if parent_node is not None and name!=collision_name else None,
                'center':world_center.tolist(),'parts':u(raw,o+92)}
        cull=raw[base+draw_pointer+1] if draw_pointer else 3
        if u(raw,o+92) and cull>3:raise ValueError(f'Invalid GX cull index for {name}: {cull}')
        light=None;normal_matrix=None
        if name in lighting_nodes:
            if not draw_pointer or raw[base+draw_pointer+2]&8:raise ValueError('Used named lighting requires original named mask')
            mask=u(raw,base+draw_pointer+4)
            if mask==0 or mask&(mask-1):raise ValueError('Used lighting bake requires one original point light')
            index=mask.bit_length()-1
            if index>=len(lighting_records) or lighting_records[index] is None:raise ValueError('Unsupported used original light')
            light=lighting_records[index];normal_matrix=np.linalg.inv(m[:3,:3]).T
        marker_points=[]
        for part in range(u(raw,o+92)):
            mo=u(raw,o+96+part*8)
            if not mo:continue
            mesh=base+u(raw,o+100+part*8);packed=bool(raw[mesh+3])
            if not packed:raise ValueError('Unpacked map display list requires conversion support')
            bits=u(raw,mesh+8);vcd=base+u(raw,mesh+12)
            if bits&~0x77:raise ValueError(f'Unsupported vertex attributes {bits:#x}')
            arrays={0:base+u(raw,vcd)+4,1:base+u(raw,vcd+4)+4,2:base+u(raw,vcd+12)+4,4:base+u(raw,vcd+24)+4,5:base+u(raw,vcd+28)+4,6:base+u(raw,vcd+32)+4}
            attrs=[bit for bit in range(7) if bits&(1<<bit)]
            mat=materials[base+mo]
            for run in range(u(raw,mesh+4)):
                dl=base+u(raw,mesh+16+run*8);end=dl+u(raw,mesh+20+run*8)
                while dl<end:
                    cmd=raw[dl];dl+=1
                    if cmd==0:continue
                    count=struct.unpack_from('>H',raw,dl)[0];dl+=2
                    local=[]
                    for k in range(count):
                        indices={}
                        for attr in attrs:indices[attr]=struct.unpack_from('>H',raw,dl)[0];dl+=2
                        p=np.array([*struct.unpack_from('>hhh',raw,arrays[0]+indices[0]*6),1.],float)
                        p[:3]/=2**u(raw,vcd+68);p=m@p;p=p[:3]*scale+node_offset
                        uv=np.array([0.,0.,1.])
                        if 4 in indices:uv[:2]=np.array(struct.unpack_from('>hh',raw,arrays[4]+indices[4]*4))/2**u(raw,vcd+72)
                        uv=mat['uv']@uv
                        color=np.array(mat['color'] if mat['color_source']==0 else [255]*4,float)/255
                        if 2 in indices:color*=np.array(list(raw[arrays[2]+indices[2]*4:arrays[2]+indices[2]*4+4]))/255
                        if light is not None:
                            if 1 not in indices:raise ValueError('Original lit geometry needs normals')
                            # mapSetPolygonVtxDesc: signed bytes, XYZ, frac6, stride3.
                            normal=normal_matrix@(np.array(struct.unpack_from('>bbb',raw,arrays[1]+indices[1]*3),float)/64)
                            length=np.linalg.norm(normal)
                            if length==0:raise ValueError('Zero original lit normal')
                            normal/=length # portable bake preserves direction across map unit scaling
                            world=p/scale-node_offset/scale
                            direction=light['position']-world;distance=np.linalg.norm(direction)
                            if distance==0:raise ValueError('Coincident original point light')
                            diffuse=max(0.,float(normal@(direction/distance)))
                            # Runtime loader scales original light position by10; GX attenuation uses engine units.
                            distance*=10
                            a0,a1,a2=light['attenuation'];denom=a0+a1*distance+a2*distance*distance
                            color[:3]*=np.clip(light['color']*light['intensity']*diffuse/denom,0,1)
                        # Tex3DS uses bottom-origin texture coordinates.
                        local.append([*p,float(uv[0]),1-float(uv[1]),*color])
                    tris=list(topology(cmd,local))
                    if name.endswith("_bero"):marker_points.extend(v[:3] for v in local)
                    if visible and not node_suppressed:
                        for tri in tris:
                            group=node_group if node_exterior else 0
                            vertices[(base+mo,group,cull,node_event_group)].extend(tri)
                            owners[(base+mo,group,cull,node_event_group)].extend([o]*3)
                            cull_counts[cull]+=1
                    elif collidable:
                        for tri in tris:
                            collision.append([v[:3] for v in tri]);collision_meta.append((attributes,o-base))
                        for tri in hit_topology(local):
                            head_geometry.append(([v[:3] for v in tri],(attributes,o-base),cull))
        if name.endswith('_bero'):
            points=np.array(marker_points) if marker_points else np.array([(m@np.array([0,0,0,1]))[:3]*scale])
            markers.append({'name':name,'bounds':[points.min(0).tolist(),points.max(0).tolist()]})
        child=u(raw,o+12);sibling=u(raw,o+16)
        if child:walk(base+child,m,visible,collidable,depth+1,node_exterior,node_group,node_suppressed,o,node_event_group,node_offset)
        if sibling:walk(base+sibling,parent,visible if name not in scene_names else False,collidable if name!=collision_name else False,depth+1,exterior,exterior_group,suppressed,parent_node,event_group,scene_offset)
    walk(base+u(raw,chunks['information']+4),np.eye(4))
    output=Path(output);output.mkdir(parents=True,exist_ok=True)
    ambient_names={t['material'] for a in animations if a['name'] in ambient for t in a['material_tracks'] if t['texgen']==0}
    count=0;batches=[];bounds=[];bindings=defaultdict(list);position_bindings=defaultdict(list)
    cutaway_ids={node:n+1 for n,node in enumerate(sorted(cutaway_nodes))}
    with (output/'map.vertices').open('wb') as out:
        for mo,group,cull,event_group in sorted(vertices,key=lambda x:materials[x[0]]['layer']):
            v=vertices[(mo,group,cull,event_group)];mat=materials[mo]
            inverse=np.linalg.inv(mat['uv']) if mat['texture']>=0 and any(n in ambient_names for n in (mat['name'],re.sub('_[vx]','',mat['name']))) else None
            for index,row in enumerate(v):
                position_bindings[owners[(mo,group,cull,event_group)][index]].append((count+index,*row[:3]))
                if inverse is not None:
                    original=inverse@np.array([row[3],1-row[4],1])
                    bindings[mo].append((count+index,float(original[0]),float(original[1])))
                if mat['texture']<0:row[3]=496/512;row[4]=1-240/256
                out.write(struct.pack('<9f',*row));bounds.append(row[:3])
            batches.append((count,len(v),mat['texture'],batch_flags(mat['layer'],cutaway_ids.get(group,0),cull,event_group),*mat['wrap']));count+=len(v)
    with (output/'map.batches').open('wb') as out:
        out.write(struct.pack('<II',len(batches),len(textures)))
        for b in batches:out.write(struct.pack('<IIiIII',*b))
    # Native event flag overlay: exact source hierarchy and converted vertex owners.
    ordered_flags=list(node_info);flag_ids={node:i for i,node in enumerate(ordered_flags)}
    flag_owners=[None]*count
    for node,rows in position_bindings.items():
        for index,*_ in rows:flag_owners[index]=flag_ids[node]
    if any(owner is None for owner in flag_owners):raise ValueError('Unowned map flag vertex')
    with (output/'map.mapflags').open('wb') as out:
        out.write(struct.pack('<8sII',b'PMFLG001',count,len(ordered_flags)))
        for node in ordered_flags:
            info=node_info[node];name=info['name'].encode('ascii')
            if len(name)>=64:raise ValueError('Native map flag name exceeds field')
            out.write(struct.pack('<iII64s',flag_ids[info['parent']] if info['parent'] is not None else -1,node-base,int(node-base in collision_nodes),name))
        out.write(struct.pack('<'+'H'*count,*flag_owners))
    runtime_tracks=[]
    for clip in ambient:
        matches=[a for a in animations if a['name']==clip]
        if len(matches)!=1 or (matches[0]['tracks'] and clip not in mesh_ambient):raise ValueError('Mesh/material clips must enable both channels')
        entry=matches[0];duration=entry['duration']
        if duration!=int(duration) or not 0<duration<=4096:raise ValueError('Unsupported ambient frame duration')
        for track in entry['material_tracks']:
            if track['texgen']!=0:continue # Multilayer runtime rendering is unfinished.
            targets=[mo for mo,mat in materials.items() if track['material'] in (mat['name'],re.sub('_[vx]','',mat['name']))]
            bound=[v for mo in targets for v in bindings[mo]]
            if not bound:
                warnings.append({'animation':clip,'material':track['material'],'limitation':'No texture-zero binding in visible packed geometry'})
                continue
            frames=[sample_material(track,frame)[:2].flatten().tolist() for frame in range(int(duration))]
            runtime_tracks.append({'clip':clip,'material':track['material'],'duration':int(duration),'bindings':bound,'frames':frames})
    used=set()
    with (output/'map.uvanim').open('wb') as out:
        out.write(struct.pack('<4sII',b'UVAN',1,len(runtime_tracks)))
        for track in runtime_tracks:
            out.write(struct.pack('<II',track['duration'],len(track['bindings'])))
            for frame in track['frames']:out.write(struct.pack('<6f',*frame))
            for vertex in track['bindings']:
                if vertex[0] in used:raise ValueError('Overlapping ambient UV bindings')
                used.add(vertex[0]);out.write(struct.pack('<Iff',*vertex))
    mesh_tracks=[];mesh_used=set()
    for clip in mesh_ambient:
        matches=[a for a in animations if a['name']==clip]
        if len(matches)!=1 or (matches[0]['material_tracks'] and clip not in ambient):raise ValueError('Mesh/material clips must enable both channels')
        entry=matches[0];duration=entry['duration']
        if duration!=int(duration) or not 0<duration<=4096:raise ValueError('Unsupported mesh duration')
        track_names={t['node']:t for t in entry['tracks']}
        if initial_pose is not None and set(track_names)&set(selected):raise ValueError('Overlapping initial and runtime map pose owners')
        if len(track_names)!=len(entry['tracks']):raise ValueError('Duplicate mesh track target')
        for owner,bound in position_bindings.items():
            chain=[];node=owner
            while node is not None:chain.append(node);node=node_info[node]['parent']
            chain.reverse()
            if not any(node_info[node]['name'] in track_names for node in chain):continue
            inverse=np.linalg.inv(node_info[owner]['world']);frames=[]
            for frame in range(int(duration)):
                world=np.eye(4)
                for node in chain:
                    info=node_info[node];world=world@info['local']
                    if info['name'] in track_names:world=world@sample(track_names[info['name']],frame)
                delta=world@inverse
                # Vertices were scaled to native units; conjugate translation.
                delta[:3,3]*=scale
                frames.append(delta[:3].flatten().tolist())
            for vertex in bound:
                if vertex[0] in mesh_used:raise ValueError('Overlapping ambient mesh bindings')
                mesh_used.add(vertex[0])
            mesh_tracks.append({'owner':node_info[owner]['name'],'duration':int(duration),'frames':frames,'bindings':bound})
    with (output/'map.meshanim').open('wb') as out:
        out.write(struct.pack('<4sII',b'MSAN',1,len(mesh_tracks)))
        for track in mesh_tracks:
            out.write(struct.pack('<II',track['duration'],len(track['bindings'])))
            for frame in track['frames']:out.write(struct.pack('<12f',*frame))
            for vertex in track['bindings']:out.write(struct.pack('<Ifff',*vertex))
    # Retain per-object local coordinates for bounded native event transforms.
    # Geometry remains ignored; descendants inherit the addressed object's matrix.
    if object_bindings:
        with (output/'map.houseobj').open('wb') as out:
            out.write(struct.pack('<8sII',b'PMHOBJ02',count,len(object_bindings)))
            for tag,name in enumerate(object_bindings):
                matches=[node for node,info in node_info.items() if info['name']==name]
                if len(matches)!=1:raise ValueError(f'Unique event object required: {name}')
                target=matches[0];matrix=node_info[target]['world'].copy();matrix[:3,:]*=scale
                matrix[:3,3]+=np.array(scene_offsets.get(name,(0,0,0)))
                inverse=np.linalg.inv(matrix);bound=[]
                for owner,rows in position_bindings.items():
                    ancestor=owner
                    while ancestor is not None and ancestor!=target:ancestor=node_info[ancestor]['parent']
                    if ancestor!=target:continue
                    for index,x,y,z in rows:
                        local=(inverse@np.array([x,y,z,1]))[:3]
                        bound.append((index,*local,x,y,z))
                out.write(struct.pack('<II12f',tag,len(bound),*matrix[:3,:].flatten()))
                for row in sorted(bound):out.write(struct.pack('<I6f',*row))
    # Original complete node hierarchy and raw local coordinates for native maps.
    if map_objects:
        ordered=list(node_info);indices={node:i for i,node in enumerate(ordered)}
        with (output/'map.mapobj').open('wb') as out:
            out.write(struct.pack('<8sII',b'PMMAP001',count,len(ordered)))
            for node in ordered:
                info=node_info[node];name=info['name'].encode('ascii')
                if len(name)>=64:raise ValueError('Native map object name exceeds field')
                rows=position_bindings.get(node,[])
                out.write(struct.pack('<i64s12fI',indices[info['parent']] if info['parent'] is not None else -1,name,*info['local'][:3,:].flatten(),len(rows)))
                inverse=np.linalg.inv(info['world']);offset=info['scene_offset']
                for index,x,y,z in rows:
                    local=(inverse@np.array([*(np.array([x,y,z])-offset)/scale,1]))[:3]
                    out.write(struct.pack('<I3f',index,*local))
    # Complete static collision mesh for upward head rays, without slope filtering.
    with (output/'map.head').open('wb') as out:
        out.write(struct.pack('<4sI',b'HEAD',len(head_geometry)))
        for tri,meta,cull in head_geometry:
            out.write(struct.pack('<9fIII',*(v for point in tri for v in point),*meta,cull))
    segments=wall_segments(collision,collision_meta,with_triangles=True)
    with (output/'map.walls').open('wb') as out:
        out.write(struct.pack('<I',len(segments)|0xe0000000))
        for segment in segments:out.write(struct.pack('<6fII9f',*segment))
    # Keep walkable collision surfaces, including ramps and elevated platforms.
    ground=[]
    for tri,meta,cull in head_geometry:
        a=np.array(tri);normal=np.cross(a[1]-a[0],a[2]-a[0]);length=np.linalg.norm(normal)
        # Retain projected surfaces; runtime applies source slope selection after
        # nearest-hit selection, rather than silently discarding steep blockers.
        if length>1e-8 and abs(normal[1])/length>1e-6:
            ground.append((tri,meta,cull))
    with (output/'map.ground').open('wb') as out:
        # Tagged format preserves original node attributes and relative source ID.
        out.write(struct.pack('<I',len(ground)|0xc0000000))
        for tri,meta,cull in ground:
            out.write(struct.pack('<9fIII',*(v for point in tri for v in point),*meta,cull))
    summary={'source':str(path),'scale':scale,'vertices':count,'triangles':count//3,'batches':len(batches),'textures':textures,
             'bounds':[np.min(bounds,axis=0).tolist(),np.max(bounds,axis=0).tolist()],
             'cull_triangles':dict(cull_counts),'cutaway_nodes':[{'id':cutaway_ids[o],'name':name} for o,name in sorted(cutaway_nodes.items())],'exit_markers':markers,'limitations':warnings,'event_groups':event_groups,'hidden_nodes':list(hidden),'nodes':len(nodes),'collision_triangles':len(collision)}
    (output/'map.animations.json').write_text(json.dumps(animations,indent=2)+'\n')
    summary['animations']=[{'name':a['name'],'duration':a['duration'],'mesh_tracks':len(a['tracks']),'material_tracks':len(a['material_tracks']),'material_table_offset':a['material_table_offset']} for a in animations]
    summary['ambient_mesh_clips']=list(mesh_ambient);summary['animated_position_vertices']=len(mesh_used)
    summary['scene_roots']=sorted(scene_names)
    summary['scene_offsets']=scene_offsets
    summary['collision_nodes']=[{'source_node':node,**data} for node,data in sorted(collision_nodes.items())]
    summary['ambient_material_clips']=list(ambient);summary['animated_uv_vertices']=len(used)
    summary['sampled_animation']=animation;summary['sampled_frame']=frame if animation else None
    summary['named_lighting_nodes']=sorted(lighting_nodes)
    (output/'map-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
    (output/'collision.json').write_text(json.dumps(collision))
    return summary
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('input');p.add_argument('output');p.add_argument('--animation');p.add_argument('--frame',type=float,default=0);a=p.parse_args();print(convert(a.input,a.output,animation=a.animation,frame=a.frame))
