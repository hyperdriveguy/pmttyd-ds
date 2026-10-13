# MusyX synthesis initialization, row types, jobs and callback

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthInit802797f8/574hex,
synthSetMusicVolumeType80279700/28hex, sndVolume8027a2a0/60hex and
hwSetMesgCallback802907f8/8hex. Addresses/offsets/masks are hexadecimal;
counts, indices and numeric values are decimal unless marked. Static source
semantics; no original audio execution or hardware timing measurement.
Read [bootstrap](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md),
[volume producer](MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md),
[job insertion](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md) and
[fade/aux consumer](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).

## Entry stores and unchecked voice allocation

synthInit(F,N) retains raw voice-count word N, computes allocation bytes
wrap32(N*458hex), and uses retained global base803f8300. Before allocation:

1. Store raw F at synthInfo.word0=803f8540.
2. Store synthRealTime low word8041eea4=0, then high8041eea0=0.
3. Store word803f8500=1800hex inside synthTicksPerSecond.
4. Store synthFlags8041ee8c=0, synthMessageCallback8041ee94=0.
5. Request salMalloc(bytes); store returned pointer at synthVoice8041ee90.
6. Request memset(returnedPointer,0,bytes).

No active/previous-allocation/free/count/multiplication-overflow/allocation-null
check, failure return or rollback. The pointer is published before memset.
This body does not set synthInfo's voice-count byte or sndActive. Its bootstrap
caller supplies a previously retained byte as N, but raw direct calls here are
not truncated. N=0 still allocates/clears zero bytes then skips the voice loop;
other unsigned N use that exact CTR count. Do not replace a raw count with the
low byte merely because per-voice IDs are byte stores.

## Complete explicit per-voice initialization

The whole allocation is zeroed first. Then visit N records, ascending, stride
458hex. Each indexed store freshly reloads synthVoice; the paired word114/118
stores share one derived pointer. There are no child requests inside the loop.
Offsets below are relative to the current record; values are raw stored bits:

| Ordered constant fields | Width | Stored value |
| --- | --- | --- |
| f4 | word | ffffffffhex |
| 118,114,110 | word | 0 |
| 10c | byte | 0 |
| 121 | byte | 255 |
| 158 | word | 0 |
| 196 | byte | 0 |
| 194 | byte | 128 |
| 195 | byte | 0 |
| 184,174 | word | 00400000hex |
| 188,178,1a4,1a8 | word | 0 |
| 1bc,1bd,11c | byte | 0 |
| 11e | byte | 23 |
| 106 | byte | 0 |
| 197 | byte | 1 |
| 1c4 | word | 0 |
| 1c8 | halfword | 0 |
| 1ca | halfword | 32767 |
| 1d0 | word | 0 |
| 1d4 | halfword | 0 |
| 1d6 | halfword | 32767 |
| 140 | word | 25600 |
| 133,11f | byte | 0 |

After these, write byte8=ordinal.low8, byte9=255, byte14=ordinal.low8,
byte15=255, byte20=ordinal.low8, byte21=255, in that order. Increment the
record displacement by458hex and ordinal by1 per iteration. These are raw
initial fields, not proof of playback ownership or priority semantics. N>256
would wrap the stored IDs while retaining the full iteration/allocation count.
Remaining record bytes stay at the earlier memset baseline in a valid,
non-aliasing/non-concurrent allocation; no invented additional field writes.

## Complete fader initialization and overrides

All32 synthMasterFader rows at803f8754, stride30hex, are initialized in ascending
order. For EACH row store float0=0, float14=1, byte2d=4, in that order.
The loop is two repetitions of16 unrolled rows. It does not clear the whole
600hex-byte table: float4/8/c/10/18/1c/20/24, word28 and byte2c are untouched.
Zero and one literals are80429158=00000000 and80429130=3f800000.

Then store active bitmap8041ee88=0, pause bitmap8041ee84=0. Override row31's
byte2d=1; override rows23..30 byte2d=0 ascending; set row21.float0=1, then
row22.float0=1. Final local state:

| Rows | Current float0 | Pause current float14 | Type byte2d |
| --- | --- | --- | --- |
| 0..20 | 0 | 1 | 4 |
| 21..22 | 1 | 1 | 4 |
| 23..30 | 0 | 1 | 0 |
| 31 | 0 | 1 | 1 |

No local type2/3 row or universal current1 is initialized. Relate these exact
raw types to volume's special selector scans; do not assign music/effect roles
without the actual callers/type writers. Reinitialization leaves the untouched
fader payload from before; zero bitmaps alone do not erase it.

## Remaining initializer order and exact cleared domains

After faders, request inpInit(0)8028fcc8. Its internal controller initialization
is separate. Then clear the following local domains, interleaved by studio
ordinal0..7 (not six independent bulk resets):

| Domain | Exact local reset |
| --- | --- |
| synthAuxACallback803f8f34 | Eight words0 |
| synthAuxAMIDI8041ee7c | Eight bytes255 |
| synthAuxBCallback803f8f74 | Eight words0 |
| synthAuxBMIDI8041ee6c | Eight bytes255 |
| synthITDDefault803f8f94 | Sixteen bytes0, two per studio |

Per studio stores are A callback, A MIDI, B callback, B MIDI, ITD byte(2i+1),
ITD byte(2i). Aux user-word arrays and MIDI-set arrays are not written here.
A/B association255 skips the respective consumer callback; do not infer that
all callback/user/set payloads are zeroed or that later installations are safe.

Next request macInit8028438c, vidInit802843f4, synthInitAllocationAids80285328,
in that order. Child results ignored. Then zero16 consecutive words at
synthGlobalVariable803f8fa4 and request voiceInitLastStarted80286138.

After that request, zero96 consecutive words at synthJobTable803f8d94..803f8f10,
covering32 slots ofthree words/chex bytes. Four unrolled blocks ofeight slots
use an ordinal increment8 and a low8 ordinal test against32. No linked-job
removal/voice cleanup is requested by this table reset itself. Store
synthJobTableIndex8041ee61=0 only AFTER clearing the slots.

Finally request hwSetMesgCallback(synthHWMessageHandler80279728). The COMPLETE
setter stores its raw pointer into salMessageCallback8041ef8c and returns; no
IRQ/null/type/context gate. The handler is a linked callback target here, not a
recovered complete callback contract. synthInit assigns no independent success
code, enables no hardware/IRQ and leaves sndActive unchanged; the enclosing
bootstrap later activates it. Callback dispatch/nesting and other children
remain separate requirements.

## Complete public volume and row-type setter

sndVolume(V,T,G) retains three raw words, requests hwDisableIrq, then
synthVolume(V,T,G,0,ffffffff), then hwEnableIrq. No sndActive/validity/bounds
precheck, input truncation or retained status across Enable. The synthesis
child owns its low8 selector/volume and low16 time behavior. This wrapper's
missing ID prevents immediate zero-duration sequence dispatch in the ordinary
non-concurrent direct-row case; no independent source ownership proof is made.

synthSetMusicVolumeType(G,Type) first reads sndActive8041ee60. Zero returns
without writing. Any nonzero selects803f8754+G.low8*30hex and writes
byte2d=Type.low8. No group bound, type enum validation, special-selector route,
IRQ request, bitmap/current/coefficient modification or success status.
Thus group32..255 derives a type-byte write past the32-row table. The source
requires a caller bound for valid direct use; contents/execution of invalid
addresses are not claimed. Group256 wraps to0 at this setter's low8 selection.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_synth_initialization_evidence.json) pins four
complete bodies,13 linked fingerprints including the callback target, two
numeric literals:19 spans,279 instruction anchors,13 resolved direct calls.
32 fader rows,96 job-word addresses,34 constant voice fields/three ordinal
fields,seven allocation/count vectors,six type-setter vectors and six reset
domain descriptors are source-derived checks. ABI save/restore entry hashes do
not close their entire fallthrough chains. Fresh full inspection ELFs/listings
stay `/tmp/inbox-synthInit.elf`, `/tmp/inbox-synthSetMusicVolumeType.elf`,
`/tmp/inbox-sndVolume.elf`, `/tmp/inbox-hwSetMesgCallback.elf`.
No original body/assets committed or original code executed.

Remaining: controller/allocation/voice/callback child semantics, other row/
bitmap/aux/job writers and callback installations, actual caller counts/groups/
IDs/time/mode/type providers and bounds/null/overflow/alias/IRQ/concurrency/
reinit invariants, original initialization/reinitialization/scheduling/audio
traces. These local services are answered; EVT-01 and full native/source/runtime/
hardware equivalence remain OPEN.

### Installed callback follow-up

[Complete hardware-message/sample notification graph](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
now closes the installed handler and its virtual start/end, hardware-ID and
macro-end local bodies. Callback implementations/table providers, linked
hardware/ID services and original message/scheduling/audio traces remain open.

### Controller initialization follow-up

[Complete inpInit/controller reset/defaults](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
now pins the exact global child, per-voice branch and common frequency-default
writes, plus cold/warm/last-note/channel-default services. Actual callers/
controller/filter/dirty consumers and original controller/audio traces remain open.
