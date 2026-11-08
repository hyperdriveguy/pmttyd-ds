# Native plain-RAM word access

The address-space adapter now reads/writes big-endian halfwords and words directly
when every byte lies inside one RAM region outside all conservative live-provider
extents. It uses byte lanes, so unaligned RAM access is supported without native
unaligned pointer casts. The existing32-bit ceiling check applies before pointer
selection. The returned pointer is used immediately and never cached across
registration, extraction, import or retirement.

Live entry/context/runtime/GP fields retain the existing provider precedence and
aligned field operations. An intersecting live extent always takes the original
facade, even for unreadable sparse aliases backed by RAM. Cross-region spans also
retain that facade. Missing-byte writes are rejected before changing any existing
byte, and failed reads preserve the caller's output. No original memory content,
VM instruction order, simulation tick, RNG call or audience visit is removed.
This is native storage policy, not a new original GameCube memory rule.

Tests compare visible bytes with halfword/word results across unaligned contiguous
RAM, adjacent regions, holes, live context aliases and the final four bytes of the
32-bit address space. They check both successful big-endian stores and unchanged
bytes after rejected stores. Existing entry/GP alias, boundary, retirement and
original script-import tests remain applicable. Full-crowd graphics performance
must be measured separately; this change alone is not a gameplay acceptance test.


## Ordered physical region lookup (2026-11-08)

Owned and external mapping vectors remain sorted by guest base. Registration
inserts by base; extraction places the retained right fragment immediately after
the left; script import inserts every prepared segment by base after its existing
all-segment validation. Guest ranges still cannot overlap. Unregistration retains
order. Neither physical bytes nor source addresses are changed by region moves.

Byte fallback and raw-span admission select the greatest base not above the
requested address, then check that the entire byte/span fits. Disjoint intervals
make that the only possible containing region. The original live semantic
precedence, GP language-alias guards, conservative mayMapRange exclusions,
unaligned access and cross-region byte fallback are unchanged. Sorted storage
is the actual mapping store, not a cache of pointers, values or authorization.
The existing32-span raw lookup cache stays callback-local and still blocks
facade mapping mutation throughout the borrow.

Regression coverage registers128 owned/external pairs in a shuffled order,
checks every byte and gap both normally and in a raw borrow, splits/replaces an
interior span, writes across adjacent fragments, rejects a gap-crossing store
without prefix writes, rejects foreign equal-byte backing, and imports between
existing lower/higher mappings. Full semantic/alias/ceiling coverage remains in
the existing address-space suite. Source-map acceleration and measurements are
recorded in [the map process contract](../field/NATIVE_SHARED_MAP_PROCESS.md).
