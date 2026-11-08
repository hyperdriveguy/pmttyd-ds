#!/usr/bin/env python3
"""Empty-destination original KemuTest4 image/data import; ISO read-only."""
import argparse,hashlib,json,struct,tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory,convert_tpl
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SOURCE='e/us/effect.tpl'
SOURCE_SHA='3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b'
# Full original constructor/update/draw and finite source-name lookup witnesses.
BODIES=[(2147932528, 2572, '0a7b5b58e7d416b4518e3c48e461483d12826a6248009e9b60ab93a8de62fd0f'), (2147870880, 136, '0fd4662fef83e21bee80f9c5340f39a8398f0b4ba6511d9412a3da80bca509bd'), (2149637748, 3580, 'd7c79d0aa0b31a014a2d4d9fe7736c6d71426dbbe95ee16e2504d44170fe3306'), (2149635668, 2080, '5eef7d8e5535b53f2d88cfa6f13b950c0f460c9b7f53aa38821080eebe90ecf5'), (2149633828, 1840, '308e3375b7c6469163fc9e40d10d559ced59060082d9ffc779fd17ce78820f4b')]
SPANS=[(2151784592, 80, 'aac4a7378e58ca2e15bcb72125a147e9e5a2ec02a5f809c548a408c532e5ca09'), (2151837680, 128, '3cc33f567326edc5b073bf0e0fda7e6f98699b654c5681b269d1663851a84296'), (2151346400, 92, '0a71e539d9a909d2dc15f44d58304cf5307d5d5e4d1cf85ffe239d35c191ef85'), (2151346308, 92, '6e7bf66842c759557c284043af0cf7fd95659288e5f9cc666f30962aef1e14d3'), (2151346220, 88, 'c1a593ba13be388e52056c6afaacd6b8733478285cb24ee3c642fad6430d32f1'), (2151346156, 64, '27c0f5a8eba64bbca862a3eed3d558443b0c6d9965f5d20c429aa9d45b14b6fe'), (2151346104, 52, 'e801bcf1043e993e258ad09f32329505313f11d604d7e956e58cf9d6ba48fe3c'), (2150668424, 8, '7ea863ea0efbf6e266ff9f1e513295b03c514fb03c24b43b34d4cf861d318376')]
def sha(data):return hashlib.sha256(data).hexdigest()
def build(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):raise ValueError('Import destination must be empty; editable imports are preserved')
 entry=next(e for e in inventory(iso)['files'] if e['path']==SOURCE)
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  disc.seek(entry['offset']);tpl=disc.read(entry['size'])
  if sha(tpl)!=SOURCE_SHA:raise ValueError('Original effect bank differs')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  sections=[tuple(struct.unpack_from('>I',header,k+4*i)[0]for k in (0,0x48,0x90))for i in range(18)]
  records=[];segments=[]
  for address,size,digest in BODIES+SPANS:
   offsets=[dol+o+address-b for o,b,n in sections if b<=address and address+size<=b+n]
   if len(offsets)!=1:raise ValueError('Source mapping differs')
   disc.seek(offsets[0]);raw=disc.read(size)
   if sha(raw)!=digest:raise ValueError('Original source witness differs')
   records.append(dict(address=hex(address),bytes=size,sha256=digest,iso_offset=offsets[0],license=LICENSE))
   if (address,size,digest) in SPANS:segments.append(dict(address=address,bytes=raw.hex()))
 # Decode original pixels only; no generated smoke/noise or blended replacement.
 with tempfile.TemporaryDirectory(prefix='ttyd-kemutest-') as temporary:
  raw=Path(temporary)/'effect.tpl';raw.write_bytes(tpl)
  png=Path(temporary)/'image.png';convert_tpl(raw,png,40);pixels=png.read_bytes()
 descriptor,=struct.unpack_from('>I',tpl,12+40*8)
 if struct.unpack_from('>HHI',tpl,descriptor)!=(32,512,0):raise ValueError('Original image40 domain differs')
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'smoke40.png').write_bytes(pixels)
 (destination/'memory.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 provenance=dict(source='US G8ME01 disc0 revision0 read-only',texture=dict(source_path=SOURCE,iso_offset=entry['offset'],bytes=len(tpl),sha256=SOURCE_SHA,image=40,descriptor_offset=descriptor,descriptor_sha256=sha(tpl[descriptor:descriptor+36]),png_sha256=sha(pixels),license=LICENSE),importer_sha256=sha(Path(__file__).read_bytes()),decoder_sha256=sha(Path(convert_tpl.__code__.co_filename).read_bytes()),spans=records,conversion='Original I4 tiled pixels to editable512x32 RGBA, no resize. Original numeric source data retained at actual addresses. Dual adjacent-frame blend remains live native GPU material, never baked replacement art.',license=LICENSE)
 (destination/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 (destination/'LICENSE.txt').write_text(LICENSE+'\nTracking these original pixels/data grants no new license.\n')
 return provenance
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
