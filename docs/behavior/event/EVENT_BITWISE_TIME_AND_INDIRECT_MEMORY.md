# Event bitwise,time conversion and indirect memory opcodes77..90

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete scoped77..90 dispatcher blocks in evtmgrCmd800316b8/13b8.
Read [integer operands](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[float operands](EVENT_FLOAT_OPERAND_ROUTING.md),
[data readers](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) and
[user buffers](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md).
Verify:`python3 tools/verify_event_data_memory_opcodes.py`. Static original
DOL evidence;no native-host pointer equivalence or execution-trace claim.

## Bitwise77..80:resolved RHS versus literal bits

| Opcode | Target | Local operation |
| --- | --- | --- |
| 77 | 80032468 | destination value AND resolved operand1 |
| 78 | 800324a4 | destination value AND RAW operand1 |
| 79 | 800324d8 | destination value OR resolved operand1 |
| 80 | 80032514 | destination value OR RAW operand1 |

77/79 capture RAW encoded destination0,load operand1 and call evtGetValue to R.
Then call evtGetValue(E,capturedDestination) to D. Compute D AND R or D OR R,
then evtSetValue(E,capturedDestination,result). RHS query precedes destination
query. Both destination queries use the same encoded operand captured before
either query;no argument pointer reload changes it. The bitwise result is the
full32-bit pattern;setter owns destination routing/coercion.

78/80 capture RAW destination0 AND RAW operand1 into retained registers BEFORE
calling evtGetValue for destination. Combine its returned D with the retained
literal bits,then call evtSetValue. No RHS query. A destination query that
changes the argument words cannot change either retained encoded destination or
raw RHS. This is a different capture policy from a raw operand loaded after
another resolver. No shift/XOR/NOT/all-bits boolean test is added by these bodies.
All return2 and continue in the same dispatcher invocation.

## Time conversions81/82:wrapped product before signed division

| Opcode | Target | Formula within original word arithmetic |
| --- | --- | --- |
| 81 | 80032548 | truncTowardZero(S32(wrap32(resolvedOperand1*60))/1000) |
| 82 | 80032588 | truncTowardZero(S32(wrap32(resolvedOperand1*1000))/60) |

Capture RAW destination0 BEFORE resolving operand1 through evtGetValue. The
product is `mulli` low32;do NOT multiply in a wider host type and divide before
narrowing.81 then uses signed high-word multiply by0x10624dd3,arithmetic shift6
and sign-bit correction.82 uses signed high-word multiply by0x88888889,adds
the wrapped product,arithmetic shift5 and sign-bit correction. These instruction
sequences implement signed division with truncation toward0 by1000/60 after
product wrap. Then evtSetValue(E,capturedDestination,quotient),handler2.

Examples:81 input±17 gives±1;input±16 gives0.82 input±1 gives±16;input±3
gives±50. Larger products can wrap and reverse sign before division. No float
rounding,+0.5,saturation,clock query,elapsed-time update or actual wait occurs.
The constants express a60/1000 conversion in these commands;they do not prove
rendering rate,pad sample rate or original manager clock frequency. Preserve
simulation-tick ownership and separate original source/cadence requirements.

## Direct original-address memory83..86

| Opcode | Target | Argument roles / ordered action |
| --- | --- | --- |
| 83 | 800325cc | resolve value0 via GetValue;THEN load RAW address1;store word there |
| 84 | 800325e8 | resolve value0 via GetFloat;THEN load RAW address1;store `stfs` there |
| 85 | 80032604 | load RAW address1 and RAW encoded destination0;load word(address1);SetValue(destination,word) |
| 86 | 80032620 | load RAW address1 and RAW encoded destination0;load `lfs`(address1);SetFloat(destination,float) |

83/84 retain argument base A before resolving value0. Address1 is loaded from
A[1] AFTER the query,so query writes can change its word. There is no address1
getter.83 stores the resolved32-bit value directly,not via a namespace setter.
84 writes IEEE single precision using `stfs`;its memory rounding belongs to
that instruction. Output/input word roles are value0,address1,not address0,
value1. Neither validates null,alignment,ownership or writable extent.

85/86 perform NO source/destination getters. Address1's raw word is used as
the load address;destination0 is a raw setter operand.85 source word passes
straight to SetValue without tag resolution;86 source bits use `lfs`,then
SetFloat. Setter return is ignored. No extra pointer dereference/tag decoder
exists. Memory itself is original big-endian;host address translation and
IEEE/signed-word conversion must be explicit. All supply inherited handler2.

## Resolved destination and double-resolved source87..90

These route through operand APIs rather than the raw-address load/store group:

| Opcode | Target | Exact ordered queries / setter |
| --- | --- | --- |
| 87 | 8003263c | D=GetValue(A[0]);THEN V=GetValue(A[1]);SetValue(E,D,V) |
| 88 | 80032670 | D=GetValue(A[0]);THEN F=GetFloat(A[1]);SetFloat(E,D,F) |
| 89 | 800326a0 | Q=GetValue(A[0]);V=GetValue(E,Q);THEN load RAW D=A[1];SetValue(E,D,V) |
| 90 | 800326d0 | Q=GetValue(A[0]);F=GetFloat(E,Q);THEN load RAW D=A[1];SetFloat(E,D,F) |

Capture A before first query.87/88 retain the resolved destination D while
loading A[1] AFTER destination query. The setter interprets D as its encoded
destination,not as an automatically dereferenced pointer.89/90 retain A and
pass the first query's RESULT WORD Q as the second getter's encoded operand.
Only AFTER the second query do they load raw destination A[1]. A query can
change that output operand before its load. Neither pair reloads E.argument18
as a new base. All supply inherited handler2;no automatic yield.

A returned Q can identify a variable,pointer-encoded read family or literal,
according to the exact linked reader bands. Double query does NOT universally
mean a memory pointer dereference. For example,first query returning immediate5
makes89 query5 again and obtain5;the integer pointer family only dereferences
when its explicit signed band is selected. Setter D in87 can likewise be a
namespace token,an unsupported immediate or a pointer-family token;the local
opcode adds no validity gate or special output sentinel. Do not collapse
83/85/87/89 to one host pointer helper.

## Acceptance cases and open requirements

Cover all14 actual77..90 targets;RHS-before-destination queries,raw RHS captured
before destination getter;all-bit signed patterns;positive/negative/zero time
inputs and multiply wrap before truncation;raw addresses and query-mutated
address1;word versus IEEE float load/store;87/88 destination query before RHS;
89/90 second query on Q and delayed raw destination load;tag/literal/pointer
routing and retained A;default2 even without explicit final li2.

Other opcodes,host memory translation,source pointer/capacity/alias/numeric/FPSCR
and clock domains,original script uses and executed memory/time traces remain
EVT-01 requirements. These complete local bodies supply implementation behavior
without proving source-domain safety or finished Jump/story runtime integration.
