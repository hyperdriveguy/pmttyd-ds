# Partner trailing coordinates,Vivian tail admission and shadow requests

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyPreDisp0x800b7a98(size0x318) and N_vivianDisplayTailCheck
0x801864a8(size0x9c). This is the post-registration tail of
[pre-display](FIELD_PARTNER_PRE_DISPLAY.md),skipped entirely only by its earlier
P+4 mask1 return. Complete shadow/animation/backend/source/runtime domains
remain open;no original visual trace is measured.

## Trailing capture and shadow defaults

Default shadow mode=2,size argument=13.333333015441895(word0x41555555). If unsigned kind P+31!=6,
copy raw current root XYZ to P+b8/+bc/+c0 in XYZ load/store order. Kind6:
copy root to trailing when run mode P+34==4,OR mode==8 and P+0 mask0x100 CLEAR.
Otherwise retain the trailing coordinates produced by its
[ordinary callback](FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md).

Then for kind6 only,if P+0 mask0x100 set,replace size with current P+174;
if P+0 high bit0x80000000 also set,replace shadow mode=1. The mode4 root-copy
route still executes that later mode/size selection. Do not freeze these fields
before pose callbacks or substitute one shadow setting for all paper forms.

## Complete Vivian tail predicate

Request N_vivianDisplayTailCheck(P) for EVERY kind. It returns0 when:

- Unsigned kind P+31!=6.
- Or byte P+4f bit0 SET.
- Or P+4 mask0x00800000 SET.
- Or current GP+12c map name equals original `bom_01` at0x802f89b0 AND
  current rootY compares equal-1000(wordc47a0000),using fcmpu.

Otherwise return1. Tests are ordered,with early returns and no map comparison
for rejected kind/flags. This is a local admission leaf,not a nullable-P guard or
full Vivian visibility/ability-state classifier. Numeric unordered Y does not
take equality-to--1000. No guessed meaning for map/flag combinations is imposed.

## Admitted tail ray and animation request

Nonzero predicate constructs origin from current root XYZ with Y+10,
direction(0,-1,0) copied from original vector0x802cb334,maximum length1000.
Request [partyHitCheck](FIELD_PARTNER_VECTOR_RAY_WRAPPER.md)(P,origin,direction,
pointOut,normalOut,&length)0x800c464c. This wrapper selects filters/quantizes its
outputs according to its own contract;it is not a raw null-filter ray.

Nonnull result copies pointOut.y(frame+1c) to trailing P+bc. Null replaces
P+bc with CURRENT rootY. Neither path changes trailing X/Z or publishes a
contact pointer here. Then request animPoseVivianMain(current P+c,&P+b8)
0x80039824. It consumes the trailing VECTOR,not main root coordinates.
Its full backend deformation/animation behavior remains open.

## Shadow submission and camera/flag order

After the possible tail ray,reread P+4 mask0x00100000. SET skips all shadow
requests. Otherwise request
shadowEntryMode(mode,P+b8,P+bc,P+c0,size)0x8002bdb0 and retain returned ID.

Read current P+164(camera ID). If integer!=4,request shadowSetCamId(ID,camera)
0x8002bd30;equal4 skips it. Then reread P+4 mask0x00200000. If set,request
shadowFlagOn(ID,4)0x8002bce0. No returned-ID validity/null check exists here.
Do not merge these calls,cache flags before entry,or assume registration failure
has a local fallback. Original camera/queue/shadow record/frame/resource behavior
remains external.

The pre-display verifier pins both complete local graphs,the original ray vector,
map sentinel and constants. Used pose/shadow services and source/alias/FP/
run-mode/flag/trailing lifecycles remain required for strongly equivalent output.
