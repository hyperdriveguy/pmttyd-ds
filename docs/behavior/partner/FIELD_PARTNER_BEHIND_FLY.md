# Flying behind-Mario motion: destination,countdown,movement and completion

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_motion_behind_mario_fly0x8014defc,size0x4e0. Both
[fly executor](FIELD_PARTNER_FLY_DISPATCH.md) and
[walk executor](FIELD_PARTNER_WALK_DISPATCH.md) request it when their selected
mobility alternatives are nonzero. This local body is not a generic lerp follower;
it requests placement4 on entry and has separate moving/completion graphs.
No original execution or trajectory has been measured.

## Entry and facing source

Capture owner M=P+160. If P+8 mask2 is set,consume only that marker,OR
0x00040000 into P+8,and inspect P+8 mask0x00100000. If set,skip entry-facing
calculation but still request destination placement below. Otherwise:

- If P+8 mask0x00010000 set,clear that mask;request angleABf(M.x,M.z,
  destination P+94/P+9c),then revise360(R(180+angle)).
- Else if P+0 mask8 clear,request toMovedir(current owner facing M+1ac).
- Else request marioGetPartyId then partyGetPtr. Nonnull Q requests
  partyToMovedir(P,current Q+10c);null falls back to toMovedir(M+1ac).
  Add180 and request revise360. There is no status/distance gate here.

Publish the resulting facing at P+54. Owner M used by the ordinary/fallback
heading path is freshly captured there;the function's initial M remains retained
for later activity/position loads. These pointer reads are not interchangeable.

Request [partyGetAppearPos4](FIELD_PARTNER_PLACEMENT4.md)(P,&P+94),ignore its
return. This buffer is the destination XYZ,not live root58. Request distABf(root
XZ,destination XZ),convert with fctiwz,store signed word P+24,and if not signed
>0 replace it with1. This word comes from distance conversion;it is not a
measured fixed-duration animation/tick count. No entry root teleport occurs.

On EVERY invocation,if P+8 mask0x00100000 is clear AND marioKeyOffChk returns
nonzero,recompute facing using the ordinary owner/buddy route above,add180/wrap,
and write P+54. This does not request placement4 again or reset P+24. No
physical input semantics beyond that exact query result are asserted.

## Moving gate and ordered services

Request angleABf(rootXZ,destinationXZ);its returned angle is unused. Then
choose planar threshold K=6 if captured M+180 is ordered>0,else2. Request
D=distABf(rootXZ,destinationXZ),retain D. Decrement word P+24 with the original32-bit wrap BEFORE testing:

```
need = ordered D>=K OR ordered abs(R(destinationY-rootY))>2
move = need AND signed current P+24>0
```

Unordered D does not satisfy >=;the vertical test may still admit. If move:

1. Request angleABf AGAIN and store its result at P+100. Request distABf
   AGAIN;that second distance result is unused,the retained D still drives
   later tests. Do not remove calls merely because their float result is unused.
2. Choose A=3.6 if P+8 mask0x00100000 set,else2. If ordered retained D<K,
   replace A with0 and request immediate speed writer0x800cbf84(P,0).
   Then always request smoothed writer0x800cbfbc(P,A). Their
   [different speed effects](FIELD_PARTNER_FLY_SPEED_WRITERS.md) are explicit.
3. Copy current rootXYZ to local V. Request partyGetMoveDirSpd(P,&heading,
   &speed),then partySearchFrontWall(P,&V,speed,heading).
4. Nonnull wall writes P+12c;also writes P+134 if current speed P+104 is not
   equal0(including unordered). Null does not clear either field here.
5. Publish V.x then V.z to root;V.y is ignored. Blend rootY toward CURRENT
   destinationY with R(destinationY-rootY),then fused coefficient0x3d4ccccd
   (approximately0.05) times delta plus rootY.
6. Request unk_800cbb10(P,current P+104),then return. This is a presentation/
   movement service request,not verified original sound/pose output.

No ground query or completion resets run in this moving branch. The
[direction/speed](FIELD_PARTNER_DIRECTION_AND_SPEED.md) and
[front-wall](FIELD_PARTNER_FRONT_WALL_QUERY.md) bodies specify those bound services.

## Completion/reset path

When move is false,store P+24=0. If P+8 mask0x00200000 set,clear P+8
mask0x00020000;when unsigned mobility P+32==1 also clear P+4 mask0x10.
Then make these ordered requests/writes:

1. partyChgMoveMode(P,1),then partyChgMot(P,0),then partySetDir(P,M.x,M.z).
2. Store float50 to P+40,clear P+4 mask0x10,request immediate speed writer
   with0,OR0x00080000 into P+0.
3. Clear P+8 masks0x00100000,0x00040000,0x00200000 in three separate live
   load/store operations,then request unk_800cba60(P).
4. If current P+134 nonnull,OR0x00080000 into P+0 AGAIN,and repeat move mode1,
   motion0,owner-direction,50-to40,P+4 mask0x10 clear and P+8 mask0x00040000
   clear. Do not merge the duplicate callbacks/writes. P+134 is not cleared.

These are bound requests;full pose/mode/direction/presentation internals remain
separate. No caller-independent teleport or destination clear is implied.

Finally request [partySearchGround](FIELD_PARTNER_FLOOR_QUERY.md)(P,100,100).
Let H be its return. If initially null,skip all floorY blending and store null
P+138. For initially nonnull H:

- Request hitGetAttr. If0x200 set,discard H unconditionally;when signed P+4c
  equals0 also OR0x01000000 into P+8 and0x02000000 into P+0. No mobility0 gate
  is added to this latch path.
- Independently,if retained planar D is ordered>current P+f0,discard H.
  This literal comparison uses horizontal distance against the height field;
  do not replace it with a vertical gap test.
- Even if those tests discarded H,blend rootY toward current P+e4 with the
  same0.05 fused pipeline. The initially-nonnull path owns this write.
- Store remaining H/null to P+138 and return.

There is no head query,out-of-screen reset or general below-world recovery in
this local body. Floor service metadata writes remain observable even when its
return is later discarded. Owner/slot/flag/distance/count domains,called math/
backend/services and original traces remain open. The flying verifier pins this
complete local graph;it does not execute original PPC or establish host parity.
