# Complete event integer/mask IF and lexical continuations

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete six integer IF and two mask IF helpers,plus scoped dispatcher12..33
regions in evtmgrCmd800316b8/13b8. Companion
[float/string predicates](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) completes
all twenty IF helpers. Read [integer routing](EVENT_INTEGER_OPERAND_NAMESPACES.md)
and [dispatch protocol](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify:`python3 tools/verify_event_if_predicates.py` checks twenty full helper
hashes,strcmp's full hash,containing dispatcher fingerprint,416 instructions
and25 data spans including all22 dispatch targets. Static original DOL evidence;
no original execution or complete source/domain claim.

## Actual12..33 dispatch table

Table is80304fd0+4*opcode. Compare operand0 L against operand1 R;function naming
and executable address order must not substitute for reading this table.

| Opcode | Dispatcher target | Helper / role |
| --- | --- | --- |
| 12 | 8003198c | evt_if_str_equal80035794/f0 |
| 13 | 8003199c | evt_if_str_not_equal800356a4/f0 |
| 14 | 800319ac | evt_if_str_small800355b4/f0 |
| 15 | 800319bc | evt_if_str_large800354c4/f0 |
| 16 | 800319cc | evt_if_str_small_equal800353d4/f0 |
| 17 | 800319dc | evt_if_str_large_equal800352e4/f0 |
| 18 | 800319ec | evt_iff_equal80035210/d4 |
| 19 | 800319fc | evt_iff_not_equal8003513c/d4 |
| 20 | 80031a0c | evt_iff_small80035064/d8 |
| 21 | 80031a1c | evt_iff_large80034f8c/d8 |
| 22 | 80031a2c | evt_iff_small_equal80034eb8/d4 |
| 23 | 80031a3c | evt_iff_large_equal80034de4/d4 |
| 24 | 80031a4c | evt_if_equal80034d18/cc |
| 25 | 80031a5c | evt_if_not_equal80034c4c/cc |
| 26 | 80031a6c | evt_if_small80034b80/cc |
| 27 | 80031a7c | evt_if_large80034ab4/cc |
| 28 | 80031a8c | evt_if_small_equal800349e8/cc |
| 29 | 80031a9c | evt_if_large_equal8003491c/cc |
| 30 | 80031aac | evt_if_flag8003485c/c0 |
| 31 | 80031abc | evt_if_not_flag8003479c/c0 |
| 32 | 80031acc | ELSE scanner |
| 33 | 80031b38 | END_IF handler2 |

Every helper and32/33 returns handler2. It clears the opcode and fetches again
in the SAME dispatcher invocation;branch evaluation/skip does not yield or
consume a new manager credit. The current command's next pointer was already
advanced by the fetch handler. No conditional stack is pushed or popped here.

## Integer comparisons24..29 and query order

Capture E.argument18 as A. Load A[0],call evtGetValue(E,A[0]) and retain L.
AFTER that query,load A[1] using the captured A,call evtGetValue(E,A[1]) to R.
Both queries always occur;no short circuit,constant folding or upfront loading
of both encoded operands. The first query can modify the second word before it
is loaded. A changed E.argument18 does not redirect the second word's base.
L stays retained even if the second query changes the first operand's storage.

Compare signed32 L against R with `cmpw`. Acceptance is24:L==R;25:L!=R;
26:L<R;27:L>R;28:L<=R;29:L>=R. No float conversion,+0.5 rounding or unsigned
comparison exists. Operand API coercion remains exactly the linked reader's
responsibility. On acceptance return2 with next14 unchanged. On rejection,
load E.next14 AFTER the queries,run the false scanner below,then return2.
Thus resolver changes to next14 affect the start of a rejected branch's scan.

## Mask IF30/31:one resolved value,one raw mask

Capture A,load A[0] and resolve it through evtGetValue. AFTER resolution load
RAW mask M=A[1] from retained A. No evtGetValue on M. AND all32 bits of the
resolved word V with M. IF_FLAG30 accepts if intersection!=0;IF_NOT_FLAG31
accepts if intersection==0. Zero M always fails30 and accepts31. Matching one
bit of a multi-bit M suffices for30;no all-bits test,flag-family decoder,byte
narrowing or sign comparison is used. A tag-shaped mask remains its raw bits.
Accepted returns2 unchanged next;rejected uses the same false scanner. Resolver
can modify the mask word before its load. The raw/resolved distinction differs
from two-query integer comparisons and must remain explicit in the implementation.

## Complete common false scanner for12..31

All twenty helpers contain this scanner,starting at live E.next14 with signed
depth0. For each header word W,derive op=low16(W) and n=S16(high16(W));advance
pointer to AFTER header+operands by4+4*n BEFORE classification. Rules:

- op12..31:increment depth and continue.
- op32 (ELSE):if depth==0,stop;otherwise continue without changing depth.
- op33 (END_IF) OR op1:decrement depth;if result<0,stop;otherwise continue.
- Every other opcode:continue without changing depth.

On stopping store the AFTER pointer to E.next14. A false IF reaching its
same-level ELSE skips the ELSE handler and starts the else body. If no ELSE is
encountered,it consumes the matching END_IF and starts after it. Nested ELSE
markers do not affect nesting. Opcode1 acts like a closing delimiter;DELETE2
is not a stop. SWITCH34/35 and loop markers do not affect this scanner.
These are actual numeric rules,not a structured-source validation pass.

No argument resolution/callback occurs during scanning. It reads signed16
counts,not the dispatcher's stored low8 argument count,and compares low16
opcodes,not low8. There is no script-length/null/missing-end/bad-count check.
Malformed word counts can move backward or revisit a header;no bounded failure
result is defined by the local code. Valid script/pointer domain remains an
implementation requirement pending source/runtime evidence.

## ELSE32 and END_IF33

ELSE32 runs only when an accepted IF's body reaches it normally (a false IF's
scanner has already consumed the matching ELSE). Its local scanner starts from
E.next14 at depth0 and uses the same signed count/advance-before-classify rules.
It counts12..31 as openers and33 OR1 as closers;on decrement<0 it stops and
stores the AFTER pointer. Unlike the false scanner,it ALWAYS ignores ELSE32,
including at depth0. It does not enter a later same-level else body. Then2.

END_IF33 only assigns handler2. No scan,state write,argument query or stack
mutation. It is consumed naturally after an accepted IF without ELSE;skipping
scanners can also consume it without dispatching it. Both paths continue in the
same invocation,but pointer advancement belongs to fetch versus scanner.

## Acceptance cases and remaining dependencies

Cover all22 actual table entries;two-query ordering/retained base/retained L,
second operand mutation,next14 mutation on rejection;signed min/max and equality;
raw zero/multi-bit/high-bit/tag masks;false branch with/without ELSE,nested IFs
and nested ELSE,accepted body reaching ELSE,END_IF no-op;scanner consumes marker
rather than dispatches it,signed16 count versus byte count,low16 versus cached
byte,op1 versus2 and unrelated SWITCH/loop markers;no automatic yield.

Float/string behavior is in the companion contract. Other VM opcodes,script
composition,pointer/alias/numeric domains and original branch/pause/multicredit
traces remain EVT-01 work;local closure does not prove a complete native VM.
