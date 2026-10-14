# Original cardWrite record assembly and checksum

US G8ME01 revision0; recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete cardWrite800b1e7c/2echex,
187 instructions. Addresses/fields/sizes hexadecimal; counts decimal. Static
source, not original card IO/round-trip observations. Read
[switch storage](../memory/SW_SET.md), [native adapter](NATIVE_SAVE_FORMAT.md).
This closes local preparation, not cardMain/read/write/backup service internals.

## Busy gate and raw slot addressing

Read W=[8041eab8], W.half0. MASK2 set exits without local preparation; no queue
or completion result is assigned. Otherwise supplied raw slot r3 forms relative
P=wrap32(2000+wrap32(slot<<14)); then fresh W.a8 is added to obtain record P.
No slot0..3, signed/range/allocation/extent guard.

Before obtaining P, source order: captured W.e0=1; fresh W.a0=raw slot; fresh
W.d8=0; fresh W.9c=0; fresh W.half0 OR2; another fresh half0 clears MASK0300;
another clears MASK0800. Repeated W reloads are not one stable snapshot.
P is retained in GPR31 afterward. Request marioGetPtr8005e0c0 at800b1f08,
retain Mario M/GPR30; memset(P,0,4000hex) at800b1f1c. No null/result guard or
rollback of already published busy/state fields if a child fails/nonreturns.

## Live GP snapshot changes before serialization

Capture GP=[80415fd0]; copy raw M8c/90/94 to GP11d4/11d8/11dc in order.
Request marioGetPartyId80141050, pass result to partyGetPtr800bf1b4. Nonnull
partner supplies signed byte31; otherwise retained M.byte247. Fresh GP11e0
receives sign-extended word. Repeat independently with marioGetExtraPartyId
80140fb8/partyGetPtr and fallback M.byte248, fresh GP11e4. This does not assume
the two partner snapshots share an owner or preserve unsigned kind IDs.

Request OSGetTime8029d84c; fresh GP receives returned LOW word at11ec BEFORE
HIGH word at11e8. Fresh GP.word11f0 increments wrap32. Then separate fresh
GP.word0 reads/writes clear MASK1 followed by MASK2. Separate fresh writes
set GP1278,127c,1280 and GP10 to0, in that order. These mutate LIVE game work,
not only a temporary saved copy; earlier modifications are retained locally.

## Six ordered block-copy requests

All sizes here are hex. Each memcpy return is ignored; sources may change
through previous children or overlap P. No transaction/alias/stable-owner guard.

| Record offset | Size | Source/request |
| --- | --- | --- |
| 08 | 13d8 | Fresh GP, memcpy800b1ffc |
| 13e0 | 05d4 | pouchGetPtr800d5eb0 return, memcpy800b2010 |
| 19b4 | 0500 | fbatGetPointer8004829c return+4c, memcpy800b2028 |
| 1eb4 | 0124 | bdsw pointer at8041ece8, memcpy800b2038 |
| 1fd8 | 0258 | yuwp pointer at8041ed28, memcpy800b2048 |
| 2230 | 002b | Raw fixed source803bd0d4, memcpy800b2060 |

All copied blocks end below2260; remaining bytes retain zero from memset in
the ordinary nonoverlapping, normally returning child domain. Do not infer full
semantic field layouts solely from these bulk-copy sizes. Pouch, field-battle,
badge-shop/game work and fixed source content still need namespace/provider
mapping and read-side interpretation before original round-trip is established.

The GP copy DOES include the proven switch extents. Record offsets are GP+8:

| Proven namespace | GP offset:size | Record offset:size |
| --- | --- | --- |
| Special global byte0 full word | 174:4 | 17c:4 |
| Global256-word flag bank | 178:400 | 180:400 |
| Global byte storage | 578:800 | 580:800 |
| Local16-word flag bank | d78:40 | d80:40 |
| Local byte storage | db8:400 | dc0:400 |

Thus write-side bank inclusion is proven, including local banks; no selective
omission of locals or truncation of special global byte0 occurs in this copy.
Read/load/restore resets, story-index writers and persistence lifetime are
separate. Storage inclusion alone does not close SAVE-01 round-trip.

## Flags, version trailer and additive checksum

After copies, read P.half0, clear MASK1/store; reread, clear MASK2/store.
Request strcpy(P+3ff0,version pointer[80416290]) at800b2084. Source version
contents/lifetime/length and strcpy implementation remain providers; the local
call supplies no bound or format-validation guard. No native PM2D/FNV format
should be substituted as an original record description.

Then store P.word3ff4=2260(decimal8800), P.word3ff8=0, P.word3ffc=ffffffff.
Checksum scans offsets0..225f:550 iterations,16 unsigned byte loads per
iteration. Source loads/adds in order, using two eight-byte groups; all bytes
are counted once. Sum is a wrapping32 word, though a stable8800-byte buffer
has maximum2244000, so ordinary unsigned byte sum cannot overflow.

Store sum at3ff8; REREAD that word, bitwise complement, store at3ffc. Final
inverse depends on the fresh stored-word read, not an assumed retained sum if
other owners mutate it. The checksum covers the first2260 bytes only: it does
not include version at3ff0, length at3ff4, sum/inverse themselves, or the rest
of the4000-byte slot buffer. It is an additive BYTE sum, not a halfword sum,
CRC/FNV, SDK card-directory checksum or measured corruption/repair policy.

No card-device write/read/flush/wait is requested in this complete body. The
work publications admit later asynchronous owner services; their IO/timing/error/
backup/completion behavior remains open. ABI16-byte frame/GPR30..31/LR; no
independent scalar return/success code inferred. No local IRQ masking.

## Evidence and remaining persistence work

[Manifest](../../../tools/card_write_record_evidence.json):eleven spans,
all187 body words/sixteen direct calls/no indirect calls. Six raw slot/four
checksum mathematical vectors, six copy requests, masks/GP publications/
checksum iteration/length/complement/call sites checked independently. Generic
verifier checks pins/targets, not complete serialization semantics/runtime.
Raw ELF/listing/generator stay `/tmp`; no record blobs/assets/code bodies committed.

[Single-slot reader validation/selection](../memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md)
now closes that local read service; live namespace restore remains separate.
Next: async write/read-all/header/create/erase/copy/error/backup and SDK services,
precise persistent namespace schemas, source pointer/index/extent/alias/IRQ/time
domains, and original multi-slot/save/load/corruption/recovery traces. Native
save replacement and SAVE-01/full-game remain OPEN.
