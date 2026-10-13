# MusyX hardware pitch/volume publication and DSP ramps

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete hwSetPitch80290cac/78,
hwSetVolume80290f50/2c0, sal_setup_dspvol80288734/48 and
salCheckVolErrorAndResetDelta80288640/f4hex. Addresses/offsets/masks hexadecimal;
shifts/divisors/thresholds decimal. Static local arithmetic and stores, not
measured volume/pitch/audio. Read [DSP lifecycle](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md),
[envelope command loop](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md) and
[pitch providers](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md).

## Complete pitch setter and duplicate suppression

hwSetPitch(I,P) captures V=dspVoice8041ef88+wrap32(I*f8). Take low16(P).
If >=4000hex replace P with3fff; otherwise retain raw P register. Encoded
word X=rotateLeft32(P,4)&000ffff0, equivalently low16(admitted P)<<4.
No signed interpretation or frequency conversion here.

Read V.bytee8 as previous slot. If notff, read V.word(38+4*bytee8) and compare
unsigned to X. Equal returns immediately with NO time-byte read/event/marker
write. Otherwise (or markerff): read salTimeOffset8041efb8, store X to
V.word(38+4*timeByte); RELOAD time byte, OR8 into V.word(24+4*freshByte);
RELOAD time byte AGAIN and store to V.bytee8. V/base stays captured throughout.
Three time reads can select different data/event/marker slots. No bound to
0..4, active/query gate, timestamp/voice identity check or IRQ control.
Marker bytes outside0..4 exceptff remain raw prior-slot indices.

## Complete volume wrapper: inputs and mixer request

hwSetVolume takes raw ordinal I in r3, raw integer arguments B/P/K in r4/r5/r6,
and floating inputs f1/f2/f3. Retain P for later ITD indexing. Capture record V
using raw I*f8. In order, compare each float with original float1.0 at804292b4;
ordered greater OR equal replaces it with1.0. Lower/negative values are retained;
unordered comparison does not take this clamp. No lower bound or finite-value
normalization. Invalid FP exception/FPSCR domains require original runtime evidence.

Read V.bytef3, then V.wordf4. Select studio S=80407c68+bc*bytef3 and read S.word54.
Request salCalcVolume with r3=B,r4=stack+08,r5=P,r6=K unchanged,
r7=V.wordf4 logical>>31,r8=(captured S.word54==1), plus clamped f1/f2/f3.
The equality flag uses wrap32(1-word), count-leading-zeros, logical>>5; it is
exact equality to1, not generic nonzero. Child body8028b694/6f0 is fingerprinted
only; full mixer/interpolation semantics remain open. No child status test.

## Three independently cached output triplets

Reload nine child-produced stack floats as three triplets. For each component,
SINGLE fmuls with original32767.0 at804292c8, then signed fctiwz; extract the
conversion's low word and retain low16 for comparisons/stores. Source does not
use unsigned conversion, nearest rounding or integer saturation before storing.
Actual child output, NaN/overflow/FPSCR/conversion domains remain open.

| Stack float offsets | V halfword destinations | Marker byte | Dirty bit in V.word24 |
| --- | --- | --- | --- |
| 08/0c/10 | 4c/4e/50 | e9 | 1 |
| 14/18/1c | 52/54/56 | ea | 2 |
| 20/24/28 | 58/5a/5c | eb | 4 |

For each triplet, markerff forces publication. Any other marker compares all
three existing unsigned halves against new low16 values; equality skips writes.
Publication stores first/second/third halves, freshly ORs dirty bit into V.word24,
then sets marker=0. The triplets are processed in table order; later flag loads
preserve earlier writes. Dirty publication always uses slot0 word24, unlike
pitch's time-indexed event word. Skipped paths leave marker/flags untouched.
No explicit active/studio/voice validity, buffer initialization or DSP completion.

## Fresh ITD gate and raw pan index

After triplets, reread V.wordf4. Bit80000000 clear skips ITD. Set:
index=(retained P logical>>16)&ff; select unsigned half from itdOffTab803040c8
at2*index. Source loads this half TWICE: first value into V.halfd0, second
value used for low16(32-second) into V.halfd2. Freshly OR200 into V.word24.
No duplicate suppression here. Original table has128 halfwords, all0..32;
raw indices128..255 exceed that extent and are not clamped by this wrapper.
Do not read beyond the table to invent values or infer reachable pan domains.
The earlier mixer sign-bit argument and this fresh gate can differ under mutation.
No salTimeOffset read for volume/ITD publication and no scalar status assignment.

## Complete coarse ramp preparation

sal_setup_dspvol(D,H,T) receives halfword delta pointer D, current/host pointer H
and raw target T. Read unsigned *H and sign-extend it; sign-extend low16(T).
Difference X=signed16(T)-signed16(*H), as32-bit subtraction WITHOUT a16-bit wrap.
Compute signed high multiply by66666667hex, arithmetic>>6, add its sign bit;
this equals signed division by160 toward zero across X=-65535..65535.
Store low16(quotient) to D. Then RELOAD *D and sign-extend it; RELOAD unsigned
*H; store low16(freshH+160*signedFreshDelta) to H.

No target equality/active/domain/alias guard or independent result code.
The refreshed loads matter if D and H overlap; do not replace them with captured
values. Small differences leave residual error; quotient0 still writes D and H.
The divisor160 is a source constant, not measured sample/frame duration.

## Complete residual correction and delta reset

salCheckVolErrorAndResetDelta(Z,D,H,T,M,B) uses r3/r4/r5/r6/r7/r8 respectively:
Z=extra halfword pointer, D=delta halfword, H=current/host halfword,
T=raw target, M=mask halfword array, B=raw mask bits. First compare unsigned
low16(T) to unsigned *H. Equal skips arithmetic and reaches common delta reset.
Otherwise compute X=signed16(wrap32(signed16(T)-signed16(*H))): unlike ramp
preparation, this difference DOES truncate/sign-extend to16 bits.

If32<=X<160: q=X arithmetic>>5, so1..4. Read M.half(2*q), store OR B aslow16;
store *D=1; RELOAD unsigned *H, store low16(freshH+(q<<5)); return1.
If-160<X<=-32: q=(-X) arithmetic>>5, again1..4; same mask OR;
store *D=ffff; RELOAD H, store low16(freshH-(q<<5)); return1.
The explicit q<5 gate controls the mask write, while admitted arithmetic already
implies it. Mask index is correction magnitude q, not a caller's current slot.
Neither correction branch writes Z or reaches common delta reset.

All other differences: if low16(T)==0 AND X>-32, store *H=0 then *Z=0.
This includes some positive/wrapped signed cases; it is not an unconditional
zero-target snap. Otherwise preserve H/Z. Then common tail stores *D=0,
returns0. Equality also clears D but never snaps H/Z. Differences exactly
+160/-160 bypass correction; +32/-32 enter it. No broad tolerance clamp,
absolute difference rewrite or mask clear. Aliases can alter refreshed H/mask
loads after stores; local no-alias guarantee and valid M extent are absent.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_hardware_pitch_volume_and_ramps_evidence.json)
pins four complete bodies, linked salCalcVolume, two float literals and128 numeric
ITD halves: eight spans,121 anchors/one direct call. Seven pitch/six ramp/eleven
correction/four triplet-gate/five ITD-index vectors are source deductions;
all131071 signed differences for the divide160 formula were checked independently
against mathematical truncation toward zero. These are not original execution;
out-of-table vectors record indices only. Full ELFs/listings stay `/tmp`;
no original bodies/binary assets committed.

Remaining: full salCalcVolume/math/table providers and DSP builder ramp callers/
command consumers, actual pitch/pan/float/flag/marker/record/studio/time/mask
writers, valid index/FP/alias/lifetime/IRQ/concurrency domains and original
volume/pitch/ramp/DSP/audio traces. Four local bodies answered; EVT-01/full
native binding remain OPEN.

## 2026-10-13 — Mixer preparation follow-up

[Coordinate/phase preparation](MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md) closes the mixer parameter/table/
sentinel/phase region and remainder wrapper. Both interpolation/output branches,
IEEE remainder/math/domain/original audio remain open; full mixer fingerprint
still does not establish its complete semantics.

## 2026-10-13 — DSP ramp caller follow-up

[DSP ramp consumers](MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md) closes the builder's three-triplet prepare/
correct/mask region, including studio exceptions and cross-triplet dependencies.
Whole builder/providers/valid alias-domain/original DSP/audio remain open.
