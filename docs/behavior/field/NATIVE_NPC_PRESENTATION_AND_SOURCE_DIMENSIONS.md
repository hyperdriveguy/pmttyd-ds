# Native ordinary NPC presentation and source dimensions

2026-10-13; bounded original US early-Rogueport implementation. Sources:
[ordinary NPC matrix](../display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md),
[dimensions/anchor/registration](../display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md),
[Main facing](../display/ANIMATION_MAIN_CAMERA_FACING.md),
[Inner facing](../display/ANIMATION_INNER_DRAW_FACING.md),
[draw admission](../display/ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md).
This does not claim complete npcMain or original SDK/runtime numeric equality.

PMNPC005 retains previous model/actor fields, adds original resource+c4 draw
mask to the model row (100 bytes), and source width/height plus case-sensitive
talk-pose name to actor rows (196 bytes). Versions1..4 remain readable without
inventing missing source metadata. NPC dimensions are separate from render scale;
objects require zero/empty NPC-only fields. Malformed input preserves the prior
catalog atomically. Static Chu and Frankly source tribe widths/heights are30/30;
their talk poses are CHU_T_1/T_1. Original root/source keys and unchanged US talk
payloads/story branches remain. Native early story envelope and A proximity/priority
are still coverage/arbitration adapters, not recovered general source rules.

The builder reads only source DOL data for tribe fields/name pointers and the
91-float system angleABf table at803041e0. Ignored `npc-angle.bin` retains the
original table through endian conversion; ignored catalog JSON records addresses,
source hashes and dimensions. System angle uses source minor/major ratio*45,
rounded table index from2*ratioAngle+double0.5, table product and quadrant routes.
This is distinct from Main/Inner's atan2 request and constant57.29579162597656.
Zero/cardinal and quadrant tests use original data; approximation bounds against
host atan2 are test bounds, not original measured accuracy or SDK parity.

Portable NpcPresentation requests camera correction unless NPC flags02000000/
08000000 suppress it, then composes source scale/reflection, negative pivot,
left-concatenated Z/X/Y rotations, positive pivot and root translation. Extra
matrix Y rotation and event yaw are separate inputs. Renderer ordinary defaults
are unit scale/zero pivot/additive rotations; script actor flags supply current
special correction masks. Unsupported scale/rotation/flag producers remain open.

Incoming animation angle is reviseAngle(-eventYaw). Ordinary source Main publishes
the facing sign before Inner reads it. Inner makes separate current-camera reads
for classification and front/rear target, unwraps retained angle and performs
one fused0.1 recurrence per admitted native draw, or source mask100 snap. It then
applies retained rotation, previous Main sign's Z reflection and the separate
uniform source draw scalar2. Constructor flags9 and angle0 reflect the stable
fresh NPC Entry/PeraOn path; native sign0 is only an unused sentinel until Main,
not a claimed source Entry initializer. Tests pin Main-before-snap ownership,
fresh camera call count/order, cardinal/quadrant data and root/scalar results.
Full original display queue/pass cadence and SDK matrix/trig/rounding remain
dependencies; local host/ARM operations adapt those requests.

Host-testable NpcPoseClock owns pose/restart state, request generation and
clock reset/elapsed conversion. Per-room retained state is keyed by source actor
key; pose clocks use simulation
milliseconds and reset on source pose requests, independent of render cadence.
MessagePlayback publishes Talk/Stay request generations/timestamps at documented
countdown and terminal/retained-stop branches. Ordinary Chu follows these to
source CHU_T_1 or stay pose, including same-name forced restarts. Other scripted
pose producers still use their current scoped event adapters. This is not the
complete NPC registry/name/flag200 routing service.

Native animation lookup is case-sensitive, matching animPoseSetAnim: an unknown
name selects the bank's original descriptor0. The local subset converter reads
the original descriptor directory at header+1ac and retains descriptor0 first,
including CHU_Z_1 for Chu and Z_1 for Frankly. Lowercase tribe d_1 therefore
selects Z_1 rather than aliasing uppercase D_1. Initial NPC pack admission and
base/talk requests use the canonical selected name before the clock sees it;
two unknown names selecting the same pose preserve restart0 timing, while source
message forced-restart requests retain their existing generation semantics.
Empty banks cannot select a fallback. Map-object lookup remains exact-name.
This string-only selector covers current exported NPC requests. Numeric source
selectors and valid original names omitted from a local pose subset remain
unsupported; future event bindings must export their requested poses before use.

Source NPC print anchors
use captured root plus height once; rendering no longer derives ordinary Chu/
Frankly anchors from sampled mesh bounds or model scale. Explicit event anchor
override, party/battle NPC anchors and later anchor updates need their owners.

Chu and Frankly now use PMANIM4 from the existing source surface converter,
retaining original RGBA, cull indices and shape passes/UV/sampler state. Clamp/
repeat/mirror atlas gutters and124px texture scaling remain documented adapters.
Both original resource draw masks are1; special pass2 admission uses the existing
citro3d depth/alpha-test path. Their original no-cull shapes remain no-cull even
when source camera-facing reflection flips winding. A prior PMANIM3 fallback
dropped this metadata and vanished after correct facing changed winding; the
final pack retains source cull/material data rather than guessing visibility.
Objects retain their existing original surface batches. Resident model packs
and time/pose sampler caches remain shared; matrices compute once per actor draw.

Validation uses original-data catalog, matrix, animation and message tests plus
isolated Azahar original Mouser talk/return captures; exact results in
[progress](../../PROGRESS.md). These captures prove visible native presentation
within the tested room, not matched original-runtime pixels or New2DS XL speed.
Remaining: complete pre-display culling/color/layers/blur/shadows, source field
NPC collision and interaction priority, wander/tavern fades, all-story/area
lifetimes, original runtime comparisons and hardware profiling.
