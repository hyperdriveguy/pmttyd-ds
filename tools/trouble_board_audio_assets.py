#!/usr/bin/env python3
"""Retain four original primary PCM samples for the finite native UI fallback."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from audio_convert import sample_directory,convert_sample
ROWS=[(5,1041,2760,105),(19,1040,4013,75),(42,1064,8159,90),(43,1063,6725,90)]
def build(iso=DEFAULT_ISO):
    work=Path('local-assets/trouble-board-audio');work.mkdir(parents=True,exist_ok=True)
    directory=work/'pmario.sdir';bank=work/'pmario.samp'
    extract(iso,'sound/proj/pmario.sdir',directory);extract(iso,'sound/proj/pmario.samp',bank)
    if hashlib.sha256(directory.read_bytes()).hexdigest()!='9529535398c5f7a839dd18f00c8e831957b26b5aab6a7bf83e5ee6acb5cb8496':
        raise ValueError('Unexpected original US sample directory')
    entries={e['id']:e for e in sample_directory(directory)};raw=bank.read_bytes();out=Path('romfs');out.mkdir(exist_ok=True)
    records=[]
    for row,sid,frames,volume in ROWS:
        entry=entries[sid]
        if entry['samples']!=frames or entry['rate']!=22050 or entry['loop']!=[0,0]:raise ValueError('Original UI sample metadata changed')
        path=out/f'trouble-sfx-{row}.wav';record=convert_sample(entry,raw,path)
        record.update(row=row,row_volume=volume,pcm_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),output=path.name)
        records.append(record)
    metadata=dict(source_directory='sound/proj/pmario.sdir',source_bank='sound/proj/pmario.samp',
        bank_sha256=hashlib.sha256(raw).hexdigest(),samples=records,
        policy='Original primary-sample fallback; full MusyX macro, envelope, voice and mix execution remain unbound',
        license='Nintendo/Intelligent Systems original resources; local use only; no redistribution rights')
    (out/'trouble-sfx.json').write_text(json.dumps(metadata,indent=2)+'\n')
    print('Original Trouble Center primary samples:',[r['row'] for r in records])
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
