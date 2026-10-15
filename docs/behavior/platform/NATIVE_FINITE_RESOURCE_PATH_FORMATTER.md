# Finite native Font/Window path formatter

`NativeSourceResourcePathFormatter` is a bounded native C-runtime adapter for
three actual resource path calls. It is **not** the original whole sprintf,
varargs, parser, writer or instruction-read/write trace implementation. It
neither normalizes resource paths nor supplies missing roots/files/ready flags.
The actual [DVD-root leaf](ORIGINAL_MARIO_DVD_ROOT.md) returns a source pointer;
this adapter separately reads its real current source bytes.

## Admitted actual requests

| Original caller/site | Actual format address and bytes | Captured inputs |
| --- | --- | --- |
| Font selected `80075048` | `802c2ba0`, `%s/f/%s.bfn` | actual root and name pointers |
| Font probe `80075094` | `802c2bac`, `%s/font/%s.bfn` | actual root and name pointers |
| Window `8007fc30` | `802c3094`, `%s/f/msgWindow.tpl` | actual root pointer |

All target original `sprintf8026a0b8`. Font requests must have exactly three
integer arguments, no reals/path, and their actual nonnull `formatDestination`.
Window requests require count2, original format/root arguments, no extra typed
argument payload/path and their actual nonnull `destination`. The original
source parents own those private `SourceFontPath`/`SourceWindowTexturePath`
objects and retain their identity while the child holds. The adapter verifies
both format address and actual format bytes. It never replaces captured names
by re-reading the parent's table or choosing a language.

## Actual source evidence and finite port boundary

The source wrapper at `8026a0b8/e0` constructs the original varargs/save area and
calls `__pformatter8026a454/774` at `8026a15c`. The formatter uses actual
`__StringWrite8026a390/6c`, parser and vararg helpers. StringWrite invokes
`memcpy8000519c`, with source count/wrap behavior; the wrapper writes its NUL
using the returned count. In particular source result-1 writes at destination-2.
See [the actual output/ABI contract](../format/FORMAT_OUTPUT_RUNTIME.md).

For these exact ordinary narrow `%s` templates, proven immutable nonnull input
C strings and an isolated private output whose entire native domain fits, a
real `std::snprintf` produces the same finite bytes/count/NUL. It operates on
owned byte copies of actual source strings and the validated actual format.
No general format grammar, floating varargs, width, precision, flags, wide
strings, numeric conversions, `%n`, negative result, source count overflow or
guest stack aliasing is acknowledged. Source-null `%s` substitutes an empty
original sentinel at `80303bb0`; this adapter deliberately **holds** that domain
instead of using host libc's null behavior or pretending to implement it.

Binding requires strong actual process/VM/parent context lifetime, a pure long
current predicate, optional-safe source byte reads, immutable-input proofs and
private-destination proofs. Those proofs must describe actual owners, not
infer them from coincidentally readable/equal bytes. Owned unavailable source
domains get first refusal; no fallback zero or hardcoded `.`/name is permitted.
The input proof covers the actual immutable data span throughout a hold. It
must reject replacement/mutation; partial native admission is not a model of
the original formatter's mutable-memory alias/read order.

`stringBudget` and `outputCapacity` include NUL and are explicit native admission
policy in1..INT_MAX, **not** original array sizes or measured source limits. The
caller must prove sufficient isolated private storage and disjoint input/output
ownership; no guessed guest SP or universal source stack-buffer extent is
created. Unknown input bytes, missing proofs, unterminated strings, address
wrap, insufficient output capacity or unsupported domain remain holds before
target writes. No truncation is published or converted into formatter success.

## Retention, exact private bytes and atomic commit

A held invocation pins actual kind/site/arguments/destination/map identity.
Successfully admitted source bytes are captured once; an unavailable next byte
resumes at that address. Replacement requests or destinations are rejected.
Callback/service bindings are immutable once bound. Reentry/reset inside a
callback is rejected, and observed actual current loss/retirement is sticky.

After all actual inputs and ownership proofs succeed, compute the finite output
extent from those exact `%s` templates and byte lengths. Run real native
`std::snprintf` into an exactly sized temporary buffer; require its nonnegative
result to equal that extent without truncation. Prepare a candidate copy of the
actual private offset-cell map and overwrite exactly result bytes plus NUL.
Every preexisting suffix cell remains unchanged; missing suffix cells remain
unknown. No target zero-fill, guessed suffix or synthetic guest buffer exists.

All allocations, native formatting and callbacks precede final target mutation.
Final input/destination/current checks must still succeed, then a nonfailing
map swap commits the whole private result. It returns the actual native count
as an engaged source result. No original guest writer-prefix effects are
claimed for an unavailable domain. The caller owns serialized execution and
must keep parent/destination/context alive, including through retire callbacks;
the bound lifetime must pin their real storage. Callbacks are pure ownership
queries and cannot destroy/mutate inputs or the output during atomic admission.

Completion consumes this child state. The original parent advances its own
phase, so a later DVD Open hold cannot repeat the root/format child. `reset`
does not revive a stale owner or undo committed private results and may only
follow the caller's genuine cancellation/new-invocation rules.

## Integration and remaining owners

At the real Font/Window Setup request provider, route only admitted actual
`8026a0b8` requests into the corresponding `request` overload and propagate its
optional result. Keep this owner, source reader and actual parent-private
buffers stable through holds. Getter `80007388` remains its own exact leaf,
with actual initialized source ABI; file/archive/DVD/heap/GX/object/async
callback owners remain separate. Existing menu-cache/animation formatters
admit other domains and are not whole original printf substitutes here.

No production binding or source startup is installed by this module. Genuine
process initialization and source data mapping, actual seq_logoExit/manager
scheduling, full font/window Setup history, native resource/GPU residency and
normal battle activation remain required. No input/control behavior, original
dialogue, asset conversion or source global ready flag is changed.

## Validation

Tests read original root/formats/US/JPN/EU names and table bytes from the
read-only ISO. Each source input-byte boundary is held twice against immediate
results, with no private destination prefix write on a hold. Tests cover raw
bytes, exact count/NUL, reused-buffer suffix preservation, unknown source bytes,
null/alias/budget/overflow/format-domain holds, exact capacity, changed request
and destination, reentry and sticky retirement before commit. Actual retained
Font and Window parents call the real root leaf and this formatter once, then
hold three times at genuinely unavailable DVD Open; explicit isolated entry/
archive conditions do not establish process/full Setup readiness.

The verifier pins seven whole original runtime bodies plus root/ABI/UI caller
and font-data evidence. These are source references for the finite equivalence
domain, not proof that those whole original runtime functions were reimplemented.
Strict host, ASan/UBSan and ARM checks do not establish Gekko ABI/FPSCR, general
printf, hardware rendering or normal startup parity. Root owns combined gates.
