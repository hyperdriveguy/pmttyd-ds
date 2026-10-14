# AUDIO-01 — Original BGM sequence/event binding and SFX cue selection

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the demo path with full-game intent ([milestones](../MILESTONES.md)).
D1 step 4 selects the tutorial battle's music and D1 acceptance requires original
music and sound effects; the same game-level binding runs through every later
scene. The documentation slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

The MUSYX **engine** is already documented (macro/DSP/ADSR/stream/voice leaves in
[`behavior/audio/`](../behavior/audio/)), as are positional SFX and several single
scene cues. What is missing is the **game-level catalog**: which original
sequence/sound ID plays for which scene/event, how the game starts, switches,
holds and stops it, its original loop markers and mix, and the SFX cue-ID table
with its event selection. The native port currently invents all of this.

## Open todos

- [ ] The original BGM/sequence entry points and their ID arguments: the
  game-level routines that start, switch, hold/tail and stop a sequence (for
  example the music-on/music-off/music-stay family), the sequence IDs the
  opening/title/demo/voyage/harbor/tutorial/underground/professor scenes use,
  and how each sequence ID maps to the shipped asset.
- [ ] The scene/event → BGM selection table: which story flag, room or event
  selects each sequence, and the transition rules between sequences (crossfade,
  hold with tail, stop/fade timing) and the original loop markers/loop counts,
  so the native sidecar `.loop` file and fixed crossfade are replaced by the
  original loop and transition behavior.
- [ ] The original per-sequence mix: playing volume/envelope and any hold/tail
  publication, so the native adapter's fixed `0.65` mix and invented track
  volume are not treated as original.
- [ ] The original SFX cue-ID table and its event selection: the source cue
  names already named in the native fallback (`SFX_CURSOR_OK1`,
  `SFX_MARIO_JUMP1`, `SFX_MARIO_DAMAGE1`, `SFX_AC_OK1`, `SFX_MARIO_GUARD1`,
  `SFX_MOBJ_BLOCK_SAVE1`, `SFX_FLD_NPC_KURI_MOVE1L/R`), their real IDs/table
  entries, and how field/battle/UI events select them — including the
  save-block cue `431` the native port keys off `saveObjects->sounds`. Keep
  engine-level playback with the existing positional-SFX contracts.
- [ ] The tutorial battle's selected music and the harbor-danger/underground/
  professor selections, worked end to end as the D1 binding: the event that
  selects it, the sequence ID, and the loop/transition the original applies.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
demo path with full-game intent: the native port has no recovered game-level
audio binding. `include/audio/audio_policy.hpp` is explicitly "a native presentation
adapter, not recovered Switch event routing": its `audioTrack`/`soundtrackTrack`
map the native `Scene`/room enum onto **invented** track indices (`0..17`) and
its `musicAssetPath` names **invented** `.wav` assets (`rogueport.wav`,
`battle.wav`, `victory.wav`, `harbor-danger.wav`, `title.wav`, … plus a
`soundtrack-switch` variant). `source/runtime/audio.cpp` then looks up a sidecar
`<asset>.loop` file for loop points, applies a fixed conservative mix
(`constexpr float gains[]`, `0.65f`), and its own comment notes that "Jump's
muted original table entry is deliberately overridden for this primary-sample
fallback". On the SFX side it keeps `OriginalCueNames` beside the invented
`CueNames`, but there is no original cue-ID table or event-selection map, and the
save-block cue is keyed only by the literal `431`.

The engine leaves (MUSYX macros/DSP/ADSR/stream/voice) and positional SFX are
already documented; single scene cues exist
([opening map BGM](../behavior/audio/OPENING_MAP_BGM.md),
[title BGM owners](../behavior/audio/OPENING_TITLE_BGM_OWNERS.md),
[save cue 431](../behavior/audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md),
[hammer charge cue](../behavior/audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md),
[battle audience sound](../behavior/audio/NATIVE_BATTLE_AUDIENCE_SOUND.md)). What
is missing is the catalog and the selection rules behind them, so the port cannot
play the original music the D1 acceptance criterion requires.

Please recover the original game-level audio contract as one system — the BGM
start/switch/hold/stop entry points and their sequence-ID arguments, the
scene/story/room → sequence selection table, the original transition and loop
behavior, the per-sequence mix, and the SFX cue-ID table with its event
selection (including the save-block cue) — so native scenes select the original
sequence IDs and SFX IDs instead of invented track numbers and `.wav` names.
Keep MUSYX engine internals and positional SFX with their existing contracts;
message playback with the message contracts, event scheduling with
[EVT-01](EVT-01.md), story flags with [GSF-01](GSF-01.md), and the result/return
flow with [RESULT-01](RESULT-01.md). No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
