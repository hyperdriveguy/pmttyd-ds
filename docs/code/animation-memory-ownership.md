# Animation pool, tangent and scratch memory

[Pool aliases](../../source/display/animation_pool_memory.cpp),
[tangent aliases](../../source/display/animation_tangent_memory.cpp) and
[scratch consumers](../../source/display/animation_scratch_memory.cpp) keep source
RAM authoritative. Native vectors are presentation snapshots. See
[animation data](animation-data.md), [cold services](wander-animation-services.md)
and [selection](animation-selection-ownership.md).
Offsets below are hexadecimal; capacities and byte counts are decimal.

## Existing pool allocations become aliases

The source root `0x80415dc0` supplies W and its model/texture/pose pointer-count
pairs. Required capacities and strides are 64×16, 64×12 and 256×`0x170`; W spans
`0x110` bytes. Six big-endian header words form the 24-byte layout guard.
The bridge reads four complete existing RAM allocations, refuses null/wrapping/
overlapping ranges and overlap with the root cell, and compares current typed
model/texture/pose bytes. Exact range end 2^32 is admitted. Missing inactive pose
objects require zero source bytes, not a fabricated allocated pose.

On first W transfer, previously unknown work bytes can be adopted, but the typed
`W+0xe8` history must match. Later attachment requires full W equality. Only after
all RAM extractions succeed are missing stable pose objects created. Failed
extraction restores earlier spans. Failed row registration removes new aliases,
restores RAM/work and removes only newly prepared objects. The source guard is
published after the complete mapping succeeds.

`matchesWork` checks current root and pointer/count header; it is not an arbitrary
mapping-generation detector. Callers retain exclusive ownership of the actual
external rows and keep VM/maps quiescent during attach/detach. Detach copies
CURRENT pool/work bytes back to RAM, not the original saved image. That snapshot
serves bind rollback only. Guard removal and mapping replacement are teardown
operations, not source Release or proof of a freed resource. See
[pool contract](../behavior/display/NATIVE_ANIMATION_POOL_MEMORY.md).

## The same live tangent bytes

The table at `0x803d9580` has 178 BE binary32 words for signed degrees -89..88;
row 89 represents zero. Binding requires the same source-attached pool/VM and no
competing tangent owner. A retained CPU table must match the existing source
bytes. Otherwise prepare a table, extract actual RAM and register its alias
before publishing ownership; failed registration restores RAM. Binding never
reruns tan or regenerates a fallback with native libm.

Detach preserves current words in both CPU storage and RAM. Tangent aliases
retire before array aliases and the VM. Mapping replacement by unrelated owners
is outside that lifetime contract. A source-backed consumer cannot substitute a
legacy table when its live tangent owner is absent. See
[tangent contract](../behavior/display/NATIVE_LIVE_ANIMATION_TANGENTS.md).

## Five scratch channels

The sampler follows actual model cache → file entry → holder → relocated MODEL
→ descriptor → timeline. Its finite native domain excludes ring output (`W+e8`
nonzero), linked pose (`P+90 != -1`), malformed file extents and unsupported
geometry/tangent domains. These are native admission checks, not original Main
guards. Zero-sized unused pointers are ignored.

| Channel | MODEL count/base | Pose cache/output | Row bytes / compressed stride |
| --- | --- | --- | --- |
| Position | f0 / 154 | 48 / 4c | 12 / 4 |
| Normal | f8 / 15c | 50 / 54 | 12 / 4 |
| Visibility | 13c / 1a0 | 58 / 5c | 1 / 2 |
| Node | 140 / 1a4 | 60 / 64 | 4 / 4 |
| Texture row | 12c / 190 | 68 / 6c | 24 / 12 |

Visibility and texture cache/output must alias. Other channels have separate
buffers. Up to eight guest ranges must fit between the scratch base/end
pointers stored at W+`0x104/108`, within `0x180000` bytes, with alignment and
disjointness checks. Counts must match original static geometry. Private backing ownership
remains caller-owned; later layout checks cannot roll back an adversarial alias.

Continuous keys are 44 bytes, discrete rows 8 bytes. Native limits are 1..1024
continuous keys, 1..65536 discrete rows, loop 0/1 and finite ordered endpoints.
Caller clocks are source-domain millisecond quotients. Explicit float rounding
converts elapsed time through 60 Hz, /1000 and pose rate. Pose time/index writes
can precede a later failure; evaluation is not a transactional whole Main.

Sparse byte zero advances an unsigned relative target index. XYZ deltas are
signed bytes in 1/16 units through FMA. Visibility and texture-selector additions
wrap bytes. Texture updates change byte zero and floats +4/+8 only, preserving
other padding/transform fields. Node interpolation uses current live tangents;
outgoing angles outside (-89,89) skip cubic, while reached incoming angles must
fit -89..88. GQR, arbitrary floating exceptions and original numeric parity are
separate requirements.

Cache predecessor -1 or a backward key resets bases and replays 0..current.
Forward motion replays only crossed keys. Position/normal/node output copies
cache and optionally interpolates the next key; equal keys copy only. At the
loop seam current > next, previous output stays. Scratch dereferences are
preflighted before buffer writes inside one synchronous raw-RAM borrow, without
yielding source children. Post-write owner/layout checks can refuse completion
while earlier source writes remain.

Readers decode current BE output at their actual usepoint. A source-view failure
never falls back to detached vectors. Retirement snapshots only matching owner
control block and pose generation with valid layout; failed snapshots leave an
unavailable weak view. The view descriptor does not retain the VM/context alive.
See [scratch contract](../behavior/display/NATIVE_SOURCE_ANIMATION_SCRATCH.md).
