# Complete event float and string IF predicates

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete six evt_iff bodies,six evt_if_str bodies and strcmp8026d464/128.
[Integer/mask/continuations](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) gives
all actual12..33 opcode bindings and shared false/ELSE scans.
[Float reader](EVENT_FLOAT_OPERAND_ROUTING.md) and
[integer reader](EVENT_INTEGER_OPERAND_NAMESPACES.md) define operand routing.
`python3 tools/verify_event_if_predicates.py` pins all full bodies,comparison/
query/scan anchors,dispatch table,null replacement and r2 startup definition.
Static original DOL evidence;no original script execution or numeric-domain claim.

## Float18..23 query and comparison order

Capture E.argument18 as A. Load A[0],call evtGetFloat(E,A[0]),retain returned L
in f31. AFTER that query,load A[1] from retained A and call evtGetFloat to R.
No local `frsp`/arithmetic,+0.5 conversion or integer reinterpretation precedes
the comparison. The reader's returned precision/value is used directly;do not
force an extra host float cast merely because these are float predicates.
Both queries occur before any predicate decision. The first may alter the
second operand word before its load;E.argument18 changes do not redirect A.
The second query cannot replace the already retained L. On acceptance next14
stays unchanged;on rejection scan starts at live next14 after both queries.
All return2 and continue in the same dispatcher invocation.

The exact CR0 tests differ from ordinary host relational operators:

| Opcode | Instruction / acceptance test | Ordered-number result | Unordered (either operand NaN) |
| --- | --- | --- | --- |
| 18 equal | fcmpu;EQ bit set | L==R | Reject |
| 19 not equal | fcmpu;EQ bit clear | L!=R | Accept |
| 20 small | fcmpo;cror EQ=GT OR EQ;resulting EQ clear | L<R | Accept |
| 21 large | fcmpo;cror EQ=LT OR EQ;resulting EQ clear | L>R | Accept |
| 22 small equal | fcmpo;GT bit clear | L<=R | Accept |
| 23 large equal | fcmpo;LT bit clear | L>=R | Accept |

Unordered has LT/GT/EQ clear and UN set.20/21 therefore accept it because their
branches test the negation of the opposite-or-equal relation.22/23 accept it
because they negate a strict opposite bit. Implementing20 as host `L<R` or22
as host `L<=R` loses this behavior for NaN. For ordered numbers,including
infinities,ordinary relations apply. +0 and-0 compare equal:18/22/23 accept,
19/20/21 reject. No epsilon or approximate-equality policy exists.

`fcmpu` versus `fcmpo` is also an instruction distinction:ordered comparison
can affect floating exception status for NaNs,while unordered comparison has
different quiet/signaling-NaN exception policy. No local FPSCR read,clear,trap
handler or exception-mode setup exists. The CR acceptance table specifies
control flow when execution proceeds through the comparison;full FPSCR/NaN
payload/exception-enabled behavior and valid operand domains remain separate
numeric/runtime requirements. Do not claim an original NaN gameplay trace.

## String12..17 pointer resolution and null replacement

Capture A;resolve A[0] via evtGetValue to pointer word L. AFTER that query,
load A[1] from retained A and resolve it via evtGetValue to pointer word R.
Both queries occur;first can modify A[1],second cannot replace retained L.
No string resource/message-ID lookup,formatting,encoding conversion or length
query is performed here. Resolved word values are used as addresses.

AFTER both queries,replace each exact zero pointer independently with8041fa2c.
This is r2(-31252),with r2=80427440 established by startup80005418/1c;the pinned
byte at8041fa2c is NUL0. Nonzero invalid/tag-shaped pointers have no local
validation or replacement. Then call strcmp(L,R),compare its returned signed
word to0,and accept12:eq0;13:!=0;14:<0;15:>0;16:<=0;17:>=0.
Rejected uses the common false scanner from live next14 AFTER strcmp;
accepted leaves next unchanged. Every path returns2.

Hence null/null and null/empty-string compare equal;null/nonempty compares as
empty. Equality compares content,not addresses. A namespace operand must first
be resolved;an integer that remains nonzero is still passed as a pointer,not
converted to decimal characters. Comparison is bytewise/case-sensitive and
locale-independent under the complete strcmp body below.

## strcmp complete read paths and return values

Load unsigned byte L[0],THEN unsigned byte R[0];subtract Lbyte-Rbyte. If the
result!=0,return the exact signed difference in[-255,255]. Equal bytes then
select path using each pointer's low2 alignment bits:

- DIFFERENT alignments:go to byte-loop gate with the already read Lbyte.
  If Lbyte==0,return0;otherwise increment/read left1 then right1,subtract and
  return difference on mismatch. Equal zero returns0;equal nonzero repeats.
- SAME NONZERO alignment k:if current Lbyte==0,return0. Read/compare up to
  3-k additional bytes sequentially with the same difference/zero exits.
  Then increment BOTH pointers by1 to their next aligned word address.
- SAME ZERO alignment:enter the word path directly after the initial byte
  equality test,including if that first byte was0.

Word path loads left32 THEN right32 at their current aligned addresses. Compute
T=(leftWord-0x01010101) AND0x80808080 using word wrap. If T!=0,fall back to byte
comparison from the start of that word (not an offset found from T). Load left
byte then right byte,subtract and return exact difference on mismatch;equal
zero returns0;equal nonzero uses the same continuing byte loop above.

If T==0,unsigned-compare the two big-endian words. Equal words advance/read
both pointers by4 and repeat the word test. Unequal words return-1 for left<right,
+1 for left>right;they do NOT locate the differing byte to compute its exact
byte subtraction. The helper therefore guarantees comparison sign for valid
strings;its exact nonzero magnitude depends on alignment and read path.
All returns are within[-255,255],with word mismatches narrowed to±1.

The T expression intentionally has no AND NOT(leftWord) term. A zero or a
high-bit byte can send this path to byte comparison;do not replace the actual
expression when documenting memory read order. Word loads read four bytes
before checking the terminator;the same-zero-alignment path may read beyond
NUL even at the first character. It can also read rightWord before discovering
leftWord's zero. The caller's null replacement does not imply bounded byte-
only reads or invalid-memory recovery. Readable memory/string termination and
host address translation are necessary domains;the exact extent of original
source allocations remains open. No case folding,Unicode collation or locale
state is read. Bytes>=0x80 are compared unsigned in the byte path.

## Acceptance cases and boundaries

Float:ordered equality/strict/inclusive,±0,infinities,NaN on either side,
operand precision/query mutation and exact CR negation. String:distinct pointers
with equal content,first/middle byte differences,case and high-bit bytes,prefix
termination,null/null,null/empty/null/nonempty,retained pointer words,mutated
second encoded word,all alignment pairs,word mismatch±1 versus byte difference,
read-through-NUL extent and shared false scan consuming ELSE/END.

Original source/pointer/NaN domains,exception status ownership,remaining opcode
bodies,host translation and actual branch/string/pause traces remain EVT-01
work. This is local original behavior,not permission to replace plot dialogue
or text resources;the original US story fidelity requirement still applies.
