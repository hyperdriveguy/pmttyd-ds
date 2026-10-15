# Guest event addresses, live aliases and RAM borrowing

Implementation: [address facade](../../source/event/event_address_space.cpp),
[entry views](../../source/event/event_entry_addresses.cpp),
[context views](../../source/event/event_context_addresses.cpp),
[runtime globals](../../source/event/event_runtime_addresses.cpp),
[GP views](../../source/event/event_gp_addresses.cpp),
[scoped accelerator](../../include/event/event_ram_access.hpp).
Public APIs are in [event headers](../../include/README.md).

All guest tokens are 32-bit big-endian addresses, never casts of native objects.
Entry/context/runtime/GP semantic views precede owned RAM, then external RAM in
byte lookup. Word access prefers one live semantic operation where possible;
unaligned/cross-field fallback assembles or merges byte lanes. Unknown entry/GP
bytes remain retained raw state. Float fields transfer their bits, not numeric
integer conversions. Caller owns the validity/lifetime of every borrowed object.

| View | Reached layout / identity |
| --- | --- |
| Entry | Packed stride `0x1b0`, independent of `sizeof(EventEntry)` |
| Entry links `0x6c/0x70/0x74` | Parent/child/brother, unresolved-mask bits 0/1/2 |
| Entry `0/4`, `0x15c` | Accumulated source lifetime high/low, separate event ID |
| Entry `0x164/0x168` | Binary32 timescale/credit |
| Context | `0xa0` bytes: capacity, 32 work/3 flag words, array token `+0x90`, raw `+0x94`, timestamp `+0x98/9c` |
| Runtime | 256 IDs at `0x803d8530`, 256 slots at `0x803d8930`; sparse active/count/allocation/ID/speed/system globals |
| GP root | Independent pointer word `0x80415fd0`, including partial BE stores |
| GP | Extent `0x13d8`; semantic refresh `+4`, context selector `+0x14`, timestamp `+0x38/3c`, other retained/bound lanes |

Entry registration rejects overlapping extents and duplicate live host entries.
`addressOf(nullptr)` succeeds with zero; `entryAt()` requires an exact slot start.
An unmapped nonzero link remains a raw token with a set unresolved bit, distinct
from a real null relationship. Registering later entry storage resolves matching
tokens. Before unregistering, native links into retiring storage become unresolved
original tokens; optional snapshot bytes preserve the old packed memory. Mapping
resolution alone does not prove allocation flags, event IDs or execution admission.

Contexts separately retain unresolved array tokens. Detach while entry address
lookup still works, **before** unregister/free. Resolve after new entry mapping.
Neither operation proves that context capacity fits an arbitrary supplied array.
`EventEntryStorage` orders this transition: original SDK allocation -> extract raw
RAM -> register/import semantic entries -> resolve arrays. Retirement detaches
contexts, snapshots/unregisters entries, restores raw RAM, then drops host storage.
It does not run source event deletion or free the SDK allocation.

GP registration does not select current GP. The pointer cell can name an
unregistered token, yielding no current globals, and retirement preserves that
word. Byte precedence within GP is bound render dimensions `+0x170..173`, bound
physical-input lanes, semantic fields, then retained bytes. These borrow actual
process storage, not per-battle snapshots. Dimension binding is one-shot; input
allows the same owner again but refuses replacement. Owner lifetimes remain
external. Runtime global writes likewise change actual schedule/global fields;
views do not sanitize invalid counts, rebuild priority tables or prove generation.

RAM registration rejects zero sizes, ordinary 32-bit extent wrap and existing
readable overlaps. For sparse live views, conservative interval probes avoid
expensive byte checks outside possible overlap. A readable-byte hole may admit
RAM even inside a semantic extent; raw acceleration still rejects that entire
extent. Register live views before RAM. Introducing later overlap through the
independent semantic registries is outside the facade's supported mapping order.
Script import prepares all segment copies before publication; extraction splits
only owned RAM and preserves neighboring fragments, not external storage.

Direct RAM access requires one complete owned/external region and no semantic
extent. Unaligned raw words are supported by byte assembly without typed host
pointer dereferences. Cross-provider/semantic ranges keep normal lookup. Fallback
stores preflight readable lanes before writing, avoiding unmapped-suffix partial
writes; this is not rollback against arbitrary provider effects/concurrent changes.

`withRawRamAccess()` supplies a noncopyable synchronous accelerator. It blocks
owned/external mapping changes and nested borrows in this facade. **It does not
lock the separate entry/context/GP registries.** Caller must keep all semantic
mappings stable, keep owner/backing alive, and avoid external children/yields.
RAII releases the borrow even on exceptions; no span survives the callback.

Thirty-two cached spans limit acceleration, not address admission. `bind()` may
succeed without caching at capacity; uncached operations fall back to the facade.
Same-start prefixes can grow after stream validation. Accelerated copy requires
nonempty fully bound ranges that are disjoint both virtually and physically,
because separate external mappings can alias the same bytes. Rejection lets the
caller preserve original forward copy semantics rather than substituting memmove.

See [service contracts](event-service-api.md) and [VM execution](event-vm.md).
These are native address/lifetime policies and source alias rules, not proofs of
original heap placement, Gekko execution, hardware timing or complete story flow.
