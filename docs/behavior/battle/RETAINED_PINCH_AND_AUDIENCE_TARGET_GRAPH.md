# Retained all-unit pinch and audience target graph

This implementation closes `BattleCheckAllPinchStatus8011efd8/214`, its full
`BtlUnit_CheckPinchStatus80126e00/100` child, both Mario audience adapters
`8019f440/38` and `8019f478/38`, and whole target parent/child
`801a1a74/180` and `801a18cc/1a8`. It is a prerequisite for real bank2 sequence
manager dispatch before Turn, not a new battle cadence or an active HUD claim.

Source behavior and original full-body fingerprints are specified in
[all-unit pinch](BATTLE_PINCH_ALL_UNIT_SCAN_AND_WARNING_CADENCE.md),
[per-unit transitions](BATTLE_PINCH_THRESHOLD_FLAGS_AND_TRANSITIONS.md),
[audience adapters](BATTLE_PINCH_AUDIENCE_REQUESTS_AND_SOURCE_THRESHOLDS.md)
and [target arithmetic](AUDIENCE_NONZERO_TARGET_ADJUSTMENT_CHUNKS_AND_ROUNDING.md).
Read-only verifiers `tools/verify_battle_pinch_status.py` and
`tools/verify_audience_target_accumulator.py` pin respectively 225 and 205
instruction anchors and all complete body hashes. Timing counts are source
invocations, not measured seconds or renderer frames.

## API and source ordering

`resumePackedBattleCheckAllPinch(B,mode,services,cursor)` retains one complete
invocation. Its update pass obtains all64 registry entries freshly in increasing
order, composing the whole per-unit body. Each danger callback completes before
the second phase reloads definition, HP and flags. Callback mutation can change
later registry entries. The direct per-unit API executes that same body and
requires a real nonnull unit.

Warning scans retain their current unit through the status27 service and then
read allegiance/kind/HP in source order. They stop at the first prequalified
actor even when that actor is wrong-kind or dead. Group2 starts a fresh registry
scan only after group1 fails. Countdown byte subtraction/wrap, signed reread,
reset60 and late sound gate are retained before a pending sound. A retry cannot
update units or decrement the timer again. No-found ordering is original Mario
lookup, optional Mario+238 clear, then unconditional B+18ff8 byte clear.

Actual actor memory and status27, source fctiwz and sound providers are explicit.
The original warning sound key address is passed to a caller-owned retained
`psndSFXOn` provider; missing sound holds only on a source branch that actually
requests it. A source-cleared sound gate needs no sound provider. No successful
sound allocation or synthetic voice is supplied. Providers/memory/lifecycle
must remain fixed until completion. Reset/reentry during a provider is refused.
A failed provider must retain its own partial effects.

The audience adapters independently obtain actual global B before reading
multiplier, baseline and zero and applying the source single multiply. Their
actual target child is composed internally, with no blind replay of the
synchronous target helper. The target parent and Sub capture global B
independently. Parent offset stores, source reloads, consumed chunks and
quotient operands survive a held conversion. Weighted updates use the target
captured before conversion; unweighted updates reload target after conversion.
Final clamps use the captured parent owner, source fresh loads and conditional
stores. The existing synchronous helper is unchanged.

## Correct original conversion bias address

The target arithmetic guide's `8030a328` address is a signed-low address decoding
error. At `801a19a4`/`801a19ac`, `lis r5,8030` followed by
`lfd f2,-23768(r5)` resolves **802fa328**. Likewise the outer `addi` at
`801a1b64` resolves802fa328. Read-only ISO bytes there are
`4330000080000000`; bytes at8030a328 are zero. This implementation reads the
actual double words at802fa328/802fa32c for the source xoris/43300000 conversion,
including denominator word wrap. The integration tests load those bytes from
the accepted original Trio pack, rather than constructing a replacement table.

## Native numerical and lifecycle boundaries

Single arithmetic results are rounded to binary32; signed integer conversion
uses the actual source bias construction. The caller supplies source fctiwz
semantics through `truncate`. An unavailable result holds its captured quotient,
without repeating earlier offset/chunk stores. It can be retried with the same
provider when actually available. Nonfinite delta or a chunk subtraction that
cannot progress enters a retained unavailable domain without further reads or
writes. This is an explicit portable domain policy, not exceptional PPC/FPSCR
or nonterminating-loop equivalence. No arbitrary iteration cap, invented zero
result, timing constant or original exceptional conversion is claimed.

Cursor reset drops native metadata only. It does not undo source stores,
reinitialize guest sequence state, replay callbacks or manufacture cleanup.
Actual owner retirement and once-per-simulation-pass scheduling belong to the
caller. The sequence manager's second child remains real Turn8011c510; completing
pinch must not acknowledge an unavailable Turn or install a synthetic queue.

## Verification

`battle_pinch_continuation_tests` loads accepted original Trio data and checks
315 finite target combinations against the existing independent synchronous
implementation's final values. Separate source-derived assertions cover exact
conditional stores, negative capacity, captured-versus-fresh target reads,
independent global owners, partial multi-chunk holds, replaced input/provider
rejection, nonprogress domain, and nonreentry. Pinch checks threshold edges,
suppression, first-callback HP/flag mutation, fresh global audience ownership,
all64 updates, future registry mutation, shadowing, warning-group order, signed
byte cases0/128/129, 60-qualifying-call cadence, sound/status holds and Mario reset.
Host captured callbacks are service tests, not original execution observations
or native sound/pixel/performance proof.
