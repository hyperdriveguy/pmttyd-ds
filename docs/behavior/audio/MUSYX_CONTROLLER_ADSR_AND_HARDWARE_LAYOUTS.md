# MusyX controller-derived ADSR and hardware layouts

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdSetADSRFromCtrl802803f8/12c,
hwSetADSR80290a18/1ac and adsrConvertTimeCents8028639c/58hex;
opcode22 stub80282f38/10. Addresses/offsets/masks hexadecimal; scalar literals
decimal. Static query/table/descriptor/field/conversion request order, not
original envelope timing/audio. Read [controller getter](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md),
[unsigned FP conversion](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md),
[hardware pool](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md) and
[fetch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md).

## Four live queries and unbounded table index

Retain voice V/command C. Each query separately reads fresh command ID,
V.byte121 channel and V.byte122 set; requests inpGetMidiCtrl(ID,channel,set).
No caller channel255 early return: that getter owns its returned0 policy.
Table byte offset=rotate(result,27)&000007fc, equivalently
`index=(result>>7)&511; offset=index*4`. No index<=128 bound. Original tables:
DLS float803bc7e4,129 words; MIDI time803bba34,129 unsigned words;
DSP scale→index803bc3e4,1024 bytes. Hash/size does not validate caller range.

Query/order/stack descriptor construction:

1. C.word0.high8 ID → DLS float[index], retained in f31 as sustain source.
2. Fresh C.word0 byte bits8..15 ID → timeTable[index], store stack word0.
3. Fresh C.word0 byte bits16..23 ID → timeTable[index], store stack word4.
4. Compute SINGLE fmuls(original literal1023,retained sustain float), request
   __cvt_fp2unsigned. Read scaleTable[RAW returned word] without1023 clamp;
   store half8=low16(193-byte). No signed/negative/table bound repair.
5. Fresh C.word4.low8 ID → fourth controller result/table index.
6. Store wordc=80000000; read timeTable[index] into a retained register;
   store word10=80000000, then that time value.low16 to stack halfa. Descriptor is at
   sp+8; all stated offsets are relative to it, with no whole-record clear.
7. Read fresh V.identityf4.low8, request hwSetADSR(slot,&descriptor,2).
8. Read fresh lowFlags118, store OR100hex. High flags untouched.

Child returns ignored except returned table/conversion values. Four channel/set/
command captures are not one controller snapshot. Sustain conversion happens
between query3 and query4. Cached first float survives later queries; no
curve/resource/matrix evaluation or field saturation added. Descriptor sentinel
words c/10 are constructed but hwSetADSR mode2 below does not read them.

## Complete hardware mode0 halfword layout

Inputs raw ordinal I, descriptor D, raw mode K. Mode=K.low8. Raw offset
wrap32(I*f8), pool pointer8041ef88; no low8 ordinal mask/bounds/active/IRQ gate.
Mode0 performs ordered operations, reloading pool before each destination:
bytea4=0; wordb8=U16(D.half0); wordbc=U16(D.half2); halfc0=
min(U16(D.half4)<<3,32767); wordc4=U16(D.half6). No descriptor type/tag check.
Then execute common flag publication below. Source half4 scales before clamp;
there is no signed sustain interpretation or silent whole voice zero.

## Hardware modes1/2 and unsupported modes

For mode1 OR2: retain raw ordinal*f8 offset; reload pool/store bytea4=1;
reload pool/store byteca=0. Mode1:

- Read D.word0, request adsrConvertTimeCents; read pool, store returned.low16
  zero-extended to wordb8.
- Read D.word4 FRESH, request same conversion; read pool, store low16 to wordbc.
- Read unsigned D.half8, arithmetic>>2 (nonnegative); cap at1023; read
  scaleTable[index]; read pool, store low16(193-byte) at halfc0.

Mode2 instead reads D.word0.low16→wordb8, then fresh D.word4.low16→wordbc,
then D.half8→halfc0, reloading pool before each destination. No conversion,
scale lookup or clamp. Modes1/2 finally reload pool and store D.halfa zero-
extended at wordc4; release is NOT sent through time-cent conversion here.

Every mode, INCLUDING unsupported low8>=3, computes raw ordinal offset, reads
pool, reads record.word24, stores OR10hex. Unsupported modes perform no other
field/descriptor reads but still publish that control bit. No independent
error/success result, rollback, DSP command/mail or measured ADSR start.
Low8 mode256 follows0; mode258 follows2. Pool reloads can split destination
records under unsupported mutation; descriptor aliasing can affect fresh inputs.

## Complete signed time-cent conversion wrapper

adsrConvertTimeCents(raw X) constructs unsigned double from X XOR80000000 plus
43300000 high word, subtracts original signed bias4503601774854144 using SINGLE
fsubs. This yields binary32-rounded SIGNED32 X. Multiply with fmuls by original
binary32 factor1.2715657859985185e-08 at804291d8.

Request pow(base DOUBLE2 at804291d0,that floating exponent)80272620. Its complete
transitive math semantics/environment are NOT closed by this wrapper's hash.
Round returned value to single with frsp; multiply with fmuls by literal1000 at
804291dc; request __cvt_fp2unsigned and return that word. The already-complete
conversion helper owns negative/NaN/upper-bound/truncation branches. No explicit
X80000000 sentinel branch, short-circuit zero, halfword output mask, pure integer
conversion or measured millisecond unit in this body. Hardware modes1/2 apply
their own later low16 truncation separately.

## Opcode registration, evidence and remaining source/runtime

Opcode22 stub passes V,&shared cstep8041ef08, ignores result and joins with
initial stop marker0 unchanged. Fetch already advanced PC; no implicit wait/
rendering-frame delay. Ordinary ADSR/pitch ADSR commands remain separate paths.

[Manifest](../../../tools/musyx_controller_adsr_hardware_layouts_evidence.json)
pins three bodies/one region/three linked identities/eight numeric data spans:
16 spans,122 anchors/11 calls/one registration. Original129 time words/129 DLS
floats/1024 scale bytes and conversion literals are numeric metadata/identities.
Seven controller-index/seven hardware-mode/four sustain vectors and descriptor/
mask/width/order deductions are static checks, not original execution. Full fresh
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.

Remaining: pow/transitive libm, other ADSR opcodes and actual controller/command/
rate/table/descriptor/mode/ordinal/flag/caller providers; valid index/float/value/
alias/shared-cell/IRQ/concurrency domains, hardware/DSP/envelope consumers and
original ADSR/controller/conversion/audio traces. EVT-01 remains OPEN.
