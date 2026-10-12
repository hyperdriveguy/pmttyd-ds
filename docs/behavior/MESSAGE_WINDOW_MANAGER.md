# Window manager traversal, partial reset and texture setup

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
windowMain0x8007f9ec,size68; windowInit0x8007fd84,size70;
windowReInit0x8007fb94,size44; windowTexSetup0x8007fbd8,size118;
completion callback unk_8007fcf0 at0x8007fcf0,size94; getWakuTexObj0x8007ad30,size10.
Pinned Japanese public windowdrv.c has corresponding structural descriptions,
including the unnamed US callback, but all contracts here were checked in US
instructions. Source presence is not US matching/execution evidence.
See [lifecycle](MESSAGE_WINDOW_LIFECYCLE.md), [states](MESSAGE_WINDOW_STATES.md),
[selection](MESSAGE_SELECTION.md) and [original baseline](../ORIGINAL_BEHAVIOR.md).

## Pool ownership and partial initialization (B)

Global pool pointer is r13+0x1b08 (0x8041ea28). windowInit calls
__memAlloc(0,0x1f8), exactly seven0x48-byte records, stores the returned pointer,
and explicitly clears only each record's flags halfword+2 and handle word+0x28.
It does not memset the whole pool, initialize state/main/delete callbacks,
free a previous pool or guard allocation failure. Later windowEntry performs
whole-record zeroing before allocating a slot, as documented in lifecycle.

Init also zeros palette pointer r13+0x1af8 (0x8041ea18) and completion word
r13+0x1afc (0x8041ea1c). It does not clear texture-object pointer r13+0x1b04
(0x8041ea24) or the scroll word at0x8041ea20.

windowReInit uses the existing pool and repeats just the seven flags/handle
clears. No allocation, main/delete callback call, smartFree, state reset or
texture-global change. Previously owned handles become unreachable through
these slots locally; whether broader reset/heap lifetime releases them is an
external dependency, not an established leak or cleanup guarantee.

## Ordered update traversal and mutations (B)

windowMain caches the pool base once and visits indices0..6, incrementing its
record pointer by0x48. At each visit, read flags; if numeric mask1 is clear,
skip. Otherwise read main callback at+0x3c; null skips, nonnull is invoked with
the current record pointer as the sole established argument. Ignore its return.

There is no layer sort, state test, poll-mask2 gate, handle-null gate or local
automatic deletion. Flags and callbacks are read at each visit, not copied into
an initial active-list snapshot. Callback mutations of later slots can therefore
affect the same pass: newly allocated later slot can update, cleared later slot
can skip. An earlier slot is not revisited. Replacing the global pool pointer
inside a callback would not change the cached traversal base for that pass.
These are static ordering consequences, not executed mutation traces.

The original marioStMain calls windowMain once at0x8006f948, after camMain and
before mapMain/hitMain/evtmgrMain. No conditional branch surrounds that call in
the recovered consecutive subsystem sequence. It follows the outer work+0x10
wait loop and makeKey. This proves ordering and one call in this function, not
global call frequency,60Hz, retrace/frame-skip policy or elapsed-time equivalence.

General/clear/selection main callbacks decide whether to register their display
callback through dispEntry(8,0,400.0-float(s16(layer)),callback,W). windowMain
does not draw or call display callbacks itself. Rendering may mutate separate
state, as [selection drawing](MESSAGE_SELECTION_DRAWING.md) demonstrates.
dispEntry queue borrowing/sort request/dispatch are now covered in
[display queue](DISPLAY_QUEUE_AND_DISPATCH.md), sorting/ties in
[sort implementation](DISPLAY_SORT_IMPLEMENTATION.md). The outer loop and
actual caller/tie executions remain open.

## Palette setup: archive path (B)

windowTexSetup first zeros palette pointer and completion word. Call
arcOpen("font/msgWindow.tpl",0,0), store its return as palette pointer. If
nonnull, call UnpackTexPalette(pointer). Read unsigned count at palette+4;
allocate __memAlloc(0,low32(count<<5)) and store texture-object pointer. For
each unsigned index starting0 while index<current palette count, call
TEXGetGXTexObjFromPalette(currentPalette,objects+0x20*index,index).

This synchronous archive path returns without writing completion word1.
Do not merge it with the asynchronous path's completion flag policy. Count is
reread on each loop test; palette/object pointers are loaded from globals in
the loop. No count/size-overflow/allocation guard or prior-object free locally.
Neither readiness nor actual descriptor/material binding is proved by names.

## Palette setup: DVD path and completion callback (B)

If arcOpen returns null, call getMarioStDvdRoot and sprintf a local128-byte
buffer with "%s/f/msgWindow.tpl". No local root-length/copy-capacity check.
Call DVDMgrOpen(path,2,0), retain entry; call DVDMgrGetLength(entry). Round the
returned word up with low32(length+31)&0xffffffe0. Allocate that size from heap0
and store palette pointer. Write entry itself to entry+0x6c. Then call
DVDMgrReadAsync(entry,palette,roundedSize,0,unk_8007fcf0) and return. No local
open/allocation/read-result guard, wait loop or synchronous fallback.

Completion callback ignores its first argument. Read handle from secondArg+0x2c,
call UnpackTexPalette(globalPalette), then DVDMgrClose(handle). Allocate the
same count*0x20 texture-object array and populate it in the same unsigned loop.
Finally write completion word1. It does not branch on the supplied read status,
validate descriptor count or release the palette buffer. The entry+0x6c store
and callback secondArg+0x2c read are explicit offsets; DVD manager/file-info
wrapper internals still require their own ownership/error/argument contract.

Repeated setup discards the prior palette pointer before opening a new one,
without a local free, and does not null the prior object pointer while the DVD
read is pending. Actual setup frequency, consumers and broader reset ownership
must be traced before judging lifetime or stale-object exposure.

## Texture lookup and fidelity obligations (B/P)

getWakuTexObj(index) returns globalObjectPointer+low32(index<<5), with no count,
null, readiness/completion-word or signed-index check. Message type1 creation
uses index3 for dimensions; lookup itself does not gate that access on loading.
Archive/DVD path literals are verified source bindings, not a claim of extracted
asset readiness. Palette unpacking/GX object construction remain service bodies
to recover; no original texture data is added to source history.

Compare partial init versus entry memset, reinit without delete callbacks,
slot order and same-pass mutations, update versus draw registration, both setup
paths' completion-word difference, rounded asynchronous size, ignored read status,
unchecked descriptor count and lookup arithmetic. Remaining: outer/display
cadence and actual queue lifetimes, full message/selection rendering, allocator/archive/
DVD/palette services, actual reset/loading callers and original story conditions.
No original execution or complete dialogue/full-game equivalence is claimed.
