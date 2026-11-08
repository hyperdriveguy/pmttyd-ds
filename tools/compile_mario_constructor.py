#!/usr/bin/env python3
"""Compile pinned whole US Mario constructor bodies to a fixed native graph.

The build consumes the checked-in graph, never a runtime decoder or ISO. This
read-only extractor/verification utility refuses changed source fingerprints.
"""
import argparse,hashlib,json,re,struct
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]

def signed(v,bits):return v-(1<<bits) if v&(1<<(bits-1)) else v

def compile_instruction(address,w,internal):
 op=w>>26;d=(w>>21)&31;a=(w>>16)&31;b=(w>>11)&31;imm=signed(w&65535,16);next_address=address+4
 address_expression=f'(r[{a}]+uint32_t({imm}))' if a else f'uint32_t({imm})'
 advance=f'c.pc=0x{next_address:08x};break;'
 record='compare(c,0,signedWord(r[%d]),0);' % a if w&1 else ''
 if op in (14,15):
  value=imm if op==14 else imm*65536
  return f'r[{d}]='+ (f'r[{a}]+' if a else '')+f'uint32_t({value});'+advance
 if op in (12,13):return f'const uint64_t value=uint64_t(r[{a}])+uint32_t({imm});c.carry=value>>32;r[{d}]=uint32_t(value);'+(f'compare(c,0,signedWord(r[{d}]),0);' if op==13 else '')+advance
 if op==8:return f'const uint64_t value=uint64_t(uint32_t({imm}))+uint64_t(~r[{a}])+1;c.carry=value>>32;r[{d}]=uint32_t(value);'+advance
 if op==7:return f'r[{d}]=r[{a}]*uint32_t({imm});'+advance
 if op in (10,11):
  field=(w>>23)&7;right=str(w&65535) if op==10 else str(imm);left=f'uint64_t(r[{a}])' if op==10 else f'signedWord(r[{a}])'
  return f'compare(c,{field},{left},{right});'+advance
 if op in (28,29):
  v=(w&65535)<<(16 if op==29 else 0);return f'r[{a}]=r[{d}]&0x{v:08x};compare(c,0,signedWord(r[{a}]),0);'+advance
 if op in (24,25):
  v=(w&65535)<<(16 if op==25 else 0);return f'r[{a}]=r[{d}]|0x{v:08x};'+advance
 if op==21:
  sh=(w>>11)&31;mb=(w>>6)&31;me=(w>>1)&31;mask=0
  for i in range(32):
   if (mb<=me and mb<=i<=me) or (mb>me and (i>=mb or i<=me)):mask|=1<<(31-i)
  return f'r[{a}]=rotate(r[{d}],{sh})&0x{mask:08x};'+record+advance
 if op in (32,33,34,40,42,48):
  reader='byte' if op==34 else 'half' if op in (40,42) else 'word'
  destination=f'f[{d}]' if op==48 else f'r[{d}]'
  if op==33:
   return f'const auto target={address_expression};uint32_t value;if(!word(target,value))return {{}};r[{d}]=value;r[{a}]=target;'+advance
  if op==42:return f'uint32_t value;if(!half({address_expression},value))return {{}};r[{d}]=signHalf(value);'+advance
  return f'if(!{reader}({address_expression},{destination}))return {{}};'+advance
 if op in (36,37,38,44,52):
  value=f'f[{d}]' if op==52 else f'r[{d}]'
  if op==37:return f'const auto target={address_expression};if(!store(target,{value}))return {{}};r[{a}]=target;'+advance
  if op==38:return f'if(!s.storeByte||!s.storeByte(s.user,{address_expression},uint8_t({value})))return {{}};'+advance
  if op==44:return f'if(!s.storeHalf||!s.storeHalf(s.user,{address_expression},uint16_t({value})))return {{}};'+advance
  return f'if(!store({address_expression},{value}))return {{}};'+advance
 if op==18:
  disp=signed(w&0x3fffffc,26);target=(disp if w&2 else address+disp)&0xffffffff
  if w&1:
   if target in internal:return f'c.lr=0x{next_address:08x};c.pc=0x{target:08x};break;'
   return f'if(!external(0x{target:08x},0x{next_address:08x}))return {{}};break;'
  return f'c.pc=0x{target:08x};break;'
 if op==16:
  bo=(w>>21)&31;bi=(w>>16)&31;disp=signed(w&65532,16);target=(disp if w&2 else address+disp)&0xffffffff
  cond=(('true' if bo&16 else f'(c.condition[{bi}]=='+('true)' if bo&8 else 'false)')))
  ctr=('true' if bo&4 else ('c.ctr==0' if bo&2 else 'c.ctr!=0'))
  return ('' if bo&4 else '--c.ctr;')+(f'c.lr=0x{next_address:08x};' if w&1 else '')+f'c.pc=({ctr})&&({cond})?0x{target:08x}:0x{next_address:08x};break;'
 if op==19:
  xo=(w>>1)&1023
  if xo==16 and ((w>>21)&31)==20:return 'c.pc=c.lr;break;'
  if xo==528 and w&1:
   known='||'.join(f'c.ctr==0x{target:08x}' for target in sorted(internal))
   return f'if({known}){{c.lr=0x{next_address:08x};c.pc=c.ctr;break;}}if(!external(c.ctr,0x{next_address:08x}))return {{}};break;'
 if op==31:
  xo=(w>>1)&1023
  if xo in (339,467):
   spr=((w>>16)&31)|((w>>6)&992);reg={8:'c.lr',9:'c.ctr'}.get(spr)
   if reg:return (f'r[{d}]={reg};' if xo==339 else f'{reg}=r[{d}];')+advance
  if xo==23:return f'if(!word('+ (f'r[{a}]+' if a else '')+f'r[{b}],r[{d}]))return {{}};'+advance
  if xo in (151,215,407):
   ae=(f'r[{a}]+' if a else '')+f'r[{b}]'
   if xo==151:return f'if(!store({ae},r[{d}]))return {{}};'+advance
   method='storeByte' if xo==215 else 'storeHalf';cast='uint8_t' if xo==215 else 'uint16_t'
   return f'if(!s.{method}||!s.{method}(s.user,{ae},{cast}(r[{d}])))return {{}};'+advance
  if xo==87:return f'if(!byte('+ (f'r[{a}]+' if a else '')+f'r[{b}],r[{d}]))return {{}};'+advance
  if xo==343:return f'uint32_t value;if(!half('+ (f'r[{a}]+' if a else '')+f'r[{b}],value))return {{}};r[{d}]=signHalf(value);'+advance
  if xo==279:return f'uint32_t value;if(!half('+ (f'r[{a}]+' if a else '')+f'r[{b}],value))return {{}};r[{d}]=value;'+advance
  if xo==266:return f'r[{d}]=r[{a}]+r[{b}];'+(f'compare(c,0,signedWord(r[{d}]),0);' if w&1 else '')+advance
  if xo==444:return f'r[{a}]=r[{d}]|r[{b}];'+record+advance
  if xo==954:return f'r[{a}]=(r[{d}]&255)<128?(r[{d}]&255):(r[{d}]|0xffffff00u);'+record+advance
  if xo==0:return f'compare(c,{(w>>23)&7},signedWord(r[{a}]),signedWord(r[{b}]));'+advance
  if xo==32:return f'compare(c,{(w>>23)&7},uint64_t(r[{a}]),uint64_t(r[{b}]));'+advance
 if op in (50,54):
  if op==54:return f'const auto value=doubleBits(double(scalar(f[{d}])));while(c.step<2){{if(!store({address_expression}+4*c.step,uint32_t(value>>(c.step?0:32))))return {{}};++c.step;}}c.step=0;'+advance
  return f'if(!c.step){{if(!word({address_expression},c.transfer))return {{}};c.step=1;}}uint32_t low;if(!word({address_expression}+4,low))return {{}};f[{d}]=bits(float(doubleScalar(uint64_t(c.transfer)<<32|low)));c.step=0;'+advance
 if op in (56,60):
  # Used scalar ABI saves/restores are unquantized paired-single words.
  displacement=signed(w&4095,12);idx=(w>>12)&7;one=(w>>15)&1
  if idx or one:raise ValueError('Unsupported quantized caller frame')
  ae=f'(r[{a}]+uint32_t({displacement}))'
  if op==60:return f'while(c.step<2){{if(!store({ae}+4*c.step,c.step?c.paired[{d}]:f[{d}]))return {{}};++c.step;}}c.step=0;'+advance
  return f'if(!c.step){{if(!word({ae},f[{d}]))return {{}};c.step=1;}}if(!word({ae}+4,c.paired[{d}]))return {{}};c.step=0;'+advance
 raise ValueError(f'Unsupported instruction{w:08x} at{address:08x} op{op} xo{(w>>1)&1023}')

def generate(iso=DEFAULT_ISO):
 bodies=json.loads((ROOT/'tools/mario_constructor_bodies.json').read_text())['bodies'];dol=Dol(iso);out=['// Whole original US revision0 Mario constructor and reached source leaves.','// Generated native continuation graph; verify with compile_mario_constructor.py --check.']
 internal={int(b['address'],16) for b in bodies}
 try:
  for body in bodies:
   a=int(body['address'],16);raw=dol.read(a,body['size']);sha=hashlib.sha256(raw).hexdigest()
   if sha!=body['sha256']:raise ValueError('Original complete body fingerprint differs')
   out.append(f'// {body["name"]} {a:08x}/{len(raw):x} SHA256 {sha}')
   for offset in range(0,len(raw),4):
    at=a+offset;w=struct.unpack_from('>I',raw,offset)[0];code=compile_instruction(at,w,internal)
    out.append(f'case 0x{at:08x}:{{{code}}} // {w:08x}')
 finally:dol.close()
 return '\n'.join(out)+'\n'
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--check',action='store_true');p.add_argument('--output',type=Path);args=p.parse_args();data=generate(args.iso);target=ROOT/'source/field/source_mario_constructor_program.inc'
 if args.check:
  if target.read_text()!=data:raise SystemExit('Compiled source graph differs')
  print('Verified',data.count('case 0x'),'original Mario constructor/child instructions')
 elif args.output:
  if args.output.exists():raise SystemExit('Output must not already exist')
  args.output.write_text(data)
 else:p.error('--check or --output required')
