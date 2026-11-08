# Native Harbor reward counting

This finite native counterpart covers the reward amount calculation in original
US btlseqEnd80215234 and its count/cooldown cases0b/0c. It does not execute the
whole ending, allocate guest End work, acknowledge source events, render icons,
or certify field return. The source audit is the original whole1470-byte body;
local disassembly `artifacts/crump-reward-audit/end.dis` has SHA-256
`9b69a17cddbcdcdb9177a8eb1913bc13175cf701d4a4475d316119435a93c297`.
See [the recovered ending contract](UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md).

Admission reads actual battle+f00 as signed32, floors to1/caps100, reads fortune
byte+18ff9 (3 doubles), then actual signed pouch level+8a (99 disables reward),
and caps100 again. These reads retain their completed prefix across holds. This
is not measured timing. Actual Crump lethal source accumulates9 points.

`start` is an explicit caller boundary: an actual native ending owner must first
complete its own introductory wait, victory event creation and PointGet resource
admission. The counter cannot prove those services. It intentionally exports no
completion receipt accepted by field sequence/completion owners.

The caller advances once per simulation tick with physical source A/B held mask
0x300. Counter increments, held input adds1000 below1000, and transfer occurs on
multiples of4 or each accelerated tick. Sound request occurs on multiples of8
and must enter the genuine `SFX_STAR_POINT_GET1` consumer. Each transfer reads
and updates the actual signed pouch EXP halfword+96. Reaching100 resets it to0
and marks level-up; the live signed level is then read for the99 cap. Counting
finishes with original10/140-counter/20 cooldown. Done concerns only this finite
count/cooldown. LevelUp and ZeroReward require separate real ending branches.

Native policy retains successful memory writes/sound admission when a later
service holds; it does not claim whole-call source trace equality during a hold.
A held tick must resume with its original tick/input; future ticks and altered
held input are rejected until that prefix completes. Completed duplicate ticks
have no additional effects. Every external borrow checks the current owner;
the production binder must pin actual battle/pouch allocation identity and host
facade lifetime. Missing services or stale ownership remain pending.

Focused tests cover9-point normal/accelerated totals and cadence, duplicate
completed ticks, each successive callback hold with exactly-once writes/sounds,
rejected changed input/future ticks during holds, stock/fortune/level domains,
level-up/max-level boundaries and stale owner. Actual Renderer/Audio integration,
full native reward visibility and field-return acceptance remain separate gates.
