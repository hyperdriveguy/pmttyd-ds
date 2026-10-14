#!/usr/bin/env python3
"""Exact original namespace-qualified Bero tables; no executable PPC included."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_event_inventory import read_bindings

def build(iso=DEFAULT_ISO):
 work=Path('local-assets/bero-info');work.mkdir(parents=True,exist_ok=True);source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
 digest=hashlib.sha256(raw).hexdigest()
 if digest!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected original US REL')
 u=lambda o:struct.unpack_from('>I',raw,o)[0]
 sections=[(u(u(16)+8*n)&~3,u(u(16)+8*n+4),0x90000000+n*0x100000) for n in range(u(12))]
 def addr(o):
  found=[a+o-b for b,size,a in sections if b and b<=o<b+size]
  if len(found)!=1:raise ValueError('Invalid REL address')
  return found[0]
 local,imports,foreign=read_bindings(raw);data={};tables=[];spans=[]
 def put(address,blob):
  for n,byte in enumerate(blob):
   if address+n in data and data[address+n]!=byte:raise ValueError('Conflicting source data')
   data[address+n]=byte
 def relspan(start,size):
  begin=start&~3;end=(start+size+3)&~3;blob=bytearray(raw[begin:end])
  for site,b in local.items():
   if begin<=site<end:struct.pack_into('>I',blob,site-begin,sections[b['section']][2]+b['addend'])
  for site,value in imports.items():
   if begin<=site<end:struct.pack_into('>I',blob,site-begin,value)
  if any(begin<=site<end for site in foreign):raise ValueError('Foreign Bero table binding')
  put(addr(begin),blob)
 with Path('artifacts/reference/us_symbols.csv').open() as f:rows=list(csv.DictReader(f))
 for room,name in enumerate(['gor_01','gor_00','gor_02','gor_03','gor_04']):
  matches=[r for r in rows if r['area']=='gor' and r['namespace']==name+'.o' and r['name']=='bero_entry_data']
  if len(matches)!=1:raise ValueError('Ambiguous original Bero namespace')
  row=matches[0];offset,size=int(row['file_addr'],16),int(row['size'],16)
  if size%60 or not 120<=size<=17*60:raise ValueError('Invalid Bero table extent')
  relspan(offset,size);entries=[];summary=json.loads((Path('local-assets')/('map' if room==0 else name)/'map-summary.json').read_text())
  for n in range(size//60):
   start=offset+n*60;binding=local.get(start);pointer=binding['file_offset'] if binding else None
   if pointer is None:
    if u(start) or n!=size//60-1:raise ValueError('Invalid source table sentinel')
    break
   for field in (0,40,44):
    binding=local.get(start+field);text=binding['file_offset'] if binding else None
    if text is not None:
     end=raw.index(b'\0',text);relspan(text,end-text+1)
   entryname=raw[pointer:raw.index(b'\0',pointer)].decode('ascii');hits=[v['source_node'] for v in summary['collision_nodes'] if v['name']==entryname]
   if len(hits)>1:raise ValueError('Ambiguous Bero hit identity')
   entries.append(dict(name=entryname,address=addr(start),file_offset=start,source_node=hits[0] if hits else 0))
  tables.append(dict(room=room,namespace=name+'.o',address=addr(offset),file_offset=offset,size=size,entries=entries))
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  def dolread(address,size):
   found=[]
   for n in range(18):
    off,va,length=(struct.unpack_from('>I',header,b+4*n)[0] for b in (0,0x48,0x90))
    if va<=address and address+size<=va+length:found.append(off+address-va)
   if len(found)!=1:raise ValueError('Invalid DOL span')
   disc.seek(dol+found[0]);blob=disc.read(size);put(address,blob);spans.append(dict(address=address,size=size,sha256=hashlib.sha256(blob).hexdigest()));return blob
  dolread(0x8030cd78,60);prefix=dolread(0x802c2c28,32);dolread(0x804204c0,32);dolread(0x80416060,20)
  if struct.unpack('>8I',prefix)!=tuple(0x804204c0+4*n for n in range(8)):raise ValueError('Original direction prefix table changed')
 # Merge adjacent original bytes; all spans have original four-byte boundaries.
 segments=[]
 for address in sorted(data):
  if segments and address==segments[-1][0]+len(segments[-1][1]):segments[-1][1].append(data[address])
  else:segments.append((address,bytearray([data[address]])))
 pos=12+12*len(segments);table=bytearray();payload=bytearray()
 for address,blob in segments:table+=struct.pack('>III',address,len(blob),pos);payload+=blob;pos+=len(blob)
 out=Path('romfs');out.mkdir(exist_ok=True);(out/'bero-info.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
 descriptor=bytearray(struct.pack('<8s4I',b'PMBRIO01',len(tables),0x8030cd78,0x80416060,0x802c2c28))
 for t in tables:
  descriptor+=struct.pack('<3I',t['room'],t['address'],len(t['entries']))
  descriptor+=b''.join(struct.pack('<I',e['source_node']) for e in t['entries'])
 (out/'bero-info.bin').write_bytes(descriptor)
 (out/'bero-info.json').write_text(json.dumps(dict(source='rel/gor.rel',source_sha256=digest,tables=tables,dol_spans=spans,policy='Original table/data pointers retained; referenced event/PPC bodies excluded except original20-byte default entry EVT',license='Original Nintendo/Intelligent Systems resources; local use only, no redistribution rights'),indent=2)+'\n')
 print('Original Bero info:',len(tables),'tables,',sum(len(t['entries']) for t in tables),'entries,',pos,'bytes')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
