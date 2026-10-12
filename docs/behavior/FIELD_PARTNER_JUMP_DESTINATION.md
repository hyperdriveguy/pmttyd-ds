# Motion2 jump dispatch and main-handler destination preparation

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
party_motion_jump0x8014d2f0,size0x79c,and party_motion_jump2 at0x8014cf54,
size0x39c. This file covers main jump's complete phase0 destination graph
and three index leaves;[initialization](FIELD_PARTNER_JUMP_INITIALIZATION.md)
and [flight/landing](FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md) close the
remaining local branches. Called target/history/terrain/pose/audio engines
and original trajectories remain separate evidence requirements.

## Actual selection and independent fields

[walkMain](FIELD_PARTNER_WALK_DISPATCH.md) uses unsigned P+36==2 as motion2.
Selector P+3b==1 dispatches main jump;selectors2/3/6 dispatch jump2. The
named-support [ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md) prepares
P+94/+98/+9c,set P+8&0x00400000 and requests motion2. That does not establish
which jump handler runs without the selector. Driver mode P+34,substate P+36
and internal phase P+38 are different owners.

P root is+58/+5c/+60;prepared target is+94/+98/+9c. Float+100 is direction,
+104 is horizontal speed,+114 vertical velocity,+11c acceleration,+128 stored
horizontal distance;word+24 is an initialization count. Signed halfword+28
participates in main destination/speed ownership. Capture M=P+160 at handler
entry;do not replace all later owner rereads with that pointer automatically.

Main jump first requests q=partyGetFootmarkId(P),the recovered(P+0>>2)&1.
If P+8 mask2 is set,clear it and set phase+38=0. Then dispatch exactly phase0
or10;all other unsigned phases return. Jump2 entry and phases differ as
specified in the companion files.

## Main phase0 target tree

Start local T from current P+94/+98/+9c. Apply this priority order:

1. If signed P+28!=0 OR unsigned driver P+34==9,replace T with root XYZ,
   move local X/Z by current P+104/P+100 using movePos,and publish T to
   P+94/+98/+9c. Skip the following history branches. The later speed/direction
   override still tests P+28 alone,so driver9 does not imply retained speed.
2. Otherwise if P+8&0x00400000 is set,keep the prepared target untouched.
   This is the used named-support destination-preservation path.
3. Otherwise query writeId(q) then readId(q). Equal indices choose the
   empty-history route below;unequal indices choose the history route.

### Equal-index route

Request partyGetTargetDist(q,P.x,P.z). If ordered distance>R(2*M+1b8),request
partyGetTargetDir(q,P.x,P.z),retain its angle,then getFrontFloor(P,&localFloor)
at0x8014b638. Add10 to that output Y,put this Y in T,reset T.X/Z to current
P root and move by R(original .6*P+f4) along the retained angle. There is no
local check of getFrontFloor's return before reading its output;the full
called output/domain contract remains open. If the distance condition did not
admit,retain the originally copied T. Publish T to P+94/+98/+9c on either
outcome,then go to common initialization.

### Unequal-index history route

Request partyGetReadId(q,&T.x,&T.y,&T.z),whose complete raw-coordinate reader
is in [history contracts](FIELD_PARTNER_HISTORY_WRITER.md). Compute planar
D=distABf(root XZ,T XZ),and cap C initially50. Let H=R(original1.4*abs(R(T.y-P.y))).
If ordered50>H,set C=H;otherwise keep50. Request angleABf(root XZ,T XZ),then
sincosf of that result. Preserve the separate helper request and FP ordering.

* For ordered D>C,start travel L=C,request partyGetTargetDistY(q),and replace
  L with0 only when its result compares equal to0. Construct new T.X/Z from
  ROOT X/Z plus fused L*u/L*v,and set T.Y=R(history T.y+10).
* Otherwise start travel L=D,request partyGetTargetDistY(q),and replace L
  with0 only for equal0. Construct new T.X/Z from HISTORY TARGET X/Z plus
  fused L*u/L*v;retain history T.Y. This adds displacement from the target,
  not from root. Do not silently turn it into a conventional clamped approach.

Publish the new T to P+94/+98/+9c. If ordered L<C,request partyNextReadId(q)
before common initialization. The Y-query result is used only as an equality
gate here;its complete live-target selection remains a called dependency.
NaN does not compare equal to0. Ordered comparisons preserve their actual
unordered branches under nontrapping continuation.

## Complete index leaves

partyGetFootmarkWriteId0x800cd1d4,size0x10 loads the word at write bank base
SDA+1bd0 plus4*q. partyGetFootmarkReadId0x800cd1e4,size0x10 does the same at
read base SDA+1bc8. Neither clamps/classifies q or changes an index. The
actual main handler's q producer supplies0/1;other argument domains remain
external. Bases are0x8041eaf0 and0x8041eae8,as already recovered.

partyNextReadId0x800cc920,size0x3c loads read and write words at those indexed
addresses. Equality returns without a store. Otherwise increment read with
the original32-bit add/store,reread it and compare signed value with80. If
signed value<80,return;otherwise store0. This is not an unconditional modulo,
not a lower-bound clamp,and not an increment of the write index. Negative/
overflow/index/array domains remain external. Main jump may request this leaf
in destination selection,again at init exit and once per phase10 call;do not
collapse those requests into one lifetime advance.

The verifier pins complete main/jump2 bodies and the three leaves,selected
destination/index predicates,request order and target stores. Useful deduced
cases include marker reset vs phase dispatch,signed counter/driver9 precedence,
preserved00400000 target,equal-index strict distance test,zero Y-query travel,
root-based long route vs target-based short route,and read/write equality/wrap80.
These are static implementation cases,not original gameplay observations.

Next:complete target-dir/Y/front-floor services and index/target writers,
all jump callers and numeric/source domains,plus original destination traces.
Full field/game documentation and requirements remain incomplete.
