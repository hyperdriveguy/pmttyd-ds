# Partner direction/speed query,history reset and motion-state writes

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyGetMoveDirSpd at0x800cad1c,size0x4ec. It takes (P,&direction,&speed).
The **first pointer receives direction**,the second receives speed. This is a
stateful query:it changes histories,counter and motion/substate fields. It does
not simply return cached P+0x100/P+0x104 values.

The [stay handler](FIELD_PARTNER_STAY_MOTION.md) calls this service twice,
passing first output as its direction and second as speed to its ordered
collision requests. Reusing a single result would omit a correction invocation
and its possible state/history changes. Both call sites are pinned separately.

## Correction and conditional history reset

Capture owner M=P+0x160 and call
[partyMarioHosei(P)](FIELD_PARTNER_OVERLAP_CORRECTION.md),retaining returned
signed counter n. If n==0,or unsigned motion-selector byte P+0x3b==2,
skip history-reset branches. Otherwise use **P+0 mask8** to choose the route:

* Mask clear:seed all80 bank0 records from a fresh global Mario position,
  write each flag byte0 and reset bank0 read/write words to0.
* Mask set:first call **anotherPartyGetPtr(1)**,using literal1 rather than
  P's slot byte. Null skips reset. Nonnull Q sets reset admission only when
  nokonokoGetStatus(Q)==3 or distance(captured owner M position,Q position)
  is ordered>=150. A nonadmitted result skips the nested seed query/reset.

For admitted bank1 reset,reacquire global Mario pointer G,query marioGetPartyId
then partyGetPtr as S. Seed from G position when S is null,status(S)==3,
or distance(G position,S position) is ordered>=150;otherwise seed from S's
position at+0x58/+0x5c/+0x60. Seed80 records with flag0 and reset only
bank1 read/write words. This differs from blindly using captured owner M or
the first Q as the seed source.

Banks/record layout are specified in the
[history writer](FIELD_PARTNER_HISTORY_WRITER.md). Each reset uses ten
iterations of eight16-byte records;only flag byte and X/Y/Z words are written,
leaving record padding unchanged. No reset branch locally recomputes retained n.
Nonzero includes negative counter values;it is not a predicate n>0.

## Counter threshold and requested motion state

Next,if P+8 mask0x00200000 is set,replace local n with0. This does not
directly clear the stored P+0x52 counter. Otherwise retain the original result.
If signed n>=30,first store halfword0 at P+0x52. Then,if unsigned kind
byte P+0x31==4 **and** mode byte P+0x34==3,skip the state writes below.
That exception still has the counter-clear side effect.

For other n>=30 states:

1. Clear byte P+0x3d.
2. If the current unsigned selector P+0x3b is6,clear P+0 mask0x8000.
3. Write selector byte2 and OR4 into P+8.
4. If unsigned substate P+0x36==3,clear P+0 masks0x70,0x03000000 and
   0x04000000 through ordered load/write operations.
5. Write substate byte1 and OR2 into P+8.

This is actual state publication consumed by
[walkMain](FIELD_PARTNER_WALK_DISPATCH.md),not an immediate recursive motion
dispatch. It occurs before direction/speed calculation even when subsequent
numeric results are zero,NaN or unusable. No local driver-mode write occurs.

## Direction vector and output order

Call sincosf(P+0x100,&u,&v). The complete system wrapper at0x80005e8c,
size0x78,computes radians=R(R(binary32Pi*angle)/180),calls double sin,
stores its single-rounded result to u,then calls double cos,rounds it to
single,negates it and stores v. Its constants at0x8041f460/0x8041f464 are
0x40490fdb and180. It is not a call to a host combined sincos implementation.

With s=float P+0x104,construct:

```
baseX = R(P+0xac + P+0xa0)
baseZ = R(P+0xb4 + P+0xa8)
X = F(s,u,baseX)
Z = F(s,v,baseZ)
direction = angleABf(0,0,X,Z)
```

R is single-precision rounding;F is single-precision fused multiply-add.
P+0xa0/+0xa8 include this query's overlap correction. The Y components
at+0xa4/+0xb0 are not used. Store direction through the first pointer before
performing the [norm/speed arithmetic](FIELD_PARTNER_SPEED_NUMERICS.md).
Pointer aliasing must preserve that order;the direction store can affect a
later field read if the caller aliases an object field.

Select final factor from a **later read** of P+0 mask0x80:0.75 when set,
otherwise1. Compute q=F(X,X,R(Z*Z)),then the original square-root path,
and store R(factor*root) through the second pointer. This final factor is
a separate stage from the same factor used by
[partyMoveWalk's target computation](FIELD_PARTNER_WALK_CALLBACK.md);
do not remove one occurrence as presumed duplicate scaling.

## Verification and open composition

tools/verify_field_partner_direction.py checks three complete bodies,64
instruction anchors,19 constants (including double refinement values and
canonical NaN word),and two actual lecture imports. The two stay call sites
and output stores are independent anchors. Metadata reports and original
corrected Gekko ELF/listings remain ignored.

Next:angleABf,double sin/cos and Gekko estimate semantics;source-query/status/
distance services,history/owner/flag/counter domains and terrain callers;
original overlap/stop/run/following trajectories and field/battle teardown.
These local closures do not complete all partner movement or the game.
