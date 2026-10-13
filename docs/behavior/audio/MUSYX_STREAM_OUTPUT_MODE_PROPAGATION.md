# Game, synthesis and stream output-mode propagation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSetOutputMode800e1494/38,
sndOutputMode8027a300/ec, streamOutputModeChanged8027b200/130 and
hwDisableHRTF802915dc/chex. Addresses/offsets/masks hexadecimal; counts/mode
values decimal. Static source deductions, not original mode/audio execution.
Read [stream lookup/mode/mix](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[voice/IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) and
[hardware mixing](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md).

## Game preference write before public request

SoundSetOutputMode(M) reads word803e0350 (base803e0138+218). Exact fullword
equality with raw M skips all writes/requests. Different: store raw M first,
then request sndOutputMode(M). Ignore child return; no validation/rollback or
status assignment. Invalid values can therefore be retained in this game word;
a later identical invalid request skips the public child. No IRQ or source
synthFlags check at this game layer.

## Public mode stores, HRTF disable and fresh change comparison

sndOutputMode captures old synthFlags8041ee8c=G. Signed mode branches admit
exactly M0/1/2. Other raw values perform no local flag stores or HRTF request.

| M | Ordered fullword synthFlags stores |
| --- | --- |
| 0 | G OR1; then (G OR1) ANDfffffffd |
| 1 | G ANDfffffffe; then G ANDfffffffc |
| 2 | G ANDfffffffe; then (G ANDfffffffe) OR2 |

Mode1's second store derives from retained G, not a fresh read of the first
store. All modes preserve other flag bits. After these stores each valid mode
requests hwDisableHRTF, even when final flags equal G. That complete three-word
leaf stores0 at word8041ef78 and returns; no active/old-value/IRQ gate or other
buffer/reset operation. The name does not prove an immediate DSP/audio outcome.

Then sndOutputMode reads synthFlags FRESH and compares fullword against G.
Equality returns; difference admits propagation. The fresh comparison also runs
for unsupported M, so concurrent flag changes are not excluded by mode invalidity.
No public IRQ guard surrounds this flag/voice propagation section.

## Voice dirty publication before stream propagation

On changed flags, scan ordinals ascending from0 under synthInfo803f8540.byte210,
reread at EACH loop condition. For each ordinal freshly load synthVoice8041ee90,
compute V=base+wrap32(ordinal*458). Read flags114 then118; store118 unchanged,
then114 OR2000. No active/type/ownership/state gate, range cap, generation check
or dirty comparison. Count changes can grow/shrink this loop; base changes can
redirect later ordinals. Count0 skips writes but still requests stream propagation.

After voice scan request streamOutputModeChanged. There is no recomparison of
mode flags or rollback after that child. Actual count/base/IRQ/provider invariants
remain separate from these local stores.

## Complete stream scan and intermediate mode stores

streamOutputModeChanged first requests hwDisableIrq80292810. Ignore return and
retain streamInfo803f98e8 row pointer plus synthInfo803f8540 count pointer.
Visit ordinals ascending, stride68hex. Fresh count byte210 supplies EACH loop
condition; no local64-row cap despite declared stream table having64 rows.

For each row read state+c. Zero skips every row write/request. Nonzero:

1. Read requested pan byte5e, then capture synthFlags. Store requested pan at5a.
2. Read requested surround byte5f, store at5b. The flags capture occurred before
   this read/store; no later recapture within the row.
3. Captured mask1 set: overwrite5a=64 then5b=0. Otherwise mask2 clear overwrites
   only5b=0; mask2 set leaves the copied requested bytes.
4. RELOAD state+c. Exactly3 skips hardware mixing; every other fresh value,
   including0/unknown nonzero, proceeds. Original state3 still received pan writes.

Requested5e/5f stay untouched. No requested-versus-effective change test,
state normalization, dirty publication or hardware-ID lookup. A later row reads
mode/count anew after earlier children; the whole scan has no stable snapshot.

## Inline hardware mix and exit

Admitted mix reads bytes59,5c,5d,5a,5b, then word4c. Each volume/aux byte converts
with unsigned bias80429180 then separate single multiply by80429178 bits3c010204,
as in the prior mix leaf. Request
hwSetVolume(word4c,0,scaled59,scaled5c,scaled5d,byte5a<<16,byte5b<<16)
at8027b2f8. No SetHWMix child call: this body inlines the same arithmetic/ABI.
No127/gain clamp, child-status gate or duplicate-request suppression.

After loop request hwEnableIrq802927d8 unconditionally, even count0; restore
stack/registers and return. There is no saved prior IRQ-state restore parameter
in this caller. Actual wrapper/OS/reentrancy/concurrency semantics require the
linked IRQ contracts and runtime domain, not an assumption of nested restoration.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_output_mode_propagation_evidence.json)
pins nine spans (four complete bodies/three linked children/two constants),all152
body instruction anchors/eight call sites. Eight public-mode/eight state/four
fixed-count/six game-gate vectors checked, including invalid/unchanged requests,
HRTF-without-propagation, state3 pan-only update and count beyond64. Fixed-count
vectors do not claim the source count remains stable across children. Generic
verifier checks source hashes/anchors/calls, not semantic/provider deductions.
Fresh ELFs/listings/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: actual game/UI callers/preference lifetime, flags/count/base/HRTF/state/
voice/resource providers and valid extent/value/alias/lifetime/IRQ/concurrency
semantics; stream engine/lifecycle/transitive DSP/hardware consumers and original
mode/stream/audio traces. Four local bodies answered; EVT-01 and native binding OPEN.
