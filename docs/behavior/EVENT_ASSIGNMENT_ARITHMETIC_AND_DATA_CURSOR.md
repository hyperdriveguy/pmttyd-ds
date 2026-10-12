# Event assignment/arithmetic50..61 and data cursor62..64

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Scoped whole dispatcher blocks50..64 in evtmgrCmd800316b8/13b8.
Read [integer APIs](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[float APIs](EVENT_FLOAT_OPERAND_ROUTING.md) and
[dispatch continuations](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify: `python3 tools/verify_event_loop_and_arithmetic.py`.
Complete used blocks,not every opcode/numerical domain or runtime trace.

## Assignment and integer arithmetic

Operand0 is always a RAW destination D,never pre-resolved as a write target.
Setters' scalar returns are ignored. All these blocks finish handler2,clearing
opcode and fetching another command in the same dispatcher invocation.

| Opcode / target | Exact operations |
| --- | --- |
| 50 /80031d8c | Capture D;evtGetValue(operand1);evtSetValue(D,result) |
| 51 /80031db8 | Read raw D and raw operand1,evtSetValue(D,raw1);NO getter |
| 52 /80031dd4 | Capture D;evtGetFloat(operand1);evtSetFloat(D,result) |
| 53 /80031dfc | Capture D;resolve RHS operand1 integer FIRST,then resolve D integer;wrapped sum;evtSetValue |
| 54 /80031e38 | Same reads;word subtraction lhs-RHS;evtSetValue |
| 55 /80031e74 | Same reads;low32(lhs*RHS) via mullw;evtSetValue |
| 56 /80031eb0 | Same reads;divw(lhs,RHS),signed truncation toward0 for valid divisor;evtSetValue |
| 57 /80031eec | Special MOD conversion/remainder below |

For53..56,one captured D is reused both as getter operand for lhs and setter
raw destination. RHS resolution can mutate lhs/source globals before its read.
Do not snapshot operands together or resolve lhs first. Integer operations
use PPC word arithmetic,no saturation or bounded-provider fallback. Zero divisor
and INT_MIN/-1 are not checked;their original divw exception/result domain needs
an explicit instruction contract rather than a guessed host result.

## MOD57's full conversion order

Capture D. Resolve RHS through evtGetValue. Convert that signed32 with biased
binary64 subtraction using fsubs (single rounding),then fadds binary320.5,
fctiwz to signed word b. Only THEN resolve lhs through evtGetValue(D),perform
the SAME signed-to-single,+0.5,fctiwz sequence to word a. Compute
q=divw(a,b),p=low32(q*b),r=wrap32(a-p);evtSetValue(D,r),handler2.
The constant0.5 has bits3f000000 at8041fa28;bias4330000080000000 at802bffb0.
This is not plain signed integer remainder on the original operands.

Derived example:lhs-3,RHS2 become a=-2,b=2,and result0. Positive small integers
usually remain themselves after+0.5 truncation;large values can change due to
single rounding. Negative RHS-1 becomes0,creating an unchecked division-domain
case. Conversion and divw/FPSCR extremes are not normalized locally. These are
static arithmetic predictions,not original execution observations.

## Floating updates58..61

Each captures D;evtGetFloat(RHS) FIRST,then evtGetFloat(D). Perform exactly
one single-rounded operation below,then evtSetFloat(D,result),handler2:

| Opcode / target | Operation |
| --- | --- |
| 58 /80031f94 | fadds(lhs,RHS) |
| 59 /80031fd0 | fsubs(lhs,RHS) |
| 60 /8003200c | fmuls(RHS,lhs),in that operand order |
| 61 /80032048 | fdivs(lhs,RHS) |

No host-double intermediate,no fused substitution,no float remainder opcode
exists in this slice. The float setter then performs its OWN1024 encoding/old-
value reads according to destination family. Finite/nonfinite/division rounding
and namespace domains follow the linked APIs and original instruction obligations.
The caller does not guard an unsupported float destination or clamp results.

## Data pointer setup62 and integer reads63/64

| Opcode / target | Whole request sequence |
| --- | --- |
| 62 /80032084 | Resolve operand0 via evtGetValue;store result to E.word150;handler2 |
| 63 /800320a0 | Read E150 cursor;capture raw destination0;store E150=cursor+4 BEFORE reading word(cursor);evtSetValue(destination,raw memory word);handler2 |
| 64 /800320c8 | Capture argument pointer;perform the63 sequence for raw destination0;THEN reload E150 and raw destination1 from captured argument pointer+4 and repeat;handler2 |

Data words are copied RAW through evtSetValue,not recursively resolved as tags
or read via evtGetFloat. There is no cursor null/bounds/label/script validation.
The second lane reloads cursor AFTER the first setter callback,so it need not
be the next word under mutating/alias adapters. It does not prefetch both words.
Both advances happen before their memory read/output callback. Aliased output
locals end with second value;source and output aliases retain original order.

Word150 is reset in root/descendant construction,explicitly preserved by restart;
plain brother110 copies source150 AFTER construction,ID brother111 does not,
and independent run/inline roots retain0 unless another source writes it. These
previously documented differences directly affect data-reader ownership.
Further multiword/float cursor opcodes65 onward remain separate work.

Acceptance cases:SET resolved versus raw tag storage;RHS-before-lhs side effects;
wrapped ADD/SUB/MUL,signed DIV,MOD negative/conversion cases;float rounding plus
setter quantization;raw data tags,advance-before-read,second cursor reload/output
alias and constructor/restart150 differences. Original numeric/alias/storage/
source/script domain and execution traces remain open;these exact blocks do not
establish a complete executed Jump or story interpreter.
