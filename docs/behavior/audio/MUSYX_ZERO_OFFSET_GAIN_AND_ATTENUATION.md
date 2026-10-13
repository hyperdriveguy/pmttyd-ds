# MusyX zero-offset fader gain and modulation/tremolo attenuation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete ZeroOffsetHandler region
80277fe0..802781e0, end exclusive. Offsets/addresses hexadecimal; row indices
and shifts decimal. Follows [admission/ramp](MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md).
Static source deductions, not measured audio or original execution.
Read [fader initialization](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md),
[fader updates](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md) and
[controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md).

## Fresh flag clear and live fader selections

Retain voice V in r30 and carried update indicator in r0. Freshly load flags118
then flags114; store118 unchanged, then114 with bit1000 cleared. This happens
on both admitted paths from the preceding ramp region. It is not conditional
on whether that earlier region set r0, and does not clear flags118 bit8000.

Capture U8 V.byte11d=kind, byte11e=row, byte120=track, in that order.
Fader base F=synthMasterFader803f8754, stride30hex. Load:

1. A=float(F+row*30), then P=float(F+row*30+14).
2. Compute single AP=fmuls(P,A).
3. B=float(F+(21+(kind!=0))*30); compute single Q=fmuls(AP,B).

Any nonzero kind chooses row22; zero chooses21. Row and kind are not validated
against the initialized32-row table. The addresses come from signed addi after
lis8040: base is803f8754, not80408754. All values are live mutable faders; no
static table payload, role label or universal current1 is inferred.

If track==ff, retain Q as f31. Otherwise read U8 synthTrackVolume803f8d54[track],
convert to single via the unsigned double bias, compute fmuls(Q,trackByte), then
fmuls(C120,thatProduct). The track byte is captured before the fader reads;
no table-extent check excludes indices64..254. This is not a capped track index.

Compare f31 with freshly loaded V.float160 using fcmpu. Equality skips the
write and preserves carried r0; every non-equal/unordered result stores f31
at160 and sets r0=1. No epsilon, NaN repair or change-trigger hardware call here.

## Base gain and attenuation admission

Freshly load U32 V.word158; convert using unsigned double bias and fsubs, then
f30=fmuls(C124,convertedValue). Capture U16 half170 then half172. If their OR0,
skip all attenuation work and reach802781e0 with f30 unchanged. This does not
clear float16c or force r0. Previous master-gain comparison still applies.

Nonzero admits inpGetModulation(V)8028f8d8 at802780d0, followed by
inpGetTremolo(V)8028fa88 at802780dc. Retain first return across second call.
X=low16 first return, T=sign16 second return. No child-return status check.
After BOTH requests freshly read half170=H and half172=J. The initial OR gate
is not rechecked if a child has made both fresh fields0.

Compute K=8192-((16384-T) arithmetic-right-shift1), exactly ceil(T/2) for all
signed16 T, including negatives/odd values. Do not substitute logical shift,
unsigned tremolo or integer truncation toward zero. Convert X and H using
unsigned double bias; convert4096-J and K using signed double bias. Each
fsubs conversion produces a single result.

Let S denote one single-precision instruction result, not host fused math:

```
a = S(X * (4096-J))
b = S(C134 * a)
c = S(1 - b)
d = S(H * c)
target = S(C12c * d)
coefficient = S(C128 * K)
```

These operations do not clamp H/J/X/T/target/coefficient to a musical domain.
The original instruction interleaving also reads fresh float16c before comparing
it with target. Only the independent arithmetic is regrouped above; subtraction
and multiplication rounding remain separate.

## Stored attenuation step and final gain

Ordered current<target: next=S(current+C138); store next at16c, then if ordered
next>target overwrite with target. Ordered current>target: next=S(current-C138);
store next, then if ordered next<target overwrite with target. Equality or
unordered comparison skips both step/store paths. fcmpo exceptions/FPSCR/traps
remain processor-domain questions; no claim of host NaN equivalence.

After all paths freshly reload float16c. Compute separately:

```
a = S(1 - freshCurrent)
b = S(coefficient * a)
c = S(1 - b)
f30 = S(previousBaseGain * c)
```

Every admitted attenuation call sets carried r0=1, even equality/unordered or
no effective change. The step is the original float C138, not elapsed-time
scaled; retained elapsed E is not used in this region. Publication of f30 to
hardware and later r0 ownership belong to the remaining handler.

## Exact constants and evidence boundary

| Address | Bits | Use |
| --- | --- | --- |
| 80429120 | 3c010204 | Track-byte multiplier C120 |
| 80429124 | 34010204 | Unsigned word158 multiplier C124 |
| 80429128 | 39000000 | C128=1/8192 |
| 8042912c | 39800000 | C12c=1/4096 |
| 80429130 | 3f800000 | Single1 |
| 80429134 | 32800200 | Attenuation C134; retain exact bits |
| 80429138 | 3e4ccccd | Step C138; approximately0.2 |
| 80429148 | 4330000000000000 | Unsigned conversion bias |
| 80429150 | 4330000080000000 | Signed conversion bias |

[Manifest](../../../tools/musyx_zero_offset_gain_attenuation_evidence.json) pins
six spans (region/whole handler/two children/two constant spans),128 whole-region
instruction anchors and two direct calls. Five address-selection/five gain/eight
attenuation vectors and all65536 signed16 half-transform cases checked. Float
vectors use a finite host single-rounding mathematical model, not an original
Gekko trace; FPSCR rounding/exceptions and special values remain open. Generic
source verifier validates hashes/anchors/calls, not those semantic deductions.
Fresh full handler ELF/listing and generator remain `/tmp`; no original bodies
or binary assets committed.

Remaining: handler pan/surround/aux/volume/hardware/output/requeue/epilogue,
actual mutable fader/track/voice/controller providers, valid index/extent/value/
alias/lifetime/IRQ/concurrency/FP domains, original gain/audio traces and native
binding. EVT-01 remains OPEN.
