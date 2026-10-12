# Native takeoff callback and private-output composition

Implementation: `include/battle_packed_takeoff.hpp`,
`source/battle_packed_takeoff.cpp`, `tests/battle_packed_takeoff_tests.cpp`.
Contract: [BATTLE_JUMP_TAKEOFF_POSITION.md](BATTLE_JUMP_TAKEOFF_POSITION.md).
Original btlevtcmd_GetTakeoffPosition80109054/354 was inspected read-only;
ELF/disassembly stay `/tmp/pm-native-takeoff.*`, outside source history.

The wrapper now operates on native event/unit/part/work bytes. Capture argument
address then shared BattleWork before resolving actor,target,part operands in
order. Capture raw output tokens after those resolutions and before actor
translation. Actor,target and candidate unit lookups use this retained work,
while hit-position queries independently recapture current work for side factors.

Actor visible X is retained before target lookup. Visible Y/Z are still read
in order into one ignored temporary. Actor home XYZ initialize the private
coordinates;requested target hit outputs overwrite X/Z and a separate ignored
value,retaining actor home Y. Candidate hit outputs use one shared Y/Z temporary.
New pointer-output overloads of packed part-world/hit queries preserve these
private aliases,including accumulated Y/Z writes through both hit layers.

EnemyBelong reads signed unit0xc:0 maps1,1 maps0,others return their signed byte.
Its side-table address is derived from the captured work. Initial unsigned side
byte is negated then narrowed to S8 for sampling bias. Request private buffer,
weapon0,translated actor,enemy belong,01001240,01000000,bias;retain sample Q.
Choice mode4 is always requested;its register return is ignored. Signed buffer
count at a6c AFTER choice alone gates candidate lookup and closer-X comparison.

For positive count,translate chosen unit again,fetch through retained work and
resolve the actual part pointer. Ordered greater absolute distance replaces
only X;ties/NaNs preserve initial X. Requested/chosen pointer identity determines
60 versus30,and nonzero Q negates that integer magnitude. Nonpositive count uses
+60 regardless of Q. Reread/sign-extend retained work's side byte,integer multiply,
convert and single add. All three integer outputs are sequential,with no raw
sentinel skip. No first-call gate or local invalid unit/part repair is introduced.

Native factory requires explicit sampling/choice providers and subtract/add/
multiply/FMA/truncate platform providers. Registered original80109054 invokes
this complete local wrapper;service object/runtime must outlive registration.
Each invocation owns one BattleCandidateBuffer,including nested invocations.
It is a native semantic container,not an emulated stack-address ABI. Its unused
bytes start zero by existing container policy;the core provider must initialize
all readable fields and chosen outputs. That policy does not establish original
uninitialized stack contents or permit a provider to consume unspecified fields.
No fixed candidate/no-sample fallback is provided. Recovered sampling/choice
cores still need native unit/part/status/source-record adapters before Game use.

Acceptance fixtures cover mixed coordinate sources,independent fresh/captured
work queries,retained raw tokens,positive/nonpositive/high-bit counts,ignored
chooser return,equal/different part pointers,Q reversal,tie/NaN,live side reread,
raw sentinel stores,shared destinations,private hit Y/Z aliases,nested buffer
ownership,and actual native byte RAM/callback registration. Their providers and
records are authored inputs,not original execution/hardware evidence.

Remaining work:bind [sampling](BATTLE_ENEMY_CANDIDATE_SAMPLING.md) and
[choice](BATTLE_ENEMY_CANDIDATE_CHOICE.md) to live native records;source geometry/
kind/part/monosiri ownership,PowerPC numeric exceptions,original motion/event
services and complete Jump/outer battle/Game integration. Documentation-agent
follow-up belongs under EVT-01/JUMP-01 in
[DOCUMENTATION_REQUESTS.md](../../DOCUMENTATION_REQUESTS.md).
