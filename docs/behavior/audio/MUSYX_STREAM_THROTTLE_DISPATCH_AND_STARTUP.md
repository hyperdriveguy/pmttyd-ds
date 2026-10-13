# MusyX stream throttle, row dispatch and startup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamHandle REGIONS
8027a6ac..8027a8f4 and8027af50..8027af64, ends exclusive. Whole body
8027a698/8e4hex fingerprinted; state2 refill/ABI remainder stays open. Offsets/
addresses/masks hexadecimal; shifts/counts decimal. Static source deductions,
not original stream/audio execution. Read [frame caller](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md),
[initialization](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md),
[sample initializer](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md),
[hardware gain](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md),
[filter](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md),
[start](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md) and
[buffer/flush bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Throttle and live row dispatch

Read U8 streamCallCnt8041eeb5. Nonzero decrements/store byte, then skips all row
work to ABI epilogue. Zero reads streamCallDelay8041eeb4, stores that byte to
counter, initializes row pointer streamInfo803f98e8/ordinal0, then tests count.
For stable delay D, admitted scans are D+1 handler invocations apart; this is a
mathematical invocation count, not measured frames or original audio timing.

Each loop condition freshly reads synthInfo803f8540.byte210. Ordinal<count admits
current row, stride68hex; count0 skips rows. No local64-row cap despite declared
stream table64 rows. Read state+c: exactly1 enters startup, exactly2 enters the
remaining refill region8027a8f4, all other bytes skip. Loop tail increments
ordinal, advances row68, then rereads count. No local IRQ/status/active-system
request or row/voice/address/lifetime check in these regions.

## Ordered stack descriptor and format-specific writes

On state1 retain row S. Read word54, then U8 byte0e; write stack descriptor D at
stack+08, D.word0=word54 OR40000000. Request hwGetStreamPlayBuffer(byte0e) at
8027a714; store returned raw word to D.word4. This buffer selector is not row
word4c's voice index. AFTER child read U8 format byte0d then word18=L. Store
D.wordc=0,word10=L,word14=0,word18=L. No return/error gate.

| Format | Additional descriptor/row/request operations |
| --- | --- |
| Exactly0 | D.byte1c=2; no D.word8 initialization |
| Exactly1 | D.word8=S+24; D.byte1c=4; request hwSetStreamLoopPS(fresh word4c,fresh byte0f); afterward reread byte0f and store it at row26 then27 |
| 2..255 | No D.word8 or D.byte1c initialization |

D.bytes1d..1f are never initialized here. Unknown formats continue startup with
incoming stack type/pointer bytes, not a default codec/error return. The sample
initializer copies ALL eight descriptor words, including incoming padding.
No whole stack descriptor clear or format validation may be inferred.

After format branch, freshly read I=word4c, synthVoice8041ee90 and that synth
voice's wordf4 at base+wrap32(I*458)+f4. Request at8027a7b4:

`hwInitSamplePlayback(I,0000ffff,D,1,ffffffff,synthVoice.wordf4,1,1)`.

Arguments are raw; full descriptor L/type/address values remain retained from
before this request. Child return ignored; no allocation/voice-generation check
or startup rollback. Initializer owns its transitive SRC/filter/ITD mutations.

## Filter, pitch, volume and start requests

After initializer, read row.byte61. Zero requests hwSetFilter(fresh I,0,0,0) at
8027a7d4. Any nonzero reads U16 half62 then64, requests hwSetFilter(I,1,A,B) at
8027a7ec. No coefficient validation/status check.

Freshly read requested frequency word54 then reference synthInfo.word0. Convert
both UNSIGNED32 through double bias80429180 and separate single fsubs. Single
ratio=requested/reference; single scaled=4096*ratio using80429188 bits45800000.
Apply fctiwz/spill, pass resulting word to hwSetPitch(fresh I,word) at8027a840.
No zero-reference/finite/range check; exceptional conversion/FP domains remain
unresolved. Finite mathematical vectors do not establish actual source rates.

Read bytes59,5c,5d,5a,5b and fresh word4c; inline the exact independent byte
conversions/multiplication by80429178 bits3c010204, then request hwSetVolume
(I,0,scaled59,scaled5c,scaled5d,byte5a<<16,byte5b<<16) at8027a8b0. No mode
recalculation, byte127/gain cap or SetHWMix child call. Read the
[mix leaves](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) for paired arithmetic.

Freshly read word4c then U8 byte60; request hwStart(I,byte60) at8027a8bc.
Ignore return. Store state+c=2 unconditionally AFTER that request. Earlier child
failure/status/mutation does not add an admission check or restore state1.

## Post-publication initial flush and row advance

After state2 store, freshly read flags word8. Mask00020000 set skips flush.
Clear reads source word14, then size word1c and selector byte0e; request
hwFlushStream(source,0,size,selector,0,0) at8027a8ec. No cache-size/alignment/
source lifetime/zero-length check or callback. Hardware bridge owns upload/IRQ
semantics. State2 is already visible before this gate and child request.

Both branches advance to loop tail8027af50; they do not run state2 refill for
the same row in this invocation. No row reclassification after startup children.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_startup_dispatch_evidence.json) pins14
spans (two regions/whole handler/eight children/three constants),151 region
instruction anchors/nine call sites. Five throttle/five state/four format/five
finite pitch/four flush vectors and exact descriptor/argument bindings checked.
Generic verifier checks hashes/anchors/calls, not semantic/provider assumptions
or original execution. Fresh full handler ELF/listing/generator remain `/tmp`;
no bodies/binary assets committed.

Remaining: state2 refill/ABI, actual delay/count/state/format/descriptor/voice/
frequency/filter/start/flags/source providers and valid extent/value/alias/
lifetime/IRQ/concurrency/FP domains, transitive hardware/DSP/ARAM consumers and
original startup/refill/stream/audio traces. EVT-01 and native binding remain OPEN.
