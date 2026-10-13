# MusyX ARAM stream-buffer lists, allocation and release

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete InitStreamBuffers80292094/c4,
aramAllocateStreamBuffer80292158/138, aramGetStreamBufferAddress80292290/38,
aramFreeStreamBuffer802922c8/f8 and dataARAMDefaultGetInfo80287534/10hex.
Read [initialization](MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md) and
[sample store/cursor](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md).
Addresses/offsets below are hexadecimal; counts/strides labeled decimal.

## Registry and complete initialization

aramStreamBuffers80410860 is400hex bytes:64 descriptors,16 decimal bytes each.
Fields: word0 linked next; word4 ARAM address; word8 active requested aligned size;
wordc retained allocation capacity. Lists are pointer heads:
idle8041efc0, cached/free8041efc4, used8041efc8. Stream boundary is8041efd4,
top8041efd8. These source names do not imply an SDK allocation object.

InitStreamBuffers stores used0, cached0, idle=registry base, in that order.
Writes next pointers0→1→...→63→null:56 links in seven eight-link blocks,
seven tail links and final null. Other descriptor fields are untouched.
Then read aramTop and store it to stream boundary. No IRQ, free/transfer call,
old descriptor release or whole-registry clear. Reinitialization does not prove
that outstanding streams/voices stopped referencing old addresses.

## Complete allocation and cached search

Input byte count N. L=wrap32(N+1f)&ffffffe0. Initialize candidate0,
previous0,best=ffffffff; traverse captured cached head by next pointers.
At each descriptor D, read capacity D.wordc:

- Exact capacity L: retain D as candidate and BREAK, keeping predecessor from
  the preceding visit.
- Unsigned capacity>L and capacity<best: retain D, update best.
- For every non-exact visit, assign previous=D and advance D=D.word0.

This does NOT retain the predecessor alongside a larger best-fit candidate.
At end-of-list previous is the LAST visited descriptor, irrespective of candidate.
Capacityffffffff cannot become a strictly larger candidate because best starts
ffffffff. List/cycle/capacity/order domains are caller prerequisites.

If candidate exists: previous nonnull writes previous.word0=candidate.word0;
otherwise cachedHead=candidate.word0. Then candidate.word8=L, candidate.word0=
fresh usedHead, usedHead=candidate. Capacity/address remain unchanged, no split,
copy, boundary update or allocation call. Preserve the actual predecessor write:
for cached capacities[64,96] decimal/request32, candidate is FIRST while previous
is SECOND, so it writes second.next=first.next and can make second self-linked.
This is a derived invocation, not an original runtime occurrence. A port cannot
silently substitute a corrected predecessor algorithm as recovered behavior;
source list invariants/reachable reuse traces remain necessary.

## New downward allocation and returned ordinal

No candidate: check idleHead nonnull; otherwise return255 decimal. Request
dataARAMDefaultGetInfo, read stream boundary, then read the returned record
word8 sample cursor. Compute wrap32(boundary-L), unsigned-compare to that cursor; lower refuses255.
No underflow/zero/null guard. On admit, reload idleHead, retain D, move idleHead
to D.word0; store D.wordc=L then D.word8=L. Reload boundary, subtract L,
store new boundary then D.word4=newBoundary. Store D.word0=fresh usedHead,
usedHead=D. No recheck of idle pointer/capacity after the child getter.

Any allocated candidate returns low8(trunc_toward_zero((D-registryBase)/16)).
Valid initialized descriptor pointers produce ordinals0..63; failure255.
No IRQ, runtime transfer, audio start or native allocation occurs in this body.
Zero and wrapping input sizes are not rejected locally.

## Complete address getter and default-cache pointer leaf

GetStreamBufferAddress(rawID,optionalSizeOut): derive offset=(rawID&ff)*16.
If output pointer nonnull, read descriptor.word8 and write it through output;
then independently form the descriptor address and return word4. No ID<64,
used-list membership or lifetime guard. Optional output aliasing can change the
later returned word4; do not treat this as an immutable pair snapshot.

IDs64..255 address beyond the initialized registry. An allocation failure255
must not be supplied as a valid ordinal without external evidence.

DataARAMDefaultGetInfo returns80407650+60c=80407c5c. Only address construction;
no source/default/cache initialization, bounds check or sample-cursor copy.
The allocation comparison reads that record's current word8 afterward.

## Complete free and source traversal writes

Free(rawID) addresses registry+(rawID&ff)*16 without a bounds guard. Scan used
head by next pointers; on pointer match unlink through predecessor or usedHead
and break. A scan miss DOES NOT RETURN: it still reads target descriptor fields
and follows the same branches. Double-free/miss/invalid ordinal can mutate lists.

Compare target.word4 to fresh stream boundary. If unequal: target.word0=
fresh cachedHead; cachedHead=target; return. No active-size clear or boundary change.

If equal: target.word0=fresh idleHead; idleHead=target. Scan remaining used list,
compute minimum unsigned word4, initializedffffffff; equal addresses replace the
minimum with the same value. Then scan cached list, retaining each next pointer
BEFORE mutation. For each cached.word4<minimum: cachedHead=retainedNext,
node.word0=fresh idleHead, idleHead=node. Otherwise leave that node's next alone.
Always advance using retainedNext. This is not a predecessor-based removal:
every qualifying node resets GLOBAL cachedHead even if earlier nodes were kept.
Safe cached-list ordering is a source invariant still to establish.

Finally boundary=minimum, or fresh top if minimum isffffffff. No released
address+capacity calculation, coalescing, voice stop, DMA synchronization, memory
zero, descriptor-size clear or independent success return. Freed interior buffers
are cached; boundary frees rebuild availability using remaining used addresses.
Actual residency/use-after-release is not proved by local list operations.

## Source validation and remaining domains

[Manifest](../../../tools/musyx_aram_init_stream_evidence.json):eight complete
bodies across these two contracts, eight linked-only SDK/helpers,251 anchors,
14 resolved direct calls. Derived64-node idle chain,8 alignment vectors,7 low-byte
index vectors and7 cached search vectors pin initialization/width/search deductions;
original functions are not executed. Full inspection ELFs stay `/tmp`.

Remaining: actual size/input/list/cycle/predecessor/order/membership/concurrency
invariants, default-cache producers, stream upload and source consumers, SDK/voice/
DMA and original allocate/reuse/free/audio traces. Five complete local bodies are
answered; full native/source/runtime behavior remains OPEN.
