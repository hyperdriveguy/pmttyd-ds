# NPC, partner and map-object owners

This guide covers packed NPC initialization/entry/setup, field CPU pose resources,
the actor catalog, talk selection and presentation, save-block/map transforms,
and the typed native partner registry/appearance helpers. See [field entry](field-entry.md)
for event leaf ordering and [field motion](field-motion.md) for wall/NPC contact.
Actual original behavior evidence belongs to [field contracts](../behavior/README.md).
These adapters do not complete original NPC Main, room lifecycle or all objects.

## Registry and setup sequencing

Packed registry addresses identify guest memory. The US work table at 0x8041e8b4
has ordinary/alternate banks separated by 0x14 bytes; live GP+0x14 selects the
bank. Initialization allocates 36 ordinary and 16 alternate 0x340-byte actors,
ordinary release work at 0x94 bytes per actor, and 0x580-byte field work. Source
allocation/store order matters: capacities and GP are re-read after callbacks,
and the auxiliary allocation is not implicitly cleared. This routine does not
silently reset registry counts beyond the stores actually present in its body.

NPC entry scans active names first, then independently selects the first free
slot. A duplicate does not reject the request or reuse the named actor. Capacity
admission must exclude the original overrun domain. Only pose result -2 retries
with original `hoshi`, re-reading the live context selector. Pose consumers load
the current stored handle when reached. Root/current/previous vector copies,
reversed timestamp word stores, original default animation, Vivian/Luigi flags,
material-light mask and final count update retain their original order.

`NoAssert` name lookup returns zero when missing. The inspected release lookup
returns the exhausted scan pointer, which may be an end pointer rather than a
valid actor. Native callers must establish registered-name admission before
using that form; the conventional function name does not imply a runtime assert.

Setup rows span 0x5c bytes. The cursor binds the event address/ID and world,
operand and event-provider identities. First phase skips missing actors, merges
flags, deletes old init events, re-reads the replacement script and captures it
before a possible native allocation hold. A retry must not repeat deletion or
already completed metadata writes. Paired loads precede both stores, while
single-word copies remain interleaved with callbacks. The repeated territory
fields are intentional, as are descriptor stay/talk/animation requests.

End of first phase sets mode 1 and returns source status 0, resetting the cursor.
Later invocations freshly resolve their table and poll all init events before
creating any regular events. A live init is a genuine source wait, distinct from
a native constructor hold. Regular events use original flag 0x20 and actor token
`field170`; init/regular `rawBytes[0]` values differ. Full constructor ownership
and real resource admission remain the caller's responsibility.

## CPU pose and catalog ownership

`FieldPosePool` installs real MODEL/TPL/animation resources and verifies descriptor
identity and descriptor-zero evaluation. MODEL header fields +0xc4/+0x148 retain
flags/descriptor count; +0x134/+0x198 describe 64-byte texture records. Type-zero
texture indices must refer to the retained TPL. Reinstallation by model name is
accepted only for the same shared owner. Unknown animation names intentionally
fall back to descriptor zero; readiness does not prove the requested name exists.
Capacity admission checks 256 pose slots and shared model/texture ownership.
An injected pool shares that namespace with player/battle roles; standalone callers
retain their independent legacy path. See [field session](field-session.md).
Successful CPU entry does not prove GPU residency or original NPC Main readiness.

PMNPC001..009 are private little endian catalog versions, not source NPC RAM.
Versions add system/story exclusion gates, hit bounds, original names, draw and
tribe dimensions, flags, ordered talk rules, wander metadata, then original
Shift-JIS event names and supported init selectors. Inline loader notes identify
each version and native bounds. Bounds validate importer scope; they are not
claims about original parser checks. Candidate catalog and angle data publish
together only after success. Repeated actor keys/names require mutually exclusive
admitted conditions. Original event bytes are distinct from native catalog keys.

Story gate zero is unconditional, one is an inclusive range, two excludes one
value. Unknown story cannot pass a conditional gate. Talk rules preserve first
source-order match, including an early unconditional rule. Legacy chapter
thresholds apply only to the already supported simple talk family. Quest guards
use known switches/inventory observations; unknown is not a false flag or zero
quantity. The unchanged message ID/control tokens and their backing text storage
survive in `FieldActorTalk`; no replacement dialogue is authored here.

Conditional Garf/Borodo2 coordinates are original engine-space init values.
Hidden Y=-1000 is a placement, not actor deletion. Missing required quest state
holds rather than choosing the ordinary placement.

## Talk selection and paper presentation

Talk inputs use source dimensions and engine units. Motion 0..2, control/background/
party state and signed key depth gate scanning. NPC flag 2 admits talk, flag 8
excludes it; vertical overlap is inclusive. Range uses half the combined widths
plus 10. Center and both width edges test camera-relative facing before a ray at
75% Mario height tests obstruction. An absent ray provider cannot prove clearance.

`nearActor` may update before facing/ray rejection and does not tighten distance.
A fully admitted actor tightens the nearest bound using strict less-than, preserving
the first candidate on ties. Per-actor admission can succeed without winning
nearest selection. Candidacy latches once, saves yaw and pauses the regular event;
loss either restores/restarts under the source flag or requests the deferred
secondary continuation. The proven-null interrupt leaf preserves only bits 8..11.

The original 91-sample angle correction table is not `atan2`: it rounds a ratio
index and maps quadrants with -Z zero. Matrix rotations still use explicit SDK/
libm adapters. Paper preparation retains separate camera requests for projection
correction and Main branches. Its reflection sign uses the previous facing;
the retained facing is then adjusted around 180 degrees and eased by the original
.1 factor unless snap is selected. Rear boundaries are asymmetric at 90/270.
Matrix order, pivot, additive rotations and final uniform scale remain explicit.

Message pose serials prevent repeated requests from restarting every visit.
Authored millisecond request time becomes 60Hz animation time independently of
render cadence. Clock regression resets that retained state. Prepared positions
and talk anchors convert engine units by .01 only at the native presentation edge.

## Save objects and map matrices

Head contact is deduplicated by semantic hit name. Changing a pointer with the
same name must not retrigger; no contact clears the cache. Head production runs
before its cancellation gates. Save-block callback stages yield after one stage;
animation Main publishes afterwards, so the callback sees the previous ratio.
The hit requests original sound 431 and `A_1`; completion selects `S_1`, optional
then system events, and finally restores controls/camera. Event entry checks
nonzero as busy, but stage waits check exactly one. Preserve that distinction.

The native facade selects save objects by original model identity and pins actual
clip intervals. Raw elapsed 60Hz frame feeds PMANIM; ratio is not clamped after
nonloop completion and is not computed as start+elapsed. The bound script is
0x8030b4ec, priority 30/type 26. Local positive IDs are facade identities. Native
failure recovery unlocks retained controls/presentation; it is not original card
error handling or successful save completion. Room change while locked recovers
and yields before installing the new instances.

`StageMapObjects` uses row-major affine 3x4 matrices and explicit recalculation.
Source translations divide by 10 to reach converted map coordinates; root scale
.1 contributes the separate final bridge. Additional matrices postmultiply.
Flag 0x200 marks a mutation series, 0x200000 retains its additional matrix, and
flag 1 hides geometry. Recalculation clears the series flag after composition;
render traversal must not reset it. Descendant flag updates follow ancestry.
Vertex bindings/visibility ranges retain the converted original hierarchy.

## Typed native partner registry and placement

Partner storage is a big endian 0x188-byte source-sized record. The registry
admits two actor owners. Fresh original sessions index a shared 256-handle pose
namespace; the legacy independent workspace uses five role slots. Signed
Mario index bytes select the first live partner without main flag 8. Control mask
0x20000000 is distinct from Mario's wrapping key depth. Entry stores camera,
owner, kind and original definition-table identity before later pose/placement
work; zero allocation alone does not initialize a partner.

Supported source resource tokens retain original Mario/Goombella identities.
Shared entry uses the actual descriptor-zero source pose; the legacy path
prepares its older visible workspace seeds. [Field session](field-session.md)
explains their different resource and copy ownership. Goombella's HP<=5 selects original `PKR_D_2`, otherwise
`PKR_S_1`; a pending name request is distinct from evaluated playback.

Appearance queries use engine units and retain every root/display/provider call.
Heading quantizes to .001 degrees with signed half bias, then wraps. Special
retained headings and camera-relative offset mirroring are source branches.
Placement casts horizontally at Y+37 for 30+height, moves back 30, then casts
38 down. It tries the opposite side only after horizontal obstruction; a floor
miss does not justify another side. On failure, output may retain the attempted
point. Geometry/numerical services remain explicit, without invented floor filters.
