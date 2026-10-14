# Native stage-work storage and counter services

Implements the scalar and inactive/counter projection of the
[original StageInit/Main contract](STAGE_WORK_INITIALIZATION_AND_INACTIVE_COUNTER_PASS.md).
`initializePackedBattleStageWorkStorage` clears the complete b3c work block,
performs the explicit twelve entry/scratch writes, initializes the distinct
active ambient/background records, and copies float literals from original
read-only DOL data. Native trio startup uses this before StageObjectInit;
the former floor-only initializer and hardcoded escape-wall Y write are replaced.

`packedBattleStageWorkMain` retains the captured work address, requests
spotlight Main first, then visits all twelve actual entry flags. Inactive
nonnull scratch calls the supplied BattleFree provider before clearing its
retained field. Active entries request a separate real provider. Four global
counters retain signed admission, wrapping decrement/elapsed arithmetic,
unsigned endpoints, signed mode, fresh duration/remaining reads, interpolation,
fctiwz provider and low-byte output. Callback changes to the global battle owner
do not redirect this pass. Provider suspension preserves prior writes.

Native scratch cleanup binds the live map heap. Spotlight Main calls the
existing packed spotlight provider; active stage entries still report their
own unavailable boundary. Counter interpolation now reuses existing
[cameraInterpolation/intplGetValue](../camera/OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md)
for built-in modes0..13, with the original count0 return before mode dispatch.
Indirect callback-pointer modes remain unavailable at80006c8c; there is no
linear fallback or step/output clamp. Native fctiwz adaptation truncates finite
values in [-2147483648,2147483648), then the original low-byte store wraps them.
An unsupported conversion reports StageMain80139150, preserving the prior
counter decrement and withholding the output write. Gekko numeric parity,
active stage/effect/display completion, complete outer cadence and field return
remain unproven. This binding does not acknowledge those remaining providers.

Host tests verify nonzero prior storage and boundary preservation, original
defaults, inactive/null and last active entry, spotlight-first admission,
free-before-clear with changed global owner, provider failure without preclear,
four-counter ordering and signed mode/endpoints, negative/zero skips, wrapping
elapsed, output low-byte conversion and callback mutation between counters.
Native trio tests inspect the actual mapped original material/position fields
and retain the existing incomplete-engine/field-control assertions.
Additional tests run all four real curve providers for15 ticks (modes0/4/11/12),
checking fixed first/middle/final output bytes and exactly60 curve requests.
Completed counters request no additional curve. An indirect-mode hold preserves
its already decremented counter and its previous output byte. These are packed
counter/provider tests, not native visual fade acceptance.
