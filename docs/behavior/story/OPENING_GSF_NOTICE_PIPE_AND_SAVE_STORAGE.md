# Opening flags: save notices, pipe departure and save storage

US G8ME01 revision0; reconciled/verified2026-10-13 for
[GSF-01](../../requests/GSF-01.md), [DEMO-01](../../requests/DEMO-01.md) and
[SAVE-01](../../requests/SAVE-01.md). Source-only bounded flag ownership, not
an exhaustive opening flag census or original save/reentry trace. Read
[save event](../field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md),
[Professor reload](PROFESSOR_STORY11_RELOAD_AND_LECTURE_REENTRY.md),
[switch storage](../memory/SW_SET.md).

## IDs are live GSF bits, not independent native booleans

Global flags use GP=[80415fd0], bank GP178,256 words/8192 bits. For these valid
nonnegative indices, word=178+4*floor(index/32), mask=1<<(index%32). Full signed
invalid-index/rotate behavior remains the switch contract; no new index clamp.
GSF event operands are signed index-130000000. Offsets/masks below hexadecimal.

| Source operand | GSF | Live GP word | Mask | Serialized record word |
| --- | --- | --- | --- | --- |
| f8405c69 | 233 | 194 | 00000200 | 19c |
| f8405c6c | 236 | 194 | 00001000 | 19c |
| f8406037 | 1207 | 20c | 00800000 | 214 |

GSW0/rawf5de0180 is the full32-bit story word GP174, serialized at record17c.
GSW4/rawf5de0184 is byte GP57c, serialized at584. GSW0 is not flag0 or an8-bit
truncated story. GSW4 and GSF233/236 are distinct state. Event LF10/LF11 and
LW0/LW10 remain event-local, not additional persistent GSF bits.

## Save-block first notice and story7+ latch

Original system event8030b4ec (16 commands) executes:

1. Set event priority0; request Mario keysOFF; set LF10=0.
2. IF GSF233==0: SET GSF233=1 FIRST, then request message key `save_first_hit`
   with source flags/speaker/coordinates0; SET LF10=1; END_IF.
3. Independently IF GSW0>=7: SET GSF236=1; END_IF.
4. SET LW0=0; RUN_CHILD803bac3c; request Mario keysON; RETURN; END.

The source closes the first IF BEFORE testing story7. Thus GSF233 already1
suppresses the first notice but does NOT suppress the GSF236 write when story>=7.
Story<7 does not clear236. Repeated entry with233=1 does not print first notice
again, but may still set236. No battle outcome/scene-completion predicate is
added to this writer; its admission remains the original object/event owners.

Both flag writes precede the later save-choice child. Refusing that choice does
not roll them back in live RAM. This is not an automatic on-disk save: original
card write admission/error/completion remains separate. The system event does
not advance GSW0. Its236 write proves a story7+ latch, not an extra message or
an invented general “post-story notice” consumer. Neither this stream nor the
197-command common save child reads236 to select such a message.

The52-byte wrapper uses entry LW0==0 to choose the common save child without
an additional1000ms wait; nonzero entry takes that wait after the same child.
Inside common event803ba444, LW10 captures entry LW0. Mode23 jumps past the
prompt. Other modes ask the original choice; refusal LW0!=0 returns before
GSW4 write. Accepted path at803ba520 writes GSW4=LW10 before warning/header/
slot-write requests. Normal save-block mode0 therefore writes GSW4=0 on acceptance;
refusal retains prior GSW4 while keeping earlier233/236 live writes.
Original message payloads/choice order remain in the save-event contract/assets;
no paraphrase, new prompt or provisional native choice semantics introduced.

## GSF1207: original east-pipe departure and story11 reload

Original gor_02_event_01 REL file offset3edac (11 commands) tests EXACT
GSF1207==1 first. If true: LW0=0, RETURN, no party/write requests. Otherwise:

- evt_party_stop(1) at3edcc;
- evt_party_cont_onoff(1,0) at3edd8;
- evt_mario_set_party_pos(1,11,0,-1000,0) at3ede8;
- SET GSF1207=1 at3ee04;
- LW0=0, RETURN, END.

Function pointers are original DOL imports, independently pinned. The callback
has NO story11 predicate. Its original BERO custom-out registration identifies
east pipe `dokan` toward `tik_01/dokan_2`; the linked reload contract owns that
registration/admission. LW0=0 retains generic route processing rather than
replacing the pipe with a fabricated scene.

Professor init at REL410c8 has an EXACT story11 arm. GSF1207==0 selects position
(-220,0,305) at41158; otherwise(0,-1000,0) at41174. These are engine coordinates,
not native rendering units. Exact11 precedes the later11..17 range; do not hide
him solely because lecture ended or apply that range to the already matched11.
Loaded actor/setup still exists; negative-Y positioning is distinct from deleting
the actor. Flag1207 is independent of GSW0/story11, so save/reload must preserve
both. The scoped module census does not exclude other modules/computed writers.

## Live/save/load ownership proved by the original services

[Writer](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md) copies GP0..13d7
at record+8, including every global/local bank and the words/bytes above.
[Reader](../memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md) validates
and selects resident data; [restore](../memory/CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md)
copies record+8 back to GP. Its explicit preservation/reset fields do not
intersect these banks. Those are concrete byte-copy/validation services, not
proof that original IO or a native round-trip has executed successfully.

[Continuation](../memory/SAVE_CONTINUE_GAME_PARTY_AND_DESTINATION_DISPATCH.md)
then uses GSW4 to choose destinations. Its mode23 arm calls swInit and resets
GSW0 before aaa_00/prologue, so retaining all restored flags is not a universal
post-load lifetime rule. Other modes/scene writers and actual admission must
remain composed with the original graph. Native flag fields must represent
these bank identities and branch exceptions, not substitute independent booleans
or treat successful card IO as the source flag-write trigger.

## Validation and remaining demo work

[Manifest](../../../tools/opening_gsf_notice_and_storage_evidence.json) pins
three DOL event spans/112 selected words, two gor REL spans/five import bindings;
command counts16/8/197/11/37. Independent raw command order/IF nesting/SET operands,
pipe imports/Professor predicates, flag masks/record offsets and six notice/two
pipe mathematical vectors checked. Switch manifest re-passed63 spans/205 anchors;
writer/restore pins reverified. Hashes alone do not establish complete event
execution. Raw source resources stay outside history; no dialogue rewritten.

GSF-01 now has these bounded notice/pipe IDs and live/save storage mapping.
Remaining: exhaustive opening read/write/actor-admission census, other flags,
cross-scene joins/results, upstream object/pipe/scene/load admission, full namespace
lifetime/IO/native binding and original reentry/save/load traces. DEMO-01/RESULT-01
remain open; this does not establish the complete playable demo or full game.
