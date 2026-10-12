# SDK heap bootstrap live reads and primary-map publication order

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
This answers EVT-01's explicit read-order addendum request. Fresh read-only
listings of complete OSInitAlloc80296634/70,OSCreateHeap802966a4/6c,
memClear80030154/c4 and memInit80030218/2a4 are under /tmp/id2-heaporder-*.
Read [complete pool/bootstrap policy](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md).
`python3 tools/verify_event_heap_release_and_bootstrap.py` pins whole identities
and the instruction regions below. Static evidence only;no original heap trace.

## OSInitAlloc: retained metadata product,live iteration bounds and bases

Arguments are start S,end E and signed count N. At80296634 calculate
M=wrap32(N*12) into retained r7 BEFORE publishing globals. Store base=S to
8041f070 at80296638,then count=N to8041f074 at8029663c. Initialize signed
index i=0 and byte offset d=0. Before EVERY iteration,including the initial
zero-iteration decision,read CURRENT global count at80296670 and compare
signed i<count at80296674. Nonpositive current count exits.

When admitted,read CURRENT global base at80296654,increment i,calculate
D=wrap32(base+d),store word D+0=-1. Increment d by12,then store word D+8=0
(allocated head),then D+4=0 (free head). Return to the live count read. The
selected D is retained for all three stores of that iteration. Do not recapture
base between those stores or swap allocated/free order because another SDK
operation publishes them differently. No count/base/pointer/overflow guard.

After exit,read CURRENT base AGAIN at8029667c. Align supplied E down32 and
publish arena end8041f07c at80296688. Add retained M to reloaded base at8029668c,
add31,store current heap8041e428=-1 at80296694,align the sum down32 and publish
usable start8041f078 at8029669c. Return that aligned word. Supplied end is
retained;metadata product is NOT recalculated from the final/current count.

Thus live descriptor count controls how many records are initialized,while the
ORIGINAL N controls metadata reservation. A final changed base also affects the
returned usable address. A host implementation using only original S/N for both
loop and result differs under observable global/descriptor alias mutation.
This body calls no external callback;provider observations and actual overlapping
memory can expose the order. Such overlap is not claimed reachable in ordinary
source/bootstrap state,which remains an independent valid-domain requirement.

Acceptance:initial count0/negative skips stores but still publishes alignment;
count changes between tests alter iterations without changing M;base changes
between iterations move later D;one retained D for its three writes;final base
reload affects result;D0→D8→D4 order;original count product captured before
base/count publication;end,current-heap,usable-start publication order.

## OSCreateHeap: capture scan domain,reload descriptor size after header writes

Unlike InitAlloc,CreateHeap reads descriptor count ONCE at802966a4 and base
ONCE at802966ac. It uses that captured count for CTR traversal and captured base
for12-byte descriptor steps. Start rounds up32,end rounds down32. Signed
count<=0 returns-1;scan ascending for first signed descriptor.size<0.

On admission retain descriptor address D and rounded start H. Compute
L=wrap32(alignedEnd-H),store D.size=L at802966d8. Store H.previous=0 at802966e0,
H.next=0 at802966e4. Then RELOAD word D.size at802966e8 and store THAT word to
H.size at802966ec. Finally store D.freeHead=H at802966f0 and D.allocatedHead=0
at802966f4;return descriptor index. It does not retain L for the header size.
There is no minimum extent/range/null/descriptor-header overlap check.

The reread matters if H.previous/H.next writes alias descriptor storage or a
provider changes D.size after those writes. For example,if H==D,the previous0
store replaces descriptor.size;the later H.size store uses reloaded0,not the
initial L. This is an address-alias acceptance case,not evidence normal arena
bootstrap allocates overlapping descriptors. A native domain restriction may
reject that state explicitly;it must not label retained-size substitution as
original instruction behavior. Do not apply InitAlloc's live count/base policy
to CreateHeap:their capture boundaries differ.

Acceptance:captured count/base despite later provider changes;first-free signed
size;negative extent remains possible;size publication→previous0→next0→size
reload→header size→freeHead→allocatedHead order;header/descriptor alias and
provider changed size;original index return and exhausted-1 policy.

## Primary-map paths: all three source loads precede length publication

Both index1 construction paths load table words in this order:

| Operation | memClear(1) instruction | memInit index1 instruction |
| --- | --- | --- |
| Read pool1 start8041e834 | 800301a0 | 80030434 |
| Read pool1 end8041e820 | 800301a8 | 80030438 |
| Read pool1 handle8041e848 | 800301ac | 8003043c |
| Compute end-start | 800301b0 | 80030440 |
| Subtract SDK header32 | 800301b4 | 80030444 |
| Store requested length8041e818 | 800301b8 | 80030448 |
| Call OSAllocFromHeap(captured handle,M) | 800301c0 | 80030450 |

The start/end/handle are captured BEFORE the requested-length store. A provider
that changes the handle table while observing that store must not affect the
handle already passed to this allocation. No handle refresh happens between
length publication and allocation. M uses word subtraction/wrap,not clamped
host unsigned size validation. These reads follow the ignored replacement
CreateHeap result;the game keeps its existing handle table rather than writing
the replacement handle.

Retain allocation pointer and local M across the successful memset/flush calls;
those calls use local M. After the optional calls,RELOAD global requested length
8041e818 at800301e4 or80030474. Publish retained allocation pointer as primary
map root8041e808,then root.next=0,root.size=wrap32(RELOADED length-32),root.used
U16=0 in that order. Root header uses the reloaded length;payload memset/flush
use retained M. Null allocation skips memset/flush but still rereads length,
publishes0 and requests header stores at0. No safe recovery is supplied here.

Acceptance:source start→end→handle order and capture-before-length;changed
handle observed on length store leaves captured allocation argument;ignored
CreateHeap result leaves stored handle untouched;local payload length versus
reloaded root-header length after allocation/fill/flush;root publication before
next/size/used stores;null path still requests header writes. Actual host cache,
arena,invalid-address behavior,scene/user-pointer teardown and original traces
remain EVT-01/native backend requirements.

The existing bootstrap contract's earlier “end/start,store length,read handle”
summary is superseded by the exact table above. This addendum corrects ordering;
it does not infer allocation ownership or complete New2DS memory budgets.
