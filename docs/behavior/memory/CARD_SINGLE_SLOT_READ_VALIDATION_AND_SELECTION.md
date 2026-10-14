# Single-slot card read, validation and copy selection

US G8ME01 revision0; recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete read_main800b0524/898hex,
550 instructions and15-entry phase table. Addresses/fields/sizes hexadecimal;
counts/arguments decimal. Static deductions, not an original save/load trace.
Read [dispatcher/callbacks](CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md),
[record writer](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md).
This reader fills card buffers; it does not restore live gameplay namespaces.

## Entry and phase/wait ownership

Read W=[8041eab8]. W9c!=0: captured W.e0=8; fresh half0 OR0200; another fresh
half0 clears2; exit without phase increment. W9c==0 and half0 MASK1 set enters
the same three fresh CARDGetResultCode requests described in the dispatcher:
first-1 exits; second equal to freshly read W9c exits; otherwise store third
result to fresh W9c and clear fresh half0 MASK1. No phase increment there.

Otherwise unsigned W.d8 selects15-entry table80313f7c; >14 goes to common
increment without phase action. No slot/pointer/buffer/operation bounds guard.
Every phase branch below, including finish14 and invalid d8, ends by fresh
W.d8 read/add1 wrap32/store, then exits. Child/callback changes to d8 are observed;
phase is not held across requests as an immutable local state.

## Complete phase sequence

| Phase / destination | Local action |
| --- | --- |
| 0 /800b05a4 | Busy1; bounded CARDProbeEx(0,sizeOut,sectorOut), normalize successful sector!=8192 to-2; publish W9c/W8 and clear1 |
| 1 /800b0670 | CARDSetFastMode(1); fresh busy1; CARDMountAsync(W8,W4,mountDetachCallback,mountAttachCallback) |
| 2 /800b06ac | Busy1; CARDCheckAsync(fresh W8,checkCallback) |
| 3 /800b06d0 | memcard_open800aecb0 request; result ignored locally |
| 4 /800b06d8 | Busy1; read header8192 bytes, file offset0 into captured W.ac scratch, CARDReadAsync(fresh W+1c,scratch,8192,0,readCallback) |
| 5 /800b0708 | Header validation below; failed result sets captured W.half0 MASK0400, no W9c error or abort |
| 6 /800b081c | Busy1; read primary16384 bytes into W.ac, offset wrap32(2000+(W.a4<<14)); file handle fresh W+1c/readCallback |
| 7 /800b0854 | Validate primary scratch; publish fresh W.b4; copy its timestamp to W.c8/cc |
| 8 /800b0980 | Busy1; read secondary16384 bytes into W.ac, offset wrap32(12000+(W.a4<<14)); fresh handle/readCallback |
| 9 /800b09bc | Validate secondary scratch; publish fresh W.b8; copy timestamp to W.d0/d4 |
| 10 /800b0ae8 | Select copy and read it into resident slot buffer, or mark resident record invalid |
| 11 /800b0be8 | Busy1; bounded CARDGetStatus802b1f0c(fresh W8,fresh W+10); store raw/forced-128 W9c, clear1 |
| 12 /800b0c4c | Busy1; bounded CARDClose802b0cf8(fresh W+1c); store raw/forced-128 W9c, clear1 |
| 13 /800b0cac | Busy1; bounded CARDUnmount(fresh W8); store raw/forced-128 W9c, clear1 |
| 14 /800b0d0c | Captured W.e0=0; fresh half0 OR0100; another fresh half0 OR2000; another clears2 |

Callback identities: read800af020,check800af090,mountDetach800af0e4,
mountAttach800af0ac. Busy writes precede async requests; immediate async return
values are ignored. Scratch/offset args are captured before a separate fresh
W supplies the file handle. Do not redirect all arguments to one owner.

ProbeEx and final status/close/unmount loops initialize counter0 and cap1000000
before their first poll; EXACT-1 retries, signed counter>1000000 forces-128.
The all-busy path makes1000001 child calls, with no OS sleep/timer/yield. This
is an iteration bound, not measured card timing. Probe result0/sector8192 stores
W9c=0,W8=0; result-2 stores9c=-2,8=0; others raw9c,8=-1, with fresh owners per
publication. Probe's sizeOut is not locally checked. Later entry owns failures.

## Header validation covers7750 bytes, not the slot extent

Use captured W.ac as H. Read H.word1ff8 sum and1ffc inverse; require exact
inverse==~sum. Failure bypasses byte scan. Otherwise sum UNSIGNED bytes0..1e45:
484 iterations×16 bytes=7744, then a six-byte scalar tail. Compare computed
wrapping32 sum to captured stored sum. No header length/version/string check.

Validation result false ORs0400 into the original captured W.half0; true skips
that write. Both continue to common d8 increment. This is a validation latch,
not an IO abort, header repair or inferred backup rejection. Header checksum
coverage excludes stored checksum words and unused header bytes.

## Primary and secondary record validation order

Each phase independently captures record P=W.ac into GPR30. Validation order:

1. P.word3ff4 must be8800, otherwise false without strcmp/byte scan.
2. strcmp8026d464(P+3ff0,version pointer[80416290]) must return0. No local
   NUL/length/extent guard; pointer/child/string validity remains a provider.
3. Capture P.word3ff8 sum; P.word3ffc must equal its full32 complement.
4. Sum unsigned bytes0..225f,550 iterations×16; exact computed==stored sum
   yields true. No length-driven scan or per-field namespace validation.

Publish bool0/1 to FRESH W.b4(primary) or W.b8(secondary). Then reload W and
its ac AGAIN, read raw record11f0/high then11f4/low; store low first into W.cc
or d4, then high into W.c8 or d0. These timestamp reads happen EVEN if record
validation failed, and may use a different scratch owner from captured P after
strcmp. Do not infer timestamp validity from publication or freeze pointers.
The writer maps these offsets to saved GP11e8/11ec OSGetTime, not GP11f0 count.

## Copy selection uses signed64 timestamps; ties select secondary

Phase10 captures W. Read b4; if nonzero read b8. If both nonzero, compare
raw pairs primary(c8,cc), secondary(d0,d4) as SIGNED64: XOR high sign bits then
subtract/borrow implements signed order. Secondary<primary selects index0;
otherwise index1, including exact equal timestamps. No wrap/newness heuristic
or subtraction-overflow time policy replaces this source comparison.

If only primary valid choose0; only secondary choose1. Both invalid choose-1.
The later b8 tests reload that field; stable truth-table models do not freeze
all concurrent validation producers. Selected index0/1:

- Capture raw W.a4 slot and W.a8 resident base before busy publication.
- Resident destination=wrap32(base+2000+(slot<<14)).
- File offset=wrap32(2000+(slot<<14)+(selected<<16)), length4000hex.
- Captured W.half0 OR1, then fresh W+1c handle for CARDReadAsync with retained
  destination/offset/length/readCallback. No revalidation of this re-read here.

Index-1: captured W.a4/a8 forms resident addressbase+(slot<<14); read half at
address+2000, OR MASK2/store. No W9c error, busy1, buffer clear, copy or device
read in that arm. Later normal cleanup/finish still executes unless entry sees
an independent child error. Thus completion flags alone do not prove a valid
loaded gameplay record; the resident record's invalid bit is a separate owner.

## Restore boundary and evidence

There are NO GP/pouch/fbat/badge-shop/game namespace memcpy requests in this
complete reader. It selects/fills resident card buffers and flags; actual live
restore, pointer repair, switch resets and scenario admission belong to later
load consumers. Write-side bank inclusion and reader acceptance do not alone
prove namespace round-trip. SDK errors/backup repair/write/read-all remain
separate; primary/secondary selection is not a complete recovery service.
ABI32-byte frame/GPR30..31/LR; no independent scalar result or IRQ guard.

[Manifest](../../../tools/card_single_slot_read_evidence.json):13 spans/
565 anchors (all550 body words/fifteen table words),seventeen direct calls,
zero indirect calls/one CTR table jump. Five validation/eight signed-selection
mathematical vectors, exact header/record scan lengths/call/flag/timestamp/table/
publication sites and fresh ISO pins checked independently. Generic checks do
not prove semantic/runtime claims. Raw ELF/listing/generator stay `/tmp`; no
record blobs/assets/body distribution. SAVE-01/native/full-game remain OPEN.

## Live restore child delivery (2026-10-13)

[Complete cardCopy2Main](CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md) now
closes local resident-to-live copy/preservation rules. It adds no validation
guard; full continueGame/admission/schema/lifetime/original round-trip remain
separate. This reader still performs resident loading only.
