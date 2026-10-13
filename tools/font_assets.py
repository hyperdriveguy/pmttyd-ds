#!/usr/bin/env python3
"""Pack the local original US BFN glyph mask, mappings and patched widths."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from PIL import Image
from iso_assets import DEFAULT_ISO, inventory, decode_gx


def decode(raw):
    def h(at): return struct.unpack_from('>H', raw, at)[0]
    blocks = []
    at = 0x20
    for _ in range(struct.unpack_from('>I', raw, 12)[0]):
        size = struct.unpack_from('>I', raw, at+4)[0]
        if size < 8 or at+size > len(raw):
            raise ValueError('Invalid BFN block extent')
        blocks.append((raw[at:at+4], at, size))
        at += size
    info = next(at for tag, at, _ in reversed(blocks) if tag == b'INF1')
    gly = next(at for tag, at, _ in reversed(blocks) if tag == b'GLY1')
    first, cellw, cellh = h(gly+8), h(gly+12), h(gly+14)
    cols, rows, width, height = (h(gly+x) for x in (22,24,26,28))
    if (first, cellw, cellh, cols, rows, width, height) != (0,24,24,21,8,512,256):
        raise ValueError('Original US glyph layout changed')
    mappings = []
    for code in range(256):
        glyph = h(info+18)
        for tag, at, size in blocks:
            if tag != b'MAP1' or not h(at+10) <= code <= h(at+12):
                continue
            kind = h(at+8)
            if kind == 2:
                glyph = h(at+16+2*(code-h(at+10)))
            elif kind == 3:
                pairs = [(h(at+16+4*i),h(at+18+4*i)) for i in range(h(at+14))]
                glyph = next((g for c,g in pairs if c == code), glyph)
            else:
                raise ValueError('US font mapping domain changed')
            break
        mappings.append(glyph)
    widths = {}
    for tag, at, _ in blocks:
        if tag == b'WID1':
            for glyph in range(h(at+8),h(at+10)+1):
                widths.setdefault(glyph, tuple(raw[at+12+2*(glyph-h(at+8)):at+14+2*(glyph-h(at+8))]))
    if set(widths) != set(range(162)) or max(mappings) >= 162:
        raise ValueError('Original US glyph width coverage changed')
    # FontSetup maps both ASCII space and unmapped 0x8140 to replacement0;
    # ordered width writes7 then9 therefore leave glyph0 width9.
    if mappings[32] != 0 or h(info+18) != 0:
        raise ValueError('Original US setup alias changed')
    widths[0] = (widths[0][0],9)
    metrics = [(g%cols*cellw+b,g//cols*cellh+2,w,cellh-2,0,0) for g,(b,w) in sorted(widths.items())]
    pixels = decode_gx(raw[gly+32:gly+32+width*height//2],width,height,0)
    image = Image.new('RGBA',(width,height))
    image.putdata([(255,255,255,p[0]) for p in pixels])
    packed = struct.pack('<8sI256H',b'PMFONT1\0',len(metrics),*mappings)
    packed += b''.join(struct.pack('<HH4B',*m) for m in metrics)
    return packed,image


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--output',default='romfs')
    args=parser.parse_args();iso=Path(args.iso);out=Path(args.output)
    info=inventory(iso)
    if info['disc_id'] != 'G8ME01':raise ValueError('Original US ISO required')
    entry=next(r for r in info['files'] if r['path']=='f/papermarioset_US.bfn')
    with iso.open('rb') as disc:disc.seek(entry['offset']);raw=disc.read(entry['size'])
    if len(raw)!=entry['size']:raise ValueError('Truncated font')
    fingerprint=hashlib.sha256(raw+Path(__file__).read_bytes()+Path('tools/iso_assets.py').read_bytes()).hexdigest()
    manifest=out/'font.json'
    if manifest.exists() and (out/'font.pmfont').exists() and (out/'font.t3x').exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    packed,image=decode(raw);out.mkdir(exist_ok=True,parents=True)
    work=Path('local-assets/fonts');work.mkdir(exist_ok=True,parents=True)
    image.save(work/'font-us.png');(out/'font.pmfont').write_bytes(packed)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'font.t3x'),str(work/'font-us.png')],check=True,stdout=subprocess.DEVNULL)
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,source_path=entry['path'],source_offset=entry['offset'],sha256=hashlib.sha256(raw).hexdigest(),glyphs=162,conversion='Original I4 intensity used as alpha; source mappings/bearings/widths retained with ordered setup width patch',license='Original Nintendo/Intelligent Systems local asset; no redistribution rights'),indent=2)+'\n')


if __name__=='__main__':main()
