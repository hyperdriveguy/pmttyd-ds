#!/usr/bin/env python3
"""Pin read-only US Mario airborne entry and landing evidence; no runtime claim."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioChkInScreen': (2147846440, 56, '209ee8968bc636f3d2e7950ea6ef1f18a3da871358eab6def6d3d7f1a64e1ce9'), 'marioMain': (2147854304, 2552, 'e7a4c22dbea9834233df4749dc96a6cf0687886f4792edc7c3fdf2b7331a52b8'), 'marioChkSts': (2147860284, 160, '3e26a349509a42cf9e0e7ff48454afae776c6da2a175751b9284223e17138ecb'), 'marioChkOverhead': (2148059396, 1188, '73cd4ad4d41aae326e993c00df7903420d6d9ca32e79b79e7c5358f3c4b5ed21'), 'marioChkLandon': (2148064368, 148, 'a050975e2ce9fbb9cbdc3c4b604c417e55835f25b2891d1e84993176ce7f7f10'), 'marioSearchGround': (2148066964, 2640, '868e271655b02db5fb04dcbbf326bff0165f1827dd959738a9ad74df47064e47'), 'marioLandOn': (2148084884, 3196, '078fee1b141da5850c046f7cbbe1e5088742703c23c52357c0920d677ed87448'), 'marioFall': (2148088080, 1064, '727db9c961bbf038a733af133978d4fb3bb58168f402f85cc516855a63ef5bb5'), 'marioJump': (2148089144, 744, '8161141f6446500f7b852be87d72f6006c47a267e0b5b7e85aa23c12c4d9ecfb'), 'marioSetFallPara': (2148090156, 144, 'c44c2dcf27dd8094e54b4b2a3049f6d12a2b3253c0edd504c6b9e99009790a01'), 'marioSetJumpPara': (2148090300, 492, '1430a91e5df6143eb177986bbeacd118e6e3b04d9d3aca56a482b2e31631fd82'), 'marioClearJumpPara': (2148090792, 52, '1696c65969c286e8e01c03d0582e394a8d4021c827626ae204287fb868059d95'), 'marioMotion': (2148091420, 100, 'f86c292be913bbb599c8db5db59d795324f2c715ca738071ee6227b8ccefd1f1'), 'marioChgStayMotion': (2148093512, 224, '2bdf307b14482899000609f65ee163864c887d8beba475eb0a1464a8f43552b6'), 'marioSetPeakYpos': (2148100336, 40, '2f51c8e7beefcd8d795a63eaef478b2029d28708ba28746a15ff84e0f75c7bd6'), 'mot_fall': (2148118300, 572, '8e5bbb5e7348f6d18fb5434813688fba54fd905eca28f5d92f0cc21827bc533a'), 'mot_jump': (2148119680, 520, '394fdc33e6fcae45fae7866f5808ea1fa58699aff38428318ebe3a2329587853')}
CONSTANTS = {2151768336: '41000000bf570a3d3da3d70abca3d70abdcccccdbe4ccccd3cc49ba6bb83126f42000000c1900000', 2151811488: '00000000', 2151811508: '3f800000', 2151811624: '3dcccccd', 2151811628: '3c23d70a', 2151811632: 'bf000000', 2151811640: 'bf06a7f0', 2151811332: '41200000', 2151811276: '3f000000', 2151811268: '3f800000', 2151811252: '41300000', 2151811312: '40000000'}
ANCHORS = {2148119772: '907f0048', 2148119776: '901f0044', 2148119816: 'b01e0050', 2148119820: 'b01e0052', 2148119828: '901e01ec', 2148119836: '4bff8ca1', 2148119868: 'd01e0190', 2148120112: '901f0048', 2148120160: '901f0044', 2148120172: 'b01f0050', 2148118384: '901f0000', 2148118396: '901f0000', 2148118460: '901f0044', 2148118544: '901f0000', 2148118548: 'a81f0050', 2148118580: '901f00dc', 2148118612: '4c401382', 2148118616: '4082001c', 2148118640: '901f0044', 2148118652: 'b01f0050', 2148089228: '41800274', 2148089284: 'd0230084', 2148089304: 'd0230080', 2148089324: 'd023007c', 2148089344: '2c000004', 2148089360: '40820010', 2148089372: 'd01e007c', 2148089392: '408201c0', 2148089408: 'b01e0052', 2148089412: '48002aad', 2148089460: 'd003007c', 2148089484: 'd0030088', 2148089576: 'd0030084', 2148089596: 'd0030080', 2148089616: 'd003007c', 2148089736: 'b01f002e', 2148089780: '901f000c', 2148089852: 'd01e0090', 2148088384: '4bfc4ee9', 2148088408: '901f0000', 2148088420: 'ec02083a', 2148088424: 'd01f0180', 2148088484: '40820038', 2148088524: 'fc030040', 2148088536: 'd07f0180', 2148088544: '901f01e8', 2148088676: 'd0230084', 2148088688: '40810008', 2148088692: 'd0030084', 2148088712: 'd0030080', 2148088728: 'd003007c', 2148088748: '40820018', 2148088752: 'd023007c', 2148088768: 'd0030088', 2148088784: '4bffa0a1', 2148088796: '907f01e8', 2148088804: '901f01ec', 2148088816: '41820014', 2148088840: 'd01f0090', 2148089060: 'd01f0090', 2148089088: '4bffef95', 2148064420: '40820020', 2148064424: 'fc20f890', 2148064432: 'fc40f890', 2148064444: '480009d9', 2148064472: '48000135', 2148064488: '4bfff7b9', 2148084920: '546005ad', 2148084936: 'd003007c', 2148084948: 'd0030088', 2148084980: 'b01f0050', 2148084984: 'b01f0052', 2148087400: 'b01f002e', 2148087444: '901f000c', 2148087784: 'b01f002e', 2148087828: '901f000c', 2147856380: '48038d3d', 2147856384: '48031905', 2147856388: '4bffea75', 2147856432: '408100a8', 2147856436: '480340b1', 2147856460: '4bffea2d', 2147856500: '48031891', 2147856504: '48038899', 2147856568: '40810020', 2147856572: '48034029', 2148090812: 'd003007c', 2148090824: 'd0030088', 2148059948: 'd01f0180', 2148059956: '60000080', 2148059968: '64000800', 2148060080: 'ec23007a', 2148060120: 'ec010028', 2148060148: '408000a8', 2148060156: 'd0210014', 2148060244: '480081f5', 2148060312: '480078c5', 2148060320: '28000003', 2148060348: '480076ed', 2148060356: '54000416', 2148060380: '48007881', 2148060384: '4800744d', 2148060392: 'd01f0090', 2148069168: 'c00294b0', 2148069188: 'fc009a10', 2148069192: 'fc010040', 2148069196: '4081002c', 2148069332: 'c0029474', 2148069344: 'fc010040', 2148069352: '40820058', 2148060060: 'c06294c4', 2148060084: 'c0029484'}

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol=struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read(address, size):
            matches=[]
            for i in range(18):
                off, va, length=(struct.unpack_from('>I', header, j+i*4)[0]
                                 for j in (0, 0x48, 0x90))
                if va <= address and address+size <= va+length:
                    matches.append(dol+off+address-va)
            if len(matches) != 1:
                raise ValueError(f'Unmapped/ambiguous address {address:#x}')
            disc.seek(matches[0])
            data=disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated DOL')
            return data
        for name, (address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest() != digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**CONSTANTS, **ANCHORS}.items():
            if read(address,len(value)//2).hex() != value:
                raise ValueError(f'Anchor/constant mismatch: {address:#x}')
    return dict(scope='Static US Mario airborne and landing; no execution/timing claim',
                bodies=len(BODIES), anchors=len(ANCHORS), constants=len(CONSTANTS))

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))
