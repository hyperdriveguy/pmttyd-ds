#!/usr/bin/env python3
"""Pin read-only US Noise/source helper and native sound-chain evidence."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS = [('NoiseAlways', '0x801a0b88', '0x164', 'e74827a007ccb0d6896b7655bc507f7e14710bdedeb87ab01ae914c5a1125afa'), ('AudienceBase', '0x801a86a0', '0xc', '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('SoundPointer', '0x801a85bc', '0x40', 'b5cb35bbd5cda0492e956f42a0e5df6e743e988f41b7d1f720a963e6e2b794cf'), ('ActorPointer', '0x801a8664', '0x3c', 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('Exist', '0x801a29c0', '0x64', '21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d'), ('PPAudienceExists', '0x801a1fa0', '0x7c', 'a025411304a9eb0c7244a2a237530021609b7f931cd67de7e6e8353f7252c0b4'), ('HalfCount', '0x801a201c', '0x74', '95fe94aff68132e76744cb73b61d1f671e7b048a932b0c927bf6dfeda5252046'), ('LeftCount', '0x801a2090', '0x24', '26c8c347827f7f658fe15fc41442d79c487e2828cc82a3b4b6b1cb158c296198'), ('RightCount', '0x801a20b4', '0x24', '06c8b223ecadab8adadf34b8eb26cb10818279b1dcb7220061fd815415406857'), ('Sound1', '0x8019fcec', '0x5c', '55ff9d0c5738ce89e4fe57b47c5d27900dc099cbdeacef4d57a25ad3c7247b73'), ('SoundInfo1', '0x8019fbe8', '0x104', '015372ad3e3459acf08bd9d5b0f3b753d0fa6c071bec2349fd8c23406de5580c'), ('Strcpy', '0x8026d5fc', '0xb8', 'd68e2a1981b1703890d7213032425b317c6d466dc5c88d957e473f2cc126426d'), ('PublicGetVolume', '0x800d9558', '0x48', '83cadc6934d839c3afdf14dbee52f1262ab40b8991df54fb91eabd054e4eb8da'), ('PublicSetVolume', '0x800d97d0', '0x38', '9d478d41ed735650da5a1638dd94cea0326e697eb86a6d5dc7c4d44dd3e49e23'), ('PublicOn3D', '0x800d9848', '0x3c', '29fdab1a516235c9bfbb11635067a2f53e115e54d41985990fbd5e09b6d1e971'), ('PublicOn', '0x800d9934', '0x540', '3a3a312954b07a93a0468dd3edfc5f1d2110d9b92e36a8aff2a9d46caec22e71'), ('OriginalNoiseNames', '0x802fa574', '0x50', 'a44e04c8440dd6b97bbadf3397f33a9e9e130b9eeb9a4e0293dc96cdbd18e245'), ('SourceFloatScalars', '0x80424988', '0x40', 'e55fde47393d310b6248421510fbb37e69f58f2d6e6c8a158e5e82eb479c022c'), ('SignedUnsignedDoubleBiases', '0x802fa328', '0x10', '6c1e5bd5fd6010fd447a298fb6c7e30190d7294583b11d4e671d5dea51b965ec'), ('FirstSoundRow', '0x80316b28', '0x14', '32d7470af440ef3b49893395cf85a064c58d5d3506e13a47f679d2388944ce51'), ('OriginalEmptyName', '0x804218c8', '0x1', '6e340b9cffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision0 disc required')
        f.seek(0x420)
        dol,=struct.unpack('>I',f.read(4))
        f.seek(dol)
        h=f.read(256)
        def read(address,size):
            for i in range(18):
                off,base,n=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
                if base<=address and address+size<=base+n:
                    f.seek(dol+off+address-base)
                    data=f.read(size)
                    if len(data)!=size:
                        raise ValueError('Truncated original source')
                    return data
            raise ValueError('Unmapped original source')
        for name,address,size,digest in SPANS:
            if hashlib.sha256(read(int(address,16),int(size,16))).hexdigest()!=digest:
                raise ValueError('Original source changed: '+name)
    return dict(source='US G8ME01 revision0', spans=len(SPANS),
        scope='Whole retained Noise and actual nested helper/API/table identities; native string/private policies explicit',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
