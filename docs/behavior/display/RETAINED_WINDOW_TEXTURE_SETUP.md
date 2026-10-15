# Retained original process window texture setup

The complete US revision-0 bodies are windowTexSetup `8007fbd8/118`, its DVD
completion callback `8007fcf0/94`, and UnpackTexPalette `80037b6c/120`.
`tools/verify_source_window_texture_setup.py` pins all three bodies, all fifteen
original direct child callsites, four original literal spans, the whole actual
`seq_logoExit8008746c/44` caller and the original TEX/GX bodies and window TPL
through the existing linked verifier. The only direct DOL call to Setup is
`seq_logoExit8008747c`. This is process loading, not a battle-window readiness
operation. See [the original manager contract](../message/MESSAGE_WINDOW_MANAGER.md),
[window packets](ORIGINAL_CAMERA8_WINDOW_PACKETS.md) and
[palette object construction](RETAINED_PALETTE_TEXTURE_OBJECT.md).

The new portable owner executes the complete original CPU control and memory
graph. It does not schedule DVD work or create a native texture. Actual optional
word/byte/store providers, whole external request owners, real memory/object
allocations, actual DVD callback invocation and a strong actual lifetime are
required. `nullopt` means unavailable, whereas engaged zero is the original
returned zero. In particular an actual zero open/allocation is not rewritten
into a successful load or an invented early return.

## Setup and completion order

Setup first writes palette `8041ea18=0`, then completion `8041ea1c=0`, then calls
actual arcOpen `800782e4` with original `font/msgWindow.tpl` at `802c3080` and
arguments0,0. It stores the actual return in the palette global. A nonzero
archive result runs the complete relocation body, rereads the global palette,
reads its count, calls actual heap0 allocation of `low32(count<<5)`, and stores
the returned object-array address in `8041ea24`. Its unsigned index loop freshly
reads the global palette/count each test and freshly reads the object global
for each actual TEX call. It **does not** write completion1.

The zero archive branch calls actual getMarioStDvdRoot `80007388`, retains the
private sprintf destination and original format `%s/f/msgWindow.tpl` at
`802c3094`, then calls actual DVDMgrOpen `80144e88` with the private path and
arguments2,0. It captures that returned handle and actual DVDMgrGetLength
`80144dcc`, rounds the length using `low32(length+31)&ffffffe0`, and requests
that exact heap0 allocation. It stores the palette result, writes handle itself
at handle+6c, freshly reloads the palette global and calls actual
DVDMgrReadAsync `80144de4` with handle,palette,rounded,0,`8007fcf0`. It returns
only when that whole retained child acknowledges its actual scheduling work.
It neither calls the completion callback nor clears/replaces the previous
object-array global while the read is pending.

The genuine completion entry ignores the original first argument. Its typed
entry accepts only the actual second argument, reads handle at second+2c first,
then freshly reads the palette global and relocates it. It calls actual
DVDMgrClose `80144dd4` using the captured handle. After that acknowledged close,
it freshly rereads global palette/count, allocates the object array, and runs
the same original late-global/index loop through real TEX/GX continuations.
Only after all actual objects complete does the source store at `8007fd68`
write completion1. No read-status gate, count bound, null-allocation guard,
prior-array free or native atlas import is inserted.

## Complete palette relocation

The original magic comparison is exactly `0020af30`. A mismatch calls the real
panic `80297894` with original filename `802c0148`, line36, and format
`802c0158`. A missing panic owner holds. If a genuine panic child returns,
source execution continues; tests explicitly label their returning test panic.

The palette table word greater than unsigned `80000000` returns immediately.
Equality still follows the relative-pointer path. Otherwise the table is
relocated by low32 base addition. Every original fresh load/store is retained:
nonnull image/header pointers are relocated, image data+8 only when byte+0x23 is
zero, and palette-data+8 only when palette-header byte+2 is zero. Each flag1
store follows its original repeated table/header reload. Null headers skip only
their own branch. Index increments as a word; the loop compares its low16 bits
against a fresh word count, and the table offset is `(index<<3)&0007fff8`.
There is no invented overflow cap or descriptor validation. Unrealistic counts
can therefore have the original nontermination/wrapping behavior; the native
resource reader must separately admit its actual owned memory.

## Retention and binding boundary

`SourceWindowTextureSetupServices.objects` supplies the existing whole
`resumeSourcePaletteTextureObject` → GXInit/GXLOD graph. It uses the same actual
optional memory/output owner as Setup and Unpack. The exact original object
address is passed to TEX; there is no substituted object index/private fake
pointer or acknowledged raw GX request. Actual memory writes must be atomic
per original store: false means that store was not acknowledged. Whole external
children retain any partially completed effects themselves.

The request carries function/callsite/captured integer arguments. Source stack
path is a typed cursor-owned sparse byte map. Formatter requests receive
`destination`; DVDOpen receives its stable borrowed `path`. Original destination
argument is replaced by this typed output, not assigned a guest address.
Formatter `arguments` are format,root; DVDOpen arguments are mode,callback.
All other request arguments are the original register arguments. Unknown suffix
is absent; no NUL, root name, normalization, capacity guard or path bytes are
invented. Formatter ownership must explicitly admit the real path domain;
this typed port does not claim arbitrary guest stack-corruption equivalence.
Borrowed paths may be held only through the current invocation.

Each parent invocation pins every service pointer, user, actual strong lifetime
including control-block identity, input and request identity. Reentry/reset while
invoking fails. Every successful read/store/child is preserved through holds;
later source loads remain late. Actual current loss latches stale. The separate
Setup and genuine DVD callback need separate cursors if the DVD owner can invoke
the callback before ReadAsync returns; do not recursively reuse the Setup cursor.
No process-memory constructor zeros, per-battle reset or inferred readiness is
provided. Repeated successful entry starts the original body again.

The actual process owner must retain the DVD entry, palette allocation, relocated
descriptors and object allocation through all source users and resident packets.
Completion1 alone cannot validate descriptor/object provenance. Conversely,
native atlas residency cannot authorize completion1. The Window9 packet query
must require the real process setup/callback producer, actual object address
and bytes from that same retained memory owner, and real native resource lease.
Long packet proof must survive legitimate camera completion while checking
these actual allocation/resource owners.

## Validation and limits

Focused tests compare all original parent memory/child traces with independent
immediate Setup/callback translations and a complete independent Unpack
translation. Every reachable optional memory/math/child boundary is held twice;
successful traces and final memory match uninterrupted execution. The cases
cover archive, DVD scheduling, real callback, null/nonzero and flagged headers,
magic panic, exact pointer threshold, zero count, partial linked memset,
reentry, replaced service/control-block/input, sticky stale owner and late
global mutation after a held close. Actual original `f/msgWindow.tpl` all51
entries run through complete relocation and actual TEX/GX source bodies with
original DOL conversion/filter constants. That CPU fixture has explicitly test
DVD/heap ownership and proves neither actual DVD delivery nor GPU residency.

Strict host, sanitizer and ARM syntax, ISO source pins and documentation links
are run for this isolated batch. Shared build registration and native binding
are unapplied integration notes. No original PPC execution, normal queued
weapon/tutorial activation, native window GPU submission or whole remake
completion is claimed.
