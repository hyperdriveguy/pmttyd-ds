#!/usr/bin/env python3
"""Check badge recovery event and scoped called service contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

from dol_event_inventory import parse_commands

BODIES = {'_get_heart_suitoru_point': (2148612432,
                              120,
                              'b0538ba74312127de3a0590541bc5b8ab33bbce0802a82e2ecbe2bdec53e8a24'),
 '_get_flower_suitoru_point': (2148612312,
                               120,
                               'f60b70076435b457321e828d1e9339677eba71c04da4b2311f7f992a06390912'),
 'btlevtcmd_RecoverHp': (2148596252,
                         212,
                         '213943e5e194954ee654911622774dc6e338ef0cd88df07970768b5912601190'),
 'btlevtcmd_RecoverFp': (2148596104,
                         148,
                         '258f93cac82ed466e78b6fbce35f6dc2b03f1fd0204cbbeb797bd776c8262bf8'),
 'btlevtcmd_GetBodyId': (2148592344,
                         116,
                         '4db02d11941acc2101d49c096b96567f0b342b86d3b942c0bf9402809d576c2c'),
 'btlevtcmd_GetStatusMg': (2148605172,
                           108,
                           'ec009752bfac06063fbe71a7defc1bb320dc98f675ca3e729258758e855aa938'),
 'btlevtcmd_GetHeight': (2148605392,
                         112,
                         '90cd998cddd70d9d0fa8cbfd0bdad54004630bcc47b93d24f91eefc8a33b4a23'),
 'btlevtcmd_GetPartsPos': (2148609432,
                           268,
                           '6bd729ff1bbe602dff82134020413527b1cf12e8e12b860e125f6791b3c3c97e'),
 'BtlUnit_GetBodyPartsId': (2148699288,
                            64,
                            '21ed3890883c9f9c73389ff655cd579de1edc7fd1aabc7dce0101ecc93ff811e'),
 'BtlUnit_RecoverHp': (2148690924,
                       36,
                       '84ccfffd597282677162c274938289bf3d316843c37089d93ce56d6d99749029'),
 'BtlUnit_RecoverFp': (2148690776,
                       148,
                       '420c1d4b5760383128546190f565d0dc491480818b44e846ff810a1620ecf332'),
 'BtlUnit_GetHeight': (2148697108,
                       8,
                       'b10ca69da15980aa09f724ea61a56e7649c02d07f97c2742caef9cb2fcdf7677'),
 'evtSetFloat': (2147681468,
                 900,
                 '07b1e305d3c2e3c1d3432397aa243d56c39c07fcafdb124586f1204b04f82921'),
 'evtGetFloat': (2147682368,
                 1212,
                 'f714b2a7ddc29e31034b9c52f37dfcf8cb0f8b21cf73c73a81da2ccc67e8e3d8'),
 'evtGetValue': (2147684960,
                 1112,
                 'ad7dce301c927dbc422a143f3ffb023aa32af8d4f3bb81c752fddc1b9e21bd7a'),
 'evtmgrCmd': (2147686072,
               5048,
               '4b6cdc0e9b940137848236a344598d586d850790e0b25beda4b9ed81da069d0e'),
 'evt_eff': (2147932528, 2572, '0a7b5b58e7d416b4518e3c48e461483d12826a6248009e9b60ab93a8de62fd0f'),
 'effGetSet': (2147870880, 136, '0fd4662fef83e21bee80f9c5340f39a8398f0b4ba6511d9412a3da80bca509bd')}

ANCHORS = {2148612468: '48011a79',
 2148612484: '28030000',
 2148612496: '88a302f9',
 2148612500: '80030284',
 2148612508: '40810008',
 2148612524: '4bf1d351',
 2148612348: '48011af1',
 2148612376: '88a302fa',
 2148612380: '80030284',
 2148612388: '40810008',
 2148596284: '4bf21825',
 2148596308: '2c04ffff',
 2148596320: '4800007c',
 2148596344: '4bf217e9',
 2148596356: '4bf217dd',
 2148596372: '4082003c',
 2148596384: '901e0208',
 2148596388: '48017149',
 2148596396: '901e020c',
 2148596404: 'b01e0200',
 2148596412: 'b01e0204',
 2148596424: 'b01e01fe',
 2148596436: '48017119',
 2148596160: '2c04ffff',
 2148596196: '4bf2187d',
 2148596208: '4bf21871',
 2148596224: '48017159',
 2148699312: '540007ff',
 2148699324: '80830000',
 2148699332: '80630000',
 2148605244: 'c0240114',
 2148605252: '4bf1e779',
 2148605456: '48016605',
 2148609512: '4bfe5ae5',
 2148609524: '4bfe5a81',
 2148609552: '48015d01',
 2148609564: '41820020',
 2148609580: 'fc00001e',
 2148609604: '41820020',
 2148609644: '41820020',
 2148690932: 'b003010c',
 2148690948: '4c810020',
 2148690828: '4bfd1c1d',
 2148690844: '4bfd1c0d',
 2148690884: 'b0640000',
 2147687948: '4bfff455',
 2147687964: '4bfff445',
 2147687968: '7cbe1a14',
 2147687980: '4bffeed1',
 2147688476: '4bffe825',
 2147688492: '4bffe815',
 2147688496: 'ec3f0072',
 2147688508: '4bffe481',
 2147682196: 'ec1f0032',
 2147682212: 'fc00001e',
 2147682236: '9004009c',
 2147685988: 'ec210032',
 2147686028: 'fc00081e',
 2147683420: 'ec210032',
 2147932628: '4bfc388d',
 2147932644: '4bfc387d',
 2147932656: '4bfc3871',
 2147932672: '4bfc3861',
 2147932692: '4bfc384d',
 2147932704: '4bfc3841',
 2147933092: '4bff0cfd',
 2147933096: 'a8030000',
 2147933120: '7c03002e',
 2147933308: '480d56e1',
 2147934968: '41820024',
 2147935004: '38600002'}

CONSTANTS = {2151807512: '44800000',
 2151807516: '3a800000',
 2150367152: '4330000080000000',
 2150557952: '7265636f7665727900',
 2150374624: '7265636f7665727900',
 2150668280: '000a0000802c1ce0',
 2150678732: '8006dc68',
 2150984672: '0001005e8034f2e4',
 2150649924: '80031a9c',
 2150650020: '80031dfc',
 2150650048: '8003200c',
 2150650248: '80032964',
 2150650256: '80032984'}

COMMANDS = {2150953700: (91, (2148612432, 4294967294, 4264967301)),
 2150953716: (29, (4264967301, 1)),
 2150953728: (110, ()),
 2150953732: (91, (2148592344, 4294967294, 4264967300)),
 2150953748: (91, (2148605172, 4294967294, 4264967302)),
 2150953764: (91, (2148605392, 4294967294, 4264967303)),
 2150953780: (60, (4264967303, 4264967302)),
 2150953792: (91, (2148609432, 4294967294, 4264967300, 4264967296, 4264967297, 4264967298)),
 2150953820: (53, (4264967297, 4264967303)),
 2150953832: (91,
              (2147932528,
               0,
               2150557952,
               0,
               4264967296,
               4264967297,
               4264967298,
               4264967301,
               0,
               0,
               0,
               0,
               0,
               0,
               0)),
 2150953896: (9, (120,)),
 2150953904: (91, (2148596252, 4294967294, 4264967300, 4264967301)),
 2150953924: (112, ()),
 2150953928: (9, (125,)),
 2150953936: (33, ()),
 2150953940: (91, (2148612312, 4294967294, 4264967301)),
 2150953956: (29, (4264967301, 1)),
 2150953968: (110, ()),
 2150953972: (91, (2148605172, 4294967294, 4264967302)),
 2150953988: (91, (2148605392, 4294967294, 4264967303)),
 2150954004: (60, (4264967303, 4264967302)),
 2150954016: (91, (2148592344, 4294967294, 4264967300)),
 2150954032: (91, (2148609432, 4294967294, 4264967300, 4264967296, 4264967297, 4264967298)),
 2150954060: (53, (4264967297, 4264967303)),
 2150954072: (91,
              (2147932528,
               0,
               2150557952,
               1,
               4264967296,
               4264967297,
               4264967298,
               4264967301,
               0,
               0,
               0,
               0,
               0,
               0,
               0)),
 2150954136: (9, (120,)),
 2150954144: (91, (2148596104, 4294967294, 4264967300, 4264967301)),
 2150954164: (112, ()),
 2150954168: (9, (125,)),
 2150954176: (33, ()),
 2150954180: (2, ()),
 2150954184: (1, ())}

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
        data=read(0x8034f2e4,0x1e8)
        if hashlib.sha256(data).hexdigest()!='dadf7e175f491ab9e4a12ae63411cd7806df2bd0e73ec2be0a12e0b3ba454179':
            raise ValueError('Badge tail full event mismatch')
        commands=parse_commands(data,0x8034f2e4,{})
        actual={c['address']:(c['opcode'],tuple(o['raw'] for o in c['operands'])) for c in commands}
        if actual!=COMMANDS:
            raise ValueError('Badge tail request graph mismatch')
    return dict(schema=1,scope='Static badge tail/scoped service routes; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),commands=len(commands),brothers=2)


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
