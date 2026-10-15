# Retained original offscreen and blur NPC callbacks

`advanceFieldNpcDraw` now implements the complete local US G8ME01 disc0
revision0 `npcDisp_offscreen8004bf28/30c`,
`npcDisp_offscreen_xlu8004c234/2d8` and `npcDisp_blur8004bc3c/2ec` bodies.
Their source SHA256 values are respectively
`410d81a182078f19d7edf646996be9548ba354e7209175dbc1f8631cb8c25dfb`,
`724f85864320cc687c07c1e7cc53635d01b6a770389fa13a535188472ac219db`,
`30cf24b31ee5c059dd8ca8ce3f2ac3b4c7b2b510dbd5f9ba0565fc8c417c8119`.
These join the existing [ordinary/xlu callbacks](RETAINED_NPC_ORDINARY_DRAW_CALLBACKS.md)
and [shared matrix](RETAINED_NPC_DRAW_MATRIX.md). All five callback identities
registered by the retained NPC Main display/shadow graph now have complete local
controllers. Native field animation/camera/render/GPU providers and normal Game
activation remain incomplete; local completion requires actual acknowledgements.

## Offscreen coverage

Both offscreen variants first call sysWaitDrawSync80005ce4 and
GXClearBoundingBox802b69d0. They then execute exactly their ordinary/xlu body,
including original yaw, matrix, material, facing-name and queue-class/Main/pass
semantics. Draws use fresh primary pose handle104. They do not substitute
alternate110 for the primary model or omit the class-dependent return paths.

Every body exit reaches the footer, even an ordinary queue class outside0..2:
wait for draw synchronization again, read GPU bounding box802b6a08 into four
independent unsigned16 locals, load fresh alternate handle110, then forward
(handle,left,top,right,bottom) to offscreenAddBoundingBox80072f38. Original
left/top/right/bottom argument order is retained. There is no new active/-1
handle gate or model-space bounding-box approximation. Copied bounds persist
across a later handle-read/callee hold. The request/response `bounds` arrays
replace only original guest stack locals; they are not source guest pointers.

Native providers must own actual GPU draw completion/coverage and the actual
offscreen work/texture lifecycle through the entire callback. The audited
AddBoundingBox leaf rejects unsigned16 R<=L before owner lookup, selects actual
GP14 bank/pool, checks active1, conditionally expands and rounds coverage using two fresh
render-mode queries, merges each edge separately and marks dirty2. Later Main
and Disp reset/snapshot/copy this coverage to the offscreen texture. Those
services remain explicit; this implementation never invents coverage or treats
missing GPU/raster ownership as an acknowledged operation.

## Blur history and material state

Blur retains the supplied callback camera ID (source r3), actor and owner.
Existing ordinary callers need no camera argument; blur callers must supply the
actual dispatched ID. It reads yaw144, revises its negation and retains incoming
A. A fresh handle104 gets material flags8004069c, retained for the final OR.

It queries the supplied camera, reads fresh camera yaw114 and evaluates the
original sine polynomial, loading original constants in instruction order.
OffsetX is fresh source TWO times that sine. It independently queries the SAME
camera ID again, reads that returned camera's yaw114 and evaluates the cosine
polynomial. Cosine is negated before another fresh TWO load; handle104 is loaded
before final Z multiply, then animation Main runs once. The two explicit camera
queries are not collapsed, and later matrix correction queries remain separate.

`advanceFieldNpcBlurTrig` retains every literal read and arithmetic boundary.
The graph loads tau,360,pi,coefficientA,coefficientB,ONE, with tau*yaw before pi
and division after coefficientB. Source single quadrant reduction is preserved;
there is no general periodic wrapping or finite-only validation. Final quadrant
subtracts threeHalfPi before loading halfPi. Polynomial operations preserve
single-precision square/FMA/FMA and sine's final multiply/sign. Coefficients and
all thresholds are original DOL bytes. Native std::fma is an explicit numeric
adapter; exhaustive original Gekko numeric parity is not measured.

History slots4,3,2,1 each run the full shared matrix helper. The result receives
SDK TransApply with retained X/Z offsets and fresh source ZERO for Y. Each sample
reloads handle104, ORs40, reads original color WORD8041fc68, replaces only its
alpha with28/53/78/103, reloads handle104 and sets that copied event color.
Passes1,2,3 each reload handle104 and original draw scalarTWO while retaining A
and the sample matrix. No slot0 draw occurs. Four samples produce twelve draw
requests, not four animation Main calls.

Footer loads fresh handle104 and clears40 with FlagOff, then reloads handle104
and ORs the originally saved flags with FlagOn. It does not assign saved flags
or discard flags added during rendering. The final event color remains alpha103;
there is no color restore. Native binding reads/writes actual shared pose e8/f0
without adding active/resource gates, including inactive mapped slot255.

## Holds, validation and activation boundary

Every successful source read, numeric/SDK/callee effect is retained across holds.
Callback/actor/camera/providers cannot be replaced by later invocation arguments.
SDK offset translation keeps a stable native payload across retries. All storage
is fixed-size; no per-call allocation was introduced. This is source-order
mechanics separated from 3DS rendering services, not a new rendering-frame clock.

Tests exercise all offscreen queue-class exits and xlu pass routing, raw unsigned
bounds and fresh alternate handle, every primitive hold in six offscreen and
four blur graphs, all twelve history passes, fresh handles/scalars, two distinct
explicit cameras plus four independent current-camera matrix queries, flag
mutation preservation, alpha/color retention, quadrant/signed-zero/nonfinite
trig graphs and original literal read order. Native fixtures use actual VM and
shared raw pose aliases; GPU synchronization/coverage/Main/draw remain explicit
fixture services and do not prove normal field acceptance or hardware speed.

Read-only listings/notes and hashes are ignored under
`artifacts/npc-main-parent-evidence/`, including
`offscreen-blur-exact-review-notes.txt`. Only unchanged color/trig data are
added to private extraction, not executable PPC or fabricated story content.
[EVT-01](../../requests/EVT-01.md) remains open. Next compose actual field
animation Main/SetAnim/draw and camera/queue ownership, plus genuine offscreen
coverage/texture services, then activate this path in normal Game. Original
runtime/numeric/visual comparisons, live field smoke admission/occlusion and
New 2DS XL performance remain required.
