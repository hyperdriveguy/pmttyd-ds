# Native heap clear borrowing

`EventMemoryBinding::allocateHeap` uses the original SDK allocation, exact
requested-byte clear and original32-byte flush sequence. It borrows stable VM
RAM only across the closed allocation/clear memory operations. It releases that
borrow before any platform flush or synchronization callback; reentrant platform
operations therefore retain their existing admission. A nested caller retains
the existing source helper and its outer borrow. Sparse semantic aliases retain
the ordinary facade; the raw cache only admits proven nonsemantic extents.

Native UI, icon, effect, animation and event-entry owners use this adapter. Icon
clear preflight additionally verifies the actual owned heap backing identity and
checks the resulting zero bytes directly while that synchronous backing proof is
valid. No pointer, cached value, source completion or borrow escapes the call.

The policy addresses startup cost from repeated linear mapping checks. It does
not change original guest allocation addresses, rounding, memset store order,
flush addresses or source timing. Tests compare original and native byte images,
zero and boundary sizes, failure, platform callback mapping mutations and nested
borrowing. The existing actual icon tests cover original allocation/DVD/callback
counts, held boundaries, cross-VM aliases and owner replacement.
