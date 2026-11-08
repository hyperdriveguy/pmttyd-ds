# Harbor ordinary lethal-damage admission

The US revision-0 source remains authoritative. This increment admits ordinary
Mario/Crump lethal damage without a Life Shroom; it does not infer victory from
HP or complete the death/event/result graph. Source instruction inspection is
B evidence; no original PowerPC execution or measured timing is claimed.

`verify_harbor_lethal_damage.py` pins the whole source pouch helper, direct damage,
emergency-revival helper, four death-event wrappers, status setter and setter
table. The existing orchestration verifier additionally pins its complete parent
and actual Nice constants. See [application](BATTLE_DAMAGE_APPLICATION.md),
[orchestration](BATTLE_DAMAGE_ORCHESTRATION.md), and
[status storage](BATTLE_STATUS_STORAGE.md).

## Original inventory helper

`packedPouchItemCount` implements whole `pouchCheckItem800d4ed4/1f8` using the
actual pointer at `8041eb00`. Supported signed item ranges capture that pointer
once, then read every signed halfword in source order:

| IDs, inclusive | First offset | Halfword count |
| --- | --- | --- |
| 1..120 | a0 | 121 |
| 121..235 | 192 | 20 |
| 240..338 | 1fa | 200 |

Other signed IDs return zero without reading the pointer or inventory. The
regular-item scan never queries the Bag key item or substitutes capacity ten.
The source unrolled loops visit the same consecutive addresses as the portable
loop. Actual reads remain visible; there is no copied inventory/count cache.
This is an atomic memory-domain helper: the caller must supply the complete real
read domain and stable owner; missing memory is not an empty inventory.

## Native admission and dynamic units

The production packed damage adapter replaces fixed private unit/part arithmetic
with `HarborTutorialPacked::battleAddress()`, `unitAddress(id)`, and the existing
source body-part lookup over the real linked part definitions. Source IDs index
four native calculator snapshots, not guest allocation addresses. Damage sound,
hit position, flags, bookkeeping and final hit-event dispatch use those same
actual units. Held item and enemy audience byte are copied from unit308/0d.

Existing ordinary power/status/counter/badge admission remains. For a potentially
lethal admitted hit, the player/partner route first validates all twenty actual
regular-item halfwords and executes the source count for item151. A present
Life Shroom holds before live damage/RNG; enemy item151 similarly holds. The
missing downstream consumption/revival-event owner is not acknowledged. The
admitted synchronous callbacks do not mutate inventory between this preflight
and the original emergency-revival query. Nonordinary status/badge branches
remain outside this finite atomic domain.

Potential lethality uses the full HP calculator on private copies of the two
units, part, weapon and battle state, followed by the application's request
clamp, signed16 HP store, upper cap/floor and HP-write suppression. The preview
checks code100 and the attr10000 special-death gate before querying item151.
This catches defense attributes, negative defense and sequence modifiers; raw
weapon power alone cannot establish whether a hit is lethal. Preview attacker
flag8 is private. The three admitted power callbacks only read stable guest
memory, and later reaction/audience flags do not affect HP. This preview is a
native availability check, not an additional source damage application; it
consumes no RNG, publishes no guest stores and supplies no presentation.


Without item151, the existing original application performs HP subtraction,
lethal flags, enemy notices and final hit-event dispatch. No clamp, completion
flag, source bank change or battle result is supplied by admission. Captured
presentation requests remain requests; this change adds no rendering/audio
acknowledgement. See [source flow](HARBOR_RETAINED_SOURCE_FLOW.md) for the separate
Main/sequence/turn/controller owner.

## Genuine death-event leaves

The production Packed.invoke adapter supplies these reached source leaves:

* `801141a4/18` captures global Battle, stores word0 at B1c70, then byte0 at
  B1c74, and returns2. It does not clear neighboring bytes.
* `8010ed8c/6c` resolves/translates the actor operand, captures the raw output,
  then reads the fresh global Battle and actual unit kind and stores the output.
* `80105e50/bc` preserves the whole operand-evaluation order and admits only
  status27. The signed raw low status byte is not EVT-resolved. Duration,
  strength and on are resolved; the actual setter table entry27 at80127f1c stores
  the duration low byte at U135. Off supplies zero. Other statuses hold before
  any setter effect. This finite binding does not claim the entire SetStatus
  function's other branches.
* `80105df8/58` executes the complete existing `ClearStatus80127b58/54` owner,
  including source Charge16 and six change-info records. It preserves each
  record's word8 rather than bulk clearing the records.

## Guard / Superguard frontier

Successful normal Guard4 and Superguard5 remain held. The real orchestration
call requires `niceEntry(mode,position)` with mode0/2 respectively and source hit
position offset by (-60,+45,-10). It returns a genuine effect allocation E;
source then stores `0x3f400000` (0.75f) through `word(E+c)+1c`. Required providers
include actual effect/work ownership and downstream native rendering, original
sound keys `802ee078` / `802ee064`, and GuardGood audience feedback. No dummy
pointer, atlas readiness or captured request supplies this ownership.

## Validation and remaining graph

The focused actual-VM tests use unchanged original Harbor tables, real Packed
callbacks and dynamic registry/body-part relocation, checking old private slots
remain unchanged. Item151 in the twentieth slot holds; removing it permits the
same lethal call. Guard4/5 hold without consuming RNG. Raw status operands,
byte preservation and ClearStatus's untouched record word8 are checked.

Two source-script fixtures launch original installed attacks and observe actual
lethal hit-event code10717. Explicit TEST entry, pose acknowledgement, menu
presentation and HP-boundary setup are labeled in the fixture. They establish
neither normal opening admission nor a native GPU battle run. The original
Mario script next holds at camera priority-limit8012c0ac; Crump next holds at
AddRotate801115f8. Rotation/math, subsequent death effects/deletion, actual
conclusion/result/field return and owned item151 revival remain required.

Focused strict host, ASan/UBSan and ARM syntax checks are recorded under ignored
`artifacts/harbor-lethal-damage-validation/`. The sanitizer instruments the new
helper/journal postimages/tests against the matching immutable6966 core; it is
not a claim that the prebuilt core was sanitizer-instrumented. Full build and
normal-flow integration belong to the parent integration graph.
