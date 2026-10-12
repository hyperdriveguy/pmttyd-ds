#!/usr/bin/env python3
"""Check US instruction/data anchors cited by docs/behavior.

These are static evidence checks, not original gameplay observation or proof
that a port's implementation is equivalent. Never executes original code.
"""
import argparse
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO


# Independent expected instructions transcribed from the inspected US functions.
ANCHORS = {
    'battle_event_dispatch': {
        0x8012985c: '28000028', 0x80129878: '3880002e',
        0x8012989c: '80bc02b0', 0x801298f0: '806302b4',
        0x80129908: '807c0290', 0x80129920: '807c029c',
        0x80129938: '807c02a8', 0x80129960: '38a00020',
        0x80129970: '93bc0278', 0x80127324: '38630008',
        0x8012971c: '38a00000', 0x80129830: '93fe029c',
    },
    'battle_event_operands': {
        0x8010a598: '801f0004', 0x8010a59c: '900302b0',
        0x8010f1b0: '83bf0004', 0x8010f1e0: '63a40100',
        0x8010f330: '7c00f039', 0x8010f3c4: '5405063e',
    },
    'event_entry_dispatch': {
        0x800366bc: '7c00e000', 0x800366d0: '3bde01b0',
        0x800376d8: '540006b5', 0x80037700: '900d193c',
    },
    'damage_application': {
        0x800fe030: '2c190063', 0x800fe040: '2c19ff9d',
        0x800fe05c: '901f0284', 0x800fe500: '2c03001e',
        0x800fe71c: '9b370270', 0x800fe734: '9b570271',
        0x800fe754: '54000043', 0x800fe760: '54000211',
        0x800fe780: 'b017010c', 0x800fe7fc: '7c000775',
        0x800fe820: '7c9a1851', 0x800fe8dc: '2c030032',
        0x800fe9a8: '60630200', 0x800fea0c: '60600400',
        0x800fea1c: '4bffd001', 0x800fead4: '60000001',
    },
    'battle_fp_owner': {
        0x80126c78: '2c0000e0', 0x80126c84: '2c0000e7',
        0x80126c4c: 'b0830112', 0x80126b2c: 'a863010e',
        0x80126bc4: 'b0640000', 0x80126bd0: '40810008',
        0x800f87d8: '2c0000de', 0x800f87f4: '2c1f0040',
    },
    'counter': {
        0x800fa6b0: '909a0028', 0x800fa708: '909a0000',
        0x800fa924: '901a0010', 0x800faaac: '901a0004',
        0x800fab44: '901a0018', 0x800fab80: '901a001c',
        0x800fac28: '38a00030', 0x800fac40: '901f0000',
        0x800fb7e4: '7c0303d6', 0x800fb93c: '2c000000',
        0x800fb9c4: '3860fffb', 0x800fb9fc: '3860ffff',
    },
    'revival_marker': {
        0x800fba34: '2c0000de', 0x800fba68: '2c0000e0',
        0x800fba70: '2c0000e7', 0x800fbaa8: '2c000097',
        0x800fbab8: '60000800',
    },
    'hit_precheck': {
        0x800fbb28: '60000008', 0x800fbbdc: '57c002d7',
        0x800fbc08: '2c03005a', 0x800fbc3c: '2c03004b',
        0x800fbc84: '2c030043', 0x800fbce4: '2c030032',
        0x800fbd14: '7f6303d6', 0x800fbd60: '4bf0a041',
    },
    'hp_damage': {
        0x800fd7fc: '633f001e', 0x800fd858: '5720039d',
        0x800fd91c: '1c630005', 0x800fd9a8: '60000008',
        0x800fdd34: '38040001', 0x800fdd68: '7c6023d6',
        0x800fddc8: '1c63ffff', 0x800fddcc: '60002000',
    },
    'fp_damage': {
        0x800fd6b0: '540004a5', 0x800fd710: '4082000c',
        0x800fd748: '7c9e2051', 0x800fd770: '60000017',
    },
    'defence_event': {
        0x801076ec: '4bff2acd', 0x801076f4: '38600002',
    },
    'defence': {
        0x800fa200: '88a30306', 0x800fa204: '88030305',
        0x800fa208: '39650003', 0x800fa210: '7d605851',
        0x800fa238: '88040013', 0x800fa254: 'a803010c',
        0x800fa260: 'a89f1d18', 0x800fa284: '88030307',
        0x800fa2ac: '38800200', 0x800fa2dc: '38800100',
        0x800fa30c: '2c1b000f', 0x800fa314: '2c1a0002',
        0x800fa394: '2c1b0002', 0x800fa404: '4801ff89',
    },
    'pad': {
        0x8011a0b0: '3800001d', 0x8011a0b8: '38a00070',
        0x8011a0d8: '80060018', 0x8011a0dc: '9006001c',
        0x8011a1b4: '2c00001e', 0x8011a1d0: '2c00ffe2',
        0x8011a314: '38800008', 0x8011a338: '38800010',
    },
    'event': {
        0x80037a10: '7c060040', 0x80037a14: '4080001c',
        0x80036f78: '60000010', 0x800371e8: '900d193c',
        0x800367b4: '90040070', 0x800367b8: '801f009c',
        0x8003684c: '900400e4', 0x80036894: '38a001b0',
    },
    'saved_work': {
        0x80085838: '80630174', 0x80085848: '88630578',
        0x8008585c: '90830174', 0x8008586c: '98830578',
        0x80085948: '38a00040', 0x80085964: '38a00400',
        0x8008598c: '38a00400', 0x800859a8: '38a00800',
    },
    'pouch': {
        0x800d3d04: 'b0640070', 0x800d3d0c: 'a8030070',
        0x800d3ca4: '1ca3000e', 0x800d3bd8: '4080000c',
        0x800d3e24: 'b0040078', 0x800d3e48: '2c0003e7',
        0x800d561c: '38600001', 0x800d562c: '3800000a',
        0x800d5634: '38000014', 0x800d564c: 'a8040192',
    },
    'rng': {
        0x8026c324: '3c6041c6', 0x8026c32c: '38034e6d',
        0x8026c334: '38033039', 0x8026c33c: '5403847e',
        0x80005db4: '4082000c', 0x80005dc0: '48266565',
        0x80005dc4: '7c03fbd6', 0x80005dcc: '7c601850',
    },
}


def verify(iso_path):
    with Path(iso_path).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected G8ME01 disc 0 revision 0')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(0x100)
        def read(address, size):
            matches = []
            for i in range(18):
                off, va, length = (struct.unpack_from('>I', header, j + i * 4)[0]
                                   for j in (0, 0x48, 0x90))
                if va <= address and address + size <= va + length:
                    matches.append(base + off + address - va)
            if len(matches) != 1:
                raise ValueError(f'Unmapped/ambiguous address {address:#x}')
            disc.seek(matches[0])
            value = disc.read(size)
            if len(value) != size:
                raise ValueError('Truncated binary')
            return value
        for group, anchors in ANCHORS.items():
            for address, expected in anchors.items():
                actual = read(address, 4).hex()
                if actual != expected:
                    raise ValueError(f'{group}: {address:#x}: {actual} != {expected}')
            print(f'{group}: {len(anchors)} instruction anchors verified')
        for address, expected in ((0x802ee018, [10, 9, 8, 8, 7, 6, 5]),
                                  (0x802ee020, [5, 4, 3, 3, 2, 1, 1])):
            if list(read(address, 7)) != expected:
                raise ValueError(f'Defence table mismatch at {address:#x}')
        print('Both seven-entry defence window tables verified')
        if read(0x804221c4, 4) != bytes.fromhex('3f000000'):
            raise ValueError('Accuracy multiplier is not binary32 0.5')
        print('Conditional accuracy multiplier verified: 0.5')
        expected = (0x800fabac, 0x800fabb4, 0x800fabbc, 0x800fabc4,
                    0x800fabcc, 0x800fac04, 0x800fabd4, 0x800fabdc,
                    0x800fabec, 0x800fabe4, 0x800fabf4, 0x800fabfc)
        if struct.unpack('>12I', read(0x8034c720, 48)) != expected:
            raise ValueError('Counter classification jump table mismatch')
        print('All 12 counter classification jump-table targets verified')
        constants = {0x804221b0: 60.0, 0x804221b8: 10.0,
                     0x804221c0: 1.0, 0x804221cc: 30.0,
                     0x804221dc: 8.0, 0x804221e0: 5.0,
                     0x804221e4: -30.0, 0x804221e8: -60.0,
                     0x804221ec: 0.0}
        for address, value in constants.items():
            if read(address, 4) != struct.pack('>f', value):
                raise ValueError(f'Damage presentation constant mismatch {address:#x}')
        sounds = {0x802ee2e0: 'SFX_MARIO_HAMMER_PIKKYO_R1',
                  0x802ee2fc: 'SFX_MARIO_HAMMER_PIKKYO_Y1',
                  0x802ee318: 'SFX_MARIO_HAMMER_PIKKYO_B1',
                  0x802ee334: 'SFX_MARIO_HAMMER_PIKKYO_G1',
                  0x802ee350: 'SFX_MARIO_HAMMER_PIKKYO_P1',
                  0x802ee36c: 'SFX_MARIO_GUARD1',
                  0x802ee380: 'SFX_MARIO_DAMAGE_FP1'}
        for address, name in sounds.items():
            expected = name.encode('ascii') + b'\0'
            if read(address, len(expected)) != expected:
                raise ValueError(f'Damage sound binding mismatch {address:#x}')
        print('Nine damage presentation constants and seven sound keys verified')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    verify(parser.parse_args().iso)
