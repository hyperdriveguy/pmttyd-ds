# Rogueport playability comparison — 2026-11-07

The tested port reaches Rogueport and supports ordinary NPC dialogue, but cannot
play through the original opening Crump battle. Its battle presentation reaches
**“Development notice: original tutorial action services pending”** and remains
there after physical input and waiting. Dolphin executes attacks, enemy turns,
the battle ending, and the original postbattle crowd scene.

This is verification of a frozen development snapshot, not completion of the
remake or a claim about later concurrent commits.

## Specific spots

| Spot | Original US GameCube observation | Native result |
| --- | --- | --- |
| Opening to harbor | A project-owned `New` File1 proceeds through the house, letter/map, voyage, prologue and harbor arrival. Mario receives field control while the ship departs. | **Pass:** empty native profile reaches story3/room1, HP10, and writes a valid v10 checkpoint. The final harbor image was inspected. |
| East Mouser talk and return | The east-side NPC was not independently visited in this reference run. | **Native integration pass only:** normal file-menu entry into a story8/room2 fixture; message opens; onset leaves save timestamp unchanged; dialogue closes; Y save succeeds; story8, room2 and checksum remain valid. This is not a matched original-runtime NPC parity result. |
| Harbor approach | Ordinary Left/Up/Right traversal, including routing around the pier railing/NPC, reaches the staged Crump/Goombella confrontation and subsequent battle gate. | **Admission observed:** story4 fixture enters through the normal frontend, then Up/Right movement and A advance the original prebattle presentation into the tutorial stage. |
| First battle and completion | Initial HUD shows HP10/10, FP5/5, Star Points0, coins100. Jump selection/targeting, a damage1 star and enemy damage reducing Mario to9HP are visible. Further A input completes battle and reaches the original crowd scene. | **Playability failure:** the stage reaches the pending-action-services notice. Repeated A, Right/A/Left/A and an additional10-second wait do not produce a usable attack flow or field return. |

The native tutorial camera and HUD also frame the actors/text substantially
smaller than the original reference. The pending notice is low contrast over the
stage. This is an observed readability/framing difference, not a complete raster
comparison. The story4 fixture starts with2HP; its unchanged on-disk header does
not measure live battle resources because battle saving is suppressed.

## Evidence and identity

Private evidence root: `artifacts/playability-2026-11-07/`.

- Read-only ISO: `/home/carson/games/roms/G8ME01.iso`, G8ME01 disc0 revision0,
  SHA256 `991ec0779b2cfd861bbeca6b3e4cb13b22c6106f26b3429b6c1714170ba1027f`.
- Native production binary: `snapshot/ttyd-2ds.3dsx`, GameCube soundtrack,
  SHA256 `24de723873fc4997c8de3cc471760bb42e05a2f669af61ed900cccde3ad910ae`.
- `snapshot-inputs.json` pins4617 copied source files and2206 runtime products.
  Rehashing after host/Python consumers found no changes to either set. Test
  harness corrections used separately copied `audit-tests/` files, outside the
  frozen production source. The final evidence manifest pins these too.
- `reference-route/scenario.json`, `inputs.jsonl`, `manual-inputs.jsonl`,
  `reference_input.py` and `reference-*.log` retain the reference input sequence.
  The finite replay parent was paused to keep its own Dolphin child alive for
  additional logged inputs. No RAM patches, forced story writes or imported
  gameplay-complete save were used. The copied original GCI began as a `New`
  slot named `66666666`; it is not an empty memory card.
- Reference frames: house `03-039-advance.png`, voyage
  `03-150-advance.png`, harbor `03-268-advance.png`; battle
  `manual-1794099104841744696.png`, damage
  `manual-1794099170708367890.png`, battle ending
  `manual-1794099323870381379.png`, crowd
  `manual-1794099415962331521.png`, all in `reference-route/`.
- Accepted native images: `native-fresh-v10/opening-harbor.png`,
  `native-npc-final/original-mouser-talk.png`, `native-npc-final/talk-return.png`,
  `native-encounter-final/encounter-96.png` and
  `native-encounter-final/ending-wait.png`. The latter run's exit0 means capture
  completion, not a gameplay pass. `checkpoint-audit.json` validates every
  relevant header and extended checksum.

Dolphin used the isolated launcher mappings A=`A`, B=`S`, Start=`Return`, stick
arrows, background input and port0 type6. Azahar used its isolated defaults.
Reference logged durations are wall time, not GameCube frames or simulation
measurements. Some jobs overlapped; displayed FPS cannot establish performance
or physical New2DS XL timing. Audio fidelity and hardware were not tested.
All emulator profiles/saves are project-owned. No user profile was modified.
Original rights remain retained; no ISO, GCI, compiled pack or screenshot is
added to source history. No editable asset import or art/story change was made.

## Validation and test repairs

The first live `make test` ran before asset preparation finished:486/553 passed,
67 failed; Python was not reached. The live `make all -j2 SOUNDTRACK=gamecube`
prepared426 editable recipes, then failed on an obsolete shader dependency in
its old build directory. Concurrent source edits were detected, so those live
attempts are not accepted snapshot gates; the two audit rebuild trees were
stopped before moving to the private snapshot.

In `snapshot/`, a fresh
`make all -j4 -o assets-ready SOUNDTRACK=gamecube` exits0. This uses the already
prepared, copied and hashed products rather than rerunning asset conversion.
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test` compiles and runs554 C++ tests:
551 pass,3 fail in305.30s. Creating the private `artifacts/` directory repairs
`battle_hit_registry`; its focused rerun passes. The remaining two failures
repeat independently:

- `field_map_source`, test line37: ActiveGroup/Work lookup-read trace assertion.
- `field_map_source_owner`, test line47: original scene object-count/work-state
  assertion after a held SDK call.

These in-progress source assertions were preserved for their implementation
owner; no gameplay conclusion is inferred from them. The complete C++ gate
therefore remains failing. A separate
`python3 -m unittest discover -s tests -p 'test_*.py'` passes247/247 in74.181s.
Exact logs include the initial failures and focused reruns.

The legacy fresh-opening test rejected an otherwise valid1568-byte v10 save.
Its accepted lengths now include the existing v10 format; the corrected fresh
run exits0, and independent checks confirm story3/room1/HP10 and all checksums.
The original failed run produced the identical checkpoint hash. NPC entry now
waits for the visible field-notes header using the installed Latin-script
Tesseract model. Earlier fixed-delay, absent-English-model and too-strict OCR
attempts remain recorded as harness failures. Final talk assertions pass.

## Reproduction and next work

From the repository root, use the binary path above for this exact snapshot:

```sh
python3 tools/emulator_smoke.py --binary artifacts/playability-2026-11-07/snapshot/ttyd-2ds.3dsx --fresh-save --seconds 35 --input-script tests/azahar_opening_flow.py --output-dir artifacts/playability-replay-opening
python3 tools/make_field_actor_fixture.py
python3 tools/emulator_smoke.py --binary artifacts/playability-2026-11-07/snapshot/ttyd-2ds.3dsx --save-fixture artifacts/fixtures/east-npc-story8.bin --seconds 35 --input-script tests/azahar_playability_field.py --output-dir artifacts/playability-replay-npc
python3 tools/make_harbor_encounter_fixture.py --resume
python3 tools/emulator_smoke.py --binary artifacts/playability-2026-11-07/snapshot/ttyd-2ds.3dsx --save-fixture artifacts/fixtures/harbor-story4-entry.bin --seconds 35 --input-script tests/azahar_playability_encounter.py --output-dir artifacts/playability-replay-encounter
```

Finish the legitimate tutorial command/input/turn/result/teardown owners, then
rerun this encounter through actual field return and the postbattle crowd/plaza
transition. Follow the [Crump integration requirements](../behavior/story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
Fix and rerun the two field-map assertions against a new frozen snapshot. Add
matched east-NPC observations, precise action-command frame measurements,
water/stairs/road scenarios and physical hardware validation. This run does not
establish guard/hammer/item eligibility, timing windows, defeat behavior, complete
crowd choreography or full prologue playability.
