# Rogueport camera porting coverage and remaining bindings

US G8ME01 revision0; reconciled2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md). This is the implementation map
for the five-room 2DS XL slice, joining existing original-source contracts and
fresh read-only inventory. It does not claim native implementation, original
runtime observation, complete engine coverage or bitwise Gekko numeric parity.
Whole-body hashes alone are not proof of every semantic branch.

## Original asset domain verified for this slice

[Inventory](../../../tools/camera_rogueport_porting_inventory.json) retains only
file hashes/sizes and numeric metadata. Original containers stay outside history.
All five have version text1.01, which camRoadSetup maps to runtime101 on exact
strcmp. All44 records have combined point/tangent row count12 (six points/six
rows), start/end indices2/3, extra-curve count+b4=0 and disable word+28=0.
These are source data properties, not generic parser defaults or engine limits.

| Original camera file | Records | Version101 fixed-Y records (indices) |
| --- | --- | --- |
| m/gor_00/c | 3 | None |
| m/gor_01/c | 11 | 2,3,4,5 |
| m/gor_02/c | 14 | 3,4,5,6,7,8,10,11 |
| m/gor_03/c | 14 | 2,3,4,6,7,9,11 |
| m/gor_04/c | 2 | 1 |

The20 fixed-Y records have word+20 nonzero; recover their actual+24 values from
the original asset and preserve the version gate. Do not use zone height as a
substitute for that anchor-Y override. Index identity must survive extraction:
road names can repeat, so merging records by name changes the original policy.

The segment inventory verifies220 consecutive pairs take straight dispatch
AFTER original near-normal collapse. It does not license replacing raw tangent
halfspaces or collapsing the entire authored path into one chord.34 records
have no duplicate XZ segment;10 have one. All494 zone fan triangles are finite;
234 are nonhorizontal,8 exactly degenerate in3D,16 have zero projectedXZ area.
Flat planes, guessed room boxes and skipped duplicates are insufficient substitutes.

## Source dependencies in porting order

| Service / dependency | Recovered contract and proven boundary |
| --- | --- |
| Asset decoding, version and state identity | [Container](OVERWORLD_CAMERA_CONTAINER.md), [state interface](OVERWORLD_CAMERA_ROAD_STATE.md): original indexing/units/fields, bounded selection and state construction; portable bounds checks are adapters |
| Candidate zone selection | [Zone triangles](OVERWORLD_CAMERA_ZONE_TRIANGLES.md): finite general downward query/plane/edge/tie rules and all494 triangle classifications; fallback-height ties differ from polygon ties |
| Main path dispatch/projection/caps | [General straight geometry](OVERWORLD_CAMERA_AUTHORED_SEGMENT_DISPATCH.md): all220 dispatch pairs,34 nonduplicate records, strict halfspaces/projection/cap/nearest tie; original floating-point evaluation remains a dependency |
| Duplicate segments and room lifecycle | [Lifetime/duplicates](OVERWORLD_CAMERA_MAP_LIFETIME_AND_DUPLICATES.md): finite retained-fraction output independence for all ten duplicates; no general skip/fraction0 rule or nonfinite proof |
| Contact and shift production | [Target/shift](OVERWORLD_CAMERA_COLLISION_TARGET_AND_SHIFT.md): retained ground contact, separately timed global shift and ordinary callback composition; source clocks/owners must be imported |
| Nonzero shift correction/inertia | [Moving correction](OVERWORLD_CAMERA_MOVING_SHIFT_CORRECTION.md): finite successful main-only path with nondegenerate unit tangent/extra count0; provider queries and retained history required |
| Comparison and transition composition | [Comparison](FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md), [manager/time](FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md), [interpolation/state](FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md): complete local graphs and exact copies/gates/constants; surrounding provider/library/runtime domains open |
| Mario target and rates | [Shift controls](FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md), [rate/ID/reset](FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md): complete local updates/reset owners; defaults are not unconditional per-tick values |
| Ordinary camera and authored events | [Lifecycle/projection](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md), [dolly/event blends](OVERWORLD_CAMERA_DOLLY_AND_EVENT_BLENDS.md), [interpolation/time](OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md): bounded mode1 consumers/producers and original gor event request census; actual event admission/execution separate |

The older19-record [cardinal path contract](OVERWORLD_CAMERA_STRAIGHT_PATHS.md)
is an initial subset; the general dispatch/duplicate contracts extend it. Do
not leave the native provider restricted to19 while claiming ordinary five-room
coverage. Moving correction accepts a source-contract query provider; its
initial19-record proof does not itself prove composed queries across all44.
That composition remains to validate, even though all main pairs dispatch straight.

## Concrete native integration requirements

Import each authentic container through its original offsets/record identities.
Keep coordinates in authored units for the road query: camRoadMain divides
six engine-unit coordinates by10, and multiplies constructed eye/at by10 later.
Use divided look-target for zones and divided player-target for main path
queries; preserve all later correction query owners and previous destination.

Implement source fan traversal/ties, raw tangent halfspaces, near-normal dispatch,
projection/fraction, side/end caps and nearest-segment ordering. Preserve the
finite duplicate proof's prerequisites instead of inventing a global duplicate
skip. Extra curves and circular dispatch are not admitted by this shipped data;
that does not remove those engine obligations for other assets/full-game work.

Compose current/candidate/destination/origin states and copies through the
manager contract. Retain GP40/44 transition time separately from GP38/3c event
and shift owners. Preserve the camera-before-EVT-before-Mario update order,
which makes the ordinary camera consume retained Mario target fields. Rendering
frames must not define these simulation updates or source clock ownership.

Bind mode1 dolly/event interpolation and retained lens/projection fields after
road construction. The road output's metadata is not automatically live lens
state. Project the original root and preserve source cached-coordinate consumers;
a native screen-bounds or sprite-center substitute changes their policy.

These are documentation requirements, not evidence that Game currently executes
them. Native fixed framing/.07 smoothing still needs explicit replacement and
original comparison traces. Matrix/trig/Gekko estimates/rounding, finite fraction
provenance, caller admission and pause/clock ownership remain numeric/runtime
contracts; an algebraic adapter is not a measured source match.

## Revalidation and open work

Nine existing verifiers passed against the read-only ISO: road_state16 spans/
46 anchors; zone_triangles5/33 plus11 cases/494 triangles; straight_paths7/44/
19 eligible records; segment_dispatch4/35/44 records/220 pairs/10 duplicates;
map_lifetime27/51; moving_shift7/60/six cases; target_shift17/61; event_blends
13/79/13 cases; interpolation13/51/31 cases/75 EVT owners/332 bindings/300 basic
request operands. Their scripts distinguish source pins/inventories and derived
math; these counts do not prove original execution or full body interpretation.
The new metadata inventory independently matches all five existing file hashes/
sizes and verifies zero extras/disables,20 fixed-Y records and all local counts/
indices. Markdown links and scoped diff checks passed. Docs/metadata only;
no native build/test claim, no raw asset/code-body distribution.

OW-CAMERA-01 now registers these previously delivered bounded geometry/lifecycle/
shift/event portions. Remaining: native composition across all44 actual records,
complete selection/correction/provider/library/state/time/FP/alias/lifetime/IRQ
and exceptional domains, original scene admission, room/control/height/pause/
cadence/visual traces, and full-game camera modes/assets. Keep its broad room
tracking/limits request OPEN until that binding is proved.
