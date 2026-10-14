# Native Bero position and starting-coordinate publication

Source baseline is [complete position/actor publication](../overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md),
[center and ray ownership](../overworld/OVERWORLD_ROAD_POSITION_AND_CONTACT.md)
and [ordinary bank/leaf evidence](HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md).
This binds the resolved-value calculation and native ROAD movement bridge. It
is not complete original initial_pos or Mario/party/camera callback execution.

FieldBeroNormalPosition starts with two copies of the original three signed
integer XYZ words. Only sentinelX100000 requests a named center. Zero distance
preserves the distinction between a resolved starting center and the original
final tuple. For nonzero distance, heading is single(360*direction)*.125 with180
added only for mode1; no heading wrap. The twoPi constant is original40c90fdb.
The fixed30 start uses fused negative multiply-subtracts; the final displacement
uses30+distance and a fresh pair of trig calls/fused additions. The floor request
starts exactly at P with direction(0,-1,0),length1000000. A resolved miss keeps
source centerY; an unavailable provider holds. Recovery values use final F with
single(F.Y+37),keeping that typed calculation separate from actual recovery
writes. Numerical providers are explicit; private arithmetic grouping is not a
claim about arbitrary original instruction/callback interleaving.

The native successful road transition now reads original selected XYZ,
direction and length rather than deriving all placement from a cardinal vector.
Its center provider admits the existing8 verified static transformed leaves.
The converter rejects any kind0 entry with a missing/ambiguous/meshless or
nonleaf collision owner; future hierarchy entries require the actual leaf-center
traversal provider. Current centers retain imported source geometry and original
node IDs. Floating map conversion and event-driven matrix changes remain
separate numerical/ownership limits; a static bbox is not a general hierarchy
center service.

Floor placement uses the existing indexed fieldShadowRay over actual staged
destination geometry: no Mario attribute/steep/footprint filtering and no Q1000
floor substitution. Runtime disabled1/80 queries still apply. An unresolved
closer object holds native placement. Source cull/winding is retained, while
full original hitMain ordering across multiple live map records/dynamic owners
and Gekko narrow-phase numerical identity remain unproved.

The native trig adapter computes double sine/cosine and rounds to binary32;
fused operations use std::fma with the source negative-FMA sign. These execute
only at a transition, using the existing spatial index for the ray. No per-frame
vertex rewrite/upload, animation or rendering allocation is added. Standard
library versus original MSL/SDK/FPSCR equivalence is not certified.

Starting publication stores single(P*1024),signed truncation and wrap32 bias
-230000000 into the exact three LW10..12 values. Fetched-word decoding follows
the original float API's <=-220000000 condition. Exceptional/out-of-fctiwz-range
values hold native admission rather than saturate. The fixed local-value helper
preserves all other words/flags; it does not claim arbitrary evtSetFloat alias
routing/old-value return. Game records both the calculated unquantized tuples
and the encoded/decoded start. beginRoadArrival uses the decoded starting XZ;
the existing movement bridge still travels toward calculated F. Checkpoint
restore clears these unsaved publication views instead of inventing provenance.

The retained start/recovery view does not acknowledge original actor setters,
mode25 reload, camera road/shift reset or real Bero global RAM stores. Those world
effects remain required before claiming the original initial_pos child returned
or registering complete house05 saved-local cases. Native visible follower,
camera and movement remain their declared adapters. The legacy no-catalog host
bridge remains; native boot requires original Bero assets.

Tests cover source mode and sign, fixed radius versus authored length, explicit
floor miss/hold, zero-distance sentinel distinction, recoveryY37, encoded local
words and untouched storage. Real Game integration checks published start/end,
checkpoint invalidation and an isolated length45 fixture. Original pack checks
verify all8 ROAD leaf bindings. Build/native evidence is in [progress](../../PROGRESS.md).
Next bind real Mario/party setters, camera resets and shared field EVT/case
lifetime using the now-established outer root/bank provenance.

Subsequent [position/camera callback binding](NATIVE_POSITION_CALLBACKS_AND_ENTRY_CAMERA_RESET.md)
now applies ordered Mario/party setters and camera road/shift resets at actual
native arrival start, with explicit unit-scaling precision limits. The typed
placement view alone still does not complete original global/recovery/motion25
writes or genuine retained initial_pos event execution.
