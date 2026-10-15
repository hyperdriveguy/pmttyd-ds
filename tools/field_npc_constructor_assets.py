"""Unchanged original NPC constructor constants; no BSS/pose success fixtures."""
import hashlib,struct
from pathlib import Path
SPANS=[(0x802c11f8,0x18,'6978ebe9f7e090d0e767ba17f3106b4c895fbe78242e44e3fea38c52ee1cfcd1'),
 (0x802c1468,8,'4754b9eb62a985deb2a5e44e47b9b6954d4c55e4d2dbdffbc18e22140bb33a5a'),
 (0x8041fc6c,0x2c,'9c05f57573a57078eea131c73d8dc843f23de0aad70b6ab61c5f8c9d1801c56a'),
 (0x8041fd6c,0x10,'28b068be85c0b6d1e8aac3827c502178c46ffc54bbbba1c37f88718a50afa129'),
 (0x8041fb64,4,'c31425b44a7ed682a3356bf6820141e1584a15c43b6d6793f552e5d4621586f7')]
def constructor_segments(iso):
 segments=[];metadata=[]
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  for address,size,expected in SPANS:
   matches=[]
   for i in range(18):
    offset,va,length=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
    if va<=address and address+size<=va+length:matches.append(offset+address-va)
   if len(matches)!=1:raise ValueError('Missing original constructor constant span')
   disc.seek(dol+matches[0]);data=disc.read(size);digest=hashlib.sha256(data).hexdigest()
   if digest!=expected:raise ValueError('Unexpected original NPC constructor bytes')
   segments.append((address,data));metadata.append(dict(address=address,size=size,sha256=digest))
 return segments,dict(dol_views=metadata,policy='Unchanged source constants/strings only. No fabricated BSS registry or pose/resource readiness; caller owns native allocation and source startup state.')
