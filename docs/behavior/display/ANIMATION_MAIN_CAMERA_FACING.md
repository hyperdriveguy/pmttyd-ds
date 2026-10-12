# Animation Main optional camera-facing sign

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete animPoseMain,
local80040540..80040648. This tail follows time/cache/interpolation and their
final index/fraction stores;[entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md)
identifies null-animation early return. No observed visuals or trajectory.

Read current R+0 mask8. CLEAR:skip facing work and preserve R+78. SET:
request camGetPtr(4)0x8000f2fc;read X18−Xc and Z20−Z14 with fsubs. Negate both
and request atan2(f1=−deltaX,f2=−deltaZ)0x802725a0. Round returned value with
frsp,then fmuls constant0x42652ee4=57.29579162597656. Do not substitute an
exact180/pi constant;the original word is pinned.

First wrap uses literal single-operation loops:

1. While angle<−360 ordered,add360.
2. While angle>=360 ordered,subtract360.
3. If angle<0 ordered,add360 once.

Subtract CURRENT R+74 via fsubs. Second wrap repeats while value<−360 add360
and while value>=360 subtract360;there is NO final negative-to-positive add.
For ordinary finite values this keeps differences in[−360,360),rather than
canonical unsigned[0,360). NaN falls through ordered tests;infinity can loop
forever. No local validity clamp,iteration cap or revise360 helper request.

Publish float−1 R+78 iff either:

- Difference>90 AND difference<=270.
- Difference<−90 AND difference>=−270.

Otherwise publish+1. Boundaries90/−90 give+1;270/−270 give−1. The two
interval tests are ordered,including explicitly ORed equality condition bits.
NaN yields+1 on this local branch graph. It does not read partner P+10c or
rewrite animation timeline/material flags. Downstream interpretation of R+78
as geometry/material orientation remains a renderer dependency.

`verify_animation_main.py` pins the full containing Main body and actual
constants,camera/atan2 request operands and publication. Camera data source,
libm/FPSCR,animation-record flag/angle writers and original visible facing
remain separate. No helper name or inequality is substituted for this tail.
