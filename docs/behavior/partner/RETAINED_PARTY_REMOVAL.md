# Retained original party removal

`field_party_lifetime.hpp/cpp` implement complete parent graphs for US revision0
`partyKill800be69c/0x160`, `partyKill2 800be594/0x108`,
`evt_mario_clear_party800e2da0/0x6c`, `evt_mario_kill_party800e2e0c/0x6c` and
`marioPartyKill80141198/0x6c`. These portable controllers still require real
memory, callback, animation-release and heap-free providers before field/story
sites can use them. They do not make the current native party transition ready.

Fresh read-only ISO evidence is in the ignored local
`artifacts/field-party-lifetime-evidence/`: complete `.txt` listings and ELF
views, `provenance.json` and `mario-party-outer-order.md`. Corresponding source
function SHA256, in the order above:

- `ecaee66555273aa04d74de2fd26873da4831c65c80609c96d0454f284161b239`
- `94913544dd411f22d5100e35616c69bc35c1c8bb703d99bea20e4761ea4d415f`
- `20b511ea81ec19a4268750bea3d6049202e71d97155ed0a670b88d7b8cff3ae1`
- `aa07c27327f63015904ac7e045191417d55012fe11e79a9512f0a4ea6bbbbb9f`
- `13c3c2da4ff16aecef9661a30d77eaa84fbe2789cda4fc1fc106e30cd07705f6`

## Kill and Kill2

Negative input skips the pointer-table read. Null initial actor returns0 without
writes. Otherwise capture the original actor P, its owner M at+160, and M+i.
Read signed owner byte+245, look up its linked actor, then read linked.kind+31
without a source null guard. Sign-extend kind and publish
`803143e8+kind*28` to currentDefinition8041ead4. Read exit hook from captured
definition+24. A zero hook skips the mode read; a nonzero hook runs with P
unless P.mode+34 is11.

Kill rereads captured owner's signed byte+245 after the hook. A nonnegative
index loads its table pointer for the null test; a nonnull result performs a
**second fresh table load** before reading raw kind+31. Save that raw byte, or0
for a negative/null result, to captured M+i+247. Kill2 instead stores0 without
these post-hook reads.

Both then write ff to captured M+i+245, clear pointerTable[original i], freshly
read captured P.pose+c, call original animPoseRelease8003b2d0, then
`__memFree800300c4(heap0,P)`. Source return is0. No invented negative-pose check,
index upper bound, kind whitelist, HP reset, unlock write, table compaction or
current-slot/global-definition clearing is added.

A child can mutate the linked table, index, definition global and pose handle.
Captured P/M and definition are retained at their source capture points. The
actor must remain allocated after table clear while release/free wait. See
[the native source release contract](../display/NATIVE_SOURCE_POSE_RELEASE.md)
for the animation child; a typed lookup after publication is insufficient.

## EVT and Mario callers

Both EVT functions first call marioGetPtr, then read EventEntry+18 and raw
args[0]. They never call evtGetValue. They read signed M+rawSlot+245: a negative
byte returns2. Raw slot0 calls Kill with that byte. Any raw nonzero slot calls
literal index1: clear uses Kill2 and kill uses Kill. Raw arithmetic wraps at32
bits; bounded native admission belongs to providers, not these source parents.
Result2 is published only after child completion.

`marioPartyKill80141198` queries Mario, reads signed primary+245, calls Kill
with a nonnegative value and explicitly writes ff to that captured owner. It
then **queries Mario again**, even when primary was absent, and repeats using
auxiliary+246 and its actual value. It never calls Kill2. It is a void caller;
no synthetic scalar return is produced. The local evidence filename
`marioPartyLeave.txt` is historical; the symbol is marioPartyKill.

## Holds and integration boundary

Each cursor pins request identity, context and all used service functions.
Unavailable memory operations return before effects; completed reads/stores
are never repeated. A child can progress internally while returning false and
must retain its own continuation. Parent call function/arguments remain pinned
across retries. Wrong requests/providers do not replace the active invocation.
Callers own scheduling, lifetimes and cancellation; copying a cursor does not
create a second independent ownership proof.

Host fixtures compare complete ordered traces, fail each memory/child primitive
once, exercise hook and provider mutations, raw signed kind/indices, null hooks,
mode11, raw EVT slot arithmetic, literal1 dispatch, fresh Mario queries, and
children that publish effects before holding. No hardware performance, pixel,
Gekko runtime or complete prologue claim follows from these fixtures.

Next bind retained native actor ownership, actual source definition records,
pose wrapper retirement and heap free. Then use the controllers at authored
harbor/plaza transitions, alongside auxiliary Entry2Pos(kind13) and a retained
ordinary reentry constructor. Merely replacing the current ending Kill barrier
would permit construction/release replay on later ending holds.
