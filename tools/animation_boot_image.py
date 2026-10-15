#!/usr/bin/env python3
"""Original cold animation/heap image: initialized DOL sections override BSS."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
# Original US revision0 data identities. Runtime arena bounds remain inputs.
SPANS = [('animation-work', 2151519344, 272, 'e4d879a3407de578f579dfab4366fcea75a6649c683d9efe4f056f6505437574'),
 ('tangent', 2151519616, 712, '283de789c3f9ae6115e627ecb921b7b39bdaa1b82289eca5e60da0b76d07a502'),
 ('work-pointer', 2151767488, 4, '47adea7a55a1a631828dd37092a4bd24dbb061d13999ac2fa641e6aaffb7666c'),
 ('vivian-destination',
  2150652160,
  3072,
  'e29b9399833a190005ca52b06b6cd88683c475af934549d62ef1bb42cf843f8f'),
 ('vivian-tail', 2151767456, 16, '8f29ecda1efca336bf3f902e47a521611bdd317973facd64a46bae0cb83f4781'),
 ('vivian-dest-pointer', 2151767476, 4, 'd6919d7a9b23b27da93b5145ac81ae2ca1078f4a3afc0dd670d25acd181394ce'),
 ('angle-factor', 2151807924, 4, '17edd7630f30d7845e83f4ce5f4ef116949a2264569382e765cf3bbf66d2983d'),
 ('integer-double-bias', 2150371440, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'),
 ('vivian-path', 2150371800, 16, 'acfecff42d7443034cfb15d497b98c9c8599524325b610b40a490ea633774b1e'),
 ('ag2tg-path', 2150371816, 16, 'fd58d623184ed46989d4008f82d6cef9eb4cb59ac71a8cb36414e62b5da18e81'),
 ('heap-config', 2151764408, 40, '84b34f8d85f9017dded4f445b63c02af16af70baa05a77c2ee9e935bc676154b'),
 ('heap-map-globals', 2151802888, 80, '5b6fb58e61fa475939767d68a446f97f1bff02c0e5935a3ea8bb51e6515783d8'),
 ('heap-current-arena', 2151801896, 12, '9d4b936a67c21ab14da09aa480239b43ea122c9c933d876013ee3012e464c16b'),
 ('sdk-arena-globals', 2151805040, 20, 'de47c9b27eb8d300dbb5f2c353e632c393262cf06340c4fa7f1b40c4cbd36f90')]
# Additional original Entry defaults, formats and live sort storage.
SPANS += [('entry-colors', 2151807756, 8, '12a3ae445661ce5dee78d0650d33362dec29c4f82af05e7e57fb595bbbacf0ca'), ('entry-one-zero', 2151807780, 8, 'e416065dd951359ab79d557107324f0f8fb0142494f2523ca9fdac35f1ac3d18'), ('entry-model-format-prefix', 2151807796, 12, 'd4e6d20a3d26563a68bab4cbc39c7f01d80e9490444a91696460f6271d8f4556'), ('entry-texture-format', 2150371432, 8, 'bda93144bb3a69b5bd1541dc0a8975bcced82e3b7399a2b532c9e043dd6d864d'), ('entry-sort-strings', 2150371752, 48, '009683c1ef3a549e20527d41aea567922b1f26b0dc59d972001f444f44efa094'), ('entry-default-sort', 2151767492, 4, 'ad95131bc0b799c0b1af477fb14fcf26a6a9f76079e48bf090acb7e8367bfd0e')]

# Original normal Mario creation tokens/triplet, party model and local-time factor.
SPANS += [('normal-model-tokens', 2150373536, 28, '6b4bb10db518b91cca6e508d2d692407debb0201f9dddb0515c63864d708daae'), ('normal-triplet', 2151767728, 12, '8a7a8b512cf150345386614e31c586a9304ce2c1d3358bb02f56d1cc6451e838'), ('goombella-token', 2151814112, 8, '18542b0684783d3889890a9fb3f5ba38e410abb8eae4d350abfe1bb6f49a8efb'), ('frame-to-ms', 2151807808, 4, '23a596e4559d2e7a848de9041c64fe7e961317ffb3aba9390759092b780dbdce')]

# Original shared animation Main/Inner camera-facing literals.
SPANS += [('draw-radians',0x8041fb2c,4,'7388adb66bc2fb8cdd730d8148adcae18abd8bb48b05b7bd9b9e05efa6c09702'), ('paper-facing',0x8041fb6c,0x2c,'18c2acf85532159d4157b603544711245e68b9f6fd3132b106a3802ae739555b')]

# Original Main clock/frame literals.
SPANS += [('main-frame-clock',0x8041fba4,8,'ddc0adc8aa32f0760b2cecfb24f4467cc09eff8318c59d91bfb09c2ba30e2057')]

def build(iso=DEFAULT_ISO,output='romfs/animation-boot'):
 root=Path(output);root.mkdir(parents=True,exist_ok=True)
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Expected original US revision0')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  sections=[(struct.unpack_from('>I',header,4*i)[0],struct.unpack_from('>I',header,0x48+4*i)[0],struct.unpack_from('>I',header,0x90+4*i)[0]) for i in range(18)]
  bss,size=struct.unpack_from('>II',header,0xd8);segments=[];provenance=[]
  for name,a,n,digest in SPANS:
   data=bytearray();runs=[]
   for address in range(a,a+n):
    matches=[(i,off+address-va) for i,(off,va,length) in enumerate(sections) if length and va<=address<va+length]
    if len(matches)>1:raise ValueError('Overlapping initialized DOL sections')
    if matches:
     section,offset=matches[0]
     if section<7:raise ValueError('Executable bytes are not startup data')
     disc.seek(dol+offset);raw=disc.read(1)
     if len(raw)!=1:raise ValueError('Truncated original data')
     data+=raw;kind='data';where=dol+offset
    elif bss<=address<bss+size:kind='bss-zero';where=None;data.append(0)
    else:raise ValueError('Unbacked original startup address')
    if runs and runs[-1]['kind']==kind and (where is None or runs[-1]['iso_offset']+runs[-1]['size']==where):runs[-1]['size']+=1
    else:runs.append(dict(address=address,size=1,kind=kind,iso_offset=where))
   if hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Original startup image mismatch: '+name)
   segments.append((a,bytes(data)));provenance.append(dict(name=name,address=a,size=n,sha256=digest,runs=runs))
 segments.sort();offset=12+12*len(segments)
 out=bytearray(struct.pack('>8sI',b'PMEVT001',len(segments)))
 for a,data in segments:out+=struct.pack('>III',a,len(data),offset);offset+=len(data)
 out+=b''.join(data for a,data in segments)
 (root/'memory.pmevt').write_bytes(out)
 meta=dict(schema=1,source='US GameCube revision0 DOL initial data/BSS',dol_offset=dol,bss_address=bss,bss_size=size,spans=provenance,pack_sha256=hashlib.sha256(out).hexdigest(),policy='Platform supplies valid runtime arena bounds; initialized sections take precedence over broad BSS extent',license='Nintendo/Intelligent Systems originals; local use only; no redistribution rights')
 (root/'memory.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs/animation-boot');a=p.parse_args();build(a.iso,a.output)
