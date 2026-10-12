# Event GOTO and loop opcodes4..8: raw counters,scanners and stacks

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evt_while80035954/d4 and scoped whole4..8 dispatcher regions in
evtmgrCmd800316b8/13b8. Read [integer operands](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[label construction](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) and
[dispatch/credit](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify: `python3 tools/verify_event_loop_and_arithmetic.py`.
Static local behavior;no original loop/cadence trace claim.

| Opcode | Target | Role |
| --- | --- | --- |
| 4 | 80031760 | GOTO label or pointer |
| 5 | 800317d0 | DO push raw count/body address |
| 6 | 80031808 | NEXT via evt_while |
| 7 | 80031818 | BREAK_LOOP scan/skip/pop |
| 8 | 80031884 | CONTINUE_LOOP scan to closing command |

All return handler2 and continue in the SAME dispatcher invocation;no loop
iteration credit debit/automatic yield exists beyond original invoked commands.

## GOTO4 and missing-label behavior

Resolve operand0 through evtGetValue. AFTER that call,reload E.argument18 as
fallback pointer. If signed resolved target<-270000000,store the raw target
word as E.next14 directly. Typical0x80xxxxxx addresses are negative signed32;
this path is pointer use,not a label index. Exact-270000000 takes label lookup.

Otherwise scan EXACT16 label records ascending. Compare S8(E+1c+i) to the
resolved target word (no target narrowing). First match reads word(E+2c+4*i)
and uses it as next14. No match leaves fallback E.argument18 as next14,not a
valid terminal/default label or existing next pointer. In either case handler2.
The scanner's0xff empty records also compare as-1;no separate initialized-label
count is tested. Constructors retain duplicate label order and low8 IDs,while
GOTO compares signed8 against full resolved32. No null/bounds/target check.

## DO5 push and exact stack layout

Capture argument pointer. Read unsigned E.byte e (loop stack index),increment
with word arithmetic,store low8 to e. Reread e,sign-extend S8 and calculate
word offset4*index. Store argumentPointer+4 at E+e8+offset as BODY address.
Read RAW argument0 word and store at E+108+offset as retained COUNT operand.
There is NO evtGetValue count resolution,zero gate or initial body skip.
Handler2 fetches the already advanced next command. Nested loop arrays are
initialized by constructors only through bulk clear;indices start0xff (-1).
No stack overflow/underflow/index validation or parent stack inheritance.

## NEXT6 complete evt_while algorithm

Capture S8(E.e),retain byte-index-derived offset,read raw count R from
E+108+4*index. Exact R0:read body pointer E+e8+offset,store E.next14=body,
return2 without decrement,variable query or stack pop. This is endless-literal0.

For nonzero R,compare signed R with-10000000 (0xff676980):

- R>=-10000000:decrement word R by1,store that word in COUNT slot. Use it as N.
- R<-10000000:evtGetValue(E,R),retain V;evtSetValue(E,R,wrap32(V-1));use
  wrap32(V-1) as N. COUNT slot retains RAW operand R;setters' return is ignored.

If N!=0,read body pointer using the retained index offset,store next14=body,
return2. If N==0,reread unsigned E.e,subtract1,store low8 e,return2 without
changing next14 (already points past NEXT). No <=0 completion rule or automatic
loop marker reset;dispatch handler2 supplies opcode clear/fetch.

Thus variable-backed DO counts are resolved and UPDATED at every NEXT after
body execution,not snapshotted/decremented privately. The branch threshold is
-10000000,not the operand API's namespace boundaries. A raw word below it can
be a literal,pointer or tag and is still passed as getter/setter input. The literal
branch decrements with word wrap only while R>=-10000000;crossing that threshold
changes routing. For example,stored literal-10000000 becomes-10000001;later
GetValue treats that word as an immediate and SetValue has no storage action,
so the retained COUNT stays-10000001 while each N is-10000002. Variable initial0
executes body once,then writes-1 and repeats rather than behaving like literal
endless0. Exact N0 alone
pops. The first-body unconditional policy must not become a host for-loop gate.

## BREAK7 versus CONTINUE8 scanners

Both traverse from E.next14 and maintain signed nesting depth0. For each word,
opcode=low16,argument count=signed high16,word advance4+4*count. DO5 increments
depth. NEXT6 OR opcode1 decrements depth;the first result<0 ends scanning.
No predicate execution,body callbacks,script-length bound or missing-end check.
Opcode2 delete does not end these scans. Other opcodes do not change depth.

BREAK7 advances past the scanned command BEFORE classification. At closure,
store next14=AFTER the closing word/operands,then reread e,decrement low8 index,
handler2. It skips NEXT's count write entirely. CONTINUE8 classifies BEFORE
advancing;at closure store next14=THE closing command header,handler2. It does
NOT pop e. Subsequent fetch executes NEXT itself (or encountered opcode1);
NEXT then owns decrement/writeback/pop. Their terminal1 handling is a concrete
lexical rule,not validation that the matched word is a proper NEXT.

Acceptance cases:direct negative pointer/threshold equality,first16 signed label
lookup/duplicate/missing fallback;DO raw variable count/unconditional first body;
literal0,1,negative and variable0,mutated count between iterations;exact zero pop;
BREAK skips decrement,CONTINUE executes it,nested scanning and opcode1/2
distinction;no implicit loop yield. Stack/script/alias/numeric domains,other
opcodes/source lifecycle and original execution remain EVT-01 dependencies.
