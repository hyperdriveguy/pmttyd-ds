#!/usr/bin/env python3
"""Original DOL data spans for the tutorial packed battle services, never text."""
import struct
from pathlib import Path
from verify_harbor_tutorial_registry import verify

def segments(iso):
 verify(iso)
 result=[]
 with Path(iso).open('rb') as disc:
  disc.seek(0);header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  base=struct.unpack_from('>I',header,0x420)[0];disc.seek(base);h=disc.read(256)
  for i in range(7,13):
   off,address,size=[struct.unpack_from('>I',h,b+4*i)[0] for b in [0,0x48,0x90]]
   if i==11:size=min(size,0x80418000-address)
   if not off or size<=0:continue
   disc.seek(base+off);data=disc.read(size)
   if len(data)!=size or size%4 or address%4:raise ValueError('Invalid DOL data extent')
   spans=[(address,data)]
   # These live event-manager/provider words are mapped by EventNativeRuntime.
   for low,high in [(0x803d8530,0x803d8e70),(0x804151e8,0x804151f0),(0x80415fd0,0x80415fd4)]:
    split=[]
    for start,body in spans:
     end=start+len(body)
     if start<high and low<end:
      if start<low:split.append((start,body[:low-start]))
      if high<end:split.append((high,body[high-start:]))
     else:split.append((start,body))
    spans=split
   result.extend(spans)
  # Immutable audience scripts/item tables only, beyond the mutable sdata cutoff.
  # Whole battle_audience_kind_data.o sdata extent, source-pinned; no globals.
  audience_address,audience_size=0x8041c638,0x14e8
  for i in range(18):
   off,va,length=[struct.unpack_from('>I',h,b+4*i)[0] for b in [0,0x48,0x90]]
   if va<=audience_address and audience_address+audience_size<=va+length:
    disc.seek(base+off+audience_address-va);data=disc.read(audience_size)
    import hashlib
    if len(data)!=audience_size or hashlib.sha256(data).hexdigest()!='27efa23577efb6deb3204baf406fdda61546d5700ec4c25f04e8b59fad724cc4':raise ValueError('Original audience script data changed')
    result.append((audience_address,data));break
  else:raise ValueError('Unmapped original audience script data')
  # Original system wait/attack/damage streams sit beyond the bounded sdata
  # import above. Admit the three8-byte streams and their actual final label-scan
  # lookahead word. Never synthesize an end sentinel or import mutable globals.
  address,size=0x80419ef8,28
  for i in range(18):
   off,va,length=[struct.unpack_from('>I',h,b+4*i)[0] for b in [0,0x48,0x90]]
   if va<=address and address+size<=va+length:
    disc.seek(base+off+address-va);data=disc.read(size)
    if len(data)!=size:raise ValueError('Truncated original system hooks')
    result.append((address,data));break
  else:raise ValueError('Unmapped original system hooks')
 return result
