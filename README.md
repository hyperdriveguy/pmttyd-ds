# Paper Mario: TTYD — New 2DS XL prototype

A native homebrew remake in development. The current slice uses original
Rogueport geometry/textures, original character idle poses, and original music
converted locally from your US GameCube ISO. It includes five connected Rogueport outdoor rooms, exploration, dialogue,
a small Crump battle harness, touch menus and versioned saves.

The Jump harness now uses the recovered US TimingA history checks and difficulty
tables. Hammer uses the recovered held-left command with the ordinary default
weapon's charge period and difficulty. Guard uses the recovered private-pad
history checks and difficulty windows. HP/FP damage calculators follow recovered
modifier ordering;the harness's weapon powers/stats,impact scheduling and original
tutorial/event/eligibility binding remain provisional. Open requests
for the documentation agent are in [documentation requests](docs/DOCUMENTATION_REQUESTS.md).

This is not a finished or fidelity-verified remake. Original skeletal/vertex
animations, sound-effect event playback, measured tutorial mechanics and broader
story content are still incomplete. Hardware performance has not been tested.

## Mechanics research

The primary behavior baseline is the supplied **US GameCube G8ME01 revision 0**.
See the [completion assessment and priorities](docs/behavior/US_COMPLETION_AND_PRIORITIES.md).
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

Output: `ttyd-2ds.3dsx`, with its RomFS embedded. Copy it to the SD card's
`/3ds/ttyd-2ds/` directory to use homebrew launcher. CIA packaging is deferred.
An alternative ISO location can be supplied as `TTYD_ISO=/path/to/G8ME01.iso make`.
The default is `/home/carson/games/roms/G8ME01.iso`.

The asset pack is cached using converter/source fingerprints. `make clean`
cleans the 3DS build, retaining expensive local conversions. Original assets and
built binaries are ignored by source history. Implementation and handoff notes are recorded in incremental Git commits.

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
The current dialogue interaction displays explicit development notices; original
message playback and event sequencing are unfinished. No authored replacement
character dialogue should be used.

Outdoor road exits link the plaza with harbor, east and west rooms. Walk toward
the original road openings to change rooms; arrival progress is saved. The west-side road also connects to the station. Interior
doors, pipes and story-dependent exits are unfinished. See docs/PROGRESS.md for
current rendering limitations and isolated reciprocal-road verification.
