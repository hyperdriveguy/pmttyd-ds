# Card dispatcher, format/check phases and completion callbacks

US G8ME01 revision0; recovered2026-10-13 for
[SAVE-01](../../requests/SAVE-01.md). Complete cardMain800b2508/678hex (414
instructions) plus eight callback leaves800af004..800af0e8 (57):471 total.
Addresses/masks/fields hexadecimal; counts/arguments decimal. Static deductions,
not measured card timing, successful persistence or observed error recovery.
Read [record writer](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md).
Child write/read/create/header routines and SDK services remain separate.

## Optional probe/change latch before operation dispatch

Load W=[8041eab8], half0. MASK8000 set requests CARDProbe802af55c(0) at800b2534.
Convert EXACT return0 to stored value-1, every nonzero return to0. Fresh W.e4
unequal to new value sets its half0 MASK4000; equality leaves flag unchanged.
Another fresh W.e4 receives new value. Another fresh W.ec!=0 decrements wrap32;
zero unchanged. Negative countdowns are decremented too, with no saturation.
MASK8000 clear skips probe/change/countdown only, not operation dispatch.
No original error meaning is assigned to arbitrary probe returns.

## Complete operation table

Fresh W.worde0 is unsigned compared to8. Values>8 exit; values0..8 index raw
nine-word table8031401c and branch through CTR. No per-entry busy/active/null
validation before that dispatch. Table destinations:

| Operation | Destination / request |
| --- | --- |
| 0 | 800b2b64 exit |
| 1 | 800b25a8 → write_main800b139c |
| 2 | 800b25b0 → write_header_main800b0dbc |
| 3 | 800b25b8 → read_main800b0524 |
| 4 | 800b25c0 → read_all_main800afa2c |
| 5 | 800b25c8 → create_main800af148 |
| 6 | 800b25d0 → local format phases below |
| 7 | 800b285c → local check/delete phases below |
| 8 | 800b2b64 exit |

Operations1..5 request the child once then exit, ignoring returned values.
This closes their caller dispatch, not complete child protocols. No pouchGetHP,
sequence or checksum call exists in this dispatcher; the former sketch's
provisional work-field map/target assumptions are superseded.

## Shared format/check entry and result waiting

Both operations6/7 first read the currently returned W.9c. Nonzero: captured
W.e0=8; fresh half0 OR0200; another fresh half0 clears MASK2; exit. Substate
and result retained locally. Zero: read current half0 MASK1. Set enters result
waiting; clear selects signed W.d8 substate. Busy MASK1 is distinct from operation
MASK2; callbacks below clear only1.

Waiting, independently in each owner, makes up to THREE fresh requests:

1. Fresh channel W8 → CARDGetResultCode802ac744. EXACT-1 exits.
2. Reload W/channel, request again. Reload W.9c AFTER child; if equal to this
   second result, exit WITHOUT local busy clear/result store.
3. Use channel from that retained W; request again. Reload W, store third
   result to9c. Reload W, clear half0 MASK1; exit.

First/second/third results can differ; do not coalesce polls or store the first
nonbusy result. Equal-second branch deliberately leaves local state untouched.
Callback publications are independent, so a normally completed async request
can clear busy through its callback even when polling sees an equal result.

## Substate0: bounded synchronous probe loop

Both owners' substate0 ORs current half0 MASK1, initializes counter0/cap1000000,
then repeatedly requests CARDProbeEx802af594(0,sizeOut,sectorOut). The stack
outputs are +10/+14 in operation6, +08/+0c in operation7. Return EXACT-1:
increment counter, signed counter>1000000 forces result-128 without another
request; otherwise retry. All-busy path makes1000001 SDK calls. This is an
iteration bound, not a measured delay, millisecond timer or yield policy.

Returned0 with sectorOut!=8192 changes result to-2. Other results/sector values
keep raw result. Publish through fresh W reloads:

| Final result | W9c | W8 |
| --- | --- | --- |
| 0 | 0 | 0 |
| -2 | -2 | 0 |
| Every other | Raw result | -1 |

Then fresh half0 clears MASK1. Continue to common substate increment below;
there is no local early exit on failed result after this publication. The next
invocation's entry gate owns that failure. SizeOut is not locally validated.

## Format operation6 signed substates

| d8 | Ordered actions before common increment |
| --- | --- |
| 0 | Probe loop above |
| 1 | CARDSetFastMode802ac9ac(1); fresh W.half0 OR1; fresh W supplies channel8/workspace4 to CARDMountAsync802afc9c(channel,workspace,mountDetachCallback800af0e4,mountAttachCallback800af0ac) |
| 2 | Current half0 OR1; fresh channel8 → CARDFormatAsync802b0720(channel,formatCallback800af074) |
| 3 | Current half0 OR1; counter0; loop CARDUnmount802afed8(fresh channel8), retry EXACT-1 up to same cap; store raw/forced-128 to fresh W9c; fresh half0 clears1 |
| 4 | Captured W.e0=0; fresh half0 OR0100; another fresh half0 clears2 |
| Other signed values | No phase action |

The unmount cap is initialized on the first poll path before the first retry
increment; the source does not use a previous invocation's counter. Direct
mount/format results are ignored locally. No automatic rollback/unmount/error
recovery inferred from merely requesting them.

## Check/delete operation7 signed substates

| d8 | Ordered actions before common increment |
| --- | --- |
| 0 | Probe loop above |
| 1 | Same fast-mode/busy/mount callback order as format |
| 2 | Current half0 OR1; fresh channel8 → CARDCheckAsync802af468(channel,checkCallback800af090) |
| 3 | Current half0 OR1; counter0; CARDDelete802b1a44(fresh channel8,fixed name pointer802cb0e0), retry EXACT-1 to cap; EXACT-4 becomes0, otherwise raw/forced-128 retained; store fresh W9c; fresh half0 clears1 |
| 4 | Current half0 OR1; bounded CARDUnmount loop/store/clear as above |
| 5 | Captured W.e0=0; fresh half0 OR0100; another fresh half0 clears2 |
| Other signed values | No phase action |

Delete-name contents/lifetime, SDK validity/error meanings and branch admission
remain providers. EXACT-4 normalization is local code, not general card recovery.
Delete limit/pointer are initialized before first call, then reused on retries.

BOTH owners' common tail ALWAYS reloads W, reads d8, increments wrap32/store,
then exits. This includes invalid signed substates and finish4/5, even after e0
has been set0. It does NOT run for entry error or busy-wait branches. A callback
or child may have changed d8 before this fresh read; no retained substate+1
substitution. Main ABI48-byte frame/GPR29..31/LR; no independent scalar result,
local OS clock, sleep or IRQ mask. Table jump is not an indirect callback.

## Eight complete completion callbacks

statusCallback800af004,readCallback800af020,writeCallback800af03c,
createCallback800af058,formatCallback800af074,checkCallback800af090 each:
load fresh W, store supplied r4 result to9c; load W AGAIN, read half0, clear
MASK1/store; return. Input channel r3 is overwritten/ignored. No operation/
generation/channel/active/extent check; result target can differ from clear
owner if W changes. No substate increment, MASK2 clear, IO or scalar result.

mountAttachCallback800af0ac first stores raw r4 to captured W9c. EXACT-6 or-13
then loads W fresh and stores9c=0; every other result skips that rewrite.
Always reload W afterward and clear half0 MASK1. Do not collapse the two result
stores or reinterpret other SDK errors. mountDetachCallback800af0e4 returns
immediately with no local read/write/call. None has a stack frame/IRQ guard.

## Validation and remaining persistence work

[Manifest](../../../tools/card_dispatch_completion_evidence.json):24 spans/
480 anchors (all471 body words/nine table words),23 direct calls/no indirect
calls, one CTR table dispatch. Six attach/four probe/five ProbeEx/four waiting
mathematical vectors, exact table/phase/cap/callback/store/child sites and fresh
ISO pins checked independently. Generic verifier does not validate semantic/
runtime claims. Raw ELF/listings/generator stay `/tmp`; no record/assets/raw
bodies committed.

SAVE-01 now has complete local dispatcher and callback contracts. Remaining:
write_main/read_main/read_all_main/header/create/buffer/init/copy/erase and SDK
internals, version/checksum reader/corruption/backup/load namespaces, actual
work/channel/result/callback/index/extent/alias/lifetime/IRQ/provider domains,
original IO/error/multi-slot/save/load traces and native persistence binding.
Full save service and full-game fidelity remain OPEN.
