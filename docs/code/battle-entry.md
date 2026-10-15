# Packed battle entry and startup

These modules implement source memory operations through injected services.
Guest addresses identify US executable data or emulated memory; they are not
host pointers. Hexadecimal offsets describe source records, not C++ layouts.
The [battle core](battle-core.md) covers typed action/status/damage helpers;
[behavior contracts](../behavior/README.md) retain recovered behavior/evidence.
Comments here document current implementation, not measured hardware timing.

## Actors, construction and ownership

[battle_actor.cpp](../../source/battle/battle_actor.cpp) reads the current battle
through `0x8041eb90`. Its registry has 64 four-byte pointers at battle `+0x20`.
A registry slot differs from an event actor selector:

| Selector | Translation |
| --- | --- |
| Nonnegative, `-1`, other unrecognized negatives | Preserve the selector |
| `-2` | Event owner; requires an event |
| `-3` | Mario's unit ID; requires Mario |
| `-4` | Partner's unit ID, or `-1` if absent |
| `-5` | System unit's ID; requires the system actor |
| `-6` | Word at battle `+0x424` |

Translation reads the global even for a literal selector. Mario is kind 222,
partners 224–230, system 220. Player scans require alliance byte `+0xc == 0`;
partner scanning also excludes mask `0x8` at `+0x1c`. The registry getter rejects
negative IDs; the original setter checks only the upper bound. Native admission
must prevent invalid setter calls rather than rely on a symmetric range check.

[battle_unit_entry.cpp](../../source/battle/battle_unit_entry.cpp) finds the
first free slot, allocates/clears `0xb34` bytes, then publishes the actor before
status, parts, display and script initialization. Full registry and initial
allocation failure return zero. Later failures have no rollback: all required
providers, memory, names, capacity and resources must be admitted beforehand.
In particular, the part buffer is cleared before its nonnull assertion.

| Record | Important fields (hex) |
| --- | --- |
| Group definition, stride `30` | Kind pointer `0`, alliance `4`, flags `8`, XYZ `c`, half-source word `18`, four work words `1c` |
| Unit | ID `0`, original/current kind `4/8`, definition `10`, part head `14`, optional key/value table `18` |
| Unit resources | HP max/base/current `108/10a/10c`; FP max/base/current `10e/110/112` |
| Unit events | Init ID `288`, wait script/ID `28c/290`; work array `218` |
| Part, stride `500` | Next `0`, definition `4`, owner `4ec`, attributes `1ac/1b0`, pose table `1bc`, pose handle `1c0`, pose name `1c4` |
| Part definition, stride `4c` | ID `0`, bank `8`, defense pointers `38/3c`, attributes `40/44`, pose table `48` |

A body attribute mask `1` copies the **definition ID** into unit `+0x2d4`.
The body lookup service still returns a part pointer. Pose type requests 65 and
43 remain source numeric types; these notes do not invent clip meanings.
Display initialization precedes kind 208's extra `0x44` allocation and pouch
import. An optional definition init script creates a priority-10, flags-zero
event owned by the registry ID; its ID is stored at `+0x288`.

[battle_unit_setup.cpp](../../source/battle/battle_unit_setup.cpp) constructs
system, Mario, optional partner, then group units in that order. The system
actor remains mandatory despite having no ordinary visible character surface.
Partner placement uses source X=-140, Z=5 and retained Y. A nonzero per-ordinal
held-item override bypasses equipment and sets mask `0x40000000`; ordinary held
items invoke equipment mode 1. Group units finish at revised maximum HP/FP;
player imports preserve pouch resources. The badge-rule table for IDs 308–319
is intentionally nonmonotonic and includes ID 314 mapping to zero.

## Numeric leaves and persistent resources

[Vector helpers](../../source/battle/battle_packed_unit_vectors.cpp) dispatch by
original helper address. Source float `0x804226c4` is an exact per-axis sentinel
only for flagged helpers. Unit base/current rotations use `reviseAngle`; current
rotation additionally replaces values at least source 360 with source zero.
Parts and other vectors do not inherit that normalization. Raw float bits and
source read ordering remain intact; this is not a renderer coordinate transform.

[Initialization](../../source/battle/battle_packed_unit_initialization.cpp)
retains the actual no-op `BattleStatusEffectInit`, distinct from a missing
provider. Change-info work has six 12-byte records: it clears bytes 1–3 and word
4, writes byte 0 as `0xff`, and leaves word 8 untouched. HP gauge ratios have no
zero-denominator guard. Source gauge counters 20/40 and +30 have no measured
seconds interpretation. Fresh callback reads must remain fresh even after a
preceding store seems to establish a constant.

[Resource import/revision](../../source/battle/battle_packed_unit_resources.cpp)
uses signed halfwords and five HP/FP per relevant badge count. Revision caps
only values above the maximum; it does not heal or clamp negatives. Partner
pouch records have decimal stride 14, while battle snapshots have hex stride
`0x2c` at battle `+0x162a0`. Snapshot fields are copied individually in source
order, including fresh kind reads after callbacks. This is not native struct
serialization. Nonpositive imported partner HP sets source status 27.

[Equipment](../../source/battle/battle_packed_unit_equipment.cpp) calls the pouch
provider before checking a null actor. Mode mask 1 clears 40 counter bytes;
system actors then return. Mode mask 6 selects all 200 equipped slots, passing
mode with mask 1 cleared to `equipOne`; otherwise it processes the explicit
item. Only item zero's early return is implemented by `equipPackedNoItem`.
Nonzero badge semantics belong to the bound provider.

[Fresh pouch initialization](../../source/battle/battle_packed_pouch.cpp)
publishes its `0x5d4` allocation before clearing and retains redundant individual
writes. The 121/20/32/200/200 capacities are key/carried/stored/badge/equipped
arrays. Initial HP/FP/BP are 10/5/3. Partner rows 1–7 leave row zero unused.
The original localized `name_party3` must fit 20 bytes including NUL; it is not
truncated or replaced. Querying equipped badge ID zero counts empty slots.

## Poses, scripts and sequence visits

[Type lookup](../../source/battle/battle_packed_pose_lookup.cpp) accepts the
first exact **or fallback** row. An earlier fallback can hide a later exact
match. Fallback tags are 28, 39, 69 for requested ranges <=28, 29–39, 40–69;
tag >=70 terminates. The table pointer is still read before rejecting a request
>=70. [Display entry](../../source/battle/battle_packed_display_entry.cpp) instead
scans at most 28 rows with fallback tag 28. Missing table/bank stores handle -1;
a present bank requires actual pose allocation and enough name-buffer storage.

[Unit EVT adapters](../../source/battle/battle_unit_event.cpp) resolve inputs
while preserving raw output destinations. Status is a raw signed low byte,
not a resolved EVT value. Work indexing has no local bounds check. Data rows
are eight-byte key/value pairs terminated by key zero; presence ignores value.
The HP opcode's selector -1 completes without writing output. Starting a wait
with no wait script leaves the existing wait event alone; explicit stop removes
it. Internal wait result zero differs from EVT completion result two.

[Sequence startup](../../source/battle/battle_sequence_startup.cpp) uses eight
word channels. Channel zero selects manager init/first-act/turn/end; channel one
runs `0x01000000..05`. Init zero falls into one in the same call, setup yields,
init-event scanning clears stale IDs even after finding a live event, and paper
probes short-circuit in source order. Nice probes instead visit every nonnull
one of seven language-table slots, rereading owner/language each time. Manager
dispatch captures its channel once; a handoff cannot dispatch the new phase in
the same manager visit.

[First act](../../source/battle/battle_first_act.cpp) uses channel two states
`0x02000000..0e`. Ordinary entry creates the root and per-unit scripts; other
first-attack types require their own provider. Some steps yield, others fall
through in the same call: 1 can enter 2; 3 through 8 can progress together;
c/d can enter e. Message and announce both execute, as do the readiness probes
required by their source branch. Raw fade duration 200 and the luck counter cap
300 do not establish display-frame timing; reaching 300 does not itself release
the luck wait. The final handoff selects turn manager/channel-three startup.

[Turn start](../../source/battle/battle_turn_first.cpp) wraps the turn halfword,
saturates a separate byte at 100, increments each actor byte modulo 256 and
uses the supplied original RNG for initiative. Signed division truncates toward
zero. Fog expiry rereads the **global** battle owner, even when turn processing
received an explicit owner. An already-zero fog timer returns without deleting
an existing effect. These details are observable through service callbacks.

## Startup resource admission

[BattleStartupResources](../../source/battle/battle_startup_resources.cpp)
validates private little-endian `PMBRDY01` metadata against retained big-endian
MODEL descriptors and CPU animation clips. Three paper banks and six US Nice
banks are fixed source identities. Native limits include bank size <=1 MiB,
<=32 descriptors, <=64 textures, <=4096 keys and UV scale 1/2/4/8. These bounds
are admission policy, not original game maxima. FNV-1a verifies corruption, not
cryptographic authenticity. Unsupported descriptors must be absent from the
native pack; supported poses require real bindings, surfaces and a start sample.

Loading uses a heap candidate. Failure preserves the previous resource set;
success replaces it and clears every texture-readiness flag. Texture admission
marks only provider success. `releaseTexture` clears readiness; its external
owner must perform actual GPU release. `ready()` means loaded CPU/source data,
while context-one resource probes also require texture admission. None of these
operations acknowledges field capture, pose playback or completed battle logic.
