#!/usr/bin/env python3
"""Read-only US command-pose whole-body and priority-table fingerprints."""
from pathlib import Path
import hashlib,struct
from iso_assets import DEFAULT_ISO
SPANS=[(2148689100, 140, '379c3bde84ad840c289fa26ecff3da61db32f5bf01cff107e7d5329542af9378'), (2148544644, 116, 'cb3b3eb0cb1e2d5c86dfb3b79cb5f26e39ec00630aebd2e6a3da0a96ddec6a62'), (2148536564, 56, '7a4b6196e4317bdd469222f296d25b540271f3b51d90678070d4166c78ad1204'), (2148693676, 128, '723a150ce51f865fb68f3188c1eafa55258221318290f673e166dae3f8032dad'), (2148536680, 148, '42b9a3e555f3bab2bbedea23ddf1d5d0602374a820d78c4c903a29b0966d93c3'), (2148699288, 64, '21ed3890883c9f9c73389ff655cd579de1edc7fd1aabc7dce0101ecc93ff811e'), (2148699352, 64, '48999a2b5a78150fbf21f4023e22a2819aa6126a8a6adc06f7e179104a778539'), (2148693804, 128, 'dd8be61c8b2829232e57fd7eab159cbefe61c54ba7869ad4083da060f247492b')]
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as f:
  if f.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  f.seek(0x420);d=struct.unpack('>I',f.read(4))[0];f.seek(d);h=f.read(256)
  def read(a,n):
   for i in range(18):
    o,b,s=[struct.unpack_from('>I',h,x+4*i)[0] for x in [0,0x48,0x90]]
    if b<=a and a+n<=b+s:f.seek(d+o+a-b);return f.read(n)
   raise ValueError('Unmapped original address')
  for a,n,expected in SPANS:
   if hashlib.sha256(read(a,n)).hexdigest()!=expected:raise ValueError('Body mismatch '+hex(a))
  if read(0x80416ab0,32)!=struct.pack('>8i',27,2,9,1,4,5,3,-1):raise ValueError('Status priority mismatch')
  word=lambda a:struct.unpack('>I',read(a,4))[0]
  kind=word(0x80416a50);table=word(word(kind+0xb8)+0x48)
  while True:
   tag=word(table)
   if tag>=70:raise ValueError('Missing original command pose')
   if tag in (67,69):break
   table+=8
  if read(word(table+4),6)!=b'M_C_3\0':raise ValueError('Original command pose name mismatch')
 return dict(bodies=8,priority=[27,2,9,1,4,5,3],scope='Static source; no original execution')
if __name__=='__main__':print(verify())
