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

The recovered ID3 local behavior is in
`behavior/BATTLE_HAMMER_STICK_KEEP_LEFT.md`. The current native Hammer branch
still uses the earlier provisional B-hold/release harness; it is not ID3.

Please recover the complete branches selecting N4/T25,30,30,40, weapon level,
difficulty, suppression/automatic/forced writers, result/damage consumers and
start/finish/stop ownership, with Crump/tutorial binding separate. Also recover
display/gauge semantics and the two sound-key macro bindings before original
charge audio is connected. Original held-left is general pad mask0x40000;
do not label the current B adaptation as the original input contract.

## Implementation coverage and next work

TimingA portable contract tests are `tests/battle_timing_a_tests.cpp`; native
integration boundaries are in `tests/core_tests.cpp`'s `timing()` scenarios.
The recovered `rand`/`irand` arithmetic has an explicit caller-owned seed. Its
presence does not establish complete shared RNG or original save/load ownership.
The harness takes no forced/random routes currently.

Next implementation work: implement the documented local Hammer command with
platform-independent held-left input and sound requests; use AC-02 answers to
bind its period/difficulty/presentation to actual scripts. Use AC-01 answers to
replace harness scheduling and connect original tutorial/story events. Defense,
damage, inventory/event/UI/audio/field systems and all remaining content still
require implementation/closure work; this request file is not a full-game spec.
