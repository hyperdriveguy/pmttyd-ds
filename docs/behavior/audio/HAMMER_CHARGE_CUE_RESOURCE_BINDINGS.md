# Ordinary Hammer charge cue resource bindings

US GameCube G8ME01 disc0 revision0, recovered2026-10-13 for
[AC-02](../../requests/AC-02.md). Answers the two original sound-key/FX/macro/
sample bindings; complete MusyX execution and original charge playback remain
open. This is a static dependency contract, not a replacement WAV recommendation.

## Concrete caller order

[StickKeepLeft](../battle/BATTLE_HAMMER_STICK_KEEP_LEFT.md) calls
psndSFXOn at8012a4b0 with SFX_AC_PI1 and8012a4d8 with SFX_AC_PONE1.
After charge countdown becomes negative, increment stage S, then let k=S-2.
For k<N-1 and retained handle[k]==-1, request PI1; for k>=N-1 and that
handle==-1, request PONE1 and set final-sound marker1. Save the returned slot
handle even when it is-1; no extra retry-success inference is supplied.

The normal source parameter N4 admits the ordinary stage pattern through this
branch; arbitrary parameter domains retain its unchecked handle indexing and
k>= comparison. No generated tone, rendering-frame cadence or guessed charge
period is part of the resource binding. The ordinary weapon's period selection
is [separately established](../battle/BATTLE_HAMMER_WEAPON_BRANCHES.md).

The [start wrapper](BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) requests default
volume/pan byteff through the corrected [provisioner](POSITIONAL_SFX_SLOT_PROVISIONING.md).
That resolves them to slot volume127 and row default pan64, then queues a slot.
The returned composite allocation handle is not the effect engine's voice word.
The [update contract](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
explains pending-bit transition; backend macro execution remains separate.

## Original table and MusyX dependency chain

All IDs/counts below are decimal unless prefixed0x. Source row parameters are
bytesC..13; row+4 descriptor's low16 is the effect ID, not the table ordinal.

| Cue | pssfxlist ordinal / row | Descriptor / FX ID | Group / object (macro) | Direct sample |
| --- | --- | --- | --- | --- |
| SFX_AC_PI1 | 2619 /0x803237c4 | 0x2000035e /862 | 6 /995 (0x3e3) | 776 (0x308) |
| SFX_AC_PONE1 | 2620 /0x803237d8 | 0x2000035d /861 | 6 /994 (0x3e2) | 775 (0x307) |

PI1 row bytesC..13 are60,64,0,0,0,0,0,0; PONE1 uses70,64,0,0,0,0,0,0.
Each original project FX entry has priority8, voices10, velocity127, pan64,
key60. These are original entry parameters, not a guarantee of ten simultaneous
voices or a measured output amplitude. Both descriptors choose the effect
branch rather than stream/song high-bit routes.

| Resource record | PI1 | PONE1 |
| --- | --- | --- |
| pmario.pool macro offset/size | 0x11818 /64 bytes | 0x117d8 /64 bytes |
| Static seven opcode bytes | 0x38,0x33,0x10,0x07,0x11,0x31,0x00 | Same |
| pmario.sdir record offset | 0x6100 | 0x60e0 |
| Rate / PCM frame count | 16000Hz /1526 | 16000Hz /5276 |
| Directory loop start/length | 0 /0 | 0 /0 |

Opcode0x10 is the direct STARTSAMPLE reference to the listed ID. The other
opcodes and their operands must be executed through recovered MusyX semantics;
this delivery establishes dependency identities, not their complete control
flow, pitch/envelope/duration effects or actual audible timing. A nonlooping
sample directory does not establish the whole cue's effective duration.

Read-only source paths: `sound/proj/pmario.proj`, `sound/proj/pmario.pool`,
`sound/proj/pmario.sdir` in the supplied ISO; sample data is in the existing
`sound/proj/pmario.samp` bank. Existing local PCM library counterparts are
`local-assets/audio/pcm/samples/pmario-0308.wav` and `pmario-0307.wav`, where
hex filenames are sample IDs. They are dependencies, not complete cue playback.
No conversion/export was performed here. Original assets remain local/ignored
under the project's [provenance/licensing policy](../../ASSETS.md).

## Reproduction and validation

[Binding metadata](../../../tools/hammer_charge_cue_evidence.json) retains asset
hashes, row fields, project FX entries, macro offset/size/opcode/direct-reference
metadata and sample record hashes/fields. It contains no macro command bytes,
original PCM or source dialogue. Reproduce original rows with
`tools.audio_map.gamecube_tables`, FX groups with `project_effects`, and macro
dependencies with `macro_samples`; use the original files found by
`tools.iso_assets.inventory`. `tools.audio_convert.sample_directory` validates
the complete1247-record directory and uniquely resolves samples775/776.

Independent read-only resource check `/tmp/verify_hammer_evidence.py` passed
3 asset hashes,2 cues,2 FX/macro bindings and2 sample records. It re-parses
rows/project/pool/directory rather than assuming names identify sample IDs.
Fresh full StickKeepLeft listing is `/tmp/inbox-hammer.disasm`.
The generic source verifier with
`tools/positional_sfx_corrections_evidence.json` pins the main body and both
caller instructions alongside the corrected provisioning source. Re-run:

```sh
python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json
python3 tools/verify_behavior_contracts.py
```

AC-02 display/gauge, full command lifecycle, automatic/forced/suppression source
writers, macro/backend execution, original charge/audio trace and Game binding
remain OPEN. This answers the resource-selection portion only.
