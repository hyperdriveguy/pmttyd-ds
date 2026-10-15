# Native battle HUD pouch counters

2026-10-14; US G8ME01 revision0. `Renderer::battleHud` now adds original green
status banner475, Star Point symbol443, coin444, multiply mark478 and original
numeral sprites to the top-right HUD. The existing Mario HP/FP presentation,
visibility gates, physical controls and lower scene continuation are preserved.
Positions and spacing are compact native400x240 layout choices, not recovered
original status-window coordinates or slide/blink choreography.

## Actual source values

Read-only ISO disassembly establishes whole `statusGetValue8013c9ac/178`.
Both field and battle arms call `pouchGetStarPoint800d3424/c` and
`pouchGetCoin800d3e64/c`. The former loads global pouch8041eb00 then SIGNED
halfword+96; the latter loads the same global then SIGNED halfword+78. The cached
reference C declaration of Star Points as unsigned does not override actual
US `lha` instructions. Negative values stay unavailable in this native HUD.

`tools/battle_hud_assets.py::source_art` now verifies both exact getter bodies
and the complete status value producer hash
`bc743f6eb6f985fd41b52dd17d76d542b1939081d4de853b7d0b73c003d473fd`.
Its existing thirteen-row art table is unchanged. Rows6..10 give IDs
475,443,444,478,478. Returned metadata includes the verified getter addresses,
signedness and pouch offsets. This is static source evidence, not a Dolphin
observation or a completed implementation of the status-window controller.

Before a packed battle pouch exists, an actual retained `ProloguePouch` supplies
coins and Star Points. When the executing tutorial/trio owner exposes a nonzero
pouch through8041eb00, that live pouch takes precedence. Coin value is the high
signed halfword of WORD(pouch+78); Star Points are the low signed halfword of
WORD(pouch+94). If an executing pouch exists but either resource read fails,
that field is omitted rather than using a stale field snapshot. The other field
can still draw. With neither field available the entire green group is omitted.
No field pouch means no fabricated initial100-coin or zero-Star-Point fallback.

This handles the actual native ownership split: tutorial imports its packed
pouch at84400000; trio imports at85000000. Rendering follows the global pointer,
not either fixed address. Trio reward counting changes live pouch+96 before
field export, so the top count can follow those changes. Reads never mutate
pouch, update rewards, advance simulation or execute a menu controller.

## Assets and remaining scope

All five art IDs and original large/small numeral cells already exist in the
shared message-icon atlas, imported by the existing message icon converter from
original `icon.bin`/`icon.tpl`. No texture import, resize, regenerated art, new
atlas allocation or source asset edit was needed. Existing provenance/licensing
in [the asset ledger](../../ASSETS.md) applies; original rights grant no new
redistribution license. The tool metadata gains counter evidence only; original
status rows, numeral arrays and existing pixel conversion stay unchanged.

Mario's original portrait was already present as icon427 in the HP group. Partner
HP is still omitted: source status rendering selects partner-specific portrait
and additional banner records whose normal native used-atlas/resource/controller
binding remains incomplete. AP capacity/fill, full partner routing, status
slide/blink choreography and audience count remain separate work. Coins and
Star Points do not establish or substitute for any AP or spectator producer.

## Focused validation

`python3 -m unittest discover -s tests -p test_battle_hud_assets.py` passes3/3,
including original table/getter/producer pins and unchanged atlas pixel/UV checks.
The updated native translation unit passes devkitARM ARMv6K hard-float syntax
checking with actual libctru/Citro2D/Citro3D headers and project includes.

A private Werror host fixture extracts the actual `battleHud` function body
from the changed source and supplies explicit test storage/draw owners. It checks
field values, live packed-pouch precedence, correct high/low signed lanes,
missing-read suppression, negative-value suppression, tutorial-vs-trio ownership
and hidden HUD behavior. This source-slice fixture is evidence for those branches,
not a normal native runtime, raster or hardware acceptance test. Files are under
`artifacts/hud-counter-source-fixture*`; no fixture data is source game state.

Root owns full matched-pack `make test`/`make all` and isolated native captures
for this next batch. Verify readable top-right art/digit spacing, live reward
count changes and fades on both screens before claiming visual acceptance.
This feature was developed separately from the already frozen menu-resource
validation snapshot and is not covered by that earlier batch's build results.
