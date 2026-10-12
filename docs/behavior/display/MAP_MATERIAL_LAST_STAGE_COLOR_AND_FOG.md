# Map material last-stage color and fog request order

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

Complete mapSetMaterialLastStageBlend80021860/0x320 and mapSetMaterialFog
800217c8/0x98 are called by animation default setup and map consumers. Current
map group is G=word[8041e660]+0x2f4*word[8041e664]. These globals select raw state;
no group count/index/null validation is implemented in these local bodies.

## Last-stage color

Inputs r3 flags F,r4 first color C1,r5 second color C2. Capture G before color
operations. Start local RGBA from8041f8fc=ffffffff. Every multiplication uses
unsigned byte operands,integer product p,and the literal signed multiply-high
sequence with signed magic0x80808081:

```text
h = signed_high32(signed32(0x80808081) * p)
q = arithmetic_shift_right32(h+p,7)
q = q + (unsigned32(q) >> 31)
store low8(q)
```

For all65536 byte pairs this equals floor(p/255),verified exhaustively by the
host checker. That is a proof of a decoded bounded integer expression,not original
execution. Truncation occurs after EACH admitted stage. Combining factors in
floating point or dividing by256 changes results.

Apply stages in this order:

1. If F&0x40,multiply every RGBA component by C1[0..3]. Otherwise C1 is not read.
2. ALWAYS read C2[0]. If !=255,multiply only current local alpha by that byte.
   **The helper reads C2+0,not C2+3.** Caller color pointers must retain this ABI.
3. Capture U16[G+4] once. If its bit2 set and F&0x800 clear,multiply RGBA by
   G bytes+16..19.
4. If captured group bit4 set and F&0x1000 clear,multiply by G+1a..1d.
5. If captured group bit8 set and F&0x800 clear,multiply by G+1e..21.
6. Copy local RGBA to request buffer;GXSetTevKColor(0,&RGBA)802b8b7c.

Group flags and group pointer are captured;the admitted color bytes are read
at their own arithmetic stage. This body writes local request buffers,not input
colors/group bytes. No local clamp,blend equation/GX backend implementation or
previous-state restoration is supplied. Note that [mapTestXLU](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md)
classifies a separate second-color alpha byte;do not silently make both helpers
read the same field.

## Fog

Call camGetCurPtr8000f2e4 FIRST and retain camera,then resolve current G.
If U16[G+4]&1,set type=word[804150c0+4*word[G+28]]. Pinned five-entry table is
[2,4,5,6,7]. No local bounds check. GXSetFog802b8ff4 receives that type,raw color
word G+34,float G+2c start,float G+30 end,float camera+30 near,float camera+34 far.
The raw color is copied to a local request buffer.

If bit1 clear,GXSetFog receives type0,color8041f900=ffffffff,and all four floats
zero from8041f930. The camera getter has already occurred even on this branch.
The helper does not itself require camera4;the [renderer](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md)
owns a separate camGetPtr(4)==camGetCurPtr() admission before calling it.
[Lighting](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md) first clears fog;
that caller's later admitted fog request can replace the lighting reset. No
original fog equation/depth packing,finite/error policy or restored state is
closed here. Actual scene group/fog/color producers and frame comparisons remain
necessary for full material equivalence.
