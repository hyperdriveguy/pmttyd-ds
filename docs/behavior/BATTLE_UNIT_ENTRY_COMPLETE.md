# Complete local battle unit entry and publication order

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BtlUnit_Entry80128fe0/6b4. Read
[registry/scalars/geometry](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md),
[pose/display requests](BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md),
[part allocation/lifetime](UNIT_PART_DEFENSE_AND_LIFETIME.md),
[status/pouch](UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md),
[badge import](UNIT_BADGE_COUNT_IMPORT.md) and
[root event creation](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md).
Verify:`python3 tools/verify_battle_unit_entry_complete.py`:33 complete/linked
fingerprints,441 anchors and11 startup/numeric spans. Static original DOL
behavior;no original actor/unit/pose/source/runtime trace claim. Called resource/
render/heap/scene services are explicit boundaries,not inferred complete engines.

## Inputs,first64 registry scan and checked unit allocation

S is supplied setup pointer. Capture battle-work pointer B=word8041eb90
(r13+1c70) ONCE. Scan i=0..63 ascending using BattleGetUnitPtr(B,i). First
returned null selects i. No free slot→return0 without allocation/registry write.
Otherwise request BattleAlloc(0xb34=2868). Exact null return→return0,again
without registry write. No S/B null or input-table validation.

For nonnull U,memset(U,0,b34),then BattleSetUnitPtr(B,i,U) BEFORE ANY identity/
kind/resource initialization. Store U.word0=i. The complete local constructor
therefore publishes a cleared partial object before helper calls and field writes.
No deferred/atomic publication,rollback unregister or later resource-failure
cleanup is supplied. The array extent64 differs from event contexts'256 slots.
BattleAlloc uses the nested battle map root,as the companion contract specifies.

## Identity and early called initializers,in order

1. Copy S.word0→U.kindPointer10;read its wordc0→U18.
2. Reload S.word0 twice;copy kind.word0→U4,then kind.word0→U8 independently.
3. BattleStatusEffectInit(U) (complete body is ONLY blr);BattleStatusIconInit(U)
   (clear U+348 length7a0);BattleStatusChangeInfoWorkInit(U) as linked contract.
4. Store U.bytee=ff;copy RAW S.byte4→U.bytec (belong);read U.kindPointer10,
   copy kind.bytee→U.byted.
5. Read SIGNED U.word8. If exact222 OR224<=kind<231,call pouchGetPtr and
   replace U.byted with low8 of signed pouch.halfword8a. Other kinds retain
   the kind byte. This does not change the belong byte.
6. Copy S.word8→U.word2c. Store word0 to U294,298,29c,28c,290,2a0,2a8
   in that order. Do not collapse/rename these independent retained fields.

Status-effect no-op does not establish an initialized renderer/effect pool.
Icon clear is an additional subrange clear after whole U clear,not allocation
of sprites. Source-pointer and pouch alias/ownership semantics remain external.

## Position/rotation/scale requests and kind-word copies

Reload S.floatc/10/14 before EACH request:SetPos(U,x,y,z),SetOffsetPos(U,x,y,z),
SetHomePos(U,x,y,z). These are three separate storage triplets;do not eliminate
one as a duplicate or infer actual world offset meaning. Then request,ordered:
SetBaseRotate(0,0,0),SetRotate(0,0,0),SetRotateOffset(0,0,0),SetScale(1,1,1),
SetBaseScale(1,1,1). Geometry helpers' sentinel/normalization policy is separately
specified. Original positive-zero/one constants are pinned.

Reload U.kindPointer before EACH row below;read/store listed words ascending.
A triplet's first two source words are loaded before its first two stores;the
third word is read/stored afterward. Preserve WORD copies,not reinterpretation.

| Kind source words | U destination words |
| --- | --- |
| 20,24,28 | 84,88,8c |
| 3c,40,44 | b4,b8,bc |
| 5c,60,64 | ec,f0,f4 |
| 68,6c,70 | f8,fc,100 |

Reload S.kindPointer for source HP/FP (not U10):kind.signedHalf8→U10a;
reload signed U10a→U108;reload S.kind/signedHalf8→U10c. Repeat kind.signedHalf
 a→U110,reload signed U110→U10e,reload S.kind/signedHalf a→U112. Store float1
at U114,word0 at U27c,138,wordffffffff at U140;then HpGaugeInit(U).
U114 scalar1 is distinct from vector scale90/9c;later scalar writers remain
open. No kind/stat minimum or signed clamp in these copies.

## Remaining kind/unit geometry/resource copies

Reload U10 for each scalar source:kind signedHalf16→Ucc,18→Uce,1a→Ud0,
1c→Ud2,2c→Ud4,2e→Ud6. Uce is the complete GetHeight source. Then reload
kind once,copy WORDS74/78/7c→Ud8/dc/e0 (first pair read before stores,third
read afterward). Reload kind per value,float80→Ue4,float84→Ue8,byte8a→U25,
byte8b→U26. Call ClearStatus(U),using linked full status contract.

Reload kind.wordac→U104;reload kind.wordb0→U144. These are POINTER/word copies,
not allocation/copy/validation of pointed data. The schema/lifetime of kind.b0
and its+10 byte consumer remains unresolved by the copy alone. Store bytes1
at U21,U22,byte0 at U188. Read SIGNED U.bytec;copy byte at B+8*belong+a
→U189. No side-index bounds check;do not derive side from unsigned GetBelong.
Copy S.words1c/20/24/28→U218/21c/220/224 in that order.

## Part block and full local tail

The complete part construction sequence80129398..801295b0 is in
[part contract](UNIT_PART_DEFENSE_AND_LIFETIME.md):unsigned count kind.b4,
BattleAlloc(count*500),unchecked allocation,bulk clear,pointer U14,linked nodes,
source stride4c,owner/attribute/defense/offset/scale/side/resource copies.
The loop enters at least once even for zero count;do not invent a zero-count
skip/null failure return after the earlier checked UNIT allocation. Companion
geometry/pose contracts close the called local setters and lookup policy.

Each part attribute bit1 can overwrite U2d4 with its source ID;multiple such
parts leave the LAST ID there. After construction,GetBodyPartsId(U) finds the
FIRST flagged node's source ID. Entry then GetPartsPtr(U,thatID),GetPoseNameFromType
(P,65)→U2d8,GetPoseNameFromType(P,43)→U2dc. Both pose requests use the same
retained P. No null/body-fallback check before those pose helper calls;valid
part/body/table domain is required. Do not substitute cached LAST U2d4 for the
FIRST lookup as if all data were guaranteed single-body by this local code.

Store bytesff at U311,U312;call btlDispEntAnime(U). Then store words0 at U314,
318. If signed U.word8==208,request BattleAlloc(44 hex=68 bytes) and store
returned pointer in U314,without null/extra initialization branch. Other kinds,
including222,retain U3140. Call SetParamFromPouch(U),then store U288=0.
Pouch import/badge/resource and geometry can therefore affect final state AFTER
initial kind-stat/gauge/presentation writes;the constructor's earlier copies
are not proof of final original Mario/partner HP/FP.

Reload kind pointer,read kind.wordbc as optional init SCRIPT. If null,return U.
Otherwise evtEntry(script,priority10,flags0);store U.word0 actor ID into new
E160;read E15c ID and store to U288;return U. No event-allocation-null check,
USER_FUNC execution or direct evtmgrCmd call. Root flags0 do not append during
an active manager pass;the next schedule rebuild can pick it up. Kind init
script source/REL/callback and actual executed unit readiness remain separate.

Acceptance:first64-free/exhaustion/unit allocation null versus unchecked part
allocation,early publication/captured B;all ordered identity/clear/pouch/type
paths,three coordinate rereads,kind and S pointer distinction/stat reloads,
scalar114 versus vector scale/height/belong;part loop/body first versus cached
last,pose types65/43,display-before-special allocation/pouch/init event actor/ID
stores. Full kind/part/memory/pose/render/status/source/domain and original
spawn/damage/death/runtime traces remain DMG-01/DMG-02 requirements. This closes
local BtlUnit_Entry while preserving external engine/lifetime boundaries.
