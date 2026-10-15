# Retained source font loading and shared memory

[Setup](../../source/display/source_font_setup.cpp) and
[shared native memory](../../source/platform/native_source_font_memory.cpp) keep
original mutable font bytes distinct from immutable packaged input. See
[original font setup](../behavior/message/FONT_RESOURCE_SETUP.md) for source
addresses, resource tables and evidence; [message drawing](message-window-drawing.md)
explains later consumers.

## Lazy setup and retained children

The explicit-selector entry is 0x80075004. The manager entry 0x80077bdc obtains
the low 16 bits of GP word 0x16c. Both first inspect pointer 0x8041e978. Nonzero
returns immediately, even if a different invocation published that pointer before
its DVD read completed. This source guard is not a native readiness predicate.

The name index uses `(selector << 2) & 0x3fffc`, without a seven-name bounds
check. The selected path is `root/f/name.bfn`; Open mode 2/argument 0 and Length
run without a local null-handle fallback. Seven independent `root/font/name.bfn`
probes follow, including duplicate names and fresh root queries. Only nonnull
probe handles receive Length/Close. The maximum uses signed 32-bit comparison;
the selected length remains separately captured for Read.

Heap 0 allocation rounds maximum length with `(maximum + 31) & 0xffffffe0`,
retaining unsigned wrap. Setup publishes the returned pointer before Read of the
separately rounded selected length, then closes the selected handle. Local code
supplies no zero fill, read-result recovery or rollback. Actual DVD, heap and GX
children must perform their own real writes and retain partial work.

Blocks start at font+0x20. Every loop condition reloads live word+0x0c; word+4
advances to the next block. The last INF1 and GLY1 matches overwrite their global
pointers. Other tags are skipped. This loop invents no malformed-size, missing-
block or file-end fallback. GX requests use GLY1+0x20 image, halfword dimensions
at +0x1a/+0x1c, I4 format 0, clamp wrap 0, no mipmap and linear min/mag 1 with
source zero LOD values. Request completion requires the actual service boundary;
it does not alone prove native texture residency or pixels.

Finally code 0x20 maps to a glyph/WID1 entry and writes width 7; code 0x8140
maps separately and writes width 9. The offset byte survives. Failed width lookup
has no local guard. In the original US BFN both codes select glyph 0, so the same
live byte at decimal offset 77 changes from disc value 1 to 7 then 9. The original
packaged BFN remains unchanged.

Each child captures its function, callsite and operands once, retaining the same
request on retry. An engaged zero is a real returned zero; `nullopt` holds.
Sparse path cells preserve unknown suffix bytes and the formatter's actual NUL.
Entry/argument, provider identities and both pointer/control-block lifetime
identity are fixed across a hold. Loss of current ownership poisons the cursor;
reentry/reset during callbacks fails. Completion clears invocation state. A later
call begins at the source pointer guard, rather than a synthetic done flag.

## Shared process state and unknown allocation tails

[NativeSourceFontFile](../../include/platform/native_source_font_memory.hpp)
accepts exactly 66,560 bytes with no trailing data. Runtime FNV-1a64 checks the
trusted pack; the converter separately pins the original SHA-256. This loader
is a corruption check, not an independent replacement for provenance.

The process memory owner captures four supplied source spans: font pointer/GX
object/descriptors at 0x8041e978, initialized palette/spacing/name table at
0x80416010, and actual map-state cells 0x80429578/0x80429598. It creates no source
BSS defaults. First successful binding supplies the snapshot; subsequent VMs
alias that same mutable storage. Each VM saves its pre-bind bytes for detach.
Duplicate bindings fail. Binding preflights all spans and reserved allocations;
partial mapping failure restores the attempted VM without discarding shared state.

Allocation adoption requires an actual guest base, live native backing and strong
allocation owner. It rejects guest and physical overlaps, source-span conflicts,
32-bit guest extent/pointer overflow and occupied capacity in bound VMs. Capacity
reserves routing ownership but starts with zero known bytes. One actual DVD Read
may install a nonempty prefix: all VMs receive the prefix mapping before bytes
are copied. Failure removes new aliases without modifying backing or known size.
The unused tail stays owned, unreadable and unmodified; a fallback must not turn
that unknown tail into readable zeros.

Halfwords/words assemble big-endian lanes. A missing byte makes the whole read
unavailable; word stores check all four destinations before mutating any byte.
`owns` means any intersection, deliberately broader than readable containment.
Bindings must die before their borrowed VMs, outside raw borrows, with mappings
serialized against outside mutation. Detach restores each VM's prior span bytes
and removes payload aliases. Shared process mutations and pinned allocation bytes
survive in the owner after every VM detaches. See the
[public lifetime contracts](../../include/platform/native_source_font_memory.hpp).
