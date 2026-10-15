# Field setup, actor cursors and shared pose views

This guide extends [map lifecycle](map-lifecycle.md) and [NPC/object ownership](npc-objects.md)
with the committed setup-world bridge, five actor callbacks and fresh shared pose
namespace. [Pouch/selection](pouch-selection.md) covers its inventory/UI dependencies.
Source evidence remains under [behavior contracts](../behavior/README.md).
This documents current native composition without claiming complete original
startup, room Main, rendering or scheduling equivalence.

## Setup admission and event lifetime

`FieldNpcSetupBinding` requires the same initialized registry owner, external
memory facade and VM. GP+0x168 must identify retained F; the raw GP context word
is captured and must match exactly on later invokes. Binding does not create
actors or acknowledge missing resource services.

Script admission compares every live word to immutable source and retains the
constructor scanner's unconditional next-word prefetch, including after END.
Native limits65536 words/16 labels and signed-count checks exclude unsafe inputs.
A readable script is not proof that every runtime callback is bound. Original
adjacent words must exist; no invented END or padding replaces scanner context.

Deletion admission walks waiting-child and active-brother graphs, validating
parent storage without deleting it. Stale/zero IDs need no deletion. Caller,
unresolved pointers and cyclic/duplicated paths are refused. This checks a
prospective source operation without performing it or repeatedly deleting during
a native allocation hold.

Row admission checks original table words, owned actor storage, descriptor/pose,
script scans, deletion graphs and reached battle/RNG services before effects.
First pass can skip absent actors. A later poll can return at the first live init,
so it does not repeatedly admit unreached regular scripts. The world bridge
preserves fresh F/mode lookups, source string copies and actual numeric battle
requests. Providers and immutable storage remain stable across cursor holds.

## Five actor leaves and checked output

Original callback tokens select flag on/off,position,get-position,direct yaw and
camera-dependent left/right yaw. The cursor binds owner,event address/ID,function
and arguments. Completed argument resolutions survive holds; unfinished argument
work retries. Captured raw output destinations differ from resolved input values.

Flag selector0 clears,1 sets. Other selectors still resolve the name but perform
no actor flag access. Position first reads the original unused zero vector,
updates previous root, then reloads current XY as a pair and Z later. The extra
reads/stores are observable to services and cannot collapse into one Vec3 update.
Direct yaw stores its supplied float without wrapping.

Left/right requires published camera4 yaw and original100/5 constants. It applies
revise(FMA(100,yaw,5)/100), truncates, then takes signed remainder of n-90 or n+90
by360. Negative results remain negative. Replacing this with fixed90/270 or an
always-positive modulo changes source behavior. Host/ARM numerical adaptation
still does not establish Gekko bit equality.

Get-position captures one axis before its checked setter. A held setter retains
that value; successful earlier outputs do not repeat, while later axes read
fresh values. Each setter defers at most one guest word and publishes only when
its routing/reads/conversion admit. A legitimate no-write destination can still
complete. This is per-output admission, not a transactional XYZ store or a claim
about arbitrary raw ABI aliasing outside the admitted stable-operand domain.

Exact me/master/slave names use retained actor ownership. Party/extparty/Mario
special values are sentinel words, not dereferenceable NPC records. A no-access
flag branch can resolve them; actual NPC access must prove owned storage. Active
event/context and camera publication are rechecked by the binding. The area VM
retains actor cursors by real event ID only while held, clearing them on source
restart, completion or retirement/ReInit.

## One original pose namespace

Fresh player resources retain token order a_mario,a_mario_r,e_mario,c_pkr and
real MODEL/TPL/descriptor metadata. Their original descriptor-zero poses are
M_Z_1,M_Z_1R,M_S_5,PKR_Z_1. Shared mode2 entry uses these; legacy visible workspace
seeds M_S_1/M_S_1R/M_I_5/PKR_S_1 apply only to the older independent path.
Missing original data does not select a substitute animation.

`FieldPartyRegistry` creates one shared `BattlePosePool` before any role view
exists. NPC field wrappers and battle consumers receive that same identity.
Returned handles belong to its256-slot global namespace, not actor indices or
independent field-local pose numbers. Optional standalone construction remains
for legacy callers; a live namespace is not migrated/replaced.

`FieldPartyPose` exposes flags,material/light flags,timestamp and CPU shape as
views into actual pose storage. Binding a scalar wrapper changes its target
without overwriting defaults. Assignment copies value into its existing owner;
it does not copy another wrapper's binding. Timestamp writes low word then high.
Shape is a reference, not a periodically synchronized duplicate. Resources remain
immutable shared objects. Views must retire before manually releasing their own
handles; wrapper destruction is not original AutoRelease.

Registry copying clones the complete namespace once and retargets all role views;
individual wrapper copying has its separate whole-backing copy behavior. Existing
external NPC/VM aliases are not reconstructed by this operation. Native record
buffers and shared pose lifetime have separate owners. Resource caches retain
their canonical identity, including atlas layout; field CPU resource admission
alone does not establish matching GPU residency.

The initial global W/file/heap/scratch graph, complete descriptor census, original
room/NPC Main/query/facing/collision/render services and authored battle entry/
teardown remain separate requirements. Sharing CPU pose handles does not complete
them or prove physical 3DS performance/pixel/float fidelity.
