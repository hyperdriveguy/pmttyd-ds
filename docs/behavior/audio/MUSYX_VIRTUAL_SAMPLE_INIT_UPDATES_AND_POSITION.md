# MusyX virtual-sample initialization, updates and position

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete vsInit80286b5c/bchex,
vsSampleUpdates802872e4/210hex, hwGetVirtualSampleState80290be4/14hex,
hwVoiceInStartup8029161c/20hex, hwGetPos80291238/achex.
Addresses/offsets/masks hexadecimal; counts/indices decimal.
Static source semantics, not original playback/underrun measurements.
Read [notifications](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md),
[hardware fields](MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md) and
[frame call order](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).

## Correct table base, layout and complete initialization

T=80406b00, matching the `vs` BSS symbol of sizeb50hex and original
`lis 8040`/`addi 6b00` instructions. The earlier notification document's
803f6b00 prose address is corrected; its original code fingerprints and
relative ordering were unaffected.

| Field | Exact address/domain |
| --- | --- |
| Configured slot count | T.byte0=80406b00 |
| Loop size | T.word4=80406b04 |
| Slot i state pointer Q | T+8+i*2chex, i0..63 |
| Voice→slot mapping | T+b08=80407608,64 bytes |
| Generation counter | T.halfb48=80407648 |
| User callback | T.wordb4c=8040764c |

Q is eight bytes after the conceptual slot base used by the notification
contract; Q.byte0 is that document's S.byte8, Q.byte3=S.byteb,
Q.half12=S.half1a. This keeps the two offset origins explicit.

vsInit performs exactly: count byte0=0; map[0..63]=255 ascending (two groups
of32 unrolled stores); generation halfword=0; callback word=0. No whole-table
clear, loop-size write, slot state/payload reset, buffer release, hardware
request, IRQ/active gate or independent status. Reinitialization preserves
prior slot fields and loop size while removing their map/callback admission.
No callback installation or configured count producer is supplied by this body.

## Complete updater admission and mapping scan

vsSampleUpdates first reads T.wordb4c. Null returns without scanning or child
requests. Nonnull admits EXACTLY64 mapping entries, ascending voice ordinal
v0..63; configured count byte0 is never read. For each:

1. Read map[v];255 skips.
2. Request hwGetVirtualSampleState(v). Return0 skips.
3. REREAD map[v], select Q=T+8+freshMap*2chex without count/bounds/state check.
4. Request hwGetPos(v), retain raw unsigned word P.
5. Read Q.byte2 sample type. Type5 rounds the child argument down to a multiple
   of14; all other types use raw P. Retain both raw P and transformed L.
6. Read fresh Q.byte0 state and route below.

Mapping may change during the hardware-state request; there is no second255
check before selecting Q. The position request uses scanned ordinal v;
subsequent identity/voice requests use fresh Q.byte3, which need not equal v
without a provider invariant. The initial callback is not retained or rechecked
by this parent between entries. Actual child callbacks remain separate.

Type5 arithmetic is the original unsigned multiply-high by24924925hex,
subtract/shift/add/shift quotient sequence, then multiply14. It gives
L=floor(P/14)*14 across32-bit P. It is not nearest rounding, signed division or
rounding of remaining-count arithmetic. Raw P remains available below.

| Q.byte0 | Complete local route |
| --- | --- |
| 1 | vsUpdateBuffer(Q,L), ignore result; proceed to next mapping |
| 2 or3 | Identity check, buffer update, remaining-count/threshold handling |
| 0/4..255 | Skip without cleanup |

vsUpdateBuffer80286f8c is a linked child here; its refill/callback semantics
remain a required complete contract. No provisional frame duration inferred.

## States2/3 identity, raw position and remaining arithmetic

Request hwGetVirtualSampleID(fresh Q.byte3). Then read fresh Q.half12 and
Q.byte3; compose (generation<<8)|voiceByte and compare with returned full ID.
Mismatch stores Q.byte0=0 and clears map[fresh Q.byte3]=255; no buffer update,
voice break/kill, end callback or hardware loop clear in that mismatch path.

Match requests vsUpdateBuffer(Q,L), ignores its result, then reads fresh
previous position B=Q.wordc. If raw P>=B unsigned, delta=wrap32(P-B).
Otherwise delta=wrap32(fresh T.word4-wrap32(B-P)). Read fresh Q.word8,
store Q.word8=wrap32(oldRemaining-delta), then Q.wordc=raw P. No underflow
saturation, wrap-count validation, multiple-lap reconstruction or type5-rounded
position substitute. Child mutations of these words affect the fresh reads.

Next load synthVoice8041ee90, fresh Q.byte3 and fresh Q.word8. Read the selected
voice.half212. Threshold H is the original multiply160/add4095/arithmetic
shift12/addze sequence. Since the source operand is an unsigned halfword,
it equals floor((pitch*160+4095)/4096) with a positive intermediate. Compare H
with remaining as SIGNED32. H<=remaining skips cleanup; H>remaining proceeds.
Wrapped remaining words may be negative in this comparison. No actual pitch/
sample-rate/physical underrun deadline is measured from this local formula.

Request hwVoiceInStartup(the previously read voice byte). Nonzero skips voice
break/kill but STILL performs common cleanup. If zero, read fresh state:
state2 requests hwBreak(fresh Q.byte3), then reload fresh Q.byte3/synthVoice
and request macSampleEndNotify(selectedVoicePointer). Any other state requests
voiceKill(fresh Q.byte3); it is not re-gated to state3 after buffer mutation.
After these children, store Q.byte0=0 and clear map[fresh Q.byte3]=255.
No generation/map recheck, virtual end callback, payload wipe or loop-buffer
clear. Cleanup may use a callback-mutated voice byte; retained Q remains selected.

## Complete hardware state/startup leaves

Both select hardware voice from base word8041ef88+wrap32(raw I*f8hex), with no
low8 truncation, bounds/active/identity/IRQ check of their own.
hwGetVirtualSampleState returns zero-extended byte9c.
hwVoiceInStartup returns1 exactly when bytef0==1, otherwise0, using
subfic/count-leading-zero/shift. It does not test merely nonzero or byte9c.
These are distinct admission fields; do not collapse them into one state.

## Complete hardware position body

hwGetPos uses the same raw index/base/stride. If hardware bytef0!=2, return0.
Otherwise read type byte90. Let A=fresh hardware.word20 and B=hardware.word78;
loads occur B then A on the admitted computation paths. All subtraction and
multiplication below wrap32; shifts are logical:

| Type byte | Return |
| --- | --- |
| 0,1,4,5 | D=wrap32(A-2*B); block=wrap32((D>>4)*14); return wrap32(block+max((A&15)-2,0)) |
| 3 | wrap32(A-B) |
| 2,6 | wrap32(A-(B>>1)) |
| 7..255 | Raw input I remains in r3; early return without a position assignment |

For types0/1/4/5, the nibble is from RAW A, not from D. Unsigned nibble<2
returns block; otherwise add nibble then subtract2. Unknown types return the
actual retained register input, not an invented error/zero/fallback position.
The entire body has no child, write, clamp or generation/bounds check. Native
valid-domain callers must establish admitted types/index/base rather than
turning incidental returns into a new general API status.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_virtual_sample_init_updates_position_evidence.json)
pins five complete bodies/seven linked fingerprints:12 spans,149 instruction
anchors,11 calls. It records the corrected BSS layout,64 mapping-reset addresses,
six state/ten rounding/five remaining/six threshold/five startup/15 position
vectors. The multiply-high type5 formula was also checked for all65536 inputs
0..65535; recorded high32-bit examples extend the source-derived evidence,
not original execution. ABI entry hashes do not close complete fallthrough
chains. Full fresh ELFs/listings stay `/tmp/inbox-vsInit.elf`,
`/tmp/inbox-vsSampleUpdates.elf`, `/tmp/inbox-hwGetVirtualSampleState.elf`,
`/tmp/inbox-hwVoiceInStartup.elf`, `/tmp/inbox-hwGetPos.elf`.
No original bodies/assets committed or original code executed.

Remaining: complete vsUpdateBuffer, configured count/size/callback/map/slot/
remaining/type/pitch/hardware field providers, actual callback implementations,
valid raw-index/type/count/bounds/alias/generation/numeric/IRQ/reentrancy/
concurrency/reinit domains and original refill/underrun/position/start-end/audio
traces. Five local services are answered; EVT-01 and full native/source/runtime/
hardware equivalence remain OPEN.

### Refill child follow-up

[Complete vsUpdateBuffer](MUSYX_VIRTUAL_SAMPLE_REFILL_DESCRIPTOR_AND_CURSOR.md)
now pins contiguous/wrapped type5/6 descriptor writes, tag1 callback and fresh
post-callback cursor advancement. Actual callback/byte-transfer implementation,
field/result domains and original refill/underrun/audio evidence remain open.
