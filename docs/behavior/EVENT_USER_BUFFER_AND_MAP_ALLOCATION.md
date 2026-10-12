# Event user-buffer setup and complete local map allocation

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete scoped74..76 dispatcher blocks in evtmgrCmd800316b8/13b8;complete
_mapAlloc8002ff58/f0,memset800050b4/30,__fill_mem800050e4/b8 and
DCFlushRange80296a54/30. Verify:
`python3 tools/verify_event_data_memory_opcodes.py` pins nine full/containing
hashes,269 instruction anchors and28 constants/dispatch/SDA spans.
Static original DOL evidence;no heap initialization/capacity/runtime trace claim.
Read [operand namespaces](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[data readers](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md),
[descendant inheritance](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) and
[deletion](EVENT_DELETE_AND_COPYBACK.md).

## Buffer opcodes74..76

| Opcode | Target | Ordered local action |
| --- | --- | --- |
| 74 | 800323d4 | Resolve operand0 via evtGetValue;store returned word E154 |
| 75 | 800323f0 | Resolve operand0 via evtGetValue;store returned word E158 |
| 76 | 8003240c | Resolve count0;load raw destination1;allocate count*4 bytes;store E154;publish pointer through SetValue |

74/75 add no local null check,length,allocation,copy or clearing of previous
storage. Getters/setters' later user-work/user-flag families route through these
fields;their interpretation is in the namespace contract. E150 data cursor is
a separate field and is not updated. Every local command supplies handler2 and
continues in the same dispatcher invocation.

76 captures argument pointer A and resolves A[0] via evtGetValue to count N.
AFTER that query load RAW encoded output A[1] into retained destination D.
Read heap-root pointer at global8041e808 = r13(8041cf20)+18e8;compute requested
byte count wrap32(N<<2);call _mapAlloc(root,byteCount). Store its returned word
to E154,THEN reload E154 and pass that word to evtSetValue(E,D,pointer).
Setter return ignored;E158 and E150 have no explicit local writes.

There is no signed-count validation,overflow check,allocation-failure branch or
old-buffer release. Failure0 is stored/published just like success. Even if
D is unsupported and setter does nothing,E154 is already replaced. Count
query can change the output word before its load and global root before its
load;setter side effects can further change state after pointer publication.
The output is the allocated payload address,not count/byte size/heap header.
Repeated76 overwrites the prior E154 without freeing it in this local body.
No allocation for E158 is requested by these three opcodes.

## _mapAlloc alignment and complete free-list search

Inputs are root pointer H0 and requested byte word B. Compute
P=wrap32(B+31) AND0xffffffe0. If P==0,return0 BEFORE dereferencing H0.
Otherwise compute required span T=wrap32(P+32). These are word operations;
no pre-overflow or huge-size guard. For valid normal requests,P is the payload
length rounded up to32 bytes. The allocator has no root-null check for P!=0.

Each list header H supplies word next at+0,unsigned size at+4,unsigned16 used
at+8. Headers occupy32 bytes by the construction offsets;other header bytes
are not read/written by this body. Start at H0,with bestSize0 and captured
bestNext0. At each node in list order:

1. Read used16;if nonzero,node is ineligible.
2. For unused node,read size S;unsigned S<T is ineligible.
3. Choose this node if bestSize==0 OR unsigned bestSize>S. Save node H,size S
   and its current next pointer. Equal sizes retain the earlier chosen node.
4. Read this node's next pointer and continue if nonzero;otherwise end scan.

The whole list is traversed,not terminated at the first adequate fit. This is
smallest eligible unsigned size with first-in-list ties. No type/owner/event ID
comparison,cycle detection,link validation,priority or allocation counter update.
If bestSize==0 at scan end,return0 with NO header mutation,fill or cache flush.
Captured bestNext comes from selection time;no callback occurs during scanning.

## Success split and initialization order

For chosen H with saved S and old next O,perform:

1. Store H.next=wrap32(H+T);payload address A=wrap32(H+32).
2. Compute remainder R=wrap32(S-T);store H.size=P;store H.used16=1.
3. Reread H.next as new header J;store J.next=O,J.size=R,J.used16=0,in that order.
4. memset(A,0,P);then DCFlushRange(A,P);return A.

A new remainder header is ALWAYS written,including R==0. No minimum-remainder
check,whole-block consumption alternative or removal of the zero-size remainder
node exists. Allocated header.size holds PAYLOAD P,whereas fit tests used the
free node's original S against P+32. Do not infer a uniform total-size meaning
for both states or replace the local split with an SDK heap allocator.
The old next pointer is the captured selection-time value;payload fill begins
only after both headers are modified. No rollback/error test on memset/cache
flush or separate allocation-list append.

The heap root's construction,region extent,header alignment,size invariants,
free/coalescing policy,ownership and source count limits are still dependencies.
If P+32 wraps to0,the local branch sequence still uses that value;valid normal
heap reasoning above does not assert correct/safe behavior for that domain.
A source/host domain assertion must be separately evidenced and identified.

## Called memset/__fill_mem complete local policy

memset retains its original destination pointer,forwards original destination,
fill value and length to __fill_mem,then returns that ORIGINAL pointer. It
ignores the fill helper's unspecified scalar return. __fill_mem truncates fill
to low8;sets working byte pointer to destination-1;tests unsigned length<32.
For short lengths,it writes bytes ascending using update stores;length0 writes
nothing. For lengths>=32:

- Compute initial byte count (-destination)&3;write that many leading bytes
  ascending and subtract from retained length,to reach4-byte alignment.
- If fill byte!=0,replicate it into all four word bytes;zero stays word0.
- Write length>>5 groups of eight ascending word stores (32 bytes per group).
- Write (length>>2)&7 remaining words ascending.
- Write length&3 trailing bytes ascending.

No source read,buffer bound,cache operation or failure result exists in the fill
helper. Allocator passes fill0 and aligned P,so normal aligned header/payload
requests take the burst word path without nonzero replication. Output extent
is exactly P logical bytes under valid pointers/lengths;volatile/fault/address
wrap semantics remain external domains. Original header alignment is not proved
merely by seeing this optimized path.

## Called DCFlushRange complete local policy

Length is unsigned word. If length==0,return without any cache instruction.
Otherwise compute count=wrap32(length+(address&31)+31)>>5;load CTR=count.
Execute `dcbf` for address,advance address by32,repeat through `bdnz` until CTR
becomes0. Cache block selection is hardware behavior;the operand addresses
retain their low alignment bits. After the loop execute `sc`,then return.
No local sync instruction,length-validation gate or heap-state change.
For a valid ordinary positive extent,this flushes every32-byte cache block
intersecting it. Wrapped arithmetic can produce count0 despite nonzero length;
the body is entered unconditionally and the CTR loop then wraps. No safe
zero-count fallback exists. System-call handler/cache coherence/platform effects
are external service requirements,not established by this complete local leaf.

## Ownership and acceptance requirements

The local event destructor clears entry memory and decrements the global event
allocation count;it performs no map-buffer free. Constructors can copy/shared
user pointers according to their separate contracts. Do not infer per-entry
heap ownership,automatic freeing on cancellation or exclusive ownership from
E154 alone. Other caller/free/scene lifecycle evidence remains necessary.

Cover74/75 resolved pointer assignment,76 delayed output/root loads,count shift
wrap,zero/failure publication and replacement without free;allocator rounded0
without root read,best-fit and equal ties,all-used/no-fit list,mandatory split
including zero remainder,ordered headers/zero/cache/pointer return;fill length0,
small/aligned/unaligned,burst/remainder/nonzero low8;cache zero/unaligned extent
and system-call boundary. Native address translation,heap initialization/free/
capacity/alias/cycle/numeric domains and original user-buffer/cancellation traces
remain EVT-01 work. These local contracts do not establish complete VM ownership
or a finished full-game memory system.
