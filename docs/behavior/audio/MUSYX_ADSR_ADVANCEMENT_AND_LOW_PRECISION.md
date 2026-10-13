# MusyX ADSR advancement and low-precision wrapper

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete adsrHandle80286940/198hex and
adsrHandleLowPrecision80286ad8/84hex. Addresses/offsets are hexadecimal;
counts and shifts decimal. Static local operations; original cadence and audio
are unmeasured. Read [state transitions/release](MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md)
for the called state-change body and descriptor fields, and
[hardware layouts](MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md).

## Inputs, gates and shared arithmetic

adsrHandle(Q,O,D) receives descriptor Q and two halfword output pointers.
Q: type byte0, phase byte1, count word4, level8, log wordc, step10,
attack14, decay18, sustain half1c, release word20, attack selector byte26.
These names describe local roles, not validated time/level units.
Type0/1 are admitted. Other types return0 without descriptor or output writes.
For either type, phase3 writes low16(arithmeticSigned32(level)>>16) to O,
then halfword0 to D, and returns0. There is no descriptor update or count
read/decrement on this branch, assuming outputs do not alias Q.
Every other phase executes advancement: phase0/4/255 are not rejected here.

All word additions, subtractions and negations wrap32. Define quantization
q(X) using signed32 X: if nonnegative, arithmetic>>21; if negative, negate
with wrap32, arithmetic>>21, then negate with wrap32. Store only low16(q).
This truncates toward zero for ordinary values, but signed minimum80000000
negates to itself and gives q=+1024, not -1024. A generic absolute-value/
division rewrite would lose this edge. No level saturation or sign repair.

## Type0: publish old level, advance level, quantize fresh step

At80286978 capture old level L, then step S. Store wrap32(L+S) to Q.level8;
write low16(arithmeticSigned32(L)>>16) to O. The published level is captured
before advancement, even when this invocation reaches a state boundary.
Reload Q.step10 AFTER writing O; quantize this fresh value into D. This differs
from quantizing the earlier captured S if O aliases a step halfword.

After BOTH output writes, read Q.count4, decrement with wrap32 and store.
If the new count is nonzero, return0. Exactly zero requests
salChangeADSRState(Q) at802869c8 and propagates its result. An initial count0
becomes ffffffff and does not request a transition. No initial-zero shortcut,
duration floor, clamping to0/7fff0000 or phase-validity guard is present.
The child can replace descriptor fields after output publication; there is no
second output update to publish the resulting phase's new level.

## Type1: selected linear attack or logarithmic table advancement

Capture phase, byte26 and old level L. If byte26==0 AND phase==1, read step
and store wrap32(L+step) to level8; log wordc is untouched. All other non3
phases/selectors take the logarithmic block, including phase0/4.

The logarithmic block reads log wordc and step10, stores their wrapped sum
back to log wordc, then reloads log. Set rounded=arithmeticSigned32(
wrap32(log+00008000))>>16 and index=max(193-rounded,0). Read unsigned
halfword at dspAttenuationTab803bc260+2*index; store halfword<<16 to level8.
The original table has194 entries, indices0..193. There is a lower index clamp
but no upper clamp; negative/wrapped log values can address beyond that table.
Do not substitute a repaired lookup or infer reachable descriptor domains.

Both blocks then write the CAPTURED OLD level's signed high half to O.
Reload Q.level8 AFTER that write; compute wrap32(freshLevel-L), interpret
signed32, quantize using q and store low16 to D. Delta therefore derives from
actual level difference, not directly from log step. Output aliasing can affect
that fresh level read. Only after both outputs, decrement fresh count exactly
as type0; new count0 calls salChangeADSRState at80286aa4 and propagates its
result. A linear update followed by a transition can thus leave O/D describing
the pre-transition advancement while descriptor fields hold the next stage.

## Wrapper: at most fifteen calls with reused outputs

adsrHandleLowPrecision retains the same Q/O/D pointers and initializes its
counter to0. Each iteration calls adsrHandle at80286b14. ANY nonzero child
result immediately returns1, with no further call. A zero result increments
the counter; its low8 is compared against15. After fifteen zero results,
return0. The counter never reaches byte wrap in ordinary execution.

O/D are reused each invocation, so the final executed child's writes remain;
there is no sum of deltas, accumulated output, first-level preservation or
post-loop averaging. Unsupported types cause fifteen calls that leave outputs
unwritten. Sustaining descriptors cause fifteen steady outputs and no count
advance, assuming disjoint outputs. State transitions returning0 allow another
iteration with the new phase; a nonzero terminal result stops immediately.
The wrapper normalizes result to0/1. It has no time argument, wait, render-frame
binding, pointer/type/domain check or IRQ control. Fifteen invocations do not
establish measured frame cadence or a simulation-to-render conversion.

## Evidence and remaining bindings

[Manifest](../../../tools/musyx_adsr_advancement_evidence.json) pins two complete
bodies, the linked state-change body and original numeric table: four spans,
83 anchors and three direct calls. It retains194 numeric table halves and
27 formula vectors: ten quantization, five linear/count, seven log-index and
five wrapper-result cases. These are source deductions, not original execution;
out-of-table cases record indices without reading beyond the table. Whole-body
hashes pin instructions outside the selected anchors. Full inspection ELFs and
listings remain in `/tmp`; no original bodies/binary assets are committed.

Outputs are written in O-then-D order. No local no-alias guarantee exists;
output/Q overlap and shared writers can alter later fresh reads, even on
sustain. Null/alignment/address validity and overlapping-output domains require
caller evidence. No audio stop, voice free, DSP scheduling or hardware completion
follows from these returns alone.

Remaining: actual type/phase/count/level/log/step/selector/caller writers, valid
index/value/alias/lifetime/IRQ/concurrency domains, envelope caller cadence,
macro/hardware/DSP consumers and original envelope/output/audio traces.
EVT-01 stays OPEN; this closes the two local advancement bodies only.
