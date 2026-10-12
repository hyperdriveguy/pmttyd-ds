# Animation selection and restart publication

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animPoseSetAnim0x80041170,size0x17c and animPoseSetPaperAnim0x80040ca4,
size0x160. These are actual services called by
[partner pre-display](../partner/FIELD_PARTNER_PRE_DISPLAY.md),not inferred interfaces.
No original instructions were executed. Resource validity and downstream
animation evaluation remain separate.

## Record/resource addressing and selector encoding

The animation work pointer W is loaded through0x80415dc0 (r13−0x7160).
Pose pool base is W+10;record R=base+0x170*inputID,using low32-bit arithmetic.
No local ID range,active-bit,pool-null or pointer validity check exists.
For ordinary selection,resource index is R+10. Resource lookup is:

Load J=word[W+0],E=word[J+16*index+8],H=word[E+a0],D=word[H].
D+148 supplies signed animation count N;D+1ac supplies the name array base.
Each name slot is64 bytes and strcmp consumes its address directly.
These offsets describe the original loaded layout,not a replacement file format.

Selector S is the raw second integer argument. The `mr.` instruction makes a
SIGNED comparison with zero. If S<0 (the ordinary0x80xxxxxx string-pointer
domain),scan i=0 while signed i<N;compare strcmp(D.nameBase+64*i,S).
First equal selects i;exhaustion selects0. If S>=0,it is interpreted directly as
an index:retain S when signed S<N,otherwise select0. Null therefore means
numeric index0,not a null-name early return. A positive address is interpreted
as an integer index;do not convert this into a generic pointer/nonpointer test.
Negative values enter string lookup even if invalid pointers. N<=0 still leads
to selected0 without proving an existing row0. No missing-name error is returned.

## Ordinary selector and forced restart

animPoseSetAnim(ID,S,force) captures R and D before scanning. If force==0 AND
selected index equals CURRENT R+14,return without timestamp/cache changes.
Otherwise:

1. Store index R+14.
2. If signed R+90 !=−1 AND R+0 mask2 set,OR mask4 into R+0.
   This does not validate R+90 or inspect the linked record.
3. Obtain the timestamp quotient described in
   [local-time contract](ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md).
4. Store quotient LOW word R+1c BEFORE HIGH word R+18.
5. Store signed−1 R+3c;positive float0 R+40;word0 R+80;float0 R+84.

The used partner path passes force1 whenever either pending name/paper marker
is admitted. Thus an equal animation name can restart even if only the paper
marker changed. It is incorrect to suppress that forced restart by comparing
names in this backend. The setter does not call animPoseMain,increment a frame,
write the paper name pointer or set local time rate R+44.

## Paper animation selector

animPoseSetPaperAnim(ID,S,thirdArgument) resolves the resource using LINKED
pose ID R+90:linked record's resource index+10,then the same W resource chain.
This dereference occurs BEFORE any−1 guard. Invalid/missing linked ID is not
turned into an ordinary-pose fallback by this function. Selector encoding and
name search are identical to the signed rule above. The incoming third integer
argument is overwritten;there is NO force/same-index skip here.

Always store selected index R+94,then timestamp LOW R+8c and HIGH R+88.
Reload linked ID after timestamp;only if !=−1 reload CURRENT work/pool and linked
record. If its word+4 mask2 is CLEAR,call animPoseMain(linkedID)0x8003ed2c.
Mask2 SET skips that call. This admission uses linked+4,whereas the ordinary
restart's mask2 reads the current record+0. Do not merge the two flags.
No ordinary+14/+18 timestamp/cache reset or paper-group search occurs locally.
Main's callback/evaluation/resource effects and reentrancy remain open.

## Binding and implementation checks

Direct DOL census pins222 ordinary-selector and4 paper-selector sites across
all seven text sections. This establishes binary references,not runtime counts
or that every site is reached in US play. Partner calls are800b7b10 and800b7b28;
Mario equivalents80057944 and8005795c. Other paper sites are
NPC event80050348 and message playback80084504. Their caller scheduling and
resource lifetime are not completed by this leaf recovery.

Review cases:negative missing string→0;null→numeric0;signed count0;force0 same
index retains caches;force1 same index resets;paper third argument0/1 has the
same local behavior;paper linked−1 is an unsupported pre-dereference domain,
not a clean success path. These are static acceptance cases,not executed tests.
`python3 tools/verify_animation_selection_time.py` pins all five setter bodies,
423 reviewed instruction words,two constants,two initial global-pointer spans,
two US lecture imports and299 direct DOL sites. Initial pointer hashes do not
prove live state. Numeric helper/resource/Main/source/runtime behavior stays open.
