#!/usr/bin/env python3
"""Original tagged NPC input adapter and unchanged DOL registration data views."""
import argparse,hashlib,json,re,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract

class Dol:
 def __init__(self,iso):
  self.disc=Path(iso).open('rb')
  if self.disc.read(8)!=b'G8ME01\0\0':self.disc.close();raise ValueError('US revision0 required')
  self.disc.seek(0x420);self.base=struct.unpack('>I',self.disc.read(4))[0];self.disc.seek(self.base);self.header=self.disc.read(256)
 def read(self,address,size):
  matches=[]
  for i in range(18):
   off,base,n=(struct.unpack_from('>I',self.header,p+4*i)[0] for p in (0,0x48,0x90))
   if base<=address and address+size<=base+n:matches.append(self.base+off+address-base)
  if len(matches)!=1:raise ValueError('Original data crosses DOL section')
  self.disc.seek(matches[0]);raw=self.disc.read(size)
  if len(raw)!=size:raise ValueError('Truncated original data')
  return raw
 def string(self,a):
  raw=bytearray()
  for i in range(256):
   c=self.read(a+i,1)[0];raw.append(c)
   if not c:return bytes(raw)
  raise ValueError('Unterminated original string')
 def close(self):self.disc.close()

def source_segments(iso):
 dol=Dol(iso);segments={};tables=[]
 def add(address,size):
  # PMEVT requires aligned spans. Retain actual source neighbors/padding,
  # never append synthetic zero bytes to an original string.
  start=address&~3;length=((address+size+3)&~3)-start
  if len(segments.get(start,b''))<length:segments[start]=dol.read(start,length)
 try:
  for name,address,size,stride,fields in [('npcTribe',0x803316d0,0x6ba0,0x54,(0,4,8,12,16,20,24,28,32,36,64,68)),('npc_ai_type_table',0x80338270,0x804,0x24,(0,)),('npc_define_territory_type',0x80416580,0x20,8,(0,))]:
   raw=dol.read(address,size);add(address,size);tables.append(dict(name=name,address=address,size=size,sha256=hashlib.sha256(raw).hexdigest()))
   for row in range(0,size,stride):
    if not struct.unpack_from('>I',raw,row)[0]:break
    for field in fields:
     pointer=struct.unpack_from('>I',raw,row+field)[0]
     if pointer:add(pointer,len(dol.string(pointer)))
  for address,size in [(0x802c3b10,len(b'BTLNO_INVALID\0')),(0x804207b4,4)]:add(address,size)
  if dol.read(0x802c3b10,len(b'BTLNO_INVALID\0'))!=b'BTLNO_INVALID\0' or dol.read(0x804207b4,4)!=bytes(4):raise ValueError('Unexpected original map NPC constants')
  metadata=dict(tables=tables,views=[dict(address=a,size=len(b),sha256=hashlib.sha256(b).hexdigest()) for a,b in sorted(segments.items())],policy='Unchanged original DOL data/strings only; no function bodies or fabricated BSS. Native parsed tag input uses a separate adapter.')
  return sorted(segments.items()),metadata
 finally:dol.close()

STRINGS=('name','tribe','aitype','territory_type','btl_setup_no')
NUMBERS=(('position',3),('dir',1),('territory_base',3),('territory_loiter',3),('territory_homing',3),('search_range',1),('search_angle',1),('homing_range',1),('homing_angle',1))
def parsed_records(raw):
 result=[];text=raw.decode('shift_jis')
 for ordinal,block in enumerate(text.split('<Npc>')[1:]):
  values={};position=0
  while True:
   position+=len(block[position:])-len(block[position:].lstrip())
   if block.startswith('</>',position):break
   match=re.match(r'<([a-z_]+)>\s*(.*?)\s*</>',block[position:],re.S)
   if not match:raise ValueError('Incomplete original NPC block')
   k,v=match.groups()
   if k in values:raise ValueError('Duplicate NPC tag field')
   values[k]=v;position+=match.end()
  if set(values)!=set(STRINGS)|{n for n,_ in NUMBERS}:raise ValueError('Incomplete or unsupported original NPC tag fields')
  strings=[];numbers=[]
  for k in STRINGS:
   value=values[k]
   if len(value)<2 or not value.startswith('"') or not value.endswith('"'):raise ValueError('Expected original quoted NPC field')
   value=value[1:-1].encode('shift_jis')
   if b'\0' in value or len(value)>=64:raise ValueError('Original parsed field exceeds64-byte adapter')
   strings.append(value)
  for key,count in NUMBERS:
   numbers.extend(float(v) for v in values[key].split())
   if len(values[key].split())!=count:raise ValueError('Wrong original NPC numeric arity')
  result.append((ordinal,strings,numbers))
 return result

def build(iso,areas=('gor_02',)):
 work=Path('local-assets/field-map-npc');work.mkdir(parents=True,exist_ok=True);Path('romfs').mkdir(exist_ok=True)
 for area in dict.fromkeys(areas):
  if not re.fullmatch('[a-z0-9_]+',area):raise ValueError('Unsafe map name')
  path=work/(area+'.s');extract(iso,'m/'+area+'/s',path);records=parsed_records(path.read_bytes());out=bytearray(struct.pack('<8sII',b'PMFMNPC1',len(records),392))
  for ordinal,strings,numbers in records:out+=struct.pack('<I17f',ordinal,*numbers)+b''.join(s.ljust(64,b'\0') for s in strings)
  output=Path('romfs/field-npc-map-'+area+'.bin');output.write_bytes(out)
  output.with_suffix('.json').write_text(json.dumps(dict(source=json.loads(path.with_suffix('.s.provenance.json').read_text()),records=len(records),stride=392,sha256=hashlib.sha256(out).hexdigest(),layout='Native parsed-input adapter, not an original source allocation; source ordinal/Shift-JIS strings/float32 words. Caller owns original parser/history gate.',license='Local original Nintendo/Intelligent Systems assets; no redistribution rights'),indent=2)+'\n')
  print('Original tagged NPC inputs:',area,len(records))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--area',action='append');a=p.parse_args();build(a.iso,a.area or ('gor_02',))
