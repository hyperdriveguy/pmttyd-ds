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

Native scratch cleanup binds the live map heap, not a fabricated successful
free. Native spotlight, active entry and positive interpolation requests remain
explicit unavailable services; the ending still holds at8013800c before any
entry/counter work runs. This does not claim spotlight initialization, active
effect/display completion, original numeric curves, whole-engine success or
field return. These services must be bound before the initializer's two child
calls and complete original StageMain can be claimed operational.

Host tests verify nonzero prior storage and boundary preservation, original
defaults, inactive/null and last active entry, spotlight-first admission,
free-before-clear with changed global owner, provider failure without preclear,
four-counter ordering and signed mode/endpoints, negative/zero skips, wrapping
elapsed, output low-byte conversion and callback mutation between counters.
Native trio tests inspect the actual mapped original material/position fields
and retain the existing incomplete-engine/field-control assertions.
