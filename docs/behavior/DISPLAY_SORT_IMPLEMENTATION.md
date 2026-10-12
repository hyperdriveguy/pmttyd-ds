# Display sort implementation, partition ties and cycle permutation

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
qqsort0x800064fc,size184 and fsort0x80006680,size60c. Pinned Japanese system.c
contains complete corresponding source, used as a structural guide and checked
against US control flow; it was not rebuilt or copied into the native runtime.
See [display queue](DISPLAY_QUEUE_AND_DISPATCH.md). This establishes these local
algorithms, not executed draw order or a general host qsort replacement.

## Global scratch and wrapper admission (B)

qqsort(base,unsignedCount,elementSize,comparator) stores comparator at
r13+0x1678 (0x8041e598) before checking count. Count<=1 returns without touching
the supplied array or invoking fsort. No validation of base, size or comparator.
For count>1, construct a pointer table at0x803c5460: table[i]=base+i*size,
using word pointer arithmetic. Call fsort(table,count), then rearrange original
elements to match that pointer permutation.

Symbol metadata and pinned source agree on a0x3000-byte table (3072 pointers)
and0x100-byte temporary element buffer at0x803c5360. Binary references those
bases and has no count<=3072 or size<=256 guard. Display sorting uses count up
to its intended1024 entries and size4. This does not protect other callers or
the display queue's unchecked excessive registrations.

fsort swaps pointer words, not pointed-to element bytes. Comparator receives
the pointed-to original element addresses, so display's comparator sees addresses
of entries in its pointer array, then dereferences each to a display record.
Every comparator call loads the current global comparator pointer. Swap and
partition state is also global: swap word0x8041e584, tail0x8041e588,
greater-bound0x8041e58c, right0x8041e590, left0x8041e594. There is no nested-call
save/restore or locking. Comparator-triggered nested sorting can overwrite both
scratch arrays, comparator and partition state. Pure nonmutating comparison is
a fidelity assumption for the derived display examples, not a local guard.

## Small-count pivot preparation (B)

Each fsort iteration sets end=base+count-1 in pointer elements and global tail=end.
For count2, compare first/last, swap only if result>0, and return. There is no
general count0 guard in fsort itself; the qqsort wrapper admits only count>1.

Count4 skips median preparation: pivot=end, left=base, right=end-1,
greaterBound=end. For all other counts<40, choose mid=base+floor(count/2).
Let a/m/z be the pointer values at base/mid/end. The exact comparison tree:

- If cmp(a,z)>0: if cmp(a,m)<0, replace(a,m,z) with(z,a,m); otherwise test
  cmp(m,z)<0, replacing with(m,z,a) when true, or(z,m,a) otherwise.
- Otherwise: if cmp(a,m)>0, swap a/m; otherwise if cmp(m,z)>0, swap m/z.

Each comparison uses current pointed-to elements and signed result tests; no
identity/index tie breaker. Count3 returns after this tree. Counts5..39 set
left=base+1, right=end-1, greaterBound=end and pivot=mid, then partition below.
This preserves the exact branches for equal keys rather than substituting a
generic median-of-three routine with different tie exchanges.

## Large-count strided sample (B)

For count>=40, derive bit length b through a word-mask loop: initialize mask to
0xffffffff, step32, k63. While mask!=0, save t=mask&count; half=step>>1;
mask &= mask<<(half==0?1:half). If t!=0 subtract step from k; otherwise shift
mask right by step. Set step=half. All are original32-bit register shifts;
bit5 of the six-bit shift count produces zero. Set b=32-k when k<=32, else0.
For this nonzero count domain b is its integer bit length.

If b is odd, sampleCount=1<<(b>>1); otherwise sampleCount=count>>(b>>1).
Set stride=floor(count/sampleCount) with divwu. Keep targetMid at
base+floor(sampleCount/2)*stride and sampleTail at base+(sampleCount-1)*stride.
These are aligned positions in the pointer table; no random pivot/RNG draw.

Quickselect the target position on this strided grid. Start=startBase=base,
tail=sampleTail. For each pass set left=start, right=tail, greaterBound=tail,
pivot=start+floor(floor((tail-start)/2)/stride)*stride. Run the shared partition
below with step=stride. If greaterBound<=targetMid, set start=greaterBound;
otherwise if right>=targetMid set tail=right; otherwise finish. Continue only
while start<tail. TargetMid is fixed; pivot location moves during a pass.

After sample selection, reset tail to the original end, left=base,
right=end, greaterBound=end. Main partition uses pivot=targetMid and step1.
For example count40 gives sampleCount5,stride8,target index16,tail index32;
count64 gives sampleCount8,stride8,target32,tail56. These are derived arithmetic
examples, not execution traces. The sampled pointer swaps remain in the table.

## Shared partition and zero-comparison movement (B)

This mechanism is used first on the large-count sample and then on the complete
table; small counts reach only the complete-table version. All addresses below
advance by step pointer elements. Save the original range end separately.

1. Scan right while right>=left. Compare value at right with value at current
   pivot. Positive sets greaterBound=right; zero leaves that bound unchanged;
   both decrement right. Negative switches to the left scan.
2. Scan left while left<right. Negative comparison with pivot increments left
   and continues. Nonnegative comparison swaps the left/right pointer words.
   Positive sets greaterBound=right; zero sets pivot=right, tracking its moved
   value. Increment left, decrement right, then resume the right scan.
3. If scans finish, and greaterBound<pivot, swap those pointer words and advance
   greaterBound by step. Otherwise make no final pivot exchange.

The pivot-location update's original bit expression is tested only after signed
nonnegative admission; there it distinguishes positive from zero. Equal keys
may move through swaps and pivot relocation, while the right scan skips them.
Do not collapse both scan decisions into a symmetric <=/>= partition rule.

## Recursion and tail iteration (B)

After main partition, compare signed pointer-element distances
right-base and originalEnd-greaterBound. If left distance is smaller, recurse
on base..right and retain greaterBound..end for tail iteration. Otherwise,
including equal distances, recurse on greaterBound..end and retain base..right.
Invoke recursion only if the smaller range's high address is strictly greater
than its low address, so count0/1 subranges are not called. Compute retained
count=(end-base)+1 with word arithmetic; repeat top-level iteration only if
unsigned count>1. Pointer differences use the original truncation-toward-zero
division by4 sequence. Valid aligned ranges give exact element counts.

Smaller-side recursion occurs before larger-side iteration. Sample work uses
global bounds too, but original base/end/pivot/count needed across phases are
held in saved registers. Recursive calls overwrite global bounds; the retained
count/base are calculated before that call and rebuilt at the next iteration.
Comparator order matters if it has side effects; no purity enforcement locally.

## Applying the pointer permutation to original elements (B)

qqsort scans destination slots in increasing index. If table[i] is null, skip;
if it equals that destination address, skip without clearing that table entry.
Otherwise save the destination's size bytes to the static temporary buffer
using memcpy0x8000519c. Set current destination to that slot and current table
slot to i. Repeat:

1. memcpy size bytes from the address in the current table slot into current
   destination. Retain that source address as the next destination.
2. Clear the consumed table slot to null. Compute the next table index as
   unsigned word(sourceAddress-base)/size via divwu.
3. If that table entry points to the original destination, close the cycle by
   copying the saved bytes there and nulling its table slot; otherwise continue.

No element swap on each comparator call, heap allocation or duplicate-key
deduplication. Original source addresses form a permutation for valid distinct
slots. Zero size, oversized scratch needs, corrupt/null table entries inside a
cycle and callback mutation are not guarded. memcpy's byte-copy internals and
arbitrary overlapping domains remain separate service contracts.

## Display tie consequences and comparison evidence (B/P)

For display keys[1A,1B,0C], count3 takes first>last, first==mid, mid>last,
then swaps first/last: result[0C,1B,1A]. Equal A/B therefore reverse. Replacing
this sorter with stable sorting would change that derived callback order.
All-equal pure comparator input instead performs no pointer exchanges, including
the sampled path; this special case is not a universal stability guarantee.

An authored contract model checked3549 inputs: all ternary key sequences of
length0..7 through the wrapper, twenty seeded repeated-key cases at each of
13 threshold/size counts from8 to3072, and nine all-equal counts including
39/40,1024 and3072. Results were ascending permutations and the tie example
matched above. This is a derived algorithm check, not original instruction/
memcpy execution or proof for every comparator/input. Separate authored models
checked all5914 cycle permutations at lengths0..7 and8072 bit-mask loop counts
(1..3072 plus5000 seeded nonzero32-bit values) against their integer reference
results. These also do not execute original instructions/memcpy. Reports remain ignored.

Differential obligations: count<=1 comparator write, count2/3/4 branches,
strict median ties,39/40 sample threshold, bit-length sample count, zero-pivot
relocation, asymmetric scans, smaller-side/tie recursion order, cycle copy/null
markers, shared scratch and equal-key callback identity. Remaining: actual
caller/outer reset-sort-draw cadence, memcpy/service domains, executed original
sort/draw traces and full message rendering/story conditions. Full rendering/
complete-game equivalence remains unproven.
