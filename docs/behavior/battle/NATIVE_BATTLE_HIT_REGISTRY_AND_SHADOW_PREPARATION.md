# Native battle HitObj registry and shadow preparation

2026-10-14. Implements the finite downward source contract in
[battle context/traversal](../field/SHADOW_DOWNWARD_RAY_ACTIVE_BATTLE_MAP_AND_FINITE_ACCEPTANCE.md)
for native setup4's selected bti_01/stg_00_1 initial snapshot.
This is an explicit constructor/publication adapter, not a live original-game
registry capture or complete bmapLoad/hitMain/outer-loop implementation.

`tools/battle_hit_assets.py` verifies the original US map bank hashes, walks
each collision root in child/sibling order, and imports all five initial slots
and166 strip-derived triangles. It retains source winding and engine units
under parent Scale10; no render-scene offsets, geometry reduction, native-scale
round-trip or field floor synthesis. Current collision SRTs have zero rotations;
other rotations are explicitly rejected by this importer. Initial flags0 come
from _hitEnt's initializer. Mesh centers use original bbox midpoint before one
single-precision.1 expansion of the maximum; spheres use transformed expanded
maximum and a declared native square-root approximation. Expansion is never
applied twice to already-generated packs. Resources/rights are in ASSETS.

`BattleHitRegistry` loads privately compiled metadata/geometry transactionally,
then publishes the source-order active chain: map/slot order, parts!=0 and
runtime flags&81==0. Flags change eligibility on explicit publication, not an
implicit rebuild during a ray. Attribute and tagged-MOBJ flag values stay live;
unresolved tagged owners prevent admission of the whole atomic native query.
No unknown owner becomes an accepted/rejected filter or a guessed farther
floor. Unknown context, transforms, generated slots or writer history must call
invalidate and remain unavailable until an explicit producer restores the owner.
The current snapshot API does not implement all source mutation/deletion/binding
services. A future such binding must preserve their publication/lifetime order.

The ray uses its own original arguments and this selected battle owner. It
never calls field groundAt/groundWithin or reads B.floor. The supported domain
is exact direction(0,-1,0),nonnegative finite length/coordinates and finite
arithmetic. Ordinary attr4 is accepted; tagged objects reject only flags800.
The fixed initial midpoint/half-length sphere gate precedes source triangles;
cull1 requires positive normalY and admits plane/edge equality. Other culls
use the distinct two-sided plane/edge gates. Strict far-end exclusion and
earlier source ties are preserved. Miss/unavailable leaves caller hit output
untouched; no empty chain generates a plane. Nonfinite intermediates hold
rather than become fabricated geometry. Native map/slot/triangle caps are
resource admission limits, not measured original allocation limits.

Runtime edges/normal use source separate binary32 subtraction and exact FMA
operand/reduction forms. Dot/distance fuse X onto pre-rounded Y product then
add rounded Z. Cross Y/Z retain negated FMA forms. Normal magnitude instead
fuses Z onto rounded X square then adds rounded Y square. The native backend
uses sqrt rather than the original frsqrte estimate/Newton sequence and one
rounded inverse followed by component multiplies. Numeric bit, exceptional
float and raster parity remain unverified; this is not a PPC instruction clone.

The trio runtime imports this owner alongside its original stage maps. Its
actual captured shadow callback binds the independent query when no explicit
external test provider overrides it. On completed hits the existing collision
cursor builds the source record matrix and ORs prepared2 after all stores.
The camera2/class5 continuation then holds on the unbound mutable profile/image
suffix; completed preparation cannot replay on that later hold. Source-shadow
geometry/bounds/cylinder/image/GPU services and full original map/history/field
return remain separate requirements. No original dialogue/story was changed.

Tests cover source order/ties, explicit flags publication, strict endpoints,
one/two-sided coplanar and winding behavior, attrs4/MOBJ flags, source sphere
gates, empty/unknown owners, failed/trailing-byte imports, actual original maps,
all three finishing queries and source callback integration. Host services and
isolated native checkpoint Bero resolution are explicit test fixtures; neither
certifies original history, runtime/raster/audio parity or hardware performance.
