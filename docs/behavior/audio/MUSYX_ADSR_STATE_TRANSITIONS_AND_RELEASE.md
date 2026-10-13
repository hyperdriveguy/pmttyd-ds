# MusyX ADSR state transitions and release

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salChangeADSRState802863f4/26c,
adsrStartRelease80286688/150 and adsrRelease802867d8/168hex.
Addresses/offsets/masks hexadecimal; phases/scalars decimal. Static state/level/
step/conversion operations, not original envelope cadence/audio. Read
[setup caller](MUSYX_RESOURCE_AND_PITCH_ADSR_COMMANDS.md),
[hardware descriptor layouts](MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md)
and [unsigned FP conversion](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md).

## Shared envelope fields and exact gates

Q: type byte0, phase byte1; count word4, level8, logarithmic-domain wordc,
step10; attack14, decay18, sustain half1c, release word20, attack selector byte26.
Names label local roles, not validated units. Raw type0/1 are recognized; other
types return0 without writes. State-change recognizes phase0/1/2/4; phase3 and
other values do not change fields and return0. No active/IRQ/null/type normalization,
identity/member/value bounds, byte26 ownership or raw pointer validity checks.
All wraps are32-bit, divides unsigned unless stated. Captured comparisons do
not revalidate the later fresh divisor reads under unsupported mutation.

## Complete type0 attack/decay/sustain/terminal fallthrough

salChangeADSRState starts result0. Phase0 reads attack14, stores it to count4.
If nonzero: phase1, level8=0, step10=7fff0000/ FRESH attack14; return0.
Attack0 falls directly into phase1's local block WITHOUT writing phase1 first.

Phase1 reads decay18, stores it to count4. Nonzero: phase2, level8=7fff0000,
read sustain then fresh decay; step10=wrap32(- unsignedDivide(
wrap32(7fff0000-(sustain<<16)),decay)). Sustain beyond32767 can wrap numerator;
no clamp. Decay0 falls directly into phase2's local block.

Phase2 reads unsigned sustain. Nonzero: phase3, reread sustain, level8=sustain<<16,
step10=0; count stays as earlier/previously stored. Sustain0 goes terminal.
Phase4 also goes terminal: level8=0, return1. Terminal does NOT independently
write phase4, count/step/log word or clear entire descriptor. Zero attack/decay
can therefore reach completion while byte1 retains its original phase.

## Complete type1 attack and effective decay duration

Phase0 reads attack14/store count4. Attack0 falls through to decay. Nonzero:
phase1, read byte26. Byte26==0: level8=0, step10=7fff0000/FRESH count4,
log wordc untouched. Nonzero: wordc=0, level8=0, step10=00c10000/FRESH count4.
Different domains/scales are selected; no combined normalization.

Phase1 captures sustain S and decay D. Exact effective count calculation:

`X=wrap32((193-S)<<16)`
`K=(unsignedHighMultiply(15390949hex,X)) logical>>4`
`count4=wrap32(D*K) logical>>16`

Read freshly stored count4. Zero falls through to sustain block. Nonzero:
phase2, level8=7fff0000, log wordc=00c10000, reread sustain/count;
step10=wrap32(- unsignedDivide(wrap32((193-S)<<16),freshCount)).
No decay duration floor1, saturating S193 clamp, widened product or unchanged
raw decay count; positive decay can yield0 through scale/truncation/wrap.

## Type1 sustain table and terminal

Phase2 sustain0 goes terminal. Nonzero: phase3, reread sustain/store S<<16 to
wordc, reread wordc. Rounded domain value = SIGNED arithmetic>>16 of
wrap32(wordc+00008000). Index=max(193-rounded,0), without upper bound. Read
unsigned half at dspAttenuationTab803bc260+index*2, store half<<16 at level8;
step10=0. Original table has194 halfwords (indices0..193). Raw sustain32768 or
65535 can produce much larger or index194 through signed rounding; extent/
reachable-value invariants remain open. Sustain0 bypasses this table block.
Phase4 or zero sustain terminal writes ONLY level8=0 and returns1, as type0.
No pow/hardware/job call in salChangeADSRState.

## Explicit versus stored release duration

adsrStartRelease(Q,R) uses raw duration R argument. adsrRelease(Q) recognizes
types0/1 before reading Q.word20, then captures that stored duration. Both
perform equivalent local release steps below; neither calls the other or the
state-change service. Unsupported type returns0 without duration/phase writes.
No phase==4/already-releasing guard; admitted calls can rewrite existing release.

Type0: store phase4 then count4=duration. Zero duration: count4=1, step10=0,
return1; level8/log wordc remain unchanged. Nonzero: read level8, compute
wrap32(-unsignedDivide(level,duration)), store step10, return0. Zero duration
does not immediately clear the linear level here, despite returned1.

## Type1 release domain conversion and zero result

First, if byte26==0 AND phase==1, read level8, arithmetic>>21 as RAW signed
index; read byte at dspScale2IndexTab803bc3e4+index, without negative/1023 guard.
Store wrap32((193-byte)<<16) to log wordc. Otherwise preserve log wordc.

Read log wordc, convert SIGNED32 through43300000/XOR80000000 and signed bias
4503601774854144 using SINGLE fsubs. Multiply with fmuls by original float at
804291e8,0.00032383418874815106. Convert captured duration UNSIGNED32 through
43300000/duration and unsigned bias4503599627370496 with SINGLE fsubs;
multiply the two single values with fmuls. Request __cvt_fp2unsigned, take
returned word logical>>12, store count4; phase4; reread count4.

Effective count0: count4=1, level8=0, wordc=0, step10=0, return1. Nonzero:
read wordc fresh, step10=wrap32(-unsignedDivide(wordc,count)), return0.
Thus converted-zero type1 clears levels while raw-zero type0 does not.
Single rounding/conversion can yield zero from positive duration; no use of
attack/decay time-cent conversion, pow, fctiwz signed output or common floor1
before computing step. Child return matters as duration word, not a status.

## Return values, evidence and remaining source/runtime

salChangeADSRState return1 means its terminal local path wrote level0; return0
also includes unsupported type/phase and untouched fields. Release return1
means selected zero-duration/effective-count path, not hardware end/completion.
No audio stop/resource/voice free, flag clear, callback, simulation/render tick
unit or measured envelope update frequency follows from these three bodies.

[Manifest](../../../tools/musyx_adsr_state_and_release_evidence.json) pins three
bodies/linked converter/five numeric data spans: nine spans,217 anchors/two calls.
Original194 attenuation halves/1024 scale bytes and FP literals are numeric
metadata. Five linear/five log-decay/five raw-release/seven index-formula vectors
are deductions, not original execution. Index formula vectors include S0 as math,
but original sustain0 bypasses lookup. Full fresh ELFs/listings stay `/tmp`;
no original bodies/binary assets committed.

Remaining: envelope advancement service and actual type/phase/count/level/log/
step/duration/sustain/selector/pool/caller writers, valid table/index/divisor/
float/value/alias/IRQ/concurrency domains, macro/hardware/DSP consumers and
original envelope transition/release/audio traces. EVT-01 remains OPEN.

## 2026-10-13 — Advancement follow-up

[Advancement/output order](MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md) closes adsrHandle and its fifteen-call
wrapper: old-level outputs precede exact-zero count transitions. Actual callers,
cadence/value/index/alias domains and original envelope/audio traces remain open.
