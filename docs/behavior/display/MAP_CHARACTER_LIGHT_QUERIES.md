# Character light bank selection, preset pointers and nearest query

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

Six complete query/preset bodies and the nearest-query comparator supply
[map lighting](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md)'s source selection.
Preset structures are runtime BSS,not initialized DOL payloads. Their
initialization/update/scene lifetime remain separate work.

| Body | Address / size | Complete local result |
| --- | --- | --- |
| lightGetPaperCraft | 8001b938 /0x0c | Pointer803c9690 |
| lightGetPaper | 8001b944 /0x40 | camGetCurNo()==4?803c9750:803c96f0 |
| lightCheckCharaLight | 8001b984 /0x6c | First active+character match→1,else0 |
| lightGetCharaAmbient | 8001b9f0 /0x70 | First active+character+ambient match pointer,elseNULL |
| lightGetNearObj | 8001bad0 /0x1fc | Ordered/filter-limited pointers and zero-filled tail |
| mapGetActiveGroup | 8002b430 /0x08 | Word[8041e664] |
| lightdrv _sort | 8001bccc /0x2c | Distance greater→1,less→-1,otherwise0 |

The lightGetPaper comparison against camera number6 has no separate result;
all non4 numbers use803c96f0. Literal lis803d plus SIGNED negative addi yields
803c9690/96f0/9750,not803d addresses. Preset records occupy96 bytes each.

For check/ambient,call mapGetActiveGroup. Return1 selects bank8041e658;all other
values select8041e650. Each bank is signed count at+0 and record-array pointer
at+4. Count<=0 returns absent. Scan96-byte records in source order;U16 record+0
bit1 means active,bit0x80 character,bit0x8000 ambient. Check requires bits1/80;
ambient requires all three. Neither tests type,distance or a separate null guard.

## Nearest-query captures and sort threshold

lightGetNearObj inputs r3 query origin V,r4 output pointer array,r5 signed maximum
M,r6 flags F. It calls mapGetActiveGroup twice. FIRST selects retained bank B.
SECOND selects bank used only to count active records. Both calls can observe
different values;do not collapse them. This initial active census counts ALL
bit1 records,including noncharacter and ambient records.

If active census>8,scan retained B's array in source order. Append an eight-byte
local pair(pointer,distance) for each active record;distance is
PSVECDistance8029eeb4(V,record+24). Retained B count is reread at each outer loop
test. Then request sorter800064fc(local pairs,pairCount,8,comparator8001bccc).
The comparator loads pair+4 floats and uses fcmpo;ordered greater returns1,
ordered less returns-1,equal/unordered returns0. Intended finite request order
is ascending distance. Sorter body,stability/equal-key handling,FPSCR effects and
nonfinite total ordering are not recovered by this comparator alone.

If active census<=8,append ONLY each active pointer in source order. Do not
compute distance or sort;the adjacent local distance words remain unwritten.
There is no nearest-first guarantee in this branch. Both paths start local pairs
at stack+8;the0x130-byte frame saves registers beginning+10c. Neither append loop
has a local pair-capacity guard. Source bank capacity invariants must be recovered;
M bounds final output attempts,not this temporary pair collection.

## Filtering, writes and return

Traverse the resulting pair list. Reread record U16 flags;reject ambient0x8000.
If low16(F)!=0,require at least one overlapping flag bit. Low16(F)==0 admits
any nonambient active record. Original flags do not require every requested bit.
For an eligible record:increment output count,write pointer,advance destination,
THEN test signed count>=M and break. Therefore M<=0 can still write one eligible
pointer;there is no prewrite zero-capacity return. If no eligible records,it writes
none. Final signed count<M zero-fills M-count trailing four-byte output entries,
using eight-word unroll plus remainder. Return count. Negative M does not cause
that tail fill. No selector consistency,output-null,capacity or record-pointer
validation is supplied locally.

A port should preserve ordinary positive-capacity used behavior and make any
safety extension explicit. Ambient records affect sort threshold and sorting
before they are excluded. Type and character restrictions belong to callers;
map lighting requests F=0x80. Query lifetime/scene mutation,sorter implementation,
original distance numeric behavior and observed selected lights remain open.
