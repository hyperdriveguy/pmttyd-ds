# Original field NPC pose leaf services

US G8ME01 disc0 revision0, read-only static DOL/resource evidence for
[EVT-01](../../requests/EVT-01.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). These are required services
of [original map NPC registration](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md),
not proof that a rendered idle model is a registered actor or ready source pose.
See also [pose dimensions and display](../display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).

## Pool access and exact leaves

Source r13=8041cf20. Animation-work pointer W is loaded from
SDA r13-7160, address80415dc0. W+10 is the pose-array pointer; poses have
stride0x170. None of the following source leaves checks handle bounds,
allocation flags or readiness. Native providers must admit those owners.

| Source body | Actual operation |
|---|---|
| animPosePeraOn800414cc/1c | Load W, calculate handle*170, load W+10, read pose+0, OR8, store pose+0. |
| animPoseSetMaterialLightFlagOn800406f0/20 | Calculate handle*170, load W then W+10, read pose+ec, OR argument, store pose+ec. |
| animPoseGetVivianType8003980c/18 | Calculate handle*170, load W then W+10, return word at pose+11c. |

Material flags, pose main flags, original MODEL+c4 flags and NPC main flags
are distinct owners. Do not substitute one for another. Source Entry's
paper call changes an ordinary newly active pose from flags1 to9; subsequent
source consumers still determine readiness/display behavior.

## Height and radius are original model metadata

animPoseGetHeight800407b4/64 and animPoseGetRadius80040818/64 share the
same pointer chain, differing only in final MODEL offset. Their ordered
reads are:

1. Calculate handle*170; load single constant2 at8041fb64.
2. Load W through80415dc0, capture pose-array pointer at W+10, then capture
   model-entry-array pointer at W+0.
3. Load pose+10 model index; multiply index by16; load model-entry+8.
4. Dereference that file object's+a0 pointer, then dereference its word0
   to obtain the original MODEL pointer.
5. Height reads signed32 MODEL+cc; radius reads signed32 MODEL+c8.
6. Source xoris/bias-double conversion produces the signed integer as a
   rounded single, then fmuls multiplies by the captured single2.

Bias double802c1070 has bytes4330000080000000. The single result is
`f32(2 * f32(int32(metadata)))`; these are engine dimensions, not native
world-unit values, rendered bounds, original tribe dimensions or draw scale.
The constructor first queries these model dimensions; the map producer later
overwrites NPC+14c/+150 from its actual tribe descriptor. That later overwrite
does not authorize skipping the earlier resource service.

Read-only existing model files c_monban, c_borodo_n, c_tyusan, c_f_mura and
c_kuribo_n have signed MODEL+c8/+cc values5/5, therefore these particular
constructor queries yield10/10. This finite observation is not a default
for other models or a substitute for imported metadata.

## Exact Vivian-type constructor assignment

animPoseEntry8004176c/4ec, tail80041b78..80041c30, compares its original
captured model-name argument with the following source strings in order:

| Model string / address | pose+11c | pose+120 |
|---|---|---|
| tst_vivi / 802c11a8 | 1 | SDA r13-7180 =80415da0 |
| c_vivian / 802c11b4 | 1 | 80415da4 |
| c_maririn / 802c11c0 | 2 | 80415da8 |
| c_majyorin / 802c11cc | 3 | 80415dac |
| No match | 0 | SDA r13-715c =80415dc4 |

Each successful arm stores type first and pointer second, then skips remaining
comparisons. No match stores0 then its default pointer. This overwrites both
fields on every admitted Entry, including reused slots. It is not inferred
from MODEL+c4 or NPC tribe. None of the twelve original gor02 NPC model banks
matches those four strings, so their constructor type is source-proven0.
The pointees' later behavior is a separate owner; their raw addresses must
not be cast into native host pointers.

Exact existing disassembly:
`artifacts/recovered-temp-evidence/battle-pose-owner-animPoseEntry.dis`.
Its full-body fingerprint is recorded in the registration contract linked
above. Constant/name bytes were independently read from the original DOL.

## Tribe lookup is not pose allocation

npcGetTribe8004b828/6c captures the requested name and starts at
npcTribe803316d0. It freshly reads row+0; null terminates with return0.
Otherwise strcmp8026d464(rowName,requestedName) returns the first exact
matching row; mismatch advances by0x54. There is no case folding, fallback
model or partial-name match. Original Shift-JIS bytes and row order matter.
The returned descriptor supplies model+4 and pose strings+8/+10/+14, but
does not itself allocate a pose, register an NPC or prove animation readiness.

## Current reusable pieces and missing field assets

BattlePosePool retains source-sized0x170 scalar records and real CPU animation
resources. Its word0 and wordec can back paper/material changes once an
actual field-owned pose is admitted. BattlePoseResource.draw, when present,
can supply unchanged MODEL+c8/+cc. modelFlags alone cannot supply dimensions.
Pool Entry now initializes+11c/+120 from captured request identity on every
entry/reused slot; paper/material/Vivian and signed metadata leaves exist.
[The native field resource bridge](NATIVE_FIELD_POSE_RESOURCE_BRIDGE.md) owns
its own catalog/pool and requires real MODEL/TPL/descriptor0 data. This separate
namespace still does not represent original global field/battle allocation
history; field-facing/Main/GPU and Game map registration remain pending.

Current tools/field_actor_assets.py provides these six ordinary field banks:
c_tyusan, c_kuri_h, c_f_mura, c_kuribo_n, c_borodo_n, c_monban. Original
gor02's twelve distinct banks additionally require c_deale, c_joho_b, c_luigi,
c_binbo, c_roten_g and c_bomt_n. Constructor fallback hoshi is another actual
resource dependency; a fabricated pose handle cannot acknowledge it.

The finite field converter currently emits selected pose animation/mesh and
GPU texture packs, rather than every required original descriptor and raw
draw dependency. Most field banks lack PMPOSE directory sidecars expected by
loadBattlePoseResource. A faithful pool bridge requires original descriptor
identity/order (descriptor0 included), admitted clips for requested names,
model/texture names, original model flags, original+c8/+cc metadata, and real
CPU animation outputs. Drawing through original shape services additionally
requires matching .draw-model.bin/.draw-textures.bin ownership, hierarchy,
shape/index data, original immutable TPL resources and actual GPU readiness.
Presence of converted .pmanim alone proves none of those additional owners.

Resource readiness, synchronous Entry/fallback effects, paper-facing Main,
clock selection, model/texture reference counts and release/room lifetime
must be connected to a retained field owner. Existing field presentation
and battle resource mechanics are reusable components, not a registration
or source file/cache ABI substitute. Full map registration and the NPC setup
event must remain unacknowledged until their actual required owners exist.

The separate [map resource exporter](ORIGINAL_MAP_NPC_RESOURCE_EXPORT.md) now
supplies full PMPOSE02, unchanged MODEL/TPL and all173 source CPU clips across
all13 gor02/fallback banks. The older finite presentation converter discussed
above remains separate. Real field-pool host/ARM admission is verified, while
Game registration, field-facing/Main/GPU and original shared lifetime remain
required; availability of these exports alone does not acknowledge them.
