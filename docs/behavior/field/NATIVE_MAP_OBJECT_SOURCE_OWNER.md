# Original map-object construction and matrix ownership

This implements complete local process initialization, lookup, counting,
construction and matrix-tree recalculation. Full map loading/unloading, live
mapMain animation/setter epochs, lamp-event dispatch and ordinary Game activation
remain open. Fixtures constructing all original harbor objects do not establish
playable prologue or original framebuffer parity.

## Source boundaries and continuation representation

The eight complete US revision0 bodies are mapInit8002af18/44,
mapGetMapObj8001f878/a8, mapEntry8002a46c/9c, mapEntrySub8002a508/278,
_mapEnt8002a780/588, mapGetJoints8002b120/4c,
mapGetJointsSub8002b16c/2c4 and mapReCalcMatrix80028130/420.
The fixed native continuation graph retains all1070 original instruction words
as address/provenance comments; `tools/verify_field_map_source.py` verifies every
word and complete span against the read-only DOL. Each case is compiled native
C++; there is no runtime guest code fetch or instruction decoder. This preserves
the source's compiler-unrolled reads, recursion, stores and branches while making
native child waits and budget yields explicit.

`FieldMapSourceCursor` owns GPR/CR/CTR/LR, binary32 float lanes, partial multiple
load/store progress and the exact queued child. Actual guest stack save areas
and SDK temporaries belong to stationary mapped native storage. Root registers
outside arguments are native private-ABI seeds, not claimed original stack
observations. Each reached callback/store can hold without replaying its earlier
prefix. Provider identity pins after installation; new invocation arguments do
not replace a held call. Budget exhaustion retains the source PC and does not
advance simulation. Complete constructors include classification, parent flag
inheritance, bounds and the final source scalar return, not just matrix creation.

The original process pointer8041e660, active group8041e664 and auxiliary8041e668
use real VM storage. Init allocates5e8 bytes from heap0, publishes the pointer,
then performs a second explicit memset and resets group/auxiliary. Groups stride
2f4; map entries stride178; runtime objects stride134. Lookup uses unchecked
source group multiplication, fresh signed outer/inner counts and actual joint
names; it does not normalize every non1 group to0 or filter visibility flags.

Entry counts root plus descendants, excluding the root's sibling. It publishes
E150 count, allocates the rounded actual array through the existing original map
allocator and publishes E154 before explicit clear and full recursive creation.
Each constructor scans the actual array for its first joint-null row. Source
null/full/malformed domains remain visible in the portable graph; they do not
become fabricated successful empty objects.

## Actual native storage and SDK

`FieldMapSourceOwner` owns the three globals, aligned process work and64KiB
private stack, all mapped as real external VM RAM. The existing primary map
pool remains with its actual caller-owned backing. Map allocation changes that
pool's real headers and uses its actual clear/cache callbacks through
`eventMapAllocate`; it never allocates detached per-object renderer records.
The process allocator's false flush retains the same zeroed block. Its adapter
supports the one cold process allocation, not repeated source Init reallocations.

A stationary noncopyable `FieldMapRequest` identifies each invocation through
all waits. Another token or operation cannot consume the held call. VM, heap,
request and child providers outlive all holds/borrows; freeze users over retained
load/unload. Physical backing checks reject byte-identical replacement memory;
conditional teardown removes only the owner's own mappings. The backing query
is pure and safe inside an existing SDK raw borrow, avoiding nested borrow
failure. Pending producers must exclude ordinary consumers/clock advancement.

`FieldMapSdk` reuses the accepted shared matrix engine for live SDK constants,
source Concat grouping/captures and retained outputs. Stable complete raw vectors
use finite native add/scale/point and squared-distance arithmetic. Exact in-place
vectors or disjoint spans are admitted; partial aliases and sparse/semantic
vector storage remain unavailable. Source `_mapEnt` calls
**PSVECSquareDistance8029ee8c**, not the neighboring Distance8029eeb4. Its result
is `round32(fma32(dx,dx,round32(dy*dy)) + round32(dz*dz))`; the source compares it
with20000 without a square root. Equality does not set flag02. Goldens distinguish
squares169,20000,18100 and30000 and exercise actual constructor flag02 branches.
Full GQR/FPSCR/exceptional and original numeric/pixel parity remain unverified;
native finite SDK adaptation is not evidence of those broader claims.

Construction publishes authored `T*Rz*Ry*Rx*S` at4c, parent*authored at1c and
executes actual SDK Identity at ac. Recalculate chooses animated7c for flag2000,
otherwise authored4c; post-concats ac only for flag00200000, clears dirty0200
through a fresh read/store, and propagates parent recomputation to children.
Siblings retain their inherited parent/force. Additional matrix and composed
world matrix remain distinct original owners.

## Original payload and publication boundary

The editable original map payload is
`assets/original/maps/harbor-source/{event,provenance}.json`:975520 exact payload
bytes from `m/gor_00/d`, all15974 original fixups applied once at explicit native
guest base8a000000, plus reached original map/SDK literals. No initialized
MapEnt/MapObj/matrix/light outputs are imported. Source rights/provenance are in
[asset ledger](../../ASSETS.md). Normal manifest builds produce
`field-harbor-map-source.pmevt`; optional area input merging happens before VM
import. Original scene selection is S, with540 actual objects; ship S_fune_b
is source ordinal52. The original Scale10 root and full source ancestry yield
its initial world diagonal10 and translation(210,0,45).

**mapEntry does not publish loaded-map count.** Full `_mapLoad` publishes the
returned scene root at Ea8; only after the full loader returns does ordinary
mapLoad write G0=1. Partial construction remains inaccessible to lookup. The
fixture explicitly models that later publication; this does not implement full
mapLoad. The source order remains mapMain, hitMain, evtmgrMain, mapDisp and later
lightMain; do not recalculate after each event simply to force a preferred epoch.

Original `_unload` calls mapUnLoad before later npcReset and evtmgrReInit.
Freeze manager/renderer users through that retained interval rather than changing
source call order. Quarantine or finish promised-live opaque operations before
freeing their backing. Full source load/unload and runtime setters are next
boundaries, followed by [persistent harbor lamp events](NATIVE_HARBOR_AREA_INITIALIZATION.md)
and [light owner integration](NATIVE_LIGHT_REGISTRY_OWNER.md).

## Evidence and limits

`artifacts/field-map-owner-evidence/report.md` contains complete bodies/word
hashes, raw layouts, exact ownership/publication order and corrected child/sibling
ancestry. Older harbor-light ancestry evidence reversed those links; this report
supersedes that ancestor list. The new vector evidence corrects an initial
Distance label to SquareDistance before implementation. These are static source
findings, not Dolphin measurements. Tests cover every primitive hold in complete
count/lookup, caller pinning, actual540-object hierarchy/ship matrices/recalc,
cache ownership, physical replacement and real constructor threshold branches.
See [progress](../../PROGRESS.md) for exact build/ARM/emulator scope.
