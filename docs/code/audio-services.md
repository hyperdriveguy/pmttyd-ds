# Audio services and numeric domains

The [runtime guide](runtime.md) covers streaming/NDSP ownership. This guide
covers source-like sound slots and field/opening adapters. Original behavior
evidence remains under [audio contracts](../behavior/README.md). Source SFX IDs,
allocation handles, native music-track selectors and NDSP channel numbers are
separate numeric domains. Hexadecimal masks below are values, not bit indices.

## Logical positional sounds

[positional_sfx_registry.cpp](../../source/audio/positional_sfx_registry.cpp)
retains 40 typed slots. A handle packs slot in low 8 bits and generation in the
next 16; generation wraps after 65,536 allocations. Only `check` compares that
generation. Volume/distance access and `off` operate on current slot occupancy,
so a stale handle can affect a reused slot. Native out-of-bank access returns
unavailable (`nullopt`/false), distinct from the source absent result -1.

Initialization resets only key/engine/flags, preserving other metadata on later
reinitialization. Volume reads return zero for -1/empty; setters narrow to byte
or halfword without clamping and succeed as no-ops for -1/empty. Off skips backend
work for queued mask 1 or engine -1. Otherwise low 13 key bits select descriptor:
mask `0x80000000` selects stream, else `0x40000000` song, else effect. Key/engine
clear only after actual backend success; generation and other fields survive.

[positional_sfx_start.cpp](../../source/audio/positional_sfx_start.cpp) allocates
a pending logical slot. This does not make a sound audible. `-1` is a source
rejection; `nullopt` means unavailable and may follow visible mutations. This
whole helper has no resumable cursor or rollback, so callers must pre-admit its
providers rather than blindly replay after a late failure.

| Original 20-byte big-endian row | Field |
| --- | --- |
| `0/4` | Name pointer / descriptor word |
| Decimal `12/13/14/15` | Row volume / pan / flag selector / mode |
| Decimal `16/17/18` | Priority / delay / halfword variation |

Negative request keys are guest name tokens: scan 4,242 names, default to row
zero on miss. Numeric keys retain upper modifier bits while low 13 select row.
Zero descriptor, global mute mask 4 without override, cooldown and story guards
can reject. IDs 91/2250/446/447/517 share a six-visit cooldown; `psndMain` owns its
decrement. Cooldown publication precedes story admission. Story <=2 rejects 869.

The first slot scan retains the **last** free slot. Matching key with mask
`0x100` can advance a shared 16-bit retrigger preference by eight. Only a full
bank uses the second scan: first slot whose priority is numerically greater
than incoming priority, with actual Off before reuse. This is not an age-based
or highest-priority victim search.

Low byte 255 selects default volume 127 / row pan, even if the original request
integer is outside byte range. Initial slot flags are `0x101`; other exact
modifier mappings are commented beside publication. Delay converts narrowed
16-bit milliseconds through `1000/live rate`, adds preference and row delay,
then uses the provided truncation service. Missing rate/truncation can leave
published key/flags/volume intact. Retrigger delay is five times preference,
narrowed to 16 bits. Pitch 32000 is source metadata, not proof of a WAV rate.
Position mode zero sets mask 2, other modes mask 4 for later processing.

[field_sfx_ndsp.cpp](../../source/audio/field_sfx_ndsp.cpp) attaches already
owned effects or pending slots. Forty logical slots share four dedicated native
channels 20–23, not forty concurrent voices. Live effect attachment requires
descriptor `0x20000000` with stream/song masks clear; pending attachment uses
engine -1. Duplicate live channels and conflicting descriptors for the same
low-13 key are rejected. `canOff` follows occupancy/queued/engine gates and
does not introduce a generation test. Clear stops earlier slots once, retaining
a failed and later slot for retry; it never invents backend support.

## Sound flags and Mario cleanup

[source_sound_flags.cpp](../../source/audio/source_sound_flags.cpp) ORs a mask
once into a retained low-16 word. A per-call cursor preserves publication and
completed fades across holds. Mask 1 selects BGM groups 0/1, each with source
1000-millisecond fade and flags zero. After both callbacks, a **fresh** mask-2
read selects environment groups 0/1. Callback mutation can affect the second
gate without replaying earlier calls. Clear changes bits without fading. A new
mask during a held call does not become a new publication; use a fresh cursor.

[mario_motion_sfx.cpp](../../source/audio/mario_motion_sfx.cpp) cleans handles
for the **old** motion. Motion 0 cancels idle then secondary, short-circuiting
on failure; motion 10 cancels fall. Motions 1/2/3/11/15 need no cleanup here;
other motions remain unavailable. Each handle becomes -1 only after successful
Off, so a partial success survives retry. [Native binding](../../source/audio/audio_field_mario.cpp)
checks readiness both at installation and invocation. Presentation mute remixes
three retained BGM slots under the mutex, while preserving logical stream clocks.

## Track selection and fanfare lifetime

[audio_policy.hpp](../../include/audio/audio_policy.hpp) uses native track
selectors: 0 field, 1 battle, 2 victory, -1 silence; Switch district variants
13–16, underground 11 and battle-think 17. Enabled professor override applies
last. Tracks 18/19 always resolve original row-217/219 item/coin fanfares,
independent of optional soundtrack. Source book waits retain the original loop,
so track 5 uses original music even in Switch mode; no invented loop points.

The PCM parser accepts the canonical 44-byte little-endian WAV header only:
format chunk 16 bytes, PCM format 1, 16-bit mono/stereo, 8–48 kHz, consistent
block/byte rates, whole frames and RIFF size `dataBytes+36`. It does not scan
ancillary chunks or validate supplied payload length; callers supply readable
header bytes and perform their own length checks.

[audio_item_get.cpp](../../source/audio/audio_item_get.cpp) keeps field stream,
fanfare group and detached tail as distinct owners. Resource-attempt caching
includes failure. Fanfare admission requires exact 32-kHz stereo frame counts
83,888 / 65,352, source volume 80, pan 64 and no loop sidecar. Preparation retains
field track zero at base/global volume 70/100, including muted presentation's
logical stream. It needs a free one of three streams and no current group/tail;
replacement and eviction are not fabricated.

EOF alone does not release a voice while NDSP buffers remain queued/playing.
A group check returns -1 after detachment even if the tail is audible. BGMMain
publishes/detaches before SoundSSMain, allowing the first tail subtraction in
the same admitted visit. Source envelope updates use simulation rate, not render
FPS or audio-buffer completion. Field mute/resume converts 1000 milliseconds
and retains file/cursor (`NULLcontinue`), without reopening/seeking/restarting.
Public operations hold `musicMutex`; internal `itemGetBgmMain` assumes that lock.

## Opening adapters

[audio_opening_house.cpp](../../source/audio/audio_opening_house.cpp) delegates
to its PCM owner and reports unavailable NDSP startup without a fallback cue.
[audio_opening_map.cpp](../../source/audio/audio_opening_map.cpp) services map-turn
sound before publishing the opening sequence's logical BGM snapshot. It attempts
a sound once per admitted transition request, resetting the attempt only when
request/enabled state clears. A failed load is not retried every render call.

Map-turn mono PCM is bounded to 512 Ki frames (about 1 MiB), cached in linear
memory and cache-flushed for NDSP. Dedicated channel 19 is distinct from the
field adapter's channels 20–23. The 12-entry mix matrix preserves source volume
100/pan 64 scaled by 127, including unequal left/right gains. These native sample
adapters do not establish complete MusyX macro/spatial/event playback fidelity.
