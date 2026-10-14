# Native Crump opening actor movement and facing

US G8ME01 revision0, static original-data evidence; no new Dolphin observation.
The [original event bindings](STORY_HARBOR_OPENING_BINDINGS.md) own speaker,
position, rotation, message order and branches. Dialogue remains the unchanged
local `msg/US/gor_00.txt` resource.

Read-only DOL tribe records establish these automatic animation profiles:

| Actor | Tribe address | Idle / talk / walk / run | Run threshold (units/second) |
| --- | --- | --- | --- |
| Goombella | `0x803316d0` | PKR_S_1 / PKR_T_1 / PKR_W_1 / PKR_R_1 | 20 |
| Crump | `0x80332690` | S_1 / T_1 / W_1 / R_1 | 50 |
| X-Naut | `0x803326e4` | S_1 / T_1 / W_1 / R_1 | 50 |

The original table fields are idle+10, talk+14, walk+18, run+1c and threshold+3c.
`tools/harbor_actor_assets.py` verifies the two enemy profiles and Goombella's
threshold directly against the supplied ISO. Local manifests retain the record
hashes, exact source names and converted geometry. Descriptor0 Z_1 is retained
in both enemy banks. Original npcMain `0x8004a0c0..15c` selects idle below
0.10000000149011612 units/second, walk below the tribe threshold, otherwise run;
phase changes request a new pose. The native adapter measures XZ root delta
per simulation tick, excluding initial placement. Pose clocks restart on change,
not on rendering. Message token pose requests use the existing NpcPoseClock and
source talk flag200. Movement/collision/clock chronology remains adapted.

Crump and both X-Nauts now render their original animated geometry/textures,
replacing the old fixed Z_1 raster. All four actors retain independent
NpcPresentation state. Original event yaw and camera snapshots feed the existing
camera-relative Main/Inner matrix adapter; Goombella uses the same path. Actors
turn independently and no longer ignore their rotations or jump immediately
between two baked sprite orientations. Flags and source scalar2 enter the matrix
service; simulation prepares matrices and drawing reads them without advancing
facing. Original NPC Entry clears secondary state+1d4. Move-position sets
state10 (and optionally1000), while `0x800526dc..730` calls npcTuningRy only
under secondary bit2. The supplied opening has no writer enabling that bit.
Movement therefore retains event yaw: Goombella retreats facing90, Crump and
X-Nauts remain270; beforebattle explicitly turns Goombella270 for her approach
and90 afterward. Native movement must not derive NPC yaw from displacement.
This uses the existing host/ARM numerical adapter, not Gekko bit parity.

Beforebattle's Mario mov_pos2 uses its resolved speed to select ordinary walk
or run (ordinary walk speed1 at60Hz is60 engine units/second). Its original
callback at `0x800e5f50` computes force-move duration from `(distance+1)/(rate/GPHz)`;
the existing native linear millisecond movement bridge remains explicitly an
adapter, with at least one millisecond and a full XZ heading. It changes XZ only,
leaving ordinary vertical movement independent. Completion restores idle unless
a later source pose request overrides it. M_R_1 joins the harbor Mario bank.

Key-off during an ordinary jump continues neutral-input field jump/fall/support
physics. Ground height remains floor height instead of absorbing the airborne
root. Airborne pose selection remains jump/fall even during scripted XZ travel;
landing allows walk/run again. Explicit source set-pose overrides survive ordinary
airborne updates. The explicit stationary source jump has its own
owner and published active flag. Source displayflag2 suppresses automatic pose
changes, retaining M_I_N through the authored surprised hop; otherwise the jump
selects M_J_1B, M_J_1C and idle. The renderer samples root plus jump height.

Limits: preceding story3 movement/timing remains the existing staged adapter;
complete NPC move-position easing/collision, Mario force-move tick arithmetic,
source direction-set duration, effect/SFX/crowd bindings, full tutorial and
postbattle completion, and hardware/original runtime comparison remain open.
No unsupported battle result or story write is introduced.
