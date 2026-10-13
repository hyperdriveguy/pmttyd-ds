# Original Goomba4 quest guards and item selection

US G8ME01 revision0; recovered2026-10-13 from read-only ISO DOL and
rel/gor.rel. Scoped follow-up to [wander/talk owner](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
and [OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Public symbols annotate
identities; original bytes and relocations establish behavior. Static analysis
is not original execution, native runtime parity or hardware measurement.

## Saved state and safe ordinary admission

Using [event namespaces](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[switch leaves](../event/EVENT_SAVED_SWITCH_STORAGE.md):

| Raw operand | Original service | Storage |
| --- | --- | --- |
| f5de070c | GSW1420 / swByteGet(1420) | U8 at GP+b04 |
| f840709d | GSF5405 / swGet(5405) | GP+418, bit29 |
| f840709e | GSF5406 / swGet(5406) | GP+418, bit30 |
| f840709f | GSF5407 / swGet(5407) | GP+418, bit31 |
| f5de0180 | GSW0 / swByteGet(0) | full WORD at GP+174 |

swInit80085980/78 zeroes global flags GP+178 length400, global bytes GP+578
length800, both local banks, then story word0. All four listed quest values
therefore become0 after that explicit owner. swReInit8008593c/44 clears only
local banks and preserves these globals. Read [initialization/import](../overworld/OVERWORLD_STORY_STATE_AND_ROAD_CONTROL.md)
for direct reset callers and GP's13d8-byte card-slot ownership. The quest byte
is source slot+b0c and flag word slot+420; this identifies storage, not a safe
card import API. A native legacy harness lacking these values remains unknown;
low story does not imply quest0. A fresh reproduced reset can explicitly own0,
with subsequent original scene writes requiring their own owners.

Source guards short-circuit in order: quest==2, flag5405==1, count95>=1;
then quest==13, flag5406==1, count107>0. Ordinary story playback is sound when
both guards are proven false. A known quest other than2/13 needs no item/flag
reads; quest2 can use known flag5405!=1, or after that flag is known1 a known
count95<1; quest13 similarly flag5406!=1 or known count107<=0. Unknown required
state cannot silently select the ordinary branch. An adapter may decline
unimplemented quest playback without falling through to a farther NPC.

## Inventory event services

evt_pouch_check_item800f0bb4/54 resolves argument0 through evtGetValue,
calls pouchCheckItem800d4ed4, writes the count to argument1 through evtSetValue,
and returns event2. firstCall is unused. Items95/107 are key items1..120;
the count scans all121 signed halfwords at pouch+a0, counting equality, including
copies after zero gaps. No carried-item10/20 capacity interpretation applies.

[Removal](../unit/INVENTORY_REMOVAL_AND_RESERVATIONS.md) documents
pouchRemoveItem800d4c38. Wrapper evt_pouch_remove_item800f0da8/78 resolves
argument0 and removes one matching item. If low-level result==1, call
pouchReviseMarioParam800d35a8 then pouchRevisePartyParam800d3430 and write
**0** to argument1. Otherwise write **-1**. Return event2. Do not substitute
low-level boolean1 as the wrapper result. The quest event does not branch on
that removal result before its flag write. Full revise owners are separate
services, even for these key-item calls.

## Item selection is distinct from message choices

Both arms invoke evt_win_item_select8020a374/c8, not evt_msg_select.
Arguments are (mode,table,resultDestination,indexDestination). Every invocation
resolves mode and table through evtGetValue. On firstCall, call
winMgrSelectEntry8023d624(table,0,1), retain its pointer at event+78, then
marioKeyOff8005c1f4. Poll winMgrSelect8023d4c0(pointer). Return event0 while
result==0. Once nonzero, read selected row index through8023cf04(pointer),
delete through winMgrSelectDelete8023cf0c, call marioKeyOn8005c1c0, write item
result to argument2; write index to argument3 only when resolved mode!=0.
Return event2. Original item-window creation/drawing/input services remain
separate; a message Yes/No window is not equivalent.

winMgrSelect returns0 until flags1000 set, -1 when flags2000 set. It also
returns-1 when pointer+38 nonzero, row index+0c==0 and flags0100 set. Otherwise
return zero-extended U16 at rows(pointer+30)+4*selectedIndex+2. Selected row
index is a WORD; cancellation is -1 rather than the message selector's signed
byte result. Source entry uses item table terminated by signed32 -1.

## Quest2 event and fixed table

Original gor02 item_tbl file43970/8 contains signed32 [95,-1]. It has no
message selection key. Talk43978/450 checks the guard, prints
`evt_msg_print(0,irai_02_03,0,me)`, then
`evt_win_item_select(0,item_tbl,LW0,0)`. If LW0!=-1, print
`evt_msg_print(0,irai_02_04,0,me)`, call
`evt_pouch_remove_item(95,LW0)`, then set GSF5406=1. Return regardless of
cancel. The acceptance test is **not** selected item==95, although the authored
table contains only95. No story word write or quest byte write occurs here.

## Quest13 dynamic table and exact loop ownership

REL callbacks are gor02 text file5680/4c free,56cc/b0 update,577c/50 make;
retained pointer iri_13_item_tbl is REL section6 BSS+7c (WORD). Relocations
resolve calls to pouchCheckItem,evtSetValue,_mapAlloc8002ff58,_mapFree8002fdc4
and mapalloc_base_ptr8041e808; unrelocated bl targets are not valid identities.

Make checks count107. If positive, allocate (count+1)*4 bytes from current
mapalloc_base_ptr and retain pointer; if nonpositive, no allocation or pointer
write. Return event2. Update rechecks count107. If positive, fill entries
0..count-1 with signed32 107 and entry count with -1. If nonpositive, **do not
rewrite a -1 at entry0**; previous bytes remain. Always write retained pointer
to argument0 and count to argument1 via evtSetValue, then return2. Free calls
_mapFree(current mapalloc_base_ptr,retained pointer), clears pointer0, returns2.
It has no null check. Original single-owner lifecycle prevents unrelated table
sharing; an implementation must not resize or replace this table arbitrarily.

After guard, make table. If GSF5407==0:

1. Print(0,irai_13_001,0,0).
2. Mario pose M_N_5B; wait500ms; pose M_N_C; wait500ms; pose M_S_1.
3. Print(0,irai_13_002,0,0); update(LW1,LW0).
4. Infinite loop: select(0,LW1,LW0,0). Break when LW0!=107. Otherwise
   remove(107,LW0); update(LW1,LW0); break if count LW0==0; otherwise
   print(0,irai_13_003,0,0) and repeat.
5. Set GSF5407=1 even if canceled/nonmatching selection.

Otherwise infinite continuing loop: update(LW1,LW0); break if count0; print
(0,irai_13_005,0,0); select(0,LW1,LW0,0); break if selected!=107; remove
(107,LW0); repeat. After either loop, check count107 into LW0; when count0
print(0,irai_13_004,0,0). Free table and return. Each first/continuing message
uses numeric speaker0, not me; retain this difference and all original tokens.
Neither loop writes story or quest byte. The source count may be re-read after
removal; do not pretend a cached quantity is the original service.

Only if quest guards do not return does the documented story switch select
ordinary gor_02_030_00..08 with speaker me; thresholds/message IDs are in the
[wander/talk owner](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md). Keep those source
branches rather than deriving a new branch from inventory or native progress.

## Evidence and remaining binding

Fresh read-only DOL extraction through inspect_dol_function.py recovered complete
swInit/swReInit,pouchCheckItem,evt_pouch_check_item,evt_pouch_remove_item,
evt_win_item_select,winMgrSelect and selected-index getter. Read-only REL
extraction and full relocation-stream decoding recovered the three C callbacks
and fixed table; rel_event_inventory.py supplied talk graph/import bindings.
Same gor.rel642564-byte SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a
as prior contract. Local ELF/disassembly and event dumps remain /tmp, outside
source history. No original dialogue prose/binary assets were added.

Remaining: native persisted global switch ownership/migration, source quest
writers, complete item-window entry/display/input/cleanup, parameter revision,
pose/wait event scheduling, interpreter control/key restoration and matched
Dolphin/New2DS observations. Best-effort ordinary admission should expose
explicit known/unknown source values and preserve unknown legacy saves. This
document does not claim any new quest event runs natively. No code/build/emulator,
user profile/save, shared index or PROGRESS edit was made by this worker.
