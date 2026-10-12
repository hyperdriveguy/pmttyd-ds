# Event multiword and float data-reader opcodes65..73

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete scoped65..73 dispatcher blocks in evtmgrCmd800316b8/13b8.
[Assignment/cursor](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) closes
62..64;[integer](EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[float](EVENT_FLOAT_OPERAND_ROUTING.md) contracts close the called setters.
Verify:`python3 tools/verify_event_data_memory_opcodes.py`. Static original DOL
evidence;no original data-source or execution trace claim.

## Actual dispatch and argument layout

E is current entry;C is word E150,data cursor. Output operands are RAW encoded
setter destinations. Table targets are read from80304fd0+4*opcode.

| Opcode | Target | Arguments / local operation |
| --- | --- | --- |
| 65 | 8003210c | destination0,1,2;sequential three integer words |
| 66 | 8003216c | destination0,1,2,3;sequential four integer words |
| 67 | 800321e8 | destination0,index1;indexed integer word |
| 68 | 8003221c | address0;set cursor through integer reader |
| 69 | 80032238 | destination0;sequential one IEEE float |
| 70 | 80032260 | destination0,1;sequential two IEEE floats |
| 71 | 800322a4 | destination0,1,2;sequential three IEEE floats |
| 72 | 80032304 | destination0,1,2,3;sequential four IEEE floats |
| 73 | 80032380 | destination0,index1;indexed signed integer→single float |

All supply handler2 and continue in the SAME dispatcher invocation. No buffer
length,data terminator,null check,alignment check or automatic allocation occurs
here. The memory is original big-endian storage. Host implementations need an
address/memory provider;encoded original addresses cannot be ordinary host
pointers merely because the outputs fit32 bits.

## Sequential integer65/66

Capture argument pointer A before the first lane. For each lane i in ascending
order,load C from LIVE E150;load RAW destination A[i];store E150=wrap32(C+4);
THEN load word at retained C and call evtSetValue(E,destination,word).
After each setter,reload E150 and load the next destination from retained A
at its next offset. Each lane completes before the next lane begins.

No prefetch of all data or destinations,no private cursor carried across setters,
no atomic multi-output batch. A setter can change the subsequent cursor and
later destination word before they are loaded. The first lane's old C remains
its read address after the cursor increment. The source word itself is NOT
passed through evtGetValue;tag-shaped data words are raw values supplied to
the setter. Output sentinel/rounding behavior remains the selected setter's.
65/66 extend the identical63/64 ordering to three/four lanes.

## Sequential float69..72

69 captures raw destination0 from current argument pointer and reads live C.
70..72 capture A before lane0 and use ascending A[i]. Each lane loads C,loads
raw destination,stores E150=C+4 BEFORE `lfs` at retained C,then calls
 evtSetFloat(E,destination,loadedFloat). `lfs` reads IEEE binary32 data into the
floating register;it is NOT an integer getter/tag decoder. No extra local
arithmetic,+0.5 or `frsp` is inserted before the setter.

Next lane reloads live E150 and its raw A[i] after the preceding setter,just
as for integer lanes. Retained argument base is not replaced by a setter's
change to E.argument18. Float setter's own destination-dependent behavior,
including rounding and storage family,still applies. Cursor stride is4 bytes
for every lane,regardless of the setter's destination family. There is no
separate float cursor field.

## Indexed integer67

Capture RAW destination A[0] BEFORE loading/resolving encoded index A[1] through
evtGetValue. AFTER the query,read live E150 as base C. Compute word offset
wrap32(index<<2),read big-endian word at C+offset,and pass it directly to
 evtSetValue with the captured destination. No local cursor advance. Index
query can alter E150 before its load;it cannot replace captured destination0.
No sign/bounds/range check or second index query. A negative index is used
through wrapped word address arithmetic;valid allocation/address domain remains
required. Setter side effects can still change E150,so “no local advance” must
not become a claim that E150 cannot change across the whole command.

## Cursor68 and indexed float73 are distinct

68 resolves operand0 with evtGetValue,then stores returned word to E150.
No evtGetFloat query or address reinterpretation is performed. This duplicates
62's cursor-setting policy in the float-reader opcode group.

73 captures raw destination0,then resolves index1 via evtGetValue. AFTER the
query read E150,calculate index<<2 and load a WORD with `lwzx`. It does NOT use
`lfs`. Sign-convert that word to a number using xoris8000 and double bias
0x4330000080000000 at802bffb0,then `fsubs` produces its SINGLE-rounded numeric
value. Call evtSetFloat with that float and captured raw destination;no+0.5,
source evtGetFloat or cursor update exists. This is indexed signed-integer
conversion,despite its position among float-reader commands.

Example:source word0x3f800000 read sequentially by69 is IEEE1.0;read via73 it
is signed integer1065353216 converted to single precision. Source0xffffffff
becomes numeric-1.0 via73,while69 loads an IEEE NaN bit pattern. These are static
instruction-derived differences,not observed original script uses. Native
implementation must not route both through one IEEE indexed-reader helper.

## Acceptance cases and open requirements

Cover each actual65..73 table target;all lane counts/integer versus float setter;
advance-before-source-read;cursor/output-word mutation between lanes;captured A
versus E.argument18 changes;raw tag-shaped data/destinations;index resolution
before live base load,captured destination before query;zero/negative/wrapped
indices;68 resolved address;73 signed integer single rounding versus69 IEEE
bits;no implicit yield. Numeric/FPSCR/NaN conversion,pointer/alias/alignment/
capacity/source domains and original data-reader traces remain EVT-01 work.
[Memory operations](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) and
[allocation](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md) cover the next opcode
families without inventing a cursor allocation or shared length check.
