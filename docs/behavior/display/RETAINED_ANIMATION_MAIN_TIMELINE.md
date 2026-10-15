# Retained Main timeline scans and scalar evaluation

US G8ME01 disc0 revision0. `advanceAnimationMainTimeline` implements the complete
local region8003ef80..8003f0f4, source SHA256
`0eb7dfa91876015ebcb4bb1b3da1b803e35569bf86951c0dd7b15e08a7b244f4`.
It ends after fraction R+30, before cache dispatch reads. See
[recovered sampling](ANIMATION_TIMELINE_SAMPLING.md),
[retained Main entry](RETAINED_ANIMATION_MAIN_ENTRY.md) and
[retained clock/sample time](RETAINED_ANIMATION_MAIN_TIME.md).

## Exact scan and publication order

Inputs are the original Main captures R/T and the loop selector captured before
the clock child. The selector is never reloaded here. All guest arithmetic uses
original low32 mulli/add/shift behavior. No active/resource/count/index/finite
or pre-table validity gates precede the source reads.

Continuous scan: load T+8 count, R+24 selected time ONCE, then T+28 key base.
Even count<=0 reads all three. Positive signed count permits per-key time reads
at stride44; only ordered keyTime>capturedSelected breaks. Equality and NaN do
not break. Count is captured for the whole scan. Store R+28=index-1, then READ
R+28 before storing successor R+2c=readPredecessor+1. A writer/alias can change
the predecessor after its store; computed scan index is not a substitute.

Discrete scan independently loads T+20 count, fresh R+24 selected time ONCE,
and T+40 base, including for count<=0. It applies the same ordered comparison
at stride8, then stores R+34=index-1, rereads R+34 and stores R+38=read+1.
Continuous and discrete selected-time captures are distinct; neither is loaded
again on a scan backedge.

Only nonlooping invocations clamp: fresh R+2c then fresh T+8 count, signed
comparison, and on successor>=count fresh R+28 is stored at R+2c. Independently
fresh R+38/T+20 and, if needed, fresh R+34 feed the second clamp. Counts are not
reused from the earlier scans. There is no lower-bound/predecessor clamp.

For all routes read fresh R+34 and T+40, read the state word at
base+(index<<3)+4, and publish R+80. Predecessor -1 is an actual pre-table read;
if mapped it returns the real source word. Missing memory holds at that exact
read, without replacing it with index0/state0 or certifying empty success.

## Fraction reloads and loop wrap

Read fresh R+28, T+28, R+24, then current key time. Retain the single numerator
selected-currentKeyTime across the remaining work. Nonloop reads fresh R+2c
and next key time from the retained key base; it does not redo count checks.
Loop reads fresh R+2c and T+8. For successor<count, use the retained key base
and current key time for the denominator.

For loop successor>=count, store R+2c=0, then reread R+28, T+24 control, T+28
key base, control END+8, and new current key time, in that order. Denominator
is fresh END-newCurrentKeyTime. Numerator stays captured from BEFORE the wrap
store and these reloads; changed selected time must not recompute it. Control's
loop selector is not read again. This can legitimately combine old numerator
and fresh denominator under source alias/writer mutations.

Always load fresh ZERO8041fb28 after computing denominator, independently of
the earlier clock child's retained ZERO. If denominator==loadedZERO, publish
that actual literal at R+30. Otherwise divide numerator/denominator with single
rounding and store R+30. No epsilon, ratio clamp or NaN rejection is invented.
Floating-point exceptions/FPSCR/NaN payload parity remain unmeasured.

## Retained scalar owner and live bindings

`advanceAnimationMainScalar` composes the completed clock/sample-time child
with this timeline child, covering contiguous8003ee08..8003f0f4. Its cursor
retains the successful time boundary and loop selector while scans, index/state
stores or fraction suspend. Input and full service identities remain pinned
across both phases, even when a numeric callback is no longer needed. It does
not report later cache replay, geometry output or full Main complete.

`AnimationMainTimeBinding::timelineServices` exposes the same real VM word/store
operations and shared pool aliases as the clock child. Every primitive checks
the current producer/lifetime predicate and actual alias ownership. No detached
state or fake source pointers are introduced. Binding, borrowed VM/alias owner
and producer pass must outlive suspended cursors.

Unavailable primitives have no effects, but a controller false may follow
acknowledged source stores. Callers retain child progress instead of replaying
parents. Scan backedges yield after one successful key consumption when another
iteration remains; this is native scheduling policy, not a total source count
cap. Final consumption and early break fall through directly. Huge positive
counts remain pending rather than clamped. All cursors are fixed-size with no
per-invocation allocation; pumping must preserve the SAME producer pass.

## Validation and next boundary

Tests pin one explicit full request/store trace and every primitive hold in 14
loop/nonloop routes: ordinary/end/empty/signed-negative/pre-first/NaN/equal keys.
They test independent selected captures, predecessor rereads, changed clamp
counts, old numerator with fresh wrap denominator, fresh ZERO, real missing
pre-table memory, huge scan yields, modulo addresses and owner/input pinning.
An actual cold process compares R20/R24/R84, indices, discrete state and fraction
against the admitted original source-backed sampler. It stops after actual R80
publication, mutates epoch and resumes only the remaining R30 store. Conversion
stays at one; completed time/indices/state do not replay. Full scalar provider
identity and native timeline availability are also tested.

All original 2152 product hashes and gor_02.s remain unchanged. No asset or
story/dialogue conversion choices change. Ignored read-only evidence lives in
`artifacts/npc-main-parent-evidence/animation-main-timeline-notes.txt` and the
matching `animation-main-timeline-review.txt` listing. The original containing
Main body remains pinned by `tools/verify_animation_main.py`.

Next cache captures begin8003f0f4: R28 current, R3c old, R2c next, R30 fraction,
then MODEL counts f0/f8/13c/140/12c, before branching on old==current. Those are
fresh future reads, not values pre-captured by this child. Retain complete cache
replay and interpolation with original captured resource/output owners, compose
them through Main/Inner and real platform/linked/queue/GPU services, then
activate source NPCs in normal Game. [EVT-01](../../requests/EVT-01.md), original
field visual/event/numeric parity and physical New 2DS XL performance remain open.
