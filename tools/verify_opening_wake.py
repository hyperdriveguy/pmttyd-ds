#!/usr/bin/env python3
"""Pin original US awakening event/effect owners read-only; no runtime claim."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events

def verify(iso=DEFAULT_ISO,symbols='artifacts/reference/us_symbols.csv'):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US G8ME01 required')
    files={f['path']:f for f in info['files']}
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US revision0/disc0 required')
        item=files['rel/gor.rel'];source.seek(item['offset']);raw=source.read(item['size'])
        item=files['rel/sys.rel'];source.seek(item['offset']);sysraw=source.read(item['size'])
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0]
        source.seek(dol+0xc704);cam=source.read(0x3c4)
    events=inventory_events(raw,symbols,'gor',['evt_mezame','mezame_init_evt'])['events']
    expected=[(92,'ac0a475fff5cc3b12df03184f5df405b5d9d5aea60130062f0f56581807e7955'),(23,'113a5536b690fd6b8629bdf006c44d4b1eed228e41db13f1c3ef5e2fd7bcdbec')]
    for event,(count,digest) in zip(events,expected):
        if len(event['commands'])!=count or event['sha256']!=digest:raise ValueError('Original awakening event changed')
    for offset,size,digest in [(0x10b88,0x110,'2ca80c57df8873b0ed6545883b9356a4336a79e11c3b5f0e4049cdf1ae6aacea'),(0x10cdc,0x1f0,'de5cc7aa6d692bd34d4616488cc3aa86c4450d0d942ef8af62bc5bf2cdf6c26e'),(0x10f10,0x604,'c32d6a14278e2e0e5312e95de12fa5567c6d4ec41132d877849e91d6666081e8')]:
        if hashlib.sha256(raw[offset:offset+size]).hexdigest()!=digest:raise ValueError('Original awakening draw/motion body changed')
    commands={c['address']:c for event in events for c in event['commands']}
    def scalar(n):
        n=n if n<0x80000000 else n-0x100000000
        return (n+230000000)/1024 if -250000000<n<=-210000000 else n
    for address,curve,a,b,duration in [(0x66928,11,2400,1200,2000),(0x66988,1,1200,2400,100),(0x669e8,1,2400,1200,100),(0x66a48,1,1200,2400,100),(0x66ab4,1,500,0,2000),(0x66b18,1,2400,0,2000)]:
        operands=commands[address]['operands']
        if operands[0].get('annotation')!='evt_sub_intpl_msec_init' or [scalar(o['raw']) for o in operands[1:]]!=[curve,a,b,duration]:raise ValueError('Original waking interpolation changed')
    for address,name in [(0x66894,'mac_0_012'),(0x668b4,'mac_0_013'),(0x66b80,'mac_0_013_01')]:
        operands=commands[address]['operands']
        if operands[0].get('annotation')!='evt_msg_print' or operands[2].get('identifier')!=name or [o['raw'] for o in [operands[1],*operands[3:]]]!=[0,0,0]:raise ValueError('Original waking message/speaker changed')
    for address,value in [(0x66844,1000),(0x6688c,3000),(0x668ac,2000),(0x668e4,300),(0x66aa8,500),(0x66b78,3000),(0x66b98,300),(0x66ba0,2000)]:
        c=commands[address]
        if c['opcode']!=10 or c['operands'][0]['raw']!=value:raise ValueError('Original awakening wait changed')
    for address,word in [(0x8000f79c,0xc03a0150),(0x8000f7a4,0xc05a0154),(0x8000f7a8,0xc07a0158),(0x8000f7b4,0x3861013c),(0x8000f7c4,0x3861010c)]:
        if struct.unpack_from('>I',cam,address-0x8000f704)[0]!=word:raise ValueError('Original view translation composition changed')
    heading=inventory_events(sysraw,symbols,'sys',['sys_01_init_evt','evt_start'])['events']
    for event,count,digest in [(heading[0],47,'de944bd9a79ab6a5940f83c6a0af357a99ba07c9341c6405c74f66c581ec6be2'),(heading[1],74,'65bf047866322730d1a7e03623b243ce0158e76f7531507bb4941bccb7be349a')]:
        if len(event['commands'])!=count or event['sha256']!=digest:raise ValueError('Original prologue event changed')
    commands={c['address']:c for event in heading for c in event['commands']}
    for address,value in [(0x2acc,3000),(0x2b24,2000),(0x2bac,3000)]:
        c=commands[address]
        if c['opcode']!=10 or c['operands'][0]['raw']!=value:raise ValueError('Original prologue wait changed')
    gate=commands[0x2bcc]
    if gate['opcode']!=30 or gate['operands'][1]['raw']!=0x1300:raise ValueError('Original heading input mask changed')
    return 31
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(f'{verify(a.iso,a.symbols)} original awakening event/draw/camera anchors verified read-only')
