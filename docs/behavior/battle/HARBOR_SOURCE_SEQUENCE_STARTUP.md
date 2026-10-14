# Original battle startup and ordinary first-action ownership

The host portable controllers recover US DOL BattleGet/Set/IncSeq at
800f89a0/800f8a08/800f895c, btlseqInit8011ee40,
BattleCheckEndUnitInitEvent80129994, BattleSequenceManager8011f1ec,
effNiceAsync80144d44, and the ordinary firstAttack0 entry plus shared states
1..14 of btlseqFirstAct8011e5c0. `verify_battle_sequence_startup.py` pins ten
original function/table/constant spans in the read-only ISO. Source bytes are
not committed. See [source field information](HARBOR_SOURCE_FIELD_INFORMATION.md)
for the original setup2 FirstAttack0 constructor.

The sequence manager reads its outer state once: Init then pinch(1),
pinch(0) then FirstAct, pinch(0) then Turn, or End. A callback changing the
outer state does not dispatch a second controller in that same call. The eight
channel offsets and invalid-channel no-op/zero behavior follow the source.

Init preserves the source same-call fall-through0→1. State4 sets5 and returns;
status activation begins on the next manager call. It requests field
Mario/party flags before UnitSetup, waits for actual unit-init event IDs, then
short-circuits ordered paper probes p_b_st/p_jyabara/p_kaiten_h with context1.
Only after all three succeed does Nice mode1 run. Nice reloads current language
and source table for each of seven slots; all nonnull entries are called even
if an earlier entry was unready. US language1 has six grade/lucky resources.
Admission must come from actual resource owners, not elapsed time. Init only
then enables source status services and selects FirstAct.

Ordinary FirstAct starts the source camera root80416d70 and each eligible
unit entry root at priority10, flags0. Entry owner is the actual packed unit
word0; IDs are stored at unit2bc. NPC flag100000 suppresses Mario/party entries.
Actual field Mario motion26 selects unit data65 for Mario; other entries retain
installed roots. Nonzero first-attack entry modes require a separate original
service and remain unchanged when no provider is installed.

FirstAct then requests fade type23, raw duration200, color000000ff and original
SFX_OFF_BTL_START_CURTAIN_UP1. Duration is not converted into a guessed completion
time. Source fadeIsFinish and all-active-event barriers retain original repeated
calls and short-circuit order. The controller preserves status message/announce
ordering, reserve consumption, audience and slot reactions, wall-close event
ownership, fortune root80416d40/owner-5 and bounded counter, and nozzle/event
gates. Only original final state14 selects outer Turn and channel3=03000000.
A source conclusion selects End; it does not publish a battle result itself.

`HarborTutorial::setStartupResources(user,paperReady,niceReady)` and
`setStartupFade(user,entry,finished)` store native producer bindings. Attach them
after beginFresh; begin resets owned runtime. Clear callbacks before their
provider resources are released. The current tutorial introduction adapter does
not yet invoke these controllers: complete UnitSetup, field motion, status,
audience, pinch and entry-service ownership must be bound before replacing it.
Setter installation does not claim resource playback, capture readiness, fade
completion, a selected attack, damage or a field result.

Focused tests exercise source fall-throughs, live/dead event cleanup, ordered
resource blocking and complete US admission, captured manager dispatch, entry
owner/motion/suppression, independent fade/event gates, reaction/status ordering,
wall/fortune barriers and actual Turn handoff. Callback fixtures are unit-test
providers; they are not native completion shortcuts.

## UnitSetup roster ownership

`packedBattleUnitSetup` recovers complete Btl_UnitSetup800f8aac. It captures
source NPC/group and pouch, initializes group headers, entries system80416a80
before Mario80416a50, applies original equip mode3 and Mario pouch parameters,
then optional source party definition with original base y/-140/5 adaptation.
It traverses live source group count/definitions, sets enemy group index and
held/preassigned item branches, copies item history and resets HP/FP from maxima.
It then clears acting unit and scans all200 signed equipped badge entries using
the original 308..319 dispatch masks (314 is a no-op).

The Entry provider must own real BtlUnit_Entry allocation and initialization;
calling UnitSetup with test fixtures does not install native startup. Original
first-free allocation yields system0/Mario1/Crump2/helper3 for fresh setup2.
The native adapter now retains that four-owner registry and executes the
original system init while keeping three visual slots separate; see
[registry/visual scope](HARBOR_NATIVE_REGISTRY_AND_VISUAL_PROJECTION.md).
Full source UnitEntry/UnitSetup allocation, display/equipment/resource and
startup-controller binding still require installation. The invisible system
is retained in source state without a fabricated model/pose handle.

## UnitEntry allocation and initialization

`packedBattleUnitEntry` recovers the complete control/storage body of
BtlUnit_Entry80128fe0. It chooses the first free registry slot, preserves full
registry and initial0xb34 allocation failure returns, zeros/publishes the unit,
then retains original status helper ordering before source kind/group fields.
It copies original vectors, HP/FP/base fields, movement/status/definition data,
level from pouch for Mario/party, and source work0..3. It allocates and zeros the
original count×0x500 part chain, initializes complete source part fields and
body identity, then resolves pose types65/43. Display initialization and pouch
parameter import precede dispatch of actual kind+bc init script, priority10,
flags0, owner equal to allocated registry ID, tracked at unit288. Original kind208
retains its extra0x44 allocation. Zero-part/null part allocation are invalid
source domains; no synthetic fallback or repair is introduced.

External helpers retain source function addresses in providers. These include
status icon/change-info, HP gauge, clear-status, scalar vector setters, body-part
selection, pose lookup, display entry and pouch parameters. A provider must
execute its actual source service; the controller asserts that providers are
bound before entry begins. The no-op BattleStatusEffectInit8019673c call is
retained in source order. Body selection combines the original GetBodyPartsId
and GetPartsPtr queries. The portable fixtures record helper calls; this does
not claim those helpers are installed in the native renderer/runtime yet.

Ignored original-data tests prove system definition80416a80→kind220 with one
part and init803adf38, followed by Mario definition80416a50→kind222 with three
parts and init80372ec4. Both actual init roots receive their newly allocated
original owner IDs0/1. No source pack expansion is needed for these records.

## Executable packed unary initialization helpers

`initializePackedBattleUnitHelper` now supplies actual source storage bodies for
BattleStatusEffectInit8019673c (empty blr), BattleStatusIconInit80253e84
(zero7a0 bytes at348), BattleStatusChangeInfoWorkInit80195d34 (six stride12
records; write first8 bytes, preserve last4), BtlUnit_ClearStatus80127b58
(zero118..135, exact type16/-99 setter branch, then change-info), and
BtlUnit_HpGaugeInit80125fd8 (signed HP copies, original ratio and timer fields).
These run in original-data UnitEntry tests, replacing their recording-only
unary fixture services. Unsupported helper addresses return false without
mutation; actual display/pouch/vector initialization remains separately owned.
Gauge initialization preserves original zero-max 0/0 NaN; it does not invent a
safe ratio for the system unit. Floating-point NaN payload is host dependent.

## Executable resource, vector, pose and display services

Packed resource import/revision now uses actual source pouch and partner
snapshots, preserving repeated pouch/index queries, signed halfword caps, badge
counts2f1/2f2,14-byte pouch partner rows,44-byte battle snapshots and the30-byte
status restore with retained unit136/137 gap. Source partner HP≤0 follows exact
type27 duration1 storage. Mario imports carried HP/FP before source revision.
System/enemy SetParamFromPouch still queries pouch but does not revise resources.

All thirteen UnitEntry unit/part vector leaves are executable. They retain exact
source destinations, per-axis -250000000 sentinel comparisons, required original
reviseAngle provider for unit rotations, and SetRotate's final ≥360→0 clamp.
The existing portable reviseJumpAngle recovers the same system reviseAngle body
and can provide that numeric service; no angle is inferred from rendering.

Original GetPoseNameFromType retains ordered exact-or-range fallback28/39/69,
including earlier fallback overriding a later exact entry. Source display
searchPoseTbl is distinct: at most28 entries, exact-or28 in original order.
btlDispEntAnime walks every source part: NULL table or bank gives posehandle-1;
otherwise it copies source initial name before calling actual bank poseEntry
context1. Actual pose allocation remains a required native provider.

Original-data UnitEntry tests now execute all recovered initialization/resource/
vector/body/pose/display services; only heap, root creation and bank allocation
are fixture providers. Tests prove fresh Mario HP7/max10/FP5/level1 and all three
original part allocations. Mario body uses initial M_S_1; parts2/3 have attributes
13010000 and copied initial KRN_H_1 via source table80417bc8. All three request
actual b_mario bank802f3fa8. KRN_H_1 is absent from b_mario's clip catalog: Entry
only stores this initial string and does not call SetAnim. Converter selection
must therefore keep actual M_S_1/M_N_2, without a fabricated shell descriptor or
replacement playback. Later shell playback remains source-service owned.

## Source fresh equipment and system init count

`equipPackedUnitItems` recovers complete BtlUnit_EquipItem800f831c outer
ownership: pouch query even for NULL unit, exact40-byte reset when mode bit1,
system220 early exit,200 signed badge entries for mode bits2/4 with bit1 removed,
or original single item, then actual resource revision. EquipOne remains its
explicit source helper provider. `equipPackedNoItem` implements only the original
_EquipItem800f7ca4 first item==0 return, before any unit access. This is sufficient
for verified fresh empty equipment and original setup2 item0; nonzero badges
remain unsupported until their source branches bind, rather than borrowing
native inventory or silently ignoring equipment.

Packed btlevtcmd_SetMaxMoveCount8010fce0 now handles every actual registry owner,
including invisible system220: capture current battle before operand resolution,
translate actor, original -1 early return, store count21 and signed clamp22.
The three visual projections capture maximum moves from actual source21.
This removes the projection-only callback dependency from original system
init803adf38, whose wait/attack/damage/count0/start-wait commands are now covered.
