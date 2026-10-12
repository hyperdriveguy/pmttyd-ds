#!/usr/bin/env python3
"""Verify scoped literal US TEV base programs by static immediate/call decoding.

Only an allowlisted immediate/call/exit grammar is inspected. No original
instruction, GX call, CPU state or material program is executed.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census

MANIFEST = Path(__file__).with_name('map_tev_fixed_evidence.json')
CALLS = {
    0x802b8e30: ('GXSetTevOrder', 4),
    0x802b89e8: ('GXSetTevColorOp', 6),
    0x802b8a50: ('GXSetTevAlphaOp', 6),
    0x802b8960: ('GXSetTevColorIn', 5),
    0x802b89a4: ('GXSetTevAlphaIn', 5),
    0x802b8c98: ('GXSetTevSwapMode', 3),
}


def decode_literals(raw, address):
    literals, requests, stage_count, exit_seen = {}, [], None, False
    for i in range(0, len(raw), 4):
        word = struct.unpack_from('>I', raw, i)[0]
        pc = address+i
        op = word >> 26
        if op == 14 and (word >> 16) & 31 == 0:
            reg, imm = (word >> 21) & 31, word & 0xffff
            if imm & 0x8000:
                imm -= 0x10000
            if reg == 25:
                if stage_count is not None or i != len(raw)-8:
                    raise ValueError('Unexpected stage-count publication')
                stage_count = imm
            elif 3 <= reg <= 8:
                literals[reg] = imm
            else:
                raise ValueError('Unexpected register in literal program')
        elif op == 18:
            displacement = word & 0x03fffffc
            if displacement & 0x02000000:
                displacement -= 0x04000000
            target = (displacement if word & 2 else pc+displacement) & 0xffffffff
            if word & 1:
                if stage_count is not None or target not in CALLS:
                    raise ValueError('Unexpected literal-program request target')
                name, argc = CALLS[target]
                if any(r not in literals for r in range(3, 3+argc)):
                    raise ValueError('Request depends on nonliteral or clobbered argument')
                requests.append(dict(address=pc, target=target, name=name,
                                     args=[literals[r] for r in range(3, 3+argc)]))
                literals.clear()  # No value may survive a call by assumption.
            elif target != 0x80022f48 or i != len(raw)-4 or stage_count is None:
                raise ValueError('Unexpected literal-program exit')
            else:
                exit_seen = True
        else:
            raise ValueError('Instruction outside literal/call/exit grammar')
    if stage_count is None or not exit_seen:
        raise ValueError('Missing stage-count publication or final exit')
    return dict(stage_count=stage_count, requests=requests)


def verify_programs(iso, expected):
    found = []
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        base = struct.unpack('>I',disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        for spec in expected['literal_programs']:
            a,n = spec['address'],spec['size']
            spans = [(o,v,s) for i in range(7) for o,v,s in
                     [(struct.unpack_from('>I',header,j+4*i)[0] for j in (0,0x48,0x90))]
                     if v <= a and a+n <= v+s]
            if len(spans) != 1:
                raise ValueError('Unmapped literal program')
            o,v,_ = spans[0]
            disc.seek(base+o+a-v)
            actual = decode_literals(disc.read(n),a)
            if actual != spec['decoded']:
                raise ValueError('Literal TEV request arguments/order/count mismatch')
            found.append(dict(mode=spec['mode'],**actual))
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(),MANIFEST.resolve()):
        parser.error('Cannot overwrite ISO or pinned manifest')
    result = verify(args.iso,MANIFEST)
    expected = json.loads(MANIFEST.read_text())
    result['literal_programs'] = verify_programs(args.iso,expected)
    result['direct_reference_sites'] = verify_census(args.iso,expected)
    result['scope'] = 'Scoped fixed TEV modes1..7/12;containing identity not full semantic closure;no original execution'
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f"Verified {result['instruction_checks']} scoped words, eight fixed programs/"
          f"{sum(len(p['requests']) for p in result['literal_programs'])} decoded requests, "
          f"{result['data_checks']} dispatch span and six direct sites")


if __name__ == '__main__':
    main()
