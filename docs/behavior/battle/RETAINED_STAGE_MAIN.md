# Retained stage main children

`battle_stage_main_continuation` supplies independent retained callers for
StageObjectMain (`80149798/438`) and StageMain (`80139150/630`). Existing
synchronous implementations remain the reference. Source evidence and domains:
[stage object cache and map requests](SETUP4_STAGE_OBJECT_CACHE_AND_NAMED_MAP_REQUESTS.md),
[stage work initialization and counters](STAGE_WORK_INITIALIZATION_AND_INACTIVE_COUNTER_PASS.md).

Each cursor captures its own original global BattleWork when first entered.
A held child retains its exact arguments; resumption does not repeat earlier
reads, cache writes, counter decrements, successful callbacks, or siblings. A
completed cursor starts a new source invocation on its next call. Memory and
provider owners must outlive held calls. Callback user/function identities,
complete actor memory services, and lifecycle observer identities must remain
stable. A mismatch refuses progress. Recursive resume and reset while a provider
executes refuse progress. Providers themselves retain any partial work inside a
single child; their false response cannot acknowledge successful completion.

The stage-object cursor visits all 32 original records. It preserves signed
ordinal admission, float comparisons, copied raw cache words, dirty-bit clearing,
shared layer-2 orientation cache, and fresh layer reread. It retains local dirty
admission across waits after source caches have already committed. Initial
translation, negative pivot translation, rotation, positive pivot translation,
visibility lookup, and visibility update have separate continuation boundaries.
The name and transform inputs are captured at each actual call, with fresh
subsequent names/pivots after a preceding callback completes. A visibility lookup
result and original hidden decision survive the later flag-update wait.

The stage-work cursor captures work before spotlight. Spotlight completes once,
then each original active entry or inactive scratch release completes once.
Scratch pointers clear only after free succeeds. The four signed positive
counters decrement once at their original source point. Signed low-byte mode,
unsigned endpoints, fresh duration and remaining count are captured before the
interpolation boundary. A successful interpolated value survives a truncate
wait; the output byte is written only after successful truncation. No numeric
approximation or fallback is supplied by this module.

Tests compare complete guest read/write and successful provider traces, and
final memory, against both synchronous reference bodies. Every external call
position is made unavailable twice, including late pivot/rotation/visibility,
spotlight, active entry, scratch release, all interpolators and truncations.
Successful map providers also mutate the global battle pointer, names and pivots
in a differential variant: the work owner stays captured while later operand
reads remain fresh. Tests additionally mutate counter inputs while interpolation
is held, verify retained operands and one decrement, reject service replacement,
check null numeric service prefixes, and attempt recursive resume/reset.

This is a source-child continuation, not normal battle activation or a claim
that map, active-entry, spotlight, or numeric providers are available. Native
Trio/Main callers must retain these cursors and actually resume the child they
started. The module does not change synchronous callers or install itself in
simulation or rendering.
