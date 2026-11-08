# Harbor source map-pool ownership

`HarborSourceMapPool` provides a native primary map arena with the same explicit
cold storage policy as `BattleSequenceStorage`. Its one MiB capacity and guest
base `0x84700000` are native allocation policy, not observed GameCube heap
addresses or measured original memory requirements. The primary free header
and original map-global pointer/length cells have stable owned byte backing.

The original `allocateEventBattleMapPool` creates the nested `0x64000`-byte
battle pool and publishes `8041e80c`. Every subsequent request uses the actual
`eventMapAllocate` best-fit/split/clear/flush implementation and paired
`eventMapFree` coalescing implementation. The existing SDK heap is separate.
Zero-length and no-fit allocations return an engaged original zero; an absent
or wrong root stays unavailable. No successful menu readiness is inferred.

Each nonzero allocation owns a distinct native lifetime token. Free retires
that token; reusing the same guest address produces a different incarnation.
The owner checks actual external backing identities through a synchronous raw
borrow and verifies the current root cell. Consumer guards must use the exact
allocation token as well as the current owner. The VM facade outlives this
owner; all consumers and borrows must stop before destruction. Teardown removes
only mappings whose backing still belongs to this owner.

Focused actual-VM tests run nested allocation, source work and odd-size UI
allocations, wrong roots, zero/no-fit returns, coalescing and reuse, root
replacement rejection, cache-operation counts, teardown and fresh binding.
This closes storage ownership only. Cold battle initializers, source UnitEntry,
queued menu drawing, physical selection, attacks and field return require their
own native bindings and end-to-end validation.
