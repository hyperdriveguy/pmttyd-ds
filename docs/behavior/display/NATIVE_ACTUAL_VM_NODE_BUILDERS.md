# Actual-VM Joint and Transform node builders

US G8ME01 disc0 revision0; static read-only source audit, not original execution.
Joint8003e2a8/544 SHA256
`4df168420dc7e8be6285bad580e6c0bce9c8e996e9d9f7437b96a8100dba4600`;
Transform8003e7ec/540 SHA256
`3bf7f5b53186d4ae7ba5acc160f9340f54fc14c1ebf62dd2e808482937485972`.
Ignored evidence: `artifacts/npc-node-push-ordered-evidence.txt`,
`artifacts/npc-node-sdk-adapter-evidence.txt` and original per-body listings under
`artifacts/npc-main-parent-evidence/`. Numeric/timing/pixel parity is unproven.

`resumeAnimationSourceNode` implements both complete local bodies using raw VM
node addresses, incoming parent floats, live literals and actual modeling globals.
[Joint](ANIMATION_JOINT_NODE_MATRICES.md),
[Transform](ANIMATION_TRANSFORM_NODE_MATRICES.md),
[stack/SDK](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) and
[rotation](ANIMATION_SDK_AXIS_ROTATION.md) remain authoritative source contracts.
This replaces neither group control nor the unfinished GPU/material/effect work.

## Captures, requests and dispatch

Both prefixes read translation X, live ZERO and TWO, primary X/Y/Z rotations,
perform the reached single multiplications interleaved with translation Y/Z,
then capture scale. Joint captures secondary XYZ next:12 node fields total.
Transform instead captures both pivots and offsets:21 fields, never N24/28/2c.
All precede the first SDK child. A node24 snapshot or sorted field copy changes
read/hold boundaries. Source float comparisons preserve unordered/NaN inequality
and signed-zero equality; no blanket finite input filter belongs to the parent.

Translation uses initial ZERO. Joint's parent compensation and own-scale gates
retain their asymmetric last-axis equality. Parent zero fallback uses a fresh
ZERO and ONE, with one fallback for all three axes. Own-scale has no fallback.
Transform's ordinary scale gate differs. A zero pivot suppresses its entire
offset block; nonzero pivots retain the exact six SDK requests and distinct
FIRST/SECOND input-output aliases. Negation flips the sign bit, including −0.

Identity for rotation is unconditional. Six Joint or three Transform axis steps
read fresh ZERO each time; only an admitted angle reads DEG and issues RotRad /
Concat. Order is secondary Z/Y/X then doubled primary Z/Y/X for Joint, doubled
primary Z/Y/X for Transform. Node values and incoming parent scale parameters are captured before SDK effects.
They are not reread through the caller request, including when that request is
mutated by a callback during the same resume invocation.
Requests identify durable local scratch slots rather than fabricated guest stack
addresses. SDK children own their partial effects; holds never reconstruct an
already admitted request or replay completed calls/stores.

Both bodies read/increment/store modeling level, then publish its pointer before
dispatch. Read the original jump-table word after those stores. All16 Joint and
8 Transform routes preserve exact local products and output aliases. An unknown
captured target holds at the native compiled-code frontier; restoring the table
cannot undo a taken branch. A missing table word may be supplied before capture.
This is an implementation admission limit, not an original validation branch.
Finally reload level after local SDK calls and request Concat(previous guest
stack,selected local,current guest stack). Do not reuse the earlier level or
modeling pointer, republish it, or pop. Arithmetic on addresses/levels wraps32.

## Native CPU and lifetime binding

[Group binding](NATIVE_ACTUAL_VM_GROUP_TRAVERSAL.md) now owns an
`AnimationNodeBinding`. If an explicit backend node provider exists, it remains
exclusive, including on false; otherwise the actual CPU node implementation runs.
The same VM/current/lease/source-owner guards apply through Inner and Draw. Fixed
node, local-matrix and SDK snapshot storage is reused; no per-node VM, mesh or
heap frame is constructed. GPU/cache/material/Vivian/render remain explicit
children; a completed CPU matrix is not a rendered NPC or queue completion.

Native Multiply/Divide narrows to binary32 and admits finite reached operands/
results. Unused NaN fields can still pass source gates that do not consume them.
Native SDK constants are distinct from node constants and remain fresh mutable
VM reads. Identity/Trans/RotTrig read SDK ZERO then ONE; Scale reads only ZERO.
Owned local matrices cannot alias guest constants or source stack addresses;
the synchronous local SDK result is an opaque native child. Explicit slot
identities still preserve source alias requests between local calls.

RotRad uses double sin then double cos, each explicitly narrowed to float,
followed by SDK constants and axis folding. This matches wrapper round points;
newlib/host trig is not measured original libm. Concat starts with a narrowed
multiply followed by ordered std::fma lanes, retaining the extra tail products
for columns2/3. GQR0/FPSCR/NaN/subnormal/exception/libm parity remains open.
Finite adaptation holds retain captured operands; modifying their old sources
does not undo an already captured unsupported numeric value.

SDK Concat snapshots input pairs in exact order A00,B00,B08,B10,A10,tail,
B18,A08,A18,B20,B28,A20,A28 (hex offsets). Tail is live pair8041e458.
All input values precede the first output store. Guest output pair order is
00,10,08,18,20,28, not sequential Copy order. Complete raw 8-byte admission
prevents a half pair from retiring; committed prefixes survive unavailable later
pairs. Input/output physical aliases retain snapshot semantics. Semantic pair
mappings without complete raw backing hold; this is a native storage domain.

## Original data and validation scope

The private boot image now imports unchanged Joint/Transform dispatch tables at
80306698/40 and803066d8/20, SDK tail8041e458/8 and SDK ONE/ZERO80429358/8.
Existing node literal spans remain unchanged. These112 initialized DOL data bytes
add48 PMEVT directory bytes and JSON provenance; no executable code is imported.
Converter hashes and strict runtime layout pin ownership. Data stays mutable
once cold startup imports it; no duplicate ad-hoc constants are mapped.

Host fixtures exercise all24 dispatch masks, every parent primitive hold,
source-derived golden dispatch/alias tuples, live literal mutations, NaN/±0
branches, pivot suppression, stale/missing targets and fresh final levels.
Native tests check known matrices and double-trig rounding, reached-only NaN
admission, mutable SDK constants, partial pair repair, interleaved capture
frontiers and physical input/output aliases. The actual original Mario group
graph executes the new node CPU owner under retained leases, with explicit GPU/
cache receipts. Those receipts do not establish original pixels or normal Game
NPC activation. Physical New2DS XL performance remains unmeasured.

Next: actual renderProc/materialProc/Vivian/callback/texture/FIFO and Citro3D
owners, any remaining reached source data, linked Draw and ordinary Game camera/
queue/offscreen scheduling. Full original events/NPCs/objects/areas remain open.
