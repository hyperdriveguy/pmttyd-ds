# MusyX stream mix parameters and local stream.c coverage

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamMixParameter8027b814/564hex,
345 instructions, plus full GPR25..31 helper spans. Offsets/addresses hexadecimal;
stages/counts decimal. Static source deductions, not original mixing/audio
execution. Read [lookup/mode/mix leaves](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[hardware volume](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Retained inputs and three lookup stages

Retain raw ID, Volume, Pan, Surround, AuxA from r3..r7. Each entered stage requests
hwDisableIrq, then selects first nonzero-state exact fullword-ID match among64
streamInfo803f98e8 rows, stride68hex. Stages0/1 inline eight iterations of eight
row checks; stage2 requests GetPrivateIndex at8027bce0. No live count, sentinel-ID
rejection, voice-active/generation/resource check. Missing ID unwinds entered
stages without undoing earlier stores/requests.

| Stage | Disable | Hardware mix site | Own enable |
| --- | --- | --- | --- |
| 0 | 8027b83c | hwSetVolume at8027ba94 | 8027bd5c |
| 1 | 8027bab0 | hwSetVolume at8027bcc4 | 8027bd58 |
| 2 | 8027bcd8 | SetHWMix at8027bd2c | 8027bd54 |

## Three exact publication routes

All found rows receive volume59=low8(Volume),AuxA5c=low8(AuxA),AuxB5d=0,
requested pan5e=low8(Pan),surround5f=low8(Surround). There is no AuxB argument;
this setter forces it0 even for states not currently receiving hardware mixing.
The routes differ in source read/store order:

- Stage0 captures synthFlags BEFORE these five stores. Then select effective
  pan/surround: mask1 set→64/0; otherwise mask2 clear→raw Pan/0;
  mask2 set→raw Pan/Surround. Store low bytes at5a then5b.
- Stage1 first stores raw low-byte Surround to stack+08, Pan to stack+09.
  Request SetupVolume(row,Volume,AuxA,0) at8027bc1c. Reload stack pan/surround,
  store requested5e/5f, then request CheckOutputMode(&stack09,&stack08) at8027bc38.
  Reload both stack bytes and store effective5a/5b. This helper captures mode
  after volume/requested-field stores and may leave the stack inputs unchanged.
- Stage2 requests SetupVolumeAndPan(row,Volume,Pan,Surround,AuxA,0) at8027bd18.
  Its existing contract owns five ordered fields, subsequent mode capture and
  effective bytes. No source alias/order justification collapses all three routes
  to one fresh-mode snapshot taken at an arbitrary common point.

All local stores narrow bytes, with no127 clamp, equality suppression, state
normalization, frequency/cursor/voice/filter reset or whole-record clear. Original
raw arguments remain retained for later companions and recursion; effective
mode-overridden bytes do not replace the requested arguments.

## Fresh state2 hardware request

AFTER publication each stage rereads state+c. Only EXACT2 requests hardware
mix. Other fresh states retain fields without a mix call. Stage0/1 inline the
prior mix leaf: read live bytes59,5c,5d,5a,5b and raw voice word4c; convert three
volume/aux bytes with unsigned double bias80429180, then separate single multiply
by80429178 bits3c010204. Request hwSetVolume(voice,0,scaled59,scaled5c,scaled5d,
byte5a<<16,byte5b<<16). No gain/voice/result validation or dirty comparison.

Stage2 calls SetHWMix(row), which owns the same local ABI/arithmetic. Byte5d was
locally set0 but is reread live; alias/concurrent/child mutations remain domain
questions. Child return ignored; no rollback or state/ID recapture. Source255
can scale above1; no universal native volume normalization follows here.

## Fresh companions, recursive forwarding and unwind

After optional mix read row.word0 FRESH. Sentinelffffffff stops; otherwise next
stage consumes that raw public ID. Stage2 non-sentinel requests
sndStreamMixParameter(ID,Volume,Pan,Surround,AuxA) at8027bd50 with original
retained raw arguments, ignoring return. Three IDs are processed locally before
recursion. No cycle/depth/visited guard, all-row transaction or duplicate handling;
state3/unknown nonzero rows still receive fields and follow links.

Unwind entered stages in reverse order: full path8027bd54,bd58,bd5c. No saved
prior-state token or independent success/failure return assignment. IRQ/chain/
stack/provider domains remain separate from request-count parity.

Frame allocates80 decimal bytes, saves LR at frame+54hex and uses complete
_savegpr_25/_restgpr_25 fallthrough (GPR25..31 at oldSP-28..-4 then blr).
Full32-byte helper spans pinned alongside CSV4-byte entry boundaries.

## Audited local stream.c function inventory

The supplied US symbol CSV tags exactly17 .text functions as `musyx.a stream.c`.
Each now matches a complete-body or composed-body source contract and full-body
fingerprint. This is coverage of that declared inventory; indirect/dynamic code,
external callers/providers and original execution are separate obligations.

| Tagged functions | Local source contract |
| --- | --- |
| streamInit,streamKill | [Init/kill](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md) |
| SetHWMix,GetPrivateIndex,CheckOutputMode,SetupVolume,SetupVolumeAndPan | [Lookup/mode/mix](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) |
| streamHandle | [Composed handler](MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md) |
| streamCorrectLoops | [Frame/no-op leaf](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md) |
| streamOutputModeChanged | [Mode propagation](MUSYX_STREAM_OUTPUT_MODE_PROPAGATION.md) |
| sndStreamAllocEx,sndStreamAllocLength | [Allocation/length](MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md) |
| sndStreamActivate | [Composed activation](MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md) |
| sndStreamDeactivate | [Deactivation](MUSYX_STREAM_DEACTIVATION_AND_COMPANION_CHAIN.md) |
| sndStreamFree | [Free](MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md) |
| sndStreamFrq | [Frequency](MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md) |
| sndStreamMixParameter | This contract |

[Manifest](../../../tools/musyx_stream_mix_chain_evidence.json) pins15 spans,
361 anchors (345 body/16 helper),16 calls. Six byte/mode/gain vectors/fifteen
stable acyclic-chain models checked. Seventeen tagged functions matched by
name/address/size to complete-body/composition evidence, all prerequisite manifests
reverified. Generic source verifier checks hashes/anchors/calls, not semantic
models/inventory completeness or runtime assumptions. Fresh full ELF/listing/
generator remain `/tmp`; no bodies/binary assets committed.

Remaining: actual callers/state/ID/parameter/mode/voice/companion/IRQ/resource
providers, valid value/index/extent/encoding/alias/lifetime/concurrency/stack/ABI/
termination/FP domains, transitive ARAM/DSP and original allocation/start/refill/
mode/mix/frequency/free/chained failure/audio traces. EVT-01 and full native/game
binding remain OPEN despite complete local tagged-function coverage.
