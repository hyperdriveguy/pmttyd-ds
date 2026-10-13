# MusyX default cache, resource reset and hook installation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete dataInitStack802874f4/40,
dataInit8027f428/e4, hwInitSampleMem80291438/24,
sndSetHooks802915c0/1c, salMalloc80292888/2c,
sndSetSampleDataUploadCallback802877d0/20 and hwSetSaveSampleCallback802914f4/20hex.
Read [group stack/resources](MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md),
[ARAM initialization/hooks](MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md),
[sample directory](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md) and
[upload configuration](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md).
Hexadecimal addresses/offsets below; explicit decimal counts are labeled.

## Complete group-stack/default-cache initialization

Inputs raw base B in r3, byte-size N in r4. gsDefault80407650 is618hex bytes.
The cache returned by dataARAMDefaultGetInfo is its+60c, address80407c5c.
Source writes in this exact order:

1. gsRoot8041ef48=0.
2. gsDefault.word4=fffffffe; half8=0.
3. gsDefault.word60c=B (cache base).
4. gsDefault.word614=B (cache cursor).
5. gsDefault.word610=wrap32(B+N) (cache end).
6. gsDefault.word0=0.
7. gsRoot=gsDefault; gsNextID8041ef40=0; gsCurrent8041ef44=gsDefault.

No whole-object/stack-row clear, active-depth preservation, ARAM allocation,
transfer, null/overflow/alignment check or voice-stop request. Existing project/
directory pointers and other stack/default bytes remain untouched. After this
body, InsertData's sample path reads the cache at gsCurrent+60c; actual subsequent
group ownership and source base/size are caller prerequisites. The initial cache
cursor is raw B, distinct from ARAM init's top and stream boundary publications.

## Complete resource reset and sample-memory request

DataInit(raw r3,raw r4) writes six U16 counts to0 in order:

| Address | Source name |
| --- | --- |
| 8041ef00 | dataSmpSDirNum |
| 8041eef8 | dataCurveNum |
| 8041eef6 | dataKeymapNum |
| 8041eef4 | dataLayerNum |
| 8041eef0 | dataFXGroupNum |
| 8041eef2 | dataMacTotal |

Then clear all800hex bytes of dataMacMainTab804012e8:32 decimal blocks,
32 ascending halfword stores per40hex-byte block. Fixed end80401ae8 exclusive.
The initial loop predicate uses constant0<512 decimal and always admits the
32-block loop; there is no input-dependent count. This is a complete clear of
that main table, not all macro/resource storage.

Request hwInitSampleMem with the original r3/r4 still intact. That wrapper moves
INPUT r4 to r3 and requests aramInit(rawSecondArgument). Original first argument
is ignored by that wrapper; no base argument is forwarded alongside the size.
Return preserves the child's register result without independent success policy.
ARAM allocation/zero-fill/queue/limits/stream writes are in its complete contract.

No preferred-directory pointer reset, directory/FX/keymap/layer/curve row clear,
entry-ref reset, registry free or voice-stop precedes this request. Counts0 hide
old rows rather than proving unloaded resources. Safe restart/runtime ownership
requires source/caller ordering; retain the stale preferred-pointer distinction.

## Complete hook installation and ordinary allocator leaf

sndSetHooks(inputPointer H) reads H.word0, writes that word to salHooks80410348
word0 (ordinary allocation), then to word4 (physical allocation). ONLY AFTER
both stores read H.word4 and write it to salHooks.word8 (free). No third input
word is read; the two allocation hooks intentionally receive the same pointer.
No null/provider/type/IRQ/state check, prior allocation release or callback wait.

This is ordered memory access, not a snapshot/memcpy. If H==salHooks, the second
read sees overwritten word4, so all three hooks become original salHooks.word0.
This is a derived alias invocation, not evidence that original setup uses it.
Input/provider/alias invariants remain required; do not normalize by copying all
input words before publication.

salMalloc(INPUT size) loads salHooks.word0 and calls it indirectly with INPUT r3
unchanged, preserving returned r3. No null/allocation/overflow/physical alignment
check or added allocator policy. Physical and free leaves use word4/word8 as
already documented; actual hook implementations/installing caller remain open.

## Complete public upload-callback forwarding chain

sndSetSampleDataUploadCallback(rawCallback,rawChunkSize) requests
hwSetSaveSampleCallback with INPUT r3/r4 unchanged; that wrapper requests
aramSetUploadCallback with the same raw pair. Neither wrapper adds IRQ, drain,
normalization, validation, return-status assignment or source callback lifetime.
The final core aligns size and stores unsigned MAX with ARQGetChunkSize when
callback nonzero; callback0 leaves chunk size unchanged, then clears callback.
These wrappers do not configure the per-request completion callback used by
aramUploadData: the two callback roles remain separate.

## Validation and remaining sources

[Manifest](../../../tools/musyx_audio_resource_init_hooks_evidence.json) pins
seven complete bodies and two previously documented child identities,87 anchors
and four resolved direct calls. Derived four cache/wrap vectors, exact800hex
macro-table clear extent and three hook-alias vectors accompany fresh complete
ELFs/listings under `/tmp/inbox-*init.elf`, sndhooks, salmalloc and upload-set files.
Generic source verification checks source identities/anchors; no original code
executes or asset/code body is exported to history.

Remaining: installing/root initialization callers and actual B/N/hook/callback
providers, per-kind registry storage, safe reset/concurrency/alias/residency,
SDK/voice/macro/MIDI and original setup/acquire/release/audio traces. Seven complete
local producer/wrapper bodies are answered; full native/source/runtime remains OPEN.
