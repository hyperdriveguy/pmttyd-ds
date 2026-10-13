# Mario presentation evidence

Ordinary Mario facing/mesh presentation, source sparse animation playback, idle/key service ownership and grounded shadow request/projection. Split out of the [mechanics ledger](../MECHANICS.md) on 2026-10-12; sections are verbatim.

## 2026-10-12 — Ordinary Mario facing and original mesh presentation

Native53d573b follows overworld/OVERWORLD_MARIO_DISPLAY_DIRECTION.md,
OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md and
OVERWORLD_DISPLAY_TO_MOVE_HEADING.md. Ordinary motion IDs0/1/2/3/10 request
M_S_1/M_W_1/M_R_1/M_J_1B/M_J_1C; motion11 retains the prior pose. Identical
selected names preserve their upload/start time. Rear selection uses retained
facing minus camera offset at inclusive70/290 sectors before display turning.
The source target0/180 turn flags and asymmetric extra callback crossing are
retained. Drawing uses display-minus-offset yaw and local Z reflection at
inclusive90..270; it does not mirror X or texture coordinates.

Wall preparation saves strictly larger requested combined movement S/H with
count14; equal/slower requests count down, then replace retained S/H. Neutral
and airborne paths clear stored speed while retaining heading/count. Requested
norm is taken after slope/air preparation, before collision rejection/sliding.
Direction rate reads prior facing+1a0, not new travel+1a4; the old documentation
owner label was corrected against the actual c03e01a0 load. Fresh facing0 gives
first right-running native travel0.026775, then0.0225. These are source-derived
adapter regressions, not Dolphin measurements. Base speed0 uses quantized
wrap(270-display+cameraOffset) for the documented standing, support and head
probe domains. Water/void admission clears base speed as documented.

Full original frame0 mesh coordinates now feed the native ordinary renderer.
Source2x model scale with0.01 engine conversion replaces provisional billboard
height on this path. Native998ec61 binds the newly recovered group-kind,
parent-scale, pivot and sibling traversal branches in asset conversion.
Source-compatible branch/order selection is distinct from numerical parity:
host NumPy matrices and trig remain adapters; Gekko/FPSCR/GQR0/libm composition
has not been compared against original runtime. No original Dolphin observation
was made. Animation interpolation/loop contracts were obtained from the
requested documentation agent, but frame advancement remains unbound. Low-HP,
special flags/pivots, recovery, partners, battle presentation and shadows remain
on incomplete paths; ordinary mesh success does not close those domains.


## 2026-10-12 — Ordinary source sparse animation playback

Native7102aaf binds OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md and
OVERWORLD_MARIO_ANIMATION_SPARSE_UPDATES.md to evaluated ordinary geometry.
Frame time is float(whole-ms current minus start)*60/1000 with default rate1;
current/start use the existing simulation-tick-to-ms GP bridge, not render
increments. Original descriptors wrap exact loop end to start; nonloop jump
end stays at end. Stateless evaluation rebuilds original bases and applies
passed full keys, then one fractional key. Fractional position updates are
linear q/16; node updates use source-duration Hermite tangents. OUT<=-89 or
>=89 holds the full scalar until key passage. Tangent table178 entries uses
source radians word3c8efa33; host libm/f32 rounding are numeric adaptations.
Texture bytes change modulo256 only at passed keys; no partial texture blend.

Animation runs through actual original joint/transform nodes and current-group
sibling parent convention, then source model scale/root/yaw/Z reflection.
Original vertex bindings move with evaluated nodes/positions; stepped texture
bindings select original atlas cells. Native tests preserve static authored
rear jump geometry where its source updates do not move visible vertices.
Queued pose restart was corrected from the agent's2f9f922 source addendum:
marioChgPose compares requested front name with CURRENT name18, including R.
A changed front request followed by rear mapping can force restart even when
the final selected rear name equals its prior value. Same current front name
does not queue. Tests cover both motion11-to-idle traces and repeated renders.

143 independent source-derived host geometry samples compare with maximum
model-coordinate difference6.79493e-06, including fractional texture-step
boundaries. This is decoder/math agreement, not original Dolphin/Gekko parity.
Isolated Azahar captures show native running/jumping plus save/traversal checks.
Low-HP/special/recovery/partner/battle pose ownership,original shadow/material/
color/normal services and original frame observations remain incomplete.

ID1 static texture/light/color/fog addendum (2026-10-12):fourteen used local
request/query bodies are specified in the
[display index](../behavior/display/ID1_MAP_MATERIAL_AND_TEV_STATUS.md#id1-used-texturelightcolorfog-local-contracts-2026-10-12).
No original execution,Dolphin observation,measured pixel/light curve or hardware
performance was added. Host exhaustive65536 byte-pair proof establishes only
the decoded integer floor(product/255) reduction. GQR0/FPSCR/libm/GX/resource/
source lifetimes and original image comparisons remain open. make test78 CTest/
90 Python and make all pass;native tests/build are not original-runtime oracles.

ID1 ordinary TEV/character-shadow evidence (2026-10-12):original Mario source
24 draw records all select mode0;see
[base/source](../behavior/display/MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md).
This is static source metadata,not an original frame or pose-flag execution
observation. Scoped base/constant/character-shadow/callback/count request blocks
and four source getter bodies are documented;remaining shader branches and
actor-shadow ownership/GX/numeric/image behavior remain open. No new Dolphin
observations or measured shadow/lighting curve. make test80 CTest/90 Python and
make all pass;these are native validation,not original runtime parity.


## 2026-10-12 — Source ordinary idle and key service ownership

Native778add7 binds OVERWORLD_MARIO_LOW_HP_IDLE.md: entry HP0 requests I_S;
lowHP<=5 with admitted key requests front-only S3; exact I_Y is retained rather
than scheduled. Signed refresh counter increments and admits live HP refresh
only after >sysMsec2Frame(1000). Low-HP entry and refresh differ at HP0; healthy
phase0 recovery resets the refresh counter, with source current-name request
comparison and subsequent rear conversion. Breathing counters request225/226
without an invented animation restart when counter/start-frame resets alone.
Display4 is preserved by ChgPose and does not suppress the actual display turn;
display100 does. These flags are distinct from input/key ownership.

Healthy idle phase0 reaches1 after3000 ordinary60Hz callbacks, then the next
callback requests effect I_5 and falls through to phase2 count49. Phase2/3
strict negative boundaries select normal S1 and later effect N_5A. Phase4 keeps
source independent sleep/sound counters and cancellation/HP rereads. Neither
I_Y nor a fabricated yawn/rear asset is scheduled. AdjustMoveDir snaps literal
display90..270 to180, otherwise0, then publishes source quantized facing/travel
and display/target. Event requests preserve start frame; ordinary requests and
rear-name writers reset it. SetLocalTime's frame-to-integer-ms conversion is
used only on a queued upload; a counter reset alone does not restart the clock.

Portable FieldKeyServices preserves ordered party lookup, sequence/input byte,
two conditional ship calls, roll/slit admission, null-partner early return,
Christine/Bobbery/Yoshi/partner flags+motion and held-item gates. Native current
follower/party/status inputs remain explicitly editable harness adapters; these
are not recovered original initialized values or complete partner ability
bindings. Live progress HP is the current native pouch adapter. The post-tick
surviving-idle callback bridge still does not execute every original interim
callback/command/NPC/event admission; tests cover its current ordinary slice.
Original idle sound IDs are retained in fieldIdleSounds, with macro playback,
handles/cancellation and source positional arguments awaiting separate binding.
The source motion dispatcher/service/story lifecycle remains required beyond
this local controller. No original Dolphin or hardware observation is claimed.

ID1 fixed TEV static addendum (2026-10-12):
[mode12 caller graph](../behavior/display/MAP_TEV_MODE12_CALLERS_AND_LITERAL_EVIDENCE.md)
binds two original renderer/material calls with N2/M12. Fixed modes1..7/12
now have exact stage tables. No original execution,Dolphin observation,GPU
arithmetic measurement or image parity added. Static immediate/call inspection
is not an executed shader oracle. make test81 CTest/90 Python and make all pass;
source/GX/numeric/lifetime and full-game rendering remain incomplete.

ID1 time-driven TEV static addendum (2026-10-12):
[pulse](../behavior/display/MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md) consumes raw GP+1c;
[scroll](../behavior/display/MAP_TEV_MODE9_SHARED_SCROLL_AND_MATRICES.md) mutates four
shared phases on every material call,with strict single-wrap arithmetic. Startup
bits are metadata,not measured clock cadence/scroll speed or immutable state.
No original execution,Dolphin observation,pixel comparison or hardware timing
added. 82 CTest/90 Python pass;3DS build fails concurrent renderer enum arguments
as recorded in PROGRESS. Source/GX/libm/lifetime/full rendering remain open.

ID1 time-TEV validation resolution:renderer enum failure above is RESOLVED
by its owner's typed-source change;make all rerun passes (log
/tmp/id1-tev-time-build-rerun.log). 82 CTest/90 Python pass. These native results
remain separate from original clock/source/GX/libm/image equivalence.

ID1 projected-TEV static addendum (2026-10-12):
[flag0x400](../behavior/display/MAP_TEV_FLAG400_TWO_STAGE_PROJECTION.md) rewrites
preceding-stage output and adds two stages;
[flag0x20000](../behavior/display/MAP_TEV_FLAG20000_PROJECTED_SHADOW.md) retains a
second live texture getter result and ignores config scalar return. Complete
local request/query behavior is static deduction,not a Dolphin/GX/image or
actor-shadow runtime observation. make all/separate90 Python pass;make test
configure missing concurrent fixture recorded in PROGRESS. Four remaining
middle branches/source/lifetime/GX/full rendering remain open.

ID1 projected-TEV host resolution:fixture arrived;make test rerun passes84
CTest/90 Python,log /tmp/id1-tev-proj-test-rerun.log. Earlier configure failure
above is RESOLVED. make all passes;no original execution/image parity implied.


## 2026-10-12 — Grounded Mario shadow request and projection

Native357a656 binds OVERWORLD_MARIO_SHADOW_REQUEST_AND_GROUND_POLYGON.md.
Ordinary type0 admission uses retained support pointer1e8, not airborne motion.
Display1/1000 or main02000000 suppress the request; null support selects type2
with the source height attenuation/minimum rules. Ground projection performs a
fresh shadow-filtered downray instead of copying Mario support point/normal.
Static map attr4 is admitted, source cull is retained and returned point/normal
are not Q1000-rounded or renormalized by the shadow leaf. Tagged runtime object
flags remain a dependency: a nearer unresolved owner prevents claiming that a
farther map floor won. Existing supported original map packs contain no such
high-bit owners; named runtime providers remain unbound.

Preparation retains the literal3x4 matrix, hitP+normal offset and prepared-bit
lifetime. Bypass flag/type3 helper retains the source rootY+1.5 route. The source
six-point strip/four triangles draws black alpha110 with no texture/depth writes.
Hidden display1 now also prevents field Mario's actor draw. Native single-record
render ordering and float/ray/PICA depth/TEV mapping are stated adapters; source
full display queue/sort, live global color/height writers and original numeric/
visual comparison remain open. Air type2 still uses the earlier provisional
ellipse while its original destination-alpha volume backend is unfinished;
source request math alone does not establish that ellipse as faithful.

Shadow follow-up d8ec56f: source camera4 callback AlphaUpdate0 is retained by
the grounded pass using native RGB-only writes. This corrects the initial
color-mask implementation without assuming initial EFB alpha0/255. Air GX
passes explicitly alter alpha masks; their finite graph is now documented but
entering frame/camera2 copy/clear state still needs native binding.

ID1 terminal-TEV static addendum (2026-10-12):
[color/context branch](../behavior/display/MAP_TEV_TERMINAL_COLOR_AND_COSINE_ALPHA.md)
and [pulse texture](../behavior/display/MAP_TEV_ANIMATED_PROJECTED_TEXTURE.md) close
local raw-clock/cosine/single/FMA/truncation/color/texture/matrix requests. Byte
multiplication floor255 host proof is bounded arithmetic,not original execution.
No Dolphin trace,measured period/fade/texture image/GPU parity added. 84 CTest/
90 Python pass;3DS build passes after native owner's RGB enum fix. Two depth
branches/source/libm/GX/Ortho/LookAt/lifetime/images remain incomplete.

ID1 complete local TEV request addendum (2026-10-12):
[local graph/boundaries](../behavior/display/MAP_TEV_LOCAL_REQUEST_GRAPH_AND_BACKEND_BOUNDARIES.md)
now indexes all request branches. Depth source returns inverted alpha-scaled
RGB with alpha0;initial two configs route3. These are static binary/data results,
not an original capture or scene/clock/GX image comparison. Host65536 byte-product
proof supports arithmetic only;composed2927-word coverage supports manual local
review. Final86 CTest/90 Python/make all pass after native fixture changes;
earlier missing/assertion failures resolved. No ID1 native/test/profile/save
changes. SDK/source/lifetime/rendering-system/original-runtime closure remains open.

ID1 map-material static addendum (2026-10-12):
[map source inputs](../behavior/display/MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md)
now specify material count/mode and object flags/model address,distinct map
light/texture/matrix/source behavior. Name miss/zero-matrix gates are local code
facts,not witnessed malformed source scenes or measured trajectories/pixels.
No Dolphin/original execution/images added. 87 CTest/90 Python/make all pass;
loader/caller/source/numeric/GX/lifetime and full-game rendering remain open.
