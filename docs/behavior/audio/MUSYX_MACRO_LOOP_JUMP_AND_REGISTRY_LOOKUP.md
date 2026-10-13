# MusyX macro loop, jumps and registry lookup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdLoop8027f8cc/154hex,
dataGetMacro8027f05c/8chex, sndBSearch80290384/a4hex, maccmp8027f04c/10hex;
complete interpreter regions80282c08..80282cac,80282cc0..80282cd0,
80282cd0..80282d18 (exclusive ends). Other opcodes/resources remain open.
Addresses/offsets/masks hexadecimal; counts and indices decimal.
Static control flow, not original loop/event/audio execution. Read
[dispatch/PC budget](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[wait/RNG](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md) and
[initial counter reset](MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md).

## Complete shared-counter loop

mcmdLoop(V,E) reads unsigned V.halfaa. This is ONE voice counter, not a
per-command/nested-loop stack. If count0, read E.word0 bit10000hex:
set requests sndRand, then read fresh E.word4.high16 N and compute signed
remainder random.low16 % N using divw/mullw/sub. Both operands are nonnegative
for valid N!=0, but there is NO zero-N guard. This differs from wait's initial
zero-duration admission; zero divisor has no established valid result here.
Clear random bit stores E.word4.high16 directly into halfaa.

After initialization reread count. Exact65535 jumps to the condition gates
without increment/decrement. Other counts increment low16 by1, then immediately
reread/decrement low16 by1. Thus configured N=0 leaves0/no branch; N=1 leaves1
and reaches gates on this first encounter. On an already-nonzero entry,65535
also bypasses countdown; any other count decrements by1. A resulting0 skips
gates/PC write. No new RNG/configuration read on an existing nonzero counter.

For stable finite N, the first encounter retains N and each later encounter
counts down; N backedges can occur before falling through. No handler stop-marker
assignment or per-invocation command-budget bypass; the enclosing interpreter
still owns its normal budget. Counter65535 is not a timed indefinite-wait
sentinel: it follows loop gates/branch handling, with no deadline/list transition.

## Complete loop gates and branch target

For a surviving counter, read fresh E.word0 W0. If W0&100hex set, read voice
high114 then low118. EXACT low bit8 set and high bit100 clear aborts: set
halfaa=0 and return without PC write. Other flag combinations proceed.
No flag writes occur in this gate beyond that counter reset.

If retained W0&1000000hex set, read high114 then low118. Low20 set bypasses
hardware query; otherwise request hwIsActive(V.wordf4.low8). Returned0 resets
halfaa=0 and returns; nonzero proceeds. There is no full-ID/generation or
post-hardware counter/flag/source recheck. Counter updates before gates survive
an abort. The loop handler itself has no active/IRQ/pointer/domain check.

On admitted branch, read fresh E.word4, then V.word34 macro base. Store
V.word38=wrap32(base+8*(E.word4.low16)). No signed-relative offset, current-PC
addition, range/alignment validation or stack push. Return is incidental;
opcode5 stub ignores it and leaves the interpreter stop marker0.

## Exact termination and conditional/direct jump regions

The source dispatch table registers these recovered cases:

| Opcode | Stub | Complete behavior |
| --- | --- | --- |
| 0 | 80282c08 | vidRemoveVoiceReferences(V), voiceFree(V), stop marker1 |
| 1 | 80282c20 | Same ordered two requests and stop marker1 |
| 2 | 80282c38 | Threshold on V.half12e; optional macro lookup/jump; continue |
| 3 | 80282c70 | Threshold on (V.word158>>16)&255; optional lookup/jump; continue |
| 5 | 80282cc0 | mcmdLoop(V,&shared cstep); ignore result; continue |
| 6 | 80282cd0 | Macro lookup/jump or cleanup+stop on miss |

Termination child returns ignored; their complete internal resource/reference
semantics are not closed by these request stubs. Fetch already advanced PC by8.
No hidden hardware-break/active-system check or rollback is added locally.

For opcodes2/3 threshold=(captured command word0>>8)&255. If voice value<threshold,
continue without lookup. Otherwise request dataGetMacro(command word0.high16).
Return0 also continues without changing base/PC; nonzero stores returned pointer
at V.word34, THEN reads shared command word1.low16 and stores
V.word38=wrap32(returnedBase+8*step). Stop marker remains0. The macro lookup child
can affect global/shared state before the fresh word1 read.

Opcode6 uses the same lookup and success base/PC order without the threshold.
On lookup0 request vidRemoveVoiceReferences(V), then voiceFree(V), set stop1.
Success sets stop0. Failure is therefore not the conditional branches' no-op.
No new bounds/type/state or independent getter-found status is invented.

## Complete macro registry lookup and scratch side effects

dataGetMacro(I) derives bucket=(I>>6)&3ffhex, using input bits6..15. Header H is
dataMacMainTab804012e8+bucket*4. Read H.half0 count, then store bucket to global
8041eee8. Count0 returns0 immediately; it does NOT clear prior result/key/start
scratch globals. Raw input high16 does not contribute to bucket or key.

For nonzero count, read H.half2 start; store I.low16 at key scratch8041eee0+4,
store start at8041eeec. Request:

`sndBSearch(&key8041eee0, dataMacSubTabmem80401ae8+start*8, count,8,maccmp)`.

Store raw returned record pointer at result scratch8041eedc. Null returns0;
nonnull returns fresh record.word0, the raw macro pointer. A matched record
whose word0 is0 therefore still returns0 to the interpreter. No pointer/code
extent/generation/ownership or record-key revalidation after search.

Original main table is800hex bytes (512 four-byte headers); pool is4000hex bytes
(2048 eight-byte records). Bucket permits0..1023 and halfword count/start are
unchecked, so those shapes are provider invariants, not guards in this body.
No registry initialization/asset residency is proved by lookup alone. The global
key/start/result/bucket scratch is shared; concurrent/nested lookups can mutate
it. No private key snapshot or IRQ/lock is requested.

## Complete binary search and comparator

maccmp(key,record) reads record.half4 FIRST, then key.half4; returns the signed
integer keyID-recordID. Unsigned halfword inputs keep difference within
-65535..65535. No record read/write except that field and no child call.

sndBSearch(key,base,N,stride,compare) retains raw pointer/stride/comparator words.
N==0 returns0 before comparator invocation. Otherwise initialize low=1, high=N
and enter search without an initial positive/bounds check. Each iteration:
mid=ASR32(wrap32(low+high),1); derive record=base+wrap32((mid-1)*stride), request
compare(key,record). Return0 selects that record immediately. Signed negative
comparison sets high=mid-1; signed positive sets low=mid+1. Continue while
signed low<=high; otherwise return0. No duplicate-first/last scan or final
record/key recheck. A duplicate match returns whichever midpoint was encountered.

No comparator-null, sortedness, positive-N/stride, multiplication-overflow,
base/pointer/bounds/alignment or callback-mutation validation. The macro caller
supplies unsigned halfword count and stride8; other direct callers need their
own valid domain. ABI helpers preserve registers but do not create those guards.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_loop_jump_lookup_evidence.json) pins four
complete bodies,three complete regions,six linked fingerprints and dispatch
table:14 spans,153 instruction anchors,15 direct calls. Comparator pointer/
indirect-call binding is explicit. Nine loop/five step-offset/seven bucket/
six stable binary-search vectors and six opcode registrations are deductions,
not original execution or proof of registry/sorted/loop ownership invariants.
Full fresh inspection ELFs/listings stay `/tmp/inbox-mcmdLoop.elf`,
`/tmp/inbox-dataGetMacro.elf`, `/tmp/inbox-sndBSearch.elf`,
`/tmp/inbox-macroSearchCompare.elf` and prior macHandleActive files.
No original bodies/binary assets committed or original code executed.

Remaining: registry/assets/count/start/key/pointer writers, actual loop/threshold/
command/counter/macro-PC/flag/ID inputs, termination/reference/free child bodies,
valid nonzero random-divisor/sorted/member/code-range/alias/IRQ/reentrancy/
concurrency domains and original loop/branch/termination/audio traces. Four
local bodies/three regions are answered; remaining interpreter opcodes, EVT-01
and full native/source/runtime/hardware equivalence remain OPEN.
