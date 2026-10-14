# Original Trouble Center window display and US cache

US G8ME01 disc0 revision0; read-only source recovery2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Complete original
`irai_window_disp` gor_irai.o,rel/gor.rel filef660/8cc, annotated address
805ca000. Relocated calls and HA/LO constants inspected independently.
Complements [board selection and acceptance](../field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md).
Addresses/offsets hexadecimal; dimensions/counts decimal. Static source evidence,
not original raster/Font/GX execution or native visual parity.

## No work94 animation owner in this display

The complete display body does **not** read or write work+94. Its reviewed
init/main producers zero94 at initialization and when cursor changes, but this
body supplies no increment, blink phase, highlight animation or time source.
The earlier board contract's label "draw-owned counter" is provisional and
unsupported as an ownership claim. Preserve this raw field without inventing
animation. No work-state check or menu-input mutation appears in this display.
Its only retained-work reads are count/list and current smooth scroll98.

## Frame request order and original title text

Read work from second callback argument r4. Disable fog first with
GXSetFog(type0,0,0,0,0,colorffffffff). Request original
windowDispGX_Waku_col(style0,color,x,y,width,height,radius12) in this order:

| Order | RGBA | X,Y | Width,height |
| --- | --- | --- | --- |
| 1 | 2ba3ffff | -280,170 | 135,90 |
| 2 | ffffffff | -129,170 | 410,340 |
| 3 | 2ba3ffff | -103,190 | 120,40 |
| 4 | 2ba3ffff | 91,190 | 120,40 |

Between frame1 and frame2, draw the two left-side title strings. After
frame2/3/4 draw the two column headers. These are source-coordinate requests,
not native top-left pixel rectangles. Waku/Font providers and 608x480-to-3DS
presentation must retain their own source coordinate/material semantics.
Original textures/materials remain required; generated decorations are not a
source-equivalent renderer.

For the first title pair: FontDrawStart,FontDrawColor(ffffffff),
FontDrawScale(0.8500000238418579),msgSearch(irai_name1_win),FontDrawEdge,
FontGetMessageWidth. Keep only the returned low16 width, convert to float.
If width>115, make X scale115/width and effective centering width115;
otherwise X scale1, effective width=measured width. Y/Z matrix scale1.
Translation X=FMA(135-effectiveWidth,.5,-280),Y152,Z0.
PSMTXConcat(translation,scale,translation),FontDrawStringMtx(matrix,string).

Then msgSearch(irai_name2_win),same width/cap115/matrix operations,translation
Y127. The prior edge setting remains active for this second string.
FontDrawEdgeOff only after both titles. Original IDs resolve exact US resources;
do not rewrite or combine them into a new title.

After all remaining frames: FontDrawStart,white,scale.85,
msgSearch(irai_client_win),Edge,measurement low16. Cap100 using same matrix
logic. Translation X=FMA(120-effectiveWidth,.5,-103),Y180,Z0; draw,EdgeOff.
Then msgSearch(irai_contents_win),Edge,same cap100,translation
X=FMA(120-effectiveWidth,.5,91),Y180,Z0;draw,EdgeOff.

Source matrix arithmetic uses single operations/fused fmadds where shown.
SDK matrix and FontGetMessageWidth details are providers, not permission to
substitute a character-count width or wrapping layout. The .85 Font scale and
additional X-only matrix scaling are separate retained states.

## Source clipping and complete list iteration

Save GXGetScissor(x,y,width,height) into four stack words. Set exactly
GXSetScissor(175,98,410,304). FontDrawStart,FontDrawColor(000000ff),
FontDrawScale(.85). Iterate signed list index i from0 while i<work.count;
**every listed row** is submitted, including rows outside the clip and completed
quests. There is no loop restricted to top..top+9 and no selected-row underline.

For each row, read ID=work.list[i]. Request swGet(5374+ID). Nonzero publishes
FontDrawColor(808080ff); zero publishes000000ff. Then request swByteGet1420,
reread list[i],and if it equals the returned active quest publishff0000ff.
Active red therefore overrides completed gray. Both switch observations occur
per row, not a frame-wide cached state or a completion-based list filter.
The main controller's source A guard remains separate from this draw styling.

Read cached client at work+9c+21*(ID-1). Row Y is single
float(140-30*i)+work.scroll98. FontGetMessageWidth(client),retain low16 width.
Copy source vector(0,.85,.85),then replace its X component:

- width>180: X=.85*(180/width), single divide then multiply.
- otherwise X=.85.

FontDrawScaleVec(vector),FontDrawString(client,X=-119,Y=rowY).
Then independently reread scroll98 and compute same rowY for cached contents
at work+312+31*(ID-1),measure low16 width,copy(0,.85,.85),replaceX:
width>260→.85*(260/width),otherwise.85.
FontDrawScaleVec,FontDrawString(contents,X=51,Y=rowY).
Then i++,row integer offset+=30,list offset+=4; repeat.

After the list restore all four saved scissor words with GXSetScissor. There
is no local fog restoration or Font color/scale restoration. Broader renderer
state ownership is a separate requirement; a native adapter must restore its
own pipeline state without claiming an original request that is absent here.

## All30 table bindings and exact US source/cache path

Original ADDR32 relocations bind client table file65ab8/78 and contents table
65b30/78. Both tables are in **quest ID order**, independently of the selection
list's story-dependent ordering (20/21 precede19). All entries were resolved;
for each one-based Q=1..30, table offset4*(Q-1) maps to:

| Quest IDs | Client IDs | Contents IDs |
| --- | --- | --- |
| 1..10 | irai_client_01..irai_client_10 | irai_contents_01..irai_contents_10 |
| 11..20 | irai_client_11..irai_client_20 | irai_contents_11..irai_contents_20 |
| 21..30 | irai_client_21..irai_client_30 | irai_contents_21..irai_contents_30 |

These are exact two-digit ID sequences, with no missing or substituted entry.
Relocation targets for client01..30 are file1e870..1ea40 at step10;
contents01..30 are1ea50..1ec94 at step14. Raw unrelocated zero table words
must not be treated as null pointers or sufficient table identity evidence.

`irai_init_func` calls msgSearch on **every** client/contents ID, including
currently locked/unlisted tasks. strcpy caches client text at9c+21*(Q-1),
contents at312+31*(Q-1),including the terminator. Actual allocator/memset
size is416c. There is no
local bounded copy or US text rewriting. Source msgSearch bank loading and
pointer lifetime remain its service owners.

Independent read-only ISO message census found all64 IDs (60 row IDs plus4
headers) in original `msg/US/gor_02.txt`, ISOoffset1177698108,size128972,
SHA2561958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16.
Client maximum is11 bytes; contents maximum23 bytes, excluding NUL; all observed
US entries fit the original21/31-byte cache slots. This is resource evidence,
not a generic bound proved by strcpy. No token stripping/normalization is
permitted. Existing tools/message_assets.py performs exact byte copying into
ignored romfs/messages/gor_02.txt; no new message/art payload is committed.

Temporary `/tmp/trouble-window-us-resource.json` retains exact resource key/text
offsets,bytes and per-entry SHA256 for each of the64 IDs. Quest2:
client_02 has8 bytes,SHA256823da343fe8bd0334d09ddb152b0caa0a36f27dd0ebb576073e8ee6eabee95c8;
contents_02 has16 bytes,SHA2560f3fb0f9b849bfe02e602d2d526ad5351de1336c0c170f3799d441ca6b449680.
The four header text hashes respectively:
90a3ab332bbbac5659ffd4ff915da1a6060e189b652954ba0c192f11b9e34254,
4de3f5610280d8d45b70e965f09557449359d396e2dbcdedd975dec93f371be9,
9e506ebc2f8eb69eb0d747b771ce0243a2508b5005d6f1475e0a98808ee64851,
b741b9dab16b9bfe688a42a4f175b436953687993142a5f696cdfe322d04498e.
Hashes identify unchanged US content without distributing the extracted prose.
Original resources remain Nintendo/Intelligent Systems; local inspection and
conversion do not confer redistribution rights.

## Evidence and remaining validation

Original REL SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Complete unmodified display-span SHA256
4d8f05dfe5567b332ea9bf907235028c7b70f8818af41d51e253df353f9b3858.
Call relocation and per-instruction constant annotation reports remain
`/tmp/trouble-display-disassembly.txt` and `/tmp/trouble-display-annotated.txt`;
inspection ELF/raw module/US text census remain temporary, outside history.
No binary code, generated art, backend, shared index/progress or inbox edited.

Remaining: native retained request renderer using original Waku/font/material
assets; exact Font/GX/SDK numeric behavior and global state restoration;
original case9/board/door integration; matched original screenshot/execution;
3DS scissor/shape/text behavior and performance/hardware validation. This
contract establishes the original draw requests, not native visual completion.
