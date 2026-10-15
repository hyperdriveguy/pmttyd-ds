# Original font setup and shared mutable source resources

This batch adds the complete retained `JUTFontSetup80075004/2f8` and its
complete `fontmgrTexSetup80077bdc/2c` caller, an immutable original US BFN
file owner, and actual native-backed shared source memory. It supplies no
normal font callback activation or native GX acknowledgement. See the original
[setup contract](../message/FONT_RESOURCE_SETUP.md),
[mapping](../message/FONT_CODE_MAPPING.md) and the
[scalar parent](RETAINED_SCALAR_FONT.md).

## Whole setup source controller

`resumeSourceFontManagerSetup` reads fresh GP.word16c and narrows it to unsigned
16 bits. `resumeSourceFontSetup(selector,...)` implements the original once-only
font-pointer guard, low16 selector table lookup and all remaining branches.
It does not guess GP language, reset an existing font or compare a new selector
against the loaded resource.

Requests retain their original identities and operands through waits:

| Original child | Retained source work |
| --- | --- |
| `getMarioStDvdRoot80007388` | Once for selection, then separately for every probe |
| `sprintf8026a0b8` | Actual `%s/f/%s.bfn` selection; seven `%s/font/%s.bfn` probes |
| `DVDMgrOpen80144e88` | Typed real private path, arguments 2,0 |
| `DVDMgrGetLength80144dcc` | Selected handle without null guard; nonzero probe handles |
| `DVDMgrClose80144dd4` | Each nonzero probe, then selected handle after read |
| `__memAlloc800300f0` | Heap0, `(signed-maximum+31)&ffffffe0` with source wrap |
| `DVDMgrRead80144e18` | Selected handle, captured actual heap, rounded selected length, offset0 |
| `GXInitTexObj802b72b0` | Static object8041e97c, actual GLY1 image, source dimensions, I4/clamp/no-mip |
| `GXInitTexObjLOD802b7544` | Linear/linear, actual source zero floats, bias-clamp/edge/anisotropy0 |

The native maximum comparison is signed32, while additions/alignment retain
32-bit wrap. Duplicate resource names are probed separately; there is no
seven-entry guard on the original selector lookup. Selected handle0, allocator0
and read return values are not replaced with invented recovery. Engaged zero
is distinct from an unavailable provider; the source ignores the read result.
Real failure/fault/missing-memory effects remain required child/provider
boundaries. The total actor readers require admitted mapped source storage;
unknown bytes must never be supplied as successful zero words.

The returned heap pointer is published to8041e978 **before** the read. After
closing the selected handle, the controller reloads that global and scans
blocks. The header count is reread at every loop condition, sizes remain
unchecked source operands, and the last INF1/GLY1 match wins. After genuine
texture child completion it reuses the shared original map/WID helpers and
performs the real byte stores7 and9. In the original US BFN both map to glyph0,
so the final mutable width is9; the immutable source file remains width1.
A failed WID lookup still addresses byte1, as the original source does; it is
not silently skipped or redirected.

`SourceFontPath` represents written source private bytes, not a fabricated guest
pointer or zero-filled C string. The actual formatter writes its real bytes and
NUL while retaining old suffix cells. No path truncation or guessed stack-buffer
size is introduced. Native formatting/stack-alias effects beyond its admitted
source domain remain external, including any overflowing formatter path.

Strong actual service lifetime, callback identities and direct-selector/caller
identity remain fixed across waits. Every external boundary checks `current`;
false permanently retires the held cursor. Reentry/reset during a provider is
rejected. Reset does not invent source DVD closes, heap frees or rollback of
already published CPU globals; the real external child owners retain their
cancellation/teardown obligations.

## Real file, heap and process memory ownership

`NativeSourceFontFile::load` retains the exact immutable original US BFN bytes.
It requires66560 bytes, EOF and the trusted private-pack FNV64 integrity value
`cdfbe0644553b623`. The converter/source verifier independently require SHA-256
`bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45`.
FNV is a trusted-pack corruption guard, not a cryptographic authenticity claim.
The canonical source identity is `f/papermarioset_US.bfn`; this does not create
DVD handles, remap `font/`, acknowledge heap/DVD calls or perform texture setup.

`NativeSourceFontMemory` begins with **no snapshot**. Its first successful
binding captures actual existing source bytes from these four spans:

| Span | Ownership |
| --- | --- |
| `8041e978..8041e9bb` | Font pointer, static GX object bytes, INF/GLY pointers, draw state |
| `80416010..80416053` | Actual original palette, spacing and seven name pointers |
| `80429578..8042957b` | Persistent fog word |
| `80429598..8042959b` | Persistent selected font color |

All spans must already be readable owned RAM. No constructor BSS defaults or
per-Trio initialization is substituted. A legitimate process-cold owner can
provide source BSS zeros through the actual DOL boot-image semantics: BSS is
`803c5360+645ac`, but initialized DOL sections take priority over its broad
extent. In particular palette/spacing/name data are initialized source bytes,
not zeros merely because their addresses also lie inside that broad interval.
Restored/live history must be adopted as actual history. The old incomplete
Trio cold-memory producer remains a genuine normal-binding frontier.

Subsequent VM bindings use the same mutable process bytes and existing history.
They do not recapture a new VM's font zeros or privately duplicate the font.
A genuine heap child supplies actual native backing/capacity and its strong
allocation owner with an explicit admitted guest address. Native addresses are
mapping policy, not recovered GameCube heap placement. Overlapping guest/native
allocations, wrap, missing owners and conflicts in bound VMs are rejected.

The actual DVD child supplies exactly the bytes it read to `installRead`.
Only that known prefix becomes readable and externally mapped. Unused rounded
capacity is retained as **owned but unknown**, with no zero fill or fallback
to another VM reader. `owns(address,size)` claims any intersecting owned range
for routing, including unknown cells. The allocator must retain the entire
reserved address interval; introducing foreign mappings into its unused tail
is outside the binding contract. Whole-capacity admission is checked again
before a read is installed or another VM binds. A read cannot be reinstalled
silently over later width mutations.

External mappings alias the actual allocation backing, so BE byte/half/word
accesses and both VM views see the same7→9 mutations. The file owner's original
bytes remain untouched. A binding retains the process owner, which retains all
actual allocation leases. Bindings must die before their borrowed VM, outside
raw borrows; callers may not alter their owned VM mappings. Detachment removes
payload views and restores that VM's prior private span bytes. This is native
binding cleanup, not an original font reset/free or a rollback of shared process
history. The process owner retains font bytes/history for subsequent bindings.

The mapped GX object initially contains only the actually captured bytes.
This owner never synthesizes SDK object fields or zeroes it as a setup shortcut.
The real InitTexObj/LOD provider must own its actual writes/native texture state
before acknowledging those source calls. A queued presentation owner must prove
that state and actual font/camera/GPU generations; captured requests, nonzero
BFN pointer and even the source guard's early return are not presentation proof.
The original publish-before-read guard is intentionally observable while a
load is held, so native source callbacks must be serialized by the outer owner.

## Raw pack, paths and rights

`tools/source_font_resources_assets.py` exports the unchanged whole original
`f/papermarioset_US.bfn` from read-only US revision0 ISO offset948050384,
length66560. Default ignored product:
`romfs/source-font/papermarioset_US.bfn`, with `source-font.json` source extent,
SHA-256, converter hash and licensing. There is no texture resize, glyph
remapping, synthetic header or converter-time width patch. Existing
`tools/font_assets.py` and `font.pmfont/font.t3x` conversions remain untouched.
The parsed/texture pack can be paired with the raw source BFN only when actual
source setup has performed its mutable width writes.

A new import requires an empty destination. An exactly matching existing pack
is reusable; changed payload, metadata or extra files are preserved and cause
rejection. Converter changes therefore require a fresh empty output/cache
rather than overwriting an existing pack. Original Nintendo/Intelligent Systems
rights remain retained; source tracking grants no new license or redistribution
rights. No original payload/ISO is committed.

## Validation and remaining frontier

Focused host tests read the original BFN and cold DOL bytes read-only, run the
whole controller through actual mapped backing and verify both real width
stores, descriptor publication and a second VM sharing the patched original
file. Every typed child hold is retried twice with identical complete source
traces. Tests also cover the nonzero one-time guard while an original read is
held, fresh manager selector narrowing, signed maxima, distinct allocation/read
sizes, unknown capacity, lifetime retirement and missing providers.

Memory/file tests cover actual original-file load and rejection of patched or
truncated data, strong backing retention, two-VM aliases/history, detach,
missing initial spans, guest/native overlap, wrap, unknown tails and admission
failure without copying source bytes. The explicit host GX callbacks are
fixtures; they do not prove native texture object encoding or raster parity.
Python tests verify exact raw export/hash/unpatched source width and preservation
of changed/extra existing files. Strict host, ASan/UBSan, ARM syntax and the
read-only whole-body/data/BSS/BFN verifier supplement these checks. Root owns
combined `make test`/`make all` after integration.

No genuine normal DVD/root/formatter/heap manager, process cold boot, GX object
encoder or renderer callback is activated here. The typed native journal records
those remaining owners, source-call boundaries and process/VM lifetime hooks;
missing ones must remain unavailable. No story dialogue, physical controls,
original menu art or lower audience scene behavior changes in this batch.
