# Partner roll gate: entry,rotation,exit interpolation and return ownership

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_roll0x8014ba6c,size0x3c0. Flying/ground homing and stay request this after
[slit](FIELD_PARTNER_SLIT_GATE.md);zero blocks the caller's later locomotion,
one admits it. This body uses presentation/heading fields,not direct rootXYZ
integration. Its phase byte P+3a/count word P+24 are shared with slit;no measured
simulation/render duration is established by their literals.

## Admission and entry marker

If unsigned P+37!=2,return1 with no local roll setup. Otherwise default return
R=1. If P+8 bit0 set,clear it,then request
[partyPaperOn](FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md)(P,`p_roll`),
partyChgPaper(P,`PM_R_1A`),partyChgPoseId(P,0). Store P+88=-6,P+8c=6,
P+7c=6,P+80=2,count24=65,phase3a=0. Request immediate speed writer(P,0).
The paper resource/animation requests precede those writes. The
[speed writer](FIELD_PARTNER_FLY_SPEED_WRITERS.md) still performs its owner
multiplications;it is not a generic direct zero assignment in every FP domain.

Read unsigned phase3a for the first phase dispatch:

- Phase0:decrement word24 with32-bit wrap,store and reread. If signed>0,no
  phase change. Otherwise request `PM_R_1B`,store phase10. No phase10 spin
  occurs from that transition in this same first dispatch.
- Phase10:store P+10c=revise360(R(270-current P+100)). Compute
  A=revise360(R(P+fc+P+10c)). If ordered A>180,compute single FMA7*speed104
  plus P+6c,wrap and store6c. Otherwise compute fnmsubs(7,speed104,P+6c),
  wrap and store6c. Ordered exact180/unordered take this latter branch.
- Other phases:no first-dispatch work.

R remains1 on these active entry/spin paths. Returning1 does not mean roll is
inactive;it means this caller gate permits subsequent locomotion.

## Exit phases100 and101

Reread phase after the first dispatch. Phase100 writes speed104=0,P+64=0,
P+6c=0,P+88=0,P+8c=0. If ordered travel P+100<180,store P+7c=3,else-3.
Store P+80=0,count24=65,phase101;request `PM_R_1C`,then partyPaperLightOff(P).
FALL THROUGH into phase101 in this same invocation;the fresh65 becomes64.
Other phases except101 skip the exit graph and return current R.

Phase101 sets R=0 BEFORE decrementing/storing/rereading count24. If signed
count>0,choose target G=90 when ordered P+100<180,otherwise270. Compute with
SEPARATE single multiply/add operations:

```
N = R(R(0.1*R(G-current P+10c))+current P+10c)
D = R(G-N)
if ordered D<1: N=G
P+10c=N
P+110=N
```

The compiled code extracts the LESS CR bit,converts boolean0/1 through the
original double bias,fabs and equality-to-zero,then conditionally selects G.
This is NOT abs(G-N)<1;negative D also snaps. Do not replace the two arithmetic
operations with FMA,normalize to shortest-arc interpolation,or impose an epsilon.
R remains0,blocking the caller's later locomotion while this count remains>0.

When count is signed<=0,set R=1. If ordered P+100>=180,set P+10c/+110=0
and P+fc/+100=270. Otherwise set P+10c/+110=180 and P+fc/+100=90.
The unordered travel domain therefore differs between earlier target selection
and this final >= test. Preserve the actual ordered predicates.

Restore P+88/+8c/+90 and P+7c/+80/+84 from six original zero words at
0x802f4670. Store P+37=0. Clear flags in this exact order:
P+0 mask0x1000;P+4 mask0x100;P+0 mask0x200;P+4 mask0x10;P+4 mask8.
Store phase3a=100,then request partyPaperOff(P). Return1. The phase100 byte is
retained even though P+37 now0 causes future roll calls to return at admission.
No root position snap,history clear or ordinary pose1 request occurs here.

## Verification and composition limits

The [slit-floor/exit bindings](FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md)
list actual shared caller sites. `tools/verify_field_partner_slit_roll.py` pins
this complete body and related local services:ten containing hashes(all complete),
482 anchors,18 constants,four spans,two US lecture imports and22 direct caller
sites across seven DOL text sections. Underlying pose/animation/math/source/
phase/alias/FP domains and original roll trajectories remain open. Static checks
are not original execution or complete paper-form equivalence.
