# Asset sources, organization, and provenance

## Sources and storage

| Source or output | Location | Contents and handling |
| --- | --- | --- |
| US GameCube source | `/home/carson/games/roms/G8ME01.iso` | Open read-only. Do not copy the ISO into the repository or distribute it. `TTYD_ISO=/path/to/G8ME01.iso` selects another local image. |
| Extracted GameCube data | ignored `local-assets/` | Private source files and converter manifests, grouped by room or subsystem. |
| Extracted Switch comparison data | ignored `local-assets/switch/` | Separate Switch 1.0.0 assets/audio; not evidence of GameCube behavior. |
| Converted build products | ignored `romfs/` | Runtime packs, textures, messages, audio and metadata consumed by native builds. Generated outputs are not source history. |
| Research captures and inventories | ignored `artifacts/` | Hashes, inventories, verifier output, emulator captures and temporary probes. |

The project has no redistribution license for Nintendo/Intelligent Systems
assets. Original dialogue and audio stay byte/source faithful where packaged;
no authored replacement dialogue or generated sound is introduced as a
substitute. Conversion is limited to native format, texture sizing, geometry
reduction, and presentation adjustments documented per asset family.

## Local asset groups

The ignored `local-assets/` tree is already separated by room and feature:

- **Shared field and map:** `map/`, `map-objects/`, `field-actors/`, `gor_00/`,
  `gor_02/`, `gor_03/`, `gor_04/`, `stage/`, `stg_00_0/`, `stg_01/`.
- **Opening and presentation:** `opening/`, `opening-backgrounds/`,
  `opening-book-pages/`, `opening-map-transition/`, `opening-ship/`,
  `opening-sounds/`, `demo-book/`, `demo-map/`, `demo-effects/`, `heading/`,
  `title-curtains/`.
- **Characters and story:** `harbor-actors/`, `harbor-event/`, `harbor-tutorial/`,
  `harbor-tutorial-combat/`, `harbor-tutorial-stage/`, `underground/`,
  `professor/`, `professor-scene/`, `professor-fence/`, `goomba-quest/`.
- **Battle and effects:** `audience/`, `battle-ready/`, `harbor-curtain/`,
  `trio-battle/`, `field-item-get/`, `save-block/`.
- **Messages and sound:** `messages/`, `fonts/`, `message-icons/`,
  `message-indicator/`, `audio/`, `audio-map/`.
- **Comparison/reference:** `original/` and `switch/`.

These folders are generated, ignored working data and may be absent in a clean
checkout. `romfs/` is the build-facing package; converter scripts under `tools/`
produce its files from the ISO or local Switch extraction.

## Documentation indexes

- [Audio assets](assets/AUDIO.md) and [audio mapping](AUDIO_MAPPING.md)
- [Camera assets](assets/CAMERA.md)
- [Field meshes and roads](assets/FIELD_MESHES.md)
- [Mario models and poses](assets/MARIO_AND_POSES.md)
- [Scripts, messages, and effects](assets/SCRIPTS_MESSAGES_EFFECTS.md)
- [Switch asset and audio research](switch/README.md)
- [Complete dated conversion/provenance ledger](ASSETS_ARCHIVE.md)

The archive records exact ISO paths, byte spans, conversion choices, fingerprints,
rights notes, and limitations for individual packs. Keep new asset provenance
there and add a link here when a new source family or storage group is introduced.

## Item-get audio control

Original fanfare and plaza stream provenance/rights remain recorded in the
[asset ledger](ASSETS_ARCHIVE.md). Final GameCube plaza PCM is original
`stg_gor2_32k`,32000Hz/stereo/3284008frames, hash
`d83abec5e38338a1740e5644523bcbddf34fc879d0af4a6906a6aeba7897fc6d`.
The final audio_map/config step selects that stream. New control envelopes
add no asset conversion and retain field PCM cursor/buffers through resume.
Original Nintendo/Intelligent Systems local-use/no-redistribution restrictions
and ignored extraction/runtime packs remain in effect.
[Exact control/asset validation](behavior/audio/NATIVE_ITEM_GET_AUDIO_VALIDATION_2026_10_14.md).

## 2026-10-14 — Original battle defaults and pose directories

Read-only US ISO sources `a/b_mario`, `a/a_mario-`, `a/c_pkr`, `a/c_pkr-`
remain private in `local-assets/trio-battle/`. The existing converter now includes
original `M_Z_1` and `PKR_Z_1` defaults alongside its selected animations.
Existing texture scaling, original geometry evaluation and tex3ds conversion
remain in use. Private `romfs/trio-attack-{mario,goombella}.pose-directory.bin`
sidecars retain original model/texture names and all85/32 descriptor names in
source order; converted subsets must not redefine original numeric indices.
Nintendo/Intelligent Systems assets; no redistribution rights. Neither model
payloads, packs nor the ISO are committed. See the
[storage/directory contract](behavior/display/NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md).

## Tutorial native menu labels and pouch state (2026-10-14)

No additional ISO payload was extracted. Existing original weapon/ItemData,
msg/US and icon packs supply native table pointers, labels and icons unchanged.
The field's complete original pouch snapshot is copied at battle entry; new-game
grants are not repeated. Mapped message RAM copies existing localized label
bytes/control tokens and retains original key pointers internally. Licensing
restrictions and ignored private pack paths remain unchanged. No texture,
geometry, lighting or generated dialogue conversion was added.

## Native weapon-menu decision evidence (2026-10-14)

Shared setup80122068/6c and decision80123210/150 follow the existing full
read-only ISO verifier and reuse original weapon, message and candidate data.
The tutorial decision writes actual target state without new game payload,
rewritten labels, texture/geometry/lighting conversion or generated dialogue.
Private asset paths and Nintendo/Intelligent Systems licensing restrictions
remain unchanged.

## Original weapon-submenu control and sound requests (2026-10-14)

Shared Main/End and audience interruption use source bodies pinned by existing
read-only US verifiers. Raw source sound IDs/keys are retained for playback; no
generated tone, rewritten label or new game payload was added. Original menu/
HUD resources remain in existing ignored packs; rendering/animation integration
is still required. Texture, geometry and lighting conversion are unchanged;
Nintendo/Intelligent Systems licensing restrictions remain.
