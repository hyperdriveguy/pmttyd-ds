# Partner fall landing: two flag-dependent cleanup paths

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
nonnull-support region0x8014ce3c–cf34 inside party_motion_fall. Entry,query
admission and air/exception branches are in
[fall air contract](FIELD_PARTNER_FALL_AIR.md). No runtime landing trace or
source/numeric/terrain equivalence is claimed from this static graph.

Before this region,the handler has retained a nonnull floor G after its
attr0x200 exclusion and stored G at P+138. There is no additional slope/height
admission here;the called [floor query](FIELD_PARTNER_FLOOR_QUERY.md) supplies
current P+e4. This landing happens BEFORE this body's velocity integration,
even when the entry velocity was positive.

## Common stores and motion selection

Test current P+0&0x20,then take its distinct cleanup path below. Both paths:

1. Clear P+0 masks0x70,then set mask2.
2. Clear P+8 mask0x00400000.
3. Store velocity P+114=0;SNAP rootY from current P+e4.
4. Compare speed P+104 with0 using fcmpu/equality. Equal0 (either signed zero)
   requests partyChgMot(P,0);otherwise requests partyChgMot(P,1),including
   unordered under nontrapping continuation.
5. Request N_partyPlaySfxPartyLanding1(P),which requests sound0x928 at current
   P root through its complete [landing leaf](FIELD_PARTNER_JUMP_INITIALIZATION.md).

Acceleration+11c is not cleared by these local landing stores. Prepared target,
stored direction/speed and count/phase are not reset here. The motion helper's
complete old-substate3 cleanup/entry-marker semantics are in
[ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md). Since the ordinary caller
entered with substate3,that helper may add its own mask cleanup;preserve it
separately from the handler's direct stores and do not recursively run the
new motion in this body.

## Flag0x20 SET on entry to landing

Perform the common sequence directly. After landing-sound request,return3.
There is NO footprint-clear request and NO extra mask0x2000 clear in this
local branch. These effects must not be merged from the other branch simply
because both request a grounded motion.

## Flag0x20 CLEAR on entry to landing

Clear P+0 mask0x2000 BEFORE the common stores. After the common motion/sound
sequence,reread P+0&8:clear selects partyClearFootmark0x800cc7c4,set selects
partyClearFootmark2 at0x800cc64c. The complete
[clear leaves](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md) ignore their incoming
argument,query live global sources and reset bank0 or bank1 respectively.
Their writes happen after landing-sound request,not before floor query.
Return3 on either clear route. Do not turn this into clearing both banks or
selecting bank solely from q=(P+0>>2)&1.

## Differences from jump handlers and validation

Main jump's landing requests substate0 and ORs00800000 into P+8;fall instead
selects0/1 by speed equality and does not OR that marker. Jump2 selects0/1
by destination distance<5,which is also a different predicate. Fall has no
post-landing -2000 root gate:its exceptional check belongs only to the null-
support branch. It returns3 rather than the jump handlers' unspecified integer
results;actual walkMain ignores it.

The fall verifier pins complete containing bytes,flag test,extra clear,common
snap/velocity/motion/sound order,both bank requests and return3. Prior target/
footmark/jump tools check the linked leaves. Deduced cases include landing
with positive entry velocity,flag20 set vs clear,index clearing after sound,
speed±0 vs NaN,acceleration retention and root/cache/contact ownership. These
are implementation fixtures,not original observations or measured landing
timing. Full sound/pose/terrain/source domains and trajectories remain open.
