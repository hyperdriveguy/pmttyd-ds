# Switch storage: corrected bit encoding, bank extents and initialization

US G8ME01 disc0 revision0, independently recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete12-body swdrv family, static
instruction evidence only. Replaces the earlier multiplied-by-five bit mapping.
Original card save serialization, complete switch-to-story mapping and runtime
lifetime are not established here. [Tattle flag owners](../battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
provide a concrete persistent/local-bank consumer.

## Bank layout proven by getters and initialization

Let GP=word80415fd0 (r13-28496). Each accessor reads GP at its shown site;
initializers reload it for each clear. Offsets/sizes below are hexadecimal.

| GP field / extent | Local source role | Initial reset |
| --- | --- | --- |
| 174 /4 bytes | Special global byte-index0 value is actually a FULL word | swInit stores word0; swReInit preserves |
| 178 /400 bytes | Global flag words:256 words,8192 bits | swInit clears; swReInit preserves |
| 578 /800 bytes | Global byte storage:2048 bytes | swInit clears; swReInit preserves |
| d78 /40 bytes | Underscore/local flag words:16 words,512 bits | Both clear |
| db8 /400 bytes | Underscore/local bytes:1024 bytes | Both clear |

These extents come from actual memset requests, not accessor bounds checks.
Accessors enforce no index range. The special global byte0 is stored at174,
not578; global byte indices1..2047 use578+index, leaving physical byte578
outside that ordinary index range. No local byte0 exception exists.

Global byte getters/setters swByteGet8008582c/24 and swByteSet80085850/24:
index0 loads/stores WORD GP174 with no8-bit narrowing. Other indices read
unsigned byte or store low8 at GP578+wrapping index. `_swByteGet`80085744/10
and `_swByteSet`80085754/10 always use GPdb8+index with byte width.
These symbol names must not cause a port to truncate the original story/control
word at global byte-index0 to255.

## Exact bit arithmetic and correction

For raw signed32 index N, the source computes quotient q=trunc(N/32) using
srawi5 then addze. Its remainder sequence is:

`S=U32(N)>>31; X=wrap32((U32(N)<<27)-S); R=wrap32(rotl32(X,5)+S)`.

This is the compiler's signed remainder construction. For N>=0, the left
shift discards all but LOW5 original bits; rotating them back produces N&31.
For N<0, R represents the signed truncation remainder N-32*q. It is NOT
`((N&31)*5)&31`. Rotate5 must be interpreted together with the preceding
left-shift27/sign correction, not as a separate layout encoding.

Address=wrap32(GP+bankOffset+wrap32(4*q)). Mask is PPC `slw(1,R)`:
retain low6 shift-count bits; count>=32 produces0, otherwise1<<count.
For a valid nonnegative index, this is conventional word N/32 and bit N%32.
For invalid negative nonmultiples of32, remainder low6>=32 and mask0; a
negative multiple can address a PRECEDING word with mask1. The source still
loads/stores that address and has no protective clamp or invalid-index return.

Global leaves use offset178; underscore leaves use offsetd78:

| Operation | Global body / size | Underscore body / size | Ordered behavior |
| --- | --- | --- | --- |
| Get | swGet800858b4/48 | _swGet800857a4/48 | Load word, AND mask; return exactly0 or1 via nonzero normalization |
| Set | swSet800858fc/40 | _swSet800857ec/40 | Load word, OR mask, store same word; no scalar status |
| Clear | swClear80085874/40 | _swClear80085764/40 | Load word, AND complement mask, store same word; no scalar status |

All body sizes are hexadecimal bytes. Set/get/clear do not allocate, change
bank pointer, expand storage, select a story state or save to the card.

## Complete initialization and reinitialization order

swInit80085980/78 reloads GP for each of four ordered memset calls:
GP178 length400, GP578 length800, GPd78 length40, GPdb8 length400, all fill0.
Then reload GP again and store word0 to174. These are independent captures;
a mutating provider must not silently retain one GP for the entire routine.

swReInit8008593c/44 reloads GP independently and clears only GPd78 length40,
then GPdb8 length400. It neither clears global flags/bytes/word174 nor
copies card/save data. This demonstrates local reset versus preserved-global
storage at this service boundary, not persistence across every scene/load path.

## Concrete index binding and validation

The [Tattle setter/checker](../battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
uses global switch4474+kind and separate battle work bits by kind. With table
kinds0..230, these switches are4474..4704, inside the8192-bit initialized bank.
This is one verified index-to-gameplay namespace. It is not the complete story
switch map and does not redefine global byte0 as that flag namespace.

[Evidence metadata](../../../tools/switch_monosiri_flags_evidence.json) pins all
12 swdrv bodies, two Tattle flag bodies, static group table/list spans and
instruction anchors. Run the existing generic DOL verifier with that manifest.
Independent arithmetic recheck covers every global index0..8191, every local
index0..511, Tattle4474..4704 and signed edge cases; no original code is executed.

Remaining SAVE-01: complete story/namespace writers, exact card record inclusion/
reload/backup/checksum/error/timing and all original persistence traces. The
bank word counts, local access/reset contracts and rotate encoding are answered.
No native PM2D migration or original save round-trip is claimed.

## Write-side serialization delivery (2026-10-13)

[Complete cardWrite](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md)
now proves GP0..13d7 copied at record+8, including all five bank extents above.
This adds original write-side inclusion, not read/load/reset/lifetime or an
original round-trip claim. Exact record/checksum and limitations are linked.

## Live restore delivery (2026-10-13)

[Complete restore](CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md) copies all
five serialized banks back through GP+8 record mapping. Its explicit subsequent
repairs/resets do not overlap the banks; no swReInit request occurs locally.
Full caller admission/later switch lifetime and original round-trip remain open.

## Continuation lifetime exception (2026-10-13)

[Complete continueGame](SAVE_CONTINUE_GAME_PARTY_AND_DESTINATION_DISPATCH.md)
requests swInit and swByteSet(0,0) in its switch4==23 arm after live restore.
That source branch clears banks before aaa_00/prologue request; do not extend
the restore leaf's byte-copy proof into unconditional bank lifetime. Actual
switch4 providers/caller admission and original execution remain open.
