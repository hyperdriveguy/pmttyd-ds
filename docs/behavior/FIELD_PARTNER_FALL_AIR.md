# Partner fall: entry, query-before-integration and air return

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
party_motion_fall0x8014cbb8,size0x39c. This file closes entry/null-support/
exceptional-air branches;[landing](FIELD_PARTNER_FALL_LANDING.md) closes the
nonnull-support branches. [Facing leaves](FIELD_PARTNER_FALL_FACING.md) specify
called conversion math. Full placement/pose/source/math services and original
trajectories remain separate. One update here is an admitted handler call,
not a measured rendering frame or a complete field simulation cadence.

## Dispatch,entry marker and history request

[walkMain](FIELD_PARTNER_WALK_DISPATCH.md) dispatches unsigned substate P+36==3
to this body for selectors0/1/2/3/6. Those actual calls ignore the integer
result. This body returns2 on its air paths and3 on admitted landing paths;
other callers must preserve their own result usage. Capture owner M=P+160 at
entry. No local phase+38 switch or count+24 decrement controls this body.

If P+0&8 is CLEAR,request partyGetFootmarkId(P) then partyNextReadId(q)
BEFORE entry-marker handling. The complete [index leaves](FIELD_PARTNER_JUMP_DESTINATION.md)
do not merge this request with a prior jump/stay advance. If P+8 mask2 is set,
consume it,request partyChgPoseId(P,4),then clear P+0 mask0x00080000. Do not
reset velocity/acceleration/speed just because this marker was consumed.

## Floor first,using old velocity

Start local G=null. If P+0&0x02000000 is CLEAR,request
[partySearchGround](FIELD_PARTNER_FLOOR_QUERY.md) with depth f1=10 and
dropLimit f2=abs(current P+114). This happens BEFORE velocity/root integration,
without a rising/descending sign test. If that flag is set,skip the call itself;
cached query fields are not refreshed by this handler. The called query also
owns its own early flag-return behavior.

Nonnull G with attr0x200 is discarded unconditionally. Latch P+8 mask0x01000000
and P+0 mask0x02000000 ONLY when unsigned mobility P+32==0 and signed byte
P+4c==0. Nonzero mobility or nonzero byte suppresses the latches,not discarding.
There is no immediate substate3 request/acceleration-1 exit here as in jump's
excluded-surface path. Store retained G or null at P+138,then branch to landing
when nonnull. Null proceeds below.

## Null support: conditional default,velocity clamp and speed damping

Clear P+0 mask2. If P+0 mask0x20 is CLEAR,OR0x20 into it and inspect
acceleration+11c. Only when acceleration compares equal to0,store original
binary32 -1.4 at+11c. If flag0x20 was already set,skip BOTH that flag store
and default-acceleration test. Existing nonzero/NaN acceleration is retained.
This is not an unconditional gravity assignment.

Set velocity+114=R(oldVelocity+acceleration),then replace it with-16 only for
ordered newVelocity<-16. Add the resulting velocity to rootY with another
single-result add. NaN ordered comparisons do not clamp under nontrapping
continuation. The -16 constant is an original local lower bound,not a measured
terminal-speed statement across all field modes.

Request marioKeyOffChk. Return0 from that signed-byte leaf admits horizontal
damping:write speed+104=R(speed+original-.2),then clamp to0 only for ordered
result<0. Nonzero key-off preserves speed. Input/force/control producers remain
external;do not equate this check with the current native pad being idle.

## Root below -2000 and placement4 request

After integration/damping,if ordered rootY<-2000,store rootY=-2001 FIRST.
If captured owner motion M+2e is31 or32,return2 immediately. This leaves the
new velocity/acceleration/speed and -2001 root in place,without placement,
motion change or horizontal tail.

Otherwise clear P+0 masks0x70,store velocity0 and acceleration0,request angle
from current root XZ to M.xz and store+100. Compute facing+54 from
revise360(R(180+toMovedir(M+1b0))) through the
[complete local leaves](FIELD_PARTNER_FALL_FACING.md). Then request
partyGetAppearPos4(P,&P+58) at0x800c59ec,IGNORE its return,add37 to the current
rootY after its writes,and request partyChgMot(P,0). Clear P+0 mask0x02000000
after that request. The full placement4 search/output contract remains open;
do not substitute captured owner XYZ or assume a success-only write. The root
buffer already has values,including the prior -2001 store.

Continue into the horizontal tail even after this recovery request. Exact
rootY=-2000 does not enter the strict exceptional branch. P+138 was already
stored null before recovery;the body does not invent a replacement contact.

## Horizontal tail and return2

If speed+104 compares equal to0,skip movement and return2. Otherwise copy
current root XYZ to local V,request
[partySearchFrontWall](FIELD_PARTNER_FRONT_WALL_QUERY.md)(P,&V,
speed=R(original1.3*P+104),direction=P+100). Ignore its hit-pointer return and
publish only local V.X/Z to root. There is no direct P+12c/P+134 contact store,
no surrounding pass and no ceiling query here. NaN speed does not compare
equal to0 and takes this request path under nontrapping continuation.

tools/verify_field_partner_fall.py pins the complete fall/facing and linked
index/key/landing leaves,actual dispatch calls,independent branch/request/store
anchors and exact constants. Deduced air cases include history-before-marker,
floor-before-integration using old velocity,flag20 preserving zero acceleration,
default-1.4 only when admitted,velocity-16 and speed0 boundaries,excluded-surface
mobility latch gating,owner31/32 early return,-2001 placement4 ordering and
post-recovery scaled horizontal movement. These are static fixtures,not
original falling or timing observations.

Next:placement4/pose and all fall callers/state/flag/source/numeric domains,
homing/fly handlers,full backend and original fall/recovery trajectories.
Full field/game documentation and requirements remain incomplete.
