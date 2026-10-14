#!/usr/bin/env python3
"""Read-only original ordinary title narrow-fast/curtain/logo/input anchors."""
import argparse,csv,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
HASHES={'titleMain': '468c3133f2ac7d86d504b5afc6b779ad30e1a33778b15c86a4a3a7182f5f93d4', 'L_titleInit': 'f68b24d65e004262c5da1859295afaa866c89d9044240d33e271d0ed8b9e7ef6', 'disp2': '3e0a44e7b60eafb79e60d7d847eecb8cf49c8c42b76b6bd5d89e56d50a2bc70c', 'pressStartGX': 'c4b1888d3ff54158a7ca9e06e11ae20f755ba4b536a91035acd05973ac8e694c', 'fadeSetNarrowFast': '6dcebb7ac292f0a2c9fb668180ab1332d799a6f18ab258d6ceff853056502db6', 'fadeMain': '369db5633483e4b397234036dae4080d6e8849d68ea747daec990b41fd891437', 'fadeEntry': '34375b105173c33a535a1d3057bc51f8f7ef1a0b677970e5576df7ff495878ff', 'animPoseMain': '9083ebe5e58f3c2bd56914adfa318296291b5ca0ddc779d2bca7e6e3cbeebd26', 'disp_maku': '75fbec4b1248b4c8f24a4c93269f01e98bbf990fa60235378ad1cf6cf35c850b'}
HASHES.update({'psndBGMOff_f_d': '38f527b79c239c86be3d5c7f82d3f69213bae597de4b7cb7ae03aca5b5bffc52', 'psndBGMMain': '8cc019f59869a9d519e40598474d996f1d9ebbb0688ac2f3c5e92c88c32cbdba', 'psndBGMOn_f_d': '7b3fe9b3bb5a74e1cb174d0a14e460621d4c922d55d95884a27926f01eb3bf0e', 'psndBGMStartCheck': '311665515b1885d37d9070afc2ad66d155dc2ca7695d3c39e47068ce95223a94', 'SoundSSFadeoutCh': '93458b146a157baacf6b66520b935de3a401ed49ae842168c02a8497e860afd2', 'SoundSSMain': '93f13bd7f43bcc570277a16e59963012648c2e2ed631754e6a3105feed04e88c', 'SoundSSPlayChEx': 'e3d25ca4426d17dc1a5122f29509fb612a46cfed8992f1f9968d5a4a5650806a'})
def verify(iso,symbols):
    rows=list(csv.DictReader(Path(symbols).open()))
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);h=f.read(256)
        sections=[tuple(struct.unpack_from('>I',h,o+4*i)[0]for o in [0,0x48,0x90])for i in range(18)]
        def read(address,size):
            o,b,l=next(s for s in sections if s[1]<=address and address+size<=s[1]+s[2]);f.seek(dol+o+address-b);return f.read(size)
        for name,digest in HASHES.items():
            r=next(r for r in rows if r['area']=='_main'and r['name']==name)
            if hashlib.sha256(read(int(r['ram_addr'],16),int(r['size'],16))).hexdigest()!=digest:raise ValueError('Original '+name+' changed')
        anchors={0x8041f814:'404000003fc00000',0x8041f80c:'415f3200',0x8041f824:'415f31005a5f31005a5f3200',0x8041f5d0:'43960000c3960000',0x8041f5b0:'42f0000043e58000c2f40000',0x8041f5cc:'c32a0000',0x8041f5d8:'3dcccccd',0x8041f598:'c3480000',0x8041f5dc:'43c80000c3c80000',0x80315af8:'802cc74080000000802cc74c58400101',0x80304790+4*4:'8001389c',0x80304790+12*4:'8001390c',0x80304790+41*4:'80013880'}
        for a,expected in anchors.items():
            if read(a,len(expected)//2).hex()!=expected:raise ValueError('Original title constant/dispatch changed')
        for a,text in [(0x802bf668+0xc0,b'OFF_d_maku_center\0'),(0x802bf668+0xa8,b'OFF_d_maku_left_right\0')]:
            if read(a,len(text))!=text:raise ValueError('Original title bank changed')
    return len(HASHES)+len(anchors)+2
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print(f'{verify(a.iso,a.symbols)} original title anchors verified read-only')
