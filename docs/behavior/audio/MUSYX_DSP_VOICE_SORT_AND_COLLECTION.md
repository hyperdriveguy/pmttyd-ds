# MusyX DSP voice sorting and collection

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SortVoices80288ba8/258hex;
complete salBuildCommandList REGIONS802891b4..80289218 and
8028a964..8028a974 exclusive. The reverse record load80289218 is separately
anchored. Whole builder80288e00/25f8 fingerprinted; other regions remain open.
Addresses/offsets hexadecimal; indices/counts/shifts decimal. Read
[studio prepass](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md),
[priority word writer](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md) and
[voice setup/admission](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
Static pointer/key order, not measured scheduling, audible priority or DSP execution.

## Complete sorter admission, midpoint and key

SortVoices(A,L,R) takes pointer-array base A and inclusive raw32 indices L/R.
Retain A/R. SIGNED L>=R returns after ABI restoration, without array/key accesses.
No pointer/count/allocation/index/nonnegative/overflow/cycle/member validation.
Every address uses wrapped index<<2 and pointer arithmetic.

For admitted range, midpoint M is computed from wrapped32 sum Z=L+R:
M=arithmeticSigned32(wrap32(Z+(Z logical>>31)))>>1. This is signed division
of the wrapped sum by2 toward zero, not widened average or L+(R-L)/2.
Ordinary nonnegative no-overflow indices yield the usual midpoint; invalid raw
ranges/sums can select unrelated memory. Other midpoint computations use the
same sign-biased division after wrapped left+right arithmetic.

Key is UNSIGNED word1c of the pointed-to record. Each comparison loads scanned
pointer, then freshly loads pivot pointer at A[left], then reads their keys.
STRICT scannedKey<pivotKey admits relocation; equal keys do not. No tie-breaker,
record identity/generation/range gate or signed priority comparison.

## Partition stores and exact recursive request order

The body contains three copies of this partition operation for ordinary valid
inclusive ranges; it is not a child partition call:

1. Capture A[left] and A[midpoint]; store midpoint pointer to A[left], then
   old-left pointer to A[midpoint]. Retain scan boundary p=left.
2. Scan j=left+1 through right. For every strict-lower key, capture A[p+1],
   increment p; store captured scanned pointer to A[p], then displaced pointer
   to A[j]. The scan pointer advances even when no relocation. Self-swaps still
   perform both stores. Pivot keys are freshly loaded, not a captured scalar.
3. Capture pivot A[left] and A[p]; store A[p] to A[left], then pivot to A[p].
   Retain resulting partition index p.

First partition the input[L,R], obtaining p. Then:

| Admission | In-body second partition | Recursive calls, in order |
| --- | --- | --- |
| SIGNED L<p-1 | Partition[L,p-1], obtaining q | SortVoices(A,L,q-1), then SortVoices(A,q+1,p-1) |
| SIGNED p+1<R | Partition[p+1,R], obtaining q | SortVoices(A,p+1,q-1), then SortVoices(A,q+1,R) |

The left admission/partition/both calls finish before the right admission/partition/
both calls. Each recursive child applies the same initial L>=R gate, so empty/
singleton child requests are still issued then locally return. The outer body's
second partition skips singleton sides. Do not replace this with different pivot
selection, stable sorting, one recursive call per side or a reordered recursion
under arbitrary mutation/alias domains.

Under stable keys, valid disjoint pointer/key storage and admitted ranges, output
is an in-place permutation in ascending unsigned key order. Equal entries can
move through midpoint/pivot swaps: three identical keys on distinct records
with initial identities[0,1,2] yield[1,0,2], not stable order. No new record/list
links or priority values are written by the sorter. ABI helper requests save/
restore registers; no independently assigned scalar result/status.

## Complete builder collection and call

Retain current studio S=r16 from preceding builder work. Read S.word48 as root;
initialize local count N=0 and collection pointer at voices$59180408248.
While captured/current record pointer nonnull: store it to the array, advance
array pointer4 bytes, increment N with wrap32, then read that record.word0c as
next. No active-byte/member/studio/generation/duplicate/cycle check. The list
root is read once, with successor loads after each array store.

Original BSS declaration is100hex bytes,64 pointer slots. The collector has no
N<64 guard and does not clear trailing slots or detach the list; source-valid
list length/cycle/array-address invariants remain required. Overlap between the
array and list/records can affect future fresh successor reads.

Request SortVoices(array,0,wrap32(N-1)) at802891f4 even for N0/1. Ordinary N0
supplies R=ffffffff (signed-1), N1 supplies0: both sorter calls return without
array/key access. No sentinel error status is checked afterward.

Form retained end pointer=array+wrap32(N<<2); retain remaining=N in r28;
set stack.word1c=0, then branch to reverse-loop test8028a96c. Another local
register r18 is set0 before collection; its later parameter-block role belongs
to still-open surrounding builder work, not a sorter result.

## Complete reverse iteration tail and record binding

The loop test is SIGNED remaining>0. If true, record load80289218 reads
V=*(retainedEndPointer-4). The [setup contract](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md)
then tests V.bytef0 and runs admitted local voice paths. At the shared loop tail
8028a964, subtract4 from retained end pointer and1 from remaining; test again.
No key/priority re-sort or array reload before each iteration.

With unchanged records/keys and valid ordinary N, this consumes sorted pointers
from highest unsigned key toward lowest, reversing the sorter's ascending array.
Current-record callback/active changes can skip later work but do not re-sort the
remaining array here. Equal-key consumption reflects reversed unstable pivot
order; do not infer FIFO/LIFO fairness or the independent synth priority-list
policy. Full budget/voice admission/DSP command execution remains separate.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_voice_sort_and_collection_evidence.json) pins
complete sorter, two regions, linked whole builder/two ABI leaves and reverse
load: six spans,180 instruction anchors/seven direct calls (four self-recursive,
two ABI,one caller). Eight identity/key/partition-order vectors, six wrapped
midpoint vectors and a3280-case source-order mathematical model check verify
permutation/unsigned ascending properties for lengths0..7 using keys0/1/ffffffff.
Tie vector records actual model identity movement. These are source deductions,
not original execution, pointer-domain proofs or mutation experiments. BSS array
extent is a symbol binding, not initialized table data. Full fresh ELFs/listings
remain `/tmp`; no bodies/binary assets committed.

Remaining: actual priority/list/count/index/record/array writers and valid signed
range/overflow/member/cycle/duplicate/alias/lifetime/IRQ/concurrency domains,
surrounding builder budget/admission/cache/commands and original collection/
sort/voice/DSP/audio traces. One complete sorter/two regions answered; whole
builder/full native binding and EVT-01 remain OPEN.
