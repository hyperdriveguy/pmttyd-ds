# Original item95 camera, key and system-level providers

US G8ME01 revision0; read-only DOL reinspection2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md). Integration follow-up, not a duplicate
recovery of the [event system-level owner](../event/EVENT_SYSTEM_LEVEL_AND_SPEED.md)
or [camera8 decision lock](../camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md).
Addresses, offsets and masks hexadecimal; level arguments decimal.

## Item provider order and raw state

Finite fresh item95 substate0 at800abc88 sets substate1,counters0,then:

1. camGetPtr(8) at800abca0; OR200 into returned camera unsigned halfword0
   at800abca4–ac.
2. marioKeyOff8005c1f4 at800abcb0.
3. marioStSystemLevel(2)8006f7c8 at800abcb8.
4. psndSetFlag(80) at800abcc0.
5. Load float0 from804210b4; write Mario174,17c,180,18c at800abcc8..d4.

This falls through substate1 presentation. It does not request camera4 XYZ,
mode or follow lock. Substate2 separately owns CtrlOff; KeyOff is not CtrlOff.
The four inline writes are additiveX,additiveZ,retained base horizontal speed,
computed combined horizontal norm; **178 additiveY is untouched**. They do not
modify heading, air flags or pending/current motion. See
[motion/provider aliases](ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md).

Successful acknowledgment after common presentation teardown performs:
SystemLevel(0)800ac430; soundClearFlag80 at800ac438;
conditional status01000000 clear/ControlOn800ac450;
KeyOn800ac454; StayMotion800ac458; camGetPtr(8)800ac460;
clear only camera halfword0 bit200 at800ac470–74; named item-entry cleanup.
Failure instead enters pending discard substate5 and does not execute these
system/key/camera success restorations. Keep those owner fields retained.

## Nested key state

Fresh marioKeyOff8005c1f4/28 obtains Mario, ORs main word0 mask8 FIRST,
then increments byte39 with low8 storage and returns its signed result.
marioKeyOn8005c1c0/34 decrements low8 byte39 FIRST; reads the stored signed
result; clears main8 only when exactly0, then returns signed byte39.
There is no saturation, zero-depth guard or depth-independent unlock.

Ordinary NPC talk already owns its KeyOff before event construction and owns
KeyOn after whole talk-event death. Item adds/removes one nested interval.
Thus successful item cleanup can leave main8 set until subsequent NPC talk
completion. Global system enum, GP mask, camera bits and CtrlOff byte38 are
separate owners; none substitutes for byte39. The exact talk/key lifecycle is
[already recovered](GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md).

## Level2 is GP mask3 plus an event-stop request

marioStSystemLevel8006f7c8/114 accepts signed0..4; no stack/reference count.
For request2: store signed enum2 at8041e940; read GP.word18,ANDfffffff3/store;
reload GP/word18,OR3/store; call evtStopAll(2) at8006f878.
For request0: store enum0; clear GP.word18 low nibble withANDfffffff0/store;
call evtStartAll(ef) at8006f818. Higher bits survive. Actual
marioStGetSystemLevel8006f7bc returns **whole GP.word18**, not enum8041e940.
A native fieldSystemFlags intended to represent that GP word must receive
low nibble3 for request2, not integer2 or a Booleanpaused flag.

StopAll/StartAll select current field/battle context from GP.word14 and
traverse active registry slots in ascending order; called traversal visits
child/owned descendants before applying each entry's type-byte intersection.
Stop sets event flag2; Start clears it. Neither call excludes the requesting
event. Thus an admitted type6 Goomba3 talk root intersects mask2 and stops;
its item owner still runs later through itemMain and can restore level0.
Do not keep polling a stopped source event as if SystemLevel were purely
metadata. Level0 StartAll ef intersects type6 and can make it eligible on a
later event-manager visit. The wrappers introduce no new render-frame wait;
field/event ordering and credit eligibility own that continuation.

A future lower nonzero level does not implicitly resume former categories;
level0's ef excludes type10. Constructor run-check reads enum8041e940 and
can stop all matching current-context events when creating a new one.
[Stop/start traversal](../event/EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md) and
[constructor admission](../event/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md)
retain those stateful effects. This finite level2→0 path is not a full native
all-event scheduler or universal NPC/physics/render freeze.

## Camera8 decision lock and viewport boundary

camGetPtr8000f2fc/14 directly resolves table803c95f8+4*ID:8 selects+20.
Camera8 is original2d, camera4 original3d; actual pointer initialization is
camInit, not disc BSS values. The bit200 operation is a retained halfword
flag write, not an immediate projection/viewport API call.

camLetterBox8000e388/1dc tests camera8 bit200 at8000e3b8. When set it skips
only automatic enable/disable decisions and preserves existing enabled100.
Remaining enabled path still grows signed bars by3 toward top49/bottom44;
disabled path still fades alpha by2*(255/GPword4),clamping below0. Independent
400 can request immediate cap/zero. It still queues camLetterBoxDraw through
dispEntry. Clearing200 re-enables automatic decisions on a later consumer
visit; it does not directly disable bars or zero their dimensions.

Therefore this item provider alone supplies no new world viewport crop,
letterbox-on assertion, camera-follow freeze or window rectangle resize.
Retain camera8 state separately; existing bars/alpha/flag100 and original
letterbox draw services determine visible impact. Original camera5 effect/icon
projection and camera8 item windows retain their separate draw owners. A native
implementation that records200 while those consumers remain unbound should
say so explicitly, without applying200 to field camera4 or guessing a crop.

## Verification and limits

Fresh named DOL extraction covered marioStSystemLevel,marioKeyOff/On,
camGetPtr and camMain; itemseq source sites were rechecked against the original
GetItem body. Camera decision-lock and event traversal complete-body evidence
is linked above. Inspection files remain ignored in
artifacts/reference/goomba3-services/system_*.

No code/shared index/progress edits; this is source recovery. Native event
stopped-state execution, all source system-mask consumers, complete letterbox
predicate/draw adaptation and original/hardware scheduling remain validation
boundaries. Preserving source request fields does not close their consumers.
