# Packed battle coordinates, movement and jumps

These helpers retain source-space IEEE fields and callback order, not renderer
coordinates. Hexadecimal offsets/masks are used below. [Packed state/candidates](packed-battle-state.md)
covers related storage; [battle entry](battle-entry.md) describes unit/part
construction. Original evidence remains in the [behavior index](../behavior/README.md).
One movement visit is a source simulation visit; constants are not measured
render-frame timing or SDK/Gekko numeric parity.

## Position and part output contracts

[battle_packed_position.cpp](../../source/battle/battle_packed_position.cpp)
keeps three different interfaces. Raw home `30` / visible `3c` copies load/store
one axis at a time; destination zero is not optional and aliases can affect later
reads. EVT getters snapshot all XYZ before conversion/output callbacks. Visible
GetPos skips raw sentinel `f1194d80` (-250000000); GetHomePos writes it too.
SetPos resolves/converts **all three integer operands**, including sentinels,
before unit lookup, then snapshots previous XYZ, restores raw-sentinel axes and
stores all axes. Movement setters instead resolve a float to speed `170` or
acceleration/gravity field `174`.

[battle_packed_part.cpp](../../source/battle/battle_packed_part.cpp) returns head
`14` for any negative requested ID. A single-part head returns regardless of
requested definition ID; only a longer list scans definition `+0`. The source
reads head's next before proving nonnull, so caller admission is mandatory.

World calculation reads part flags `1ac` and owner `4ec` even on absolute mask
`10000000`. Absolute XYZ `18` loads per output axis. Relative calculation
captures owner visible XYZ, part offset `24` and signed facing `189`; X uses
FMA(offset,facing,base), Y/Z use addition. Value/pointer/guest-address output
overloads preserve their own capture and alias behavior. The EVT performs an
unused first unit lookup, then recaptures global battle inside part lookup;
those owners must not be combined. It privately snapshots coordinates before
truncation/output and skips raw sentinel axes.

[battle_packed_hit.cpp](../../source/battle/battle_packed_hit.cpp) captures signed
alliance and current battle-row direction before part-world calculation, then
adds two offset layers at part `170/17c` with unit scale `114`. X/Y use rounded
product followed by FMA, Z one FMA; source factor one is loaded for Y. First X
multiplication precedes its output read, while later X/Y read before multiply.
Pointer outputs stay live/aliasable; the EVT snapshots privately. Its first
unit token is retained while part lookup can recapture another global owner.

## Linear movement and sounds

[battle_packed_move_position.cpp](../../source/battle/battle_packed_move_position.cpp)
uses baseline `154`, target `160`, count `16c`, speed `170`, heading `17c`,
distance `180` and direction `188`. Baseline/target setters compare each axis to
source **float** sentinel `804226c4`, distinct from the raw EVT operand sentinel.
Input values copy before stores; source sentinel reads remain fresh per axis.

Every invocation resolves all operands with original interleaved getter/
conversion order. First-call setup copies visible to baseline, stores target
and geometry, and sets direction 1/255 only for a nonzero horizontal delta.
Count zero derives trunc(distance/speed), forces one if still zero **only in
this controller**, then adjusts residual speed. Explicit count divides distance
by count. No extra division/finiteness guard is introduced.

Horizontal integration loads source tau/degrees, computes radians with supplied
single-result math, calls double sin/cos and rounds their results. Z flips the
IEEE sign bit of speed, preserving signed zero/NaN payload shape. X store
precedes old-Z read, so aliased addresses cannot become simultaneous vector
updates. Publish baseline, decrement unsigned count, then test signed >0:
remaining work controls movement sound and returns 0; otherwise snap full target,
end retained sound and return 1. No render-delta scaling belongs here.

[battle_packed_movement_sound.cpp](../../source/battle/battle_packed_movement_sound.cpp)
retains a source record, not an NDSP voice object:

| Record offset | Field |
| --- | --- |
| `0/4` | First/second sound keys |
| `c/e/10` | Initial/first/second timing halfwords |
| `12/14/18` | Signed countdown / alternation halfword / retained handle |

Unit record is `18c`, source mode table `1c4`. Loading always resets toggle and
handle -1. Mode -2 retains keys/timing; -1 with types 0–3 copies exact source
rows (type 2 zeros timing); other modes clear them. Countdown initializes from
`c`. Controller still reads discarded scratch-vector words before key checks.

First-key-only sound starts once using supplied position, then updates retained
handle using fresh part/unit position. Periodic sounds instead decrement signed
countdown through zero to -1, alternate parity, reload timing and trigger using
the original supplied position; no handle is retained. End stops only retained
handle `18` and resets it to -1. The trigger forwards original sentinel
-250000000 and zero argument unchanged. Lookup/playback/lifetime need actual
providers; no silent successful audio substitute is supplied.

## Generic jump versus Mario curve

[battle_packed_generic_jump.cpp](../../source/battle/battle_packed_generic_jump.cpp)
uses baseline Y `158`, vertical velocity `178` and gravity `174`: add velocity,
then subtract gravity each source visit. Count operand zero preserves an existing
stored count before deriving one if still zero; unlike linear movement it never
forces one. Zero derived count returns 2 after setup writes and before sounds or
integration. Otherwise the source coefficient, residual distance and vertical
delta determine initial velocity. When updated velocity is negative and baseline Y is below target, clamp
baseline Y to target, integrate horizontal, publish and decrement. Signed >0
returns 0; exhaustion snaps target, plays landing key and returns 1.

JumpContinue sets baseline only on first call, integrates and asks actual floor
provider each visit. Strict Y<floor clamps/completes; equality continues. It has
no countdown or descending-only floor gate. Do not replace floor service with
constant height or use the other jump's completion rule.

[battle_packed_mario_jump.cpp](../../source/battle/battle_packed_mario_jump.cpp)
reuses offsets with different phase meanings. The event initialized byte is
phase 1/2/3, not Boolean. Mode 1 selects eligibility phase 2 and skips ordinary
setup; other modes establish baseline/target and trajectory. All operands still
resolve every invocation.

| Mario field | Trajectory meaning |
| --- | --- |
| `1a8/1ac/1b0` | Per-axis baseline increments |
| `178/174` | Curve phase / phase-advance factor during trajectory |
| `1b4/1b8` | Theta degrees / angular increment |
| `1bc/184` | Prior visible Y / height coefficient |

Ordinary derived count zero returns 2 without a forced one. Height rounds after
each double step of `((distance-20)/6)+47`; modes 2/4 load distinct original
multipliers. Theta begins at source 90. Modes <3 use float 360/count; other
modes use **integer division** then conversion. Curve advance uses source double
FMA and single rounding. Initial prior-Y zero is deliberately stored twice.
Do not fold these operations or substitute one shared ballistic formula.

Phase 1 advances baseline XYZ, computes visible Y using height × sine plus
baseline, clamps upward to target when signed count<3, then decrements.
Completion uses **exact unsigned zero**, unlike generic signed >0. It retains
negative self-subtraction for velocity reset rather than folding to zero, keeping
signed-zero/NaN/provider effects, then loads source gravity. Phase 2 queries real
eligibility: failure returns 2; success sets count one / phase 3 and returns 0
before its continuation. Phase 3 updates XY and completes on unsigned zero,
without generic target snapping. Overwritten original sound scratch loads remain
explicitly unmodeled in the implementation; no stronger source-read parity is
claimed by these comments.

Angle revision uses original double 360 through fmod, rounds single, converts
NaN to source zero and adjusts negative results with source float 360. It is not
a generic host angle-normalization replacement. AC eligibility first reads
alliance/kind (so a unit is required), rejecting nonzero alliance, kind 223,
statuses 1/2/9/27 or status-word mask 1.

## Takeoff positioning

[battle_packed_takeoff.cpp](../../source/battle/battle_packed_takeoff.cpp)
keeps actor home Y while target-hit Y goes to scratch. It samples a private
buffer with weapon zero, source admission/attribute masks and negated low-byte
side direction; chooser mode 4 runs even for empty candidates. The buffer and
output references cannot escape the call.

Default horizontal magnitude is 60. Positive signed candidate count can choose
a nearer X, changing only X. Requested/chosen part pointer equality uses 60,
otherwise 30; the sampler's swap scalar negates magnitude only in that branch.
A fresh signed battle-row direction multiplies the offset before XYZ truncation.
All raw outputs are written without sentinel skipping. This does not select an
attack, move a root or invent a target when a provider is unavailable.
