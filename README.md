# Paper Mario: TTYD — New 2DS XL prototype

A native homebrew remake in development. The current slice uses original
Rogueport geometry/textures, original ordinary Mario and Goombella animations, other character idle poses, and original music
converted locally from your US GameCube ISO. It includes five connected Rogueport outdoor rooms, exploration, dialogue,
a small Crump battle harness, touch menus and versioned saves.

The Jump harness now uses the recovered US TimingA history checks and difficulty
tables. Native physical input and battle pad ownership/update order follow the
recovered local contracts;original polling,pause and event scheduling remain open.
Hammer uses the recovered held-left command with the ordinary default
weapon's charge period and difficulty. Guard uses the recovered private-pad
history checks and difficulty windows. Portable status storage/recovery and
move-count helpers follow the recovered contracts and feed the damage readers.
Original status infliction,turn cadence and presentation remain unfinished.
Portable unit resource import/export and status clearing follow the recovered
pouch and partner snapshot rules. Native battle start uses status clearing;
pouch ownership, equipment import and partner switching remain unbound.
HP/FP damage calculators follow recovered
modifier ordering,and direct application uses recovered resource/accounting rules.
Original effect/audio/audience/hit-event handlers remain unbound;the harness's
weapon power callbacks now use recovered US tables. Player levels/bonuses,stats,
impact scheduling and original
tutorial/event/eligibility binding remain provisional. Open requests
for the documentation agent are in [documentation requests](docs/DOCUMENTATION_REQUESTS.md).

Jump currently executes one documented hit;its original follow-up event is still
unbound. Successful timing is recorded separately from per-hit power. This
partial harness must not be treated as the original complete Jump/tutorial attack.
Portable Mario jump motion, distance-adjusted count preparation and sound-mode
loading now follow recovered local contracts. Their original math/audio services
and attack event scheduling still require native binding.
Native StartAC, ResultAC and StopAC use the recovered command-service protocol;
original help UI and prize-byte producers remain unbound.
Portable approach queries, movement and movement-sound counters follow their
recovered contracts; native script and audio ownership still require binding.

This is not a finished or fidelity-verified remake. Complete character animation
coverage, original material/shadow rendering, complete sound-effect event playback, measured tutorial mechanics and broader
story content are still incomplete. Hardware performance has not been tested.

## Mechanics research

The primary behavior baseline is the supplied **US GameCube G8ME01 revision 0**.
See the [completion assessment and priorities](docs/behavior/meta/US_COMPLETION_AND_PRIORITIES.md).
The supplied Switch remake **1.0.0** remains a separate comparison reference.
See the [Switch research collection](docs/switch/README.md) for verified binary
identity, combat/field data, native contracts and remaining gaps. Current research
is **static analysis only**. The prototype and earlier behavior contracts remain
GameCube-based; they are not verified Switch implementations.

## Build and run

Installed dependencies: devkitARM, libctru, citro3d, citro2d, tex3ds and picasso.
Host asset tools require Python 3, Pillow and NumPy. Host tests require CMake and
a native C++17 compiler. The local ISO is required at build time only.

```sh
make all
make test
azahar ./ttyd-2ds.3dsx
```

Audio uses NDSP and requires the launcher-provided DSP component or a locally
dumped `sdmc:/3ds/dspfirm.cdc`. With the supplied local firmware copied into
the isolated Azahar profile, music/jump output and mute/restore were verified
by a dedicated audio capture. Hardware and original-runtime fidelity remain open.

Output: `ttyd-2ds.3dsx`, with its RomFS embedded. Copy it to the SD card's
`/3ds/ttyd-2ds/` directory to use homebrew launcher. CIA packaging is deferred.
An alternative ISO location can be supplied as `TTYD_ISO=/path/to/G8ME01.iso make`.
The default is `/home/carson/games/roms/G8ME01.iso`.

The asset pack is cached using converter/source fingerprints. `make clean`
cleans the 3DS build, retaining expensive local conversions. Original assets and
built binaries are ignored by source history. Implementation and handoff notes are recorded in incremental Git commits.

`make audio-assets` extracts all original music streams and the MusyX bank,
then converts the streams and all 1,247 bank samples into the ignored
`local-assets/audio/pcm/` library. Original rates/channels and loop intervals
are retained in PCM16 WAVs with `.wav.loop` sidecars; the library manifest records
source paths, hashes and sample IDs. An alternate ISO can be supplied with
`TTYD_ISO=/path/to/G8ME01.iso make audio-assets`. The full library stays outside
RomFS; the current slice still packages its three selected music tracks.
Bank WAVs are named `pmario-<four-digit-hex-ID>.wav` in `pcm/samples/`;
only the 212 looped samples have loop sidecars. These are original sample
sources; MusyX macros and game event playback still require implementation.

`make audio-map` creates a searchable catalog of 3,218 named GameCube entries
and all 10,256 Switch sound records with wave dependencies. `make all` packages
five original GameCube primary-sample effects alongside the corrected Rogueport,
battle and victory music. Complete macro/event playback remains unfinished.
See [audio mappings and trigger guidance](docs/AUDIO_MAPPING.md).

The separate Switch 1.0.0 audio library is generated with
`python3 tools/switch_audio.py` from the verified extracted
`local-assets/switch/romfs/`. Its raw resources, PCM16 WAVs and loop sidecars
stay in ignored `local-assets/switch/audio/`; current native audio selection
remains GameCube-based. Decoder setup, asset coverage and provenance are in
[Switch audio assets](docs/switch/AUDIO_ASSETS.md).

## Controls

| 3DS control | Function | Azahar default keyboard |
| --- | --- | --- |
| Circle Pad | Walk; hold left and release to charge Hammer | Arrow keys |
| A | Interact, jump, advance dialogue; attack/guard timing | A |
| B | Close dialogue/leave defeat | S |
| X | Talk to Goombella in the field | Z |
| D-pad left/right | Menu tabs or battle selection | F / H |
| Y | Selected field menu action: save, item, sound | X |
| Touch | Main menu and battle selection interface | Mouse on bottom screen |
| START | Exit; saves when exploring | M |

Walk east toward Crump and press A to enter the encounter. The battle timing
meter is a development aid. A mushroom cannot be used at full HP. Victory returns
to exploration and saves the checkpoint. Saves reside at
`sdmc:/3ds/ttyd-2ds/progress.bin`; invalid saves fall back to a valid backup.

## Verification and handoff

```sh
python3 tools/iso_assets.py inventory --output artifacts/iso-inventory.json
python3 tools/emulator_smoke.py --seconds 8
python3 tools/emulator_smoke.py --fresh-save --seconds 5 --input-script tests/azahar_walkthrough.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_hammer.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_defense.py
python3 tools/emulator_smoke.py --reference-dolphin --seconds 12
```

The capture tool creates an isolated emulator profile and virtual display. It
never uses your existing emulator configuration or saves. It requires Xvfb,
xdotool and ImageMagick. Restricted environments may require display-socket
approval. Captures/logs are under `artifacts/azahar` or `artifacts/dolphin`.
The walkthrough is a capture sequence for manual review, not an automated claim
that every gameplay requirement passed. It assumes a fresh save and default
window layout.

Read [progress](docs/PROGRESS.md), [architecture](docs/ARCHITECTURE.md),
[asset provenance](docs/ASSETS.md), and [mechanics evidence](docs/MECHANICS.md)
before continuing. User preference is to preserve original art, music and sound;
the early redrawn assets and generated tones were rejected and retired.

The [original behavior reference](docs/ORIGINAL_BEHAVIOR.md) indexes recovered
US binary behavior, documents event execution and subsystem ordering, and tracks
the evidence still needed for a strongly equivalent reimplementation. Reproduce
its binary fingerprints with `tools/behavior_evidence.py`.

Original US English dialogue and faithful story reproduction are required.
`make all` now packages all 260 original `msg/US` resources byte for byte under
ignored `romfs/messages/`. Portable room/global lookup retains IDs and tokens;
story-window playback is still unbound.
The ordinary exploration harness still displays development notices. A native
story3 harbor checkpoint now plays the first six original Goombella/Crump/X-Naut
messages with source speaker/order/position bindings, original US font and danger
music. Arrival, tutorial continuation and full event/window services remain
unfinished. No authored replacement character dialogue should be used.

To inspect that development checkpoint in an isolated emulator profile:

```sh
python3 tools/make_harbor_checkpoint.py
python3 tools/emulator_smoke.py --save-fixture artifacts/fixtures/harbor-story3.bin --seconds 10 --input-script tests/azahar_harbor_story.py --output-dir artifacts/azahar-harbor-story3
```

This native test checkpoint bypasses the unfinished arrival intro; it is not an
original GameCube save or the complete prologue demo. A/B advance or accelerate
the original message window; its closed page waits preserve the source controls.

Outdoor road exits link the plaza with harbor, east and west rooms. Walk toward
the original road openings to change rooms; arrival progress is saved. The west-side road also connects to the station. Interior
doors, pipes and story-dependent exits are unfinished. See docs/PROGRESS.md for
current rendering limitations and isolated reciprocal-road verification.
