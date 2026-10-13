# Save-block original sound ordinal431 and native residency boundary

US G8ME01 disc0 revision0;2026-10-13. Static read-only source/cached-native
catalog inspection, no audio implementation or original execution. Complements
[save callback](../field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) and
[SFX start wrappers](BATTLE_POSITIONAL_SFX_START_WRAPPERS.md).

Source state0 requests psndSFXOn_3D800d9848 at80066fbc with r3=431
(0x1af), r4=&objectXYZ. This occurs once on the admitted hit, before A_1
restart; no direct callback retry or completion sound is inserted here.
The complete0x3c wrapper passes default volume/pan0xff, position pointer,
flags0x10 and other zero arguments into source provisioner800d9934.

| Source binding | Original value |
| --- | --- |
| pssfxlist ordinal / row | 431 /80318cd4 |
| Name / alias pointers | 802d0f40 /802d0f58 |
| Exact name / alias | SFX_MOBJ_BLOCK_SAVE1 /SE4_OBJ_BLOCK_SAVE1 |
| Descriptor / FX ID | 0x2000026a /618 |
| Row parameter bytes+0xc..13 | 100,64,0,20,0,0,0,0 |
| MusyX group / macro object | 5 /745 (0x2e9) |
| FX priority / voices / velocity / pan / key | 2 /10 /127 /64 /60 |
| pmario.pool macro offset / length | 55112 (0xd748) /64 |
| Direct sample ID | 550 (0x226) |
| Sample rate / PCM frames / loop | 22050Hz /24422 /none |

Ordinal431, FX618, macro745 and sample550 are different domains. Do not
index the sample bank with431 or interpret the alias as proof of a standalone
stream. Original20-byte row SHA256 is
`7e20b42038c26c91e27cc1348468b138c79919586b264ea5e66e86005c65b873`;
macro span SHA256 is
`47787f3ff0cec1ef1e2baef47c61771865e0562bba8d0433153eddd3f399fa18`.
Read-only original project/pool records confirm one group5 FX binding and
one direct macro sample dependency. Complete MusyX modulation/voice/timing
behavior and original runtime group5 loading/residence are not established by
these static records.

Current ignored native/source audio catalogs contain the named row and full
sample metadata. The ignored original library file
`local-assets/audio/pcm/samples/pmario-0226.wav` exists. Current
config/audio_bindings.json and cached native slice catalog package only
confirm/jump/hit/success/guard; ordinal431 is absent from that selected slice.
Thus catalog availability/library PCM does not establish resident native playback
or a registered save cue. A future primary-sample fallback must remain explicit;
it does not close original macro/group/positional audio fidelity. No audio assets
or playback code were added during this documentation pass.

Validation: source row/literal reads, complete psndSFXOn_3D listing and fresh
read-only sound/proj/pmario.proj/.pool reads using existing project_effects/
macro_samples tools reproduce bindings and hashes. Cached sample/catalog/config
residency inspected separately; no claim about a future build or actual sounding
voices. Outputs remain /tmp/ignored original libraries. Original Nintendo/
Intelligent Systems rights remain; no redistribution is implied. Docs-only:
no make/build, Dolphin or hardware claim. Original group loading/voice/macro
services and native cue residency remain open.
