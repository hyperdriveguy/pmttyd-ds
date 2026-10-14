# Native Switch soundtrack adapter

Version: locally decoded Switch 1.0.0 music; native gameplay remains the
GameCube US slice. These mappings are explicit presentation choices, not
recovered Switch event conditions or measured behavior.

[Bindings](../../../config/switch_soundtrack.json) preserve the 13 existing
native BGM cue IDs. IDs13..16 add harbor/east/west/station arrangements;
ID17 adds Normal_etc_Think. Plaza uses Town_S0_A, harbor Town_Harbor_S0,
east Town_S0_B, west Town_S0_C and station Town_S0_D. Underground uses
Town_Under_1st. Battle Select or trio Command/Target chooses Think; attacks and
enemy turns choose Normal_etc. Victory uses its nonlooped Win arrangement.
The harbor tutorial retains its explicit cue with SmallBoss. Muting stops
music and primary SFX; harbor-danger and professor cues retain priority.
The GameCube build retains its previous district/ordinary battle selection.

Existing opening cue publications, fade volumes and fixed-address three-stream
NDSP ownership stay in force. Title readiness validates the selected Switch PCM
even when NDSP is unavailable. Source SFX channels2..9 and house channels10..17
remain separate from BGM channels1,0,18. No generated tones are introduced.

[Packager](../../../tools/switch_soundtrack.py) reads the ignored decoded
[library](../../switch/AUDIO_ASSETS.md), selects exact BFSAR names, validates
source PCM hashes/canonical44-byte headers/rates/channels/frame extents/loop
sidecars, and copies unchanged WAV payloads. Loops are inclusive/exclusive
little-endian PCM frame indices. A final SWM1 marker selects the Switch paths;
missing tracks fail packaging rather than silently substituting another song.
All18 files stream from RomFS through three2048-frame buffers per active owner;
the full songs are never loaded into linear RAM. Original GameCube assets stay
separate so their converters/cache checks are unchanged. SOUNDTRACK=gamecube
removes only the Switch pack and writes GCM1 to the same top-level marker,
retaining SFX and original music. The marker forces embedded RomFS rebuilding
in both directions when the soundtrack selection changes.

Limitations: changing normal/Think tracks restarts the selected stream. Phase
synchronization, Switch stems, seamless district transitions, title/home/battle
intro chains, contextual underground arrangements, full original music/SFX
mixing and native hardware playback remain open. Original-rate48k stereo uses
more storage/bandwidth than the former32k tracks. Asset inspection and host
checks establish format/selection/build behavior; hardware performance and
original-runtime audio parity require separate listening/capture evidence.

Provenance and licensing: Nintendo/Intelligent Systems original assets remain
copyrighted; local conversion provides no redistribution rights. Source files,
PCM and compiled RomFS/binaries remain ignored. Decoder licensing and exact
upstream identity are retained in the [library record](../../switch/AUDIO_ASSETS.md).

Validation (2026-10-13): isolated Azahar fresh-file flow reached saved harbor
story3 with original local DSP firmware. Dedicated temporary audio-sink capture
matched Switch Demo_1 (normalized waveform correlation .998) and Title (.637),
plus original house flight/postbox/open/shut samples (.931/.819/.975/.649).
Capture/report: `artifacts/azahar-switch-soundtrack/audio.wav`, `audio-analysis.json`
and opening screenshots. These establish native output for those cues, not
all18 audible tracks, seamless loops or hardware equivalence. Both soundtrack
modes rebuilt embedded3DSX successfully; the final build is Switch.
