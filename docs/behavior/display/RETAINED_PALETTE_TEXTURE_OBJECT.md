# Retained original palette-to-GX object producer

`source_palette_texture_object` implements the complete US revision-0
`TEXGetGXTexObjFromPalette80037a80/dc` CPU body. Source arguments are palette
`r3`, supplied output-object address `r4`, and original index `r5`. It composes
the complete retained [GX Init and LOD bodies](RETAINED_SOURCE_GX_TEXTURE_OBJECT.md),
not opaque constructor acknowledgments. It does not import native textures,
submit GPU work, publish a source ready byte, or activate a menu callback.

The read-only verifier pins the whole body SHA256
`7c513078ebe80f85256108702b3b74d65b9036b6a1115b88c8a3750d19819fe6`,
its two linked children and original unsigned-conversion constant. It also pins
the real common battle TPL and original message-window TPL file identities and
used headers. The existing GX verifier independently pins both SDK bodies,
linked memset, initialized SDK tables/scalars and original r2/r13 values.
Original game/SDK/asset rights remain with their owners; no new asset is added.

## Exact ordered source operations

The source captures `table=word(palette+8)` at `80037aa0` and the wrapped 32-bit
`index<<3` at entry. It reads `header=word(table+offset)` at `80037aa4`, unsigned
bytes `header+33/+34` at `80037aa8/aac`, then image word+8, width half+2,
height half+0, format word+4, wrapS word+12 and wrapT word+16 at
`80037ac4/acc/ad0/ad4/ad8/adc`. The first byte comparison alone supplies
`mipmap=(min!=max)`. It invokes actual `GXInitTexObj802b72b0` at `80037ae0`.
No palette count check, index clamp or metadata fallback exists in this body.

Only after Init completes, it reloads `header=word(capturedTable+capturedOffset)`
at `80037ae4`. It does **not** reload palette+8. It reads fresh unsigned min/max
bytes at `80037af4/afc`, loads the original double at `802c0140/144` with the
single source `lfd80037b0c`, and performs min's `fsubs80037b1c`. Filter words+20
and+24 follow at `80037b20/b28`, then raw bias float word+28 at `80037b30`,
max's `fsubs80037b34`, and edge byte+32 at `80037b38`. Actual
`GXInitTexObjLOD802b7544` follows at `80037b3c`, with literal biasClamp0 and
anisotropy0. The original helper's return is void; completion proves only that
these actual CPU operations completed.

The unsigned conversions construct private raw binary64 values
`0x4330000000000000 | byte` and subtract the actual captured source double,
rounding to binary32 at each `fsubs`. With the pinned original constant, bytes
128..255 stay positive. The numeric service receives both raw binary64 operands
and returns raw binary32 bits. Unsupported source/FPSCR domains remain holds;
the implementation supplies no saturation, NaN result or guessed numeric
success. No fabricated guest-stack address or guest stack stores are introduced.

## Retention and service ownership

Every source load and numeric instruction has its own retained phase. Both SDK
children use their real nested retained cursors; child clear, stores and numeric
waits cannot restart successful source mutations. Source operands loaded before
a hold remain captured, while later loads stay fresh. In particular, table-entry
changes during Init affect the LOD header, while palette+8 changes do not; the
conversion constant loaded before a numeric hold remains captured for both
conversions. Aliasing the output onto a descriptor-table slot is observable:
real Init writes overwrite that slot before the second header load.

Services, raw entry arguments and their shared lifetime are immutable through
a held call. Replacement functions, user, strong lifetime identity or arguments
are rejected without progress. A failed current predicate latches stale.
Reentry/reset during invocation is rejected. `pending()` exposes the current
parent source instruction or SDK child; `childPending()` exposes the actual SDK
leaf frontier while that child is held. Successful completion consumes the
cursor. The caller owns scheduling; no generic retry of a synchronous mutating
SDK constructor is permitted.

`gx.clear` must provide the genuine whole `memset800050b4(object,0,32)` invocation,
including its memory effects. It may hold only with its own retained partial
work. Missing clear, source memory, initialized constants or numeric providers
remain explicit holds. `half` and `doubleWord` are actual source-sized loads;
the latter owns one original 64-bit load rather than independently admitting
two source half-loads. The binding pins the actual process/VM resource and
output-allocation context until all nested work completes.

## Initially unknown native private output storage

`SourcePrivateTextureObject` is an optional 32-byte storage adapter. The caller
must supply a genuinely reserved address and a strong actual lifetime/current
owner. Binding does not allocate/register guest addresses, claim an original SP
alias, mint source incarnations or infer an unused address range. It cannot
rebind, and it starts with all32 bytes unknown. Only actual admitted source
stores make bytes known. Size1/2/4 loads/stores use source big-endian ordering;
partial/out-of-range writes are rejected. A full eight-word snapshot is
available only after every byte is known.

Memory adapters must test `overlaps(address,size)` **before** falling back to
other memory: an unknown or partly overlapping private span is unavailable,
even if another provider offers bytes at those addresses. `owns` tests a wholly
contained span; it is not the first-refusal predicate for crossing accesses.
Explicit retirement/current loss remains sticky, and reentry from the current
predicate is rejected. This small utility is not a process-wide allocator or
overlap registry. The actual caller must reject overlap with existing resource
and VM mappings when it reserves the private address domain.

For [submenu packets](ORDERED_ORIGINAL_SUBMENU_PACKETS.md) or the actual window
resource path, constructor completion plus these known bytes can contribute to
a genuine SDK-object witness. It cannot by itself certify palette/image lifetime,
matching converted pixels, source whole-Setup completion, actual window ready
history, current camera8, native texture residency or actual GPU submission.
Returned handles must remain the actual supplied output identity, not a texture
index masquerading as an object. Normal native resource integration remains
unbound until those actual owners are supplied.

## Validation

An independent immediate parent translation and the retained implementation
produce identical complete memory/SDK/numeric traces and output bytes for actual
common images0..3 and actual original message-window image0. Every external
boundary is separately held twice. Tests exercise partial memset without replay,
late header and table mutations, captured numeric operands, unsigned LOD bytes,
full index-shift wrapping, missing memory/numerics, changed identity, sticky
lifetime loss, source/table/output aliasing, unknown private bytes, partially
overlapping accesses and callback reentry. Strict host, ASan/UBSan and ARM syntax
checks do not establish Gekko FPSCR, hardware rendering or native activation
parity. Root owns combined `make test`/`make all` after registration.
