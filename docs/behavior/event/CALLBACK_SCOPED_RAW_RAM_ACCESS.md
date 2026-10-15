# Callback-scoped raw RAM access for source animation

`EventAddressSpace::withRawRamAccess` admits one noncopyable access object for
one synchronous callback. Its facade and semantic-provider mappings must stay
in place: no copying/moving the facade, remapping independent provider owners,
yielding or external children during that callback. All six facade storage
mutators reject while the callback is active. Nested access rejects without
calling the nested callback. RAII releases the guard on success, false or host
exception. This is native acceleration policy, not a new original VM primitive.

Explicit `bind(address,size)` admission remains available. Facade reads/writes
within the same borrow also cache complete actual RAM regions after proving that
no semantic-provider extent overlaps the whole region. The cache stores byte
addresses only; it never stores values. When the whole region cannot be admitted,
a separately excluded exact request range can be cached. Fully nonsemantic unmapped-range proofs
can also be reused while mappings remain fixed.
Binding uses the existing semantic-provider exclusion and contiguous-span rule;
GP/context/entry/runtime semantics retain the ordinary facade. Data edits through
other views remain immediately visible because the cache contains storage views,
not copied values. No raw host pointers are exposed by the interface.

Thirty-two fixed entries avoid heap allocation. Growing same-address prefixes
replace the existing entry; contained requests reuse admission. A full explicit-bind cache
still admits valid storage and falls back to ordinary reads/writes. Automatic
region admission uses bounded round-robin replacement; eviction changes only
lookup cost. A failed
binding grants no unchecked access. All unbound, cross-region and semantic
reads/writes retain the previous facade, including failed-write atomicity and
32-bit ceiling behavior. BE word loads/stores preserve unaligned byte layout.

Bulk copy requires completely bound, guest-disjoint AND physically disjoint
storage. Distinct guest external mappings can alias the same native backing;
those copies reject acceleration, preserving the caller's forward word/byte
loop. Admitted disjoint copies use native `memcpy`. During this callback-free
nonalias phase no observer distinguishes the underlying byte-store schedule;
source scalar publications retain their original order.

The [original animation scratch sampler](../display/NATIVE_SOURCE_ANIMATION_SCRATCH.md)
binds source controls/keys/sparse streams and all admitted cache/output spans.
It retains all existing ownership, file, count, alias and tangent checks, original
reset/replay/interpolation arithmetic, timing and loop-seam behavior. It reuses
the raw access session for final decoding but retains fresh owner and View
validation after scratch writes. Remapped external backing can physically alias
a MODEL field despite disjoint guest addresses; that final validation must still
reject a field overwritten during sampling. Cache/output RAM remains authoritative; there is no native
float copyback. Independent held traversal/material/FIFO readers retain fresh
admission at their own capture points. Retirement and copied-pool snapshots each
create a separate short-lived access session.

Validation covers semantic live state, unaligned BE lanes, cross-provider and
ceiling edges, all mutator guards, nested/rejected/exception cleanup, cache
capacity overflow, prefix growth, physically aliased external storage and
source-buffer/held-consumer/retirement behavior. Private before/after benchmark
hashes include every source scalar/cache/output byte plus presentation vertices
and colors for each sampled frame. Those bounded comparisons do not prove every
clip or original execution/FPSCR domain, full renderer equivalence or physical
New 2DS XL performance. See the latest [progress](../../PROGRESS.md) and
[optimization record](../../optimization/SOURCE_ANIMATION_RAM_ACCESS.md).

The retained native Main executor and atomic mesh capture/decode now use this
closed synchronous scope. Current/pass/reservation predicates must remain pure;
source Main and scalar fallback header/lifetime/numeric/GQR checks read fresh
values. Opt-in private decode has a separate invariance proof below.
The W guard uses a fresh root and 24-byte BE header comparison, with native
memcmp only over proved raw storage and byte fallback elsewhere. Structural
tangent/scratch owner identities are proved once per closed borrow and RAII
clears that proof before return; comparison results are never cached.
Capture releases its lease before model geometry admission, assembly and final
publication; no nested lease or yield is introduced. Exact current measurements
and remaining limits are in [progress](../../PROGRESS.md).

The optional closed-native replay copy service requires whole owned destination
RAM, nonwrapping disjoint virtual/physical ranges and no physical destination
overlap with any external region. The owned W-root word is separately excluded.
Only positive byte copies or positive truncated-word counts are admitted; other
cases retain the original scalar helper. Failure stores nothing, active scalar
cursors never switch lanes, and parent service identity includes the copy slot.
See [native publication](../display/NATIVE_ATOMIC_MESH_PUBLICATION.md).

## Private publication storage proof (2026-10-15)

`RawRamAccess::disjointStorage(pointer,size)` checks physical separation from
ALL registered owned/external VM regions, without retaining a pointer. It proves
no semantic-provider ownership claim. The [private capture contract](../display/NATIVE_PRIVATE_CAPTURE_PROOFS.md)
combines this check for the owner object and six candidate write vectors with
raw root/header/source-range proofs and explicit predicate independence. Only
that admitted closed decode slice can omit repeated ownership checks. Failure
falls through the scalar continuation; flags clear by RAII at every return.
Fresh reads, finite failures, stores and visit counts retain their order.
