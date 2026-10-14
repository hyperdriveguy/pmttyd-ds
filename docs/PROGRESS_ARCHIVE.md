## 2026-10-14 — Source state6 registration and status-window arbitration

Committed `9a5e83c`: native player command launch and enemy action state6 now
use the existing complete source registration helper. Actor recapture, status/
category/gravity selection, RNG, real ROOT construction, actual event ID/owner
binding, wait deletion after construction and channel6 advancement replace
manual/native shortened paths. Selection/count publication stays separate;
suppressed registration does not invent attack completion or delete its wait.

Implemented shared original status-window priority/checker, Event OnOff and
field setters. Native harness entry projects source Init state5 EventOn then
SystemOn; state6 uses SystemOff, and state9 restores SystemOn under its original
Bef4 mask30 gate. Original icon/font HUD uses the native projected visibility;
open/close projection is immediate and original window slide animation remains
unimplemented. No source timing measurement or full renderer fidelity claimed.

Validation: shared `TMPDIR=artifacts/tutorial-admission-tmp
CMAKE_BUILD_PARALLEL_LEVEL=4 make test` passed220/220 C++ and138/138 Python tests.
Focused original registration/trio regression passed2/2. Native make all passed
and rebuilt 3DSX. Six complete status-window bodies passed read-only US SHA256
verification; existing Jump registry/state6 and root-constructor verifiers also
passed. No Azahar/Dolphin run this step. No new game payload or conversion;
source inspection stays ignored. Scoped commits preserve concurrent break-slot,
audio and other work; host/native validation reflects the combined tree.

Next: tutorial original menu/table/input and player action admission; complete
original action/turn sequencing, guard/lethal/status/counter and presentation
providers; source camera/shake, window animation, effect/audio/audience consumers.
Preserve original menu/HUD art and physical-button routes, with optional audience
taps on the lower screen. Full tutorial/battle objective remains active.

## 2026-10-14 — Original command attack selection and records

Committed `1e3d142`: complete shared _btlcmd_SetAttackEvent selector, weapon attack
classifier and borrowed unit-data lookup. All14 command modes/all9 subcommands
publish original event/category/byte stores and source selection counts, with
captured battle ownership, fresh audience-helper ownership, null-event Wait
fallback, exact special-pointer exemption and signed normalization. Native
trio confirmed commands call this selector before creating their source root;
Jump now publishes80355cf0/category0 and counts16f38 before execution. The
selector does not choose a menu row, run an event, fabricate damage or spend a
turn. Physical menu navigation/cancellation and optional touch remain intact.

Validation: shared make test passed218/218 C++ and137/137 Python tests; final
focused selector/trio rerun passed2/2 after adding the pre-execution native
record assertion. Existing read-only ISO verifier passed11 body identities,
301 instruction anchors,8 data spans and7 scoped DOL call sites. Initial native
link saw concurrent break-slot references before that new source was available
in the build. Once its object existed, rebuilding battle_attack_selection.o
and TMPDIR=artifacts/tutorial-admission-tmp make all -o assets-ready passed and
rebuilt 3DSX; full asset preparation had already run. No Azahar/Dolphin trace or
new game payload. Shared-index overlap was confined to a launch hunk explicitly
left staged by the concurrent author; scoped commit/index handling retained it.

Next: original tutorial weapon-table/menu/input ownership and player action
admission; use source state6 root registration and full action/turn controller;
connect camera, effect, audio and audience consumers, including optional lower
screen audience taps and physical alternatives. Guard/lethal/general status/
counter dependencies remain. Full tutorial/battle goal stays active; this
selection primitive is not a complete native tutorial or menu replacement.

## 2026-10-14 — Ordinary tutorial attack/reaction return

Committed bb063b1: shared original horizontal camera-shake request wrapper
8012b3d0/f0 and helper8012b4c0/60 now bind tutorial and trio. Signed priority,
integer-to-float coercion, operand captures, late global-battle load, packed
shake fields and conditional padRumbleOn(0) ordering follow the read-only US
bodies. Native adapters retain rumble requests. The dedicated tutorial fixture
now finishes the original ordinary Crump attack and timed Mario reaction:
HP7 remains6, Mario returns to M_S_1, both roots finish with no pending callback,
and installed Mario/Crump wait handles are restored. No turn/story advancement
or combat outcome is inferred. Guard/lethal boundaries remain retained.

Validation: shared make test passed217/217 C++ and136/136 Python tests, including
source-body SHA256 checks and operand/global-pointer mutation tests. The first
native make all stopped during NM symbol-list generation after linking; direct
NM then passed. Rebuilding the camera object and running
TMPDIR=artifacts/tutorial-admission-tmp make all -o assets-ready passed and rebuilt
ttyd-2ds.3dsx (asset preparation had already completed). No Azahar/Dolphin run;
these checks do not prove visible camera shake, hardware rumble, or complete
native tutorial play. The temporary source snapshot preserved concurrent
incomplete CMake work; the final full tests ran against the shared tree.
No new game payload was extracted/committed. Updated the impact contract to
reflect the now-completed reaction/attack return.

Next: source camera-main/shake consumption; tutorial original player command,
action and turn ownership; guard/lethal/status/counter services; actual effect,
audio and audience consumers. Preserve original menus/HUD and physical input;
optional audience taps belong on the lower screen. Full tutorial/battle goal
remains active, including complete native scene/audience presentation.

## 2026-10-14 — Original ordinary tutorial impact and Mario damage art

Committed `343d692`: tutorial CheckDamage now uses the shared original wrapper,
portable BattleCheckDamage controller, original default-power callback/tables,
HP/FP calculators and direct application for the admitted ordinary nonlethal
Crump-to-Mario impact. Host execution of the installed original Crump attack
with empty physical input takes HP7 to6, preserves source raw flags100 and
produces hit code10117, packed before/after/accounting/action records, original
sound/effect/audience requests and Mario's actual source hit root/M_D_1 pose.
The next unsupported request is camera8012b3d0; attack/reaction roots stay live.
Guard feedback and HP1 lethal admission retain CheckDamage without mutation.

Committed `bf1df7b`: dedicated original Mario tutorial pack now includes M_D_1.
Its host decoder produces visible geometry. Existing conversion uses original
animation keys,102 triangles per pose and24 texture slots in512x256 RGBA8;
private extracted/compiled payload remains ignored and licensing/conversion
choices are recorded in ASSETS. No generated placeholder or rewritten story.

Validation: initial shared make test passed214 C++/134 Python tests. Final
shared `TMPDIR=artifacts/tutorial-admission-tmp CMAKE_BUILD_PARALLEL_LEVEL=4 make
test` passed216/216 C++ and134/134 Python tests after the pose/accounting checks.
Both ordinary-impact native make all and rebuilt damage-pose make all passed.
The final focused tutorial/reaction run also passed2/2. No Azahar/Dolphin run
this step; source-script host execution and asset decoding do not prove native
particle, sound-macro, crowd or damage-pose playback. The Game still lacks full
original tutorial command/action/turn ownership; no manual battle-start or
story-completion shortcut was added.

Next: source camera8012b3d0 and original reaction/attack return; connect actual
presentation/audio/audience consumers; bind guard/lethal and general status/
counter providers; finish original tutorial player commands and turn flow.
Keep physical controls throughout, preserve original menu/HUD, and make audience
taps optional on the lower screen. Full tutorial/battle goal remains active.

## 2026-10-14 — CheckDamage calling-event ownership

Implemented `924b300`: the shared original CheckDamage event wrapper now owns
captured-battle/operand/output ordering and the live calling-ID termination
query. Native trio attacks use it around the existing supported core. Negative
core results preserve output and skip the ID query; nonnegative results write
output before checking live event existence. Tests deliberately disagree with
cached entry flags and mutate the global pointer, output token, and entry ID
at source service boundaries. Null defender/part is forwarded to the core.

Validation: shared `make test` passed 214/214 C++ and 134/134 Python tests;
the extended null-target/cached-flags fixture also passed after its final edit.
`TMPDIR=artifacts/tutorial-admission-tmp make all` passed. Static read-only US
ISO verification now pins 11 complete bodies; CheckDamage is 100 hex bytes,
including its final blr8010a888 (older fc annotations omitted that instruction).
No Azahar or Dolphin run for this event-lifecycle change. No new asset payload;
private inspection files remain ignored. Full shared changes are concurrent;
this commit includes only the wrapper, adapter, tests and source contract.

Next: install the tutorial's CheckDamage core with real calculator/application,
source hit-root dispatch, and effect/audience service ownership. The tutorial
still retains that core boundary. Keep physical defense samples separate from
dialogue input, and finish original command/action/turn ownership. The original
menu/HUD and optional lower-screen audience interactions remain mandatory;
this change adds no touch-only control. The full tutorial/battle goal stays
in progress.

## 2026-10-13 — Native verification round: event nesting, game-wide GSW census, result escape

Independent ISO-read verification of three documentation deliveries with
`tools/dol_event_inventory.parse_commands` and `tools/rel_event_inventory.py`
over `/home/carson/games/roms/G8ME01.iso` (read-only; artifacts in `/tmp`). No
native/source/test/CMake/asset change.

- **Child-entry nesting** (`docs/behavior/event/EVENT_CHILD_ENTRY_NESTING_AND_FLAT_PARSE.md`):
  reproduced exactly — 7290 events / 234878 commands over 29 REL modules; pairs
  107→109 2779, 108→109 120, 110→112 837, 111→112 106 (total 3842); closers 109
  = 2899, 112 = 943; 0 unbalanced, 0 cross-family; depth distribution
  5850/1434/6; the six depth-2 events match. The three cited `GSW[0]` stores are
  opcode 50 at depth 0 (values 2 / 403 / 9).
- **Game-wide GSW census** (`docs/behavior/story/SAVED_STORY_WORD_CENSUS_GSW.md`):
  counts reproduce exactly under operand-position-0 counting — 31 words / 2213
  refs; `GSW[0]` 427/1514; `GSW[1420]` 33/86; `GSW[1469]` 10/6 `{50,53,79}`;
  `GSW[0]` 394 distinct values 2..405 with gaps 22/36/126/177/186/281/349/351/
  357/381; the single non-literal store is `tou2/evt_random_battle2` cmd 83;
  `GSW[1469]` opcode-79 OR bits at `rsh/talk_cook` cmds 42/46/50.
  **One delivery error flagged:** `GSW[1420]` "exactly 32 distinct literals" is
  an off-by-one — the set listed (`0..30`) is 31 contiguous values, which is
  what the bytes give.
- **RESULT-01 escape mask arm**: `0x8034f038` = WaitEventEnd(LW15), then
  END_EVENT at 0x8034f044 before END_IF at 0x8034f048 — the mask2 arm ends the
  event and does not fall through to the terminal `InterruptStop(0)`.

Recorded in `docs/requests/DEMO-01.md` and `RESULT-01.md` (append-only), commits
`7ca08ef` / `62d7d05` / `d144b75`. Commit method: guarded temp-index
`commit-tree`/`update-ref` against the current HEAD, never the shared index
(a concurrent agent's plain commit had swept a staged set; the docs agent
documented that recurrence in `08861f3`, files intact). The doc-link regression
the deliveries introduced was already corrected by the docs agent in `fc1f090`
(checker back to 0 broken at 1103 files).

Limitations: static byte census only, no original execution; the `GSW[1420]`
off-by-one is flagged in the request file, not edited in the delivery. Next:
verify remaining open deliveries (STAR-01 fill callers, ITEM-01,
PARTNER-FULL-01) and file G1 requests for map/transition entrance types and UI
pause/focus token edges.

## 2026-10-13 — Tutorial ordinary Crump hit reaction

Implemented and committed `d1b2e99`: shared original GetDamage signed-byte
query and null-safe U16+1fe reaction marker, bound in tutorial and trio.
The original Crump ordinary 0x300|23 reaction now enters D_1, respects its
timed source interval, and finishes without a pending service. Tests retain
source root replacement/cancellation and unchanged HP/result. These callbacks
do not calculate/apply damage or complete a player attack.

Validation: native `TMPDIR=artifacts/tutorial-admission-tmp make all` passed.
Shared `make test` first could not configure while a concurrent CMake edit
named missing tests/field_item_registry_tests.cpp. Once that file existed,
the shared rerun passed 213/213 C++ tests and 134/134 Python tests.
A separate HEAD-based snapshot with only this change passed all 209 C++ and
133 Python tests. Its first Python run had used a stale copied seven-body
expectation; the corrected full rerun passed. Focused shared ISO verifier test passed, checking
10 complete read-only original US DOL bodies. No Azahar or Dolphin run this
step; host source-script completion does not prove visual playback.

Next: connect tutorial command/action ownership and complete attacks/turn flow;
retain physical-button paths, original command menu/HUD, and audience taps as
an optional second-screen action. Generic DefaultDamage wait restoration is
separate from the Crump fixture. Full tutorial, lower-screen audience handling,
and real battle delivery remain in progress. Original assets remain private.

## 2026-10-13 — ID 3 — NOTE: shared-index commit hazard (recurrence)

The game-wide GSW census and child-entry nesting contracts in the entry above
were staged with `git add` on the seven documentation paths only, and
`git diff --cached --name-only` confirmed exactly those seven. A concurrent
agent then committed (`d144b75`, "verify RESULT-01 mask2 arm WaitEventEnd
ordering against US ISO") and **swept the staged ID 3 files into their
commit**. The content is intact in HEAD and attributed through
`docs/requests/DELIVERY_LOG.md` (row "Game-wide GSW census, all29 REL
modules", agent ID 3), but the commit *message* does not mention it.

This is the third occurrence of the shared-index hazard. It is not avoidable
while the working tree is shared, so the mitigation is procedural: always
`git diff --cached --name-only` immediately before committing, and treat
`DELIVERY_LOG.md` as the authoritative attribution record rather than the
commit message. No files were lost and no other agent's staged work was
altered.

## 2026-10-13 — ID 3 — game-wide GSW census; 402/404 origins closed

Widened the event sweep from six modules to all 29 shipped `rel/*.rel`:
**7290 events, 234878 commands**. Delivered
`docs/behavior/story/SAVED_STORY_WORD_CENSUS_GSW.md` and
`docs/behavior/event/EVENT_CHILD_ENTRY_NESTING_AND_FLAT_PARSE.md`, and revised
the opening chapter table.

Closed gaps:

- **402's writer found**: `las/last_evt_4@las_29.o` cmd 489. This overturns
  the ordering earlier deliveries implied (402 pending -> 2 -> ... -> 403).
  402 is the last value of the `las` chapter band, written just before the
  epilogue's 403, so 402 is a late-game state and how a fresh save reaches
  the prologue gate is now open rather than settled.
- **404's writers found**: the two `end/unk_evt_end_*` events. Plus 405 by
  `gor/return_gorotsuki_town` cmd 206, a post-ending return to town.
- **GSW[0] is a 405-chapter counter**: 427 stores, 394 distinct values over
  2..405, ten gaps. One game-wide sequence, not per-area; `gor` alone spans
  3..405. Per-module chapter map tabulated. 281 is never written even though
  `gor_03` tests `>= 281`.

Corrections to my own earlier work:

- **GSW[1420] is not a boolean.** 86 read sites in 13 modules against exactly
  32 literals, 0..30. All 33 writers are opcode 50, 32 storing literal 0. No
  shipped REL event writes it 1..30; no producer invented.
- **My writer-opcode set was incomplete.** It omitted opcode 79 (`OR
  resolved`), which hid GSW[1469] - the only bitfield in the game
  (`rsh/talk_cook` does |= 1, |= 2, |= 4). Writer and non-writer sets are now
  both listed explicitly in the contract.

New method finding: `parse_commands` is a flat walk, so INLINE/BROTHER bodies
are indistinguishable from parent commands. Established over all 7290 events:
four openers share only two closers (INLINE_ID uses END_INLINE, BROTHER_ID
uses END_BROTHER), exact pairing counts 2779/120/837/106 = 3842, zero
unbalanced spans, max depth 2. Every previously published index was re-checked
at depth 0 and stands; no opener/closer falls inside any SWITCH ladder span.

Validation: read-only ISO re-read via tools/iso_assets.inventory and
dol_event_inventory.parse_commands; band and writer classifications taken from
existing class-B contracts, not re-derived. Docs-only; `make test` /
`make all` not rerun. No execution trace. ISO bytes and ELFs stayed in /tmp.

## 2026-10-13 — SERVICE-01 open-shape half: shop setup and flag

Delivered behavior/field/FIELD_SHOP_SETUP_AND_FLAG.md: `evt_shop_setup`
record build (216-byte alloc, currentMapName copy, stock loop over the
arg2 table) with key-item admission for 51/55–59 via `pouchCheckItem`, and
`shop_flag_onoff`'s shop-work bit switch at [[r13+7232]+0]. Both bodies
newly extracted read-only; callee identities from us-symbols. Flows,
prices, gating, presentation remain open. Complements the coin-* leaves
commit; docs-only, no make rerun. Full-game incomplete.

## 2026-10-13 — ID 3 — GSW census; GSW[1420] resolved

Closed the last named gap in the opening chapter table with a complete `GSW`
census over the six scanned modules (dmo, aaa, sys, gor, muj, tik). Every
operand in all sixteen events falling in the `GSW` band resolves to just
three indices:

- `GSW[0]` x60 - the chapter gate, the WORD exception at GP+174
- `GSW[1420]` x2 - **resolved**: `== 1` at gor_01 cmd 126 and `!= 1` at
  gor_02 cmd 63, each gating one opcode-91 call. Never written in scope,
  like 402 and 404.
- `GSW[108]` x1 - **not a flag read**: it sits in operand position 1 of
  opcode 91 at gor_01 cmd 64, a raw service argument.

That last item settles a question left standing from the area-init pass: the
`GSF` 5300-5600 numeric band seen in opcode-91 operand positions is
callee arguments, and `GSF[5393]` at gor_03 cmd 23 is a genuine flag read
only because its opcode is 24 in operand position 0. The opcode decides the
operand role, not the numeric band. Recorded as a three-row role table in the
contract, with the six-module scope limitation stated explicitly so it is not
read as a game-wide GSW layout.

Validation: ISO-read REL spans rescanned; band bases and the wrap32 index rule
taken from the existing EVENT_INTEGER_OPERAND_NAMESPACES contract rather than
re-derived. Docs-only change; `make test` / `make all` not rerun. No execution
trace.

## 2026-10-13 — ID 3 — opcode-34 SWITCH ladders on GSW[0]

Expanded all eight opcode-34 sites on `GSW[0]` in the opening modules,
closing the second-to-last gap on the chapter table. Ladder boundaries were
recovered by applying the already-documented next-case scanner
(`EVENT_SWITCH_STACK_AND_CASE_SCANS.md`: depth 1, opcodes 36..47, stop at
`END_SWITCH` 49) over the ISO-read REL spans. All eight sites terminated
cleanly, so each ladder is complete rather than truncated.

Headline results:

- `gor_02` cmd 0 is a 7-way `CASE_OR` 43 group on chapters
  18/64/120/174/221/271/347 with a `CASE_END` 46; `gor_02` cmd 112 is the
  *same seven chapters* as seven `CASE_EQUAL` 36 cases. Same membership,
  different dispatch form — the two sites are a cross-check pair, not a
  duplicate.
- `gor_03` cmd 17 tiles `[278, inf)` exactly with `BETWEEN` 278..280 plus
  `>= 281` and no `DEFAULT`; `gor_03` cmd 60 is an ascending `<` ladder where
  first-match-wins is load bearing (an accepted case writes state 0, so later
  cases short-circuit).
- Two ladders are **not ascending** (`gor_00` cmd 12 puts `== 268` after
  `== 402`; cmd 98 puts `BETWEEN 172..229` after `== 217`). They must not be
  sorted or binary-searched.
- Three of the eight ladders have **no `DEFAULT`**, so large chapter bands
  fall out with no effect at all.

Also resolves one open question: `GSF[5393]` at `gor_03` cmd 23 is a genuine
flag read (operand0 of `op24`, `wrap32(-129994607 + 130000000) = 5393`), so at
least that member of the 5300-5600 numeric band is a flag access rather than
a callee argument. Not generalised: in opcode-91 operand positions the same
band is a callee argument, so the role is per-opcode.

Validation: REL event spans re-extracted from the read-only ISO; scanner and
case semantics reused from existing class-B contracts rather than re-derived.
Docs-only change; `make test` / `make all` not rerun. No execution trace.

## 2026-10-13 — Original reaction control and no-damage completion

Feature `e8e6097` binds complete CheckAttribute, ResetFaceDirection and On/
OffUnitFlag callbacks, with original raw masks, source capture order, signed
belong and exact byte/word destinations. The trio no longer resolves raw flag/
attribute masks as script variables or uses unsigned belong for facing reset.
Tutorial CheckDataEvent/RunDataEventChild now use existing complete services,
including actual source child creation/owner inheritance. Seven complete query/
control bodies are pinned read-only; no asset/script/dialogue expansion.
[Native scope](behavior/battle/NATIVE_REACTION_ATTRIBUTES_FACING_AND_FLAGS.md);
[assets](ASSETS.md).

The actual code100|18 no-damage reaction now finishes through original VM roots,
retaining HP/result and the stored finished handle. Its earlier pending-root
assertion was correctly updated to test natural completion. Normal-element
code300|23 continues through metadata/data requests and remains live at
GetDamage8010f458 (signed amount270). That amount still awaits source damage
application; no HP change or complete attack is fabricated. Tests also cover
raw-mask domains, any overlap, pre-resolution facing ownership, signed belong,
adjacent-byte preservation and flag OR/off behavior. Initial mixed-auto C++
declaration error was corrected; stale-executable output from that failed build
is not final evidence. Original-data probes distinguish no-damage from ordinary
normal-element hit codes.

Shared `make test` reached210/211 CTest but failed the trio ending assertion while
concurrent audience/RNG bootstrap edits were in progress
(`artifacts/tutorial-reaction-final-tests.log`). A current-source configuration
also briefly referenced an absent audience test. The coherent HEAD-based snapshot
plus this feature at `artifacts/reaction-review/` passed209/209 CTest and133/133
Python (`artifacts/tutorial-reaction-review-tests.log`), invoked as
`TMPDIR="$PWD/artifacts/tutorial-admission-tmp" CMAKE_BUILD_PARALLEL_LEVEL=4 make -C artifacts/reaction-review test`.
The snapshot excludes uncommitted audience work; its passing result does not
claim that the mixed shared tree's audience ending assertion has been fixed.

Normal `make all` passed (`artifacts/tutorial-reaction-all.log`); debug build
with `-o assets-ready` passed (`artifacts/tutorial-reaction-debug.log`). Immutable
private image: `artifacts/reaction-services-native-snapshot/ttyd-reaction-services.3dsx`.
Isolated Azahar physical harbor route still reaches the original tutorial
stage/actors/HUD and existing action-service notice in
`artifacts/azahar-reaction-services/`. This validates introduction regression,
not native damage/turn completion or hardware parity. Final doc links:1091 files,
zero broken; earlier concurrent optimization-link failures are not final results.
Scoped diff checks pass. Unrelated edits/staging are preserved.

Next: bind GetDamage and remaining common damage motion/status/animation/effect
providers; complete original damage application/orchestration, startup/UnitEntry,
player command ownership and turn scheduling. Continue original rotating menus
and lower-screen crowd with optional hostile-item taps plus physical controls.
No-damage root completion is a source branch, not completion of the requested
battle/tutorial goal. Preserve original US dialogue/tokens and simulation ticks.

## 2026-10-13 — ID 3 — GSW[0] write set: chapter transition closed

Solved the top open question on DEMO-01. The previous delivery called the
chapter transition unprovable because opcode 50 stores
`evtGetValue(operand1)` rather than a literal. The handler claim was right;
the conclusion was wrong, because operand1 is an operand of the same
command and therefore lives in the event span. All three sources are
`operand > -20000000`, which `evtGetValue` returns raw.

Recovered write set for `GSW[0]` (only 3 of 54 `GSW[0]`-marker commands are
stores; the other 51 are comparisons, opcodes 24/25/26/28/29):

- `aaa/evt_prologue2` cmd 49 → **2**
- `aaa/epilogue_evt2` cmd 117 → **403**
- `gor/kurihakase_first_talk` cmd 176 → **9**

Load-bearing correction: **402 is not 403 and is never written** in the six
scanned modules (`dmo`, `aaa`, `sys`, `gor`, `muj`, `tik`). The epilogue
writes 403; all six `== 402` gates test 402; `gor_00` cmd 148 tests
`!= 404` and 404 is also never written. So 402/404 must originate outside
the scanned set and no origin is guessed. Also: `GSW[0]` is a WORD store
(`swByteGet`/`swByteSet` at index 0 operate on `WORD(GP+174)`), so a byte
store would truncate 403/404.

Self-correction: the remaining-gaps list named 3 opcode-34 SWITCH sites on
`GSW[0]`; the complete list is 8 (`gor_00` 12/98, `gor_02` 0/112, `gor_03`
17/60/158, `muj_00` 82).

Validation: `/tmp` REL event spans re-extracted from the read-only ISO and
rescanned; opcode-50 semantics and the immediate-band return are class-B
contracts already in `EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md` and
`EVENT_INTEGER_OPERAND_NAMESPACES.md`. Docs-only change; `make test` /
`make all` not rerun. No execution trace.

# Progress and next-session handoff

Newest entries first under this single heading. Keep entries dated, write link paths relative to `docs/`, and record exact validation, limitations and next tasks in each entry. Append-only: do not rewrite earlier entries.

## 2026-10-14 — Original key-item fanfare export and native streaming provider

Committed `8a80166e8c28d0225ca7948bf7096f4783934d25`. Original key-item
fanfare is now packaged with ISO provenance/PCM hash and source row217,
volume80/pan64. Native track18 always uses the original GameCube asset in both
soundtrack modes. Fresh playback takes an available existing music slot, streams
through the existing three4096-frame buffers and exposes -1 vacant/0 assigned.
It refuses duplicate/full-pool admission instead of overwriting another owner.
Exact source stereo gain is published before first buffer submission. Natural
retirement waits producer EOF and absence of queued/playing NDSP buffers;
explicit backend release and shutdown clear submissions before releasing storage.
No fixed duration timer or extra full-PCM allocation is used.
[Native scope](behavior/audio/NATIVE_KEY_ITEM_FANFARE_PROVIDER.md),
[source stereo gain/group fade](behavior/audio/FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md).

Independent review corrected two issues before final validation: initial gain
publication must precede queuing, and stale fanfare loop sidecars must prevent
cache reuse. Final review confirmed both fixes and found no other concrete
slot/buffer/mutex/shutdown issue. Another session's adjacent existing audio-policy
test assertion was preserved during narrow private-index commit resolution;
no foreign working/staged changes were discarded.

Exact validation:

- Review tree `0501e87ca8f0c776b599e99d47c1dfafa29009ec`, baseline
  `44dc9b85101b2cb7baeca24dd7f4a5eb704782ec`; all2332 tracked files matched
  the isolated review tree (`artifacts/item95-fanfare-source-audit.txt`).
- First host run exposed an older isolated pack missing its new fanfare manifest
  field; regenerated through the source exporter, then final `make test` passed
  210/210 CTests and135 Python tests (`artifacts/item95-fanfare-reviewed-test.log`).
  Asset tests verify original stream provenance/hash, row/gain/pan, stereo32000Hz,
  83888frames/PCM16, exact PCM hash and absence of loop sidecar. Final
  `make -j4 all` passed without compiler warnings
  (`artifacts/item95-fanfare-reviewed-native.log`); exporter log
  `artifacts/item95-fanfare-reviewed-assets.log`.
- Preserved normal artifact `artifacts/ttyd-item95-fanfare-reviewed.3dsx`, SHA256
  `2227c085988f4d5848ab258bcb9ee98afa150fe42dd7e572413343b65203b486`.
- Private harness `/tmp/item95-fanfare-preview.py` modified only isolated
  source/runtime/main.cpp after preserving that normal artifact; stable test artifact
  `artifacts/ttyd-item95-fanfare-preview.3dsx`, build log
  `artifacts/item95-fanfare-preview-build.log`. Harness applies no story/pouch
  writes and uses a private development checkpoint. It invokes provider methods,
  logs results and explicitly releases the second play; release is not sourceOff201.
- `/tmp/item95-fanfare-audio-capture.py` used a temporary Pulse null sink and
  isolated tools/emulator_smoke.py profile/save at `artifacts/azahar-item95-fanfare`,
  with the existing local DSP firmware copied only into that profile. All tools
  exited0 and temporary sink was unloaded. Native log: ready1; begin at tick60
  query0; duplicate refused; natural query-1 at tick217 (2.624048s after start);
  second begin tick307/query0; explicit release tick337/query-1. Actual drain
  drove natural vacancy. Captured48000Hz stereo PCM is nonzero (peak18434),
  retained in provider.wav/statistics; viewed boot.png showed the field at60FPS
  without an exception. These prove bounded backend playback/lifetime, not a
  full original event or matched source-device audio schedule/loudness.

Next: bind original Off201 integer group fade/detached100ms tail and group0
quantized attenuation/retained-cursor resume before wiring the original
acquisition acknowledgment. Explicit `releaseKeyItemFanfare` is backend cleanup,
not an equivalent Off201 callback. Then integrate original Goomba3 quest2 event,
item-main order/second active check, control/motion/camera/windows/effect/pouch
services and5405 after wrapper completion. Source forced replacement/full-pool
arbitration, original SoundSS prefetch/IRQ/codec/device cadence and hardware
performance remain incomplete. The overall remake and requests remain active.

Audio comparison follow-up: the initial isolated Azahar profile enabled audio
stretching, causing variable alignment and weak whole-wave comparison despite
99.3–99.94% explained energy in local stereo segments/source gain near80/127.
Read-only diagnostics are in `artifacts/azahar-item95-fanfare/diagnosis-summary.json`
and local/stereo fits. No concrete ring-buffer defect was found.
A NEW profile `artifacts/azahar-item95-fanfare-unstretched` explicitly disabled
stretching (`enable_audio_stretching=false` and its default=false; runtime log
confirmed false); `/tmp/item95-fanfare-audio-capture-unstretched.py` repeated the
same stable test binary, temporary sink/private save/DSP setup and clean exit.
Native ready/begin/duplicate/natural-drain/release results all passed again.
The complete48000Hz stereo capture correlates0.98832866 with original32000Hz PCM
under linear reference resampling; fitted gain0.62030529 vs requested0.62992126,
peak18491. `original-pcm-comparison.json` retains that evidence. Resampler/phase,
source-device timing and physical loudness parity remain unverified; this is
strong native PCM/provider evidence, not complete MusyX/hardware equivalence.
Final documentation link check:1118 Markdown files/zero broken links before
index/handoff insertion; all inserted links use existing checked destinations.

## 2026-10-14 — Original fanfare retirement and container loop correction

Source review closed the key-item fanfare ambiguity, enabling faithful native
item-get audio lifetime work. Original SoundSSPlayChEx_main selects looping from
STM COMMON+0c, not DSP channel loopflags. For `ff_get2_32k.stm`, ffffffff means
one-shot; producer end sets pending stop, stream main publishes flags0 before
close/free and BGM main then publishes group1 vacancy. Native acknowledgment
can stop it earlier. No elapsed2.6215-second timer substitutes for those owners.
[Source contract](behavior/audio/FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md).

`tools/audio_convert.py` now exports original container loop policy and removes
stale sidecars for one-shots. Non-sentinel offsets must be frame aligned and
inside the source encoded extent, then map to restartByte/8*14 through actual DSP
sampleCount. COMMON total must match ceil(sampleCount/14)*8. DSP start/end must agree
with admitted container restart/extent; independently reviewed malformed bounds
are refused, with tests. All146 original both-channel DSP ends match2*total+2. Original coefficients,
initial histories, final-frame sample extent, sample rate and channels are kept.
This fixes35 streams that the prior converter incorrectly treated as looped.
Existing converter fingerprints invalidate old converted-library caches; the
native build rebuilds its selected PCM tracks. Full ignored-library rebuild is
`make audio-assets`. No original assets enter source history.

Exact validation:

- Independent read-only ISO source review surveyed all146 original stream
  headers:111 container loops,35 one-shots, every DSP loopflag1. An additional
  local source-provenance/hash audit checked both channels, restart/DSP relationships
  and sample/encoded extents for all146 (`artifacts/item95-stm-header-audit.json`).
- Original `ff_get2_32k.stm` hash847b529744401aa5590b821ff7502a65b1ac59045306dc8792d6d44b4e05f7dc
  decoded at32000Hz/stereo/83888 frames, with no loop sidecar, in ignored
  `artifacts/item95-audio/`; metadata `conversion.json`. It is not yet packaged
  as native item-get playback. Tests include DSPloop1/container sentinel, stale
  sidecar removal, stereo sample order/extent, intro-loop preservation and invalid
  container offset/extent refusal.
- Review tree `44dc9b85101b2cb7baeca24dd7f4a5eb704782ec`, based on prior
  BGM query review tree6c5ddfbb5f2dafd8c641edfa877080a07137a34f. All2331 tracked
  sources matched that isolated tree (`artifacts/item95-stm-source-audit.txt`).
  `make test`:210/210 CTests and134 Python tests passed, log
  `artifacts/item95-stm-reviewed-test.log`. Native build evidence appended below.

Next: package the original key-item fanfare and bind native streaming/group1
vacancy, field-group attenuation/retained-cursor resume and early acknowledgment
cleanup. Then wire the original Goomba3 quest2 event through retained item-main,
control/motion/camera/window/effect services and once-only pouch grant;5405 follows
wrapper completion. Source buffering/IRQ/decoder-loop histories, original-device
cadence, physical3DS profiling and playable acquisition remain unfinished. The
remake and parent requests remain active. Shared foreign changes are preserved.

Committed implementation `aef09a4b6cb64a2b61b90e9395bffa0db8f638aa`.
Final `make -j4 all` passed (`artifacts/item95-stm-reviewed-native.log`);
normal artifact `artifacts/ttyd-item95-stm-reviewed.3dsx` SHA256
`c62f81d690195c6b1ae04d20b549278ef29b7d4a4e4747bd6dc6317dc0928ba4`.
This rebuild verifies source conversion/package compatibility; no new emulator
or hardware audio capture was run. Independent source/admission review caught
the mismatch checks above; final targeted7 audio tests and full suite passed.
Documentation link check passed1112 Markdown files/zero broken links before
index/handoff append; new relative links use the same checked destinations.

## 2026-10-14 — Original item registry, camera5 parcel icon and BGM query

Committed `c1ce7485dc373c43b5078b56ba456e4fc32da47e` (original field-item
ownership/native icon) and `1119998a1cd7b7458478f9d383e7f30668faee6a`
(raw source BGM vacancy query). Host mechanics preserve independent32-slot
item/icon pools, first-inactive allocation, retained stale item names, active
duplicate icons, force/poll predicates and name-wide deletion. Item/icon flags
remain separate; the second active check gates icon synchronization after an
owner callback. Native short-name/available-pool admission is explicit policy;
source full-pool eviction/autonaming remain unbound.

The ordinary ready parcel icon uses original descriptor307/TPL31240x40 from
the existing original atlas, source.6 reduction/24x24 bottom anchor and camera5.
PICA/C3D projection, source color/alpha/depth states and a lazy6912-byte32-icon
arena avoid per-frame uploads and vertex overwrite. The map40 grayscale branch
and metadata2000 pending state are retained/refused rather than guessed. Source
camera4 queues/prepared vectors, source cylinder shadow and other icon domains
remain future bindings. See [native provider](behavior/display/NATIVE_ITEM95_REGISTRY_AND_ICON.md),
[entry/name/icon recovery](behavior/field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md),
and [map blend/shadow admission](behavior/display/ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md).

Audio recovery proves `psndBGMChk(1)` is -1 when vacant and0 when assigned.
The portable get callback now returns that raw result; tests cover assigned-to-
vacant field-music resume before acknowledgment and prevent a repeated resume.
Original group0 resume retains its stream cursor. Original key fanfare binds
`sound/stream/ff_get2_32k.stm`; its source header loops, so backend request/loop
retirement must be recovered before claiming automatic one-shot completion.
[Audio contract](behavior/audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md)
records source flags, attenuation, routing and remaining ownership questions.

Exact validation:

- Registry review tree `7bd8694af248a6aee48ac075e53707f7ca43737b` (baseline
  `71e5c31`) passed `make test` (210/210 CTests,133 Python tests) and
  `make -j4 all`; logs `artifacts/item95-registry-reviewed-test.log` and
  `artifacts/item95-registry-reviewed-native.log`. Tracked-source byte audit is
  `artifacts/item95-registry-source-audit.txt`.
- BGM correction review tree `6c5ddfbb5f2dafd8c641edfa877080a07137a34f` passed
  the same210/210 CTests,133 Python tests and native build; logs
  `artifacts/item95-bgm-reviewed-test.log` and
  `artifacts/item95-bgm-reviewed-native.log`. All2331 tracked files matched the
  reviewed tree (`artifacts/item95-bgm-source-audit.txt`). The portable owner has
  no native service caller yet, so normal artifact remains byte-identical:
  `artifacts/ttyd-item95-bgm-reviewed.3dsx`, SHA256
  `5377bf0889932b8b6cb03d9f85be95eec256c4dba8b954d3cadc350da34e3f3e`.
- Isolated `tools/emulator_smoke.py` ran stable graphics-only artifact
  `artifacts/ttyd-item95-registry-icon-preview.3dsx`, private profile/save fixture,
  input `/tmp/item-get-windows-steady-input.py`, output
  `artifacts/azahar-item95-registry`. Viewed `windows-steady.png`: original Box
  icon over Mario, original name/help, effect behind it, Azahar60FPS. Viewed
  `windows-closed.png`: icon/windows/effect retired and field view restored,
  60FPS. Harness source/build is private; normal artifact was preserved before
  instrumentation. This does not execute real acknowledgment, grant95, write5405,
  validate fanfare audio, match source GX pixels or prove physical3DS throughput.
- Documentation link check passed1108 Markdown files/zero broken links before
  the final index/handoff append; final check recorded below if count changed.

Next: bind native item-main dispatch and get services to the original Goomba3
quest2 event, including control/motion/camera/windows/audio and once-only pouch
mutation; verify actual branch predicates and saves. Resolve stream-loop
retirement via original sound backend before automatic fanfare vacancy. Preserve
source item/icon slot order and second active check when wiring dispatch. Then
verify the playable original acquisition, not merely its graphics preview.
Extend area objects/NPCs through shared source-derived registries and event
services. The remake and parent requests remain incomplete. Other sessions'
working/staged changes were preserved by narrow private-index commits.

## 2026-10-13 — Original projected item-get effect and camera5 provider

Committed `5b7b910`: portable source projected texture matrix and explicit
camera4-default/camera5-request field, finite camera5 frame from copied original
eye/up/target, and native original effect6/count1 two-pass renderer. Recovered
image44 POS texgen, separate image42 UV, repeat/mirror sampling, actual GX
blend/Z/alpha state and camera5 independent projection/depth. PICA vertex shader
projects the original texture matrix; three hardware combiner stages preserve
source gray/orange interpolation and raster modulation without tint baking.
Lazy65KiB original texture residency and12-vertex432-byte arena; original
clock/fade remains independent of drawing. Synchronized inactive/shutdown
resource release and following UI depth/texture state restoration are explicit.
Corrected “layer5” terminology: payload+38 is camera5, display class2.
[Native scope](behavior/display/NATIVE_ITEM_GET_EFFECT6.md),
[GX/projected texture](behavior/effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md),
[camera5 producer](behavior/camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md).

Exact validation: fixed complete baseline d18faf7 plus owned host changes formed
review treea8a02694a3fea07fc682c2dba05019bbc2126c6c; `make test` passed
209/209 CTests and133 Python tests. Independent source review then caught
camera5's RGB-only framebuffer-write mask; correcting that3DS-only source formed
finaltree7d0a5282b76091d1c50e9a45ecd2ed75fea77593. Final `make -j4 all`
passed, all final tracked review files byte-audited before preview instrumentation.
Logs `artifacts/item-get-effect-reviewed-{test,native}.log`.
Normal binary `artifacts/ttyd-item-get-effect-reviewed.3dsx`, SHA256
57df4d180cae4cd871a9743ca52c6126d0508b082c652fbad535bf7cc6ba29e0.
Other active sessions' changes were excluded from validation and preserved.

Isolated Azahar graphics-only harness displayed original M_I_U, title/help and
projected dotted effect using separately prepared camera5 lens25/near1/far100,
source viewportdepth.2/.3. An effect-only second capture confirmed original
image42 starburst opacity silhouette unobscured by title/help. Both settled
captures show60FPS(native rollingCPU~9.3..9.4ms/GPU.5ms) in software Azahar;
this is emulator observation, not hardware profiling or matched Dolphin pixels.
Harness explicitly requests soft deletion at600 simulationticks, then returns
to ordinary drawing with no native exception. It tests rendering/deletion,
not original acknowledgment or quest acquisition. Captures under
`artifacts/azahar-item-get-effect{,-outline}/`; outline binary/script retained
ignored for inspection. Private Main/Render instrumentation removed afterward.
Only isolated generated test profiles/checkpoints used; no user emulator saves,
item grant, quest/story flag write or invented dialogue.

Documentation scan found7 broken header links in another session's new
optimization request files; corrected just their relative paths in the worktree,
preserving those untracked files for their owner. Subsequent documentation link
check passed1089 files/0 broken. New contracts stay in subsystem folders.

Next: supply source camera4 prepared vectors/outer update and camera5 display
queue/depth-sort rather than infer them from final camera4 view; bind original
icon307/TPL312, key/fanfare/audio/control and named item registry. Connect the
existing original receive/window/effect providers to actual Goomba3 quest2
acquisition and5405 progression. Failed-receive discard and global window
allocation remain pending. Continue remaining original NPC/event/area content;
this is an early slice and does not complete the broad remake goal.


## 2026-10-13 — SERVICE-01 coin half: add/set/gate leaves

Delivered behavior/battle/POUCH_COIN_TRANSACTION.md: +120 coin field with
complete add/set 0/999 clamps and returns, plus newly extracted coin_chk
(affordability gate with global price-table read, 1/0 destination write).
One of my own check assertions was wrong (store form); doc unaffected and
re-verified. Purchase/deduction path has no reference body — scoped open.

Validation: store/clamp/gate re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: shop flows, inn services. Full-game
incomplete.

## 2026-10-13 — STAR-01 storage half: gauge add/set leaves

Delivered behavior/battle/STAR_POWER_GAUGE_STORAGE.md for requests/STAR-01.md
todo 1 (storage): pouchAddAP/pouchSetAP complete add/floor/cap at +122/+124
with US rows, 2-link-site count, and an explicit storage-vs-policy boundary.
Two of my own check-script assertions were wrong (store counts); the doc was
unaffected and all claims re-verified. Fill sources, thresholds, max writers
and save mapping stay open.

Validation: store-shape re-checks, link audit, diff --check clean. Docs-only,
no make rerun. Full-game incomplete.

## 2026-10-13 — Filed STAR-01 and cross-checked pouch offsets

Filed [STAR-01](requests/STAR-01.md) on the full-game path and added its inbox
and milestone-map rows: the Crystal Star / star-power gauge / special-move
system. Grounded in the absence of any star-power contract (no behavior doc
mentions star power, crystal star or special moves) and of any native
star-power/special-move symbol, while the audience pipeline that charges the
gauge is already recovered
([battle fortune](behavior/battle/BATTLE_FORTUNE_PRODUCER.md)). The request asks
for the gauge fill/consume rules, the battle Star menu, per-move action-command
resolution and effects, the star-to-move mapping and per-chapter story gating,
and the presentation. Scoped G1 (the prologue grants no star).

Also cross-checked the delivered ITEM-01 pouch contract against the native
code: `source/inventory/prologue_pouch.cpp` initializes key items at offset `0xa0` with
121 slots, coins at `0x78` to100, and the equipped-badge region at `0x38a` with
200 slots, matching the recovered
[pouch remove](behavior/battle/POUCH_REMOVE_ITEM.md) layout (key `+0xA0`/121,
equipped `+0x38A`/200). No discrepancy found; the held region (`+0x192`,
10/20) is not exercised by the prologue init, consistent with the contract.

Validation: doc-link audit 1067 files / 0 broken before this commit; docs only,
no make rerun. Limitation: the pouch check is a source-consistency check, not an
original execution trace. Concurrent-session commits continue to build on these
commits; if `6b4494b`/`997a698`/`d18faf7` leave the log, the content is in the
worktree and needs re-committing. Full-game incomplete.

## 2026-10-13 — Filed FIELD-ENEMY-01 (roaming field enemies)

Filed [FIELD-ENEMY-01](requests/FIELD-ENEMY-01.md) on the full-game path and
added its inbox and milestone-map rows. Grounded in native source: the field
actor record carries only `kind` 0=NPC/1=map object with a wander domain and no
enemy kind or aggro/contact fields, and the only field battle the port drives is
the scripted harbor Crump encounter. The request asks for the original roaming
enemy kind/registration and per-area lifetime, the field AI (notice/aggro,
approach/retreat, speeds, timers), the Mario-contact and first-strike trigger
that hands off to the existing
[field battle entry/return protocol](behavior/field/FIELD_BATTLE_ENTRY_AND_RETURN.md),
the danger presentation and the post-battle respawn. Scoped G1 (the demo's
encounter is scripted); it must not be short-circuited by a scripted-only entry.

Validation: doc-link audit 1012 files / 0 broken; docs only, no make rerun.
Limitation/incident: the first commit of this change (`08f9605`) was dropped when
the concurrent session ran `reset: moving to HEAD~1` on top of it (reflog
HEAD@{3}); the worktree content survived and it was re-committed on the current
tip with only these paths staged. Next: continue the D1/G1 request loop and
verify delivered answers against the read-only ISO before applying. Full-game
incomplete.

## 2026-10-13 — ITEM-01 removal symmetry: pouchRemoveItem

Delivered behavior/battle/POUCH_REMOVE_ITEM.md: table select (held/key/
badge with verified +0x192/+0xA0/+0x1FA/+0x38A offsets), 10/20/121/200
capacities, scan-and-compact with single/dual-table variants, TRUE/FALSE
returns — matching the snapshot source branch for branch. Strengthens
ITEM-01 todo 1 alongside the acquisition contract.

Validation: offset/capacity/branch re-checks, link audit, diff --check
clean. Docs-only, no make rerun. Next: effects/consumption, save mapping.
Full-game incomplete.

## 2026-10-13 — Original item95 name/help window owner and native drawing

Committed `37e7946`: finite original name/help definitions, explicit manager
update/display phases, source sequence0 reuse, partial Entry/Open initialization,
AutoDelete retained-parameter lifetime and prior-display help publication.
Original US title/description remain verbatim with control tokens. Direct font
color commands compile once at preparation; native drawing reuses cached glyph
input/color requests, original font/frame textures, C3D batches and PICA clip.
Name starts black, original Box token is red, /col resets font palette0; frame
snapshot precedes name rectangle resize, matching the source display order.
[Native scope](behavior/display/NATIVE_ITEM95_WINDOWS.md),
[source display review](behavior/message/ITEM95_GET_WINDOW_DISPLAY_ORDER_AND_FONT_COLORS.md).

Exact validation: complete known baseline21eee2a plus owned changes formed
review tree3d2205ca52da4327b0ce18ebeb2e132b0c8b7459; all tracked review files
were byte-audited before private preview instrumentation. `make test` passed
206/206 CTests and132 Python tests; `make -j4 all` passed. Logs
`artifacts/item-get-windows-reviewed-{test,native}.log`.
Normal binary `artifacts/ttyd-item-get-windows-reviewed.3dsx`, SHA256
6691fd2e636702a3e4660c470066e1da1f245bfa06353b90ba9ff76a8e5ef67a.
Initial latest-shared-HEAD validation hit an unrelated uncommitted battle-header
dependency; validation moved to the last complete baseline. Native compiler
then found int32_t/long std::max deduction mismatch; explicit int32_t fixed it.
The final complete validation above includes the fix and cached color path.
Other active sessions' staged/uncommitted work remained excluded and preserved.

Isolated Azahar graphics-only harness rebuilt and visibly drew unchanged
original acquisition title and two-line Box description over Rogueport, with
original M_I_U pose. Automatic closure returned to ordinary field drawing;
no native exception. Captures `artifacts/azahar-item-get-windows/` and
`artifacts/azahar-item-get-windows-steady/`. Settled open and later closed captures
report60FPS in software Azahar. Settled native metric CPU9.4ms/GPU.5ms is an
emulator observation, not a hardware measurement. Initial open capture reports9FPS in the native rolling
metric and includes cold resource admission, not steady-state performance.
No hardware performance/raster equivalence claim. Private preview source
instrumentation is removed after checks; isolated generated profile/checkpoint
only, no item/pouch grant, story/quest flag change or user profile/save mutation.

Limitations/next: this is a native-capable name/help provider, not yet actual
Goomba3 acquisition. Global manager allocation, failure/discard window, source
outer cadence, effect two-texture shader/camera/layers, audio/control/name entry
teardown and native event binding remain pending. The existing receive owner
must not restore success at an unimplemented discard boundary. Connect the
remaining original providers, enable quest2 acquisition/5405 only once the
whole sequence can run, then continue remaining original NPC/event/area content.


## 2026-10-13 — Native original hit/reaction roots and metadata queries

Feature `d852ae4` connects original BattleRunHitEvent/Direct to the tutorial VM:
installed or explicit source roots, original code40/data46 guard, ordered
wait/phase/attack/reaction cancellation, actual owner IDs, full code278 and
tracked damage handle2b4. Source main-body IDs and the complete body getter are
bound. Live reaction observation gates scheduler bridges, preserves finished
handles for their original cleanup owner and distinguishes hit-canceled intro
from natural completion. Replacing a pending child clears its event-owned
service diagnostic. No HP/result/turn completion is invented.
[Native reaction contract](behavior/battle/NATIVE_HARBOR_HIT_REACTION_OWNERSHIP.md).

Shared CheckDamageCode/GetDamageCode/CheckDamagePattern wrappers preserve raw
mask/pattern/output tokens, post-translation global reload, current unit words,
any-bit code overlap, unsigned low-byte getter and exact pattern equality.
The former trio pattern operand resolution is corrected. Trio call sites were
incorporated into concurrent `934236d`; this feature commits their shared library
and build dependency alongside native tutorial bindings. Three complete bodies
are pinned read-only. Existing HitEvent/Direct source fingerprints remain valid;
no imported payload/texture/dialogue expansion is needed. [Assets](ASSETS.md).

Original-data tests execute actual Crump/common-child and invisible system hooks,
check cancellation/replacement/owner2, null direct script and skipped code40,
preserve HP/result, and retain a live reaction at CheckUnitAttribute8010e430.
That is the next source-service boundary, not complete damage playback. Original
intro/physical defense tests still pass. Full `make test` passed209/209 CTest and
132/132 Python (`artifacts/tutorial-hit-final-tests.log`), invoked with
`TMPDIR="$PWD/artifacts/tutorial-admission-tmp" make test`. The newly added source
verifier test then passed separately, and final full Python discovery passed
133/133 (`artifacts/tutorial-hit-final-python.log`). An earlier shared test build
failed while stage-work dependencies were being added; its unresolved symbols
are retained in `artifacts/tutorial-hit-tests.log`, not counted as final success.

Normal full `make all` passed (`artifacts/tutorial-hit-final-all.log`); debug build
with `-o assets-ready` passed (`artifacts/tutorial-hit-debug.log`). Immutable private
snapshot: `artifacts/tutorial-hit-native-snapshot/ttyd-tutorial-hit.3dsx`.
Isolated Azahar physical harbor route/introduction regression passed in
`artifacts/azahar-tutorial-hit-regression/`, showing original actors/stage/HUD and
the unchanged action-service notice. Native hit dispatch remains an original
producer bridge, not a new development input action; reaction execution evidence
comes from the real host VM with original private scripts. No live original
runtime, complete attack or hardware-parity claim. Doc links/diff checks pass.

Next: bind the common damage child's remaining attribute/animation/status/effect
services and complete original damage orchestration, startup/UnitEntry, player
commands and turn scheduling. Native command and damage producer ownership is
still incomplete. Preserve original US dialogue/tokens and tick-based input.
Complete rotating menus and lower-screen scene/audience with optional hostile
item taps and physical controls. Full requested battle/tutorial end state remains
open; pending callbacks must keep their actual source roots alive.

## 2026-10-13 — Original item-get resource providers and native pose bank

Committed `fb4bd41`: read-only original M_I_U static pose importer, actual
base effect bank42/43/44 conversion, portable US item-title expansion and source
item-get effect6 update/draw plan. Native renderer selects the authored pose
from its separate2MiB bank, admitting it only when requested and synchronizing
retirement on ordinary-pose return, room change and shutdown. Exported pixels,
packs and source provenance remain ignored; no generated replacement artwork.
Corrected earlier effect quad documentation from±2 to original±32/y0..32.
[Native scope](behavior/display/NATIVE_ITEM_GET_VISUAL_RESOURCES.md),
[US expansion/window gates](behavior/message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md),
[startup color/quad verification](behavior/effect/FIELD_ITEM_GET_TYPE6_STARTUP_COLOR_AND_QUAD_VERIFICATION.md).

Exact validation: isolated review tree23374b766394f561723e650d96e97a98236f8422
(all tracked files byte-audited before preview instrumentation) passed
`make test`:205/205 CTests and132 Python tests; `make -j4 all` succeeded.
Logs `artifacts/item-get-visual-reviewed-{test,native}.log`.
Normal binary `artifacts/ttyd-item-get-visual-reviewed.3dsx`, SHA256
57f5b0282a776d3e04488d7a2541d475a5b2a5dfdc3fe9927dd1f764ece81440.
Initial isolated retry missed local PNG pixel fixtures; copying the three
original conversion fixtures resolved it, with final complete checks above.
Other active sessions' uncommitted/staged work was excluded and preserved.

Isolated graphics-only Main harness rebuilt and ran via emulator_smoke.py
using a project-local40-byte test checkpoint/profile. Captures
`artifacts/azahar-item-get-visual/original-item-get-pose.png` and
`ordinary-pose-return.png` visibly show authored raised-hands pose and return
to ordinary Mario drawing; no native exception. Later capture reports60FPS in
software Azahar, not New2DS hardware performance. Cold initial capture reports
10FPS and is insufficient for a steady-state or allocation-cost conclusion.
Private Main instrumentation was removed after capture; no quest/pouch grant,
story event, flag write or user emulator profile/save change was part of this
preview. Full GPU/window fidelity is not established by a pose smoke check.

Next: connect original window action/open/AutoDelete gates and source item95
receive services; implement two-texture GX-to-PICA effect shading/camera/layer
ordering and audio/control/name teardown; then enable and check actual Goomba3
quest2 acquisition/5405 progression. Keep full-storage discard continuation
explicitly pending. Continue remaining NPC/event/area bindings using source
contracts and shared resource lifecycle; the remake remains an early slice.


## 2026-10-13 — Original portable item95 receive owner and packed insertion

Implemented finite fresh mode16/main1003 itemseq_GetItem owner with typed
original service requests: same-call0/1 control/motion/effect, source50/62
positions, effect6, correct silence/BGM/fanfare routing, both original window
definitions before deferred receive95, next-call2/3 control-off, window action0
and A/B-trigger acknowledgment, original teardown/restoration and all-name
entry cleanup. No fixed500ms/render delay, fabricated dialogue, parameter
revision or task5405 write. Failed receive opens original failure window and
reaches source sub5 close/stay/iconflag2/type1 selection boundary; discard
continuation remains pending, holding control and not manufacturing success.

Added exact packed key-array insertion: highest zero120..0, backward overlapping
shift, front insertion, preserving gaps/duplicates/raw bytes. Full121 slots
return0 without writes. ProloguePouch exposes the array branch, not the full
resource/upgrade dispatcher. Tests use actual packed pouch through the receive
callback, proving once-only grant after both windows and untouched HP/FP.
Corrected legacy unit/POUCH_GET_ITEM.md's erroneous lookup/pointer description;
verified US receive is800d50cc and95 returns success1/failure0.

Source documentation now pins name/help definitions/callbacks, US template
expansion dependency, static original a/a_mario M_I_U pose, icon307/TPL312,
and actual e/us/effect.tpl bank/type6 clock/scale/two-quads/TEV. Prior get-item
frontier shorthand is superseded by the recovered direct receive sequence.
Full effect pass0 color producer/PICA mapping, US expansion and window-manager
services remain explicit providers. Native Goomba3 acquisition is not yet
playable and stays unavailable until those providers are supplied.

Final scoped source treee85100b42222092ca87a84c5653c4ea2ab7af37f verified2264
tracked files byte-for-byte. make all passed (artifacts/item95-reviewed-native.log),
make test passed201 CTest/131 Python (artifacts/item95-reviewed-test.log).
Initial treeaa704c9e639d775959b1bb2d79004b87c9c5848f passed200/130 and native
build before stronger routing tests/current committed battle-icon integration.
Independent source review found no finite receive/insertion mismatch; exact
request order, BGM counts/placement, original windows, once-only grant,
action/button gates, terminal no-repeat, failed/no-ready admission, full storage
and highest-zero gap semantics are covered. Documentation1051 Markdown files/0
broken links and diff check passed. No new GUI/emulator/hardware claim: these
new providers are portable mechanics, with native compilation verified.

Next bind original window-manager/action/text expansion, M_I_U/icon/effect6
and original audio/registry/control services, then actual Goomba3 acquisition
and later reward/Trouble Center acceptance. Continue original actor/object
coverage through shared catalogs/portable services. Original ISO remains
read-only; user emulator profile/saves unchanged. Full remake goal remains
active/unfinished; ITEM-01/OW-INTERACT-01 remain open.

## 2026-10-13 — Original equipment and partner weapon-menu art

Feature `2d25ab0` adds original boot/Hammer/Headbonk sprites to the existing
native weapon rows. Jump/Hammer use complete source equipment icon leaves with
independent pouch queries and exact level2/3/default rules; partner Headbonk
retains unsigned weapon+4 icon344. Unknown legacy pouch data does not invent
an upgrade. Original US names, type0 frame, font and animated hand remain.
Whole-row touch regions and physical input/timing are unchanged. Private command
metadata PMTCMD2 adds the original icon field; old packs require conversion.
[Native scope](behavior/display/NATIVE_WEAPON_MENU_ICONS.md);
[asset record](ASSETS.md). Full rotating class-menu choreography remains open.

The read-only importer pins both complete icon-helper bodies and three complete
default weapon records. Original icon.bin/icon.tpl cells164..169/344 retain
pixels/dimensions/loops/delays, packed without resampling into the existing
512×512 RGBA8 atlas1MiB. No extra GPU texture bank, generated sprite or dialogue
change is introduced. Source/default versus upgraded icon selection is distinct
from implementing additional upgrade moves or the complete weapon-table builder.

Final shared `make test` passed203/203 CTest and131/131 Python
(`artifacts/weapon-icons-final-tests.log`), with
`TMPDIR="$PWD/artifacts/tutorial-admission-tmp" make test`. Tests cover original
source fingerprints, independent fresh pouch queries, signed-byte/default
boundaries, actual command/partner icons, absent-pouch admission, original pixels
and nonoverlapping atlas rectangles. Initial test compilation found a reused
fixture variable name; it was corrected before this passing run. Normal full
`make all` passed (`artifacts/weapon-icons-all.log`); final native layout rebuild
with `-o assets-ready` passed (`artifacts/weapon-icons-all-final.log`). Debug
build/final layout rebuild passed (`artifacts/weapon-icons-debug.log` and
`artifacts/weapon-icons-debug-final.log`). Final doc links/diff checks pass.

Immutable private image:
`artifacts/weapon-icons-native-snapshot/ttyd-weapon-icons-final.3dsx`.
Isolated Azahar physical-input captures in `artifacts/azahar-weapon-icons-final/`
show normal boot/Hammer rows at after-A-54 and ordinary target entry at after-A-55.
Explicit development v8 pouch fixtures also exercise level3 boots/level2 Hammer;
they are not original card saves or claimed story/item progression. Fixed-count
upgrade capture names did not reliably identify menu states, so a follow-up in
`artifacts/azahar-weapon-icons-navigation/` detects actual visible menu admission.
Physical Right/A/B selects Hammer, opens its front-Goomba target and restores
Hammer selection with original upgraded art visible. One intermediate capture
has blank row text/art while its hand is visible; final return capture is stable,
so no continuous-frame/render/hardware-parity claim is made. Earlier44-press
capture stayed in original intro and is not menu evidence. Repeated later attack
presses reach existing pending-service notices; this is not a completed battle.

Next: install complete original startup/UnitEntry resources and tutorial
command/action ownership; connect source damage/effect/audience/turn services.
Complete rotating action-class menus and full weapon builder/eligibility, then
lower-screen scene/audience with optional hostile-item taps and physical controls.
Keep original dialogue IDs/tokens and simulation-tick timing; do not turn a
pending service into a fabricated success. Full goal completion remains open.

## 2026-10-13 — Original NPC map-side movement and plaza Goomba3 wander

Bound portable source npcHitCheckSide finite null-filter/no-reflection body:
ordered min(.2height,10)/.4height/.75height rays, extentL+.75width, middle45degree
gate, exact fused half slide and ignored hit distance. npcMain publishes first
contact before a second tentative-delta query; preserves that first contact
through second miss/hit/zero. Native source culled map/object cache supplies
queries without the optional Mario wall-attribute filter. No per-query texture
work or heap allocation is added by this portable body.

Enabled original Goomba3 shared radius100/speed30 wander via area metadata,
80603->00480603, source pose/sound names and existing talk pause/resume. Added
no direct-horizontal bypass, floor snap, gravity or Y/contact300 write: recovered
source flags establish zero-vertical npcMoveG. Actual movement after map/Mario
responses owns idle/walk/run pose. Original model/texture/vertex batching reused.
Catalog JSON now includes additional plaza tagged-setup extraction provenance.

Final source tree59eb97ba321f4ee1fd3ebcc3e62eeb60cdaae6fc verified2253 tracked
files byte-for-byte. make test passed199 CTest/130 Python and make all passed
(artifacts/goomba3-wander-reviewed-test.log and goomba3-wander-reviewed-native.log).
Tests cover zero/no-hit, all heights, middle boundary/rejected contact, source
half slide, two-pass first-contact publication/retention, ignored hit distance
and missing/unbound domains. Real original-plaza Game test observes movement,
map contacts, W_1, exact flags and no Y writes over2000 simulation ticks, then
checks unchanged original ordinary/waiting dialogue and event return. Independent
source review identified publication and normal-product ordering improvements;
both corrected before final validation. Earlier pre-review builds/test runs
passed, but final source evidence refers to the reviewed tree above.

Preserved native artifacts/ttyd-goomba3-wander-reviewed.3dsx SHA256
 d00107026713bb60fafc8131fc63fc76aba6092fd9d5a4f0549946997f2b0810.
Isolated artifacts/azahar-goomba3-wander observation passed (goomba3-wander-smoke.log).
Reviewed original NPC position/facing/activity and renewed A-talk candidacy in
0/4/9second captures, sampled60FPS/~10.2–10.6ms emulator frames. These are harness
sampling times, not source timing measurements; map-contact semantics are proven
by source/portable/original-geometry tests, not inferred from screenshots.
Native artifacts/azahar-goomba3-wander-talk also passed the original
quest2 waiting-dialogue route after enabling wandering; attributed bubble/pages
reviewed, onset55FPS/~11.2ms and later60FPS/~10ms. Fixed repeated A presses
can reopen the event; final capture name does not establish native return.
The40-byte fixture stayed byte-identical.

New item95 source recovery pins mode16 separately from mainflags, the actual
deferred pouch grant, both original windows, acknowledgment and control/entry
cleanup. Corrected prior mode-vs-flag terminology in source contracts. This
is documentation delivery, not an implemented acquisition or manufactured grant.
Documentation1046 Markdown files/0 broken links and scoped diff check passed.

Limits: standard-library sin/cos/norm adapters and converted geometric backend
remain short of full original SDK/Gekko/polygon-registry parity. No matched
Dolphin raster, global scheduler/RNG/audio-owner or physical New2DS assertion.
Native admission still excludes vertical/attribute/reflection domains requiring
other providers. Original item95 get-item service recovery continues; next bind
its actual deferred grant/windows/control, coin acknowledgment and full switches,
then Trouble Center acceptance/cancel menus and additional source NPC/object
profiles. User emulator profile/saves and read-only ISO untouched. Goal remains
active and unfinished; OW-INTERACT-01/ITEM-01 remain open.

## 2026-10-13 — Goomba3 native rendering and original talk captures

Final immutable602d1d81 native binary passed isolated Azahar runs
artifacts/azahar-goomba3-waiting4 and artifacts/azahar-goomba3-ordinary
(goomba3-waiting4-smoke.log/goomba3-ordinary-smoke.log). Reviewed original
plaza Goomba3 drawing, attributed irai_02_01 waiting bubble/pages and unchanged
gor_01_028_00 ordinary bubble/pages. Explicit40-byte fixture checkpoints stayed
byte-identical after each run. Sampled field/late-talk60FPS/~10–13ms, onset54–55FPS;
these are emulator observations, not physical hardware or matched original raster.
Both use normal frontend load and physical camera-relative input; the successful
approach is Right0.04seconds from the fixture, a harness input, not measured game
behavior. The three earlier attempts missed proximity/facing and do not establish
dialogue success. Fixed-count confirm presses can reopen talk; final capture names
alone do not prove native return. Host/Game tests prove event return and unchanged
story/flags; native captures prove the original drawing and print branches.

Documentation1043 Markdown files/0 broken links and git diff check passed.
User profile/saves and read-only ISO unchanged. Source Goomba3 map-side movement
recovery is in progress with the documentation agent. Next bind that original
wander domain, full get-item acquisition and coin/window/working-switch services,
then actual Trouble Center menus. Broader remake goal remains active/unfinished.

## 2026-10-13 — Shared original stage-floor and tutorial jump recovery

Feature `2849043` binds the complete US floor lookup and its three-operand
TransStageFloorPosition event for tutorial and ordinary trio combat. Signed X/Z
conversion, raw Y output capture, global stage ownership reload, mutable packed
threshold/floor fields, NaN comparison and truncation follow original source.
Fresh provision copies the existing original DOL90.0/-50.0 constants into the
two floor fields. Tutorial JumpContinue now has an actual source floor provider;
regular counter recovery uses the shared wrapper. No source dialogue, attack
selection, damage result or turn transition is fabricated. See the
[native floor/recovery contract](behavior/battle/NATIVE_STAGE_FLOOR_AND_JUMP_RECOVERY.md)
and [asset record](ASSETS.md).

Final shared `make test` passed201/201 CTest and130/130 Python, invoked with
`TMPDIR="$PWD/artifacts/tutorial-admission-tmp" make test`; exact output is
`artifacts/stage-floor-complete-tests.log`. Tests cover owner changes between
wrapper capture and lookup, never resolving input Y, output-token ordering,
mutable fields, both tiers, strict landing equality, NaN and negative fractional
truncation. Existing generic-jump tests now use the real floor provider. Original
tutorial fixtures verify both source initialization words. The max-moves fixture
was updated to supply the original constants required by provision; its initial
missing-data failure is retained in `artifacts/stage-floor-tests.log`. A later
full run failed the concurrently added Goomba3 test's old gor_01 collision path;
the updated roomAsset-based test and jump test passed focused retry, then the
final full201/130 run passed. No failing attempt is counted as final validation.

Normal `make all` passed (`artifacts/stage-floor-all.log`). Debug build initially
hit an undefined stageMap symbol during concurrent object updates; the current
object was inspected and the link retry passed
(`artifacts/stage-floor-debug-retry.log`). Its immutable private3DSX snapshot is
`artifacts/stage-floor-native-snapshot/ttyd-stage-floor-debug.3dsx`. Isolated Azahar
physical Left/Up/Right/A routing in `artifacts/azahar-stage-floor-tutorial/`
again reaches the original harbor scene and tutorial stage/actors/HUD, stopping
at the unchanged action-service boundary. This is entry regression evidence,
not a live attack/recovery, full battle or hardware-parity claim. Source verifier
pins four complete DOL bodies and two constant spans read-only. No asset payload
expansion was needed. Final doc links and scoped diff checks pass.

Next: install complete original startup/UnitEntry resource ownership and the
player command/action services, then connect generic damage orchestration and
turn scheduling. Keep source waits and original dialogue IDs/tokens intact.
Complete original rotating menus and lower-screen scene/audience with optional
hostile-item taps and a physical control path. Full mutable stage initialization
and later floor writers remain separate owners; only its two floor writes are
installed here. Full tutorial and real battle completion remain open.

## 2026-10-13 — Original plaza Goomba3 and task working callbacks

Added original plaza Goomba3 through the per-area catalog: tagged XYZ(-400,0,
-325),yaw90, shared c_kuribo_n art/tribe dimensions/S_1, flags80603 and source
registry order. Explicit nonquest2 state binds unchanged ordinary US dialogue;
known quest2/5406false/5405true binds original irai_02_01, speakerme. Unknown
state and incomplete acquisition/reward remain unavailable, with no invented
fallback or quest writes. Availability scans allocate no message strings.

Added portable irai_start_func/irai_cancel_func bodies with provider-owned
switch/pouch storage: clear5405..5421 then bytes1469..1474, recovered quest2/13
item tables, count/remove/recount until all copies removed, low-level removal
without parameter revision, ignored removal result and original return2.
Unbound tables/missing providers fail native admission before source writes.
Actual acceptance/menu/cancel caller and full switch persistence remain open.

Documentation recovery establishes actual Goomba3 ordinary/quest branches,
shared wander radius100/speed30 and the missing side/gravity dependency. The
native initial-pose presentation is provisional; this NPC lacks east Goomba's
direct-horizontal flag, so its regular motion is not yet bound. Item acquisition
must run itemseq_GetItem; direct pouch insertion would skip original services.
Original coin acknowledgment strict waits/A/input/return ordering and signed16
truncate-before-clamp leaf are documented for the next reward binding.

Validation: final scoped tree0beb872ec770a3625d525fe42dc1d0a5119ba80b verified
byte-for-byte across2241 tracked files. make test passed197 CTest/129 Python
(artifacts/goomba3-final-test.log). Real catalog/Game tests cover proximity,
ordinary/waiting print attribution, original message bytes, event return and
unchanged story/quest/flags. Callback tests cover exact reset ordering, items
behind gaps, multiple copies, other-item/resource preservation, ignored remove
result, empty inventory and pre-write failed admission. Initial test failed
because the test requested gor_01 collision filenames rather than the native
roomAsset(0)=map alias; corrected to the shared room provider and rerun passed.
make all passed (artifacts/goomba3-native.log). Preserved native binary
artifacts/ttyd-goomba3-reviewed.3dsx SHA256
602d1d8196629642a9787291af625d855cf40003849a549bead03aff8b5b668f.
Independent read-only source review found no guard/table/reset mismatch.
Native visual evidence is recorded in the follow-up below.

Original tagged map extraction/catalog outputs remain ignored, assets and
source provenance recorded in docs/ASSETS.md. OW-INTERACT-01/ITEM-01 remain open.
Next bind original NPC side/gravity+shared wander, source95 get-item presentation,
coin/window services, full switches and Trouble Center acceptance/cancel menus;
continue other source actors/objects via existing area catalog/portable events.
Matched Dolphin raster/material/audio parity and physical New2DS performance
remain unverified. Full remake goal remains active and unfinished.

## 2026-10-13 - Final quest13 integrated pose capture

The corrected HUD-confirmed isolated route passed in artifacts/azahar-goomba13-residency-poses2 (quest13-residency-poses2-smoke.log). Reviewed both original M_N_5B/M_N_C frames in the final b32f82e0 binary, with no generic fallback bubble;60FPS/~8.85ms emulator frames. Original duplicate selector/scroll/cancel/continuing route also completed. This replaces the two mistimed final pose attempts as visual evidence. Source dialogue/quest fixtures remain explicit harness input; no hardware/raster or task-acceptance parity claim. Documentation1039 files/0 broken links and diff check passed. Goal remains active; next bind source Trouble Center/task acquisition.

## 2026-10-13 — Original Goomba quest13, pouch checkpoints and GPU residency

Bound original Goomba4 quest13 first/continuing event through the extensible
field NPC owner. Retained source US message IDs/tokens, numeric speaker0,
M_N_5B/500ms/M_N_C/500ms/M_S_1 sequence, one duplicate107/-1 table allocation,
ready-only selector deletion, source removal/revision ordering and first-exit
5407 write. Known-state source arbitration/control remains authoritative.
Original effect poses use a separate21-cell1024x512 RGBA8 atlas (2MiB), not
placeholders. Fixed the generic harness bubble appearing during authored waits.

Native v8 checkpoints now preserve the unchanged0x5d4 packed pouch and explicit
5407 knowledge in1536 bytes with a full checksum. Legacy32/40-byte saves retain
unknown state; no source flags or pouch manufactured. Deep snapshots keep save
workers and imported file slots independent of mutable gameplay pouch state.
This is a local adapter, not the source card driver/full switch banks.

Retired unused battle maps at field entry, then field map/NPC textures before
battle allocation; field return reloads its original room. Area changes retire
old graphics before staging and restore them on failure. GPU retirement occurs
before frame submission with Citro3D synchronization. Admission ordering also
retires field resources before scene actor texture requests. Rendering reuses
native texture/linear vertex resources, with no per-frame conversion.

Validation: checkpoint tree b195314cc0157b28ca7812c9f53c21c93b787733 passed
make all and190 CTest/124 Python tests (quest13-save-isolated logs). Quest13
tree25379f81d22bdb31a82cb31194705396377597ca passed make all and194 CTest/127
Python tests (quest13-reviewed logs). Isolated Azahar original-pose,12-row
selector/scroll/cancel/continuing005 captures reviewed in
artifacts/azahar-goomba13-reviewed; preserved binary
artifacts/ttyd-goomba13-reviewed.3dsx SHA256
4f04664a5fd379fd048601993df8dc59ec473f6fb1d3ba37ad1fa35a59fadb3a.
Observed59–60FPS/~9–13ms emulator performance; no hardware parity claim.

Resource-provider probe artifacts/azahar-goomba13-residency-probe3 succeeded:
linear baseline985728, legacy admission1/free9471104, return1/free985728;
trio admission1/free9724672, return1/free985728. Final original field/NPC frame
reviewed. Before field retirement both admissions failed. Probe substitutes
scene ownership only for map allocation; it does not test battle logic/raster.
First two frontend probe attempts did not reach the saved field and establish
no admission result. Initial parallel private build collision was rerun cleanly;
no failed build is counted as validation. Final tree45fc1488ce93897a618012fd658a2be4aa059ce9 (2234 tracked files
verified byte-for-byte) passed make all and195 CTest/129 Python tests
(artifacts/quest13-residency-native.log, quest13-residency-final-test.log).
Preserved normal binary artifacts/ttyd-goomba13-residency.3dsx SHA256
b32f82e0c5395fee1f4affbbac3bb1e2cb76f75b609ae7b38f1e0f0aa4992ead.
Final selector/scroll/continuing capture run succeeded; early pose frames in
that run preceded field entry and are not pose validation. A first retake
misidentified the title as field; its early pose images also establish no
pose admission result. Reviewed earlier quest13 pose frames remain valid.

Recovered original Trouble Center acceptance/cancel reset tables, Goomba3
quest2 item-entry/reward writers, and Glitzville quest13 acquisition/completion
in the subsystem contract. OW-INTERACT-01 and ITEM-01 remain open.

Next bind these upstream acceptance/item-entry/GetItem services and actual
quest acquisition, expand original NPC/event coverage through the same area
catalog/portable owners, and finish battle field restoration/ordered engine
services. Matched Dolphin raster, original material/lighting/audio/window/RNG
parity and physical New2DS performance remain unverified. User saves/profile
and read-only ISO untouched. Documentation link check1039 Markdown files/0 broken links and scoped diff
check passed. Full remake goal remains active and unfinished.

## 2026-10-13 — Native harbor admission and original combat GPU residency

Feature `b36c849` fixes the native harbor admission blocker. A valid story3
checkpoint passed host decode/collision/ground/walkability checks, but debug
restore reported `Map texture import failed: romfs:/gor_00-32.t3x`. Renderer
initialization no longer eagerly imports the quest13 Mario and trio Mario/
Goombella GPU atlases. Real active consumers acquire them; inactive consumers
and room entry retire them after GPU synchronization. Animation/UV metadata,
original pixels, filtering and source story remain unchanged. The three existing
RGBA8 banks total3.5MiB of texture pixels; this is a size calculation, not measured
hardware free memory. Debug restore failures now remain visible as harness
notices instead of silently appearing to admit a fixture. See the
[lifetime contract](behavior/display/NATIVE_SCENE_ACTOR_GPU_TEXTURE_LIFETIME.md)
and [asset record](ASSETS.md).

Native validation uses isolated Azahar profiles and physical inputs only.
`artifacts/azahar-tutorial-admission-diagnostic2/` identifies the failed import.
`artifacts/azahar-tutorial-admission-lazy/` loads the original harbor message;
`artifacts/azahar-tutorial-admission-route/` follows Left/Up/Right around the crates
and reaches setup2. The reviewed final private image
`artifacts/tutorial-admission-snapshot/ttyd-admission-final-packed.3dsx` repeats
that route in `artifacts/azahar-tutorial-admission-final-packed/`, showing original
stage, Crump/Mario/helper actors and HUD. It reaches the explicit original
tutorial action-service boundary. The earlier lazy build also loads both original
trio combat atlases and physical menu/target inputs in
`artifacts/azahar-tutorial-admission-trio/`; capture names do not guarantee the
active actor/weapon after repeated intro inputs.

Validation details: shared `make all` passed
(`artifacts/tutorial-admission-final-all.log`). Final scoped debug and normal
`make all` builds passed from `/tmp/tutorial-admission-review/`, using the
existing private asset pack with `-o assets-ready`; logs are
`artifacts/tutorial-admission-review-debug.log` and
`artifacts/tutorial-admission-review-all.log`. ROMFS was repacked directly from
the project private directory with `3dsxtool`; the first review package was only
86MiB and rendered black, and is not success evidence. Final packed images are
430MiB. Final reviewed `make test` passed195/195 CTest and129/129 Python
(`artifacts/tutorial-admission-review-final-tests.log`), invoked as
`TMPDIR="$PWD/artifacts/tutorial-admission-tmp" make -C /tmp/tutorial-admission-review test`.
Doc links:1038 Markdown files/zero broken; scoped diff checks pass.

The reviewed tree contains HEAD27779ad plus this feature, preserving unrelated
working and staged changes. Shared `make test` initially could not configure
because an in-progress stage-map test file was absent; a mixed-source fallback
later failed the trio original assertion. An initial reviewed host attempt ran
out of `/tmp` space, and a later Python run lacked two fixture directories.
Build outputs/compiler temporaries now use project artifacts and the review
has the original private fixture paths. These failed attempts are distinguished
from the final reviewed validation, not attributed to this feature.

Next: bind the actual tutorial action services and connect source damage/turn
scheduling; preserve original dialogue IDs/tokens and tick timing. Complete the
original rotating command menu. Render the lower-screen scene/audience and add
optional hostile-item taps with a physical control path. Current lower screen
is not that audience implementation; tutorial/full battles and hardware parity
remain incomplete. Never substitute a fabricated result for a source wait.

## 2026-10-13 — Native original tutorial registry and init readiness

Featuref85edaf replaces the projection-only Crump0/Mario1/helper2 ownership with
original system0/Mario1/Crump2/helper3 while preserving three visible original
actors. The actual invisible system definition/kind/part/init are imported:
type220, HP999/FP0, null bank/table and original seven-command init. Its real
wait/attack/damage hooks, max/current move0 and wait root execute in the same VM.
System/Mario/Crump init creation order, helper3 in Crump work0, actual request/
phase/attack/speaker IDs and rendering/animation lookup are covered. No fake
system model, pose handle or source dialogue is introduced.
[Registry/visual contract](behavior/battle/HARBOR_NATIVE_REGISTRY_AND_VISUAL_PROJECTION.md).

The bounded importer adds exactly28 original DOL bytes for three8-byte system
hooks and the actual final label-scanner lookahead100. Five source spans and
identity/resource/part/init/hook bindings are pinned read-only; raw scripts and
compiled packs remain private/ignored. See [asset record](ASSETS.md).
The original init-readiness helper now binds actual VM event existence, clears
finished unit288 handles across all64 slots and retains live roots. A blocked
Mario init stays blocked while completed system cleanup continues. The old
VM allocationCount==0 bridge condition is retired; full original startup/
UnitEntry/UnitSetup/resource/status/audience/turn integration remains open.

Validation: normal final `make all` passed (artifacts/tutorial-init-gate-all.log).
The same make-test recipe in an isolated host build directory passed195/195
CTest and127/127 Python (artifacts/tutorial-init-gate-tests.log), invoked with
`make -f /tmp/tutorial-registry-validation.mk test`; that temporary Makefile
changes only the three host build-directory paths. Earlier shared `make test`
runs reached191/192 checks but had transient permission-denied/not-run executables
while shared targets were being relinked; the initial three failed launches
passed their focused retry. First isolated build caught a concurrently changed
Goomba begin signature/Quest13 linkage; reconfigure/rebuild then passed193/193
CTest+127 Python before the final init-gate change. Final195/127 passing result
is distinguished from those failed attempts. Focused wrong-ID animation ACK
checks prove system0/Crump2/helper3 cannot release Mario1's live wait.
Read-only verifier output: artifacts/tutorial-registry-source-pins.json.
Doc links checked1027 Markdown files/zero broken at the registry stage; final
link/diff/py_compile checks pass. No original-runtime/hardware parity claim.

Native debug build and coherent private-pack rebuild passed. Immutable snapshots
in artifacts/tutorial-registry-native-snapshot/ were tested in isolated Azahar
profiles only. All three current attempts end in the plaza rather than admitting
the requested harbor checkpoint; screenshot names mentioning tutorial are not
proof of entering it. The coherent snapshot SHA256 is
91c28b0ab0c896f4506fa21b4d9d3a156c743f80270932953d12fc04f9a8b11f.
The documented(-1.5,-1.1) checkpoint independently loads as room1/story3 and
passes host ground/walkability validation (private /tmp/check_harbor_fixture).
The native load/transaction/resource admission boundary still needs diagnosis;
no tutorial GPU/speaker/animation-completion observation is claimed from these
plaza captures. Latest captures: artifacts/azahar-tutorial-registry-coherent/.
User emulator profiles/saves and the original ISO remain untouched.

Next: diagnose actual native checkpoint admission with a narrowly scoped isolated
probe, then obtain actual setup2 original actor/bubble/ACK captures. Bind complete
source UnitEntry/UnitSetup and startup services, original player commands/turns,
damage/status/effect/audience/hit-event providers and result/field continuation.
Complete original rotating menus/HUD cadence and lower-scene/audience rendering
with optional bad-item taps plus physical alternatives. Tutorial combat and the
full battle objective remain unfinished and active; no full-game claim is made.

## 2026-10-13 — Native original Goomba4 quest2 hand-in

Featureb724535 binds the original gor_02 Goomba4 quest2 arm to existing NPC
arbitration, original US message playback, original item-selector rendering and
live original pouch storage. Admission preserves known quest1420=2,
GSF5405=1/count95>=1 and nearest-candidate priority. Source `irai_02_03` precedes
the retained `[95,-1]` selector; cancel returns unchanged. Every non-1 result
prints `irai_02_04`, removes95 through the original revision wrapper, then
writes5406=1 even on original removal failure. Story/quest/5405 stay unchanged.
Talk/selector depths1/2/1/0 retain the outer control lock across windows. Native
unsupported resources/pouch domains stay pending rather than fabricating a
source completion. [Native scope](behavior/field/NATIVE_GOOMBA4_QUEST2_HAND_IN.md)
and [source control/poses](behavior/field/GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md).

Original Mario and party pouch recalculation now scans all200 equipment slots,
preserves signed halfword writes/clamps and signed BP costs, and revises all
seven partner records without healing. The416-byte local pack copies the pinned
340 source BP bytes and32 partner HP halfwords from the read-only US DOL; only
halfword byte order changes. It loads once at renderer initialization; revision
has no allocation. [Source contract](behavior/unit/POUCH_PARAMETER_REVISION.md).
No ISO/extracted payload is committed or redistributed.

Validation: isolated source tree76b77737c2c35b6f0867ca97bfaae4197358310f,
`make test` passed189/189 CTest and124/124 Python
(artifacts/goomba-hand-in-isolated-test.log); subsequent `make all` passed
(artifacts/goomba-hand-in-isolated-native.log). Focused original-data tests
passed for admission knowledge, message order/tokens/Goomba attribution,
ready-only selector deletion, cancel/accept/ignored removal failure, damaged
HP/FP preservation, final equipment slot, duplicate badges, all partner/rank3
records, halfword/BP wrap and unknown-domain refusal. Native Game checkpoint
placement uses the live wandering NPC root and actual proximity/arbitration/
event entry; cancel releases control with item95 and story untouched. Fixture
placement and tick counts are harness inputs, not original observations.

Isolated Azahar boot of that binary passed after35 seconds; inspected
artifacts/azahar-goomba-hand-in-boot/boot.png shows the original opening book
scene and live message at emulator60 FPS. The first8-second capture was still
black during initialization; it was not accepted as a successful visual boot.
Binary SHA256c15113d02cea0ae29b16e62e7db83702c96bc254114a8c6d7c0b15c2972ad216.
This is startup evidence, not a native quest scene or hardware performance
measurement. User emulator profiles/saves were untouched.

The shared preliminary `make test` ran against concurrent unfinished edits and
failed opening_sequence_original, harbor_encounter_original and
harbor_tutorial_original; the scoped isolated run above is the passing evidence.
The shared native build also failed its link rule without a linker diagnostic;
the isolated native build passed. Unrelated working/staged changes were
preserved. No hardware performance or Dolphin parity is claimed.

Remaining: bind quest13 only with explicit known/value GSF5407 persistence and
legacy-unknown migration, add original e_mario M_N_5B/M_N_C poses, then execute
its retained duplicate107 table/update/free graph and original500ms waits.
Persist the complete original pouch/switch banks before claiming faithful
hand-in save round-trips. Task acceptance, other quest stages, general event
ownership, status/SFX/global-manager/RNG and matched original/3DS rendering
remain open. Continue source NPC/object/event expansion through the existing
area catalog and per-subsystem service owners. The full-game goal stays active.


## 2026-10-13 — Original damage-controller composition and physical harbor route

Featuref39c355 implements the complete local BattleCheckDamage800fb01c and
BattleCheckPikkyoro800fde2c orchestration over packed source state and explicit
service providers. It preserves independent guard reads, mode-specific
superguard responses, shared HP/FP code, status/reaction priority, hit-count and
sequence publication, retained RNG, audience classifications and full counter
tail call ordering. Counter contributions retain separate wrapped50/100 signed
divisions, negative amounts and zero-only fallback; AC stop retains the battle
captured before callbacks. No PreCheckDamage is inserted, and no effect handle
or callback success is fabricated. The old preliminary BATTLE_CHECK_DAMAGE
page now points to the accurate entry/body contract.
[Portable owner and limits](behavior/battle/NATIVE_DAMAGE_ORCHESTRATION_OWNER.md).

The failing harbor walking fixture was colliding with original A_ni_03 and
pCube101_q00025 geometry, not proving a failed encounter gate. Private probes
retain those walls and find a route through the gap ahead of pCube94_q00019.
The regression now uses actual physical Circle Pad snapshots to walk left,
then through that gap, then right into the unchanged source trigger. Original
three field messages, HP floor, setup2 and first tutorial message follow without
touch, teleport, collision bypass or forced story/result writes. Removed only
the temporary fixture diagnostic print. Direct positive/negative non-Q1024
encounter regressions from the prior jump-admission work are retained.
[Follow-up route scope](behavior/story/NATIVE_HARBOR_ENCOUNTER_EVENT_FLOAT_ADMISSION.md).
Fixture steering counts are not measured original timing constants.

Validation: final `make test` passed186/186 CTest and122/122 Python
(artifacts/damage-orchestration-tests-reviewed.log). Final `make all` passed
(artifacts/damage-orchestration-all-final.log). Focused controller/physical-route
checks2/2 passed (artifacts/damage-orchestration-route-focused.log). The read-only
verifier pins both full source bodies (640/106 instructions), five original
effect constants and13 literal capture/guard/multiply/divide/stop/help anchors
(artifacts/damage-orchestration-source-pins.json); it does not execute original
code. Tests cover superguard bytes0..5, flag short-circuiting, every five-gate
selection subset/rank, odd/negative/wrapped counter amounts, FP code overwrite,
status priority, source status arguments, retained RNG and publication/capture
ordering. Doc links checked1017 files/zero broken; diff check passed.
No new Azahar/Dolphin/hardware capture or performance parity is claimed. ISO
access stayed read-only; user emulator profiles/saves and private source payloads
were untouched by this increment.

Remaining: the new complete portable damage controller is not yet installed in
HarborTutorial or TrioAttack; their existing admission/frontiers remain honest.
Bind actual calculator/application/status/revival/effect/audience/hit-event and
AC/help providers before replacing scoped native paths. Then install original
system/Mario/Crump/helper registry/entry ownership and original startup/player-
command/turn/result/field continuation. Preserve original rotating menus and
complete HUD/window cadence; lower-scene continuation and optional audience
bad-item tapping still need original actors/throw gates and physical controls.
The full battle/tutorial objective stays active; neither native tutorial combat
nor the full-game remake is reported complete.

## 2026-10-13 — Native original item-selector font, icons and controls

Feature0a08d95 adds source US plain-window-font geometry/measurement/outline
consumers and a native renderer for the retained explicit-table selector.
Original list/header/help frames, source text/metadata, centered item icons,
mirrored bottom-anchored cursor, scroll arrows, clipping and post-draw header
mutation now have native services. Glyphs batch through preallocated citro3d
slices; no font draw allocates memory. Whole clipped glyphs omit native vertices
while preserving all source glyph/RNG requests, supporting121 duplicate key
rows without consuming arena space for invisible rows. Read
[native rendering](behavior/message/NATIVE_ITEM_SELECTOR_RENDERING.md).

Documentation/check delegation caught earlier source misattribution:16/20 are
draw flags rather than cameras, and rows use centered winIconSet rather than
general bottom anchoring. Source contracts/API corrected; cursorflags20 mirrorsX.
Opaque rows now use alpha>=128/blending disabled, with explicit native state
restoration and failure propagation. Immutable shared text resources survive
reload. New [revision contract](behavior/unit/POUCH_PARAMETER_REVISION.md) closes
Mario/party writes required after quest-item removal; native quest binding is
still next. Help masks100/200 are source C-stickY direction bits, not A/B or
horizontal axes; corrected wording does not change helper arithmetic.

Validation: isolated treee5edc8377e39cbbe98503dbfed730b831bb21065 passed make all
(/tmp/window-font-isolated-final3-native.log) and, after separating its icon PNG
cache, make test:183 CTest/118 Python (/tmp/window-font-isolated-verified-test.log).
Icon bin/t3x/json regenerated byte-identically; initial Python cache failure
was shared PNG repacking against older isolated metadata. Shared preliminary
test failed harbor_encounter_original; no passing claim for that run. Shared
postcommit make all passed (/tmp/window-font-postcommit-native.log), including
concurrent battle-HUD/frame-on-bottom changes. An isolated staging declaration
placement error was corrected; committed renderer prototype covers its existing
message-indicator body. Whitespace checks and link audit passed1017 Markdown
files/zero broken links. The isolated tree and shared workspace are different
validation scopes; no whole-workspace green-test claim is made.

Private native preview builds show original95 single-row and twelve duplicate107
rows. artifacts/azahar-original-item-selector-stick contains directly inspected
one-row, scrolled row7, trigger-wrap and closed-owner captures; original source
art/text, clipping, cursor mirroring and arrows are visible. App60 FPS/about
10..14ms emulator frame time is observed, not New2DS hardware performance.
Preview SHA256db539765d2b578e3912b8c693a49e16193decd73811ce5a0192e0d3effc8c320;
source-only diagnostic main restored afterward. Initial D-pad script did not
navigate because this selector reads main-stick direction, then corrected
CirclePad/Down input verified scrolling/wrap. Preview bypasses quest entry and
inventory/story writes; it does not prove a quest event runs. All emulator
configuration/save fixtures/captures/packs remain ignored and user profiles/
saves untouched. No matched Dolphin raster or hardware capture was obtained.

Next: bind original Goomba4 quest2/13 event graphs and key ownership, with real
pouch removal plus complete parameter revision and saved global switches.
Preserve unknown legacy state. Bind status-window/SFX macros and global manager/
GP/RNG/camera lifetimes; add tagged font consumers, gray/additional-row branches
and native C-stick help input. Compare source/Dolphin/New2DS raster and profile
hardware before claiming correctness/performance closed. Full-game goal remains
active; renderer components and development previews are not finished quests.

## 2026-10-13 — Original battle HUD art and physical-control submenu binding

Feature3c4bf18 imports verified original US status-window art and both numeral
sets through read-only DOL/icon.bin/icon.tpl sources. Rectangle packing keeps
unchanged pixels/frame dimensions/order/delays in the existing1MiB atlas.
The live tutorial/trio HUD reads current packed Mario HP/FP instead of waiting
for field export; the tutorial's explicit statusVisible gate is retained.
Trio lower-screen lists use original type0 frames, US glyphs and selection hand.
Shared visible-row/tap geometry preserves D-pad/Circle Pad selection, A confirm
and B cancellation; no required menu action is touch-only. The duplicate top
placeholder target box was removed after capture showed it overlapping the HUD.
[HUD/menu scope](behavior/display/NATIVE_BATTLE_HUD_AND_SUBMENU.md),
[asset provenance](ASSETS.md).

Also corrected harbor stationary-jump admission to compare resolved X/Z to the
actual root's exact source Q1024 event-float round-trip. Non-Q1024 field roots
no longer stall this bounded adapter merely because the original preceding
get-position encoded them. General travel/height/30Hz jump behavior remains
unbound; no source dialogue/story/branch was rewritten.
[Encounter scope](behavior/story/NATIVE_HARBOR_ENCOUNTER_EVENT_FLOAT_ADMISSION.md).

Validation: final normal `make all` passed
(artifacts/battle-ui-all-reviewed.log). Debug-short build and final private-pack
rebuild passed (artifacts/battle-ui-debug-pack-final.log). `make test` reached
183/184 passing CTest checks; harbor_encounter_original still fails its further
field-walking fixture before entering the encounter trigger (line58), after
its direct non-Q1024 encounter assertions pass. This failure is unresolved,
not counted as a passing full suite (artifacts/battle-ui-test-final.log).
Separately all120 Python tests passed (artifacts/battle-ui-python-final.log),
including original table pins, unchanged HUD pixels, atlas bounds/nonoverlap
and existing selector art checks. Final focused trio command/intro/tutorial
checks3/3 pass (artifacts/battle-ui-focused-tests-final.log). Doc links checked
1013 files/zero broken; scoped diff check passed.

Isolated Azahar uses immutable artifacts/battle-ui-snapshot/ttyd-2ds-debug-short.3dsx
with only the native underground story11 test save. Physical-input capture
completed successfully in artifacts/azahar-battle-original-ui-reviewed/;
reviewed commands.png actually shows Goombella's Headbonk menu, and the target
captures show original Goomba/Spiky Goomba/Paragoomba labels and visible cursor.
Old script capture names are not proof of Mario/Hammer ownership. These captures
predate removal of the duplicate top target box; final build validates that
simple presentation removal, without a second capture claim. The observed
Azahar status shows20FPS/100% speed at the reviewed command capture, not a
physical2DS performance or original-runtime parity result. Initial sandboxed
Xvfb socket admission failed; the authorized isolated-display rerun passed.
User emulator profiles/saves and the ISO remained untouched.

Next: resolve the further walked harbor fixture, bind original tutorial player
commands/turn/attack/result progression and complete field-return ownership.
Replace this weapon-submenu adapter with the full original rotating action menu;
add partner status/AP/coin/Star Point display and original window cadence.
Bind original audience actors, throw telegraphs/targeting and a physical stop-
throw action before optional lower-screen tapping. Lower scene continuation,
audience interactions and complete tutorial combat remain unfinished; the
user's battle/tutorial goal is active, and full-game completion is not claimed.

## 2026-10-13 — Filed MENU-01 and PROGRESS-01 (full-game path)

Filed two full-game requests on the G1 path, each grounded in native source, and
added both rows to the [inbox](DOCUMENTATION_REQUESTS.md) and the
[milestone request map](MILESTONES.md). [MENU-01](requests/MENU-01.md) — the
original field pause menu: native `Menu` is only Status/Settings/Items
(`source/runtime/game.cpp` handles those three; `README.md` maps Y to "save, item,
sound"), so it asks for the entry table/order, per-entry enable conditions and
the state each reads, navigation and pause ownership, input mapping, and whether
the menu saves versus the save block. [PROGRESS-01](requests/PROGRESS-01.md) —
character progression: native `struct Progress` has only `hp`/`mushrooms` and
`jumpLevelFixture`/`hammerLevelFixture` are unbound, so it asks for the
star-point producer, level-up curve and HP/FP/BP bonus selection, BP accounting
and badge equipping, partner rank, presentation order and save round-trip.
PROGRESS-01 is G1-only past D1 because the opening trio has a no-level-up tail.

Validation: doc-link audit 1011 files / 0 broken; `git diff --check` clean; docs
only, no make rerun. Limitation: commit `03e44c4` also carried a stray staged
`Makefile` hunk (a trailing blank-line removal the docs agent had staged) because
that commit wrote the whole index; the content is preserved and the docs agent's
worktree was not touched. Next: await the
DEMO-01/RESULT-01/GSF-01/TRACE-01/ITEM-01/PARTNER-FULL-01 answers (DEMO-01 and
GSF-01 already have bounded deliveries) and verify any candidate fix against the
ISO before applying it. Full-game incomplete.

## 2026-10-13 — ITEM-01 todo 1 slice: pouch acquisition dispatch

Delivered behavior/battle/POUCH_GET_ITEM_DISPATCH.md for requests/ITEM-01.md
(todo 1 core): verified type-id routing, slot bases +0xA0/+0x192 and 10/20
capacity against the snapshot layout, single shared pouch pointer, 1/0
returns with compaction. Tightened two loose routing claims pre-commit
(capped-counter identity, member-scan shape). Recorded the delivery in the
request file without touching request text. Todos 2–4 (effects, save,
acquisition sites) stay open.

Validation: structural re-checks, link audit, diff --check clean. Docs-only,
no make rerun. Full-game incomplete.

## 2026-10-13 — First-demo result Escape-domain override producer

RESULT-01 portion checked; [InterruptStop/Escape](behavior/battle/BATTLE_INTERRUPT_STOP_FLAGS_AND_ESCAPE_RESULT_ROUTE.md)
completes the raw-gated flag10/20 publisher and pins Escape's literal0 terminal
request. AC result mask2-set arm ends before it; mask-clear continuation retains
presentation/child waits. Fresh battle capture follows mandatory operand
resolution. This proves an Escape-domain route to4 subject to higher priorities,
not universal4/5 meanings or actual tutorial admission.

Validation:7 spans/80 anchors/one service; all30 callback instructions, scoped
Escape control/publication order,16 mathematical vectors and736 annotated DOL
streams/one call checked read-only. ISO pins pass; existing IF predicates
(22 bodies/416 anchors) and dispatcher tail (nine bodies/164 anchors) pass.
Link audit passed1005 Markdown files/zero broken links; scoped diff passed.
Docs only; no make test/all rerun. Raw ELF/listing/generator stay /tmp;
concurrent native changes excluded. Approved push still awaits valid GitHub
credentials; no retry after unchanged authentication failure.

Next: other override/timer/tutorial restrictions, full ending/reward/field/story/
save composition and original TRACE-01; DEMO-01 joins/reentry remain open.
RESULT-01/demo/full-game remain OPEN.

## 2026-10-13 — First-demo result ending admission gates

RESULT-01 scoped admission portion checked; [ending gates](behavior/battle/BATTLE_END_ADMISSION_GATES_AND_CONCLUSION_CALLERS.md)
connect two turn event barriers and first-act index3 to ordered ending banks.
First-act audience service precedes writes; zero-result continuation differs.
Exact table bindings and ten direct DOL conclusion callers distinguish query
from ending admission. Complete controllers/tutorial configuration stay open.

Validation:18 spans/215 anchors/17 scoped direct calls; literal instructions/
branches/owners/returns,93 table words/35 bindings,ten mathematical models and
exhaustive ten-site direct DOL census checked; read-only ISO pins pass. Link audit
passed1004 Markdown files/zero broken links; scoped diff passed. Docs only;
no make test/all rerun. Raw ELF/listing/generator remain /tmp; concurrent native
changes excluded. User explicitly approved all-commit push; git push origin
HEAD:master failed with GitHub Invalid username or token/authentication failed.
Push remains pending valid credentials, not additional authorization.

Next: tutorial config/status/death/override writers and complete ending/reward/
field/story/save composition; DEMO-01 joins/reentry and original TRACE-01.
RESULT-01/demo/full-game remain OPEN.

## 2026-10-13 — First-demo result conclusion group publisher

Completed BattleCheckConcluded; RESULT-01 portion checked. [Conclusion contract](behavior/battle/BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md)
strengthens existing evidence with ordered group publications, two64-slot scans,
fresh config bit2 and before/after-status attribute reads. Return count>=2 or
flags10/20 is distinct from result publication;40 alone does not force it.

Validation:3 spans/all96 words/four calls;20 literal vectors/group writes/fresh
config+attribute/return mask/call sites checked; ISO pins pass. Link audit
passed999 Markdown files/zero broken links; scoped diff passed. Docs only; no
make test/all rerun. Raw ELF/listing/generator stay /tmp. Concurrent native
edits excluded. Approved push pending valid GitHub credentials.

Next: actual tutorial configurations/status/death/override/end admission and
result/ending/reward/field/story/save composition plus DEMO-01 joins/reentry and
original TRACE-01. RESULT-01/demo/full-game remain OPEN.

## 2026-10-13 — First-demo inbox: battle result phase0 producer

Completed result set/get/enemy-belong leaves and scoped initial end-phase
publisher; RESULT-01 portion checked/DEMO-01 linked. [Result contract](behavior/battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
registers existing common producer with fresh proof: captured I, flag40/20,
own-loss before opponent-result1, fallback10/no-write and getter order. Field
nonzero wait, ending ready bit, story and save-unblock remain distinct owners.
No4/5 flee/timeout meaning or native outcome synthesized.

Validation:10 spans/91 anchors/12 scoped calls; independent15 leaf+71 prefix
words/16 vectors/masks/capture/five setter/get/table sites/exhaustive9-site DOL
census/fresh pins passed. Link audit passed996 Markdown files/zero broken
links; scoped diff passed. Docs only; no make test/all rerun; raw ELF/listings/
generator stay /tmp. Concurrent native edits excluded. Approved push pending
valid GitHub credentials.

Next: ending-bank admission/conclusion/death/override writers and reward/ready/
sequence/tutorial story/save-unblock composition; DEMO-01 cross-scene joins/
reentry and original TRACE-01 execution. RESULT-01/demo/full-game remain OPEN.

## 2026-10-13 — Retained original three-window item selector owner

Feature4cc7402 binds explicit-table template0 lifecycle in an area-independent
portable owner: original list/header/help rectangles, duplicate rows, per-slot
sequence/main/deferred-display registration order, A-before-B, close-before-ready,
index retention and ready-only deletion. Pure draw requests retain source
clipping/easing, distinct name/icon color services, outlined heading, low16
centering, header post-draw mutation, mutable font palette and exact help paging/
icons. Source refresh/interpolation/sound/status/font/input remain injected
services. See [native owner](behavior/message/NATIVE_ITEM_TABLE_SELECTION_OWNER.md)
and [draw requests](behavior/message/ITEM_TABLE_SELECTION_DRAW_REQUESTS.md).

Delegated source recovery now covers
[window-font consumers](behavior/message/WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md),
including outlined string versus message consumers and destructive embedded-icon
font resets. The six original US strings used by quest95/107 have no tags; both
descriptions have one LF, hence two help lines. Existing native glyph metrics
retain patched source WID1 widths; read-only US startup evidence sets original GP+16c to1, selecting
spacing2 and single-byte mapping. Broader mutable-font/config lifetime remains
a separate service. No dialogue rewrite or asset conversion was added.

Validation: isolated source snapshot audited byte-for-byte against1248919, then
applied only this owner/tests/contracts/CMake/index additions. Its make test
passed167 CTest/110 Python (/tmp/item-selection-runtime-isolated-test.log),
and make all passed (/tmp/item-selection-runtime-isolated-native.log). Shared
make all also passed after fixing ARM int32_t/std::max type deduction
(/tmp/item-selection-runtime-checked-native.log). Shared make test final rerun
failed linking field_wall_tests with undefined main
(/tmp/item-selection-runtime-checked-test.log); current source and object both
contain main. Shared build output is not counted as a final passing test;
isolated validation distinguishes this feature from concurrent source/build
changes. Independent two-pass read-only source review found no lifecycle or
scoped draw arithmetic mismatch after API clarification. Link audit passed994
Markdown files/zero broken links; diff check passed. No new native selector
capture or hardware benchmark: its display/event binding is still unbound.

Next: implement original plain window-font GPU drawing and deferred native
list/header/help rendering using the recovered consumers, source configuration/
spacing and existing original texture/icon packs. Bind manager-slot allocation,
physical repeat/GP cadence/status/audio and original event key ownership. Then
execute Goomba4 quest95/107 messages/branches with original pouch removal and
persistent switch writes; legacy unknown state must stay unknown. Full selector,
quest, hardware performance and full-game goals remain open.

## 2026-10-13 — First-demo inbox: notice/pipe flags and save mapping

Prioritized new DEMO-01/GSF-01 opening-demo requests. [Flag contract](behavior/story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md)
closes bounded233/236 notice writers,1207 pipe/Professor story11 predicate and
GSW0/4 live bank/record mappings; GSF-01 portions checked/DEMO-01 linked.
233 precedes first message;236 independently testsstory>=7; refusal retains live
flags; GSW4 writes only on accepted mode path. No exhaustive census/trace claim.

Validation:3 DOL spans/112 words/2 REL spans/5 imports; exact command nesting/
SET/refusal/pipe/Professor operands/eight vectors/masks/record mappings checked.
Switch manifest63 spans/205 anchors and writer/restore pins re-passed. Link audit
passed993 Markdown files/zero broken links; scoped diff passed. Docs only; no
make test/all rerun or raw asset distribution. Concurrent native edits excluded.
Approved push pending valid GitHub credentials.

Next: exhaustive opening flag read/write/actor-admission census and DEMO-01
cross-scene joins/reentry; RESULT-01 producer/story advance/save-unblock and
original traces. New demo requests and full-game objective remain OPEN; source
bank copy services alone do not establish native/original runtime round-trip.

## 2026-10-13 — Three more demo/end-goal requests: TRACE-01, ITEM-01, PARTNER-FULL-01

Continued the milestone-scoped request loop.
[TRACE-01](requests/TRACE-01.md) asks for the matched original opening-encounter
execution trace — the validation oracle D1 acceptance needs (the completion
assessment notes no matched trace exists, P0a asks for the branch/re-entry trace,
and AC-01/INPUT-01/OW-* already ask for pieces of it).
[ITEM-01](requests/ITEM-01.md) asks for the field inventory/pouch and field item
use, grounded in the native mushroom-counter fixture (`Menu::Items`,
`progress.mushrooms`) versus the separately recovered battle pouch and key-item
reads. [PARTNER-FULL-01](requests/PARTNER-FULL-01.md) asks for the six
non-Goombella partners' field lifecycle and abilities (OW-PARTNER-01 covers only
Goombella; battle-side partner routing exists, the field does not) for full-game
breadth. Index rows and the [milestones](MILESTONES.md) map updated. The docs
agent's concurrent work on item selection and the field party registry overlaps
these, which is expected and fine.

Validation: doc-link check 988 files/0 broken; index diff additive only. No
`make` rerun (docs only). Full-game incomplete.

## 2026-10-12 — Stage-12 counter provenance closed, stale hypothesis withdrawn

Traced the travel-setup region: stage-12 countdown is a computed
travel-tick count (lane float → `fctiwz` → `extsh` → [72] at 8013b2a4–ac),
not the stage-10 stale inheritance hypothesized earlier — withdrawn in the
doc. Lane identity and the [72]==0 bypass rationale stay open. With this,
FIELD-RECOVERY-01 todo 1's numeric half is as closed as static reading gets
(stages, counters incl. provenance, HP tick, transitions, SDA set, named
authored gaps); float-chain lane roles remain the honest remainder.

Validation: sequence asserts, diff --check clean. Docs-only. Next: triangle
interiors, pose domains. Incomplete.

## 2026-10-13 — Inbox save continuation party and destination caller

Completed local continueGame/table/source keys; SAVE-01 portion checked.
[Continuation contract](behavior/memory/SAVE_CONTINUE_GAME_PARTY_AND_DESTINATION_DISPATCH.md)
pins restore/current slot/party/sound/countdown order, switch4 dispatch,25 exact
GP110/108/room/door/sequence3 requests and switch23 stg0/party/bank reset arm.
Switch0 retains saved room; other arms can request authored destinations.
No scene execution/dialogue or unconditional post-load bank lifetime invented.

Validation: ISO manifest37 spans/388 anchors/39 calls; independent all340 words/
25 literal cases/special reset/prefix/call/string/source pins passed; restore
pins reverified. Link audit passed987 Markdown files/zero broken links;
scoped diff passed. Docs only; no make test/all rerun. Raw ELF/listing/generator
stay /tmp; no save blobs/assets/body distribution. Concurrent native edits
excluded. Approved push pending valid GitHub credentials.

Next: upstream loadMain/event/UI admission, GP110/108/switch4 writers and full
party/sound/countdown/stg0/sequence/room/door child composition; async/header/
read-all/repair/SDK schemas and original load/reset/room/party/corruption/native
traces. SAVE-01/native persistence and parent/full-game requests remain OPEN.

## 2026-10-13 — Shared original type0 frame geometry and citro3d binding

Committed feature1248919 adds host-testable original absolute/centered nine-quad
geometry, selector base/tab plans and a reusable native GPU renderer. It reuses
the original72x72 msgWindow descriptor0 texture; source RGB/low8alpha, signed
tab centering and independent rectangle transforms are retained. Original
trio EXP now calls this backend. A preallocated64-frame arena, unique54-vertex
slices and one draw per frame avoid draw-time allocation and deferred-buffer
aliasing. Review fixed explicit citro2d depth/alpha/blend restoration. See
[native scope](behavior/display/NATIVE_TYPE0_WINDOW_FRAME.md).

Validation: shared make test passed167 CTest/110 Python, make all and debug-short
builds passed (/tmp/window-frame-final-{test,native,debug-native}.log). An
isolated source snapshot at1248919 passed166 CTest/110 Python, normal and
debug-short native builds (/tmp/window-frame-isolated-{test,native,short-native}.log).
The shared suite includes concurrent work; isolated results identify this
feature. Temporary diagnostic source probes were restored to exact commit
blobs. Link check passed980 Markdown files/zero broken links; diff check passed.

A clean isolated Azahar capture after40 seconds shows original trio dialogue
(artifacts/azahar-window-frame-crossed-warmup/boot.png). Earlier12-second black
captures were insufficient startup waits: diagnostic logs completed frames,
and the new frame backend had not yet been called. The original before-trigger
fixture also renders both screens. Emulation profiles/save fixtures and assets
remain ignored; the user's emulator profile/saves were not modified.

The longer clean battle run completed successfully in
artifacts/azahar-window-frame-isolated-exp-warm. Direct inspection of
original-exp-and-victory-24.png and its frame-detail crop shows the translucent
original frame beneath animated stars;25/28 show original PointGet/victory
content afterward.40 shows the existing field-restoration-pending development
notice. Reward captures display about29..30 emulator App FPS, not hardware
measurements. Xvfb socket access needed sandbox escalation for this isolated
run. No native exception was reported.

Limitations/next: full selector list/header/help/scissor and manager/input/event
ownership remain unbound. Bind original inventory/persistent switch banks before
Goomba4 quest95/107 branches; expand the existing per-area actor/object registry
without fabricated story. Compare rotated/scaled selector frames, original
GX/PICA raster and New2DS XL hardware speed/memory. No original SDK arithmetic,
matched Dolphin raster, complete selector/quest or full-game completion claim.

## 2026-10-13 — Inbox live save restore and preserved GP fields

Completed cardCopy2Main/cardGetFilePtr and scoped continueGame entry; SAVE-01
portion checked. [Restore contract](behavior/memory/CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md)
pins six reverse copies/all five switch extents, six pre-copy live words/bit8,
early168 repair/late preserved writes, clocks/resets/literal123 and fresh owners.
One direct DOL caller supplies slot and overwrites GP11d0/11b8 after return.
Local read/write/validate/select/restore now documented; full persistence open.

Validation: ISO manifest9 spans/117 anchors/ten calls; independent all99 words/
seven vectors/six copies/preservation/reset/literal/clock/caller sites/exhaustive
one-site census/fresh pins passed. Link audit passed982 Markdown files/zero
broken links; scoped diff passed. Docs only; no make test/all rerun. Raw ELF/
listings/generator stay /tmp; no record/assets/body distribution. Concurrent
native edits excluded. Approved push pending valid GitHub credentials.

Next: full continueGame/admission/party/map/pointer repair/namespace schemas and
later switch lifetime; async/read-all/header/backup/SDK/provider domains and
original save/load/multi-slot/corruption/recovery/native traces. SAVE-01 and
other parent/full-game requests remain OPEN.

## 2026-10-13 — Requests scoped to the first demo milestone

Added [milestones](MILESTONES.md) defining **D1** (the Rogueport opening demo,
the next step), its acceptance criteria and a request map, with **G1**
(full-game playability) as the end goal. Filed three demo-critical provenance
requests the existing set does not cover: [DEMO-01](requests/DEMO-01.md)
(opening entry conditions, persistent writes, branch/join and re-entry — the
integration contract the completion assessment calls missing),
[RESULT-01](requests/RESULT-01.md) (battle-result publication, victory, story
advance and the save-unblock producer) and [GSF-01](requests/GSF-01.md) (opening
story-flag map and save-block story switches). Grounded in
`Game::returnFromBattle`/`Scene::Victory`, `originalFlag1207`/`233`/`236` and the
opening playtest chain. Docs-agent concurrent edits were left untouched.

Validation: doc-link check 980 files/0 broken; index diff additive only. No
`make` rerun (docs only). Full-game incomplete.

## 2026-10-13 — Inbox single-slot read validation and copy selection

Completed local read_main/table; SAVE-01 portion checked.
[Single-slot reader](behavior/memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md)
pins15 phases,7750-byte header/two8800-byte record validation, signed64 timestamp
selection/tie-to-secondary, resident-buffer load/invalid flag, cleanup/finish/
fresh waiting. Header failure latches without abort; validation failure still
copies raw timestamp. Live GP/pouch/switch namespace restore is absent here,
so write inclusion plus reader acceptance does not prove original round-trip.

Validation: read-only ISO manifest13 spans/565 anchors/17 calls; independent
all550 words/15 table entries/13 literal vectors/header+record sums/signed
selection/call/flag/stamp sites/fresh pins passed. Link audit passed976 Markdown
files/zero broken links; scoped diff passed. Docs only; no make test/all rerun.
Raw ELF/listing/generator stay /tmp; no record/assets/body distribution.
Concurrent native edits excluded. Approved push pending valid GitHub credentials.

Next: live resident-record restore/schema/pointer repair/switch-reset consumers,
read-all/async write/header/backup repair/SDK/provider domains and original
save/load/multi-slot/corruption/recovery traces. Native persistence replacement,
SAVE-01 and other parent/full-game requests remain OPEN.

## 2026-10-12 — Water-doc slot-attribution repair

Rewrote the rise/travel stage bullets after finding systematic [68]/[72]
confusion in my own draft (stage-10 "counter = 11", stage-13/14 "counter =
20", inverted land-check branches, drafting leftover in stage 30): [68] =
stage and [72] = counter throughout, verified per transition (13→20, 14→20,
12→13 with one-tick 13). Also corrected stage 11 ([708] −= 1.0 with no-op
10.0 floor; bgmode≠1 → splash+SFX, ==1 → SFX only) and added the travel-rate
fdivs finding. This is the second self-found slot error in these docs;
slot claims now require adjacent-instruction proof, not recollection.

Validation: transition-slot asserts, instruction re-checks, diff --check
clean. Docs-only. Next: triangle interiors, pose domains. Incomplete.


## 2026-10-13 — Inbox card dispatcher and completion callbacks

Completed local cardMain/eight callback bodies and operation table; SAVE-01
portion checked. [Dispatcher contract](behavior/memory/CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md)
supersedes inaccurate CARD_MAIN sketch. Optional probe/change/countdown,
format/check-delete phases, three fresh result polls, iteration caps and
busy/result/error/completion order pinned. Local write/header/read/read-all/
create dispatch is complete; delegated child services remain separate.

Validation: read-only ISO manifest24 spans/480 anchors/23 calls; independent
all471 words/nine table entries/19 literal vectors/eight callbacks/cap/probe/
result/substate/call sites/fresh pins passed. Link audit passed973 Markdown
files/zero broken links; scoped diff passed. Docs/evidence only; no make
test/all rerun. Raw ELF/listings/generator stay /tmp; no save blobs/assets/body
distribution. Concurrent native edits excluded. Approved push pending valid
GitHub credentials after authentication rejection.

Next: read_main/read_all_main/write_main/header/create/card buffers/copy/erase/
SDK validation/backup/error services and exact persistent namespace/lifetime
mapping; original save/load/multi-slot/corruption/recovery traces. SAVE-01/native
persistence replacement and other parent/full-game requests remain OPEN.

## 2026-10-13 — Inbox original save record and checksum preparation

Completed local cardWrite; SAVE-01 record/checksum/write-side bank inclusion
portion checked. [Record contract](behavior/format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md)
pins busy/raw slot/live GP/Mario/party/time publications, six source block copies,
all five switch extents including locals/full-word byte0, version trailer and
8800-byte additive sum/complement. This gives the 2DS XL save adapter concrete
source serialization requirements; full original persistence remains unclosed.

Validation: read-only ISO manifest11 spans/187 anchors/16 calls; independent
all187 words/ten literal vectors/six copies/five switch extents/checksum loop/
masks/time/call sites/fresh pins passed. Link audit passed972 Markdown files/
zero broken links; scoped diff passed. Docs/evidence only; no make test/all
rerun. Raw ELF/listing/generator stay /tmp; no save blobs/assets/body distribution.
Concurrent native edits excluded. Approved push pending valid GitHub credentials.

Next: original reader validation/version/header/async write/cardMain/error/
backup/copy/load plus precise namespace schemas/lifetime; original multi-slot/
corruption/recovery/round-trip traces. Native save replacement, SAVE-01 and
parent/full-game requests remain OPEN.

## 2026-10-13 — Switch soundtrack variants and native audio build

Implemented default Switch1.0.0 soundtrack packaging from the existing decoded
local library:18 exact named arrangements cover the13 existing native cues,
separate Rogueport harbor/east/west/station variants and ordinary battle Think.
Native scene/district/command selection uses these files while keeping original
GameCube primary and house SFX. Title admission validates the selected asset
independently of NDSP/mute. Existing opening BGM fades and three fixed streaming
owners remain active. [Adapter scope](behavior/audio/NATIVE_SWITCH_SOUNDTRACK.md),
[bindings](../config/switch_soundtrack.json), [provenance](ASSETS.md).

The packager verifies PCM hashes, canonical headers/payload extents and source
BFSAR loop sidecars before copying unchanged48k stereo music into ignored
romfs/soundtrack-switch. No original media or converted assets are committed.
Default make all explicitly requires the decoded Switch library;
SOUNDTRACK=gamecube remains supported. A persistent SWM1/GCM1 top-level marker
forces embedded RomFS rebuilds in either direction. Both modes retain original
SFX; the final ttyd-2ds.3dsx uses Switch music. The Switch pack is270MiB;
resulting native binary444,989,908 bytes (GameCube162,148,348). Streaming retains
fixed buffers rather than loading whole songs into linear memory.

Validation: final make test passes173/173 CTest and116/116 Python checks
(artifacts/switch-soundtrack-tests-complete.log). make all passes with both
SOUNDTRACK=gamecube and default Switch, including rebuilt3DSX and mode/track-count
checks (artifacts/switch-soundtrack-{gamecube,switch}-final.log and
switch-soundtrack-build-modes.txt). Isolated Azahar immutable integration binary
under artifacts/switch-soundtrack-snapshot reached original fresh-file house,
letter/voyage/prologue and saved harbor story3. Dedicated temporary audio sink
capture matches Switch Demo_1/Title (.998/.637 normalized correlation) and all
four original house samples (.931/.819/.975/.649). Evidence under
artifacts/azahar-switch-soundtrack/:audio.wav/audio-analysis.json/stages.json
and screenshots; launch log in artifacts/switch-soundtrack-audio-smoke.log.
Original emulator profiles/saves and source ISO/RomFS were untouched.
Final doc-link audit passes and git diff --check passes.

Earlier combined runs caught the concurrent field-party Game-copy compilation
change and two transient missing partner test executables. The stable final
rerun above is green; those failures were not bypassed. Incremental soundtrack
commits fc072e4 and d5a5900 precede the marker/test/validation handoff commit.
Other concurrent native/doc changes remain separate.

Limits/next: selection is a native slice adapter, not recovered Switch adaptive
routing. Normal/Think changes currently restart their streams. Implement phase
synchronization, seamless district transitions, Switch intro/stem chains and
remaining contextual arrangements as later audio work. Listen through all18
tracks/loop boundaries, profile48k stream I/O on actual New2DS XL, and finish
full MusyX/spatial/ENV/SFX consumers. Captured opening output does not establish
all-track or hardware parity, complete prologue gameplay or full-game completion.

## 2026-10-13 — Inbox Rogueport camera coverage reconciliation

[Porting coverage map](behavior/camera/ROGUEPORT_CAMERA_PORTING_COVERAGE_AND_REMAINING_BINDINGS.md)
joins existing bounded geometry/shift/lifecycle/event deliveries into native
2DS XL dependency order; OW-CAMERA-01 delivered portions checked. Fresh metadata
matches five original file hashes;44 version1.01 records have no extra curves/
disables, count12/endpoints2,3;20 have fixed-Y gates. Shipped220 segment pairs
use straight dispatch and494 triangles include234 nonhorizontal/8 zero3D/
16 zeroXZ cases. Full native composition across these domains remains open.

Validation: nine existing source verifiers passed (exact counts in coverage
contract); fresh metadata assertions/file hashes passed. Corrected previous
manager ABI ambiguity: seventh float f7 captured in f22, distinct from integer
r7; added verified source pin, manager manifest13 spans/399 anchors/nine calls.
Link audit passed971 Markdown files/zero broken links; scoped diff passed.
Docs only; no make test/all rerun; no raw assets/body distribution. Concurrent
native edits excluded. Approved push pending valid GitHub credentials.

Next: validate composed candidate/correction queries across all44 source records,
remaining provider/clock/pause/math/FP/alias/runtime domains and original native
camera traces; prioritize fresh field/story/encounter inbox dependencies rather
than re-recovering already delivered bounded contracts. Parents/full-game OPEN.

## 2026-10-13 — Original animated textbox advance star

Implemented in b20c9e8. Ready textboxes now show the original `hoshi_2` /
`HOS_C_1` animated star at the lower right. Native regular windows retain source
(x+width-52,y-height+31), yaw180 and scale5, with uniform0.5 handheld scaling.
The asset converter retains original geometry, node/texture updates and80-frame
loop; no generated replacement star is used. Advance eligibility requires
state3/key wait1/full opacity and excludes selection windows. Typing, opening,
closing, once_stop and closed windows have no star. Input consumption hides it
immediately; the next waiting page shows it again. The separate opening/demo
bottom-text renderer and development notice boxes also use this indicator.
GPU storage is separate per frame/submission, and citro2d restores the matching
screen target after each indicator draw.

Source paths, hashes, local conversions and rights are recorded in
[assets](ASSETS.md); original/extracted/compiled assets stay ignored and the ISO
is read-only. See[native indicator contract](behavior/message/NATIVE_MESSAGE_ADVANCE_INDICATOR.md).

Validation: final `make test` passes **173 CTest / 115 Python tests**
(`/tmp/star-test-final.log`); `make all` builds3DSX without warnings
(`/tmp/star-build-final.log`). Original-data `message_playback_tests original`
passes20 harbor/29 professor payloads plus original indicator import and distinct
animation-phase checks. Host gates cover opening/ready/consumed/closed/selection/
opacity. An initial animation comparison used two equivalent phases0/40 and
failed; changing the comparison to0/10 proves actual animation change. A first
full test run had two Not Run executables while concurrent builds were changing
shared output; the final complete rerun passes. Link audit checks970 Markdown
files/zero broken links; diff check passes. Additional isolated supported
DEBUG_SHORT_BOOT build succeeds (`/tmp/star-short-build.log`).

Isolated normal-build captures use immutable `artifacts/ttyd-message-star.3dsx`.
`artifacts/azahar-message-star-verified/` confirms original harbor text with the
star at lower right, differing ready-animation images, disappearance after
advance and return on the next page. `artifacts/azahar-message-star-front/`
confirms Parakarry's original opening message with the star on the bottom
text area. The first fixed-delay script remained in the title/demo flow; the
revised `tests/azahar_message_indicator.py` verifies a real window before
capturing. Bottom capture used a temporary /tmp script that detects the original
star color in its expected rectangle. User emulator profiles/saves are untouched.

Limits/next: original shared pose-active bit, callback/material lifetime and
render-dependent animation cadence remain unbound. The retained original
animation runs against native60Hz simulation ticks. Frontend text-area placement
and small-window cue policy remain documented native presentation adapters.
Hardware and matched Dolphin indicator images remain unverified; complete
message services/story/full-game equivalence is still unfinished.


## 2026-10-13 — Inbox camera-road manager and timebase composition

Completed full local camRoadMain/getV60FPS; OW-CAMERA-01 portion checked.
[Manager/timebase supplement](behavior/camera/FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md)
strengthens existing finite transition evidence with fresh/captured owners,
eight ordered pair-copy loops, timestamp/progress/publication/scaling order,
signed division requests and exact returned-quotient rollover branch without+1.
Earlier comparison/interpolation contracts now link the completed local owner.
No new authored geometry/limit or measured camera/pause/cadence claim.

Validation: read-only ISO manifest13 spans/398 anchors/nine calls; independent
all370 words/17 literal vectors/eight96-byte copy layouts/publication/time/
divider/call sites/constants/fresh pins passed. Existing finite transition
verifier passed seven spans/62 anchors/eight admission cases. Link audit
passed969 Markdown files/zero broken links; scoped diff passed. Docs only; no
make test/all rerun. Raw ELF/listings/generator stay /tmp; no binary assets/
bodies committed. Concurrent native edits excluded. Approved push pending
valid GitHub credentials after authentication rejection.

Next: calcCamRoad/collisionCurve authored selection/contact/geometry and limits,
using existing road-state/transition evidence before further recovery. Library/
work/state/record/time/bus/GP/IRQ/alias/FP domains and original room/control/
pause/cadence/visual/native binding remain OPEN, as do parent/full-game requests.

## 2026-10-13 — Inbox road interpolation and current camera state

Completed local calcHokanCamRoad/calcCurrentCamState; OW-CAMERA-01 portions
checked. [Interpolation/current construction](behavior/camera/FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md)
pins exact phase/easing constants, upper-only weight clamp, nearest-angle wraps,
ordered field writes/mode0 points, unused r6 argument, prefix-before-inactive
state and captured active geometry/cos-sin-cos/matrix requests. These supply
transition construction dependencies for the 2DS XL camera; original room
selection/limits and measured native framing remain unclosed.

Validation: read-only ISO manifest20 spans/294 anchors/twelve direct calls;
independent all274 words/14 literal mathematical vectors/ordered writes/capture/
gate/loop/call/caller sites/twelve constants/fresh pins passed. Link audit
passed965 Markdown files/zero broken links; scoped diff check passed. Docs only,
no make test/all rerun. Raw ELF/listings/generator stay /tmp; no binary assets/
bodies committed. Concurrent native edits excluded. Approved push pending valid
GitHub credentials after authentication rejection.

Next: full camRoadMain transition timestamp/copy/progress/consumer composition,
then calcCamRoad/collisionCurve authored selection/contact/geometry and limits.
State/profile/math/matrix/FP/alias/value domains and original room/control/
pause/cadence/visual/native binding remain OPEN, as do parent/full-game requests.

## 2026-10-13 — Inbox camera-road comparison and reciprocal rate

Completed local compareCamRoad and two scoped caller directions; OW-CAMERA-01
portion checked. [Road comparison](behavior/camera/FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md)
pins distinct integer/float progress rejection, transition-mask accumulation,
fresh profile-weight metrics, point-distance/sqrt branches, joint deadband and
reciprocal publication. Early rejection preserves output; camRoadMain can
reverse state arguments. No authored geometry/clamp or measured native timing
inferred. This supplies a road-transition dependency for 2DS XL framing.

Validation: read-only ISO manifest18 spans/306 anchors/one call; independent
all279 body words/17 literal vectors/masks/FP/global/weight/output/caller sites/
constants/fresh source pins passed. Link audit passed963 Markdown files/zero
broken links; scoped diff check passed. Docs/evidence only, no make test/all
rerun. Raw ELF/listings/generator stay /tmp; no bodies/assets committed.
Concurrent native edits excluded. Approved push pending valid GitHub credentials.

Next: calcCamRoad/collisionCurve authored geometry/contact/selection and
calcHokanCamRoad/calcCurrentCamState/camRoadMain transition composition; actual
state/profile/FP/alias/value domains, room limits and original visual/pause/
cadence/native binding. OW-CAMERA-01/other inbox/full-game remain OPEN.

## 2026-10-13 — Inbox camera defaults, IDs and reset owners

Completed eight local camera rate/ID/reset leaves; OW-CAMERA-01 portion checked.
[Reset contract](behavior/camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md)
separates follow15c reset .2 from shift150/154/158 .03/.06/.06, preserves current
shift channels, and pins distinct raw ffffffff templates/ordered writes. Forty
direct DOL callers identify init/reinit, camera, party, motion/EVT/item sources;
caller admission and other writers remain open. Linked the previously recovered
camera lifecycle/projection evidence into the request instead of re-recovering
its bounded ordinary graph. Native .07 smoothing remains provisional.

Validation: read-only ISO manifest16 spans/167 anchors/eight calls; independent
all116 body words/exact reset layouts/templates/capture sites/constants and
exhaustive40-site DOL caller census passed. Existing lifecycle verifier passed
39 spans/72 anchors/five camera files/75 event owners/332 bindings. Link audit
passed961 Markdown files/zero broken links; scoped diff check passed. Docs only;
no make test/all rerun. Raw ELFs/listings/generator stay /tmp. Concurrent native
changes excluded. Approved push still pending valid GitHub credentials.

Next: authored road selection/curve/transition/limits through calcCamRoad,
collisionCurve, compareCamRoad, calcHokanCamRoad, calcCurrentCamState and
camRoadMain, using existing pinned room containers. Other callers/writers/
owner/value/FP domains, pause/cadence/original visuals/native binding and parent
requests/full-game remain OPEN.

## 2026-10-13 — Inbox native camera dependency: shifts and update controls

Completed local marioCamMain/marioUpdateCam and four camera-control leaves;
OW-CAMERA-01 portion checked, MAINLOOP-01 child delivery linked.
[Camera contract](behavior/camera/FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md)
records three biased shift channels and separately produced rates, post-shift
position-update gate, FollowY target construction, motion25 height adjustment,
entry/timer/heading owner captures and expiration/cached-heading request order.
This gives the 2DS XL port concrete source rules without treating native .07
smoothing as original framing or claiming room-camera closure.

Validation: read-only ISO manifest15 spans/294 anchors/17 calls. Independent
all290 words/27 literal vectors/every65536 signed-halfword timer/duration input,
decoded masks/channel/call identities/source pins passed. Link audit passed959
Markdown files/zero broken links; scoped diff check passed. Docs/evidence only,
no make test/all rerun. Raw ELF/listings/generator stay /tmp; no binary assets/
bodies committed. Concurrent native edits excluded. Approved push still pending
valid GitHub credentials after authentication rejection.

Next: camera rate/default writers, room/camMain/zoom/event/trig consumers and
original height/lock/pause/cadence/visual binding; choose further inbox contracts
from current native field/story/encounter needs. OW-CAMERA-01/MAINLOOP-01 and
other parent/full-game requests remain OPEN.

## 2026-10-13 — Documentation gap analysis and remaining work

Completed analysis of documentation completeness against US GameCube (G8ME01 rev 0)
baseline. Key findings:

- 7,061 DOL text entries; 297 fingerprinted with direct branch references
- 260 US message files retained byte-for-byte under romfs/messages/
- 276 battle docs, 96 event docs, 56 field docs already delivered
- 20 registered action command IDs; only ~5 fingerprinted with full DOL references

**Critical open requests and their remaining gaps:**

| Request | Status | Key Gaps |
|---|---|---|
| **AC-01** (Crump/Jump callers) | Open (partially answered) | Tutorial exceptions/overrides, original execution traces, EVT-01 scheduler binding, RNG/seed writers at call sites |
| **AC-02** (Hammer binding) | Open (period/base answered) | Suppression/automatic/forced writers, result/damage consumers, display/gauge semantics, original charge audio traces |
| **AC-03** (Defense callers) | Open (attack graph answered) | SDK physical poll/clamp/reset, full pause/physical cadence, weapon gate (+0x13) writers, guard/status gates, original single/mash/A+B/held/early/late traces, shared forced-command counter |
| **EVT-01** (Event scheduler) | Open (local scheduler answered) | Original scheduler traces, weapon cursor invariants, dynamic/global source provisioning, full audio/Monosiri binding, pause/multicredit/creation/deletion/wait traces |
| **JUMP-01** (Jump flow) | Open | Complete event 80355cf0 control flow, original timing/state traces |
| **RESULT-01** (Battle result) | Open | Result record producer, victory/defeat determination, story advance, reward/field-return, save-unblock condition |
| **MAINLOOP-01** (Mario field) | Open (graph answered) | Child/provider/native binding, sequence5 ownership, physical cadence/pause, camera/strcmp/HP sequence identities |

**Already-delivered documentation (substantial progress):**
- 700+ behavior documentation files across 27+ subsystems
- 260 US message files packaged byte-for-byte under ignored romfs/messages/
- Comprehensive battle, event, field, input, unit, partner, camera, display, audio, message, story, file, memory, format, binary, and subsystem documentation
- 297 function fingerprints with instruction anchors and data-word candidates
- US completion assessment with priority register (P0a-P3)

**Root cause of remaining gaps:** The project has recovered static analysis evidence (function boundaries, callback signatures, data layouts, opcode implementations) but lacks original execution traces (Dolphin gameplay recordings with controlled inputs/timing) needed to validate runtime equivalence. Portable helpers alone do not answer full runtime binding requests.

Next work: Prioritize P0a (opening harbor→Crump→Rogueport story graph) through P0b (encounter and common attack inputs), then P0c (main-loop/input/ownership foundation). Resolve shipped US call chains, request-inbox blockers, and original encounter's story/event/persistence path before advancing to generic library edge cases.

See [US completion assessment](behavior/meta/US_COMPLETION_AND_PRIORITIES.md) for detailed system rows and [behavior/README.md](behavior/README.md) for full documentation register.

## 2026-10-13 — Inbox DVD error category and recovery dispatch

Completed CategorizeError/cbForStateGettingError; EVT-01 portions checked.
[Category/recovery contract](behavior/file/SDK_DVD_ERROR_CATEGORY_AND_RECOVERY_DISPATCH.md)
pins exact full-word special case, low24/retry reread/last-error, captured high
byte, cancellation resume and retry/seek/last-state/cover/motor dispatch. This
preserves loading/cancellation semantics relevant to the 2DS XL port; hardware
recovery children, physical error meanings and actual providers remain open.

Validation: read-only ISO manifest passed12 spans/198 anchors/17 direct calls.
Twelve category/eight resume/nine route/six status mathematical vectors and
three indirect/global/publication sites checked; error-wrapper pins reverified.
Link audit passed956 Markdown files/zero broken links; scoped diff check passed.
Static deductions remain separate from original execution. Docs/metadata only;
no make test/all rerun. ELFs/listings/generator stay /tmp; no bodies/assets
committed. Concurrent native work excluded. Approved push pending valid GitHub
credentials after authentication rejection; no push retried without a change.

Next: select inbox work by concrete native 2DS XL dependencies before further
DVD hardware internals. EVT-01 recovery/provider/IRQ/alias/lifetime/ABI and
original IO/audio traces remain open, as do other inbox/full-game requests.

## 2026-10-13 — Inbox priority-zero DVD error/timeout/fatal bridge

Completed cbForStateError/stateTimeout/__DVDPrintFatalMessage; EVT-01 checked.
[Error wrappers](behavior/file/SDK_DVD_ERROR_CALLBACK_TIMEOUT_AND_FATAL_BRIDGE.md)
pin state-before-exact16, timeout/error0 reentry, optional fatal handler, fresh
post-handler executing/dummy/fatal flag and cancel-pointer-before-clear/ready.
Error/reset/handler/providers/original IO/audio/native binding remains open.

Validation: read-only ISO manifest passed six spans,68 anchors,six calls.
Five status/six cancel/three handler/two snapshot models, timeout order/three
indirect/global/publication sites checked; whole busy callback pins reverified.
Link audit passed951 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 cbForStateGettingError/reset/error-store/low-level and
actual fatal handler/status/executing/cancel/provider/value/extent/alias/lifetime/
IRQ/concurrency/ABI domains, original timeout/nonreturn/fatal/cancel/IO/audio
traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Original item presentation resources and visible frontend colors

Committed7559a76 adds the reusable
[item presentation catalog](behavior/unit/NATIVE_ITEM_PRESENTATION_CATALOG.md)
and original selector art. Read-only source itemDataTable803108a8/3520hex,
verified SHA256cd9e52c1f0946f785301879d0e9560a5b87860abd648e9721973159291d0add9,
exports all340 physical records in original ID order. Original name+4/description+8 keys, their pointer identities and iconU16+20hex
survive;
null record339 stays unavailable, distinct from an empty key. Pack16578bytes.
The transactional native reader rejects malformed/truncated/trailing data and
preserves resident pointers on failure. Renderer startup admits metadata once;
lookup is direct indexing with no per-draw file read/allocation/search. Existing
room-first/global-second message lookup resolves original keys/tokens; all keys
for0..338 resolve in original US global bank. No fabricated item prose or
weapon/game-code table import.

Shared original icon atlas now includes item IDs307/308 (TPL312/313,40x40),
up/down445/446 (465/466,48x48) and stripe447 (467,24x24), preserving source
loop/frame/delay metadata and pixels. These fit existing512x512 RGBA8 atlas
with2px transparent padding and no resampling or GPU dimension increase.
Metadata covers future areas/items; only admitted icons are resident. General
animated item icons must retain their complete source frames when later packed.
Local original packs/assets remain ignored; licensing/conversion recorded in
ASSETS. No new quest or item window is activated here.

Isolated startup capture exposed original title images rendered solid white.
87a1c49 corrects frontend C2D tint strength1 to0: preserve original texture RGB
while retaining source-alpha modulation. This also fixes shared raster map/
letter/prologue/demo image color routing. It does not change story logic or
text. b635b5f moves the unchanged136x32 copyright draw fromY215 to208 to retain
its last7 lines inside the240-line native viewport; this is native screen reflow,
not a measured original coordinate. Original pixels/aspect remain unchanged.

Delegated source recovery delivered
[complete physical metadata](behavior/unit/ITEM_PRESENTATION_METADATA.md) and
[item-selector frame/tab](behavior/display/ITEM_SELECTOR_WINDOW_FRAME.md),
with OW-INTERACT-01 breadcrumbs; request remains OPEN. Frame recovery pins
Rz*S, separate rectangle-center translations for base and blue tab, exact
nine-slice36vertices/180writes, original descriptor0 texture/material and
low8alpha ownership. Original SDK rounding/GX raster/backend parity remain open.
Independent native review found no blocker; no foreign work staged.

Validation: final shared `make test` passes166 CTest/110 Python,
/tmp/item-presentation-reflow-test.log; `make all` passes without warnings/errors,
/tmp/item-presentation-reflow-native.log. Clean committed-source snapshot
artifacts/source-checks/item-catalog-0chgvphv passes165 CTest/110 Python at
87a1c49, /tmp/item-catalog-isolated-final-test.log. All1991 tracked blobs matched
that commit. Updated snapshot b635b5f passes native `make all`,
/tmp/item-catalog-isolated-reflow-native.log; final binary
artifacts/ttyd-item-presentation.3dsx comes from that snapshot. Shared/isolated
count difference reflects concurrent uncommitted test registration, not a skip.
Tests cover null/empty-key and key-byte boundaries, extent/truncation/reserved/
trailing rejection and retained pointers, original US lookup for every populated
record, exact95/107 keys/icons, original icon dimensions/frame metadata and
pixel-identical selector atlas regions. Link audit949 Markdown/zero broken,
whitespace passes. No added trivial tint-value unit test; visual smoke verifies
that correction instead.

Azahar isolated final startup capture succeeds under
artifacts/azahar-item-presentation-final with original colored title/full
copyright and file menu visible, no native exception. Initial short presses
missed low-rate startup frames; temporary held-input capture script reached both
states. Earlier before/after evidence stays in azahar-item-presentation-held and
azahar-item-presentation-colors. These are startup/visual regression captures,
not matched Dolphin pixels, complete selector UI or hardware benchmarks. User
emulator profiles/saves untouched; concurrent battle/audio/docs work preserved.

Next: bind the three source selector windows/list/help/font/scissor/status/audio
and original input/GP services using recovered frame transforms; retain event
ownership across multi-message/selector quest lifetimes. Complete original
pouch/switch persistence before enabling full Goomba item95/107 quest mutations,
then execute exact original graphs/messages/speakers/flags and extend original
NPC/event/object lifetimes into more areas. Frontend title/scene placement and
material/camera fidelity beyond these corrections still require source/runtime
comparisons; full New2DS performance remains unmeasured. Full requested goal
remains active; early Rogueport is still a slice.

## 2026-10-13 — Inbox priority-zero whole DVD busy callback

Completed error tail/whole local cbForStateBusy; checked EVT-01 portion and
kept child/provider/runtime/native work open.
[Error/composition](behavior/file/SDK_DVD_BUSY_CALLBACK_ERROR_AND_WHOLE_COMPOSITION.md)
pins inquiry priority/four-read command gate, equality despite success-clear,
cancel/complete fresh-record behavior and error request. Whole local body closed.

Validation: read-only ISO new manifest passed eight spans,73 anchors,five calls.
Three indirect/two bindings/eight route/three raw callback models/publication
sites checked; prior regions reverified. All406 disjoint words cover658-byte
callback against ISO;22 direct calls/15 indirect sites composed. Link audit
passed950 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full callback ELF/listing/scoped generator stay /tmp;
no bodies/assets committed. Concurrent native work excluded. Approved push
pending valid GitHub credentials.

Next: priority-zero EVT-01 cbForStateGettingError/cbForStateError/low-level/reset/
cover/cancel and actual providers/value/extent/alias/lifetime/IRQ/concurrency/ABI,
original inquiry/equality/error/short-read/recovery/callback/IO/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero busy callback success/audio

Completed cbForStateBusy success/ImmCommand table region; checked EVT-01 portion.
[Success/audio](behavior/file/SDK_DVD_BUSY_CALLBACK_SUCCESS_COMPLETION_AND_AUDIO.md)
pins motor-before-cancel, resume0/cancel, exact DMA equality/reissue, mutable
immediate/controller results and command6 reissue/reject/completion. Error tail/
whole callback/providers/original IO/audio/native binding remain open.

Validation: read-only ISO manifest passed six spans,198 body+3 table anchors,
nine calls. Five gate/five precedence/five DMA/seven immediate/five result/four
audio vectors and eight indirect/global/publication sites checked; prefix pins
reverified. Link audit passed949 Markdown files/zero broken links; scoped diff
check passed. Mathematical deductions separate from original execution. Docs/
metadata only; no make test/all rerun. Fresh full callback ELF/listing/scoped
generator stay /tmp; no bodies/assets committed. Concurrent native work excluded.
Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 same callback error tail/whole composition, actual
status/command/DMA/table/controller/progress/motor/cancel/audio/callback/providers
and valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains, original
chunk/short-read/immediate/audio/callback/IO traces. Other inbox/full-game open.

## 2026-10-13 — Inbox priority-zero busy callback prefix

Completed cbForStateBusy fatal/stop/progress/cancel prefix/return ABI;
checked EVT-01 portion and added explicit success/error continuation todo.
[Callback prefix](behavior/file/SDK_DVD_BUSY_CALLBACK_STOP_PROGRESS_AND_CANCEL_PREFIX.md)
pins exact16/two-read stop, DMA-progress-before-status8, stop-resume7 versus
non-stop cancel and command-before-fresh-cancel callback/ready order. Whole
success/error/children/providers/original IO/audio/native binding open.

Validation: read-only ISO manifest passed nine spans,135 anchors,eight calls.
Eight status/stop/seven DMA/six wrapped-progress/six prefix-cancel vectors and
four indirect/global/publication sites checked; busy pins reverified. Link audit
passed947 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full callback ELF/listing/scoped generator stay /tmp;
no bodies/assets committed. Concurrent native work excluded. Approved push
pending valid GitHub credentials.

Next: priority-zero EVT-01 same callback success/error continuation and actual
status/controller/command/chunk/progress/cancel/resume/callback/providers and
valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains, original stop/
cover/cancel/short-read/callback/IO/audio traces. Other inbox/native/full-game open.

## 2026-10-13 — Inbox priority-zero SDK DVD busy dispatch

Completed stateBusy/17-entry dispatch table; checked EVT-01 portion.
[Busy dispatch](behavior/file/SDK_DVD_STATE_BUSY_COMMAND_DISPATCH_AND_READ_CHUNKS.md)
pins LastState/command/MMIO, input versus global executing, zero callback/ready,
fresh512KiB chunk/address fields/audio variants/optional ABI. Completion/low-level/
providers/original IO/audio/native binding remain open.

Validation: read-only ISO manifest passed thirteen spans,208 body+17 table
anchors,seventeen calls. Eight chunk/three address/four auto/six optional vectors,
full dispatch/MMIO/publication/ABI/two indirect sites checked; ready pins reverified.
Link audit passed944 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELF/listing/generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 cbForStateBusy/optional/low-level read and other command
engines/providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains, original
zero/chunk/short-read/controller/callback/IO/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Original item-table services and live key-item guards

Committedd938dbe: [item-selection core](behavior/message/NATIVE_ITEM_TABLE_SELECTION_CORE.md)
and [key-item services](behavior/unit/NATIVE_KEY_ITEM_INVENTORY_SERVICES.md)
provide original packed row initialization/result polling/event protocol,
three-window list lifecycle/navigation/easing and sequence0/2 transition models.
Source rows preserve duplicates, low16 identities and separate allocation/clear;
count includes-1, fill rereads each admitted entry, and row-count publication
precedes item write. Result uses U16 flags and duplicate index reads; event
wrapper caches stream pointer, retains handle in E+78, locks keys, polls,
gets index before delete/unlock and reads destinations afterward. No fake
cancel row or message Yes/No substitution.

List control preserves independent directions, trigger-only endpoint wrap,
eight-row scroll, disabled-row/A-before-B priority, active-slot/help-reset gates,
close-before-ready and original sound/status requests. Sequence uses original
signed-word refresh*8/60 formula and curve12 requests, fresh timing across
rotation/scale/comparison, and leaves transition4 until next idle. Source
recovery corrected an initial documentation mask error: idle preserves visible2.
Fused easing and separate divide/add rounding are modeled; original GP/input/
SDK numeric providers and native drawing remain distinct future bindings.
No fixed8-tick transition is represented as measured original behavior.

Key helpers scan all121 signed key halfwords and compact only the first match
through every active slot, including gaps/zeros. Generic item event removal
calls Mario then party revision only for low-level1 and writes0, else-1.
The live original ProloguePouch adapter supplies count/removal mechanics;
source Goomba guard now reaches unchanged ordinary US dialogue when a matching
known quest/trueflag has an actual original key count0. Missing pouch stays
unknown regardless of native mushroom quantity; positive items keep unsupported
quest playback blocked. Other actor profiles do not scan this key array.
No new checkpoint schema, inventory inference, quest flag/story write or
positive-item quest scene is enabled here.

Delegated source documentation recovered
[original item-window contract](behavior/message/ITEM_TABLE_SELECTION_WINDOW.md)
from read-only ISO and appended OW-INTERACT-01. It pins manager templates,
rows, controls, read/effect order, original US keys/icons307/308 and source
TPL312/313. Native full UI/font/help/status/audio providers and numbered selector
producers remain open. No new image/audio conversion; original resources remain
local. Added /romfs/ ignore rule so all future generated packs/metadata stay out
of source history. d92bbdf places inventory contract in its owning unit folder.

Validation: final shared `make test` passes164 CTest/107 Python,
`/tmp/item-window-shared-test-complete.log`; shared `make all` passes without
warnings/errors, `/tmp/item-window-shared-native-complete.log`. After correcting
late concurrent Game::tick staging overlap in19e8f65 (working code preserved),
clean committed-source snapshot
`artifacts/source-checks/item-window-koxm16ky` passes163 CTest/107 Python,
`/tmp/item-window-isolated-test.log`, and native `make all` without warnings/
errors, `/tmp/item-window-isolated-native.log`. Binary
`artifacts/ttyd-item-window-core.3dsx` copied from that snapshot. Snapshot uses
ignored project artifacts storage rather than filling16GiB /tmp.

Tests cover two-pass allocation/read/write order, duplicate/cast/prepend rows,
callback table mutation, ready/cancel/unsigned result, retained E+78 and late
operand destinations, fresh/repeat wrapping, scroll, A/B/error, transition/key/
status lifecycle, active-slot gates, independent rational/IEEE easing vector,
sequence visibility/close/timing re-reads, key gaps/first-removal/order/extent,
wrapper revision0/-1 ordering, and original-data live empty/positive/missing
pouch Goomba guards with unchanged US message/return. Independent review found
no blocker and suggested numeric/mutation vectors; both added. Link audit
passes942 Markdown/zero broken links; whitespace passes. No emulator rerun or
new on-screen item menu/hardware/runtime parity is claimed. User profiles/saves
untouched; concurrent battle/audio/docs working changes preserved.

Next: complete source pouch/switch persistence, original item metadata/US
name/icon lookup and three-window drawing/help/input/status/audio services;
register item/inventory callbacks and execute both complete Goomba quest graphs
with exact messages/speakers/flags/return ownership. Then extend original area
NPC/event/object lifetimes and compare Dolphin/New2DS rendering/performance.
The full requested goal remains active; early Rogueport is still a slice.

## 2026-10-13 — Inbox priority-zero SDK DVD ready/resume

Completed stateReady; checked EVT-01 portion.
[Ready/resume/alarm](behavior/file/SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md)
pins check/pause/pop/fatal/two-read resume/motor, dummy-before-callback, recursion-
before-resume-clear and reset/raw alarm order. Busy/low-level/callback/providers/
original IO/audio/native binding remain open.

Validation: read-only ISO manifest passed fourteen spans,186 anchors,nineteen
calls. Six admission/ten resume/nine command/five read-pair/five alarm vectors,
three indirect sites/six bindings/global/state/recursion sites checked; queue
pins reverified. Link audit passed943 Markdown files/zero broken links; scoped
diff check passed. Mathematical deductions separate from original execution;
alarm vectors do not establish measured time. Docs/metadata only; no make test/
all rerun. Fresh full ELF/listing/generator stay /tmp; no bodies/assets committed.
Concurrent native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 stateBusy/low-level/reset/ready callbacks and actual
queue/command/pause/fatal/resume/motor/clock/alarm/provider/value/extent/alias/
lifetime/IRQ/concurrency/ABI/numeric-time domains, original command/recovery/
recursive-callback/IO/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero SDK DVD queue consumers

Completed pop/check/dequeue and all5 local dvdqueue.c functions; EVT-01 checked.
[Consumers/coverage](behavior/file/SDK_DVD_QUEUE_POP_CHECK_DEQUEUE_AND_LOCAL_COVERAGE.md)
pins four-head predicates/split IRQ/fresh selected head/post-restore node clears/
retained dequeue links. Command/cancel/providers/original IO/native binding open.

Validation: read-only ISO manifest passed five spans,86 anchors,eleven calls.
Five selection/three gap/five dequeue/one stale-link models/publication sites
checked; clear/push pins reverified, all5 functions/126 words matched ISO. Link
audit passed940 Markdown files/zero broken links; scoped diff check passed.
Mathematical models separate from original execution. Docs/metadata only; no
make test/all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/assets
committed. Concurrent native work excluded. Approved push pending valid GitHub
credentials.

Next: priority-zero EVT-01 stateReady/command engine/cancellation and actual
queue/link/priority/token/state/ownership/provider/value/extent/alias/lifetime/
IRQ/concurrency/ABI domains, original order/pop/dequeue/pause/cancel/callback/
IO/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero SDK DVD async submission/append

Completed DVDReadAbsAsyncPrio/__DVDPushWaitingQueue/__DVDClearWaitingQueue;
checked EVT-01 portions. [Submission/append](behavior/file/SDK_DVD_ASYNC_SUBMISSION_AND_QUEUE_APPEND.md)
pins command/cache-before-IRQ, state2, raw priority/fresh-tail link order, ready
conditional and normal1 return. Queue consumers/command engine/providers/
original IO/audio/native binding remain open.

Validation: read-only ISO manifest passed seven spans,95 anchors,seven calls.
Eight invalidation/five ready/seven priority/three append models, six field
writes/four head resets/publication sites checked; read-wrapper pins reverified.
Link audit passed939 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 queue pop/check/dequeue/stateReady/command engine,
actual queue/priority/command/global-state/token/providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, original order/pause/cache/callback/
IO/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero SDK DVD read/close/wake

Completed DVDReadPrio/DVDClose/dvdfs cbForReadSync; checked EVT-01 portion.
[SDK read/close](behavior/file/SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md)
pins wrapped bounds/async submission, saved IRQ token/three terminal states/
shared sleep-wake queue and unconditional close1 after cancel. Child commands/
thread/cancel/providers/original IO/audio/native binding remain open.

Validation: read-only ISO manifest passed twelve spans,88 anchors,eight calls.
Ten bounds/six state/four submission/three close vectors and ABI/field/callback/
publication sites checked; manager pins reverified. Link audit passed937 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh full ELFs/listings/generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 DVDReadAbsAsyncPrio/command engine/DVDCancel/thread/
IRQ/panic and actual fileInfo/state/completion/queue/provider/value/extent/alias/
lifetime/concurrency/ABI domains, original submission/status/short-read/error/
wakeup/cancel/IO/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero DVD lifecycle/worker

Completed DVDMgrInit/Delete/dvdmgr.o proc_main; checked EVT-01 portion.
[Lifecycle/worker](behavior/file/DVD_MANAGER_INITIALIZATION_CANCELLATION_AND_WORKER.md)
pins queue-before-thread, zero-create self-loop, flag-before-resume, cancel-only
cleanup and infinite main-yield ordering. SDK/providers/original lifecycle/IO/
audio/native binding remain open.

Validation: read-only ISO manifest passed ten spans,57 anchors,seven calls.
Five creation/four deletion/four finite worker-prefix vectors/thread args and
publication/backedge sites checked; main pins reverified. Link audit passed936
Markdown files/zero broken links; scoped diff check passed. Mathematical source
deductions separate from original execution. Docs/metadata only; no make test/
all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/assets committed.
Concurrent native changes excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 SDK DVDReadPrio/Close/path/fileInfo/thread/cancel/yield,
actual lifecycle/queue/callback/request/providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains, original failure/cancel/reinit/yield/IO/audio
traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero DVD manager sorted service

Completed DVDMgrMain/DVD-specific comparator; checked EVT-01 portion.
[Sorted service](behavior/file/DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md)
pins occupied snapshot/low8 sorting/full-priority signed cap, result-3/negative80
block/nonnegative requested-count advancement, fresh callback/close and first
nonnegative scan break. Init/Delete/worker/SDK/providers/original IO/audio/native
binding remain open.

Validation: read-only ISO manifest passed six spans,170 anchors,five calls.
Seven cap/seven result/five advancement/six comparator/five completion-flag/seven
service-gate vectors, three indirect sites/comparator binding/publications checked.
Link audit passed934 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 DVDMgrInit/Delete/proc_main/SDK/read/error recovery,
actual queue/thread/request/callback/providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains, original sorted service/short-read/error/
close/callback/IO/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero DPCM decoder/table

Completed ssDecodeDPCM/16-value dtbl; checked EVT-01 portion.
[DPCM decoder](behavior/audio/GAME_STREAM_DPCM_MARKERS_PREDICTOR_AND_BOUNDARIES.md)
pins marker/no-output, packed nibble/state, signed scale/saturation, output/count,
buffer/retry/switch/loop predictor reset and source-pointer return. Providers/
manager/original decoded audio/native binding remain open.

Validation: read-only ISO manifest passed two spans,132 anchors,zero calls and
16 signed scale values. Six nibble/six marker/seven predictor/six packet/seven
boundary/three zero-request vectors and publication sites checked; PCM/producer
pins reverified. Link audit passed932 Markdown files/zero broken links; scoped
diff check passed. Mathematical deductions separate from original execution.
Docs/metadata only; no make test/all rerun. Fresh full ELF/listing/generator stay
/tmp; no bodies/assets committed. Concurrent native changes excluded. Approved
push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 deferred DVD manager/providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/AI/DSP and
original packet-boundary/saturation/starvation/loop/refill/callback/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero PCM and whole stream producer

Completed PCM continuation/whole local _sscallback; checked EVT-01 and kept
ssDecodeDPCM/manager/providers/original/native binding open.
[PCM/composition](behavior/audio/GAME_STREAM_PRODUCER_PCM_AND_WHOLE_COMPOSITION.md)
pins initial alignment/per-copy count, boundary-before-position, retry skipping
position/end and copied-count early-stop return. All local producer routes closed.

Validation: read-only ISO new manifest passed two spans,136 anchors,zero calls.
Prior dispatch/encoded manifests reverified; all521 disjoint words cover complete
824-byte callback, checked against ISO. Six alignment/seven step/four return
vectors/publication sites checked. Link audit passed931 Markdown files/zero
broken links; scoped diff check passed. Mathematical deductions separate from
original execution. Docs/metadata only; no make test/all rerun. Fresh full callback/
helper ELFs/listings/scoped generator stay /tmp; no bodies/assets committed.
Concurrent native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 ssDecodeDPCM/deferred DVD manager and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/
AI/DSP and original starvation/buffer-switch/loop/early-stop/refill/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero producer live encoded region

Completed _sscallback subrecord/decode-call/ADPCM byte-copy region; EVT-01 checked.
[Live encoded](behavior/audio/GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md)
pins tag/format/helper-return chain, rounded bytes/fresh cursor/count, boundary
rollback/switch/total restart and second-span/requested-return ordering. PCM/
decoder/whole callback/providers/original refill/audio/native binding open.

Validation: read-only ISO manifest passed two spans,172 anchors,two calls.
Ten size/seven tag/five route/six boundary/four total/four return vectors checked;
prefix pins reverified. Link audit passed929 Markdown files/zero broken links;
scoped diff check passed. Mathematical deductions separate from original
execution. Docs/metadata only; no make test/all rerun. Fresh full callback/helper
ELFs/listings/scoped generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback PCM/whole composition and ssDecodeDPCM,
deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/ABI
and original starvation/loop/early-stop/short-span/refill/audio traces. Other
inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero producer dispatch/silence prefix

Completed _sscallback dispatch/silent-fill prefix and return ABI; EVT-01 checked.
[Producer silence](behavior/audio/GAME_STREAM_PRODUCER_DISPATCH_AND_SILENT_FILL_PREFIX.md)
pins low8 row/all-zero/gates/latch, PCM per-store count and ADPCM whole-block
zeroing with original count return. Whole live refill/helpers/providers/original
mute/refill/audio/native binding remain open.

Validation: read-only ISO manifest passed two spans,213 anchors,zero calls.
Ten encoded-count/seven tag/ten dispatch/six PCM/five all-zero vectors checked.
Link audit passed928 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution; raw large loop words
are models, not actual fills. Docs/metadata only; no make test/all rerun. Fresh
full ELF/listing/scoped generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback live refill/helpers, deferred DVD manager/
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains and original
mute/block-boundary/short-span/refill/callback/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header whole callback

Completed allocation/zeroing/mix/cache tail and whole SoundSSPlayChEx_main;
checked EVT-01 local completion while keeping original/provider binding open.
[Tail/composition](behavior/audio/GAME_STREAM_HEADER_ALLOCATION_CACHE_AND_COMPOSITION.md)
pins fixed zeroing, raw allocation failure publication, deferred callbacks,
fade/mix recapture and fresh cache setup. _sscallback/manager/providers/original
startup/audio/native binding remain open.

Validation: read-only ISO new manifest passed eleven spans,253 anchors,ten calls.
Prefix/initial-read manifests reverified; all627 disjoint words cover complete
9cc-byte callback, independently checked against ISO. Six zero/five allocation/
five secondary/six cache/four mix vectors, ABI/callback/publication sites checked;
allocation/pan child pins reverified. Link audit passed927 Markdown files/zero
broken links; scoped diff check passed. Mathematical deductions separate from
original execution. Docs/metadata only; no make test/all rerun. Fresh full ELF/
listing/scoped generator stay /tmp; no bodies/assets committed. Concurrent native
work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback/deferred DVD manager and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive SDK/ARAM/AI/
DSP and original allocation-failure/cache/short-read/callback/startup/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header initial reads

Completed SoundSSPlayChEx_main initial-read region; checked EVT-01 portion.
[Initial reads](behavior/audio/GAME_STREAM_HEADER_INITIAL_READS_AND_PENDING_PUBLICATION.md)
pin cache gate, full unsigned chunk/strict32 advance, independent storage-path
reads/fresh post-primary type and request-before-count/pending. Whole callback
allocation/zeroing/cache/providers/original IO/audio/native binding open.

Validation: read-only ISO manifest passed six spans,189 anchors,six calls.
Twelve linked chunk/seven admission/four type/six pending vectors, four callback
bindings/publication sites checked; prefix/chunk pins reverified. Link audit
passed926 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELF/listing/scoped generator stay /tmp;
no bodies/assets committed. Concurrent native work excluded. Approved push
remains pending valid GitHub credentials.

Next: priority-zero EVT-01 same callback allocation/zeroing/cache continuation,
deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/ABI
and original initial-read/callback/type-change/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header matching/format prefix

Completed SoundSSPlayChEx_main matching/fields/format/reset prefix and exit ABI;
checked scoped EVT-01 portion and added explicit continuation todo.
[Header prefix](behavior/audio/GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md)
pins unmatched third-row/index3 fallthrough, raw mappings/type narrowing,
sentinel preserving prior200, formats/copy/reset. Whole allocation/initial IO/
cache continuation/providers/original header/playback/native binding open.

Validation: read-only ISO manifest passed two spans,185 anchors,zero calls.
Seven match/five sentinel/five format/five type vectors, twelve field mappings/
two coefficient copies/eighteen reset stores checked. Link audit passed923
Markdown files/zero broken links; scoped diff check passed. Mathematical source
deductions separate from original execution. Docs/metadata only; no make test/
all rerun. Fresh full callback ELF/listing/scoped generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 same callback allocation/initial-read/cache continuation,
then deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/
ABI domains and original header/cache/playback/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero public stream play wrappers

Completed SoundSSPlayCh/Ex/getMarioStDvdRoot; checked EVT-01 portion.
[Play wrappers](behavior/audio/GAME_STREAM_PLAY_WRAPPERS_REPLACE_SLOT_AND_HEADER_READ.md)
pin raw caching argument, replace-before-validation, slot reservation/failure
retention, ordered defaults/shared path/two opens/deferred header request.
Header callback/providers/manager/original playback/native binding remain open.

Validation: read-only ISO manifest passed15 spans,182 anchors,twelve calls;
23 ordered writes/header binding and six slot/four caching-outcome/five raw-
caching vectors checked; stop pins reverified. Link audit passed922 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELFs/listings/generator stay /tmp; no bodies/assets committed. Concurrent
native edits excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSPlayChEx_main header callback, deferred DVD
manager/providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains,
transitive SDK/ARAM/AI/DSP and original replacement/slot-exhaustion/open-failure/
header/cache/playback/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream main

Completed SoundSSMain; checked EVT-01 portion.
[Main fades/stops](behavior/audio/GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md)
pins initial stop priority, sequential40/20/10 mode processing, zero-before-
subtraction, recaptured rows/three mix/two stop blocks and counter/close-free/
marker order. Providers/FP/original cadence/fade/stop/audio/native binding open.

Validation: read-only ISO manifest passed eight spans,398 anchors,21 calls.
Thirteen finite stable-frame/six stop-gate/five counter vectors plus three mix/
two stop groups checked; pan/stop pins reverified. Link audit passed921 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELF/listing/generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 game stream play/deferred DVD manager and actual
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive
SDK/ARAM/AI/DSP and original cadence/fade/stop/mix/audio traces. Other inbox/
native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream cache callbacks

Completed _ssDVDReadAsync_cache_next/cache_aram/DVDMgrGetLength; EVT-01 checked.
[Cache callbacks](behavior/audio/GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md)
pin fixed32768 transfer despite short logical count, serial/low8 owner, ordered
cursor/progress, fresh length/continuation, resets/type and initial playback IO.
Providers/SDK/original cache/DMA/startup/audio/native binding remain open.

Validation: read-only ISO manifest passed nine spans,294 anchors,eleven calls.
Six owner/four next/seven continuation/six branch/six index/twelve linked chunk
vectors and six callback bindings checked; interrupt/completion pins reverified.
Link audit passed920 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh ELFs/listings/generator stay /tmp; no bodies/assets
committed. Concurrent native changes excluded. Approved push remains pending
valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/deferred DVD manager and actual
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains, transitive
SDK/cache/ARAM/AI/DSP and original cache/error/short-read/DMA/callback/startup/
audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream completion callback

Completed _ssDVDReadAsync_activeChk; checked EVT-01 portion.
[Completion callback](behavior/audio/GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md)
pins first-match/negative exit/requested count/terminal flags/latch, independent
ready-row scan, activation-before-inline mix-before-old flag8 and counter order.
Providers/SDK/FP/original callback/IRQ/audio/native binding remain open.

Validation: read-only ISO manifest passed seven spans,262 anchors,seven calls.
Eight completion/four match/seven result/eight ready/five mix-gate/five counter
vectors checked; linked pan manifest reverified. Link audit passed919 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELF/listing/generator stay /tmp; no bodies/binary assets committed.
Concurrent native edits excluded; approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/cache-next/cache-ARAM callbacks,
deferred DVD manager and actual providers/value/extent/alias/lifetime/IRQ/
concurrency/ABI/FP domains, transitive SDK/ARAM/AI/DSP and original callback/
short-read/error/startup/IRQ/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream interrupt engine

Completed SoundSSMainInt/DVDMgrReadAsync/cache_flush; checked EVT-01 portion.
[Interrupt engine](behavior/audio/GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md)
pins three-row gates, unsigned chunk/strict32 advance, storage-path and fresh
buffer reads, request-before-pending, companion copy/shared latch and deferred
DVD/ARQ callback bridge. Callback/manager/providers/SDK/original IRQ/IO/audio/
native binding remain open.

Validation: read-only ISO manifest passed six spans,302 anchors,seven calls.
Twelve chunk/ten stable gate/six latch/six copy vectors and two callback bindings
checked. Link audit passed918 Markdown files/zero broken links; scoped diff
check passed. Mathematical source deductions separate from original execution.
Docs/metadata only; no make test/all rerun. Fresh ELFs/listings/generator stay
/tmp; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/three _ssDVDReadAsync callbacks,
deferred DVD manager consumers and actual providers/value/extent/alias/lifetime/
IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/AI/DSP and original IRQ/
IO/callback/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero game stream volume

Completed SoundSSSetVolCh; checked EVT-01 portion.
[Volume setter](behavior/audio/GAME_STREAM_VOLUME_CACHE_AND_REQUESTS.md)
pins low8 float10-before-floatc, fresh old/new gates, pool recapture and ordered
mono/dual requests using new-row data. Providers/FP/original volume/audio/native
binding remain open.

Validation: read-only ISO manifest passed four spans,102 anchors,three calls.
Six cache/eight gate/seven finite split/four type vectors and pool annotations
checked. Link audit passed917 Markdown files/zero broken links; scoped diff
check passed. Mathematical deductions separate from original execution. Docs/
metadata only; no make test/all rerun. Fresh ELF/listing/generator stay /tmp;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials; local commits retained.

Next: priority-zero EVT-01 game stream engine/play/callbacks and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/FP domains, transitive ARAM/AI/DSP and
original volume/mono/dual/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream pan/surround

Completed SoundSSSetPanCh/SoundSSSetSrndPanCh; checked EVT-01 portion.
[Pan/surround](behavior/audio/GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md)
pins low8 halfword caches, raw mono pan, divisor63 split, every non-one type's
ordered dual requests and old/new pool rows in surround setter. Providers/FP/
original pan/surround/audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,185 anchors,six calls.
Seven finite split/four type/five cache vectors and pool bindings checked.
Link audit passed916 Markdown files/zero broken links; scoped diff check
passed. Mathematical deductions separate from original execution. Docs/metadata
only; no make test/all rerun. Fresh ELFs/listings/generator stay /tmp; no bodies/
binary assets committed. Concurrent native edits excluded. Explicitly approved
push of all commits attempted again; GitHub rejected username/token. Local
commits retained; push requires valid credentials.

Next: priority-zero EVT-01 game stream volume/engine/play/callbacks and actual
provider/value/extent/alias/lifetime/IRQ/concurrency/FP domains, transitive ARAM/
AI/DSP and original mono/dual/pan/surround/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero game stream frequency

Completed SoundSSSetPlayFreqCh; checked EVT-01 portion.
[Game frequency](behavior/audio/GAME_STREAM_FREQUENCY_CACHE_AND_REQUESTS.md)
pins halfword-before-fresh-flag8, shared counter, primary/conditional secondary
post-child frequency/type rereads and restore order. Engine/providers/IRQ domains/
original frequency/audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,53 anchors,four calls.
Six narrowing/seven gate/four fresh-request/six counter vectors checked. Link
audit passed914 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical source deductions are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials.

Next: priority-zero EVT-01 game stream pan/volume/engine/play/callbacks and actual
channel/base/flag/frequency/type/ID/counter/saved-state providers and valid
value/extent/alias/lifetime/IRQ/concurrency domains, transitive ARAM/AI/DSP and
original frequency/dual-stream/IRQ/audio traces. Remaining macro/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Original Goomba4 ordinary talk with explicit quest knowledge

Committed91a091e: [native known-state talk](behavior/field/NATIVE_GOOMBA4_ORDINARY_TALK.md)
retains all11 source-order ordinary kuribo4_talk message rules, including exact
164/175/176 and multi-suffix boundaries. Original US message text/control tokens
and source speaker me use the existing actor/anchor/paper presentation; no
ordinary story/quest write or fabricated fallback is added. Saved state now
retains optional GSW1420/GSF5405/GSF5406 knowledge. Both source quest arms must
be excluded by known byte/flag values before ordinary talk becomes available.
Legacy unknown remains unavailable and retains source candidacy/A suppression;
possible item-count arms remain unbound rather than treating native mushroom
quantity as original key-item storage. Guard scope is exact gor_02/Goomba4
profile2b0a9f83, independent from reusable character art.

Explicit fresh-file opening applies recovered swInit zero to those three fields;
room rebuilds retain them. Native40-byte checkpointv7 packs value/knowledge bits
into the existing native quantity word, validates reserved/value-without-known
bits and quantity range, and preserves knownfalse. Wholly unknown state still
writesv6; v1..6 loads clear any prior quest knowledge. This is a partial native
saved-switch adapter, not the complete original switch/inventory/card format.
Fixture tool --quest/--flag5405/--flag5406 supplies explicit development state,
with omission remaining unknown. No product quest guesses or implementation UI.
Original model/message assets are reused; catalog provenance/licensing updated.

Documentation agent recovered [full quest/item-window graph](behavior/field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md)
and OW-INTERACT-01 breadcrumb from read-only ISO, including original reset
banks, fixed95 table, dynamic107 table, check/remove wrappers and item-window
entry/poll/delete/key ownership. Item selection is not message Yes/No selection.
Both quest arms and their original item services remain next native work.
Independent review found no blocking binding/save defect and requested source
boundary/legacy clearing tests; those are included.

Validation: final shared `make test` passes151 CTest/103 Python,
`/tmp/goomba-talk-shared-test-complete.log`. Isolated snapshot
`/tmp/pm-goomba-talk-fzkn9v87` passes150 CTest/103 Python,
`/tmp/goomba-talk-isolated-test.log`. Tests cover all ordinary thresholds,
unknown/knownfalse/quest2/13 guard combinations, original raw US message and
return without story write, v7 optional-value roundtrips/reserved corruption,
quantity overlap rejection and explicit v1..6 clearing of known prior state.
Shared/isolated `make all` pass without warnings/errors:
`/tmp/goomba-talk-shared-native-complete.log`,
`/tmp/goomba-talk-isolated-native.log`. Intermediate shared attempts hit a
concurrent reward narrowing/type mismatch and full /tmp; final runs supersede
those failures. Applied one-line std::min<int32_t> ARM portability fix inside
concurrent uncommitted trio reward work, preserving its feature scope for its
owner. Removed only obsolete build outputs/duplicated romfs packs from earlier
owned live-wander/static snapshots to recover /tmp space; their source snapshots
and final project artifacts remain. Future snapshots should use ignored project
artifacts storage to avoid the16GiB /tmp limit.

Final isolated binary `artifacts/ttyd-goomba-ordinary-final.3dsx` passes corrected
field-ready `tests/azahar_static_npc_talk.py` with both explicit quest0 and legacy
unknown fixtures. Captures/profiles `artifacts/azahar-goomba-ordinary-final` and
`artifacts/azahar-goomba-unknown-final`; logs
`/tmp/goomba-ordinary-emulator-final.log` and
`/tmp/goomba-unknown-emulator-final.log`. Actual original Goomba field/window/
return inspected. Known ordinary talk opens original window without onset save;
unknown opens no substitute window; return admits save with room2/story8,
coordinates, exact knowledge bits and checksum unchanged. Earlier shared native
binary also passed original talk at `artifacts/azahar-goomba-ordinary`.
Software captures show60fps field/return and53fps window; not controlled New2DS
hardware, dense-area benchmark or matched original-runtime fidelity evidence.
Link audit passes912 Markdown/zero broken links; whitespace and script checks
pass. 2a5ac1a removes accidentally staged concurrent EXP ledger subsection from
feature history while preserving its working entry. ISO/assets/packs/captures
remain local/ignored; user emulator profile/saves untouched.

Next: bind original item-window tables and native item selection, key-item
count/removal/revise owners, both quest graphs and full switch/pouch persistence;
then extend original actor/event/object profiles and additional areas, complete
scheduler/shared RNG/clock and lifetimes, and measure Dolphin/New2DS rendering/
performance. The full requested goal remains active; early Rogueport is a slice.

## 2026-10-13 — Inbox priority-zero game stream fade/stop

Completed SoundSSFadeoutCh/StopCh/SoundSetFadeTime/DVDMgrClose; checked EVT-01
portion. [Fade/stop/close](behavior/audio/GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md)
pins signed GP/duration/minimum step, nested counter wrap/fresh decrement,
flags-before-deferred-DVD/stream-free and global invalidation order. Consumers/
GP/counter/IO/provider/FP domains/original stop/audio/native binding remain open.

Validation: read-only ISO manifest passed nine spans,117 anchors,six calls.
Seven finite fade/six counter/five stop/three time-set/four deferred-close vectors
checked. Link audit passed913 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 game stream engines/play/frequency/pan/volume and
GP/time/counter/marker/base/flags/table/handle/stream-ID/type providers/deferred
DVD owner and valid value/extent/alias/lifetime/IRQ/concurrency/FP domains,
original fade/stop/IO/audio traces. Remaining decoder/DVD/DMA/macro/song/backend,
lower-priority inbox/full native/game work remain open.

## 2026-10-13 — Inbox priority-zero game stream leaves

Completed SoundSSMuteOn/Off/Check/GetVolCh/ContinueCh; checked EVT-01 portion.
[Game stream leaves](behavior/audio/GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md)
pins raw channel/flags/zero gates, mute-off zeroing, signed volume conversion
and clear/copy/fresh-OR continue order. Engine/writers/callbacks/provider/FP
domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,59 whole-body anchors,
zero calls. Seven flag/four fresh-continue/seven volume/five address vectors
checked; invalid FP/conversion outputs unresolved. Link audit passed911 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELFs/listings/generator stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/Int/play/fade/stop/frequency/pan/volume
writers and producer/decoder/DVD/DMA callbacks, actual channel/base/flags/float/
index/IRQ/providers and valid extent/value/alias/lifetime/concurrency/FP/
conversion domains, original mute/check/continue/fade/audio traces. Remaining
macro/song/backend, lower-priority inbox/full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream mix/local inventory

Completed sndStreamMixParameter/ABI helpers and audited all17 tagged stream.c
local functions; checked EVT-01 portion.
[Mix/local inventory](behavior/audio/MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md)
pins AuxB0/three publication routes/fresh state2/retained raw argument recursion/
IRQ order. All17 local tagged functions answered; provider domains/transitive
ARAM/DSP/original audio/native binding remain open.

Validation: new read-only ISO manifest passed15 spans,361 anchors,16 calls.
Six byte/mode/gain vectors/fifteen stable chain models checked. All17 CSV-tagged
functions matched name/address/size to full fingerprints and complete/composed
contracts;16 inventory/prerequisite manifests reverified. Link audit passed910
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source/inventory checks are separate from original execution and full runtime
coverage. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Previously approved push remains pending
invalid GitHub credentials.

Next: priority-zero EVT-01 actual stream caller/state/ID/parameter/mode/voice/
companion/IRQ/resource providers and valid value/index/extent/encoding/alias/
lifetime/concurrency/stack/ABI/termination/FP domains, transitive ARAM/DSP and
original startup/refill/mix/frequency/free/chained failure/audio traces.
Remaining macro/song/backend, lower-priority inbox/full native/game work open.

## 2026-10-13 — Inbox priority-zero stream frequency chain

Completed sndStreamFrq/ABI helpers; checked EVT-01 portion.
[Frequency](behavior/audio/MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md)
pins raw frequency-before-fresh-state2, reference/voice reads, exact single
multiply-before-divide/signed conversion, retained-frequency companions/recursion
and four-stage enables. Mix/FP/chain/IRQ/providers/original audio remain open.

Validation: read-only ISO manifest passed11 spans,422 anchors,16 calls. Eight
finite/exception-domain pitch vectors/fifteen stable chain models checked for
writes/state2 requests/partial progress/recursion/IRQ counts. Link audit passed907
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution; invalid FP/conversion
outputs unresolved. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original ELF/listing/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream mix and actual frequency/reference/ID/state/
voice/companion/IRQ/providers and valid value/index/extent/alias/lifetime/
concurrency/stack/ABI/termination/FP/conversion domains, transitive ARAM/DSP
consumers and original frequency/chained partial-update/audio traces. Remaining
macro/song/backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream free/reverse clear

Completed sndStreamFree/ABI helpers; checked EVT-01 portion.
[Free/reverse clear](behavior/audio/MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md)
pins five stages, deactivate-before-fresh-selector/link, parent buffer exits
before descendants and deepest-first retained-slot state0/enables. Frequency/
mix/resource/chain/IRQ/provider domains/original free/audio/native binding open.

Validation: read-only ISO manifest passed ten spans,453 anchors,24 calls.
Fifteen finite-chain models checked for stable deactivation/exit/recursion/reverse
clear/missing links/retained fields/request counts. Link audit passed903 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream frequency/mix, actual ID/state/voice/buffer/
companion/IRQ/resource providers and valid value/index/extent/alias/lifetime/
concurrency/stack/ABI/termination domains, transitive ARAM/DSP consumers and
original chained free/deactivation/failure/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream deactivation chain

Completed sndStreamDeactivate whole local body; checked EVT-01 portion.
[Deactivation](behavior/audio/MUSYX_STREAM_DEACTIVATION_AND_COMPANION_CHAIN.md)
pins five IDs before recursion, fresh state1/2 unblock-before3, retained buffer/
ID/voice/cursor, partial progress and exact enables. Free/lifecycle/chain/IRQ/
provider domains/original deactivation/audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,453 whole-body anchors,
seventeen calls. Five state/fifteen finite acyclic-chain models checked for
lookup/gates/unblock/state/retained fields/missing links/recursion/IRQ counts.
Link audit passed902 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical/source checks are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 stream free/frequency/mix and actual ID/state/voice/
companion/IRQ/providers and valid value/index/extent/alias/lifetime/concurrency/
stack/ABI/termination domains, transitive ARAM/DSP consumers and original
chained deactivation/reactivation/failure/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero activation companions/composition

Completed sndStreamActivate companion continuation and composed whole local
body; checked EVT-01 portion.
[Companions/composition](behavior/audio/MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md)
pins four stages before recursion, fresh companion/partial voice/state writes,
recursive return propagation and exact enables. Local source answered; chain/
IRQ/provider domains/original activation/audio/native binding remain open.

Validation: new read-only ISO manifest passed six spans,272 anchors,fourteen
calls. Prior primary manifest reverified; disjoint union392+14 external ABI
words,total406/658hex,and all body anchors covered. Fifteen finite acyclic-chain
models checked for success/failure/missing links/partial writes/recursion/counts.
Link audit passed899 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical/source checks are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 stream free/deactivate/frequency/mix and companion
writers/acyclicity/ID/state/priority/voice/IRQ/providers and valid index/extent/
value/alias/lifetime/concurrency/stack/ABI domains, original chained activation/
failure/audio traces. Remaining macro/song/backend, lower-priority inbox/full game.

## 2026-10-13 — Source trio EXP icons, point-get art and counting

Documentation-only [EXP presentation contract](behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
pins original08..0b service/order/fallthrough, Fog/StockExp, realnineteenicon
state/asset/framewindow gates, originalpoint-get phrase/digit texture mappings,
effect lifetime/easing and actualvictorybank poses. CompleteDispMain does not
copy Bf24→E0. Goombella type43 category69→PKR_S_1 corrected separatelyfrom
animation missing-name fallback. Inbox source deliveries/dependencies updated.

Validation: complete read-only local DOL listings/table reads, original
icon404/effectUS TPL conversion/sourcepixel inspection and git diff --check.
No native edits, full build rerun or commit. Native precedingDropMaterial
validation belongs to its implementation entry. Next: sourceEXP icon/effect/
counting providers and originalpose imports; exactinnerGPU/window/iconclock
numeric/rasterparity and laterreward/export/field return remain unfinished.

## 2026-10-13 — Inbox priority-zero stream activation primary

Completed sndStreamActivate primary lookup/voice/state/terminal-return regions
and ABI; checked EVT-01 portion.
[Primary activation](behavior/audio/MUSYX_STREAM_ACTIVATION_PRIMARY_ADMISSION.md)
pins64-row lookup/fresh state3, voice publication before failure, cursor/state1
stores and companion-sentinel return. Companion continuation/providers/IRQ/
domains/original activation/audio/native binding remain open.

Validation: read-only ISO manifest passed six spans,134 anchors,four calls.
Five lookup/seven primary/four slot vectors checked. Link audit passed897 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 sndStreamActivate companion continuation/recursive
requests/enables/return composition; actual state/ID/priority/voice/companion/IRQ
providers and valid value/index/extent/alias/lifetime/concurrency/ABI domains,
stream deactivation/free/frequency/mix and original activation/failure/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Original stationary candidacy and simulation paper facing

Committed eee81fa: [stationary NPC binding](behavior/field/NATIVE_STATIC_NPC_CANDIDACY.md)
adds portable visited-candidate transitions and binds original Chu3/Darkly
first-admission turn, retained yaw, skipped-gate ownership and next-pass null
interrupt consumption. The documentation agent recovered original
npcCheckInterrupt and verified REL raw null words plus absent relocations for
both exact source setup rows. Static loss posts secondary10000000; the next
flags1/2-admitted NPC pass preserves only secondary00000f00 and clears
state00001000 without restoring yaw or creating an event. Source profile
admission is scoped to room2 and exact area/name IDs525f654d/e4c10e3d,
so future areas reusing their character models do not inherit null scripts.
Full interrupt/event metadata/providers remain future work.

Native static NPC instances now own original yaw, message-driven pose clocks
and paper Main/Inner preparation in simulation; the renderer reads their
prepared matrix/pose clock without advancing retained facing on repeat draws.
Scripted Frankly retains his event owner. Original artwork, source poses,
message IDs/control tokens/attribution and room model batching through
citro3d/tex3ds are retained. Talk start respects source no-turn flags and
publishes its facing to the live instance. The shared candidate helper retains
existing Goomba stop/resume callbacks and source no-turn/restore distinctions.
Independent review caught angle-table rounding to360; candidate/talk headings
now normalize through source reviseAngle with a boundary regression. No new
story dialogue, branch fallback, story writes or replacement art/audio.
No asset conversion/licensing change; existing local asset ledger applies.

Validation: final shared `make test` passes141 CTest/103 Python,
`/tmp/npc-static-candidate-shared-test-latest.log`. Isolated source snapshot
`/tmp/pm-static-candidate-7a5rc5ru` with only owned implementation overlays
passes138 CTest/103 Python, `/tmp/npc-static-candidate-isolated-test-final.log`.
Tests distinguish first/repeated candidacy, wandering restore/start, static
loss/request/next-pass consumption, no-turn actors and normalized near-Z
heading. Original-data Darkly fixture settles the camera before approach,
then verifies saved/retained yaw, skipped keys, local loss and later consumption.
Original talk and existing Goomba/side-collision checks remain passing.
Shared and isolated `make all` pass without warnings/errors, logs
`/tmp/npc-static-candidate-shared-native-final.log` and
`/tmp/npc-static-candidate-isolated-native-final.log`. Intermediate shared
runs caught incomplete concurrent battle build registrations, missing pouch
test and a battle-drop memory assertion; the final141-test run supersedes them.
The isolated overlay excludes concurrent pouch edits. Link audit passes896
Markdown files/zero broken links; whitespace and emulator-script compilation pass.

Final isolated binary `artifacts/ttyd-static-candidate-final.3dsx` passes
`tests/azahar_static_npc_talk.py` in separate Azahar/Xvfb profiles/captures
`artifacts/azahar-static-candidate-chu-verified` and
`artifacts/azahar-static-candidate-darkly-verified`; logs
`/tmp/npc-static-candidate-chu-emulator-verified.log` and
`/tmp/npc-static-candidate-darkly-emulator-verified.log`. Corrected script
requires field HUD plus shaded Rogueport geometry before talk; Darkly requires
original contact approach. Window onset performs no save; return admits save,
retaining room2/story8, player contact bounds and checksum. Inspected actual
original actors, field and speaker-anchored windows. Earlier fixed-delay script
mistook title artwork for a window; those earlier green results are discarded.
Captures show60fps exploration and51..58fps dialogue with two simultaneous
software-emulator instances; these are smoke observations, not controlled
hardware/dense-area benchmarks or matched original-runtime visual parity.
ISO is read-only; binary packs/captures/profiles remain ignored, and user profile
and saves are untouched. Concurrent battle/pouch/audio work is preserved.

Next: original Goomba4 quest/switch/item-table/window binding, general actor
script metadata and interrupt replacement/regular lifetime, complete ordinary
talk return/cadence, original shared RNG/clock, additional original area actors/
events/objects and matched Dolphin/New2DS rendering/performance measurements.
The full requested remake goal remains active; this is an early Rogueport slice.

## 2026-10-13 — Inbox priority-zero stream allocation/length

Completed sndStreamAllocEx/sndStreamAllocLength/ABI helpers; checked EVT-01
portion. [Allocation/length](behavior/audio/MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md)
pins first free/collision ID, wrapped sizing, partial record/coefficients/mode/
filter/state3-before-buffer/deferred activation/state-only failure and IRQ order.
Child lifecycle/providers/domains/original allocation/audio/native binding open.

Validation: read-only ISO manifest passed11 spans,349 anchors,eight calls.
Thirteen length/six ID/four slot/five format/six completion vectors and16 copy
rows checked. Link audit passed896 Markdown files/zero broken links; scoped
`git diff --check` passed. Mathematical source deductions are separate from
original execution. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream activation/deactivation/free/frequency/mix,
actual callers/flags/count/ID/state/length/source/callback/voice/default/resource
providers and valid extent/value/encoding/alias/lifetime/IRQ/concurrency/ABI
domains, transitive ARAM/DSP consumers and original allocation/failure/stream/
audio traces. Remaining macro/song/backend, lower-priority inbox/full game open.

## 2026-10-13 — Inbox priority-zero stream footer/composition

Completed streamHandle footer/loop-PS/ABI and composed whole local source body;
checked EVT-01 portion. [Footer/composition](behavior/audio/MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md)
pins fresh gates/raw translated read/loop byte/helper fallthrough order. Whole
local handler answered; lifecycle/providers/domains/transitive ARAM/DSP/original
stream/audio/native binding remain open.

Validation: new read-only ISO manifest passed seven spans,44 anchors,three calls.
Four prior region manifests and ABI helper prerequisite reverified. Six disjoint
regions cover558 semantic+11 ABI words,total569/8e4hex. Seven gate/five source/
four leaf vectors and region composition checked. Link audit passed895 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution; address vectors do not prove valid
loads. Documentation/metadata only; no `make test`/`make all` rerun. Fresh full
original handler/leaf ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream allocation/activation/deactivation/free/frequency/
mix lifecycle; actual producer/delay/count/state/format/descriptor/position/cursor/
length/flags/source/selector/voice/loop-PS providers and valid value/divisor/extent/
alignment/encoding/address-map/cache/alias/lifetime/IRQ/concurrency/ABI/FP domains,
transitive ARAM/DSP consumers and original startup/refill/stream/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Source trio drop material and entry pouch prerequisites

Documentation-only [drop/entry-pouch contract](behavior/battle/UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md)
closes complete source DropMaterial HP/FP/item draws, actual used tables,
equipment queries, retained fields and field consumer gates. Current unit
resources own ratios; real import/default FP5 is required, with no divide-zero
fallback or entry heal. Sourceinitial-stage equipment6/0x32/9 mutation and
pouchrevision/import pinned; ordinary title/newgame/boots6 lifecycle remains
unclosed, and native suppliedentrypouch remains a separate adapter. I14 stock
coin/S14 flower distinction corrected; inbox DMG/EVT/JUMP history updated.

Validation: complete read-only DOL body/data/REL event inspections, source
hash witnesses and git diff --check. No native code edits, build rerun or commit.
Next: bind DropMaterial over actual packed unit/setup/pouch and source RNG,
then original ending/reward/export/field continuation. Persistent Game/save
pouch owner and scene-specific drop admission/collection remain unfinished.

## 2026-10-13 — Inbox priority-zero stream nonzero-position refill

Completed streamHandle nonzero-position wrapped refill; checked EVT-01 portion.
[Nonzero wrap](behavior/audio/MUSYX_STREAM_NONZERO_POSITION_WRAPPED_REFILL.md)
pins two-span callback, strict returned-count split, down-versus-up block sizes,
zero-length second post, fresh source reads and uploads-before-cursor publication.
Footer/ABI/providers/domains/original refill/audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,152 region anchors,eight
calls; two indirect callback sites recorded. Six gate/fourteen ordered-refill
vectors checked, including equality/mutation/split/zero-size/bypass/invalid length
unresolved. Link audit passed894 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh full
original handler ELF/listing/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle common footer/ABI and whole-body
composition, actual callback/position/format/cursor/length/flags/source/selector/
voice/count providers and valid divisor/value/extent/alignment/encoding/alias/
lifetime/IRQ/concurrency domains, original wrapped-refill/stream/audio traces.
Remaining stream lifecycle/macro/song/backend, lower-priority inbox/full game open.

## 2026-10-13 — Inbox priority-zero stream zero-position refill

Completed streamHandle zero-position wrapped refill; checked EVT-01 portion.
[Zero-position refill](behavior/audio/MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md)
pins callback gates/fresh modulo, byte-versus-logical length distinction, retained
block offset and flush-before-cursor order. Nonzero wrap/footer/ABI/providers/
domains/original refill/audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,113 region anchors,four
calls; two indirect callback sites recorded. Six dispatch/five callback/nine
refill vectors checked, including mutation/length distinction/bypass/invalid
length unresolved. Link audit passed892 Markdown files/zero broken links;
scoped `git diff --check` passed. Mathematical source deductions are separate
from original execution. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh full original handler ELF/listing/generator stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded. Previously approved
push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle nonzero-position wrap/footer/ABI and
callback/position/format/cursor/logical-versus-byte-length/flags/source/selector/
voice/count providers and valid divisor/value/extent/alignment/encoding/alias/
lifetime/IRQ/concurrency domains, original refill/stream/audio traces. Remaining
stream lifecycle/macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Inbox priority-zero stream forward refill

Completed streamHandle position/forward refill region; checked EVT-01 portion.
[Forward refill](behavior/audio/MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md)
pins /14 position/block mapping, callback return/state gate, fresh modulo versus
retained pre-callback offset and flush-before-cursor publication. Wrapped refill/
footer/ABI/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,126 region anchors,five
calls; two indirect callback sites recorded. Nine block/seven route/six callback/
nine refill vectors and65540 division cases checked. Link audit passed890 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution; invalid divisor output unresolved.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
handler ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Previously approved push remains pending
invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle wrapped-refill/footer/ABI, actual callback/
position/format/cursor/length/flags/source/selector/voice/count providers and valid
value/divisor/extent/alignment/encoding/alias/lifetime/IRQ/concurrency domains,
transitive ARAM/DSP consumers and original refill/stream/audio traces. Remaining
stream lifecycle/macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Inbox priority-zero stream startup

Completed streamHandle throttle/dispatch/state1 startup/row tail regions; checked
EVT-01 portion. [Startup](behavior/audio/MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md)
pins partial descriptor fields, ordered filter/pitch/mix/start, state2-before-
optional-flush and fresh count. Refill/ABI/providers/domains/original audio open.

Validation: read-only ISO manifest passed14 spans,151 region anchors,nine calls.
Five throttle/five state/four format/five finite pitch/four flush vectors and
exact descriptor/argument bindings checked. Link audit passed888 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle state2 refill/ABI, actual delay/count/state/
format/descriptor/voice/frequency/filter/start/flags/source providers and valid
extent/value/alias/lifetime/IRQ/concurrency/FP domains, transitive DSP/ARAM/hardware
consumers and original startup/refill/stream/audio traces. Remaining stream
lifecycle/macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero output-mode propagation

Completed SoundSetOutputMode/sndOutputMode/streamOutputModeChanged/hwDisableHRTF;
checked EVT-01 portion. [Mode propagation](behavior/audio/MUSYX_STREAM_OUTPUT_MODE_PROPAGATION.md)
pins game write-before-request, public flags/HRTF/fresh change gate, voice dirty
before stream, requested/effective mode/state3 skip and live count/inline mix/IRQ
order. UI/providers/domains/stream engine/original audio/native binding open.

Validation: read-only ISO manifest passed nine spans,152 whole-body anchors,
eight calls. Eight public-mode/eight state/four count/six game-gate vectors
checked. Link audit passed881 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 streamHandle/AllocEx/Free/Activate/Deactivate/Frq/
MixParameter and actual game/UI/preference/flags/count/base/HRTF/state/voice/
resource providers and valid extent/value/alias/lifetime/IRQ/concurrency domains,
transitive DSP/hardware consumers and original mode/stream/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Live original Goomba4, source pause ownership and NDSP footsteps

Committed live [Goomba4/wandering binding](behavior/field/NATIVE_LIVE_NPC_WANDER.md)
in acf058b. PMNPC008 retains original source talk-event presence separately
from playback, adds188-byte typed wander/pose/sound metadata and preserves
original tagged relative registry order. Source init(-370,0,130),direction270,
radius70/speed30,30/30 dimensions and40480603 regular flags remain exact.
Actor state owns retained work, previous root, previous NPCdt/contact, yaw,
automatic S_1/W_1/R_1 phase and simulation-owned paper presentation. Rendering
reads its live matrix/pose clock without a second facing update; model resources
still stage/batch through tex3ds/citro3d. Source fresh zero-vertical/no-gravity
profile takes the proven no-ground-probe path rather than inventing a floor ray.

Regular USER_FUNC executes before Mario using prior NPC publications; NPC-main
publishes current interval, applies direct displacement, clears components,
responds to Mario, chooses pose from actual speed and snapshots. Candidate
admission is independent of native quest playback and strict nearest victory;
fully admitted wandering actors pause even when not nearest. Stop/resume
retains work/countdown; skipped global/actor gates do not resume. Yaw turns
only on first candidacy, saves/restores source yaw without heading mutation.
Unknown nearest quest playback cannot redirect A to farther Chu/Darkly.
Original Goomba quest/flags/items remain unavailable: A jumping is suppressed,
no substitute dialogue/story write is generated, and a labeled development
notice explains missing playback. Source/native contracts and inbox retain
that boundary. Native seed1 and simulation60Hz clock remain explicit adapters
pending full shared seed/scheduler/clock binding.

Original c_kuribo_n descriptor0Z_1/S_1/T_1/W_1/R_1 uses14 cells,
1024x256RGBA8/UV4,1MiB nominal texture payload. Original step names request
source primary samples993/994 through libctru NDSP channels8/9; decoded
16000Hz monoPCM16 lengths540/672 and tablevolume45/127 are retained, with the
existing0.65 master gain. Named cue lookup uses the loaded original clip table.
Original name/mask/root requests remain inspectable; full macro/child voices,
material-mask routing and spatial attenuation remain adapters. No generated
replacement art/tones. ISO/raw resources/packs/captures remain ignored.

Validation: final shared `make test` passes134 CTest/103 Python at
`/tmp/live-wander-shared-test-complete.log`; isolated snapshot
`/tmp/pm-live-wander-4wc3cmxh` passes133 CTest/103 Python at
`/tmp/live-wander-isolated-test-complete.log`. Shared and isolated `make all`
pass without warnings/errors, logs `/tmp/live-wander-shared-native-complete.log`
and `/tmp/live-wander-isolated-native-complete.log`. Original-data tests verify
freshdt0/prior-publication movement, bounded1600tick walking, run/idle/sound
requests, retained pause/resume and skipped gates, first-turn retention, room
invalidation and unsupported-nearest/no-farther-dialogue with real message
providers. Parser tests reject incomplete moving profiles/malformed v8 data;
older formats and object support remain covered. Intermediate shared links
raced library rebuilds; final complete shared/isolated runs are the validation.
Independent source/runtime/render review caught repeated candidacy turning and
playback-winner mismatch; both corrected and regression-tested.

Final binary `artifacts/ttyd-live-wander-final.3dsx` passes isolated Azahar/Xvfb
`tests/azahar_live_wander.py` with goomba/goomba-observe fixtures from the fixture
tool. Profiles/captures `artifacts/azahar-live-wander-pause` and
`artifacts/azahar-live-wander-observe`; logs `/tmp/live-wander-emulator-pause.log`
and `/tmp/live-wander-emulator-observe-final.log`. Strict original-hat visibility/
motion assertion finds centers357.32,357.29,377.38,395.35 across captures;
stationary player, room2/story8, save admission/checksum and unavailable-quest
no-window assertion pass. Inspected original moving/paused art and development
notice; captures run60fps in software emulator (observed sim~1.0ms,CPU~7.0ms,
GPU~0.5ms). This is not New2DS hardware speed, full audio-runtime fidelity or
matched original-runtime pixel/trajectory measurement. Link audit passes867
Markdown files/zero broken links; whitespace passes. f371744 removes an
accidentally duplicated asset-ledger chapter, preserving the original foreign
entry. Concurrent battle/opening/audio work excluded from feature commits.

Next: bind Goomba4 quest/switch/item-table/window services before ordinary talk;
complete shared RNG/clock/event-category scheduling and all-story lifetimes;
static-NPC candidate turn/secondaryflag140 ownership; additional original area
NPC/events/objects and full sound/spatial/shadow/backend services. Compare
original traces and dense-area New2DS performance. Early Rogueport remains an
incremental remake slice; the full requested goal is still active.

## 2026-10-13 — Inbox priority-zero stream lookup/output mode/mix

Completed GetPrivateIndex/CheckOutputMode/SetupVolume/SetupVolumeAndPan/SetHWMix;
checked EVT-01 portion. [Lookup/mode/mix](behavior/audio/MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md)
pins64-row first-active-ID scan, requested/effective pan writes, exact unclamped
byte scaling and fixed studio0 hardware request. Stream engine/lifecycle/
providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed eight spans,157 whole-body anchors,
one call. Five lookup/64 individual-slot/six mode/four finite gain vectors checked.
Link audit passed880 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical source deductions are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 streamHandle/AllocEx/Free/Activate/Deactivate/Frq/
MixParameter/output-mode ownership, actual ID/state/voice/volume/mode/resource
providers and valid extent/value/alias/lifetime/IRQ/concurrency domains, transitive
DSP/hardware consumers and original stream/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Source trio victory controller and field-return ownership

Documentation-only [ending contract](behavior/battle/UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md)
pins all32 original end-state entries, distinct field/NPC owners, initial
result/audience/Koura/status/barrier order, complete audience target adjustment
float ABI and original coefficient addresses, drop/EXP/victory/level/export/
fade/publication/field continuations. Per-ID status loop retains Charge and
skips25..27; no blanket clear or invented full-resource recovery. Source
MarioM_V_2/GoombellaPKR_Y_1 scripts and fresh field callback initialization
identified; broader callback producers remain bounded requests. JUMP/DMG/EVT
inbox append deliveries record remaining dependencies.

Validation: read-only DOL/table/REL/event inventory; git diff --check passed.
No native edits/build rerun/commit. The implementation owner's next increment
is00→05→06 to actual DropMaterial801141bc; its validation is reported separately.
Full reward/pouch/display/fade/teardown and original scene continuation remain
unfinished native work; this does not complete the prologue or remake.

## 2026-10-13 — Inbox priority-zero low-precision final pitch/composition

Completed LowPrecisionHandler output/requeue/ABI helpers and composed whole
local source body; checked EVT-01 portion.
[Final pitch/composition](behavior/audio/MUSYX_LOW_PRECISION_FINAL_PITCH_AND_COMPOSITION.md)
pins narrow lookup/fraction/wrapped addends/scaling, half212 before hardware,
requeue and complete GPR helper fallthrough. Whole local handler answered;
providers/domains/transitive consumers/original voice/audio/native binding open.

Validation: new read-only ISO manifest passed11 spans,53 anchors,seven calls.
Four prior region manifests reverified. Eight disjoint regions cover454 semantic
words plus11 anchored ABI words,total465/744hex. Six lookup/six doppler vectors
and bit-extraction/composition checks passed. Link audit passed879 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 actual voice/note/fraction/sample/ADSR/periodic/
controller/index/job providers and valid value/extent/alias/lifetime/IRQ/
concurrency/stack/ABI domains, transitive hardware/DSP services and original
pitch/voice/audio traces. Remaining macro/stream/song/backend, lower-priority
inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision portamento

Completed LowPrecisionHandler controller transitions/interpolation; checked
EVT-01 portion. [Portamento](behavior/audio/MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md)
pins unchanged-controller exception, strict threshold, raw init/flag/cache order,
wrapped signed step versus unsigned endpoint and elapsed-only completion.
Remaining handler/providers/domains/original glide/audio/native binding open.

Validation: read-only ISO manifest passed three spans,115 whole-region anchors,
one call. Twelve transition/eight interpolation vectors checked, including
threshold/ff/mode/zero-divisor/overshoot/wrap. Link audit passed877 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler final ADSR/pitch/scaling/hardware/
requeue/ABI and body composition; actual channel/set/controller/cache/mode/flags/
duration/current/time/note/voice providers and valid index/extent/value/alias/
lifetime/IRQ/concurrency domains, original glide/pitch/audio traces. Remaining
macro/stream/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero low-precision pitch preparation

Completed LowPrecisionHandler note/cents/bend/modulation region; checked EVT-01
portion. [Pitch preparation](behavior/audio/MUSYX_LOW_PRECISION_PITCH_PREPARATION.md)
pins retained note, signed base versus unsigned modulation cents, cached/query/
ff-skip bend and fresh scale flag/wrapped product/signed shift order.
Remaining handler/providers/domains/original pitch/audio/native binding open.

Validation: read-only ISO manifest passed four spans,96 whole-region anchors,
two calls. Seven base/six gate/five bend/six modulation vectors and512 byte
/100 cases checked. Link audit passed873 Markdown files/zero broken links;
scoped `git diff --check` passed. Mathematical source deductions are separate
from original execution. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh full original handler ELF/listing/generator stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded. Previously approved
push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler portamento/controller transition,
final pitch/ADSR/scaling/hardware/requeue/ABI, actual note/cents/cache/range/factor/
depth/oscillator/flag/controller/voice providers and valid value/index/extent/
alias/lifetime/IRQ/concurrency domains, original pitch/audio traces. Remaining
macro/stream/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero integer sine lookup

Completed sndSin/original1024 numeric coefficients/two oscillator bindings;
checked EVT-01 portion. [Integer sine](behavior/audio/MUSYX_SINE_QUARTER_WAVE_LOOKUP.md)
pins low12 phase,1023 mirror/narrowed negation, bounded indices and source
peak4095. Runtime table/caller domains/remaining handler/original audio remain open.

Validation: read-only ISO manifest passed three spans,37 anchors (35 body/two
caller),two bindings. Eighteen boundary/high-bit vectors/all4096 phase checks and
numeric coefficients' big-endian serialization/hash passed. Link audit passed872
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution. Documentation/metadata
only; no `make test`/`make all` rerun. Fresh original ELF/listing/generator stay
`/tmp`; no binary bodies/table bytes/assets committed. Concurrent native edits
excluded. Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler pitch/controller/portamento/
requeue/ABI, actual table/phase/period/divisor/time/depth/dirty/flag/voice providers
and valid value/index/extent/alias/lifetime/IRQ/concurrency domains, original
oscillator/audio traces. Remaining macro/stream/song/backend, lower-priority inbox
and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision modulation/pan ramps

Completed LowPrecisionHandler oscillator/periodic/pan ramp region; checked
EVT-01 portion. [Modulation/pan ramps](behavior/audio/MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md)
pins wrapped unsigned phase requests, changed-half dirty OR, separately flagged
oscillator, distinct elapsed masks, periodic reset and raw-expiry versus clamped
continuing pan paths. Sine/handler remainder/providers/domains/runtime remain open.

Validation: read-only ISO manifest passed three spans,146 whole-region anchors,
two calls. Seven phase/five dirty/six periodic/eight pan-ramp source vectors
checked, including invalid-divisor outputs left unresolved. Link audit passed871
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution. Documentation/metadata
only; no `make test`/`make all` rerun. Fresh full original handler ELF/listing/
generator stay `/tmp`; no bodies/binary assets committed. Concurrent native edits
excluded. Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 sndSin and LowPrecisionHandler pitch/controller/
portamento/requeue/ABI, actual phase/period/rate/countdown/target/dirty/flag/time/
voice providers and valid index/extent/value/divisor/alias/lifetime/IRQ/concurrency
domains, original oscillator/pan/audio traces. Remaining macro/stream/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset finish/composition

Completed ZeroOffsetHandler filter/priority/requeue/dirty/ABI finish and composed
whole local body; checked EVT-01 portion.
[Finish/composition](behavior/audio/MUSYX_ZERO_OFFSET_FILTER_REQUEUE_AND_COMPOSITION.md)
pins wrapped unsigned filter delta/conversion, priority first store/signed clamp,
raw wrapped requeue delay and post-requeue dirty replacement. Whole local source
body answered; providers/domains/transitive consumers/original audio remain open.

Validation: new read-only ISO manifest passed13 spans,106 anchors,nine call sites.
All four prior region manifests reverified. Union audit covers461 semantic+31 ABI
words,total492/7b0hex,without gaps/overlap. Six filter-gate/six filter-math/six
priority/six requeue/three dirty vectors checked. Link audit passed870 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original handler ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials; no repeat
push attempted this turn.

Next: priority-zero EVT-01 LowPrecisionHandler remainder and actual voice/time/
gain/controller/filter/priority/dirty/index/source providers and valid value/
address/extent/alias/lifetime/IRQ/concurrency/ABI/FP/conversion domains, transitive
hardware/DSP/job consumers and original filter/requeue/voice/audio traces.
Remaining macro/stream/song/backend, lower-priority inbox and full native/game work.

## 2026-10-13 — Inbox priority-zero zero-offset volume/aux output

Completed ZeroOffsetHandler volume/aux output region; checked EVT-01 portion.
[Volume/aux output](behavior/audio/MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md)
pins dirty admission/six-call order, exact separate single main/pre/post/byte
terms, post-child fields and half454 publication before hardware ABI reads.
Filter/requeue/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed11 spans,97 whole-region anchors,six
calls. Eight admission/seven finite single-output vectors checked, including
zero main/nonzero AuxA and negative convertible snapshot. Link audit passed869
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source checks are separate from original execution. Documentation/metadata only;
no `make test`/`make all` rerun. Fresh full original handler ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials; no repeat
push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler filter/requeue/epilogue and whole
body composition, LowPrecisionHandler remainder, actual gain/controller/byte/
studio/dirty/voice providers and valid value/extent/alias/lifetime/IRQ/concurrency/
FP/conversion domains, original output/audio traces. Remaining macro/stream/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset pan/surround

Completed ZeroOffsetHandler pan/surround region; checked the EVT-01 portion.
[Pan/surround](behavior/audio/MUSYX_ZERO_OFFSET_PAN_AND_SURROUND.md) pins
captured/fresh mode, dirty admission, flag-before-request, post-child base reads,
wrapped signed pan versus unsigned surround clamps and fixed/clean indicator.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,82 whole-region instruction
anchors,two direct calls. Nine pan/eight surround/eight gate vectors and65536
low16 rotate-mask cases checked. Link audit passed868 Markdown files/zero broken
links; scoped `git diff --check` passed. Mathematical/source deductions are not
original execution. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original handler ELF/listing/generator remain `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded. Previously approved push
remains pending invalid GitHub credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler volume/aux/filter/output/hardware/
requeue/epilogue and LowPrecisionHandler remainder, actual mode/dirty/voice/base/
controller providers and valid value/extent/alias/lifetime/IRQ/concurrency domains,
original spatial/audio traces. Remaining macro/stream/song/backend, lower-priority
inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset gain/attenuation

Completed ZeroOffsetHandler live fader/track/base gain and modulation/tremolo
attenuation region; checked EVT-01 portion.
[Gain/attenuation](behavior/audio/MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md)
pins row21/22 selection, unsigned conversions, post-child halfword reads,
signed half transform, exact single constants/step/reload and update order.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed six spans,128 whole-region instruction
anchors,two direct calls. Five selection/five gain/eight attenuation finite model
vectors and65536 signed16 half-transform cases checked. Link audit passed867
Markdown files/zero broken links; scoped `git diff --check` passed. Source checks
and mathematical deductions are separate from original execution. Documentation/
metadata only; no `make test`/`make all` rerun. Fresh full original handler ELF/
listing/generator remain `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no authentication changes or repeat push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler pan/surround/aux/volume/hardware/
output/requeue and LowPrecisionHandler remainder, actual fader/track/voice/
controller providers and valid index/extent/value/alias/lifetime/IRQ/concurrency/
FP domains, original gain/audio traces. Remaining macro/stream/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Source Paragoomba wing-fall transition and next victory owner

Documentation-only [used wing-fall contract](behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins failed-H1 per-hit admission, original tracked damage child, exact timed
pose/data/part/descent/grounded-init sequence, original c_kuribo PTK→KUR art,
packed reset/preservation and event/sound setter ABI. ChangeKind preserves U10
and HP; source model/pose recreation and partattrs9 are required. Inbox JUMP-01
history/DMG-02 updated. Next last-enemy victory owner is original btlseqEnd bank7,
with FieldBattleInfo+3c callback or audience-end admission8021290c still requiring
native binding before source reward/field return can be claimed.

Validation: read-only US DOL leaf disassembly/tik REL inventories; scoped source
facts, hashes and relocated pointers recorded; git diff --check. No native edits,
build rerun, native completion claim or commit. Next: execute real fall child and
recreated grounded art, then close original end/reward/pouch/return graph.

## 2026-10-13 — Inbox priority-zero zero-offset admission/ramp

Completed ZeroOffsetHandler admission/timestamp/ramp region; checked the EVT-01
portion. [Admission/ramp](behavior/audio/MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md)
pins retained voice, low elapsed/shift, wrapped product before signed clamp,
fresh high-word gate clear, word158 publication and carried update indicator.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,79 whole-region anchors,
one direct call. Four admission/five elapsed/fourteen ramp source-operation vectors
checked, including equality, zero step/rate and signed wrap. Link audit passed
866 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
handler ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Approved push of all current commits attempted;
GitHub rejected configured credentials (invalid username/token). Push remains pending.

Next: priority-zero EVT-01 ZeroOffsetHandler remaining table/gain/controller/aux/
hardware/output/requeue, LowPrecisionHandler remainder, actual voice/time/rate/
target/flag/accumulator/IRQ/concurrency domains and original voice/audio traces;
remaining macro/stream/song/backend, lower-priority inbox and full native/game work.
Retry the authorized push once GitHub credentials work.

## 2026-10-13 — Inbox priority-zero cache syscall vector/installation

Completed SystemCallVector/__OSInitSystemCall/ICInvalidateRange/memcpy and
pinned direct OSInit call; checked the EVT-01 portion.
[Vector/installation](behavior/audio/GEKKO_CACHE_SYSCALL_VECTOR_AND_INSTALLATION.md)
pins28-byte source copy/256-byte cache lengths, temporary SPR1008 bit8/isync/
sync/restore/rfi, IC barrier/range and original overlap byte direction. Actual
boot/vector residency/exception/cache/hardware/original DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (four bodies/two linked),67
whole-body/caller anchors/four direct calls. Four SPR/six finite overlap-copy/
six IC range-barrier vectors and copy source/end/destination/hash/cache lengths
checked. Link audit passed864 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh ELFs/listings stay `/tmp`; no vector/bodies/binary assets
committed. No live vector/processor/emulator state modified. Concurrent native
edits excluded.

Next: priority-zero EVT-01 complete boot/vector/handler residency/patch owners,
original exception/MSR/SRR/HID0/address/privilege/cache/IRQ/ordering/trap/coherency
domains, actual copy/caller ranges/alias/lifetime/DSP consumers and original
syscall/cache/voice/audio traces. Remaining precision/macro/stream/song/backend
work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero cache ranges/source corrections

Completed five DC cache-range bodies; checked the EVT-01 portion.
[Cache/source corrections](behavior/audio/GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
pins wrapped line count/unconditional first loop/opcodes/post-loop sc. Corrected
six earlier contracts/four linked-body pins: e0 DCStoreRangeNoSync,ab4
DCFlushRangeNoSync,a54 DCFlushRange,a28 DCInvalidateRange. Prior prose names
were wrong; historical entries remain intact and this correction supersedes
those attributions. Syscall/hardware/domain/original cache/DSP/audio remain open.

Validation: new read-only ISO manifest passed five spans/five complete bodies,
57 whole-body instruction anchors/zero direct calls. Eleven range/zero/threshold/
wrap vectors and8224 small-domain count checks passed. All ten composed-builder
prerequisite manifests reverified, including corrected child pins; source region/
call/address evidence unchanged. Link audit passed863 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original syscall vector/handler and cache/address/
IRQ/coherency/ordering/trap/ownership domains, actual caller range/alias/lifetime/
partial publication/DSP consumers and original cache/voice/audio traces. Remaining
precision/macro/stream/song/backend work, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero startup finish/composed DSP builder

Completed startup loop/fallback endpoints/filter/volume seeds/start-slot/active
publication and composed the local whole salBuildCommandList body; checked the
EVT-01 portion. [Startup finish/composition](behavior/audio/MUSYX_DSP_STARTUP_ENDPOINTS_AND_COMMON_FINISH.md)
pins raw endpoint/default-register behavior and exact field/mode/publication
order. Providers/ABI/value/alias/IRQ/concurrency/cache/DSP/original audio/full
native binding remain open; local source composition is not runtime fidelity.

Validation: new read-only ISO manifest passed two spans (region/whole body),286
anchors (275 region/11 ABI),zero calls within new region. All ten prior region
manifests reverified against ISO. Union audit covers all2419 non-ABI instruction
words, plus11 anchored ABI words,total2430/25f8hex. Eight loop/six fallback/six
initial-enable vectors,nine seed bindings/all256 start-byte predicates checked.
Link audit passed861 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
fresh builder ELF/listing/coverage generator stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 descriptor/address/count/offset/loop/type/zero-buffer/
filter/mode/gain/start-slot/register/flag/list/PB/callback providers and valid
extent/residency/encoding/index/overflow/alias/lifetime/ABI/IRQ/concurrency domains;
transitive cache/DSP/command consumers and original startup/endpoints/voice/audio
traces. Remaining precision/macro/stream/song/backend work, lower-priority inbox
and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero startup callback/ITD/format prefix

Completed builder startup callback/ITD/type0..6 descriptor/direct/rounded-seek/
cursor prefix; checked the EVT-01 portion.
[Startup format prefix](behavior/audio/MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md)
pins callback return versus fresh length gate, original copy/request order,
wrapped seek rounding and unsupported type's stale/incoming-register fallthrough.
Loop/endpoints/finish/provider/domain/original sample/DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (one region/five linked),256
instruction anchors/five calls. Nine dispatch/seven rounded-seek/six initial-
cursor/five callback-length vectors and sixteen descriptor-copy binding rows
checked. Link audit passed859 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh builder ELF/listing stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 word8c/type-specific startup loop/endpoints and common
finish, actual type/callback/address/count/descriptor/offset/history/ITD/flag/
record/register-state providers and valid extent/residency/encoding/alias/lifetime/
IRQ/concurrency domains; cache/DSP consumers and original startup/seek/sample/
audio traces. Remaining precision work, lower-priority inbox/full native/full-game
remain open.

## 2026-10-13 — Inbox priority-zero existing voice endpoints/completion

Completed existing-voice type4/5/6 field/pending-buffer endpoint/completion
region; checked the EVT-01 portion.
[Existing endpoints/completion](behavior/audio/MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md)
pins captured/fresh type dispatch/latch/c2/raw wrapped address/count formulas and
independent unsigned progress/length/message-before-deactivate order. Startup/
provider/encoding/domain/original endpoint/DSP/audio execution remains open.

Validation: read-only ISO manifest passed four spans (one region/three linked),
72 instruction anchors/two calls. Eight endpoints/seven gates/seven completion
vectors and69638 unsigned high-multiply quotient/remainder14 cases checked;
algebra proves the quotient formula across unsigned32. Link audit passed857
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original fresh builder ELF/
listing stays `/tmp`; no bodies/binary assets committed. Concurrent native/staged
edits excluded.

Next: priority-zero EVT-01 startup/sample-format bodies, actual type/latch/PB.c2/
bytea0/address/count/word78/word8c/progress/length/callback/record writers and
valid encoding/index/overflow/residency/alias/lifetime/IRQ/concurrency domains;
cache/DSP consumers and original endpoint/completion/voice/audio traces. Remaining
precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP budget/rejection/voice chain

Completed three initial/cost/rejection/PB-chain/command02/03 gate regions;
checked the EVT-01 portion.
[Budget/rejection/chain](behavior/audio/MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md)
pins original cost tables/wrapped formulas/unsigned limit and current/tail/future-
studio depop/deactivation/callback ordering, fresh pointer/captured successor
choices and admitted PB-chain publication. Startup/sample/provider/domain/
original performance/DSP/audio remain open; system-word fixtures unmeasured.

Validation: read-only ISO manifest passed ten spans (three regions/five linked/
two numeric tables),197 instruction anchors/ten calls plus initial comparison.
Seven initial-budget/eight mixer-index/six SRC-index/six rejection/three chain
vectors checked. Link audit passed856 Markdown files/zero broken links; scoped
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 startup/sample/voice-format regions, actual D/system
word/flags/mode/type/count/new-setup/result/table/array/list/PB/callback writers
and valid index/overflow/member/cycle/alias/lifetime/IRQ/concurrency domains;
cache/DSP consumers and original cost/rejection/chain/voice/audio traces.
Remaining precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP studio payloads/fade bindings

Completed ten studio/input/aux/mode/surround/compressor/voice-chain-tail/fade/
loop regions; checked the EVT-01 portion.
[Studio payloads/fades](behavior/audio/MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md)
pins exact fields/raw indices/fresh frame pairing/mode gates/modulo/nine fades/
cache/outer-loop order. Compressor table fingerprint is identity only; remaining
voice/budget/admission/domain/cache/DSP/original audio remain open.

Validation: read-only ISO manifest passed14 spans (ten regions/whole-builder/
fade/cache/table),323 instruction anchors/11 calls. Six auxiliary-index/five raw-
frame/seven mode/six surround-gate/one compressor-layout vectors, nine fade
bindings and all256 auxiliary-byte quotient/modulo cases checked. Link audit
passed854 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original fresh
builder ELF/listing stays `/tmp`; no bodies/binary assets committed. Compressor
table stored only as hash identity. Concurrent native edits excluded.

Next: priority-zero EVT-01 preceding command03 admission, full voice/sample/
budget/command paths, actual studio/input/count/reference/frame/aux/mode/buffer/
coefficient/block/alias/lifetime/IRQ/concurrency domains, cache/DSP/table consumers
and original studio/input/aux/surround/compressor/fade/command/audio traces.
Remaining precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP command rollover/finalization

Completed command-cell initialization/twelve rollover blocks/final command/cache
publication; checked the EVT-01 portion.
[Command rollover/finalization](behavior/audio/MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md)
pins unsigned fit/low16 aligned sizes, deferred previous-link size/flush, fresh
pointer publication and final0e/0f/cache order. Other payload/admission/budget/
domain/cache/DSP/original command/audio execution remain open.

Validation: read-only ISO manifest passed16 spans (fourteen regions/whole-builder/
cache child),533 instruction anchors/14 calls. Whole aligned body census found12
exact li0,0d rollover sites; twelve threshold/equality/wrap,seven size/three final-
command vectors checked. Link audit passed853 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 pending command payload/admission/budget/studio/voice
regions, actual pointer/cell/X/size/allocation/cache/DSP consumers and valid total
extent/wrap/alias/lifetime/IRQ/concurrency domains; original chunk/link/flush/
command/DSP/audio traces. Remaining precision work, lower-priority inbox and
full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP sorter/collection

Completed SortVoices and builder list collection/reverse iteration regions;
checked the EVT-01 portion.
[DSP sorting/collection](behavior/audio/MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md)
pins unsigned keys/wrapped midpoint/pivot stores/exact recursive order/unstable
ties, unchecked64-slot array and reverse consumption. Actual list/priority/index/
alias domains, budget/admission/original DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (sorter/two regions/three
linked),180 instruction anchors/seven calls plus reverse-load binding. Eight
identity/order/six midpoint vectors and3280 source-order mathematical model
cases checked for unsigned ascending/permutation invariants, lengths0..7/three
key values. Link audit passed852 Markdown files/zero broken links; scoped `git
diff --check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native/staged edits excluded.

Next: priority-zero EVT-01 actual priority/list/count/index/record/array writers,
valid signed range/overflow/member/cycle/duplicate/alias/lifetime/IRQ/concurrency
domains, surrounding builder budget/admission/cache/commands and original
collection/sort/voice/DSP/audio traces. Remaining precision work, lower-priority
inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero depop/fade/studio prepass

Completed HandleDepopVoice/DoDepopFade and builder studio two-list prepass;
checked the EVT-01 portion.
[Depop/fade/prepass](behavior/audio/MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md)
pins nine accumulator gates, wrapped-then-clamped addition/residual fade and
captured versus post-child successor/callback/sentinel/active/break/root order.
Fade callers/list providers/domain/original depop/DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (two bodies/one region/three
linked),291 whole-body/region instruction anchors/four calls. Seven accumulation/
clamp,thirteen fade-boundary/residual,six admission/sentinel/deactivation vectors
and nine field bindings checked. Link audit passed850 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 fade callers/studio output/cache/DSP consumers,
surrounding builder admission/sorting/command/budget paths, actual accumulator/
PB sample/gate/latch/active/break/sentinel/root/link providers and valid studio/
list/member/cycle/index/value/alias/lifetime/IRQ/concurrency domains; original
list/depop/fade/callback/DSP/audio traces. Remaining precision work, lower-priority
inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero paired sound cosine/root helpers

Completed sndCos/sndSqrt; checked the EVT-01 portion.
[Paired cosine/root helpers](behavior/audio/MUSYX_PAIRED_COSINE_AND_SQRT_HELPERS.md)
pins original constants/paired reduction/signed conversion/Gray bits/fused
polynomial and estimate-before-zero/three double refinements. GQR/FPSCR/estimate/
caller-domain/original math/filter/audio execution remain open.

Validation: read-only ISO manifest passed five spans (two bodies/three numeric),
74 whole-body instruction anchors/zero calls. Nine ordinary phase/eight odd-Gray/
five synthetic-seed/five zero-gate vectors checked. Paired/scalar lane semantics
cross-checked against Dolphin's primary interpreter source (links in contract);
no emulator/original instruction execution claimed. Link audit passed849 Markdown
files/zero broken links; scoped `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Full original fresh ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original GQR0/FPSCR/scalar-paired lane/estimate/
rounding/trap/NaN/denormal execution, actual angle/frequency/coefficient/caller
value domains, filter/DSP command consumers and original math/filter/audio traces.
Remaining DSP/precision work, lower-priority inbox and full native/full-game work
remain open.

## 2026-10-13 — Original wandering-NPC driver core and lifecycle evidence

Implemented the [portable wander core](behavior/field/NATIVE_NPC_WANDER_CORE.md)
in commita8d09d6, compiled for host/3DS. It retains original44-byte work,
four phases, live frame conversion, RNG ordering/multiple20 countdown,
separate paper-facing readiness calls, strict radius/contact conditions,
minimum displacement cap, preserved components on early exits, idle yaw flips
without heading writes and exact sound threshold/toggle/FMA requests. Source
LCG and wrapped frame helpers consume caller-owned seed/rate, not an invented
private original trajectory. Original work allocation, event lifetime, roots,
NPC-main component consumption and platform providers remain separate owners.

The inbox yielded [full original driver/Goomba4 evidence](behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md):
initial XYZ(-370,0,130) overrides tagged placement; radius70/speed30; initflags
40080603 become40480603 through regular initialization. Source corrections
confirmed min(speed*dt,20*speed/1000), idle speed threshold0.1 and distinct
main00400000 versus auto-pose00000400. Fresh Goomba vertical0/no gravity takes
npcMoveG's zero exit before floor probes or Y writes. NPCdt begins0 and source
clock anchor begins current time. Regular event executes before Mario/NPC,
consuming prior dt/contact/Mario touch. Candidacy stops regular events beforeA,
including eligible non-nearest actors; candidate loss/resume preserves work,
phase/countdown rather than rerunning init. Full sound tail and original quest
branches before ordinary story text are recorded. Unknown quest state cannot
be treated as quest0 or replaced by generic dialogue.

Prepared original c_kuribo_n Z_1/S_1/T_1/W_1/R_1 locally through the existing
read-only converter. Ignored field-goomba manifest records14 cells,
1024x256RGBA8/UV4; raw/provenance/compiled resources stay ignored. Asset ledger
records paths, conversion and Nintendo/Intelligent Systems rights. This bank
is not yet registered as a live moving actor, and no stationary Goomba substitute
was added. The native core contract/inbox explicitly retains this boundary.

Validation: final shared `make test` passes130 CTest/103 Python at
`/tmp/npc-wander-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-wander-6f1t0umc` passes129 CTest/101 Python at
`/tmp/npc-wander-isolated-test-final.log`. Targeted core checks pass at
`/tmp/npc-wander-target-final.log`; final suite adds mixed readiness/three-versus-
four-query order, signed-byte sound toggles and cap equality. Shared/isolated
`make all` pass at `/tmp/npc-wander-shared-native.log` and
`/tmp/npc-wander-isolated-native.log`. Isolated build has no warnings/errors;
shared build has an unrelated concurrent trio_attack.cpp:365 indentation
warning. Concurrent battle/audio changes excluded from this scoped commit.
Independent source/disassembly/core review passed; suggested meaningful
ordering/byte/interface coverage was added. Link audit passes848 Markdown files,
zero broken links; whitespace passes. Static deductions are not Dolphin
observations; core not activated, so no new emulator/gameplay/hardware claim.
ISO read-only, no original executable bodies/assets/dialogue in source history.

Next concrete native step: instantiate source regular work per actor, run its
USER_FUNC before Mario with prior published dt/touch/map contact, apply current
NPC-main direct-horizontal/zero-vertical/side-response/automatic-pose services,
and expose live root/yaw/pose to existing rendering. Preserve original stopped
regular work on candidate/message transitions. Bind Goomba4 quest/flags/items
or explicitly leave that event unavailable; do not publish an assumed ordinary
message. Then use original bank/step-sound resources and verify native walking,
radius reversal, contact, pause/resume and rendering/performance with isolated
fixtures and matched original observations. Shared clock/RNG/backend/FPSCR,
all-story lifetime and broader event/NPC/object/area work remain open. The full
goal remains active; this is an incremental Rogueport foundation, not completion.

## 2026-10-13 — Inbox priority-zero filter coefficients/DSP parameters

Completed hwSetFilter/hwLowPassFrqToCoef and the builder's five parameter-copy
gates; checked the EVT-01 portion.
[Filter/DSP parameters](behavior/audio/MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md)
pins exact low-byte1 mode admission/raw active-byte preservation, unsigned/single
math child/conversion/output order and fresh ITD/SRC/polyphase/filter gates.
Math/FPSCR/domain/whole builder/command/original filter/DSP/audio remain open.

Validation: read-only ISO manifest passed13 spans (two bodies/three linked/one
region/seven numeric literals),80 anchors/two calls. Nine setter-state/seven
consumer-gate/five input-scaling/six synthetic final-clamp vectors checked. Link
audit passed847 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded.

Next: priority-zero EVT-01 sndCos/sndSqrt/source math/FPSCR/transitive execution,
actual frequency/mode/coefficient/flag/record/PB/type/value/alias/lifetime/IRQ/
concurrency domains, surrounding builder setup/cache/command consumers and
original filter/parameter/DSP/audio traces. Remaining DSP/precision work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP volume-ramp consumers

Completed salBuildCommandList three-triplet prepare/correct/mask region;
checked the EVT-01 portion. [DSP ramp consumers](behavior/audio/MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md)
pins nine component bindings/dirty versus residual paths/fresh level-status
flag decisions/studio suppression/cross-triplet C dependency on B2. Remaining
builder/provider/domain/command execution and original ramp/DSP/audio stay open.

Validation: read-only ISO manifest passed four spans (one region/whole-builder/
two helpers),422 anchors/22 calls. Nine dirty-pair/nine residual-pair/four alternate-
C flag vectors, six instruction-decoded low-half clear masks/nine component
binding rows checked. Link audit passed846 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 surrounding DSP builder admission/list/budget/sample/
setup/cache/command regions, actual desired/host/PB flag/mode/mask/index/record/
alias/lifetime/IRQ/concurrency domains and original ramp/mask/command/DSP/audio
traces. Remaining precision work, lower-priority inbox and full native/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero IEEE mixer remainder

Completed __ieee754_fmod used by the mixer phase requests; checked the EVT-01
portion. [Original IEEE remainder](behavior/audio/IEEE_REMAINDER_USED_BY_MUSYX.md)
pins exceptional gate/double multiply-self-divide, finite exponent/significand
normalization, subtract/shift loop, normal/subnormal reconstruction and signed
zero. Original FPSCR/NaN/trap/runtime/mixer/DSP/audio evidence remains open.

Validation: read-only ISO manifest passed two spans (complete body/numeric signed-
zero table),94 anchors/zero calls. Twelve finite bit-pattern/seven exceptional-
gate vectors checked; deterministic4096 finite exact integer-rational versus
host math.fmod bit-pattern crosschecks passed. These are mathematical deductions,
not original instruction execution or exception/FPSCR experiments. Link audit
passed843 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELF/listing/crosscheck generator stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original FPSCR/rounding/trap/NaN/denormal execution,
actual gain/coordinate/index/table/output/alias/lifetime/IRQ/concurrency domains,
DSP ramp/command consumers and original mixer/audio traces. Remaining DSP/
precision work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero mixer interpolation/output branches

Completed both salCalcVolume output regions and composed the whole local
mixer body with the prior preparation contract; checked the EVT-01 portion.
[Interpolation/outputs](behavior/audio/MUSYX_VOLUME_INTERPOLATION_AND_OUTPUT_BRANCHES.md)
pins original tables, exact single interpolation/products/nine-store order,
nonzero mode's ignored third gain/zero slots and cross-table extra pan pairing.
IEEE math/FP/index/alias/runtime domains, DSP consumers/original audio remain open.

Validation: read-only ISO manifest passed16 spans (whole body/two regions/four
linked/four numeric tables/five literals),337 anchors/19 calls. Eight binary32
finite/nonalias formula vectors checked, including paired E changes;266 original
volume/pan floats retained. Link audit passed842 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 IEEE remainder/math/FPSCR/transitive execution,
actual gain/coordinate/index/table/output/alias/lifetime/IRQ/concurrency domains,
DSP ramp/command consumers and original mixer/volume/DSP/audio traces. Remaining
DSP/precision work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero mixer coordinate/phase preparation

Completed salCalcVolume coordinate/table/sentinel/compression/phase region and
fmod wrapper; checked the EVT-01 portion.
[Coordinate/phase preparation](behavior/audio/MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md)
pins low-byte table selection/raw sentinel/unsigned floor/single conversion,
optional pre-compression phases and common child-request order. Both table/output
branches and IEEE remainder/math/domain/original mixer/audio remain open.

Validation: read-only ISO manifest passed13 spans (one region/one wrapper/three
linked/two numeric tables/six literals),108 anchors/13 calls. Seven raw-coordinate/
sentinel/carry,six binary32 compression and six table-selection vectors checked;
258 original table floats retained. Link audit passed841 Markdown files/zero
broken links; scoped `git diff --check` passed. Documentation/metadata only; no
`make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 both mixer interpolation/output branches, original
pan-table/literal providers, IEEE remainder/math/FPSCR/transitive execution,
actual coordinate/volume/table/output/alias/lifetime/IRQ/concurrency domains and
original mixer/volume/DSP/audio traces. Remaining DSP/precision work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Original Darkly and reusable per-actor talk rules

Added source Darklyクラガリさん to the east-side early0..55 slice in
commit3b08537. Read-only tagged/REL/DOL evidence retains XYZ(-335,0,-335),
direction270,30/30 dimensions,flags0x40080603, drawmask1 and original
c_f_mura descriptor0Z_1/idleS_1/talkT_1.
[Source contract](behavior/field/ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md)
pins original static setup/null regular, lifetime census, asset provenance
and complete later talk thresholds/quest exception. Goomba4 was investigated
but requires original wandering/quest owners; no stationary replacement added.

[Native binding](behavior/field/NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md)
uses existing actor matrix/camera-facing, source pose/message clocks, dimensions,
collision and talk-ray services without a renderer character branch. Original
US gor_02_034_00 bytes,wait/key tokens and speaker attribution remain unchanged;
no story write. Darkly's later175 flag/name child remains unbound. Source
thresholds differ from Chu, so PMNPC007 adds per-actor source-order/full-ID
rules instead of reusing one global table. Current Chu keeps all nine original
ordinary requests; Darkly binds onlyLESS56. Empty/unmatched bindings do not
fallback or suppress A jump. Generic room prefixes and model metadata support
future area resources without model-index or gor_-only exporter coupling.
Legacy1..6 remain readable; staged parser rejects malformed counts/comparisons/
IDs/room namespaces/truncation. Rule tests include equality, greater/equal,
unconditional and duplicate first-match cases, plus future tik_04 namespace.

Original a/c_f_mura/a/c_f_mura- resources convert locally to13 wrap-aware124px
texture cells,1024x256RGBA8 atlas (1MiB),UV4 and original sparse hierarchy/
geometry/pass/cull. No generated replacement visuals/audio/dialogue. Source
positions and scalar2 remain separate from *.01 native mapping. Asset ledger
and documentation inbox record source paths/conversion/rights and remaining
quest/lifetime domains. Packs, manifests, raw assets and captures are ignored.

Validation: shared `make test` passes126 CTest/101 Python,
`/tmp/darkly-shared-test.log`; isolated snapshot `/tmp/pm-darkly-si2xzi8u`
passes125 CTest/101 Python, `/tmp/darkly-isolated-test.log`. Shared/isolated
`make all` pass without warnings/errors at `/tmp/darkly-shared-native.log`
and `/tmp/darkly-isolated-native.log`. Snapshot excludes foreign working edits;
count difference is concurrent test coverage outside that snapshot. Original
asset tests verify model/descriptor/idle/talk, metadata, contact and unchanged
message/story through closure. Initial side fixture used the wrong local stick
for the alley's source camera90; corrected approach uses right/world+Z and
retains original geometry/placement. Independent parser/model/source review
passed; its ordering/unknown-talk-admission followups were implemented.

Isolated Azahar/Xvfb `tests/azahar_darkly.py` with fixture from
`tools/make_field_actor_fixture.py --actor darkly` passes source contact at
native(-3.350000,-3.625000), visible original message, onset-save suppression,
return/save, unchanged room2/story8 and checksum. Binary `artifacts/ttyd-darkly.3dsx`,
profile/captures `artifacts/azahar-darkly`, log `/tmp/darkly-emulator.log`.
Inspected original model/room/contact/message/return captures; contact capture
shows60fps,simulation3.23ms/CPU6.7ms/GPU0.5ms in software emulator. This is not
New2DS hardware speed or matched original-runtime pixel proof. Link audit
passes839 Markdown files/zero broken links; whitespace passes. ISO read-only;
no assets/original prose committed, foreign battle/audio work preserved.

Next: original Goomba4 urouro/quest and other per-area source creation/event
owners; Darkly's later175 switch/name child and remaining lifetimes; original
NPC timer/movement/registry/auto-turn and full interaction/encounter composition.
Expand source rules only for verified one-print branches; complex events need
original state/service graphs. Continue inbox documentation/best-effort binding,
matched original trajectories/rendering and dense-area New2DS measurements.
The deliverable remains an early Rogueport slice, not a finished full remake.

## 2026-10-13 — Inbox priority-zero hardware pitch/volume/DSP ramps

Completed hwSetPitch/hwSetVolume/sal_setup_dspvol/salCheckVolErrorAndResetDelta;
checked the EVT-01 portion. [Pitch/volume/DSP ramps](behavior/audio/MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md)
pins duplicate/event-slot/dirty publication, float triplets/ITD table, signed
coarse division and residual correction/mask/reset order. Mixer internals,
DSP ramp callers/consumers and valid index/FP/alias/runtime domains remain open;
source constants do not establish measured cadence or audible playback.

Validation: read-only ISO manifest passed eight spans (four bodies/one linked/
two float literals/one numeric table),121 anchors/one call. Seven pitch/six
ramp/eleven correction/four triplet-gate/five ITD-index vectors and all131071
signed divide160 differences checked;128 ITD halves retained. Link audit passed840
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original inspection ELFs/
listings stay `/tmp`; no bodies/binary assets committed. Concurrent native edits
excluded.

Next: priority-zero EVT-01 full salCalcVolume/math/table providers, DSP ramp
callers/command consumers, actual pitch/pan/float/flag/marker/record/studio/time/
mask writers and valid index/FP/alias/lifetime/IRQ/concurrency domains; original
volume/pitch/ramp/DSP/audio traces. Remaining precision/builder work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP initialization/activation

Completed salInitDspCtrl/salActivateStudio/salInitHRTFBuffer/salActivateVoice/
hwStart/hwKeyOff; checked the EVT-01 portion.
[DSP initialization/activation](behavior/audio/MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md)
pins allocation/partial failure/cache/record/studio bindings, unchecked per-voice
buffers, fresh count/base reads, head insertion/start time byte/indexed key-off.
Allocator/cache/DSP children and valid domain/original init/start/audio remain
open; source local requests do not prove successful audible playback.

Validation: read-only ISO manifest passed15 spans (six bodies/nine linked),305
anchors/33 calls. Six count/size,six activation/root,four indexed-event vectors,
eight studio slice offsets/seven explicit failure-stage cases checked. Link audit
passed839 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded.

Next: priority-zero EVT-01 allocator/ARAM/cache transitive behavior, full DSP
builder/consumer, actual count/base/time/studio/record/link/descriptor providers,
valid static extent/index/null/value/alias/partial-init/reinit/lifetime/IRQ/
concurrency domains and original init/allocation/start/key-off/DSP/audio traces.
Remaining precision-handler sections, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP ADSR setup/command loop

Completed four salBuildCommandList setup/start/envelope-command regions and
three host-position/message/deactivation leaves; checked the EVT-01 portion.
[DSP ADSR loop](behavior/audio/MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md)
pins all seven local ADSR call sites, release precedence/mask/delta commands,
previous-result/repeated callback completion, flush range and retained playback
position/unlink order. Other builder/provider/consumer/domain/cadence/original
voice/DSP/audio execution remains open.

Validation: read-only ISO manifest passed14 spans (three bodies/four regions/
six linked/nine-halfword numeric table),309 anchors/18 calls and alternate index
source. Six loop/eight release-precedence/five delta/five host-position vectors
checked. Link audit passed838 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 DSP builder sample/mixer/budget/mask/index/record/
setup producers and branch admission, callback registration/transitive services,
cache/DSP command consumers, valid studio/list/stack/type/value/alias/lifetime/
IRQ/concurrency domains and original voice/envelope/command/audio traces.
Remaining precision-handler sections, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision ADSR caller/census

Completed four LowPrecisionHandler admission/clock/envelope/pitch/dirty
regions and the six-service DOL direct-call census; checked the EVT-01 portion.
[Caller/census](behavior/audio/MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md)
pins retained voice/elapsed low, stack outputs/status-bit clearing and independent
fresh descriptor-level pitch contribution. Fourteen direct calls identified;
seven DSP builder sites remain semantic work. Remaining precision/DSP sections,
indirect/REL/domain/cadence/original envelope/audio stay open.

Validation: read-only ISO manifest passed15 spans (four regions/nine linked
bodies/two nonempty DOL text sections),80 anchors/15 calls. Fresh opcode18/LK/
AA/displacement scan of all seven text slots found14 exact calls to six entries;
five elapsed/five flag/five signed pitch vectors checked. Link audit passed836
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original inspection ELFs/
listings/census generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded.

Next: priority-zero EVT-01 DSP builder envelope setup/release/update/command
regions, remaining precision modulation/controller/portamento/pitch/requeue,
actual descriptor/flag/depth/time providers and valid value/alias/stack/IRQ/
concurrency/lifetime domains, indirect/REL callers and original voice/DSP/audio
traces. Lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero ADSR advancement/wrapper

Completed adsrHandle/adsrHandleLowPrecision for EVT-01; checked the inbox
portion. [Advancement/output order](behavior/audio/MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md)
pins two domains, pre-update level publication/fresh delta, count wrap/exact-zero
transition and fifteen-call output reuse/early return. Caller cadence/value/index/
alias/DSP/original envelope/audio remain open; no measured timing inferred.

Validation: read-only ISO manifest passed four spans (two complete bodies/one
linked state-change/one numeric table),83 anchors/three calls. Ten quantization,
five linear/count,seven log-index/five wrapper formula vectors checked;194 table
halves retained. Link audit passed834 Markdown files/zero broken links; scoped
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Authorized push of all committed
changes failed: GitHub rejected configured username/token; no push succeeded.

Next: priority-zero EVT-01 actual envelope callers/cadence and type/phase/count/
level/log/step/selector writers, valid table/value/alias/lifetime/IRQ/concurrency
domains, macro/hardware/DSP consumers and original output/audio traces. Other
opcodes, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Original NPC side collision and accelerated wall queries

Implemented [native ordinary NPC side collision](behavior/field/NATIVE_NPC_SIDE_COLLISION.md)
in commits74a28cf/c398f21. Portable source response preserves original
sphere/body admission, compAngle one-adjustment ties, finite binary32 source
polynomials, previous-XZ rollback/recheck, retained-travel40-degree branch,
source amplification/cap and required wall retry. PMNPC006 retains original
Chu0x80603/Frankly0x40080603 flags and source dimensions. Scripted creation,
explicit placement generations, continuous movement, flag OR/CLEAR ownership
and draw hiding remain distinct. Current contact/root/flags feed talk/render;
source8000 snap binding now reaches presentation100. Source Around saves the
uncapped first correction and admits negative/unordered second dot. Road case
checks follow NPC ownership. Source-derived inbox evidence appended to
[OW-INTERACT-01](requests/OW-INTERACT-01.md); no new Dolphin observations.

Instances reset by room/catalog generation; native mechanics remain independent
of3DS services. An initial unconditional engine/native position rescale caused
exact-coordinate tests to fail; corrected to publish MarioXZ only after actual
contact. Tests now cover original Chu walking contact, Professor collision
mask clears, invisible active bodies, deletion, explicit placement versus
continuous rollback and rejected-slot history. Original US message content,
IDs/control tokens and story state remain unchanged.

A warmed emulator run exposed contact slowdown: around retry visited4,816
static grid candidates. Cached expanded XYZ bounds now reject unreachable
triangles before the unchanged narrow phase, preserving source slot order,
filtering, cull, nearest/equal-hit and dynamic-object policy. HEAD serialized
prefix remains48 bytes; derived bounds stage/swap with geometry. gor_02 adds
82,800 cache bytes for3,450 triangles. Native margin is an acceleration adapter,
not measured original behavior. Five-room full-scan parity covers800 horizontal/
vertical/oblique queries plus16 contact rays and boundary/unsupported cases.

Validation: final shared `make test` passes126 CTest/101 Python at
`/tmp/npc-ray-bounds-shared-test.log`; isolated snapshot
`/tmp/pm-npc-collision-u6mshs4d` passes123 CTest/101 Python at
`/tmp/npc-ray-bounds-isolated-test.log`. Shared and isolated `make all` pass,
no warnings/errors, logs `/tmp/npc-ray-bounds-shared-native-final.log` and
`/tmp/npc-ray-bounds-isolated-native-final.log`. Independent numerical/history/
rendering/cache review found no blocker. Link audit passes833 Markdown files,
zero broken links; scoped whitespace checks pass. Difference in test counts
reflects concurrent additions outside the isolated source snapshot.

Isolated Azahar/Xvfb `tests/azahar_npc_collision.py` with original story8 native
fixture passes contact position(1.225000,-4.000000), unchanged room2/story8,
checksum, visible original message, onset save suppression and return/save.
Final binary `artifacts/ttyd-npc-collision-bounds.3dsx`, test-only profile/captures
`artifacts/azahar-npc-collision`, log `/tmp/npc-ray-bounds-emulator.log`.
Inspected contact/US talk/return captures retain original art and message window.
Warm baseline before bounds showed contact7fps and simulation47.42ms; final
contact capture shows60fps and simulation3.70ms in the software emulator.
Baseline captures remain `artifacts/azahar-npc-collision-baseline`; this is not
New2DS hardware performance or a matched original-runtime pixel measurement.
ISO read-only; extracted resources/packs/binaries/profiles/captures ignored.
Concurrent battle/opening/audio edits excluded from these scoped commits.

Next: extend original per-area NPC/event/object creation from source bindings;
full NPC timer/speed/gravity/pose/regular-event and alternate registry ownership;
transformed Mario dimensions and remaining retained-heading writers; complete
encounter/object/door/pipe arbitration. Verify matched original trajectories,
near-degenerate Gekko/SDK/backend numerics and dense-area New2DS performance.
Continue documentation inbox/best-effort bindings and incremental commits.
The current deliverable remains an early Rogueport slice, not the full remake.

## 2026-10-13 — Inbox priority-zero ADSR transitions/release

Completed salChangeADSRState/adsrStartRelease/adsrRelease for EVT-01; checked
the corresponding inbox portion.
[ADSR transitions/release](behavior/audio/MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md)
pins zero-stage fallthrough/two domains/count/step/table/terminal and explicit/
stored release. Linear-zero preserves level; converted-zero clears level/log.
Advancement/actual type/index/divisor/FP/alias/caller/runtime domains remain open;
no common zero handling/table repair or measured cadence inferred.

Validation: read-only ISO manifest passed nine spans (three bodies/one linked/
five numeric data),217 anchors/two calls. Five linear/five log-count/five release/
seven index-formula vectors and domain/mask/width/table/order deductions checked.
Link audit passed833 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 envelope advancement and actual type/phase/count/
level/log/step/duration/sustain/selector/caller writers, valid table/index/divisor/
float/value/alias/IRQ/concurrency domains, macro/hardware/DSP consumers and
original transition/release/audio traces; other opcodes/full native binding.
Lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero resource/pitch ADSR commands

Completed mcmdSetADSR/mcmdSetPitchADSR/adsrSetup and opcodes12/32 for
EVT-01; checked the corresponding inbox portion.
[Resource/pitch ADSR](behavior/audio/MUSYX_RESOURCE_AND_PITCH_ADSR_COMMANDS.md)
pins LE layouts/sentinel coefficients/single-FP operation order, signed command
pitch adjustment, sustain/time/release/setup and post-child flags. Setup wrapper
is closed; state engine/pow/actual resource/index/FP/caller/runtime remain open.

Validation: read-only ISO manifest passed19 spans (three bodies/two regions/six
linked/seven data/dispatch table),164 anchors/12 calls/two registrations. Seven pitch/
five LE/four index/four sentinel vectors, all256 signed fractional divisions and
byte/mask/FP/field/order deductions checked. Link audit passed832 Markdown files/
zero broken links; scoped `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 salChangeADSRState/pow/transitive math, other opcodes,
actual resource/command/volume/base-note/coefficient/descriptor/envelope/flag/
caller writers, valid length/type/index/FP/value/alias/shared-cell/IRQ/concurrency
domains and original ADSR/pitch/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller ADSR/hardware layouts

Completed mcmdSetADSRFromCtrl/hwSetADSR/adsrConvertTimeCents and opcode22
for EVT-01; checked the corresponding inbox portion.
[Controller ADSR/layouts](behavior/audio/MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md)
pins four fresh queries/stack descriptor, three hardware layouts/unsupported
control-bit publication and signed time-cent/pow/FP conversion wrapper. Original
129/129/1024-entry tables retained; unchecked input indices can exceed extents.
Other ADSR/pow/actual caller/index/float/DSP/runtime remain open.

Validation: read-only ISO manifest passed16 spans (three bodies/one region/three
linked/eight numeric data),122 anchors/11 calls/one registration. Seven index/
seven mode/four sustain vectors and descriptor/mask/width/table/order deductions
checked. Link audit passed829 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 ordinary/pitch ADSR and remaining opcodes; pow/
transitive math, actual controller/command/table/descriptor/mode/ordinal/flag/
caller providers, valid index/float/value/alias/shared-cell/IRQ/concurrency domains,
hardware/envelope/DSP consumers and original ADSR/controller/audio traces.
Full native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro pan/volume/envelopes

Completed pan/surround/scale/envelope/fade/sndConvert2Ms/curve lookup/comparator
and opcodes13..15/20..21 for EVT-01; checked the corresponding inbox portion.
[Pan/volume/envelopes](behavior/audio/MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md)
pins raw versus converted pan duration,127 versus128 scale/ramp arithmetic,
curve interpolation/sentinel and signed step/publication. Pre/post-child source
captures retained; no unified divisor/post-curve clamp or immediate job inferred.
Actual command/divisor/value/curve/mix/DSP/runtime domains remain open.

Validation: read-only ISO manifest passed19 spans (eight bodies/five regions/
five linked/table),210 anchors/19 calls/five registrations. Five pan/six scale/
four target/four curve/four step vectors and division/mask/width/address/order
checked as deductions. Link audit passed828 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual command/rate/pan/volume/
curve/registry/flag/caller writers, valid raw-versus-converted divisor/value/
alias/shared-cell/IRQ/concurrency domains and envelope/mix/job/DSP consumers;
original pan/fade/audio traces. Full native/runtime binding, lower-priority inbox/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero random notes/pitch lookup

Completed mcmdRandomKey/sndGetPitch/sndPitchUpOne/unsigned conversion helper
and opcode23 for EVT-01; checked the corresponding inbox portion.
[Random notes/pitch](behavior/audio/MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md)
pins interval/cents/note RNG/shared rewrite/publication/wait and single-precision
table/scale/divide/conversion. Distinct one-step/up-table constants and ignored
opcode23 wait return retained. Actual seed/float/table/command/runtime domains open.

Validation: read-only ISO manifest passed18 spans (four bodies/one region/six
linked/six numeric data),95 anchors/nine calls/one registration. Six interval/
five selection/five cents/six route/seven conversion vectors, all65536 cents
quotient cases and exact float tables/literals checked as deductions. Link audit
passed826 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual seed/RNG/command/note/
cent/reference/rate/table/caller producers, valid interval/table/float/divisor/
shared-cell/alias/IRQ/concurrency domains, pitch/DSP consumers and original
random-note/pitch/audio traces. Full native/runtime binding, lower-priority inbox/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro vibrato/portamento

Completed mcmdVibrato/mcmdPortamento/synthInitPortamento and opcodes27/28
for EVT-01; checked the corresponding inbox portion.
[Vibrato/portamento](behavior/audio/MUSYX_MACRO_VIBRATO_PORTAMENTO_AND_INITIALIZATION.md)
pins conversion/zero duration versus signed-byte/phase, controller65 write/query/
mode gates and initializer early-return/state stores. Mode2 does not clear400
on low controller; initializer skip still permits caller flag publication. Actual
command/provider/consumer/shared-cell/runtime domains remain open.

Validation: read-only ISO manifest passed ten spans (three bodies/two regions/
four linked/table),151 anchors/ten calls/two registrations. Eight vibrato/seven
mode/five initializer vectors and byte/mask/order deductions checked. Link audit
passed825 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual command/duration/rate/
amplitude/byte133/base132/flags/controller/caller producers, modulation/pitch/job
consumers and valid shared-cell/alias/IRQ/concurrency/initialization domains;
original vibrato/portamento/controller/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero hardware sample playback/pitch

Completed hwInitSamplePlayback/DoSetPitch/SRC/filter/ITD for EVT-01; checked
the corresponding inbox portion.
[Hardware playback/pitch](behavior/audio/MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md)
pins inclusive slot clear/preserved bit, full descriptor/padding copy, field/
SRC/filter/ITD writes and unsigned pitch normalization/strict backward table scan/
interpolation. Stable slot255 never ends; exact octave reaches pre-table reads.
Actual input/extent/alias/DSP/runtime remain open; no arithmetic repair inferred.

Validation: read-only ISO manifest passed nine spans (five bodies/three tables/
preceding halfword),146 anchors/three calls. Eight copy bindings, four finite slot/
nine pitch vectors and mask/width/divisor/table/address/order deductions checked.
Diagnostic stops are not original returns. Link audit passed823 Markdown files/
zero broken links; scoped `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 actual hardware pool/time-slot/descriptor/padding/
flags/SRC/ITD/pitch/caller providers, valid ordinal/table/ratio/divisor/preceding-
memory/alias/IRQ/concurrency domains and DSP/format/loading/consumer/original
playback/pitch/audio traces; other macro opcodes. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro sample start/resource lookup

Completed mcmdStartSample/dataGetSample/smpcmp and opcode16 for EVT-01;
checked the corresponding inbox portion.
[Sample start/lookup](behavior/audio/MUSYX_MACRO_SAMPLE_START_AND_RESOURCE_LOOKUP.md)
pins referenced-row search/shared scratch/ordered descriptor output, wrapping
mode offset/extent clamp, eight hardware arguments and fresh post-child voice/
pitch/flag/job publication. Length0 retainsffffffff offset. Actual residency/
descriptor/domain/hardware/pitch/runtime remain open; no fallback or guard inferred.

Validation: read-only ISO manifest passed11 spans (three bodies/one region/six
linked/table),138 anchors/eight calls/one registration. Eight output bindings,
seven offset/six extent/five flag vectors and comparator/unsigned division/address/
order deductions checked. Link audit passed821 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 hwInitSamplePlayback/DoSetPitch and actual resident
registry/descriptor/ID/ref/output/command/word158/length/flags/metric/pitch/caller
providers, valid sorted/duplicate/member/zero-length/shared-cell/alias/IRQ/
concurrency domains and original start/pitch/sample/audio traces; other opcodes.
Full native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro subroutines/note commands

Completed mcmdGosub/mcmdAddKey/voiceIsLastStarted and opcodes24..26/36..37
for EVT-01; checked the corresponding inbox portion.
[Subroutines/notes](behavior/audio/MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md)
pins rotating push/raw return/miss cleanup, low16/S16 note clamp, route-specific
last-note gates and shared command rewrite/wait/result propagation. Actual ring/
note/table/command/caller/domains/runtime remain open; no index repair inferred.

Validation: read-only ISO manifest passed14 spans (three bodies/five regions/
five linked/table),162 anchors/13 calls/five registrations. Six push/four return/
six note/six predicate vectors and mask/address/order deductions checked. Link
audit passed820 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Earlier authorized push failed GitHub username/
token authentication; no publication claimed. Latest objective removes periodic
pushes, so this delivery is committed locally.

Next: priority-zero EVT-01 other macro opcodes and actual ring/note/base131/132/
last-started/code/command/caller writers, valid count/index/slot/generation/table/
alias/shared-cell/IRQ/concurrency domains and original subroutine/note/wait/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero key-off/pedal/event traps

Completed macSetExternalKeyoff/macSetPedalState and opcode40/41 for EVT-01;
checked the corresponding inbox portion.
[Key-off/pedal/traps](behavior/audio/MUSYX_MACRO_KEYOFF_PEDAL_AND_EVENT_TRAPS.md)
pins low publication/high defer, event versus wake, full-word pedal and post-child
release clears, fresh shared trap install and selected clear/three-base enable
scan. Existing sample-end contract revalidated; no duplicate completion. Actual
flag/event/index/caller/shared-cell/runtime domains remain open.

Validation: read-only ISO new manifest passed seven spans (two bodies/two regions/
two linked/table),127 anchors/five calls/two registrations; existing notification
manifest16 spans/201 anchors revalidated. Six key-off/seven pedal/five address/
five displacement vectors and masks/order checked as deductions. Link audit
passed819 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. User requested periodic upstream pushes;
commit/push result is verified separately after this delivery is committed.

Next: priority-zero EVT-01 actual flag/event/enable/command/code/caller/pedal/job
writers/consumers, valid event index/state/member/null/alias/shared-cell/reinit/
IRQ/concurrency domains; other opcodes and original key-off/pedal/sample/message/
audio traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero macro messages/VID/key groups

Completed mcmdSendMessage/mcmdGetVID/mcmdSetKeyGroup and opcodes42..44/89
for EVT-01; checked the corresponding inbox portion.
[Macro messages/VID/groups](behavior/audio/MUSYX_MACRO_MESSAGES_VID_AND_KEY_GROUP.md)
pins variable payload/macro-versus-VID/callback routing, queue/trap/pop/copy and
clear-own-group-before-key-off/kill order. Repeated mapped delivery/raw index
access before normalization/empty-zero receive retained. External key-off/callback/
actual command/queue/ID/group/domain/runtime remain open; no ring repair inferred.

Validation: read-only ISO manifest passed14 spans (three bodies/four regions/
six linked/table),183 anchors/11 calls/four registrations. Five enqueue/four
receive/six group/five index vectors and raw queue/mask/address/order deductions
checked. Link audit passed818 Markdown files/zero broken links; `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 external key-off/callback provider, queue/trap/base/ID/
group/command/caller/count/pool writers and valid ring/slot/member/generation/
null/duplicate delivery/alias/IRQ/reentrancy/concurrency domains; other macro
opcodes and original message/activation/group/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Original harbor before-battle event execution

The ordered opening now continues from the original six harbor messages into
`gundan1000tai_beforebattle`, loaded from read-only US `gor.rel` with all132
commands and relocated source operands intact. Native bindings retain original
integer coordinate admission, fresh/resume branches, actor/speaker ownership,
022→023→024 messages, independent charges, camera requests, HP floor5 and
Crump setup2. Existing native movement is an explicitly limited presentation
adapter. The stationary200ms surprise jump uses the recovered normal binary32
recurrence, including same-call ascent/descent transition and floor admission;
its scoped verifier checks20 instruction anchors and five SDA constants.
Original event bytes/messages/poses remain local and ignored; provenance and
conversion choices are in [assets](ASSETS.md), [the encounter contract](behavior/story/NATIVE_HARBOR_BEFORE_BATTLE.md)
and [the scoped jump contract](behavior/field/FIELD_SCRIPTED_STATIONARY_JUMP.md).

Validation: isolated `/tmp/pm-harbor-continuation` `make test` passes105 CTest
and100 Python tests; `make all` builds the native3DS executable. Tests cover
signed integer poll edges, fresh/resume coordinates, original message order,
HP2→5/HP10 retention, stationary jump samples, and integrated story3→4 control
and save suppression. The original-source verifier passes against the read-only
ISO. Isolated Azahar `artifacts/azahar-harbor-fresh-chain` was inspected at the
original Goombella bubble and the pending battle boundary after actor charge.
No user profile/save was modified. Link audit724 Markdown files/0 broken links
and whitespace checks pass. An earlier resume smoke did not cross the X admission
threshold; it is not counted as successful native encounter validation.

Limits/next: the real tutorial battle-start service is unbound. The source VM
stays blocked at that call with a development notice, retains story4, suppresses
saving and never fabricates a battle result or dispatches the postbattle child.
Bind the original setup2/tutorial/return services before story5 and original
025..033 continuation. Original effect/audio request consumers, numeric NPC
movement, roll/horizontal jump branches and complete opening scene choreography
remain open. Keep the user goal active; the earlier title/files/opening adapter
plus this encounter are incremental playtest milestones, not a complete remake.

## 2026-10-13 — Inbox priority-zero RPN parameter writers

Completed inpSetRPNHi/Dec/Inc for EVT-01; checked the corresponding inbox portion.
[RPN writers](behavior/audio/MUSYX_RPN_PARAMETER_WRITERS_AND_VOICE_PROPAGATION.md)
pins selector routes/byte clamps and steps, high-word nine-preserved/eight-input
bits, equality-only word guards and distinct retained/fresh propagation. Boundary
cases still scan; step defaults can differ across matches under unsupported
mutation. Actual selector/caller/table/value/alias/runtime domains remain open.

Validation: read-only ISO manifest passed three complete spans,219 anchors/zero
calls. Six high-mask/seven byte/six whole-word/six selector vectors and packed-
address/rotate/scan-order deductions checked. Link audit passed815 Markdown files/
zero broken links; `git diff --check` passed. Documentation/metadata only; no
`make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual selector/controller/
default/voice/count/caller producers/consumers, valid set/channel/packed-value/
address/matching/alias/reinit/IRQ/concurrency domains and original RPN/controller/
audio traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero MIDI setters and notification

Completed inpSetMidiCtrl/inpSetMidiCtrl14/synthKeyStateUpdate for EVT-01;
checked the corresponding inbox portion.
[MIDI setters](behavior/audio/MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md)
pins byte stores/change versus forced marker, RPN/default-before-store propagation,
matching fresh voice scans/job requests, MIDI aux dirty replacement and unclamped
14-bit pair order. FX special38 retains set255 MIDI/default formulas. RPN children/
actual caller/table/count/job domains/runtime remain open; no address repair inferred.

Validation: read-only ISO manifest passed nine spans (three complete/six linked),
213 anchors/18 calls. Eight store/six parameter/six pair/four address vectors and
mask/address/matching/order deductions checked. Link audit passed814 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 RPNHi/Dec/Inc and actual controller/selector/default/
voice/count/dirty/job/caller providers, valid table/voice/matching/alias/IRQ/
concurrency/reinit domains; other macro opcode paths and original pair/publication/
pedal/audio traces. Full native/runtime binding, lower-priority inbox/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero macro variable commands

Completed mcmdVarCalculation/mcmdIfVarCompare/inpSetExCtrl and opcodes96..101/
112..113 for EVT-01; checked the corresponding inbox portion.
[Variable commands](behavior/audio/MUSYX_MACRO_VARIABLE_CALCULATION_COMPARE_AND_SET.md)
pins S16 arithmetic/saturation versus signed32 comparison/inversion/PC, immediate
assignment and14-bit controller setter request order. Incoming result registers
for unsupported modes, fresh command reads/raw indices/reused channel CR retained.
Downstream MIDI setter/commands/domain/runtime and other opcode paths remain open.

Validation: read-only ISO manifest passed17 spans (three bodies/eight regions/
four linked/two tables),224 anchors/24 calls/eight registrations. Eleven arithmetic/
seven comparison/eight clamp/six index vectors and PPC comparator/rotate/mask/
address/order deductions checked. Link audit passed813 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 complete downstream MIDI controller setter, other
macro opcode paths and actual command/source/variable/controller/base/PC/incoming
register/reachable mode/domain writers, shared-cell/alias/IRQ/concurrency and
original calculation/comparison/controller/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Original ordinary NPC talk selection and retained owner order

Commit `0203759` binds [native ordinary talk selection](behavior/field/NATIVE_NPC_TALK_SELECTION.md)
to recovered source dimensions, Mario display/main flags, key-off admission,
live viewYaw versus rounded camera snapshot/display getter, inclusive vertical/
radius checks, center/+halfwidth/-halfwidth halfplanes, null-callback map
obstruction and strict nearest/first tie. Portable NpcTalkPlayer/NpcTalkScan
separate mechanics from native geometry/registry services. Source nearActor
r25 remains distinct from chosen candidate; native registry/auto-turn ownership
is not invented. Current static entries map source Entry flags3 and known area/
story lifetimes. Source20/37 dimensions cover current ordinary Mario only;
transformed dimensions/modes/party and nested-key/forced providers remain open.

The retained prior NPC candidate blocks following Mario A jump without a fresh
facing/radius scan. Later NPC scan publishes current candidate; A commits pending
selection and the next NPC invocation starts the original simple Mouser talk.
Wait/return clears without rescanning in that invocation. Talk yaw turns toward
Mario and catalog saved yaw restores after closure. Source US IDs/text/control
tokens/story branches remain unchanged; no story write added. Prompts consume
retained simulation state rather than running map rays while rendering. Catalog/
original angle-table staging is atomic. Resident source-winding head meshes,
spatial indices and dynamic object geometry are reused; null talk rays bypass
ordinary Mario wall attributes/ordinarySolid. ARM object inspection confirms
VFP float arithmetic and vsqrt.f32 for the ordinary finite norm, with fmaf/trig
library adapters preserving their stated numeric boundaries. Existing citro3d
material/pass/cull/texture rendering remains.

Delegated [side-response numerics](behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
and [talk null callback/display getter](behavior/field/FIELD_NPC_TALK_RAY_AND_DISPLAY_DIRECTION.md)
pin exact polynomial/FMA/cap/constants, direct NPC-main gates, null callback
policy, getter and next-invocation ordering. Earlier scope0 terminology was
corrected. [OW-INTERACT-01](requests/OW-INTERACT-01.md) remains open for full
registry/encounter/regular-event/auto-turn/collision/backend/runtime ownership.

Validation: final shared `make test` passes117 CTest/100 Python at
`/tmp/npc-talk-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-talk-qt8g5ba7` passes116 CTest/100 Python at
`/tmp/npc-talk-isolated-test-final.log`. Difference is the existing private
professor fence-reference test excluded from the snapshot. Shared and isolated
`make all` pass, no warnings/errors, logs `/tmp/npc-talk-shared-native-final.log`
and `/tmp/npc-talk-isolated-native-final.log`. Independent source/predicate/
geometry/order/render review passes; its identified live Mario flag gap was
fixed and covered. Tests include full original camera/head geometry, source
boundary/width/flag/nearest/nearActor vectors, retained candidate and pending/
return ordering, old out-of-range jump and an attributes4 polygon proving
filtered wall miss versus null-callback talk obstruction.

An initial emulator fixture failed to open a message despite passing weak
return/save assertions. Full-camera host warmup proved the authored view settles
from0 to180, reversing eligible world side. Corrected fixture is original
room2/story8 at native(1.2,-4); tests now warm source camera and explicitly
require a visible original message cloud plus no onset checkpoint write.
Final isolated binary `artifacts/ttyd-npc-talk-final.3dsx`, profile
`artifacts/azahar-npc-talk-cached`, script `tests/azahar_npc_source_presentation.py`
passes; inspected idle/talk/return show original NPC/window and restored field,
unchanged story/checksum and return/save assertions pass. Final emulator log
`/tmp/npc-talk-emulator-cached.log`; final image runs60fps in software emulator,
which does not prove New2DS XL speed or original-runtime pixel equality.
Rejected `artifacts/azahar-npc-talk` is not validation; corrected intermediate
`artifacts/azahar-npc-talk-final` also passes strict window assertions.
Documentation links pass812 Markdown files/zero broken links; whitespace passes.
ISO read-only, artifacts/compiled packs/profiles ignored; concurrent battle/audio
work preserved through scoped Git indices.

Next: bind original stationary NPC side response with the now-pinned polynomial/
FMA/quadrant/cap graph, active1+2/state10 and bypass8/20/4000 gates, previous-NPC-XZ
rollback and ordered Mario wall retry; preserve Mario→objects→NPC scheduling.
Then complete dynamic registry/auto-turn/regular events and source encounter/
door/pipe/A composition, expand original per-area NPC/event/object coverage,
finish display/material/shadow services and profile New2DS XL hardware. Full
remake and broader goal remain unfinished.

## 2026-10-13 — Inbox priority-zero macro metric/priority inline state

Completed macHandleActive opcodes48..57 for EVT-01; checked the corresponding
inbox portion. [Metric/priority inline state](behavior/audio/MUSYX_MACRO_METRIC_PRIORITY_AND_INLINE_STATE.md)
pins metric add/set/scaled-byte/hardware publication, priority signed16-sum clamp,
rate division/truncation and global/aux/tremolo/flag writes. Captured versus fresh
shared command reads retained; no generic saturation or measured age/time unit
inferred. Actual command/provider/domain/consumer/runtime and other opcodes open.

Validation: read-only ISO manifest passed13 spans (ten regions/two linked/table),
112 anchors/five calls/ten registrations. Seven metric-add/six priority-add/six
scale/six rate vectors and rotate/mask/address/order bindings checked as source
deductions. Link audit passed812 Markdown files/zero broken links; `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELF/listing stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcode paths and actual commands/metric/
rate/priority/word158/global/aux/tremolo/flag producers/consumers, valid table/ID/
member/shared-cell/alias/IRQ/concurrency domains; original metric/priority/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero VID lifecycle and last-started reset

Completed vidInit/vidRemoveVoiceReferences/vidMakeRoot/vidGetPublicId and
voiceResetLastStarted for EVT-01; checked the corresponding inbox portion.
[VID lifecycle](behavior/audio/MUSYX_VID_LIFECYCLE_AND_LAST_STARTED_RESET.md)
pins112-node initialization, conditional table reset, non-root detach/root
promotion/terminal recycle and root/public-chain lookup. Exact repeated pointer/
identity reads and FX slot-index reset retained. Actual caller/graph/null/member/
identity/concurrency/runtime domains remain open; no null repair/atomicity inferred.

Validation: read-only ISO manifest passed five complete spans,252 anchors/one
call. Seven branch/six last-started vectors,112-node links/endpoints and global/
field addresses checked as source deductions. Link audit passed811 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 actual chain/node/last-started/identity/caller writers,
remaining macro opcodes/metric producers and valid null/equality/member/slot/
generation/sorted/cycle/alias/reinit/IRQ/concurrency/rollback domains; original
promotion/removal/reservation/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero voice block/unblock and allocation peek

Completed voiceBlock/voiceUnblock/voiceAllocatePeek for EVT-01; checked the
corresponding inbox portion. [Block/unblock/peek](behavior/audio/MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md)
pins fixed FX search, free/count bookkeeping, protected FX/group/synthetic
identity publication, raw unblock hardware/recycle/final pool reload and peek's
group-word output admission. Miss/free peek leaves output unwritten; raw ordinal
and low8 identity indexing remain distinct. VID/callers/domains/runtime open.

Validation: read-only ISO manifest passed nine spans (three complete/six linked),
180 anchors/ten calls. Seven counter/seven identity/four peek vectors and global/
table addresses checked as source deductions. Link audit passed810 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 VID reference removal and actual block/unblock/peek/
priority/metric/kind/identity/count callers/providers, valid raw ordinal/synthetic
ID/member/sorted/cycle/alias/reinit/IRQ/concurrency/accounting/rollback/stream/
resource domains; remaining macro opcodes and original reservation/free/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero priority-list and voice-free writers

Completed voiceSetPriority/voiceFree/hwSetPriority for EVT-01; checked the
corresponding inbox portion. [Priority/free writers](behavior/audio/MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md)
pins early return/unlink/prepend/sort insertion, unconditional inactivity/free
append/wrapping counters and packed hardware priority memory publication.
Corrected reversed next/previous priority-sort labels in the earlier kill
contract using freshly inspected source; exact previous source stores retained.
Actual caller/metric/identity/member/hardware domains/runtime remain open.

Validation: read-only ISO manifest passed four spans (three complete/one linked),
150 anchors/two calls; existing kill manifest revalidated. Six packed-priority/
five admission/six counter vectors and sort/address bindings checked as source
deductions. Link audit passed809 Markdown files/zero broken links; `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 actual priority/metric/kind/identity/caller providers,
other list/count writers and coherent slot/member/sorted/cycle/alias/reinit/IRQ/
concurrency/accounting/rollback domains; hardware/DSP consumers, remaining macro
opcodes and original allocation/free/replacement/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero allocation-aid initialization

Completed synthInitAllocationAids for EVT-01; checked the corresponding inbox
portion. [Aid initialization](behavior/audio/MUSYX_ALLOCATION_AID_INITIALIZATION.md)
pins four count reads, free-link/endpoints, membership-only/head reset and ordered
FX/root/Music writes. Zero endpoint count retains root0/marker255 and a store
before the free table. Valid counts/call order/other writers remain open; no
empty-root repair,64-slot guard or voice/resource cleanup inferred.

Validation: read-only ISO manifest passed one complete span,142 anchors/zero
calls. Eight stable-count/six free-row vectors and exact global/table addresses
checked as source deductions. Link audit passed808 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 voiceSetPriority/free/recycle writers and actual
count/caller/argument/limit/marker/metric/ID providers, valid slot/member/sorted/
cycle/alias/reinitialization/concurrency/accounting/rollback domains; remaining
macro opcodes and original initialization/allocation/audio traces. Full native/
runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero voice selection/allocation bookkeeping

Completed voiceAllocate/voiceAllocateFind for EVT-01; checked the corresponding
inbox portion. [Selection/allocation](behavior/audio/MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md)
pins limits/group counts/protection/type/priority/minimum-metric/free choices and
free-record unlink/wrapping running counters. Matching protected voices count
against group cap; no eviction/hardware/macro/VID cleanup occurs in these bodies.
Actual aid/priority/free/accounting providers and valid domains remain open;
no unlimited cap0, saturation or original replacement/audio policy inferred.

Validation: read-only ISO manifest passed four spans (two complete/two ABI
entries),168 anchors/three calls. Six limit/seven counter/seven group eligibility/
three metric/five cap vectors and exact table/global addresses checked as
source deductions. Link audit passed807 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 allocation-aid initialization/voiceSetPriority/free/
recycle writers and actual argument/limit/count/marker/metric/ID providers,
valid slot/member/sorted/cycle/alias/concurrency/accounting/rollback domains;
remaining macro opcodes and original allocation/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro start/priority/VID construction

Completed macStart/seqGetMIDIPriority/vidMakeNew for EVT-01; checked the
corresponding inbox portion. [Start/priority/VID](behavior/audio/MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md)
pins lookup/priority/allocation requests, old state/wait cleanup, pending fields/
compound ID, public VID construction and active publication/failure cleanup.
VID selector0 returns a fresh internal ID; nonzero returns public ID. Counter
advances before free-node admission. Allocator/priority/resource/free providers
remain linked/open; no complete rollback/lifetime or original playback inferred.

Validation: read-only ISO manifest passed15 spans (three complete/12 linked),
234 anchors/15 calls. Six key/five priority/four address/five compound/five state/
four flags/four counter/four return-mode vectors and24 field bindings checked
as deductions. Link audit passed805 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 voiceAllocate/voiceSetPriority/reference/free/resource
services and actual argument/registry/ID/VID/list/pending/flag/clock lifecycle,
valid slot/member/generation/sorted/alias/concurrency/rollback domains; remaining
macro opcodes and original activation/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro spawn/links/FX clone

Completed mcmdPlayMacro/synthFXCloneMidiSetup and opcode8 stub for EVT-01;
checked the corresponding inbox portion.
[Spawn/links/FX clone](behavior/audio/MUSYX_MACRO_SPAWN_LINKS_AND_FX_CONTROLLER_CLONE.md)
pins signed key clamp/raw16-argument request, temporary suppression/fixed clear,
public-node and predecessor/successor publication, fresh clone admission and
five limited controller requests. macStart itself remains linked/open. No
identity/lifetime/bounds/chain repair, prior-byte restoration, full clone or
implicit dirty invalidation is inferred.

Validation: read-only ISO manifest passed six spans (two complete/one stub/two
linked/one table),86 anchors/eight calls. Seven key/seven ID/three argument
vectors, clone controller/byte and suppression/chain order checked as deductions.
Link audit passed803 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 complete macStart/allocation/resource/VID constructors,
actual pending/ID/link/clone/suppression writers and identity/lifetime/alias/
cycle/concurrency/invalidation domains; remaining macro opcodes and original
spawn/clone/audio traces. Full native/runtime binding, lower-priority inbox and
full-game work remain open.

## 2026-10-13 — Original NPC descriptor0 fallback and collision/talk research

Commit `b5b1242` retains original animation descriptor0 in NPC subsets and
selects it on case-sensitive missing-name requests: Chu CHU_Z_1, Frankly Z_1,
including original lowercase tribe d_1. Canonical base/talk names reach retained
pose clocks before change detection, so two unknown names selecting one pose
preserve restart0; source message generations still force same-name Talk/Stay
restarts. Empty-bank rejection and map-object exact-name lookup remain. Packs
retain original material/pass/cull/UV/color data and existing citro3d submission;
source descriptor directory/provenance/rights are recorded in [assets](ASSETS.md)
and the [native contract](behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).
Numeric selectors, unexported valid source poses and repeated same-name scripted
Frankly restart requests remain separate coverage adapters.

Delegated documentation pins [NPC sphere/side response](behavior/field/FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md)
and [ordinary talk selection/lifecycle](behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md):
source vertical-segment query, inclusive side geometry/NPC rollback/Mario push/
wall retry; Mario20/37 dimensions, NPCEntry flags3, camera-relative center/width-
edge halfplanes, obstructing map ray, strict nearest/first tie, original A state
transition and Mario→objects→NPC update order. [OW-INTERACT-01](requests/OW-INTERACT-01.md)
remains open. These recovered predicates are not yet the native interaction
binding; the current .65-native-unit shortcut remains explicitly provisional.

Validation: final shared `make test` passes116 CTest/100 Python at
`/tmp/npc-fallback-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-fallback-gxc4y_oe` passes115 CTest/100 Python at
`/tmp/npc-fallback-isolated-test-final.log`. Difference is the existing private
professor fence-reference check excluded from the snapshot. Shared and isolated
`make all` pass at `/tmp/npc-fallback-shared-native.log` and
`/tmp/npc-fallback-isolated-native.log`, no warnings/errors. Independent read-only
fallback/cache/material review passes. Isolated binary
`artifacts/ttyd-npc-fallback.3dsx`, profile `artifacts/azahar-npc-fallback`,
original Mouser idle/talk/return script passes; inspected talk image shows the
original NPC and message window visible, and unchanged-story/checksum/save-return
assertions pass. Log `/tmp/npc-fallback-emulator.log`; this software emulator
check does not establish New2DS XL performance or original-runtime pixel parity.
Final documentation link audit passes801 Markdown files/zero broken links;
`git diff --check` passes. ISO read-only; assets/profile/build artifacts ignored;
concurrent battle/audio/source-history edits preserved through scoped indices.

Next: replace provisional ordinary NPC search with recovered motion/flag/
vertical/radius/directional/occlusion/nearest rules and preserved candidate
ownership across Mario/NPC update order; adapt fixture positions/facing to source
radius. Bind source stationary/dynamic side response after exact polynomial/wall
and caller domains, retain distinct partner scan. Then expand original per-area
NPC/event/object lifetimes, collision and dialogue branch coverage; complete
pre-display/material/shadow services and hardware profiling. Full remake and
broader goal remain unfinished.

## 2026-10-13 — Inbox priority-zero macro loop/jumps/registry lookup

Completed four loop/registry/binary-search/comparator bodies and three control-
flow regions for EVT-01; checked the corresponding inbox portion.
[Loop/jumps/lookup](behavior/audio/MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md)
pins shared countdown/random/gates/loop PC, header/scratch lookup and exact
conditional/direct miss/termination requests for six opcodes. Zero random divisor
is unchecked; binary lookup assumes valid sorted/provider domains. No private
loop stack, registry/code bound repair or full resource/free semantics inferred.

Validation: read-only ISO manifest passed14 spans (four complete/three regions/
six linked/one table),153 anchors/15 calls. Nine loop/five offset/seven bucket/
six search vectors/six registrations and comparator binding checked as deductions.
Link audit passed801 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macro opcodes, registry/assets/resource/
reference/free providers and actual command/counter/PC/flags/ID writers, valid
nonzero-divisor/sorted/member/code-range/alias/reentrancy/concurrency domains;
original loop/branch/termination/audio traces. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro wait/queue/time conversion

Completed five macro wait/queue/random/conversion local bodies and two opcode
stubs for EVT-01; checked the corresponding inbox portion.
[Wait/queue/time conversion](behavior/audio/MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md)
pins staged flags/early-return effects, random remainder/indefinite sentinel,
four exact clock bases, due-deadline publication and equal-before queue insertion.
Tick conversion retains division-before-multiply/wrap. Source clock units are
not converted into measured original milliseconds or renderer-frame counts.

Validation: read-only ISO manifest passed11 spans (five complete/two stubs/three
prior children/one table),185 anchors/nine calls. Six RNG/six conversion/five
rate/four base/five deadline/five queue/five remainder vectors checked as
deductions. Link audit passed798 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macro control-flow/opcode paths and actual
command/flag/seed/rate/index/clock/timestamp/list writers, nonzero-rate/member/
sorted/cycle/alias/concurrency/indefinite domains; original wait/random/queue/
event/audio traces. Full scheduler/native/runtime binding, lower-priority
inbox/full-game work remain open.

## 2026-10-13 — Original NPC facing, dimensions, talk clocks and surface culling

Commit `1e959c7` binds [ordinary NPC local presentation](behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md)
and records newly delegated [dimensions/anchor/display owners](behavior/display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).
Original event yaw is now separate from the NPC matrix: source angleABf table/
ordered camera correction, scale/pivot/Z-X-Y/root composition, Main sign then
Inner front/rear target/unwrap/per-draw fused0.1 smoothing and Z reflection replace
literal yaw-based mirroring. Fresh angle0/flags9 state is retained per room actor;
sign is published by Main, not invented as a source Entry+78 initializer. Host/
ARM SDK/trig/matrix rounding and complete original pass scheduling remain adapters.

PMNPC005 exports original model+c4 masks, tribe30/30 dimensions and CHU_T_1/T_1
names. Old1..4 remain readable; dimensions do not change geometry scalar2.
Original Chu/Frankly NPC talk anchors use root+height once, not mesh bbox/scale.
MessagePlayback now publishes source Talk/Stay generations and timestamps, and
host-testable NpcPoseClock preserves/restarts pose timing from simulation time,
including same-name requests and clock reset. Native ordinary Chu uses original
CHU_T_1 while talking. Explicit anchor overrides, other party/battle NPC anchors,
full NPC name/flags/pre-display/collision/arbitration and scripted pose owners
remain partial. No source dialogue/text/story branches are rewritten.

Original Chu/Frankly now retain source PMANIM4 RGBA, shape-cull/pass and UV/sampler
state via the existing surface compiler. Resource mask1 uses original special
pass2 admission and existing citro3d depth/alpha-test services. Correct facing
changes winding: an initial emulator capture exposed that older PMANIM3 fallback
had dropped original no-cull metadata and made Chu invisible. The final pack keeps
actual source no-cull indices/material data; the final NPC is visibly rendered.
Original texture124px conversion/sampler-aware gutters and source table/dimension
hashes/rights are recorded in [assets](ASSETS.md). Extracted/compiled artifacts
remain ignored; ISO read-only; no original executable payload is exported.

Validation: final shared `make test` passes116 CTest/100 Python at
`/tmp/npc-final-clock-shared.log`; final isolated snapshot
`/tmp/pm-npc-presentation-r_n1cotc` passes115 CTest/100 Python and `make all`, logs
`/tmp/npc-final-clock-isolated-test.log` and `/tmp/npc-final-clock-native.log`.
Only the existing private professor fence-reference check is absent in the
isolated snapshot. No native warnings/errors. Committed owned implementation
matches that tested source snapshot; shared renderer/platform changes were
staged as scoped NPC deltas so concurrent battle work remains preserved.
Original-data tests cover angle cardinal/quadrant/orientation/table migration,
Main-before-Inner/snap sign/camera call count, matrix scalar/root, source anchors,
pose persistence/same-name restart/message generation/reset, catalog1..5 atomic
rejection/original dimensions/talk poses, original shape no-cull/pass/color and
unchanged US story talk. The catalog upgrade initially broke a hidden-object
filter test's hardcoded96-byte model stride; schema-aware100-byte access restores
all three original hidden-name collision cases. An intermediate shared run failed
an in-progress unrelated trio-attack expected callback; final shared run above
passes. No unrelated source behavior was reverted to obtain that result.

Isolated Azahar final binary `artifacts/ttyd-npc-source-final.3dsx` and profile
`artifacts/azahar-npc-source-final` pass original east Mouser talk/return and
versioned checkpoint/story/checksum assertions with
`tests/azahar_npc_source_presentation.py`; log `/tmp/npc-final-emulator.log`.
Idle/talk/return images were inspected and show the source NPC, original message
and source-height bubble anchor. Earlier invisible capture in
`artifacts/azahar-npc-source` is rejected evidence; corrected surface and final
captures are the accepted verification. Warm captured emulator reports60fps,
simulation4.57ms/CPU5.6ms/GPU0.5ms; transient talk capture53fps is also visible.
These are emulator diagnostics, not New2DS XL hardware performance or original
runtime/pixel comparisons. Final link audit796 Markdown files/0 broken;
owned commit whitespace passes. User emulator profile/saves were not modified.

Next: bind source field NPC collision and A/X/B interaction priority using actual
inbox owners/dimensions, add remaining source actors/constructor-specific objects
and areas, recover wander/tavern fades before claiming those NPCs. Complete
pre-display/layers/color/blur/shadow/pose producers, case-sensitive missing-name
index0 fallback against original bank order, nonordinary anchor/override services,
full field/shared VM/pause/lifetime and matched original-runtime/hardware checks.
Continue source coverage from the documentation inbox; do not treat data packaging
or this local presentation milestone as complete NPC/full-game fidelity. The
broader user goal remains active and incomplete.

## 2026-10-13 — Inbox priority-zero active macro initialization/last-started

Completed the macHandleActive initialization prefix and two last-started voice
local contracts for EVT-01; checked the corresponding inbox portion.
[Active init/last-started](behavior/audio/MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md)
pins admission/break/pending/input/note/default/reset/flags/local-variable/clock/
job-start order and exact table setter/reset behavior. Initialization clears
known selector first-use flags before dispatch, but actual lifecycle/reinit
writers remain open. No whole-record reset, atomic timestamp, valid ID/table
bounds or original audible-start guarantee is inferred.

Validation: read-only ISO manifest passed13 spans (two complete/one region/ten
linked),244 anchors/ten calls. Six flag/five pending/four previous-note/six
setter vectors,128+64 table addresses and22 suffix fields checked as deductions.
Link audit passed797 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macHandleActive opcode paths/children and
actual pending/ID/note/channel/flag/PC/clock writers, last-started consumers and
valid bounds/alignment/alias/IRQ/concurrency/reinit/first-mode/r28 domains;
original initialization/selection/job/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro scheduler/scoped dispatch

Completed whole macHandle outer contract and four macHandleActive dispatch
regions for EVT-01; checked the corresponding inbox portion.
[Scheduler/scoped dispatch](behavior/audio/MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md)
pins wait/active/event/captured-next/deadline and macro-clock order, shared
command-budget fetch/PC publication,17 selector registrations and common join.
Normal unmodified counter admits32 commands; attempt33 returns before fetch.
Remaining active initialization/other opcodes are open; no whole-interpreter,
original execution or rendering-frame timing claim from a function fingerprint.

Validation: read-only ISO manifest passed29 spans (one complete body/four regions/
23 linked/one table),158 anchors/24 calls.17 registrations/six wait/six clock/11
opcode/five budget/five event vectors checked as deductions. Link audit passed796
Markdown files/zero broken links; `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Full original inspection ELFs/listing stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 complete remaining macHandleActive initialization/
opcodes/children and actual command/flag/list/debug/time writers; valid sorted/
member/cycle/alias/reentrancy/IRQ/concurrency/clock/first-mode/r28 domains and
original macro scheduling/audio traces. Full scheduler/native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro input selectors/first-use/dirty

Completed18 macro input selector/global dirty setter local contracts and four
mask tables for EVT-01; checked the corresponding inbox portion.
[Selectors/first-use/dirty](behavior/audio/MUSYX_MACRO_INPUT_SELECTORS_FIRST_USE_AND_DIRTY.md)
pins15 voice/two aux builder callers, conditional first-use mode0/reset,
separate signed coefficient decoding and post-builder dirty publication.
Existing selectors still use command mode; shared aux descriptors have per-voice
flags. This proves a conditional first-row/r28 chain, not global reachability/
flag lifecycle/shared ownership or original interpreter/audio timing.

Validation: read-only ISO manifest passed23 spans (18 complete/one prior builder/
four tables),480 anchors/19 calls.15 schemas/four first-use/four field/four global-
dirty/nine coefficient vectors, common voice-body regions and all65536 high-half/
all256 fraction-byte division cases checked as deductions. DOL scan found17
builder calls in these selectors. Link audit passed795 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from the commit.

Next: priority-zero EVT-01 interpreter/command and first-use-flag lifecycle,
shared aux ownership/other descriptor/dirty writers, actual caller/global first-
mode/r28/count/bounds/alias/concurrency domains; original selection/recompute/
audio traces. Full scheduler/native/runtime binding, lower-priority inbox and
full-game work remain open.

## 2026-10-13 — Inbox priority-zero descriptor builder/FX copy/translation

Completed inpAddCtrl/inpFXCopyCtrl/inpTranslateExCtrl local contracts and one
jump table for EVT-01; checked the corresponding inbox portion.
[Builder/copy/translation](behavior/audio/MUSYX_INPUT_DESCRIPTOR_BUILDER_FX_COPY_AND_TRANSLATION.md)
pins zero-low8-mode reset, four-row append cap, count-before-row publication,
variable flag/alias translation and ordered FX byte-copy routes. Builder can
admit nonzero first mode; it does not universally establish evaluator r28.
FX copy adds no dirty invalidation or full voice-ID guard. Actual callers must
supply those domains; no automatic recomputation or bounds repair inferred.

Validation: read-only ISO manifest passed six spans (three complete/two ABI
entries/one table),58 anchors/three calls. Nine builder/17 translation/256 copy-
route/five ID-row vectors and exact publication order checked as deductions.
Documentation link audit passed793 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual builder/copy/translator callers and dirty
invalidation owners, other descriptor/controller writers, first-row/r28 and
FX bounds/lifetime/alias/concurrency domains; original build/copy/recompute/audio
traces. Full scheduler/native/runtime binding, lower-priority inbox/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero matrix cache/aux/external wrappers

Completed17 matrix cache/aux/external-controller local contracts and three
tables for EVT-01; checked the corresponding inbox portion.
[Wrappers/dirty/cache/external](behavior/audio/MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
pins14 voice wrapper schemas, aux A/B shared masks, pre-evaluation dirty clears
and external remap dispatch versus original-index fallback. Wrappers do not
initialize r28 or restore dirty flags after the evaluator. No boolean result
conversion, matrix-special fallback substitution or bounds repair inferred.

Validation: read-only ISO manifest passed22 spans (17 complete/two prior children/
three tables),224 anchors/17 calls.14 schemas/eight masks/nine jump targets/nine
dispatch/16 dirty/five aux-address/five signed-half vectors checked as deductions.
Documentation link audit passed792 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 descriptor construction/dirty/index/channel/set writers
and actual wrapper callers, incoming-r28/first-mode/count/numeric/bounds domains,
cache/voice/filter/aux consumers and original recompute/audio traces. Recover
inpAddCtrl/FX-copy/translation mutations next. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero input matrix/variable domain

Completed _GetInputValue/varGet local contracts for EVT-01 and checked the
corresponding inbox portion. [Matrix evaluator/domain](behavior/audio/MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md)
pins live descriptor count/source markers/wrapped coefficient and combine
arithmetic/cache and variable reads. The evaluator does not initialize incoming
r28 interpretation state; valid caller/first-row domains remain a requirement.
Recorded different first-mode2 residue results without assigning a provisional
flag default, widening integer arithmetic or inferring original audio timing.

Validation: read-only ISO manifest passed six spans (two complete/four linked),
191 anchors/six calls.256 sources/64 combines/three residue/eight scaling/six
elapsed/six variable vectors checked as deductions. Documentation link audit
passed791 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full
inspection ELFs/listings stay `/tmp`; no bodies/assets committed. Concurrent
native edits excluded from the documentation commit.

Next: priority-zero EVT-01 wrappers/descriptor construction/count/first-mode/
coefficient/index/flag writers and incoming-r28/reachable numeric domains,
external-controller child and voice/variable/clock/cache consumers; original
matrix/audio traces. Full scheduler/native/runtime binding, lower-priority
inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller/default/last-note reads

Completed three controller/default-pointer/last-note local contracts for EVT-01
and checked the corresponding inbox portion.
[Controller/getter reads](behavior/audio/MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md)
pins all256 controller routes, raw paired-byte OR, byte>=64 thresholds,
constant-zero RPN range and packed/default/last-note table getters. Only the
controller getter rejects channel255. Static cold/warm read compositions are
recorded without clamping raw bytes, inventing shared gates or claiming
original controller/audio timing.

Validation: read-only ISO manifest passed three complete spans/67 anchors,
including both threshold carry/mask sequences/no direct calls.256 routes/eight
pair/eight threshold/six single/six destination vectors checked as deductions.
Documentation link audit passed790 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 _GetInputValue/matrix/filter/dirty/last-note consumers,
actual read/reset/write callers and valid bounds/raw-byte/alias/concurrency
domains; original controller/audio traces. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller initialization/defaults

Completed four controller initialization/reset/default/last-note contracts and
two default-table interpretations for EVT-01; checked the corresponding inbox
portion. [Controller init/reset/defaults](behavior/audio/MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
pins58 per-voice stores, global aux-byte/dirty-word resets, common frequency
defaults, cold copy/warm255 preservation, last-note request and packed9-byte
channel defaults. Global inpInit does not reset all controller values; warm
reset stores only ten positions. No blanket clear, bounds repair, padded host
record or original filter/controller timing inferred.

Validation: read-only ISO manifest passed nine spans (four complete/three linked/
two tables),223 anchors/four calls.58 decoded stores/64 aux/128 dirty writes,
all134 classified bytes per table/six destination/five preserve vectors checked
as deductions. Documentation link audit passed789 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from the commit.

Next: priority-zero EVT-01 actual reset/init callers and controller/matrix/aux/
dirty/last-note/filter consumers/other writers, valid bounds/alignment/alias/
IRQ/reinit/concurrency domains and original controller/audio traces. Full
scheduler/native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Execute original NormalJump through first damage request

Mario's confirmed setup4 Normal Jump now executes the unchanged268-command
DOL stream in a native battle VM. Shared source-address unit/part/menu/candidate
storage feeds the recovered child, targeting, takeoff, approach, position,
MarioJumpParam and MarioJumpPosition services. Original empty datakey7 and
Majinai child events execute. Used facing/pose/declaration/AC setup callbacks
follow the new source contract; setup reset scope remains separate from TimingA
initialization. Fresh precheck and physical trigger history reach source timing
failure(flags100) or first success(flags20000). The live attack root is retained
at attackerU2a8 for the forthcoming reaction barrier. Source coordinator facing
is preserved when publishing the native battle registry.

Original b_mario supplies seven used poses and its own correctly bound battle
atlas. M_A_5 has a single source key over range0..2; import/loader now preserve
that constant pose without generating a terminal key. Original data/constant
sections are retained with live native manager/global ranges excluded. See
[native scope](behavior/battle/NATIVE_ORIGINAL_JUMP_RUNTIME_BINDING.md),
[used callbacks](behavior/battle/NORMAL_JUMP_USED_POSE_FACING_DECLARE_AND_AC_SETUP.md)
and [new reaction/death evidence](behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#original-used-hit-death-and-variant-lifecycle-2026-10-13-follow-up).

Validation: `make test` passed115 CTest/100 Python tests
(`/tmp/trio-attack-tests-final.log`), including actual source approach/airborne
motion, failed timing, single valid presses for all three enemy selections,
source per-hit flags/counter/HP/pose flags/attack-handle ownership, constant
original pose and Game retaining field position/story11/result0/control lock.
`make all` passed(`/tmp/trio-attack-all-final.log`); isolated debug-short build
passed(`/tmp/trio-attack-native-final.log`). Azahar helper
`tests/azahar_trio_jump.py` passed in
`artifacts/azahar-trio-original-jump-ready` with48second boot wait; manually
inspected battle scene and original Mario landing pose at the explicit damage
boundary. Earlier10second run was still black at boot and missed the input;
its later field-only captures do not verify the attack. Final coordinator-facing
preservation was host-tested/build-checked after the native capture. No original
execution, full attack/turn parity or hardware performance claim.

Limits/next: CheckDamage8010a78c remains genuinely waiting; no HP, turn, reaction,
battle result or story progress is fabricated. Camera/SE/voice requests are
retained without complete original providers. Unit entry/equipment/seed/private
pad/helper/prize/display clocks remain bounded adapters. Bind source damage/
counter/calculator/direct application with actual nonlethal/death/wing-fall and
Frankly warning event lifetimes, then bounce/tail/second hit, Hammer/Headbonk,
source move counts/enemy phases, rewards/end/result and field093/094/094_01/
story12. The prologue deliverable and full-game fidelity remain unfinished.


## 2026-10-13 — Inbox priority-zero virtual table provider/DOL census

Completed a scoped configuration-writer and DOL entry census for EVT-01 and
checked its inbox portion. [Provider/census](behavior/audio/MUSYX_VIRTUAL_TABLE_PROVIDER_AND_DOL_ENTRY_CENSUS.md)
consolidates known count/size/generation/callback writes,28 bounded literal-
address constructions owned by the five complete virtual bodies, and seven
exact entry calls. Those bodies supply no positive count/loop-size/nonzero
callback installation. Wider indirect/REL/pointer/alias providers remain open;
no original unused-feature/reachability/runtime conclusion is inferred.

Validation: bounded predicate scan and independent exact DOL direct-call decode
ran read-only. Manifest passed10 spans (two whole text segments/five prior bodies/
three callers),66 anchors/seven calls. Generic verifier checks bytes/hashes/
targets; scan completeness is limited to the explicitly documented predicate.
Documentation link audit passed788 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Scanner/generator stay `/tmp`; no original bodies/assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 wider indirect/REL/alias configuration/callback search,
actual transfer/result/provider contracts and original playback traces; continue
other open scheduler/audio/controller/source requirements. Full native/runtime
binding, lower-priority inbox/full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero virtual-sample refill

Completed the whole vsUpdateBuffer local contract for EVT-01 and checked the
corresponding inbox portion. [Refill descriptor/cursor](behavior/audio/MUSYX_VIRTUAL_SAMPLE_REFILL_DESCRIPTOR_AND_CURSOR.md)
pins type5/6 offset encoding, forward/tail/wrapped two-segment descriptor order,
six tag1 callback sites and nonzero-return advancement using fresh cursor/size
and a wrapped32-bit sum. Zero return adds no parent cursor write. No invented
callback-null/bounds repair, result cap or original byte-transfer/audio timing.

Validation: read-only ISO manifest passed one complete span/141 anchors,
including six indirect sites/no direct calls. Ten offset/13 stable descriptor/
nine post-callback vectors plus all65536 low-input type5 encoding cases checked
as deductions. Documentation link audit passed787 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Original full inspection ELF/listing stays `/tmp`; no body/
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual callback installation/implementation and byte
transfer, slot/cursor/type/size/result providers and valid range/nonzero-size/
alias/reentrancy/concurrency domains; original contiguous/wrapped/partial/
zero-return refill/audio traces. Full scheduler/native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero virtual init/update/position

Completed five virtual initializer/updater/hardware position/state local
contracts for EVT-01 and checked the corresponding inbox portion.
[Virtual init/update/position](behavior/audio/MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md)
pins partial table reset,64-map scan independent of configured count, fresh
identity/state/position/remaining/signed-threshold/cleanup order and hardware
position type routes. Corrected the prior notification prose table base from
803f6b00 to80406b00; original instruction hashes/relative ordering were correct.
No whole-slot reset, position fallback or physical underrun timing inferred.

Validation: read-only ISO manifest passed12 spans (five complete/seven linked),
149 anchors/11 calls; prior notification manifest reverified.64 reset addresses/
six state/ten rounding/five remaining/six threshold/five startup/15 position
vectors plus all65536 low-input rounding cases checked as deductions.
Documentation link audit passed786 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 complete vsUpdateBuffer, table/count/size/callback/
remaining/type/pitch/hardware providers and valid bounds/generation/numeric/
reinit/concurrency domains; original refill/underrun/playback traces. Full
scheduler/native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero sample identity/hardware fields

Completed six sample-start child contracts for EVT-01 and checked the
corresponding inbox portion. [Identity/hardware fields](behavior/audio/MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md)
pins public-ID internal voice chains, active-only sequence/note association
with eight whole-list retries and exact hardware type/ID/extra/loop fields.
The sequence counter is not an eight-node cap and has no paused fallback;
lookup services add no voice identity revalidation. Loop setter reloads base
between raw stores. No original hardware output or association traces inferred.

Validation: read-only ISO manifest passed seven spans (six complete/one prior),
50 anchors/no calls. Eight index/eight width/seven stable-sequence/seven stable-
VID vectors checked as deductions, including ten-node first-pass association.
Documentation link audit passed783 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual voice/sequence/note/ID and hardware field
providers, backend consumers and bounds/generation/cycle/alias/concurrency
domains; original sample association/playback traces. Full scheduler/native/
runtime binding, lower-priority inbox and full-game work remain incomplete.

## 2026-10-13 — Live original save-block events, System graphics and asynchronous SD persistence

Commit `0675712` binds [live save blocks](behavior/field/NATIVE_LIVE_SAVE_BLOCK.md)
by original model identity/catalog source owner and name. Actual source bbox head
contact triggers flag8; per-instance S_1/A_1 clocks and source previous-published
ratio control the original stages. Native raw input/windows/events precede Mario;
object callback and animation publication follow Mario. Neutral-input physics
still lands Mario while nested source key/system ownership is active. Camera8
bit200 has a separate owner; field camera4 does not receive a follow-freeze guess.
The same catalog/registry path serves future rooms with source save-block records.

Original System pattern5, shape42..45/ring46..49 and selection index0 now draw
with original source dimensions/masks, four-quad/nine-slice geometry, source UV/
phase and PICA texture interpolation/alpha stages. Original type3 scissor and
no-tail routing, separate retained-message effect caches, original cursor1f8 and
independent choices are live. Source221-command event graph/US first-hit/dialogue/
branches are unchanged. Renderer samples per-instance raw X with source clip
starts/clamps and caches time as well as pose/tick; it never decides completion.
Cue431 now schedules the original sample550 on resident NDSP channel7. This is
an original primary-sample fallback; MusyX macro/3D/group parity is still unproven.
[Asset provenance](ASSETS.md) records unchanged texture/PCM conversion and rights.

Accepted writes use a libctru worker with an owned checkpoint/path,16KiB stack,
priority31/default permitted core, release/acquire completion and the existing
atomic v6 SD/temp/backup writer. No enqueues no write. Harness writes suppress
while source saving owns input, and exit autosave is limited to legacy harness
state without originalStory. Other harness autosaves remain adapters. Original
GC card/header/error classification, shared field VM/category/pause ownership,
optional custom source object programs and camera8 letterbox UI remain partial.
Native provider fault recovery is an adapter, not invented source behavior.

Validation: shared `make test` passes114 CTest/100 Python. Isolated source/romfs
snapshot `/tmp/pm-live-save-cp7b1_2f` passes113 CTest/100 Python and `make all`;
only the existing private professor fence-reference fixture check is excluded.
All committed implementation files match the validated isolated source snapshot.
Logs: `/tmp/live-save-shared-test.log`, `/tmp/live-save-isolated-test.log`,
`/tmp/live-save-native.log`, `/tmp/live-save-isolated-native.log`. Game tests use
actual jump/head physics, original first-hit/No/Yes windows and delayed real file
writes; they check landing, nested keys, camera separation, unchanged No files,
source switch persistence, restored S_1 and previous ratio at250ms. No compiler
warnings/errors were emitted by the isolated native build. Source clock samples
and test timestamps are adapter inputs, not measured original behavior.

Isolated Azahar binary `artifacts/ttyd-live-save.3dsx` passes repeat-hit No in
`artifacts/azahar-live-save-no2` (byte-identical checkpoint, no temp/backup), Yes
in `artifacts/azahar-live-save-yes` (original success window, atomic v6 file and
backup), and all three first-hit explanation pages followed by No in
`artifacts/azahar-live-save-first` (original flags0 file unchanged). Scripts are
`tests/azahar_save_block_no.py`, `tests/azahar_save_block_yes.py` and
`tests/azahar_save_block_first_hit.py`; fixtures remain ignored in artifacts.
All final choice/return/page/success images were inspected. First No smoke used
an input before the selection finished opening, so its file-only assertion did
not prove cancellation; inspected screenshots exposed that error and the final
scripts wait for the completed source choice. The final No captures show the
cursor on No and return to exploration. Sandboxed Xvfb failed its display socket;
scoped isolated-profile escalation completed the final runs without modifying
user emulator profiles/saves. Warm captured emulator reports60fps, simulation
2.47ms/CPU8.4ms/GPU0.5ms in the idle return; this is not hardware profiling or a
New2DS XL performance guarantee. Final link audit782 Markdown files/0 broken;
owned commit whitespace check passes. Original ISO stays read-only, and extracted
assets/compiled packs/binaries remain outside history. Concurrent battle/audio
research and pending shared edits are preserved through isolated scoped staging.

Next: ordinary NPC source pre-display/facing/tribe dimensions/talk-anchor services
and source interaction/collision arbitration, then remaining constructor-specific
objects and area catalog expansion. Keep using OW-INTERACT-01/other inbox contracts
rather than proximity or fabricated story. Verify source runtime/pixel/audio parity,
full field pause/category/shared VM/save state, narrow System frame cases and
source3D/macro sounds; profile actual New2DS XL hardware. Original S_1/A_1 assets,
source events and reusable live save infrastructure now work; the full-game/area
coverage and overall user goal remain active and incomplete.

## 2026-10-13 — Inbox priority-zero hardware message/sample notifications

Completed five hardware-message/sample notification local contracts for EVT-01
and checked the corresponding inbox portion.
[Message/start-end order](behavior/audio/MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
pins the installed four-message dispatch, hardware virtual-ID leaf, virtual
end matching/callback cleanup, macro sample-end restoration/wakeup and virtual
start reclaim/reserve/map/generation/payload/callback order. Callback return0
accepts a start. End cleanup reads fresh slot voice after callback; message0
checks full voice identity only after virtual end. No original audio timing,
callback mutation guarantee or post-callback identity repair is inferred.

Validation: read-only ISO manifest passed16 spans (five complete/11 linked),
201 anchors/20 calls. Nine message/eight ID/15 generation/eight wakeup/four
callback/five stable-slot vectors checked as deductions. Documentation link
audit passed782 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full
inspection listings/ELFs stay `/tmp`; no bodies/assets committed. Unrelated
concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 virtual table/map/count/counter/callback and voice/
event/flag providers, hardware/VID/sequence-instance children, actual callbacks
and bounds/alias/IRQ/reentrancy/concurrency domains; original start/end/refusal/
reuse/message/audio traces. Full scheduling/native/runtime binding, lower-
priority inbox and full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero synthesis initialization

Completed synthInit/synthSetMusicVolumeType/sndVolume/hwSetMesgCallback local
contracts for EVT-01 and checked the corresponding inbox portion.
[Initialization/type/jobs/callback](behavior/audio/MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md)
pins raw voice allocation/count and exact initial fields, partial fader resets,
row types/current overrides, aux/global/job resets, child request order and
callback publication. The type setter gates on sndActive but has no direct-row
bounds check. No universal fader reset, reinitialization repair or original
audio timing is inferred; linked children remain separate requirements.

Validation: read-only ISO manifest passed19 spans (four complete/13 linked/two
literals),279 anchors/13 calls.32 fader rows/96 job words/34 constant+three ordinal
voice fields/seven count/six setter/six reset-domain vectors checked as deductions.
Documentation link audit passed780 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 controller/allocation/voice/callback children, other
row/bitmap/aux/job writers and actual caller bounds/reinit/concurrency domains;
original initialization/scheduling/audio traces. Full native/runtime binding,
lower-priority inbox and full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero synthesis volume producer

Completed synthVolume/sndConvertMs source contracts for EVT-01 and checked
that inbox portion. [Selectors and fader publication](behavior/audio/MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md)
pins six row-type scans/direct indexing, four numeric literals, low16 duration
conversion, ordered fader/mode/ID stores, zero-duration sequence dispatch and
fresh bitmap OR. Zero duration preserves earlier start/coefficient fields yet
sets active; direct selectors beyond31 derive unchecked table writes. These
are static semantics, not original executed corruption or audible fade timing.

Validation: read-only ISO manifest passed11 spans (two complete/five linked/four
literals),192 anchors/15 calls.256 selector/eight duration/eight target/ten mode/
six row-type vectors checked as deductions. Documentation link audit passed779
Markdown files/zero broken links; `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Original full listings/ELFs stay `/tmp`;
no bodies/assets committed. Unrelated concurrent native work excluded.

Next: priority-zero EVT-01 fader initialization/other bitmap/row writers,
caller group/ID/mode/time/numeric domains, sequence deferred/track consumers and
original duration/audio traces. Full scheduling/native/runtime binding and
lower-priority inbox/full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero sequence speed/volume

Completed four local sequence speed/volume/public-wrapper contracts for EVT-01
and checked their inbox portion. [Speed/volume/group forwarding](behavior/audio/MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md)
pins16 speed halfword stores, tagged flag/payload writes, immediate primary
volume request then64 fresh group comparisons without deduplication, four tagged
volume modes and raw public IRQ forwarding. Speed has no lookup-miss guard and
derives writes before seqInstance; volume returns on miss. No original audible
tempo/fade behavior is inferred from local field/request semantics.

Validation: read-only ISO manifest passed nine spans (four complete/five linked),
137 anchors/ten calls. Eight halfword/seven address/16 mode/15 flag vectors,
16 speed offsets and three64-byte group-order examples checked as deductions.
Documentation link audit passed777 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh original full listings/ELFs stay `/tmp`; no bodies/assets committed.

Next: priority-zero EVT-01 complete synthVolume and deferred/track consumers,
caller lookup-success/private-ID/group domains and original tempo/volume/miss/
audio traces. Full scheduling/native/runtime binding, lower-priority inbox work
and the full remake remain incomplete.

## 2026-10-13 — Inbox priority-zero sequence continue

Completed the local seqContinue/sndSeqContinue source contracts for EVT-01 and
checked their inbox portion. [Continue and missing-ID domain](behavior/audio/MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md)
pins paused→active publication, tagged pause-bit clearing and the raw public
IRQ wrapper. Continue has no lookup-miss guard: source arithmetic derives a
byte write at803eb432 before seqInstance. Runtime contents/execution are not
claimed; caller lookup-success/private-ID requirements remain open.

Validation: read-only ISO manifest passed four spans (two complete/two linked),
53 instruction anchors and three resolved calls; seven ID/tag/address, five
flag-clear and five state-route vectors checked. Documentation link audit
passed776 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full code
listings/ELFs stay `/tmp`; no original bodies/assets committed.

Next: continue priority-zero EVT-01 caller/private-ID, track/note/deferred-field
and audio provider contracts; original resume/missing-ID/audio traces and full
scheduler/native/runtime binding remain open. Lower-priority inbox work and
the full remake remain incomplete.

## 2026-10-13 — Original setup4 command and target menu

Bound a native command/target menu after Frankly's original battle introduction
and window closure. Source default Normal Jump/Normal Hammer/Headbonk weapon
IDs, admission/attribute masks and unchanged US menu/unit labels are imported
from the read-only ISO. PMBUNIT2 adds original local/hit/sampling geometry and
setup forward bias. The existing recovered sampler owns admission/occlusion:
Jump admits all three enemies, including Spiky Goomba; Hammer initially admits
only the front Goomba. Target refresh retains unit/part across row changes,
uses stored status27 rather than HP as death, wraps navigation and gives B
priority over A. Native D-pad/Circle Pad repeats and touch selection operate.
Owned views rebind after copies; confirmations retain actor/weapon/unit/part
without damage, move consumption or field resume. Partner Headbonk targeting
is host-tested but awaits the native turn scheduler. See
[native binding](behavior/battle/NATIVE_TRIO_COMMAND_TARGET_BINDING.md) and
[source follow-up](behavior/battle/UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md).

Validation: `make test` passed113 CTest/100 Python tests
(`/tmp/trio-commands-tests-final.log`). `make all` passed
(`/tmp/trio-commands-all-final.log`); isolated development build with
`DEBUG_SHORT_BOOT=1 BUILD=build-trio-battle TARGET=artifacts/ttyd-trio-battle`
passed (`/tmp/trio-commands-native-final.log`). Native smoke helper passed
`tests/azahar_trio_battle_targets.py` in
`artifacts/azahar-trio-battle-targets`; manually inspected Jump's three targets
with Spiky selected and Hammer's single Goomba. The target label received a
contrast panel afterward. Source tests additionally cover pair compaction,
HP-versus-status27, empty lists, cancellation, copied owners, retained weapon
IDs, physical/touch Game entry and partner targeting. Fixed an ARM int32_t
callback return mismatch in concurrent save-object work; that work stays
separate. No original execution/hardware/full combat parity claim.

Limits/next: confirmation ends at the explicit original-attack-pending notice;
B returns to inspect targets while dispatch remains unconsumed. Execute the
original Jump/Hammer/Headbonk event streams with motion/AC/hit/counter services
and source poses, then source move counts/phases, enemy actions, death/wing-fall
reactions, rewards/result publication and field093/094/094_01/story12. Keep the
inbox requests open for source death/event barriers and remaining composition.
Full playable prologue remains unfinished.






## 2026-10-13 — Priority0 sequence pause/mute/deferred field contracts

Delivered [pause/mute](behavior/audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md).
Three complete bodies pin inline lookup, state1 active→paused before note kill/
recycle, high-bit pause/mute deferred fields, raw mute words and public IRQ
forwarding. No timing/silence/private-index repair inferred. Marked local
EVT-01 portions complete; deferred consumers/provider/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),160 anchors/
five resolved calls passes.10 pause/6 mute/5 flag vectors and three-head order
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original execution.
Link audit775 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: ID/state/list/note/deferred-byte/word/mute-field providers/consumers,
resume/controller/voice scheduling, index/member/alias/IRQ/concurrency and original
pause/mute/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 synthHandle fade/aux region and local body closure

Delivered [fades/aux](behavior/audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).
Complete final region pins zero-offset gate, master/pause coefficient/bitmap
publication, mode requests/early exit and8-studio A→B/tag1/control-halfword
callbacks. Prior queue/voice/clock plus this contract closes local whole
synthHandle. Marked local portion complete; child/provider/runtime EVT-01 OPEN.

Validation: generic read-only verifier9 spans (region/6 linked/whole fingerprint/
zero literal),150 anchors/six resolved calls passes.6 modes/5 bitmap/16 studio-side
rows are source deductions, not original execution. Full listing/ELF remains
`/tmp`, source reverified. Link audit773 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: fade/bitmap/mode/coefficients and queue/voice/time providers, PPC/
alias/IRQ/concurrency domains, sequencePause/Mute/precision/mac/ADSR/aux children,
callback installation and original scheduling/fade/audio traces. Continue
priority0 source/backend then priority1 damage. No hardware/full-game completion.

## 2026-10-13 — Priority0 scoped synthesis job consumer and clock tail

Delivered [consumer/clock](behavior/audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md).
Two complete regions/no-op leaf pin category0→2→1 captured-next/marker/root
order, voice flag/pedal/hardware requests, fresh index advancement and64-bit
raw-input time addition. Whole synthHandle fingerprint does not close its
master-fade/aux middle. Marked local portions complete; parent EVT-01 OPEN.

Validation: generic read-only verifier13 spans (2 regions/1 leaf/9 linked/whole
fingerprint),163 anchors/10 direct calls passes.3 queue/5 index/5 clock vectors
and65536 pedal cases are derived checks; original code not executed. Full fresh
ELF/listing stays `/tmp`. Link audit772 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: synthHandle zero-offset/master-fade/aux middle, precision/zero/mac/
pedal/ADSR/hardware handlers, queue/voice/flags/time providers and member/alias/
IRQ/concurrency domains, original scheduling/audio traces. Continue priority0
source/backend then priority1 damage. No hardware/playback/full-game completion.

## 2026-10-13 — Retained original save windows and ticketed persistence services

Commit `bd61acc` adds [portable save presentation](behavior/field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md).
The original221-command graph now runs with real original global US message
payloads, retained main windows, separate source selection windows and nested
key ownership. Continue preserves cursor/schedule/key wait/identity rather than
force-closing; Add respects raw versus keyed operands. Window poll flags are
separate from playback data flags and survive ordinary waits/closing until
source deletion. Original system parser type/color and state7-to8 fallthrough
are implemented. No save dialogue is authored or rewritten.

Write requests carry one-use tickets and four source switches captured after
same-invocation VM writes. Simulation performs no filesystem I/O; an external
provider supplies delayed success0 or provisional generic error-1. Stale,
duplicate, untaken, finished and moved-from acknowledgements reject. Header's
one-update yield adapts combined native SD serialization; original card hardware,
classification, format/create/IPL and real worker binding remain absent. Missing
resources/unsupported operations fault and yield for owner recovery. Save v6
retains40 bytes/checksum, stores GSF1207/233/236 in byte34 and GSW4 in byte35,
and preserves legacy1..5 decode/reserved-byte checks. Live propagation is next.

Inbox documentation independently recovered camera8's
[letterbox decision lock](behavior/camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
and [original cue431](behavior/audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md).
Bit200 must not freeze native field camera4 following; it preserves camera8's
automatic letterbox choice while fades/draw continue. Original cue431 maps
FX618/group5/macro745/sample550; the ignored source WAV exists but is absent
from the current five-cue native fallback pack. Source runtime residence is
unproven; no generated sound replaces it.

Validation: final shared `make test` passes112 CTest/100 Python. Isolated source/
romfs snapshot `/tmp/pm-save-presentation-v2zjcjj3` passes111 CTest/100 Python and
`make all`; only the existing professor_fence_animation_original check is absent
because its private `artifacts/professor-fence-reference.bin` was not copied.
Logs: `/tmp/save-presentation-final-shared.log`,
`/tmp/save-presentation-final-isolated-test.log`,
`/tmp/save-presentation-final-isolated-native.log`. Real original-data tests cover
first-hit pages, raw scroll/stop, Yes/No/B cancellation, delayed success/failure,
retained main/choice ownership, nested keys, same-tick save-mode snapshot,
stale/duplicate/moved acknowledgements and window lifetime. Save tests cover all
three flag combinations, full mode bytes, v5 migration, older versions and
atomic file/backup recovery. An initial original first-hit test failed because
system parser control was unsupported; the documented type/color implementation
resolved it. Final Markdown link audit771 files/0 broken; owned whitespace checks
pass. No emulator/hardware capture is claimed: presentation remains unbound in
Game, so this milestone does not activate the block's live interaction. ISO
remains read-only and generated/extracted assets remain outside history.

Next: bind SaveBlockPresentation to live source catalog owner/name hit publication,
MapObjectState callback and independently timed S_1/A_1. Publish animation results
after object callback, use original clip starts, and key renderer cache by pose
plus per-instance time. Bind event processing before Mario, nested key/system
ownership while preserving falling physics, separate camera8 letterbox lock,
retained windows/choice drawing and an asynchronous libctru SD worker. Propagate
GSF233/236/GSW4 into progress/write snapshots; suppress automatic harness writes
while original save flow owns persistence and ensure No never writes. Then run
isolated Azahar head-hit/No/Yes/save-file checks and measured hardware profiling.
Other objects/NPCs/areas and complete source field/category/pause/resource/audio
ownership remain open. The full user goal remains active.

## 2026-10-13 — Priority0 synthesis job insertion and start timestamps

Delivered [jobs/start time](behavior/audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).
Two complete bodies pin32-slot arithmetic, distinct category duplicate/reschedule
rules, ordered node/root writes and two real-time captures before0/1 requests.
Unsupported categories have no guarded failure; source type precondition remains.
Marked local EVT-01 portions complete; queue consumers/runtime remain OPEN.

Validation: generic read-only verifiertwo spans/75 anchors/two direct calls passes.
21 slot/3 category/9 duplicate/two timestamp maps checked as deductions; original
code not executed. Full fresh ELFs/listings stay `/tmp`. Link audit771 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: queue/table/index/markers/time init and consumer advancement, category/
member/null/alias/IRQ/concurrency source domains, actual job/macro/voice scheduling
and original queue/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 macro active/inactive/wait/init lifecycle

Delivered [macro lifecycle](behavior/audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).
Four complete bodies pin state equality gates, active/time unlink, two-word wait
sentinels/clear/time and literal flags, activation's two-job request and live-count/
per-field pointer init. No scheduler/resource completion inferred. Marked local
EVT-01 portions complete; jobs/interpreter/source/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),125 anchors/
three resolved calls passes.6 wait/5 mask/7 state vectors and ordered job metadata
are deductions; no original code executes. Full fresh ELFs/listings stay `/tmp`.
Link audit768 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: synthAddJob/macro interpreter/wait-time producers, state/list/voice/
count/pointer providers and member/alias/IRQ/concurrency domains, original job/
macro/voice/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 stream kill/init and hardware break flags

Delivered [stream/hardware leaves](behavior/audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).
Four complete bodies pin64-record first-match kill/callback, configured-prefix
stream reset with fresh tail count and exact DSP break/active flags. Record
publication is separate from hardware/voice/audio completion. Marked local
EVT-01 portions complete; deeper providers/consumers/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),78 anchors/
one direct call passes.10 initialization/10 kill-route/16 break cases are derived
source checks. Full fresh ELFs/listings stay `/tmp`; no original code executes.
Link audit767 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: voiceUnblock/callback/stream-state/count providers, valid index/offset/
reset/concurrency domains, streamHandle/DSP consumers and original shutdown/audio
traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Original setup4 units, stage and Frankly battle introduction

[Native introduction](behavior/battle/NATIVE_SETUP4_BATTLE_INTRODUCTION.md)
imports original tik setup4 four-unit kind/part/group data: HP2 normal/spiked/
airborne Goombas, FranklyHP10, defense arrays and exact unit/part/counter flags.
Actual Frankly init15/entry7/phase39 commands run in a separate VM while the
original field scene remains suspended at battle_start. Source fresh lectureID6,
work0/maxmoves0,900msentry, counted45-step R_1 move, phase04000001/turn1 gate,
five original092_01..05 messages, status/camera requests and actor facing execute.
Native field-battle services publish sequence4/result0/controls-off; field
checkpoint/story stay unchanged. Full original scheduler/unit setup/player turns
are separate dependencies, not replaced by the Crump harness.

Imported original bti_01/stg_00_1 and source selected five object subtrees.
Recovered visibility excludes alternate A2/B geometry while revealingselected
isi1/isi2/B_mati descendants; default escape_wall/if_body/if_wire are hidden.
Authored ancestor matrices and source additionaltranslations remain; opaque/
translucent passes now filter separately. This fixes observed floor/beam occlusion
without invented actorY lifts. Source homes are Mario(-100,0,-5),Goombella
(-140,0,5); enemy positions retain grouprecords. Imported original PAD_Y_ON115
56x56icon; used atlas512x256/0.5MiB. Bound battle commonprint mode3 and used
one-shot toge selector1/mode0. Closing window continues after event completion.

Validation: final make test111 CTest/100Python and make all pass; separate
DEBUG_SHORT_BOOT artifacts/ttyd-trio-battle build passes. Tests check rawunits/
flags/stats, init/entry/firstturn/laterturn behavior, exact five messages,
status/camera/windowclose, retained field owner/control/result and checkpoint
coordinates. Final isolated Azahar artifacts/azahar-trio-battle-stage-final
captures original dialogue/cameras and complete intro; inspected cleanstage
shows allsix characters and explicit pendingplayercommands. Prior native capture
exposed whole-map visibility/pass occlusion and a generic dialogue fallback;
both were corrected. Initial092_05 parse failed for missing PAD_Y_ON, then used
source icon import fixed it. No user emulator profile/save touched.

Limits/next: bind original player target/Jump/Hammer/Headbonk graphs and private
pad/impact clocks, enemy turns, spike conditional092_06/07, airborne damage/fall,
result/loss/escape and source wait_battle_end return. Then execute original
093/094/094_01/story12 field continuation. Current introduction sequences
Frankly init→entry→phase; complete concurrent firstAct/phase scheduler, wait-event
and unit registries remain open. Stage spring scales/curtain/dynamic lifecycle,
full material/depth/lighting/shadows, camera defaults, effects/SFX and original
runtime traces remain partial. Source fixed-group RNG consumption and full
starting pouch/party setup are pending. Original setup3 lecture and ruins route
still required; fullprologue/hardware parity is not achieved. Goal remains active.

## 2026-10-13 — Priority0 sound-chain kill and voice allocator recycling

Delivered [sound/voice kill](behavior/audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
Two complete bodies pin external mapping/identity/captured successor, stale skip,
internal priority/free-list/count recycle and stream/hardware tail. Requested kill
and allocator state changes are separate from actual hardware/audio completion.
Marked local EVT-01 portions complete; child/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/4 linked),124 anchors/
five resolved direct calls passes.5 mask/4 counter/6 index/4 chain-route vectors
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original code
executes. Link audit766 Markdown files/0 broken; whitespace checks pass. Docs/
metadata only: no make rerun, native change or original body/asset export.
Isolated index excludes unrelated staged/worktree work.

Next/limits: map-reference/macro-inactive/stream/hardware children, identity/
priority/free/count providers and valid chain/index/member/IRQ/concurrency domains,
original note/voice recycle/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence initialization and free chains

Delivered [sequence free-chain init](behavior/audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md).
Complete seqInit pins empty active/paused roots,8-instance chain/IDs0..7/state0,
128 priority halfwordsffff,256 notes and next-ID0. Cross-record +18 writes are
next.previous within the20-byte note layout; payloads are not zeroed. Marked
local EVT-01 portion complete; constructors/voice/resource/runtime remain OPEN.

Validation: generic read-only verifierone body span/88 anchors/no calls passes.
Exact extents/chain metadata and emitted32*8 note-store simulation512 link
assertions/all writes within1400hex checked as deductions. Full fresh ELF/listing
stays `/tmp`; no original execution. Link audit763 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or
original body/asset export. Isolated index excludes unrelated shared work.

Next/limits: sequence constructors/ID/state/list/note/payload providers and MIDI
priority consumers, voice/resources/reset/concurrency and original initialization/
reuse/audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence validity and stop lifecycle

Delivered [sequence validity/stop](behavior/audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).
Four complete bodies pin active-before-paused key/private-ID lookup, validity
boolean, public IRQ wrapper and high-bit field-clear versus untagged state/note/
instance recycle. Source lacks private-index/unknown-state repair; no audible
validity guarantee. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (4 complete/5 linked),157 anchors/
8 resolved direct calls passes. Nine validity/five width/five state vectors and
original8-instance extent checked as deductions. Full fresh ELFs/listings stay
`/tmp`; no original execution. Link audit762 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: key/private-ID/state/instance/note/free/efc providers and valid index/
list/cycle/alias/concurrency domains, voice/resource/sequencer execution and
original validity/stop/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Source save lifecycle and original221-command event core

Commit `a0905a6` adds the portable source save-block callback, owned hit-name
cache and uint8 nested key leaves. The callback preserves exact stage0..4
request order, previous-published ratio/ordered NaN comparison, optional versus
system children, entry priority30/type26, distinct running predicates and
retained eventID. These mechanics are compiled for host/ARM and do not yet
change the Game's live save-block interaction.

Original system/wrapper/common-child arrays retain221 commands/14 callback
identities/89 sites in ignored local packs. The builder validates exact source
hashes and reads only DOL data spans/operand strings plus actual label-scanner
lookahead words. No original executable body is exported or dialogue invented.
The existing interpreter executes the original child/branch/switch/flag graph;
typed services preserve raw/resolved arguments, source speaker/coordinates,
ID-versus-raw controls, firstCall/yielding semantics and output references.
See [native core](behavior/field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md)
and [asset provenance](ASSETS.md).

Documentation inbox work recovered
[restart/event identities](behavior/display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md)
and [retained Continue/Add ownership](behavior/message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md).
Continue resumes/polls, not force-closes; raw scroll/once_stop appends preserve
the retained main window while selection has its own handle. Actual native
message/card providers remain next; test callbacks are not presentation/I/O.

Validation: final shared `make test` passes110 CTest/100 Python tests. Isolated
source/romfs snapshot `/tmp/pm-save-flow-jikuwdrm` passes107 CTest/100 Python
and `make all` after final request/move changes; concurrent underground work
is excluded from that snapshot. Original-data tests check No/success flags and
mode/story effects, exact ID/raw-control order, source attribution/arguments,
yielding header/write, all normal-mode error families, accepted format/create,
format/create/write failure repolls, guard break and moved runtime ownership.
Lifecycle tests cover suppression, callback stage/order, nested key wrap, NaN,
optional children and helper-versus-wait predicates. An initial test backend
accepted every later creation choice while retaining a persistent error and
entered the source retry loop; the owned process was stopped and test responses
corrected. Source loop was preserved. An earlier shared run failed a concurrent
unfinished underground test syntax error; the final shared run above passes.
Documentation links758 files/0 broken and whitespace checks pass. No emulator
run is claimed for this library-only milestone; current rendering is unchanged.
ISO remains read-only and compiled packs/assets remain outside history.

Next: bind actual source instances/head-name publication, independently timed
S_1/A_1 with Main publication after callback, original431 audio when available,
event manager before Mario, nested Game key/system/camera ownership, retained
message windows and original selection. Then connect a yielding native persistence
adapter and preserve GSF233/236/GSW4 alongside existing source flags. Avoid
force-close on Continue or an extra Continue on success; No must not write.
Source pause/category/camera consumers, shared field VM/state, full lifetime,
original execution traces, other areas/actors and hardware profiling remain open.
The full user goal remains active.

## 2026-10-13 — Priority0 MusyX listener update and cross/inverse math

Delivered [listener/cross/inverse](behavior/audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md).
Three complete bodies pin four-vector publication, cross-product order, stack
matrix layout, inverse cofactor/translation stores and low-byte volume/127.
No normalization, singular rejection or in-place snapshot guarantee added.
Marked local EVT-01 portions complete; numeric/source/consumer/runtime OPEN.

Validation: generic read-only verifier10 spans (3 complete/4 linked/3 literal),
229 anchors/six resolved direct calls passes. Derived stack/store maps and two
admitted matrix/one cross examples remain static deductions; no original code
executes. Full fresh ELFs/listings stay `/tmp`. Link audit761 Markdown files/
0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: listener/basis/allocation providers, null/nonalias/finite/invertible/
PPC numerical domains, CalcEmitter/consumer transforms and original geometry/
audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game effect main and outer sound updates

Delivered [effect/listener/completion main](behavior/audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).
Three complete bodies pin listener displacement/basis arguments, separate40-record
update/completion passes, captured traversal/fresh pool cleanup, double voice
checks/countdown continuation, exact sequence-valid1 and effect/DVD/SS outer
order. No measured velocity or physical audio cadence. Marked local EVT-01
portions complete; provider/child/runtime remains OPEN.

Validation: generic read-only verifier21 spans (3 complete/14 linked/4 original
literals),135 anchors/25 resolved direct calls passes.40 pool offsets/four countdown
boundaries/double-query metadata are derived source cases. Full fresh ELFs/listings
stay `/tmp`; no original execution. Link audit759 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or original
body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: listener update/transform and S position/heading providers, pool/
record/sequence/countdown writers and alias/concurrency invariants, DVD/SS/sequence/
controller/voice and original completion/audio traces. Continue priority0 source/
backend then priority1 damage graph. No hardware/playback/full-game completion.

## 2026-10-13 — Priority0 emitter vector/volume update and 3D reset

Delivered [update/reset](behavior/audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md).
Two complete bodies pin ordered two-vector copies, low-byte volume/127 target,
current-volume one-sided clamp and exact root/studio/flag reset. Update does not
check emitter membership; reset does not clear old emitter flags/voices. No
smoothing/cadence/release inferred. Marked local EVT-01 portions complete;
construction/consumers/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 IRQ/2 literal),47
anchors/two resolved direct calls passes. Whole-DOL direct census2 sites;8 volume/
7 flag/5 finite clamp vectors derived only. Full fresh ELFs/listings stay `/tmp`;
no original execution. Link audit758 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: emitter construction/list/vector/volume consumers, actual SoundEfxMain
provider/gate graph, listener/studio setup, alias/null/finite/reset/concurrency and
original update/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 emitter removal and FX unload lifecycle

Delivered [emitter lifecycle](behavior/audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md).
Three complete bodies pin active-system admission, exact neighbor/root unlink,
all-high16 flag clear, retained links/voice and key-off request. Bulk/FX-array
traversal preserves captured next and first-match break even without mutation.
Marked local EVT-01 portions complete; full voice/unload/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),117 anchors/
9 resolved direct calls passes. Whole-DOL aligned caller census9 sites;6 clear/
12 gate vectors checked as deductions. Fresh full ELFs/listings stay `/tmp`;
no original code executes. Link audit757 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: emitter construction/active/list/FX providers, member/cycle/array/
IRQ/concurrency source domains, full caller unload/silence and macro/voice/key-off
execution, original lifetime/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 effect spatial controls and emitter query leaves

Delivered [spatial/emitter leaves](behavior/audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md).
Five complete bodies pin early pan/surround record stores, fresh pool/voice lookup,
raw/controller width and two surround requests, retained volume read and active/
flag-gated emitter voice lookup. No rollback on voice miss or inferred audible
liveness. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier10 spans (5 complete/5 linked),75 anchors/
11 resolved direct calls passes. All256 surround mappings,9 raw-width and15
active/flag vectors checked; original functions not executed. Fresh full ELFs/
listings stay `/tmp`. Link audit756 Markdown files/0 broken; whitespace checks
pass. Docs/source metadata only: no make rerun, native changes or original body/
bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: effect-pool/emitter flags/voice providers and valid ordinal/null/
lifetime/concurrency domains, controller/MIDI/voice/backend services and original
pan/surround/fade/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 remaining positional SFX live/fade/mute graph

Delivered [SFX live/fade/mute](behavior/audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md).
Two complete regions pin global fade/percentage, spatial/nonspatial controls,
backend scaling, mute→fade→fresh stop/clear→final mute. Previous sweep/startup/
loop/tail plus this delivery close the local whole psndSFXMain body. Calls remain
ordered even when final mute follows key/engine clear. Marked local EVT-01
portions complete; backend/source/original audio remain OPEN.

Validation: generic read-only verifier18 spans (2 regions/15 linked/whole body
fingerprint),446 anchors/35 resolved direct calls passes.65536 division127 byte
products and65536 division100 unsigned×signed byte products checked;6 fade wrap/
decrement vectors are derived only. Fresh full ELF/listing remains `/tmp`;
no original code executes. Link audit754 Markdown files/0 broken; whitespace
checks pass. Docs/source metadata only: no make rerun, native changes or original
body/asset export. Isolated index excludes unrelated shared work.

Next/limits: SS/song/effect child controls/stop/query semantics, G/slot/descriptor
writers and flag/fade/denominator/alias/concurrency/lifetime domains, original
live/fade/audio observations. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 listener setter and sequence camera source bindings

Delivered [listener/camera source](behavior/audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md).
Two complete bodies/two scoped argument regions pin alias-sensitive XYZ stores,
heading publication, unchecked camera table lookup, field position/repeated-heading
queries and battle fixed XYZ/flag100 Z adjustment. Exact direct argument bindings
are closed; whole camera/sequence gates/cadence remain open. Marked local EVT-01
portions complete; parent OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 scoped/1 linked/
1 original literal),46 anchors/8 resolved direct calls passes. Whole-DOL aligned
setter census2 sites; original constants0/400/200 decoded. Alias example is a
static deduction, not original execution. Full fresh ELFs/listings stay `/tmp`.
Link audit752 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original code-body/asset export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: camera pointers/position/heading producers and finite/null/source
invariants, whole sequence admission/cadence, battle flag/shared-result writers,
indirect/REL/dynamic routes and original spatial/audio traces. Continue priority0
live SFX/source then priority1 damage graph. No hardware/full-game/audio claim.

## 2026-10-13 — Original underground approach, dialogue and Goomba animations

[Native trio scene](behavior/story/NATIVE_UNDERGROUND_TRIO_SCENE.md) runs actual
161-command gorotuki_sanninngumi and10-command footstep child in the existing
VM. Story<=11 roomrestore admits sourceinitial placement/flags and MarioX>=-100
crossingpoll, then original keys/cameras/party positioning and seven exact
086..092 messages with correct named speakers/Goombella suffix. Parallel actor
run/hop/move blocks preserve original local/shared event IDs. Imported actual
c_kuribo KUR/PTK/TGK stay/talk/walk/run/damage sparse poses and Marioe_mario
M_N_7. Native citro3d batches draw allthree variants; Frankly now follows/renders
as temporary partner underground and accepts original party1 placement.

Scoped source callbacks cover facing towardNPC, Mario/party positions and NPC
run/damage poses. Original script requests battleinfo4 and remains atbattle_start
until its actual backend exists; no postbattle messages/story12 are synthesized.
Documentation inbox delivered full setup4 contract: threeHP2 variants PLUS
Frankly coordinator, five firstturn messages and two conditional spike messages,
variant weapons/flags/counter/fall and source wait/result ownership.

Validation: staged source snapshot /tmp/pm-underground-trio-dbe8scf9 passes
make test108 CTest/100Python. Shared make all and separate DEBUG_SHORT_BOOT
artifacts/ttyd-underground-trio build pass; focused underground/professor tests
pass. Host original-data checks pin placements, passive poll, inclusive crossing,
message order, source pose/party positions, four cameras, setup4 and retained11.
Isolated Azahar artifacts/azahar-underground-trio captures original086, allthree
variants/Frankly, camera/dialogue pages and final pendingbattle notice; images
inspected. It is manual presentation evidence, not a full nativebattle pass.
Initial unusedZ_2 imports rejected non-endpoint topology, so only actualused
poses are admitted. InitialM_N_7 bank assumption rejected; corrected to original
e_mario. First shared make test encountered an unrelated build-host concurrent
archive race; isolated staged build excludes unfinished concurrent save-event
work and preserves its files. No user emulator profile/save touched.

Limits/next: bind setup4 battle/coordinator and original attack/counter/fall/result
services, then original093/094/094_01/GSW0=12 continuation. Full party stop/run,
force-restart pose clocks, NPC terrain/ease/hop arithmetic, effects/SFX, source
cadence, original lighting/material/shadow sorting and original-runtime traces
remain incomplete. Source first-arrival main map services/dynamic hit toggles and
actual pipe BOTH-party restoration also remain partial. Live scene saves remain
suppressed because native event state is not serialized. Complete setup3 lecture,
prologue route/ruins and New2DS hardware validation still required; goal active.

## 2026-10-13 — Priority0 calc3D spatial result and angle table

Delivered [calc3D/angle-table contract](behavior/audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
Two complete bodies pin flag/capture precedence, original91-row angle table,
literal196 pan fold, PPC sqrt refinement/cos/sin surround, distance falloff and
byte-first volume clamp. Shared output preserves byte3. No host math replacement,
measured numeric/audio behavior or guessed listener invariant. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (2 complete/2 linked libm/1 table/
4 literal spans),161 anchors/four resolved direct calls passes.91 original table
rows/literal bytes checked;15 signed fold vectors remain static arithmetic.
Full fresh ELFs/listings stay `/tmp`; no original execution. Link audit750 Markdown
files/0 broken; whitespace checks pass. Docs/source metadata only: no make rerun,
native changes or original body/bank export. Isolated index excludes shared work.

Next/limits: listener/shared-byte source writers, valid heading/flags/finite and
PPC fused/frsqrte/fctiwz/libm domains, live SFX/backend/original spatial/audio
traces. Continue priority0 live/source owners then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 audio frame and aux callback order

Delivered [frame/aux graph](behavior/audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
Seven complete bodies pin one-time active gate, DSP submission before five
seq/synth slices, frame-byte/live voice clears, final3D/stream/virtual-sample
order and direct OS critical wrappers. Aux A/B callback/buffer capture and fresh
B gates/count reads remain explicit; no measured physical cadence/action ticks
inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier17 spans (7 complete/10 linked),127 anchors/
29 resolved direct calls passes. All256 byte arithmetic cases, five-slice order,
three-channel extent and6 aux-gate vectors checked; no original execution. Full
fresh ELFs/listings remain `/tmp`. Link audit748 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change
or original body/bank export. Isolated index excludes unrelated shared work.

Next/limits: seq/synth/stream/3D/virtual-sample/command-builder engines, actual
voice/studio/aux providers and pointer/format/IRQ/reentrancy domains, original
frame/DMA/aux/audio observations. Continue priority0 source/live SFX/calc3D then
priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 DSP task callbacks and command-mail source

Delivered [DSP task/callback/mail](behavior/audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md).
Six complete bodies pin task publication/init wait, distinct ready/init/resume/
thread-wake callbacks, pending-wait consumption before active gate, command
pointer/size captures, two-mail polls and raw wrapping clock arithmetic. Original
DSP program identity is pinned without decoding/export. Marked local EVT-01
portions complete; parent and actual audio execution remain OPEN.

Validation: generic read-only verifier21 spans (6 complete/13 linked/2 original
program and length),98 anchors/16 resolved direct calls passes. Length1ec0 equals
program span;9 resume-gate/5 mail/6 clock-domain vectors are derived source checks,
not original execution. Fresh full ELFs/listings remain `/tmp`. Link audit747
Markdown files/0 broken; whitespace checks pass. Docs/source metadata only: no
make rerun, native change or original DSP/bank/code-body export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: SDK/DSP task/mail execution, command builder/control/voice/frame
engine and state/thread/clock/pointer/concurrency domains, original startup/resume/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Original east pipe and first underground map

[Native underground route](behavior/story/NATIVE_UNDERGROUND_PIPE_ROUTE.md)
packages original gor_02/dokan↔tik_01/dokan_2 records and hit centers/identities.
Case6 retains strict signedmainY>30/contact gating; case10 uses frontcontact,
without A. Original11-command gor pipe departure executes in the existing VM,
sets actual GSF1207, preserves GSW0 and retains party requests. Already-setflag
returns without repeat requests. Expanded metadata4 contains329commands and
64callbacks. Native transactional map staging reaches original tik_01 as room5,
with original geometry/textures/collision/camera and source underground music.
Prologue zero-flag wall display hides are extracted; source hit toggles remain
unbound. Source horizontal endpoint20/start-40 offsets replace provisional0.65.

Inbox deliveries recover exact pipe event/geometry and full161-command Goomba
trio/init/arrival contracts. Original scene waitsMarioX>=-100, uses three actual
c_kuribo tribes and original086..094_01 payloads (last Frankly text in tik_04),
requires battle setup4 and only then advancesstory12. This room import/route
preservesstory11; binding that event/actors/battle is the next milestone.

Validation: isolated staged implementation snapshot /tmp/pm-east-pipe-1mezum8k
passes make test107 CTest/100Python after final pipe ground-query change. Shared
make all and separate DEBUG_SHORT_BOOT artifacts/ttyd-east-pipe build pass.
Focused original pipe and professor tests pass. Initial new pipe landing query
failed because ordinary groundAt has a narrow support window; corrected to
source-start downward-ray bridge. Shared make test initially hit unrelated
in-progress save-block event assertion, then hung in that test; that own run was
stopped and is not reported passing. Snapshot excludes that concurrent unfinished
feature and retains its files. Isolated final Azahar artifacts/azahar-east-pipe-final
passes contact/stick travel to underground, save(room5/story11/flag1), reverse
frontcontact return and save(room2/story11/flag1). Both map captures inspected;
Frankly is correctly hidden on east-room return. Repro fixture tool is
make_east_pipe_fixture.py. No user emulator profile/save touched.

Limits/next: case6 Mario+1e8 producer is not proved identical to native ground
contact; source mode/Vivian/nonride/contact lifecycle, pipe paper poses/SFX/camera
and BOTH-party restoration remain incomplete. Direct emergence/fade and ground
support are native adapters; temporary Frankly underground rendering is pending.
Bind original Goomba trio scene/setup4 and later badge/door/ruins route, actual
lecture setup3, dynamic wall flags/collision and original environment/reverb.
NDSP streams original underground music; audio/device and New2DS performance
were not measured here. Full requested prologue/full-game fidelity remains open.

## 2026-10-13 — Priority0 AI buffers, callback and IRQ initialization

Delivered [AI/IRQ/compressor source](behavior/audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md).
Six complete bodies pin initial IRQ ownership, four-slot allocation/null failure,
state/registration/initial DMA/rate publication, callback rotation/tick-before-gates,
OS enable/dispatch/disable ordering, CPU destination and compressor flag. These
are local source requests, not measured DMA/frame/audio behavior. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier15 spans (6 complete/9 linked),83 anchors
and11 resolved direct calls passes. All256 byte-index arithmetic cases, exact
four-slot extent and9 DSP/logic gate vectors checked; original code not executed.
Fresh complete ELFs/listings remain `/tmp`. Link audit746 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native changes or
original code-body/bank export. Isolated index excludes unrelated shared work.

Next/limits: DSP/control/frame callback services, actual state writers and SDK/
physical coherence/alias/concurrency/reinit domains, original interrupt/callback/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game allocator providers and MusyX bootstrap

Delivered [game allocator/bootstrap](behavior/audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Four complete bodies and scoped SoundInit startup prefix establish original
hook table/providers, fixed game initialization inputs, low-byte configuration,
hardware admission/failure and successful default-cache/resource/child init order.
Game prefix ignores sndInit result; no child failure unwind or residency/playback
proof inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier29 spans (4 complete/1 scoped prefix/
1 original hook table/23 linked),134 anchors/25 resolved direct calls passes.
Independent8-byte table matches actual provider symbols; nine derived clamp/width
vectors checked. Full fresh ELFs/listings remain `/tmp`; no original execution.
Initial link audit742 Markdown files/0 broken; later shared audit743 files found
one unrelated new underground link pending. All6 delivery Markdown files have
0 broken links; whitespace checks pass. Docs/metadata only:
no make rerun, native change or code-body/bank export. Isolated index excludes
unrelated worktree/staged work.

Next/limits: whole SoundInit effects/loader/settings tail, child OS/AR/AI/DSP/IRQ/
frame/seq/synth/stream/virtual-sample/3D, provider/size/reset/concurrency/failure
and original startup/audio traces. Continue priority0 source/live SFX/calc3D,
then priority1 damage graph. No hardware/audio/full-game completion claim.

## 2026-10-13 — Priority0 default cache, resource reset and hook installation

Delivered [cache/reset/hook producers](behavior/audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md).
Seven complete bodies pin gsDefault base/end/cursor and root/current publication,
six U16 registry-count resets, whole macro main-table clear, second-argument-only
ARAM initialization, ordered hook copy, ordinary allocator and callback forwarding.
Reset counts do not erase all registries or prove stopped voices; alias-sensitive
hook copy is preserved. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (7 complete/2 linked documented
children),87 anchors and4 resolved direct calls passes. Four derived cache/wrap
vectors,800hex macro-clear extent and3 hook-alias vectors checked. Full fresh
ELFs/listings remain `/tmp`; no original code executes. Link audit741 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original code-body/bank export. Isolated index excludes unrelated work.

Next/limits: actual setup callers/B/N/allocator/callback providers, per-kind
registries, alias/reset/concurrency/residency domains, voice/macro/MIDI/SDK and
original setup/audio/runtime traces. Continue priority0 source and live SFX/calc3D,
then priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 generic ARAM upload and hardware stream bridge

Delivered [upload/stream bridge](behavior/audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).
Five complete bodies pin raw upload/full-word priority normalization/user callback,
high/low queue post-before-count/index and hardware stream offset/length alignment,
DCStoreRange and exact init/exit/address delegation. The flush ignores returned
active size; play-buffer wrapper reads an address, not hardware playback position.
Marked local EVT-01 portions complete; full stream/source/runtime remains OPEN.

Validation: generic read-only verifier16 spans (5 complete/11 linked),114 anchors,
14 resolved direct calls passes. Independent whole-DOL aligned caller scan25
sites (17 Flush/1 Get in streamHandle,1 Init in sndStreamAllocEx,5 Exit in
sndStreamFree,1 upload in Flush), each instruction anchored. Eight priority and
42 offset/count vectors remain arithmetic deductions, not original execution.
Full inspection ELFs/listings stay `/tmp`. Link audit739 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change,
original body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: actual streamHandle/AllocEx/Free caller ownership, source/providers,
valid callback/index/count/offset/extent/lifetime/concurrency domains, OS/cache/
ARQ/DMA and original audio traces. Continue priority0 source/live SFX/calc3D
owners, then priority1 damage graph. No playback/hardware/full-game completion.

## 2026-10-13 — Original lecture choices, talk requests and story11 reload

[Native lecture binding](behavior/story/NATIVE_LECTURE_CHOICES_AND_RELOAD.md)
executes original selection/add/partykill/setup requests. Declining practice via
source results0/1 advances GSW0 to11 and retains the original question window
across084_03/084_04. Both accepted branches preserve Goombella NPC attribution,
visibility/hop and setup3, then stop at the unbound original battle_start.
Source choice opening/closing, press/repeat/cancel/result semantics and separate
window geometry run with original US text. Imported original STICK/cursor pixels
and frame delays; sound requests remain unbound. Glyph-driven talk countdown
replaces window-active pose inference; added original Goombella PKR_T_1.

Story10 reload admits a fresh lecture crossing poll; story11 reload restores
Frankly's source outside position/yaw or flag1207-dependent hidden state.
Save version5 retains actual GSF1207 in byte34 and reads native versions1..4.
Documentation inbox deliveries cover speaker talk/stay ownership, exact flag
and map reentry, NPC visibility and distinct lecture backend ownership.

Validation: make test passes106 CTest/100 Python tests; make all passes. Separate
DEBUG_SHORT_BOOT build-lecture-verify/artifacts/ttyd-lecture-choices build passes.
Tests exercise accepted and declined original branches, retained append,
selection gates/geometry, source icon records, glyph countdown, story10 reentry,
story11 visible/hidden restore and save-v5/legacy compatibility. Isolated Azahar
final run artifacts/azahar-lecture-choices-final passes both choices and verifies
saved originalStory11; captured original windows and returned field inspected.
Separate story11 reload capture artifacts/azahar-lecture-story11-reload/boot.png
shows Frankly outside with ordinary field restored. A prior OCR attempt failed missing English data/incompatible older data, then
was replaced by pixel window detection plus independent native save assertion.
An earlier native pixel run also passed. No user emulator profile/save touched.

Limits/next: implement actual setup3 teaching battle/coordinator and field return,
original pipeout/GSF1207 writer/underground maps, complete NPC/party conversion,
force-restart pose ownership, icon transforms/depth, original selection sounds,
GX fence capture and source event/animation cadence. Native renderer uses source
assets through citro2d/citro3d; physical New2DS performance and full prologue
parity remain unverified. The full-game/prologue goal remains active.

## 2026-10-13 — Priority0 ARAM initialization and stream-buffer owners

Delivered [initialization/hooks](behavior/audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md)
and [stream-buffer lifecycle](behavior/audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md).
Eight complete bodies pin zero allocation/fill/post/wait/free, limits/queue resets,
hook requests,64-descriptor idle chain, cached/downward allocation, low-byte
getter/default pointer and exact free traversal. Larger-candidate predecessor
and global-head cleanup writes are preserved; source list/order/membership
invariants remain required. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
251 anchors and14 independently resolved direct calls passes. Derived64-node
chain,8 alignment,7 index and7 cached-search vectors accompany full listings;
no original functions execute. Fresh complete ELFs/listings remain `/tmp`.
Link audit738 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native changes or original bank/code-body export. Isolated index
keeps unrelated staged/worktree changes out of this commit.

Next/limits: actual default-cache and allocator-hook producers, valid stream-list/
cycle/predecessor/order/membership domains, upload/SDK/OS/ARQ/DMA and original
sample/voice/runtime traces. Continue priority0 source/backend and live SFX/calc3D
owners, then priority1 damage graph. No original playback/hardware/full-game claim.

## 2026-10-13 — Original object top/side collision and supported checkpoint restore

Commits `4e12f64` and `1d853a6` bind the unchanged original12-face save-block
cube to top support, ordinary Mario forward/around wall queries and checkpoint
validation. PMNPC004 retains original model names so the source hidden-model
floor/wall exclusions remain explicit; older catalogs remain loadable. Dynamic
floor/wall owners stay separate from static REL offsets/array indices. Existing
five-foot support, two-height wall rays, source corrections and landing response
remain shared. Static object geometry caches catalog generation and room/story/
interior/system admission, avoiding rebuilds across unchanged queries/frames.
Successful catalog replacement invalidates reuse; failed loading retains it.
See [native collision](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md).

Source documentation through OW-INTERACT-01 establishes
[ordinary Goombella save-cube admission](behavior/partner/FIELD_PARTNER_SAVE_BLOCK_COLLISION_BINDING.md),
including distinct floor/front/around filters and tie rules. Goombella's current
native floor adapter includes the cube; its complete original wall driver remains
unbound. The file menu/reload path previously rejected on-block checkpoints via
static-only validation. Both now use the same object-aware candidate service
and existing transactional area replacement. Native save elevation quantization
stores the top fixture as.9, then source support resolves actual top.85.

Validation: final isolated source/romfs snapshot
`/tmp/pm-object-surfaces-mkwzez51` passes `make test`104 CTest/100 Python tests
and `make all`. Original-data tests check top/side normals/backfaces, dynamic
owner metadata, hidden-name floor/wall rejection with head geometry retained,
standing, full top jump/landing/walk-off, malformed V4 name atomicity and cache
invalidation after catalog replacement/system gating. The final checkpoint
regression is covered by that complete run. An initial Azahar top capture
exposed static-only checkpoint rejection; it was corrected before acceptance.
Immutable final binary `artifacts/ttyd-saveblock-checkpoint.3dsx` loads the
`--on-block` fixture in isolated `artifacts/azahar-saveblock-top2`; inspected
`saveblock-head-landed.png` shows Mario on the original block after jumping.
No user emulator profile/saves touched. These host/emulator checks establish
this native slice, not matched original execution or New2DS performance.
Documentation links731 files/0 broken before the handoff; whitespace checks pass.
Unrelated concurrent lecture/message/save/audio changes remain outside these commits.

Next/limits: connect source name-contact cache and save callback0..4 with
independently timed S_1/A_1 poses, previous-published loop ratio, nested keys,
system/camera ownership, original US save choices and a native persistence
adapter retaining source switch namespaces. Hit animation/prompt remain unbound.
Source partner body-wall driver, moving objects, dynamic shadow queries,
multiple-map-record hit ordering, full area/event content, original lighting/
translucent ordering and hardware profiling remain open. Keep the full goal active.

## 2026-10-13 — Priority0 ARAM sample store and queue completion

Delivered [sample store/cursor/configuration](behavior/audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md)
and [queue completion/synchronization](behavior/audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
Eight complete local bodies pin32-byte wrapping alignment/capacity, direct versus
callback chunks, retained return/fresh destination/cursor, post-before-count/index,
full callback scan and low-only busy sync. Removal only rewinds caller cursor,
ignoring the supplied address. No timeout, invalid-completion guard or fabricated
free/voice-stop semantics. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
207 instruction anchors and16 independently resolved direct calls passes.
Derived8 alignment/rewind vectors,256 byte-index arithmetic cases and5 callback
count boundaries checked; these are static arithmetic evidence. Fresh complete
inspection ELFs/listings remain `/tmp/inbox-aram*`. Link audit734 Markdown files/
0 broken; whitespace checks pass. Documentation/source metadata only: no make
rerun, native changes or original code-body/bank export. Isolated commit index
excludes unrelated staged/worktree native work.

Next/limits: aramInit/stream/cache producers and other queue upload paths, actual
callback providers/removal order/queue concurrency, SDK flush/interrupt/ARQ/DMA,
voice/macro/MIDI/resource ownership and original audio/runtime traces. Continue
priority0 initialization/source and live SFX/calc3D, then priority1 damage graph.
No hardware/original playback/full-game equivalence claim.

## 2026-10-13 — Priority0 sample directory lifetime and storage bridge

Delivered [directory/reference lifetime](behavior/audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
and [sample hardware bridge](behavior/audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md).
Eight complete local bodies pin preferred-pointer publication, duplicate/capacity
behavior, mutable low16 refs and resolved addresses, acquisition preconditions,
release/removal/compaction and raw hardware/cache requests. First acquisition
requires a matching preferred inactive candidate; no source not-found fallback.
Encoded transfer sizing is proved over the24-bit count domain; it is not audible
length or successful residency. Marked these EVT-01 portions complete; parent OPEN.

Validation: read-only generic verifier13 DOL spans (8 complete/5 linked),256
anchors,10 independently resolved calls and1 original directory resource hash
(no map nodes) passes. Independent original .sdir parse1247 records, all refs0/
formats0; Hammer775/776 source transfers3016/872 bytes.54 derived boundary vectors
and exact-domain arithmetic proof remain static evidence. Fresh full ELFs and
listings remain `/tmp`. Link audit731 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make test/all rerun, native changes or original
asset/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: ARAM store/remove/sync/cache/allocation/DMA services, per-kind pool
registries, bank/source initialization, macro/voice/MIDI stops and actual runtime
residency. Continue priority0 audio source owners and live SFX/calc3D requests,
then priority1 damage/counter graph. No original audio/hardware/full-game claim.

## 2026-10-13 — Original map-object bbox head collision

Commit `12d4b56` connects the original save-block cube to the existing native
five-probe head query and ceiling response. PMNPC003 adds original model bbox
metadata and source object names while retaining PMNPC001/002 loading. The
converter reads original model0xd0..0xe4; source dimensions, forcedY0 midpoint,
eight vertices/six alternating strips, source cull1 and owner attribute are
retained. Dynamic owner keys remain separate from static REL node offsets.
Actual contacts publish the owner; near misses/backfaces do not. Original far
ray endpoint rejects and equal hits retain the earlier static/object slot.
Reused per-Game storage bounds the current plaza object to12 triangles.
See [native binding](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md) and
[asset provenance](ASSETS.md).

Documentation inbox work recovered the
[original shared hit registry/order](behavior/field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md):
source map/slot traversal, dynamic insertion, strict upward ties/endpoint,
whole-box ground/wall eligibility and borrowed name/delete lifetime. Current
native packs cover one map record; multiple-record chain reconstruction remains
explicitly pending. Model draw scalar and S_1/A_1 deformation do not change the
source collision cube.

Validation: independent source/romfs snapshot
`/tmp/pm-object-collision-er9tx9p7` passes `make test`104 CTest/100 Python tests
and `make all` after the final endpoint change. Tests cover source bounds,
12 faces/bottom winding, head actual/near-miss/backface/far-endpoint queries,
room/story/system gates, malformed V3 atomicity and a full Game jump that
publishes the owner, clamps, falls and lands. Initial shared full run failed an
unrelated concurrently modified professor lecture assertion; it is not reported
as passing. Snapshot excludes that unfinished lecture/choice work and preserves
its shared edits. Immutable binary `artifacts/ttyd-saveblock-head.3dsx` boots
with `--under-block` fixture in isolated Azahar; inspected contact/landed images
are `artifacts/azahar-saveblock-head/saveblock-head-contact.png` and
`saveblock-head-landed.png`. The images support presentation/landing, while
host assertions establish the collision owner/correction. Emulator samples
are not stable hardware performance evidence. No user profile/saves touched.
Documentation link check727 Markdown files/0 broken; whitespace checks pass.

Next/limits: bind this same source cube into top support and side-wall queries;
connect name-contact caching and callback0..4 with independently timed poses,
previous-published loop ratio, nested key/system/camera ownership, original
save-choice graph and explicit native card persistence adapter. Hit animation
and save prompt are not yet triggered by the new collision. Moving object
transforms, full event/area content, source observations, original lighting/
translucent ordering and New2DS profiling remain open. Full user goal stays active.

## 2026-10-13 — Priority0 group lifecycle and game audio loader source bridge

Delivered [complete group push/pop/resource lists](behavior/audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md)
and [game warm-up/loaded-bank/free](behavior/audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
Group push publishes stack pointers before resources, preserves typed pool/list
ranges and FX insertion; pop decrements first, requests voice stops, removes
samples backward and remaining lists forward. Game explicit-list push counts
attempts while sequential mode counts exact successes; warm-up ignores push
returns. Ready and return values are not proof of actual residency. Marked those
local EVT-01 portions complete, leaving child/source/runtime ownership open.

Validation: generic lifecycle manifest29 spans (6 complete/23 linked),476
anchors,56 resolved direct calls and1 original project resource hash passes
(no map nodes). Independent project parse12 groups0..11/2998 IDs; reverse
samples match forward reversal in all12; group6 contains Hammer775/776 samples
and994/995 macros. Fresh complete ELFs/listings and derived report remain `/tmp`.
Link audit726 Markdown files/0 broken links; whitespace checks pass. Docs and
metadata only: no make rerun, native code or original asset/code-body export.
Unrelated staged/worktree edits remain excluded from this commit.

Next/limits: sample-directory/reference/per-kind pool registry producers,
actual file/sample/project/bank initialization and residency, voice-stop/MIDI/
macro/OS/hardware/source/runtime still open. Continue priority0 these source
owners and calc3D/live audio/query bindings, then priority1 damage/counter graph.
No original executed/audio/hardware/full-game completion claim.

## 2026-10-13 — Priority0 FX registry insertion/removal and live resource mutation

Delivered [complete FX registry producers](behavior/audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md).
First insert references the caller's10-byte entry array and writes every byte9
to31 before publishing the fresh group count. Duplicate registration retains
existing pointer/count, increments16-bit refcount and returns0; full refusal
also returns0. Removal decrements refs, compacts12-byte records when zero and
leaves unused tail/entry arrays/voices untouched. Linked synth-start extra
operand therefore comes from live-mutated data, not immutable .proj bytes.
Marked the local EVT-01 producer portion complete; no full residency claim.

Validation: fresh complete two-body listings under `/tmp`; generic registry
manifest5 fingerprints/87 anchors passes. Derived reference boundary metadata
retains0/1/2/65535 wrap. Link audit722 Markdown files/0 broken links; whitespace
checks pass. Docs/metadata only; no make rerun, native/resource conversion or
original ISO/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: sndPushGroup/sndPopGroup and actual project residency, sorting,
resource/voice lifetime on unload, macro/synthStartSound/controller mutations,
calc3D/live audio and original scheduler/encounter/runtime remain open. Continue
priority0 source/core/query binding before lower-priority integration. No original
executed/audio/hardware/full-game equivalence claim.

## 2026-10-13 — Original save-block surfaces and material passes

Commit `c3e36cb` adds the original plaza `mobj_save` presentation through the
shared per-area catalog. PMNPC002 retains original `(185,60,-55)`, scalar10,
S_1, story341 exclusion and constructor system mask0x20. PMANIM4 preserves
original indexed RGBA, untextured draws, discrete texture shifts, material wrap
overrides, source pass/cull and nonzero animation start. Portable triangle
partitioning preserves texture/color interpolation across clamp/repeat/mirror
boundaries. Citro3d submits original opaque/translucent admission and flushes
CPU-written linear vertex data. PMANIM1/2/3 and PMNPC001 remain loadable.
Conversion/resources stay local and ignored; provenance/rights are in
[assets](ASSETS.md) and [surface format](behavior/display/NATIVE_ACTOR_SURFACE_PACK.md).

Documentation work through OW-INTERACT-01/SAVE-01 recovered
[material/UV binding](behavior/display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md),
[head cube and callback lifecycle](behavior/field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
and the [complete common save event](behavior/field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
These establish original contact, five-state callback, previous-published
animation ratio, key ownership, exact original choices, flag namespaces,
cancellation and card error/retry requests. Story text is retained from
msg/US; no replacement dialogue is introduced.

Validation: shared-worktree `make test` passed103 CTest/100 Python tests and
`make all` built the native3DS binary before concurrent fence changes. Isolated
feature snapshot `/tmp/pm-object-verified-j_rg9rov` independently passed103
CTest/100 Python tests and `make all`; its source/romfs were isolated from later
concurrent edits. Dense barycentric tests check all nine wrap combinations,
geometry area and RGBA interpolation; original S_1/A_1 tests check discrete
scrolling, timing, pose wrap and malformed-pack retention. Idle peak402
vertices. Its immutable binary `artifacts/ttyd-saveblock-verified.3dsx` passed
isolated Azahar capture `artifacts/azahar-saveblock-verified2`: original colored
block and animated glow were inspected in the plaza. An earlier3-second cold
launch stopped at title; the successful12-second startup reached the field.
Single emulator HUD sample60fps/sim3.43ms/CPU8.3ms/GPU0.5ms is not a percentile
trace or hardware-performance result. Documentation links721 files/0 broken;
whitespace checks pass. User emulator profile/saves remain untouched.

Limits/next: bind the recovered source collision cube and head-name cache,
then the hit lifecycle with independently timed per-instance poses and original
common save event/card adapter. Save-block hit/save interaction is not yet
connected. Original lighting, global translucent sorting, NPC collision/search,
wandering, area event completeness and New2DS hardware profiling remain open.
Keep the full user goal active; this is an incremental Rogueport milestone.

## 2026-10-13 — Priority0 synth FX resource and chained control cores

Delivered [complete synth request cores](behavior/audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md):
loaded-group/shared-scratch lookup, comparator/binary search, default255 and
all16 sound-start operands, voice-generation checks and fresh chain reloads.
Controller cores stop at stale identity; active-gated KeyOff skips stale nodes
and continues. Returns report requests rather than confirmed hardware changes.
Marked the local EVT-01 portion complete; registry/macro/voice/MIDI/resource/
source/runtime dependencies remain explicit and open.

Validation: generic musyx_synth_effect_core_evidence.json verifier12 spans
(8 complete/4 linked identities),192 anchors and independently resolved15 direct
call sites pass. Fresh new full body inspections remain `/tmp`; existing complete
voice lookup identity is revalidated. Link audit719 Markdown files/0 broken
links; whitespace checks pass. Docs/metadata only: no make rerun, native or
original asset/code-body export. Unrelated staged/worktree edits excluded.

Next/limits: FX registry insertion/removal and project residency, actual
synthStartSound/MIDI/external-keyoff/voice-chain producers, calc3D/live audio and
original scheduler/encounter/audio observations remain open. Continue priority0
producer/source/query binding, then priority1 consumer/counter graph. No original
executed/hardware/full-game fidelity claim.

## 2026-10-13 — Original house-exit fence event and live lecture entry

[Native fence/lecture entry](behavior/story/NATIVE_PROFESSOR_FENCE_AND_LECTURE_ENTRY.md)
executes the original63-command wall-removal event through its story9 close-door
contact+A callback. Door-end wait, temporary-party-to-NPC conversion, original
083/084 messages, cameras/movement/hops and source GSW0=10 run in the existing
VM. Expanded original REL catalog preserves318 commands/63 callbacks across
first-talk, fence and lecture. The source lecture crossing poll remains live
with field controls enabled, then runs084_01/084_02 through its original pending
choice service. Added source `<o>`/once_stop retained-window behavior.

Imported actual OFF_d_saku A_1 nonloop140-frame deformation/seven keys. Native
capture bounds unblock the original integer/aspect commands; original78 fence
vertices/materials deform in a frozen camera-plane adapter. Original map hide
and image display ownership remain distinct from actual A_saku_04 collision
removal. Source story>=10 restores collision removal on reload. Saves/exit
saves remain suppressed while the live event lacks persistence. Asset paths,
adaptations and rights are recorded in [assets](ASSETS.md).

Validation: final shared-worktree `make test` passes104 CTest/100 Python tests;
`make all` builds the3DS binary. Separately exported staged source compiles
host/native and passes102 CTest tests after regenerating its field actor format
in `/tmp/professor-fence-stage`; workspace assets are untouched by that rebuild.
Original-data tests complete first-talk, walk to the close-door contact, wait
for capture/animation, compare collision before/after, reach story10, walk the
opened gap and reach the pending original lecture choice. Independent23-paper-
animation samples pass0.00025 local-unit tolerance, max error0.000213623; the
initial generic0.0002 check rejected that large-fall sample, and ordinary default
tolerance remains unchanged. Key/boundary interpolation sampling was expanded.

The first Azahar run overflowed the native checkpoint-restore stack. Main game,
validation/warp candidates and full reset temporaries now use heap storage;
retry `artifacts/azahar-professor-fence-heap/` restores the checkpoint and shows
original083/084, fence gap and restored controls without that exception. Native
`artifacts/azahar-professor-fence-gap/` replay walks through the gap and reaches
original084_02 retained text and the pending choice service. Earlier straight
replays hit the pipe; corrected navigation uses the opened gap. Initial staged-tree
compile raced the file export and lacked main; rerun after export completes
passes. No physical3DS or original runtime trace was obtained.

Next/limits: bind original selection/add services, both lesson branches, actual
lecture battle/return and story10 reload admission. Source offscreen/GX capture,
rectangle clipping/depth/shadow, image clock and SFX/backend/flag consumers are
still partial; current map-material deformation is an explicit handheld adapter.
Earlier harbor/prologue gaps, temporary-party persistence and hardware
performance still prevent full playable prologue parity.

## 2026-10-13 — Title/file menu and fresh-file opening order adapter

Normal boot now plays the original storybook/market demonstration, title and
four-file menu before starting an empty file's opening. Native controllers own
input separately from field mechanics. A/START skips the demo to title and
opens file selection; D-pad/touch chooses a file, B returns to title. Original
US messages are preloaded and parsed before the new-file sequence starts:
37 demo payloads and11 opening payloads preserve IDs/raw bytes/control tokens
and explicit speaker operands. Diary sets source type7. New-file order is
house → letter → house return/map display → voyage/input gate → original
prologue heading → awakening → arrival → saved harbor story3 at(0,15,225).
Map display retains the source +2/clamp255 counter over128 simulation updates;
source GSW0 writes2/3 are explicit. Existing original saves are not a priority
per the user's steering; slot0 keeps the existing path as a low-cost convention.

Original title/desk/book/letter/map/prologue raster assets, house/voyage/wake
geometry, four character banks and six music streams are converted locally.
House outside/interior snapshots use distinct visibility roots; Luigi retains
idle/talk/read clips. Front-end caches release before field loading and empty
actor-atlas rows are cropped with checked UV scaling. No generated art, rewritten
story dialogue, synthesized tones or redistributed ISO assets were added. See
[asset choices](ASSETS.md) and [opening contract](behavior/story/NATIVE_OPENING_FLOW.md).
`make debug-short` creates a separate `ttyd-2ds-debug-short.3dsx` and build directory
that starts in the prior checkpoint/plaza harness without the opening.

Incremental commits: controller `d47d21f`, test/index registration `e19eda1`,
native/assets/order adapter `a096639`, committed-professor API compatibility
`433a001`. Concurrent work was changing the shared field actor packs and source
APIs during testing. Final reproducible validation used isolated checkout
`/tmp/pm-opening-validation-a096639` with the exact433a001 main.cpp correction,
without incorporating unrelated pending edits. `make test` passes **101 CTest /
96 Python tests** (`/tmp/opening-isolated-test.log`); `make all` passes without
warnings (`/tmp/opening-isolated-build.log`); `make debug-short` passes
(`/tmp/opening-isolated-debug-build.log`). The primary working tree additionally
passed103 CTest/100 Python tests at `/tmp/opening-preload-test.log`, but that
mutable-worktree result is not the reproducible feature baseline. Original-data
playback tests verify all48 IDs, source speakers, automatic demo completion,
source-story writes and voyage input admission. Document audit checks720 Markdown
files with0 broken links; whitespace check passes.

Isolated Azahar fresh-file `tests/azahar_opening_flow.py` passes the full sequence
and verifies a native40-byte saved room1/story3 checkpoint
(`/tmp/opening-isolated-smoke.log`). Reviewed title, file menu, voyage and final
harbor exploration captures; copies are under ignored `artifacts/azahar-opening-isolated/`.
Separate debug-short Azahar boot passes directly into the plaza
(`/tmp/opening-isolated-debug-smoke.log`); reviewed debug-short-boot.png.
Local verified artifacts are `ttyd-2ds-opening-verified.3dsx` and
`ttyd-2ds-debug-short-verified.3dsx`. The isolated source/profile/assets stay under
/tmp; original extracted assets/binaries remain ignored. No user emulator profile
or saves were modified. Xvfb required display-socket escalation. Earlier shared
runs exposed initialization/resource failures and are superseded by this stable
checkout validation; no original-runtime/Dolphin or hardware measurement is claimed.

Limitations/next: this is an **event-order/presentation adapter**, not complete
source cutscene execution. Book/page transforms, original camera interpolation,
NPC movement/yaw/pose ownership, ship transitions, fades, title curtains, diary
border/glyph effects, environment/SFX services and full new-game pouch/card
initialization remain unbound. Preserve original concurrent/branch/control semantics
when replacing snapshots with REL execution. The new-file path reaches story3;
the existing six-message harbor encounter reaches4, but original022..024 battle
entry, tutorial/full Jump continuation,025..033 postbattle/crowd joins and the
subsequent story progression still need native binding before fresh-file playtests
can reach all later events in order. Continue with those source graphs and the
remaining opening choreography; do not call the full opening or remake complete.
The user's goal remains active.

## 2026-10-13 — Priority0 MusyX effect API and voice/IRQ closure

Delivered [eight complete API/lookup/IRQ bodies](behavior/audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Public effect wrappers retain raw inputs and core returns; StartEx loads a
studio-indexed default byte with no bounds guard. Check returns the original
external ID when internal mapping exists, not the internal index. The original
repeated backend checks retain fresh lookup/IRQ boundaries. Internal mapping
is an unsigned-key linked-list scan with early miss; MusyX IRQ count/state is
separate from SoundEfxPlayEx's nesting pair and retains16-bit wrap semantics.
Marked the local EVT-01 portion complete while leaving core/runtime work OPEN.

Validation: generic musyx_effect_api_evidence.json verifier12 body hashes
(8 complete/4 linked identities) and77 anchors passes. Eight fresh complete
listings remain under `/tmp`; IRQ boundary vectors distinguish0/1/2/65534/
65535. Link audit717 Markdown files/0 broken links and whitespace checks pass.
Docs/metadata only; no make rerun, native code or original asset/code-body
export. Shared unrelated staged/worktree changes excluded from this commit.

Next/limits: core synthFXStart/controller/KeyOff, voice-map/default-table
producers, resources/macros and hardware/OS/source/runtime need recovery.
Continue priority0 calc3D/live audio and query-specific weapon/menu source
bindings, then priority1 damage/counter consumers. No original audible/executed
trace, hardware or full-game completion claim.

## 2026-10-13 — Priority0 pending SFX startup and effect backend contracts

Delivered [complete pending region](behavior/audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md),
[effect PlayEx](behavior/audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md) and
[five control leaves](behavior/audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md).
Queued delay reaches0 before a later start visit; successful startup enters
live processing the same update. Three backend routes retain original volume,
spatial packet, aux and cleanup order. Effect engine uses a separate40-record
pool with first-free/priority<=steal, raw start/controllers, interrupt nesting,
stored narrowed fields, fresh-pool lookup and repeated liveness checks. Check,
Volume,Pitch,Aux1 and Stop preserve their different checks and field widths.
Marked those local EVT-01 portions complete; no full audio-chain completion.

Validation: fresh current SFXMain and six complete effect listings in `/tmp`.
Generic DOL verifier sfx_startup_backend_evidence.json passes25 spans/143
anchors; independent direct-call census50 sites resolves all targets. Arithmetic
check65026 products proves division127;195078 offset/clamp cases pass. Final
link audit716 Markdown files/0 broken links; whitespace checks pass. Metadata/
documentation only: no make rerun, native code, generated tone or asset export.
Unrelated staged/worktree edits remain excluded from the commit.

Next/limits: full calc3D/listener/live volume/fade, stream/song services and
MusyX/resource/emitter/interrupt/hardware/source/runtime still open. Continue
priority0 spatial/core/backend and query-specific weapon/menu source domains,
then priority1 consumer/counter graph. Original execution/audio/hardware/full
remake fidelity remains unverified; no provisional timing is original evidence.

## 2026-10-13 — Tattle flag owners and switch encoding/storage answer

Delivered [complete Tattle flag graph](behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
and rewrote [swdrv storage](behavior/memory/SW_SET.md). Actual rotlwi5 plus
preceding shift27/sign correction implements ordinary signed remainder; valid
nonnegative bit index is modulo32. Init proves global256-word8192-bit and
local16-word512-bit banks,2048/1024-byte banks, and full32-bit global byte0.
ReInit clears only local banks. Tattle setter scans48 lists/247 member occurrences,
writes persistent4474+kind and retained battle bits; checker has policy bypass
and asymmetric original-kind local check. These are not table-population writes.
Marked corresponding EVT-01 and SAVE-01 portions complete; preserved request text.

Validation: fresh14 bodies and table/48 list spans total63 fingerprints;
205 source anchors pass generic verifier with switch_monosiri_flags_evidence.json.
Independent signed arithmetic8951 cases cover global/local/Tattle indices and
signed boundaries. Source groups/kinds and first-match order independently
checked. Link audit712 Markdown files/0 broken links; whitespace checks pass.
Docs/metadata only; no make rerun, native save/code or original asset export.
Inspection outputs remain `/tmp`; unrelated staged/worktree edits excluded.

Next/limits: complete cardMain record/checksum/error/backup/load/timing and
full namespace/writer/scene/save lifetime still open. Continue priority0 SFXMain
used startup/spatial backend, query-specific weapon menu source domains and
original scheduler/encounter traces, then priority1 consumer/counter graph.
No original execution, save round-trip, hardware or complete-game parity claim.

## 2026-10-13 — Temporary Frankly follows the party

Bound [temporary Frankly follow](behavior/partner/NATIVE_TEMPORARY_FRANKLY_FOLLOW.md)
after the original first-talk. He now advances independently through imported
terrain and door travel with original W_1/S_1 poses. Source kind11 uses the same
partyMoveWalk callback; recovered secondary source selection follows Goombella
when ordinary and within150 original units of Mario, otherwise Mario. Added
[used lifecycle/fence admission](behavior/partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md)
and inbox evidence. This does not advance story9 or enable unsupported saves.

Validation: `make test` passes103 CTest and96 Python tests; `make all` builds
the3DS binary. Original-data test completes29-message first-talk then walks100
ticks, checks independent Frankly history/movement, story9 and save suppression.
Selection tests cover the150-unit threshold, missing/special party state and
unordered floating comparison. Fixed the actor test's closed catalog count to
allow additional models while asserting required original model identities;
the initial full test run exposed concurrent save-block catalog expansion.
No new emulator capture or hardware test for following.

Next/limits: bind the source story9 house close callback to fence removal,
original OFF_d_saku paper/image/offscreen animation and A_saku_04 collision
removal, then source story10 lecture/choices/battle. Current portable follower
uses adapted trail/terrain stepping and explicit ordinary Goombella mode/status;
complete source ring/flags/pose timing and temporary save lifecycle remain open.
Earlier harbor story gaps and measured handheld performance still block full
prologue parity.

## 2026-10-13 — Original professor first-talk runs natively

[Native scene](behavior/story/NATIVE_PROFESSOR_FIRST_TALK.md) executes the
original180-command gor REL event in EventNativeRuntime, preserving brothers,
shared waits, actual GSW0 writes, all29 original message IDs/control bytes and
119 callback starts. The original story8 house door starts the event before
its door-end wait; completion reaches story9 and restores field controls.
Bound source cameras, seven Mario event poses, original Frankly/Goombella
animation assets, source map icon314/TPL319 and Frankuri1 stream. Corrected
the source hop to225ms/height30 and scripted travel facing. Save/exit-save
guards suppress unsupported temporary-party checkpoints. Asset provenance and
handheld atlas resampling are recorded in [assets](ASSETS.md).

Validation: full shared-worktree `make test` passes101 CTest/96 Python tests;
`make all` builds the3DS binary. Independent staged-source host/native compile
passes; after regenerating staged-format field actor assets in a separate
`/tmp/professor-stage-verify` tree, all100 staged CTest tests pass. The first
staged test run exposed the concurrent PMANIM-format cache mismatch, rather
than a scene failure; the isolated rebuild does not modify workspace assets.
Original-data tests check29 ordered messages,119 callbacks,17 cameras, source
item/hop/music, door admission, story9 and save suppression. Independent Mario
animation comparison passes102 samples, maximum error8.58307e-06. Azahar
isolated replay/captures at `artifacts/azahar-professor-scene-memory/` shows
original attributed dialogue and restored field HUD. Initial4MiB scene atlas
run failed to restore the east checkpoint; retry with1MiB succeeds. Final
scripted-facing/save-guard changes were rebuilt and host-tested after capture.

Limits/next: temporary Frankly currently remains at his saved source position;
follow/lifecycle/persistence and original story9 door-close fence event are
next. Paper fence animation, reaction/SFX/display-flag consumers, exact turn
interpolation and source movement/collision stepping remain unfinished. Story10
lecture/choice/battle continuation, earlier harbor gaps and hardware performance
remain required. This increment is not complete prologue or original parity.

## 2026-10-13 — Priority0 Monosiri table, consumer and census answer

Delivered [Tattle table/consumers](behavior/battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md)
and rewrote [lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md). Corrected
spatial-sound misclassification, lis arithmetic, impossible-null and safe-caching
claims. Original sampling actually tests pointer0, reloads kind and looks up
again; arbitrary invalid kind can wrap to0. Initialized231-record table supplies
six name/help/log/model/pose/additional-key fields. Complete message-key callback
resolves its unused second argument then raw output before fresh unit lookup,
without actor translation. Direct census:17 DOL calls,2 REL24 imports.

Validation: generic metadata verifier13 spans/40 anchors; sampling verifier7
bodies/48 anchors/14 constants passes. Independent DOL scan and US usage audit
agree17 linked/0 nonlink/0 data-word candidates plus2 imports across29 RELs.
All231 table records/column counts and original example identifiers inspected;
wrapped-zero derivation checked. Inspection files and reports remain `/tmp`.
Link audit708 Markdown files/0 broken links; whitespace check passes. Docs and
metadata only: no make rerun, native change or original asset/dialogue export.
Unrelated staged/worktree edits are preserved outside the commit.

Next/limits: table mutation/alias/valid-kind and full Tattle/log/AC/REL/event/
save/runtime binding remain open. Separately observed flag setters/readers use
persistent switches and battle work, not table-population writes. Recover that
shared flag graph and correct the SW_SET rotate-by-five interpretation, then
continue priority0 source/menu/audio/runtime requests. No original gameplay/
hardware/full-game equivalence claim.

## 2026-10-13 — Original Hammer charge cue resource answer

Delivered [two charge cue bindings](behavior/audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md)
and checked that portion of AC-02. Actual callers8012a4b0/4d8 resolve PI1/PONE1
through table ordinals2619/2620, FX862/861, group6/macros995/994 and original
samples776/775. Directory rate16000 and nonloop1526/5276 frame lengths are
static dependencies, not a complete cue duration or substitute WAV playback.
Three original bank asset hashes and derived metadata stay in a source manifest;
no original macro bytes/PCM/ISO or new asset conversion enters history.

Validation: `/tmp/verify_hammer_evidence.py` reparses original ISO rows/project/
pool/directory, passing3 assets/2 cues/2 macro bindings/2 sample records and
all1247 directory records. Generic DOL verifier passes14 spans/85 anchors,
including full Hammer main/two caller instructions; complete behavior verifier
passes (`/tmp/inbox-hammer-behavior-check.log`). Both existing ignored PCM
counterparts exist. Link audit706 Markdown files/0 broken links; whitespace
check passes. Documentation/metadata only; no make test/all rerun. Unrelated
staged/worktree edits preserved outside this commit.

Next/limits: full SFXMain spatial/start/volume/effect backend/MusyX execution,
EVT-01 query-specific menu/source/global domains and original scheduler/audio
traces remain open. AC-02 lifecycle/display/forced/suppression writers and
original charge playback remain unchecked. Continue by inbox priority; no
original runtime/hardware/full-game equivalence claim.

## 2026-10-13 — Current opening asset build blocked by empty interior variant

The latest `make all` fails in `tools/opening_assets.py` while `map_convert.py`
computes bounds for `aaa_00-interior`; the requested hidden-node set produces no
render vertices, so NumPy rejects the empty minimum. Isolated conversions
confirm base `aaa_00` has8,542 triangles, hiding `S_ie_door` alone leaves16,739,
and hiding `S_aaa_00` alone leaves zero. I did not change the visibility set,
because doing so without source evidence could render the wrong interior view.

Therefore the latest build and Azahar timings below cover the previous
buildable asset state, not the newest opening/professor source update. No new
performance optimization was made. Next: resolve the correct original interior
visibility subset, rebuild, then repeat the gameplay and Voyage timing samples.
The performance review remains open; this is a scene correctness/build blocker,
not evidence for a renderer optimization.

## 2026-10-13 — Latest-build harbor performance samples

`make all` succeeds on the latest front-end/field worktree. Reached a current
story3 harbor state through the native new-game flow and reloaded that generated
version4 save through the file selector in a separate isolated Azahar profile.
The settled harbor scene reported60 FPS,1.31ms simulation,5.0ms CPU and0.5ms
GPU. After a1.2-second movement/camera-follow input over animated water, it
reported60 FPS, approximately1.1ms simulation,5.3ms CPU and0.5ms GPU. Captures
are `artifacts/azahar-perf-current-harbor-valid/field-settled.png` and
`artifacts/azahar-perf-current-harbor-moving/moving-harbor.png`. This verifies a
current-build gameplay path; it remains a small software-emulator sample, not a
percentile run or hardware result.

No substantial optimization is justified by these measurements. The isolated
29–30 FPS Voyage observation remains unconfirmed: a second quiet run did not
reach the same state and held60 FPS in the letter scene. Next: use a state-aware
Voyage fixture/input, then gather repeated timings for east-side actors, harbor
and battle before closing this review. No hardware parity claim.

## 2026-10-13 — Current front-end performance spot check

After matching `Renderer::renderFrontend` to the current four-argument call,
`make all` succeeds. Latest-build isolated captures show60 FPS on the opening
House scene and60 FPS on the settled letter scene. One quiet run reached Voyage
and showed29–30 FPS on the “A / START: continue” interstitial and again five
seconds later; a repeated quiet run did not reach that same state and stayed at
60 FPS on its settled letter scene. The apparent Voyage dip is therefore
unconfirmed. The project opening-flow script's separate5 FPS screenshot was
captured while its `xdotool key` calls flooded the terminal and is excluded as
harness interference.

No performance optimization was made. Front-end rendering currently lacks the
gameplay CPU/GPU timing overlay, and the only29–30 FPS reading lacks a repeat
in the same state. Next: use a quiet, state-specific Voyage capture and collect
repeated FPS plus CPU/GPU timing for that scene and the field. The previous
east-room fixture also failed native restore validation and must be replaced by
a verified current-format fixture before it can support an actor-load sample.
No hardware performance claim; review remains open.

## 2026-10-13 — Priority0 positional-SFX correction and update producers

Reinspected and rewrote original [provisioning](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
[stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md) and [accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md).
[Update/correction delivery](behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
closes full psndMain cooldown producer and scoped SFXMain completion/pending/
preference-reset paths. Corrected low13 table index, pending flag direction,
row+4 high backend bits, numeric/name ABI, mute override,40-slot free search,
strict reclaim priority, live255 defaults, independent delay/offset stores and
coherent16-bit generation wrap. Prior conflicting historical deliveries are
explicitly superseded; original request text remains unchanged.

Validation:9 complete body listings independently regenerated under `/tmp`.
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`
passes13 DOL body/table/numeric spans and78 instruction anchors. Link audit
checks704 Markdown files with0 broken links; `git diff --check` passes.
Metadata only; no tool/native code change, so make test/all not rerun. Read-only
ISO and user emulator profiles/saves untouched; no original code/assets exported
into history. Commit excludes unrelated staged/worktree changes.

Limits/next: SFXMain middle/spatial/startup and Sound/MusyX resources/macros remain
partially recovered; original execution and other source writers stay open.
Next recover full used effect startup and two Hammer charge cue macro bindings,
then continue EVT-01 menu/global/source and original scheduling evidence.
Full game documentation/remake and hardware parity remain incomplete.

## 2026-10-13 — Original east Mouser talk and per-area actor resources

Feature commit `8cdab4a` adds stationary east Chu3/Mouser using original
`m/gor_02/s` XYZ/tribe/direction, `a/c_tyusan` / texture bank and `CHU_S_1`
sparse animation. Its simple original talk event selects CASE_LESS branches
at56/112/164/215/261/339/373/401; original US IDs/payloads/control tokens and
actual actor attribution are retained with no story write. Current runtime
coverage is explicitly story0..55, not a recovered visibility rule. Native
proximity0.65/height0.25 and grounded A-before-door/jump priority remain
provisional until original search/arbitration binds. No original story is
inferred from absent native save data. Original Frankly story8/10 init positions
move into the same catalog; his complete scenes remain separate owners.

The portable catalog validates model namespaces, story/interior gates and
whole records atomically. Destination-area models stage with geometry; failed
imports retain the resident area. Animation samples persist per model and cache
simulation ticks/poses; hierarchy arrays retain capacity, recursive dispatch
avoids std::function allocation, and root trig runs once per instance. GPU
texture batches use citro3d linear buffers and whole-mesh capacity admission.
Visible actor/vertex/sample-failure/overflow counters expose bounded submission.
See [native scope](behavior/field/NATIVE_AREA_ACTOR_CATALOG.md),
[source bindings](behavior/field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[provenance](ASSETS.md). Initial transform helper commit is `c46ee29`.

Validation: isolated source snapshot `/tmp/pm-field-verified-at8uf8ls` of the
feature changes passes `make test`: **99 CTest / 96 Python tests**
(`/tmp/goal-field-isolated-test.log`), and `make all` without warnings
(`/tmp/goal-field-isolated-build.log`). Snapshot excludes concurrent opening/
professor additions and uses local ignored original resources. Original-data
tests cover18 branch-boundary stories/all9 unchanged US payloads, actual talk
entry/closure, speaker identity, no story/save mutation while talking, original
room fixture ground/wall admission, and sampled model streams. Catalog tests
cover malformed/unsafe/overlapping records and retained state on failed reads.
Earlier shared runs also passed100 CTest/96 Python; shared native builds briefly
failed during in-flight frontend/audio/professor edits. Those intermediate
failures are not claimed as feature validation; the isolated build is passing.

Immutable ignored `artifacts/ttyd-field-actors.3dsx` is the validated snapshot
binary. Isolated Azahar run uses `tools/make_field_actor_fixture.py` and
`tests/azahar_field_actor_talk.py`, output
`artifacts/azahar-field-actors-verified/`, log
`/tmp/goal-field-verified-smoke.log`. Reviewed original-Mouser talk and final
return captures: source art, speaker-reaching tail, restored exploration/menu
and actual manual save with unchanged story8/checksum. The script requires a
new save timestamp after source-window closure, avoiding repeated-A reopening
as false success. Xvfb required display-socket escalation; user profiles/saves
and ISO were untouched. This is native scene validation, not Dolphin or hardware
parity/performance evidence. No new Dolphin observations were taken.

Documentation worker recovered ordinary NPC and save-block bindings and added
inbox breadcrumbs. Save-block original spawn `(185,60,-55)`, scalar10 and
hit/system-event graph are documented in
[source binding](behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).
Conversion exposed S_1 UVs outside0..1 plus scrolling texture keys; the ordinary
compiler correctly rejects that domain. The incomplete probe pack was removed
and no save-block record ships. Bind original animated texture matrices and
clamp/material services before adding its visual/hit/save implementation.

Next: close original NPC search/priority/facing/collision and shadow/material
services; bind Goomba wandering and tavern NPC fade; integrate professor/fence/
lecture scenes through their source owners; implement save-block texture/hit/
card lifecycle. Gather dense-room/movement timing traces on New2DS hardware and
matched Dolphin observations. Broader prologue/game fidelity remains incomplete;
the user's goal stays active. Other concurrent worktree/staged changes are
preserved outside this feature commit.

## 2026-10-13 — Priority1 initialization/count and priority2/3 inbox portions

Delivered [DMG-01 initialization/count composition](behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md):
full local Entry/UnitSetup, signed HP/FP copy and later imports, distinct body-part
selectors, setup count0/result1, four encoded TimingA increment routes and
read-only shared count query. Independently corrected result-reader battle/output
capture order and pending bit0/mask1 wording. Marked scoped delivered portions
in DMG-01, AC-03, AC-01 and the shared STATUS-01 initialization prerequisite;
request text remains unchanged and parent integration/runtime work stays OPEN.
Priority4 AC-02 full lifecycle/display/macro bindings remain pending.

Validation: full Entry verifier33 bodies/441 anchors/11 spans; UnitSetup8 bodies/
190 anchors/5 spans; Jump graph268 commands/19 bodies/57 anchors/5 concurrent
blocks; input cadence11 hashes/154 anchors/28 calls/3 data bindings; boss Jump
10 fingerprints/55 instructions/9 constants; lecture helper roles and complete
general behavior verifier pass. Setup/TimingA/count/result reader listings
regenerated to `/tmp/inbox-*.elf`, read-only ISO. `python3 tools/check_doc_links.py`:703 Markdown files,0 broken links;
`git diff --check` passes. Documentation only:
`make test`/`make all` not rerun. Unrelated staged/native edits remain excluded.

Limitations/next: actual loadout/stat/bonus and remaining counter writers, full
consumer/reward/defeat/tutorial eligibility graph, query-specific cursor domains,
source/platform services and original multihit/input/RNG/pause/turn traces still
need recovery. Continue priority0 remaining source/runtime acceptance, then
priority1 consumer/counter owners, priority2 global defense/forced-counter gates,
priority3 tutorial writers, priority4 Hammer display/audio and priority5 turn
lifetime. No new Dolphin observations, hardware or complete-game fidelity claim.

## 2026-10-13 — Priority0 documentation inbox delivery

Consolidated [EVT-01's local service contract](behavior/event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md)
and corrected the [six unit-query leaves](behavior/battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md).
Independent read-only US ISO inspection confirms HP's retained early battle,
ID-1-before-lookup/output gate, work's late lookup, and distinct status/data
capture order. Split EVT-01 todos into checked static portions and open runtime/
source requirements; preserved request text and registered the delivery.

Validation: nine source verifiers pass: event manager/root/descendants/dispatch
 tail/run-controls, weapon menu domains, ActMenu, menu interruptions and Jump
source registry. Registry confirms3 streams,156 direct sites and72 callbacks.
Six complete leaf listings were regenerated to `/tmp/inbox-*.elf` only.
`python3 tools/check_doc_links.py`:701 Markdown files,0 broken links.
`git diff --check` passes. Docs only; no make rerun or native code changes.
Existing unrelated worktree edits were preserved and excluded from this commit.

Limitations/next: EVT-01 stays OPEN for original pause/multicredit/creation/
deleting/wait traces, query-specific unsupported cursor slots, global/dynamic/
transitive sources and backend domains. No original observation or hardware/
full-game equivalence claim. Continue priority1 DMG-01 allocation/stat/init and
attack-count ownership, then AC-03 defense integration in inbox order.

## 2026-10-13 — Rebuilt movement timing sample

Added the renderer declaration required by in-flight front-end code so the
current tree builds; no performance behavior changed. `make all` succeeds. The
rebuilt 3DSX capture under `artifacts/azahar-perf-current-plaza/` holds60 FPS
after a camera move and reports0.79ms simulation,5.1ms CPU and0.5ms GPU. The
ordinary plaza scene includes Mario, Goombella and nearby NPCs. This is a single
software-emulator sample, not a percentile trace or hardware measurement.

An east-room fixture attempt under `artifacts/azahar-perf-current-east-valid/`
was rejected by native restore validation and displayed “Saved room unavailable;
started in plaza”; it is not an east-actor timing sample. No substantial
optimization is indicated by the valid capture. Next: construct a verified
current-format east-room fixture, then gather repeated timing samples across
animated harbor/east scenes and cutaway-active movement. Hardware profiling is
still required; performance review remains open.

## 2026-10-13 — Current renderer workload and build limitation

After the field actor/frontend source update, the current `romfs/manifest.json`
records plaza at40,714 triangles/150 batches, harbor22,922/87, east33,444/145,
west34,885/127 and station41,084/54. Plaza has26 cutaway groups with1,425
triangles total (maximum186 in one group). The renderer performs bounds checks
before triangle intersection tests, so cutaway testing is a bounded workload;
full map geometry and batch submission are more plausible GPU/driver costs.
This is a static workload assessment, not a measured bottleneck. Existing
128-pixel map texture caps and atlas reductions remain in effect. No behavior or
asset changes are justified without profiling evidence.

Ran `make all` against the current shared worktree; it fails in
`source/display/render.cpp:794` because the new `Renderer::renderFrontend` definition has
no declaration in `include/runtime/platform.hpp`. This frontend integration is in-flight
work outside the performance review; no source files were changed to work around
it. Because the build failed, the prior Azahar captures exercise an earlier
executable and cannot verify this updated renderer. Next: once the interface
integration is complete, rebuild and repeat moving-scene measurements with the
timing overlay visible, then profile on the target 2DS XL before selecting any
substantial optimization. Do not mark performance complete yet.

## 2026-10-13 — Emulator performance review

Reviewed the renderer's per-frame costs: map animation vertex updates/cache flushes,
material batch submission, actor pose evaluation, and triangle-level exterior
cutaway checks. Existing size reductions are documented in [asset provenance](ASSETS.md):
source maps cap textures at128 pixels, and original character atlases trim unused
regions. No code optimization was justified by this pass; preserving original
geometry, animation, culling and gameplay behavior takes priority until a measured
hotspot is found.

Isolated Azahar software-renderer captures use separate profiles under
`artifacts/azahar-perf-plaza/` and `artifacts/azahar-perf-professor/`. The plaza
boot overlay showed60 FPS,1.63ms simulation,5.1ms CPU and0.5ms GPU. The professor
story8 fixture showed60 FPS,4.15ms simulation,5.3ms CPU and0.5ms GPU. The captures
are brief overlay samples, not percentile/frame-time traces. The attempted
walkthrough completed, but its movement capture hid the timing overlay behind a
development notice, so it provides no moving-camera timing result.

No `make test` or `make all` was run because this was a read-only performance
review and the shared worktree contains unrelated in-flight implementation edits.
The emulator reports steady60 FPS with CPU/GPU headroom in these scenes; no
reproducible emulator dip was observed. Software Azahar timing does not establish
real 2DS XL performance. Next: collect repeated per-frame timings on hardware and
capture cutaway-active movement without overlays before changing hot paths. If
original-equivalent behavior and measured hardware performance conflict, track
the affected behavior as a high-priority fidelity/performance task rather than
silently changing it.








## 2026-10-13 — Original Frankly animation and first-talk message prerequisites

Added the original c_kuri_h model/texture bank and six real clips
Z_1,S_1,T_1,W_1,R_1,D_1 to the local animation pipeline. Native story8 professor
interior draws original animated S_1 at source init(-135,0,-240). The12-cell
RGBA8 atlas crops unused rows to1024x256 without resizing those cells or changing
colors, saving3MiB of texture memory; a checked V scale adapts the existing
animation pack UV domain. Original animated geometry/hierarchy/visibility stays
intact. Actual bank D_1 versus tribe literal d_1 is explicitly recorded with no
invented alias. See [assets](ASSETS.md) and
[complete scene evidence](behavior/story/PROFESSOR_FIRST_TALK_AND_FENCE.md).

Message parser/playback now binds dkey and its close: source0xfff5 payload1/0,
nonzero sets0x80/clears acceleration/anchors elapsed to record schedule; zero
clears0x80 only. Timed source waits stay active under repeated A input. Tests now
parse/play all29 original first-talk payloads, including resolved party `_kur`
keys, original malformed-looking `</dynamic 3>`, styles/colors and unchanged
raw bytes. No narrative text or substitute scene is authored.

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/professor-tests.log`); `make all` builds native3DSX with no warnings
(`/tmp/professor-build.log`). Explicit original-data message test passes20
harbor+29 professor payloads. Independent animation reference generation and
`actor_visibility_tests romfs/professor.pmanim artifacts/professor-animation-reference.bin`
pass45 source samples, maximum host model error1.90735e-06. Native input-gate
regression verifies acceleration reset, real elapsed wait and timed reopening.
Document audit checks695 Markdown files with0 broken links; diff whitespace
check passes. Isolated Azahar story8/interior2 fixture under
`artifacts/azahar-professor-idle/` boots inside the original house with Frankly
visible; reviewed boot.png. Fixture is a native development save, not original
GameCube progress. User emulator profiles and read-only ISO are untouched;
all original/generated assets stay ignored and outside source history.

Limits/next: this is character/message prerequisite work, NOT complete
first-talk/fence execution. NPC yaw/clock/scalar and init visibility are scoped
native adapters awaiting producer/backend binding in OW-ROAD-01. Source
first-talk's initial position override, concurrent brothers/shared wait, actor
movement/poses, cameras/music/effects/item display, temporary Frankly party1 and
story8→9 remain to execute. Fence removal must proceed9→10 and dispatch the
original lecture, whose common return is11. Consider the existing EventNativeRuntime
for the original REL script rather than manually flattening concurrent commands;
DOL-only pack_event_scripts needs REL-section relocation/data provisioning first.
Broader arrival/tutorial/story progression and hardware performance remain
unfinished. Goal stays active.


## 2026-10-13 — Harbor partner fall-through and original follow distances

Reproduced the user's harbor ledge complaint on original converted geometry:
the old center/crossing landing query could reject valid support and fall below
the dock until recovery copied Mario's root. Read-only original contracts prove
five-probe support BEFORE fall integration, current-velocity drop admission,
normal homing floor-loss acceleration-1 versus stay-1.5, velocity cap-16 and
horizontal damping0.2 followed by1.3*retained speed/direction. Native support now
uses Goombella's original width26/height24, ordinary partner attribute filtering,
Q1000 pointY and highest admitted support. Wall occupancy no longer rejects
valid landing; only Mario's jump flag marks history jumps.

Corrected an initial distance interpretation during validation:45/50 are speed
thresholds, not a fixed trailing gap. Original q0 activity normally remains
nonzero through dash-base2.25 fallback even at idle. Replaced fixed0.5 trail
spacing with width/speed history reach13+speed, separate wake-up50/idle100,
source speed selection/thresholded bonus and ordinary overlap box/corrections.
Restore now uses a0.3 leftward native seed based on general placement's30-unit
magnitude instead of the provisional0.8; full facing/search placement is unbound.
Evidence/contract commit `41b0e6d`, implementation/fixtures `9672cd6`; see
[ground-follow distances](behavior/partner/FIELD_PARTNER_GROUND_FOLLOW_DISTANCES.md)
and [native binding scope](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md).

Validation: final `make test` passes **95 CTest / 96 Python tests**, log
`/tmp/partner-fall-final-test.log`; final `make all` builds3DSX without warnings,
`/tmp/partner-fall-final-build.log`. Actual harbor regression uses two deck
positions,90 idle ticks, a verified admitted Mario jump and300 following ticks;
partner minimumY>=0.149, zero resets and grounded0.15 arrival. Portable tests
cover support-before-gravity, side-probe landing independent of wall occupancy,
retained speed damping, history reach, overlap, dense jump marks and blockages.
The old idle catch-up test's100-tick dash assumption was changed to260 ticks
for the original1.5-unit idle bonus cap. `core_tests --original` passes18,315
indexed/full-scan probes and all original-room scenarios, log
`/tmp/partner-fall-original-final.log`. Extended homing verifier passes64
instructions/12 constants; ground, direction, fall, jump, lifetime and front-wall
verifiers pass against the read-only ISO.695 document links and diff check pass.
One intermediate Python visibility test failed during concurrent map changes;
the final full run passes after that concurrent fix. Door/save/message work was
preserved and excluded from the staged partner hunks.

Isolated Azahar final capture passes with15-second startup, original converted
harbor geometry and `tests/azahar_partner_harbor_fall.py`, under ignored
`artifacts/azahar-partner-harbor-final/`; log `/tmp/partner-fall-final-smoke.log`.
The input script samples the short fall window and then reveals Goombella by
moving Mario sideways. Inspected `partner-dock-side-view.png` confirms both
characters grounded on the dock. Earlier straight-line pictures hid Goombella
behind Mario; temporary isolated tracing proved minimumY0.15 and zero resets.
All temporary instrumentation was removed before the final build. A reproducible
native fixture generator is `tools/make_partner_harbor_fixture.py`; no original
save/ISO or compiled asset is committed. All profiles/saves are isolated; Xvfb
needed display-socket escalation and the user's emulator state was untouched.

Limitations/next: this fixes the reproduced dry harbor descent using source
physics and distance rules, not full instruction-exact party locomotion. Native
vector history, exact smoothing-call multiplicity/trig/rounding, ray/cache/flag
ownership, forced/fly/ability modes and dynamic surfaces remain adapters. Bind
full80-record ground homing, partner wall services and placement3/4 (+37 for fall
recovery), then compare original Dolphin jump/fall/cadence and partner distances.
No new Dolphin observations or hardware claim; full prologue/game fidelity remains
incomplete. Original art and existing original sound assets were retained.

## 2026-10-13 — Stretch bubble tails to the visible speaker

User requested a reaching tail rather than the original fixed window-relative
length. Native story drawing now extends the tail from its existing bubble
base to four screen pixels before the projected talk anchor. Original sheared
rows, base/width, placement and suppression gates remain; too-close anchors
suppress the tail. White/shadow geometry independently compensates the original
48x64 masks' transparent margins, so both last opaque texel centers reach the
same visible endpoint and the shadow does not overshoot the head. Original
raster assets, IDs/raw dialogue/controls/timing and4:3 proportions are unchanged.
The four-pixel gap is an authored native adaptation, not measured original
behavior. See[native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and[asset conversion notes](ASSETS.md).

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/tail-stretch-test.log`); `make all` passes without warnings
(`/tmp/tail-stretch-build.log`). Original-data20-payload playback and six-speaker
harbor integration tests pass. Added left/right/inverted stretch checks for
retained base, four-pixel gap, white/shadow endpoint equality and close-anchor
suppression. Document audit checks694 Markdown files with0 broken links;
`git diff --check` passes.

Isolated `tests/azahar_textboxes.py` capture with the native story3 fixture and
immutable `artifacts/ttyd-tail-stretched.3dsx` completes and saves story4 under
`artifacts/azahar-tail-stretched/`. Reviewed Goombella page08 and X-Naut page14:
the visible tails now extend from the bubble toward their heads. No diagnostic
marker remains; user emulator profiles/saves and original ISO are untouched.

Limits/next: this tail extension follows the user's presentation preference,
not original fixed-endpoint parity. Pose/image-derived talk anchors and
handheld aspect remain adapters. Original NPC heights/projection, full window
services/indicator and matched Dolphin/hardware verification remain open.
Broader dialogue/story/full-game fidelity remains incomplete.


## 2026-10-13 — Save and reload playable interiors

Native PM2D version4 adds the completed inn/professor interior mask while
retaining40-byte records and version1/2/3 loading. Story remains a separate
optional signed word; no original story is inferred or changed. Unknown masks,
room/mask mismatches and reserved bytes reject atomically. Native temporary-file,
backup recovery and checksum behavior is preserved. This is a native format,
not a recovered original card format; [SAVE-01](requests/SAVE-01.md) stays open.
See [format contract](behavior/format/NATIVE_SAVE_FORMAT.md).

Game restore applies saved door collision/display state before floor/wall
validation in the native startup transaction. Completed interiors permit the
existing manual save; in-flight door choreography still suppresses saves. A
room transition resets the previous room's door mask. Original-data tests enter,
save/reload and exit both buildings through their actual close contacts.

Validation: final `make test` passes **96 CTest / 96 Python tests**
(`/tmp/indoor-save-final-tests.log`); `make all` builds native3DSX
(`/tmp/indoor-save-final-build.log`); `build-host/field_door_tests original`
passes both source rooms after save/reload. New native-save tests cover old
versions, invalid masks/reserved bytes, actual version3 file loading, version4
writing/loading and backup recovery without mutating decoded state on failure.
Document audit checks694 Markdown files with0 broken links; diff whitespace
checks pass. Isolated Azahar `tests/azahar_indoor_save.py` asserts version4,
room0/interior1 in the actual saved file. A second launch with that isolated
save reloads inside the original inn; reviewed
`artifacts/azahar-indoor-save/boot.png`. User profiles/saves remain untouched.
Original assets/packs remain local and ignored. Validation runs on the current
shared worktree; concurrent partner/message changes are kept separate from
this incremental save commit.

Next: implement the original professor story8 first-talk with recovered
ordered US message keys (party print expands to `_kur`), original Frankly
`c_kuri_h` art/poses and source camera/actor chronology. Source `dkey` parser/
playback behavior is documented but not yet bound; `col` is already supported.
Then implement story9 close/wall-removal and its story10 lecture dispatch, not a
fabricated terminal story state. Door animation/light/audio, indoor NPC/item
activation and upstairs doors remain unfinished, as do the broader prologue
arrival/tutorial/story transitions and hardware performance. Goal stays active.


## 2026-10-13 — Correct tail direction and preserve original4:3 bubble proportions

Fixed the observed Goombella tail miss in79c15b1. Tail masks now draw through
source sheared quads (two textured triangles per pass), retaining horizontal
base/end edges and avoiding rotated transparent-margin expansion. A dedicated
12-vertex buffer preserves the separate shadow/white submissions. Flush and
restore citro2d around the tail GPU pass. Bubble/glyph scaling is uniformly0.5,
fitting original640x480 presentation into centered320x240 with40-pixel margins
on the400-wide top display. World drawing still fills the wide viewport.

Speaker projection now reuses the actual tilted world transform and converts
its clip coordinates to the 2D screen plane. Goombella talk position uses her
evaluated original pose bounds; Crump/X-Naut use opaque bitmap top/center
instead of padded billboard tops. Original print-entry roots/placement caching,
speaker/message attribution/order and control playback remain intact. Ignored
PMHNPC2 actor metadata stores the new offsets; source paths/conversion/rights
are recorded in[assets](ASSETS.md). See the updated
[native bubble contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md).

Validation: latest shared `make test` passes **95 CTest / 96 Python tests**
(`/tmp/tail-fix-test.log`); final production `make all` passes without warnings
(`/tmp/tail-fix-final-build.log`). Original-data playback20-payload and harbor
six-speaker integration tests pass. Added steep left/right/inverted tail cases
verify horizontal edges, width, mask coordinates and base-to-speaker direction.
Document audit checks693 Markdown files with0 broken links; diff check passes.
An earlier shared run failed the concurrently changing partner core check.
A clean archived baseline plus the tail host changes independently reproduced
partner/core failures (`/tmp/tail-fix-isolated-test.log`); message tests passed.
The later shared run after partner/test updates is the passing full validation.

Isolated captures in `artifacts/azahar-tail-fix-aspect/` complete story4.
`artifacts/azahar-tail-anchor-diagnostic/` temporarily marks the talk anchor:
reviewed Goombella's marker on her visible head and the white mask centerline
aiming there. Original shadow artwork extends beyond the white mask. The marker
was removed before the final production build. Final marker-free captures use
`tests/azahar_textboxes.py`, native fixture and immutable
`artifacts/ttyd-tail-fixed-final.3dsx` under `artifacts/azahar-tail-fixed-final/`;
script checks story4 persistence and overlay restoration. User emulator profiles
and saves are untouched; ISO input stays read-only and packs/images ignored.

Limits/next: pose/image-derived anchors are native adaptations; original NPC
tribe height+150 and original getScreenPoint services remain unbound. Original
shared RNG, full window types/indicator/history/selection/icon/pose/paper/audio/
noise and matched Dolphin/hardware comparison remain open. Check additional
actors/cameras as more story content is bound. Full dialogue/full-game fidelity
is not complete.


## 2026-10-13 — Original same-map inn and professor door access

Bound the plaza inn/bar ground-floor and east professor house as original
same-map doors, preserving the five existing room IDs. The documentation
agent delivered [door evidence](behavior/overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
and appended the [request inbox](requests/OW-ROAD-01.md). Source admission uses
registered forward hit identity plus A and ordinary motion/common case gates,
with no invented radial door trigger. Extracted first hit-triangle normals and
original hit centers produce40-unit-offset, EVT-integer destinations. The native
fixed-tick bridge uses the original135-unit/sec request and500ms waits; it
consumes A before the native jump harness and preserves the story word.

Original inner subtrees start hidden and are revealed on entry. The professor
interior collision subtree enables/disables across wall, floor, overhead and
shadow queries, with a cached mask change rather than per-frame asset rebuilds.
Inn's original null collision field stays null. Source geometry is retained;
outer subtrees use an explicit provisional indoor cutaway until the original
flag2/0x800 fade consumers are documented. See [conversion/licensing](ASSETS.md).
The ignored doors.pmdoor pack and doors.json manifest contain only locally
converted original data; neither the ISO nor extracted assets are committed.

Validation: final `make test` passes **95 CTest / 96 Python tests**
(`/tmp/interior-final-tests.log`); final `make all` builds the native3DSX
(`/tmp/interior-final-all.log`). `build-host/field_door_tests original` tests
actual original contact admission, both buildings' entry/exit, unchanged story,
and interior-save suppression. Geometry partition tests preserve all vertex
records and byte-identical collision packs. Document link audit checks693
Markdown files with0 broken links; working/index diff whitespace checks pass.

An initial default-binary Azahar run displayed a generic load error and is NOT
counted as a gameplay pass. An isolated build then loaded and revealed an
exterior-wall occlusion problem; the explicit cutaway fixes it. Reviewed
`artifacts/azahar-inn-cutaway/inn-inside.png` and `inn-exited.png` under the
isolated profile: original bar interior, visible Mario/Goombella and restored
exterior on exit. Professor access is original-data host tested, not yet visually
compared against Dolphin. Captured cutaway build precedes the final central save
suppression guard; final `make all` and tests include that guard. User emulator
profiles/saves remain untouched. Concurrent partner/message refinements remain
separate working-tree work; this increment stages only its own changes.

Limits/next: original door rotation/light/dark/sound/letterbox services, indoor
NPCs/items, inn upstairs callbacks, professor story8 first-talk and story9 wall
removal are not executed yet. Camera and relative partner movement are explicit
native adapters. Interior saves are suppressed to preserve the existing outside
save until a versioned door-state extension is implemented. Next bind those
services and original dialogue, expand door registration to the other source
records and verify professor captures. Harbor arrival/tutorial/plaza/east story
transitions and hardware performance remain incomplete; the goal stays active.


## 2026-10-13 — Original-style speaker bubbles and dialogue overlay suppression

The harbor checkpoint now renders the original `f/msgWindow.tpl` ordinary
border/shadow/tail masks, with source default(-280,225,560,176) geometry adapted
to the400x240 top display. Print-entry speaker roots and cached left/right/
offscreen placement use the recovered contracts; source tail endpoints aim
at the attributed speaker through the current native camera. Bubble opening/
closing interpolates from the speaker and ordinary anchored glyphs draw at
full opacity. See [native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and [local asset conversion/licensing](ASSETS.md).

Expanded parser controls with pos, scaleX, wave, col and closing controls,
small, clear and wpos. Existing key/page/wait/speed/scale/dynamic/shake tokens
and original ID/raw-byte preservation remain intact. Shake is now rendered;
wave uses unaccelerated simulation-clock time and dynamic follows original
draw-time relaxation. Partial glyph UV cropping keeps enlarged effects visible
at the window boundary without leaking GPU scissor state. Ordinary source
vertical admission precedes effects; clipped dynamic records still relax.
Unsupported service controls reject atomically instead of dropping tokens.

Top HP/location/interaction prompts and bottom menu/captions/buttons/notices/
FPS disappear for ordinary dialogue and the entire locked harbor scene.
A/B/touch still advance the original scene, and overlays restore on completion.
Development notices use the original ordinary border without fabricated NPC
attribution. No original dialogue was authored or rewritten.

Validation: final `make test` passes **94 CTest / 96 Python tests**
(`/tmp/textbox-test.log`); final `make all` builds3DSX without warnings
(`/tmp/textbox-build.log`). Original-data `message_playback_tests original`
parses/plays all20 opening payloads; `harbor_story_tests original` passes six
source speakers/order/positions and story4 completion. Source contract verifier
passes (`/tmp/textbox-source-contracts.log`); document audit checks691 Markdown
files with0 broken links; `git diff --check` passes. Tests include control layout,
rollback, source geometry/selector/equality/offscreen/tail gates, partial clipping
and speaker snapshots. An initial new test incorrectly assumed right-side
placement implied an endpoint on the right of its base; corrected it to check
the recovered base-to-speaker interval. The initial assertion failure is not
counted as a passing validation.

Final isolated Azahar capture runs `tests/azahar_textboxes.py` using the native
story3 fixture under `artifacts/azahar-textboxes-final/`. Reviewed opening,
Crump page/Goombella responses, and final story4 overlay restoration; the script
checks saved story4. The earlier rapid walkthrough also passes under
`artifacts/azahar-textboxes/`. Sandbox Xvfb first failed display-socket access;
the approved isolated run succeeds. User emulator profiles/saves are untouched.
Original ISO/extracted raster assets/packs remain read-only-source/local/ignored.

Limits/next: native eye/aim/FOV projection, Goombella0.55 height, extracted
Crump/X-Naut billboard tops and separate shake RNG are explicit adapters,
not measured source behavior. The tail uses a rotated original mask rather
than the exact sheared GX quad. Complete NPC tribe heights/getScreenPoint,
original shared RNG, full window types/history/selection/icon/pose/paper/audio/
noise, original star indicator and matched Dolphin image comparison remain
open. Arrival/tutorial/full-game story services and hardware performance remain
unfinished. Next bind those consumers and compare original captures; do not
claim complete dialogue or game equivalence from this slice.


## 2026-10-13 — Combined dialogue-window support and isolated validation

Concurrent work committed the harbor/font foundation as48d8c73 and extended
the shared message renderer while this session was preparing its commit.
ff225fa picked up that extension; bd4abf8 adds its required
`message_window.hpp` and `message_window_assets.py` support files, so the
committed dependency set is complete. Original ordinary bubble/tail masks now
replace the plain native panel. Source-derived anchor/box/tail behavior and
additional retained style controls improve presentation; specialized window,
NPC anchor/pose and original shared RNG/material fidelity remain unfinished.

Validation of the combined snapshot: **94 CTest / 96 Python passing**,
original20-payload and six-speaker harbor integration checks pass. A shared
native build raced another linker and produced an invalid-ELF packaging error;
isolated `make BUILD=build-harbor-verify
TARGET=artifacts/ttyd-harbor-verified all` passes. The immutable output at
`artifacts/ttyd-harbor-verified.3dsx` completes the original story3 checkpoint
and saves story4 in `artifacts/azahar-harbor-combined-verified/`; manually
inspected first-dialogue image shows original font/bubble masks. New
`emulator_smoke.py --binary` supports such isolated build verification without
replacing another workstream's artifact or touching user emulator profiles.
Current concurrent uncommitted refinements are separate from that validated
snapshot. Next: validate those refinements, then continue arrival/tutorial,
original NPC/event/window services and indoor access. Full prologue parity
remains incomplete.

## 2026-10-12 — Original harbor dialogue checkpoint and US font

Added a source-derived story3 harbor sequencer for original mac_0_016..021,
with the recovered position gate (integer decoded Z<=-100), exact message IDs/
raw payloads/speaker order, independent delayed actor moves, source camera
requests and terminal story4 write. It is enabled only by explicit native
originalStory=3 in harbor; legacy/new harness state does not invent a story
checkpoint. The gate has no added A/grounded condition, and neutral field
physics continues an existing jump while the scene blocks new player actions.

Opening glyph/key/page/wait/speed/scale/dynamic records and finite ordinary
window fade/acceleration/scroll behavior are portable. Original US BFN glyph
masks, mappings, width patches and row offsets render through citro2d, retaining
all original message bytes/tokens. Speaker captions are presentation labels,
not rewritten dialogue. Original c_kanbu_ibt/c_zako_n setup art and the exact
BGM_EVT_DANGER4 stream are packaged locally; see
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md) and
[source choreography](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md).

This remains a development checkpoint, not complete scene/REL VM fidelity:
NPC numeric stepping/collision/automatic poses use a documented temporary
speed adapter; Crump/subordinates use original static setup art. Mario
set_dir/reardir, nested key/GSF persistence, environmental/NPC/text SFX, crowd,
original bubble/history/selection/paper/shake/noise services remain unbound.
Story2 arrival/resume and story4 tutorial continuation/restore are unfinished.
Fresh launch remains the plaza harness. No new fabricated dialogue was added.

Native checks parse/play all20 opening payloads with their exact source font
and retained bytes; integration checks the six speaker/ID order, position-only
and airborne admission, independent movement/owner completion, save suppression
and story3→4 boundary. Isolated Azahar captured original font/dialogue and
persisted story4. The first test checkpoint was on water and lost1 HP; the
final helper places it on independently probed dry harbor ground. Source camera
and NPC timing are static-derived/adapted, not Dolphin observations.

Validation: `make test` **93 CTest / 96 Python passing**, `make all` builds
3DSX without warnings; original-data `message_playback_tests original` and
`harbor_story_tests original` pass. `check_doc_links.py` checks690 files with
0 broken links. Final isolated checkpoint capture is
`artifacts/azahar-harbor-story3-physics/`, driven by
`tests/azahar_harbor_story.py`; final story4 persistence passes. An airborne
regression test first exposed a missing air-motion dispatch in the dialogue
physics bridge; adding that ordinary dispatch lets the jump land while the
original key wait remains active. User emulator profiles/saves were untouched.
No original-runtime or hardware performance observation was made.

Next: bind original arrival and tutorial continuation, full message/window/NPC
services, original event runtime composition and indoor entrances/maps. Full
prologue parity and hardware performance remain incomplete.


## 2026-10-12 — Partner 3D following, jumps, falls and wall/path recovery

Recovered selected missing `party_motion_homing_walk` branches directly from
read-only US G8ME01: grounded history flag1 admits jump preparation, repeated
wall contacts use a signed byte counter greater than8, and ground homing calls
screen recovery. Added pinned body identity/26 instruction anchors and
[native contract with explicit fidelity boundaries](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md)
in `432293c`. Independent existing source verifiers still pass.

Replaced the ground-only XZ follower in `55d00bb` with portable 3D history,
independent ground/jump/fall state, source-derived ordinary jump initialization
and fall coefficients, swept wall/axis sliding, supported stale-path pruning,
and screen candidate recovery. The partner can traverse air without floor,
land independently, and recover without the old queue-size teleport. Original
`PKR_J_1A` now joins the existing original Goombella pack; provenance is in
[assets](ASSETS.md). Native motion uses actual post-collision Mario output.
Refinement retains stationary-hop spacing, continues over gaps, handles a jump
behind dense short trail marks and caps append pressure without teleporting.
Concurrent harbor/message work was preserved and excluded from partner commits.

Validation: final `make test` passes **93 CTest / 96 Python tests** in
`/tmp/partner-validation-test.log`; `make all` builds3DSX without warnings in
`/tmp/partner-validation-build.log`. `build-host/core_tests --original` passes
all original-room scenarios and **18,315** indexed/full-scan collision probes
(`/tmp/partner-original-room.log`), including partner stair support. New partner
scenarios cover stationary/dense-history jumps, retained spacing, gap crossing,
ledge fall, thin walls, stale path, persistent wall/capacity nonteleport, screen
recovery and Game binding. Homing/jump/fall/recovery/history static verifiers
pass;690 document links and `git diff --check` pass.

Initial full runs exposed a concurrent harbor test failure, independently
reproduced with pre-partner Game sources under `/tmp/partner-preexisting-check`;
that concurrent work subsequently resolved it. One rebuild hit a transient
unrelated field_battle_tests link failure; targeted relinking and the final
full run pass. Neither intermediate run is claimed as a passing validation.

Isolated Azahar movement/jump/save capture passes with15-second startup and
`tests/azahar_partner_follow.py`. Final files are in ignored
`artifacts/azahar-partner-follow-spacing/`, log `/tmp/partner-smoke-final.log`.
Inspected `partner-air-early.png` and `partner-follow-landed.png`: Goombella
uses her original air pose and returns to the floor with following space.
Earlier captures exposed an overlapping stationary landing, fixed and covered
by a regression. Xvfb required display-socket escalation; all profiles and
saves are project-isolated. No user's profile/save was modified.

Limitations/next: this implements equivalent native behavior categories,
not full instruction-exact party locomotion. Native vector scheduling, scale1,
terrain landing/sweeps and path lookahead remain adapters. Complete original
homing targets/ring indices, five-ray partner floors/front walls, near-Mario
placement3, fall placement4/+37, dynamic platforms, ability modes and partner
sound playback remain unbound. Record original Dolphin jump/ledge/wall/warp
trajectories before claiming measured parity; see [mechanics](MECHANICS.md).
Hardware behavior and full prologue/full-game fidelity remain incomplete.


## 2026-10-12 — Prologue map object visibility from original events

Implemented recovered ordinary-entry visibility for harbor ship subtrees and
wakes, east interior/gate and west alternate foliage in commit `8c6c5fa`.
The pirate ship group `S_fune_c` is hidden in prologue states2..6; original
init reveals it only for172..229 except the earlier separate217 case.
The later `S_fune_b` group uses>=268 except402/404. Other initialized-hidden
ship/wake groups remain hidden until their scripted overrides are bound.
See [visibility contract](behavior/field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md)
and [asset provenance](ASSETS.md). Source subtrees inherit visibility into
children without affecting siblings; batches split by event group, retaining
geometry for subsequent story states. Both drawing passes use the same policy.
Missing native story uses a conservative story0 rendering envelope without
changing persistent story storage. Pre-existing concurrent prologue work was
preserved and excluded from this feature commit.

Validation: `make test` passes **92 CTest / 96 Python tests** (log
`/tmp/pm-visibility-test.log`); final `make all` builds3DSX without warnings
(`/tmp/pm-visibility-build.log`). New tests cover source geometry partition,
prologue subtree omission, unchanged collision packs, metadata fields and
story boundaries/special cases. `check_doc_links.py` checks689 files with0
broken links; `git diff --check` passes. Isolated Azahar captures use synthetic
native story3/6 saves at harbor(0,-2), with18/15-second startup windows;
`artifacts/azahar-prologue-visibility/boot.png` shows source dialogue and
`artifacts/azahar-prologue-visibility-6/boot.png` shows unobstructed dock/water.
The initial5-second capture was too early (black); no visual pass was claimed
for it. Xvfb needed display-socket escalation. All profiles/saves are isolated.

Limitations/next: this is recovered map initialization, not complete event VM
activation. Bind story2 arrival, ship motion/departure/wake overrides, remaining
plaza/NPC/item/door conditions and original collision activation; verify scene
transitions against Dolphin. No original-runtime observation or hardware
performance claim. Full prologue/full-game fidelity remains incomplete.

## 2026-10-12 — Goombella original animations, visibility and facing correction

Field Goombella now renders original `PKR_S_1`/`PKR_W_1`/`PKR_R_1` idle,
walk and run clips from `a/c_pkr` + `a/c_pkr-`, using original scalar2 and a
separate texture atlas. Static zero-span `PKR_Z_1` is also packaged for setup.
PMANIM3 adds original visibility bases, group indices and sparse full-key
modulo256 updates. Hidden ancestors suppress descendants while preserving
siblings; shapes hidden at frame0 remain available for later keys. Corrected
the offline bake's old `delta==1` assignment. PMANIM1/2 remain readable.
Source clips replace her field billboard/procedural bob. See
[asset provenance](assets/MARIO_AND_POSES.md) and
[Goombella binding](behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).

The user caught reversed travel-facing in the initial native adapter. Replaced
that snap mapping with the recovered ordinary `partyMakeDispDir` target ranges,
four interpolation/latch branches and wrapped Z-reflection. Positive X uses
source heading90 / target180; negative X targets0. Original moving pose
selection keeps partner speed, owner scale1 and Mario stick class distinct.
Native displacement still adapts source speed/heading; original homing, special
turn flags, sound helper, HP/dot-table and event/battle poses remain unbound.
This is animation progress, not completed partner/story fidelity.

Validation: `make test` **90 CTest / 93 Python passing**; `make all` builds
3DSX without warnings. Independent original-data comparison passes **39
Goombella samples**, max model error5.53811e-6, and **251 Mario samples**,
max error3.05176e-5, including texture/visibility boundaries. Synthetic tests
cover byte wrapping, hidden descendants/siblings/cycles, rewind, invalid pack
rollback, zero-duration poses and both facing endpoints. Source checks pass
47 scale anchors/13 DOL spans/2 converter models and515 partner instructions/
32 constants/7 spans;688 document links pass. Refreshed a previously stale
converter-derived Mario numeric baseline without changing source fingerprints.

An initial isolated west-road capture passed movement/jump/save. A later short
hold failed its0.1-unit distance assertion (actual0.072); the longer dedicated
`azahar_partner_animation.py` capture passed, saving room0 X=-3.20559 from
-5.05. These are software-emulator fixture checks, not measured original timing
or hardware performance. Final facing capture under `artifacts/azahar-partner-facing`
passes the expanded right/left movement/jump/save script; manually inspected
`partner-run-later.png` and `partner-idle-left.png` confirm correct facing in
both directions. All emulator profiles/saves are isolated from the user's profile.

Next: native original message parser/window playback, harbor REL scene
activation and indoor doors/maps; remaining partner motion/poses and prologue
visibility conditions. The playable prologue / original-game parity goal
remains incomplete.

## 2026-10-12 — Harbor position gates and original Mario position callback

Documentation cross-check recovered original story3's integer Z<=-100 gate,
and story4's nested Z<=-100 / X>=-25 gate, with original message IDs, speakers
and control-token inventory. Added `fieldMarioGetPosition` with callback-registry
binding at US0x800e71f8 and `harborPositionGate`; both compose with the existing
event operand services, preserving 1/1024 float writes and integer truncation.
Independently re-disassembled the complete US callback: argument pointer is
captured before marioGetPtr, Mario pointer before mode resolution, and each
destination/position write remains ordered. All nonzero modes subtract1 from Y.
See [harbor bindings](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md) and
[inbox breadcrumb](requests/OW-INTERACT-01.md).

Validation: `make test` **89 CTest / 92 Python passing**, `make all` builds
3DSX; `verify_field_lecture_callbacks.py` verifies15 bodies/54 instruction
anchors/6 constants/10 imports; `verify_behavior_contracts.py` passes;
`check_doc_links.py` checks687 files with0 broken links. Tests cover fractional
gate boundaries, mode behavior, quantization, pointer capture, ordered field
reads and registry dispatch. No emulator run: these portable services are not
yet invoked by `Game`, so current development notices/trigger remain unchanged.

Next: original message parser/window presentation, REL harbor scene activation,
NPC poses/choreography and indoor entrances. Goombella's PKR_Z_1 pack probe
encountered original animated visibility, which PMANIM2 explicitly rejects;
implement that domain before replacing its baked idle image. The playable
prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native original US message library and resource lookup

Imported all 260 `msg/US` resources / 13,186 key/text pairs byte for byte
into ignored `romfs/messages/`, with hashes, source offsets and licensing in
its local manifest. `make all` now packages this library. Portable
`MessageResource` retains original IDs/control tokens/high bytes, builds an
offset index, and atomically rejects invalid files. `MessageAssets` provides
room-before-global lookup. Missing/ambiguous keys deliberately return null;
original fallback and duplicate-key qsort behavior remain unbound. See
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md).

Validation: `make test` **88 CTest / 92 Python passing**, `make all` builds
3DSX, `build-host/message_assets_tests romfs/messages/gor_00.txt` verifies
328 actual harbor pairs and all 20 opening IDs. No story playback or emulator
behavior changed in this increment. Next: original-token presentation and
story-call bindings, position-triggered harbor scenes, indoor entrances.
The playable prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native cross-check fixes: sink guard + default lanes

The native session's independent re-read falsified my water doc's sink
guard direction ([124]==0.0 initializes −6.0, not !=; fixed and credited)
and flagged the undocumented r4-sourced default fall lanes — added to the
bottomless doc ([r13−28176]+16 vs absolute 0x80416208+16 by [46]==5,
verified by direct reads). Both fixes verified against the listings. This
is the inbox cross-check loop working as designed: independent re-reads
catch single-bit errors that self-review missed.

Validation: direct-instruction re-checks, diff --check clean. Docs-only.
Next: triangle interiors, pose domains. Full-game incomplete.

## 2026-10-12 — HitVec region-graph correction (false asymmetry withdrawn)

Re-verified the traversal doc's region claims against the listing and found
my own region-1 asymmetry finding was backwards: R1 allocates counter=r30 /
pointer=r31 (inits `lwz 31,172` / `li 30,0`), regions 2–4 the reverse — same
loop semantics, no executability consequence. Withdrew the pointer-size
claim; kept the verified cascade edges (R1-hit→R2/R3-gateway, drains) and
the airtight R4-unreachable finding (no entry edge, dead fallthrough).
Two crashed verification scripts produced zero passing asserts this turn —
all claims above are re-verified by direct reads instead.

Validation: direct-instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains. Incomplete.

## 2026-10-12 — Water/void recovery: apply recovered bottomless drop-back lanes

Reviewed the
[water](behavior/overworld/OVERWORLD_WATER_RECOVERY_NUMERICS.md) and
[bottomless](behavior/overworld/OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS.md)
numerics deliverables against the `WaterRecovery` prototype. Independently
re-read the ISO DOL and confirmed the mot_bottomless drop-back lanes
(`0x80091bd4`: `[124]/[128]/[132]/[136] = 0.34/-0.002/-0.01/0.0`), so replaced
the prototype's authored `-.001/-.002/+.00024/-.00004` cascade (which matched
only `-0.002`) and pinned it in `tests/field_camera_tests.cpp`. Did **not**
apply the `-6.0` sink velocity, the `0.08` travel step or the `state==15`
handling: the deliverable leaves them unestablished, the `0.08` constant has no
SDA source, there is no original stage-15 slot, and the prologue guard reads as
`[Mario+124]==0.0` on the DOL (the contract states `!= 0.0`). The prototype
still omits the `[Mario+552]`-mismatch condition and the `r4` default lanes.
Breadcrumb: `requests/FIELD-RECOVERY-01.md`.

Validation: `make test` 87 CTest / 90 Python pass, `make all` 3DSX pass,
warning-free compile of the changed TU. Docs mid-restructure (inbox split into
`requests/`); breadcrumbs went to the per-request file. Full-game incomplete.

## 2026-10-12 — Merge the overworld request inbox into docs/requests

Migrated `OVERWORLD_DOCUMENTATION_REQUESTS.md` into the standard inbox style:
seven per-request files under `requests/` (OW-MOVE-01, OW-WALL-01,
OW-SUPPORT-01, OW-PARTNER-01, OW-ROAD-01, OW-INTERACT-01, OW-CAMERA-01), each
with a status line, an Open-todos checklist, verbatim request text and an
append-only answer/delivery history. The seven rows join the status table in
`DOCUMENTATION_REQUESTS.md`, which also absorbs the story-integrity and
static/runtime rules that headed the separate file; the "tracked separately"
pointer is gone. Cross-links in `behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md`
and `behavior/battle/BATTLE_HIT_VEC_TRAVERSAL_ORDER.md` now target the
per-request files.

Validation: `python3 tools/check_doc_links.py` (686 markdown files, 0 broken)
plus a line-by-line comparison of the old file against the new files — every
original line survives; only heading levels, the three header provenance lines
and the relative link prefixes differ. Docs-only, no make rerun. Limitation:
overworld deliveries stay recorded in each request's history and are not
back-filled as `requests/DELIVERY_LOG.md` rows, because the original inbox
never named a numeric documentation-agent ID for them. Next: answer the OW-*
open todos and register new deliveries/locks in the delivery log.
Full-game incomplete.

## 2026-10-12 — EVT-01 accessors, start wrappers, Monosiri, and an address correction (ID 3)

Second EVT-01 pass as documentation agent ID 3, continuing the inbox queue.

**A correction that outranks the new work.** I had published three data
bases computed **without the sign extension `lis` performs**, so all three
were wrong by `0x3B0000`:

| Published earlier | Correct | Symbol now confirmed |
| --- | --- | --- |
| `0x8002F988` | **`0x803DF988`** | `pssfx` (`.bss`, 1600 B = **40 slots** x 40) |
| `0x80016B28` | **`0x80316B28`** | `pssfxlist` (`.data`, 84840 B) |
| `0x8004CF40` | **`0x8036CF40`** | `monosiriMessageTable` (`.data`, 5544 B = **exactly 231 records** x 24) |

The symbol names and sizes are the decisive part: `1600/40` and
`5544/24` both divide exactly, which independently confirms both strides
and turns the slot count (40) and table extent (231) into facts rather than
guesses.

The same error had reached seven field-collision contracts from the previous
session, where `lfs` float constants were given as `0x800494xx`. Those were
internally inconsistent with each other, which is what exposed them. All are
now in the authoritative `N(r2)` form, read from the instruction bytes
(a contiguous pool from `-27544` to `-27380`), and the `lis`-derived globals
correct to `0x80343B78` / `0x80343D88`, both resolving into `.data`. A
correction note heads each affected document.

I also **reverted one of my own "corrections"**: the three values
`0x80091434` / `0x80091504` / `0x800915e4` are not debug strings but the
symbol entry points of `chkfilterVecVivian`, `chkfilterVecRoll` and
`chkfilterVec`, passed to `hitCheckVecFilter` as filter labels. Two were
already right in the original text; I had briefly "fixed" them wrongly.

**New complete bodies**, all in `docs/behavior/audio/`:

- [Slot accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) —
  `psndSFX_get_vol`, `psndSFXChk`, `psndSFX_dist`, `psndSFX_pit`,
  `psndSFX_vol`. Completes the accessor family. Key finding: the handle
  packs **slot index in bits 0..7 and channel allocation in bits 8..23**,
  validated against `slot+0x04` with a `-1` return. Also `stb`/`sth`
  truncation with no clamp, and the local `slot+0x14` vs `SoundEfxSetPitch`
  pitch split gated on the same low-5 bits `psndSFXOff` uses.
- [Start wrappers](behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) —
  `psndSFXOn`, `psndSFXOnEx_3D`, `psndSFXOn_3D`, plus the **derived
  eight-argument `__psndSFXOn` ABI**. `arg5` and `arg7` are always `0`;
  `psndSFXOn_3D` silently discards the caller's `r5`.
- [Monosiri lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md) —
  `battleGetUnitMonosiriPtr` 8012ef20, the item EVT-01 called
  unclaimed. Pure signed-multiply address computation with no bounds,
  no negative-kind check and no null return. Noted that the caller's
  `== null` test is therefore reading a **record field**, not a failure.

Also noteworthy: **40 slots exist but the handle masks with `& 0xFF`**, so
indices 0..255 are all reachable and nothing bounds them.

Updated [behavior/README](behavior/README.md) (its rows were overwritten
twice by concurrent commits and were re-added each time), appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md), marked the Monosiri todo
answered with the residual work, and registered three validated delivery
rows.

Validation: bodies re-disassembled per function with ELFs in `/tmp` only;
every callee resolved against `us_symbols.csv`; `lis`/`addi`/`addis`
arithmetic re-derived in Python and cross-checked against named symbols;
repo-wide sweep for the four wrong address patterns returns clean;
`git diff --check` clean; link audits clean. Docs-only, so `make test` /
`make all` were not rerun. Full-game incomplete.

**Process lesson worth keeping:** the fabricated addresses survived a
commit and a README row because they were plausible-looking. The two
cross-checks that caught them were (a) does the address land in the
section the symbol table says it should, and (b) do claimed strides divide
the claimed symbol sizes evenly. Both are cheap and should be applied to
any future address claim before it is written down.

Next tasks:

1. **`__psndSFXOn` 800d9934 (1344 bytes)** — now the only writer of
   `pssfx` fields `+0x00`/`+0x04`/`+0x06`/`+0x18` and the place the
   handle is packed. With `pssfx`/`pssfxlist` named and the layout fixed,
   this is the last structural gap in the audio chain.
2. **`psndSFXMain` (2964 bytes)** — reader of volume, distance and the
   published position; closes the update/stop loop.
3. Monosiri record layout beyond `+0x04`, and its table writers
   (`battleAcMain_MonosiriTarget`, the `monosiriExit` call at `8017225c`).
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` is a contended file: three concurrent commits
   have dropped rows from it. Worth an owner check before it is treated as
   a reliable index.

## 2026-10-13 — DEMO-01: `GSW[0]` chapter value table (ID 3)

[OPENING_GSW0_CHAPTER_VALUE_TABLE](behavior/story/OPENING_GSW0_CHAPTER_VALUE_TABLE.md).
Extracted the comparison right-hand sides the previous delivery left open, so
the chapter gate now has values.

**The sentinel is 402** — 6 × `== 402` and 4 × `!= 402` across the
scanned events, the most-tested value in the opening by a wide margin. The
load-bearing fact is that **`aaa/aaa_00_init_evt` admits only when
`GSW[0] == 402`** (cmd 19): the prologue module's init, and the only
single-equality gate in the set. The `!= 402` tests in `gor_00`, `gor_02`
and `tik_00` sit at the top of their inits and read as skip guards.

**I am not claiming 402 is the demo milestone's completion value.** The
tests are consistent with "prologue complete", but nothing scanned names it
and no trace shows a write of it. The only `GSW[0]` writer across all
sixteen events is `kurihakase_first_talk` cmd 176, and opcode 50 stores
`evtGetValue(operand1)` — so **which chapter the professor advances to is
not recovered**. This delivery recovers what gates test against, not what
sets them; the chapter *transition* stays unproven.

**34 distinct values**: `2 6 7 10 19 23 56 121 123 172 215 217 223 229
230 231 257 258 260 267 268 269 277 341 377 382 400 402 404`.
`GSW[0]` behaves as a **monotonic chapter counter** — values below 25 are
tested by identity, 121-404 also by threshold.

**Two range decompositions a port must not simplify.** `gor_02` cmds
97/101/102/109 give `>= 7`, `>= 10`, `<= 10`, `== 10` — an identity test
written as a threshold sandwich. `muj_00` cmds 63/70/75/76 give `<= 255`,
`< 260`, `>= 257`, `< 258`. Collapsing either into a single range test
changes which chapters are admitted.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. The **written values** for `GSW[0]` — opcode 50 stores
   `evtGetValue(operand1)`, so the transition is unproven until the stored
   value is resolved. This is now the top gap on the chapter gate.
2. Chapter *names* for the 34 values.
3. SWITCH (opcode 34) case bodies on the `GSW[0]` tests at `gor_00`
   cmds 12/98 and `gor_02` cmd 0.
4. Map `GSF` indices to save-block slots for GSF-01.
5. Remaining inbox: RESULT-01, TRACE-01, EVT-01 residue.

## 2026-10-13 — DEMO-01: opening area-init story-state map (ID 3)

[OPENING_AREA_INIT_STORY_STATE_MAP](behavior/story/OPENING_AREA_INIT_STORY_STATE_MAP.md).
Scanned the fifteen `gor`, `muj` and `tik` opening / area-init events with
the same REL-inventory + operand-namespace method as the previous delivery.

**Corrected an over-broad claim of mine.** The previous delivery said
`GSF` is "**never** used by any opening event". It was never used by
`dmo`/`aaa`/`sys` — all that scan covered — but across the area inits
`GSF` is the **dominant** persistent flag family. That claim is now scoped
to its actual evidence rather than deleted, since it remains true for those
three modules.

**`GSW[0]` is the chapter gate, read by every area init** — `gor_00` 12
times, `gor_02` 11, `gor_01` 10, `muj_00` 9, `gor_03` 6, `muj_20` 5,
`tik_00` 3 — with a **multi-way** opcode mix (`==`, `!=`, `<`, `<=`, `>=`,
SWITCH). A port testing only `== 0` will mis-admit chapters.

**The Professor writes `GSW[0]`.** `kurihakase_first_talk` in `gor_02`
writes it at cmd 176 — the only such write across all sixteen events
scanned in both deliveries. So the chapter transition is driven by the
professor scene; treating the prologue/epilogue writes as the sole producer
would miss the professor branch entirely.

**`GSW[1420]`** is a second story word in the opening (`gor_01` cmd 126,
`gor_02` cmd 63), stored as a byte rather than the index-0 word exception.

**`GSF` map**, including **`GSF[39]` written by both `gor_00` and
`muj_00`** — a cross-area marker, not an area-local one. Plus `gor_01` →
1191/1189/1190, `gor_02` → 1203, `gor_03` → 1229, `muj_00` → 3125,
`tik_00` → 6/237.

**Two joins resolved.** `gor_muj_ship_start` and `muj_gor_ship_start` both
exit to `muj_20` — the ship voyage is a **bidirectional join**. And
`muj_20/evt_prologue` exits to `sys_01`/`prologue`, making it the join
between the `aaa` prologue and the `sys_01` sequence.

**Not resolved:** which comparison literal on each `GSW[0]` test maps to
which chapter (only operand index and band tabulated), and whether the
`GSF` 5300-5600 band — which appears solely in opcode-91 operand positions
— is callee arguments rather than flag accesses. Neither is claimed.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. Extract the comparison literals on the `GSW[0]` tests to build the
   chapter-to-value table — that is the entry-condition answer DEMO-01
   still lacks.
2. Map `GSF` indices to save-block slots for GSF-01.
3. Confirm whether the `GSF` 5300-5600 band is callee arguments.
4. `gor_04_init_evt` and the remaining area inits.
5. Remaining inbox: RESULT-01, TRACE-01, and the EVT-01 residue.

## 2026-10-13 — DEMO-01: opening scene state and persistence census (ID 3)

Pivoted from EVT-01 — its static items are now essentially exhausted, with
the concurrent agent closing most of the rest. The inbox front-lines the
opening demo milestone (DEMO-01 / RESULT-01 / GSF-01 / TRACE-01), so I took
DEMO-01.

[OPENING_SCENE_STATE_AND_PERSISTENCE](behavior/story/OPENING_SCENE_STATE_AND_PERSISTENCE.md).
Answers the persistent-write ask and part of the entry-condition ask for
`dmo` / `aaa` / `sys`.

**Method note.** `tools/rel_event_inventory.py` reads each annotated REL
event span from the read-only ISO and annotates relocated operands with DOL
symbol names. Routing each integer operand through the published
[namespace table](behavior/event/EVENT_INTEGER_OPERAND_NAMESPACES.md)
(`index = wrap32(operand - base)`, signed32) turns those annotations into
a concrete state census, and opcode direction comes from the existing
assignment and IF contracts. Ten event spans scanned, SHA-256s tabulated.

**Headline: the opening's cross-scene persistence is a single word.**

- **`GSW[0]` is the only saved story value** any opening event touches.
  Written by `aaa/evt_prologue2` cmd 49 and `aaa/epilogue_evt2` cmd 117;
  **read as a gate** by `aaa/aaa_00_init_evt` cmd 19. It is the documented
  index-0 exception and must be stored as a **word** at GP+174, not a byte
  in the GP+578 array.
- `GSF` is **never used** by these modules. Save-block 233/236 stay with
  GSF-01; nothing here claims they are unreached.
- **`GF` (16 uses) and `GW` (4) do not persist** — they belong to the
  current event context, not BattleWork or the GP save arrays. Every `GF`
  use is a set-then-test **within one event**: `demo_evt` writes `GF[0]` at
  cmd 294 and reads it at 324, writes `GF[1]` at 295 and reads it at 330,
  and writes `GF[10]` at cmd 1 and reads it at cmd 382. That last pair is
  the one most likely to be mistaken for a persistent flag.
- `LW` (292 uses) is entry-local and needs no persistence.

**Branch/join graph — and a correction.** My first pass reported the
`evt_bero_mapchange` targets as "raw immediates the inventory does not
decode". **That was wrong**: the inventory resolves relocated string
operands and emits them as `identifier`; I had read only `raw`. The route
decodes to a nine-step chain in `sys/evt_start`

```
gor_10 -> hei_00(dokan_2) -> win_06(dokan1) -> tou_20 -> usu_00(dokan_1)
       -> muj_20 -> rsh_02_a -> bom_00(dokan) -> las_00(w_bero)
```

plus four `aaa` exits (`gor_10`, `muj_20/prologue`, `gor_02`, `end_00`).

And the **ordering** ask falls out: `GSW[0]` is written *immediately
before* the fade in `evt_prologue2` (write cmd 49, fade 48, map change 50)
and two commands before in `epilogue_evt2` (117 / 118 / 119). The opening's
persistent state is committed **before** the transition, matching the
save-notice ordering GSF-01 pins.

Not claimed: whether the nine are a *linear* route — they are sequential
commands, but nothing here shows each waits on a trigger. `-1` appears in
the demo fades as a likely "unchanged" sentinel; that semantics was not
verified.

Also flagged rather than guessed: opcode direction for **93, 95 and 108**,
which carry the four `GW` operands (claimed as neither reads nor writes);
the `gor` / `muj` / `tik` modules, unscanned this pass; and the **written
values**, since opcode 50 stores `evtGetValue(operand1)` rather than a
literal operand.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. Scan `gor` / `muj` / `tik` — the professor, underground and lecture
   legs — for the same state census, which is what remains of the
   harbor → professor → underground → lecture graph.
2. Find each branch's **requirement** (gate), which the scanned modules do
   not contain.
3. Opcode direction for 93/95/108.
4. Remaining inbox: RESULT-01, GSF-01, TRACE-01, then the EVT-01 residue
   (Monosiri mutation, NormalJump menu/global writers).

## 2026-10-13 — The startup spatial route is dead code (ID 3)

Resolved the item the previous entry left as "not determined": what sets
bit 15 of `slot+0x06`. **The answer is that nothing can**, so the startup
`calc3D` route is unreachable.

The four steps are recorded in
[POSITIONAL_SFX_BACKEND_DISPATCH](behavior/audio/POSITIONAL_SFX_BACKEND_DISPATCH.md)
and in the EVT-01 reply. In brief:

1. The guard is `(int16)slot+0x06 < 0`, i.e. bit 15 — `lhz` sign-extends and
   mask `0x40000000` sits in the sign-extension.
2. `__psndSFXOn` builds the field from `0x101` plus ORs of 2, 4, 16, 32,
   128, 512, 640, 1024 and 2048. **Maximum `0xFB7`** — below `0x8000`.
3. A binary-wide scan for the `addi rD, rA, -1656` forming the `pssfx`
   address (`0x803DF988`) returns **22 sites in 13 functions, all inside
   `pmario_sound.o`**. A second scan for the alternate global-relative
   route (`imm = 0x806 + 40*i`) returns 4 candidates, all in unrelated
   files and all against a different base register. **No external writer
   exists.**
4. The only remaining in-file stores to `slot+0x06` are the ten provisioner
   ORs, the normalisation/clear pair in `psndSFXMain`, and
   `sth 5, 6(6)` in `psndInit` — none can set bit 15.

So the `calc3D` call at `0x800DA264` is unreachable.

**Corrected the same day:** my claim that attenuation then "flows through
the live site at `0x800DA3F8`" was wrong. That site is *also* unreachable —
`0x800DA394` clears `slot+0x06` four instructions before its own test at
`0x800DA39C`, so `bt 2` always branches away. The two sites use **different**
`rlwinm` masks (`0,24,22` preserves the low 16; `0,16,30` clears it) and I
had conflated them. Both `calc3D` call sites are unreachable; the PAN record
is never written by `psndSFXMain`. Attenuation still exists because
`__psndSFXOn` computes a distance at provisioning time into `slot+0x0E`/
`+0x10` and `psndSFXMain` counts it down.

**Why this is worth stating rather than leaving open.** A negative result
like this is easy to re-derive wrongly: a reader who sees a `calc3D` call
inside the startup path would reasonably implement both spatial paths, or
assume the startup one is primary. Both would be additions to the
original's behaviour.

Practical consequence for the port: **do not delete either path on this
evidence.** This is class B static analysis with no execution trace, and a
decompilation reading can be defeated by an untraced path — for instance if
`0x800DA528` or the reclaim loop re-enters the startup block with a field
value set elsewhere. Implement both, or reproduce the guards exactly, until
a trace confirms `calc3D` is never entered.

Process note: this was the third error in this area in one session, and all
three had the same cause — reasoning about masks, sign-extension or branch
polarity from memory instead of reading the bytes and computing them. Each
was caught only by going back to the disassembly.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. The live `calc3D` site `0x800DA3F8` and its `PAN` consumer chain — the
   last structural item in the positional-SFX chain.
2. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.

## 2026-10-13 — `rlwinm` masks and the live `calc3D` site: two corrections (ID 3)

Chasing the bit-30 writer flagged in the previous entry produced two
corrections to my own work, one of them to a correction I made while
checking.

**"Bit 30" was a mislabel.** `lhz 0, 6(25)` **sign-extends**, and the mask
`0x40000000` sits wholly inside the sign-extension region, so the condition
is really `(int16)slot+0x06 < 0` — bit 15. That matters: a reader taking
"bit 30" literally hunts for `0x40000000` in the provisioner, finds nothing,
and wrongly concludes the spatial route is dead. The contract now states it
in code form.

**A documented claim was wrong.** I had written that `0x800DA048`
(`rlwinm 0, 3, 0, 24, 22` / `sth 0, 6(25)`) *clears* `slot+0x06`. It does
not: with **MB > ME** the mask wraps and **includes bits 0..15**
(`0xFF7FFFFF`), so the store preserves the low 16 and merely normalises a
sign-extended halfword. The genuine clear is `rlwinm 0, 0, 0, 16, 30` at
`0x800DA390` (MB <= ME, mask `0x7FFF0000`) — in the **live** pass, after
the startup test. Both masks are now tabulated side by side so they cannot
be conflated again.

I reached the wrong answer twice by reasoning about `rlwinm` mentally
instead of computing it. Decoding both cases — including the MB>ME wrap — in
Python is what settled it, and the result is recorded.

**Substantive finding: two `calc3D` sites, one live.** `0x800DA264` sits in
the startup path behind the sign guard; `0x800DA3F8` sits in the live pass
with no such guard and runs every frame for playing sounds, followed by a
`row.word1` bit-0 dispatch to `SoundSSSetPanCh`. So the per-frame spatial
update serves the stream backend too — my dispatch doc implied effects
only, and is corrected.

**Still open, honestly:** what sets bit 15 of `slot+0x06`. The provisioner
tops out at `0xBB7`; the only other writer in `pmario_sound.o` is
`sth 5, 6(6)` in `psndInit`. A binary-wide scan for any `addi`/`oris`
loading `0x4000` found nothing in `pmario_sound.o`, which is consistent
with the provisioner never setting it but does **not** rule out a `stw` of a
data-derived value from another file. Either the startup spatial route is
dead for provisioner-produced slots or an external writer exists; **not
determined**.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. The bit-15 writer for `slot+0x06`, searching outside `pmario_sound.o`
   (the remaining blocker on the effect startup route).
2. The live `calc3D` site `0x800DA3F8` and its `PAN` consumer chain.
3. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.

## 2026-10-13 — `psndSFXMain` backend dispatch conditions (ID 3)

[POSITIONAL_SFX_BACKEND_DISPATCH](behavior/audio/POSITIONAL_SFX_BACKEND_DISPATCH.md).
Closes the per-channel dispatch item left open in the frame-driver
contract.

The backend is selected by **two bits of one word**: `pssfxlist[row].word1`
(offset `+0x04`), bits 0 and 1 — bit0 clear → stream, bit0 set and bit1
clear → song, both set → effect. This is the same encoding `psndSFXOff`
already uses to pick `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`,
so stop and start agree by construction.

The interesting part is how much the three routes differ:

- **3 stream channels, 4 song channels, effects unpaged.** Stream and song
  both probe for a free channel via their `Check` service; effects don't
  probe at all and store whatever `SoundEfxPlayEx` returns.
- **Only the stream route sweeps stale channels**, in the order
  find-free → sweep-stale → find-free, storing `-1` if the second probe
  fails. A port that only probes would report exhaustion while channels
  are actually occupied.
- **Resource ids come from different fields**: streams `row.word2`, songs
  `row.word1 & 0xF`, effects `row.word1 & 0xFFFF` — the type word's own
  low bits for the latter two.
- **Bit 30 of `slot+0x06`** gates the positional route for effects; when
  set `calc3D` runs and its `PAN` record is staged before `SoundEfxPlayEx`.

All three converge at `0x800DA36C`, where an exact `-1` handle clears
`slot+0x00` and `slot+0x18` and skips the slot.

**Flagged, not resolved:** nothing sets bit 30 of `slot+0x06`. The
provisioner sets 1, 2, 4, 16, 32, 128, 256, 512, 640, 1024 and 2048 but
not 30, and `psndSFXMain` clears the whole field at `0x800DA048` for
engine-backed slots each frame. The spatial route therefore depends on a
writer outside this family, which I did not locate. That single gap
prevents the effect route being fully closed.

**On the `r2` pool:** I swept beyond the two tabulated windows and the
region is **larger than documented** — dense with π/180, 2π, π/4, π/2, 6π,
π/3, 2π/3 and 1/2^k scales. I deliberately did **not** state a boundary:
my automatic literal detector rejects π/180 and 2π themselves while
accepting integer tables read as floats, so any extent it produced would
be an artifact of the filter. Recorded in the contract as a measured
extent plus an explicit refusal to bound it.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. The **bit 30 writer** for `slot+0x06` — the last blocker on the effect
   route. Search outside `pmario_sound.o` for writers of that field.
2. The later half of `psndSFXMain` (`0x800DA330`..epilogue): per-frame
   volume/pan/pitch updates for playing sounds.
3. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.

## 2026-10-13 — `r2` small-data base resolved: the provisional-constant gap closes (ID 3)

[SMALL_DATA_R2_BASE_AND_FLOAT_POOLS](behavior/binary/SMALL_DATA_R2_BASE_AND_FLOAT_POOLS.md).
This closes the item I recorded as "the largest single source of
provisional numbers" across three sessions, and retires the fabricated
`0x800494xx` addresses I published in the field-collision session.

**`r2 = 0x8042B038`, verified rather than guessed.** My first attempt at
this (a brute-force scan for a plausible float pool) produced 3700
candidates and then 11862 — the filters were worthless because integer
tables and zero regions dominate. What settled it was anchoring on a
semantic fact I had already established: `marioSlitChkWallAround` sweeps 8
equally spaced directions with its increment at `-27524(r2)`, and at this
base that word is **π/4** — 8 × 45° = 360°, exactly what the loop provably
does. A base 4 bytes higher puts 2.0 where π/4 must be, so alignment is
unambiguous. The audio pool at the same base then independently yields π/180,
1/128, 1/32, 2π, π/9, π/30, 360, 255 — a coherent trigonometry pool.

**`r2` is shared across object files.** The `mario_hit.o` and
`pmario_sound.o` pools are two windows into one contiguous constant region
at `0x8042449c`..`0x8042550c`, not per-file bases — which is why one base
resolved both.

46 field and 21 audio constants are now ISO-read and annotated inline in
the affected contracts. Concrete consequences:

- `marioCheckWall` / `marioCheckWall_roll`: gate is **`f1 >= 24.0`**, and
  `mario[+0x214]` is published as **40.0**.
- `marioSlitChkWallAround`: angle step confirmed **π/4**.
- Roll/head-search branchless select: **-0.01** and **1.4**.
- `searchFrontWall`: compares `fabs(d)` against **78.75**.
- `calc3D`: scales by **π/180**, **1/128**, **1/32**, multiplies by **π/9**.
  Its `3.14` `lfd` is a separate constant from the pool's 2π and π/180.

I also repaired six field/audio documents where an earlier bulk regex had
mangled disassembly lines into `8008b7c4lfs lfs 0, , -27540(r2)`.

**Flagged, not resolved:** `-23384(r2)` is `0.0` and sits in the divisor
of `fdivs 0, 3, 2` in `calc3D`. The pool value is not in doubt and the
instruction order is unambiguous; whether the branch is reachable or
guarded was not determined, so I left it open rather than rationalising it.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. `psndSFXMain`'s per-channel dispatch conditions — the last item in the
   positional-SFX chain.
2. Constants outside the two windows, and whether the pool continues past
   them.
3. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.

## 2026-10-13 — Audio frame driver, plus two corrections to my own ID 3 work

Added [AUDIO_FRAME_DRIVER_AND_SFX_UPDATE](behavior/audio/AUDIO_FRAME_DRIVER_AND_SFX_UPDATE.md)
as documentation agent ID 3: the complete 84-byte `psndMain` and
`psndSFXMain` at structure level. `psndMain` is gated on **`init_f`**
(`0x8041EB08`), decrements the signed-byte cooldown at `0x800DC4AC..4BC`
**before** a fixed service order — `psndBGMMain`, `psndENVMain`,
`psndSFXMain`, `SoundMain`. `psndSFXMain` has a literal `cmpwi 31, 40` loop
bound (independently confirming the 40 slots), reaps slots via
`SoundSSCheck`, clears `slot+0x06` and counts down `slot+0x0E` per frame,
writes the live count to `G5B`, and resets the preference cursor `G54` to 0
every frame. That last fact matters: the `+8` cursor advanced in
`__psndSFXOn` is a **within-frame** round-robin, not persistent state.

**Two errors of mine, corrected in place and recorded:**

1. **Cooldown owner.** I stated three times that `psndSFXMain` owns the
   cooldown decrement and that omitting it would wedge all further starts.
   Wrong — `psndSFXMain` never touches `global+0x5c`. The decrement is in
   `psndMain`, gated on `init_f`. Corrected in the provisioning contract,
   the delivery log and here. The real practical dependency is therefore
   `psndMain`, not `psndSFXMain`.

2. **`clrlwi x, 19` decodes to the LOW 13 bits**, not the top 13. I read it
   as `key >> 19` throughout, which made the "unresolved `slot+0x00`
   index-versus-packed-key conflict" I reported as an open item an
   artifact of my own error. The allocator settles it: `r31 = clrlwi(r18,
   19)` where `r18` is a plain 0..4241 scan index, then `cmplw 0, 31`.
   Under the top-13 reading `r31` is always 0 and the slot match is
   vacuous; under low-13 it is an exact match. Independent work by another
   agent (`894a5ad`) reached the same conclusion and consolidated the
   accessors, stop and provisioning contracts, correcting them; I have
   adopted its corrected readings rather than maintaining a divergent
   copy.

A third of mine was also wrong and is now corrected: I claimed
`slot+0x04` and the returned handle diverge when the 16-bit channel
counter wraps. `0x800D9E50` **reloads** the counter after the store, so
both always use the same truncated value.

**Note for whoever picks this up:** another agent rewrote three of my
contracts in `894a5ad`, consolidating ~950 lines into ~350. My strcmp/4242
name scan, the 517/869 gates, the dead volume clamps and the handle
packing survived in the consolidation; the detail that did not survive is
folded into their
[POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS](behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
and my new frame-driver contract. `docs/behavior/README.md` has now been
overwritten by concurrent commits four times; rows must be re-verified
before the index is trusted.

Docs-only; `make test` / `make all` not rerun.

Next tasks:

1. `calc3D` (1268 bytes) — the distance/pan model behind `slot+0x0E` and
   `slot+0x10`, and the only remaining positional-SFX consumer.
2. `psndSFXMain`'s per-channel dispatch conditions (which pssfxlist field
   or slot flag selects each of the ~20 backend services).
3. `psndInit` initial global values; `psndENVMain`/`psndBGMMain` touching
   `pssfx` or the global block.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.

## 2026-10-12 — `__psndSFXOn` slot provisioning decoded (ID 3)

Third EVT-01 pass as documentation agent ID 3, taking the top item from
the previous queue: `__psndSFXOn` (`0x800d9934`, 1344 bytes), the last
structural gap in the positional-SFX chain because it is the **only**
writer of the `pssfx` fields the accessors read.

Delivered [POSITIONAL_SFX_SLOT_PROVISIONING](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
decoded end-to-end. Evidence class B; the `make test` / `make all` gates
were not rerun because this is docs-only.

Recovered:

- **The `key` argument is a string.** Name resolution is a `strcmp` scan
  over `pssfxlist` (stride 20, up to **4242** entries) against
  `word0`. Key `0` skips the scan; a scan that runs off the end resolves to
  index 0. There is no not-found failure.
- **Three pre-allocation gates**: global `+0x56` bit 29 as a mute that
  applies only when `arg7 != 0`; a signed-byte **cooldown of 6** at global
  `+0x5c` that rejects while nonzero; and for index 869 an event flag at
  `0xF5DE0180` that must equal 2. Indices 517 and 869 are the only
  per-index specials.
- **Two-stage slot search**: 20 same-key slots first (with a `+8`
  preference cursor at global `+0x54`), then a 40-slot reclaim pass that
  **inlines a full copy of `psndSFXOff`** — same low-5 gate, same
  `key >> 19` table index, same two-bit SoundSS/Song/Efx stop — and
  additionally requires `slot+0x0c == pssfxlist[+0x10]`. Exhaustion
  returns `-1`.
- **Channel bits are a pure function of the pssfxlist index**: base
  `0x101`, plus bits 5/13/14/15 of the index and per-index/per-field
  overrides. This is the routing mechanism.
- **Handle packing confirmed**: `channel = ++global[0x50]`,
  `slot[+0x04] = (u16)channel`, return `slot_index | (channel << 8)`.
  Independently confirms what `psndSFXChk` validates. Recorded asymmetry:
  `slot+0x04` gets the full 16-bit counter but only `<<8` reaches the
  handle, so the two diverge when the counter wraps.
- **Two unreachable volume clamps** at `0x800d9c60`/`0x800d9c70` — the
  `clrlwi ...,24` then `cmplwi ...,255` can never be greater. Recorded as
  observed rather than "fixed" into live clamps.

**One conflict deliberately left open.** `__psndSFXOn` writes `slot+0x00`
as the bare pssfxlist index (0..4241), but `psndSFXOff` and `psndSFX_pit`
derive their pssfxlist row from `(slot+0x00) >> 19`, which is always 0 for
that range. Read literally the accessors always resolve `pssfxlist[0]`.
The bytes are unambiguous (`stw 18, 0(29)`) and no second writer is
visible, so **no reconciliation was invented**. Flagged in the contract and
in the EVT-01 reply as something to settle before either side is relied
on.

**The practical dependency this created:** nothing in `__psndSFXOn`
decrements the cooldown. If `psndSFXMain` is ported without its decrement,
`global[0x5c]` stays nonzero and every later start fails permanently. That
makes `psndSFXMain` (2964 bytes) the single remaining item in the chain.

Registered one validated delivery row and appended the reply to
[requests/EVT-01.md](requests/EVT-01.md).

Next tasks:

1. **`psndSFXMain`** (2964 bytes) — cooldown decrement, preference-cursor
   expiry, and the reader of volume, distance and the published position.
   This is the last item in the positional-SFX chain.
2. Resolve the `slot+0x00` index-versus-packed-key conflict above.
3. Monosiri record layout beyond `+0x04` and its table writers.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` remains a contended file; verify rows survive.

## 2026-10-12 — OW-MOVE-01 input remainder: key-data update

Re-extracted marioUpdateKeyData (0x80095cb0, ISO read-only) and contracted
it in behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md: six GP pad
bytes with alternating negation into +594..599, [600/601] zeroing, flag
halfwords, fixed-order per-flag pair zeroing, key-check gate and the
0x803DA398 rising-edge word. No deadzone/clamp exists in-body (native .15
is authored). Reference pair placed under ignored artifacts/.

Validation: instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains, deadzone
ownership elsewhere. Full-game incomplete.

## 2026-10-12 — Full-tree gap re-audit plus lightSort alias

Re-ran the name-mention audit over all 664 reference binaries against the
current docs tree: exactly one unnamed file (`lightSort`), verified as alias
of contracted `lightdrv _sort` @8001bccc (three-way float compare plus
`sorter800064fc` caller in behavior/display/MAP_CHARACTER_LIGHT_QUERIES.md)
and recorded in the register. Re-added the plane-motion README row dropped
by the recent restructure. Zero unnamed-and-uncontracted binaries remain.

Validation: audit script, listing-vs-doc structural match, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains,
pipe lifecycle. Full-game incomplete.

## 2026-10-12 — Inbox request: EVT-01 positional SFX chain (ID 3)

Worked the [request inbox](DOCUMENTATION_REQUESTS.md) as documentation
agent **ID 3** (the ID the user assigned; ID 1 and 2 were already taken).
Picked the priority-0 EVT-01 audio chain, which had **no dedicated contract
at all** and was explicitly listed as open by three existing documents.

First surveyed the inbox: `docs/requests/` per-request files plus the
[delivery log](requests/DELIVERY_LOG.md) lock table. Only one lock was
active (ID 2, audience target accumulator), so EVT-01's SFX subtask was
free. Confirmed the manager loop and constructor policies making up the rest
of EVT-01 todo 1 are already complete in `EVENT_MANAGER_COMPLETE_LOOP.md`,
`EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md`,
`EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md` and
`EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md`; the genuinely unclaimed work was
the audio chain.

Delivered three **complete bodies** in `docs/behavior/audio/`:

- [Unit positional SFX trigger](behavior/audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md)
  — `BtlUnit_snd_se_pos` 80126088/114
- [Positional SFX position update](behavior/audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)
  — `psndSFX_pos` 800d96f8/4c
- [Positional SFX stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md)
  — `psndSFXOff` 800d95f8/c8

New facts, none previously recorded anywhere in the repo:

- The shared **positional-SFX slot record**: base `0x803df988`, **stride
  40**, selected by the **low 8 bits** of the handle. Fields `+0x00` key,
  `+0x06` channel bits, `+0x18` handle, `+0x1c/+0x20/+0x24` position.
- The **channel table** at `0x80316b28`, **stride 20**, indexed by
  `(slot.key >> 19)` — by the key, not the handle. Two bits of `word0`
  select `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`.
- **Stop-without-stop**: `psndSFXOff` clears the record without calling any
  stop service when `(halfword6 & 0x1F) == 0` or `handle == -1`; and an
  already-free slot returns *without writing*, so a double stop is a true
  no-op rather than a re-clear.
- `psndSFX_pos` writes **exactly three words** and never the key or handle,
  so it cannot start or stop a sound.
- `BtlUnit_snd_se_pos` status 10 **overrides** status 11 when both are set
  (sequential `if`s overwriting one register), and its start id is
  `(int16)(irand(41) + belong + offset - 20)`.
- The volume predicate is `r0 < 0x4D80` unsigned, where
  `r0 = (T & 0xFFFF0000) + 0xE7700000`. For the sentinel the movement-sound
  callers actually pass, `250000000`, this is the **preserve-current-volume**
  path. Derived from the compared words, not an observed trace.
- The wrapper's return is the **handle**, confirming the existing R18 store
  in `BATTLE_MOVEMENT_SOUND_CONTROL`.

Updated [BATTLE_MOVEMENT_SOUND_CONTROL](behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)
and [OVERWORLD_MARIO_IDLE_SOUND_CUES](behavior/overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
to link the new bodies instead of listing them open; no contradiction found
with the existing low-8-handle-bits claim. Registered and then **released**
the lock in the [delivery log](requests/DELIVERY_LOG.md), marked the EVT-01
todo answered with an explicit list of what stays open, and appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md).

Validation: all three bodies disassembled from the read-only ISO with ELFs
written to `/tmp` only; every callee resolved against `us_symbols.csv`;
disassembly re-read against each written instruction block; `git diff
--check` clean; README, cross-link and request-file link audits all clean.
Docs-only, so `make test` / `make all` were not rerun. Full-game incomplete.

Note: while this session ran, commits ec92b10/990b12e/488983a landed from
other work and prepended entries above this one. No conflict; this session's
field-collision entry is intact at its original position.

Next tasks, in the order the inbox implies:

1. **`__psndSFXOn` provisioning** (`0x800d6934`, 1344 bytes) — the slot
   allocation and macro/voice/resource chain that fills the record layout
   recovered above. Now the biggest blocker for the audio chain, and the
   known layout makes it tractable.
2. **`battleGetUnitMonosiriPtr` 8012ef20** — still explicitly unclaimed,
   with no dedicated contract. Small and bounded.
3. **`psndSFXMain`** (2964 bytes) — the reader of the published slot
   position, which closes the update/stop loop from this session.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, `NormalJump`
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, and the `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. The constant block `0x80049468`..`0x8004950c` from the field-collision
   session is still unextracted.

## 2026-10-12 — Re-extracted placeholders: plane flyMain contract, two aliases

Re-extracted three zero-byte placeholders read-only via
tools/inspect_dol_function.py into behavior/: mot_plane_flyMain (mot_plane.o
flyMain @8009c9b8, contracted in behavior/overworld/OVERWORLD_PLANE_FLIGHT_MOTION.md
with ISO-read SDA set; fixed my own hex/decimal stage confusion pre-commit),
btlevtcmd_AnnounceMessage (matches contracted announcement doc — alias),
PSVECDotProduct (9-instruction leaf contracted in the traversal doc).
Remaining placeholders (_announceMessage/BattleGetFirstAttackTarget/
partyGetOtherPtr/partyGetTargetNum cases) have no DOL text symbols and cannot
be extracted by name.

Validation: extraction outputs verified (ELF identity, entry/size match),
stage immediates re-checked after the hex fix, link audit, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains.
Full-game incomplete.

## 2026-10-12 — Fifth gap pass: guard wrapper, act-status publisher, small-data setter and float-to-unsigned

- [Small unit/runtime leaves](behavior/unit/SMALL_UNIT_RUNTIME_AND_MOTION_LEAVES.md):
  `BtlUnit_CanGuardStatus` (801272dc) is a frame-only forwarder to
  `BtlUnit_CanActStatus` (8012724c), so a `bl 801272dc` site is a `bl 8012724c`
  site for call-graph purposes. `btlevtcmd_CheckActStatus` (80105cb4) resolves a
  unit id, calls that same `BtlUnit_CanActStatus`, and publishes a fixed literal
  1 or 0 through `evtSetValue` using the **raw** second operand word as the
  destination. `unk_80151d20` (party_motion.o) is a three-instruction setter
  that writes the constant `60.0` (0x42700000 at 0x8042318c) to the small-data
  slot at `0x8041ec10` and returns. `__cvt_fp2unsigned` (80264860, runtime.c)
  maps `f1 < 0` to 0, `0 <= f1 <= 2^32` to `0xFFFFFFFF`, `f1 > 2^32` to
  `trunc(f1)` and NaN to `0xFFFFFFFF`; the 2^31 constant at `0x80303a30` is
  loaded but dead on every reachable path because the `f1 > 2^31` branch is
  always true once execution passes the 2^32 test.

Register state after this pass: 360 reference listings have no snapshot
definition; 31 of them still have no substantive prose contract. The largest
remaining unaddressed bodies are `seq_battleMain` (253 lines), `partyReInit`
(461), `_getSickStatusParam` (286), `JUTFontSetup` (196),
`FontDrawStringVecPitch` (211), `pouchRemoveItemIndex` (183),
`npcSetupBattleInfo` (166), `vivian_move`/`cloud_move`, the pad readers
(`DEMOPadRead`/`DEMOPadCopy`), `btlevtcmd_ConsumeItemReserve` and
`btlevtcmd_AnimeChangePoseType`. `marioLandOn` and `marioSearchHead` are now
carried by multiple contracts and are no longer counted as open.

Validation: docs-only; `make test` and `make all` were run for the previous pass
and are unchanged by this one (logs `/tmp/pm-gap3-test.log`,
`/tmp/pm-gap3-build.log`). `python3 tools/check_doc_links.py` reports 0 broken
links. No native/test/asset conversion change, hence no runtime, cadence,
emulator or original-execution claim. EVT-01/JUMP-01/DMG-02 and the full-game
requirements remain incomplete.

## 2026-10-12 — Native JUMP-01 correction: packed Mario jump SFX

Verified the JUMP-01 SFX-provenance answer against the read-only ISO and found
it analysed the generic `btlevtcmd_JumpPosition` (0x8010d208/0x4c4), not the
body the native port binds, `btlevtcmd_MarioJumpPosition` (0x801093a8/0x8c0).
Reproduced the Mario body from the DOL (`tools/inspect_dol_function.py`):
r30=0x802ee458, so the only 0x802E reads are the dead 24/28/32(r30)=
0x802EE470/474/478 loads (overwritten by unit floats before the call); one
`psndSFXOn_3D` (0x801098fc) keyed on `word[unit+0x18c]`, position
unit+0x154/158/15c; heading/distance from `system_angleABf` 0x8000703c /
`distABf` 0x8000722c and sine from MSL `sin` 0x80272410. Applied the one
native fix that survives: removed the three dead `0x802ee470` reads in
`source/battle/battle_packed_mario_jump.cpp`; kept the single `play` call (the "model
both records" follow-up came from the generic body and is withdrawn). Filed the
correction breadcrumb in `docs/requests/JUMP-01_HISTORY.md`.

Validation: `make test` 87 CTest / 90 Python pass; `make all` 3DS build pass;
`diff --check` clean. Docs are mid-restructure (inbox split into
`docs/requests/`), so the breadcrumb went to the per-request history file, not
the index. Full game still incomplete.

## 2026-10-12 — Medium-size field collision functions (mario_hit.o)

Continued the medium-size sweep requested for a US port. First verified
that the 359-function fingerprint set in
`artifacts/reference/behavior/evidence.json` is now fully referenced by
existing docs (its `documented` flag is stale and was not used), then
widened the search to the full `us_symbols.csv`: 1098 game-text functions
in the 500-1500 byte band, 810 of them not referenced anywhere.

Worked the Mario field-collision family, picked because it is directly
needed for movement fidelity. Five new contracts in `docs/behavior/field/`:

- `MARIO_SLIT_CHK_WALL_AROUND.md` - complete structural read of the 8
  direction slit probe: two sweeps, three-way per-direction
  classification, and the two-group triple-run rule that returns 0 on
  three adjacent blocked directions.
- `MARIO_CHECK_WALL_ROLL.md` - the roll wall check, including the 14-tick
  wall-hold re-arm record at `mario+0x1cc..0x1d4`.
- `MARIO_SEARCH_HEAD_ROLL.md` - the 5-iteration indexed direction dispatch,
  with the `frsp` roundings between trig and scale called out.
- `MARIO_SEARCH_FRONT_WALL_STAYING.md` and `MARIO_CHECK_WALL.md` - both
  documented as **complete bodies** (every branch and call site).
  The staying search contributes the `mario[+0x2e] == 19` veto, the
  field-name gate and the triple `vivianGetStatus` gate; `marioCheckWall`
  contributes the 5-char hit-name hysteresis (thresholds 10/20) on a
  **signed** byte at `mario+0x3f` and the in-place `movePos` slide.
- `SEARCH_FRONT_WALL.md` and `MARIO_CHK_WALL_AROUND.md` - the two largest
  callees (2404 and 2460 bytes), documented at **structure level only**:
  entry gates, probe grouping, SDK vector math. Their probe sequences and
  result reduction are explicitly not decoded.

Cross-references in the earlier three docs were corrected where they listed
these two callees as undocumented, including one `marioChgWallAround` /
`marioChkWallAround` transcription error.

Newly recorded facts: the field literal at `0x800c3d88` gates both
`searchFrontWall` and stage 3 of `marioSearchFrontWall_staying`, so those
two paths are tied to one specific field; the 5-char hit-name literal
compared in `marioCheckWall` lives at `0x80049504`.

Validation: all callees resolved against `us_symbols.csv`; disassembly
regenerated per function into `/tmp` only; every `git diff --check` clean;
README link audit and field cross-link audit both clean. Docs-only, so
`make test` / `make all` were not rerun. Full-game work remains
incomplete.

Limitations: no Dolphin observations were taken this session, so every
claim is class B. The two structure-level docs must not be read as
equivalent to the original implementations.

Next tasks:

1. Decode the 9-probe reduction in `searchFrontWall` and the 12-probe
   reduction in `marioChkWallAround` - these block full movement parity.
2. Read the two unresolved literals from the ISO: `0x800c3d88` (the
   gated field name) and `0x80049504` (the hysteresis hit-name prefix).
3. Extract the `lfs` constants at `0x80049468`..`0x8004950c` - they are
   referenced by nearly every function documented in this session and are
   currently the largest single source of provisional numbers.
4. Continue the 810-function medium backlog; `party/*` and
   `npcCheckHitMarioSide` / `mobjHitEntry` are the next highest-value
   targets after collision.
5. `docs/PROGRESS.md` tables should be reconciled against actual file
   contents if any further drift is found.

## 2026-10-12 — Native request 3 slice: pose call-site survey

Delivered OVERWORLD_POSE_CALL_SITE_SURVEY: 18 marioChgPose sites with
ISO-read literals (KPA_D_1/D_2 table pair, M_D_2/M_U_3 recovery pair,
M_J_1B/M_J_1C, M_W/M_O/M_R alternation) plus gate/ordering notes; all
surveyed sites are marioChgPose (no EvtPose). Remaining request-3 items
(named domains, pipe-paper asset work) explicitly scoped out with a
non-finding (no reference body) for the pipe lifecycle.

Validation: all 18 call sites re-checked, literal strings ISO-read, link
audit, diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — Native request 2 remainder: bottomless numerics

Delivered OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS: first-tick gate,
win_03/win_01/tik_01 lane selection (rodata-read), angle setup, stage
machine via ISO-read table (0/2/10/11/12 live, 13 terminal-by-gate),
fallSpeed-shape integrator, min −10.0 snap, land/HP chain, and the
0.34/−0.002/−0.01 drop-back triple (port's jerk cascade matches one lane).
Dead −2000.0 load and SDA set pinned from ISO. Request 2 fully answered
except float-chain interiors.

Validation: call/SDA/stage-write re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: pose-table slice (request 3). Incomplete.

## 2026-10-12 — Native request 2: water recovery numerics

Delivered OVERWORLD_WATER_RECOVERY_NUMERICS for field/AC-03: full
stage/counter machine ([68]=stage via ISO-read table 0x8036ED4C, [72]=
counter), 12-slot ISO-read SDA set, sink −6.0 init, 40-count stage-1 hold,
two-tick stage-10 arm, HP-penalty tick at stage-31 expiry, 12-count stage-32
window, and named gaps where the port authored ratios/jerk (no 0.08-class
constant, no jerk lane in-body). Stale "stage15" has no slot (no-op).

Validation: dispatch/stage-write/SFX-ID/HP-call re-checks with int/float
register-file care; link audit; diff --check clean. Docs-only, no make
rerun. Next: bottomless motion33/34, pose-table slice. Full-game incomplete.

## 2026-10-12 — Native JUMP-01 request 1 answered (SFX provenance)

Fulfilled the packed-Jump provenance request from the native inbox with
BATTLE_JUMP_SFX_PROVENANCE: vec3 scratch proven zero-bytes (not a key row),
U+0x18C key domain with two-call psndSFXOn_3D record (second inherits r3),
and no-PS-math finding for 801093a8. Recorded the answer in
DOCUMENTATION_REQUESTS.md without touching the request text. Water/motion
and pose-table requests (2/3) remain open — mot bodies located for next.

Validation: dual-offset ISO reads, call-site arg traces, PS-op sweep,
link audit, diff --check clean. Docs-only, no make rerun. Full-game
incomplete.

## 2026-10-12 — Third and fourth gap passes: event leaves and the shadow driver

- [Weapon/status/wait leaves](behavior/battle/BTLEVTCMD_WEAPON_STATUS_WAIT_LEAVES.md):
  six `battle_event_cmd.o` bodies with no snapshot definition.
  `btlevtcmd_GetWeaponActionLv` publishes `lbz [resolved+0x6E]` through
  `evtSetValue` with a **raw** destination word; `btlevtcmd_AcSetDifficulty`
  drives `BattleActionCommandSetDifficulty(B, unit, value)`; and
  `btlevtcmd_AnnounceSetParam` writes into an 8-byte-stride record array at
  `BattleWork+0x18C8C` at the `+4` value word of `4*index`, with no bounds check.
  `btlevtcmd_AfterReactionEntry` resolves operands into
  `BattleTransID`/`evtGetValue` and calls the queue helper from the
  after-reaction contract. `btlevtcmd_StatusWindowOnOff` is a signed four-way
  dispatch writing `word[B+0x2748]` (modes 0/1) or `word[B+0x2744]` (mode 3) and
  then always calling `BattleStatusWindowCheck`; those are exactly the two words
  whose nonzero state forces that checkpoint's result to 0, which is recorded as
  a static inference. `btlevtcmd_StartWaitEvent` calls `BattleRunWaitEvent` only
  when `word[unit+0x28C]` is nonzero, pairing with the existing stop leaf.
- [Shadow driver leaves](behavior/display/SHADOW_DRIVER_ACCESSOR_LEAVES.md):
  the whole fifteen-body `shadowGet*` family in `shadowdrv.o`, previously absent
  from every prose file. Three SDA words at `0x804150c8/cc/d0` select the
  depth/projected/character shadow records; two static texture objects live at
  `0x8041e738`/`0x8041e758`; the three `ProjTexMtx` getters share relative
  `+124`; the two `Config` getters index a 72-byte-stride table at `0x80304a60`
  from `word[record+8]` with no bounds check; the three `TexObj` getters gate on
  bit 0 of `word[record+0]` and tail-call `smartTexObj(record+220, word[record+
  252])`, returning NULL when the gate is clear. `shadowGetDepthShadowColor` is
  the only value-producing body: it packs `(255-c)*alpha/255` for the three
  component bytes at `record+256..259` using the `0x80808081` signed-division
  idiom into one word with a zero high byte.

`BattleAfterReactionMain` was checked against the existing
[waits and conclusion](behavior/battle/BATTLE_WAITS_AND_CONCLUSION.md)
contract instead of being rewritten: the queue scan, `BattleConsumeReserveItem`,
`BattleCheckConcluded`, `BtlUnit_GetData`, `evtEntry` and the recursive restart
on a missing unit all match that prose, so the queue contract now links to it.
`btlevtcmd_AcSetParamAll` was likewise left alone: it stores eight resolved
operands into `word[B+0x1CC8+4*i]`, skipping the `0xf1194d80` sentinel, which the
action-command callback contract already documents.

Validation: `make test` passes and `make all` passes; logs
`/tmp/pm-gap3-test.log`, `/tmp/pm-gap2-build.log` (docs-only pass, no native
change). `python3 tools/check_doc_links.py` reports one pre-existing broken link
owned by another agent (`partner/FIELD_PARTNER_FLY_MAIN_DISPATCH.md`); every link
added by this pass resolves.

Next: remaining unaddressed rows are larger bodies — `BattleCheckPikkyoro`
(112 lines), `battleMakePhaseEvtTable`/`battleSortPhaseMoveTable`,
`partyDoWork`, `_getRegistStatus` and `marioLandOn`/`marioSearchHead`.
EVT-01/JUMP-01/DMG-02 and the full-game requirements remain incomplete.

## 2026-10-12 — Source audit: documentation requests for packed Jump and recovery

Observed the implementations under `source/` and filed three native
documentation requests in docs/DOCUMENTATION_REQUESTS.md (appended at the end;
no docs-agent lock reserved). Reviewed the recent native commits that shipped
without leaving requests (b79bbe2 packed Mario Jump services, 81c143d
water/void/pipe recovery poses, 778add7 idle/low-HP pose scheduling,
e1257d8/357a656 field shadows).

- JUMP-01: the packed Mario Jump position binding (commit79bbe2) requests the
  0x802ee470/474/478 sound-key block, the original jump psndSFXOn_3D
  invocation record (timing/vector/return/landing cue), and the PS-family
  sin/cos identity plus FPSCR entry state. Local bodies were already recovered
  (BATTLE_MARIO_JUMP_MOTION/NUMERICS, BATTLE_UNIT_SOUND_MODE,
  BATTLE_DAMAGE_ORCHESTRATION for the mode1 AC-possibility fallback).
- field/AC-03: `source/field/water_recovery.cpp` water/void return-fall parameters
  (mot_damage motion31 / mot_bottomless motion33 stages) are authored and
  source-derived per include/field/water_recovery.hpp; pose timings are recovered,
  the numeric stage contract is not. Requested measured original sink/rise/arc/
  travel/return counters, HP-penalty tick, and bottomless placement/fall.
- field/AC-03: recovery pose-table domains 4..40 and pipe paper (p_dokan_x)
  lifecycle requested per the OVERWORLD_MARIO_RECOVERY_POSES open tail.
- P0c main-loop/field-dispatch: the native fixed-60Hz accumulator and
  Game::tick/move/startBattle substitutes have no original marioMain
  (8005a7e0) request; asked for the motion-ID→subsystem dispatch chain,
  battle-transition construction, input gate and marioStMain retrace/pause
  cadence (MARIO_MAIN.md sketch gaps), leaving existing pause/RNG asks open.
- P1b save/persistence: source/runtime/save.cpp uses a native PM2D format and one
  opaque originalStory int; asked for the complete original cardMain save
  record/state machine and the swSet/swGet story-switch bank, since CARD_MAIN.md
  and SW_SET.md are early sketches with explicit remaining gaps and no inbox
  entry.

Address-coverage check: of every original 0x8xxxxxxx address referenced by
source/, only 0x802ee470 (sound-key block, requested) lacked a contract; the
memory-bootstrap table, game-pointer clock block and sampling constants are
already documented.

Follow-up: the documentation agent answered the JUMP-01 request in commit
044ba5c with behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md. It corrects two
premises — 0x802EE470/474/478 is `vec3_802ee470` zero scratch (not a key row),
and 801093a8 has no Gekko PS math (trig via system_angleABf/distABf). Recorded
the resulting native action items (drop the spurious 0x802ee470 reads, bind the
same-TU angle/dist helpers, model both psndSFXOn_3D calls) as a ledger
breadcrumb; no native code changed.

Validation: docs-only change; no native/test/CMake edits and no make rerun
(latest recorded host state remains 87 CTest/90 Python and make all passing).

Next: documentation agent answers the five requests; native closures for
packed Jump sound cadence, water/void physics replacement, pose-table driven
recovery presentation, the original field main-loop/pause cadence and original
save persistence; full-game story/encounter closure stays open.

## 2026-10-12 — Getter plus status dispatch; case-table rescue

Closed BattleGetUnitPtr (NULL-domain table, −1-first, asymmetric setter,
487-site US row; snapshot definition with JP/US caveat) and BtlUnit_GetStatus
(28-slot ISO-read jump table, paired sign gates, nullable outputs).
Verification caught a badly misaligned case table in my own draft (wrong
lanes for statuses 0/2/3/16–24/27, inverted pair directions); rewrote it
from a machine dump of every block and re-verified all 20 lane blocks plus
6 paired blocks. Also resolved a phantom read-tool/listing discrepancy by
re-reading bytes from disk (disk won).

Validation: full re-verification, link audit, diff --check clean. Docs-only,
no make rerun. Next: zero-byte placeholder re-extraction; triangle/math
interiors. Full-game incomplete.

## 2026-10-12 — OW-WALL-01 traversal half: hit-vector skeleton

Delivered BATTLE_HIT_VEC_TRAVERSAL_ORDER for the overworld wall inbox:
4-region/call-census (49 bl all resolved: PSVEC family, checkTriVec_xz,
mapGetWork)/return skeleton with ISO-read SDA values, NaN-safe gate
patterns, and a verified region-1 counter/pointer asymmetry (r30+1/r31+84
vs r31+1/r30+84 elsewhere) reported without inference. Triangle interiors,
chunk ownership, and callback identity stay open per the request's own
remainder. Fixed two of my own early assertions (stride count, uniform-loop
claim) during verification.

Validation: machine-extracted census/edges/strides/counts, link audit,
diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — New denomination: flagged-callee contracts (CheckStatus, filter, search)

Survey found the register's flagged open callees now have reference pairs but
no dedicated contracts. Closed three: BATTLE_UNIT_CHECK_STATUS (null gate,
GetStatus byte, 6-compare, +759 test, exact {0,1}), BATTLE_HIT_CHECK_FILTER
(8-int/6-float marshal, hitVec call, six outputs, residue return),
MESSAGE_MSG_SEARCH (two-table binary search with gp fallback, no miss-null).
Caught two wrong relative links pre-commit (battle/→overworld, message/→battle).

Validation: full instruction re-checks (incl. corrected stfs count and
output-opcode proof), link audit, diff --check clean. Docs-only, no make
rerun. Next: BtlUnit_GetStatus, BattleGetUnitPtr dedicated contracts;
zero-byte placeholder re-extraction. Full-game incomplete.

## 2026-10-12 — Register complete: landing query 2, hitVec alias (43/43)

Closed checkLand2 as marioChkLandon2 (five-probe loop, variant dispatch,
lane quantization, angle tail; doc overworld/OVERWORLD_LANDING_QUERY_2.md)
and resolved hitVec as alias of partially-contracted hitCheckVecFilter
(SLOPES doc covers orientation/normalization; triangle phases stay open).
Final tally: 38 behavior contracts + 5 alias resolutions = all 43 register
rows addressed. Residual open work outside the register: angleABf/
system_angleABf bodies, hitVec triangle phases, and the usual lifetimes/
cadences. Fixed two of my own errors pre-commit (wrong bl census, wrong
callee-share claim).

Validation: instruction re-checks, ISO SDA reads, link audit incl. a
repaired register link; diff --check clean. Docs-only, no make rerun.
Full-game goal stays incomplete.

## 2026-10-12 — Porting-critical large function documentation

Added detailed documentation for high-priority functions needed for porting,
as identified in `docs/behavior/meta/US_COMPLETION_AND_PRIORITIES.md`. All
new files follow the per-subsystem folder requirement from AGENTS.md.

### Overworld category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioMain` | `0x8005a7e0` | 2528 B | `overworld/MARIO_MAIN.md` |

Primary field player update: motion dispatch, gravity, subsystem ordering,
battle transitions, and save-slot management.

### Battle category

| Function | Address | Size | File |
|----------|---------|------|------|
| `battleMain` | `0x800f904c` | 144 B | `battle/BATTLE_MAIN.md` |
| `BattleCalculateDamage` | `0x8005f958` | 1692 B | `battle/BATTLE_CALCULATE_DAMAGE.md` |
| `BattleSetStatusDamage` | `0x800fcf04` | 4220 B | `battle/BATTLE_SET_STATUS_DAMAGE.md` |
| `BattleDamageDirect` | `0x800feaa8` | 2852 B | `battle/BATTLE_DAMAGE_DIRECT.md` |
| `BattleCheckDamage` | `0x800fb9cc` | 2560 B | `battle/BATTLE_CHECK_DAMAGE.md` |
| `BattleCheckCounter` | `0x800fabcc` | 1444 B | `battle/BATTLE_CHECK_COUNTER.md` |

Battle system entry, damage calculation pipeline, status damage application,
direct damage, damage validation, and counter checking.

### Event category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evtmgrCmd` | `0x800316b8` | 5048 B | `event/EVTMGR_CMD.md` |
| `btlseqTurn` | `0x80042b94` | 7896 B | `event/BTLSEQ_TURN.md` |
| `btlseqAct` | `0x8011bb38` | 2220 B | `event/BTLSEQ_ACT.md` |
| `btlseqMove` | `0x8011c264` | 1680 B | `event/BTLSEQ_MOVE.md` |
| `evt_npc_wait_battle_end` | `0x8004efbc` | 184 B | `event/EVT_NPC_WAIT_BATTLE_END.md` |
| `evt_npc_set_battle_info` | `0x8004f0a4` | 100 B | `event/EVT_NPC_SET_BATTLE_INFO.md` |
| `evt_npc_battle_start` | `0x8004efbc` | 184 B | `event/EVT_NPC_BATTLE_START.md` |

Event VM command interpreter, battle turn/action/move sequence controllers.

### Message category

| Function | Address | Size | File |
|----------|---------|------|------|
| `msgAnalize` | `0x80081de0` | 5796 B | `message/MSG_ANALIZE.md` |
| `msgDisp` | `0x80050b88` | 2900 B | `message/MSGDISP.md` |
| `msgWindow_Main` | `0x8011bb38` | 2052 B | `message/MSGWINDOW_MAIN.md` |
| `msgMain` | `0x8008460c` | 1832 B | `message/MSG_MAIN.md` |
| `msgWindow_Disp` | `0x800808ec` | 1756 B | `message/MSGWINDOW_DISP.md` |
| `_evt_msg_print` | `0x800d2d60` | 1244 B | `message/EVT_MSG_PRINT.md` |

Message text analyzer, dialogue display, window update, message main loop,
window rendering, and event-driven message printing.

### Input category

| Function | Address | Size | File |
|----------|---------|------|------|
| `btlevtcmd_ResultACDefence` | `0x80107694` | 144 B | `input/BTLEVTCMD_RESULT_AC_DEFENCE.md` |

Defense action-command result handler.

### Field category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evt_bero_mapchange` | `0x8007aad0` | 176 B | `field/EVT_BERO_MAPCHANGE.md` |
| `evt_mario_jump_pos` | `0x800e4c54` | 1984 B | `field/EVT_MARIO_JUMP_POS.md` |

Room transition event command and scripted Mario movement.

### Memory category

| Function | Address | Size | File |
|----------|---------|------|------|
| `swSet` | `0x800858fc` | 64 B | `memory/SW_SET.md` |
| `cardMain` | `0x800b2508` | 1692 B | `memory/CARD_MAIN.md` |

Story switch setter and save/load state machine.

### Unit category

| Function | Address | Size | File |
|----------|---------|------|------|
| `pouchGetItem` | `0x800d5520` | 1188 B | `unit/POUCH_GET_ITEM.md` |
| `BtlUnit_Entry` | `0x80129644` | 1716 B | `unit/BTL_UNIT_ENTRY.md` |

Item retrieval from pouch and battle unit initialization.

### Format category

| Function | Address | Size | File |
|----------|---------|------|------|
| `__dec2num` | `0x80266834` | 4224 B | `format/DEC2NUM.md` |
| `__strtold` | `0x8026e68c` | 4108 B | `format/STRTOLD.md` |
| `__sformatter` | `0x8026cdf4` | 2476 B | `format/SFORMATTER.md` |
| `__ieee754_pow` | `0x8027067c` | 2096 B | `format/IEEE754_POW.md` |
| `__pformatter` | `0x8026ab78` | 1908 B | `format/PFORMATTER.md` |
| `float2str` | `0x8026b294` | 1820 B | `format/FLOAT2STR.md` |
| `__minus_dec` | `0x800d2d60` | 1276 B | `format/MINUS_DEC.md` |

Decimal conversion, string-to-float, printf/scanf formatters, power function,
float-to-string, and decimal subtraction.

### Platform category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioStMain` | `0x8006faec` | 176 B | `platform/MARIO_ST_MAIN.md` |

Main field system update orchestrator.

### Validation

All functions inspected with `tools/inspect_dol_function.py` against read-only
US G8ME01 ISO. Documentation records exact address, byte size, disassembly,
called functions, and data offsets. No original executable code linked or
copied.

### Limitations

- Many called helper functions remain undocumented
- Full structure layouts partially documented via field references
- Motion ID semantics and subsystem chains need further recovery
- Battle transition conditions require original event script analysis
- Shared helpers across commands need individual documentation

### Next tasks

- Document remaining medium-size functions (500-1500 bytes) by priority
- Recover full structure layouts for battle work, unit, and event contexts
- Cross-reference shared helper functions across command implementations
- Document remaining event command handlers and their argument processing
- Recover complete motion ID table and associated behavior chains

## 2026-10-13 — Native original Jump damage and owned Goomba reactions

`TrioAttack` now imports installed tik damage/wait wrappers and original kind/
part/data pointers. Fresh ordinary Goomba NormalJump calls the recovered power,
HP/FP calculation and direct application services, writes packed accounting,
and starts the original owned hit root with exact cancellation order. Original
generic damage/subset children execute their source waits and pose requests.
A missed first timing deals1 damage (2→1), completes Mario recovery/home movement,
and returns Goomba to automatic type28/KUR_S_1. Successful timing starts the
source second hit; two successful timings preserve20000/20100 and HP2→1→0.
First-success/second-miss also reaches0 with source full code10717. Source
status27 is written by the death child after the reaction and WAIT_FRAME30,
not by the HP setter. Lethal presentation currently suspends at AddRotate801115f8.

StopAC now persists the original1f54 finish countdown and clears all four
callback registrations only when Main returns0. Original stylish window/result
helpers, generic recovery jump integration and used reaction/event-wait leaves
are bound. Missing source callbacks preserve the first pending address. A completed
action retains its battle resources and locked field control pending original
turn scheduling; the development overlay now describes that boundary and stops
advertising a cancel action that cannot run. Source battle result remains0 and
story remains11. No win, reward or field return is inferred from HP0.

Validation: `make test` passed116/116 CTest checks and100/100 Python tests
(`/tmp/trio-hit-tests-final.log`). Focused original Jump checks cover first miss,
first-success/second-miss, both successes, exact per-hit flags/accounting, owned
reaction, source death status, alive idle and AC cleanup. `make all` passed
(`/tmp/trio-hit-all-final.log`), and `make debug-short` passed
(`/tmp/trio-hit-native-final.log`); no compiler warnings in those final logs.
`git diff --check` passed. Read-only asset conversion produced a1,351,884-byte
local event/data pack and48-byte PMTHIT1 mapping; no original files were staged.

Isolated Azahar run: `tools/emulator_smoke.py --binary
ttyd-2ds-debug-short.3dsx --save-fixture
artifacts/fixtures/underground-trio-story11.bin --seconds48 --input-script
tests/azahar_trio_jump_reaction.py --output-dir
artifacts/azahar-trio-jump-hit-reaction-final` passed launch/capture. The extended
30-second post-input capture shows original Goomba damage pose, Mario at home
and the pending reaction service. The first shorter run captured target selection
and the setup interval, so it alone does not verify damage. Native HP/control
words were verified by host integration tests, not emulator memory inspection.
The setup delay needs profiling; emulator FPS is not a hardware result. The
profile/saves remain isolated.

Limitations/next: bind original AddRotate/rotation publication, death sound/effect
requests, rewards and deferred removal; then install Mario idle and original turn
scheduler. Source Spike counter/Frankly warning and Paragoomba wing-fall remain
pending at CheckDamage8010a78c. Feedback/audience/effect/audio providers retain
requests but full presentation remains unbound. Difficulty/pouch level1, empty
equipment and seed0 remain scoped fresh-prologue adapters; original seed/math/
clock parity and other statuses need binding. Continue the original battle-end
and field return ownership after turns; the prologue demo goal remains active.

## 2026-10-13 — Original Goomba death, rewards and deferred removal

The native NormalJump now runs the original Goomba death core through all48
Y-spin requests,30 X-tip requests, reward bookkeeping, packed status clearing
and KillUnit. Rotations preserve integer inputs, raw SetRotate sentinel handling
and original angle normalization. The recovered fresh c_kuribo draw path now
uses Rx*Rz*Ry, zero pivot, the camera-dependent inclusive mirror gate, depth−5
and direct incomingA0/180/model scalar2. Original pose initialization confirms
flag8/100clear and unlinked−1 for this battle branch. Other paper/linked/status
rendering domains and hardware numeric/material equality remain separate.
Per-actor matrix/trig setup is shared across vertices; zero angles skip it.

PMTHIT2 adds the original relocated opponent group pointer. The native source
chain is Battle2738→FieldInfo, FieldInfo+c→NpcInfo, NpcInfo18→group26460.
This corrects the earlier stage-pointer interpretation. GetExp uses group count4
(factor1.3), original kind+e→unit+d level7 and the explicit fresh pouch level1,
yielding3 exp. GetCoin keeps its one irand100 draw and original chance60.
Stores retain FieldInfo14/Battlef00 owners, pending later pouch export.

KillUnit marks the unit during eventpassN; the next native battle call scans64
slots before action/event services, deletes wait/phase/hit IDs, clears matching
Battle120 IDs and unregisters last. The unit remains visible/registered on its
marking call. HP0 alone does not hide it. Native units/parts are preallocated,
with shared immutable animation packs; retired bytes remain diagnostic storage.
Packed status clearing preserves each change-info record's last word. Mario
now installs original wait_event80417c38 and returns to automatic M_S_1.

Validation: `make test` passed117/117 CTest checks and100/100 Python tests
(`/tmp/trio-death-tests-final.log`). Focused Jump tests verify both hit outcomes,
HP0 preceding status27,48/30 rotation counts, numeric axis/pivot/mirror boundaries,
reward accounting, retained registration during KillUnit, retirement exactly on
the next call, original Mario/Goomba idle, and unchanged pending variant targets.
`make all` passed (`/tmp/trio-death-all-final.log`); `make debug-short` passed
(`/tmp/trio-death-native-final.log`). Final build logs contain no warnings.
`git diff --check` passed.

Isolated Azahar: `tools/emulator_smoke.py --binary
ttyd-2ds-debug-short.3dsx --save-fixture
artifacts/fixtures/underground-trio-story11.bin --seconds48 --input-script
tests/azahar_trio_jump_reaction.py --output-dir
artifacts/azahar-trio-death-removal` completed. The early capture shows original
Goomba damage pose; the extended final capture shows Mario idle, Goomba absent
and the other two foes present. It reaches the turn-scheduling development
boundary rather than the previous pending death callback. Exact HP/reward/event
words are host-verified, not emulator memory observations. Captures do not prove
Dolphin image parity or every intermediate spin angle; no hardware FPS claim.
Original profiles/saves were untouched.

Remaining/next: source turn scheduler and repeated command ownership, Hammer/
Headbonk, Spike counter/Frankly warning and Paragoomba wing-fall. Death smoke,
star-point effects and original sound requests remain retained without full
presentation providers. Original RNG seed/earlier draw owners, equipment/item
rolls, pouch/field reward export and full resource teardown are not bound.
Battle result stays0 and story11 stays locked pending original conclusion/end/
field return; do not synthesize victory from HP. Profile native event-bank setup
latency separately. The full playable original-fidelity prologue goal remains
active and incomplete.


## 2026-10-13 — Persistent player turns and original Goombella Headbonk

The trio battle retains native battle/unit/AC resources between player actions.
Original packed PhaseFirst ordering, selection, phase-end checks and actor move
consumption are host-testable helpers. Normal post-action states wait for owned
reaction/event IDs and source message/reservation/audience/BreakSlot gates before
spending the move. Move10/8 hands Mario's completed turn to Goombella without
rebuilding the phase, replaying Frankly's lecture or resetting target HP.
Command sampling uses the live registry and retained HP/status.

Goombella installs original kind/part/damage/wait definitions and executes
Headbonk weapon80393ea8/root803941a8. Used CP wrappers, source power lookup,
raw position/rotation sentinels, scale and XY-angle trajectory are bound for the
fresh ordinary ground path. Original c_pkr idle/run/walk/jump/attack/talk assets
are locally extracted and batched with citro3d; per-pose clocks and battle
rotations apply to her action. Original data/script bytes stay ignored/read-only.

Focused tests pass: source stable priority/home ordering, positional tables,
deferred/dead actor selection, signed move counts, bypass flags/state chains,
Mario-to-Goombella live menu handoff, retained Goomba HP1 then Headbonk HP0,
once-only reward/retirement, original idle and both spent moves. `make test`
initially passed121/121 CTest and100/100 Python tests. With concurrent opening/
NPC work added, the latest full CTest run passes122/123; the remaining failure
is `field_actors_original`, tests/field_actors_tests.cpp:133 (scripted inactive
NPC contact assertion). This turn's battle checks pass. An earlier opening
pose failure was resolved by regenerating stale local opening assets; the
latest opening tests pass. Exact latest logs: `/tmp/trio-turn-tests-final.log`
and `/tmp/trio-turn-python-final.log`. `make all` and `make debug-short` pass
(`/tmp/trio-turn-all-final.log`, `/tmp/trio-turn-native-final.log`), with no
compiler warnings. ARM's int32_t/std::min deduction issue was corrected.
`git diff --check` passes. Concurrent native asset conversions caused one
truncated-image run; sequential rebuild passed.

Isolated Azahar capture `artifacts/azahar-trio-headbonk` uses the debug-short
binary, underground-story11 fixture,48-second boot and
`tests/azahar_trio_headbonk.py`. It shows original tutorial dialogue, Mario's
Goomba defeat/removal, Goombella Headbonk menu/target selection and her original
jump/rotation pose. This input run then attacks Spiked and correctly reaches
the retained pending damage service. It does not prove completed ground
Headbonk recovery; the host integration test covers that path. Native profiles/
saves are isolated; no hardware FPS or Dolphin-image parity claim.

Limits/next: phase11 onward/enemy AI, Spike counter and Frankly warning,
Paragoomba wing-fall, full first-turn/command init, original audience/BreakSlot
producers, equipment/pouch ownership, RNG and sound/effect/stylish playback.
Documentation inbox work now records the phase2→empty3→enemy4 transition and
variant callback/part-replacement owners. After both player moves the native
runtime stays locked at Move11 with result0/story11; victory is not inferred.
Fresh partnerHP10/level0 and empty equipment remain explicit adapters. The
original-fidelity prologue demo goal is active and incomplete.


## 2026-10-13 — Original player-to-enemy phases and Spiked approach

The native trio controller advances through source phase2→empty3→enemy4.
Move0 rebuilds the actor/event tables; selector1 hooks fan out and selector0
hooks run serially, preserving pair holes and keys. Frankly's original phase
script runs in3/4 and returns at its phase1 gate without replaying dialogue.
Move10 observes conclusion and actual remaining counts before phase end;
Move11 uses actual audience mask8/owned-event readiness; Move12 drains empty
message work before advancing. Original turn counts are not reset/incremented
at the player-to-enemy boundary. Native phase2 audience inventory remains an
explicit empty presentation adapter; mask8-set reaction and phase4-end audience
providers remain pending.

Enemy Move8 uses source selection/bookkeeping and Act0→3→6 launch ordering.
The installed Spiked attack root retains owner/action ID, then deletes its wait.
EnemyItemUseCheck binds the source held-item0 early return. Original signed side,
weapon masks, packed sampling and deterministic candidate weighting choose its
target. No fixed Mario target is substituted. Its original approach, jump and
rotation run through the verified relocated tik krb_get_dir93105b2c ABI and
original angle table. The native action reaches ResultACDefence80107694;
guard/impact services remain the next unbound boundary. Player menus now close
when the enemy root owns the action.

Typed pose lookup follows the original first requested-or-band-fallback scan,
including force1 only for29..39; Spiked43 correctly becomes TGK_S_1. Unit live
scale precedes outer rotation and display offset translates in world axes
afterward, following the documented source matrix order. Original scale/display setters are
bound; source requests/effect/camera playback and later impact remain incomplete.
PMTHIT3 (68bytes) retains original kind/damage/wait/attack, group and Frankly
phase pointers. Extracted assets and immutable capture binaries remain ignored.

Validation: `make test` passes126/126 CTest and101/101 Python tests
(`/tmp/trio-enemy-tests-final.log`). Integration verifies actual phases2/3/4,
two gated Frankly roots/no dialogue replay, phase-ID cleanup, original Spiked
root/Act3/owner, packed sampler actor4, player-menu exclusion, original fallback
pose and preserved field lock/story11/result0 at the guard boundary. Earlier
Jump/Headbonk HP/reward/deferred-removal checks still pass. `make all` passes
(`/tmp/trio-enemy-all-final.log`); `make debug-short` passes
(`/tmp/trio-enemy-native-final.log`); final logs contain no compiler warnings.
`git diff --check` passes. The previous session's NPC test failure is now fixed
by its own workstream; the complete current host suite is green.

Isolated Azahar final capture: `artifacts/azahar-trio-enemy-phase-final`,
immutable debug-short snapshot, underground-story11 fixture,48second boot,
`tests/azahar_trio_enemy_phase.py`. It shows Mario's missed Jump leaving Goomba,
Goombella menu/ground target, subsequent Goomba removal and Spiked approaching
Mario in its original airborne/rotated pose at the pending impact boundary.
It does not prove enemy damage or completed rounds. The capture precedes the
last menu-title cleanup; that change is host state/build verified. Original
profiles/saves are untouched, and no hardware FPS/Dolphin parity is claimed.

The first native capture stopped during Mario setup. A symbolized isolated
GDB run caught abort from renderer animation-string allocation. Actor layout
had changed while one native compilation was in flight; its object timestamp
masked an inconsistent layout. Forcing a fresh trio_attack object build
resolved the stop in the immutable final capture. Do not change shared data
layouts during active compilation; finish edits before launching final builds.
The event-bank setup delay remains unprofiled.

Next: bind private defence WorkUpdate/result/reset ownership, original Mario/
Goombella damage roots/poses, enemy CheckDamage and recovery; then Para dive,
Spike counter/warning, wing-fall, phase4 audience/end and next-turn/conclusion/
field-return owners. Inbox evidence now pins fresh no-input result2, Spike
power2/Para1, code10117 and original player reaction scripts. Held-item,
equipment/status/guard/lethal/pouch/effect/audio branches remain incomplete.
The full original-fidelity playable prologue goal remains active and unachieved.

## 2026-10-13 — Original trio enemy impacts, private defence and recovery

Spiked Goomba and Paragoomba now execute their original impact/recovery scripts
through the native event VM. Enemy CheckDamage admits the fresh ordinary source
weapons and uses original power2/1, damage calculation, direct application,
player-owned reaction roots and source guard codes. Mario/Goombella take source
selected targets, not a fixed Mario target: the no-input host fixture ends this
round at Mario8HP/Goombella9HP. Source default waits restart after damage; both
enemy attacks and tracked reaction barriers drain. Original damage/guard poses
are packaged from the read-only ISO.

Private battle pad WorkUpdate now receives full physical samples, independently
of general action triggers and without requiring an installed main AC callback.
Packed snapshots/history/reset and ResultACDefence use the recovered portable
logic in source sequence→private-pad→AC→event order. A guard fixture yields4,
reduces Spike damage to1 and completes the round. B superguard yields5, leaves
Mario10HP, reduces Spike to1HP, increments A280 once, retains codes8012a/137
and cancels the original enemy attack. WaitEventEnd and the original 40-sample
mode5 spin/float-output interpolation are bound. Superguard subsequently stops
at original TransStageFloorPosition8010510c; its exact three-operand ABI and
embedded-stage90/-50 producer are now documented for the next step.

Paragoomba DivePosition supports the used mode0/curves0,1,4, constant count,
source elapsed/intermediate/visible positions, signed amplitude and source
endpoint/sound ordering. Portable interpolation preserves low-word signed
integer squares for intpl_sub. Native 3D sqrt and sine are math adapters, not
bit-identical Gekko estimates/library results. Movement sound keys/counters are
retained and requests collected; playback handles/position/off remain adapters.

Phase4 end retains its mandatory RNG draw/reset/mask8 initialization. The
existing explicitly empty native audience follows source polls0→13→15, clears
mask8 and decrements/clamps BreakSlot turn before advancing phases5/6. Turn
bank4c→4d and phase reset preserve turn count1. Native player level now reads
low byte of the existing fresh pouch level1. Original next-turn audience owner
8019f298 remains pending; no new turn, victory, dialogue or field return is
fabricated. Nonempty audience/setup4 crowd/SP/bingo/throw mechanics remain open.

Validation: `make test` passes130/130 CTest and103/103 Python tests
(`/tmp/trio-impact-tests-final.log`). Integration covers earlier Jump/Headbonk,
phase order2/3/4/5/6/0, source audience polling, target HP, reset/guard/counter
codes, attack cancellation and unchanged story11/result0/field lock. `make all`
and `make debug-short` pass (`/tmp/trio-impact-all-final.log`,
`/tmp/trio-impact-native-final.log`); final native logs have no compiler warnings.
`git diff --check` passes. Numeric bit fidelity, complete successful player
follow-ups, lethal player damage, statuses/equipment/items, full presentation,
next-turn/conclusion/field-return owners and hardware performance remain open.
The full original-fidelity playable prologue goal remains active and unachieved.

Isolated Azahar capture: `artifacts/azahar-trio-enemy-impacts`, immutable
`artifacts/trio-impact-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and `tests/azahar_trio_enemy_impacts.py`. Manual review
shows Goombella's menu/Headbonk, ordinary Goomba removal, Spiked airborne
recovery with Mario's original damage pose (sample04), then both remaining
enemies and player waits restored (sample15/final). One-second captures do not
prove every Para/Goombella reaction frame or guard timing; host assertions cover
HP and ownership. The sequence reaches the explicit next-turn boundary and
keeps story11/field lock. Profiles/saves outside the isolated folder are untouched;
no hardware FPS or Dolphin parity is claimed.

Next: provision the source embedded-stage threshold90/lowfloor−50, bind the
three-operand TransStageFloorPosition and finish superguard recovery; bind
original next-turn audience/turn-first owners with their RNG/order before
opening another player turn. Then successful player follow-ups, Spike warning,
wing-fall, conclusion and original field/story return. Do not bypass these
owners with fabricated completion or reset turn bookkeeping at phase boundaries.

## 2026-10-13 — Superguard recovery and original fresh next-round control

TransStageFloorPosition now captures the three source operands, resolves X/Z,
uses original embedded-stage threshold90/lowfloor−50 and writes only raw Y.
Original B-superguard counter bounces/recovery and owned-event barriers drain.
No unconditional zero floor or fabricated completion is used.

TurnEnd retains original four-unit group metadata (including defeated Goomba),
sum23/player-level1/threshold46, chance increment, mandatory RNG, unique category
queue and target-count arithmetic. The empty native crowd follows actual
zero-amount/capacity gates; known chance categories filter through the no-SysCtrl
record path without invented RNG. Nonempty crowd allocation/movement,
threshold target-size changes and other category graphs remain explicit boundaries.

Fresh initialization now calls the original portable TurnFirst processor from
count0 to1; next round calls it to2. Rule/fortune/fog/record/move/priority and
unit U24 ordering reuse native shared-memory services. Frankly's original kind
is resolved from the fourth source group unitset, including level/priority/data
instead of the old synthetic kind row. The integration confirms U24=2 despite
his count0 and the source turn record counter2.

Original Turn2→3→5 and ordered zero-status/zero-Happy-badge scan branches,
word/halfword countdown tails and Turn4a/4b return run in the original logical
order. Admitted nonfresh periodic damage/recovery/Happy-effect paths remain
pending at their actual provider; they are not skipped. Original get_turn feeds
Frankly's phase1/turn gate. Phase0→1→2 then uses existing original phase hooks,
event barriers and actor selection to open Mario's next command menu. The
first-turn lecture does not replay; HP remains Mario8/Goombella9 in the no-input
fixture, Mario9 after A guard, and Mario10 after B superguard (SpikedHP1).
Field HP/story11/result0/control lock remain unchanged until original field
return is implemented. The demo is still incomplete.

Validation: `make test` passes133/133 CTest and103/103 Python tests
(`/tmp/trio-next-turn-tests-final.log`). Focused integration verifies full phases
2/3/4/5/6/0/1/2, count2, source Frankly pointer/U24, record counter2, chance3,
queue0, actual Mario command state, retained HP, guard/counter codes, attack
cancellation and recovered hit-event cleanup. `make all` and `make debug-short`
pass (`/tmp/trio-next-turn-all-final.log`, `/tmp/trio-next-turn-native-final.log`),
without compiler warnings. `git diff --check` passes. Full nonfresh status,
audience/crowd, successful player follow-ups, Spike warning, Hammer, wing-fall,
conclusion/field return, original numeric/cadence parity and hardware performance
remain unfinished. The original-fidelity playable prologue goal remains active.

Isolated Azahar capture: `artifacts/azahar-trio-next-turn`, immutable
`artifacts/trio-next-turn-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and updated `tests/azahar_trio_enemy_impacts.py`. Manual
review shows original Mario damage pose/Spiked recovery (sample04), then
`source-next-player-command.png` shows restored idle actors, surviving Spiked/
Paragoomba and Mario's Normal Jump/Normal Hammer menu. Host assertions establish
turn2, HP and event ownership; screenshots do not prove native guard windows or
every reaction frame. Other emulator profiles/saves are untouched. No hardware
FPS or original-runtime parity is claimed.

Next: bind original ordinary Hammer wrapper80416c40/core8035d6d4 and W80353de8,
its held-left command3 packed/general-pad ownership, source Hammer power and
original M_H poses. Preserve actual approach/waits/result branches instead of
inventing an impact schedule. Then conditional Spike warning/counter, successful
Jump/Headbonk follow-ups, Para wing-fall, conclusion and original field/story
return. Documentation work has recovered the Hammer dependency map from the
request inbox/read-only original; full prologue completion remains unproven.

## 2026-10-13 — Original ordinary Hammer event and held-left native command

Original weapon80353de8 launches wrapper80416c40/core8035d6d4 through the
persistent VM. Its approach/ftomsec windup, M_H_1A/B/C poses, StartAC/ResultAC
poll, swing WAIT7 and source damage/result/recovery branches run unchanged.
No fixed impact tick is introduced. AcSetParamAll preserves resolved sentinels,
mode remains raw, and the level helper reads signed pouch99. Fresh pouch98/99
are explicitly initialized1; imported field upgrades remain unbound.

ID3 now round-trips every documented packed Hammer field, including source
registration, shared signed forced counter, stage/countdown/scratch/handles,
actor automatic/options/difficulty and suppression. General pad atBf2c receives
full native samples independently of private guard B1d1c. Held-left comes from
B1034, not an A/B edge. Source manager return0 clears main only; original1007
clears all four pointers. Stop writes1005 and the original60-step cleanup runs.
Raw-output0 evt_snd_sfxon retains its original sound key without writing a fake
voice result. Charge cue handle0 is a presentation adapter; actual cue/macro
playback and original gauge drawing remain unfinished. Circle Pad prompts are
presentation adaptations, not story dialogue.

Hammer power now reads the selected source weapon's words, actual pouch98/99
and U2ff/U300 bonuses through recovered callbacks/calculators. Fresh Hammer
hits ground Goomba(kind1) and Spiked(kind3); W78 resistance2 prevents the early
spike counter in this admitted scope. Para's original flying kind is2. No-input
and early release cause H1/code10117, leaving Spiked1HP. Stage5 held-left release
produces result2/class3/H2/code30717, original Spiked death/reward/registry and
visual removal, then Goombella's command menu. Earlier guard/counter/turn
bookkeeping remains green. HP0 does not bypass original death or manufacture
victory/field return.

Original M_H_1A/B/C are imported from read-only ISO a/b_mario/a/a_mario-, using
the existing atlas/mesh pipeline. Data-only script banks already contained the
source Hammer. Extracted assets/packs and binaries stay outside source history.
No new dialogue is authored, and no generated Hammer artwork or tones are used.

Validation: `make test` passes134/134 CTest and103/103 Python tests
(`/tmp/trio-hammer-tests-final.log`). Focused integration covers source target
occlusion (Spiked only after Goomba removal; airborne Para excluded), timeout,
early/full-stage release via full physical snapshots, exact HP/result/class/code,
original death/removal and Goombella handoff, four-pointer cleanup, story11/
result0/field lock. `make all` and `make debug-short` pass
(`/tmp/trio-hammer-all-final.log`, `/tmp/trio-hammer-native-final.log`) without
compiler warnings. `git diff --check` passes.

Isolated Azahar: `artifacts/azahar-trio-hammer`, immutable
`artifacts/trio-hammer-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and `tests/azahar_trio_hammer.py`. Reviewed screenshots
show Hammer menu/Spiked target, original hammer geometry/windup/swing,
Circle Pad prompt and restored Goombella Headbonk menu after no-input recovery.
Native early/full charge-window success is host-verified, not captured/measured
on hardware. Other emulator profiles/saves are untouched; no hardware FPS or
Dolphin parity claim is made.

Next: bind original Para wing-fall/damage graph and ground replacement parts/
poses/scripts. ChangeKind writes U8 while retaining original U10; source model
bank remains c_kuribo, so do not invent a replacement resource. Then Jump/
Headbonk successful follow-ups, Spike warning/counter, full conclusion and
original field/story return. Gauge/voice, optional stylish/teaching/mask2,
nonfresh status/crowd/equipment and original timing/numeric parity remain open.
The full original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Native original wing fall, grounded attack and final removal

A fresh failed Headbonk on flying Para(kind2) now adds source reaction400000
only through weapon4000/part800/per-hit100 admission, yielding410117/HP1.
Original installed damage/root/child owns the complete fall: attribute clears,
PTK_D_1/wait1000, raw pose/data tables, actual part replacement, KUR_D_1,
JumpPosition10 toY0/home update, wait500, ChangeKind1, original grounded init,
wait1 and final pose. Integer SetHomePos preserves raw XYZ truncation; it is not
a float/sentinel setter. No independent fall timer, teleport or HP reset is used.

ReplaceParts preserves the specified old fields and imports the original ground
record. Native resource recreation reuses the preloaded one-part c_kuribo bank;
full multiparts/resource lifetime remains an adapter. Current kind1 changes
without replacing original kind-definition93521b48, identity, level or HP1.
Original ground wait/attack/damage/confusion/sound pointers install through
source event setters. The grounded attack executes its real root, shared
krb_get_dir93105a14 angle-table ABI and default power1; native generic-jump
numeric services remain adapters. The next player menu follows source phase/
barriers and retains HP. Live part attributes/current kind refresh command
sampling and original US name: grounded unit5 is Goomba and admits Hammer.

Original c_kuribo now packages KUR_R_2 and keeps original descriptor0 KUR_Z_2
first. Authored PTK_R_2 is absent from the original bank; original battle uses
shared missing-name fallback to descriptor0. No replacement animation is
invented. Native source assertions preserve exact tables/part/event pointers,
original definition and floor/homeY0. All asset extraction remains read-only;
originals, compiled packs and snapshots remain ignored.

The host scenario now defeats and removes all three opponents through source
hits, death/reward scripts and deferred registry retirement. After wing fall/
grounded attack, Mario's missed Hammer kills the remaining HP1 Goomba with
10717. Original conclusion sets bank7=07000000. Runtime now exposes the exact
next ordinary owner btlseqEnd80215234 instead of silently remaining active.
It does not fabricate result1, export rewards/stats or release field controls;
story11/result0/control lock remain until original ending/return is implemented.

Validation: `make test` passes134/134 CTest and103/103 Python tests
(`/tmp/trio-wing-tests-final.log`). Integration covers original Hammer and prior
guard/turn behavior, full wing transition, HP/definition retention, exact part/
pose/data/ground event pointers, grounded attack's sampled one-damage target,
correct name/Hammer admission, final original death/removal and ending boundary.
`make all` and `make debug-short` pass (`/tmp/trio-wing-all-final.log`,
`/tmp/trio-wing-native-final.log`) without compiler warnings. `git diff --check`
passes. Capture helper Python syntax check passes after suppressing xdotool
verbosity; that edit changes only logging, not the captured input sequence.

Isolated Azahar: `artifacts/azahar-trio-wing-fall`, immutable
`artifacts/trio-wing-snapshot` debug-short binary/ELF, underground-story11 fixture,
48second boot and `tests/azahar_trio_wing_fall.py`. Reviewed captures show the
flying target/Headbonk, wings present before replacement, grounded KUR_D_1
(sample10), restored ground idle/menu and original Goomba name alongside Spiky
Goomba after the enemy round. Screenshots do not prove every intermediate frame,
exact HP, charge success or final enemy kill; those are host assertions. Other
emulator profiles/saves are untouched. No hardware FPS or Dolphin parity claim.

Next: bind original btlseqEnd victory/result/audience/reward/pouch/export owners
and original field-result1 gorotuki continuation. Source first-ending audience
services do not become no-ops merely because the current crowd adapter is empty.
Retain the original FieldInfo/NpcInfo callback producer; do not force itnull or
skip ending presentation to fabricate field return. Successful Jump/Headbonk
follow-ups, Spike warning/counter, gauge/voice, optional stylish/teaching,
nonfresh status/crowd/equipment and full story/hardware parity remain open.
The original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Native initial original trio victory stages

After original enemy death/deferred removal and conclusion, the native trio
now executes btlseqEnd's initial result selection through the existing portable
source-priority helper. Packed battle information I10 becomes1; the field
sequence/result consumer remains unchanged until real teardown/publication.
Story11 and field control lock remain. ResetMoveColorLvAll scans all64 slots
and writes only U311=ff to surviving records, including Frankly.

The source original group, retained after enemy deletion, owns Great/Fast
victory audience checks. Both real AddTarget requests execute the exact zero
Sub early return and outer bias/target clamps with original packed constants.
Nonzero Sub remains pending801a18cc. Original Koura deletion stream803b0c50
runs as an actual owner-Mario priority10 root; the fresh no-guard leaf resolves
its actor/output and returns-1. End work uses existing SDK heap0 allocation
adapter, initializes only the original used fields (including separate E30=0),
and retains the required irand120+60 request. Display/clap requests are traced;
full source crowd/audio/end display remains an adapter.

Original state05 uses ordered SetStatus(U,id,0,0), party then Mario for IDs0..24,
rather than ClearStatus: it preserves Charge16 and IDs25..27 and retains the
portable move-count/opposite-status semantics. Original owned-event barrier and
all-unit data3f lookup follow; the existing original zero-terminator query is
reused. The precise next pending owner is DropMaterial801141bc at bank7 state
07000006. Full drop RNG/equipment/pouch ownership, rewards/EXP/victory art,
fade/export/sequence publication and preserved field dialogue return remain
unfinished. Nonfresh callback, alternate party flag/body and Mario10 paths
remain explicit pending providers; no false field completion is produced.

Documentation agent recovered and persisted the full original ending state
map, local audience/status/Koura ABI and bounded fresh FieldInfo callback
constructor evidence, with remaining work in DMG-02/EVT-01/JUMP-01 inboxes.
No original assets or binaries were added to source history.

Validation: make test passes137/137 CTest and103/103 Python tests
(/tmp/trio-ending-tests-final.log). Extended source scenario removes all three
enemies, verifies packed result1 versus retained field result0, exact pending
DropMaterial/state06, initialized end fields/RNG range, reset color bytes,
two audience adjustments, real Koura callback and clap request. Prior attacks,
wing fall, enemy rounds and guard checks remain green. make all passes
(/tmp/trio-ending-all-final.log); git diff --check passes. No new emulator
capture was run for this controller-only increment; visible victory and field
return are not yet implemented or claimed. No hardware/Dolphin timing parity.

Next: bind source DropMaterial and correct live pouch/resource ownership,
then EXP/victory poses and presentation, fade/export/field sequence return,
and original gorotuki continuation. The full playable prologue goal remains
active and incomplete.

## 2026-10-13 — Native opening chain, original scene consumers and combined verification

Integrated the normal demo → title → four file slots → fresh-file opening →
source story3 harbor checkpoint, with separate debug-short target. Existing
native-save compatibility is not required by the user; automated testing still
uses isolated profiles/fixtures only. Original37 demo/11 opening payloads,
message IDs, tokens, attribution and input gates remain source-owned.

House rendering now consumes original actor roots/poses/local simulation clocks,
source cameras, door curve11/Q1024 requests and original hinge/postbox geometry.
Fixed queued actors overwriting one another's GPU vertex ranges. Original demo
marketplace rendering consumes Peach/merchant/chest choreography, original opaque
pass1/color/cull surfaces and the dedicated gor_01 map with five hidden interiors
and both source wall subtrees. Identical selected actor samples are cached,
avoiding repeated wrapped-chest tessellation while its pose is static.

The original OFF_d_meku_right A_1 page mesh and rune back now render. Texture
admission/final180 sample acknowledgements gate the four turns; original scale,
third-message timing and curve11 book request are retained. Native orthographic
outer projection/depth is explicitly an adapter. A first smoke exposed paper
behind its2D underlay; corrected depth layering is visible in focused captures.
No substituted mesh/art or rewritten story dialogue was added. Golden-chest
forensics separately proved the colorful rug objects are original S_roten stock;
independent original conversion matches native chest PMANIM4/T3X/crop bytes.

The harbor source event owns admission, messages022..024, charges/HP minimum and
blocking setup2 request. Native tutorial entry runs original init/phase/t1_000
on source-selected bti_01/stg_00_0 and original BGM126. Packed three-unit/part,
work/HP/hook/target ownership and source Crump approach/jump/precheck services are
integrated. ResultACDefence now consumes explicit physical samples; abstract
story A/B edges cannot stand in for guard input. Original fresh pouch, Mario
init/wait hooks and shared counter/hit-event cancellation/priority services are
bound. Actual CheckDamage, legitimate first-turn/command/attack ownership,
turn/end/reaction/reward and genuine field return remain unfinished.
Result0/story4/save blocking are retained; no victory or source story5 is faked.

Validation:
- Combined committed tree4ed0b6c plus408df97/faa0184 in
  /tmp/pm-opening-merged-4ed0b6c: make test135/135 CTest +103/103 Python;
  make all passed (/tmp/opening-merged-mario-{test,build}.log).
- Fresh normal Azahar route completed and saved source story3;
  artifacts/azahar-opening-merged-fresh contains title/files/house/harbor captures.
- Original tutorial intro reached its pending combat boundary with three distinct
  actors; artifacts/azahar-harbor-packed-intro and
  artifacts/azahar-harbor-fresh-mario-intro (fresh pouch/Mario init). This is not
  a fight result; screenshots supplement the host initialization assertions.
- Original unskipped37-message demo completed its final input gate before the
  paper depth fix; focused corrected fold/rune/back/narration captures are in
  artifacts/azahar-original-paper-depth. The corrected full230-second demo now
  reaches its final input guard; B retains the demo and A enters title, captured
  in artifacts/azahar-corrected-full-demo. Screenshots supplement host exact-ID
  and guard assertions; they are not full original-image/timing parity.
- make debug-short passed; isolated25-second boot in
  artifacts/azahar-opening-debug-short-long shows direct development field.
  The earlier5-second capture was still blank during initial loading.
- Read-only source verifiers:15 house object/matrix anchors,51 demo scene/map
  anchors and53 book/capture constants; independent10-sample chest UV/byte check
  passed. Source checks are static evidence, not Dolphin observations.
- Isolated defense integration129/129 CTest +103/103 Python and native build passed;
  its bridge was then rechecked in the combined132-test tree. No user profile,
  original ISO or user saves were modified. Originals/artifact packs are ignored.

Local playtest binary: artifacts/ttyd-opening-faa0184.3dsx (normal opening chain,
verified checkpoint; combat remains pending). New contracts live under story/
and display/, indexed by docs/behavior/README.md. Full-game and original-image/
New2DS hardware parity are not claimed.

Next: bind legitimate source startup/first-turn/command/Jump owner and full
damage/reaction services; retain fresh pouch through the actual field/save path; then real battle end and
original field continuation. Finish
original cover geometry/outer projection, fade/light/tree/environment, sound and
crystal/radiation consumers; profile actual New2DS memory/performance. The goal
remains incomplete while the real harbor fight/return and these scene services
are still pending.

## 2026-10-13 — Original harbor field battle construction

Fresh harbor admission now constructs source NPC battle information and original
FieldInfoInit/battle_init fields. Source setup preserves its seventeen RNG
requests on an explicit seed0 adapter and validates setup2 group/stage pointers.
Fresh no-party/no-field-attack state yields firstAttack0; configuration20000010
is not substituted for a first strike. The packed Fbat/NPC/FieldInfo allocations
are explicit native carriers; field internals outside those source fields remain
unbound. FirstAct/resource/fade/command ownership remains the next startup work.

Validation in /tmp/pm-harbor-integrated-fa27933: make test134/134 CTest and
103/103 Python, make all passed (/tmp/opening-field-info-{test,build}.log).
Original packed tests check actual group/stage/firstAttack values; the read-only
verifier retains source addresses. No victory/field return is published.

## 2026-10-13 — Native original trio drops and supplied prologue pouch

BattleInformationSetDropMaterial801141bc now runs its complete valid source
HP→FP→item algorithm with typed services: signed current-unit ratios, upper
cap only, original threshold/outer/inner draws, story<=19 weights, both actual
badge314 queries, original weighted first-unit/back-selected item routes and
25-word Item Hog fallback (including zero outcomes). It writes only setup
S10/S14/S0c; original death coin I14 and selected/back/returned slots remain
distinct. It does not deposit drops or coins into the pouch. The original
source data addresses/tables come from the existing unchanged local pack.
Original encounter-selected item setup and shared field RNG ownership remain
separate unbound inputs; the used native fresh no-item admission is preserved.

Added an explicit source-prologue initial pouch snapshot from recovered pouch
Init and initial-stage grants6/32hex/9, tier writes1/1, source insertion order
[9,32,6], coins100 and fresh zero-equipment resource revision. The original
localized default partner name is copied from msg/US/global.txt; only the name
stays resident, not a second complete global bank. This implements the pouch
part of the authored initial-stage snapshot, not proof of ordinary original
title/new-game callback reachability or the full original stage initialization.

Native fresh-file handoff now owns this block in Progress via shared storage,
retaining it through Game/room rebuilds. Mario's actual field HP bridges into
entry once. TrioAttack accepts the supplied live block, initializes actual
Mario/Goombella kind HP/FP copies, clears status then imports source resources
and initialized seven-party snapshots. Supplied current HP7/FP2/partyHP6 stay
7/2/6; neither import nor a later turn heals them. Mario maxFP/currentFP no
longer start incorrectly at0. Fresh snapshots restore Goombella scale1 and
preserve kind FP5 through the HP-only partner import.

Legacy40-byte checkpoint files still omit this block; their development entry
uses the explicit initial-stage snapshot plus saved Mario HP. This is not
original inventory/equipment/save reconstruction. Original later item/equipment,
full field/player/harbor shared pouch services and serialization remain open.
The same source-owner block is preserved during live new-file room traversal;
normal loading of a legacy checkpoint is a separate limited adapter.

Ending06 now computes actual drops and advances08. Fresh FogForceStop's exact
no-fog early return is bound. Original signed stockEXP clamp/fortune-byte/
level99/upper-cap/halfword copies prepare E8/Ea, while sourceflagsE0c/Bef8
writes retain original roles. The all-three scenario yields10 stock/remaining
EXP and reaches real _GetExpIcon_Init802149ec; pouchEXP stays0, sourceIresult1
is distinct from fieldresult0, story11/control lock remain. No icon/effect,
reward counting, victory/fade/export or field completion is manufactured.

Validation: make test passes141/141 CTest and103/103 Python tests
(/tmp/trio-drop-tests-final.log). Focused DropMaterial tests cover signed ratios,
upper-only cap, draw order/count, early-story mandatory item roll, weighted
back/selected slots, badge multiplication and fallback. Prologue pouch tests
verify source fields/grant order/resources and retained import. Native trio
integration verifies supplied HP/FP/partnerHP, shared pointer survival through
Game restore, original all-enemy death/removal, actual drop fields/two badge
queries/3-or5-or7 draws, original10EXP preparation and exact icon boundary.
Prior attack/wing-fall/guard/round tests stay green. make all passes
(/tmp/trio-drop-all-final.log) without compiler warnings; git diff --check passes.
No new emulator capture or hardware/Dolphin timing claim for this state/owner
increment. Original assets/packs/binaries stay outside source history.

Documentation agent also recovered the next actual19icon404 frames and
positions/alpha policies, original US PointGet texture strips and source08→0b
fallthrough/counting. Next: import/render those original reward assets, bind
source count/victory children/fade/export/sequence and preserved gorotuki field
continuation, then finish shared field pouch/save/encounter input ownership.
Full original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Source startup controllers, native readiness and original curtain

Integrated original Init/SequenceManager, US six-effect readiness row, ordinary
firstAttack0/shared FirstAct states1..14, mandatory system/Mario UnitSetup and
complete portable UnitEntry storage/control bodies. Actual fresh source order is
system kind220/ID0, Mario kind222/ID1, then Crump/ID2 and spawned helper/ID3.
These controllers retain required service providers. They do not replace the
current bounded intro with an unconditionally successful initialization or enable
an internal attack diagnostic as a legitimate command/turn owner. Packed literal
registry IDs and public three-actor visual slots still need coherent migration.

Actual9 original US startup banks/25 descriptors/23 supported clips and original
GPU atlases now admit source readiness. Two unsupported dormant accordion clips
retain their raw descriptors/bank; admission does not acknowledge their playback
or paper field capture. The original p_jyabara texture owner is p_slit. Wrapped
UV/pass/cull/RGBA remain in explicit PMANIM4 conversion; default ordinary capture
kind rejection is retained. Configured ISO overrides pass through all exporters.

Bound original fade23/context2 OFF_d_maku_left_right A_2, actual14 keys/216
triangles/0..45 span and original atlas. Portable fade polls resource admission,
uses source pre/post-main progress queries, RGB truncation*.98 afterprogress.5,
then original release/flag2 atprogress1. Duration200 is retained rather than used
as a substitute timer. Native completion uses actual evaluated pose and simulation
time. Unique queued ranges retain all actors. Native image-plane mapping remains
an explicit adapter; admission occurs after field GPU rooms are released.

Validation: isolated integrated make test144/144 CTest +103/103 Python and
make all pass (/tmp/opening-startup-unit-final-{test,build}.log); focused curtain
original-pack and callback-order checks pass. Read-only original verifiers pin15
startup/UnitSetup/UnitEntry spans,10 US resource/row anchors and12 curtain provider
hashes/dispatch/constants/keys. Isolated temporary curtain component smoke in
artifacts/azahar-original-curtain-projection caught and corrected a100x image-unit
error. Reviewed sample129 shows original pleated curtain moving off screen and
sample130 restores stage/actors. That temporary trigger is removed before the
normal build. This proves the bounded native consumer, not source FirstAct
choreography, complete fade/camera/material parity or a battle result. User
profiles/saves and the source ISO remain untouched; original packs are ignored.

Next: bind required UnitEntry helper services and literal registry/visual mapping,
then actual Init/FirstAct/Turn/command/damage/reaction/end/reward/field owners.
Retain fresh pouch through actual field/save ownership. Full source harbor fight
and return remain pending; result0/story4/control/save barriers remain intact.
Original cover/fade/environment/effects/audio consumers and hardware parity remain
ongoing. The opening playtest goal remains active and incomplete.

## 2026-10-13 — Original ten-point ending tail source delivery

Documented [ordinary trio ending tail](behavior/battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md):
exact0c..1f timer/invocation order, independent effect softdelete/pointer
release, voice/bodytype42 conditions, source fade10 refresh/strict completion,
Rule0 and ordered persistent-pouch/field-party export. Original field wait and
161-command scene remain the postbattle owners; no fabricated heal/story write.

Validation: read-only US DOL complete bodies/tables/constants; scoped
`git diff --check`. Documentation only; no native edits, builds or commit.
Next bind actual PointGet presentation/counting/tail and original icon creation
clock/window geometry, then full sequence/field restoration; alternate EXP/
level-up/fortune/audio/GPU/field constructor services remain open.

## 2026-10-13 — EXP icon clock and original frame source delivery

[Icon clock/window frame](behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md)
closes per-record source404 frame0/delay6/reset/Main/hidden advancement and
same outer-pass creation/decrement ordering. Original type0 window texture
72x72CMPR and36-vertex nine-slice geometry/UV/color material are documented.

Validation: read-only original bodies/TPL header,180float FIFO symbolic census
and `git diff --check`. Documentation only; no native edits/tests/builds/commit.
Next native providers can replace borrowed clock/frame placeholders; actual
icon draw, camera/GX/palette lifecycle and original raster comparison remain.

## 2026-10-13 — Original US PointGet complete layout delivery

[PointGet source layout](behavior/effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md)
closes original US phrase/digit widths/cap, foreground-derived shadow geometry,
ordered matrices/quads/UV and effect entry/Main/softdelete lifetime for10EXP.
Fresh line half-width322 and actual scaling200/322; original assets preserved.

Validation: complete read-only original bodies/constants and quad/matrix paths;
`git diff --check`. Documentation only; no native edits/tests/builds/commit.
Next bind original PointGet graphics/counting; source camera/display dispatcher
state/pool/texture scheduling and actual numeric/raster comparisons remain open.

## 2026-10-13 — Original EXP icon bottom anchor correction evidence

[Used404 icon draw](behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md#2026-10-13-used404-draw-anchor-follow-up)
confirms sourcecenterX/bottomY quad and actual1011 geometry/flip/scale branch.
BigY25 spans25..61; smallY0 spans0..18 before declaredviewport scaling.
Validation: read-only complete iconDisp/iconGX/constants; git diff --check.
Documentation only; no native edits/tests/build/commit. Native renderer can
replace centerY assumption; sourcecamera/GX/runtime raster comparison remains.

## 2026-10-13 — Native trio EXP icons and victory events

Implemented actual ending08→09→0a with original19-entry EXP setup, source
positions/scales, alpha64 increments/cap255, hidden behavior and per-record
six-update frame clock (including hidden records). Creation-pass120 wait
decrements to119 before presentation. Source data40 creates the original
Mario/Goombella victory roots with source priority/owner/U2a8 tracking.
Original404 frames418..421 and M_V_1/M_V_2/PKR_Y_1 are locally packed;
Citro2D preserves source texture RGB/alpha, bottom anchors and type0 nine-slice
frame geometry using original descriptor0 and a declared half-size viewport.
Native reward UI uses a layer independent of stage depth; existing original
victory PCM selects during this positive-EXP prefix.

Validation: make test passed151/151 CTest and103/103 Python tests
(`artifacts/trio-exp-tests-final.log`). Final make all and make debug-short
passed (`artifacts/trio-exp-bottom-all.log`, `artifacts/trio-exp-bottom-debug.log`).
Original assets regenerate locally; Python capture syntax and git diff --check
passed. Host checks pin ten-point setup, alpha/visibility, source wait/result
boundary and original victory pose names; icon metadata checks pin all four
original40x40 frames, delay6 and wrap. Integrated workspace also includes
concurrent workstreams; this commit stages this increment only.

Actual attacks/removal in isolated Azahar reach the original reward prefix.
Earlier diagnostic captures caught white texture tint and incorrect center-Y
anchoring; both corrected from native/source evidence. Final immutable binary
and matching ELF are `artifacts/trio-exp-bottom-snapshot/`; final isolated
capture evidence is `artifacts/azahar-trio-exp-bottom/`; reviewed final capture
and enlarged diagnostic confirm the original frame, bottom-anchored animated
star and victory poses after actual enemy removal. The original ISO and
user emulator profile/saves remain untouched. Full original frame/camera/GX
raster parity and hardware performance remain unverified.

The native stop remains actual PointGetEntry8021b70c, bank0700000a, before
counting. PouchEXP remains0, fieldresult0/story11/control lock remain intact;
this is not a completed victory or prologue. Next implement original US
PointGet strips/lifecycle, bank0b counting and ordinary0c..1f fade/export/field
return, then resume unchanged scene dialogue. Original camera/crowd/voice,
WIN2 routing, shared RNG/equipment/save ownership, alternative rewards/level-up
and broader prologue owners remain incomplete. Goal remains active.

## 2026-10-13 — Source book composition, captured paper and scene consumers

Implemented original procedural cover/page-base/illustration composition, source
final adjust over the newly visible market, and actual native GPU page capture.
Four independent RGBA8 textures acknowledge only a completed, successful old
composition copy. Native readback crop/rotation/padding/Morton layout has host
coverage. Focused isolated Azahar first exposed vertically inverted page fronts;
the actual display-transfer row mapping is corrected and subsequent fold captures
show upright original artwork. Screenshots also exposed opaque blend state leaking
into bottom-screen text; book consumers now restore alpha blending.

Original book6000ms fade, marketplace start after3000+1000ms and750ms retiring
tail now feed two fixed original-PCM stream owners. Original crystal/radiation
subtypes2/6/3 use original source geometry/masks and independent simulation
clocks. Original house sai_ki2/sai_ki1 geometry and finite4000ms opposite-X
parallax are restored. Source UnitEntry helpers now cover initial fields,
vectors, equipment, pouch import/revision, pose-name lookup/display admission;
source Init state4 returns after setting5, rather than falling through. These
helpers remain distinct from still-pending active startup/turn ownership.

Validation: integrated make test155/155 CTest and105/105 Python passes
(/tmp/opening-composite-capture-corrected-test.log). Native make all passes
(/tmp/opening-composite-alpha-restored-build.log). Focused isolated captures
artifacts/azahar-source-book-capture-corrected show upright old-page faces and
original fold geometry. A subsequent complete demo run is pending at this
commit; no original raster, audible/hardware parity or complete battle proof is
claimed. Original source hashes/anchors and asset converters remain read-only;
all raw resources/compiled packs/captures are ignored local products.

Also verified the clean combined primary ca0565a checkout:150/150 CTest,103/103
Python, native make all and isolated fresh-file route to saved source story3
Rogueport checkpoint pass (artifacts/opening-current-ca0565a-validation). Initial
combined build/test ran out of temporary-disk space; after archiving only our
generated outputs/caches to ignored project artifacts, the full rerun passed.
User emulator profile/saves and source ISO are untouched.

Next: complete native unskipped demo and new-file regression, integrate literal
harbor registry/entry/local-frame providers and original Init/FirstAct/Turn,
then command/damage/reaction/end/reward/field return. Original fade/camera/glide/
environment services and hardware comparison remain open. Result0/story4/save
barriers remain intact. The opening goal remains active and incomplete.

Follow-up: the first complete new-composition run raised a native exception
at the book-to-market transition, so it did not pass. Azahar's log showed
unmapped stack writes in original actor hierarchy evaluation; native default
stack is32KB and frontWorld reserved roughly16KB for its Game scratch snapshot.
That renderer-only snapshot now has scene-owned heap storage. Smoke captures
now detect native exception dialogs even while Azahar remains running. Two
focused harness checks pass; complete native rerun is pending.

## 2026-10-13 — Independent source EXP count/end audit

[EXP count follow-up](behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md#2026-10-13-independent0a0b0c-audit)
rechecks sourceheldpad/fastcounter latch, exactone-point/sound/levelguard
ordering, next-invocation0c transition, and icon-end retained bytes/pointers.
Validation: read-only complete used bodies; git diff --check. Docs only, no
native edits/tests/build/commit. Next native provider uses source count ticks
and soundrequests; actualaudio/raster/update/pouch-save lifecycle remains open.

## 2026-10-13 — Native original PointGet and Star Point counting

Bound actual source0a→0b→0c→0d→0e: used PointGet creation, original US
phrase/digit/shadow strips, signed packed EXP counting, held-pad fast latch,
source count-cue requests, exact wait/deletion/softdelete and five-call pointer
release. Separate host-testable effect retains entry/depth capture, source45
enter/hold/60leave ticks and original width-cap/cursor/UV/shadow geometry.
Native cosine is an explicit numeric adapter. Effect update follows battle/
event/icon work once per simulation timestamp, including creation pass.
Victory music ownership stays active through icon deletion independently of
icon visibility. GPU submission reuses the existing original UI atlas.

Validation: final integrated make test164/164 CTest +107/107 Python
(`artifacts/trio-pointget-tests-integrated.log`), make all and make debug-short
(`artifacts/trio-pointget-all.log`, `artifacts/trio-pointget-debug.log`) pass.
Earlier integrated runs caught concurrent opening-effects registration and
item-selection build-state failures; current full suite passes without changes
to those unrelated implementations/assertions. Original local asset generation
passes. Host checks cover original image dimensions, phrase/digit/shadow layout,
phase/softdelete timing, actual normal40-call and one-press/released fast10-call
payout, count-cue totals, delays and pointer release before effect disappearance.
Python capture syntax and scoped git diff --check pass. Tests/builds cover the
integrated workspace; commits exclude concurrent workstream edits.

Native evidence uses immutable binary/ELF `artifacts/trio-pointget-snapshot/`
and isolated capture `artifacts/azahar-trio-pointget/`: reviewed frame32 shows
original You got10 Star Points! art, source-scaled digits/shadow and victory
poses; final original-pointget-counted-return-boundary capture shows the effect/
icons gone and the development return notice. Actual original attacks/removal
reach this boundary; no shortcut result was injected. User emulator profile/
saves and read-only ISO remain untouched; extracted/compiled assets are ignored.
Original camera/dispatcher/GX raster and hardware performance are not verified.

Frontier is bank0700000e, before full victory export and field restoration.
Ten Star Points reach the battle's actual packed pouch; fieldresult0/story11/
control lock remain intact, and persistent pouch/save export is still pending.
Next implement ordinary0e→10→19..1f body/fade/export and actual sequence/field
wait publication, then resume unchanged scene dialogue. Source shared RNG,
equipment, camera/crowd/voice/count sound playback and alternative rewards/
level-up/fortune branches remain incomplete. Full prologue goal remains active.

## 2026-10-13 — Original victory-to-field handshake source delivery

[Field-return handshake](behavior/battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md)
closes exact sequencepoll>30, retainedfadepreset/source9/300initialization,
authoredGoombella joining-pouch flag, BattleExit serviceorder andnestedfield
control/post-placement requirements. Original161-command continuation owns
remaining dialogue/escapes/story12/keys/music; no battlefinishstorywrite.

Validation: read-only complete bodies/tables/originaljoinrecord andnativeAPI
inspection; git diff --check. Documentation only, no native edits/tests/build
orcommit. Next bind BattleEnd/contextrelease/pausedfieldroots andactualfield
partyconstructor/appearance/sequence/pouchsave ownership at1f; alternateflags/
fadepresets andoriginalexecution remain separate provider domains.

## 2026-10-13 — Original ending fade10 consumer source delivery

[Fade10 GPU consumer](behavior/display/BATTLE_END_FADE10_TEXTURE_CONSUMER.md)
closes embeddedwhite1x1I4/blacksourcecolor/bytealpha, exactcamera8quad and
material, completedflagdrawgate andNPC140OR100 typednamespace. Actualused
fade10 requiresnoexternalart; nativeprojection/coverage/GXrestoration remains
anexplicitbackendadapter untiloriginalrastercomparison.

Validation: read-only completebody/table/constants/embeddedtexel;
`git diff --check`. Docs only; no native edits/tests/build/commit. Next bind
actualsourcebytealpha/material/coverageandfieldreturn;otherfadeart/lifecycle
andcamera/GXnumerics remainopen.

## 2026-10-13 — Ending map-heap free and fresh field-party source delivery

[Ending allocation/teardown](behavior/battle/ENDING_HEAP_FREE_AND_REMAINING_UNIT_TEARDOWN.md)
closesactualBattleFree mapheader/null/coalescing andlaterBattleEndordered64
unit/resource releases. [Fieldconstructorcomposition](behavior/partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md#2026-10-13-battle1f-ordinary-goombella-constructor-composition)
pinsoriginalGoombellabank/HPpose/length56 andfreshHello10position-helperorder.

Validation: readonlycompleteusedbodies/serviceidentities;git diff --check.
Docs only; no native edits/tests/builds/commit. Next native1f mayretireE via
declaredmapallocator thenholdactualfieldconstructor; remainingtypedAppear2
ray/provider/context/registry/modelandlaterBattleEndtransitives remainopen.

## 2026-10-13 — Native ordinary finishing poses, fade and resource export

Implementation commit f068e8a binds source ordinary0e→10→19..1f through
resource export before BattleFree. Original body-part/pose lookup selects M_V_1
then type42 M_R_1/PKR_R_1 after60calls. Retained Mario flags and party-death
voice gates, actual crowd/BGM/voice request addresses and per-call state order
are preserved. Field encounterNPC140 OR100 has a distinct native flag view
and keeps ordinary NPC0/visibility flags untouched.

Used fade10 preserves sourcefloat division/add order,200ms duration, supplied
native refresh-rate adapter60, strict ratio>1 completion and truncated byte
alpha. Original embeddedwhite1x1I4/black-color result is adapted to native
screen coverage; no external fade image is missing. Packed exporter follows
original Mario HP/FP and party HP/raw snapshot order, including30status bytes
and retained gap22/23. Canonical pouch publication updates Progress HP/FP/EXP,
also retaining battle-owned storage for legacy checkpoints, without a normal
heal or end-of-battle Goombella unlock. Legacy inventory/quest/save state still
needs complete original lifecycle ownership.

Validation: final make test164/164 CTest +107/107 Python pass
(`artifacts/trio-ending-tail-tests-final.log`). make all/debug-short pass
(`artifacts/trio-ending-tail-all.log`, `artifacts/trio-ending-tail-debug.log`).
Host checks pin finishingpose/timer order, strict fade equality boundary,
actual packed export/noheal/maxima preservation, exact partner snapshot/gap
copy, Progress pouch publication and fieldresult0/story11/control lock.
Scoped metadata audit checked1486 relative link targets with zero missing files
(anchor identities were not separately checked). Initial test exposed and corrected missing canonical Progress ownership when
a legacy fixture entered without original storage. Python capture syntax and
scoped git diff --check pass. Integrated tests/builds include concurrent
workstreams; scoped commits keep unrelated pending changes separate.

Isolated native actual-attack evidence `artifacts/azahar-trio-ending-tail/`
reaches the completed black fade; immutable binary/ELF is
`artifacts/trio-ending-tail-snapshot/`. The development notice initially failed
depth ordering and was corrected; rebuilt final overlay binaries/ELFs are
`artifacts/trio-ending-tail-overlay-snapshot/` with capture
`artifacts/azahar-trio-ending-tail-overlay/` and successful all/debug-short
logs `artifacts/trio-ending-tail-overlay-{all,debug}.log`. Reviewed finalcapture
confirms the completed black fade and readable field-restoration notice;
frame34 confirms PointGet sliding out during Mario finishing pose. Original ISO and
user emulator profiles/saves remain untouched; raw/compiled assets are ignored.
Original camera/GX raster and hardware performance remain unverified.

Native frontier is actual BattleFree800f8908 before E teardown/field-party
reconstruction. Source1f result/Rule0/context/sequence/field admission owners
are not replaced by an immediate return or fabricated dialogue. Next bind
original map allocator/BattleFree, actual party constructor/appearance/hello,
Rule0/audience stop, remaining BattleEnd/sequence counter/field recovery and
resume the preserved161-command root. Source authored Goombella join owns
P0eOR1; legacy snapshots must not pretend that an ending grants the unlock.
Crowd/voice/BGM provider playback, original shared RNG/equipment/save domain,
alternative rewards/level-up/fortune and broader prologue work remain incomplete.
The full prologue goal remains active.

## 2026-10-13 — Source ending allocation/map-heap pairing delivery

[Original E allocation](behavior/memory/BATTLE_END_MAP_ALLOCATION_BINDING.md)
closesactualBattleAlloc8934/2ac call,32-byteheader/bestfit/split,704byte
zero/cache andnested400KiB rootbootstrap. Existingprimarymap/SDKsource
contracts reused; nativearena/priorallocationhistory remainsdeclaredadapter.

Validation: readonlycompleteallocator/wrapper/call/bootstrap andvalidheader
arithmetic;git diff --check. Documentation only; no native edits/tests/builds
orcommit. Next nativeE migratescorrectsourcealloc/freepair beforefield
constructor; fulloriginalresource/SDK/cache/history mapping staysopen.

## 2026-10-13 — Preserved underground postbattle callback audit

[Fieldroot audit](behavior/story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md#2026-10-13-preserved-native-return-callback-audit)
pinsactual61/62catalog meaning, ownfieldVMchildren/tribes andordered
postwaitdialogue/story graph. ConcretepartyRunStop/a_rec/musicfade/native
owner gaps are explicit; noforcedpouchunlock/control/storyrelease.
Validation: readonlyoriginal/currentnativecatalog/bodies;git diff --check.
Docs only; no native edits/tests/build/commit. Next bind actualfieldMario/party
registry/pouchjoin/context-restoration beforeoriginal61/posttailresume.

## 2026-10-13 — Native original ending map allocator/free

Ending work now uses the existing recovered eventMapAllocate/eventMapFree
implementation and source BattleAlloc/BattleFree requests. A declared E-only
primary reservation supplies the original400KiB nested battle subpool and
mandatory32-byte headers. Source bootstrap precedes battle-context selection;
E2ac rounds to2c0, the full704bytes are cleared/flushed, and free coalesces the
remaining tail back to63fe0. The1f caller clears Bf28 and writes topbank4.
Other native battle/unit/part work remains separately mapped; this is not
original prior allocation history or full primary-map/OS arena parity.

Validation: make test164/164 CTest +107/107 Python
(`artifacts/trio-mapheap-tests.log`), make all/debug-short
(`artifacts/trio-mapheap-all.log`, `artifacts/trio-mapheap-debug.log`) pass.
Integration checks pin mapped pool/header sizes,704-byte clearing, correct
allocation/free pairing, pointer clear/topbank write and original control/story
barriers. Existing portable allocator tests cover fragmentation/coalescing.
Actual attacks in isolated Azahar reach the completed fade/restoration boundary
without a native exception (`artifacts/azahar-trio-mapheap/`, immutable copied
binary/ELF `artifacts/trio-mapheap-snapshot/`); reviewed final capture.
`git diff --check` passes. Original ISO and user profiles/saves untouched;
original assets and compiled/capture products remain outside source history.

Frontier is actual field Mario getter8005e0c0 before field word4/party
constructor/Rule0/audience stop and subsequent context/sequence restoration.
No field result/control/story success is fabricated. Next bind the actual
field player/party owners and preserved parent wait61, then source postwait
run/stop/hitobj/music callbacks. Source Goombella unlock belongs its authored
joining event; full control/resource/save/raster/hardware parity and remaining
prologue owners remain incomplete. Goal remains active.


## Original fresh-demo entry / title exit fade ownership (2026-10-13)

Replaced immediate demo A/START switching with source buttonCheck and mapChange
ownership. Original demo.tpl GPU admission starts source type9 black500ms; final
rendered alpha0/frame completion gates input. A|START publishes original17/500
(or final-main17/2000) and the source dmo mapChange override17/2000 produces
F0 white alpha. Original repeated f32 simulation timing and strict inequalities
finish entry on31 and exit on121 ticks. Actual opaque final GPU presentation
owns title publication; B remains in Demo and triggers cannot dispatch twice.
Native adapter adds only original flat fade quads; no authored curtain geometry.
Corrected the previous next-task assumption: fade28 is a conditional restored
entry branch, while fresh-demo uses demo_init gp+108/+10c and seq_gameMain9/500.

Validation in /tmp/pm-frontend-source-fades:13 original source anchors verified
read-only; make test156/156 CTest and105/105 Python passed
(/tmp/frontend-source-exit-test.log); cached make all -o assets-ready passed
(/tmp/frontend-source-exit-all.log), compiling native renderer/main and .3dsx
with private original fixture assets and corrected fade23 pack. No emulator
run claimed for this increment; parent owns integrated visual validation.
Source contract:
[entry/exit fades](behavior/display/NATIVE_OPENING_DEMO_FADES.md).
Remaining: integrate narrow main/frontend renderer diffs into parent newer
native composition; original title curtain/logo/input services remain unbound.

## Ordinary original title ownership (2026-10-13)

Implemented portable OpeningTitle and source-linked native original curtain
consumer, bounded exporter and original-pack regressions. Demo→title waits
actual title PCM owner and original GPU center/sides resources. Source34 enables
narrowFast: first animated sample1.5→1 over90ticks (3 initial field; ordinary
non-fast alternative3→1/180 retained). Actual final geometry presentation owns
completion. Original20/500 white-in overlaps logo300→120 curve10 and copyright
-300→-170 curve11. After strict source second60-tick input gate, original
A|START accepts; B does not. Files publication waits genuine source63 A_1
0..80 then26 Z_1 0..60 and actual final-pose/frame sync for each. Unique GPU
ranges, original pass/cull/wrap/RGBA and original textures are retained.

Validation:21 source anchors verified read-only; make test157/157 CTest and
105/105 Python passed (/tmp/frontend-title-test.log); bounded private original
packs exported and host evaluated (center1416 bindings/4 textures; sides648
bindings/one texture); native make all -o assets-ready passed .3dsx
(/tmp/frontend-title-all-final.log), no native compiler warnings/errors.
No integrated emulator claim; root owns newer composition/native smoke.
[Source boundaries](behavior/display/NATIVE_OPENING_TITLE.md).

Next: import narrow frontend/main/header/helper diffs preserving parent native
palette/residency changes. Actual muted BGM admission, original title audio
fade, dual-texture prompt TEV/scroll, title actors, original19-second idle
return, seq6 return and full original file menu remain unbound. Current prompt
mask/file menu are explicit native adapters; the ordered playable route is
still being integrated and the full remake is not complete.

## Original title idle / prompt / BGM consumers (2026-10-13)

Bound original eligible-counter idle return (strict1141 ticks, A/START wins),
source33 center/A_2 actual0..160/161keys, source10/1000 black and actual final
rendered black/frame completion before fresh dmo clock. Original idle logo/
copyright curve11 endpoints and prompt amplitude fade remain. The genuine
US16 mask + TPL6 RGB pattern now use recovered two-TEV-stage math and source
scroll-counter/200; original unit288x32 quad0,-122 maps through the explicit
native viewport adapter. Separate six-vertex queue ownership avoids overwriting
message/curtain geometry; private cached original pattern64x4 is repeated into
native64x8 minimum storage and sampled inversely.

Source title audio now starts original volume88, publishes3000ms group0/1 off
and1000ms idle off, preserves already-running book6000 fade and detached750ms
float tail. A reachable market-on-during-exit scenario requires three source
streams. Native fixed PCM owners now use NDSP1/0/18 and72KiB buffer storage;
no queued wave buffers move and no live tail is dropped. Original PCM/loops
remain; fake resource readiness and generated audio are absent.

Validation:31 read-only source anchors; explicit original bounded packs exported;
make test158/158 CTest and105/105 Python passed
(/tmp/frontend-title-remaining-test.log); cached make all -o assets-ready built
.3dsx without warnings/errors (/tmp/frontend-title-remaining-all-final.log).
Parent owns integrated emulator visual/audio validation; none is claimed here.
[Title source boundaries](behavior/display/NATIVE_OPENING_TITLE.md) and
[BGM owners](behavior/audio/OPENING_TITLE_BGM_OWNERS.md).

Next: import narrow audio/header/main/title-helper diffs preserving parent NPC
and house-audio additions; regenerate private center pack nowtwo original poses.
Remaining original title actors, seq6 return branch, muted audio resource
admission and full original file menu/SFX are separate. Fresh empty-slot menu
adapter remains the authorized playtest target; full remake is unfinished.

## 2026-10-13 — Combined opening title validation

The combined isolated opening build now keeps demo events running during the
white map-change fade, rather than freezing them as soon as A/START is accepted.
A regression asserts that the original message owner continues until the final
opaque frame has actually been acknowledged. Title resource admission validates
the original PCM header/payload independently of mute or unavailable NDSP.

Validation: combined parent `make test`164/164 CTest +107/107 Python and
`make all` pass (`/tmp/opening-title-parent-tests-ready.log`,
`/tmp/opening-title-parent-all-final.log`). Initial title test correctly failed
before its newly required original asset packs were generated; the ready rerun
passes. Isolated native fresh-flow validation and primary integration validation
are underway. Source title actors, full original file-menu/card services,
complete harbor battle/return and original-runtime/hardware parity remain open.
The goal remains active.

## 2026-10-13 — Opening scene performance checkpoint

Latest native opening build renders the book scene and narration without missing
resource notices. Isolated Azahar capture reports 60 FPS and 1.68ms frame time
at the captured opening state (`artifacts/azahar-review-latest/boot.png`). The
current opening title work is still concurrent/uncommitted; this entry does not
claim full-scene coverage.

Comparison limitation: isolated Dolphin capture at 20 seconds remains on the
health and safety notice (`artifacts/dolphin-review-latest/boot.png`), so there
is no matched original title-scene frame or timing sample yet. Earlier one-off
quiet-opening 29–30 FPS reading remains unreproduced; no performance change is
justified from it. Do not resize or otherwise reduce title assets based on the
current evidence.

Validation: `make all` passed for the current source state; latest Azahar
opening capture completed at 60 FPS / 1.68ms. Isolated Dolphin startup capture
completed but did not reach the opening scene. `git diff --check` passed.
Next: advance the isolated Dolphin profile past its safety notice and capture
the matching original scene; continue matched sampling in Voyage, field and
battle states before proposing substantial optimizations. Hardware validation
remains open.

## 2026-10-13 — Primary title and fresh-file sequence verification

Primary normal `make all` passes (`artifacts/opening-title-primary-all-final.log`).
The immutable integration binary is
`artifacts/opening-title-integration-snapshot/ttyd-2ds.3dsx`; it includes concurrent
window-rendering work and is not asserted to be a clean single-commit artifact.
Isolated native fresh flow passes through original demo exit, title input/curtain
transitions, four empty files, house, letter, voyage/prologue and saved harbor
story3 (`artifacts/azahar-title-primary-fresh-chain/`). Reviewed title/files/house
and arrival captures; exact original raster/audio and hardware parity are not
claimed. No ISO, original user emulator profile or existing saves were modified.

Primary `make test` was run twice. The stable rerun passes169/170 CTest;
`underground_story_original` still fails its unchanged postbattle position
assertion (expected X=-55, diagnostic actual X=-80,Y=20,Z=35 at time7123).
The first run also lacked a field-object test executable during concurrent
asset/build activity; that test passes in the stable rerun. The full suite is
therefore not reported green. Opening/frontend scoped CTest passes15/15 and
separately run Python passes110/110
(`artifacts/opening-title-primary-{tests-ready,scoped-tests,python}.log`).
Combined isolated parent checks pass164/164 CTest +107/107 Python.

The source final-pose GPU acknowledgment is now issued once per title curtain
request; the retained pose remains available without requesting repeated
frame waits. Smoke input now waits for source title input eligibility and both
file-selection curtains, so one early press is not mistaken for New Game.

Next: integrate preserved Parakarry glide and house original sample requests,
correct the harbor fade23 bank in primary, reconcile the untouched underground
postbattle test with its actual event branch, and finish literal native harbor
battle/field owners before claiming the broader prologue ready. Original title
actors, full original file/card services, source display projection/reflow and
all original gameplay/runtime/hardware comparisons remain open. Goal active.

## 2026-10-13 — Isolated Dolphin opening comparison attempt

Retried the original startup with multiple virtual-button sequences in separate
project-local Dolphin profiles. The initial 20-second capture remained on the
health and safety notice. A virtual A input produced intermittent black/faded
frames; subsequent captures alternated between the notice and its continue
prompt. Repeating with a keyboard-configured controller and a 40-second
post-input wait again ended on the notice. These captures do not establish a
Dolphin rendering fault or a native-game performance dip; they are invalid for
matching the port opening. No source or asset optimization is indicated.

Evidence: `artifacts/dolphin-review-latest/boot.png`,
`artifacts/dolphin-review-advanced/after-safety.png` and
`after-startup.png`, `artifacts/dolphin-review-advance2/advance-*.png`,
`artifacts/dolphin-review-held/after-held-a.png`, and
`artifacts/dolphin-review-longwait/after-long-wait.png`. The alternate keyboard
mapping existed only in `/tmp/emulator_smoke_keyboard.py`; every profile and
capture was isolated under `artifacts/`. No user Dolphin profile or save was
used.

Next: resolve isolated Dolphin controller focus/mapping or run the original
manually in its isolated profile, then capture the matching opening. Until then,
retain the existing Azahar 60 FPS opening result as port-only evidence. Scene
coverage for Voyage, field and battle, repeatable dip measurements, and hardware
validation remain open.

## 2026-10-13 — Preserved underground battle start/wait binding

The preserved underground VM now dispatches original callbacks8004ef54 and
8004eed8 through its supplied FieldBattleServices owner. Game supplies that
owner only for the underground scene and starts the trio on the exact original
start callback, removing duplicate battle initialization. Unbound standalone
scenes and previous-party placement retain their service barriers.

The host regression verifies first-call initialization, repeated NPC lookup,
sequence4 rejection even with a result, and sequence2 rejection with result0.
Only sequence2 plus a nonzero result resumes original mac_0_093, attributed
to NPC1, with story11 and the original PTK_D_1 field pose/placement. This
revealed that deleted movement events kept interpolating after the root placed
the NPCs. Movement now retains its eventID and stops when that event is deleted.
The regression failed before this fix and passes afterward. Linear movement,
native field controls/registry tokens and party placement remain adapters.

Validation: make test passed170/170 CTest and110/110 Python checks
(artifacts/trio-fieldwait-tests-final.log); make all passed
(artifacts/trio-fieldwait-all.log). Scoped git diff --check passed. No new
Azahar or hardware return capture; the native return is still unbound.
Initial integrated build encountered
a transient unrelated collision-test link failure; rerun reached the new
placement assertion and exposed the movement bug above.

Native ending still stops at field Mario getter8005e0c0 before field party
construction, Rule0, resource/context teardown and sequence2 publication.
The host test supplies its result; it does not establish a native playable
field return. Next: bind actual field Mario/party storage, authored join
history and constructor/appearance providers, then original exit/sequence
publication and the remaining original postbattle party/collision/music
callbacks. Full prologue parity and hardware validation remain unfinished.

## 2026-10-13 — Opening goal scope and Parakarry glide

The user clarified that this opening goal ends at Rogueport arrival. Harbor
battle/tutorial and Goombella joining are separate follow-up work. Existing saves
are not a constraint for this goal. The source Parakarry delivery/departure glide
is now integrated into primary, including independent source float curve11,
vertical arc, incremental deltas, terminal arithmetic and completion-yield gates.
[Glide contract](behavior/story/NATIVE_OPENING_NPC_GLIDE.md).

Validation: primary `make all` and `make test`171/171 CTest +110/110 Python pass
(`artifacts/opening-glide-primary-{all,tests}.log`). The prior concurrent
underground postbattle assertion failure is resolved in this integrated rerun.
Read-only glide evidence verifies four original body hashes and43 anchors
(`artifacts/opening-glide-source-evidence.log`). Title integration also passed
`make debug-short` (`artifacts/opening-title-primary-debug-short.log`).
Next finish prepared original house PCM cues, run the final fresh-file native
route, and complete this arrival-scoped goal. Broader battle/SDK work remains
separate; no full-game completion is claimed.

## 2026-10-13 — Persistent field Mario control/ending owner

Game now owns the field Mario word0/word4/byte38 views independently of
battle units. Zero initialization is the recovered MarioInit memset point,
not a complete field Mario constructor. Battle services use original
marioCtrlOff/On wrapping-byte nesting and signed marioCtrlOffChk; field
input admission reads the same byte. CtrlOn clears word0 bit2 only on the
transition to zero; unrelated flags survive. The Crump development retry
releases its old hold before creating another battle, preventing a stuck
control counter after retry and return.

The original trio ending now obtains this Game-owned field Mario view and
clears word4 bit1 after export/free/topbank4. Its next suspended provider
is marioPartyEntry801412fc, while standalone callers without a field owner
still stop at marioGetPtr8005e0c0. Party construction is not substituted
with battle actors or an invented unlock. The native ending regression
checks the retained control hold, current ending bank, cleared/free ending
work and unchanged story11/result0. Portable checks cover nested holds,
0/255 and127/128 wrapping, signed checks and preservation of other flags.

Validation: make test passed173/173 CTest and111/111 Python checks
(artifacts/trio-field-mario-tests-final.log); make all passed
(artifacts/trio-field-mario-all.log). Targeted field battle/trio checks
passed (artifacts/trio-field-mario-target.log); scoped git diff --check passed.
First full run saw an empty concurrently regenerated startup texture; the
focused asset check and full rerun passed without weakening admission. Read-only
original CtrlOff/Prepare disassembly retained under ignored
artifacts/recovered-temp-evidence/native-field-*. No original assets changed.

Remaining: full MarioInit/ReInit, Mario battle preparation/post position and
mode services, global two-slot party controls/registry, authored Goombella
join history and actual constructor/appearance/Hello providers. Source
sequence battle start has its own second control hold and exit releases it;
those sequence/context owners remain unbound. Next implement them alongside
party restoration, Rule0, audience stop and original exit/sequence/result
publication. No native field-return capture or full prologue/hardware parity
is claimed. Goal remains active.


## 2026-10-13 — Original opening house SFX owners and native PCM

Implemented original house flight/postbox/door sound scheduling, raw sound keys,
source command IDs/control parameters and retained environment/scope requests.
The independent flight inline waits3000ms after spawn, calls14 times at200ms,
then400ms with a WW1 stop test AFTER each wait; pro_00 completion writes WW1=1,
followed by17 final200ms calls. It never stops early on glide completion or
fabricates a loop tail. Each postbox sequence requests one sound before its
three shakes; four door sounds precede their original interpolations. Source
NPC queries retain Q1024 X/Y and literal Z=-100; door/postbox positions come
from original map-object leaf averages and current additional transforms.
Original dialogue, message IDs/order and story writes are unchanged.

Original sound identities resolve four primary MusyX samples1176/479/840/851
with their table/FX/object IDs and parameters preserved. Private read-only
exporter produces four mono PCM16 WAVs at16000Hz with no resampling or loops
(~75KB), plus404-byte opening-house-sounds.bin source metadata. Runtime admits
all four PCM headers/sizes with metadata before opening begins. Extracted/raw/
compiled assets/provenance stay ignored in local-assets/opening-sounds/romfs;
no parent mutable pack hardlinks or source payload commits. See
[source sound contract](behavior/audio/NATIVE_OPENING_HOUSE_SOUNDS.md),
[house presentation](behavior/story/NATIVE_OPENING_HOUSE_PRESENTATION.md) and
[asset record](ASSETS.md).

Core/exporter/test commit e7d3a01; separate native presentation commit7cf2de5,
in isolated /tmp/pm-opening-demo-stage. Native OpeningHousePcmPlayer lazily
imports original samples, supports overlapping wave buffers, uses source
sample rate/table volume/pan and reports load/position/voice failures. Its
NDSP10..17 channels avoid BGM0..1 and primary eight clips2..9. Native patches
are narrow: Audio declaration/member, one shutdown hook, separate implementation
files and one main updateOpeningHousePcm call while OpeningPart::House. Parent
music/fade/field-NPC Audio changes were not overwritten. Parent must integrate
these hooks and copy the private four WAVs/404-byte metadata, not hardlink them.

ENV_OPN_AAA1 source name index1/five rows refer to birds1/2/3 and leaves1/2.
Original ENV on0x110, off0x4800, LPF(0,800) and BGM scope(0,1) remain explicit
unbound records, with no fabricated ambience or successful consumer acknowledgement.
Native primary-sample PCM is separate from full MusyX macro delay/pitch/RNG/
envelope/child voices, original positional calc3D/global-volume/priority/handle
owners, and ENV random/timed services. Those remain incomplete.

Validation: final make test133/133 CTest and106/106 Python; make all builds
native ttyd-2ds.3dsx without warnings. Read-only verifier pins187-command house
hash, seven source service hashes and24 sound/loop/WW1/environment anchors.
Host tests cover loop counts/stop ordering/tail, holding pro_00, original
postbox/door/env counts/parameters/position availability, full scene progression,
actual PCM identities/header/rate/frame/hash/nonzero content and original leaf
metadata. py_compile/git diff --check pass; check_doc_links checks835 Markdown
files/0 broken links. Logs /tmp/opening-sounds-tests-final.log and
/tmp/opening-sounds-all-final.log.

No native audible smoke, original-runtime/Dolphin trace or hardware playback
claim for this isolated branch. Next integrate source SFX/PCM hooks and verify
actual playback, then implement original listener/spatial/MusyX/ENV/fade
consumers. Full opening/prologue fidelity and overall goal remain incomplete.

## 2026-10-13 — Field party lookup gate before restoration

Completed source lookup/entry audit after Mario owner commit bda11e8:
primary then auxiliary signed indices require nonnull actors with word0 bit8
clear; ordinary entry checks the canonical pouch unlock before destroying
existing primary actors. Added three whole-body hashes and an EVT-01 inbox
breadcrumb. Native fixed party tokens remain unbound registry adapters.
Next bind actual two-slot field ownership and constructor/appearance/Hello
services using these gates and original joining history. Documentation-only
follow-up: scoped diff check passed; prior native validation remains173 CTest,
111 Python and make all. Full prologue/field return remains unfinished.

## 2026-10-13 — Owned field party registry and ending entry prelude

Replaced fixed field-battle partner token1 with Game-owned two-slot field
actor storage, source primary/auxiliary current-index selection and global
controls for both registered actors. Records are188hex bytes with explicit
big-endian words; fresh pointers are null and Mario indicesff/ff. Slot
allocation follows first-null selection and source-sized clearing; simulation
copy fixtures copy independently. Original field rendering/join history is
not inferred from the existing Goombella billboard.

Ending now checks the canonical source pouch unlock before party creation.
Unjoined storage executes the original failure return without slot/unlock
changes, performs live current-party lookup/flag cleanup and stops at RuleKeep.
An explicitly joined checkpoint reaches EntryMain allocation and stops at
marioGetCamId before inventing a pose, owner, placement or constructor result.
Original existing-party destruction and other kind domains remain providers.
Both branches keep story11/result0 and the Mario control hold.

Recovered complete appearance2 with the documentation agent: source accepts
any nonnull down hit, uses38-unit down length and distinct single/dual query
order; general placement's floor classifications do not apply. Confirmed
source empty registry and current-slot constructor stores. Added subsystem
contracts with original body/constant/template hashes; no raw assets changed.

Validation: make test passed173/173 CTest and115/115 Python checks
(artifacts/trio-field-registry-tests-final.log); make all and debug-short
passed (artifacts/trio-field-registry-all.log and
artifacts/trio-field-registry-debug-final.log). Targeted field battle/trio
tests and scoped diff/link checks passed. First full build saw a transient
partner-follow-test link failure (missing main); debug packaging saw a
concurrently regenerated soundtrack directory disappear. Both reruns passed
without source/test changes. Isolated Azahar completed using immutable
artifacts/trio-field-registry-snapshot binary/ELF and legacy story11 fixture
(artifacts/azahar-trio-field-registry-ready). Reviewed actual battle target
selection, final Headbonk, original10 Star Points/poses and opaque ending
fade/development field-restoration notice. The initial8-second startup input
missed battle entry (artifacts/azahar-trio-field-registry);45-second startup
reached the intended battle. Capture does not prove joined constructor
completion, runtime control words, native return or hardware behavior.

Next: actual camera/animation/material/kuribo_init, placement2 and Hello,
source joining graph and persistent party-slot lifetimes; then RuleKeep,
audience stop and original exit/context/sequence/result publication. Native
field return/full prologue parity and hardware validation remain unfinished.

## 2026-10-13 — Original house sound integration and observed opening corrections

Primary now schedules original Parakarry/postbox/door sound requests and plays
the four original primary samples through separate NDSP10..17 voices, preserving
existing eight effects2..9 and three music owners0/1/18. Source ENV requests are
retained; complete MusyX/spatial/ambient services are not claimed.

Validation: `make all` and `make test`173/173 CTest +111/111 Python pass
(`artifacts/opening-house-sfx-primary-{all,tests}.log`). Read-only evidence pins
the187-command event, seven service hashes and24 sound/loop/environment anchors
(`artifacts/opening-house-sfx-source-evidence.log`). An immutable binary in
`artifacts/opening-house-sfx-snapshot/` completes the fresh-file route and saved
story3 in isolated Azahar (`artifacts/azahar-opening-house-sfx/`). The supplied
local DSP firmware was copied into that isolated profile; Cubeb/HLE audio starts.
No audible waveform/original-runtime/hardware parity assertion is made.

The user then observed six corrections still required: mail camera oddities,
missing skies, input-driven character dialogue bubbles, severe book-to-Peach
slowdown, visible scene stubs, and ship arrival/departure motion. These are part
of the arrival-scoped opening goal; it remains active. Three explicitly authorized
isolated agents now own transition allocation/performance, original skies/ship
and voyage, and dialogue/camera/wake. Root owns used harbor jump and native
prologue heading display. Findings: Peach4MiB atlas repeatedly exceeds free
linear memory; renderer lacks original b/<map>.tpl backgrounds; source ship/wake/
heading timing differs from current simple adapters. Finish and integrate these
consumers, then verify the complete corrected sequence before completion.

## 2026-10-13 — Field party camera/actor initialization before pose entry

The joined ending branch now advances from camera query through source
EntryMain actor writes: camera164, negative pose/event IDs, flags, slot,
encoded native owner identity, kind, conditional primary saved kind, global
definition and Mario80000000→party8000 propagation. Ordinary kind1 resolves
the original c_pkr resource804213e0 or d_mario override802cb340, then issues
animPoseEntry(resource,2) and waits there. It creates no pose handle or
constructor result while the animation service is unbound. Native owner
identity85700000 is an explicit encoded port handle for bound typed Mario
views, not an original heap address or invented full Mario record.

Source audit confirmed Mario camera initialization writes4 AFTER zeroing.
The persistent owner uses that initialization; raw setter values survive
native restore/re-entry, matching the recovered camera-ID preservation.
This does not claim a full MarioInit/ReInit or rendered viewport binding.
Tests cover supplied camera7, rawffffffff retention, both registry slots,
owner/kind/negative IDs, unchanged current indices, saved-kind zero versus
nonzero, auxiliary-kind preservation and dot-flag propagation. Resource
selection/order is checked against the recovered source contract.
Joined native ending fixtures remain paused with original story11/result0,
control hold and canonical pouch; unjoined behavior is unchanged.

Documentation delivery closes exact mode2/archive0 selection, PeraOff mask8,
new material defaults and two live Mario animation selectors; original
resource/shape/time/matrix/render owners remain explicit dependencies.

Validation: make test173/173 CTest +116/116 Python passed
(artifacts/trio-field-pose-entry-tests-final.log). Native make all passed
(artifacts/trio-field-pose-entry-all-final.log); scoped diff check passed. No new emulator/hardware camera/pose claim; prior
isolated registry battle capture still covers the unchanged unjoined path.

Next: actual positive animation resource/pose admission using the original
bank, PeraOff, Mario pose/material readers and setters, original HP/pose/init,
appearance2 and Hello. Also bind authored joining history and persistent
party/map lifetimes, RuleKeep/audience and exit/context/sequence/result
publication. Native return/full prologue parity remain unfinished.

## 2026-10-13 — Admit original Peach during the book-to-market overlap

Reproduced the reported natural opening slowdown in an isolated Azahar profile.
The initial book rendered60FPS, but the final zoom repeatedly reported
`Scene texture failed: c_peach (3273 KB free)` and App1FPS. The native renderer
was retrying the entire original19-pose Peach bank on each failed frame; its
1024×1024 RGBA8 atlas required4MiB while the authored book/market overlap left
only3273KiB of linear storage. This was failed resource admission, not successful
scene playback at a slow rate.

The original Peach atlas now uses half-size Lanczos512×512 RGBA8 (1MiB), keeping
all original cells, normalized UV and19 poses. Private evidence confirms the
PMANIM and uvscale SHA256 hashes exactly match the pre-fix originals; model,
geometry and animation data remain unchanged. The renderer retains the admitted
market map, actors, samples and map animation clocks when the authored180-tick
adjustment finishes, synchronizing and retiring only book textures/captures.
It no longer destroys/reloads the whole market at that presentation boundary.
Original scene events, message IDs/dialogue/order and the final zoom are retained.
Implementation/asset contract commit76ebdf0; no extracted or compiled payloads
were committed. See [book resource contract](behavior/story/NATIVE_OPENING_BOOK_PRESENTATION.md)
and [asset record](ASSETS.md).

Validation in ignored `artifacts/opening-transition-perf/`: `make all` passed
(`artifacts/build-fixed.log`), final `make test` passed170/170 CTest and110/110
Python (`artifacts/tests-fixed.log`), py_compile and scoped git diff --check
passed. Private `artifacts/peach-atlas-evidence.json` records unchanged animation
hashes, all19 poses and exact1MiB/4MiB allocation sizes. Isolated natural smoke
used `tools/emulator_smoke.py --fresh-save --seconds 1 --input-script
artifacts/profile_transition.py --output-dir artifacts/fixed`; captures at120s
show the actual book zoom over the live market at36FPS, and130/155s show original
Peach/merchant scene/dialogue at60FPS/100% speed, without resource error notices.
These are Azahar status-bar observations under software rendering, not measured
physical New2DS XL performance or original-runtime/Dolphin parity. The source
snapshot's doc-link checker reported one existing PROGRESS link to the parent's
uncommitted house-sound contract; no new links were broken.

Remaining: finish integrated parent house-audio/fresh-file arrival validation,
retain the original scene rendering/timing fidelity gaps already documented,
and measure physical hardware performance. This fixes the demonstrated texture
admission loop; it does not claim full opening or full-game fidelity/completion.

The final240s capture retained the original merchant/chest scene at60FPS;
A then requested the original demo-to-title exit; the last capture shows its
white fade (`artifacts/fixed/title.png`), not a fully displayed title. The
isolated smoke exited successfully with no application exception; user profiles
and saves were untouched. Next callback work is assigned separately: original
Voyage logo/start/paper-boat local transforms, coordinated with the voyage owner.


## 2026-10-13 — Interactive opening bubbles, house lens and used awakening

New Game messages now use the existing original top-screen window/panel/masks/
font/tail renderer. House anchors come from the actual rendered original speaker
pose and Mario root projected by that same world matrix. Duplicate flat lower
character text/name labels and the development-sequence footer are removed.
Original US IDs/bytes/tokens/order/speakers remain. Opening playback opts into
an input wait for otherwise unkeyed terminal text; original final<k> retains one
press, and demo/book default timing is unchanged. The house/world camera now
uses source25 lens and centered304x240 crop/effective19/15 aspect, preserving
its original eye/aim/curve11/5000ms/2000ms requests.
[Input/framing contract](behavior/story/NATIVE_OPENING_INPUT_AND_FRAMING.md).

Bound used gor awakening waits, literal speakers0 and original depth/eyelid WW
interpolation/publication, source camera road at retained sys Mario0/0/0 and
simulation-owned cam_yure Y-view translation. Original marukongou/densha/
pukupuku hidden subtrees are omitted. Native current-world GPU rendering feeds
recovered eight polynomial UV taps/weights into the original quad; original
black eyelid quads draw before windows. Real VRAM/render-target admission,
resource lifetime and failed-target checks replace the initial readback design.
The new-map consumer uses the frame owner's existing synchronization. The
portable Prologue helper exposes actual fade/display-work/start/presentation
acknowledgments and source3000/2000/3000 waits plus0x1300 input; it does not
invent elapsed-only display completion. Parent owns the actual heading provider.
[Awakening/event contract](behavior/story/NATIVE_OPENING_AWAKENING.md).

Validation: scoped branch make test171/171 CTest +110/110 Python passed
(artifacts/wake-tests-final.log); normal make all passed without warning/error
(artifacts/wake-native-final.log). Read-only verifier pins four original events,
three draw/motion bodies and31 awakening/heading/camera anchors
(artifacts/wake-source-verification.log); original house verifier31 anchors
also passes. Earlier bubble/lens-only fresh-file native flow reached saved
story3 (artifacts/azahar-dialogue-camera/); that run predates new heading/Wake
binding. A separate ignored fixture starts the real Wake owner directly;
reviewed closed-eyes held012, source opening/blur/reveal and013_01 printing in
artifacts/azahar-wake-reviewed/. These samples report60FPS/3.81–5.56ms.
Cold boot captures at5s were premature: original resource initialization
completed around11s in diagnostic trace, so0FPS startup frames were not proof
of a persistent rendering stall. Trace/temporary fixture main edits were
restored; no production debug bootstrap or fake native heading acknowledgment
is committed. New links are valid; whole-branch link audit retains one existing
broken link to the parent pending house-sound contract. py_compile/diff check
pass. Original ISO, user emulator profiles and user saves remain untouched.

Limitations/next: integrate parent's actual sys heading display/resources and
boat/sky/arrival providers, then run the whole fresh-file route against the
combined normal binary. This branch's new heading protocol intentionally waits
its real native display provider. Source eyelid/depth clocks/global cadence,
SDK trig, RGBA8 versus RGB565/TEV precision, full map animation, waking sleep/
find/creak/ENV audio and hardware/original-runtime raster comparison remain
open. Full remake and broader battle/tutorial/join are not complete; current
user scope ends at Rogueport arrival.

## 2026-10-13 — Integrated bubble, camera, wake and book-memory corrections

Primary integrates the independently reviewed original opening bubble/camera,
Wake event/effect and bounded Peach atlas changes. New Game keeps unkeyed
terminal pages until input, preserving original IDs/control tokens; these held
pages also use the existing original advance indicator. Ordinary message
playback and the storybook remain outside this opt-in presentation change.

Validation: primary `make all` passed
(`artifacts/opening-corrections-primary-all.log`); `make test` passed175/175
CTest and116/116 Python (`artifacts/opening-corrections-primary-tests.log`).
Read-only Wake verifier passed31 original anchors; doc links993 files/0broken.
Individual isolated Wake and natural book-transition evidence is recorded above.
This combined build has not yet had a complete native fresh-flow test: its
portable heading now correctly waits for the pending real native display owner.
Skies/ship arrival/departure and the animated heading are still being integrated.
The user corrections and arrival-scoped goal remain active; no complete opening,
full-game, original-runtime or hardware parity claim is made.


## 2026-10-13 — Bounded fresh-opening GW8 event provenance

Pinned Init's explicit initial GW8 word0 at fieldC0+0x24 versus ReInit's bank
preservation, and original arrival's incomingGW8→dummy position→getterLW1→
SETF GW8→resolved nohit jump flow. A conservative local REL reference closure
from opening roots covers59 events/2701 commands with zero literalGW8 operands;
gor00 init174 commands and the arrival prefix before25920 also have none.
[Scope and conditional conclusion](behavior/story/OPENING_FRESH_GW8_PROVENANCE.md).
No hardcoded zero replaces the callback's resolved Y. The manifest contains
only source identifiers/extents/hashes, not original bodies/dialogue/assets.

Validation: read-only verifier checks59 original hashes/2701 command operands,
root/reference closure, two full Init/ReInit body hashes, nohit jump body/−4
height literal, two original arrival hashes and four operand/getter/SETF/jump anchors
(artifacts/gw8-prefix-verification.log). py_compile/scoped diff check pass.
Docs/verifier only; no make test/all rerun or native/Dolphin observation claimed.
Whole-branch link audit retains the existing parent pending house-sound link.

Remaining exact dependency: opaque C++ Title/load/callback direct or aliased
bank writes, synthesized operands, imported/dynamic event owners and NPC
getter/physics/scheduler values are not closed by the event-token census. The
verifier explicitly reports callback_store_closure=false. Native fresh bank0
plus retained actual writes/resolved position is an explicit source-based
bridge; unconditional original-runtime zero/full-flow parity remains unproved.
Parent owns ship jump/arrival integration and fresh-route validation.

## 2026-10-13 — Combined native mail hold and natural transition observations

Immutable `artifacts/opening-corrections-snapshot/ttyd-2ds.3dsx` from the combined
bubble/camera/Wake/book correction build was tested with fresh isolated Azahar
profiles. `artifacts/azahar-opening-corrections-bubbles/` reaches the real title
and file selector; `mail-dialogue-held.png` and `mail-dialogue-still-held.png`
show the unchanged original Mail call bubble/advance star separated by8seconds
without input,60FPS/5.26–6.79ms. A closes the bubble and resumes original
Parakarry flight (`mail-dialogue-next.png`,60FPS/4.94ms). Skies are not yet in
this snapshot. This scoped run deliberately ends before the pending heading
provider; it is not complete arrival validation. Initial sandbox Xvfb failed
to bind its local socket; the authorized isolated-display rerun passed.

`artifacts/azahar-opening-corrections-book/` follows the natural unskipped demo:
at120s the real final book zoom displays37FPS/84% speed/12.47ms; at155s the
original Peach marketplace scene displays60FPS/100%/7.56ms, continuing through
the240s capture without an allocation failure. A then requests the original
white exit; the final capture is that fade, not proof of fully displayed title.
Both scripts exit successfully with no native application exception. Values
are Azahar status-bar observations, not a physical2DS benchmark or original
execution/raster parity. User emulator profiles/saves remain untouched.

Next: integrate actual heading display and original skies/ship consumers,
finish the unresolved Voyage sky appearance, and run the combined fresh-file
route through actual boat arrival/departure and the saved story3 endpoint.

## 2026-10-13 — Original opening sky, voyage and harbor ship owners

Isolated `artifacts/opening-skies-boat-worktree` retains original four opening
backdrops and original miniature/harbor steamship geometry. Source voyage seeds,
logo admission/acceptance, approach/departure, original camera requests, completed
`yoru_hiru` mode2 pose and local sun translation replace native timer/sky/boat
snapshots. Harbor approach, speaker0 messages014/015, original nohit dummy jump,
actual OFF_d_kaiten ship-only framebuffer capture/A_1 final-pose acknowledgment
and departure are bound. Fresh GW8 is an explicit native word and nonzero values
are passed/monitored rather than forcing the jump Y to zero. Original dialogue
bytes/IDs/order are unchanged. [Voyage scope](behavior/story/NATIVE_OPENING_VOYAGE.md),
[ship scope](behavior/story/NATIVE_OPENING_HARBOR_SHIP.md).

Actual scene/image/capture failures now stop native opening progression and
cannot generate a successful arrival save. Background raster draws disable
depth writes; isolated native house geometry/clouds render together. Source
room13/14 private wake groups have bounded admission without changing ordinary
room limits. Per-object cached trigonometry/inverse matrices and dense child
geometry ownership replace tens of thousands of per-vertex trig calls and
heap allocations; original geometry/art was retained.

Validation: isolated make test175/175 CTest +112/112 Python and cached
make all -o assets-ready pass (`artifacts/ship-ready-final-{tests,all}.log`).
Harbor source verifier31 and voyage28 event/body/request anchors pass; companion
Voyage callback verifier passed (`artifacts/voyage-callback-source.log`).
Original completed-sky/local-sun regression passes; nonzero dummy jump,
source timing/message gates, sparse/dense clocks, final actual render/opaque
acknowledgments, missing scene failure and private original geometry are covered.
First smoke correctly exposed private visibility-group admission and background
depth defects; both were fixed. Companion isolated fresh route reached actual
Rogueport field at60FPS. Latest day-pose/cache/25FOV capture reports Voyage,
logo and harbor60FPS (`../artifacts/opening-heading-native/artifacts/azahar-boat-day-pose/`);
its remaining opaque white sky/material and small voyage ship are unresolved
visual defects, not fidelity/performance completion. No user profile/save or ISO
was modified; all original products remain private/ignored.

Next: integrate this scoped source/independent private packs with primary's
newer house SFX, source bubbles/Wake/heading and viewport consumers; inspect
original used Voyage sky color/TEV and camera/model framing to correct white
background/horizon scale, then run the final combined fresh-file native route.
Original BGM six-second fade, spatial/MusyX/ENV, alpha callback, overlapping
wave channels, generic image projection, exact runtime and hardware parity
remain open. This is the arrival-scoped opening goal; full remake is unfinished.

## 2026-10-13 — Native original partner animation admission

Joined ending now admits an actual converted c_pkr animation resource and
owned CPU pose/shape workspace, publishes its nonnegative registry handle
in actorPc and applies original PeraOff mask8. Mode2/archive0 and new
material/light defaults0 are recorded; simulation milliseconds are explicit
native metadata. Original positions/normals/nodes/visibility and hierarchy
workspace come from the strict existing PMANIM decoder. No animation name
is selected, vertex draw submitted or GPU consumer acknowledged at entry.
Immutable bank data is reused; simulation clones keep independent mutable
shape workspaces. Missing, unsupported d_mario or exhausted native partner
pose slots retain animation admission rather than minting a fake handle.
The partner-only native handle namespace/capacity is not original global
pose-pool ABI/capacity. Mario ownership must be joined consistently before
original shared-handle callbacks are declared supported.

The ending frontier is now MarioAnimeId800593fc before original material
inheritance. Documentation recovered WORD4 effect2/rear1/front0 priority,
actual logoExit/marioPoseInit creation of a_mario/a_mario_r/e_mario mode2
poses and re-entry/char-mode/door/blend material writers. Ordinary ReInit
clears material and adds light mask2; newly created zero defaults do not
justify a constant current Mario material. Added exact source hashes and
inbox breadcrumbs; no original asset conversion or raw payload changed.

Validation: make test176/176 CTest +116/116 Python and make all passed
(artifacts/trio-field-animation-tests-final.log,
artifacts/trio-field-animation-all.log). Original geometry-backed checks
are separate from portable field protocol checks. First run correctly failed
because the new asset check ran from the portable test's build directory;
added a source-root original-assets test instead of weakening admission.
Checks cover supported/failing resource/mode/pool admission, real shape
buffers, bank reuse, paper flags and independent snapshots. Native joined
ending checks real handle/defaults and unchanged current indices/story/result.
Scoped diff/link checks passed. No new native GPU/animation/material or
hardware capture claim; prior battle capture uses the unchanged unjoined path.

Next bind actual Mario field pose ownership/selectors/material producers,
then exact HP/pose/kuribo_init, appearance2 and Hello with source assets,
renderer/lifecycle and original join/map history. RuleKeep/audience and
exit/context/sequence/result publication remain open. Full native prologue
parity and field return are unfinished; goal remains active.

## 2026-10-13 — Primary original skies, Voyage and harbor integration

Primary integrates original backdrop assets, used ship hierarchies/clocks,
Voyage logo/acceptance, source harbor approach/departure, real ship capture/
paper turn and nohit jump. Fresh GW8 storage is explicit and preserved through
the actual dummy-position/jump flow; it is not a forced height constant.
Scene admission now waits for original geometry/capture and successful opaque
draws; capture failure clears admission and stops progression. Wake clocks
begin when its scene is admitted. Original camera clipping planes1/32768 are
adapted by the same.01 world scale as the eye/aim. Original skin/art remains.

Primary validation: native `make all` passed after the shared item-selector
renderer appeared (`artifacts/opening-boat-primary-all-ready.log`); scoped
opening/frontend/message tests24/24 and Python118/118 passed. Original harbor/
Voyage/callback verifiers pass. Initial full host build encountered a concurrent
item-selector missing-link dependency; after admitting its already present
source files in working CMake, full CTest179/181 passed: harbor_encounter_original
expects an obsolete battle request in the current shared field-control state,
and item_selection_runtime expects a heading width different from its current
shared implementation. These failures are outside the imported opening files
and remain recorded, not counted as a green full run. Initial native link
also preceded that concurrently created renderer and failed; rerun passed.

Actual combined heading/map reveal and complete fresh-file arrival still need
native integration/verification. Isolated latest Voyage now reaches60FPS, but
white sky/horizon appearance is being compared with the original before any
fidelity claim. Goal remains active; full-game/hardware parity is not claimed.

## 2026-10-13 — Mario animation selector at partner return

Bound original marioAnimeId800593fc to persistent Mario WORD4: effect
mask10000000 wins over rear80000000, otherwise front. Added explicit signed
M22c/230/234 handle views with original MarioInit -1 defaults. Joined ending
now records the live selector and selected handle before holding at original
MaterialGet8004069c. No positive Mario handle or material value is fabricated.
Battle cleanup preserves selector bits and handles while clearing WORD4 bit0.

Validation: make all passed; separately invoked Python suite118/118 passed.
make test initially caught a concurrent item selector signature change; a
fresh rerun built successfully and passed183/184 CTest, including both field
protocol/resource checks and trio_attack_original. Its only failure is the
new uncommitted fractional-position harbor encounter test at line32. The
committed harbor test compiled against the same current core and passed
(artifacts/harbor-encounter-committed-check.log); informed the active harbor
worker without changing that work. Logs: artifacts/trio-mario-selector-tests-
final.log, trio-mario-selector-python.log and trio-mario-selector-all.log.
Checks cover effect/rear precedence, unrelated WORD0/WORD4 bits, retained
handles and ending cleanup. Scoped diff checks passed. No new asset conversion,
native emulator capture or hardware measurement. Full working-tree suite is
not reported green while concurrent harbor coverage fails.

Next install actual field Mario resource-backed poses in the shared ownership
domain, preserve original ReInit/door material producers, then copy material/
light in original separate selector order and continue HP/init/appearance2/Hello.
Field return and full prologue remain unfinished; goal remains active.

### 2026-10-13 original sys prologue heading provider

Replaced the native static prologue card with the original US sys display
consumer: eight source glyphs/background palette, exact staggered curve5 letter
clock and subtitle strip from original `msg_stg_seq_prolouge`. Source curtain32
Z_1 and35 A_1 use their actual pose hierarchy/materials; exit19 performs the
real500ms black fade. Entry/open/start/exit producer flags now acknowledge only
successful GPU presentation followed by synchronization. Display state1 requires
real texture/pose/font/subtitle target admission; resource failure cannot advance
the handshake. The private subtitle target uses512x32 VRAM, as Citro3D rejects
linear texture memory for RenderTargetCreateFromTex. No CPU readback.

Exact validation in isolated artifacts/opening-heading-native: make test
172/172 CTest and110/110 Python; make -j8 all builds ttyd-2ds.3dsx,
py_compile and git diff --check pass. Logs artifacts/heading-tests-vram-final.log
and heading-all-vram-final.log. Original asset exporter successfully validates
Z_1 span0/one key and A_1 span60/121keys, original11 raster dimensions, descriptor
and subtitle ID. Asset hashes/conversion/license manifest remains ignored.

Isolated Azahar private direct-heading fixture, with temporary main.cpp injection
restored before final normal build: artifacts/azahar-heading-vram shows the
original closed curtain, moving PROLOGUE letters and B-triggered exit to Wake,
60FPS. artifacts/azahar-heading-held boots35s, holds input a further16s, shows
all eight glyphs, spinning original coin/background and complete rotating
A ROGUE'S WELCOME subtitle ring at60FPS; B then exits to Wake. No native
exception or heading admission failure after the VRAM fix. This fixture proves
the heading provider only; it does not prove natural full-opening integration.

Limits: source glyph projective rainbow multi-TEV passes, exact SDKFontGetWidth
subtitle cache metrics and original Stardust mode4 particle consumer remain
unbound; the source particle request is retained without a Crystal substitute.
Original-runtime/Dolphin and hardware raster parity remain unverified. See
behavior/display/NATIVE_OPENING_HEADING.md. Parent integrates the provider with
the current Voyage/Wake producers and verifies the natural transition.

## 2026-10-13 — Actual heading and combined arrival-scoped playtest

Primary now renders original sys heading background/individual glyphs,
source-staggered spins, original32/Z_1 and35/A_1 center curtains and rotating
subtitle ring using unchanged msg_stg_seq_prolouge. Actual resource admission
and completed GPU frames publish source entry/start/exit acknowledgments.
A/B/START exit after the original minimum hold; no elapsed-only display
readiness. Native subtitle cache uses VRAM; normal assets-ready now runs the
heading exporter before packing, preserving source glyph/curtain provenance.

Combined native `make all` passed (`artifacts/opening-heading-primary-all.log`);
scoped opening/frontend/message25/25 and Python118/118 pass. Full make test
183/184 CTest passes; harbor_encounter_original remains the separately changing
field/battle fixture failure recorded above. The previous item-selector
assertion is resolved by its owner. No full-green claim is made.

Immutable `artifacts/opening-heading-snapshot/ttyd-2ds.3dsx` completes the fresh
title→files→house→Voyage→actual heading→Wake→ship arrival/departure→field route
in isolated `artifacts/azahar-opening-heading-integrated/`. Original house sky
and camera display at60FPS; step350 shows the actual ship at60FPS/11.36ms,
then the final field capture60FPS/8.13ms. Isolated40-byte PM2D checkpoint has
opening marker1, known flag1 and story3; no native exception/resource-failure
notice was observed. This is actual scene admission plus checkpoint evidence,
not checkpoint alone, and not physical hardware/original-runtime/raster parity.

Still active: replace the map reveal's incomplete timing/scene adapter, bind
original heading Stardust, finish original Voyage lighting appearance, and
rerun the combined final flow/debug-short build. Projective glyph rainbow TEV,
full SDK subtitle metrics and general original rendering/sound remain limited.

## 2026-10-13 — Shared field Mario poses and partner material inheritance

Native ordinary boot now admits actual original a_mario/a_mario_r/e_mario
resource-backed CPU pose/shape records and installs three distinct Mario
handles. Existing converted characters.pmanim shares immutable bank data;
source resource identities and mutable buffers remain separate. The supported
native pool now covers three Mario plus two partner owners, so Goombella's
handle cannot alias a Mario owner. This is a scoped converted backend, not
original global pool capacity, asynchronous file/cache ABI or completed GPU
rendering. Existing original clips/atlas are reused unchanged.

Game re-entry retains actual Mario records, handles/resources/entry clocks,
clears each material and then adds light mask2, preserving other light bits.
The reset skips animation bootstrap to avoid discarded shape allocations.
Joined EntryMain performs separate live WORD4 selections for material/light,
reads each actual source pose and ORs the masks onto the actual new partner
pose. Missing owners retain the read boundary. No constant material0/light2
substitution; the native ending frontier is now partyGetHp80140ea4.

Validation: make all passed (artifacts/trio-mario-owners-all.log); separately
invoked Python suite120/120 passed (trio-mario-owners-python.log). make test
built and passed183/184 CTest (trio-mario-owners-tests.log), including portable
field protocol, original shared pose/resource/re-entry checks and full
trio_attack_original. Only the concurrent uncommitted harbor approach test
failed at line58; its earlier fractional-coordinate branch now passes.
Notified its active worker and did not modify that work. Tests cover real
three-bank identity, same-pool partner handles, retained record identity and
clock, material clear/light OR, shared immutable versus independent mutable
copy ownership, and live nonzero differing Mario masks inherited by joined
ending. Scoped diff/link checks passed. No new native capture or hardware
measurement; full working-tree suite is not claimed green.

Next source HP/pose selector and kuribo_init, import actual PKR_D_2 for low
HP, then appearance2/Hello/field draw and actor lifetime integration. Authored
join/map material producers, alternate Mario resources, RuleKeep/audience,
exit/context/sequence/result publication and original runtime/hardware parity
remain open. Full playable prologue and original-fidelity return are unfinished;
goal remains active.

Secondary heading source inspection corrected Stardust mode4 lifetimes to
20/22/24/26 (separate from owner32), signed-ADDI vertex base803a91c8, eight
original texture32 strip windows and source jump permutation/palette/scale
bytes. Recorded the used-domain findings in the heading contract without
claiming a particle consumer. Global RNG continuity and effect/display queue
ordering remain required for that consumer.

### 2026-10-13 original heading Stardust mode4 consumer

Bound the used Stardust mode4 requests to actual original effect_n64.tpl32
GPU admission/draw, original eight quad arrays/frame permutation, palette,
scale curve, lifetimes20/22/24/26, source gravity/damping and calculation order.
The common entry prologue consumes two radius rand calls before angular and
acceleration rand calls: four per particle. RadiusX=(rand%40000)/1000,
radiusY=(rand%8000)/1000; the previous fixed-radius interpretation was wrong
and has been corrected in code/evidence. Original rand recurrence uses an
explicit native presentation seed1. Existing groups tick before new display
requests at60Hz; these are documented adaptations, with no claim of original
global RNG history or source effect-manager/display queue continuity. Real
particle bank/texture admission is required before heading display readiness;
there is no substitute Crystal mask or unbound request acknowledgment.

Validation: make test172/172 CTest and111/111 Python, including original
entry/main/display/rand/data hashes and actual exported quad/scale/constants;
make -j8 all builds normal ttyd-2ds.3dsx after restoring temporary private main
fixture. Logs artifacts/heading-stardust-tests.log and heading-stardust-all-final.log.
py_compile and git diff --check pass. check_doc_links checks963 files with one
preexisting missing house-audio contract link in this isolated baseline, no new
broken heading links. Isolated Azahar artifacts/azahar-heading-stardust private
fixture captures original colored particle draws around spinning glyphs at
star-130/140, App60/58FPS, without resource failure or native exception. The
fixture uses original resources and no synthesized rendering assets; its
temporary scene-entry injection is absent from committed source. Full natural
Voyage→heading→Wake integration and original/hardware raster parity remain
parent validation work; source projective rainbow glyph TEV remains incomplete.

## 2026-10-13 — Original Goombella HP pose request and Main initialization

Joined ending now reads signed canonical party HP at14*K+6, chooses original
PKR_S_1 or PKR_D_2 at HP<=5 and publishes the borrowed source name pointer.
The used fresh constructor clears only halfwordP20, sets P8 pending2000 and
clears P1c; its repeated same-name comparison leaves the request unchanged.
Current kind1 record invokes kuribo_init, setting P33=1/P32=0, original
binary32 dimensions24/26 and clearing owned global8041eae0. No HP healing,
unlock or current-slot publication. Nonfresh/inhibited/dot domains stay
separate providers. Main is initialized; native ending now holds in partyEntry
wrapper800bea68 before scales/P38/two owner-display reads and appearance2.

Imported PKR_D_2 from actual read-only ISO a/c_pkr and a/c_pkr- into the
existing private partner PMANIM/mesh/atlas pipeline, retaining prior six clips.
Original low-HP geometry evaluates into a real nonempty shape. Resource/pose
request is not a completed animation display or native field return. Source
asset hashes and converted pack choices recorded in ASSETS; no raw assets
or packs committed. Documentation agent verified exact next wrapper ordering
and existing .display/engine-root mappings, four full-body source hashes and
original scale vector. Added scoped partner contract and inbox breadcrumb.

Validation: make assets-ready and make all passed (artifacts/trio-goombella-
init-assets.log and trio-goombella-init-all.log). make test built and passed
183/184 CTest, including portable field protocol, original animation bank and
full trio_attack_original (trio-goombella-init-tests.log). Only the concurrent
harbor approach test failed; later harbor changes do not retroactively make
this run green. Separate Python suite121/121 passed (trio-goombella-init-
python.log). Checks cover signed HP6/5/0/-1, exact pointers, halfword isolation,
marker preservation, dimensions/global clear, no index publication, real
PKR_D_2 geometry and actual joined ending. Scoped diff/link checks passed.
No new native capture, original-runtime playback or hardware measurement.

Next bind raw retained Mario display and engine root, exact wrapper scalar
prefix, appearance2 ray services, position copies, repeat HP selection and
mode, then delayed Mario245 publication and fresh Hello's second placement.
Field draw/lifetimes, authored join/map materials, RuleKeep/audience and
ending exit/sequence/result publication still need binding. Full playable
prologue/original parity remain unfinished; goal remains active.

## 2026-10-13 — Original partner placement, publication and fresh Hello

Joined ending now completes the used ordinary partyEntry and Hello path.
Wrapper copies scale1/P38=10 and two live raw Mario display reads. Appearance2
retains exact single/dual path, current versus retained headings, length56,
Y+37/down38, side retry only after horizontal hit, failure output preservation
and unconditional raw downward pointY admission. Both callers ignore success,
copy position into58..60/b8..c0, reread canonical HP and preserve unchanged
name frame markers, then mode1. Mario245 publishes only after first placement;
fresh Hello repeats placement/HP and keeps substate10, with no invented entry
jump. Current registry lookup now returns the real new partner and clears
word4 bit1. Joined/unjoined paths both reach RuleKeep8018ef8c.

Native providers read actual persistent Game display/root/current auxiliary
index and camera offset/field flags, use original heading policies with explicit
host/ARM libm arithmetic adaptation, and raycast original current map/object
triangles through the existing generic backend without wall-attribute gates.
Real hit distance reconstructs raw pointY, with no floor quantization/search
substitute. Complete original hit-record/filter/Gekko numeric/alias domains
and full field drawing/lifetimes remain separate. Existing original assets
reused; no conversions/private payload or new visual playback claim.

Validation: make test built and passed184/185 CTest including new portable
field_party_appearance and trio_attack_original (artifacts/trio-partner-
appearance-tests.log). Only concurrent harbor approach test at line58 failed.
Python121/121 passed (trio-partner-appearance-python.log). Initial make all
correctly rejected mixed auto declarations because ARM uint32_t is unsigned
long while selected literals are unsigned int; explicit uint32_t fixes this.
Final make all passed (trio-partner-appearance-all-final.log), and affected
host targets rebuilt with4/4 field protocol/real animation/placement/trio tests
passing (trio-partner-appearance-host-final.log). Scoped diff/link checks pass.
Tests cover obstruction/retry/down failure, rawY, live versus retained angles,
slot offsets, original math conventions, two placement calls, position copies,
scale/mode/substate/current index and unchanged story11/result0/control hold.
No new emulator/original-runtime/hardware capture; full suite is not claimed green.

Next bind original Rule0 prefix/status and actual audience record/clap/backend
history, then BattleEnd/resource/context/NPC/event/sequence/result teardown.
Documentation confirms slot1 applause has real producers during ending; fresh
zero initialization does not prove silence. Authored joining, shared actor
render/lifetime, general FbattlePost placement and full original prologue
performance/parity remain unfinished; goal remains active.

## 2026-10-13 — Primary original heading Stardust consumer

Primary now consumes the original used mode4 Stardust request with original
texture32, eight signed-short quads, palette/scale data, lifetimes and source
random arithmetic. The separate presentation seed1 and60Hz update ordering
are explicit port choices; original global RNG continuity is not asserted.
No Crystal substitute or unbound-particle acknowledgment is used.

Primary native make all passed (artifacts/opening-stardust-primary-all.log),
scoped opening/frontend/message25/25 and Python122/122 passed. Full CTest
182/184 passed; independently changing trio_attack_original and
harbor_encounter_original fixtures failed (full log preserved). Actual isolated
particle rendering was reviewed by its owner at58–60FPS; the final combined
map-transition/sky/particle flow still needs its updated native smoke.

## 2026-10-13 — General Rule judgment and source audience storage

Implemented portable full68-case original general Rule judge, retaining
explicit-B Mario/party query prefix, signed HP/null handling, ordered byte
sums, fresh helper thresholds, captured versus helper status write ownership,
unsigned direct turn rules/no255 bypass and source duplicate/last-byte cases.
Byte-identical end/turn zero helper algorithm is reused without merging cadence.
Native ending now writes real fresh Rule0 descriptor byte19=6 after field
restoration, then holds at AudienceSoundStop8019fd68. No story/result/sequence/
control change or teardown acknowledgment is inferred from that status.

Corrected all native audience idle/phase/turn-end/ending owners to original
inline BattleWork+2858. Removed synthetic Work+400 pointer; existing19100 RAM
covers source19098 and sound bank including slot1 B+15c78. This is real backed
layout/initial bytes, not full Audience_Init/crowd/applause playback history.
Two Clap requests remain retained; source docs reject inferring ending silence
from zero creation state. Next crowd population source audit requested.

Validation: make test187/187 CTest +122/122 Python and make all passed
(artifacts/trio-general-rule-tests-final.log and trio-general-rule-all.log).
Earlier run correctly failed old audience counter assertions after relocation;
updated source-relative flags/state/chance/counter checks, then full rerun green.
The previous harbor approach failure is now resolved by its separate work.
Original rule verifier passed8 bodies/390 anchors/2 spans/60 direct sites
(trio-general-rule-source.log); audit confirmed57 scalar/HP modes and ordered
fields match verified source contract (trio-general-rule-selector-audit.log).
Behavior checks cover prefix/query order, helper recapture and distinct outputs,
wide sums, negative HP, null Mario, ignored rules, threshold/255 and duplicate
field cases. Native trio checks actual Rule0 status and unchanged audience
phase/RNG counter behavior at original offsets. Scoped diff/link checks pass.
No new emulator/original-runtime or hardware capture; full fidelity unverified.

Next real crowd/count/clap/record/SFX backend, then sound stop, BattleEnd64-unit
resource release, map/smart/context/NPC/event restoration and exact ending
flag/counter/sequence/result publication. Authored join, live field actor draw/
lifetimes, general FbattlePost placement and full prologue parity remain open.
Goal remains active; this is not a complete playable field return.

### Opening map display correction (isolated dialogue/camera branch)

Replaced the128-render-call MapDisplay exit with aaa's used source producer:
300ms/I_1/map item96 at Luigi position+(-25,15,-3),1000ms item lifetime,
S_1/M_L_3,1500ms, then original mariost.tpl3 alpha+2 per simulation tick.
The original item icon314/image319 and source poses join ignored packs. The
interior remains resident and visible under the map alpha. Resource admission
starts this callback; an actual opaque native frame must submit and synchronize
before the native GF0 bridge admits the source1000ms tail and GSW0=2/Voyage.
No input gate was added to the map because its original source has none.
HouseReturn also binds its source actor positions/yaw and camera requests.

Bound house music through Letter/HouseReturn/MapDisplay and the source
BGMoff512/2000ms. Original group0 table volumes80/86/78 and same-group750ms
SoundSS replacement tail feed a separate Audio opening overload using existing
three fixed streaming channels. No existing Audio.cpp/effect channel owner was
replaced. Source/body/operand contracts are in story/OPENING_MAP_DISPLAY.md and
audio/OPENING_MAP_BGM.md; read-only verifier passes25 anchors.

Validation: `make test` passes172/172 CTest and110 Python checks in
`artifacts/map-test-final.log`. Native `make all -o assets-ready` passes in
`artifacts/map-native-final.log` after bounded original pose/icon repacks.
An additional full `make all` asset pass is recorded separately in
`artifacts/map-native-full.log` (completion recorded below). The isolated
`tools/emulator_smoke.py` map fixture uses its own fresh profile/save and native
binary. Reviewed `artifacts/azahar-map-reviewed/` captures show the cold-loaded
original house, item/reading stages, actual map translucency over the house,
opaque original map and Voyage handoff, with no native exception. House/map
steady screenshots show60FPS/~5.38–8.18ms; the opaque screenshot reports55FPS/
7.05ms. Voyage cold admission reports24FPS in this fixture; that old Voyage
adapter belongs to the concurrent boat correction, not this map claim.

Limits: final-frame GF0 synchronization is an explicit native presentation
adaptation; source publishes GF0 immediately before registering its final draw.
No Dolphin raster or audible DSP parity measured. Source envOff512 and geometric
mapchange49/50 remain unbound here. The later boat BGMoff512/6000ms needs an
explicit portable request from the boat event owner; this bounded producer binds
the map's2000ms off and source On replacement, not all sound SDK modes. The
fixture bootstrap was temporary and restored; no fixture source/art/ISO payload
is committed. Next: merge narrow source hunks with parent house SFX, heading and
boat owners, bind those precise transition/environment/later off services, and
review a natural integrated fresh New Game through Rogueport arrival.

Full `make all` completed successfully with refreshed original asset packs in
`artifacts/map-native-full.log`; final source includes a single opaque-boundary
synchronization rather than repeating it during the source1000ms hold. The
reviewed captures preceded that synchronization-only optimization; presentation
geometry/alpha/admission are unchanged. `git diff --check` and the25-anchor
read-only source verifier pass after final changes.


### Voyage original named light and matched Dolphin crop, 2026-10-13

The explicit original S_hai01 named-mask0x100 now selects original pointLight9
from m/muj_20/d. Its original RGB20/30/87, intensity1.5, constant distance
attenuation and signed-byte XYZ normals (/64) are consumed in the converter,
with normalized inverse-transpose normals for the portable static lighting bake.
All126 sky vertices retain original blue ratios/directional gradients. No new
art/color palette or ship enlargement was introduced. Private original named
light native capture artifacts/azahar-voyage-named-light/opening060 shows actual
Day clouds, full-width water, the original small boat and logo at60FPS/6.40ms;
white arch is removed. Source runtime light colors/TEV remain a limitation:
this native sky is darker than the matched original scene.

Actual isolated original Dolphin fresh file reached Voyage; fresh265/270/275 in
artifacts/dolphin-original-voyage-confirm establish authored tiny boat and the
world's cinematic EFB48..432 crop. Native Voyage adds its measured half-size
world scissor24..216, leaving the original full-height projection and logo draw
separate. This removes exposed bottom sun/moon and upper geometry corners;
source camera8 lifecycle/conditions remain open. MECHANICS records the actual
observation and private controller-profile correction. No user saves modified.

Validation before the crop: make test175/175CTest and113/113Python; original
named sky regression checks126vertices/colorbounds/gradient/ratios; private ISO
ship export and make all -o assets-ready pass. Native full route and post-crop
validation are recorded below once complete. This is a bounded scene correction,
not original/hardware pixel parity or completion of the larger remake.


The named-light private Azahar full fresh route completed without native
exception, reaching rendered Rogueport field; its isolated40-bytePM2D save has
opening marker1/story3/known flag1. This checkpoint is supplementary to actual
Voyage/harbor/resource screenshots and does not establish pixel parity. The
original Dolphin matched run completed through fresh345; reference helper now
writes only its generated profile's BackgroundInput=True/connected port0 so
its retained original keyboard mapping works. Post-crop make all passed
(artifacts/voyage-cinematic-crop-all.log); full tests are in
artifacts/voyage-cinematic-crop-tests.log. A distinct post-crop fresh native
capture is running under artifacts/azahar-voyage-source-crop.


Post-crop verification completed: make test175/175CTest and113/113Python,
make all -o assets-ready, py_compile and diff check pass. Isolated
artifacts/azahar-voyage-source-crop opening060 now shows original clouds/water,
small ship and logo without white arch, exposed upper corners or bottom sun/moon
silhouette, at60FPS/4.59ms. Final screenshot shows actual Rogueport field at
60FPS/5.44ms, no native exception, separate isolated40-bytePM2D checkpoint has
opening1/story3/known1. Screenshots are actual resource/scene evidence; save
alone is not success evidence. Original selected light bridge remains darker:
matched sky sample native19/29/83 versus original51/99/195. Source raw point9
color is unmodified, and opening does not invoke unused moonout alpha callback;
no invented recolor was applied. Next close the original actual visible sky
light/material/ambient consumers; hardware and exact raster parity remain open.


### Original completed Voyage light-color producer, 2026-10-13

The remaining dark sky was traced to an omitted ORIGINAL light-color animation
table, not an art substitution. Read-only original Dolphin liveMEM1 sampling
validated exact ownchild/profile, unique24MiB mapping and64ISO DOLbytes before
bounded light-bank excerpts; actual fresh095 pointLight9 is54/104/202 RGBA,
coefficient1.5, noambient. Source yoru_hiru entry+28 light-color track final250
RGB floats, mapMain's255/fctiwz/stb publication and component3 intensity agree
exactly. Converter now explicitly samples original completed light-color track
for used S_hai01, preserving first staticrawlight fallback for other consumers.
No invented exposure/palette or unused alpha callback was added. MECHANICS
records actual original conditions and verified source addresses.

Original sky regression now independently checks sourcefinalRGB54/104/202,
intensity1.5 and all126 vertices' bounded gradients/channel ratios including
blue saturation. Private exporter and make all pass; full final tests and
separate completed-light native capture are recorded once finished. Earlier
failed strictblue ratio assumed no final-light channel saturation; corrected
expectation verifies exact min(1,sourceRGB×diffuse) behavior.


Completed-light validation: make test175/175CTest +113/113Python,
make all -o assets-ready, py_compile and diff check pass. Separate isolated
artifacts/azahar-voyage-completed-light full fresh route completed without
native exception, actual Rogueport field60FPS/8.26ms, separate40-bytePM2D
opening1/story3/known1. Voyage opening060 is60FPS/4.90ms; original bright Day
sky/clouds/water/tinyship/logo now appear with source lighting and measured
cinematic world crop. Sample native sky52/100/194 and54/104/203 versus original
51/99/195 and54/105/205, supporting the recovered original light producer;
this comparison is not whole-frame/hardware pixel parity. Whitearch, exposed
upper corners and bottom moon/sun silhouettes are absent. The earlier dark
sky blocker is closed by original source data and original live state.

Remaining ownership boundary: story3 publishes after three seconds of original
departure, but this branch keeps the frontend scene until the eight-second ship
exit to preserve visible motion. Original detached event continues alongside
ordinary field control after the parent ends; exact field handoff/input timing
requires a field render owner for that remaining departure interval. The
existing harbor contract records this explicitly; no exact earlier handoff
claim or original Goombella join was made. Root integration owns this boundary.

### 2026-10-13 original Voyage sky light source correction

Read-only closure for the boat renderer owner: original sky S_hai01 white
vertex color is lit by named pointLight9 RGB20/30/87 with intensity1.5 and
no distance attenuation, not final white output. _mapLoad/lightEntry map raw
+48/+52 toE4c/E50 and integer+56 to three table coefficientsE54/E58/E5c;
raw+60 is a marker-gated flag word. Original map normals are S8/64 stride3,
not S16/16384. The first actual referenced sky normal is(0,0,-1), and source
map draw publishes inverse-transpose(cameraView*objectModel) to the GX normal
matrix. Boat owner received these findings before implementing its opt-in
named-light converter bake. No primary source or assets were modified here.

The new read-only source verifier pins four bodies, source instruction anchors,
attenuation coefficients and original map/selection/light/normal resource data.
Dolphin primary shader source supplies an explicitly attributed normalization
and lighting-equation bridge, with no original GX/hardware execution claim.
Contract: behavior/display/OPENING_VOYAGE_SKY_LIGHT.md. Native visual correction
is owned and validated by the boat agent separately.

Validation for this source-only sky increment: read-only verifier confirms
4 bodies/69 instructions, coefficient table and used original map data; make
test172/172 CTest and112/112 Python, make -j8 all builds normal native3DSX.
Logs artifacts/sky-light-tests.log, sky-light-all.log and sky-light-verification.json.
py_compile/git diff --check pass. Documentation links check964 files with the
same one preexisting missing house-audio contract in this isolated baseline.
No native renderer change or visual acceptance is claimed by this increment.

### 2026-10-13 Voyage alpha_tev producer and attachment closure

Read-only muj20 _ap section6+BSS20 is a pointer allocated as one requested byte
and initialized255 by alpha_init_func; alpha_set_func writes evtGetValue arg0
lowbyte. Complete self-relocation census has only sixBSS references in the
three producer/consumer functions and eight function/event bindings. Actual
alpha callback targets are S_umi_l_siro_b in nami_alpha (255→0/500ms), and
S_hai02 plus S_hai03_moon in moonout_effect (255→0/1500ms), reached by
onryo_event and moya_effect. Neither opening prologue initbranch nor its
evt_prologue binds/calls these functions. Sent the boat owner this negative
source result; inventing alpha0 for the opening sky would violate original
conditions. Source day rotation/culling/material ownership remains the next
visual investigation. Extended the same display contract and read-only sky
verifier with complete REL identity, nine spans and fourteen references.

Additional original ambient lifecycle recovered for the boat owner: current-bank
lightReInit zeroes every96-byte record and is called by both normal mapUnLoad
and bmapUnLoad. Standard same-bank map teardown therefore clears prior ambient
records before the15point-only muj table is loaded. Extended verifier pins this
fifth body and both original unload call instructions (71total anchors).

Validation for alpha/ambient source extension: final read-only verifier confirms
5 DOL bodies/71 instruction anchors, complete two-site light reset direct-call
census,9 REL spans and14 producer/attachment references. make test172/172 CTest
and112/112 Python; make -j8 all builds normal native3DSX. Logs artifacts/
sky-alpha-tests-final.log, sky-alpha-all.log and sky-alpha-verification-final.json.
Targeted source unittest/py_compile/git diff --check pass. No native renderer
changes or visual parity claims in this increment. Boat comparison still finds
day sky brightness different after source light9 bake; rawRGB copying is
unchanged in lightEntry, so that mismatch needs actual visible-layer/TEV/source
state investigation, not an invented exposure multiplier.

### 2026-10-13 original completed-day Voyage light animation producer

Original liveRAM from the boat agent showed pointLight9 RGB54/104/202, not its
static20/30/87. Read-only source closure found yoru_hiru animation entry+24
light pose tracks and+28 color tracks; +20 is zero in this entry. mapMain
publishes Hermite-sampled5-component104-byte color keys to runtimeRGBA via
sourcefmuls255/fctiwz/stb, intensitycomponent3→E50 and component4→E4c.
Original final250 point9 key quantizes EXACT54/104/202 alpha255,intensity1.5;
point8 finalwhite,intensity2. Source mode2 backdating permits this completed
pose. Sent exact field/format/producer mapping to the boat owner for its
parser/bake integration. This replaces the static-default explanation of day
sky brightness; no invented color palette or exposure multiplier is needed.
Extended source verifier pins complete mapMain/mapPlayAnimationLv bodies,
116 totalinstructionanchors, table layout and original point8/9 key times and
final byte/intensity publication. No primary or native rendering code changed.

Day-light source validation: verifier7DOLbodies/116instructionanchors, current-bank
resetcensus,9RELspans/14alpha refs, original point8/9 light endpoint publication
pass. make test172/172 CTest and112/112 Python; make -j8 all builds normal
3DSX. Logs artifacts/sky-day-light-tests.log, sky-day-light-all.log and
sky-day-light-verification.json. Targeted source unittest/py_compile/diffcheck
pass. Documentation check964files retains the same one preexisting missing
house-audio contract. No native visual change or parity claim in this source-only
increment; boat owner integrates and validates original completed-light sampling.

### Map49/50 source geometry and retained music tail follow-up

Bound the used original OFF_d_meku/Z_1/context2 static exit49 and A_1/context2
finite80-frame entry50. The original88-triangle geometry/sparse poses/material
kinds/order are exported read-only. A real VRAM render target captures the
opaque full map; capture completion and source final poses0/80 publish only
after actual frame submission and synchronization. Capture/pose ownership
survives MapDisplay→Voyage and retires after the entry's final presented pose.
Voyage event/ship clocks remain concurrent with50, with no invented fadeEndwait.
The used materialkind2 has no second capture: source absent-pointer branch
inherits capture0 sampler/UV after44kind1 triangles, followed by44kind2; native
bridge preserves this exact order and actual sampler rather than inventing art.

The music producer now accepts source duration6000ms, retains its existing
ordinary group through phases without a new On, and retires the native stream
on zero volume. Root merged boat owner must call `requestBgmOff(6000)` once at
its source acceptedTick+240 command33c58. `environmentOffRequested` retains
actual map envOff512 for the root house sound owner. The isolated worktree's
older Voyage fixture does not fabricate newer boat callbacks. Source verifier
now pins46 anchors, including actual model/bank/dispatch, dynamic texture owner
bodies, full used Voyage event/init hashes and original6000/17/16 operands.

Validation pending final native fixture below; earlier this follow-up's
`make test` passed172/172 CTest and110 Python checks. No ISO or asset payload
committed. Native image-plane projection is a presentation adaptation, with
original geometry/material ownership retained; no measured Dolphin raster or
DSP parity assertion. Root integration owns the later boat callsite and house
environment request consumer.

Final follow-up validation: `make test` passes172/172 CTest and110 Python checks
in `artifacts/map-turn-test-reviewed.log`; full `make all` passes in
`artifacts/map-turn-native-reviewed.log`. The read-only map verifier passes46
anchors and `git diff --check` passes. Reviewed isolated fresh-save captures in
`artifacts/azahar-map-turn-staged/` show actual retained captured map, original
corner fold/diagonal peel over Voyage and clear Voyage after original pose80.
Native capture/resource gates did not invent elapsed-only acknowledgment.
The fixture bootstrap was restored and is absent from source history.

New-world map resource staging now runs before FrameBegin/target clearing for
this used transfer. Capture00 shows the old map during this interval, but
capture03/05 still show a whole Qt/frameview black desktop snapshot, including
UI/bottom; no native exception is logged. This evidence does not prove the
cold-admission/capture artifact is eliminated. Parent integrated runtime review
must resolve whether those transient desktop captures reflect emulator/X capture
or native presentation before claiming uninterrupted hardware display. Other
reviewed frames show the correctly oriented retained-map geometry and actual
page deformation. No Dolphin raster observation or audibleDSP parity claim.

Parent merge must invoke the portable6000ms Off at boat acceptedTick+240,
consume `environmentOffRequested` in its real house/environment sound owner,
and retain the source sailing track after late acceptance rather than inventing
an opening-track On. The isolated older Voyage producer is deliberately not
substituted for the parent boat implementation. New GPU display ownership lives
in display/OPENING_MAP_TURN_CAPTURE.md, with links from the story contract.

## 2026-10-13 — Primary map turn integration checks

Original49/50 captured page fold and concurrent Voyage admission now bind in primary. Late acceptance retains sailing9 and accepted+240 requests source6000ms Off. Original50 sample869/1235 binds once after real entry admission, with documented primary-sample limits. Corrected the host display provider to acknowledge original49, rather than falsely bypassing the new capture gate. Scoped opening/frontend/message28/28 and Python124/124 passed. Native make all completed before this last small Audio binding; final combined native build/smoke remains next. Full CTest187/190 passed in the earlier run: opening host gate corrected here; changing harbor encounter/tutorial source-service fixtures still failed and require their owner validation.


### Original +3s field release with detached +8s ship owner, 2026-10-13

The known extra five-second frontend input hold is removed: Arrival parent now
ends on its actual180-tick story3 publication, Game retains the still-running
original harbor scene with an explicit field/source clock anchor, and the child
continues to480ticks independently of field input/control locks. Native renderer
frame-syncs then compacts original ship/wake groups1/13, remaps their source
vertex IDs/cached transforms and transfers only used GPU textures before freeing
whole frontend world/capture/actors. Live field worldTransform draws the original
remaining boat motion alongside Mario. Resource preparation and field restore
must succeed before successful handoff/checkpoint request; no fake save success.

Host coverage checks actual179/180 release, continuous source clock, source479/480
retirement, sparse/dense equivalence, actual Game input moving Mario while ship
continues, dialogue input lock with still-advancing child, original exact vertex
positions after compaction and rejected incomplete remap. make test176/176CTest
and113/113Python +make all -o assets-ready pass (artifacts/field-departure-final-
{tests,all}.log); diff check passes. Native fresh input-while-departure fixture is
running under artifacts/azahar-opening-field-departure; actual screenshots and
retirement are recorded after inspection. General event/field/camera/hardware
parity remains outside this bounded used-branch ownership bridge.

## 2026-10-13 — Primary arrival BGM and final validation pass

Combined primary make all and make test191/191 CTest plus124/124 Python passed (opening-final-primary-all.log / opening-final-primary-tests.log). Then restored the used fresh gor_00 init story2 On512 BGM_STG0_GOR1 at actual Arrival admission, original table67 volume70/pan64. Original init hash/conditional request/table bytes verified read-only; no invented Sailing1 restart. Final rebuild/test and combined emulator capture follow this small adjustment. Documentation1025 links checked with0 broken; git diff check clean.

## 2026-10-13 — Primary fresh arrival route and debug boot verified

Normal final scene snapshot played original Title→Files→House→letter→map→Voyage→heading/Wake→ship approach→arrival→field in artifacts/azahar-opening-final-integrated. Read-only source-root images reviewed: Mail call bubble and source house sky60FPS/5.09ms; full map60FPS/3.75ms; original completed-Day Voyage60FPS/4.95ms; harbor ship60FPS/7.32ms; final field59FPS/7.27ms. Scene transitions include real cold IO/black fades; first saved checkpoint can precede the first loaded field frame. Actual isolated40-byte checkpoint had room1, storyKnown1/story3. No source scene admission failure/native exception. Left input after field handoff and final departure retirement captured; field projection places the continuing hull outside the viewport, so these do not prove a simultaneously visible boat. Original portable+180/+480 boundary/control tests and actual compact draw diagnostic cover that ownership.

Normal make all and final make test191/191 +125/125 Python passed before the following small status-label/fixture maintenance. make debug-short passed; actual isolated debug boot opens the plaza harness60FPS/13.49ms without frontend. Documentation1026 links/0 broken. Original harbor init/table verifier now34 anchors. Additional unskipped final book smoke is running.

The status menu now uses the actual field party registry instead of claiming Goombella joined at arrival; story3 displays an arrival status instead of the later plaza harness objective. The smoke accepts the independently extended1536-byte v8 save, preserving the same room1/story3 header assertion. No existing user saves were migrated or touched. Final UI rebuild and updated fresh smoke follow.

## 2026-10-13 — Final unskipped smoke exposed backdrop reload regression

Final combined unskipped book smoke found a real resource-lifetime regression after the newly imported sky/backdrop: final book cleanup erased all frontTextures, including opening-bg-dmo_00. The marketplace then loaded this original backdrop again on every frame. Actual captures market-arrival10FPS/2.57ms and peach-turn0FPS/2.21ms showed frame IO churn despite cheap draw work. This is not accepted as a passing performance run.

Corrected cleanup to find and retire only demo-prefixed book images, preserving the market backdrop and its single admission. GPU synchronization remains before freeing queued book pointers. Native rebuild and a new unskipped run are required to close this correction. No original art, pose, story clock or message IDs were dropped.


Field bridge private native validation completed: actual ordinary field appears
at opening116, Circle Pad Left moves Mario/camera while detached child remains
active, retirement/finalfield returns60FPS without exception. First cold-load
rollingmetric was9FPS despite7.79ms latestframe; Leftcapture53FPS/11.26ms,
continuing60FPS/7.58ms and retired60FPS/5.88ms. These are exact private branch
observations, not steady cold-transition performance claims; primary lazy field
resource handling differs. Isolated files under artifacts/azahar-opening-field-
departure include field-release-with-ship/field-left-with-ship names from the
fixture, but actual hull is outside the field camera; names are not visualproof.

Separate compile-time diagnostic profile azahar-opening-field-departure-
diagnostic writes only its generated SDMC opening-departure-trace.csv. Actual
field tick0 anchors source departure180/X-435; tick30→210/X-517.5; tick60→240/
X-600; tick270→450/X-1177.5. Opaque submissions11190vertices/12batches plus
translucent576vertices/2batches use11 transferredoriginaltextures, preserving
11766originalvertices rather than68766wholeworldvertices. Actualfieldclip-Y at
tick30 is1.909..4.164, entirelyoutside[-1,1], explaining absent hull without a
hidden ownership bug. LiveLeft shiftscameraX0→-1.208 while child clocks/movement
continue. Wake source384vertices spans eightnativeunits and stays at original
fixedX-475; it can remain visible until original480 retirement. No group1/13
mesh animation bindings overwrite its event placement. No fabricated geometry
or camera reveal was added. Source owner/admission/control-order boundary is
closed for the used branch; fullfield/camera/SDK/hardware parity remains open.

## 2026-10-13 — Real audience sound record history and stop

Added portable Clap/Stop/SetVolAll and live right/left crowd filters against
actual inline audience/sound records. Counts use200 current getters with
source existence/state exclusions; original four tier keys only apply to
positive count. Clap always requests backend, stores actual failure-1 without
installing voice/lifetime/volume, and admits real handles only through providers.
Stop reloads flags after backend and clears only1; volume setup touches all24
records with raw target/duration and no premature stop.

Native ending now executes both Clap requests, actual SetVolAll(0,30) and
Stop(1). Unassigned-key backend requests explicitly fail-1 and retain absent
key/count/result diagnostics; no original stack bytes, C string or playing
voice is invented. This is best-effort native resource failure, not measured
original silence/lookup. Positive known keys/active voices keep unsupported
real playback/stop providers. Original population/RNG/zero-weight divide and
SoundMain/Noise2/NDSP remain open; newly delivered source contract distinguishes
fresh pouch8c0 desired count0 from universal successful zero population.
Joined/unjoined ending now reaches upper BattleMain800f8e28, keeping story11,
field result0/control hold/fade rather than immediate field return.

Validation: make test192/192 CTest +125/125 Python and make all passed
(artifacts/trio-audience-sound-tests-final.log and trio-audience-sound-all.log).
Initial shared-build run hit unrelated field_pipe_tests link with missingmain;
inspection confirmed unchanged source and current object exportedmain, and
full rerun passed. Logical owned-voice fixture tests cover tier boundaries,
right/left filtering, count0 request, failed/unavailable provider, flag reload
and all24 volume records. Actual native battle checks two failed unassigned
requests, slot1 handleffffffff and retained30 duration. Scoped diff/link checks
pass. No emulator/audio capture, original stack behavior or hardware assertion.
No new assets/conversion/private payload.

Next original upper-engine remaining services, lower-wrapper80000000 flag,
sequence>30 and BattleEnd/map/smart/context/NPC/event/field-wait teardown.
Actual DSP/crowd/render/lifetime and authored joining remain required for
original parity; full playable prologue goal stays active and unfinished.

## 2026-10-13 — Book backdrop fix verified; original atlas aliases retained

Corrected unskipped run artifacts/azahar-opening-book-backdrop-fixed completed without admission/native exceptions. Actual market105/110/120 and merchant128 frames are60FPS, respectively10.41/10.54/9.36/10.82ms; final demo input guard60FPS/6.76ms. A enters the original white exit at60FPS/7.31ms. That1-second image is an exit fade, not a fully opened title; the fixture now waits16seconds for the title, as the separate fresh flow already does. The repeated-backdrop0FPS regression is closed by actual native evidence. Cold first resource loading is still synchronous; emulator results do not establish hardware performance.

Current fresh route also succeeded with version8/1536-byte checkpoint, room1/storyKnown1/story3, actual no-partner field menu and input. Afterward a concurrent Mario pose addition exposed65 logical texture cells against the64-cell native limit. Rather than drop original art/poses, the main atlas now opts into exact converted RGBA raster sharing. Original read-only inventory found57 distinct rasters among65 logical entries; all source aliases survive. Other banks default to their previous mapping, preserving fixed curtain descriptors. Two regression checks cover65 aliases with every triangle/raster retained, and rejection of65 distinct rasters. Final coherent pack/build/test validation follows.

## 2026-10-13 — Ordered engine completion and delayed field return branches

Implemented portable original eleven-service upper Main, independently
captured lower wrapper/end flag and actual end polling. Completion is checked
after every required service; unavailable native providers produce no engine
result. Original GP gate uses a fresh second bank read, flagsOR60 and exact
selector0/7 writes. Null/later changed owners retain source wrapper semantics.

New native pass exposed a previous ending1f bug: literal B0=4 changed the
header and left bank0 at3. Read-only source confirms BattleSetSeq(B,0,4), now
writing B+4 and preserving both header halfwords. Next native tick runs broken
unit, four general pads with fresh owners, real bank4 default dispatch and
private pad/action prefix, then holds actual StageObjectMain80149798. Native
unused channels are explicit inactive input projections. The end flag, field
result0, story11/control hold and black fade remain while providers are absent.

Portable battle sequence5/6 now preserves readiness-only GP+c override,
state5-only camera/listener tail and signed wrapping counter>30, including
alternate(5,1,0). Incoming field state0 fade preserves full32-bit source
presets, special event-before-fresh-clear order, separately recaptured GP
preset stores and fresh state increment without fallthrough. These branches
are not yet a native whole sequence/exit owner; no invented immediate return.

Validation: make test195/195 CTest +127/127 Python and make all passed
(artifacts/battle-main-return-tests-final.log and battle-main-return-all.log).
Original Main source verifier passed20 body fingerprints/117 anchors,1data
span and2direct callers (battle-main-return-source-audit.json/log).
Scoped diff checks passed. Initial new native test caught its
one-pass boundary expectation, then revealed the genuine wrong bank write;
both corrected. No new assets, emulator/original execution/audio/hardware
assertion. New documentation closes local ordered gates and identifies real
seven-service owners; audience/spotlight/draw/lifecycle remain explicit.

Next bind original five selected stage objects/transforms, icon/help/slot
source work, stage spotlight/audience window/sound/display providers, then
BattleEnd/map/context/NPC/event restoration and the native sequence owner.
Full playable prologue goal remains active and unfinished.

## 2026-10-13 — Opening frontend goal completed through Rogueport arrival

Completed the requested scoped route: original unskipped storybook/Peach opening, Title, four-file select, fresh house/mail scene, original letter, map reveal/49→50 turn, Voyage/logo, original prologue heading/Stardust, wake-up, harbor approach/paper/jump/departure, and story3 arrival checkpoint. New-game character bubbles require input. Source mail camera/glide and backdrops are bound; completed original daylight light animation replaces the dark/white sky. Book cleanup preserves the market backdrop, eliminating the observed per-frame reload. Control releases at source departure+180; original child continues to+480 through the compact field owner. No partner join or completed Crump tutorial is claimed.

Final exact validation: make all passed with the coherent pack (artifacts/opening-final-coherent-pack-all.log); make test193/193 CTest and127/127 Python passed (artifacts/opening-completion-tests.log); make debug-short passed (artifacts/opening-completion-debug-short.log). Current main atlas retains62 source texture aliases in54 byte-distinct native cells; original poses are retained. Read-only map verifier46 and harbor verifier34 anchors passed. Documentation1032 Markdown files/0 broken links and git diff check passed. Intermediate concurrent pose capacity failures and the earlier real backdrop regression are recorded above, with their corrections.

Final immutable scene/asset build artifacts/opening-completion-snapshot/ttyd-2ds.3dsx passed the isolated fresh Title→Files→opening→arrival route (artifacts/azahar-opening-completion, opening-completion-smoke.log), with actual1536-byte version8 checkpoint room1/storyKnown1/story3. Native final harbor frame59FPS/7.16ms, no scene admission failure or native exception. The no-partner arrival label was reviewed in the preceding version8 run. The unskipped fixed-book run completed through its source input guard: sampled market/Peach/merchant frames60FPS and final guard60FPS; original A white exit was captured. Complete opened Title was reviewed separately in the fresh route; no claim that its1-second white-exit image already shows Title. Earlier isolated8-second character hold/A resume and actual boat approach/departure captures remain valid. The optional draw diagnostic proves ship/wake submissions and continued clock even when the hull leaves the field frustum. User emulator profile/saves and ISO were untouched.

The opening goal is complete at the user-selected arrival endpoint. The full remake remains unfinished. Next tasks: physical New2DS XL performance/audio playtest; remaining full original TEV/shadow/cinematic raster and MusyX/ENV/global RNG parity; then separately complete Crump tutorial/battle integration and authored Goombella joining before extending story progression. Cold scene loading remains synchronous and full field/battle UI/services remain under separate development. Existing-save migration was not part of this goal.

## 2026-10-13 — Original named stage-object update and geometry ownership

Implemented complete portable StageObjectMain:32 slots, signed ordinal gate,
ordered fourvector raw cache writes, forcebit2 clear, shared layer2 orientation
cache, two-step floattranslation, layer-specific rotations/Ypivots, fresh
descriptor/name reads and independent mask1 visibility. Missing providers
retain actual early cache writes and produce explicit suspension. Source Main
uses no scale or animationplayer request; these remain upstream owners.

Imported original named geometry bindings via existing converter:3393 selected
stage vertices partitioned exactly456/360/1488/1083/6 for isi_1,isi_2,A1,B_mati,C.
Retains authored B/C transforms and current placement bake. Original hidden
escape_wall/spotlight owners exported as zero draw-vertex groups; spotlight
now explicitly excluded according to source Init. No private payload committed.
Source contract closes cache999, real rank0 builtin record6/7 (not fabricated
curtain), offsetY200 override and right-side additional-matrix/latch rules.

Validation: make test196/196 CTest +129/129 Python and make all passed
(artifacts/stage-object-tests.log and stage-object-all.log). Scoped diff
checks passed; both new private metadata paths confirmed ignored. Asset command python3
tools/trio_battle_assets.py passed; ignored PMHOBJ hashes/paths/licensing in
docs/ASSETS.md. Tests cover entire Main graph/cache/provider ordering and
original metadata exact vertex coverage/reconstruction/authored hierarchy.
No emulator/GPU/original execution/hardware assertion; native ending still
holds at StageObjectMain pending actual map/renderer providers.

Next consume named bindings with real additional matrices and native vertex
uploads, source frame-latch/recalculation, actual seven-record initializer and
upstream writers, then remaining ordered engine/teardown/sequence services.
Full playable prologue goal remains active and unfinished.

## 2026-10-13 — Native original stage-map objects and live GPU buffers

Exported original complete stage/background map-node hierarchies124/108,
authoredlocal matrices and vertex-local bindings with placement bake removed
exactly once. Source sidecar retains actual stage and seven descriptor
addresses. Native rank0 builds five selected+two hidden records from original
readonly definitions/cache999, retains escapeY200, source root/group flags,
real null script gates and ironframe hiding. Pouch rank is signedhalf88; level
is the separatehalf8a. Typed map owner replaces address-based MapWork/index
projection, without claiming whole original constructor/loader/ABI semantics.

Source translate/rotate reset once perflag200 series, right-concat source
T/Rz/Ry/Rx/pivots; explicit simulation map-frame recalculation preserves
authored hierarchy, clears only200 and retains200000. Native 3DS renderer
uses this same owner to update Room linear vertices/cache only on revisions
and skips named hidden draw ranges through actual material batches. Cached
visibility runs change only with mask1 flags, removing repeated pervertex
scans from every drawn batch. Model
.1/engine.01 coordinate bridge reproduces every original3393 stage vertex.
Portable math is an explicit numerical adapter; original paired-single/
GX/map-animation/lighting/outer-cadence/hardware parity remains open.

Native attack ticks now execute actual StageObjectMain; ending's ordered
engine pass completes stage objects, checks16 live icon records and reaches
StageMain's mandatory spotlight8013800c. Seven records/hidden names/source
request order tested, wrapperendbit remains0, fieldresult0/story11/control
hold retained. Real spotlight work/bank/init/display/free documentation now
delivered for next implementation; no forced engine/field success.

Validation: make test204/204 CTest +131/131 Python and make all passed
(artifacts/native-stage-map-tests-final.log and native-stage-map-all.log).
Isolated DEBUG_SHORT_BOOT build using build-native-stage-map-debug and target
ttyd-native-stage-map-debug passed (native-stage-map-debug-diagnostic.log).
Immutable artifacts/native-stage-map-loader2-snapshot binary completed actual
trio attacks through EXP10/victory/black fade in azahar-native-stage-map-final,
reviewed target/victory30/ending captures. Target29FPS/3.47ms, EXP30FPS/3.61ms,
black60FPS/3.91ms are sampled Azahar values, not hardware or a controlled
performance comparison. No native exception observed; fixture is legacyunjoined.
Prior valid precache capture also reached the same actual arena/end boundary.

The first normal-frontend capture consumed input during storybook and did not
reachbattle; it is not battle evidence. A later cached snapshot returned to
Azahar launcher beforegameplay; its capture is explicitly rejected. Startup
now names the rejected asset loader via console/native debug only onfailure
while preserving original load order/short-circuit admission. Latest diagnostic
image boots and completesroute withno rejectedloader; earlierstartup failure
was not reproduced, its underlying cause is not claimed resolved by logging.
Shared debug-short target hit absentELF duringnm whileanotherbuild produced
that samefile; a private build/target completed and supplies immutable evidence.
Scoped diff/link checks pass; no emulatoruserprofile/save or ISO changes.

Initial test found rank word
read included adjacent level, fixed to sourcehalf. Later fullCTests passed but
Python asset tests exhausted shared /tmp; rerun uses project-local TMPDIR
without removing any foreign files. New original assets rebuilt read-only
with python3 tools/trio_battle_assets.py; private paths/hashes/licensing in
docs/ASSETS.md. No full prologue or hardware completion claim.

Next actual spotlight allocation/work and stage idle/tween domains, then
audience window/sound/slot/display, BattleEnd/map/context/NPC/event release
and native delayed sequence/field return. Goal remains active and unfinished.

## 2026-10-13 — Original stage-work storage and ordered cleanup/counters

Consumed the recovered complete StageInit and inactive/counter StageMain
contract from the documentation inbox. Native now initializes the full b3c
work storage, twelve entry/scratch fields, distinct active ambient/background
records and original six float fields from unchanged imported DOL constants.
Replaces the floor-only projection and hardcoded escape-wall Y overwrite.
Named StageObjectInit follows this storage initialization; spotlight Init
remains pending, so complete original child initialization is not claimed.

Portable Main retains the captured owner across spotlight/entry callbacks,
requires spotlight first, checks all twelve real flags, and frees inactive
nonnull scratch before clearing its field. Native free binds the real map heap.
Four counters preserve signed admission, decrement-before-interpolation,
wrapping elapsed, unsigned endpoints, signed mode, fresh duration/remaining
reads and low-byte fctiwz-provider output. Active entries and positive numeric
curves remain explicit services. Native still holds at spotlight8013800c;
no field result/control/end flag is forced to bypass remaining engine work.

Validation: make test207/207 CTest and131/131 Python passed
(artifacts/native-stage-work-tests.log); make all passed and produced the3DSX
(artifacts/native-stage-work-all.log). New tests cover prior nonzero storage,
write bounds/defaults, spotlight-first admission, last active record,
free-before-clear and owner mutation, failed providers, counter order/modes,
negative sentinels/wrapping elapsed and byte conversion. Native trio tests
inspect original mapped ambient/background/position fields and retain ending
hold assertions. Initial test compilation exposed a missing static qualifier
on a fixture array used by a callback; corrected before the successful rerun.
Documentation1055 files/0 broken links and git diff check passed. No new emulator
capture, physical hardware or performance assertion; previous native route
coverage is recorded separately above. Private assets and ISO remain unchanged.

Next bind persistent spotlight Work/bank allocation, original Init/Main/Free
and actual display admission, then remaining engine/teardown/context services
and delayed native field return. Full playable prologue goal remains active.

## 2026-10-13 — Real spotlight allocation and native ending stage pass

Implemented original spotlight Entry/Init/Main/Free with source pointer/table,
real mapheap dc allocation, repeated owner reads, cleared five-record bank,
original raw default/override writes, single FMA numerical adapter and preserved
flags/gate/free fields. Source SBSS2 colour804295d4 is mutable boot0, established
by original __init_data zero-table evidence; Entry copies0 then Init overrides
white from original immutable literal. No invented texture/ISO colour asset.
Native maps actual12-byte separate SBSS work and colour, verifies source owner
pointer, and publishes session history after Init/Main/destruction. Per-Game
history survives room/checkpoint restore; separate Games boot independently.
This is live-memory/session ownership, not a new saved-checkpoint field.

StageWork storage→StageObjectInit→spotlight Init now follows source child order.
Fresh ending completes actual spotlight Main, twelve inactive StageWork entries
and four zero counters. Bound AcHelp Main's real key/mode/Bef4 predicate and
original active display request; ending now reaches AudienceMain801a836c.
Positive spotlight/help gates remain actual display dependencies; no forced
engine end bit, field result/control release or story advance. Spotlight stays
allocated while EndingWork frees; native heap tests now inspect this actual
allocation history. StageEnd/BattleEnd/active cones remain unbound.

Documentation agent delivered complete AudienceMain ordering/inline owners,
window/throw side effects and corrected prior misnamed801a1cdc to WinSetActive
(actual low8 window-active write), with true NumToTargetSub801a1bf4 distinct.
Corrected its owned AcHelp contract before commit; this step binds no setter
or fabricated crowd-count callback. Source AudienceInit/live actor/resource
history requested next; do not infer empty original crowd from unbound init.

Validation: make test209/209 CTest and132/132 Python passed
(artifacts/native-spotlight-tests-final.log), make all passed
(native-spotlight-all.log), isolated DEBUG_SHORT_BOOT target passed
(native-spotlight-debug.log). First test run exposed old trace-front expectation
before the newly real initialization allocation; assertion now checks that
allocation then original attack callback, and the complete rerun passed.
Tests cover original records/zero-to-white order, first-free/flags, callback
owner recapture, actual allocator/free/coalescing, retained gate/history,
display requests, AcHelp signed nonzero admission and native joined/unjoined
ordered ending/held field state. Scoped diff and documentation link checks pass
(native-spotlight-doc-links-final.log;1060 Markdown files/0 broken links).

Immutable artifacts/native-spotlight-snapshot completed isolated Azahar actual
trio attacks→10EXP/victory→black fade/held-restoration notice
(artifacts/azahar-native-spotlight; native-spotlight-smoke.log), terminal0 and
reviewed last target/victory30/final images. Sampled target20FPS/2.87ms,
EXP30FPS/3.86ms, black60FPS/3.94ms are Azahar samples, not physical hardware or
a controlled performance comparison. No native exception observed. Legacy
unjoined fixture; joined branch verified by host tests. User emulator profile/
saves, ISO and original private asset conversions were unchanged.

Next implement original AudienceInit/live records/resource handles, full ordered
AudienceMain window/sound/slot/display services, then BattleEnd/StageEnd real
frees, map/context/NPC/event release and delayed native field return. Full
playable original-fidelity prologue goal remains active and unfinished.


Follow-up (same 2026-10-13 request session): filed CONTENT-01 (reachable
chapter/interlude/minigame/optional-content census -- the still-uncovered G1
register row) committed 669924e. Verified two more deliveries against the US
ISO: DEMO-01's GSW[0] gate (aaa_00_init_evt cmd19 op24 GSW0==402; evt_prologue2
cmd49 and epilogue_evt2 cmd117 write GSW0 just before evt_bero_mapchange) commit
13397dc; RESULT-01's Escape route (btlevtcmd_InterruptStop 0x80105368 OR10/OR20
at B+0xef4 with fresh B=*(r13+0x1c70)=0x8041eb90; lone InterruptStop(0) USER_FUNC
at 0x8034f1d4) commit efa1783. New commits this follow-up: 669924e, efa1783,
13397dc. Docs-only; no make rerun; artifacts in /tmp. All own commits are
ancestors of HEAD; docs-agent worktree edits not swept.

## 2026-10-13 — Actual audience startup, original TPL owners and target math

Implemented original setup4 parameter/range requests and complete used mode0,
zero-capacity population. Corrected prior type10 inference: admission counts
only nonzero weights, so type10 always enters and the divisor stays positive.
Preserves source tables, desired perturbation,107 draws,300 irand0 requests
without advancement, fixed-weight/index-only swaps and12 published weights.
Proven zero allocations/no Entry retain original cleared actor bytes; positive
capacity/overrides/other modes remain real population providers.

Native follows StageInit→AudienceInit and retains actual inline work, normal/
Luigi TPL resources, guest metadata, throw-max draw, live counts/window amount,
noise attempt/volume ramp and source NumToTarget gate. All205 normal+19 Luigi
CMPR64x64 images imported unchanged into validated typed payload owners; active
GPU crowd conversion/drawing and original file/cache ABI remain open. Native
noise preserves explicit unassigned-key failed request(-1), before ending claps.
Shared trio LCG/irand boot1 state survives restore/runtime recreation; original
field/opening/development RNG interleaving and save ownership remain partial.

True baseline8 exposed AddTargetNumSub801a18cc. Implemented full finite signed
chunks/ratio/rounding, cancellation and source clamps, preserving nonzero work
before capacity0 clamp. Exceptional conversions remain numeric boundaries.
Actual ending reaches AudienceMain801a836c; no field result/control/end flag or
story progression forced. Full Main/window/slot/audio/display and teardown are
still unfinished. Received next live sound/window/throw/display contract.

Validation: make test211/211 CTest+134/134 Python passed
(artifacts/audience-init-tests-final4.log). Covers actual tables, eighty RNG
seeds, exact requests/draws, actor bytes, golden original resource hashes,
noise/volume/window history, positive-domain rejection, persistent RNG,
weighted/unweighted target math and native ending/guard/superguard. Recovered
startup RNG changed enemy victims: checks now follow actual target/damage and
observe guard codes across both player units. Nonzero ending hold was resolved
with the original helper. A concurrent angle-table refresh caused an earlier
read failure; publication is now atomic with original bytes unchanged.

make all passed (audience-init-all-final.log). Later shared debug builds hit
in-progress foreign helper linkage, then duplicate Renderer declarations and
GPU_DEPTHFUNC in platform.hpp. These hunks are excluded from this change.
Coherent private source baseline971156684f plus audience work rebuilt all native
sources with prebuilt original packs (audience-init-isolated-native.log).
Immutable artifacts/audience-init-native-snapshot completed isolated legacy
story11 attacks→EXP10/victory→black fade/held notice, terminal0; reviewed target,
victory30 and final captures (artifacts/azahar-audience-init,
audience-init-smoke.log), no exception. Samples20FPS/3.11ms target,30FPS/4.10ms
EXP,60FPS/6.22ms black are Azahar samples, not hardware/controlled performance
parity. Joined path verified by host tests. Shared renderer integration requires
its own completion; the validated snapshot excludes uncommitted foreign changes.
Documentation links/scoped diff pass (audience-init-doc-links-repaired.log).
Asset paths/conversion/licensing in ASSETS; no original payloads/ISO committed.
User emulator profile/saves and ISO remain unchanged.

Next bind actual retained public40slot SFX ownership, full ordered AudienceMain
and remaining engine services, then StageEnd/BattleEnd/map/context/NPC/event
teardown and delayed native field return. Full playable prologue goal is active.

## 2026-10-14 — Behavior-preserving rendering optimization repair

Replaced the attempted working-tree GPU cache with a pass-local map cache:
first-draw state establishment, texture-specific wrap handling and bind after
wrap mutation. Preserved original batch order/depth/cull/alpha and restored
all world TEV initialization across Citro2D/System/capture transitions. Removed
unused/incomplete renderer-wide cache helpers and per-vertex dirty tracking;
world geometry already flushes only the regenerated vertex prefix. Simulation
cadence, action-command ticks, story/dialogue and resource sync remain intact.

Added fixed-workspace upload-run accumulation for house/ship geometry, merging
actual callback-written ranges with a safe encompassing fallback. Original
pack tests compare complete resulting vertex-buffer bytes against full uploads
across moved/restored poses: house 248,832/1,139,616 bytes in 5 ranges,
harbor 423,576/2,475,576 in 3, voyage 423,576/808,704 in 2 (about 78/83/48% reductions).
Actor texture rotation trig now computes once per evaluated surface after
shifts, retaining point arithmetic order. Bitwise UV/complete clipped geometry
and color tests cover forward/repeated/backward animated shifts. Mixed map
state replay checks 10,000 draws against unconditional setup. Static all-visible
plaza 150 batches retain draw count but state calls become texture 129/cull 26/
depth 2; these are CPU call counts, not measured hardware performance.

Exact validation: TMPDIR="$PWD/artifacts/optimization-repair/tmp" make test
passed 214/214 CTest + 134/134 Python (test-validated.log); make all passed
(all-final.log); focused rendering tests 4/4 passed (scoped-tests.log).
Original-upload byte counts are in upload-work.log. Logs live under
artifacts/optimization-repair/. A full rerun initially caught a concurrently
edited battle test missing <vector>; its owner corrected it before the passing
rerun. No unrelated battle implementation was changed by this repair.

Immutable snapshot SHA256 d0a1d8e8475ccb3a36b14ad773d956de88c1b93f99edd7b252846e8bfc8bc9e7
completed isolated fresh opening→arrival using tools/emulator_smoke.py and
tests/azahar_opening_flow.py (smoke.log), expected 1536-byte room 1/storyKnown 1/
story 3 checkpoint. Reviewed actual house/interior and final harbor captures in
artifacts/azahar-optimization-repair; early screenshot filenames are fixture
timing labels, not guaranteed scene identities. No native exception detected.
Initial Xvfb sandbox socket failure required an isolated display-access rerun;
user emulator profile/saves and ISO were untouched. No asset extraction or
conversion, private asset commit, original execution, hardware timing or
pixel-perfect GX parity claim. Existing asset provenance remains in ASSETS.md.

Scoped commits 2510625 and 32f3a9b. A concurrent staged documentation edit was
removed from the first commit by amendment; subsequent commits used separate
indices. Foreign working-tree/staged changes remain with their owners.
Full analysis and follow-up paths: [rendering repair](optimization/BEHAVIOR_PRESERVING_REPAIR.md).
Next measure whole-loop percentile frame times on New 2DS XL, then ordinary
save-worker integration, preload first-use house PCM, transactional music
admission outside the refill mutex, conservative dynamic-aware batch cutaway
bounds and complete-input actor sample caches. Do not add a second VBlank wait
or reorder translucent draws. Full remake remains unfinished.

## 2026-10-14 — Opening feedback: facing, automatic Title and PCM continuity

Corrected Peach/merchant presentation to use the recovered NPC camera-relative
matrix, original angle lookup, retained rotation and side-dependent Z reflection
once per simulation tick. The former binary yaw transform selected the wrong
paper side. Kept authored actor poses, scene requests and all37 original US
message IDs/control tokens. Original input guard remains documented as source
evidence; the user's requested automatic chest→Title behavior now starts after
all final requests/waits and still requires the completed opaque GPU frame.
A/START continues to skip earlier. The completed-demo button prompt is removed.

Native PCM refill now runs on a separate worker; NDSP's callback only wakes it.
Track admission/retirement and refill share a lock, initial queued buffers use
published envelope gain, and shutdown joins before releasing stream owners.
Three4096-frame buffers per owner are native choices, not original measurements.
Chest effect textures admit with the market instead of at first chest draw.
Title images/curtains admit before the frontend clock/music start, survive the
Demo/Title/Files phases and final book adjustment, and retire for New Game.
The initial linear-memory preload failed Peach admission (1285KB free,1–2FPS
retry); final Title textures use GPU VRAM, preserving the overlap's linear arena.
No original pose or asset was dropped. Native fidelity/hardware timing remain
bounded adapters rather than full original-runtime parity.

Audio-active isolated recording also found the optional Switch book cue ends
before the original US scene's Off request:2.262s digital silence. Book track5
now uses the original ISO's looping sys_demo1_32k even when that optional
soundtrack is selected; no fabricated Switch loop points. Final242.0467s PCM
capture has only11.2595s initial boot silence and3.689s after emulator shutdown;
no >50ms digital silence inside active playback. This detects gaps, not pitch,
stretching, musical/perceptual fidelity or physical2DS output.

Validation: frozen source base52bfd21 plus the opening VRAM/book-policy patches,
with private copies of the existing RomFS packs (no hardlinks), passes native
`make -C artifacts/opening-feedback-build -o assets-ready all` and the same
command with DEBUG_SHORT_BOOT=1 BUILD=build-debug-short
TARGET=ttyd-2ds-debug-short. The -o option reuses the already converted private
packs without re-extraction. `make -C artifacts/opening-feedback-build test`
passes209/209 CTest and133/133 Python (opening-feedback-verified-tests.log).
Final debug log:opening-feedback-verified-debug-short.log; normal log:
opening-feedback-verified-all.log. The immutable normal/debug3DSX and exact
source/artifact hashes live in artifacts/opening-feedback-build with
opening-feedback-manifest.json. No claim that this older coherent battle
baseline incorporates every concurrent battle/render change.

Shared-checkout checks were run first: final initial make test210/211 CTest,
with an unrelated trio_attack_original defended-reaction assertion; standalone
Python134/134 passed. Earlier native builds encountered concurrent added-source
link misses and a renderer header mid-edit. A consistent snapshot avoided
accepting a stale binary. Scoped opening/frontend/message/NPC30/30 passed.
Snapshot's first Python run needed the ignored message-icon/item-effect source
fixtures copied; subsequent complete runs pass. All are retained in
artifacts/opening-feedback-*.log. Final doc link check1104 Markdown files/0
broken links, and scoped git diff checks pass.

Final native artifacts/azahar-opening-feedback-verified unskipped run shows
book/market60FPS, source merchant/Peach poses, chest effects, automatic white
exit and opened real Title60FPS, with no end-of-demo input. Final t206 effects
sample36FPS/6.29ms (other late samples60FPS), so no claim of an entirely hitch-free
sequence. The failed linear-preload run and OCR fixture failure are retained;
missing local Tesseract language data stopped that harness after its real
no-input Title capture. Final harness uses captured timing instead. Its2s
post-A capture was still the authored curtain; the separate corrected6s wait
artifacts/azahar-opening-feedback-files reaches actual four-file selection60FPS.
No native exception in the successful runs. DSP firmware was copied read-only
into isolated test profiles; a temporary audio sink recorded only test output.
User emulator profile/saves and ISO were not modified.

Remaining: physical New2DS XL audio/performance; profile the transient chest
sample and other cold scene admissions; complete original renderer/TEV/shadow
and sound-engine parity. This correction does not revalidate the entire fresh
New Game route or extend story beyond the previously tested story3 Rogueport
arrival. Crump/joining/full-game progression remain separate work. The opening
flow still supports normal boot and compile-time short debug boot.

### 2026-10-14 — Audience window/sound pass and original public sound registry

Completed the portable original WinCtrlProcess801a4570: captured audience and
independent window owner, signed count follower with post-store reload/clamp,
wrapping timer with asymmetric source bounds, and single-rounded mode4
interpolation using original constants. Completed SoundMain8019f97c record
pass behind an explicit NoiseMain provider:24 checks regardless of flags,
source bit1 clearing, stop ordering, ramp then independent lifetime fade,
shared interpolation fields and actual positive-only lifetime decrement.

Added typed40-slot PositionalSfxRegistry with boot/public-init state, retained
generation on reinit, actual Chk acceptance, generation-independent volume/
distance/Off, truncation and descriptor-selected backend stop. Out-of-bank
handles and unavailable backends remain explicit provider boundaries. This
is not original On/Main provisioning or NDSP voice equivalence. Contracts
and next slot/display source evidence are indexed under docs/behavior; see
[binding limits](behavior/battle/AUDIENCE_WINDOW_SOUND_PASS_AND_PUBLIC_REGISTRY_PORT.md).

Validation: final make test216/216 CTest and134/134 Python pass
(artifacts/audience-live-tests-final.log), including new audience-live and
registry suites. Earlier full run also passed before additional boundary
tests. make all passes (artifacts/audience-live-all.log), building native
modules and linking ttyd-2ds.3dsx. Doc checker1109 Markdown files/0 broken
links (artifacts/audience-live-doc-links.log); scoped whitespace check passes.
Concurrent unrelated source changes were present in the shared full builds;
this scoped commit includes only this feature and its documentation. No
emulator run needed for these still-unbound mechanics; no rendering/audio
or physical2DS correctness/performance claim. Original ISO read-only, private
asset packs remain ignored; user emulator profiles/saves untouched.

Remaining/next: bind the persistent public registry in the actual audio
lifetime and source sound provisioning; implement Ctrl/throw, Item/AP and
NoiseMain and integrate complete AudienceMain. Ending still holds at801a836c;
then real SlotInit/idleMain and surviving-unit display providers are required.
No battle result, field return or prologue completion is asserted. Goal active.

### 2026-10-14 — AudienceMain native integration and retained sound ownership

Bound AudienceMain801a836c in the real ordered engine completion pass. Source
B/A captures, conditional window activation, five service order and fresh
final flag publication are preserved. Ctrl now performs real right/left
recounts, original flag10 scalar motion/ground clamp, generic AI admission
and throw gates/RNG/shuffle/ItemOn rejection prefix. Inactive item/AP gates
use actual live fields; active AI, item/AP and positive ItemOn continuations
remain their providers. NoiseMain uses source L-then-R live counting and
category/transition/amplitude state. SoundMain now runs against the retained
public slot registry. See [native scope](behavior/battle/NATIVE_AUDIENCE_MAIN_AND_PUBLIC_SOUND_OWNER.md).

Game passes a persistent shared public SFX bank through runtime creation and
retains it across restore; AudienceInit does not erase it. Movement/Hammer
mocks now declare request failure-1 rather than returning unallocated0. This
can change continuous sound retry history. It is not original On provisioning
or proof that absent NDSP playback makes original On fail: source allocation
can precede backend creation. Original public provisioning, audible battle
effects, field/global audio interleaving and full backend parity remain open.
Source live-count/AP/motion and native failure-adapter evidence is indexed
and the EVT inbox is updated. Existing private original item-table data was
used; no new asset transformation or distributed payload.

Validation: make test217/217 CTest and136/136 Python pass
(artifacts/audience-main-tests-final.log). After a formatting correction,
current-source scoped rebuild and CTest2/2 pass (audience-main-scoped-final.log).
make all passes (audience-main-all.log); final make -o assets-ready all also
passes (audience-main-all-final.log), reusing already converted private packs.
Both successful full test runs retained. Coherent native short-boot snapshot
basebb063b1 plus owned overlay passes full native build and final rebuild in
artifacts/audience-main-native-source; logs audience-main-debug{,-final}.log.
Immutable3DSX/ELF and SHA256 manifest:artifacts/audience-main-native-snapshot.
Shared-checkout builds contained concurrent unrelated work; commit is scoped.
Doc checker1114 Markdown files/0 broken links (audience-main-doc-links-final.log); scoped diff check passes.

Isolated artifacts/azahar-audience-main story11 fixture runs actual trio
attacks, remaining target selection, source10-Star-Point reward/victory and
black fade/export hold. Script terminal0; reviewed final target, victory30 and
ending capture with no observed native exception. Emulator samples20FPS/2.78ms
target,30FPS/3.55ms reward,60FPS/4.17ms held black are observations, not physical
New2DS XL parity. User emulator profile/saves and original ISO unchanged.

Bridge regression now reaches BreakSlot_Main80203aa0 after AudienceMain,
without forced engine end/result/story/control writes. Native capture remains
a development field-restoration hold, not a completed field return. Next:
real SlotInit/mapheap array owners and idle SlotMain, surviving-unit display
(HpGaugeMain first), queued draw consumers and original BattleEnd/field
restoration. Complete playable original-fidelity prologue remains the goal.

### 2026-10-14 — Original slot arrays, constructor and idle engine pass

Bound original BreakSlot_Init after AudienceInit with real controller/reel
owners, original DOL8/8/5-word tables and three battle-mapheap allocations.
Each allocator request reads8041e80c freshly; no placeholder tables, false
pointers or audience/ending resets. Source32-byte alignment makes the three
requests consume192 bytes including headers/padding. Main preserves actual
controller/reel state and implements the ordinary idle flag/byte writes and
two-stopped transition before active-provider admission. Portable Slot_End
is ready for ordered BattleEnd frees without nulling retained pointer words;
it is not called as idle cleanup. [Native scope](behavior/battle/NATIVE_BREAK_SLOT_INITIALIZATION_IDLE_AND_ARRAY_OWNERS.md).

Bridge regression reaches btlDispMain801024f0 after audience/slot services,
without writing forced engine end, result, control or story progression.
Delivered complete source gauge initialization/Main/damage timer producers
and next status-effect/icon/part gates; indexed contracts and updated EVT
inbox. Next display work must retain damage timers and real icon flags;
no-status alone does not prove an empty icon queue. No new asset conversion;
existing private original data tables were used, ISO stays read-only.

Validation: verified make test219/219 CTest and137/137 Python pass
(artifacts/break-slot-tests-verified.log); scoped2/2 pass
(break-slot-scoped.log). First two full runs exposed prior spotlight-only
fixed EndingWork/free-header addresses. Tests now follow actual allocation
owners/free-tail sizes and retain allocated slot tables; no source allocations
were removed to preserve stale addresses. Failure logs retained. make all
passes (break-slot-all.log). Coherent prior audience snapshot plus slot-only
overlay builds native DEBUG_SHORT_BOOT (break-slot-debug.log); baseline objects
were reused and the Trio object was removed/rebuilt. Immutable3DSX/ELF and
SHA256 manifest:artifacts/break-slot-native-snapshot.

Isolated artifacts/azahar-break-slot story11 fixture script exits0 through
actual trio attacks, original10-Star-Point reward/victory and black fade/field
restoration hold. Reviewed reward30 and final capture; no observed exception.
Emulator samples30FPS/3.92ms reward and60FPS/4.86ms black are observations,
not physical New2DS XL parity. User emulator profile/saves untouched.
Doc checker1120 Markdown files/0 broken links (break-slot-doc-links-final.log)
and scoped whitespace check pass. Shared checkout contained concurrent work;
owned commit uses exact slot hunks rather than committing those edits.

A later shared native rebuild hit a concurrent attack-registration callback's
int versus ARM int32_t function-pointer return type; added an explicit int32_t
return annotation in the working tree for that feature's owner. This separate
edit is not part of the slot commit. Final shared make -o assets-ready all
retry passes (break-slot-all-current-fixed.log).
Remaining: bind live gauge initialization/direct-damage timers/Main, original
status/parts/display queue consumers and ordered teardown/field restoration.
Active slot/controller states still retain source providers. Full playable
original-fidelity prologue remains incomplete and the goal stays active.

## 2026-10-14 — Further optimization audit

Read-only audit found additional concrete work: eight house NDSP channels are
cleared on every non-House update after sample loading (480 clear calls/second
at 60 updates/second); actor rendering deep-copies complete records, including
talk-rule vectors/strings, before rejecting mismatched models; actor surfaces
run four clip passes even when the triangle is inside one UV tile. Further
paths include collision ray scratch reuse, unchanged music-mix suppression,
exact map-frame caching, immutable animation prefix checkpoints and eliminating
the small per-frame book-cover vector. Earlier synchronous save/cold audio/
music-mutex/cutaway candidates remain relevant. No runtime code was changed.

Temporary header instrumentation under artifacts/optimization-audit evaluated
first poses in all 40 local animation packs at frames 0/17/45: 221,707 surface
triangle calls, 209,823 exact single-bin-contained candidates (94.6%), zero
failed evaluations and zero direct-output comparison mismatches. Complete
vertex bytes and colors compared against the existing clipping result. This
is sampled asset-work evidence, not a gameplay-weighted profile or measured
FPS gain; broader boundary/pose/native validation remains necessary. Probe
source/instrumented headers/executable and surface-work.tsv are retained.
No private asset conversion/extraction or user emulator profile/save changes.

The [further audit](optimization/FURTHER_OPTIMIZATION_AUDIT.md) records current
source locations, priorities, behavior-preserving boundaries and implementation
validation requirements. Documentation-only handoff plus local analysis probe;
no make/test/native rebuild because product code is unchanged. Next implement
and validate idle-audio cleanup, actor-copy filtering and single-bin surface
fast paths as separate scopes; profile whole-loop percentiles and allocation/
NDSP call counts on hardware. Full remake remains unfinished.

## 2026-10-14 — Additional behavior-preserving CPU/audio optimizations

Implemented the next three audited paths plus fixed book workspace. Field
rendering now references immutable catalog metadata and copies only local
position/yaw/pose after admission, retaining source story/NPC/professor/save
object override order. Removes east-room static loop counts of 28 record
copies/16 nonempty talk-rule vector copies/120 rule elements per render call
(plaza 14/8/60); these are catalog counts, not measured allocator/frame rates.
Book cover uses a fixed five-quad array in unchanged composition order.

Surface UVs wholly contained in the sole S/T bins bypass four clipping passes,
using identical normalization/atlas arithmetic and output ceilings. Ordered
comparisons keep NaNs on the old path. Frozen original clipping oracle verifies
complete vertex/color bytes and return/partial-output behavior for 12,000
random cases, boundaries/signed zero, invalid/nonfinite inputs and limits.
Original-pack probe covers 221,707 triangle appends in 40 packs, no mismatches.
Retained-vector host O2 eligible-kernel microbenchmark: median 184.620→48.7615ms
(3.79x); not whole-game or physical New 2DS XL performance. House player now
tracks queued channels, including DONE voices, clears them once on retirement
and retains PCM; repeated idle/muted updates issue no clears. Actual player
host NDSP stub verifies playback/mix, failure/overflow, mute/re-entry, restart,
cursor rewind and shutdown. Strict compile + ASan/UBSan pass (LSan disabled
under ptrace). No assembly/fast-math or simulation/draw-order changes needed.

Validation: make test 224/224 CTest + 138/138 Python passed
(artifacts/optimization-pass2/test-final.log); make all passed (all.log).
Earlier attempts hit an absent concurrently authored battle test and its
int→uint32_t callback mismatch; added the explicit return type in that owner's
working test and reran successfully. Foreign work was not swept into commits.
Scoped implementation commits 532203b, bd5b111, fbd726a; separate indices used.

Immutable snapshot SHA256 f46e8b552ee45854c0a53a30cd61124d6d1d4b682d902c7806f0ebb8c7216f6a
under artifacts/optimization-pass2/snapshot completed isolated fresh opening
through arrival (1536-byte room1/storyKnown1/story3), and separate normal-menu
legacy east/story8 load→original Mouser talk→field/save/checksum route. Logs
opening-smoke.log/field-smoke.log; captures azahar-optimization-pass2-opening
and azahar-optimization-pass2-field. Reviewed house/map/harbor and talk/return
images; no native exception observed. Local firmware copied only into isolated
SD test roots; no audible/source/hardware parity assertion. User emulator
profiles/saves and ISO unchanged; no new assets/formats/conversion choices or
private payload committed. Shared builds included concurrent feature edits;
retained snapshot identifies the exact emulator subject. Diff/link checks pass.

[Implementation details](optimization/IMPLEMENTATION_PASS2.md) record evidence
and boundaries. Next profile hardware whole-loop percentiles, allocation and
audio mutex waits; then separately address ordinary save worker ownership,
cold PCM staging, exact actor-time caching and collision scratch reuse. These
selected optimizations are complete; full remake remains unfinished.

### 2026-10-14 — Live gauge history and surviving-unit display prefix

Bound portable original HpGaugeInit/Main with signed timers,20/50 source
durations, single-rounded linear/cubic remaining-clock curves, ratio ownership/
completion stores and independent visibility/latch tail. Native constructor
initializes before resource import, restores255/255 alpha and part speed1/1/
colorffffffff defaults, and retains later writers. Existing portable direct
damage already produces correct timers/latch; native now imports/exports
them and before/after HP rather than losing that history. Gauges advance once
per distinct unblocked simulation timestamp after the harness's event pass;
this integration order is declared, not whole original manager cadence proof.
See [native scope](behavior/battle/NATIVE_HP_GAUGE_HISTORY_AND_DISPLAY_PREFIX.md).

Ending display now runs actual ordinary alpha/gauge/size11-then10/inactive
effects/icon gates, preserving live statuses and old handles. Actual first
part204bit2 selects btlDispAnimeSpeed and local-rate8004142c, not the initially
guessed status-pose selector. Its source208=1 store precedes the held pose
provider. Original field/battle pose pool registration remains absent; legacy
raw0 is not a successful allocation. No forced sentinel-1, erased status/icon
flag, fake pose write or engine/field success was supplied.

Source alpha/status-lane, forced-pose and true256-slot/64-resource/64-texture
pool ownership contracts delivered/indexed; EVT inbox updated. Allocation
zeroing is source-proven. Original descriptor0 M_Z_1/PKR_Z_1 is missing from
converted battle subsets, so next native owner must import actual defaults
and use retained shared-history or an explicitly typed handle projection.
First converted idle pose is not original descriptor0. Existing native
original-asset animations remain in the presentation adapter.

Validation: final make test222/222 CTest and138/138 Python pass
(artifacts/hp-gauge-tests-final.log); prior scoped2/2 pass
(hp-gauge-scoped-final.log). Failure/diagnostic logs retained: an early
regression expected the wrong part branch; source inspection confirmed the
actual bit2 flag, which remained intact. Gauge numerical checks cover provider
aliases, exceptional ratios and source timer/flag stores. make all passes
(hp-gauge-all.log); final make -o assets-ready all passes
(hp-gauge-all-final.log), reusing private packs. Coherent native short-boot
source snapshot base32b7379 plus owned overlay builds from all sources
(hp-gauge-debug.log). Immutable3DSX/ELF+SHA256:hp-gauge-native-snapshot.
Shared checkout contained concurrent work; commit uses only owned changes.

Isolated artifacts/azahar-hp-gauge story11 script exits0 through actual trio
attacks, original10-Star-Point reward/victory and black fade/export hold.
Reviewed reward30/final captures with no observed exception. Emulator samples
20FPS/4.47ms reward and60FPS/7.21ms black are observations, not physical2DS
parity. Source ISO read-only, packs ignored; user emulator profiles/saves
untouched. Final doc link checker output is retained in
artifacts/hp-gauge-doc-links-final.log; scoped whitespace checks pass.

Remaining: real mode1/archive2 pose registration/default resources/local-rate/
SetAnim/Main, full per-unit/queued display and ordered teardown/field return.
Main's elapsed-clock*rate dependency must not become per-call phase accumulation.
Full playable original-fidelity prologue remains incomplete; goal active.

## 2026-10-14 — Original weapon rows, eligibility and party domain

Committed `3b5c7bd`: shared complete local weapon-table builder, target-count
wrapper and eligibility updater. It emits original Jump/Hammer/badge/inventory/
Special rows and delegates party key1 through its real callback/count-address
protocol. Private cursor/count cells, partial descriptors, untouched padding,
late tutorial override, original name/icon order, empty/dummy rows, signed
levels/items/counts, wrapped cursor correction and fresh owner/pointer reads
follow source contracts. Unbound party/update/sample providers retain explicit
port boundaries; no fabricated labels, party rows or success values.

Native launch correction: Goombella uses party domain6/cursor4c8 instead of
Item domain2/cursor51c; Hammer uses its4bc cursor instead of Jump4b0. Physical
navigation/cancellation and original labels/icons stay unchanged. Live Headbonk
checks domain6 and source attack-class count16f55. Generated source tables still
need real native providers and tutorial menu/input integration; this library
is not a completed tutorial menu or gameplay substitute.

Validation: shared make test passed224/224 C++ and138/138 Python tests. Final
focused builder/trio tests passed2/2 after a store-order fix and mutation fixture;
native make all passed/rebuilt 3DSX. Existing read-only ISO verifiers passed
10 bodies/52 data spans/894 builder anchors and20 menu/target bodies/344 anchors.
Tests cover private count mutation, empty/negative cursor preservation, signed
inventory, live levels/captured Special mask, late replacement presentation,
item-pointer read after ordinal/ID stores, captured eligibility count and raw
side addressing into a private target buffer. No Azahar/Dolphin run; no new
game payload or conversion. Scoped commits preserve concurrent gauge/audio/
rendering work. A fixture callback return type was corrected before final checks.
A concurrent subsystem folder migration began after validation (battle files
now under source/battle and include/battle); its combined build is not rechecked
by this step. Preserve that reorganization and use current paths next session.

Next: real source table/party/cost/sampling/message providers in native runtime;
original tutorial class/submenu/input/selection, player action and turn ownership;
source camera, window/effect/audio/audience consumers and physical alternatives
to optional lower-screen audience taps. Full tutorial/battle goal stays active.

## 2026-10-14 — Battle part isolation and original pose defaults

Feature commit `83402d8a` includes the native part-storage fix, genuine default
pose conversion, full original descriptor sidecars and their regression checks.
Original Mario has three source parts, not two. The former third record at
84202000 overlapped Goombella's primary; additional Mario parts now occupy the
unused native unit0 partition, preserving primary addresses and live linked
source order. Original definition/owner pointers and per-part gauge/color
defaults are tested. This address projection does not implement the original
contiguous BattleAlloc/BattleFree lifetime.

Read-only ISO assets supply M_Z_1/PKR_Z_1 and original85/32 descriptor directories;
no extracted model, texture, animation pack or ISO is committed. Unknown source
names must resolve to descriptor0; known but unconverted source poses remain
an evaluation boundary. Original additional Mario request KRN_H_1 is absent
from b_mario and therefore uses source default0. The documentation worker
recovered per-part entry, separate model/texture reference counts, release order
and the next zero-effect/status/offset services. Current display still stops
at animPoseSetLocalTimeRate8004142c: no fabricated registered handle, pose-owner
completion, battle-return or prologue parity is claimed.

Validation:

- Final combined make test passed224/224 CTest and141/141 Python checks;
  `artifacts/battle-pose-combined-tests-final.log`. Earlier shared runs exposed
  concurrent tutorial callback type/ABI issues, an unfinished menu fixture and
  the visibility fixture's migrated include path. Those were resolved before
  this final pass; do not treat earlier failed logs as final results.
- Combined native build passed with assets-ready already completed by the
  preceding full make all asset phase:
  `make -o assets-ready all BUILD=build-pose-constructor TARGET=ttyd-pose-constructor -j4`;
  `artifacts/battle-pose-combined-native.log`. The original reused build first
  failed on stale pre-migration shader dependencies; a fresh build was used.
- Reproducible baseline deadc793419e0eb335e0e6ca80357a12d50dcc25 plus the owned
  patch passed223/223 CTest and141/141 Python tests in
  `artifacts/battle-pose-clean-tests.log`, and a full DEBUG_SHORT_BOOT native
  source build in `artifacts/battle-pose-clean-native.log`. This older flat
  baseline excludes concurrent migration/menu work and is distinct from the
  final combined checkout.
- Immutable binary/ELF and SHA256 manifest:
  `artifacts/battle-pose-constructor-snapshot/`. The manifest's baseline was
  corrected to the actual archived base after concurrent commits advanced HEAD;
  exact source equality against that base plus the owned storage hunk was checked.
- Isolated Azahar script returned0 using the story11 fixture and original trio
  EXP input script; output `artifacts/azahar-battle-pose-constructor`, log
  `artifacts/battle-pose-constructor-smoke.log`. Reviewed reward30 and final
  boundary captures: original10 Star Points/victory poses, then the existing
  black development field-restoration hold. No detected exception. User profile
  and saves were untouched. Observed20FPS/3.39ms at reward and60FPS/4.60ms at
  hold; these are emulator samples, not physical hardware measurements.
- Documentation links:1130 Markdown files,0 broken links in
  `artifacts/battle-pose-doc-links-validated.log`; scoped diff check passed.

Next: retained256-pose/64-resource/64-texture native ownership with real converted
CPU workspaces; source mode1 part entry/publication; exact source directory
selection/restarts, local rates, effects and release counts. Preserve additional
Mario parts and their authored pose names. Bind the remaining original display
consumers and ordered teardown/field/result/save owners afterward. Full playable
original-fidelity prologue remains an active goal.

## 2026-10-14 — Real player battle pose ownership

Feature commit `3a8194ec` introduces the real retained pose/resource owner,
complete original descriptor identity, native player binding and shared
immutable renderer/battle banks. Source-backed fields include mode/archive,
source index0 defaults, timestamps/cache resets, colors/matrix/local rate and
linked sentinel. Mario's three source parts own distinct handles with modelref3
and textureref1; Goombella has its own modelref1/textureref1. Source copied names
are distinct from applied animation requests. Unknown KRN_H_1 selects actual
M_Z_1 when requested; known unconverted M_S_3 stays unavailable.

Native forced display executes source rate product, whole-word force choice,
pending flags and mode/status/color/attribute gates, unsigned phase counters,
unlinked zero-effect/material-light8 services and existing world-position query.
Best-effort CPU evaluation uses actual original geometry and retained workspace
at the full animPoseMain8003ed2c boundary. Main/facing/GX publication remains
held; scene rendering keeps its existing separate clock. Unregistered enemy/
Frankly factories remain explicit before a constructor -1 can bypass admission.
No source global SDK/GPU/file pointer/lifetime or original global handle-history
equivalence is claimed. The native model/texture cache counts own actual CPU
resources; renderer Tex3DS/citro3d lifetimes stay separate.

The final refinement admits fully hidden defaults based on real position/node/
hierarchy buffers, rather than visible vertex count. A meaningful hidden→walk
fixture verifies later genuine visibility. Clock inputs are named modeZero and
modeNonzero for source GP40/44 and GP38/3c; source field mode2 also uses the
nonzero branch. No new game payload was acquired or committed. Existing private
ISO-derived Z1 poses, full names and texture/animation banks were reused.

Validation:

- Combined make test:229/229 CTest,141/141 Python passed;
  `artifacts/battle-pose-pool-combined-tests.log`. Full native
  `make all BUILD=build-pose-owner TARGET=ttyd-pose-owner -j4` passed;
  `artifacts/battle-pose-pool-combined-native.log`. These precede the final
  hidden-default/clock-name refinement and include concurrent menu/field work.
- Final coherent baseline4f71901326959551a781708459530300a5236e60 plus only
  owned source overlay passed226/226 CTest and141/141 Python checks;
  `artifacts/battle-pose-pool-tests-validated.log`. The complete source native
  DEBUG_SHORT_BOOT build passed, then rebuilt after the final refinement with
  assets-ready already satisfied:
  `make -C artifacts/battle-pose-pool-source -o assets-ready all BUILD=build-pose-pool TARGET=ttyd-pose-pool DEBUG_SHORT_BOOT=1 -j4`;
  `artifacts/battle-pose-pool-native-validated.log`. Immutable final binary/ELF
  and source SHA256 manifest are in `artifacts/battle-pose-pool-validated-snapshot`.
- Pool tests cover genuine workspace ownership, descriptor/subset differences,
  unknown/unconverted names, equal-index force0/1, rate timing and phase jumps,
  repeated evaluation, uint64 wrap, nonfinite rates, separate cache references,
  borrowed and double releases, smart/linked boundaries, capacities/first-free
  reuse and hidden→visible selection. Real bank tests use full85/32 directories.
  Native trio tests prove all4 distinct bindings and source finishing M_R_1 at
  index4, pending-bit consumption and new Main endpoint.
- Early trio test incorrectly expected victory M_V_2 at this later display
  checkpoint. The existing source ending already switched to finishing M_R_1;
  assertion was corrected to that actual name/index without changing source
  behavior. A concurrent missing menu-test file briefly prevented rebuild and
  left an old test binary; coherent and combined final runs resolved it.
- Isolated Azahar on `artifacts/battle-pose-pool-snapshot` (first owner build
  before the hidden-default/clock-name refinement) used the story11 fixture and
  original EXP input script, exited0 with no detected exception. Output
  `artifacts/azahar-battle-pose-pool`, log `artifacts/battle-pose-pool-smoke.log`.
  Reviewed reward30 and final boundary: original10 Star Points/victory, then
  black development field-restoration hold. User profile/saves untouched.
  Observed20FPS/3.06ms reward and60FPS/3.49ms hold are emulator samples, not
  hardware measurements or proof of speedup. The final isolated refinement
  binary has host/native validation but no additional Azahar run.

Next: complete original animation Main/facing/GX publication and actual
registered-pose draw consumption; automatic/status-pattern branches; remaining
enemy/Frankly source factories; global field/battle allocation/cadence; queued
display and ordered teardown/field/result/save owners. Additional Mario parts
must retain their real source attributes and ownership. Prologue fidelity and
physical performance remain an active unfinished goal.


## 2026-10-14 — Source battle animation time publication

Added animation_sample_time provider and tests. Native pose evaluation stores
frame20 before rereading/multiplying local rate; ratio84 is published and
reloaded before loop integer conversion; selected24 feeds evaluateSelected
directly. Continuous sample28/2c/30 comes from the actual converted authored
key sequence. Zero-span precedes loop mode; early frames use positive-zero
ratio; nonloop end selection preserves an unclamped ratio. Loop selected time
uses fma(-integer,span,frame), retaining the source fused residual. Tests cover
nonzero starts, boundaries, ratio reload, zero-span ordering, NaN/conversion
unavailability, signed truncation bounds and a fused-rounding distinction.

Source discrete34/38/80 is independent and unavailable without its retained
data; compressed cache3c/40, full Main/GX publication, optional facing producers
and runtime cadence remain open. The CPU sampler rejects predecessor-1 and
nonfinite selected domains. Integer conversion accepts finite int32 in-range
values only; unknown original FPSCR behavior is not guessed. The source
Main8003ed2c pending boundary remains in place.

Documentation work recovered ordinary Mario post-Main CheckStatus18, integer
alpha products, TestXLU modelwordc4 and queue admission. Draw suppression is
P204 mask100. Fresh unlinked pose flags8clear skip the optional Main facing
tail; actual b_mario wordc4=1 makes TestXLU false. Post-Main dispCalcZ still
requires actual camera4 view/projection before queued registration.
Implementation breadcrumbs appended to requests/EVT-01.md; parent remains open.

Validation:

- Shared make test compiled current code and passed234/235 CTest checks. The
  concurrent battle_facing assertion at tests/battle_facing_tests.cpp:95 failed;
  Python stage was therefore not reached. Log animation-sample-time-tests.log.
  A prematurely attempted focused timing executable was not yet built; no
  stale binary is counted as final validation. Separate direct fresh g++
  timing test passed; standalone Python141/141 passed.
- Coherent snapshot artifacts/animation-sample-time-source is HEAD
  d771b764dab5c511200569d2c24d00f38e7cbb24 plus the six owned code/test files
  and exact CMake registration. make test passed232/232 C++ and141/141 Python
  checks, animation-sample-time-isolated-tests.log. Original-pack owner tests
  passed; private RomFS/gfx/assets were linked without distributing payloads.
- Shared make all BUILD=build-sample-time TARGET=ttyd-sample-time -j4 passed,
  animation-sample-time-native.log. Complete isolated make all with
  -o assets-ready and the same BUILD/TARGET passed,
  animation-sample-time-isolated-native.log. No performance claim from builds.
- Immutable isolated ELF/3DSX, SHA256SUMS and source manifests are retained in
  artifacts/animation-sample-time-validated. Scoped whitespace check passed;
  documentation link checker1143 files/0 broken links passed.

No ISO writes, new asset format/conversion, user emulator profile/save changes
or Azahar/hardware run for this step. Next: source discrete timeline/cache
ownership and full Main, then original camera4 depth/queue and real ordered
battle teardown/result/field-return ownership. Full prologue parity remains
unachieved.
