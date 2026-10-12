#!/usr/bin/env python3
"""Decode original STM/MusyX Nintendo DSP ADPCM without resampling.
STM header/decoder reference: vgmstream (ISC; see third_party).
MusyX directory layout researched using Nisto/musyx-extract; no source copied.
"""
from pathlib import Path
import array
import json
import struct
import sys
import wave

def u(raw,o):return struct.unpack_from('>I',raw,o)[0]
def nibbles_to_samples(n):return n//16*14+max(0,n%16-2)
def decode(data,count,coefficients,hist1=0,hist2=0):
    if len(data)<((count+13)//14)*8:raise ValueError('Truncated DSP stream')
    out=array.array('h')
    for o in range(0,((count+13)//14)*8,8):
        frame=data[o:o+8];predictor=frame[0]>>4;scale=1<<(frame[0]&15)
        if predictor>7:raise ValueError('Invalid DSP predictor')
        a,b=coefficients[predictor*2:predictor*2+2]
        for n in range(min(14,count-len(out))):
            value=(frame[1+n//2]>>(4 if n%2==0 else 0))&15
            if value>=8:value-=16
            sample=((value*scale<<11)+1024+a*hist1+b*hist2)>>11
            sample=max(-32768,min(32767,sample));out.append(sample);hist2,hist1=hist1,sample
    return out

def wav(path,samples,rate,channels):
    path=Path(path);path.parent.mkdir(parents=True,exist_ok=True)
    if sys.byteorder!='little':samples.byteswap()
    with wave.open(str(path),'wb') as out:
        out.setnchannels(channels);out.setsampwidth(2);out.setframerate(rate);out.writeframes(samples.tobytes())

def stm(input,output):
    if Path(input).resolve()==Path(output).resolve():raise ValueError('Output must not overwrite source')
    raw=Path(input).read_bytes()
    if len(raw)<256:raise ValueError('Truncated STM header')
    if raw[:2]!=b'\x02\x00':raise ValueError('Not a TTYD STM')
    channels=u(raw,4);rate=struct.unpack_from('>H',raw,2)[0];stride=(u(raw,8)+32)//32*32
    if channels not in (1,2) or not 8000<=rate<=48000:raise ValueError('Invalid STM format')
    decoded=[];loop=None;loop_enabled=None
    for n in range(channels):
        header=64+n*96;count=u(raw,header);coefs=struct.unpack_from('>16h',raw,header+28)
        hist1,hist2=struct.unpack_from('>hh',raw,header+64)
        decoded.append(decode(raw[256+n*stride:256+(n+1)*stride],count,coefs,hist1,hist2))
        enabled=struct.unpack_from('>H',raw,header+12)[0]
        if enabled not in (0,1):raise ValueError('Invalid STM loop flag')
        current=(nibbles_to_samples(u(raw,header+16)),min(count,nibbles_to_samples(u(raw,header+20))+1)) if enabled else (0,count)
        if not 0<=current[0]<current[1]<=count:raise ValueError('Invalid STM loop bounds')
        if loop_enabled is not None and enabled!=loop_enabled:raise ValueError('Mismatched channel loop flags')
        if loop is not None and current!=loop:raise ValueError('Mismatched channel loops')
        loop=current;loop_enabled=enabled
    if len({len(x) for x in decoded})!=1:raise ValueError('Mismatched sample counts')
    pcm=array.array('h')
    if channels==1:pcm=decoded[0]
    else:
        pcm=array.array('h',[0])*(len(decoded[0])*channels)
        for n in range(channels):pcm[n::channels]=decoded[n]
    wav(output,pcm,rate,channels)
    sidecar=Path(str(output)+'.loop')
    if loop_enabled:sidecar.write_bytes(struct.pack('<II',*loop))
    else:sidecar.unlink(missing_ok=True)
    return {'source':str(input),'rate':rate,'channels':channels,'samples':len(decoded[0]),'loop':loop if loop_enabled else None,'loop_enabled':bool(loop_enabled),'conversion':'DSP ADPCM to PCM16; original rate, channels, and loop points retained'}

def sample_directory(path):
    raw=Path(path).read_bytes();out=[];o=0
    while o+32<=len(raw) and raw[o:o+4]!=b'\xff'*4:
        sid=struct.unpack_from('>H',raw,o)[0];offset=u(raw,o+4);rate=struct.unpack_from('>H',raw,o+14)[0]
        count,start,length,coef=u(raw,o+16),u(raw,o+20),u(raw,o+24),u(raw,o+28)
        if not count or not 8000<=rate<=48000 or start+length>count:raise ValueError('Invalid MusyX sample bounds/rate')
        if coef+40>len(raw):raise ValueError('Invalid MusyX coefficient table')
        out.append({'id':sid,'offset':offset,'rate':rate,'samples':count,'loop':[start,start+length],
                    'coefficients':list(struct.unpack_from('>16h',raw,coef+8))})
        o+=32
    if raw[o:o+4]!=b'\xff'*4:raise ValueError('Missing sample directory terminator')
    return out

def convert_sample(entry,bank,output):
    """Convert an indexed MusyX sample; loop units are PCM frames, end exclusive."""
    s=entry;size=((s['samples']+13)//14)*8
    data=bank[s['offset']:s['offset']+size]
    wav(output,decode(data,s['samples'],s['coefficients']),s['rate'],1)
    enabled=s['loop'][1]-s['loop'][0]>1
    sidecar=Path(str(output)+'.loop')
    if enabled:sidecar.write_bytes(struct.pack('<II',*s['loop']))
    else:sidecar.unlink(missing_ok=True)
    return {**{k:v for k,v in s.items() if k!='coefficients'},'channels':1,'loop_enabled':enabled,
            'loop':s['loop'] if enabled else None,'conversion':'MusyX DSP ADPCM to PCM16; original rate and sample ID retained'}

def sample(sdir,samp,sid,output):
    matches=[x for x in sample_directory(sdir) if x['id']==sid]
    if len(matches)!=1:raise ValueError('Sample ID not found')
    if Path(output).resolve() in (Path(sdir).resolve(),Path(samp).resolve()):raise ValueError('Output must not overwrite source')
    return convert_sample(matches[0],Path(samp).read_bytes(),output)
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('input',help='STM or MusyX SDIR input');p.add_argument('output')
    p.add_argument('--sample-bank',help='MusyX SAMP file');p.add_argument('--sample-id',type=int)
    a=p.parse_args()
    if a.sample_bank:
        if a.sample_id is None:p.error('--sample-id is required with --sample-bank')
        print(sample(a.input,a.sample_bank,a.sample_id,a.output))
    else:print(stm(a.input,a.output))
