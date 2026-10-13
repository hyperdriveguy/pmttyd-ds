# Save-block common save child, original choices and card request graph

US G8ME01 disc0 revision0;2026-10-13. Static read-only DOL event and US
message-resource evidence. Complements [spawn](ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md),
[head/hit lifecycle](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md),
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[SAVE-01](../../requests/SAVE-01.md). This graph establishes local original UI
requests; it does not close original card persistence or reproduce every
memory-card error on a native SD filesystem.

## Exact operand namespaces and first-hit ordering

Decode signed operands using the complete
[integer namespace contract](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md),
not their hex appearance or a generic map-local storage name:

| Raw operand | Source name/index | Ownership |
| --- | --- | --- |
| f8405c69 | GSF233 | Saved global flag; swGet/swSet |
| f8405c6c | GSF236 | Saved global flag; swGet/swSet |
| f5de0180 | GSW0 | Story word at GP+0x174 |
| f5de0184 | GSW4 | Saved byte at GP+0x57c |
| fbd3e28a | LF10 | Current event's local flag |
| fbd3e28b | LF11 | Current event's local flag |
| fe363c80 | LW0 | Current event's local work/result word |
| fe363c8a | LW10 | Current event's local work word |

Earlier save-block prose called fbd3e28a map-local; its actual decoded band is
**LF**, not LSW/GW or map storage. Child construction copies16 LW words and
three LF words by value, then deletion copies locals/flags back to the parent;
see [child ownership](../event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md).
These transient LF flags are distinct from persistent GSF233/236.

System event8030b4ec has16 commands, SHA256
`a580e9894d07ae58b8cb24d35b825df91eb9b3d42e003f84cd4e5a15dcaa3752`:
set priority0; Mario keysOFF; LF10=0; if GSF233==0, setGSF233=1 BEFORE
printing `save_first_hit`, then LF10=1; if GSW0>=7,setGSF236=1;
LW0=0; RUN_CHILD803bac3c; Mario keysON; RETURN; END.
Declining the later save choice does NOT undo GSF233 or236. There is no story
write. Full original on-disk flag persistence remains the card owner.
Nested Mario key ownership and the object's independent lifecycle lock are
specified in the head/hit contract.

## Common wrapper and prompt path

`unk_evt_803bac3c` is52 bytes/eight commands, SHA256
`b3467d3aa54cf4ec1ff2b9f90de94862d99c4325e5d8805775b4448a95b39b1e`:
IF LW0==0 RUN_CHILD(evt_memcard_save); ELSE RUN_CHILD(same),WAIT_MSEC1000;
END_IF;RETURN;END. For the save block's explicit initialLW0=0 there is no
1000ms post-child wait. Other callers entering with nonzeroLW0 take that
wait regardless of the child's later result/copy-back.

`evt_memcard_save`803ba444 is0x7f8 bytes/197 commands, SHA256
`fd79ccecf3d3d6f041a8e7ab14f6d78776eeb0a642a83a6ad72d877ca6123dac`.
Preserve request order and numeric mode gates:

1. LW10=LW0 captures entry mode. If mode23,GOTO label1 (skip initial prompt).
2. Label0: if LF10==1, `evt_msg_print_add(1,"<scrl_auto><once_stop>")`,
   then `evt_msg_print_add(0,"msg_mcard_save_select")`, then LF10=0.
   Otherwise `evt_msg_print(0,"msg_mcard_save_select",0,0)`.
3. Mode21 selects `msg_mcard_yesno_ending`; every other mode selects
   `msg_mcard_yesno`. `evt_msg_select(0,ID)` writes selected signed result
   intoLW0 after the original selection window closes.
4. If LW0!=0, call `evt_msg_continue`; RETURN immediately. This ordinary
   refusal performs no header/file write and shows no extra refusal warning.
5. Label1: GSW4=LW10. If mode23,print
   `msg_savefile_first_warnning` with flags0/speaker0/coordinates0;
   otherwise append `msg_mcard_save_warnning`. GOTO label3.
6. Label2: append raw `<scrl_auto><once_stop>`, append
   `msg_savefile_first_warnning`; fall through tolabel3.
7. Label3: USER_FUNC(memcard_header_write), then USER_FUNC(memcard_write).
   These are yielding requests, not adjacent unconditional synchronous writes.
8. Label4: LF11=0; USER_FUNC(memcard_code,LW0); SWITCH LW0 as below.

For the save-block's normalmode0, accepting the prompt follows label1 with
GSW4=0, warning, header request, slot write request, completion-code read.
Success0 appends `msg_mcard_save_finish` and RETURN. The source success path
does not insert an extra `evt_msg_continue`; preserve the message owner/calls.

## Exact original choices and cancellation

All source IDs below resolve to unchanged `msg/US/global.txt` payloads.
US choice rows in source order:

| ID | Original selection token | Rows |
| --- | --- | --- |
| msg_mcard_yesno | `<select 0 1 0 40>` | Yes; No |
| msg_mcard_yesno2 | `<select 1 1 0 40>` | Yes; No |
| msg_mcard_yesno3 | `<select 0 -1 0 40>` | Save again.; Continue without saving. |
| msg_mcard_yesno_ending | `<select 0 -1 0 40>` | Yes; No |

Under [selection input](../message/MESSAGE_SELECTION.md), first argument is
initial row; cancellation config1 resolves to result1, while-1 ignores cancel.
Thus B on the normal save prompt takes the same no-write return as row1 No;
`yesno2` initially highlights No; ending/retry choices disable cancellation.
Do not infer disable-cancel from the ID name alone. Source navigation/result
closure and confirm/cancel priority remain that service's owner.
The four original payloads are24,24,55 and25 bytes respectively, including
newlines. Preserve exact IDs/raw resource tokens; adapted 3DS button prompts
may alter presentation only.

## Complete completion-code branches

The following table follows original switch cases. `append(ID)` means
`evt_msg_print_add(0,ID)`. `raw` means append with flags1 the exact
`<scrl_auto><once_stop>` controls. Joined cases use CASE_OR43; CASE_END46 and
SWITCH_BREAK48 keep recovery branches from falling into later error cases.
Signed code meanings below are described by the actual message requests,
without inventing native filesystem-to-GameCube error equivalence.

| Code | Original ordered branch |
| --- | --- |
| 0 | append(msg_mcard_save_finish); RETURN |
| -3 | raw; append(msg_mcard_nocard); append(msg_mcard_save_miss) |
| -2 | raw; append(msg_mcard_different_card); append(msg_mcard_save_miss) |
| -13 OR -6 | recovery-arm helper8025c40c; raw; append(msg_mcard_format); two-step format/create/save sequence below |
| -4 | recovery-arm helper8025c40c; append(msg_mcard_make_data); create/save sequence below |
| -10 | append(msg_mcard_different_savedata); append(msg_mcard_save_miss) |
| -8 OR -9 | raw; append(msg_mcard_nospace); mode23 system-menu option below |
| default | append(msg_mcard_damage); append(msg_mcard_save_miss) |

Formatting branch(-13/-6), in exact nesting/order:

1. Select `msg_mcard_yesno2`. Result!=0 appends `msg_mcard_save_miss` and
   leaves the branch. Result0 appends `msg_mcard_format_alldelete` and selects
   `msg_mcard_yesno2` AGAIN. Result!=0 appends save_miss and leaves.
2. With both accepted, call guard8025c3a4(LW0). If LW0==1,append
   `msg_out_mcard_warnning` and SWITCH_BREAK directly to shared aftermath.
   Otherwise append `msg_mcard_format_warning`; request memcard_format;
   memcard_code(LW0).
3. Nonzero code appends `msg_mcard_format_miss`, setsLF11=1 and leaves.
   Code0 appends `msg_mcard_format_finish`, then `msg_mcard_make_data`,
   then selects `msg_mcard_yesno`.
4. Declining data creation appends save_miss and leaves. Accepting appends
   `msg_mcard_make_data_warning`; runs the same guard/out-warning/break gate;
   requests memcard_create; memcard_code(LW0).
5. Nonzero create code appends `msg_mcard_make_data_miss`, setsLF11=1.
   Code0 appends `msg_mcard_make_data_finish`, appends
   `msg_mcard_save_warnning`, requests memcard_write, reads memcard_code.
   Code0 appends save_finish and RETURN; nonzero setsLF11=1.

No-file branch(-4) starts directly at the creation sequence:
append(make_data), selectyesno; refusal appendssave_miss. Acceptance appends
make_data_warning; guard/out-warning/break gate; memcard_create; code read;
create-failure appendsmake_data_miss,LF11=1; create-success appends
make_data_finish then save_warnning, memcard_write, code read;
write-success appendssave_finish and RETURN; write-failure setsLF11=1.
It does not call memcard_header_write again inside this branch.

Space branch(-8/-9): after raw,nospace, only mode23 appends
`msg_gc_mcard_open` and selects `msg_mcard_yesno2`. Refusal appends save_miss.
Acceptance appends `msg_gc_mcard_nosave`, selectsyesno2again; refusal appends
save_miss, acceptance callsmemcard_ipl. Other modes append save_miss directly.
This original GameCube IPL request is not a native SD-card service.

## Shared failure aftermath, retry and return

After END_SWITCH, call helper8025c380 unconditionally on surviving failure
branches. If LF11==1,GOTO label4 and repoll memcard_code (which resetsLF11=0).
This is a source status retry, not a new user prompt or a synthetic success.

Otherwise, if mode0,call `evt_msg_continue`; RETURN. This makes normal
save-block card failures close through their original error messages and
return to the object owner; the mode0 path does not show the general retry UI.

For every other mode: `evt_msg_continue`; print
`msg_savefile_nosave_warnning` with flags0/speaker0/coordinates0; select
`msg_mcard_yesno_ending` if mode21,else `msg_mcard_yesno3`.
Result0 retries: mode23 GOTO label2 (raw/first-warning/header/write), other
modes GOTO label1 (mode assignment/warning/header/write). Result!=0 calls
`evt_msg_continue`;RETURN;END. Spelling in actual labels is numeric1/2;
there is no automatic back-to-prompt label0 on retry.

## Blocking card leaves and native backend boundary

Fresh local leaf inspections:

| Leaf | Local behavior |
| --- | --- |
| memcard_header_write8025c19c/0x40 | firstCall!=0 calls cardWriteHeader800b1174; every call polls cardIsExec800b2b8c; busy→handler0, idle→2 |
| memcard_write8025c0b8/0x48 | firstCall!=0 calls cardWrite800b1e7c with slotwordGP+0x11d0; polls cardIsExec; busy0,idle2 |
| memcard_code8025c454/0x6c | capture cardGetCode800b2b80 BEFORE cardIsExec; busy→0 without publishing; idle→evtSetValue(rawdest,capturedCode),return2 |
| guard8025c3a4/0x68 | calls800b2bdc; writes destination1 when nonzero,else0; returns2 |
| cleanup8025c380/0x24 | calls800b2c08; returns2 |
| recovery-arm8025c40c/0x24 | calls800b2c2c; returns2 |

Backend guard800b2bdc returns1 only when card-state halfwordflags contain
BOTH0x8000 and0x4000. Cleanup800b2c08 clears those two bits via two separate
loads/stores. Recovery-arm800b2c2c copies card-state word+8 into+0xe4,
writes+0xec=10, and ORs0x8000 into its flags. Full cardMain producers and
operation transitions remain open; do not rename these guards as a verified
physical-card condition without their source owners.

A native PM2D save backend can be an explicit platform adapter after the
original Yes choice, under the same object/scene lock, reporting actual success
only when its atomic write completes. Its failed operation must not publish
source success0. It must preserve GSF233/236,GSW4 and unchanged original
prompt/warning/success message order, and restore controls on cancellation or
failure. Original GameCube card format/create/IPL/code domains, slot records,
checksums and persistence remain unfinished; do not claim PM2D is cardMain.
No native error-code mapping is established by this document.

## Field dispatch and head-publication order

Fresh complete US `marioStMain` is8006f904/0x238. It waits whileGP+0x10!=0
via retrace call,
then requests cardMain800b2508 at8006f930 and evtmgrMain800368d0 at8006f954
BEFORE marioMain8005a7e0 at8006f95c, then mobjMain8005f968 at8006f960.
These listed calls are unconditional along the admitted local dispatch path.

Mario's current head publication therefore precedes the map-object new-contact
producer in that same iteration. The new object event starts during mobjMain,
after that iteration's event-manager invocation; do not run its original script
before the current Mario collision. Existing hit transforms are updated during
mobjMain, after Mario's query; moving-object collision has a distinct previous-
publication lifetime, while this stationary save box is stable. Full outer
retrace/pause and event entry/run-check rules remain separate owners; this call
order alone does not prove every new event waits exactly one rendered frame.

## Validation and limitations

Fresh `dol_event_inventory.py` extraction pins the three streams/221commands
and hashes above. All27 keyed messages plus `save_first_hit` were located in
read-only `msg/US/global.txt`; source controls and source choice rows were
inspected without rewriting their payloads. Metadata/hash/token witness is
/tmp/sb-save-message-witness.json; raw original text is not copied into source
history. Fresh DOL leaf/card-helper/field-Main listings remain /tmp only.
`verify_event_operand_namespaces.py` pins the namespace/storage dependency;
this new event-flow witness itself is static manual recovery, not an executed
original event or a complete graph verifier. Original Nintendo/Intelligent
Systems asset rights remain; no redistribution is implied. Docs-only: no
make/build, Dolphin or hardware claim. Common card format/error/timing, backend
code mapping, global persistent namespaces, original UI captures and native
choice/save/cancel regression coverage remain open.
