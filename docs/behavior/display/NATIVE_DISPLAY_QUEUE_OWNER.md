# Native display queue ownership and deferred dispatch

The portable owner retains1024 actual16-byte records, sort references, active
count and current-record identity. Callback/userdata are source identities;
the native dispatcher resolves them through typed providers, never casts PPC
function words into ARM pointers. Userdata remains borrowed and entry invokes
no callback. Source authority: [records/dispatch](DISPLAY_QUEUE_AND_DISPATCH.md),
[sort](DISPLAY_SORT_IMPLEMENTATION.md) and
[ordinary battle admission](ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md).

Entry preserves padding, narrows camera/class bytes after validating the
supported native class domain, applies all11 original offsets/directions and
rejects NaN before writes. Infinity and signed zero are admitted. Capacity or
unsupported class requests return unavailable; they are not silently dropped
as completed original services. Count-only reset leaves storage, references
and current identity intact; registration can overwrite a record still exposed
by current-work identity.

Sorting follows recovered small-count pivot branches, large strided sample,
partition zero-comparison/pivot movement and smaller-side recursion/tail
iteration. Native slot indices replace original pointers. The resulting slot
permutation is applied with a separate native copy rather than original global
cycle scratch. This is an equivalent private, nonaliased size4-reference
adapter with a pure record-key comparator, not arbitrary qqsort/copy/reentrant
global scratch support. Equal keys have no registration-index tie breaker.

Dispatch filters stored camera byte against the full draw argument. Common
and class-specific GX requests are emitted in source order through typed
services; classes6..8 inherit state after common requests. Callback invocation
publishes current identity and passes camera plus borrowed userdata. Native
unavailability retains a cursor rather than replaying already completed state
requests. Captured callback arguments survive count reset until that invocation
completes; nested current-work changes are not restored. After callback the
loop reloads live count/references, admitting same-traversal appended records
and count-only reset. Repeated completed draw calls restart traversal normally.

This module does not map GX enums to PICA, supply original camera/reset/draw
cadence, acquire userdata lifetime, implement queued part/shadow/message
callbacks or complete the trio display footer. It is a real queue service
ready for those owners; the current trio still waits on inherited camera4
matrix ownership before its first ordinary pose registration. Deferred callback
execution remains distinct from immediate source continuation.

Tests cover all ternary key inputs through length7, repeated/all-equal keys at
small/large/sample thresholds through1024, the source three-entry tie reversal,
record admission/key transforms, callback suspension, live append, reset/current
identity and full-camera-word matching. Ascending/permutation checks do not
prove original instruction execution, every anomalous comparator domain or
visible compositing. Exact source contracts remain the tie/order authority.
