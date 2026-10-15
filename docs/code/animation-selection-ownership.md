# Retained animation selection and native ownership

[The portable setter](../../source/display/animation_selection.cpp) executes the
original selection/restart graph over raw guest words. Its
[native process adapter](../../source/display/animation_native_selection.cpp)
adds actual pool/resource/scratch identity and producer exclusion. See
[memory ownership](animation-memory-ownership.md) and the
[source contract](../behavior/display/NATIVE_RETAINED_ANIMATION_SELECTION.md).
Offsets/masks below are hexadecimal; capacities and budgets are decimal.

The setter captures W, pose address using handle stride `0x170`, model record
index/stride 16, FileEntry+`0xa0` holder, actual MODEL and signed descriptor count
`+0x148`. Raw arithmetic wraps 32 bits. High-bit selectors are signed-negative
source name addresses; nonnegative selectors are indices. Name scan uses the
first equal 64-byte descriptor slot, reloading MODEL+`0x1ac` for every comparison
while retaining count. Miss, out-of-range or nonpositive count selects row zero.
No portable handle, resource or size clamp is introduced.

Force zero alone can return early when P+`0x14` already matches. A changed/forced
selection stores that field first, then checks link P+`0x90`. A non-minus-one link
and captured flags mask 2 admit OR 4; no linked-record validation or traversal
is implied. Main and geometry evaluation are separate operations.

Clock reads are mode, hardware bus `0x800000f8`, GP pointer, then captured high/low
pair: GP+`0x38/3c` for nonzero mode, `0x40/44` for zero. Original reciprocal
`0x10624dd3` multiply-high/shift derives quarter-bus ticks per millisecond; actual
signed-64 division is a child. Low timestamp P+`0x1c` is stored first, then the
zero literal `0x8041fb28` is read, then high timestamp P+`0x18`. Cache index -1,
fraction zero, discrete zero and ratio from the same captured zero follow in
source order. A held provider must not replay or collapse these boundaries.

Each native phase consumes one visit, even an internal branch. Budget exhaustion
returns incomplete after any committed prefix; zero budget starts nothing.
This is scheduling policy, not original instruction timing or a render frame.
Resume the same cursor/arguments/services under exclusive producer ownership.
The public portable cursor is not a reentry lock or a lifetime token, and clearing
it does not roll back source stores.

Native fresh admission requires handle <256, a nonzero native generation and
actual resource/source-buffer owners. Each primitive rechecks that process,
VM alias, generation, resource and scratch identity remain current. It does not
adopt a replacement namespace at the same numeric handle. This adapter has no
independent sticky-stale flag; current identity checks determine availability.

The native strcmp adapter compares unsigned bytes and preserves a captured left
byte if the right is unavailable. Its 256-iteration per-call budget does not cap
accepted string length. It does not emulate SDK optimized word loads/overreads.
The divide callback uses the existing native signed clock policy and can hold
on an unsupported divisor.

Entry, Main and unrelated/later presentation phases exclude this setter. A
same-handle presentation at phase zero may perform its own setup selection;
there is no independent pass token to distinguish competing callers there.
The reserved setup remains exclusive through waits. Only complete selection
releases held resource/buffer references. Selection alone never publishes a draw,
updates GPU residency or proves complete original animation scheduling.
