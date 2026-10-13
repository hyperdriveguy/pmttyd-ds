# MusyX stream free, buffer exit and reverse state clear

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamFree8027c3e0/6d4hex,
437 instructions, plus complete GPR25..31 save/restore helper spans. Addresses/
offsets hexadecimal; stages/counts decimal. Static source deductions, not original
free/ARAM/audio execution. Read [deactivation chain](MUSYX_STREAM_DEACTIVATION_AND_COMPANION_CHAIN.md),
[ID lookup](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[buffer exit bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md),
[buffer-list lifetime](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Five entered lookup stages

Retain raw input ID. Each stage requests hwDisableIrq, then selects first
nonzero-state exact fullword public-ID match among64 streamInfo803f98e8 rows,
stride68hex. Stages0..3 inline eight iterations of eight checks; stage4 requests
GetPrivateIndex at8027c9e0. No live-count bound, ID sentinel rejection, generation
check, state1/2 restriction or hardware-active query. State3/unknown nonzero rows
can be freed. Missing ID skips that stage's state clear but still enables its
entered level and unwinds all retained parents.

| Stage | Disable | Deactivate | Buffer exit | State0 store | Enable |
| --- | --- | --- | --- | --- | --- |
| 0 | 8027c3f8 | 8027c54c | 8027c564 | 8027ca94 | 8027ca98 |
| 1 | 8027c580 | 8027c6cc | 8027c6d8 | 8027ca7c | 8027ca80 |
| 2 | 8027c6f4 | 8027c840 | 8027c84c | 8027ca64 | 8027ca68 |
| 3 | 8027c868 | 8027c9b0 | 8027c9bc | 8027ca4c | 8027ca50 |
| 4 | 8027c9d8 | 8027c9f8 | 8027ca04 | 8027ca34 | 8027ca38 |

## Deactivate first, fresh buffer selector, then fresh companion

After a match request sndStreamDeactivate(currentID), even for an already-state3
row. It processes its own companion chain and IRQ requests; it is not a single
voice-release leaf. No child-return/status check or lookup/generation recapture.
Retain the originally selected ordinal and its byte offset.

AFTER deactivation read that row's buffer selector byte0e; request
hwExitStream(selector). No ff-selector rejection, reference/generation/capacity
check or state gate. Hardware bridge owns stream-buffer release details.
AFTER buffer child freshly read row.word0 companion. Sentinelffffffff begins
unwind; every other word enters next local stage. A child can alter the selector
or companion before these fresh reads. This is not a stable chain snapshot.

No local record clear happens before the companion traversal: row state remains
whatever deactivation/children wrote. There is no buffer-exit success gate or
partial-failure rollback. ID/flags/source/format/preference/voice/cursor words
are not erased by this caller.

## Recursion and deepest-first state publication

Stage4 non-sentinel companion requests sndStreamFree(ID) at8027ca20; ignore
return. Up to five IDs are processed locally before recursion. No visited-ID set,
chain-depth/recursion cap, cycle or duplicate-row check. Deactivation itself can
follow the same chain before this function reaches its buffer exit. Valid chain
termination/provider domains cannot be inferred from state changes.

After sentinel, missing descendant or recursive return, unwind in reverse local
stage order. For EACH FOUND retained stage: recompute its row from retained
ordinal, store state+c=0, then hwEnableIrq. A missed stage enables without clearing
any row. Full found local unwind: state0/enable stage4,3,2,1,0. Recursive child
completes its own clears before parent stage4 clears.

The final stores use retained slots without public-ID/state/generation checks.
A child replacing or reusing a row does not prevent this caller's later clear.
Public ID and companion fields remain stored after state0; normal lookup then
ignores the inactive row. Parent buffer exits precede descendants, while state
clears follow descendants. Do not move state0 to before traversal or omit repeated
deactivation calls without proving original equivalence and provider domains.

IRQ requests have no saved prior-state token; children have independent nesting/
OS behavior. Local disable/enable counts are source requests, not proof of safe
critical-section restoration or original interrupt state. No independent success/
failure return assignment is made; residual r3 is not a documented Boolean.

## ABI and validation

Frame allocates48 decimal bytes; LR is saved at frame+34hex. _savegpr_25 at
80264980 falls through seven stores GPR25..31 at oldSP-28 through-4 then blr.
_restgpr_25 at802649cc performs seven corresponding loads then blr. Suffix
restores LR/stack. Complete32-byte helper execution spans are pinned in addition
to CSV4-byte entry boundaries; valid stack/ABI remains a precondition.

[Manifest](../../../tools/musyx_stream_free_chain_evidence.json) pins ten spans,
453 anchors (437 body/16 helper),24 call sites. Fifteen finite stable acyclic-chain
models cover lengths1..7, missing links at each stage, mixed/already-deactivated
states and inactive gap. They apply the recovered stable deactivation effect,
then model exit order and reverse state clears; child IRQ/list internals and
concurrency are excluded. Generic verifier checks hashes/anchors/calls, not
semantic deductions or original execution. Fresh full ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Local whole body answered.

Remaining: stream frequency/mix, actual ID/state/voice/buffer/companion/IRQ/
resource providers and valid value/index/extent/alias/lifetime/concurrency/stack/
ABI/termination domains, transitive ARAM/DSP consumers and original chained
free/deactivation/failure/audio traces. EVT-01 and native/full-game binding OPEN.

## 2026-10-13 — Frequency follow-up

[Frequency](MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md) closes the full frequency/state2 pitch/companion/
recursive/IRQ body. Mix/FP/chain/provider domains/original audio/native binding open.
