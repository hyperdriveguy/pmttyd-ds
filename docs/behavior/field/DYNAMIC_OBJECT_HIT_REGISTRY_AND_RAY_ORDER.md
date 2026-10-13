# Dynamic object hit registry, shared ray order and owner lifetime

US G8ME01 disc0 revision0;2026-10-13. Static read-only DOL evidence, no original
execution. This extends [save-block collision](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
and [OW-INTERACT-01](../../requests/OW-INTERACT-01.md) for the native dynamic
head-hit binding. Full ground/wall and registry/cadence integration remain open.

## One shared chain, rather than separate static/dynamic winners

`hitMain`80018264/0x24c calls mapGetWork and rebuilds active head pointer
r13+0x1708 (8041e628) from all map records. Outer map records advance0x178,
ascending index0..word[mapWork+0]-1. Each record supplies count+0x158 and
HitObj array+0x15c. Scan slots ascending, stride0xe4:

- Source node is word[HitObj+8]. Its geometry part countnode+0x5c must be
  nonzero. Hierarchy-only nodes are not active triangle owners.
- Runtime halfword[HitObj+0]&0x81 must be0. Runtime masks1/0x80 disable
  admission independently of source draw attributes.
- Append eligible slots in this exact order through HitObj+0xe0; terminate
  last eligible next pointerNULL. Empty active domain leaves headNULL.

This rebuild is8001827c..8001830c. Later hitMain map-binding/transform work
has its own producers; the linked-list construction itself does not split
static map nodes from MOBJ nodes. `hitCheckVecFilter`800158a4 reads that
same head8041e628 and follows+0xe0 for normal Mario head, ground, wall and
NULL-filter queries. There is no static winner followed by a separately
privileged object query. Native overlays must preserve source order and common
nearest-hit rules across both categories.

## Initial static allocation and MOBJ insertion order

`hitEntry`80018658/0x9c receives rootnode,matrix,mapIndex. It computes
mapGetJoints(root), sets that selected map record's count+0x158, allocates
roundUp32((count+128)*0xe4) bytes, stores array+0x15c and clears the entire
allocation. Then calls hitEntrySub with the selected map index, constructing
initial hierarchy slots. The extra128 entries reserve runtime object capacity;
this is allocation capacity, not a dynamic object count established by tests.

`hitEntryMOBJ`800185e8/0x70 computes mapGetJoints(objectNode), ADDS that
count to the FIRST map record's count+0x158, and calls hitEntrySub with literal
mapIndex0. It does not substitute current/destination map index. `_hitEnt`
8001896c/0x48c resolves the selected record and scans its array fromslot0
while nodepointer+8 is nonnull. FirstNULL slot receives the borrowed source
node pointer, runtime flags0, mapIndex halfword+0xaa, matrices and geometry.
The save block's one root/one mesh adds one owner slot.

With an initially populated record0, newly constructed MOBJ occupies the
first unused reserve slot after its original static hierarchy slots. Multiple
MOBJ initially follow construction order. If multiple map records are loaded,
record0 MOBJ nodes occur BEFORE record1's static nodes during active-chain
rebuild. Therefore “all static triangles, then all objects” is justified only
for a single-map-record domain. No source global ordering by distance, actor
name, render pass or model namespace is performed.

Neither hitEntryMOBJ nor _hitEnt inserts the new owner directly into the active
+0xe0 chain. Publication waits for a subsequent hitMain or another source
operation that rebuilds it. Field Main invokes hitMain before evtmgrMain,
then Mario, then MOBJ: a constructor during that event-manager call has a
different active-chain publication lifetime from an already resident object.
See [field order](SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md). Native room
staging is a platform adapter; do not claim its instant overlays reproduce
all original creation-order/cadence behavior.

## Ordinary Mario head filter and stable nearest winner

Normal marioHitCheck2 path80090598..800905e8 submits origin/direction/length
with query fields+4=4,+8=0x00800000 and callbackchkFilterAttr2=800916c4.
The complete0x2c callback receives queryr3,HitObjr4, reads source attribute
wordHitObj+4 and returns0 only when attributes intersect query+4 AND fail to
intersect query+8; otherwise1. For the ordinary head domain that means reject
attr4 unless attr0x00800000 is set. Source save-node attributes0x80000000
pass. Attribute0x80000000 alone is neither a head-only flag nor a skip flag.

Upward ray(0,1,0) uses live region80015e14..80016170 in hitCheckVecFilter;
source dispatch separately checks directionX/Z==0 and directionY==1.
Each owner runs the callback first, then midpoint/radius broad phase, then
triangles in buffer order (stride0x54, signed16 count+0xa8). The save-box
triangle order is the original six strips/two triangles per strip in the linked
collision contract. Native array order is not arbitrary when distances tie.

Nearest winner updates80016100..8001614c: if retained bestLength<0,accept;
otherwise accept only when retained bestLength>newLength. Equality retains the
earlier owner/triangle. Far-end equality is rejected before that winner step:
for nonnegative current query length, candidate distance>=currentLength fails
80016060..80016084. Thus stable ties follow the initial active chain and within-
owner triangle order. Normalized final normal, final length and point belong
to the stored winner, not the most recently inspected candidate. Source tail
80016538 explicitly restores returnr3=winning owner pointer after normalizing.

The five marioSearchHead probes retain their separate source admission/order:
first nonrejected probe yields contact or near-miss; do not compare all probes
into a new globally closest head result. This shared-owner nearest policy is
WITHIN each probe, before the head leaf's inclination/contact checks.

## One-sided box admission and other collision consumers

The source box node+0x58 points80415f28: draw-mode bytes00,01,00,00,
attribute word+8=0x80000000. Thus cullbyte1 selects one-sided geometry.
ALL12 box triangles share these same node-level attributes/cull settings.
There are no distinct bottom/top/side gameplay flags in the source template.

For finite upward head rays, let V0/1/2 and E0/1/2 be the runtime triangle,
N=cross(E0,E1), Q=dot(N,P−V0), and
Ki=(P−Vi).z*Ei.x−(P−Vi).x*Ei.z. One-sided upward admission:
N.y<0; Q>=0; eachKi<=0; T=−Q/N.y; for nonnegative current length require
T<length. Edge equality and T0 are admitted. The bottom of the source cube
hasN.y<0 and therefore participates; top and vertical sides fail the upward
normal gate. Two-sided upward admission uses N.y*Q<0 and Q*Ki<=0 instead;
it is not the save-node's source cull domain. Full float/FMA exceptional-domain
and original backend arithmetic equivalence remain separate.

For downward rays cull1 admits upward normals, so the SAME node's top can
provide ground. Horizontal wall queries use the same chain and own direction/
face/intersection tests, so the SAME node's sides can provide walls. Adding
only bottom head triangles is useful scoped activation, but does not implement
complete source object collision. Preserve full cube geometry and separate
query-specific admission rather than assign fabricated head/floor/side bits.

Ordinary `chkfilterVec`800915e4/0xe0 used by normal Mario collision queries:
attributes&0x00080080 accepts first; otherwise attr4 without attr0x00800000
rejects; otherwise non-MOBJ attributes accept. For MOBJ mask0x80000000,
resolve its owner, compare model stringobject+0x15 to only these three:
`MOBJ_HiddenHatenaBlock`, `MOBJ_HiddenBadgeBlock`,
`MOBJ_Hidden10CountBlock`; matching names reject, others accept. Source
`MOBJ_SaveBlock` therefore passes this normal filter. Transform-specific
Vivian/roll/partner filters are different owners and not generalized here.
The top/side finding proves geometric eligibility, not full landing/step/wall
response or a matched original observation.

## Names, deletion and borrowed owner lifetime

The MOBJ source node is stored inside the live object record; its name pointer
references object-owned `HOBJ_`+objectName bytes. HitObj+0xd0 references that
object. `hitGetName`80014328/0x9c first checks whether the pointer is inside
any current map record's allocated HitObj range (count+128 slots), then returns
word[word[HitObj+8]+0], elseNULL. It does not allocate/copy a name or establish
a generation identity. MOBJ new-head-name comparisons use this borrowed name,
as recovered in the save hit contract.

`hitDelete(name)`800184b0/0x138 scans map/slot order, skipping runtime0x80,
compares source-node name, marks first match runtime0x80 and IMMEDIATELY
rebuilds the same active chain. It does not free its triangle allocation, clear
node+8, shrinkcount+0x158 or compact slots. Consequently ordinary deletion
does not return that slot to _hitEnt's firstNULL-slot allocator. Existing
name/owner pointers can still refer to excluded records; inactive geometry must
not be queried just because its data remain allocated.

`mobjDelete`80060a94/0x108 finds active object by its name; for its two
potential hit names atobject+0x78/+0x100, calls the field Mario hit-reference
cleanup whenGP+0x14==0, then hitDelete. Finally clears only active objectflag1.
It does not memset the complete object or free/copy its hit names here.
Reusing that MOBJ record through mobjEntry subsequently clears/repopulates its
borrowed node/name bytes. Room reset/heap release and stale pointer domains
remain distinct services; this is not a safe lifetime guarantee for retained
native pointers or copied source-node generations.

Native catalogs should provide stable owner identity/lifetime and source order
within an admitted area, invalidate queries on successful area replacement,
and preserve source name-based edge semantics. Those are bounded platform
adaptations; they do not justify reading dangling original pointers or claiming
original deletion/allocation equivalence. Failed staging must retain resident
collision/name owners alongside visuals.

## Validation and remaining work

Fresh named `inspect_dol_function.py` read-only listings cover hitMain,
hitEntry,hitEntryMOBJ,hitEntrySub,_hitEnt,hitDelete,hitGetName,mobjDelete,
marioHitCheck2,chkFilterAttr2,chkfilterVec,hitCheckVecFilter and mapGetJoints.
Original template/hidden-model literals were reread from DOL. Inspection ELF/
text outputs remain /tmp/sb-* outside source history. Existing
[NULL ray verifier](../overworld/OVERWORLD_ROAD_NULL_DOWNWARD_RAY.md) pins
complete registry/backend containing bodies; upward branch dataflow above is
fresh static recovery, not an independent executed reference trace.
Original Nintendo/Intelligent Systems asset rights remain; no redistribution
is implied. Docs-only: no native build/test, Dolphin or hardware claim.

Remaining: native source-order query overlay for head/ground/walls; original
multi-record/source node ordering fixtures; dynamic creation/deletion/reset
traces; disabled/event-visible/runtime flags; transformed/partner collision,
scene moving-object matrices/spheres, exact query math and original captures.
