# Ordinary Goombella collision with the original save-block owner

US G8ME01 disc0 revision0;2026-10-13. Static read-only DOL/resource evidence,
not an original runtime observation. Scoped integration dependency for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md), following
[dynamic registry/ray order](../field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md)
and [source save cube](../field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md).
Existing partner contracts specify the complete local floor/front/around paths;
this file supplies their original MOBJ_SaveBlock resource-domain binding.

## Ordinary owner filter: high bit is ownership, not an exemption

Original save hit-node attribute word is0x80000000 and cullbyte1=1. Its
HitObj+0xd0 is the owning map-object pointer; owner+0x15 is the original model
string `MOBJ_SaveBlock`, while the source hit-node name is `HOBJ_mobj_save`.
These are different identifiers. A partner filter compares the MODEL string,
not the room object's name or an assumed texture/model array index.

Fresh complete `chkfilter`800c4a9c/0xe4 inspection confirms these ordered
rules for candidate attributesA:

1. A&0x00080080 !=0:accept1.
2. Otherwise A&0x00800004 !=0:reject0.
3. Otherwise A&0x80000000==0:accept1.
4. Otherwise call mobjHitObjPtrToPtr(candidate); compare owner+0x15 in order
   to `MOBJ_HiddenHatenaBlock` at802cb8a0,
   `MOBJ_HiddenBadgeBlock` at802cb8b8,
   `MOBJ_Hidden10CountBlock` at802cb8d0. Any match rejects; no match accepts.

The three exact literals were reread from the ISO. Original save attributes
reach step4 and match none, therefore **accept**. Do not reject all MOBJ/high-
bit geometry or make a Goombella-only save-box pass-through rule. The first
owner name read has no null guard; valid native dynamic ownership must precede
this filter. Match paths perform a second owner query whose return is ignored;
see [filter body contract](FIELD_PARTNER_FRONT_WALL_FILTERS.md).

Generic partner rule2 differs from ordinary Mario's attr4-with-00800000
exception: generic partner rejects either0x00800000 OR4 after its earlier
priority acceptance. A single shared unordered “solid flags” mask is therefore
not the complete original callback family. The source save attributes happen
to lie in a common admitted domain.

## Which partner services select which callback

Both [floor](FIELD_PARTNER_FLOOR_QUERY.md) and
[front wall](FIELD_PARTNER_FRONT_WALL_QUERY.md) independently select per ray:
nonzero global yoshiGetStatus→chkfilterYoshi800c49b0;
else input partnerkind2 plusflag0x100→chkfilterNokotaro800c48cc;
else genericchkfilter800c4a9c. Ordinary Goombella kind1 with no global active
Yoshi service uses generic. This is the bounded normal state; do not infer
it solely from the visible sprite if global party state changes.

For completeness, both alternate partner callbacks ALSO reach the same model
comparison for source save attributes0x80000000 and therefore accept it.
Their earlier overlapping-mask priority differs: Nokotaro rejects
0x00800005 before accepting0x00080080; Yoshi accepts0x00080080,then
0x00800000,then rejects4. This source cube is not a reason to collapse those
filters for future object resources.

[General vector rays](FIELD_PARTNER_VECTOR_RAY_WRAPPER.md) use that same
partner selector. Ordinary front-wall work casts its upper ray first;
nonnull upper result performs the documented X/Z normal projection and returns
without a lower ray. Do not replace it with a generic radial body collision
or count an admitted object as a special no-collision NPC.

[Surrounding body correction](FIELD_PARTNER_SURROUNDING_WALL.md) is a separate
owner. Partner wrapper800c27c0 passes position,mode1,width and height into
marioChkWallAround800888d0. Its callbacks are selected by GLOBAL Mario state:
motion28→chkfilterVecVivian; else mainflag0x01000000→chkfilterVecRoll;
else normalchkfilterVec800915e4. It does NOT reuse the partner Yoshi/shell/
generic selector. Normal Mario filter accepts this model through the same
three hidden-model exclusions, as recovered in the dynamic registry contract.

For attr0x80000000, the surrounding core's later pass-through classifier does
not set an exemption: masks0x00080000,0x80 and0x00800000 are all clear, so
it retains the admitted candidate as a blocker. It then applies its original
two-pass rays/push/cap rules. This proves normal save-side blocker eligibility,
not complete partner trajectories or every transformed callback policy.

## Ground endpoint and ties, static versus object

Partner ground rays use the SAME hitCheckVecFilter active chain as static
geometry. Source map-record/slot ordering determines candidate order; static
and dynamic do not obtain separate priority buckets. Source save top has
upward normal, so its cull1 downward surface is eligible. No per-face object
flag marks it as partner-only or excludes it from support.

For finite normal direction(0,-1,0), positive current lengthL, source one-sided
ray distanceT admits0<=T<L. Far-end equalityT==L is rejected. Edge equality
on the triangle is admitted without a barycentric epsilon. The backend retains
the first admitted owner/triangle unless a strictly smaller distance occurs;
equal-distance static/object candidates keep the earlier active-chain owner.
See [downward backend](../overworld/OVERWORLD_ROAD_NULL_DOWNWARD_RAY.md).
Do not select an equal-height dynamic support just because it was appended
later, or extend the ray endpoint to include the source save top.

This per-ray winner is distinct from the partner's five-sample reduction.
`partySearchGround` casts center then its four oriented edge samples; ALL five
run. It Q1000-quantizes hit Y, performs source slope checks and vertical
admission, then retains highest admitted height using strictgreater. Equal
support heights preserve the earlier sample. A source save-box hit can still
fail height/drop/inclination tests; passing the callback alone does not force
Goombella onto its top. Root publication/contact/postpass are separate owners.

For surrounding wall rays the body core quantizes distances, discards its own
pass-through candidates, then prefers second height only when firstdistance>
seconddistance; ties retain first. This later two-height choice is another
separate reduction, not a change to backend static/object tie ownership.

## Native binding boundaries and validation

Ordinary Goombella can use the generic source owner/model filter and source
cube alongside static floor/wall geometry. Carry owner identity/model, full
attributes, cull1, active visibility/lifetime and source ordering into the
same query provider. Preserve floor, front-wall and surrounding selection as
separate services. Source support thresholds and body correction are recovered
in the linked contracts; no new proximity, height or pass-through exemption
is justified by this object.

Fresh read-only `inspect_dol_function.py` listings cover all three partner
filter bodies; hidden-model strings were read directly from DOL. Existing
complete source verifiers pass:
`verify_field_partner_front_wall.py` (8 bodies/62anchors/16constants/4spans),
`verify_field_partner_ground.py` (4bodies/100anchors/28constants/4spans),
`verify_field_partner_around.py` (surrounding dependency), and
`verify_overworld_road_null_ray.py` (registry/downward backend dependency).
Listings remain /tmp, no original payload/code is committed or redistributed.
Original Nintendo/Intelligent Systems asset rights remain. Docs-only: no
make/build, Dolphin or hardware claim. Native dynamic Goombella floor/front/
body collision, transformed callback coverage and original matched movement
observations remain open.
