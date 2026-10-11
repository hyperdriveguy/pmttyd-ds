"""Generate isolated test saves positioned inside the original plaza road exits."""
import struct
from pathlib import Path
out=Path(__file__).resolve().parents[1]/'artifacts/fixtures';out.mkdir(parents=True,exist_ok=True)
for case,x,z in [('east',5.05,.5),('west',-5.05,.5),('harbor',0,5.1),('harbor-arrival',0,-5.1),('station',0,-5.1),('invalid-room',50,50)]:
    b=bytearray(32);b[:4]=b'PM2D';struct.pack_into('<IffII',b,4,2,x,z,7,1);b[25]=1
    if case=='station':b[26]=3
    if case in ('invalid-room','harbor-arrival'):b[26]=1;b[27]=12
    h=2166136261
    for v in b[:28]:h=((h^v)*16777619)&0xffffffff
    struct.pack_into('<I',b,28,h);(out/(case+'-road.bin')).write_bytes(b)
