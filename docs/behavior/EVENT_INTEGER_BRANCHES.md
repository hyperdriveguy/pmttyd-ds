# Integer IF and pointer-switch behavior used by Hammer

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete evt_if_equal
0x80034d18,size0xcc;evt_if_not_equal0x80034c4c,size0xcc;evt_case_equal
0x80034640,size0x15c;selected inline evtmgrCmd regions. Scoped to the original
[Hammer weapon setup branches](BATTLE_HAMMER_WEAPON_BRANCHES.md). No executable
interpreter implementation or original execution claim is made.

## Dispatcher bindings and integer IF

Original dispatch table0x80304fd0 contains these used targets:

| Opcode | Target | Local behavior |
| --- | --- | --- |
| 24 | 0x80031a4c | Call evt_if_equal |
| 25 | 0x80031a5c | Call evt_if_not_equal |
| 32 | 0x80031acc | ELSE skip to matching END_IF |
| 33 | 0x80031b38 | END_IF completion |
| 34 | 0x80031b40 | Resolved SWITCH |
| 36 | 0x80031bc0 | Call evt_case_equal |
| 42 | 0x80031c20 | Default case |
| 49 | 0x80031d64 | END_SWITCH pop |

IF helpers resolve operand0 then operand1 with evtGetValue,and compare32-bit
integers. Equal helper continues when equal;not-equal continues when unequal.
Both return handler2. False predicates scan from entry+0x14 (next-command
pointer),starting nesting counter0. Each scanned word advances by4 plus4 times
its **signed high16 count**;opcode is low16. For ordinary valid streams this
matches their command boundaries.

Scanning opcode12..31 increments nesting. Opcode1 or33 decrements it;stop when
negative. Opcode32 stops only when nesting0. Other opcodes continue scanning.
Store the advanced pointer **after** the selected ELSE/END_IF/end marker at
entry+0x14. The scanner is unbounded and does not validate the stream/span or
operand count. Do not use this as a malformed-input safety parser. Opcode1's
special scan treatment is a recovered fact,not ordinary story flow equivalence
with explicit event-delete opcode2.

ELSE's inline scan uses the same nested-IF/end handling but ignores ELSE markers;
it exits on the END_IF/end marker that makes nesting negative. END_IF itself
simply supplies handler2. Their execution uses the common handler protocol in
[original baseline](../ORIGINAL_BEHAVIOR.md),which distinguishes handler2 from
a scheduler yield. A portable VM must preserve branch skipping inside the same
eligible invocation instead of adding an artificial tick per branch marker.

## Resolved switch state

SWITCH resolves operand0 first. Increment the byte depth at entry+0x0f,then
sign-extend the new byte for indexing. Store the resolved value at
entry+0x130+4*depth and signed-byte state1 at+0x128+depth. No local nesting bound
is checked. This is a snapshot value:subsequent changes to the source event
variable do not change that stored switch value.

CASE_EQUAL captures/sign-extends depth,then resolves its comparison operand,
loads switch state and stored value. This resolution occurs even if a prior case
has matched. When signed state<=0,scan to the end of the switch without executing
this case. When state>0 and the comparison matches,store state0 and return2;
the next body commands execute. The next case sees state0 and skips the remainder.
Thus adjacent cases here do **not** fall through as C switch bodies can.

When state>0 but unequal,scan to the next case/default marker at this switch's
nesting depth,or its END_SWITCH. Store the pointer **to the marker word**,
not after it,so that marker is actually dispatched. The scan's nesting starts1:
opcode1 or34 increments;49 decrements;35 is explicitly skipped without
incrementing. At nesting1,opcodes36..47 stop the case search;48 does not.
These are literal original scanner conditions;do not silently generalize them
to all possible switch variants. The supported Hammer blocks use34/36/42/49
and contain no nested switch variant needing that distinction.

The state<=0 end scan likewise starts1,increments on opcode1 or34,decrements
on49 and stops when0. It stores the END_SWITCH marker pointer,allowing normal
pop execution. There are no local stream bounds/failure returns in either scan.

## Default and pop

Default42 reads signed current switch state. If state>0,set it0,return2 and
execute the default body. If state<=0,run the same end-switch scan and place
entry+0x14 at the END_SWITCH marker. An already matched earlier case therefore
cannot also run Hammer's period40 default.

END_SWITCH clears state at the current sign-extended depth,then decrements the
stored depth byte and supplies handler2. It does not erase the stored switch
value word. Byte wrap/nesting limits and complete variable/inline-event contexts
need separate recovery before claiming a general VM replacement.

These rules establish the specific original pointer dispatch and mask2 integer
branches in Hammer. Other conditional/loop/case variants,jump labels,inline
ownership/pause and malformed streams remain outside this local contract.
Original scene/branch execution comparisons are still required.
