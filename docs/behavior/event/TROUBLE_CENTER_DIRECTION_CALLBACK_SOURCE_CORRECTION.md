# Trouble Center direction callbacks: fresh source correction

US G8ME01 disc0 revision0; read-only DOL/constants2026-10-14. This contract
corrects the angle-base and threshold prose in
[earlier professor turn notes](../story/PROFESSOR_ITEM_POSE_JUMP_AND_TURNS.md).
Both source helpers use **270 minus requested yaw**, not360 minus yaw.
Mario's recenter comparison is **270**, not180. Apply this fresh evidence to
[board first lecture](../field/TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md).
Recovery inspected source read-only; native integration is recorded in the
[contact/presentation contract](../field/NATIVE_TROUBLE_CENTER_CONTACT_AND_PRESENTATION.md).

## Exact bodies, constants and native field identities

| Source | Address,size |
| --- | --- |
| evt_mario_set_dir | 800e6de8,1e0 |
| evt_party_set_dir | 800eb21c,1dc |
| sysMsec2Frame | 800059cc,28 |
| revise360 | 80096564 |
| toMovedirSimple | 800961f8,cc |
| partyGetPtr | 800bf1b4,20 |

Actual binary32 constants independently reread from the ISO:80421b1c=270,
80421b44=360,80421d5c=270. The Mario body loads r2-22820 at800e6e60,
6eac,6eb8,6f70; all four loads address80421b1c. Its subtract360 at6e7c
and6ec4 loads80421b44. Party loads270 from80421d5c at800eb2ec,3xx
negative/zero-duration branches. These are directly read static bytes, not guesses
from displayed facing names.

Mario1ac is current display,1b0 displaytarget,1a0 retained facing,1a4 travel
heading,19c current camera/input offset. Native MarioPresentation.display,
target,facing represent1ac/1b0/1a0 respectively; FieldMovement retains travel
as an XZ vector and has headingOffsetDegrees for19c. Ordinary
MarioPresentation::turn is a separate damped display owner, not this event's
interpolator. Source event writes both current and target on every turn visit.

## Mario first/poll and interpolation

Every invocation captures Mario pointer, resolves requestedYawFLOAT,
durationINT, converts duration through sysMsec2Frame, then resolves modeINT.
sysMsec2Frame uses signed lowword duration*FPS divided by1000 towardzero.
For200ms atFPS60, N=12. Duration is not an asynchronous walltime deadline.

On firstCall only, set eventcounter+78=0,finish+80=0, snapshot unmodified
Mario1ac into event+7c. If requestedYaw>270, subtract360 from that invocation's
local requestedYaw; this adjustment is not stored back into the script operand.
On later invocations the angle is freshly resolved without this first-only step.

If finish==1, write Mario1a4=0,1a0=0 and return2 before movement.
For N>0 compute T=revise360(270-requestedYaw); if T>270, subtract360.
Do not recenter the retained initial display or choose a shortest arc.
Let I=retained initial,c=retainedcounter. With separate single-result
operations, calculate q=float(c)/float(N),d=T-I,p=d*q,R=revise360(I+p).
Original instructions are fdivs,fsubs,fmuls,fadds, not a fused interpolation.
Write1ac=R,copy that to1b0,call toMovedirSimple(1ac),write result1a4 then copy1a0.

Increment c. If c<=N return0. Otherwise mode0 returns1; nonzero mode sets
finish=1 and returns0, allowing next invocation's explicit cleanup/return2.
N<=0 instead writes revise360(270-requestedYaw) to1ac/1b0, derives1a4/1a0,
and returns2. PositiveN includes visits c0 throughN, preserving exact start
and endpoint requests. For board(270,200,0), endpoint display0 and13 turn
invocations atFPS60; final handler result is1, not2.

## Party selector, absence and retained member

First call reads operand0 **raw**, without evtGetValue. Raw0 reads global
partyCtrlNo8041ead0; any raw nonzero uses literalindex1. Store the selected
ID into event+78 and call partyGetPtr(ID). Null immediately returns2 without
angle resolution, interpolation or invented actor creation. If present,
snapshot P10c into event+7c and clear counter+80.

Each continuation skips operand0, resolves requestedYawFLOAT and durationINT.
Nonnegative duration uses sysMsec2Frame; negative stays negative. It relooks
up the SAME retained memberID before writing; disappearance returns2. It does
not reselect the currently controlled member during interpolation.

N>0 target T=revise360(270-requestedYaw), with no Mario recenter branch.
Use the same separately rounded I+(T-I)*(counter/N), then revise360.
Write P10c,copy P110,call toMovedirSimple(P110),write P100. Incrementcounter;
return0 while incrementedcounter<=N, else1. NegativeN writes only P110 and
derived P100, preserving P10c, then returns2. N0 writes both P10c/P110 and
derived P100,returns2. Board selector0/1,yaw270,duration200 therefore end at
display0 for each PRESENT member; absent members genuinely return2.

partyGetPtr returns null for signednegativeID; nonnegative reads pointertable
8041ead8+4*ID with no source upper-bound check. Native's finite0/1 domain
must be explicit. Do not substitute marioGetPartyId/FieldPartyRegistry.current:
that helper inspects Mario owner-slot bytes and actor flags and does not read
the distinct partyCtrlNo global.

## Fresh boot current-global evidence, not a checkpoint reconstruction

Fresh __start80005240/15c calls __init_data8000542c/c0 at80005258.
The latter's80005498..cc walks pairs at800055b0 until length0 and requests
memset(start,0,length). Actual original pairs are:

| Table address | Start,length |
| --- | --- |
| 800055b0 | 803c5360,4fbb0 |
| 800055b8 | 8041e580,ea4 |
| 800055c0 | 80429500,40c |
| 800055c8 | 0,0 |

Thus original freshBOOT partyCtrlNo8041ead0 is0, **not-1**. partyInit800bd6ac
clears only the two pointertable words8041ead8/8 and does not reset the index.
Known Entry/destruction helpers do not locally publish this global either.
Source freshboot0 plus null slot0 legitimately makes selector0 complete2.
This does not establish the current value after arbitrary checkpoint/story
execution. Existing direct-r13 store census excludes aliases,REL and bulk
writes; later writer coverage remains open. A native explicit freshboot view
can start0; an unknown imported current-global state must not be called-1 or
aliased to marioGetPartyId merely to acknowledge a turn.

## Derived heading, cue identity and carrier phase

[toMovedirSimple](../camera/FIELD_SIMPLE_CAMERA_DIRECTION.md) captures fresh
global Mario19c and computes X=single(single(270-D)+19c), signed-half-biased
J=fctiwz(fma(1000,X,X>=0?0.5:-0.5)),Q=single(float(J)/1000),then repeated360
wraps. It has no paperflag override. For finite native math,
fieldPartyHeading(D,freshHeadingOffset,0,unusedCurrent) supplies the same
scoped policy. MarioPresentation.moveHeading applies it to its own display
and storedoffset, so an explicitly fresh19c snapshot is required.

The board requests all three turns as INLINE scripts, then starts a blocking
lecture child. Do not await completion of those independent turns before
starting the lecture. Keep each event's retained counter/initial/member;
advance on simulation callback visits. Handler0 yields retaining USER_FUNC;
handler1 clears opcode and yields this visit;handler2 clears and continues
immediately. A carrier accepting only0/2 cannot reproduce the used final1.

Selection requests0x20005/move,0x20012/confirm,0x20013/cancel; menu requests
ordinal5/18/19/42/43. Original psndSFXOn800d98bc forwards numeric keys;
__psndSFXOn masks LOW13bits for the original row. Thus encoded selection
keys and corresponding menu ordinals select the same5/18/19 rows, while
retaining original requestedkeys/source slot semantics. They are not MusyX
FX/macro/sample IDs. See [menu bindings](../audio/TROUBLE_CENTER_MENU_SFX_BINDINGS.md).
Existing native eight-primary-clip playback is not a general original cue
allocator or complete macro backend; actual row/resource readiness and cue
request owner remain required. No generated tone or genericconfirm fallback.

## Evidence and limits

Ignored fresh listings: artifacts/trouble-mario-direction.txt,
trouble-party-direction.txt,trouble-party-bootdata.txt,trouble-party-bootstart.txt.
Complete body SHA256 Mario
b9a9fa1cd299647ac3a8f3d1b17439dd10adc2bea28df01eeea522dc551ecc1a;
party87d173d201a73f4355b91670289a9d74399dd588f8700576cbb459bc4270747e;
__init_data d914cfeeba969b19278cdc6b151829c1683d7852fb7cf568770d450c2eaaa8bb;
__start b00f6c9d8e761a4b3696c782578b4b104ce2987b2d3eac5303833d00ed9b6989.
No original execution, later-global writer closure, bit-identical ARM/Gekko
numerics, native actor construction, audible macro parity or gameplay capture
is established by this source inspection.
