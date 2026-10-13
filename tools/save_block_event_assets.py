#!/usr/bin/env python3
"""Package only original US save-event data/strings from the read-only ISO.
No original executable instructions or fabricated event commands are exported.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from dol_event_inventory import parse_commands
EVENTS=[(0x8030b4ec,164,'a580e9894d07ae58b8cb24d35b825df91eb9b3d42e003f84cd4e5a15dcaa3752'),
        (0x803bac3c,52,'b3467d3aa54cf4ec1ff2b9f90de94862d99c4325e5d8805775b4448a95b39b1e'),
        (0x803ba444,2040,'fd79ccecf3d3d6f041a8e7ab14f6d78776eeb0a642a83a6ad72d877ca6123dac')]
CALLBACKS={0x800d2090:'evt_msg_select',0x800d21fc:'evt_msg_continue',0x800d23c4:'evt_msg_print_add',
           0x800d284c:'evt_msg_print',0x800e7474:'evt_mario_key_onoff',0x8025c0b8:'memcard_write',
           0x8025c188:'memcard_ipl',0x8025c19c:'memcard_header_write',0x8025c2bc:'memcard_create',
           0x8025c2fc:'memcard_format',0x8025c380:'unk_8025c380',0x8025c3a4:'unk_8025c3a4',
           0x8025c40c:'unk_8025c40c',0x8025c454:'memcard_code'}

def build(iso=DEFAULT_ISO):
    intervals=[];events=[];strings={};locations={};sites={address:0 for address in CALLBACKS}
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        def read(address,size):
            # DOL data sections only. This exporter never includes text bodies.
            matches=[]
            for i in range(7,18):
                off,base,length=(struct.unpack_from('>I',header,p+i*4)[0] for p in [0,0x48,0x90])
                if base and base<=address and address+size<=base+length:matches.append(off+address-base)
            if len(matches)!=1:raise ValueError('Event operand/span outside original data sections')
            locations[address]=dol+matches[0]
            disc.seek(dol+matches[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated ISO data')
            return data
        for address,size,expected in EVENTS:
            data=read(address,size)
            if hashlib.sha256(data).hexdigest()!=expected:raise ValueError('Original event identity mismatch')
            commands=parse_commands(data,address,CALLBACKS);intervals.append((address,address+size+4)) # Source label scanner reads one word past END.
            events.append(dict(address=address,size=size,source_disc_offset=locations[address],sha256=expected,commands=len(commands)))
            for command in commands:
                values=[p['raw'] for p in command['operands']]
                if command['opcode']!=91:continue
                function=values[0]
                if function not in CALLBACKS:raise ValueError('Unadmitted source callback')
                sites[function]+=1
                if function in [0x800d2090,0x800d23c4,0x800d284c]:
                    pointer=values[2];text=bytearray()
                    for n in range(256):
                        byte=read(pointer+n,1)[0]
                        if not byte:break
                        text.append(byte)
                    else:raise ValueError('Unterminated source message operand')
                    strings[pointer]=bytes(text).decode('ascii')
                    intervals.append((pointer&~3,(pointer+len(text)+4)&~3))
        merged=[]
        for start,end in sorted(intervals):
            if merged and start<=merged[-1][1]:merged[-1]=(merged[-1][0],max(end,merged[-1][1]))
            else:merged.append((start,end))
        segments=[(start,read(start,end-start)) for start,end in merged]
    output=Path('romfs');work=Path('local-assets/save-block')
    for target in [output/'save-block-events.pmevt',output/'save-block-program.bin',work/'event-program.json']:
        if target.resolve()==Path(iso).resolve():raise ValueError('Output must not overwrite source ISO')
    output.mkdir(exist_ok=True)
    table=bytearray();payload=bytearray();offset=12+12*len(segments)
    for address,data in segments:
        table+=struct.pack('>III',address,len(data),offset);payload+=data;offset+=len(data)
    (output/'save-block-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
    metadata=struct.pack('<8s4I',b'PMSAVE01',*[e[0] for e in EVENTS],len(CALLBACKS))
    metadata+=b''.join(struct.pack('<II',address,index) for index,address in enumerate(CALLBACKS))
    (output/'save-block-program.bin').write_bytes(metadata)
    manifest=dict(source_disc='G8ME01',source='DOL data sections in read-only ISO',events=events,
                  callback_sites=sites,callbacks=CALLBACKS,strings=strings,
                  segments=[dict(address=a,size=len(d),source_disc_offset=locations[a],sha256=hashlib.sha256(d).hexdigest()) for a,d in segments],
                  fingerprint=hashlib.sha256(Path(__file__).read_bytes()+payload).hexdigest(),
                  policy='Original addresses/commands/strings retained. Data-only selected spans; no text-section or executable code export.',
                  license='Original Nintendo/Intelligent Systems resources; local conversion only, no redistribution rights')
    work.mkdir(parents=True,exist_ok=True)
    (work/'event-program.json').write_text(json.dumps(manifest,indent=2)+'\n')
    return manifest

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();build(a.iso)
