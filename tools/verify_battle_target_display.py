#!/usr/bin/env python3
"""Pin original whole target display, every linked child and read constants."""
import argparse, hashlib, struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS = {'0x8001a918': {'size': 36, 'sha256': '140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d'}, '0x80075528': {'size': 36, 'sha256': '80a8f34948ea70c05ca31348c2e0f08bef5c0e5b899d77a2495404dc4bb7b01b'}, '0x800773d0': {'size': 104, 'sha256': 'ef1f387006006d59d37f2cb505442bfd24bc69433426d29031ec585047b69d25'}, '0x80077b4c': {'size': 144, 'sha256': '9cd10925bd73263fdf1feb2e122e030aa6ce00a54e86ac67f46cca6a6bf1e8e3'}, '0x8007b44c': {'size': 1676, 'sha256': 'fe1dc4fcff76fed45e6d185b88cc128b9ea57b31167cff0b7c10d22a29a18c02'}, '0x80081b8c': {'size': 224, 'sha256': 'ee2ba7bc44a8d684cc45162e620790ba4698524a3d0205613bdd30cf4c83565e'}, '0x800d2db4': {'size': 72, 'sha256': '66e95f3a70c87c482999f97efb58986105b8dcce47702883ec5a72d1c48daa9f'}, '0x800f88cc': {'size': 60, 'sha256': '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'}, '0x801009b4': {'size': 152, 'sha256': '4c37f7a1ca33a927980eb5d53717b8d0b8df830a04d1e76b39393ade9aab13de'}, '0x80120a44': {'size': 1440, 'sha256': '41d904d1c5a77b59a0f63c9b7e24ab30fb94c1ee0b17070b08d7a77cb28ad9c3'}, '0x80124cb8': {'size': 272, 'sha256': '503374044e2b5599babc1e0f64c05b7338460ecca99246067924e0310ad2049b'}, '0x80128cd8': {'size': 64, 'sha256': '48999a2b5a78150fbf21f4023e22a2819aa6126a8a6adc06f7e179104a778539'}, '0x8012c028': {'size': 132, 'sha256': '4b894e0aee993e8e3f8d099f3d3db18fe0d4cb49987d8c073f3ab684ce3c8c5f'}, '0x802efe78': {'size': 16, 'sha256': '6c1e5bd5fd6010fd447a298fb6c7e30190d7294583b11d4e671d5dea51b965ec'}, '0x804225b0': {'size': 16, 'sha256': 'b424ecfc6bdd97a2e172a6820366bb6c3b0eaec8fd85ed4fe9ba195652cf5e89'}, '0x804225cc': {'size': 4, 'sha256': 'b9e69238a01189ddad0c70c62fc04d362b55ee6eefaa558dd13fc45ac1d665be'}, '0x804225d8': {'size': 4, 'sha256': 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'}, '0x804225dc': {'size': 4, 'sha256': 'de620abd6d3615746360c1d15ce3a56291236d37424124054a49d25e947ffc4d'}, '0x804225f4': {'size': 4, 'sha256': '46f83e3a0dcc4f04f3eecc056ca777afc551ff50c61ae15f1486e3497cd51bfb'}, '0x80422610': {'size': 4, 'sha256': 'e828c9a7b4066dc6355f7a36266709e0dc4ecd3cc10ff0c82d00d19b443fab74'}, '0x80422614': {'size': 4, 'sha256': '257e018fa43f2263d088d600e54473f37ca4ee6ff248baf2258b60630528571d'}, '0x80422618': {'size': 4, 'sha256': 'c85db1dac2e3ab55c39af639eebe32fb19a81e520af19e5d6cdefa61bbeebac2'}, '0x8042261c': {'size': 4, 'sha256': '75cb7daee7462ed75eb816f631e7021934137b9568291c878c211e783133b8ca'}, '0x80422620': {'size': 4, 'sha256': 'e300c2211a74cdfa8ba96c816589a503a5ef108434c14fe16199ab87f501f0d3'}, '0x80422624': {'size': 4, 'sha256': 'cdbde07fa06718980f87ed64790135afe5babca97c0633c9948bb61117c5dc64'}, '0x80422628': {'size': 4, 'sha256': '9d34e827aabe81eb5d45f0be088dbca972d09556b52ac6867a7e9392df8c3f9b'}}
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US revision0 disc required')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  for token,pin in PINS.items():
   address=int(token,16);locations=[]
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
    if base<=address and address+pin['size']<=base+size:locations.append(dol+off+address-base)
   if len(locations)!=1:raise ValueError('Unmapped/ambiguous original span: '+token)
   disc.seek(locations[0]);raw=disc.read(pin['size'])
   if hashlib.sha256(raw).hexdigest()!=pin['sha256']:raise ValueError('Original target display changed: '+token)
 return {'whole_parent_bytes':1440,'pinned_spans':len(PINS),'scope':'Static original source pins; no native GPU or gameplay completion receipt'}
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);print(verify(parser.parse_args().iso))
