# Battle dual-screen UI fidelity audit

2026-10-14. Audit of the current native implementation against the user's two
original-game UI screenshots and rough top/bottom sketch. This is a source and
read-only ISO audit; no new emulator capture, Dolphin measurement or hardware
acceptance is claimed. The tutorial and trio remain partial battle owners.

The references require original compact HUD art above the stage, the original
rotating action-class icons and selected-command bubble near Mario, and the
lower screen continuing into the audience. The narrower lower view is centered
and cropped at the same scene scale. Required actions need physical controls;
taps on actual harmful throwers may supplement them. The rough sketch provides
composition, not replacement artwork or recovered camera coordinates.

## Current bound paths and missing owners

| Requirement | Current evidence | Required next step |
| --- | --- | --- |
| Original top HUD art | `Renderer::battleHud` in `source/display/render_battle_hud.cpp` draws icon IDs472/427/480 for Mario HP and473/441/480 for FP, using the original message-icon atlas and original large/small numeral IDs. `overlay` invokes it for tutorial and trio with visibility gates. Packed Mario resources override the field pouch. | Preserve these resource owners and original sprites while adding the remaining source status-window presentation. Native positions are adaptation choices, not source status choreography. |
| Full original status content | Live HUD draws only HP and FP groups; partner HP, AP bar, coins and Star Points are absent from this method. Source controllers/data elsewhere do not prove those groups are rendered. | Bind actual partner, pouch and AP owners and source visibility/slide/blink rules. Do not fill missing groups with inferred values. Test values changing during battle before field export. |
| Original action-class ring/bubble on upper screen | `render_battle_class.cpp` has an original-assets backend, half-size centered top projection, disabled-icon material and state restoration. At audit time a whole `source/`/`include/` reference search finds `drawClassMenu` and `classMenuAssets` only in declarations and definitions, with no native runtime call site. `HarborTutorial::classMenuPresented` exists separately. | Wire the actual source UI/display lifecycle to asset acquisition, complete batch drawing and presentation acknowledgement. A portable controller, `classMenuOpen` flag or fixture image does not close this normal-runtime gap. Do not acknowledge a menu that was never drawn. |
| Menu placement and clear audience view | Tutorial and trio `Renderer::menu` now immediately return, leaving their lower scenes unobscured. Trio command/target state calls `battleMenuRows` from upper `overlay`; the original frame/font/icon fallback is adapted to that target. | Verify upper placement and readability in native captures. This relocation preserves the interim list rather than completing the original rotating class menu. Keep actual command labels, selection and target rules while binding the full presentation. |
| Actual lower scene continuation | `Renderer::render` draws tutorial `worldTutorial(g,true)` or prepared trio `lowerTrioScene(g)`, then lower scene4 retained icons. `lowerBattleSceneMatrix` gives the next240 vertical pixels with40px horizontal crop on each side; depth/W are unchanged. | Keep the established transform and immutable snapshot. Validate useful audience framing after real command/menu binding and source camera motion; do not independently advance simulation for the second screen. |
| Original audience geometry/frames | `render_audience.cpp` loads `audience-frames.bin/.t3x`, asks the actual tutorial/trio owner for opaque then translucent source requests, converts engine coordinates once and reuses the same top-pass vertex buffer below. Missing preparation produces no invented spectators. | Exercise real population/actor cadence and resource waits through normal entry. Original art availability and a draw API do not alone prove visible populated audience or working throw windows. |
| Optional harmful-thrower tap | `runtime/main.cpp` captures a touch-down coordinate. The relocated upper Trio list has no lower command tap hitboxes; tutorial touch is reduced to dialogue A semantics in `Game::tick`. No rendered audience hit target is bound here. | Add hit targets from actual published visible actor geometry and existing harmful-item eligibility. Revalidate actor identity/throw state at input consumption and route to the same retained interruption mechanism as physical input. An arbitrary screen rectangle or every-audience-member target would not satisfy the request. |
| Physical alternatives | Main maps physical X to GameCube0x400 and A/B/D-pad/Circle Pad through the source pad bridge. `HarborTutorialPacked::attackAudienceMenu` binds actual item-on selection, SetTarget and retained UI release. Trio command/target state retains its physical selection path after removal of lower command-row taps. | Keep X available for eligible harmful-throw interruption when adding touch, including while UI release waits. Verify actual actor/action execution, not only the packed selector. Do not let audience touches become dialogue-confirm events concurrently. |

## Portable publication bridge added during this audit

`BattleClassPublication` in `include/display/battle_class_publication.hpp` and
`source/display/battle_class_publication.cpp` now accepts complete source-produced
`BattleClassBackend` batches for an explicitly bound native token/UI allocation
lifetime and increasing publication sequence. It copies only geometry and the
actual asset-owner pointer, stripping stack-local label/icon/random callbacks.
Published batches are immutable shared snapshots. Replacement leaves an already
borrowed snapshot alive; partial primitives cannot replace the current frame.
The native asset owner must outlive its frames.

Rendering may report admission only for the exact snapshot after actual successful
`drawClassMenu`; simulation consumes that notification once. Old allocations,
old sequences and retired/reused-address tokens reject delayed admissions. This
bridge never executes DrawMainMenu, writes readiness bytes or substitutes for the
source controller. Its header documents these native ownership adaptations.
It remains unbound to normal runtime, so the class-renderer call-site gap above
is still open. In particular, original DrawMainMenu owns its guest writes and
cannot be blindly replayed on failure: its existing API is non-resumable.

Focused Werror host compilation and tests passed against the existing frozen core
and original class assets. Tests cover copied geometry/callback removal, failed
and repeated draw admission, replacement while borrowed, partial primitive
rejection, wrong allocation/owner and same-address new lifetime. Root owns build
registration, full required validation and eventual runtime hooks.

## Original-resource verification

`tools/battle_hud_assets.py::source_art` was run against
`/home/carson/games/roms/G8ME01.iso` read-only. It accepted US G8ME01 revision0
and verified the156-byte status table at8036edd0, SHA256
`a44bcbdc5e0be34e351fd345b6b66a914ca77e16a6436c5d2a910d919ac10fbc`, and
40-byte numeral table at80415084, SHA256
`54b8acd42d57db411d64d29bb6eb6f2741bd2ecadea16b20bd0752c36e693137`.
The13 status records contain IDs472,427,480,473,441,480,475,443,444,478,478,474,442.
The last has scale0.8999999761581421; the others have scale1. Numerals are
481,483,...,499 large and482,484,...,500 small. This verifies static original
asset/table identity; it does not recover every corresponding controller branch.

The same read-only FST inventory finds `icon.bin` at948139664,8174 bytes and
`icon.tpl` at948147840,677792 bytes. Use existing converters/catalogs, not new
substitute art. No extraction or source asset import was performed for this audit.
Original ownership/licensing and editable-asset rules remain in
[the asset ledger](../../ASSETS.md). A new import must use an empty destination;
existing checked-in edits must not be overwritten.

## Optional audience touch geometry boundary

`BattleAudienceTouchFrame` copies actual transformed source-world quads and
projects them through the retained source camera and established lower scene
continuation. It requires a completed displayed pass and exact live actor/item
incarnations. Eligibility is supplied explicitly by the real owner: visible,
dangerous and interruptible; no item-number heuristic creates a target.
Uncertain overlapping geometry blocks a tap. An exact actor explicitly hidden
by its live owner may be ignored. Invalid, stale, incomplete or ambiguous passes
never produce a selection. Source units are converted once before projection.

This helper has no runtime input or render caller yet. Native audience emission
still lacks published per-slot/item incarnations, confirmed harmful-item state
and visible-pass ownership. Do not turn its result into synthetic X: original
GetItemOn may choose a different first eligible holder. A real chosen-slot
adapter must revalidate the original item-on conditions and publish the actual
selected cache/position before existing SetTarget and retained UI End. Physical
X keeps its original independent query and target path.

## Concrete native acceptance scenarios

1. Enter the normal harbor tutorial from fresh-file original story progression.
   Capture both screens at HUD reveal and the first actual player-command state.
   Verify source dialogue/attribution, original HP/FP sprites and actual top
   ring/bubble presentation; a direct `beginFresh` fixture proves only that fixture.
2. Navigate every admitted class/weapon/target using physical D-pad or Circle Pad,
   confirm A and cancel B without touching the display. Observe disabled icons,
   bubble text and original selection hand. Return repeatedly through retained
   allocation/render/release waits; no invisible acknowledged menu or double input.
3. Capture trio command, target, action-command and return states. The upper menu
   must remain legible without covering its compact HUD. The bottom must show
   continuing audience rows, including while selecting commands, with no large
   submenu/dashboard replacing those rows.
4. Follow a recognizable original spectator across the seam during a source
   camera change. Upper Y240 and lower Y0 should meet at equal scale; lower X
   is upper X minus40 before clipping. Confirm no duplicated animation advance,
   random draw, actor update, pad update or action-command tick from the lower pass.
5. During a real eligible harmful throw, compare physical X with a tap on the
   actual displayed thrower. Both must use existing target/state eligibility and
   identical interruption ownership. Tap empty floor, a harmless spectator,
   outside320x240, an actor no longer eligible and an actor removed since publish:
   no forged target, no ordinary menu confirmation and no dialogue advance.
6. Hold UI release/render/resource services unavailable, then resume. No repeated
   target publication, counter increment, repeated sound or fake item clearing.
   Acceptance requires the eventual original action root and visible result;
   selecting type11 alone is not completed throw interception.
7. Capture nondefault HP/FP/partner/AP/coins/SP using real owners. Apply recovery
   and damage, verify live values before export, hidden HUD phases, digit widths
   and no zero-filled invented groups. Audit top/bottom fades and material/sampler
   restoration after disabled icon, message, audience and effect passes.

The existing `tests/battle_scene_views_tests.cpp` proves matrix/point seam,
horizontal crop, depth/W preservation and lens admission. It does not inspect
native pixels or normal story/runtime ownership. Existing direct-tutorial visual
evidence is scoped in [the two-screen contract](NATIVE_BATTLE_TWO_SCREEN_SCENE.md).
Use isolated `tools/emulator_smoke.py` profiles for new captures and leave user
profiles/saves untouched. A rendered fixture is useful evidence but must be named
as such; full acceptance remains the normal battle flow and hardware evaluation.

Related contracts: [HUD/submenu scope](NATIVE_BATTLE_HUD_AND_SUBMENU.md),
[two-screen continuation](NATIVE_BATTLE_TWO_SCREEN_SCENE.md), and
[class UI binding](../battle/ACT_SELECT_UI_ALLOCATION_AND_NATIVE_BINDING.md).
