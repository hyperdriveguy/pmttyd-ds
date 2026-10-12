# Audio asset mapping and native slice playback

The port now has a local, reproducible audio catalog and five original GameCube
primary-sample fallbacks. `make all` packages them with the music; sound requests
are issued from simulation ticks. Native playback uses GameCube assets. The
Switch library supplies descriptive names and resolved wave dependencies for
future implementations, without silently substituting remake recordings.

## Catalog and evidence

Run `make audio-map` to regenerate:

- `local-assets/audio-map/catalog.json`: 3,218 named US GameCube BGM/SFX table
  records, all 1,247 GameCube sample-directory entries, and all 10,256 Switch
  archive sound records. Original IDs, table addresses, flags, parameters,
  stream aliases, macro references and wave dependencies are retained.
- `local-assets/audio-map/catalog.csv`: searchable category/name/ID/source and
  dependency rows for implementation planning.
- `local-assets/audio-map/raw/`: read-only ISO extracts and provenance sidecars.
- `local-assets/audio-map/pcm/`: selected GameCube fallback effects and music.

`make all` generates its separate small catalog under
`local-assets/audio-map/native/`, and installs five effect WAVs, three music WAVs
and `romfs/audio-map.json`. This avoids replacing the full Switch catalog during
ordinary native builds. Both libraries and RomFS derivatives are ignored by Git.
The full catalog requires the extracted Switch library described in
[Switch audio assets](switch/AUDIO_ASSETS.md); the native build needs only the ISO.

GameCube table spans are `psbgmlist` at `0x80315ac8`, length `0x1060`, stride16,
and `pssfxlist` at `0x80316b28`, length `0x14b68`, stride20, in US revision0.
The reader translates DOL addresses through section headers and records span
hashes. Empty BSS string sentinel `0x804218c8` is handled explicitly. String
metadata uses Shift-JIS; no message text/story dialogue is extracted here.

GameCube FX IDs resolve through `pmario.proj` group tables to object IDs, then
`pmario.pool` macros to direct STARTSAMPLE operands and `pmario.sdir` sample IDs.
An FX ID must never be used directly as a sample ID. Macro references are
potential dependencies, not a simulation of branches, calls, keymaps or layers.
The current selected cues each have one group binding and one direct sample.
Other entries can be empty, indirect, aliased or multi-sample and stay explicit.

Switch archive sound IDs resolve through their FWSD file ID and wave-sound
index, then note wave indexes to wave archive/member IDs and PCM filenames.
Stream records resolve to their extracted PCM files. Group-local conflicts are
retained on affected records as `group_variants`; these require the actual group
owner before choosing a wave. Dependencies do not imply playback order or mix.

Primary format inspection reference: [GCMusyXTools](https://github.com/soneek/GCMusyXTools/blob/master/MusyXToolsv2.cpp)
for macro STARTSAMPLE operand layout. The authored reader copies no implementation.
The existing local NintendoWare reference sources and licenses are recorded in
[Switch audio assets](switch/AUDIO_ASSETS.md).

## Game-use routing

Categories in the catalog are derived from original names. They are search
hints, not proof of a scene's trigger, story branch or scheduling. Use the exact
original sound request and its event condition when binding imported scripts.
When a Switch association is ambiguous, resolve the original GameCube name and
bank indirection; retain an unresolved record if that also needs more evidence.
Do not invent a sound's story condition or choose a random bank sample.

| Game use | Catalog selection | Implementation trigger / lifetime |
| --- | --- | --- |
| Overworld area music | `BGM_M_STG*`, `BGM_STG*`; Switch `BGM_MAP*` and named area tracks | Original room/event BGM request; retain playback through ordinary dialogue and adjacent rooms sharing the cue. Story requests can override it. |
| Rogueport outdoor music | `BGM_M_STG0_GOR1` / `BGM_STG0_GOR1` → `sound/stream/stg_gor2_32k.stm` | Current five outdoor rooms use this track. `stg_gor1_32k` belongs to the `BGM_M_STG0_CSN1` table entry, so it is no longer the outdoor default. |
| Other chapters/interiors | Original BGM entries preserve chapter/area tokens and exact stream aliases | Bind room init and original conditional event requests. Region names alone cannot identify alternate story versions or transition timing. |
| Ambient loops | `ENV*` / `SE_ENV*`, categorized `ambience` | Room/environment owner starts/stops loops; positional emitters move with their owner. GameCube environment sequence table interpretation remains open. |
| Dialogue windows | `SFX_WINDOW*`, corresponding named Switch window cues | Original window open/close/page lifecycle, not every interaction button. Current development notice only gets the harness confirmation cue. |
| Dialogue character sounds | Switch `SE_SYS_MSG_<speaker>` and GameCube named message cues | Original speaker/control-token dispatch and text cadence. Preserve speaker, message ID, branch and ordering; current story message playback is unbound, so no fabricated speech loop is attached. |
| Field movement | `SFX_MARIO_*`, `SFX_FLD_*`, Switch `SE_MAR_*` | Accepted takeoff, landing or recovered movement-sound request; each lifetime belongs to its movement owner. Only field takeoff is currently connected. |
| Battle attack/movement | `SFX_BTL_*`, `SFX_ENM_*`, unit-specific names | Original script/movement callback; stop retained movement voices on completion/cancellation. Native movement sound services remain unbound. |
| Battle impacts/status | `SFX_MARIO_DAMAGE*`, `SFX_PARTY_DAMAGE*`, enemy/material/status variants | Original damage/hit event after damage selection; harness uses one generic impact fallback pending unit-specific dispatch. |
| Action-command result | `SFX_AC_OK1`; Switch `SE_BTL_SYS_OK`, `Nice`, `Good`, etc. | Result producer chooses grade; harness player success uses the primary GameCube success sample. Grade/prize/audience producers remain unbound. |
| Guard/superguard | `SFX_MARIO_GUARD1`; Switch `SE_BTL_SYS_Guard` / `SuperGuard` | Defense result at enemy impact. Current guard uses its own sample; no superguard behavior is added. |
| Battle music/results | `BGM_ZAKO_BATTLE1`, original boss entries, `BGM_BATTLE_WIN*` | Encounter/results owner; harness uses normal battle and win1. Specific original tutorial/boss eligibility remains unverified. |
| Event scenes/fanfares | `SFX_EVT*`, `SFX_STG*`, `BGM_EVT*`, `FF*`; Switch event names retain area/actor/action tokens | Bind original event request and conditions. Do not play every event in a chapter on area entry. |
| Items/interface | Original item/cursor/select cues and named Switch equivalents | Accepted item/UI operation, with cancel/invalid choices kept distinct when those producers are implemented. |
| Unclassified resources | Catalog category `unclassified` | Retain IDs and aliases; additional caller/ownership research is required. |

## Current native effects

[Authored cue bindings](../config/audio_bindings.json) select these primary voices:

| Native cue | Original name | FX ID | Macro ID | Sample ID |
| --- | --- | --- | --- | --- |
| Confirm | `SFX_CURSOR_OK1` | `0x047e` | `0x04fd` | `0x0413` |
| Field jump | `SFX_MARIO_JUMP1` | `0x021e` | `0x02b0` | `0x020f` |
| Generic impact | `SFX_MARIO_DAMAGE1` | `0x0316` | `0x0393` | `0x02c9` |
| Action success | `SFX_AC_OK1` | `0x0489` | `0x050d` | `0x041e` |
| Guard | `SFX_MARIO_GUARD1` | `0x0317` | `0x0394` | `0x02ca` |

Success at enemy impact selects Guard; success on Victory selects no effect,
avoiding a second action-success sound over the victory music. Each cue uses its
own NDSP channel and bounded linear-memory PCM buffer. Repeated requests restart
that cue; different cues can overlap. Muting clears active effects and music.
Dialogue retains area music; defeat stops music until a result owner is bound.

These effects are explicitly **primary-sample fallbacks**. No macro pitch sweep,
envelope, child voice, reverb, positional attenuation or original voice-priority
policy is rendered. Jump uses an audible harness gain despite the original jump
table's zero volume. Generic impact is not a recovered enemy-specific sound.
Original source rates remain 16,000/22,050 Hz mono for these effects. Music stays
32,000 Hz stereo, with intro and source-loop points retained. Source win1 does
have a whole-file loop; it retains that loop. Files without loop sidecars now
finish instead of being forcibly repeated by the streaming backend.

## Validation and remaining work

Authored fixtures test FX/object/sample separation, group conflicts, opcode
alignment, Switch note/archive/member indirection, stream sound ID0, category
routing, guard/result suppression, scene music policy and PCM header rejection.
`make test` and `make all` are required. The isolated emulator boot/walkthrough
checks boot integration; it is not an audible comparison or hardware test.
The initial 2026-10-12 isolated walkthrough reported Audio unavailable because
its SDMC lacked DSP firmware. That blocker is now resolved using the user-supplied
`/home/carson/games/roms/dspfirm.cdc`, copied only into
`artifacts/azahar/data/azahar-emu/sdmc/3ds/dspfirm.cdc`. Source and copy SHA-256:
`8e213f3e71d2e3e45d1169bac6465a70eabeb22b303f1fa6d7679370ffad0f54`.
Settings now reports Audio service ready. Firmware is ignored and not embedded
in the port or committed. libctru's component lookup is documented in its
[NDSP initialization source](https://github.com/devkitPro/libctru/blob/master/libctru/source/ndsp/ndsp.c).

The firmware-enabled isolated walkthrough completed. A further focused test
routed only the emulator into a temporary PulseAudio null sink and recorded its
monitor, then unloaded the sink without changing default audio devices.
`artifacts/azahar/audio-verify.wav` contains stereo48kHz PCM16 output;
`audio-verify-stages.json` and `audio-verify-analysis.json` contain capture markers
and measurements. Settled area music has RMS3534.17 and peak17077;mute has
RMS/peak0 over75,373 stereo frames;restored music has RMS1460.95 and peak7133.
A normalized waveform correlation of0.49176 against the resampled packaged jump
sample peaks after the jump press, supporting field-effect playback in the mix.
These are emulator output measurements, not original-game timing/mix equivalence.
The first focused capture's instantaneous mouse click missed Settings;the final
capture used a held touch press and visually confirmed Sound OFF/ON. Only the
final focused capture is used for the mute/restore claims.

Logs: `/tmp/pm-audio-dsp-smoke.log`, `/tmp/pm-audio-dsp-capture.log`.
Focused input/capture/analysis scripts remain `/tmp/pm-audio-input.py`,
`/tmp/pm-audio-capture.py`, `/tmp/pm-audio-analyze.py`. No native code changed for
this validation. The tested3DSX SHA-256 is
`49061fb77f2b4585517604d7a8d04873c08ec8b1bafca74325c4bfa123c0ed66`.
The user's normal emulator profile, saves, firmware source and default sinks
were unchanged. Battle-effect variants,all-five-cue coverage,hardware behavior
and audible comparison with original execution remain unverified.

Next: bind original message/event and unit audio producers; interpret environment
sequences and complete MusyX/FWSD playback; resolve story-conditioned area/battle
music choices; compare audible loops/transitions/mix in Dolphin and on hardware.
Mapping coverage and working primary-sample playback do not establish full-game
sound fidelity or completion of the remake.
