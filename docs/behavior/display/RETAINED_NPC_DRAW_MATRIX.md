# Retained original NPC draw matrix

`advanceFieldNpcMatrix` implements the complete local US G8ME01 disc0 revision0
`mtx_setup` body at `8004ca80`, size `2b8`, source SHA256
`e2e76f969f024380c1e3e40f3357607b520687ed28fac9671301da1e78519dac`.
Its callers include ordinary,
translucent, offscreen and blur NPC display callbacks. This is a shared draw
transform dependency, not an activated NPC renderer or a completed callback.
See [yaw and pose fallback](NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md) and
[Main display registration](NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).

The cursor captures the source actor, raw signed history slot bits and providers.
Each acknowledged memory/numeric/SDK primitive advances exactly once. A hold
publishes no output matrix. The final successful root translation publishes the
result and resets the cursor. Providers returning false/empty must have no
effects; mappings and the source producer pass must remain alive across holds.
Changing call arguments while held cannot substitute another actor or owner.

Camera correction is conditional on both main flags `02000000` and `08000000`
being clear. It queries current camera once, calls original angle `8000703c`
with camera c/14/18/20, reloads camera c/14 and actor8c/94 for a second angle,
then revises the single-precision difference. Additive yaw f0 is independently
loaded and revised twice. Inclusive 90..270 sets Z reflection to -1; NaN
excludes this comparison. The matrix starts with SDK identity.

Scale short-circuits X/Y/Z unit checks. If admitted, it retains first X but
reloads Z then Y before `PSMTXScaleApply`, multiplying Z by reflection. Both
negative and positive pivot translations independently short-circuit zero
checks, retain first X, and reload Y then Z. NaN enters the nonidentity/nonzero
transform paths. Negative pivot precedes rotations; positive pivot follows.

Z(f4), X(ec) and Y(f0+correction) rotations occur in that order, each
left-concatenated with the retained matrix. Y rereads f0 after the earlier SDK
children. Addition precedes the radians constant load. Degrees use original
binary32 `8041fd7c`, bits `3c8efa35`, not exact pi/180. Slot -1 selects current
root8c/90/94; every other slot uses low32-bit actor+a4+12*slot addressing,
without a new bounds clamp. A missing mapping holds. Event yaw144, animation
Main/facing, draw scale2 and bounding-box correction do not occur in this helper.

The numeric provider acknowledges current-camera, angle and revise calls. SDK
requests identify original functions: identity8029dd04, scale-apply8029e2d8,
translation-apply8029e264, rotation8029dff0 and concat8029dd64. Matrix payloads
are native bounded locals, not guest stack pointers. Existing finite-domain
native math can satisfy these requests, but its trigonometry and FMA behavior
are not measured Gekko/SDK numeric parity.

Tests compare an explicit source trace, exact SDK operand order, all primitive
hold boundaries in nine graphs, fresh camera/pivot reads, retained scale X,
90/270 boundaries, raw negative/wrapping slots, NaN branch admission, unavailable
providers and final-only output publication. NaN tests use a graph-only recording
SDK provider and make no numeric-rendering claim.

## Next callback boundaries

The independently audited ordinary `npcDisp8004c50c` and
`npcDisp_xlu8004c7e0` share yaw/matrix/darkened-material setup and a conditional
14-pair facing-pose-name swap graph. Ordinary chooses passes from actual current
queue class; translucent runs Main then passes1/2/3. Implement the entire swap
graph and real pose/camera/draw ownership before activating either callback.
Offscreen callbacks draw primary handle104 and publish the synchronized GPU
bounding box to alternate110. Blur makes two fresh camera queries, draws
history4..1 with alpha28/53/78/103 and clears40 then OR-restores saved material
flags, without restoring color. These variants remain separate required work;
this matrix helper never acknowledges them as successful empty draws.

Ignored read-only evidence is under `artifacts/npc-main-parent-evidence/`:
`*-callback-review.txt`, `callback-review-notes.txt` and
`callback-review-hashes-constants.txt`. No original executable bytes are copied
into source history. The request [EVT-01](../../requests/EVT-01.md) remains open.

The complete local ordinary/xlu callback graphs and native VM/shared-pose
alias binding are now delivered in [retained callbacks](RETAINED_NPC_ORDINARY_DRAW_CALLBACKS.md).
This supersedes their implementation task above; actual field Main/SetAnim/draw
producers, offscreen/blur variants, normal Game activation and measured parity
remain open.
