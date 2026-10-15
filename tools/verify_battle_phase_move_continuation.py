#!/usr/bin/env python3
"""Read-only whole source Phase/Move pins; static evidence, not gameplay proof."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

EVIDENCE = {'schema': 1,
 'source': 'US G8ME01 disc0 revision0 read-only DOL',
 'bodies': [{'name': 'memset',
             'address': '800050b4',
             'size': 48,
             'sha256': '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'},
            {'name': 'BattleGetMarioPtr',
             'address': '800f87a8',
             'size': 108,
             'sha256': '26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'},
            {'name': 'BattleGetUnitPtr',
             'address': '800f88cc',
             'size': 60,
             'sha256': '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'},
            {'name': 'BattleIncSeq',
             'address': '800f895c',
             'size': 68,
             'sha256': '6dfd37fed866d7feeb50f7439621e12efb16813a0565e596e8f8f87536da31eb'},
            {'name': 'BattleGetSeq',
             'address': '800f89a0',
             'size': 104,
             'sha256': '00dd741e83176881d659d4201d6c68033d217de3a408dce9965d23774520ab60'},
            {'name': 'BattleSetSeq',
             'address': '800f8a08',
             'size': 96,
             'sha256': 'c37da15d68a33b6a9bc520d6c77c96a476750cfbb01c9512d645c893f498479d'},
            {'name': 'btlseqMove',
             'address': '8011bc24',
             'size': 1680,
             'sha256': '57a7cfe302fd8f68ccd7d0fa464f72dc5465fda72c4e16c6907378fa8dc1bb6e'},
            {'name': 'btlseqPhase',
             'address': '8011c2b4',
             'size': 156,
             'sha256': '2010dae50a20ef4cb62d7d69f98d2e00264fb5c328e24c3ac2244423966db8b3'},
            {'name': 'BtlUnit_SetTotalHitDamage',
             'address': '80127434',
             'size': 12,
             'sha256': 'c86172079e01f08445366e7d74d9e2d1c5811d9640e134853cc33840b5852cd9'},
            {'name': 'BattleRunPhaseEvent',
             'address': '801297bc',
             'size': 144,
             'sha256': 'b17e5341a1551530fb20c1af3fc84659150d979b1f60ae930717400612759045'}],
 'move_table': {'address': '80363c8c',
                'bytes': 60,
                'sha256': '9679486ada681e3679c69be1a2eb0cbed6b9478c33a88284755ad90ed7dd40a4',
                'entries': ['8011bc6c',
                            '8011bc88',
                            '8011bc9c',
                            '8011bd38',
                            '8011bd5c',
                            '8011bda8',
                            '8011bdcc',
                            '8011be60',
                            '8011bec0',
                            '8011c120',
                            '8011c12c',
                            '8011c204',
                            '8011c230',
                            '8011c26c',
                            '8011c27c']},
 'move_linked_calls': [{'callsite': '8011bc44', 'function': '800f89a0'},
                       {'callsite': '8011bc70', 'function': '8011b0d0'},
                       {'callsite': '8011bc7c', 'function': '800f895c'},
                       {'callsite': '8011bc90', 'function': '800f895c'},
                       {'callsite': '8011bc9c', 'function': '800d5eb0'},
                       {'callsite': '8011bca8', 'function': '800f87a8'},
                       {'callsite': '8011bcc4', 'function': '800f89a0'},
                       {'callsite': '8011bcdc', 'function': '800f895c'},
                       {'callsite': '8011bce8', 'function': '800f895c'},
                       {'callsite': '8011bd00', 'function': '800f7718'},
                       {'callsite': '8011bd04', 'function': '800f76d4'},
                       {'callsite': '8011bd18', 'function': '800374ac'},
                       {'callsite': '8011bd34', 'function': '800f895c'},
                       {'callsite': '8011bd3c', 'function': '8011ace8'},
                       {'callsite': '8011bd50', 'function': '800f895c'},
                       {'callsite': '8011bd68', 'function': '800f88cc'},
                       {'callsite': '8011bd84', 'function': '801297bc'},
                       {'callsite': '8011bd9c', 'function': '800f895c'},
                       {'callsite': '8011bdac', 'function': '8011ace8'},
                       {'callsite': '8011bdc0', 'function': '800f895c'},
                       {'callsite': '8011bde8', 'function': '800f88cc'},
                       {'callsite': '8011be20', 'function': '801297bc'},
                       {'callsite': '8011be3c', 'function': '800f895c'},
                       {'callsite': '8011be54', 'function': '800f8a08'},
                       {'callsite': '8011be64', 'function': '8011ace8'},
                       {'callsite': '8011be74', 'function': '8011aa34'},
                       {'callsite': '8011be8c', 'function': '800f8a08'},
                       {'callsite': '8011be9c', 'function': '800f8a08'},
                       {'callsite': '8011beb4', 'function': '800f8a08'},
                       {'callsite': '8011bed0', 'function': '800050b4'},
                       {'callsite': '8011beec', 'function': '800f88cc'},
                       {'callsite': '8011bf54', 'function': '800f88cc'},
                       {'callsite': '8011bf80', 'function': '80127814'},
                       {'callsite': '8011bfc4', 'function': '800f88cc'},
                       {'callsite': '8011bfdc', 'function': '800f895c'},
                       {'callsite': '8011bfec', 'function': '800f8a08'},
                       {'callsite': '8011c014', 'function': '800f88cc'},
                       {'callsite': '8011c048', 'function': '800f88cc'},
                       {'callsite': '8011c05c', 'function': '800d5eb0'},
                       {'callsite': '8011c124', 'function': '8011b2dc'},
                       {'callsite': '8011c130', 'function': '8011aa34'},
                       {'callsite': '8011c148', 'function': '800f8a08'},
                       {'callsite': '8011c158', 'function': '800f8a08'},
                       {'callsite': '8011c16c', 'function': '800f88cc'},
                       {'callsite': '8011c17c', 'function': '80127434'},
                       {'callsite': '8011c198', 'function': '8011bb88'},
                       {'callsite': '8011c1d4', 'function': '800f89a0'},
                       {'callsite': '8011c1d8', 'function': '801a7c68'},
                       {'callsite': '8011c1e4', 'function': '800f895c'},
                       {'callsite': '8011c1fc', 'function': '800f8a08'},
                       {'callsite': '8011c204', 'function': '801a6ce8'},
                       {'callsite': '8011c214', 'function': '8011ace8'},
                       {'callsite': '8011c228', 'function': '800f895c'},
                       {'callsite': '8011c234', 'function': '801956f8'},
                       {'callsite': '8011c240', 'function': '80195af8'},
                       {'callsite': '8011c254', 'function': '800f895c'},
                       {'callsite': '8011c264', 'function': '800f8a08'},
                       {'callsite': '8011c274', 'function': '800f895c'},
                       {'callsite': '8011c284', 'function': '800f895c'},
                       {'callsite': '8011c294', 'function': '800f8a08'}],
 'boundary_anchors': {'8011bc3c': '3bc00000',
                      '8011bd34': '4bfdcc29',
                      '8011bd38': '7fa3eb78',
                      '8011bf6c': '8b3f0020',
                      '8011bf80': '4800b895',
                      '8011bf98': '2c190001',
                      '8011c058': '40820240',
                      '8011c0a4': '4200ffd4',
                      '8011c10c': '4180018c',
                      '8011c11c': '4800017c',
                      '8011c120': '7fa3eb78',
                      '8011c124': '4bfff1b9',
                      '8011c1e8': '480000b0',
                      '8011c228': '4bfdc735',
                      '8011c22c': '4800006c',
                      '8011c298': '2c1e0000',
                      '8011c29c': '4082f99c'},
 'correction': 'Move8 does not fall through9: all branches reach8011c298 with local immediate=0; '
               'Act only separate entry9.'}

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol = struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', header, b+4*i)[0]
                          for b in (0, 0x48, 0x90)) for i in range(18)]
        def read(address, size):
            offsets = [off+address-va for off, va, n in sections
                       if off and va <= address and address+size <= va+n]
            if len(offsets) != 1:
                raise ValueError('Ambiguous/unmapped source span')
            disc.seek(dol+offsets[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated source span')
            return data
        for expected in EVIDENCE['bodies']:
            raw = read(int(expected['address'], 16), expected['size'])
            if hashlib.sha256(raw).hexdigest() != expected['sha256']:
                raise ValueError('Original body changed: '+expected['name'])
        table = EVIDENCE['move_table']
        raw = read(int(table['address'], 16), table['bytes'])
        if hashlib.sha256(raw).hexdigest() != table['sha256'] or list(struct.unpack('>15I', raw)) != [int(v,16) for v in table['entries']]:
            raise ValueError('Move dispatch changed')
        for address, expected in EVIDENCE['boundary_anchors'].items():
            if read(int(address,16),4).hex() != expected:
                raise ValueError('Move source boundary changed: '+address)
        move = next(v for v in EVIDENCE['bodies'] if v['name']=='btlseqMove')
        start = int(move['address'],16)
        raw = read(start,move['size'])
        calls = []
        for i in range(0,len(raw),4):
            word = struct.unpack_from('>I',raw,i)[0]
            if word & 0xfc000003 == 0x48000001:
                relative = word & 0x03fffffc
                if relative & 0x02000000:
                    relative -= 0x04000000
                calls.append(dict(callsite=f'{start+i:08x}', function=f'{(start+i+relative)&0xffffffff:08x}'))
        if calls != EVIDENCE['move_linked_calls']:
            raise ValueError('Move complete original callsite list changed')
    return dict(bodies=len(EVIDENCE['bodies']), dispatch_entries=15,
                linked_calls=len(calls), source_boundary_pins=len(EVIDENCE['boundary_anchors']))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
