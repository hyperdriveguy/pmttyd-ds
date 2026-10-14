# Native hit-position composition and capture order

Implementation: `include/battle/battle_packed_hit.hpp`, `source/battle/battle_packed_hit.cpp`,
`tests/battle_packed_hit_tests.cpp`. Contract:
[BATTLE_UNIT_PART_POSITION_QUERIES.md](BATTLE_UNIT_PART_POSITION_QUERIES.md).
This binds a query to native records; it does not establish original field
writers, executed trajectories, numeric exceptions or full Jump/Game fidelity.

Original read-only inspections: BtlUnit_GetHitPos801289ec/124 and
btlevtcmd_GetHitPos801120a8/11c. Temporary disassembly/ELFs are
`/tmp/pm-native-hit-pos.*` and `/tmp/pm-native-hit-event.*`, outside history.

The explicit unit's signed belong byte0xc is read before current BattleWork.
Its signed side factor is captured from work+8*belong+0xa before calling the
part world-position query. No local negative-index clamp is introduced. The
part query still uses its retained owner, independently of the explicit unit.

Two triplets at part170/174/178 then17c/180/184 each reload unit114 for every
axis. X/Y multiply through an explicit rounded-single platform provider,
then FMA with captured side factor or recovered Y coefficient, respectively.
Z uses direct FMA(offset,scale,currentOutput). X/Y and Z cannot be combined
into one generic multiply/add operation without changing rounding.

Instruction-order detail beyond the earlier contract: the first X rounded
multiplication80128a64 precedes current outX load80128a68. All remaining X/Y
output loads precede their multiplication. The Y coefficient is read once from
804226a8 at80128a74, after the first current X load and before its FMA. The
original coefficient is binary32 1.0; native composition reads that source
word instead of reloading it for each Y axis. Stores are ordered and source
reads remain live: output aliases can change later offset/scale/output reads.

The event wrapper retains the first unit lookup's result. Its separate registry
part helper recaptures BattleWork/unit, so the unit used for scale/belong and
part's owner can differ. Raw output tokens are captured after part lookup.
All coordinates finish in private float temporaries before per-axis integer
truncation/SetValue; raw f1194d80 skips conversion/store, not the full query.
Return2 has no first-call gate. Native registry binding uses original801120a8.

Fixtures cover distinct owner/explicit unit, independent facing/side signs,
signed belong -1, both triplets, six live scale reads, captured sign/coefficient,
shared outputs and outputs overlapping future offsets, rounded X/Y versus
fused Z, fresh work/unit/part lookups, retained raw outputs and native byte RAM.
The host std::fma/multiply/truncate implementations are injected test providers;
they do not claim PowerPC FPSCR/exception or original hardware fidelity. Missing
native arithmetic providers remain explicit failures, not synthesized defaults.

Remaining integration: source unit114/part offset construction and writers,
original takeoff callback/motion state, transitive Jump services, full outer
battle/menu/Game flow and original executed/hardware validation. Further
numeric/backend evidence can be left under EVT-01/JUMP-01 in
[DOCUMENTATION_REQUESTS.md](../../DOCUMENTATION_REQUESTS.md).
