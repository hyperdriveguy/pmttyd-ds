# Complete event integer operand namespaces and return semantics

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtGetValue80031260/458,evtSetValue80030cfc/400 and evtGetWork80037a60/20.
Read [float APIs](EVENT_FLOAT_OPERAND_ROUTING.md),
[saved storage](EVENT_SAVED_SWITCH_STORAGE.md) and
[dispatcher](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify: `python3 tools/verify_event_operand_namespaces.py`.
Static local contracts;on-disk persistence/original traces remain open.

## Signed routing and storage map

E denotes event entry;C=evtGetWork(),selected by CURRENT GP.word14 (field0,
battle nonzero). BOTH functions call evtGetWork before testing any operand,
even immediate/no-store cases. All operand/tag comparisons are SIGNED32.
Address/index arithmetic wraps32;the bands are routing intervals,not array bounds.
For each named band,index i=wrap32(operand-base). No index/pointer validation.

| GetValue routing interval | Base / conventional name | Read |
| --- | --- | --- |
| operand<=-270000000 | Immediate | Return raw operand |
| -270000000<operand<=-250000000 | Immediate/sentinel band | Return raw operand |
| -250000000<operand<=-220000000 | Encoded float | Decode then fctiwz |
| -220000000<operand<=-200000000 | -210000000 / UF | Bit at word(E+158)+4*q |
| -200000000<operand<=-180000000 | -190000000 / UW | Word at word(E+154)+4*i;conditional float coercion below |
| -180000000<operand<=-160000000 | -170000000 / GSW | swByteGet(i),return unchanged |
| -160000000<operand<=-140000000 | -150000000 / LSW | _swByteGet(i),return unchanged |
| -140000000<operand<=-120000000 | -130000000 / GSF | swGet(i),return unchanged |
| -120000000<operand<=-100000000 | -110000000 / LSF | _swGet(i),return unchanged |
| -100000000<operand<=-80000000 | -90000000 / GF | Bit at C+84+4*q |
| -80000000<operand<=-60000000 | -70000000 / LF | Bit at E+dc+4*q |
| -60000000<operand<=-40000000 | -50000000 / GW | Word at C+4+4*i;conditional float coercion |
| -40000000<operand<=-20000000 | -30000000 / LW | Word at E+9c+4*i;conditional float coercion |
| operand>-20000000 | Immediate/pointer | Return raw operand |

GF/GW belong to the current EVENT CONTEXT,not BattleWork directly or GP save
arrays. UF/UW follow retained user-storage pointers. LF/LW belong to this entry.
A pointer-looking positive value passes through;there is no dereference-by-address
rule in this reader. Encoded operands must not be interpreted as IEEE float bits.

## Full integer reader coercion

Define C32(w):convert signed32 to binary64 using bias4330000080000000,then
single-rounded fsubs yields binary32. Define D(w)=single(C32(w+230000000)*1/1024),
with the initial word addition wrapped32. Direct encoded float branch returns
fctiwz(D(operand)). The redundant greater-than--220 check in its taken branch
does not create another reachable outcome.

For fetched UW/GW/LW word w,return RAW w if w<=-270000000 OR w>-220000000.
Only -270000000<w<=-220000000 is decoded via D(w),then fctiwz. This includes
stored-250000000,even though direct operand-250000000 passes through above.
Saved GSW/LSW words/bytes are returned raw and never decoded by GetValue.
Flags normalize to integer0/1. Numerical conversion outside fctiwz's valid
finite domain requires original instruction/FPSCR policy;no local clamp.

For UF/GF/LF bit addressing: q=truncSigned(i/32),r=i-32*q,then mask=PPC_slw(1,r).
PPC_slw uses low6 shift-count bits:bit5 set yields0,otherwise1<<(count&31).
This preserves original signed division/remainder sequence,not Python floor
quotient or modulo-positive normalization. Valid nonnegative indices reduce to
q=i>>5,mask=1<<(i&31). Negative/out-of-domain indices are not rejected locally.
Read word&mask and normalize nonzero to1.

## Complete evtSetValue(E,destination,value)

Routing differs at the leading bands:destination<=-270000000 returns RAW value
without storage. Otherwise destination<=-220000000 does not store;return
fctiwz(D(value)) when signed value<=-220000000,else fctiwz(C32(value)). There is
NO lower--270 bound on this return-only decoding. A literal/float-tag destination
therefore does not mean a memory write. Destination>-20000000 also returns raw
value without storage. Intermediate UF/UW/GSW/LSW/GSF/LSF/GF/LF/GW/LW bands
match the remaining routing table,with these exact actions/returns:

| Destination family | Action / returned integer |
| --- | --- |
| UF/GF/LF | Read word and set mask if value!=0,clear mask if value==0;return supplied RAW value (not prior bool or normalized1) |
| UW/GW/LW | Read old word FIRST,store supplied raw word;return old word unchanged (no float decode) |
| GSW | swByteGet(index) old;swByteSet(index,raw value);return old |
| LSW | _swByteGet old;_swByteSet(index,U8(value));return old |
| GSF | swGet old;swSet for value!=0,swClear for0;return old |
| LSF | _swGet old;_swSet for value!=0,_swClear for0;return old |

Word writes preserve encoded input words rather than resolving them again.
Saved helper calls can reload GP independently of the initially captured C.
No generic ignored-output sentinel is recognized by evtSetValue;individual
wrappers explicitly skip-250000000 where their own contracts require it.
Outputs supplied as raw destinations must not pass through evtGetValue first.

Acceptance cases:every signed boundary/base±1;direct versus stored float sentinel;
raw pointer/pass-through;context capture even immediate;UW/GW/LW old-word return,
flag supplied-value return,saved old-value return;negative bit quotient/remainder;
shared user storage versus copied event locals;no implicit output sentinel.
Complete float routing and saved storage leaves are linked separately. Source/
namespace allocation,capacity/alias domains,save lifecycle and original traces
remain open under EVT-01.
