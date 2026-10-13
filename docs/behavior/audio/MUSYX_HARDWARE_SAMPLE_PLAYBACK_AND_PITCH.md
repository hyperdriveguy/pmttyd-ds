# MusyX hardware sample playback initialization and pitch conversion

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete hwInitSamplePlayback80290814/1b4,
DoSetPitch80280000/1a0, hwSetSRCType80290d24/2c,
hwSetPolyPhaseFilter80290d50/2c and hwSetITDMode80290ef4/5chex.
Addresses/offsets/masks hexadecimal; scalar literals decimal. Static hardware
memory/pitch operations, not original DSP/audio execution. Read
[sample-start caller/descriptor](MUSYX_MACRO_SAMPLE_START_AND_RESOURCE_LOOKUP.md),
[priority storage](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md) and
[frame/DSP order](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).

## Playback inputs, raw ordinal and inclusive time-slot reset

Inputs (I,ID,D,A,P,F,B,J): raw voice ordinal, sample ID, descriptor pointer,
reset selector, packed priority, full identity, SRC/filter reset selector and
ITD mode. Compute retained wrap32(I*f8) offset. Hardware pool pointer8041ef88
is reloaded as specified below. No ordinal mask/bounds/null/active/IRQ guard.
The caller supplies I.low8, but direct callers need their own domain.

Initialize index0/accumulator0. At EVERY loop test read salTimeOffset8041efb8
byte T; admit index.low8<=T. Reload pool, read selected record.word24+4*index.low8,
retain only bit20hex, store that word0, OR retained bit into accumulator. Increment
full index and repeat. Then reload pool, store accumulated20-or0 at record.word24.
Stable T0 processes ONE word; stable Tn processes n+1 words for n<255. Stable
T255 never fails the low8<=255 test; it repeats wrapped indices. No count repair
or independent iteration budget. A changing T/base can alter subsequent writes.

## Prefix publication and descriptor word copy

Each prefix operation reloads the hardware pool separately: record.word1c=P,
word18=F, wordf4=0, half70=ID.low16. Capture pool record pointer R for descriptor
copy. For source offsets0,4,8,c,10,14,18,1c in pairs: load each source pair,
store to R offsets74,78,7c,80,84,88,8c,90 respectively. Eight FULL32-bit words.
The last copy includes descriptor padding bytes1d..1f, which the referenced
lookup does not initialize. No descriptor/length/codec validation or code decoding.

A FULL-word nonzero test was captured before this copy. If A!=0, separately
reload pool for each store: bytea4=0, wordb8=0, wordbc=0, halfc0=32767, wordc4=0.
A0 leaves those fields unchanged. Then always separately reload pool and set
bytes e8/e9/ea/eb=255 in order. These are field operations, not a whole hardware
voice zero. No sample reference release, hardware break or DSP message here.

If B FULL word!=0, request hwSetSRCType(I,0) then hwSetPolyPhaseFilter(I,1).
B0 skips both. Always request hwSetITDMode(I,J). Returns ignored; descriptor
copy/earlier mutations are not rolled back. Actual DSP/control consumers remain
open; this initializer does not by itself prove audible sample start.

## Complete SRC/filter and ITD leaves

SRC/filter: compute raw wrap32(I*f8), capture current pool. Table index=K.low8,
offset=index*2. Read unsigned halfword at SRC8041e3f8 or filter8041e400. Each
original table contains THREE halfwords0,1,2; no index<=2 bound. Store result to
record.halfcc (SRC) orhalfce (filter), then read word24 and store OR100hex or
OR80hex respectively. These children use captured pool for their own stores.

ITD tests K.low8, differing from A/B's full-word tests. Low8==0: compute raw
ordinal offset, reload pool for wordf4 OR80000000, reload for halfd0=16, reload
for halfd2=16. Low8!=0: capture pool record, read wordf4, clear ONLY high bit,
store it; d0/d2 remain unchanged. K256 therefore takes the zero route. No
half-range validation, flag-bit reinterpretation, job/DSP completion or status.

## Pitch inputs, equal-frequency branch and original table

DoSetPitch(V) captures requested word128 then sample word124. Define unsigned
low24 D=requested&ffffff, S=sample&ffffff, N=sample.high8. Equal S==D stores
V.half12e=N then byte130=0 and returns, including both zero. Unequal uses
unsigned divide with no zero-divisor guard. No pointer/active/ID/IRQ checks.

Original table T at803bba18,28 bytes/14 unsigned halfwords:

`4096,4339,4597,4871,5160,5467,5792,6137,6502,6888,7298,7732,8192,0`

Last0 is included in original span identity; normal neighbor interpolation uses
indices0..12. Threshold search starts11, not13. Do not replace this table with
floating-point equal-tempered constants or normalize its source edge cases.

For S<D (up) use R=unsigned wrap32(D<<12)/S. For S>D (down) use
R=unsigned wrap32(S<<12)/D. Shift wraps BEFORE division. Starting octave o0,
up to11 increments: while (R>>12)>=2^(o+1), increment o. Divide R by2^o
(integer floor), call result Z. No larger octave search or quotient saturation.

Set j11; read T[j]. While Z<=T[j], decrement j and move pointer BACK2 bytes.
Stop only when Z>T[j], STRICT comparison. There is no j>=0 guard. Even an
exact octave ratio8192 normalizes to4096 and walks below the declared table:
T0 is4096, preceding halfword803bba16 is20948, so that comparison also continues.
Source keeps reading earlier memory; it does not return a table error. Extent/
preceding-memory/reachable-ratio domains remain required, not invented outputs.

## Exact pitch publication and unsigned interpolation

After search stops, k=wrap32(12*o+j). Up stores low16(N+k) to half12e, without
127 upper clamp. Then read table neighbors, compute wrap32((Z-T[j])*100), divide
UNSIGNED by T[j+1]-T[j], store quotient.low8 to byte130.

Down compares k SIGNED32 with N. k>N stores byte130=0 THEN half12e=0 and returns.
Otherwise store low16(N-k) to half12e; compute wrap32((T[j]-Z)*100), divide
UNSIGNED by T[j+1]-T[j], store quotient.low8. The backward difference normally
wraps negative; do not substitute signed division or complementary cents. Static
in-table example N60,S32000,D24000 gives note56/byte135. Up N60,S32000,D48000
gives note67/byte1; N255 with that ratio gives half262, not127. These are deductions,
not original sound observations. No hardware pitch setter/job/flags write here.

## Evidence and remaining requirements

[Manifest](../../../tools/musyx_hardware_sample_playback_and_pitch_evidence.json)
pins five bodies/three tables/preceding halfword: nine spans,146 anchors/three
calls. Eight descriptor copy bindings, four finite slot-reset/nine pitch vectors
check deductions. Diagnostic pitch vectors STOP analysis at undefined divisor
or first read before declared table; those markers are NOT original returns.
Full fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed.

Remaining: actual hardware pool/time-slot/descriptor/padding/flags/ITD/SRC/pitch/
caller providers, valid ordinal/table/slot/ratio/divisor/preceding-memory/alias/
IRQ/concurrency domains and DSP/format/loading/consumer/original playback/pitch/
audio traces. Other macro opcodes/full native binding and EVT-01 remain OPEN.
