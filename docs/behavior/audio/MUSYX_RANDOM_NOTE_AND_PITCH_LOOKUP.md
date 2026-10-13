# MusyX randomized note commands and pitch lookup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdRandomKey80280c78/19c,
sndGetPitch802862b8/e4, sndPitchUpOne8028627c/3c,
__cvt_fp2unsigned80264860/5chex and opcode23 stub80282f48/10.
Addresses/offsets/masks hexadecimal; scalar values decimal. Static RNG/command/
floating-point/conversion operations, not original randomness/music/audio traces.
Read [RNG/wait](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[note/publication](MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md),
[last-note setter](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
and [integer pitch conversion](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md).

## Random interval capture and absolute/relative domains

mcmdRandomKey retains voice V/command C. FIRST capture W1=C.word4. If its byte
bits8..15 is0, read W0: A=U8(W0>>8),B=U8(W0>>24), set L=min(A,B),H=max(A,B).
These absolute endpoints can be128..255; no initial note127 clamp.

If captured W1 byte bits8..15 nonzero, read W0 then unsigned current V.half12e;
L=clamp(current-A,0,127),H=clamp(current+B,0,127). Arithmetic is signed32 here
without intermediate low16 store/sign extension; current can be65535. Relative
endpoints saturate independently; they are not signed command offsets.

No pointer/command/activity/IRQ/seed/identity/member check. Captured W1 determines
both relative and cents modes despite later child/shared-cell mutation.

## Exact RNG call count, command rewrite and wait

Captured W1.low8!=0: request sndRand FIRST for cents. Take low16 R, compute
R%201-100 using the original signed-high-multiply quotient sequence with constant
028c1979hex, then narrow low8 to encoded cents K. For R0, K156 encodes-100;
R100 gives0,R200 gives100. No reject/resample or signed-byte storage here.

W1.low8==0 instead reads fresh C.word0 bits16..23 as K without a cents RNG call.
Then in BOTH cases request sndRand for note, take low16 Q. Width=H-L+1; compute
signed quotient Q/width (operands nonnegative), remainder Q-quotient*width,
selected=L+remainder. Rewrite C.word0=25 OR (selected<<8) OR (K.low8<<16),
then C.word4=0. This is modulo selection including both endpoints, not rejection-
sampled uniformity or independent per-frame randomness. Even width1 consumes Q.

From fresh rewritten word0 store (word>>8).low7 to V.half12e; read it again,
store bits16..23 at byte130. Absolute selected200 therefore becomes note72,
not127. Request voiceIsLastStarted(V); true publishes fresh note/channel/set to
inpSetMidiLastNote. Then C.word0=4, preserving already-zero word4; request
mcmdWait(V,C). Return that child's register result, but opcode23 stub IGNORES it
and joins with initial stop marker0 unchanged. This differs from add-key opcode24
which propagates wait return into stop marker. Normal fetch already advanced PC.
No hardware pitch/sample call or RNG-seed reset in this body.

## Complete sndGetPitch default, lookup and single-precision order

Inputs raw note I and reference word W. Exact Wffffffff substitutes40005622hex:
base note64 in high8, rate22050 in low24. Other W is retained verbatim. N=I.low8,
B=W.high8,R=W.low24. No note127/positive-rate/table-bound/active/IRQ checks.

Convert unsigned R using double construction (43300000 high word, R low word),
subtract original bias4503599627370496 with SINGLE-precision fsubs. Thus result
is rounded to binary32 before later math. N==B uses that converted rate directly.
N>B loads toneup_tab803bbe60[index=N-B]; N<B loads tonedown_tab803bc060[index=B-N].
Each original table contains128 binary32 entries, but differences can reach255;
there is no index<128 guard. Multiply converted rate by table value with fmuls.

Then multiply by original binary32 literal4096, again fmuls. Read synthInfo
803f8540.word0 output rate FRESH, construct/subtract unsigned double bias using
single fsubs, divide scaled rate by that single value with fdivs. Request
__cvt_fp2unsigned on the resulting floating value and return its word.
No combined double-precision formula, output-rate0 guard or pitch-word clamp.
Exact tables differ from recomputed powers or rounded host constants: up index1
is1.0594635009765625, down index1 is0.9438743591308594.

## Complete unsigned floating conversion

__cvt_fp2unsigned reads original three double constants at80303a20:
0,4294967296,2147483648. Compare unordered-capable input X with0 and2^32.
X<0 returns0. Otherwise start resultffffffff; if X is NOT strictly less than
2^32, return it (includes positive infinity and NaN/unordered).

For 0<=X<2^32, compare with2^31. Below: fctiwz X, extract integer word. Otherwise
DOUBLE fsub X-2^31, fctiwz, extract integer word, add80000000hex wrap32.
Returns unsigned truncation toward0 in that valid finite interval. No original
floating environment/exception behavior asserted beyond these branches. Negative
infinity follows X<0; negative zero takes finite conversion0. Division/result
conditions in sndGetPitch remain separate from this helper's local conversion.

## Complete one-step upward helper and distinct constant

sndPitchUpOne(raw P) narrows P.low16, constructs unsigned double/subtracts bias
using single fsubs, multiplies with fmuls by original literal at804291b8:
1.0594631433486938. Then fctiwz, extracts low integer word, returns it. No
16-bit output mask, unsigned conversion helper, sample rate/global state or
hardware call. This multiplier differs from toneup index1; do not unify them.
Low16 input yields nonnegative in-range signed conversion, but broader caller/
floating-environment semantics remain separate. This is arithmetic, not measured
musical accuracy or DSP interpolation behavior.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_random_note_pitch_lookup_evidence.json) pins four
bodies/one region/six linked identities/six data spans:18 spans,95 anchors/nine
calls/one registration. Original128+128 float tables and conversion literals are
recorded as numeric metadata/identities. Six interval/five selection/five cents/
six pitch-route/seven unsigned-conversion vectors are deductions, not execution.
Full fresh ELFs/listings stay `/tmp`; no original bodies/binary assets committed.

Remaining: actual command/seed/RNG/note/cent/reference/rate/table/caller producers,
valid interval/table/float/divisor/shared-cell/alias/IRQ/concurrency domains,
pitch/modulation/DSP consumers and original random-note/pitch/audio traces;
other opcodes/full native binding. EVT-01 remains OPEN.
