# MusyX voice allocation selection and bookkeeping

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete voiceAllocate80284fdc/110hex
and voiceAllocateFind80284c54/388hex. Addresses/offsets/masks hexadecimal;
counts/scalars decimal. Static selection/list/counter semantics, not original
voice-stealing/polyphony/audio observations. Read
[macro start](MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md),
[voice kill/recycle](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md) and
[synthesis initializer](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md).

## Inputs, gate and captured type limit

Use raw inputs (P,G,Q,F): requested priority, per-group cap, group key and
requested kind. voiceAllocate retains F and forwards all four to
voiceAllocateFind. Search first reads synthIdleWaitActive8041ee98; nonzero
returnsffffffff before list search. There is no independent sndActive/IRQ gate.

F.low8==0 chooses Music running byte8041ef36 and synthInfo.byte211 limit L;
any nonzero chooses FX running8041ef37 and synthInfo.byte212 limit L.
Read synthInfo.byte210 total capacity T only when running>=L. Force same-type
candidate filtering iff running>=L AND T>L. This is not simply a full-type
counter test or refusal when the limit is reached.

If L>G.low8, run the group-cap phase below; otherwise go directly to free/
general selection. Priority comparisons use P.low8. These source branches do
not establish producer validity for limits/running/priority or normalize F into
0/1 for later exact candidate-byte equality.

## Tables and traversal primitives

Captured synthVoice pointer uses458hex-byte records. Allocation-aid domains:

| Domain | Exact address/shape |
| --- | --- |
| Priority root ordinal | halfword8041ef34, sentinel65535 |
| Priority voice-link records | 80406340, stride4; successor byte1 |
| Per-priority head byte | 80406440+priority; sentinel255 |
| Priority sort records | 80406540+priority*4; successor halfword0 |
| Free-link records | 80406940+slot*4; previous byte0/next byte1/flag half2 |
| Free insert marker/root | bytes8041ef38/8041ef39; sentinel255 |

The original voice shapes support64 slots,256 priority heads/sort rows, but
these traversals do not add count/bounds/member/cycle/sortedness guards. Node
indices/flags and selected voice fields are read fresh when visited; captured
base pointers/scan ordinals persist. No full voice-ID generation validation.

## Complete group-cap selection and remaining count scan

Load priority root, synthVoice and table bases. Start matching count N=0 and
candidate S=ffffffff. Matching count increments wrap32; cap comparisons use
signed count against the nonnegative low8 cap. Scan priority buckets ascending through the sort successor
chain while bucket<=P.low8 and S is still missing. For EVERY voice in a scanned
bucket whose word100==RAW Q, increment N BEFORE eligibility checks.
A matching voice can supply a candidate only if:

- byte11c==0 (not notification-suppressed).
- If forced same-type, byte11d==F.low8 EXACTLY.
- low flags word118 bit2hex==0; high114 contributes no eligibility bits.

Among eligible voices in the current bucket select the minimum UNSIGNED word110;
equal values retain the first visited candidate. Finish that bucket's whole
voice chain even after finding S, then stop seeking candidates in later buckets.
Thus bucket order dominates metric order across different priorities.

If N>=G.low8, return S immediately, including missingffffffff. Protected/
suppressed/type-mismatched matching voices still count. Cap0 is not interpreted
as unlimited: N=0 already satisfies it and can return missing before free use.
This phase does not independently recheck selected byte10c priority.

If N<G.low8, continue through REMAINING priority buckets, counting matching
word100==Q voices only until N>=cap or chain end. This count scan is not limited
to requested priority and does not apply suppression/type/flag eligibility.
If it reaches cap, return the earlier S (possibly missing). Otherwise proceed
to free/general selection. No new candidate is chosen during this count-only
scan; do not substitute a generic scan of all voices or counts of eligible ones.

## Complete free-list admission and general replacement

Read free root byte8041ef39. If it is not255 AND same-type filtering is not
forced, return that raw free slot immediately, without list mutation here.
If unavailable or forced, read priority root fresh. Requested P.low8 below
that ordinal returnsffffffff (including the sentinel65535 case).

General replacement scans priority buckets<=requested priority, stopping after
the first bucket containing an eligible candidate (after finishing that bucket).
Eligibility is the same suppression/exact-kind/low-flag rule above, WITHOUT
word100==Q matching. Again minimize unsigned word110, retaining first on ties.
No candidate returnsffffffff. Otherwise read selected voice.byte10c fresh:
if greater than P.low8 returnffffffff; else return selected byte index.
This final priority check exists here, unlike the group-cap early return.

No eviction/voiceKill/hardware break, active/time-list unlink, VID removal,
reference cleanup, byte11d update, priority insertion or allocation occurs in
voiceAllocateFind. It returns a choice; mutations are in the wrapper/callers.

## Complete allocator free-record removal

voiceAllocate receives search result S. Exactffffffff returns unchanged with
no bookkeeping. Other S selects free record R=80406940+wrap32(S*4), without
low8 masking or another slot/bounds check. If R.half2==1, unlink it:

1. Read previous byte0. Non255 stores fresh R.byte1 into previous.next1;
   previous255 stores fresh R.byte1 into freeRoot8041ef39.
2. Read fresh R.byte1. Non255 stores fresh R.byte0 into next.previous0.
3. Read insert marker8041ef38; if S equals it, write fresh R.byte0 there.
4. Store R.half2=0.

Own link bytes remain uncleared. No membership/root consistency or alias repair.
A free record does not decrement either running counter.

For R.half2!=1 instead, derive voice from fresh synthVoice+wrap32(S*458hex),
read byte11d. Nonzero decrements FX running, zero decrements Music, using low8
stores with wrap. No record flag/state/priority/identity validation or eviction
child request. A stale/inconsistent count can underflow; no saturation is added.

Finally F.low8!=0 increments fresh FX running; otherwise increments fresh Music,
also wrapping bytes. Return raw S. It does NOT set the selected voice's kind
byte or initialize its macro/controller/VID state; the macro-start caller owns
those later preparations. There is no rollback if that caller subsequently
fails, beyond whatever its separately recovered cleanup children provide.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_voice_allocation_selection_evidence.json) pins
two complete bodies/two ABI entry fingerprints:four spans,168 instruction
anchors,three direct calls. Six limit/seven counter/seven group eligibility/
three metric/five cap vectors and exact global/table addresses are source
deductions, not original execution or validity proofs. Eligibility vectors
model the group phase; the general phase intentionally omits its group match.
ABI entry hashes do not close complete save/restore fallthrough chains. Full
fresh inspection ELFs/listings stay `/tmp/inbox-voiceAllocate.elf`,
`/tmp/inbox-voiceAllocate.listing`, `/tmp/inbox-voiceSelection.elf`.
No original bodies/binary assets committed or original code executed.

Remaining: allocation-aid initialization/priority insertion/free/recycle writers,
actual P/G/Q/F and type limits/running/voice marker/metric/ID providers, valid
slot/priority/member/sorted/cycle/alias/IRQ/reentrancy/concurrency/accounting/
rollback domains and original quota/free/replacement/audio traces. Two local
bodies are answered; EVT-01 and full native/source/runtime/hardware equivalence
remain OPEN.
