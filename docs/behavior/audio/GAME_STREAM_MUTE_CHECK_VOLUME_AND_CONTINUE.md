# Game stream mute, raw check, volume read and continue

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSMuteOn800dd940/28,
SoundSSMuteOff800dd918/28, SoundSSCheck800dd968/18,
SoundSSGetVolCh800ddbd8/44 and SoundSSContinueCh800ddf14/40hex.
Offsets/addresses/masks hexadecimal; shifts decimal. Static source deductions,
not original game-stream/playback/audio execution. Read
[positional SFX startup](POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md),
[SFX completion](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md) and
[outer sound frame](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).

## Common raw channel pointer and flag gate

Input channel I is retained fullword without low-byte/index truncation. Compute
wrap32(I*138), read pointer word803e0238 (sound state803e0138+100), form row V
from captured base+offset. Read U16 flags at row0. No channel-count/range/null/
active-system/generation/member/lifetime check, IRQ request or hardware query.
Each function captures the base for its own work; there is no shared whole-call
snapshot or base recapture after row writes.

SoundSSCheck simply returns that raw unsigned halfword as fullword0..65535.
It does not normalize nonzero to1, decode a playing bit, inspect MusyX ID/DSP
state or query completion. The source SFX wrappers' zero/nonzero decisions must
remain separate from the raw flags returned here.

## Mute leaves and zero-gate behavior

SoundSSMuteOn: flags0 returns without writes. Every nonzero flags stores low16
(flags OR2) to row0. SoundSSMuteOff: same initial zero gate, then store low16
(flags ANDfffffffd). No independent result assignment or hardware-volume write.
No other row/volume field changes, child call, callback or dirty publication.

Off can therefore turn raw flags2 into0, which the check leaf subsequently
returns0. It does not universally preserve a separate active bit. On flags0
does not allocate/start/reactivate a channel. These raw examples do not prove
that actual flag producers admit every halfword state, or that audible playback
changes immediately with this bit store.

## Volume query conversion and inactive return

SoundSSGetVolCh: flags0 returns0 WITHOUT reading float+c. Nonzero freshly loads
float+c, applies signed fctiwz, spills result into its16-byte stack frame and
returns the low32 converted word. Finite in-range values truncate toward0:
127.9 single returns127, negative-1.9 single returns wordffffffff. No unsigned
byte cast, volume127 cap, rounding increment, fade/current-state interpretation
or physical unit guarantee.

Out-of-range/NaN/infinity/FPSCR/exception/conversion domains remain unresolved;
no local repair, finite check or status branch appears. Mathematical null
outputs in evidence are not a source return value or no-op claim. Query does
not mutate row fields or request playback/hardware services.

## Continue publication order and fresh flags

SoundSSContinueCh: flags0 returns without writes or float read. Nonzero:

1. Store captured flags with bit2 cleared to row0.
2. Freshly load float+c, store it to float+14. No arithmetic or whole-record copy.
3. RELOAD flags at row0, store fresh flags OR10 as halfword.

The final OR uses the post-store/post-float-copy flag read, not a retained
pre-call snapshot. For stable input flags2: first store0, then copy volume, then
store10; no re-admission test after first clear. Other fresh bits are preserved
by the final OR. Special float load/store encoding and valid source/alias domains
remain processor/provider obligations; no exceptional-value bit-identity claim.

No current float+c overwrite, countdown reset, producer/hardware/stream
activation request or success result assignment. This closes local field
publication, not the main engine's consumption or original continue timing.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_mute_check_continue_evidence.json) pins
five complete spans,all59 instructions,zero calls. Seven flag/four fresh-continue/
seven volume/five raw-address vectors checked. Finite volume examples use host
single representation and convertible signed32 values; exceptional outputs left
unresolved. Generic verifier checks hashes/anchors, not semantic/provider
assumptions or original execution. Fresh full ELFs/listings/generator remain
`/tmp`; no bodies/binary assets committed.

Remaining: SoundSSMain/Int/play/fade/stop/frequency/pan/volume writers, producer/
decoder/DVD/DMA callbacks and actual channel/base/flag/float/index/IRQ/providers;
valid extent/value/alias/lifetime/concurrency/FP/conversion domains, transitive
MusyX/ARAM/AI/DSP and original mute/check/continue/fade/audio traces. Five local
leaves answered; EVT-01 and full native/full-game binding remain OPEN.
