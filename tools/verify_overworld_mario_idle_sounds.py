#!/usr/bin/env python3
"""Pin original idle sound owners and exact cue dependencies, not playback."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from audio_map import gamecube_tables,project_effects,macro_samples
from verify_overworld_roads import verify
MANIFEST=Path(__file__).with_name('overworld_mario_idle_sounds_evidence.json')
def assets(iso,e):
    files={r['path']:r for r in inventory(iso)['files']};raws={}
    with Path(iso).open('rb') as disc:
        for asset in e['assets']:
            row=files[asset['path']];disc.seek(row['offset']);raw=disc.read(row['size'])
            assert len(raw)==asset['size'] and hashlib.sha256(raw).hexdigest()==asset['sha256']
            raws[asset['path'].split('.')[-1]]=raw
    rows,_,_=gamecube_tables(iso);rows={r['table_index']:r for r in rows if r['table']=='sfx'}
    fx=project_effects(raws['proj']);mac=macro_samples(raws['pool']);n=0
    for cue in e['cues']:
        r=rows[cue['table_index']]
        for key in ['name','address','flags','fx_id','stream_alias','parameters']:assert r[key]==cue[key]
        assert fx[r['fx_id']]==[{k:v for k,v in g.items() if k not in ('macro','commands','sample_records')} for g in cue['group_bindings']]
        for g in cue['group_bindings']:
            m=mac[g['object_id']];assert m==g['macro'];assert g['commands']==[raws['pool'][q:q+8].hex() for q in range(m['offset']+8,m['offset']+m['size'],8)]
            for s in g['sample_records']:
                raw=raws['sdir'];o=s['record'];assert struct.unpack_from('>H',raw,o)[0]==s['id']
                assert struct.unpack_from('>H',raw,o+14)[0]==s['rate']
                assert list(struct.unpack_from('>3I',raw,o+16))==[s['samples'],s['loop'][0],s['loop'][1]-s['loop'][0]]
                assert s['loop']==[0,0] and s['rate']==22050;n+=1
    assert b'SE4_FLD_M_FURAFURA1' not in raws['etbl']
    assert rows[371]['fx_id']==0 and rows[371]['parameters'][0]==0
    assert all(rows[c]['fx_id']!=rows[c]['table_index'] for c in (225,226,228,239))
    return n
def handles():
    def cancel(newmotion,first,second):
        stopped=[]
        if first!=-1:stopped.append(first);first=-1
        if newmotion not in (19,20) and second!=-1:stopped.append(second);second=-1
        return first,second,stopped
    assert cancel(1,12,13)==(-1,-1,[12,13])
    assert cancel(19,12,13)==(-1,13,[12]) and cancel(20,-1,13)==(-1,13,[])
    assert cancel(1,-1,-1)==(-1,-1,[])
    return 4
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);iso=p.parse_args().iso
    e=json.loads(MANIFEST.read_text());r=verify(iso,MANIFEST);r.update(cues=5,original_bank_assets=6,literal_sample_dependencies=assets(iso,e),derived_handle_cases=handles());print(json.dumps(r,sort_keys=True))
