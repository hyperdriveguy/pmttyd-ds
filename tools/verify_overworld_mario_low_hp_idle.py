#!/usr/bin/env python3
"""Pin original low-HP idle requests and source pose metadata; host math adapted."""
import argparse,hashlib,json,math,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from verify_overworld_roads import verify
from verify_overworld_animation_updates import animation,evaluate
MANIFEST=Path(__file__).with_name('overworld_mario_low_hp_idle_evidence.json')
def check_assets(iso,e):
    files={r['path']:r for r in inventory(iso)['files']};poses=samples=0
    with Path(iso).open('rb') as disc:
        for model in e['models']:
            row=files[model['path']];disc.seek(row['offset']);raw=disc.read(row['size'])
            assert len(raw)==model['size'] and hashlib.sha256(raw).hexdigest()==model['sha256']
            u=lambda a:struct.unpack_from('>I',raw,a)[0]
            directory=[raw[u(0x1ac)+64*i:u(0x1ac)+64*i+60].split(b'\0')[0].decode() for i in range(u(0x148))]
            if model['path'].endswith('_r'):assert 'M_S_3R' not in directory and 'M_I_SR' not in directory
            for pose in model['poses']:
                assert directory[pose['index']]==pose['name']
                a=animation(raw,pose['name']);assert (a['loop'],a['start'],a['end'],[k['time'] for k in a['frames']],a['stats'])==(pose['loop'],pose['start'],pose['end'],pose['times'],pose['stats'])
                for t in [0,.25,1,28.5,a['end']-.5,a['end'],a['end']+1]:
                    output=evaluate(a,t);assert all(math.isfinite(x) for row in output['positions'] for x in row) and all(math.isfinite(x) for x in output['nodes']);samples+=1
                poses+=1
    return poses,samples
def key_admitted(seq,mario39,ship_first,ship_second,roll,slit,partner,
                 christine=0,bomhei=0,yoshi=0,partner_flags=0,partner_motion=0,held_item=0):
    if seq!=2 or mario39!=0 or ship_first==1 or ship_second==2 or roll==0 or slit==0:return False
    if not partner:return True
    if christine==1 or bomhei!=0 or yoshi!=0:return False
    if partner_flags&0x100 and partner_motion in (5,6,7):return False
    return held_item==0

def check_key():
    base=dict(seq=2,mario39=0,ship_first=0,ship_second=0,roll=1,slit=1,partner=True)
    assert key_admitted(**base)
    for change in [dict(seq=1),dict(mario39=1),dict(ship_first=1),dict(ship_second=2),dict(roll=0),dict(slit=0),dict(christine=1),dict(bomhei=1),dict(yoshi=1),dict(held_item=1)]:assert not key_admitted(**(base|change))
    for motion in (5,6,7):assert not key_admitted(**(base|dict(partner_flags=0x100,partner_motion=motion)))
    assert key_admitted(**(base|dict(partner_flags=0,partner_motion=5)))
    assert key_admitted(**(base|dict(partner_flags=0x100,partner_motion=4)))
    assert key_admitted(**(base|dict(christine=2)))
    assert key_admitted(**(base|dict(partner=False,held_item=1,christine=1)))
    # Two ship call results differ; only first==1 and second==2 reject.
    assert key_admitted(**(base|dict(ship_first=2,ship_second=1)))
    return 19

def derived():
    # Nonnegative ordinary pouch HP, entry domain, unlocked pose requests.
    def entry(hp,key,current):
        if hp==0:return 'M_I_S'
        if hp<=5 and key:return 'M_S_3'
        return current if current=='M_I_Y' else 'M_S_1'
    for hp,key,current,want in [(0,False,'M_S_1','M_I_S'),(1,True,'M_S_1','M_S_3'),(5,True,'M_S_1','M_S_3'),(6,True,'M_S_3','M_S_1'),(5,False,'M_S_3','M_S_1'),(5,False,'M_I_Y','M_I_Y')]:assert entry(hp,key,current)==want
    # Signed increment and strict threshold: reset entry5a=0, rate60 =>61st callback.
    assert 60<=1000*60//1000 and 61>1000*60//1000
    return 8
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);iso=p.parse_args().iso
    e=json.loads(MANIFEST.read_text());result=verify(iso,MANIFEST);poses,samples=check_assets(iso,e)
    result.update(original_front_poses=poses,derived_pose_samples=samples,derived_cases=derived(),key_predicate_cases=check_key());print(json.dumps(result,sort_keys=True))
