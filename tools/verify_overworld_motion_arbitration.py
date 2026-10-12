#!/usr/bin/env python3
"""Pin read-only US ordinary Mario motion arbitration evidence; no runtime claim."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioMotion': (2148091420, 100, 'f86c292be913bbb599c8db5db59d795324f2c715ca738071ee6227b8ccefd1f1'), 'marioChgMot': (2148091228, 192, 'cb0d860ff6a94e5e6d0da5dded999fef1e6b881c2169730ce9d09c7f0439da26'), 'marioChkJump': (2148083732, 552, '2295945b6b5a5b9b6d8351091ab52da46c9b3d33b3058ab9b54bc9548181d6a0'), 'marioChkTransform': (2148083236, 496, '98dc692eacb3ba732c8ac1dd40f8b8b022b1ab90b856e368a9444850b9aee758'), 'mot_stay': (2148166736, 1952, '069e5535b3ccbcfd7ab9decef8927617d6f87731f7fdeb862ef7db9960322d04'), 'mot_stay_post': (2148166416, 172, '045f375d0d3f44bb67ed8d1b0e36f43a7e5d4d90a7a3fdc15e1477764c3b4376'), 'mot_walk': (2148039060, 848, '579debb26484eea74504b0617ed05fda92e5e9655d4deccce9fb23f2683625a8'), 'mot_dash': (2148038204, 856, '7b316ca67080614da3bfaedc2eb097079c48ff4f4d3caf20a44eac5253cf9907'), 'mot_jump': (2148119680, 520, '394fdc33e6fcae45fae7866f5808ea1fa58699aff38428318ebe3a2329587853'), 'mot_fall': (2148118300, 572, '8e5bbb5e7348f6d18fb5434813688fba54fd905eca28f5d92f0cc21827bc533a'), 'mot_fall_post': (2148118228, 72, 'a550924067b1490e927b00641a5bd14a08f89171d80a4b36cee8c270092dafc9'), 'mot_land': (2148117992, 236, 'ff05c605cf13ab5796e03f91b422457a0e57a88bc18e6c73439f802b1a377a74'), 'marioLandOn': (2148084884, 3196, '078fee1b141da5850c046f7cbbe1e5088742703c23c52357c0920d677ed87448'), 'marioChgMotAuto': (2148166588, 148, '70489eaf4c37a8f4b7d802384f37ecd7cb3d685d6f417b767bde91bda5d97d0c'), 'marioUpdateKeyData': (2148097200, 480, 'db6e3882e60cc9de3b1bc7aef328126769532d7db38c8fde1877f2b342ea0ab0'), 'marioGetStick': (2148096760, 440, 'a6aba7a06277bc002c9b636e7cdc13e346b2f3a2c50d1ef6cedc01692778c741'), 'marioMain': (2147854304, 2552, 'e7a4c22dbea9834233df4749dc96a6cf0687886f4792edc7c3fdf2b7331a52b8'), 'marioJump': (2148089144, 744, '8161141f6446500f7b852be87d72f6006c47a267e0b5b7e85aa23c12c4d9ecfb'), 'marioClearJumpPara': (2148090792, 52, '1696c65969c286e8e01c03d0582e394a8d4021c827626ae204287fb868059d95'), 'sincosf': (2147507852, 120, 'eae1a7db8a084513605a95d0468daace6c82fc17e054e8a78a6b64f56578e947')}
CONSTANTS = {2150694960: '800a6c5000000000800a6b10', 2150694972: '800879940000000000000000', 2150694984: '8008763c0000000000000000', 2150694996: '8009b4800000000000000000', 2150695080: '8009af1c000000008009aed4', 2150695092: '8009af1c0000000000000000', 2150695104: '8009b1580000000000000000', 2150695116: '8009ade80000000000000000', 2151809032: '3f800000', 2151809208: '40100000', 2151811684: '428c0000', 2151813136: '00000000', 2151812296: '00000000', 2151806048: '40490fdb', 2151806052: '43340000'}
ANCHORS = {2147855324: '4803b0d5', 2147855336: '4803af11', 2147855344: '48039a2d', 2148091480: '4e800421', 2148091492: '4082ffe4', 2148091344: 'b01f0030', 2148091348: 'b3df002e', 2148091392: '901f000c', 2148083752: 'a003024c', 2148083764: '4082000c', 2148084128: '40820084', 2148084204: 'b09f0030', 2148084212: 'b01f002e', 2148084244: '4e800421', 2148084256: '901f000c', 2148083264: '54000529', 2148083268: '4082000c', 2148166832: '901f000c', 2148166844: '901f000c', 2148166856: '418201c4', 2148166864: '901f000c', 2148166880: '4bfed6c9', 2148166892: 'b01f0050', 2148166896: '901f0048', 2148166900: '901f0044', 2148167292: 'd01f0180', 2148167968: '4bfeb6f5', 2148167972: '4bfeb501', 2148168000: '54000253', 2148168048: '2c000bd1', 2148168076: '4bfed3d1', 2148168100: '4bfed3b9', 2148039272: '901f000c', 2148039308: 'a81f004e', 2148039420: 'd3ff0180', 2148039672: '4800a861', 2148039684: '4800ac11', 2148039688: '4800aa1d', 2148039736: '2c000bd1', 2148039748: '4800c919', 2148039756: 'c01f0198', 2148039760: 'd01f01a4', 2148039816: '901f0048', 2148039860: 'fc010000', 2148039872: '4800c89d', 2148038416: '901f000c', 2148038572: 'a81f004e', 2148038688: 'd3ff0180', 2148038812: '4800abbd', 2148038824: '4800af6d', 2148038828: '4800ad79', 2148038884: '901f0048', 2148038928: '2c000bd1', 2148038956: '4800cc31', 2148038964: 'c01f0198', 2148038968: 'd01f01a4', 2148119744: '901f000c', 2148119772: '907f0048', 2148119776: '901f0044', 2148119816: 'b01e0050', 2148119820: 'b01e0052', 2148119864: 'c01e0180', 2148119868: 'd01e0190', 2148120112: '901f0048', 2148120160: '901f0044', 2148120172: 'b01f0050', 2148118372: '901f000c', 2148118384: '901f0000', 2148118396: '901f0000', 2148118460: '901f0044', 2148118548: 'a81f0050', 2148118652: 'b01f0050', 2148118036: '907f000c', 2148118056: 'b01f0050', 2148118068: 'd01f0180', 2148118148: '4bff96d9', 2148084980: 'b01f0050', 2148084984: 'b01f0052', 2148086596: 'fc010040', 2148086612: '40820350', 2148086972: '48007811', 2148087388: '38000001', 2148087400: 'b01f002e', 2148087444: '901f000c', 2148087772: '38000000', 2148087784: 'b01f002e', 2148087828: '901f000c', 2148118268: '4803e6fd', 2148118276: '901f0284', 2147507908: '4826c54d', 2147507920: 'd01e0000', 2147507924: '4826bfd5', 2147507932: 'fc000050', 2147507936: 'd01f0000'}

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
    return dict(scope='Static US ordinary Mario motion arbitration; no execution/timing claim',
                bodies=len(BODIES), anchors=len(ANCHORS), constants=len(CONSTANTS))

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))
