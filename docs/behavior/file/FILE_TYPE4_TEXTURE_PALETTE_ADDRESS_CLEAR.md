# Complete type4 texture-palette address clearing

US G8ME01 revision0; ID2,2026-10-12. Complete type4 block8018b220..8018b31c
in fileGarbageDataAdrClear,plus linked dispatch. Verify
`python3 tools/verify_file_type4_relocation.py`;read
[address setup](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md),
[dispatch/entry resolution](FILE_RELOCATION_TYPE_DISPATCH.md) and
[file move](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
Static source recovery;no original resource clear/move/render execution trace.

## Entry resolution precedes this block;signature precedes pointer guard

Clear receives file ENTRY E. Its dispatch first reads type byte E01,holder
E.a0 and holder.word0 payload B. Type4 then checks WORD B00==0020af30.
Mismatch calls OSPanic(`filemgr.c`,line153,original invalid-version format).
If backend returns,the caller continues without rechecking signature or returning
failure. Panic/platform behavior is a linked contract,not a locally invented
Boolean validator. No null guard precedes these entry/holder/payload accesses.

Read WORD B08,compare unsigned to B. If table<B,skip entire handler. Otherwise
retain root pointer in memory while processing nested slots;it is converted to
a relative word ONLY after the loop. Equality admits Clear and later writes0.
This is opposite Set's root>=B skip rule;no payload-size/range validation occurs.

## Same low16/live-count loop,absolute table until all slots finish

Initialize full index I=0. Before each iteration,reread WORD B04 COUNT;
compare unsigned LOW16(I)<COUNT. For admitted record,compute O=8*LOW16(I),
process O then O+4,increment full I by1 with word wrap. COUNT is not captured
and index is not an ordinary full-width array index. Counts above65535 can
revisit records indefinitely absent a live count change. No cap/yield/timeout.
All table accesses reread B08;its memory word stays absolute until final step.

## First slot clears flag before nested data,then converts descriptor pointer

Read current table[O] as D. Exact0 skips slot. Otherwise read BYTE D23. Exact0
skips the nested data conversion;ANY nonzero flag takes it:
store BYTE D23=0 FIRST;reread B08/table[O] as D2;read WORD D2.08,subtract B with
word wrap,store D2.08. Thus the data target is a fresh table lookup after flag
clear,not retained D under all alias domains.

Regardless of its flag branch,for every initially nonzero slot reread B08 and
current table[O] word Q;store table[O]=Q-B with word wrap. No separate Q!=0
or pointer-range recheck occurs at this later publication. Descriptor conversion
therefore follows data/flag conversion,opposite Set's descriptor-first sequence.

## Second slot uses flag02 with the same order

Read current table[O+4] as D. Zero skips. Nonzero reads BYTE D02. Any nonzero
flag is cleared0 before a fresh root/table lookup and D2.08 subtraction by B.
Flag0 skips nested data conversion. Then reread current table[O+4] and subtract
B from its word,regardless of whether the data branch ran. Slots/flags/data can
alias each other or root fields;preserve the exact later reads and store order.

After the loop exits,read CURRENT B08,subtract B with word wrap,store B08.
Even COUNT0 performs this root conversion after signature/guard admission.
No descriptor-table byte copy,heap/free/cache/DVD/callback call occurs here;
only the version-diagnostic branch can call the linked panic service.

## Shared descriptors and movement requirements

Flags prevent repeated data subtraction through another descriptor reference
only while their byte is nonzero. Descriptor-slot subtraction still occurs for
all initially nonzero slots. A shared descriptor's first visited flag can affect
later references;do not replace the sequence with independent native records or
clear all flags after all data operations. Zero data words are not preserved as
null when their flag admits conversion;they become -B modulo2^32.

File move requests this Clear(E) before copying ready resources,then moves
bytes,publishes holder.word0 and calls Set on current payload/type. Root/table/
descriptor/data aliases and original source domains determine whether that
composition is valid. Keep entry/holder/header/payload identities separate and
reuse original bytes outside source history. This is not a complete decoder
or proof that a relocated effect texture is rendered correctly.

Acceptance:root threshold equality,COUNT0/large/live change,all zero/nonzero
slots,flag0/nonzero variants,flag clear before fresh data target,retained versus
live pointer differences,shared descriptors,data0,word wrap and root last.
Compare Set→Clear and Clear→move→Set on source-shaped resources separately from
authored edge cases;binary fingerprints do not measure original runtime.

Type5 nested graph,source formats/consumers/panic/cache/worker/platform/scene
and full-game requirements remain open. Complete type4 local Set/Clear behavior
is supplied;successful native loading/rendering and original traces are unproven.
