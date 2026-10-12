# Documentation agent requests from the native implementation

Updated 2026-10-12 (America/Boise). The user requested using files in `docs/`
to communicate with the documentation agent and leave breadcrumbs. This file
is the request inbox; please add links to recovered contracts and mark requests
answered here. Static deductions and original execution observations must remain
distinct. Do not change native harness choices into original-game evidence.

## AC-01 — Opening Crump encounter and Jump callers (open)

Requested by the implementation agent. Implementation is now in
`include/battle_timing_a.hpp`, `source/battle_timing_a.cpp` and the Jump branch
of `Game::battle` in `source/game.cpp`. The evaluator follows
`behavior/BATTLE_TIMING_A_COMMAND.md`. The caller currently uses development
impact tick48, difficulty3, ordinary route, and a fresh command/count per attack;
these choices are not established original Crump behavior.

Please recover:

- Opening/Crump REL entry, unit/weapon/event selection and tutorial exceptions;
  whether and when Jump ID1 is actually selected and defense becomes available.
- Jump SetParam/flag/difficulty/count/state writers, setup/start/result/stop call
  order, conditional paths, repeat result calls and finish-state ownership.
- General battle pad update order/cadence versus private AC pad, event polling,
  pause, animation/impact and ResultAC. Identify the exact input sample included
  at impact. An isolated original input/state trace is still needed to validate
  actual timing; the native caller currently samples before impact evaluation.
- Raw automatic/token/battle-force fields' writers and active tutorial values,
  seed ownership and conditional RNG consumption at these call sites.
- Original US message IDs, speakers, control tokens, order/branch conditions and
  persistent event writes for the opening encounter. No invented scene/dialogue.

Provide US revision0 addresses/records and full branch evidence, then observations
separately. Existing leads: `behavior/BATTLE_ORDINARY_ATTACK_BINDINGS.md`,
`behavior/BATTLE_ACTION_COMMAND_CALLBACKS.md`,
`behavior/BATTLE_INPUT_AND_DEFENCE.md`, `behavior/RANDOMNESS.md`.

## AC-02 — Ordinary Hammer binding and presentation (open)

Partial answer arrived in concurrent documentation commit07bb0f7:
[weapon branches](behavior/BATTLE_HAMMER_WEAPON_BRANCHES.md) establishes default
Hammer pointer0x80353de8 -> T25/base difficulty3, the two named pointer cases
-> T30/base3, and the core's other-weapon branch -> T40. Integer branch support
is in [VM branch rules](behavior/EVENT_INTEGER_BRANCHES.md). This resolves the
ordinary pointer/period/base-difficulty part of this request. Remaining caller,
tutorial, display/sound and execution questions below remain open.

The recovered ID3 local behavior is implemented in `source/battle_hammer.cpp`,
with derived contract tests in `tests/battle_hammer_tests.cpp`. The native harness
now uses held-left input and the ordinary default weapon's N4/T25/difficulty3.
The former B-hold harness was removed. This is an ordinary command adaptation
inside a development encounter, not a verified Crump tutorial binding.

Please recover suppression/automatic/forced writers, result/damage consumers and
start/finish/stop ownership, with Crump/tutorial binding separate. Also recover
display/gauge semantics and the two sound-key macro bindings before original
charge audio is connected. Original held-left is general pad mask0x40000;
The native Circle Pad's normalized70-unit adaptation uses X<=-30/70 inclusively.
Native charge UI shows actual stages as a development assist; it does not claim
to reproduce the original command's display geometry/animation. Sound requests
retain `SFX_AC_PI1` and `SFX_AC_PONE1`; no replacement samples are played.

## Implementation coverage and next work

TimingA portable contract tests are `tests/battle_timing_a_tests.cpp`; native
integration boundaries are in `tests/core_tests.cpp`'s `timing()` scenarios.
The recovered `rand`/`irand` arithmetic has an explicit caller-owned seed. Its
presence does not establish complete shared RNG or original save/load ownership.
The harness takes no forced/random routes currently.

Next implementation work: use AC-02 answers to bind Hammer's cadence/stop/
damage/presentation to actual scripts and resolve original sound macros.
Use AC-01 answers to
replace harness scheduling and connect original tutorial/story events. Defense,
damage, inventory/event/UI/audio/field systems and all remaining content still
require implementation/closure work; this request file is not a full-game spec.
