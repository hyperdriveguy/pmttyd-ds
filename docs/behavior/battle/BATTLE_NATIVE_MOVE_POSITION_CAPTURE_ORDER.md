# Native MovePosition,record setters and horizontal integration

Implementation: `include/battle/battle_packed_move_position.hpp`,
`source/battle/battle_packed_move_position.cpp`, `tests/battle_packed_move_position_tests.cpp`.
Contracts: [BATTLE_MOVE_FRAME_AND_POSITION.md](BATTLE_MOVE_FRAME_AND_POSITION.md),
[BATTLE_JUMP_EVENT_CALLBACKS.md](BATTLE_JUMP_EVENT_CALLBACKS.md) and
[BATTLE_PACKED_MOVEMENT_SOUND_ORDER.md](BATTLE_PACKED_MOVEMENT_SOUND_ORDER.md).
Original complete MovePosition8010ca74/334,btlMovePos80125b70/ac and
SetMoveCurrent80128210/34 inspected read-only;artifacts stay
/tmp/pm-native-move-position.*,/tmp/pm-native-horizontal.*,
/tmp/pm-native-move-current.* outside source history.

## Live event setup and native fields

Original8010ca74 now registers over native event/unit/work storage. Each call
resolves/translates actor and re-resolves integer XYZ/count/sound-mode/type.
Raw Y/Z/count are prefetched BEFORE their predecessor's signed-word conversion
at8010cae4/cb14/cb44. Captured argument pointer/raw next operand survive provider
mutations. No evtGetFloat,unit-null repair or query speed fallback is introduced.
Current work/unit lookup occurs after all seven resolutions on every invocation.

FirstCall alone controls setup. Event initialized (original byte0xd) becomes1;
U16c receives count. Visible XYZ are captured,then current154/158/15c and target
160/164/168 setters compare each retained incoming float to a fresh804226c4
sentinel read,storing non-equal/NaN axes. One skipped target axis retains its
previous target value. Heading17c and distance180 use original private visible
X/Z and raw requested X/Z,independently of retained target axes. Zero80422280 is
captured before X subtraction for ordered direction188 update;equal/unordered
preserves prior direction. Native fields are not a MarioJumpUnit snapshot.

Stored U16c is reread before choosing automatic/explicit count policy. Automatic
uses current distance180/speed170,divide/truncate/store/reload,exact-zero1 and
another count reload. Adjustment rereads speed before count conversion,then
rereads distance before fused residual. It converts retained count again AFTER
residual to supply its denominator. Speed is oldSpeed+residual/count;nonzero
stored count instead stores distance/count. No positive-residual count increment
or local fallback1 speed is borrowed from GetMoveFrame. Loader uses real U1c4/
U18c records,type/mode from retained resolutions. Setup falls through to a step.

## Horizontal read/store and completion order

The helper captures incoming speed/heading. Load original tau80422690 and
36080422694 before rounded multiply/divide to radians. Request sin,round returned
double to single,request cos;then form rounded X step before reading destinationX.
Only AFTER that read round cos's returned double,flip the captured speed sign,
add X and multiply Z step. Store X BEFORE loading/adding/storing Z. This preserves
aliased X/Z and provider mutations across trig/conversion calls. Cosine rounding
is not silently moved ahead of the X read. Required sin/cos/double-round/float
arithmetic providers make numeric ownership explicit;host defaults are not used.

Every invocation reads speed170 then heading17c,increments baseline X/Z,loads
all baselineXYZ before ordered visible stores,decrements16c and reloads signed
count. Positive count requests native unit movement-sound core and returns0.
Nonpositive count loads all retained targetXYZ,stores visible axes in order,
requests native end sound and returns1 (advance+yield). BaselineY remains fixed;
terminal targetY can differ. Negative supplied count still steps before snap.
Sound/type changes on resumed operands do not reload stored target/count/record.
Original playback providers remain required at their used boundaries.

## Native interpreter acceptance and limits

Fixtures bind shared byte RAM/event mapper and explicit authored numeric/playback
providers. They cover first-call step,positive blocked sound,terminal snap/end,
resumed operand re-resolution without reinitialization,event byte0xd,query21 versus
motion20 with5.05 adjusted speed for D101/S5,zero count/speed adjustment,negative
count step,per-axis sentinel/NaN/zero-sign storage,fresh sentinel after prior store,
horizontal alias,trig/current-X/cos-round read order,prefetched operands/fresh work,
live stored count/terminal reread and direct registry binding. An actual native
USER_FUNC stream remains cached on the first blocked dispatch,then resumes and
sets opcode0/yields BEFORE the END command when movement returns1.

These are local authored domains,not measured scene times/positions. std::sin/
cos/hypot and scalar conversions injected in tests do not prove original
PowerPC/FPSCR/libm exception behavior. Callback invocations are simulation events,
independent of rendering;full manager/pause/input cadence and original motion
call sites still need scene composition. No floor/Y interpolation/collision/HP/
AC evaluator,provisional impact constant or replacement sound is added here.
Next:native MarioJumpParam/MarioJumpPosition,original source initialization/math/
audio services,transitive Jump graph,outer battle/Game and original/hardware checks.
