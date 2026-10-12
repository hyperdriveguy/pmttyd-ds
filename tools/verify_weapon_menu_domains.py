#!/usr/bin/env python3
"""Pin native-requested weapon menu setup/decision and scoped type/cursor source paths."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleCommandAttackAudienceCheck': (2148660516,
                                      204,
                                      '079b8fc88ba1ba515e84fff5f2971416ded071deff5361167f490bf7436e2eaf'),
 'BattleCommandDisplay_ActMenuMain': (2148672268,
                                      1160,
                                      '53fe5beadedfa2f1d77079afd347532d5d8d2710fde22ec4982239bf0c7370df'),
 'BattleCommandDisplay_WeaponSelectMenuSetup': (2148671592,
                                                108,
                                                '03fe897e299e7da264a561d436a51b75813586a67bcc8ce1fe11349e20e83385'),
 'BattleCommandInput': (2148674008,
                        2104,
                        '134db69496df23570819886d7034c00a770c7dcac5b3a3e97b8e19306947bd1a'),
 'BattleGetSelectWeapon': (2148663836,
                           76,
                           '45a0fd19eb7bbc1234c94071ef9ce1097f883f108ed85ea2d6762d663e114cf7'),
 'BattleGetUnitPtr': (2148501708,
                      60,
                      '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 'BattleMenuDisp_WeaponSelect_Init': (2148622448,
                                      116,
                                      '1358303410e47292ae9302c342d469d3a20364e0f7057181736da7d5be123d6b'),
 'BattleSamplingEnemy': (2148529600,
                         196,
                         '9c0761564dcca42b319ce87e4a6323e8dbb3520ebb94e2393bd8ca41c904a421'),
 'BattleSetConfuseAct': (2148660780,
                         1552,
                         '5b711312b066f2475b296cd82209cb8303d263f8d036ed05e8436af3d8e54681'),
 'BtlUnit_GetBelong': (2148696388,
                       8,
                       '6dca30fe4b0c437e7fd4e3134d890f800cf942931beb565332cb240db2589d42'),
 'BtlUnit_GetEnemyBelong': (2148693076,
                            44,
                            'b53539b262b9fd2fbbb85efa7c5412e0459911ca5d3ac8015e96c1166b9a46ba'),
 '_btlcmd_GetCursorPtr': (2148682936,
                          272,
                          '503374044e2b5599babc1e0f64c05b7338460ecca99246067924e0310ad2049b'),
 '_btlcmd_MakeSelectWeaponTable': (2148679360,
                                   3408,
                                   'f5c5d8b7e96d3250b0d4ce29e746c2d4f39b5ac5dd0e58c421b8646c55c14d1b'),
 '_btlcmd_SelectWeaponDecide': (2148676112,
                                336,
                                '371f0ff4ae27b0ea615db3221ec277266bfbc306415955969192c37051a3035d'),
 '_check_present_item': (2148663332,
                         504,
                         'f072a60e070c934de5a50bbf73058e4e7521318e645b94c24c66e527684f0149')}

ANCHORS = {2148660664: '90bf1c5c',
 2148661108: '80610008',
 2148661112: '80030000',
 2148661116: '1c000014',
 2148661120: '7c7f0214',
 2148661124: '80030008',
 2148661128: '901f0540',
 2148661132: '80610008',
 2148661136: '80030000',
 2148661140: '1c000014',
 2148661144: '7c7f0214',
 2148661148: '8003000c',
 2148661152: '2c000000',
 2148661156: '40820010',
 2148661160: '3c608035',
 2148661164: '386332b0',
 2148661168: '48000478',
 2148661172: '809f0540',
 2148661176: '2c040004',
 2148661180: '41820030',
 2148661184: '40800018',
 2148661188: '2c040003',
 2148661192: '40800458',
 2148661196: '2c040000',
 2148661200: '4080001c',
 2148661204: '4800044c',
 2148661208: '2c040009',
 2148661212: '40800444',
 2148661216: '2c040006',
 2148661220: '40800008',
 2148661224: '48000180',
 2148661228: '7fa3eb78',
 2148661232: '480046d1',
 2148663720: '2c1f0000',
 2148663724: '4180000c',
 2148663728: '2c1f0153',
 2148663732: '4180000c',
 2148663736: '38600000',
 2148663740: '4800004c',
 2148663744: '38000001',
 2148663748: '981e1c74',
 2148663752: '801e171c',
 2148663756: '2c00000c',
 2148663760: '4182000c',
 2148663764: '2c00000d',
 2148663768: '4082000c',
 2148663772: '38000002',
 2148663776: '981e1c74',
 2148663780: '7fc3f378',
 2148663784: '48000635',
 2148663788: '80be1c5c',
 2148663792: '3880000c',
 2148663796: '3800001d',
 2148663800: '38600001',
 2148663804: '90be1c60',
 2148663808: '909e1c5c',
 2148663812: '901e171c',
 2148671592: '9421ffe0',
 2148671596: '7c0802a6',
 2148671600: '90010024',
 2148671604: 'bf810010',
 2148671608: '7c7c1b78',
 2148671612: '7c9d2378',
 2148671616: '7cbe2b78',
 2148671620: '3bfc171c',
 2148671624: '48001e39',
 2148671628: '7fe3fb78',
 2148671632: '7fa4eb78',
 2148671636: '38a10008',
 2148671640: '48002c21',
 2148671644: '80810008',
 2148671648: '7f83e378',
 2148671652: '7fc6f378',
 2148671656: '38bf0080',
 2148671660: '4bff3fc5',
 2148671664: '801f0004',
 2148671668: '64000100',
 2148671672: '60000010',
 2148671676: '901f0004',
 2148671680: 'bb810010',
 2148671684: '80010024',
 2148671688: '7c0803a6',
 2148671692: '38210020',
 2148671696: '4e800020',
 2148672384: '907f0540',
 2148672844: '901f0540',
 2148673364: '907f0540',
 2148674008: '9421ffc0',
 2148674012: '7c0802a6',
 2148674016: '90010044',
 2148674020: 'bec10018',
 2148674024: '7c781b78',
 2148674028: '80830420',
 2148674032: '4bfd5edd',
 2148674036: '3798171c',
 2148674040: '7c7d1b78',
 2148674044: '4082000c',
 2148674048: '38600001',
 2148674052: '480007f8',
 2148674056: '7f83e378',
 2148674060: '38a10008',
 2148674064: '3880000e',
 2148674068: '480022a5',
 2148674072: '4bffd60d',
 2148674076: '2c030000',
 2148674080: '4182000c',
 2148674084: '38600000',
 2148674088: '480f3ee5',
 2148674092: '4bffcbc5',
 2148674096: '3c60802f',
 2148674100: '3b600001',
 2148674104: '3be3fe98',
 2148674108: '7ffefb78',
 2148674112: '480007b0',
 2148674116: '3b600000',
 2148674120: '80bc0000',
 2148674124: '2805001d',
 2148674128: '4181078c',
 2148674132: '3c608036',
 2148674136: '54a0103a',
 2148674140: '38634074',
 2148674144: '7c03002e',
 2148674148: '7c0903a6',
 2148674156: '88181c74',
 2148674160: '28000000',
 2148674164: '41820038',
 2148674168: '801c0540',
 2148674172: '2c00000d',
 2148674176: '40800018',
 2148674180: '2c00000b',
 2148674184: '40800008',
 2148674188: '4800000c',
 2148674192: '801c0544',
 2148674196: '901c0540',
 2148674200: '88181c74',
 2148674204: '28000002',
 2148674208: '4082000c',
 2148674212: '38000000',
 2148674216: '98181c74',
 2148674848: '835c0540',
 2148674852: '7f83e378',
 2148674856: '3b380428',
 2148674860: '38a10008',
 2148674864: '7f44d378',
 2148674868: '48001f85',
 2148675012: '80610008',
 2148675016: '80030000',
 2148675020: '1c60001c',
 2148675024: '38030080',
 2148675028: '7c7c002e',
 2148676112: '9421ffe0',
 2148676116: '7c0802a6',
 2148676120: '90010024',
 2148676124: 'bf410008',
 2148676128: '7c7d1b78',
 2148676132: '7c9b2378',
 2148676136: '7cba2b78',
 2148676140: '3bfd0428',
 2148676144: '80830420',
 2148676148: '4bfd5699',
 2148676152: '7c601b78',
 2148676156: '7fa3eb78',
 2148676160: '7c1c0378',
 2148676164: '7f64db78',
 2148676168: '48000c79',
 2148676172: '1c1a001c',
 2148676176: '7f83e378',
 2148676180: '7c9d0214',
 2148676184: '83c4179c',
 2148676188: '480041f9',
 2148676192: '881c000c',
 2148676196: '7c660774',
 2148676200: '80bd0420',
 2148676204: '7fe3fb78',
 2148676208: '7c000774',
 2148676212: '80fe0064',
 2148676216: '54001838',
 2148676220: '811e0068',
 2148676224: '7d3d0214',
 2148676228: '7fc4f378',
 2148676232: '8809000a',
 2148676236: '7c090774',
 2148676240: '4bfdc331',
 2148676244: '38000000',
 2148676248: '981f0ab7',
 2148676252: '801e0068',
 2148676256: '54000087',
 2148676260: '418200a4',
 2148676264: '7f83e378',
 2148676268: '3b80ffff',
 2148676272: '48004e95',
 2148676276: '7c7a0774',
 2148676280: '3b600000',
 2148676284: '48000070',
 2148676288: '387b0a6d',
 2148676292: '801e0068',
 2148676296: '7c7f18ae',
 2148676300: '540000c7',
 2148676304: '7c630774',
 2148676308: '1c630024',
 2148676312: '38030004',
 2148676316: '41820028',
 2148676320: '7c9f02ae',
 2148676324: '7fa3eb78',
 2148676328: '4bfd55e5',
 2148676332: '48004e59',
 2148676336: '7c600774',
 2148676340: '7c1a0000',
 2148676344: '40820030',
 2148676348: '7f7cdb78',
 2148676352: '48000028',
 2148676356: '7c9f02ae',
 2148676360: '7fa3eb78',
 2148676364: '4bfd55c1',
 2148676368: '48004e35',
 2148676372: '7c600774',
 2148676376: '7c1a0000',
 2148676380: '4182000c',
 2148676384: '7f7cdb78',
 2148676388: '48000018',
 2148676392: '3b7b0001',
 2148676396: '881f0a6c',
 2148676400: '7c000774',
 2148676404: '7c1b0000',
 2148676408: '4180ff88',
 2148676412: '2c1c0000',
 2148676416: '41800008',
 2148676420: '9b9f0ab7',
 2148676424: 'bb410008',
 2148676428: '38600000',
 2148676432: '80010024',
 2148676436: '7c0803a6',
 2148676440: '38210020',
 2148676444: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20',
 2151039092: '80122a6c80122b1c80122b3480122b4c80122b6c80122b9480122b9480122c4c80122c6880122c8880122ce080122d2080122cb080122cc880122fd880122fd8801230088012302080123040801230688012306880123098801230b0801230d0801230f8801230f8801231288012314080123154801231a4'}

CALL_SITES=json.loads(Path(__file__).with_name('weapon_menu_call_sites.json').read_text())['sites']

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol_offset=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_offset)
        header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    matches.append(dol_offset+off+address-va)
            if len(matches)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
        expected={(int(site['address'],16),int(site['target'],16),site['instruction']) for site in CALL_SITES}
        actual=set()
        targets={0x8012021c,0x801057e8,0x80122068,0x80123210,0x80123ec0}
        for i in range(7):
            va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0x48,0x90))
            if not length:
                continue
            data=read(va,length)
            for offset in range(0,len(data)-3,4):
                word=struct.unpack_from('>I',data,offset)[0]
                if word>>26!=18 or not word&1:
                    continue
                displacement=word&0x3fffffc
                if displacement&0x2000000:
                    displacement-=0x4000000
                target=(displacement if word&2 else va+offset+displacement)&0xffffffff
                if target in targets:
                    actual.add((va+offset,target,data[offset:offset+4].hex()))
        if actual!=expected:
            raise ValueError('Direct DOL weapon menu call census changed')
    return dict(schema=1,scope='Complete setup/decision;scoped menu writer/cursor-reuse and direct caller census;broader source/Game/runtime remain open',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),dol_direct_call_sites=len(CALL_SITES))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output')
    a=p.parse_args();result=verify(a.iso)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
