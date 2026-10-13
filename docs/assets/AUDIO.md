# Audio asset provenance

DSP validation firmware, sound mapping and GameCube fallbacks, Switch audio extraction, complete SFX sample extraction and the full loopable audio library. Split out of [ASSETS.md](../ASSETS.md) on 2026-10-12; sections are verbatim.

## Isolated native audio validation firmware (2026-10-12)

User supplied /home/carson/games/roms/dspfirm.cdc. Read-only source copied to
ignored artifacts/azahar/data/azahar-emu/sdmc/3ds/dspfirm.cdc;49,756 bytes,
SHA2568e213f3e71d2e3e45d1169bac6465a70eabeb22b303f1fa6d7679370ffad0f54.
Source/copy hashes match. No patch/conversion,embedding,source-history addition
or redistribution occurred;original firmware copyright/licensing remains in
force. This component is separate from GameCube/Switch game sound assets.

The prior isolated Audio unavailable finding is resolved:Settings shows Audio
service ready,private-sink PCM capture verifies nonzero music,field jump waveform
support,digital mute silence and restored output. Capture derivatives/metrics
stay ignored in artifacts/azahar/audio-verify*. No user emulator profile/saves,
firmware source or default audio device changed. Dedicated sink was removed.
No original-game audible equivalence/hardware/all-cue validation is claimed;
see AUDIO_MAPPING.md and the progress follow-up for exact measurements/logs.

## Sound mapping and GameCube native fallbacks (2026-10-12)

[Audio mapping](../AUDIO_MAPPING.md) documents a reproducible catalog of3,218 named
US revision0 GameCube BGM/SFX entries,1,247 sample directory IDs and10,256 Switch
sound records. Original DOL table hashes/addresses/flags/parameters and exact
stream aliases are retained;FX→group/object→direct macro/sample and Switch
sound→FWSD note→wave archive/member dependencies are separate identifiers.
9,679 Switch wave references and577 stream references resolve to existing PCM;
28 records expose conflicting group-local variants. Name-derived categories do
not establish original story conditions,event order or ownership.

Sources:read-only /home/carson/games/roms/G8ME01.iso sound/proj/pmario.{proj,pool,
sdir,samp},DOL sound tables and selected sound/stream files;existing Switch
local-assets/switch/audio library. Full raw/catalog/CSV/PCM outputs stay ignored
in local-assets/audio-map/. Native-only conversion uses its separate native/
subdirectory and ignored romfs/audio-map.json and five effect WAVs. Authored
config/audio_bindings.json and tools/audio_map.py contain no original PCM/code.

Fallback PCM is decoded at original rates/channels,without normalization or
resampling. One-shot effects omit sidecars while retaining source-loop metadata
in catalog. Primary samples do not render macro pitch/envelope/child voices,
reverb/spatialization or original voice-priority rules. Field jump deliberately
uses audible fallback gain despite its zero-volume original entry. Outdoor
Rogueport now uses the original GOR-table stream stg_gor2_32k rather than the
CSN-table stg_gor1_32k. Win1's original whole-file loop is retained;nonlooping
source files are no longer forcibly repeated. Complete original event binding,
hardware/Dolphin audible comparisons and native NDSP playback validation remain
open. Isolated Azahar lacks a DSP component and reports Audio unavailable.

Original assets retain original copyright;local conversion grants no
redistribution rights. No ISO,original banks,PCM,dialogue or compiled packs are
committed. GCMusyXTools is a primary STARTSAMPLE layout reference;no external
implementation was copied/compiled. NintendoWare sources/licenses remain as
recorded in switch/AUDIO_ASSETS.md. All source inputs were read-only;the automated
emulator used only the project-local isolated profile and test saves.

## Switch 1.0.0 audio extraction (2026-10-12)

Completed the separate Switch audio library from the verified read-only
`local-assets/switch/romfs/` extraction of the supplied base NSP. Ignored
`local-assets/switch/audio/` preserves all 670 sound-tree files and 594 structured
bank/control/group extracts, and contains PCM16 WAVs for all 577 Opus streams
and 9,691 DSP bank waves. The full library is separate from GameCube assets and
native RomFS. [Switch audio provenance](../switch/AUDIO_ASSETS.md) records paths,
source package identity, conversion choices, decoder build and licensing.

Manifest and archive inventory retain all 10,256 sound names/IDs, all 62 global
wave-archive records, source hashes/spans and external loop metadata. Group-local
file-ID conflicts are preserved as contextual variants rather than overwriting
global resources. WAVs preserve source rates/channels; 421 music tracks and
807 bank waves have native-format loop sidecars. PCM files total
11,385,943,684 bytes; source sound-tree files total 1,941,989,744 bytes.
Verified all 10,268 WAV formats/full lengths/hashes/loops, original FWAV header
fields for all 9,691 waves, and all 670 original/copied source hashes. All 22,453
library files remain ignored and no originals are tracked.

Decoder: external vgmstream r2117, commit7c520e8f9b078dfde2f84f1b5bcbad0c6c415806,
ISC with existing matching third_party notice. libpulsar MIT and Citric Composer
snapshots supplied format facts only; no implementation copied. Existing original
copyright and no-redistribution rights remain unchanged. No NSP/key/firmware,
source RomFS, emulator profile/save or native audio selection was modified.
These are decoded assets, not faithful runtime mixes/event playback; see the
linked provenance for source-domain, alias, codec and audit limitations.

## Complete GameCube SFX sample extraction (2026-10-12)

The earlier loop-only conversion is now extended to **all 1,247 samples** from
`sound/proj/pmario.sdir` and `sound/proj/pmario.samp` in the read-only US
`/home/carson/games/roms/G8ME01.iso`. `make audio-assets` refreshes all six bank
resources and converts every indexed sample into ignored
`local-assets/audio/pcm/samples/pmario-<four-digit-hex-ID>.wav`. The 1,035
nonlooped samples have no sidecar; the 212 looped samples retain their
`.wav.loop` intervals. All preserve original sample IDs, mono PCM16 data,
rates and lengths without resampling, normalization or generated replacements.

Sample WAVs total 39,383,724 bytes. Library schema 2 now records 1,393 outputs
(146 music tracks + 1,247 bank samples), totaling 930,759,732 WAV bytes.
Verified all WAV formats/full data lengths/hashes/sidecar bounds, exact coverage
of every directory ID, and all 152 raw-resource hashes. All 2,057 library files
are Git-ignored and none is tracked. Unchanged music can be reused only after
source/decoder fingerprints, WAV metadata, loop sidecars and PCM hash validation;
corrupt music is reconverted. The complete bank directory and original
`pool/proj/etbl/slib` resources remain preserved with extraction provenance.

This completes sample extraction/conversion, not MusyX effect rendering:
macro sequences, envelopes, pitch, layered voices and game event-to-effect
bindings remain unimplemented. Original copyright/no redistribution rights and
existing format-reference licensing remain unchanged; no additional third-party
implementation was copied or linked. Assets remain outside RomFS and source
history. No original-game listening or hardware audio equivalence is claimed.

## Full loopable audio library (2026-10-12)

`make audio-assets` / `tools/extract_audio.py` reads
`/home/carson/games/roms/G8ME01.iso` read-only. The completed local library is
under ignored `local-assets/audio/`, separate from the current RomFS pack:

- `raw/sound/stream/*.stm`: all146 original streams with extraction provenance.
- `raw/sound/proj/pmario.{sdir,samp,pool,proj,etbl,slib}`: six original MusyX
  resources. `sample-directory.json` preserves all1,247 indexed sample IDs,
  offsets, coefficients, rates, counts and loop bounds.
- `pcm/streams/<original-stem>.wav`:146 PCM16 tracks, retaining source32,000Hz
  and mono/stereo channels. All146 STM headers enable looping.
- `pcm/samples/pmario-<four-digit-hex-ID>.wav`:212 mono looped bank samples,
  retaining original rates and exact IDs. Nonlooped samples remain available
  in the preserved bank and can be converted with `tools/audio_convert.py`.
- `<file>.wav.loop`: two little-endian uint32 PCM frame indices `[start,end)`;
  indices apply before playback resampling, count frames rather than individual
  stereo samples, and preserve the intro before repeating the loop region.
- `manifest.json`:152 source records with ISO paths/offsets/sizes/SHA-256,
  358 output records with formats/loops/PCM hashes, and converter fingerprints.

Conversion uses the existing DSP ADPCM decoder; no normalization, resampling,
crossfade, generated tones or inferred event names. STM inclusive nibble ends
become exclusive PCM frame ends and are capped at source sample count, following
the existing converter. MusyX uses source start+length; lengths0/1 are nonloops.
Every output was checked for WAV format, full data length, frame count, valid
loop bounds and exact sidecar bytes. A separate post-conversion pass verified all
358 outputs/hashes and152 raw hashes. Rogueport, battle and victory PCM/loops
match the existing RomFS files byte-for-byte. PCM WAV payload files total
903,557,248 bytes; original resources total271,317,240 bytes. The full library
is not embedded into the3DS executable; the early slice retains three tracks.

These are local original-game derivatives with original copyright and no new
redistribution rights. ISO, raw files, WAVs, sidecars, indexes and manifests stay
ignored; only authored tools/tests/docs are committed. Existing vgmstream ISC
format-reference notices and MusyX research provenance above/below still apply;
no new third-party implementation was copied or linked. MusyX macro/envelope/
pitch/event rendering remains unbound. PCM repetition has not been audibly
compared with original ADPCM loop-history execution; no Dolphin/hardware audio
equivalence or seam-free playback claim is made.
