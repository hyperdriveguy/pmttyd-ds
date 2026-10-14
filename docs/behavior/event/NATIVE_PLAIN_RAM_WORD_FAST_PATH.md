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
