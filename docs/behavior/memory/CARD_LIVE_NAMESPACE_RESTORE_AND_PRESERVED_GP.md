# Live card namespace restore and preserved GP fields

US G8ME01 revision0; recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete cardCopy2Main800b2388/180hex
(96 instructions), cardGetFilePtr800b33e4/chex (3), plus scoped continueGame
entry and direct DOL restore caller census. Addresses/fields/sizes hexadecimal;
counts decimal. Static deductions, not observed successful persistence.
Read [writer](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md),
[single-slot validation](CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md).

## Restore uses raw slot and does not repeat validation

Supplied raw r3 slot: P=wrap32(W.a8+2000+wrap32(slot<<14)), with W=[8041eab8].
Capture GP=[80415fd0] separately. No busy/card/slot0..3/record flag/length/
version/checksum/null/extent validation. The reader's validation and resident
invalid flag are upstream admission requirements; invoking this function alone
does not establish them. P stays retained in GPR25 through all requests.

Before bulk copy, capture LIVE GP.word0 MASK8 and raw fields168,160,16c,1274,
1294,11b8 into saved GPRs. These are explicit word preservations, not a proof
that every saved pointer or transient field is repaired. Do not invent semantic
names or additional exclusions solely from the offsets.

## Six reverse copies in original order

Each memcpy child return ignored; no transaction, temporary snapshot, source
ownership/overlap guard or rollback. Source-relative offsets/sizes are hex:

| Record source | Size | Live destination / request |
| --- | --- | --- |
| P+08 | 13d8 | CAPTURED initial GP, memcpy800b23d8 |
| P+13e0 | 05d4 | pouchGetPtr800d5eb0 return, memcpy800b23f0 |
| P+19b4 | 0500 | fbatGetPointer8004829c return+4c, memcpy800b2404 |
| P+1eb4 | 0124 | bdsw pointer[8041ece8], memcpy800b2414 |
| P+1fd8 | 0258 | yuwp pointer[8041ed28], memcpy800b2424 |
| P+2230 | 002b | Fixed live destination803bd0d4, memcpy800b243c |

Immediately AFTER the GP memcpy and BEFORE pouchGetPtr, reload GP fresh and
restore its168 from the captured live value. Other preserved fields restore
later. Thus first copy destination and that repair destination can differ if
the global pointer changes. Later namespace destinations are independently
obtained; the function does not keep a single immutable live-global snapshot.

This reverses the writer's six source blocks, including all five GP switch
extents: special full-word global byte0 at174, global flags178:400/global bytes
578:800/local flagsd78:40/local bytesdb8:400. None of the explicit post-copy
writes below overlaps those extents;11b8 is immediately AFTER local bytes.
No swReInit/local-bank clear is requested in this body. Later lifecycle consumers
may reset/change banks; this local copy proof is not an original round-trip trace.
Detailed field schemas and valid runtime pointer/namespace interpretation remain
separate from knowing exact bytes copied.

## Post-copy changes and preservation order

After the six copies, each write below reloads GP independently unless noted:

1. Set1278=0,127c=0,1280=0,10=0, in that order.
2. OSGetTime8029d84c at800b2464; fresh GP gets LOW word34 before HIGH word30.
3. strcpy8026d5fc(fresh GP+13c,source80421134) at800b2480. The original literal
   is **"123"**, bytes31323300. Copy is not an empty-string clear or a whole
   field memset; no local source/destination length check.
4. Fresh GP15c=0; restore captured live1274,1294,11b8,160, in that order,
   with a fresh GP per write.
5. Fresh GP08=0, then fresh GP0c=0.
6. Captured original live bit8 nonzero: fresh GP.word0 OR8; otherwise fresh
   GP.word0 clears8. Other current word0 bits retain their post-child values.
7. Fresh GP16c receives captured live value; exit.

GP168 was already restored before other copies. This does not restore the
original live GP as a whole: most fields come from record+8, selected fields
keep live values, clocks/transition words are replaced, and bit8 is composed
into a freshly read word. Children/global changes affect later owners. No
independent scalar return, card/IO request, IRQ mask or physical timing claim.
ABI48-byte frame/GPR24..31/LR restored; no floating register state used.

## File pointer leaf and actual direct restore owner

cardGetFilePtr loads W=[8041eab8], then W.a8 into r3 and returns. No slot offset,
validation, allocation, state transition or null fallback. Returned pointer is
resident buffer base, not the selected record or loaded live GP.

An exhaustive direct branch-with-link scan of all seven DOL text sections finds
ONE call to cardCopy2Main: continueGame800f39dc at800f39fc. Scoped entry reads
load-work pointer[804169d0], its word20 slot, then requests restore. No local
record validation between that entry and call. Caller admission from the rest
of loadMain/event/UI is outside this entry slice.

After return, scoped caller reloads load-work20 and writes it to fresh GP11d0,
then independently reloads and writes current slot to fresh GP11b8. Thus the
restore leaf's live11b8 preservation is NOT an unconditional lifetime rule;
its immediate caller overwrites that word afterward. Full continueGame then
owns later party/map/story/control setup, which remains separate.

The census excludes REL/indirect pointers/tail branches and does not prove
runtime execution. The full containing continueGame hash is pinned, but only
its entry through800f3a1c is interpreted here. No caller identity inferred from
an unverified pointer registration.

## Validation and remaining persistence work

[Manifest](../../../tools/card_live_restore_evidence.json):nine spans/
117 anchors (all99 leaf words/one literal word/seventeen scoped caller words),
ten local direct calls/no indirect calls. Six copies/six preserved fields/seven
zeroed fields, three preservation/four bit8 mathematical vectors, exact
capture/repair/string/clock/caller sites and full direct DOL census checked
independently. Generic pin checks do not establish all prose/runtime claims.
Raw ELF/listings/generator stay `/tmp`; no save records/assets/raw bodies committed.

SAVE-01 now has local write/validate/select/restore service contracts. Remaining:
full continueGame/admission/party/map/pointer repair, record schemas and later
switch lifetime, read-all/async write/header/backup/SDK service internals, actual
slot/value/extent/alias/IRQ/clock/namespace domains and original save/load/
corruption/recovery/multi-slot traces. Native persistence/full-game remain OPEN.
