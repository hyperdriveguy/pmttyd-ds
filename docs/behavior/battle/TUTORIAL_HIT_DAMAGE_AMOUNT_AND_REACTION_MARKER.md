# Tutorial hit damage amount and reaction marker

The shared portable callbacks and tutorial packed registry now bind the original
US `btlevtcmd_GetDamage` (8010f458/74), its `BtlUnit_GetHitDamage` leaf
(80127448/c), and `unk_80113f1c` (80113f1c/6c). Complete-body SHA256 checks in
`tools/verify_battle_hit_code_events.py` pin the read-only source evidence.

GetDamage resolves/translates the actor, reloads the global battle pointer,
looks up the unit, then captures the raw output operand. A null unit leaves
that output unchanged and returns 2. A present unit supplies signed byte +270;
this callback does not calculate or apply HP damage.

The marker captures the global battle pointer before actor resolution. A
nonnull unit receives U16(+1fe) OR 1; null returns 2 without a memory write.
It produces no output, HP change, random draw, or extra wait.

Both native tutorial callbacks and the underground trio use these shared
implementations. The host tutorial fixture dispatches the original Crump
ordinary hit code 0x300|23: it reaches original pose D_1, remains active during
the early timed interval, and completes within the bounded run without an
unsupported callback. HP/result remain unchanged. The test also replaces a
live reaction with the actual tiny system hook to check root cancellation.
This is host execution of source scripts, not a Dolphin observation or proof
of a complete player attack, animation playback, turn flow, or field return.

See [the trio source graph](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md) for
ordinary subsetDamage branches and separate DefaultDamage wait restoration.
The Crump fixture does not claim that the generic wait-restoration tail ran.
