# Animation work initialization,pools and tangent table writer

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animInit0x80042854,size0x274. Actual direct DOL caller marioStInit8006fde4;
full caller scheduling/resource lifetime remains open. No original execution.

## Counts,allocations and retained work domains

Work pointer W is read/reloaded through0x80415dc0. Store W+4=64,W+c=64,
W+14=256,W+f0=1024,W+f4=0,with fresh W between writes. Request __memAlloc
0x800300f0 heap0 for these CURRENT retained count-derived lengths,in order:

| Publication field | Requested size | Purpose from consumer layout |
| --- | --- | --- |
| W+0 | 16*current W+4 | Resource records |
| W+8 | 12*current W+c | Secondary records;full consumer open |
| W+10 | 0x170*current W+14 | Pose records |
| W+ec | 12*current W+f0 | Auxiliary shared storage;full consumer open |

After each call reload W and store returned pointer. Set W+e8=0 before the
fourth allocation. No allocation failure guard,cleanup,retry or explicit pool
memset occurs here. Allocator's zeroing/ownership/cache behavior is separate;
do not infer initialized per-pose flags/timestamps from allocation alone.

Fresh W for each subsequent field write:zero30,34,38,24,28,2c,cc,d0,d4;
set d8=1,dc=1;zero104,10c. Read CURRENT W+104;only if null request
heap0 length0x180000 and publish W+104. Since104 was just zeroed,the stable
work-pointer domain allocates;the literal null guard still exists. Reentry can
lose prior allocations;no release is local. Later set W+108=CURRENT W+104.
Not all work fields are reset:in particular Main's alternative outputs W+f8/fc
are not set by this function. Do not replace this with a blanket work clear.

## Runtime tangent table

Fill178 binary32 words beginning0x803d9580. For i=0..177,signed angle=i−89;
convert integer through double bias0x802c1070=0x4330000080000000,subtract via
fsubs,and fmuls original factor0x3c8efa33=0.017453288659453392.
Request tan0x802724e8 with that single-rounded input,frsp the return and store
at tableBase+4*i. Iterate in ascending order;no copied static tangent asset.
This factor differs from some other game subsystems' nearby degree constants.
Do not silently reuse a mathematically exact/shared pi factor.

The zero-angle address is tableBase+89*4=0x803d96e4. Main's cubic node channel
uses signed byte tangent indices around that address,valid generated angles
−89..88. Its byte3 guard admits only−88..88;byte2 is unguarded.
Table storage is runtime/BSS and cannot be pinned as original DOL initialized
bytes. The complete writer/factor/libm request is pinned;libm numeric parity,
writer cadence and live-table lifetime remain separate.

## Original auxiliary file requests

Request fileAllocf0x8018a61c(type0,format at0x802c11d8=`a/vivian.bin`).
If returned handle Hnonnull:load payload P=**(H+a0),copy0xc00 bytes using
memcpy_as4 to word[CURRENT r13−0x7170+4];then reload payload and copy16 bytes
from P+c00 to address r13−0x7180;finally fileFree(H)0x8018a260.
Null handle skips those copies/release. There is no size/format/failure cleanup
check. Source pointers are reloaded for the second copy. Field naming/consumers
of these Vivian stores remain separate;no animation semantics are invented.

Request fileSetCurrentArchiveType(0)0x80189d38,then
fileAllocf(type0,format0x802c11e8=`a/ag2tg.bin`) and publish returned handle to
CURRENT W+100. No local null guard or immediate release follows that second
request. File acquisition/reference/holder lifetime follows the separately
recovered file contracts;this leaf does not prove all payload semantics.
No extra variadic arguments are supplied on either literal-format path.

## Validation boundary

`verify_animation_main.py` pins this complete local body and Main,1773 words,
17 constants,five original data spans (initial work/global pointers,bias,two
file strings),two US lecture imports and65 direct DOL sites. The original
M_Z_1 serialized resource example is separately hash/metadata checked. Initial
pointer spans are not live state. All heap/file/copy/libm/platform/resource
services and source/reentry/lifetime/visual domains remain incomplete.

ID1 address correction/consumer update (2026-10-12):binary lis/addi producers
are now checked by verify_animation_draw.py,including signed low9580. Correct
runtime tangent base803d9580 and zero row803d96e4 supersede prior803e addresses.
[Draw ring](ANIMATION_DRAW_SHARED_BUFFERS_AND_PAPER_SELECTION.md) now closes
local consumers of capacity/cursor/alternate outputs;source/lifetime remains open.
