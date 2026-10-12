# Animation local-time writes and literal clock requests

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animPoseSetLocalTime0x8004130c,size0x120 and
animPoseSetLocalTimeRate0x8004142c,size0x18;timestamp paths also fully read in
[selector/restart](ANIMATION_SELECTOR_AND_RESTART.md) and
[paper-group binding](ANIMATION_PAPER_GROUP_REUSE.md). Original runtime,
clock cadence and evaluation remain open. No rendering frame rate is measured.

## Shared timestamp request

Each selector/group/local-time function reads CURRENT pose R+c. If zero,read
global game pointer through0x80415fd0 and its64-bit pair HIGH+40/LOW+44;
otherwise read HIGH+38/LOW+3c. These are literal branches,not proof that all
mode values mean a specific game/battle clock. Independently read hardware word
0x800000f8;compute unsigned:

A=word[0x800000f8]>>2
D=high32(0x10624dd3*A)>>6
Q=__div2i(numeratorHIGH,numeratorLOW,0,D) at0x80264ad8.

The arithmetic uses `mulhwu`,not a float reciprocal or direct division by a
rounded platform frequency. Q is the helper's64-bit return HIGH r3/LOW r4.
Clock/hardware words are live inputs,not pinned initial values. Divisor0,
helper signed arithmetic,overflow,platform clock/pause publication and original
numeric behavior remain separate dependencies. Do not convert this contract
into a guessed monotonic host-clock policy. All timestamp stores publish LOW
before HIGH;reentrant observers can distinguish that order.

Ordinary forced/changed selection stores Q to R+18/+1c;paper selection and
nonnull group binding store Q to R+88/+8c. The same helper request is shared,
but the target timestamp pair differs.

## Explicit local time

Input ID r3,float T f1. Load binary32 factor16.66666603088379,
word0x41855555 at0x8041fb40. Compute SINGLE fmuls U=round32(factor*T).
This happens before the original conversion call;do not replace with a double
multiply or an exact rational50/3. Resolve current pose R using stride0x170.
Request __cvt_dbl_usll(U)0x80264fc4,retaining returned64-bit K high/low.
Then request Q by the clock branch above,using CURRENT R+c after conversion.
No local negative/NaN/infinite/range clamp exists;helper behavior needs recovery.

Write timestamp Q−K modulo2^64:subtract LOW with `subc`,store LOW R+1c,
subtract HIGH with `subfe` carry/borrow,store HIGH R+18. Reread those fields;
copy LOW to R+8c BEFORE HIGH to R+88. This explicitly synchronizes ordinary
and paper timestamps to the SAME adjusted pair,not independent counters.
It does not reset index+14/+94,cache fields+3c/+40/+80/+84 or rate+44.

Reload signed linked ID R+90. If−1,finish. Otherwise reload CURRENT work/pool
and linked record L;word L+4 mask2 SET suppresses propagation. Mask2 CLEAR:
reread current R+18/+1c and copy LOW L+1c BEFORE HIGH L+18. Do NOT write
linked+88/+8c or call Main here. If L aliases R,the literal read/store ordering
still applies;no local distinct-record guard exists.

The used [partner pre-display](FIELD_PARTNER_PRE_DISPLAY.md) converts unsigned
halfword P+20 to float and calls this setter AFTER ordinary and paper selectors.
Their separate reset timestamps are thus overwritten by the adjusted common
pair on that path. Exact clock/helper behavior is required for equivalent frame
sampling;storing the incoming frame directly loses the original state contract.

## Rate setter and used bindings

animPoseSetLocalTimeRate(ID,rate) writes incoming f1 as binary32 at CURRENT
poolBase+0x170*ID+44. No ID/active/finite/range checks,timestamp update or linked
propagation occurs. Zero,negative or NaN are not locally normalized. The
[partner draw](FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md) calls it AFTER both
draw requests at800b7a80 with0.75 or1.0. Those requests do not prove when Main
consumes the rate or that the current draw already used the new value.

Exact seven-text-section census pins40 local-time and13 rate references,
including partner800b7b54 and800b7a80,Mario,NPC,event/menu/battle/effect sites.
Census is static,not runtime frequency or full US reachability. Verify through
`python3 tools/verify_animation_selection_time.py`:five complete bodies,
423 instructions,two binary32 constants,two initial global pointer spans,two
lecture imports and299 direct sites. Pointer spans identify original initial
work globals only;live contents/platform timestamps remain unmeasured.

Review cases:T0 still resets timestamps;T nonzero computes single-scaled U
before conversion;borrow crosses high word;linked−1 skips;linked flag2 skips;
linked alias retains ordered writes;rate setter touches only+44. These are
source-derived acceptance requirements,not original execution results.
