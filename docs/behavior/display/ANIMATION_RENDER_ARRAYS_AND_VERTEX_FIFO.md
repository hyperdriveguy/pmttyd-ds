# Renderer arrays,index width and triangle-fan FIFO emission

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete renderProc,
local8003d06c..8003d5f8 plus initial shape captures. No original execution.
[Entry/default](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md) and
[overrides](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md) precede this block EACH
admitted draw. materialProc has no corresponding emission block.

## Shape versus draw offsets

At initial shape admission,capture shape position base H+40,normal base H+48,
color base H+50. Position/normal byte offsets are12*base,color4*base.
Also capture eight UV-array bases:
UV[k]=word[initial D+18c]+8*word[H+58+8*k],k0..7.
These remain from that initial resource/shape even if material helpers change
CURRENT W/D/P later. Do not substitute per-draw offsets for these array bases.

After overrides,read CURRENT D from W+20. Call-run pointer R=word[D+150]+
8*word[B+38];signed runCount=word[B+3c]. Capture texture slot count N=B+0,
vertex-stream base offsets Ip=B+40,In=B+44,Ic=B+48,and UV-offset table B+4c.
Color array=word[current D+164]+captured color byte offset.
If CURRENT W+e8!=0,position array=W+f8+captured position byte offset,
normal array=W+fc+captured normal byte offset. Otherwise current pose+4c/+54
supply those bases. Resource/pose/captures are intentionally mixed at these
specified points;loaded-source/alias invariants remain separate.

## Descriptor/format/array requests

GXClearVtxDesc;then for each entry below,in order,
GXSetVtxDesc(attribute,3),GXSetVtxAttrFmt(0,attribute,components,type,0),
GXSetArray(attribute,pointer,stride):

| Attribute | Components | Type | Array pointer | Stride |
| --- | --- | --- | --- | --- |
| 9(position) | 1 | 4 | position array | 12 |
| 10(normal) | 0 | 4 | normal array | 12 |
| 11(color0) | 1 | 5 | color array | 4 |

For N==1,attribute13 uses components1,type4,UV[k],stride8,where
k=SIGN-EXTENDED byte[B+30]. No k0..7 validation;negative/out-of-range accesses
can escape the captured local eight-pointer table.
For N!=1,for j0 while signed j<N,configure attribute13+j with those same
format/stride values,but k=SIGNED byte[B+30+N−j−1]. Thus coordinate attributes
consume reversed draw-selector order. N<=0 configures no UV attributes.
No N<=8 guard or resource array/index validation occurs locally.
Attribute names,16-bit descriptor type3 and primitive0xa0 match the
[primary libogc GX interface](https://raw.githubusercontent.com/devkitPro/libogc/master/gc/ogc/gx.h).
Original SDK register/FIFO implementations remain separate.

## Per-call primitive and three common index streams

For each run while signed i<captured runCount:
first=word[R+0],count=LOW16(word[R+4]). Request GXBegin(0xa0,0,count).
0xa0 is a TRIANGLE FAN,not an inferred strip/triangle list. Zero count still
requests GXBegin but emits no local indices. Run advances8 bytes after emission.

Position index address=word[CURRENT D+158]+4*(first+Ip)+2;
normal=word[CURRENT D+160]+4*(first+In)+2;
color=word[CURRENT D+168]+4*(first+Ic)+2.
Each per-vertex lhz reads the LOW half of a big-endian32-bit index slot.
Advance each pointer4 bytes per vertex;emit position,normal,color halfwords
in that order to EXACT MMIO FIFO address0xcc008000 via sth. No overflow/clamp/
index-domain check. Physical FIFO effects/cache behavior remain platform work.

## Single versus multiple texture index reads

N==1:k=signed byte[B+30]. UVfirst=word[B+4c+4*k];
UV index pointer=word[CURRENT D+16c+4*k]+4*(first+UVfirst)+2.
Each vertex emits lhz from that pointer AFTER position/normal/color,then advances4.
Original loop unrolls FOUR vertices with remainder,in the same vertex/component
order. It does not batch all positions then all UVs.

N!=1:for each vertex v,emit the three common indices above,then for j0..N−1
(signed comparison) let k=signed byte[B+30+N−j−1],UVfirst=B+4c+4*k.
Reload CURRENT D through W+20 for each UV read,then
I=word[word[D+16c+4*k]+4*(first+word[UVfirst])+4*v].
Emit LOW16(I) with sth. This path uses full lwz per UV entry then truncates,
whereas the single path uses lhz from+2. N<=0 emits no UV indices. The UV loop
is four-slot unrolled plus remainder;preserve reverse selector and store order.
No vertex data itself is pushed here:indices reference previously set arrays.

## Local completion and boundaries

After runs,advance captured draw B by108 and continue the initial signed draw
count loop. No GX state/array/model matrix restore,GXEnd helper or CPU primitive
conversion request occurs. A portable renderer may translate fans into triangle
lists while preserving vertex/attribute/visibility/material results;that is a
porting choice,not original code behavior or measured numeric equivalence.
`verify_animation_render_material.py` pins complete original words and source
fan/count metadata. SDK/GX/cache/normal-matrix/resource/source/lifetime/numeric
and original frame outputs remain open.
