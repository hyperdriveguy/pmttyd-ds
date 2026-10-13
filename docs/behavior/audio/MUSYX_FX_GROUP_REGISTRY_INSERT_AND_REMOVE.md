# MusyX FX group registry registration, mutation and removal

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete dataInsertFX8027e7d0/140 and
dataRemoveFX8027e910/1cchex. These are source producers for
[loaded FX lookup/start](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).
Project/group loading, memory lifetime, voice/macro/controller/hardware execution
and original traces remain separate. No original resource is modified on disk.

## Registry schema and ownership

BSS dataFXGroups80400ce8 has128 records of12 decimal bytes; current count is
U16 dataFXGroupNum8041eef0. All offsets below are hexadecimal:

| Group record field | Local source role |
| --- | --- |
| 0 /unsigned16 | Group identity, compared with LOW16 supplied ID |
| 2 /unsigned16 | Number of10-byte FX entries |
| 4 /unsigned16 | Registration/reference count |
| 6..7 | Not initialized by insertion; copied with record during removal |
| 8 /word | Caller-supplied FX entry-array pointer, retained without copying |

The array is live referenced memory. First insertion writes into caller entries;
it is not an immutable pointer to original ISO bytes. No allocation, ownership
transfer, deep copy, sorting or resource decoder occurs in these two bodies.
The global table's extent is original symbol evidence; input pointers/lengths
and all other count/table writers require source validation.

## Complete insertion: duplicate versus new record

Retain raw group ID G, entry pointer P and count N. Capture current group count
ONCE and scan records ascending for first identity==U16(G). For a match:
read CURRENT reference half4, store low16(ref+1), return0. This path makes NO
IRQ request, does not replace P/N or patch the supplied/new entry array, and
performs no resource comparison. Ref65535 wraps to0 without removing the group.

For a miss, if captured count>=128 return0 without registration/mutation.
Otherwise call hwDisableIrq. NOW reread current global count; derive the new
record from that fresh ordinal, without rechecking128 or rerunning duplicate
search. Store in order: low16 G at0, low16 N at2, RAW P at8, reference half4=1.
Padding6..7 stays unchanged. Do not reuse the pre-IRQ ordinal as if it were
retained for publication; pointer/count/source invariants remain necessary.

Then let C=U16(N). For C>0, set byte9 of every entry at P+10*j to31 decimal,
ascending j0..C-1. Eight stores per block plus C&7 tail implement the same
order. C0 skips entry mutation even for a null pointer. Counts whose low16 is0
store count0 and produce no loop, regardless of high argument bits.

Only AFTER all entry writes, freshly read global group count and increment/
store its low16; call hwEnableIrq; return1. No rollback, counter saturation,
whole-record clear or IRQ-status-based failure. The source assumes stable
registry/memory domains across these requests; it does not enforce them.

The returned0 covers BOTH duplicate-reference success and full-capacity refusal.
Do not rename it a universal failure or overwrite the old pointer on duplicate.
A duplicate with ref0 can increment to1 while preserving the original array.

## Complete removal and compaction

Mask supplied group ID low16, capture current count and scan ascending.
No matching record returns0 WITHOUT IRQ, writes or resource free.
For a match, decrement reference half4 with16-bit wrap, store it and reread it.
If nonzero return1, preserving registry count and all other record fields.
Thus old ref0 wraps to65535 and keeps the group; there is no underflow clamp.

When reread ref is0, call hwDisableIrq. Freshly read CURRENT global group count.
Starting at original matched ordinal+1, copy every following12-byte record left
one slot in ascending source order. Each record loads source words0 and4 before
storing destination words0 and4, then loads/stores source word8. This copies
padding and pointers as RAW WORDS, not field-level reconstruction. The bulk path
unrolls eight records; the tail preserves the same read/store order.

After shifts, freshly read global count again and decrement/store low16;
call hwEnableIrq; return1. No last-slot clear, entry-array free, entry-byte9
restoration, voice stop or group reload request. Unused tail bytes can retain
a duplicate of the prior last record; dataGetFX's live count excludes that tail
under normal source invariants. Count changes during IRQ/provider services are
not snapshotted or repaired.

## Concrete connection to the start operand

First registration overwrites each original FX row's additional byte9 with31.
[The synth start](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md) later reads
that LIVE byte into caller stack+18 for synthStartSound. Raw .proj metadata
therefore cannot substitute for the loaded row unchanged at this point.
For the recovered Hammer FX862/861/group6/macro995/994, the resource identities
remain the [verified dependencies](HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md), but
this registry producer adds a mutable runtime-field prerequisite. Actual project
residency and group-push binding are still to be recovered.

No byte9 from pssfxlist, positional slot or sample directory is affected by this
statement; these are distinct source structures. Do not conflate their fields
because each happens to have a byte at the same relative offset.

## Validation and remaining producer graph

[Manifest](../../../tools/musyx_fx_registry_evidence.json) pins both complete
bodies, selected search/width/IRQ/mutation/compaction/count/return anchors and
linked lookup/IRQ identities. Fresh complete inspection ELFs remain
`/tmp/inbox-fxinsert.elf` and `/tmp/inbox-fxremove.elf`, read-only ISO. Re-run
existing generic DOL verification with the manifest. No macro/PCM/ISO or raw
resource array is committed.

Remaining: sndPushGroup/sndPopGroup/project residency and count reset writers;
entry sorting/pointer ownership, voice references on unload, IRQ/concurrency/
capacity/refcount domains and original group/start/unload traces. Registration,
reference and mutation contracts are answered; complete audio chain stays OPEN.

[Complete group push/pop and resource-list request order](MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md) plus [game warm-up/loader](GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md) now closes those exact local producers and static project-list bindings. Actual file/memory/voice residency and child resource/stop internals remain open.
