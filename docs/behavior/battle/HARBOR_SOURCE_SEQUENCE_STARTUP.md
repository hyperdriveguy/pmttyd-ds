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

Init preserves source same-call fall-throughs 0→1 and 4→5. It requests field
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
The existing native projection adapter uses Crump0/Mario1/helper2 and omits
system. This discrepancy must be resolved by separating three visual slots from
the complete packed registry before original owner allocation is installed.
The system actor may be visually absent but cannot be omitted from source state.

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
