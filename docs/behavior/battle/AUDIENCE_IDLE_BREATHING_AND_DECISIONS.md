# Audience idle breathing and decisions

US G8ME01 revision0, read-only DOL inspection, not Dolphin measurement.
Portable whole local Wait body80192260/7ac is in
[`battle_audience_idle.cpp`](../../../source/battle/battle_audience_idle.cpp).
Native binding is the existing member-state provider in TrioAttackRuntime;
this does not insert audience calls into a new outer cadence.

| Source body/data | Address / bytes | SHA256 |
| --- | --- | --- |
| BattleAudienceCtrlProcessKinopioWait | 80192260 / 7ac | ba41b43c41fdc15fd137dbaef56d0620241e28c64bf57c0293ba964b70613667 |
| Type dispatch | 80397af8 / 28 | df95c49ba6490a6b429f21ac3372b5eda71efcf47cf3947281584b463862d73b |
| GetAudienceNoFromOffset | 801a2768 / 80 | 2e49c7ddf721ad096afc887a4ddb024e40eaf81e8fad43521c6a77b4c2f366c8 |
| BreakSlot_GetBreakTurn | 80203370 / 24 | 179deb1e6e16d83c96ae590e53cdc9ca62a913a787e5aa982bb776cd17f76f7b |

Wait captures audience base and member, calls pouchGetPtr (return unused), then
selects animation0 for front low-byte1, otherwise1. It computes single-rounded
ratio=(desired1377c+bias13780)/baseline13778. Distinguish original FPR0 from
FPR29:

| Ratio | FPR0 | FPR29 |
| --- | --- | --- |
| >=.75 | 1 | 1 |
| >=.5 | .5 | 2 |
| >=.25 | .25 | 4 |
| otherwise, including unordered | 0 | caller register retained |

The low branch loads FPR0=0 at8019233c; it does **not** write FPR29. The service
carries an optional double callerF29; absence stops at the first instruction
which needs it. It never substitutes zero, reuses a previous member's local
FPR29, or assigns it merely because earlier branches usually do so. Type2 never
uses this register in breathing; type4 through timer188 also need not use it.
Native caller provenance remains unbound on the remaining low-ratio paths.

Independently, ratio bands >=1.75/1.5/1.25/1 choose FPR31,FPR30 as
(10,4)/(5,2)/(2,1)/(1,0); lower/unordered selects(0,0). These are later reaction
weights, independent of the breathing period multiplier.

## Breathing

All FMA updates use coefficient * **FPR0** + scale64, not coefficient * scale64
+ scale64. The subtracting phases execute fnmsubs: negate the single-rounded
fused(coefficient,FPR0,-scale). Keep the final negate, including signed zero.
Typed single-product/fused services expose the source numeric boundary.
Native adapters use single multiply/std::fma for exactly single-valued inputs;
NaN or non-single-valued inputs remain unavailable. No Gekko FPSCR/GQR bit parity claim.

Type2 skips breathing and timer30 entirely. Type4 increments timer30 without a
scale write through180, adds .005*FPR0 on181..188, then alternates minus/plus/
minus/plus/minus through inclusive thresholds188+(8/16/24/32/40)*FPR29. Past the
last threshold it resets scale to1 and timer to0 before the common increment.
The remaining type table maps0/1/3/5/8 (and types>9) to the default branch:

| Type | Reset when timer >= | Positive through timer <= | Coefficient |
| --- | --- | --- | --- |
| default | 32*FPR29 | 15*FPR29 | original .0025 |
| 6 | 36*FPR29 | 17*FPR29 | original .00175 |
| 7 | 22*FPR29 | 10*FPR29 | original .00375 |
| 9 | 64*FPR29 | 31*FPR29 | original .00125 |

Reset writes scale1 and timer0, then reevaluates the midpoint with the new timer.
Both branches increment timer with32-bit wrap. Constants are consumed from the
existing unchanged PMEVT data, including their original float bits; decimal
labels above are descriptions, not replacement literals or measurements.

## Decisions after breathing

Flag40 takes priority: clear60, call whole ChangeStatus10, then read unit0's
kind10. Values236..239 suppress the target adjustment; the upper comparison
requeries unit0 as in the original. Otherwise fresh audience base supplies
baseline13778, multiplied by original80424570 (-.14999999105930328), and whole
AddTargetNum(delta,0) is called. Missing/invalid unit or unavailable target/math
service preserves earlier stores and stops. The native AddTarget helper's
existing finite conversion domain remains in force.

Otherwise flag20 or signed item half1c!=-1 returns without RNG. All other cases
make one bound360000 original RNG call:

- draw<10 selects state1; draw<20 selects state2.
- cutoff=truncate(single(20+single(10*FPR31))); draw<cutoff selects state4.
- BreakTurn>=1 or captured audience base WORD13910==1 permits state3 when
  draw<wrapped(cutoff+300), excluding current type11.
- Otherwise add truncate(single(10*FPR30)) to cutoff with32-bit wrap. Only below
  that cutoff, GetAudienceNoFromOffset(slot,0,1) finds the preceding row. If it
  exists, current type is not11, and fresh GetExist(neighbour) low-byte1, select3.

Offset helper uses signed slot remainder modulo20 plus column offset for the
column boundary, then wrapped(slot-rowOffset*20+columnOffset) in0..199. Positive
row offset therefore subtracts20, not adds20. BreakTurn reads fresh battle slot
work1616c+10. GetExist requires flags1 and excludes flags102.

## Validation and remaining boundaries

Host tests pin the original type table and constants, cover all breathing
species/boundaries and FPR0 bands, low-ratio register admission, partial
stores, item/suppression/damage gates, source RNG cutoffs and neighbour rules.
A coupled positive fixture runs original population8 -> Control -> member
controller -> Wait for20 ticks with the same original RNG. Its camera-facing
provider is explicitly a test value1. This proves the portable control chain;
it does not prove native normal outer Main/camera cadence or visual framing.
Native state4 now runs through the original
[cheering/reaction provider](AUDIENCE_CHEERING_JUMPS_AND_BOOING.md). Remaining
states7/8/10..20 still stop at their actual source addresses. Main idle remains
implemented within its declared caller-register and numeric limits.
Original menu/HUD art, dialogue, action-command timing and physical controls are
unchanged; audience throw telegraphs and optional taps remain future work.
