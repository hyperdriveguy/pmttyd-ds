"""Editable original PMMAP001 hierarchy sources; native pack stays generated."""
import math
import struct

FORMAT='PMMAP001'
MAX_VERTICES=1_000_000
MAX_OBJECTS=4096
NAME_BYTES=64

def integer(value,low,high):
    if type(value) is not int or not low<=value<=high:
        raise ValueError('Hierarchy integer outside native admitted domain')
    return value

def scalars(row,count):
    if not isinstance(row,list) or len(row)!=count:
        raise ValueError('Hierarchy matrix/point component count')
    if any(type(value) not in (int,float) or not math.isfinite(value) or abs(value)>3.4028234663852886e38 for value in row):
        raise ValueError('Hierarchy requires finite binary32 components')
    return row

def encode(document):
    if document.get('schema')!=1 or document.get('format')!=FORMAT:
        raise ValueError('Expected editable original PMMAP001 hierarchy')
    count=integer(document['vertex_count'],1,MAX_VERTICES)
    objects=document['objects'];integer(len(objects),1,MAX_OBJECTS)
    data=bytearray(struct.pack('<8sII',FORMAT.encode(),count,len(objects)))
    owners=set()
    for index,node in enumerate(objects):
        parent=integer(node['parent'],-1,index-1)
        name=node['name'].encode('ascii')
        if not name or len(name)>=NAME_BYTES or b'\0' in name:
            raise ValueError('Hierarchy requires nonempty original ASCII name <64 bytes')
        matrix=scalars(node['matrix'],12);rows=node['bindings'];integer(len(rows),0,count)
        data+=struct.pack('<i64s12fI',parent,name,*matrix,len(rows))
        for row in rows:
            if not isinstance(row,list) or len(row)!=4:raise ValueError('Hierarchy binding requires index and source-local point')
            vertex=integer(row[0],0,count-1)
            if vertex in owners:raise ValueError('Hierarchy vertex has multiple owners')
            owners.add(vertex);data+=struct.pack('<I3f',vertex,*scalars(row[1:],3))
    if len(owners)!=count:raise ValueError('Hierarchy does not own every original template vertex')
    return bytes(data)

def decode(raw):
    offset=0
    def take(fmt):
        nonlocal offset
        size=struct.calcsize('<'+fmt)
        if offset+size>len(raw):raise ValueError('Truncated hierarchy pack')
        value=struct.unpack_from('<'+fmt,raw,offset);offset+=size;return value
    magic,count,total=take('8sII')
    if magic!=FORMAT.encode():raise ValueError('Expected original native hierarchy pack')
    integer(count,1,MAX_VERTICES);integer(total,1,MAX_OBJECTS);objects=[]
    for _ in range(total):
        parent,name,*values=take('i64s12fI');bindings=values.pop()
        if b'\0' not in name:raise ValueError('Unterminated hierarchy name')
        if any(name[len(name.split(b'\0',1)[0])+1:]):raise ValueError('Unknown nonzero hierarchy name suffix')
        integer(bindings,0,count)
        objects.append(dict(parent=parent,name=name.split(b'\0',1)[0].decode('ascii'),matrix=values,
                            bindings=[list(take('I3f')) for _ in range(bindings)]))
    if offset!=len(raw):raise ValueError('Trailing hierarchy bytes')
    result=dict(schema=1,format=FORMAT,vertex_count=count,objects=objects)
    if encode(result)!=raw:raise ValueError('Hierarchy source round trip changed original pack')
    return result
