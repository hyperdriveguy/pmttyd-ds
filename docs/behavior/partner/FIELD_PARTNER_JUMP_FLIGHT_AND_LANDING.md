# Partner jump flight, landing and exceptional root ownership

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
party_motion_jump0x8014d2f0 and party_motion_jump2 at0x8014cf54. Actual
[dispatch/targets](FIELD_PARTNER_JUMP_DESTINATION.md) and
[initialization](FIELD_PARTNER_JUMP_INITIALIZATION.md) are separate files.
Each update described here is one admitted handler call,not a measured frame
duration. No original trajectory trace or complete called-service equivalence
is claimed.

## Main phase10: advance history,integrate Y,then exceptional floor loss

If P+0&8 is CLEAR,request fresh footmark ID(P) and partyNextReadId(q).
Then write velocity P+114=R(P+114+P+11c),and rootY P+5c=R(rootY+newVelocity).
The velocity update precedes displacement. No word+24 decrement occurs.

If ordered new rootY<-2000,clear P+0 mask0x10,store velocity0,request angle
from current rootXZ to captured entry owner M.x/z and store P+100. Copy M.xyz
to root and request partyChgMot(P,0),then RETURN. This gate runs BEFORE any
floor query on main phase10. Acceleration/speed/destination are not cleared
by these direct stores. Exact -2000 does not trigger the strict gate.

Otherwise test ordered velocity<=0. If true,clear P+0 mask0x10,OR0x20 into
P+0 and request [partySearchGround](FIELD_PARTNER_FLOOR_QUERY.md) with
depth=abs(velocity),dropLimit=0. Preserve these actual parameters;the prepared
target Y and word+24 are not the landing condition.

For a nonnull floor G with attr0x200 SET:if signed byte P+4c==0,OR0x01000000
into P+8 and0x02000000 into P+0. Regardless of that byte,store contact P+138=0,
request partyChgMot(P,3),store acceleration-1 and RETURN. This excludes G even
when the latch is suppressed. It does not call the landing sound.

For all other floor outcomes,store G (possibly null) at P+138 and snap rootY
to current P+e4 BEFORE testing whether G is null. A null-floor initialization
or stale-output early return therefore affects the root according to the full
floor-query cache contract. If G is nonnull,set P+0 mask2,clear P+8
mask0x00400000,clear P+0 masks0x70,request landing sound0x928,store velocity0,
request partyChgMot(P,0),OR0x00800000 into P+8,and RETURN. This main landing
always requests substate0,not a destination-distance-dependent substate.

## Main horizontal tail and special scratch branch

When rising or when the descending floor result is null,continue to the
horizontal tail. If P+0&0x01000000 is set,request movePos with current speed/
direction and local stack X/Z at+1c/+24,then RETURN. This branch does not
initialize those scratch coordinates in this handler and does not copy them
to root. Do not invent root movement by treating this as a front-wall bypass;
the uninitialized scratch arithmetic/numeric domain remains explicit.

Otherwise copy current root XYZ to local V and request
[partySearchFrontWall](FIELD_PARTNER_FRONT_WALL_QUERY.md)(P,&V,speed,direction).
Nonnull H overwrites P+12c;when speed is not equal to0,it also overwrites P+134.
Null H preserves those old contact fields. Then publish local V.X/Z to root;
do not publish V.Y. There is no second surrounding-wall or ceiling query in
this flight tail. Floor work precedes this horizontal publication in main jump.

## Jump2 phase dispatch and movement-before-floor ordering

Jump2 unsigned phase0 only stores phase1 and reaches the final -2000 gate.
Phase1 performs the update below. Other unsigned phases skip integration and
also reach that final gate. Entry-marker initialization is described separately.
Jump2 does not advance footprint read indices in its local body.

On phase1,integrate velocity then rootY with the same ordered single-result
adds as main. Copy root to local V,request front-wall(speed,direction),and
publish V.X/Z BEFORE descending floor work. The front-wall return is ignored;
jump2 does not directly publish P+12c/P+134 contacts from it. There is no
main-handler scratch-flag bypass here.

If ordered velocity<=0,request floor with depth=abs(velocity),dropLimit=0.
Copy P+e4 to local Y before attributes. A nonnull G with attr0x200 uses the
same byte-dependent latches/contact0/substate3/acceleration-1 route as main,
then RETURNS without reaching the final -2000 gate. For other outcomes,copy
local floor Y into root and store G at P+138 before testing G.

For nonnull admitted G,request landing sound FIRST,then clear P+0 masks0x70,
set mask2,clear P+8 mask0x00400000,store velocity0. Request planar distance
from current root XZ to prepared target XZ:ordered distance<5 chooses motion0;
otherwise (including unordered) choose motion1. Request partyChgMot(P,chosen),
then continue into the final gate. Unlike main,jump2 does not OR0x00800000
after landing and does not always select stay.

## Jump2 final -2000 gate and mutation ordering

After ordinary phase dispatch/null floor/admitted landing,test current rootY
ordered<-2000. If true,clear P+0 mask0x10,store velocity0,request/store direction
to captured owner M.x/z,copy M.xyz to root and request motion0. Otherwise finish.
This comes AFTER horizontal movement and ordinary floor-root snap in jump2,
whereas main checks before floor/horizontal. It can consequently run after a
successful jump2 floor landing or on an otherwise inactive phase. The attr0x200
early-return route skips it. Neither gate is a clamp at -2000 or a complete
physics/state reset.

## Linked leaves and scoped acceptance cases

partyChgMot's complete old-substate3 cleanup/entry-marker behavior is in
[ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md). Jump callers do not
recursively dispatch a new motion in the same body. Floor's retained height,
last slope/cache writes and early flag return are specified independently;
do not attach all those outputs to the accepted contact by convenience.

Deduced cases include phase initialization delaying integration,count+24
unchanged in flight,exact velocity0 floor query,exact -2000 gate retention,
attr0x200 byte0/nonzero exclusion,main floor-before-horizontal vs jump2 reverse,
null floor root cache snap,main landing0/marker00800000 vs jump2 distance5
boundary/no marker,special main scratch branch causing no published X/Z,and
different exceptional-gate priority. These are static implementation fixtures,
not original executions or rendering-frame timing observations.

tools/verify_field_partner_jump.py pins both complete handlers,linked index/
landing leaves,selected independent request/branch/store/numeric anchors and
exact constants/data spans. Prior floor/front/target verifiers check recovered
dependencies;targetY/dir/front-floor/pose/audio writers and unsupported pointer/
numeric/source domains remain open. Full field/game documentation and eventual
complete-game requirements remain incomplete.
