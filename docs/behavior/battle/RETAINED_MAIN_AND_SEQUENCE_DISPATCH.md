# Retained Main and sequence dispatch

This native continuation composes the complete recovered
[Main order/result](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md) and
[manager/outer wrapper](BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md).
It does not acknowledge missing children or install the whole normal battle
loop. Original source addresses/order are checked by
[the read-only verifier](../../../tools/verify_battle_main_dispatch.py).

The uppercase Main cursor captures its own global BattleWork once, retains each
of the eleven ordered child calls, and runs the original final-state checks only
after all eleven complete. The lowercase wrapper first captures its independent
outer owner. Null outer completes without upper entry, child providers or
stores; otherwise upper performs a fresh independent capture. Only upper result0
publishes the end bit to the captured outer owner. The whole source call chain
is retained across unavailable providers, including different outer/inner owners.

SequenceManager independently captures fresh global B and its bank once. Routes
are Init then pinch1; pinch0 then FirstAct; pinch0 then Turn; or End alone.
Other banks perform no handler call. Handler writes do not select another route
mid-invocation, and a held second child never repeats the first. Native boolean
admission is not an original child scalar or the manager's incidental raw r3.

Providers, actual guest memory and their lifecycle observer remain fixed while
held. Each child must itself retain any partial work before returning false.
Wrong entry mode or replaced service tables refuse progress before guest reads.
Reentrant calls and reset during a provider are refused. Explicit reset between
calls discards native continuation metadata; it does not invoke guest cleanup.
These are one-invocation cursors, not render clocks or globally idempotent pass
keys. The caller owns original once-per-simulation-pass scheduling.

The real Trio completion path now uses the retained wrapper and a stable runtime
service context. Its initial physical sample is copied into that invocation;
retrying a later service cannot update the pads again or substitute a new
physical edge. Existing retained display/depth continuations report completion
back to this exact pending display child. Main then owns the final source check
and outer flag publication, rather than a separate manual approximation of its
tail. Existing synchronous audience, stage and action children may partially
mutate before reporting an unavailable provider. The native caller therefore
holds at those boundaries without automatically replaying them; each needs its
own retained continuation before retries can be admitted. Only the existing
retained display/depth completion explicitly resumes this Main invocation.

Host tests compare all eleven wait positions against immediate source execution
for ended and forced-conclusion paths, hold twice without guest prefix replay,
test independent outer/inner owners and reentry, and exercise every sequence
route/wait with changing globals and bank. Actual Trio regression preserves
queued geometry, pose prefix and the captured pad while waiting for depth.
Normal tutorial activation, whole ordinary BattleInit/End and complete original
turn/command/display/camera scheduling remain separate integration work.
