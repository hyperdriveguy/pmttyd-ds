#!/usr/bin/env python3
"""Pin ordinary Jump's scoped registration and direct callback-address census."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from dol_event_inventory import inventory_events
from iso_assets import DEFAULT_ISO


def verify(iso,symbols):
    expected=json.loads((Path(__file__).resolve().parent/'jump_callback_registry.json').read_text())
    inventory=inventory_events(iso,symbols,[e['name'] for e in expected['events']])
    actual_events=[{k:e[k] for k in ('name','address','size','sha256')} for e in inventory['events']]
    if actual_events!=expected['events']:
        raise ValueError('Source streams differ')
    actual={}
    for event in inventory['events']:
        for cmd in event['commands']:
            if cmd['opcode']==91:
                operand=cmd['operands'][0]
                actual.setdefault(operand['raw'],[]).append(dict(event=event['name'],address=cmd['address']))
    pinned={c['address']:c['sites'] for c in expected['callbacks']}
    if actual!=pinned:
        raise ValueError('Direct USER_FUNC registry sites differ')
    with Path(iso).open('rb') as disc:
        disc.seek(0x420);base=struct.unpack('>I',disc.read(4))[0]
        disc.seek(base);header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+4*i)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    matches.append(base+off+address-va)
            if len(matches)!=1:
                raise ValueError('Unmapped/ambiguous original address')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated original region')
            return data
        for record in expected['callbacks']+expected['containing_bodies']+expected['spans']:
            if hashlib.sha256(read(record['address'],record['size'])).hexdigest()!=record['sha256']:
                raise ValueError(f'Body/span differs: {record["address"]:#x}')
        for address,value in expected['registration_anchors'].items():
            if read(int(address,16),4).hex()!=value:
                raise ValueError(f'Registration instruction differs: {address}')
    return dict(schema=1,scope=expected['scope'],events=len(expected['events']),
                callbacks=len(pinned),user_func_sites=sum(map(len,pinned.values())),
                registration_anchors=len(expected['registration_anchors']),spans=len(expected['spans']),
                containing_bodies=len(expected['containing_bodies']),
                limitation='Callback identities/sites are pinned;full callback contracts,menu/dynamic sources and original execution remain separate')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--symbols',default='artifacts/reference/us_symbols.csv')
    parser.add_argument('--output')
    args=parser.parse_args();result=verify(args.iso,args.symbols)
    if args.output:
        output=Path(args.output)
        if output.resolve() in (Path(args.iso).resolve(),Path(args.symbols).resolve()):
            raise ValueError('Output must not overwrite inputs')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
