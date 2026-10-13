# MusyX group push/pop and pool resource lists

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndPushGroup802877f0/34c,
sndPopGroup80287b3c/350 and InsertData80287544/28chex. Resource registries,
loading/memory/voice/hardware services remain their own contracts.
Read [FX registry](MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md) and
[game loader](GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
No original execution, residency or playback-equivalence claim.

## Project scan and group stack publication

Push receives raw project base B, group ID G, sample address A, directory D,
and pool P in r3..r7. Retain B,G,D,P; A is used before later resource requests.
Zero sndActive byte8041ee60 returns0. Otherwise capture gsCurrent pointer8041ef44
and its SIGNED half8 depth N. N>=128 returns0; negative depth has no rejection.

Scan project headers starting at B. Each header H.word0 is a next offset
relative to B; word-1 terminates before comparing the header. Match when H.half4
matches U16(G). Missing group returns0. No byte-length, cycle or lower-bound
validation. On match publish stack data BEFORE resource insertion:

- H at captured stack+0c+12*N, using original captured N.
- B at CURRENT stack+14+12*CURRENT signed depth.
- D at CURRENT stack+10+12*CURRENT signed depth.

Each latter publication reloads stack pointer/depth independently. The12-byte
stack record therefore retains header/directory/project pointers, not group ID
or pool ownership. No transaction/rollback, duplicate stack check or pool pointer
publication is supplied by this body.

## Push requests and forward list encoding

First request hwTransAddr(A), then dataInsertSDir(D,translatedA). Nonzero
InsertSDir result admits the H+0c sample list; zero skips ONLY that list.
The remaining pool lists still execute. All list pointers are B+H.word(offset).
No resource-helper scalar result is used to decide overall success thereafter.

| Ordered list / source header offset | InsertData source / type code |
| --- | --- |
| Samples /0c, conditional as above | D /1 |
| Macros /08 | P /0 |
| Curves /10 | P /4 |
| Keymaps /14 | P /2 |
| Layers /18 | P /3 |

Each forward list reads unsigned16 Hword. ffff ends the list. If mask8000 is
clear, request InsertData(Hword,source,type,0), then advance2 bytes. If set,
start=Hword&3fff, end=U16(next half); while U16(current)<=CURRENT end, request
InsertData(current,source,type,0) and increment current with word arithmetic.
Then advance4 bytes. End is reread per test; the comparison index narrows16.
No range-count cap or special endpoint repair. Invalid ranges can wrap/repeat;
source lists/pointers and termination are required domains.

After all lists, if H.half6==1, FXtable=B+H.word1c; request
`dataInsertFX(rawG,FXtable+4,U16(FXtable.half0))`. Other header types omit this
call. Ignore its duplicate/capacity return; then hwSyncSampleMem, reload CURRENT
stack/depth, increment depth low16, and return1. Early published fields and
resource effects are not rolled back if a child declines registration.

## Pop: decrement before use, stop before resource removal

Pop has NO sndActive, empty-stack or positive-depth gate. Capture gsCurrent,
read signed depth, subtract1 and store low16 FIRST. Resolve H from stack+0c
using the sign-extended decremented register value. Reread stored depth to
resolve B and D at+14/+10. Valid nonempty stack/alias domains are prerequisites.
Then Disable MusyX IRQ and request, in exact order:

1. H.type1: s3dKillEmitterByFXID(B+H.word1c+4,table.half0).
   Other types: seqKillInstancesByGroupID(U16(H4)).
2. synthKillVoicesByMacroReferences(B+H.word08).
3. synthKillVoicesBySampleReferences(B+H.word0c).
4. Enable MusyX IRQ.

These are stop requests, not proof every voice has ended. Resource removals
below occur AFTER that critical section; there is no outer IRQ covering them all.

For samples, find the ffff terminator by2-byte scan, then process BACKWARD from
the preceding halfword. When cursor!=start and preceding half has8000, treat
that preceding half as range start(&3fff) and current half as SIGNED16 endpoint.
Decrement current, comparing SIGNED16(current)>=unsigned14 start, requesting
InsertData(U16(current),0,1,1). Then move back4 bytes. Otherwise remove one
unsigned16 ID and move back2, including the first element unconditionally on
that branch. This signed reverse endpoint differs from forward range processing;
do not replace both with one unsigned loop.

After sample list requests, dataRemoveSDir(D). Then macros/curves/keymaps/layers
process forward lists using the SAME forward ID/range scanner but source0 and
operation1. Header type1 finally calls dataRemoveFX(U16(H4)). Return1; do not
clear popped stack record, deallocate B/P/D, restore FX entry byte9 or inspect
removal-helper outcomes. Actual resource refcount/voice ownership remains separate.

## Complete InsertData type/source dispatch

Mask type LOW8; invalid values outside0..4 perform no delegate request.
Operation0 inserts, any nonzero operation removes. Type1 ignores the supplied
source and requests dataAddSampleReference or dataRemoveSampleReference with
raw ID and CURRENT gsCurrent+60c. Other types use this original pool schema:

| Type | Pool base offset for size-linked records | Lookup key | Insert arguments / remove |
| --- | --- | --- | --- |
| 0 macro | pool.word0 | U16(ID) | dataInsertMacro(rawID,record+8); dataRemoveMacro(rawID) |
| 4 curve | pool.word4 | U16(ID) | dataInsertCurve(rawID,record+8); dataRemoveCurve(rawID) |
| 2 keymap | pool.word8 | U16(ID OR4000) | dataInsertKeymap(rawID OR4000,record+8); dataRemoveKeymap(taggedID) |
| 3 layer | pool.wordc | U16(ID OR8000) | dataInsertLayer(rawID OR8000,record+c,U16(record.word8)); dataRemoveLayer(taggedID) |

On insertion, null pool yields null record; nonnull begins at pool+its offset.
At each record, word0==-1 ends before key comparison. Compare record.half4,
otherwise advance by that record's word0 size relative to CURRENT record.
No zero-size/cycle/end-bound check. Missing/null record STILL calls the insert
service with null data; layer additionally passes count0. Removal skips pool
lookup. Child internals decide what those null data requests mean.

InsertData has no common scalar success contract; push/pop discard delegate
returns. Do not promote its switch to an all-resources-loaded predicate.

## Validation and remaining lifetime graph

[Manifest](../../../tools/musyx_group_lifecycle_evidence.json) pins complete
bodies, original list/type/range/stack/IRQ/request anchors and linked service
identities. All source listings remain `/tmp/inbox-group*.elf` / `.disasm`.
Generic DOL source verification does not establish reachable source domains,
actual memory/voice release or original audio timing.

Remaining: dataInsert/RemoveSDir, sample references, per-kind pool registry
internals, group-stack/source initialization, actual sample/project memory loading,
voice/emitter/sequence stop internals and original push/pop/unload traces.
Local request order/list decoding is answered; full audio chain stays OPEN.

## Original project-list binding check

Independent read-only parsing of sound/proj/pmario.proj confirms12 headers with
IDs0..11 and all five lists per header (2998 decoded member occurrences).
Forward and reverse sample decoding produce opposite sequences for all12
supplied sample lists; their endpoints stay in the shared valid signed range.
Group6 explicitly includes samples775/776 and macros994/995, connecting the
original Hammer dependencies to the push lists. This is static source inclusion,
not proof that a particular runtime push succeeded. The manifest pins the whole
project hash in the generic verifier's resource-span (`maps`) slot with no map
nodes; it makes no geometry claim. Decoded counts/hashes are metadata only.

[Complete directory/sample reference lifetime](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md) and [hardware storage bridge](MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md) now answer those exact local child bodies, including mutable refs/addresses and original encoded transfer sizes. Actual ARAM/cache/voice loading/lifetime and original runtime stay open.

[Complete emitter individual/all/FX-array removal](MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md) now answers the exact group-pop child. It unlinks active emitters, clears the high flag half and requests key-off; it does not free nodes or prove immediate voice/sample release. Other stop/source/runtime owners remain open.
