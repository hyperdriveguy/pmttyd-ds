#!/usr/bin/env python3
"""Read a named US DOL function into an inspection-only ELF for llvm-objdump.
Corrects Gekko paired-single mnemonics after LLVM disassembly.
Never executes original code; outputs must stay outside source history.
"""
import argparse,csv,struct,subprocess,sys
from pathlib import Path
from iso_assets import DEFAULT_ISO
from gekko_disasm import correct_disassembly

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('name',nargs='?');p.add_argument('--symbols',required=True)
    p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',required=True)
    p.add_argument('--address',help='RAM address to inspect instead of a named symbol')
    p.add_argument('--length',help='Byte length for --address',type=lambda s:int(s,0))
    a=p.parse_args()
    if a.address is None:
        if not a.name: raise ValueError('Supply a function name or --address/--length')
        matches=[r for r in csv.DictReader(Path(a.symbols).open()) if r['area']=='_main' and r['name']==a.name and r['sec_type']=='text']
        if len(matches)!=1:raise ValueError('Choose one original DOL text symbol')
        symbol=matches[0];length=int(symbol['size'],16)
        if not 0<length<1024*1024:raise ValueError('Invalid function size')
        ram=int(symbol['ram_addr'],16);file_addr=int(symbol['file_addr'],16)
    else:
        if a.length is None or a.length<=0:raise ValueError('--address requires a positive --length')
        ram=int(a.address,0);length=a.length
        file_addr=None
    with Path(a.iso).open('rb') as f:
        if f.read(6)!=b'G8ME01':raise ValueError('US G8ME01 ISO required')
        f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0]
        if file_addr is None:
            f.seek(dol);header=f.read(0x100)
            texts=[(i,struct.unpack('>I',header[0x00+4*i:0x04+4*i])[0],struct.unpack('>I',header[0x48+4*i:0x4c+4*i])[0],struct.unpack('>I',header[0x90+4*i:0x94+4*i])[0]) for i in range(18)]
            file_addr=next((off+(ram-addr) for _,off,addr,size in texts if addr and addr<=ram<addr+size),None)
            if file_addr is None:raise ValueError('Address is outside the DOL loaded text/data image')
        f.seek(dol+file_addr);code=f.read(length)
    if len(code)!=length:raise ValueError('Truncated DOL function')
    strings=b'\0.text\0.shstrtab\0';offset=52;string_offset=offset+length;sections=(string_offset+len(strings)+3)&~3
    header=b'\x7fELF\x01\x02\x01'+bytes(9)+struct.pack('>HHIIIIIHHHHHH',1,20,1,0,0,sections,0,52,0,0,40,3,2)
    data=header+code+strings;data+=bytes(sections-len(data))
    data+=bytes(40)+struct.pack('>10I',1,1,6,ram,offset,length,0,0,4,0)+struct.pack('>10I',7,3,0,0,string_offset,len(strings),0,0,1,0)
    output=Path(a.output)
    if output.resolve()==Path(a.iso).resolve():raise ValueError('Cannot overwrite source ISO')
    output.parent.mkdir(parents=True,exist_ok=True);output.write_bytes(data)
    result = subprocess.run(['llvm-objdump','-d',str(output)],check=True,
                            capture_output=True,text=True)
    corrected, counts = correct_disassembly(result.stdout)
    sys.stdout.write(corrected)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if counts['unrecognized']:
        print(f"Warning: {counts['unrecognized']} unrecognized Gekko encodings emitted as .long",
              file=sys.stderr)
if __name__=='__main__':main()
