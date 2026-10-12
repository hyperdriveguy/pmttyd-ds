# Event dispatch tail,diagnostic formatting and slot probe

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete local113..118 and missing prefix/no-op/WAIT_WHILE details,complete
 evt_debug_put_reg80032a70/73c and evtGetPtr80035a88/3c. Called GetWork/saved
readers and sprintf are fingerprinted and linked below. Verify:
`python3 tools/verify_event_dispatch_tail.py`:nine containing/complete/linked
hashes,164 instruction anchors,145 spans including all119 dispatch words and23
NUL-terminated diagnostic templates. Static original DOL evidence;no original
debug output,slot-capacity or valid numeric/source trace claim.

## Tail and previously implicit entries

| Opcode | Actual target | Local behavior |
| --- | --- | --- |
| 1 | 800329f4 | Direct common tail with initialized result2;clear/fetch |
| 3 | 80031758 | Set handler2;no argument query or other write |
| 11 | 80031960 | WAIT_WHILE:resolve operand0 each invocation;nonzero0,zero2 |
| 113 | 80032994 | Set handler2;no argument query or other write |
| 114 | 8003299c | Set handler2;no argument query or other write |
| 115 | 800329a4 | Call evt_debug_put_reg(E);use returned handler2 |
| 116 | 800329b4 | Load RAW operand0;store word E1a4;handler2 |
| 117 | 800329c8 | Set handler2;no argument query or other write |
| 118 | 800329d0 | Probe slots0..255 with evtGetPtr;then handler1 |

[Fetch/waits/common tail](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) supplies
0/2/9/10/91 and result handling. Opcode1's “terminal” role in constructor and
lexical scans is separate from its runtime table behavior:it does not delete,
yield or stop dispatcher execution. Runtime3 is a no-op although constructors
use label3 while scanning scripts.113/114/117 do not format/read/discard an
operand through any API. No original assertion or failure behavior is supplied
by these no-op bodies.116 is raw metadata assignment,not script-ID lookup,
resolved line-number conversion or an immediate print request.

11 reads E.argument18 and calls evtGetValue on its operand0 EVERY poll. Its
neg/or/arithmetic-shift/AND-complement computes result2 for exact returned0,
result0 for every nonzero word,including negatives. No marker/timer storage or
first-call gate. Result0 preserves opcode and returns dispatcher0;later eligible
invocations query again. Result2 clears/fetches in the same invocation. It is
not WAIT_FRAME's cached count or a generalized <=0 completion policy.

## Slot118 and complete evtGetPtr

Start index0;call evtGetPtr(index),compare returned pointer exactly to current E.
If equal,stop searching. Otherwise increment index;repeat while signed index<256.
Whether found or all256 fail,set handler1. No result publication,ID query,
entry deletion or metadata write. Common handler1 clears opcode and returns0.
Thus the following command waits for another dispatcher invocation;another
credit may supply that invocation in the same manager pass.

Each evtGetPtr invocation independently reads GP pointer80415fd0 and GP.word14.
Select field context803d8d30 for0,battle803d8dd0 otherwise. Read its entryArray90;
compute pointer=array+wrap32(index*0x1b0);read flags8;return pointer if active bit1
set,else0. No index/capacity/signed-domain/null/ID/type/stop/inhibition check.
118 does not consult context.capacity0 before probing up to255. The current
E's activity is not checked directly;comparison uses the helper's active-only
returned pointer. [Init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) now
proves an initial256-slot array in BOTH contexts. Capacity/array mutations and
source/scene validity remain separate;bounded lookup still differs if those
invariants cease to hold. The loop does not prove256 simultaneously active events.

## Debug115 entry,context and direct operand classification

Capture E.argument18 as A BEFORE calling evtGetWork;retain selected context C
from that call;THEN read RAW A[0] as X. GetWork uses current GP.word14;later
saved readers can read GP independently. There is NO evtGetValue/evtGetFloat
call in this complete diagnostic body. It formats one string into shared scratch
803d8430 using sprintf and returns2. No local screen draw,log emission,audio,
print callback or recursive diagnostic evaluation occurs. The formatted result
and sprintf return are not published as event outputs.

Every signed threshold is inclusive on its upper end. Address/index arithmetic
wraps32. For a family base B,index i=wrap32(X-B). q=truncSigned(i/32),r=i-32*q;
mask=PPC_slw(1,r),with the same low6-bit count/bit5-zero-result policy as the
[integer reader](EVENT_INTEGER_OPERAND_NAMESPACES.md). No index validation.

| Signed X interval | Read / diagnostic category |
| --- | --- |
| X<=-270000000 | Raw X as ADDR;no memory dereference |
| -270000000<X<=-220000000 | Decode fixed-point encoded float;FLOAT |
| -220000000<X<=-200000000 | UF at word(E158)+4*q;print raw word&mask |
| -200000000<X<=-180000000 | UW word at word(E154)+4*i;value formatting below |
| -180000000<X<=-160000000 | GSW via swByteGet(i);value formatting below |
| -160000000<X<=-140000000 | LSW via _swByteGet(i);value formatting below |
| -140000000<X<=-120000000 | GSWF via swGet(i) |
| -120000000<X<=-100000000 | LSWF via _swGet(i) |
| -100000000<X<=-80000000 | GF at C+84+4*q;print raw word&mask |
| -80000000<X<=-60000000 | LF at E+dc+4*q;print raw word&mask |
| -60000000<X<=-40000000 | GW word at C+4+4*i;value formatting below |
| -40000000<X<=-20000000 | LW word at E+9c+4*i;value formatting below |
| X>-20000000 | Raw X signed integer,unlabelled |

Direct diagnostic float includes the entire(-270M,-220M] band;GetValue's direct
operand reader passes(-270M,-250M] through unchanged. Do not share their leading
routing test. D(w)=singleMultiply(singleConvert(wrap32(w+230000000)),1/1024),
with signed conversion via double bias0x4330000080000000 then `fsubs` and
`fmuls`. The redundant greater-than-upper check inside the taken direct-float
branch is unreachable;it does not establish a second direct conversion outcome.
No+0.5,fctiwz or second operand query. Numeric/FPSCR domain remains external.

For UW/GSW/LSW/GW/LW fetched word w,choose formatting only:if w<=-270M use
hex;else if w<=-220M decode D(w) and use float;else signed integer. Float
branches preserve family/index and pass f1 with CR bit6 SET. Integer/hex/flag
branches clear bit6 for sprintf's varargs save policy. There is no normalization
of UF/GF/LF intersections to0/1:bit31 can print as negative signed decimal.
Saved swGet/_swGet return their own0/1 results;swByteGet index0 is GP174 word,
while its nonzero indices and _swByteGet are bytes,as documented in
[saved storage](EVENT_SAVED_SWITCH_STORAGE.md).

## Exact diagnostic templates and argument anomalies

Template base802bffb0;offsets and strings below are pinned with NUL terminators.
All calls use destination803d8430. Inputs listed are actual varargs semantics;
no corrected index is silently substituted. Ordinary families have integer
index i in r5 and integer value in r6,or float f1 for their `%f` branch.

| Offset(s) | Template(s) / formatting role |
| --- | --- |
| 08 | `ADDR     [%08X]`;r5=X |
| 18 | `FLOAT    [%4.2f]`;f1=D(X) |
| 2c | `UF(%3d)  [%d]`;i,raw intersection |
| 3c /4c /60 | `UW(%3d)  [%08X]` / `UW(%3d)  [%4.2f]` / `UW(%3d)  [%d]` |
| 70 /80 /94 | `GSW(%3d) [%08X]` / `GSW(%3d) [%4.2f]` / `GSW(%3d) [%d]` |
| a4 /b4 /c8 | `LSW(%3d) [%08X]` / `LSW(%3d)  [%4.2f]` / `LSW(%3d) [%d]` |
| d8 | `GSWF(%3d)[%d]`;i,saved flag |
| e8 | `LSWF(%3d)[%d]`;i,saved flag |
| f8 | `GF(%3d)  [%d]`;i,raw intersection |
| 108 | `LF(%3d)  [%d]`;i,raw intersection |
| 118 /128 /13c | `GW(%3d)  [%08X]` / `GW(%3d)  [%4.2f]` / `GW(%3d)  [%d]` |
| 14c /15c /170 | `LW(%3d)  [%08X]` / `LW(%3d)  [%4.2f]` / `LW(%3d)  [%d]` |
| 180 | `         [%d]`;r5=X |

Hex UW branch at80032bdc sets r5=w while r6 is already w:the string prints
UW(w)[hex(w)],not UW(i)[hex(w)]. Hex GSW/LSW branches at80032cb8/80032da8 set
r5=w and leave r6=C from the initial GetWork result;their leaf saved-byte getters
do not modify r6. Thus GSW(w)[hex(C)] or LSW(w)[hex(C)],not the expected index/
value pair. These paths are reachable for GSW0's full word;ordinary byte returns
cannot satisfy signed w<=-270M. Hex GW/LW branches DO use correct i,w. This
records actual register assignments,not inferred formatting intent.

[sprintf/varargs/string sink](../format/FORMAT_OUTPUT_RUNTIME.md) documents the complete
local wrapper:CR6 controls saving f1..f8;writer limit0xffffffff is not the actual
scratch capacity;return-based NUL termination can differ on error. Its grammar/
numeric helpers are separately linked there.115 supplies no local capacity/
error guard and ignores sprintf return. Shared scratch consumers,original debug
uses,exception/alias domains and platform output policy remain requirements.

Acceptance:113/114/117 no queries;116 raw assignment;11 repeated resolution,
negative/nonzero retention and exact0 continuation;118 found/absent/active/context
selection and unchecked256 bound;all diagnostic thresholds,bases,raw masks,
float rounding and direct-versus-reader band;stored hex/float/int paths,actual
UW/GSW/LSW hex varargs,CR6 and exact template bytes;no automatic diagnostic
screen output. Other source/domain/platform and original traces remain EVT-01
work. This closes local opcode behavior without proving full runtime integration.
