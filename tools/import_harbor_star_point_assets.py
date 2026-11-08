#!/usr/bin/env python3
"""Empty-destination original StarPoint numeric source import, read-only ISO."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
LICENSE="Nintendo/Intelligent Systems original game asset; no redistribution license granted"
PINS=[(2149626100, 804, '1a9943cc83ec84427d32df6cbe885f05a39548a889bc03f17271f8cb79fa9ee4'), (2149625712, 388, 'ef3f22f3c705d5c413c89ca7227df31b405858f1ba85400e6831aba0b0a9dfc1'), (2149625484, 228, 'e79c3335888d1cfdcce154b0597737b0137416f90848c212a2bb640d68d7d2ec'), (2147932528, 2572, '0a7b5b58e7d416b4518e3c48e461483d12826a6248009e9b60ab93a8de62fd0f'), (2150023972, 32, '6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5'), (2151837448, 56, 'd17a2540554bc0e5536e2e49d7326bd21cbed7e15e6c2cad11570adfdfd25e2c'), (2150622528, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), (2151809852, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2147870880, 136, '0fd4662fef83e21bee80f9c5340f39a8398f0b4ba6511d9412a3da80bca509bd'), (2150668400, 8, '7ed2deeedfdd3ea457665eb36e05eb6a0ad23c65ba61ad6d229ac875d1b2327c'), (2150374724, 12, 'ae68ba5dc9cd09145a49635daf4289ab387c9e8ae70201c8fe9569d0d787d0a4')]
def sha(b):return hashlib.sha256(b).hexdigest()
def build(iso,destination):
 destination=Path(destination)
 if destination.exists()and(not destination.is_dir()or any(destination.iterdir())):raise ValueError("Import destination must be empty")
 with Path(iso).open('rb')as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  sections=[tuple(struct.unpack_from('>I',header,k+4*i)[0]for k in(0,0x48,0x90))for i in range(18)]
  records=[];segments=[]
  for address,size,digest in PINS:
   positions=[dol+o+address-b for o,b,n in sections if b<=address and address+size<=b+n]
   if len(positions)!=1:raise ValueError('Source mapping differs')
   disc.seek(positions[0]);raw=disc.read(size)
   if sha(raw)!=digest:raise ValueError('Original witness differs')
   records.append(dict(address=hex(address),bytes=size,sha256=digest,iso_offset=positions[0],license=LICENSE))
   if address in(0x80426f08,0x802fe540,0x8042033c):segments.append(dict(address=address,bytes=raw.hex()))
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'memory.json').write_text(json.dumps(dict(schema=1,segments=sorted(segments,key=lambda x:x['address']),license=LICENSE),indent=2)+'\n')
 (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0 read-only',spans=records,importer_sha256=sha(Path(__file__).read_bytes()),conversion='Unchanged original numeric source spans, no synthesized values or executable payload.',license=LICENSE),indent=2)+'\n')
 (destination/'LICENSE.txt').write_text(LICENSE+'\nTracking original data grants no new license.\n')
 (destination/'README.md').write_text('# Original StarPoint numeric data\n\nUnchanged original scalar/bias/empty-literal source spans. No replacement artwork.\nNative display reuses original shared icon404 (images418..421).\n')
 return records
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();print('Verified/imported',len(build(a.iso,a.destination)),'source blocks')
