# Original item-table selection window

US G8ME01 revision0; read-only ISO recovery2026-10-13. Scoped source answer to
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) for the
[Goomba4 quest calls](../field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md).
This contract covers negative-address explicit item tables with entry arguments
(table,0,1). Built-in numbered selectors0..18 have distinct row producers and
are not interchangeable with this path. Static recovery does not establish
matched original execution, native performance or complete UI equivalence.

## Entry, rows and ownership

winMgrSelectEntry8023d624/f18 allocates and clears a3c-byte context through
__memAlloc(0,3c). Context fields:

| Offset | Source field |
| --- | --- |
| 00 | U16 flags; third argument nonzero ORs0100 |
| 04 | Original signed32 first argument/table address |
| 08 | Main state0..4, initially0 |
| 0c | Selected index WORD, initially0 |
| 10 | First visible row WORD, initially0 |
| 14/18 | Cursor X/Y binary32 |
| 1c | Smooth list displacement binary32, initially0 |
| 20 | Number of manager windows WORD |
| 24/28/2c | Manager indices for list/header/description |
| 30/34 | Row pointer / row count WORD |
| 38 | Second argument, initially0 for both quest calls |

A negative first argument uses template0. Template dispatch table803b6174
has8-byte records (U16 window count, padding, definition pointer). Template0
is count3, definitions803b5ef4, each28hex bytes. Manager records are44hex
bytes. Entry finds the first manager record whose flags bit1 is clear for each
definition, sets flags1 and clears lifecycle/transform/help state, copies the
source rectangle, and binds the same context at manager+2c. It is a retained
shared context across three windows, not three independently owned selectors.

For an explicit table, count the signed32 source entries through and including
first -1. Allocate and clear (countIncludingTerminator+1)*4 bytes. Set rowCount0.
If second argument nonzero, prepend a row with its low16 as itemID. Then append
all entries before -1 in original order, storing their low16 at row+2, increment
rowCount once each. Row+0 remains zero flags. Duplicates are preserved; there is
no sorting, quantity collapsing, inferred carried-item restriction or ownership
check here. In the quest calls second argument0 adds no cancel row. With n
items allocation capacity is n+2 rows while published count is n. A source table
with only -1 produces zero rows; this code supplies no safe empty-list UI policy.
Authored quest guards/table owners ensure positive counts before entry.

The first count pass at8023e41c is a do/read loop: read WORD(table+offset),
increment count and offset by1/4 before testing that read for -1. Allocation
bytes use original32-bit (countIncludingTerminator+1)<<2. Call __memAlloc with
heap0, store pointer context+30, reload pointer context+30, then memset(pointer,
0,allocationBytes). Only afterward store context rowCount0. Allocation and clear
are distinct source calls; a typed allocateCleared adapter must preserve the
observable source-memory reads/writes around this ownership boundary.

The optional prepended row reads old rowCount then rows pointer, stores
rowCount=old+1 **before** storing low16(second argument) at rows+4*oldCount+2.
The fill pass restarts at table and tests the terminator again, not the cached
first-pass count. Its precondition reads WORD(cursor); if not -1, body reads old
rowCount, advances cursor4, rereads WORD(table+offset) as the stored item,
advances offset4, computes next count, reads rows pointer, stores next rowCount,
then stores item's low16 at rows+4*oldCount+2. The next condition reads the
advanced cursor. Each nonterminal item is therefore read twice in fill (test
then value); the terminator is read once. Source table mutation between the
allocation/count and fill phases can alter the published rows; immutable native
vectors are an adapter policy, not a second-pass fixed-count source contract.
No row flags are explicitly written after the initial memset.

Template0 definitions, in source centered screen coordinates with Y up:

| Window | Sequence type / decoration type / camera | X,Y,width,height | Callbacks |
| --- | --- | --- | --- |
| List | 0 / 1 / 8 | -24,116,300,240 | select_main8023ca6c; select_disp8023bc74 |
| Header | 2 / 0 / 8 | -280,120,240,45 | no main; select_disp2 at80239ae8 |
| Description | 0 / 0 / 8 | -240,-130,500,80 | select_main3 at80239a00; select_disp3 at80239980 |

All three definitions have RGBAffffffff. Initial cursor X=listX, Y=listY-54;
row displacement0. If rowCount<8, list height becomes240-24*(8-rowCount).
Other windows retain their original rectangles. Eight visible rows is the
source layout; reflow for the3DS must preserve source order, row identity,
selection result and control semantics.

## Main lifecycle and manager ordering

winMgrMain8023f1b0/d8 visits manager slots in ascending order. For each active
flags1 slot it calls winMgrSeq, then that definition's nonnull main callback,
then registers display when flags2 is set. Display uses definition camera8,
priority300+manager+30, winMgrDisp callback and the manager record. Do not run
select_main once for each of the three windows; only the list owns it.

select_main8023ca6c/498 state0 sets every owned manager action1 (open), clock0,
help page0/text0/lineCount0. It also clears those description fields explicitly
when there are at least3 windows. Requests psndSFXOn(2002a),
statusWinForceOpen8013c354, increments context state to1 and returns.

State1 returns immediately while list manager flags4 is set (transition).
Otherwise it navigates, confirms/cancels, then updates smooth cursor/displacement.
State2 assigns close action2 and clock0 to each active owned manager, requests
psndSFXOn(2002b) and statusWinForceOff8013c234, increments state3 and returns.
State3 waits while list flags4 remains set. Then clears flags1 in all owned
manager records, ORs context1000 (ready), increments state4. State4 is inert.
Thus a confirm is not a ready result on its input invocation: closure and
manager transition finish first. Sound values are original named-service IDs;
they do not establish complete MusyX native playback.

winMgrSeq8023ecb8/4f8 starts each invocation by ORing flags4. For template
sequence0, idle action0 clears **only flags4**, preserving visible2. The
source wrapping rlwinm mask isfffffffb (MB30,ME28), not a contiguous low-bit
clear. Open action1 ORs visible2, computes
rotation30->0 and scale0->1 through intplGetValue(type12, clock, N), sets alpha255,
increments clock, then sets action0 when incremented clock>N; visible2 stays
set, and transition4 stays until the next idle invocation. Close action2 uses
rotation0->30/scale1->0 and similarly ends, clearing **only visible2** via
maskfffffffd (MB31,ME29). If flags8 is present, completion also clears active1.
It leaves transition4 until the next idle invocation. Here
N=original signed word arithmetic trunc(GP.word[4]*8/60); the GP word's owner
remains a separate dependency; mode12 uses the existing
[finite interpolation contract](../camera/OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md#intplgetvalue-built-in-finite-contract).
Its source libsin/single rounding remain numeric adaptations. Sequence2
header open/close is immediate with rotation0,scale1,alpha255 and action0;
open ORs visible2, close clears visible2 and optionally clears active1 when
flags8 is present. Both leave transition4, which clears on the following idle
invocation. A native8-tick transition is an
adapter unless the GP value/cadence/interpolation is explicitly bound.

Manager packed sequence fields are flagsWORD+0, actionWORD+4,
clockWORD+8, alphaWORD+c, scaleFLOAT+10, rotationFLOAT+14. Definition pointer
is managerWORD+28 and sequence kind is definitionWORD+0. Every sequence
invocation ORs4 before dispatching. Idle preserves active1, visible2, all
transforms/alpha/clock and any unrelated flags; it changes only4. These masks
are revalidated from instructions8023ed30,8023eef0,8023f120,8023f16c. Source
open completion sets action0 without clearing4 or2; the next callback's idle
branch admits list input in that same manager-main iteration. Close completion
clears2 but leaves4; its next idle invocation admits context state3 cleanup.

### Sequence0 timing reads and interpolation effect boundaries

The three count calculations are **not cached** across interpolation calls.
For open: OR visible2; load GP pointer, load window clock+8, load GP.word+4,
compute N1, call interpolation(mode12,clock,N1,30,0), store rotation+14.
Reload GP pointer and GP.word+4, reload clock+8, compute N2, call
interpolation(mode12,clock,N2,0,1), store scale+10. Store alpha255; reload
clock+8, increment and store it. Reload GP pointer, reload incremented clock+8,
reload GP.word+4, compute N3, then compare clock>N3 to complete action.
Close differs in first-phase operand read order: load GP pointer and GP.word+4
before loading clock; endpoints are rotation0->30,scale1->0. The later phase,
alpha/increment/comparison ownership is the same as open. All count arithmetic
uses the original32-bit shifted refresh word with signed truncating divide60.
Type0 idle and type2 immediate paths do not read GP timing at all.

Source finite mode12 itself is a pure numeric service in its bound finite
scope, but the packed owner reloads these fields rather than assuming purity
across the service boundary. A typed helper supplied an immutable refresh value
captures a deliberate timing snapshot adapter. An effect-capable test/service
must permit GP/clock mutations to be observed by the subsequent reloads; do not
silently reuse the first count or first clock at the compare boundary.

## Navigation and A/B order

All key reads use index0 and existing source snapshot services, as in
[message input readers](MESSAGE_SELECTION.md#snapshot-readers-and-navigation-masks-b).
This selector does not read button-direction masks4/8 locally.

1. Save old selected index. If keyGetDirRep mask2000, increment index.
2. Independently, if keyGetDirRep mask1000, decrement index.
3. If index>=rowCount, check keyGetDirTrg2000. New trigger wraps index0 and
   firstVisible0; otherwise decrement once (repeat endpoint hold).
4. If index<0, check keyGetDirTrg1000. New trigger writes rowCount-1 and,
   when rowCount-8>0, firstVisible=rowCount-8; otherwise increment once.
5. If oldIndex<newIndex and newIndex>5, increment firstVisible when it is
   below rowCount-8. Otherwise, if oldIndex>newIndex and
   newIndex-firstVisible<3, decrement firstVisible when positive.
6. If final selected index differs from old, psndSFXOn(20005).

These are independent down/up operations with boundary normalization afterward.
New presses wrap, held repeats do not. Preserve this distinction in an adapted
pad producer instead of treating all repeated navigation as fresh triggers.

Then **A precedes B**. keyGetButtonTrg100 tests current row flags2. If set,
request20014 error and retain state1; B is not tested on that invocation.
Otherwise request20012 and increment state2. Only if A absent, test trigger200
and contextflags0100. Allowed B requests20013, ORs context2000, increments
state2. B without0100 does nothing. This differs from message-selection cancel
priority. In the bound explicit tables all row flags are initially0 and third
argument1 admits B cancellation.

On every admitted state1 invocation, even one initiating closure, binary32
smooth displacement += (24*firstVisible-displacement)/6; cursor X +=
(listX-cursorX)/6; cursor Y +=
(listY-54-24*(selected-firstVisible)-cursorY)/6. The source fmsubs/fnmsubs
forming targets are fused; no elapsed-time scaling or snap threshold exists.
Transition-skipped callbacks do not ease these fields.

winMgrSelect8023d4c0/64 returns0 until ready1000. Once ready, cancel2000 returns
-1. Also returns-1 for nonzero context+38, selectedIndex0, flags0100. Otherwise
return zero-extended row.itemID. Getter8023cf04/8 returns selected index WORD.
winMgrSelectDelete8023cf0c/44 frees rows then context through __memFree(0,p);
it neither closes manager records nor restores Mario keys. That work is owned
by the lifecycle and evt_win_item_select wrapper: delete only after ready.

### Exact easing operations and active-slot gates

Source8023cd44..ce00 uses pitch24 bits41c00000 at8042804c and divisor6
bits40c00000 at80428050. Signed WORD-to-float conversion uses the source
4330000080000000 double bias at80300800 followed by fsubs, producing binary32.
The operation sequence, with each listed single instruction rounding separately:

1. Convert firstVisible to float; read old displacement. Compute
   delta=fmsubs(24,firstVisibleFloat,oldDisplacement), quotient=fdivs(delta,6),
   newDisplacement=fadds(oldDisplacement,quotient), then store context+1c.
2. Convert signed listX to float; read old cursorX. Compute
   delta=fsubs(targetX,oldX), quotient=fdivs(delta,6),
   newX=fadds(oldX,quotient), then store context+14.
3. Compute baseY=listY-54 and relative=selected-firstVisible in source32-bit
   WORD arithmetic. Convert each independently to float; read old cursorY.
   Compute targetY=fnmsubs(24,relativeFloat,baseYFloat),
   delta=fsubs(targetY,oldY), quotient=fdivs(delta,6),
   newY=fadds(oldY,quotient), then store context+18.

fnmsubs negates the fused (24*relative-baseY) result; equivalent algebra with
a positive-zero cancellation can change its signed-zero bit. Use the original
fused helper/order for bit-level obligations. Final old+quotient is separate
fadds, not fused. Dividing by6 is not multiplication by an approximated1/6.
There is no coefficient load, reciprocal, clamp or render-count dependency here.

State0's per-window action1/clock0/help38,3c,34 reset is gated by managerflags1.
The additional explicit description38,3c,34 reset when windowCount>=3 is
**not** gated by that description record's active flag. State2's per-window
action2/clock0 writes are gated by managerflags1. State3 cleanup clears flags1
in all owned slots without an active gate. Sound/status/state increments occur
outside these per-slot gates, even if no slots were active.

## Packed result and event operand read order

The result poll reads context.flagsU16 once. If1000 absent, return0 before
reading other fields. If2000 present, return-1 before reading other fields.
Otherwise read WORD+38. Only if nonzero, read WORD+0c and test zero; only
if zero, test cached flags0100 and return-1 when set. The success path then
reads WORD+0c again, reads WORD+30, computes original32-bit index<<2 addition,
and reads U16 row+2. Thus the index can be read twice when +38 is nonzero;
there is no rowCount/rowflags/bounds/null check or result rewriting in this
leaf. Portable safe bounds policies must remain separate from this source
packed reader; malformed-pointer behavior is not a native gameplay feature.

Fresh evt_win_item_select8020a374/c8 reads E+18 (argument-stream pointer) once,
then reads raw arg0 and resolves mode through evtGetValue, then reads raw arg1
and resolves table. This happens on every poll invocation, even when firstCall
is false. On firstCall, create(table,0,1), store returned pointer at E+78,
then marioKeyOff. Reload E+78 and poll result. If0 return event0 immediately.
On nonzero result, get selected index **before** delete; delete context;
marioKeyOn; only now read raw arg2 and evtSetValue(result). Then test the
cached resolved mode; only if nonzero read raw arg3 and evtSetValue(index).
Return event2. The source never clears E+78 after deletion and does not capture
result destinations before callbacks. If event services mutate the argument
stream during entry/deletion/key restoration, these later reads see those
mutations; the stream pointer itself is the original cached pointer.

## Original drawing and asset binding

winMgrDisp8023f288/484 draws manager background first through
windowDispGX2_Waku_col8007ad68 using retained rotation/scale, definition RGB,
low8 manager alpha, source rectangle and border20. Then nonnull definition
decoration adds its own background; list decoration1 uses additional source
border geometry, while header/description decoration0 adds none. Finally call
definition draw callback. Border/System resources and complete GX translation
remain separate owners; a message Yes/No border alone does not reproduce this UI.

select_disp skips contents while list flags4 is set. It disables fog, saves
scissor and clips rows at source (x+304,274-y,width,height-50). For each row i,
row icon anchor is (x+35,y-44-24*i+displacement); name anchor
(x+60,y-32-24*i+displacement), through source window font with width185.
Item records are itemDataTable803108a8 +40*U16(itemID); name key pointer+4,
description key pointer+8, iconU16+20hex. Row flag1 requests gray icon/text;
flag8 requests an extra icon154 before item icon. Explicit quest rows have0,
so neither applies. Item icon scale vector is(0.5,0.5,0.5), producing20px
art from the40px source textures. Name lookup uses msgSearch, not the debug
Japanese item-name pointer+0. Numbered selector price/count columns are separate
branches and do not apply to negative-address table selectors.

After restoring scissor, negative-selector list heading uses message key
msg_window_title_1 at803008c4, centered above list with the original font service.
Header callback defaults to msg_window_select_1 at803006f8. Description callback
resolves the selected item record+8 through msgSearch, assigns manager+34 and
calls winMgrHelpDraw8023e53c. Help clips source608x56 area at screenY384 and draws
text at centeredX-210,Y=-144+28*helpPage, then restores608x480 scissor. Its main
callback reads prior description text, computes lineCount+1, and when>2 uses
keyGetDirTrg100/200 (horizontal directions) to move description page by2 within
original bounds. It does not use A/B button triggers; the same numeric masks
belong to a different key namespace. Full help arrow geometry/clock and text
service behavior remain dependencies.

Cursor icon504 draws at retained smooth cursor with scale1. Its source static
request uses camera20. When rowCount>8, arrow blink admits GP.word[1c]&31<20,
with icon445 for rows above and446 for rows below, source scale0.6. List stripe
icon447 and full header/border primitives are original additional draws;
complete matched pixels still require the callback/backend parity audit.

| ItemID | Name message key | Description message key | IconID | TPL image |
| --- | --- | --- | --- | --- |
| 95 | in_irai_butsu | msg_irai_butsu | 307 | 312 |
| 107 | in_fighters_pants | msg_fighters_pants | 308 | 313 |

iconInit8001b698 resolves `%s/icon.tpl` and `%s/icon.bin` using original DVD root,
then async loads their separate retained buffers. iconGetTexObj8001a29c indexes
icon.bin via U16 offset at2*(iconID+1); descriptor's frame count+6 and per-frame
(image,delay) pairs at+8 determine original TPL lookup. Icons307/308 are static
one-frame descriptors at12f2/12fe, loop0, images312/313 and delay0. Both TPLs
are40x40 GX14(CMPR). Cursor504 loop1 uses ten40x40 frames533..542 with delays
2 for first9 and28 forlast; already covered in native message-icon assets.
Arrow445/446 and stripe447 map465/466/467. Never treat iconID as TPL image index.

## Evidence and remaining work

Fresh tools/inspect_dol_function.py recovery uses local original public symbol
CSV for exact spans, direct ISO bytes and DOL address mapping for data. Functions
inspected: entry, main, row/list/header/description callbacks, help main/draw,
manager main/sequence/display, deletion and icon init/get/set. Data inspected:
all3 template definitions, item95/107 records, startup r2=80427440 constants,
read-only icon.bin descriptors and icon.tpl image headers. Inspection-only
ELFs/raw assets/logs remain `/tmp/item-win-*` and `/tmp/item-window-*`, ignored
from Git. icon.bin8174bytes SHA256
0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b;
icon.tpl677792bytes SHA256
fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9.
Nintendo/Intelligent Systems original assets have no redistribution grant;
extract/package locally only, outside source history. No dialogue prose copied.

Remaining: full numbered selector producers, GP timing/input repeat producers,
numeric interpolation parity and complete window/font/System/GX draw services, global key
ownership/call cadence, original UI sound macros, matched Dolphin captures and
New2DS measurements. Source-bound best effort can retain rows, original US
keys/icons, eight-row reflow, exact navigation/A/B/result/cleanup ownership and
label any unbound transition/backend behavior in implementation documentation.
No source code/shared PROGRESS/index/build/emulator/profile/save changed here.
