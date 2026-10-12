#!/usr/bin/env python3
"""Check US instruction/data anchors cited by docs/behavior.

These are static evidence checks, not original gameplay observation or proof
that a port's implementation is equivalent. Never executes original code.
"""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO


# Independent expected instructions transcribed from the inspected US functions.
ANCHORS = {
    'phase_tables_and_consumption': {
        0x8011b13c: '40820014', 0x8011b168: '801d0104',
        0x8011b184: '7cbf212e', 0x8011b010: '40810014',
        0x8011b08c: '40800014', 0x8011ae60: '937c0000',
        0x8011aef8: '40810024', 0x8011bf94: '41820014',
        0x8011bfa4: '981f0020', 0x8011b920: '38600001',
        0x8011b964: '981f0022', 0x8011b9a0: '981f0024',
        0x8011bbd8: '40810014', 0x8011c2e8: '4182000c',
    },
    'move_counts_and_eligibility': {
        0x80126710: '981f0022', 0x80126730: '2c000002',
        0x8012675c: '5463083c', 0x80126790: '981f0020',
        0x801267a0: '7c000775', 0x8011b25c: '981c0024',
        0x8011b298: '7c002850', 0x8010fd18: '2c04ffff',
        0x8010fd40: '987f0021', 0x8010fd58: '40810008',
        0x8012729c: '3880001b', 0x801272e8: '4bffff65',
    },
    'inventory_removal_and_reservations': {
        0x800d4c60: '2c1d0079', 0x800d4cd0: '2c1d0153',
        0x800d4d30: '7c1f1b2e', 0x800d4d38: '7c1e1b2e',
        0x800d4a50: 'a8030000', 0x800d4a70: '7c1d2000',
        0x8010ae04: '2c0000de', 0x8010ae1c: '1cdd0028',
        0x8010ae54: '41820024', 0x8010ae68: '93a39060',
        0x8010aedc: 'ec010024', 0x8010af30: '2c000097',
    },
    'battle_waits_and_conclusion': {
        0x8011ac00: '901f029c', 0x8011ac30: '901f02a8',
        0x8011ac60: '901f02b4', 0x8011ac90: '901f02bc',
        0x8011acc0: '4bfdcc3d', 0x800f795c: '4bffffa1',
        0x800f79bc: '38600001', 0x800f7aa0: '4d820020',
        0x800f7a60: '38e7ffff', 0x800f74f0: '7fdce92e',
        0x8011aa5c: '901f000c', 0x8011aac8: '546000c7',
        0x8011ab2c: '5400035b', 0x8011ab80: '2c1e0002',
        0x8011ab94: '540306b6', 0x8011ab9c: '7c601910',
    },
    'recovery_event_services': {
        0x80037530: '4bfcdb85', 0x800375dc: '90fe00c4',
        0x80109fc4: '83df0004', 0x8010a074: '4082000c',
        0x8010a090: '981d000a', 0x8010a094: '93e30160',
        0x8010a250: '901f0290', 0x8010a2b4: '4801f42d',
        0x80105f4c: '7c1e0774', 0x80106370: '4080000c',
        0x801063b8: '7c003800', 0x80106528: '4bffabcd',
        0x80125434: '80630160', 0x8012816c: 'd0230174',
    },
    'turn_regeneration': {
        0x8011dbb8: '54000043', 0x8011dbc8: '901c0208',
        0x8011dbf4: '60000001', 0x8011dc38: '38630005',
        0x8011dd64: '48008df5', 0x8011dee0: '7c0403d6',
        0x8011dee8: '23400021', 0x8011df18: 'b01c010c',
        0x8011e018: '480003bc', 0x8011e08c: 'a89a0112',
        0x8011e094: 'a81a010e', 0x8011e098: '7c0403d6',
        0x8011e0d0: '48008a89', 0x80126bf4: 'b003010c',
    },
    'periodic_damage': {
        0x8011d90c: '7cc60774', 0x8011d918: '39000117',
        0x8011da14: 'b01f0002', 0x8011daf8: '418208dc',
        0x80195e04: 'b01f0338', 0x80195e9c: 'ec02007a',
        0x80195eb0: '2c00003c', 0x80195f00: '39000118',
        0x80195f40: 'b083033a', 0x80125d20: '2c0000e4',
    },
    'turn_recovery': {
        0x8011c538: '2800004d', 0x8011c744: '90030160',
        0x8011c750: '2c190040', 0x8011c7a4: '48001c30', 0x8011cc80: '3900011a',
        0x8011ccf8: '4bfbcb51', 0x8011d680: '48009055',
        0x8011ad30: '901f82b4', 0x800f8984: '38a50001',
    },
    'damage_orchestration': {
        0x800fb0a4: '2c030005', 0x800fb104: '64000008',
        0x800fb138: '2c000001', 0x800fb20c: '4802e6c5',
        0x800fb364: '2c030003', 0x800fb448: '3bc00001',
        0x800fb4b0: 'b01b0260', 0x800fb4b8: '54e0035b',
        0x800fb5b4: '6000001d', 0x800fb630: '2c030000',
        0x800fb720: '4bf0a681', 0x800fb73c: '5400039d',
        0x800fb794: '39400001', 0x800fb7a8: '4082fb18',
        0x800fb7bc: '981b0273', 0x800fb9d8: '480025fd',
    },
    'guard_eligibility_and_selection': {
        0x801273a8: '2c0000df', 0x801273f4: '3880001b',
        0x8012741c: '5403d97e', 0x80127800: '80030138',
        0x800fde94: '54000529', 0x800fdea0: '806363f4',
        0x800fdf30: '4bf07e71', 0x800fdf4c: '7c601851',
        0x800fdf8c: '67ff0100', 0x800fdfb8: '93fe0000',
    },
    'status_presentation': {
        0x800fc1b0: '540007bd', 0x800fc238: '4182002c',
        0x800fc268: 'c042ad88', 0x800fc2d0: '480eeca5',
        0x800fc324: '98c40038', 0x800fcbf8: '4bf61dc9',
        0x800fcc20: '90040018', 0x800fcc54: '4182012c',
        0x800fccbc: '7c002800', 0x800fccf4: '38c00000',
        0x800fcd94: '4802bc59', 0x800fcf3c: '38600001',
    },
    'status_infliction': {
        0x800fbde8: '7c680774', 0x800fbeb8: '281f001c',
        0x800fbf8c: '54600085', 0x800fc05c: '2c1a0000',
        0x800fc098: '7f4303d6', 0x800fc0a8: '7f5ada14',
        0x800fc0b0: '4bf09cf1', 0x800fc104: '60000020',
        0x800fc130: '4802ba7d', 0x800fd330: '5400083c',
        0x800fd470: '3bc00009', 0x800fd474: '9bfc0000',
    },
    'status_storage': {
        0x80127864: '881e02f7', 0x801278d0: '2c000000',
        0x80127950: '981e0118', 0x801279d4: '480001d9',
        0x80127a40: '987e0123', 0x80127be8: '98070119',
        0x80127c38: '98070122', 0x80127df4: '98070129',
        0x80127dfc: '7c600775', 0x80127e18: '2c000063',
        0x80127e9c: '98070130', 0x80128108: '540004e7',
    },
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
        for address, value in {0x804221c8: 300.0, 0x804221cc: 30.0,
                               0x804221d0: 40.0, 0x804221d4: 20.0,
                               0x804221d8: 15.0}.items():
            if struct.unpack('>f', read(address, 4))[0] != value:
                raise ValueError(f'Status effect constant mismatch {address:#x}')
        print('Five status effect position constants verified')
        for address, value in {0x804221b0: 60.0, 0x804221b4: 45.0,
                               0x804221b8: 10.0, 0x804221bc: 0.75,
                               0x804221c0: 1.0}.items():
            if struct.unpack('>f', read(address, 4))[0] != value:
                raise ValueError(f'Orchestration effect constant mismatch {address:#x}')
        for address, name in {0x802ee064: 'SFX_SYSTEM_GREAT1',
                              0x802ee078: 'SFX_SYSTEM_NICE1',
                              0x802ee08c: 'SFX_BTL_THUNDERS_ATTACK4'}.items():
            expected = name.encode('ascii') + b'\0'
            if read(address, len(expected)) != expected:
                raise ValueError(f'Orchestration sound binding mismatch {address:#x}')
        if read(0x802ee050, 20) != bytes(20):
            raise ValueError('Pikkyoro candidate initializer mismatch')
        print('Orchestration: five floats, three sound keys and candidate initializer verified')
        if hashlib.sha256(read(0x80363cc8, 78*4)).hexdigest() != '42ae54671dbfed589235c6a229767eba9b48984a21a16ee5d10788bc9c85343f':
            raise ValueError('Turn sequence dispatch table mismatch')
        recovery_sites = (0x8011c710, 0x8011c7c8, 0x8011c888, 0x8011c948,
                          0x8011ca08, 0x8011cac8, 0x8011cb88, 0x8011cc50,
                          0x8011cd70, 0x8011ce30, 0x8011cef0, 0x8011cfb0,
                          0x8011d070, 0x8011d130, 0x8011d1f0, 0x8011d2b0,
                          0x8011d36c, 0x8011d42c, 0x8011d4ec, 0x8011d5ac,
                          0x8011d66c, 0x8011d734, 0x8011d7f4)
        for site in recovery_sites:
            expected = 0x48000001 | ((0x80127890-site) & 0x03fffffc)
            if struct.unpack('>I', read(site, 4))[0] != expected:
                raise ValueError(f'Recovery helper call mismatch {site:#x}')
        for address, name in {0x802efb80: 'SFX_REST_CONDITION1',
                              0x802efb94: 'SFX_CONDITION_BREAK_ICE1'}.items():
            expected = name.encode('ascii') + b'\0'
            if read(address, len(expected)) != expected:
                raise ValueError(f'Recovery sound binding mismatch {address:#x}')
        if read(0x8042256c, 4) != struct.pack('>f', 1.0):
            raise ValueError('Freeze expiry effect scale mismatch')
        print('Turn recovery: dispatch hash, 23 calls, two sounds and scale verified')
        for address, value in {0x80424728: 1.0, 0x8042472c: 5.0,
                               0x80424730: 0.4, 0x80424734: 1.6}.items():
            if read(address, 4) != struct.pack('>f', value):
                raise ValueError(f'Burn effect constant mismatch {address:#x}')
        print('Four Burn effect constants verified')
        if hashlib.sha256(read(0x80416bc0, 16)).hexdigest() != '7cb2b9d5e89d7f24419e079a349f2238cc7f6ec5d506892b0e7db744db0742a4':
            raise ValueError('Regeneration wait event mismatch')
        print('Regeneration wait event encoding verified')
        recovery_event_scripts = ((2150971140, 192, '8a00f3d9dd134fad1fcc56c5f314556116efc10e7e8fe18095f82822e4595a6a'), (2150963772, 212, 'cd1e1b388eb4deb749c6287b09731885aa9d3ed6424a4210cab6259b44fe0a3b'), (2150963984, 212, '22d36fc11de505e490dba830f8bebfc64d14b95f935383e94a2a29f88d710716'))
        for address, size, digest in recovery_event_scripts:
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError(f'Recovery event script fingerprint mismatch {address:#x}')
        print('Three recovery/Happy event script fingerprints verified')
        for address, value in {0x804222d8: 100.0, 0x8042228c: 1.0}.items():
            if read(address, 4) != struct.pack('>f', value):
                raise ValueError(f'Reservation refund constant mismatch {address:#x}')
        print('Two reservation refund constants verified')
        if hashlib.sha256(read(0x80363c8c, 60)).hexdigest() != '9679486ada681e3679c69be1a2eb0cbed6b9478c33a88284755ad90ed7dd40a4':
            raise ValueError('Move sequence dispatch table mismatch')
        for address, value in {0x80422568: 0.0, 0x80422580: 1000.0}.items():
            if read(address, 4) != struct.pack('>f', value):
                raise ValueError(f'Phase event sort constant mismatch {address:#x}')
        print('Move dispatch table and two phase event sort constants verified')
        status_tables = {
            0x80364204: [0x7970, 0x792c, 0x7970, 0x7970, 0x7adc,
                         0x7970, 0x7970, 0x7970, 0x7970, 0x7970,
                         0x79f4, 0x79f4, 0x7a68, 0x7a68, 0x7adc,
                         0x7adc, 0x7b40, 0x7b40, 0x7970, 0x7970,
                         0x7970, 0x7970, 0x7970, 0x7adc, 0x7adc],
            0x80364268: [0x7e2c, 0x7be0, 0x7bf0, 0x7bf8, 0x7c00,
                         0x7c0c, 0x7c14, 0x7c1c, 0x7c24, 0x7c48,
                         0x7c6c, 0x7c6c, 0x7cd4, 0x7d08, 0x7d3c,
                         0x7d3c, 0x7dec, 0x7e4c, 0x7e54, 0x7e84,
                         0x7ec4, 0x7e5c, 0x7e7c, 0x7f04, 0x7f10,
                         0x7f20, 0x7f20, 0x7f1c],
            0x803642d8: [0x8080, 0x7f60, 0x7f6c, 0x7f78, 0x7f84,
                         0x7f90, 0x7f9c, 0x7fa8, 0x7fb4, 0x7fc0,
                         0x7fcc, 0x7fe4, 0x7ffc, 0x8014, 0x802c,
                         0x8044, 0x8074, 0x808c, 0x8098, 0x805c,
                         0x8068, 0x80a4, 0x80b0, 0x80bc, 0x80c8,
                         0x80dc, 0x80dc, 0x80d4],
        }
        for address, offsets in status_tables.items():
            expected = tuple(0x80120000 + offset for offset in offsets)
            if struct.unpack(f'>{len(offsets)}I', read(address, len(offsets)*4)) != expected:
                raise ValueError(f'Status dispatch table mismatch {address:#x}')
        print('All 81 status getter/setter/recovery dispatch targets verified')
        # Fingerprints of the separately inspected case maps, not an execution proof.
        infliction_tables = (
            (0x8034c7f8, 28, '78ec0d9865ffbf5a197b16a22800f14c27f642873d6061c0e265b67e35cf4a4f'),
            (0x8034c794, 25, 'c67b16aaef8708d9bc9af9553796fee24ddd20cac8ee56f938843297612dcea9'),
            (0x8034c750, 17, '0df1727cb6b1b471c2b3b04c27d2308c33e6bd20e9a14de0b8e0d9e835173d0c'),
            (0x8034c994, 25, '3e6d231a24a0bd9498fb481225a285aee6fff4a8f1a3b4171ec3dc0cca9636a6'),
            (0x8034c924, 28, 'c12058ae38eb7cc956e7047bf4ce84a67823257e03172a88131b7631a57c2b15'),
            (0x8034c8b4, 28, '1f1d356e7471a229883f54d6e727fa1026ca43bcdf857d4926b0dfa6882a7ecb'),
            (0x8034c868, 19, '38d78863472bb41196a8840c644aaa21616f781c5e9fd7d4ccaaecdfdaac06c7'),
            (0x8034c9f8, 28, 'a3a16c14935305c0ab1ff9eebd8b169c8d338260dc1efd8d979f0f8031628ba1'),
            (0x8034ca68, 28, 'e12bc54914b01f8f264fa6c794660307ce1d07fc93420d1d27bface856bf85fd'),
            (0x8034cad8, 28, 'b6132dc24a888c207329575a790a8a9d530f022130de2168a9a30b16b47acde7'),
        )
        for address, count, digest in infliction_tables:
            if hashlib.sha256(read(address, count*4)).hexdigest() != digest:
                raise ValueError(f'Status infliction table fingerprint mismatch {address:#x}')
        print('Ten status infliction/presentation table fingerprints verified (254 entries)')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    verify(parser.parse_args().iso)
