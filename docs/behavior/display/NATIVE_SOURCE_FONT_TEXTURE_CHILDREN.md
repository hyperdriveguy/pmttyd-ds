# Actual mapped font Setup texture children

`NativeSourceFontTexture` binds the [whole original CPU texture constructors](RETAINED_SOURCE_GX_TEXTURE_OBJECT.md)
to [shared process font memory](NATIVE_SOURCE_FONT_RESOURCE_OWNERSHIP.md) and an
actual `EventNativeRuntime`. It owns the real `NativeSourceFontBinding`; an
already-bound VM cannot be silently adopted or replaced. Binding requires all
actual source state spans, preserving their existing bytes. No constructor
initializes source BSS, chooses a language/heap pointer, or creates an SDK object.

## Actual services and source boundaries

The owner accepts only original font Setup's Init call at `800751b8` and LOD
call at `800751e0`. It verifies current source BFN/GLY identities, the original
66560-byte finite-US file extent and512x256 layout, current GLY image address,
static object `8041e97c`, exact argument sequences and source zero bits from
`804203f8`. An actual contiguous mapped BFN prefix and its known last byte prove
a complete live file view. The CPU memory owner retains its actual native heap
allocation. These are native admission limits; they do not add guards to the
portable original source bodies or claim full/general BFN support.

Source reads claim shared process-owned bytes first, including unavailable
owned storage. Other reads use the real VM facade. Missing original SDK scalar,
format/min-filter table or conversion services hold the retained source child;
no embedded table, guessed zero or default successful callback fills the gap.
The actual platform `truncate` callback is invoked only for finite representable
int32 operands. Exceptional numeric domains remain unavailable.

The linked original memset child first admits the whole aligned32-byte object
as actual raw RAM and checks the shared bytes against the VM view. It then runs
existing `eventFillMemory(object,0,32,actualMemoryServices)` under a synchronous
RAM borrow: eight original ordered word stores, no source callback/yield inside
that borrow. Object updates use actual `writeByte`/`writeHalf`/`writeWord` on the
bound VM; in particular, original STH at `8041e998` reaches shared process
storage. Completed stores are checked against that same owner rather than an
independent descriptor. Source-current checks bracket actual child services.

A pending request pins its exact operands and original scalar bits. Changed
requests, unsupported call sites and attempts to interleave another child are
rejected. Only complete source execution returns engaged zero for the actual
void SDK child. A completed child is never acknowledged twice. Init must finish
before LOD; the owner represents this one original Setup pair, not arbitrary
texture management. Source allocation/pointer loss is sticky stale.

## CPU proof and lifetime

`object()` is unavailable until both actual source SDK children finish. It then
returns a copied description of the source BFN/GLY/image identity, dimensions
and actual32 object bytes. The real shared allocation is retained by the memory
owner. Any later object-byte or BFN/GLY identity change permanently invalidates
this proof, including restoration to identical bytes after observed loss.
The query does not invent a complete record from preexisting object bytes.

This is a CPU object-production proof only. LOD returns before original Setup's
width7 then9 writes. The owner neither calls nor acknowledges whole Setup and
cannot prove root/DVD/catalog/heap services, actual imported atlas pixels,
resident GPU generation, camera/frame ownership or native draw submission.
`BattleFontGxState.staticTextureCurrent` still needs those actual native owners
in addition to this CPU proof. Font source image mutation/import correspondence
remains the real source/native texture owner's responsibility; the CPU query
does not hash/certify live texture pixels or a T3X import.

The external strong lifetime must pin the actual VM/context and numeric callback
userdata. `current` is a pure long allocation/history predicate, independent of
short source-camera execution. The owner retains the actual binding before
releasing that lifetime, including when it is the sole VM owner. Explicit retire
cancels source work and invalidates proof before detaching. Retirement requested
inside a source child defers destruction until the invocation unwinds.

When using an embedded VM, retire/destroy this owner before that VM's destruction,
outside RAM borrows. The caller must not alter owned external mappings or
semantic views while bound. Process CPU font/heap history survives detachment;
only that VM's earlier private spans are restored. Original SDK/GX functions do
not perform such port detach work. The process setup owner should survive the
original font allocation and own its source VM; creating a fresh owner per
battle cannot manufacture provenance for already-initialized object history.
Cross-VM production-proof transfer/adoption is not implemented by byte matching.

## Concrete binding and remaining frontier

Replace the relevant process VM's direct `processFont->bind(vm.memory)` with
`NativeSourceFontTexture::bind(vm,processFont,actualContextLifetime,current)` and
retain the result with correct VM destruction order. Route only the two typed
SDK requests from actual `SourceFontSetupServices.request` into `request(q)`;
all other original source requests retain their genuine linked providers.
A missing owner returns unavailable. Do not call this from a renderer readiness
poll or synthesize successful root/probe/DVD/heap requests to reach its children.

Current Trio VM construction does not supply this complete cold process-font
state/root/DVD/heap path. No new Trio/Renderer hook is activated here. Missing
true initialized SDK spans or numeric callbacks remain visible source waits.
The existing process/cold-memory, OS/catalog/heap and native glyph/GPU ownership
frontiers must be closed before normal UI activation.

## Validation and provenance

Tests read the actual US BFN and initialized DOL sections from the read-only ISO,
with real DOL BSS zero semantics only where no initialized section covers a cell.
They exercise real shared VM mapping, original Init/LOD, halfword aliases into a
second VM, source-table/numeric waits, captured-word alias behavior, interleaved
request rejection, sticky source/object loss, true VM lifetime pinning and
retirement inside numeric execution. An isolated whole Setup test uses clearly
fixture-only path/catalog/heap/DVD providers; its Read installs actual original
bytes at that call, and its SDK requests execute this real retained owner. It
proves composition plus actual final width9, not native OS/catalog readiness.

Focused strict host, sanitizer and GNU ARM syntax checks use a private consistent
header/library snapshot. The existing whole Setup/SDK source verifiers also run.
No original asset export/conversion, original pixel comparison, normal queued
activation or full-build claim is made. No shared source or frozen batch is
changed. Original Nintendo/Intelligent Systems rights remain; no ISO, original
binary asset or compiled pack is committed.
