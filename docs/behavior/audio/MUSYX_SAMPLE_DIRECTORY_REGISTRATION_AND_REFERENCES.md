# MusyX sample-directory registration and sample reference lifetime

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete dataInsertSDir8027e318/168,
dataRemoveSDir8027e480/1dc, dataAddSampleReference8027e65c/d8 and
dataRemoveSampleReference8027e734/9chex. Read
[group lifecycle](MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md) and
[sample hardware bridge](MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md).
Static source evidence only; actual sample memory/voice/cache execution remains open.

## Shared directory registry and mutable records

Registry dataSmpSDirs804006e8 has128 records of12 decimal bytes. U16 count is
8041ef00; preferred directory pointer dataSmpSDirCurrent is word8041eefc.
Directory registry fields: word0 caller directory pointer, word4 caller sample
address base, half8 scanned entry count, bytesa..b untouched padding.
Sample directory entries have stride32 decimal, terminated by ID half0==ffff.
Used entry fields: half0 ID, half2 reference count, word4 original offset,
word8 mutable resolved address, and metadata starting at+c. Do not replace
these live records with immutable original .sdir bytes.

## Complete registration and preferred-pointer side effect

Retain supplied directory D and raw sample base A. Capture current registry
count and scan ascending for an exact POINTER match (not content or sample ID).
After the scan, ALWAYS store D to preferred directory, before duplicate/capacity
branches. Duplicate pointer returns1 without updating base/count or clearing
entry refs. A miss with captured count>=128 returns0, but preferred pointer
still changed. No null-directory or sample-base validation.

On an admitted miss, Disable MusyX IRQ. Scan D's32-byte entries until IDffff,
counting in a word. Let C=U16(scannedCount). Clear half2 references of exactly
entries0..C-1, in ascending order; eight-entry bulk stores plus tail retain this
sequence. C0 skips clearing. An oversized directory's count truncates for this
clear/storage; it is not a bounds-checked count of all possible source entries.
Offsets/resolved addresses/metadata/terminator are not cleared or relocated here.

Freshly read registry count, derive publication ordinal from that read, increment/
store count low16 FIRST, then store D at record0, low16 scanned count at8,
raw A at4. Enable IRQ; return1. No capacity recheck after Disable, whole-record
clear, allocation, reference saturation or rollback. Publication order is not
atomic count-last construction; source/IRQ/count domains remain prerequisites.

## Complete sample acquisition and missing-candidate precondition

Mask requested sample ID low16. Capture registry count and preferred pointer.
Set candidate pointer0; scan exactly captured registry count directories in order.
Within each, scan IDs toffff. At the FIRST matching ID in that directory:

- Ref half2 nonzero: store low16(ref+1) and return1 immediately. Any directory
  can satisfy this active reuse, even if it is not preferred. Overflow wraps.
- Ref0: only when this directory's original base pointer equals captured
  preferred pointer, retain the entry and registry record as the candidate.
  Then advance to NEXT directory, without scanning further IDs in this one.

Continue seeking active reuse after retaining an inactive preferred candidate.
If none was active, use that retained candidate: read original offset word4,
add its registry sample base word4 with word wrap, publish resolved address
entry.word8 BEFORE the hardware request. Put entry+c into a stack pointer word;
call hwSaveSample(&thatPointerWord,&entry.word8,rawCallerCache). Then store
reference half2=1 and return1, regardless of the hardware register return.
The hardware helper may rewrite the published address.

There is NO not-found return: candidate0 still gets dereferenced, and candidate
registry register is not initialized before a match. Valid requested-ID/preferred
resident directory is a real source precondition. A port must not fabricate a
nearest sample, silence success, reference-only insertion or fallback bank.
First acquisition is distinct from active reuse and has no local IRQ wrapper.

## Complete reference release

Mask ID low16; capture registry count; scan every directory/entry in order.
Matching ID with ref0 continues scanning, including later entries in the SAME
directory (unlike acquisition's inactive-match break). First matching nonzero
ref decrements/stores low16 and rereads it. Nonzero returns1 with no hardware
request; zero calls hwRemoveSample(entry+c,entry.word8,rawCallerCache), then
returns1. Missing/no-active-match returns0. No IRQ request, address reset,
whole-record clear or registry compaction. Metadata and old address remain after
release; hardware cache/voice lifetime is a separate service.

## Complete directory removal

Capture registry count, scan exact directory-pointer match; miss returns0.
On match Disable IRQ, scan D toffff or first entry with nonzero ref half2.
Any nonzero ref prevents removal: Enable, return0 without registry changes.
For an all-zero-ref directory, shift later12-byte registry records left one,
using freshly read current count. Each copy loads words0/4 before their stores,
then loads/stores word8; eight-record blocks/tail preserve order and padding.
Freshly reread count, decrement/store low16, Enable, return1.

No tail clear, directory free, sample-address clear, hardware remove or preferred
pointer reset. Removing the preferred directory can leave that pointer stale;
subsequent valid acquisition depends on other producer/caller ordering.
This is not a directory-wide registration reference count: per-sample refs gate
removal, while duplicate directory insertion simply returns1.

## Source checks and remaining owners

[Manifest](../../../tools/musyx_sample_lifetime_evidence.json) pins complete
bodies, key search/ref/width/publication/IRQ/call/compaction anchors and linked
hardware identities. Fresh full ELFs remain `/tmp/inbox-sdir*.elf` and
`/tmp/inbox-sref*.elf`, read-only ISO. Original supplied .sdir has1247 entries;
its reserved half2 values are0 before these runtime mutations. Resource metadata
and source bytes on disk are distinct from the live mutable directory.

Remaining: actual directory/sample-bank/cache/voice provisioning and sorting/
duplicate/capacity/preferred-ID/concurrency domains; sample-address providers,
ARAM storage/removal and original acquire/release/unload traces. Four complete
local lifetime bodies are answered; full audio/native/runtime chain remains OPEN.
