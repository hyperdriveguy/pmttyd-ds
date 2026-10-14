# Original Trouble Center menu SFX bindings

US G8ME01 revision0; read-only recovery2026-10-14 for
[AUDIO-01](../../requests/AUDIO-01.md) and
[ITEM-01](../../requests/ITEM-01.md). Source menu caller and original sound-key
rows/FX/macros/direct samples are established. Complete audible MusyX/native
parity remains separate; original sample dependency is not automatically the
complete rendered cue. No generated tones or asset conversions are introduced.

## Source menu requests and primary identities

The [original board controller](../field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md)
requests psndSFXOn42 after state0 icon creation/state1 publication;18 on valid
A selection;19 on B;5 for each admitted direction/page movement;43 after all
five icon deletions in either state100/101 exit. Valid A wins over simultaneous
B; invalid A can continue through B/navigation. A/B request visits retain
normal menu display and return0; next exit visit requests43 and returns2.
These are sound table ordinals, not FX IDs or macro/sample IDs.

| Ordinal | Original name | Row VA | Descriptor / FX | Group,macro | Direct sample | Row volume,pan |
| --- | --- | --- | --- | --- | --- | --- |
| 42 | SFX_STATUS_WINDOW_OPEN1 | 80316e70 | 20000491 /1169 | 8,1307 | 1064 | 90,64 |
| 18 | SFX_CURSOR_OK1 | 80316c90 | 2000047e /1150 | 8,1277 | 1043 | 80,64 |
| 19 | SFX_CURSOR_CANCEL1 | 80316ca4 | 2000047b /1147 | 8,1274 | 1040 | 75,64 |
| 5 | SFX_CURSOR_MOVE1 | 80316b8c | 2000047c /1148 | 8,1275 | 1041 | 105,64 |
| 43 | SFX_STATUS_WINDOW_CLOSE1 | 80316e84 | 20000490 /1168 | 8,1306 | 1063 | 90,64 |

Each row's remaining bytesE..13 are0. Original aliases18=SE1_SELECT1,
19=SE1_SELECT2,43=SE1_STATUS_W_CLOSE1;42/5 alias is empty. Descriptors select
MusyX effect branch, not a stream/song branch. Each original FX entry has
priority8,voices10,velocity127,pan64,key60. These are requested resource values,
not measured amplitude or proof of simultaneous voice capacity.

[psndSFXOn wrapper](BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) passes volumeff,
panff,delay0,nullposition,mode0,distance16,final0. Correct
[slot provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md) defaults suppliedff
volume to127,pan to rowbyteD64; rowbyteC90/80/75/105/90 is a separate retained
field. Backend original mixing must preserve that distinction. Caller ignores
returned allocation handle, requests no explicit stop/retry and does not attach
sound ownership to a generated UI label.

## Original macro and sample dependencies

All five macros have72-byte records, eight8-byte commands after their header.
Opcode sequence in hexadecimal:38,33,19,10,07,11,31,00.
Direct STARTSAMPLE10 references the listed sample; other commands must retain
[fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[note handling](MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md),
[wait semantics](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[metric/aux state](MUSYX_MACRO_METRIC_PRIORITY_AND_INLINE_STATE.md) and
[sample/pitch](MUSYX_MACRO_SAMPLE_START_AND_RESOURCE_LOOKUP.md) providers.
Direct-note25(decimal,19hex) requests note60,cents0; its wait word high16 is0,
so that wait returns0 before scheduling. Later opcode7 has indefinite65535
wait and source hardware/event flags; nonloop directory alone is not proof of
whole macro lifetime/envelope/mix. Source opcode38hex is decimal56; do not
mistake opcode bytes for documentation tables written in decimal.

| Cue ordinal | pool offset | Sample.samp offset | Rate | PCM frames | Directory loop start,length |
| --- | --- | --- | --- | --- | --- |
| 42 | 16d40 | 10291424 | 22050 | 8159 | 0,0 |
| 18 | 165a8 | 10193920 | 22050 | 4013 | 0,0 |
| 19 | 164d0 | 10188096 | 22050 | 4013 | 0,0 |
| 5 | 16518 | 10190400 | 22050 | 2760 | 0,0 |
| 43 | 16cf8 | 10287552 | 22050 | 6725 | 0,0 |

Offsets in pool column are hexadecimal; samp offsets/frame/rate fields decimal.
Local original sample-library names use hexadecimal sample IDs:
pmario-0428.wav,pmario-0413.wav,pmario-0410.wav,pmario-0411.wav,pmario-0427.wav
under ignored local-assets/audio/pcm/samples. Their existence or PCM equality
must be checked by the consumer; this delivery performs no export. Decode
original DSP ADPCM coefficients/history at original rate; preserve IDs and
nonloop directory instead of substituting another cue by name similarity.

## Reproduction/provenance

Read only supplied ISO `sound/proj/pmario.proj`,`.pool`,`.sdir`; sample bank
is original `sound/proj/pmario.samp`. Original asset hashes:
proj f00a06f50bd9da5769f07fa751c2e4ae3731f3e0850236d62cd901c7f8eccdeb;
pool d0b12b497cee5718a5433285bdec8fcfe9b2dd25b1c1c0daa77ddb804cb7d1a0;
sdir 9529535398c5f7a839dd18f00c8e831957b26b5aab6a7bf83e5ee6acb5cb8496.
Complete macro-span hashes42/18/19/5/43 respectively:
71dd81ab7794201f3f59c5d11ee628e51ad782b2e3824ad0496052b64f8386bd,
0217333313fd992194788929e9c2a19293508ceb2e9fe27be4e5b087978e52ae,
75c34c45fe2af0bb9db722c390f048e2e805a6c1faaf12e05c8ac3135f573fa2,
51a0421a8a4bf34f1d26b41a9153d1fb3157e86137b68a76696bcc2f064fdb76,
6638a3a7c3c1b2483453015d3296570265829d965804f70f85903ba0d550a524.

Temporary `/tmp/trouble-icon-sfx-bindings.py` reads original rows using
 tools.audio_map.gamecube_tables,project_effects,macro_samples and validates
sample_directory through tools.audio_convert. Its JSON records original ISO
paths/offsets/sizes/hashes and all cue/resource parameters. No source binaries,
macro bytes or audio payload are committed. Original assets remain Nintendo/
Intelligent Systems; local inspection does not grant redistribution rights.

Remaining: full cue macro/envelope/pitch/mix execution, source slot allocation/
retirement/psnd scheduling, faithful native backend/sound preference behavior,
original playback comparisons and hardware validation. AUDIO-01 remains OPEN.
