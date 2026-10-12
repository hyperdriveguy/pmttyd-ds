# Animation texture objects, palette selection and loading

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

Complete animSetMaterial_Texture body:8003dcfc,size0x5ac. This file specifies
its first pass;[coordinate matrices](ANIMATION_TEXTURE_COORDINATE_MATRICES.md)
specifies its second. The original renderProc/materialProc callers both request
this helper during their [default material setup](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md).
A nonzero pose+e4 skips that caller-owned default setup. The helper itself has
no frame/tick gate.

## Inputs and reverse slot selection

| Register | Retained input |
| --- | --- |
| r3 | Signed slot count N |
| r4 | Array of four-byte slot IDs |
| r5 | Eight-byte base texture records A |
| r6 | Twenty-four-byte texture state records T |
| r7 | Signed texture descriptor count C |
| r8 | Sixty-four-byte texture descriptor array D |
| r9 | Texture resource record U |

The first loop runs j=0 while signed j<N. It selects S=word[IDs+4*(N-j-1)],
base=A+8*S,and I=low32(word[base]+U8[T+24*S]). If signed I<0 or I>=C,it replaces
I with0 and calls printf8026a2a8 using diagnostic802c10c4. The diagnostic setup
reads the current work/pose/resource afresh to supply the resource name and
current animation-name address;the call visibly prepares those two arguments.
Its format also mentions numeric arguments,which this call does not explicitly
prepare. Do not invent a reliable printed numeric error value from that format.
This is a diagnostic,not original US story dialogue. A zero descriptor count
still falls back to descriptor0;there is no successful-range/null fallback.

Capture wrap override=word[base+4] and descriptor=D+64*I. Read its+4 TPL index
and+8 kind. Only kind0 loads an original image object in this pass. Other kinds
skip loading but still receive the second-pass coordinate request. No guard
validates S,slot array extent,texture state extent or N against GX capacity.

## Palette payload and image headers

Resolve palette payload as word[word[word[U+8]+0xa0]]. Call TEXGet80037b5c
with payload and TPL index. Its complete0x10-byte body returns
word[payload+8]+8*index,without checking a palette count,format or index.
The returned pair contains image-header pointer+0 and palette-header pointer+4.
If palette pointer nonzero,choose the CI branch;otherwise choose ordinary.
Each branch re-resolves the U handle chain and calls TEXGet a second time.
Retain that second pair;do not collapse the two lookups across helper effects.

| Image header field | Access / request meaning |
| --- | --- |
| +0,+2 | U16 height,width (width passed first) |
| +4,+8 | Word format,image-data pointer |
| +0c,+10 | Word default S/T wrapping |
| +14,+18 | Word minimum/maximum filter request |
| +1c | Float LOD bias |
| +20 | U8 edge-LOD request |
| +21,+22 | U8 minimum/maximum LOD |

Signed wrap override<0 retains header S/T. Otherwise S=(override&4)?2:(override&1)
and T=(override&8)?2:((override>>1)&1). Mipmap boolean is byte+21!=byte+22;
this body does not inspect available image levels.

## Exact GX request order

Ordinary branch:GXInitTexObj(local object,data,width,height,format,wrapS,wrapT,
mipmap)802b72b0;GXInitTexObjLOD802b7544;GXLoadTexObj(object,j)802b7880.
CI branch:first GXInitTlutObj802b78d4(local TLUT,palette word+8 data,word+4 format,
U16+0 count);then GXInitTexObjCI802b74fc with the ordinary parameters plus TLUT
ID0 as the stack argument;then LOD;GXLoadTlut(local TLUT,0)802b790c;GXLoadTexObj
(object,j). Every CI slot uses/reloads TLUT0. Original GX loading/state semantics
remain called boundaries;this local order alone is not proof of a portable
multitexture palette strategy.

Both LOD calls pass minFilter=word+14,magFilter=word+18,minLOD=float(byte+21),
maxLOD=float(byte+22),bias=float+1c,biasClamp=0,edgeLOD=byte+20,maxAniso=0.
The unsigned byte-to-float conversion uses original double bias802c1060,
0x4330000000000000;byte values are exactly representable. Header reads for the
LOD request occur after object initialization. No local source mutation,cache
flush,resource retention,GX restore or texture lifetime guarantee is supplied.

Original a/a_mario has24 one-slot draws and150 kind0 descriptors,as verified
by [the source witness](ANIMATION_RENDER_RESOURCE_EXAMPLE_AND_PORT_BOUNDARIES.md).
That witnesses this ordinary branch's source domain,not every palette/wrap/
LOD choice or an observed image. Loader relocation,image data ownership and
kind1/2 actual US scene bindings remain separate work.
