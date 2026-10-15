# Cold native UI startup ownership

[NativeUiStartup](../../source/platform/native_ui_startup.cpp) binds the original
font-manager prefix to one actual field-animation process. See
[source font loading](source-font-loading.md) and [text/cost owners](battle-ui-text-cost.md).
This owns CPU/source startup, not whole logo exit, Window async readiness,
Camera Main, a display queue invocation or native GPU residency.

## Cold source image and lifetime

Ten exact address/byte-length/FNV-1a-64 records cover font literals, SDK format
and filter tables, palette/spacing/name pointers, font globals, root string,
draw/texture scalars, fog and selected color. FNV offset basis
14695981039346656037 and prime 1099511628211 hash every original byte. The
exporter independently records SHA-256 and initialized-data/BSS provenance.
Every range must be unmapped. Partial installation removes only newly registered
ranges; a failed factory rolls back its image. Successful mappings become process
history and remain after wrapper retirement, preventing a second cold import.

Cold admission checks all 256 precreated pose-row objects for zero flags,
resource and generation, plus no held Entry/Release. Pointer presence alone is
not allocation. The actual GP pointer at `0x80415fd0` is captured; a mismatch
stales the owner. Any nonzero font pointer at `0x8041e978` must equal its actual
captured heap. Zero alone does not prove whole font setup. Retire rejects further
admission without performing a guest heap free or source reset.

Native phases are 0 (original US GP contribution), 1 (whole retained FontManager)
and 2 (completed texture-object description). Phase 0 stores language 1 at actual
GP+`0x16c` once. Completed resumes do not repeat that store or cold admission;
`language()` reads later real process writes. The wrapper/state stay pinned
through reentrant admission callbacks. Game room/checkpoint retention must keep
this same process owner, not copy a default language into another process.

## Finite original path and DVD producer

The formatter admits only exact immutable literal/root ranges, checked against
live original bytes, and its exact private 256-byte Font destination. The real
US FST has no `font/` entries. Only the three exact `./font/papermarioset_*`
probes complete with catalog miss zero; unknown paths remain unavailable.
Selected `./f/papermarioset_US.bfn` opens the staged original raw file read-only.
The earlier immutable asset preflight did not perform this source DVD delivery.

Native records are 152 bytes (`0x98`), from candidate base `0x60030000`, stride
152, up to 256 slots, each checked against actual VM mappings. They are native
address policy, not original heap or ISO pointers. Fields are filename +0,
length +`0x74`, destination +`0x7c`, request length +`0x80`, offset +`0x84`,
callback +`0x8c`, flags +`0x90` and low-byte open mode in half +`0x92`.
Flags begin occupied `0x8000`; read owns busy/completed bits 1/2, close marks 8.
The finite adapter records its delivered byte count at +`0x88`; that does not
claim the whole original worker's semantics for this field.

Read captures exact arguments and updates the backed entry, performs one actual
`fread`, and compares every observed byte with immutable original input before
source delivery. A delivery gate can hold after the read; retry reuses observed
bytes. After actual delivery, destination advances, remaining length/offset clear,
count/flags become coherent, and the child returns the original length field.
Close closes the real FILE but leaves its mapped record until native retirement.
This is a finite producer, not full DVD scheduler/hardware emulation.

## Source heap and transfer

Allocation runs actual source heap 0 in the same VM, including allocator header,
list, clear and flush writes. Transfer waits retain that allocation. Only its
exact payload is detached into strongly shared font backing; headers remain at
their source addresses. Failed alias admission restores captured RAM when possible
and never reallocates the prefix. A successful allocation result zero remains a
source result, not fabricated provider readiness.

Font setup publishes its pointer and performs actual delivered bytes, glyph-zero
width mutations and SDK texture constructors under the existing retained owners.
A source pointer publication alone does not expose `fontObject` or `fontMemory`;
whole setup and the actual object must complete. Raw BFN input remains unchanged.
Real byte delivery, mutable VM aliases and resource history remain separate from
later native texture import and queued drawing. See the
[startup contract](../behavior/display/NATIVE_UI_STARTUP_FONT_PREFIX.md).
