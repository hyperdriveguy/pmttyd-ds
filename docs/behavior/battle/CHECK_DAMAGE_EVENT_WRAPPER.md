# CheckDamage event wrapper and calling-event ownership

The complete US `btlevtcmd_CheckDamage` body begins at 8010a78c and spans
**0x100 bytes**, including its final `blr` at 8010a888. Earlier /fc annotations
omit that return instruction. The read-only ISO verifier pins the full body.

`battleCheckDamageEvent` implements the wrapper around the separately owned
BattleCheckDamage core. It captures the argument pointer and global battle
pointer first. It resolves/translates attacker, looks up that unit in the
captured battle, then resolves/translates and looks up the defender in the same
captured battle. Part ID is resolved before the part lookup, followed by
resolved weapon. Flags and output token remain raw and are captured before
calling the damage core. No fresh global-battle lookup replaces the captured
pointer when operand services mutate state.

A negative core result returns 255 immediately. It neither writes the output
nor calls evtCheckID. For a nonnegative result, it writes the captured output
first, then reads the calling entry ID and queries the live event registry.
An existing ID returns 2; an absent ID returns 255. Cached entry flags are
insufficient when damage dispatch cancels/replaces roots or reuses storage.

The native trio adapter now executes this shared wrapper around its existing
supported damage core. The old inline cached-flags return was removed.
Unsupported core admission retains the port service boundary (return 0) before
output or ID query; this is an explicit port limitation, not an original
BattleCheckDamage result. Full counter/status/effect/audience orchestration and
tutorial core admission remain separate unfinished dependencies. This change
does not widen the supported weapon/target combinations or bypass those gates.

Host tests verify exact callback order, captured global battle despite operand
mutation, raw output capture despite core mutation, post-output entry-ID read,
existing IDs with cached flags clear, absent IDs with cached flags set, null
defender/part forwarding, negative-result output preservation and absence of
ID query, and unsupported-service retention. Native trio attack regression
covers the real adapter. These are host executions and static ISO evidence,
not Dolphin runtime observations.

See [damage orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) and
[trio setup and return](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
