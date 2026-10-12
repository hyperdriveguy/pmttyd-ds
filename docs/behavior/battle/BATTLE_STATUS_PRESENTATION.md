# Status application effects, announcements and completion result

US G8ME01 revision0, inspected 2026-10-12. This completes local control-flow
coverage of `BattleSetStatusDamage` from0x800fc198 through0x800fcf50, after
the [infliction prefix](BATTLE_STATUS_INFLICTION.md). B evidence means original
US instructions/data; effect and sound names are symbol/data annotations.
See [baseline](../../ORIGINAL_BEHAVIOR.md) for identity/provenance. No original
execution, visual comparison or Switch-version equivalence is claimed.

Notation: D/P are recipient unit/part, id is status ID, T/S are requested
duration/strength, oldCharge is the pre-application Charge strength captured
at entry, and q is the local SetStatus return (zero for special IDs25/26).
This local tail does not reinterpret q as the function's final return value.

## Completion result and route selection (B)

After admission, chance and prefix result-bit updates, q≠0 enters the stamp
effects block at0x800fc198. q=0 instead enters the cleanup block at0x800fcb88.
Both reach announcements, sound selection and `return1` at0x800fcf3c.
There are no further local failure returns in this tail.

Thus full function return0 means rejection by the earlier gates/chance, while
return1 means the request reached this tail. It does not establish that the
requested status remains active, that SetStatus returned1, or that an effect/
announcement was emitted. The prefix's accumulated bits, setter result and
whole-call result are three distinct outputs/conditions. The weapon wrapper
uses the whole-call result for its id18 `btlDispPoseAnime(P)` call.

## Position selection on q≠0 (`0x800fc198`–`0x800fc294`)

Start from `BtlUnit_GetPos(D)` as (x,y,z). Obtain integer height h and compute
signed `trunc(h/2)`. If D attributes at+0x104 have mask2, subtract this value
from y; otherwise add it. Then inspect kind ID at D+8:

- IDs23,132,171: x−=300; z+=30.
- IDs149 or150: z+=40.
- Others: no further adjustment.

The large x adjustment is literal binary data, not a guessed typo to normalize.
Constants are binary32 at0x804221c8=300,0x804221cc=30,0x804221d0=40.
This branch uses unit position, not target-part hit position.

## Stamp selection and payload writes (B)

Call `effStampN64Entry(type,x,y,z+10)`. The returned effect's +0x0c points
to a payload. Most cases then store six bytes at payload offsets0x38–0x3d.
The table gives their exact ordered values; color-channel interpretation and
effect animation internals require the effect's own contract.

Case dispatch is the28-entry US table at0x8034c7f8. The default outside0–27
uses the same effect as IDs17/25/26. IDs25/26 normally have q=0 and therefore
do not reach their stamp cases through this function's ordinary path.

| ID | Stamp type | Bytes38,39,3a,3b,3c,3d |
| --- | --- | --- |
| 0 | 2 | 160,220,0,10,10,10 |
| 1 | 0 | no payload override here |
| 2 | 2 | 220,110,110,10,10,10 |
| 3 | 2 | 0,110,220,10,10,10 |
| 4 | 2 | 0,220,0,10,10,10 |
| 5 | 1 | no payload override here |
| 6 | 2 | 220,220,0,10,10,10 |
| 7 | 2 | 220,220,220,0,220,0 |
| 8 | 2 | 220,220,0,220,0,0 |
| 9 | 2 | 220,220,220,0,110,220 |
| 10 | 2 | 220,220,220,220,0,0 |
| 11 | 2 | 220,0,0,10,10,10 |
| 12 | 2 | 220,220,220,220,0,0 |
| 13 | 2 | 220,0,0,10,10,10 |
| 14 | 2 | 220,220,220,0,0,220 |
| 15 | 2 | 0,0,220,10,10,10 |
| 16 | 2 | 220,220,220,220,220,0 |
| 17 | 2 | 200,200,200,10,10,10 |
| 18 | 2 | 220,220,220,110,110,110 |
| 19 | 2 | 220,220,220,220,110,0 |
| 20 | 2 | 220,110,0,10,10,10 |
| 21,22 | 2 | 220,220,0,0,0,220 |
| 23 | 2 | 220,220,220,220,110,110 |
| 24 | 2 | 220,220,220,160,220,0 |
| 25,26 /default | 2 | 200,200,200,10,10,10 |
| 27 | 2 | 220,220,220,10,10,10 |

After the stamp, IDs10–13 and16 call `effUpdownEntry` with integer arguments
`(0,S8(S),60)` and floats `(x,y,z+15)`. IDs14/15 instead use integers
`(1,S8(S),60)` and floats `(x,y,z+20)`. Other IDs skip it. The strength passed
is the requested S, not the re-read resulting status strength. Constants are
0x804221d8=15 and0x804221d4=20. These calls join the announcement block without
passing through the q=0 cleanup path. Effect allocation results are dereferenced
without null checks on payload-writing branches.

## Cleanup on q=0 (`0x800fcb88`–`0x800fcc48`)

Re-read unit position and height; set y to positionY+trunc(height/2), regardless
of attribute mask2 and without the kind-specific x/z corrections above.

- id8: if pointer D+0x32c is nonnull, call `effSoftDelete(pointer)`. This
  block does not clear the stored pointer itself.
- id9: call `effVaporN64Entry(0,x,y,z,1.0,60)`. In its returned payload at
  effect+0x0c, write word255 at offsets18,1c,20,28,2c,30 (hexadecimal).
- Other IDs: no effect call here.

This branch is selected by q=0, not a fresh check that Burn or Freeze is absent.
Earlier setter cancellation determines the state. The whole application still
returns1 after this cleanup. General effect deletion/lifetime and payload
ownership remain separate dependencies.

## Announcement selection (`0x800fcc4c`–`0x800fcd7c`)

Initialize local sound-enable a=0. If q=0, skip all announcement calls.
Otherwise re-read the resulting status through GetStatus(D,id,&t,&s), then:

| ID/group | Condition | Info value and announcement mode |
| --- | --- | --- |
| 16 Charge | S8(s)>0 | value=S8(s); mode1 if it differs from S8(oldCharge), otherwise mode0 |
| 4 Poison | S8(s)>0 | value=S8(s), mode1 |
| 0–3,5–15,18–24 | S8(t)>0 | value=S8(t), mode1 |
| 17,25–27, other | none | no announcement |

For each selected case, call in order:
`BattleStatusChangeInfoSetAnnouce(D,id,value,mode)` and
`BattleStatusChangeMsgSetAnnouce(D,id,mode)`, then set a=1.
Mode0 for unchanged positive Charge still sets a=1 and allows its sound.
Negative durations can be present according to CheckStatus yet fail these
positive-only announcement gates. Shared signed statuses are re-read through
the getter's sign filter; do not announce solely from requested duration T.

These are registrations with the announcement subsystem; message text,
display order, queue lifetime and animation need that subsystem's contract.
No dialogue text is invented or transcribed by this document.

## Sound selection and exit (`0x800fcd80`–`0x800fcf50`)

Always fetch `BtlUnit_GetHitPos(D,P)` at this stage, even if a=0. If a≠0 and
id≤24, dispatch through the25-entry table at0x8034c794 and play one
`psndSFXOn_3D(key,hitPosition)` call for the mapped case. ID17's table entry
skips sound. Others outside the table skip it. Original keys/addresses:

| ID | US address | Key |
| --- | --- | --- |
| 0 | 802ee234 | SFX_CONDITION_ALLERGY1 |
| 1 | 802ee0b0 | SFX_CONDITION_SLEEP1 |
| 2 | 802ee0c8 | SFX_CONDITION_STOP1 |
| 3 | 802ee0dc | SFX_CONDITION_GURUGURU1 |
| 4 | 802ee10c | SFX_CONDITION_POISON1 |
| 5 | 802ee0f4 | SFX_CONDITION_CONFUSE1 |
| 6 | 802ee13c | SFX_CONDITION_BIRIBIRI1 |
| 7 | 802ee124 | SFX_CONDITION_YOKEYOKE1 |
| 8 | 802ee1c0 | SFX_CONDITION_FIRE1 |
| 9 | 802ee1d4 | SFX_CONDITION_ICE1 |
| 10 | 802ee154 | SFX_CONDITION_DEKADEKA1 |
| 11 | 802ee16c | SFX_CONDITION_MINIMINI1 |
| 12 | 802ee24c | SFX_CONDITION_TUYOTUYO1 |
| 13 | 802ee264 | SFX_CONDITION_YOWAYOWA1 |
| 14 | 802ee184 | SFX_CONDITION_KATIKATI1 |
| 15 | 802ee19c | SFX_BTL_FUNYA1 |
| 16 | 802ee27c | SFX_CONDITION_CHARGE1 |
| 18 | 802ee294 | SFX_CONDITION_TOUMEI1 |
| 19 | 802ee1e8 | SFX_CONDITION_FAST1 |
| 20 | 802ee1ac | SFX_CONDITION_SLOW1 |
| 21 | 802ee2ac | SFX_CONDITION_COUNTER1 |
| 22 | 802ee2c4 | SFX_BTL_THUNDERS_COUNTER1 |
| 23 | 802ee1fc | SFX_CONDITION_REST_HP_SLOW1 |
| 24 | 802ee218 | SFX_CONDITION_REST_FP_SLOW1 |

Then return1. There are no direct irand/rand calls in this tail; called effects
or services may themselves consume randomness. Do not assert the entire tail
is RNG-free without inspecting those callees.

## Verification obligations and remaining scope (P)

Derived cases: admitted opposite-status cancellation gives q=0 but whole-call1;
negative duration suppresses announcement after a stamp; unchanged positive
Charge uses mode0 yet emits sound; id27 gets a stamp but no announcement/sound;
unit kind/attribute position corrections differ from cleanup; sound uses part
hit position independently of the stamp's unit position; exact byte vs word
payload writes. These are not yet original execution or rendering comparisons.

Together with the infliction and storage documents this covers the local
application routine's branches. Effect implementations, sound macros,
announcement queues, status update cadence and battle callers remain open.
Complete-game requirements cannot treat those dependencies as satisfied merely
because this function's local dispatch is now specified.
