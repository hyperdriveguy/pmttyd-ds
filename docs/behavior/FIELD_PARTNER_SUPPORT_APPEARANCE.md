# Named-support partner appearance-position search

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partyGetAppearPos5 at0x800c5794,size0x258. Its actual used caller is the
[ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md),after the exact named-
support classifier succeeds. Inputs are(P,hit H,output V);return is integer0/1.
No original trajectory or scene outcome is inferred from this static contract.

## Admission and exact angle table

Copy twelve signed integer angle offsets from0x802cc564 to local storage:
0,+30,-30,+60,-60,+90,-90,+120,-120,+150,-150,180,in that order. Then return0
without output writes if H is null or hitGetAttr(H)&0x80000000 is clear.
Otherwise request mobjHitObjPtrToPtr(H) and retain object O. There is no O-null
guard before reading O+38/+3c/+40 as its position. This owner is the hit's map
object,not P+160 or global Mario.

Initialize a local origin S=(O.x,R(50+O.y),O.z). The angle-loop cursor starts0.
For each offset,read current P+100,convert the signed offset to float with the
original bias double at0x802cc5b0,and request sincosf(R(P+100+offset)). Let its
returned components be u=sin and v=-cos. No heading wrap or radius clamp occurs.

## Horizontal obstruction followed by a downward probe

Reset local length to50 and request
[partyHitCheck](FIELD_PARTNER_VECTOR_RAY_WRAPPER.md) with origin S,
direction(u,0,v),separate local point/normal outputs and the length pointer.
If any hit is returned,advance to the next angle immediately. There is no
attribute exemption for this first ray and no output-position write.

Only after a first-ray miss,set length150,replace S.x=F(u,50,current O.x)
and S.z=F(v,50,current O.z),and use downward direction(0,-1,0) for a second
partyHitCheck. S.y remains the original elevated owner Y. A null second result
ACCEPTS this sampled S. A nonnull second result accepts only when its
hitGetAttr result has0x200 CLEAR;with0x200 set,advance to the next angle.

S.x/z are NOT restored to O.x/z when advancing after a second-ray rejection.
The next first/horizontal probe therefore begins at the previous second
sample,with the newly requested heading. Advancing after a first-ray hit keeps
that current S too. The second endpoint calculation itself rereads O.x/z and
uses owner coordinates,not S plus another displacement. Do not make every
horizontal probe originate at the object center or accumulate the second
endpoint from the previous sample.

The query wrapper publishes a quantized hit point and distance, but this
appearance routine does not use that point to fill V. Accept S even when the
second ray missed;do not require a floor hit that the original did not require.
All ray filter selection remains per-call through the recovered wrapper.

## Return and actual caller consumption

On acceptance before twelve angles are exhausted,write S.x/S.y/S.z to V
in that order and return1. After twelve rejected angles,return0 without writing
V. Null/high-bit admission failures likewise preserve V. The body does not move
P's root or change motion. Input/output/owner pointers have no additional guards.

Ground passes V=&P+94 and IGNORES the integer return. It then ORs0x00400000
into P+8,copies current root Y from P+5c into P+98,and requests partyChgMot(P,2).
Thus even on appearance-search failure,the caller still marks/requests motion2
and overwrites the prepared Y while P+94/P+9c retain their prior values. On
success those X/Z values are sampled endpoints,but the returned elevated Y is
replaced. Do not gate this caller transition on the search's success or infer
that the resulting destination is the floor intersection point.

tools/verify_field_partner_recovery.py pins complete appearance/ray/reset/projection/
control leaves,selected backend regions,exact table words/vector templates and
actual ground call. Finite deduced cases include null/nonobject hit preserving
V,first horizontal hit skipping the downward ray,null downward accepting,
attr0x200 downward rejection followed by the next horizontal origin at the
previous sample,twelve rejects preserving V,and caller motion2 even on failure.
These are implementation fixtures,not original executions.

Remaining:object/hit lifetimes and whitelist/caller domains,all motion2 destination
consumers,full scene-ray/traversal/numeric services and original named-support
trajectories. Full field/game documentation and requirements remain incomplete.
