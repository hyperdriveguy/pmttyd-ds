# Event service and public-header contracts

The [event headers](../../include/README.md) expose the portable VM's service
boundaries; [VM execution](event-vm.md) covers operand/opcode details and
[addressing](event-addressing.md) covers guest aliases and native backing.
This guide explains interface ownership and result domains, not a replacement
script specification or full native story-completion claim.

`EventNativeRuntime` composes one semantic manager, field/battle contexts,
address facade, memory binding and entry storage. Context tokens are original
`0x803d8d30/0x803d8dd0`; supplied GP backing is initial state, while services resolve
the current pointer on reached calls. Returned closures borrow the runtime and
consult its current platform hooks. Public semantic mappings remain caller-owned
invariants. Entry storage retires before the facade/providers disappear during
normal member destruction; destruction during an outstanding borrow is unsupported.

Platform owns clock, numeric/FPSCR, cache, USER_FUNC and formatting effects.
Declaring extra math hooks in `EventNativePlatform` does not mean the interpreter
bridge forwards or executes all of them. Numeric users and call/format users are
separate; nested bridges must preserve them. Required domains use assertions,
not optional retries; disabled assertions are not a supported missing-memory or
missing-provider fallback. Actual conversion/clock values belong to providers,
not guessed hardware defaults.

| Interface result | Meaning |
| --- | --- |
| Opcode-router `nullopt` | Module does not route that opcode, not a provider hold |
| Engaged handler 0 | Retry current handler |
| Handler 1 / 2 | Complete and yield / complete and continue |
| Handler 3 / 255 / negative | Redispatch / end visit / abort manager pass |
| Dispatcher 0 / -1 / 1 | Yield / end visit / abort pass |
| Registry `invoke` false | No binding; output untouched |
| Registry `dispatch` unbound | Explicit unsupported-domain abort, including release builds |

USER_FUNC caches function and adjusted argument list on first call; later polls
reuse them with firstCall 0. Registry copies callback/user before invocation so
callback-side registry mutation does not invalidate that lookup. Unbind does not
cancel an executing callback or its external retained work. Registry/provider/VM
lifetimes still belong to the caller.

`EventEntry::lifetime` is **accumulated eligible source time**, not an immutable
allocation serial. Manager advances it once per eligible visit; credit may drive
multiple dispatches at the same lifetime. Source ID plus schedule slot rejects
reuse within a collected pass, but IDs may wrap/reset and are not permanent UUIDs.
Any backend requiring allocation-incarnation admission must retain its own actual
owner proof rather than reinterpret this clock accumulator as one.

Creation uses caller-provided available storage, at most 16 labels, original
append-to-live-manager rules and separate root/child/brother relationships.
Semantic pointers borrow arrays; they do not own allocations. Context Init
allocates 256 packed `0x1b0` entries (`0x1b000` bytes) from heap 0 for each context,
then clears work/flags. ReInit clears only selected entries/timestamp plus shared
allocation/active state; work/flags and schedule count remain. Both publish
timestamp low then high and do not retire earlier allocations themselves.

Stop/start visits descendants before owner and mutates flag 2 only when type
intersects mask. By-ID operations use `0xef`; priority/type narrow low8; speed
multiplies current global speed. Context/global recaptures preserve compiler/source
traversal order. These are not memory free operations.

Delete recurses through waiting child and owned brothers, then copies 16 locals,
three flag words and seven message words to a waiting parent, clearing its wait.
Optional native `retiring` observes **after** descendants/copy-back and **before**
entry reset/count decrement. It is read-only observation, not source world work
or resource cancellation. The runtime installs that bridge only when configured
as creation services are built; invocation checks the current platform hook.
Restart preserves ID/locals/links while resetting instruction/lifetime/stack/label
progress. Source deletion, mapping retirement and heap free are different owners.

Operand headers keep saved GP byte/flag namespaces distinct from context GW/GF
and entry LW/LF. GSW0 is the full story word; other saved bytes narrow low8.
PowerPC shift masks use low6 count bits, with 32..63 yielding zero. Integer/float
read/store bands are deliberately asymmetric; the VM guide retains their numeric
tables. Do not combine all access into a generic typed-variable lookup.

Memory helpers operate on original-address headers through actual services.
SDK descriptors stride 12, node/payload offset 32, and allocation rounds
`(bytes+63)&~31`; wrapper clears/flushes requested bytes only. Map sizes exclude
32-byte headers, SDK spans include them, so their coalescing arithmetic differs.
Map null free is a no-op; SDK free requires a valid payload/handle. SDK Init returns
usable address, Create returns handle/-1. Five configured pools use fixed KiB or
percentages of one captured remainder. Nested battle map pool is `0x64000` bytes.
Semantic entry retirement is not implied by any raw allocator operation.

Requested system scalar 0..4 at `0x8041e940` differs from GP `+0x18` inhibit mask.
Setter ignores invalid requests, changes low-nibble gates and requests source
stop/start masks. Clock publication samples upper/lower/upper until consistent,
then preserves source GP recaptures and low-then-high stores through pause/window/
system gates. It implements a retrace clock prefix, not hardware tail or renderer
frame advancement. Zero-length cache flush performs no syscall; nonzero source
ranges require actual reached cache services.

`PMEVT001` is a native container with BE aligned/sorted/nonoverlapping segment
triples. Its original payload bytes remain intact; borrowed segment views last
until successful reload/destruction. Writable VM import copies bytes, preserving
the asset owner. Original IDs/control tokens/conditions stay authoritative.
Diagnostic sprintf destination `0x803d8430` is shared guest scratch, not dialogue
or a host string sink; the formatter owns the reached request's varargs behavior.
