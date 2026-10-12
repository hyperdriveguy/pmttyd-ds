# Movement sound control: retained voice,alternating triggers and stop

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete _MoveSoundControlCore80104850/180,
_MoveSoundControlEndCore801047bc/44,_UnitMoveSoundControl80104a24/54,
_UnitMoveSoundControlEnd8010482c/24. Original sound playback/lookup internals
and actual US audio output remain dependencies. Read with the
[record loader](BATTLE_UNIT_SOUND_MODE.md) and [MovePosition](BATTLE_MOVE_FRAME_AND_POSITION.md).

## Core arguments and record schema

The core receives unit U,record R,optional part P and binary32 position XYZ.
It first copies that XYZ into a temporary vector,then reads keys. There is
no local pointer/null/domain validation. In this body the record fields have
these concrete roles:

| Record field | Read/write behavior |
| --- | --- |
| +0 / +4 | First/second playback key inputs;zero suppresses that request |
| +0xc | Initial remaining count supplied by loader;not read by this core |
| +0xe / +0x10 | Signed first/second reset periods |
| +0x12 | Signed remaining trigger count,stored as16 bits |
| +0x14 | Signed16 alternation counter;parity selects key/period |
| +0x18 | Retained playback handle word;sentinel-1 means unstarted |

Bytes+8..+0xb remain outside this core's use. The record loader's partial
reset/retention policy remains separate;do not label key words themselves as
live voice handles. Only this body's retained+0x18 route stores a trigger return.

## Empty and single-key retained-voice branch

If both key words are0,return without record mutation. Otherwise,when first
key!=0 and second key==0:

1. Read R18. If it equals-1,request
   BtlUnit_snd_se_pos(U,firstKey,rawSentinel-250000000,0,&suppliedXYZcopy)
   and store its returned word at R18. No success/error classification occurs.
2. If optional P!=null,GetPartsWorldPos(P,&updateXYZ);otherwise GetPos(U,&updateXYZ).
3. Request psndSFX_pos(currentR18,&updateXYZ) and return.

Step2/3 occurs even if the trigger returned-1;the core has no local failed-
handle skip. It rereads geometry after trigger rather than reusing supplied
XYZ for the position update. Subsequent calls with a non-1 handle do not
retrigger;they request the position update. Core trigger/position semantics,
valid handles and effect/audio lifetime must come from the actual callees.

## Periodic alternating branch

Every other nonempty key combination uses this branch,including first0/second
nonzero. Read S16(R12). If negative,return without decrement or trigger.
Otherwise decrement and store its low16 bits,reload as signed. If now>=0,
return. If it became negative:

1. Increment S16(R14) and store/reload the narrowed halfword.
2. If low bit is1,select first key and store first period S16(Re) into R12.
   Otherwise select second key and store S16(R10) into R12.
3. If selected key!=0,request
   BtlUnit_snd_se_pos(U,key,rawSentinel-250000000,0,&suppliedXYZcopy).
   The returned word is **not stored**. A zero selected key omits playback
   after counter/period mutation;it does not skip alternation.

This branch performs no optional-part/current-unit position query after the
supplied position vector and no psndSFX_pos request. With loader alternation0,
the first expiry selects first key,next second,and so on. A nonnegative reset
period k expires after k+1 core invocations;negative period disables further
decrement/trigger until another owner changes it. These are callback counts,
not measured rendering-frame or wall-clock sound intervals. Counter14 wraps
as signed16 before parity testing;there is no saturation or randomized footstep.

## Unit wrappers,stop and actual movement caller

_UnitMoveSoundControl(U) first GetPos into separate float temporaries,then calls
the core with `(U,U+0x18c,null,visibleXYZ)`. The core can therefore request
another GetPos on its retained-voice update route. Keep those ordered queries;
they are not one cached vector service call.

_UnitMoveSoundControlEnd(U) calls the end core with R=U+18c. End core reads
word R18;if it equals-1,return. Otherwise call psndSFXOff(handle),then store-1
at R18. It does not reset keys,periods,remaining or alternation;any other handle
word is passed without range/return-value validation. Its U argument is unused
by the local end body.

MovePosition loads the record on its first call,integrates,then calls normal
unit sound control only while remaining count>0. Its final n<=0 path snaps to
targetXYZ and calls the end wrapper instead. Thus termination stops a retained
voice but does not invent playback of a final periodic key. The first-call
loader always assigns R18=-1 without stopping a prior retained handle;the owner
graph must ensure prior cleanup or document its actual behavior. This body does
not repair that lifetime ordering implicitly.

MarioJumpPosition instead has a separate setup-only psndSFXOn_3D request and
ignores its return. Do not connect its sound path to this retained/periodic
movement core merely because both use the same loader/record fields.

## Implementation requirements and unclosed resources

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| MOVE-SE-01 | Preserve empty/single/alternating branch selection | Empty does not mutate;first nonzero/second0 retains trigger result;first0/secondnonzero still alternates |
| MOVE-SE-02 | Preserve signed expiry and period/parity order | Remaining0 expires on current call;period2 waits three calls;negative remains disabled;zero selected key still resets/alternates |
| MOVE-SE-03 | Preserve retained voice position/stop ordering | New trigger then fresh geometry then position update;end Off then handle-1;periodic trigger return ignored |
| MOVE-SE-04 | Bind movement versus jump sound ownership separately | MovePosition only updates on blocked steps and ends on final snap;Mario jump's setup request does not create R18 retention |

tools/verify_jump_preparation.py pins complete core/wrapper fingerprints and
counter/voice/request anchors. BtlUnit_snd_se_pos80126088/114,psndSFXOff800d95f8/
c8,psndSFX_pos800d96f8/4c,__psndSFXOn resource/macro/voice internals,record
source writers,other callers and original audio traces remain open. No generated
sample,guessed sound key or event duration substitutes for them. Original ISO
inspection/data artifacts remain under /tmp or ignored paths and are read-only.
