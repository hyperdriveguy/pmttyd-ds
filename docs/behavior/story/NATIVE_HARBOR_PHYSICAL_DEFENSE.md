# Native harbor physical pad and defense boundary

The original `btlevtcmd_ResultACDefence` (`80107694`, 0x74 bytes) resolves its
actor and weapon, calls `BattleActionCommandCheckDefence` (`800fa1b8`, 0x268),
then returns2. The core clears Battle+1cb0, uses actual actor HP+10c and
simplifier/unsimplifier/automatic bytes305..307, weapon byte13, signed override
halfword Battle+1d18, and private BattlePadWork at Battle+1d1c. Existing portable
`DefenseCommand` retains the source A/B windows, press-count restriction,
result writes and terminal `BtlPad_WorkInit` reset (`8011a38c`, 0xb0).

`HarborTutorial::tick(milliseconds, dialogueKeys, physical)` accepts an optional
pointer to an already-produced `BattlePadSample`. The ordinary Game input owner
must provide its physical channel sample. Dialogue keys never populate battle
input. A NULL pointer keeps this callback pending; it does not assert that no
physical button was pressed. A supplied empty sample is a real no-input tick.

The reusable packed-pad adapter reads/writes the original big-endian layout:
current snapshot0, previous12; thirty trigger words18, repeat90, held108 and
released180; repeat phase/counter bytes1f8/1f9. Physical producer press/repeat/up
masks are copied rather than reconstructed. The portable pad update retains
original stick virtual-mask rules. Private sampling occurs at most once for each
supplied simulation timestamp, before the event-manager pass. A second pass at
the same timestamp cannot reinsert a trigger cleared by the defense callback.
The full original BattleMain sequence-controller-before-private-pad ordering
remains dependent on the battle scheduler; this bridge does not claim that
scheduler is installed.

The original tutorial test executes actual Crump approach, jump and precheck.
Dialogue A input without a physical sample preserves pending80107694. A real A
sample resolves to4, resets private trigger history, and advances the original
script to pending CheckDamage8010a78c. HP and source result remain unchanged.
The packed source HP is asserted by preserving its actual pre-defense value.

`tools/verify_harbor_defense_services.py` pins the three original bodies read-only;
`tools/verify_input_cadence_contracts.py` separately checks source producer/update
cadence. No ISO bytes or extracted scripts are committed.

Next: generic packed damage import/application and actual reaction ownership,
then a source first-turn/command/Jump route. The diagnostic installed-enemy-attack
entry is internal; it does not stand in for first-turn scheduling or player input.
See [approach/jump/precheck](NATIVE_HARBOR_CRUMP_APPROACH_JUMP_AND_PRECHECK.md)
and [packed owner](NATIVE_HARBOR_TUTORIAL_PACKED_TARGET_SERVICES.md).
