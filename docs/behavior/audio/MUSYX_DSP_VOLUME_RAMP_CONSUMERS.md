# MusyX DSP volume-ramp consumers

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGION
80289c28..8028a2c0 exclusive. Whole builder80288e00/25f8hex fingerprinted;
other sample/mixer/budget/list/command sections remain open. Offsets/masks/
addresses hexadecimal; shifts/counts decimal. Read
[ramp/correction helpers](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md),
[hardware desired-triplet writer](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md),
[DSP lifecycle](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md) and
[following envelope command loop](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
Static local field/mask/request order, not measured DSP interpolation/audio.

## Retained records and nine component bindings

Retain hardware record V=r29, parameter block PB=r19 and current studio S=r16
from preceding builder work. Studio pointer binding at80288ec8 comes from the
outer studio loop, not a fresh read of V.bytef3 here. Mask array M=stack+10.
Let F be PB.half0c. H below is a halfword offset in V storing host/current value;
T is desired halfword offset. Z/D are level/delta halfword offsets in PB.
Read F/V flags freshly as specified; there is no base/pointer recapture.

| Group/component | V desired T | V host H | PB level Z | PB delta D | Correction mask B |
| --- | --- | --- | --- | --- | --- |
| A0 | 4c | 5e | 12 | 14 | 1 |
| A1 | 4e | 60 | 16 | 18 | 2 |
| A2 | 50 | 62 | 2e | 30 | 4 |
| B0 | 52 | 64 | 1a | 1c | 8 |
| B1 | 54 | 66 | 1e | 20 | 10 |
| B2 | 56 | 68 | 32 | 34 | 20 |
| C0 | 58 | 6a | 22 | 24 | 40 |
| C1 | 5a | 6c | 26 | 28 | 80 |
| C2 | 5c | 6e | 2a | 2c | 100 |

Prepare(component) reads fresh unsigned V.T and calls
sal_setup_dspvol(PB+D,V+H,target). Correct(component) reads fresh unsigned V.T
and calls salCheckVolErrorAndResetDelta(PB+Z,PB+D,V+H,target,M,B).
The complete helper contracts retain signed subtraction/truncation/correction/
snap/alias semantics. M is not initialized or cleared in this region.
Helper mask indices are correction magnitudes1..4, not current audio-slot indices.

Process groups A then B then C. Each group's dirty gate freshly reads V.word24
and tests its own bit. No dirty-bit clear is performed by this region. Earlier
hardware stores can set these bits; full slot/reset ownership remains separate.

## Shared ordinary pair and third-component rules

| Group | Dirty bit | Pair enable bits E | Pair ramp bit R | Third enable U | Third ramp W |
| --- | --- | --- | --- | --- | --- |
| A | 1 | 3 | 8 | 4 | 8 |
| B | 2 | 30 | 40 | 80 | 100 |
| C, studio mode0 | 4 | 600 | 800 | 1000 | 2000 |

On dirty: prepare components0/1/2 in order, then inspect fresh PB deltas0/1.
If either nonzero, F |= E|R. Otherwise inspect fresh PB levels0/1;
either nonzero gives F |= E; both zero gives F &= ~E. The latter does NOT
clear R or unrelated bits. Then handle third: nonzero fresh delta gives
F |= U|W; else nonzero fresh level gives F |= U; else F &= ~U. Again, W
is not independently cleared. This branch skips residual correction this invocation.
Group-specific studio exceptions below override the relevant third/pair behavior.

On nondirty: if fresh F&E nonzero, correct component0, retain returned status,
then correct component1. Reload both PB levels. If either child status OR either
level nonzero, freshly F |= R. Otherwise freshly F &= ~E. Status is not the
only continuation condition; levels after helper writes matter. Pair enable bits
are not unconditionally set on correction. If initial F&E zero, skip both
children and set delta0=0 then delta1=0, without clearing their levels/host values.

Then independently inspect fresh F&U. Set admits third correction. After child,
reload its PB level; if return OR level nonzero, freshly F |= W;
otherwise F &= ~U. Clear U skips child and zeros ONLY third delta.
No whole-triplet reset or unconditional ramp-bit clear. F stores are halfword
read/modify/write preserving unrelated fresh bits, not reconstruction of a new
flag word from the local dirty bit.

## Group A studio-mode exception

A dirty still prepares ALL three components before reading S.word54. If freshly
read mode!=0, skip third delta/level checks and clear ONLY F bit4. A2's prepared
delta/host mutation remains. Pair preparation rules stay ordinary.

A nondirty first follows ordinary pair correction, then tests F bit4. If clear,
zero delta30. If set, read S.word54: nonzero clears F bit4 without calling A2
correction or zeroing delta30; zero admits ordinary A2 correction. The mode check
therefore occurs at a different stage on each dirty/nondirty path. No cached mode
or rule that all suppressed components have zero delta.

## Group B has no studio-mode gate

B follows the shared ordinary rules regardless of S.word54. Desired/host/level/
delta/mask bindings stay those in the table. This includes B2 at PB32/34, which
the following alternate C mode can read. Its writes happen before C's fresh mode
and cross-group field tests in the same invocation.

## Group C nonzero-studio-mode branch

C freshly reads S.word54 separately on dirty/nondirty admission. Zero uses all
ordinary C rules above. Nonzero dirty:

1. Prepare ONLY C0 then C1; skip C2 preparation entirely.
2. If either fresh C pair delta24/28 nonzero, F |= 4e00 (4600|800).
3. Otherwise inspect C pair levels22/26 PLUS B2 level32 AND B2 delta34.
   Any nonzero gives F |= 4600; all zero gives F &= ~4600.
4. Branch to8028a2c0, skipping ordinary C third handling.

This is an actual cross-triplet dependency on B2, not a substituted C2 test.
No C2 delta/level/host clear is introduced. The extra4000 bit accompanies pair
600 in4600; existing800 is preserved on the level-only/clear paths.

Nonzero nondirty: test fresh F&4600. Nonzero admits C0 then C1 correction.
Reload C pair levels; any child status OR either level nonzero gives F |= 800;
otherwise F &= ~4600. No B2 read on this correction path. Zero F&4600 instead
stores pair deltas24=0 then28=0, then F |= 4600. This zero-gate branch thus
SETS enable/extra bits after zeroing deltas; it does not leave flags untouched.
Both alternatives skip C2 and continue at8028a2c0. Keep this asymmetry rather
than replacing it with the ordinary pair rule or a common mode reset.

## Request/publication order and evidence

Dirty/correction branches do not normalize desired halves, alias domains,
studio mode, initialized masks, pointers, active state or DSP status. Child calls
can change fields used by later loads. Sequential group/child/F stores and fresh
level/delta/desired/mode reads are part of the contract. Mask updates produced
here later feed the envelope command loop; that downstream region emits selected
zero-delta command pairs but is not proven actual DSP execution by these stores.
The source divisor160 and correction stride32 remain arithmetic constants,
not measured frame/sample durations.

[Manifest](../../../tools/musyx_dsp_volume_ramp_consumers_evidence.json) pins the
complete698hex-byte region, whole-builder fingerprint and two helper bodies:
four spans,422 instruction anchors/22 direct calls (eleven prepare/eleven correct
sites). Nine dirty-pair/nine residual-pair/four alternate-C flag vectors and
nine component binding rows are deductions, not original execution; vector
initial flags are explicit fixtures, not reachable-state proofs. Full original
fresh builder ELF/listing stays `/tmp`; no bodies/binary assets committed.

Remaining: surrounding builder admission/list/budget/sample/setup/cache/command
regions, actual desired/host/PB flag/mode/mask/index/record/alias/lifetime/IRQ/
concurrency domains, original ramp/mask/command/DSP/audio traces and complete
native binding. One local three-triplet consumer region answered; whole builder
and EVT-01 remain OPEN.

## 2026-10-13 — Following DSP parameter region

[Filter/DSP parameters](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md) closes the next ITD/SRC/polyphase/filter
copy region and filter setter/coefficient wrapper. Full math/FP/caller domains,
surrounding builder/command execution and original DSP/audio remain open.
