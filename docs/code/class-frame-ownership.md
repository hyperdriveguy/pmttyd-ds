# Retained class frame, native admission and renderer ownership

[The class pipeline](../../source/display/battle_class_frame_pipeline.cpp)
retains source work and publishes copied geometry.
[The native frame](../../source/display/battle_class_native_frame.cpp) adds the
actual queued camera epoch and optional native submission.
[The renderer binding](../../source/display/render_battle_class_runtime.cpp)
retains their concrete resource and callback contexts. Each layer has a distinct
completion boundary; an individual backend append cannot complete all three.

## The pipeline retains one source prefix

Bind copies the display/raster service tables and retains a pointer to the
actual backend, plus the asset/font identities that may not be replaced while
bound. Wrapped callbacks use the pipeline State as userdata but forward through
the original display, draw, raster and math contexts independently. Sharing an
operation name does not imply those contexts are interchangeable.

Each wrapped provider checks allocation and backend identities before and after
execution. Observed loss invalidates the invocation even if values later return.
Only the Camera raster request routes to the supplied external camera owner;
finite GX/texture/font requests route to the bound backend. The backend's atomic
FontString contract must reject unsupported work before RNG/geometry effects.
Footer providers remain real retained children.

A new monotonic key resets the mutable backend once. Retrying an active key
requires identical camera/battle operands and preserves the accepted prefix.
The complete queued source return sets sourceComplete; later publication holds
do not rerun source lookups, RNG or footers. Publication requires complete
backend state too. A previously completed key with the same operands is an
idempotent acknowledgment, not another source draw.

Pipeline retirement hides publication immediately. During a provider call it
marks invalidation and defers storage destruction until the guard exits. It
neither calls original UI End nor changes source readiness. The stable empty
publication facade prevents old drawable state escaping after ownership loss.

## Native frame pins the short dispatch epoch

Source Camera8 and original callback 0x8012210c must match the invoking display
record. The shared bridge additionally verifies successful Main, Draw/queue
membership and reset epoch. A held pass fixes submission userdata and function
identity as well as source pass and camera ownership. Camera services are
available only during the guarded resume call; callers cannot use the borrowed
bridge between retries or after finish.

The copied source batch can become complete while native submission is not
ready. False from the submission callback retains source completion and the
camera lease. Retry reaches the same batch without executing the source again.
Once submission returns true, the frame records submitted before subsequent
liveness, admission and finish guards. A later guard failure cannot replay
already accepted GPU work.

Submission is optional in this API. Without it, successful source completion
can finish the camera lease and expose a batch for later external rendering/
admission. Production Renderer supplies an actual draw callback. Admission is
metadata consumed once by simulation; it never fabricates original readiness.

The outer publication accessor exposes only a sequence that completed this
native frame. If a new source batch has published internally but native submit/
finish is still pending, that new sequence remains hidden. Do not infer outer
completion merely from the inner pipeline's copied batch. Finish clears the
short lease before Draw advances; long allocation-current publication proofs
remain independent. Allocation loss is sticky at the native-frame layer.

## Renderer resources and callback storage stay pinned

ClassMenuBinding requires the exact font generation and import identities,
message-icon import, class surface pair and footer generation. Equivalent
pixels or reused texture addresses do not establish those lifetimes. Missing
sound or other genuine source children remain retained waits; a vector append
does not acknowledge them.

Provider closures use weak self references, while frame.bind's resource pin
deliberately retains the outer binding. Since that binding owns the frame,
explicit release must mark nativeAlive false and retire the frame to break the
pin before GPU resources disappear. Retirement during an invocation defers
context destruction, keeping numeric/footer userdata alive until return.

A pending attempt under the same battle incarnation cannot be replaced with a
fresh resource context merely because current readiness fails; doing so would
restart accepted source prefixes. prepareClassMenuFrame preserves that hold
until the actual lifetime/retirement policy permits replacement.

At the first retained attempt, sourceTick captures Game's simulation tick and
the icon metadata map clears. Existing animation lookup copies each requested
MessageIconFrame at that pinned tick. std::map nodes keep returned addresses
stable across later inserts. Source/GPU holds do not advance this animation by
render-frame count; only a completed attempt clears sourcePending for the next
one. This describes the existing native playback policy, not newly measured
original timing.

The submit callback waits for classMenuSubmitReady before calling drawClassMenu.
The wait retains source/camera state; source scheduling, resource preparation
and the actual submission phase remain their owners' jobs. These modules do
not prove complete command activation, original numeric/raster parity or
hardware timing. See [the cold pipeline contract](../behavior/display/NATIVE_COLD_CLASS_FRAME_PIPELINE.md),
[camera/numeric admission](battle-camera-numeric.md) and
[publication rules](battle-publications.md).
