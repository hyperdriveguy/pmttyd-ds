# Native type5 model pointer relocation

The native file service now implements the complete local type5 Set and Clear
blocks through EventMemoryServices. These handlers are prerequisites for actual
loaded animation/model graphs; they do not initialize files or acknowledge a
header-only model fixture as a complete loaded resource.

## Source and service boundary

US G8ME01 revision0 original blocks8018ac80..8018ae54 (Set) and
8018b320..8018b4f0 (Clear), within fileGarbageDataAdrSet/Clear.
See [original setup](FILE_TYPE5_NESTED_POINTER_SETUP.md),
[original clear](FILE_TYPE5_NESTED_POINTER_CLEAR.md) and
[dispatch](FILE_RELOCATION_TYPE_DISPATCH.md). The read-only verifier pins the
complete original bodies and nested type5 anchors. This is static binary
evidence; native tests do not constitute original runtime observations.

setPackedFileModelPointers and clearPackedFileModelPointers receive a resolved
payload token. The caller must perform the original dispatch/type/file-entry/
holder resolution separately and provide every writable address required by
the graph. Synchronous memory-provider failure remains the provider's explicit
domain policy; no missing read becomes0 or a successful partial relocation.
No heap allocation, native-pointer cast, file acquire/free, callback yield,
cache flush, signature/range/size check or source-absent cleanup is added.

## Operation order

Set captures first root14c, skips all work when it is unsigned>=base, otherwise
converts that captured word followed by fresh words150..1ac ascending. It then
captures signed count148. Nonpositive count exits after root conversion.
Positive count executes exactly that captured number of64-byte records using
full-word wrapping offsets. Each iteration reads current table1ac, retains the
parent slot, adds the root base to that slot, then RELOADS table/child. If first
child24 is unsigned<child, it converts the captured first value then fresh
28..40 fields relative to the CHILD base, in order. Parent0 becomes root base;
child zero fields are not preserved specially. Shared children naturally skip
already-relocated fields through the original guard.

Clear skips everything when first root14c is unsigned<base. Otherwise it
captures signed count, traverses admitted records before altering root pointers,
and retains absolute child C before publishing parent C-base. Child first24
unsigned>=C admits subtraction; all child fields use retained C despite a parent
store changing the live table. Finally all25 root words14c..1ac are freshly
read/subtracted in order, including first14c after child aliases and table1ac
last. Count<=0 still clears roots. Arithmetic wraps32; there is no visited map,
count clamp, null-child shortcut or interchange of Set/Clear capture boundaries.

## Validation and remaining integration

The fixture independently checks full original Mario and Goombella model root,
record and embedded child pointer values after Set, then demands exact complete
model-byte restoration after Clear and guard idempotence on repeated calls.
Original cached draw-model payloads remain private/unchanged. Authored cases
cover unsigned root guards, negative/zero captured counts, count mutation during
parent stores, shared children, explicit root/table/child aliases, Set fresh
child lookup versus Clear retained C, late root-first reread, zero fields,
wrapping arithmetic and child guard equality. These authored malformed/alias
cases are service-order evidence, not claims about ordinary original histories.

This local handler does not activate normal source file loading or cold animation
startup. Source file pool/archive/DVD adaptation, genuine file/holder/payload
lifetime and typed metadata synchronization, memory/movement ownership, actual
live tangent/output bindings and startup before Mario entries remain required.
No complete prologue, original renderer or physical3DS performance claim.
