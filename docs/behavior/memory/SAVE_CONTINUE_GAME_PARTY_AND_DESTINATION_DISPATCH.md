# Save continuation: party setup and destination dispatch

US G8ME01 revision0; recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete continueGame800f39dc/550hex,
340 instructions;25-entry switch table and23 source string anchors. Addresses/
fields hexadecimal; numeric arguments/indices decimal. Static original requests,
not measured save/load/scene execution. Read
[live restore](CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md),
[single-slot validation](CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md),
[switch storage](SW_SET.md). Full loadMain/admission and child internals separate.

## Unconditional restore, slot and party preparation

Capture rodata base802edb88 into GPR31. Fresh load-work pointer[804169d0].word20
supplies slot r3 to cardCopy2Main800b2388 at800f39fc. No local record flag/
checksum/card/busy/slot-range guard before restore. Upstream loadMain/event/UI
must establish valid resident data; calling this function alone does not.

AFTER restore, independently reload load-work and GP=[80415fd0] twice: copy
current load-work20 to GP11d0, then GP11b8. These are fresh per-publication values,
not one retained pre-restore slot. GP11b8 overwrites that leaf's captured-live
preservation. Then requests/writes in exact order:

1. marioPartyKill80141198 at800f3a20.
2. marioGetPtr8005e0c0 at3a24; captured Mario bytes247=0,248=0.
3. psndSetFlag800dc110(4) at3a38.
4. Fresh GP11e0 → marioPartyEntry801412fc at3a44.
5. Fresh GP11e4 → marioPartyEntry at3a50, even if equal/zero/negative.
6. psndClearFlag800dc0f8(4) at3a58.
7. countDownSaveReStart80200e14 at3a5c.
8. Fresh GP.word0 OR1; swByteGet8008582c(4) at3a74.

No child-result gate, null/ID/partner deduplication or rollback. Sound flag4
brackets both party requests; it is not locally preserved/restored to an old
value and its backend meaning remains child-owned. Child failures/nonreturn/
mutation can affect later reads. Saved party kinds come from restored GP words,
not the Mario bytes cleared in this caller. Countdown initialization remains
its own child contract.

## Exact switch4 destination table

Unsigned getter result>24 skips destination actions and exits AFTER the whole
prefix above. Values0..24 index table8034c628 and branch through CTR. No later
switch reread. All selected arms request seqSetSeq8002e218 with sequence3.
Ordinary arms first store literal GP110, then independently fresh GP108;
sequence arguments room/door below follow in original order. Numbers decimal.
Null door means literal0, not an empty string. Names are exact original keys.

| Switch4 | GP110 | GP108 | Room argument | Door argument |
| --- | --- | --- | --- | --- |
| 0 | 0 | 28 | Fresh GP+12c pointer | Null |
| 1 | 61 | 56 | aji_10 | Null |
| 2 | 62 | 58 | kpa_00 | Null |
| 3 | 60 | 54 | nok_01 | nokotarou |
| 4 | 61 | 56 | aji_10 | Null |
| 5 | 62 | 58 | hei_00 | Null |
| 6 | 60 | 54 | mri_00 | Null |
| 7 | 61 | 56 | aji_10 | Null |
| 8 | 62 | 58 | mri_01 | Null |
| 9 | 60 | 54 | tou_02 | Null |
| 10 | 0 | 28 | usu_00 | Null |
| 11 | 62 | 58 | gor_04 | Null |
| 12 | 60 | 54 | usu_00 | Null |
| 13 | 0 | 28 | muj_12 | Null |
| 14 | 62 | 58 | usu_00 | Null |
| 15 | 60 | 54 | muj_00 | Null |
| 16 | 0 | 28 | pik_03 | next |
| 17 | 62 | 58 | gor_01 | Null |
| 18 | 60 | 54 | gor_04 | resha_return |
| 19 | 62 | 58 | pik_01 | Null |
| 20 | 60 | 54 | aji_18 | e_bero |
| 21 | 60 | 54 | gor_00 | Null |
| 22 | 0 | 28 | jin_04 | Null |
| 23 | 0 | 25 | aaa_00 | prologue |
| 24 | 61 | 56 | aji_10 | Null |

Do not infer meaning of GP110/108, switch4 writer domains or scene admission
solely from literal patterns. Sequence child and actual room/door event graph
own resulting behavior. The table does not directly set Mario XYZ or fabricate
arrival dialogue; room/string pointer arguments are requests, not executed scenes.
In particular switch0 uses restored GP room storage, while other cases can
request an authored destination rather than that saved room.

## Switch23 reset arm precedes its destination request

At800f3e90 request stg0_00_init800ee688 FIRST. Fresh load-work20 to fresh GP11d0,
then marioPartyKill again; marioGetPtr and clear bytes247/248 again. Fresh GP110=0,
then fresh GP108=25. Request swInit80085980 at800f3ecc, then swByteSet80085850(0,0)
at800f3ed8, then seqSetSeq(3,"aaa_00","prologue") at800f3ee8; exit.

The already recovered swInit clears all global/local bank extents, and byte0
setter is a full-word operation. Thus this arm is a source-owned exception to
retaining restored switch banks through continuation; do not impose the ordinary
restore leaf's bank-copy rule as an unconditional post-load lifetime guarantee.
No original interpretation of why switch4==23 arises is invented. Full stg0
initialization/other writers/story conditions still need their own contracts.

All arms exit after the sequence child returns. No OS wait/clock/counter/null/
return-result check or independent scalar return. ABI16-byte frame/GPR31/LR.
One table jump, no indirect calls; direct children retain the normal source ABI.

## Validation and remaining work

[Manifest](../../../tools/save_continue_game_evidence.json):37 spans/388 anchors
(all340 body words/25 table words/23 string anchors),39 direct calls. Every
25-case mapping/operand/string/field/call site checked independently against
read-only ISO pins; restore body pins reverified. Generic verification checks
bytes/targets, not complete child/story/runtime claims. Strings are keys, not
paraphrased or fabricated dialogue. Raw ELF/listing/generator stay `/tmp`;
no save blobs/assets/code bodies distributed.

SAVE-01 now has complete local continuation caller. Remaining: upstream
loadMain/event admission, GP110/108/switch4 providers/namespace schemas,
party/sound/countdown/stg0/sequence/room/door child composition, actual slot/
record/value/extent/alias/lifetime/IRQ domains and original successful load/
reset/room/party/corruption/recovery/native traces. Full persistence/game OPEN.
