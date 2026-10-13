# Battle Monosiri/Tattle record lookup: corrected complete leaf

US G8ME01 disc0 revision0; battleGetUnitMonosiriPtr8012ef20/14hex
(20 bytes), independently reinspected2026-10-13. This corrects the earlier
spatial-sound classification, lis arithmetic, null-domain, overflow and caller
simplification claims. Read [table/consumers/census](../battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md).
No original execution or source-domain reachability is claimed.

## Exact local address computation

Treat input as a32-bit word K. The entire body is mulli24, lis8037,
addi-12480, add and return. The intermediate lis value is80370000, not80070000;
subtract30c0hex to get8036cf40. Return:

`wrap32(0x8036cf40 + wrap32(24*K))`.

mulli has signed arithmetic interpretation but retains low32 bits. Signed and
unsigned multiply produce the same low32 address bits; host signed C overflow
would be undefined, so implement wrapping arithmetic explicitly. No original
clamp, index check, field read, allocation or null/failure branch occurs.

`monosiriMessageTable` is initialized DOL data at8036cf40, size15a8hex=
5544 bytes, with231 records of24 bytes. Allocated table indices0..230 return
nonnull pointers inside this span; that does not prove every index is allowed
by each consumer. Kind0 points to an all-zero record, rather than returning0.
Negative kind-1 computes8036cf28 before the span. Arbitrary invalid words can
wrap to address0: K=1aa86208hex gives wrap32(base+24*K)==0. Therefore the
blanket claim “no path yields0” was false; there is simply no guarded null
failure return for valid table indices.

## Actual sampling caller and source semantics

The C-mask1 route in _btlSamplingEnemy does a REAL returned-pointer comparison
at800ffc88. Zero rejects the candidate. Nonzero reloads unit.kind at800ffc98,
calls lookup AGAIN at800ffc9c, then reads returned record.word4 and rejects
if it is0. That second kind load and lookup must remain in an ordered adapter;
the leaf's purity alone does not prove the two captured inputs equal under all
source/callback/memory domains. Do not reinterpret the pointer test as a
record-field test or import a guessed spatial-sound fallback.

The record's word4 contains original battle-help/Tattle message-key pointers.
The table also provides displayed name, log entry, model/pose and another
message-lookup input. It is not a sound table. Its concrete record/source roles
and17 DOL call sites plus2 shipped REL imports are in the linked contract.
Original US message IDs/control tokens remain data; no text is paraphrased.

## Validation and remaining scope

[Metadata](../../../tools/monosiri_source_evidence.json) pins leaf/table and
scoped consumer fingerprints/anchors. Re-run the existing generic verifier:
`python3 tools/verify_overworld_roads.py --manifest tools/monosiri_source_evidence.json`.
Fresh listing remains `/tmp/inbox-monosiri-lookup.elf`; the ISO is read-only.

Table mutation/alias/writer invariants, complete log/Tattle/AC/event/persistence
composition, all indirect/dynamic callers and original traces remain open.
The initialized table is original provisioning evidence; flag setters write
separate switch/work banks rather than populating these six-pointer records.
