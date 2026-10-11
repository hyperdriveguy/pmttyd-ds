# Paper Mario: TTYD — New 2DS XL prototype

A native homebrew remake in development. The current slice uses original
Rogueport geometry/textures, original character idle poses, and original music
converted locally from your US GameCube ISO. It includes four connected Rogueport outdoor rooms, exploration, dialogue,
a small Crump battle harness, touch menus and versioned saves.

This is not a finished or fidelity-verified remake. Original skeletal/vertex
animations, sound-effect event playback, measured tutorial mechanics and broader
story content are still incomplete. Hardware performance has not been tested.

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
built binaries are ignored by source history. This workspace's `.git` directory
is read-only and is not a functional checkout; no commits have been made.

## Controls

| 3DS control | Function | Azahar default keyboard |
| --- | --- | --- |
| Circle Pad | Walk | Arrow keys |
| A | Interact, jump, advance dialogue; attack/guard timing | A |
| B | Hammer hold/release; close dialogue/leave defeat | S |
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

Original US English dialogue and faithful story reproduction are required.
The current dialogue interaction displays explicit development notices; original
message playback and event sequencing are unfinished. No authored replacement
character dialogue should be used.

Outdoor road exits link the plaza with harbor, east and west rooms. Walk toward
the original road openings to change rooms; arrival progress is saved. Interior
doors, pipes and story-dependent exits are unfinished. See docs/PROGRESS.md for
current rendering limitations and isolated reciprocal-road verification.
