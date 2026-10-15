# Wander events and animation memory

The [field motion guide](field-motion.md) describes field simulation adapters;
[animation data](animation-data.md) describes clips and traversal. This guide
covers the retained wander event, cold initialization and recursive scratch
allocation. Hexadecimal numbers below are guest offsets/addresses unless units
or decimal counts are stated. [Behavior contracts](../behavior/README.md) retain
the recovered source evidence. Comments explain the current implementation;
source constants do not establish measured timing or native/Gekko numeric parity.

## One wander invocation versus a held child

[field_npc_wander_event.cpp](../../source/field/field_npc_wander_event.cpp)
implements original `urouro_init_func` / `urouro_main_func` at `0x80237878` /
`0x802372a8`. Its cursor is tied to the actual event pointer and ID, guest entry
address, function and provider owner. The caller must retain admitted actor/work
mappings through a hold. A provider request returning false has no effects;
its captured inputs survive retry, while completed calls/stores never replay.

Init completes with EVT result 2. Main's source result 0 ends **this invocation**
and clears the cursor, allowing the next scheduled invocation to reacquire its
inputs. An unavailable child also returns 0, but keeps the cursor inside the
same invocation. Treating those as the same case would either replay effects or
prevent the continuing event from progressing. Native stage numbers are cursor
labels; they are separate from the source four-phase word in the work block.

Init resolves name/actor first, then four float operands and flags. It requests
44 bytes from `_mapAlloc`, publishes the token through guest entry `+0xc4`
(LW10), reloads it and initializes fields in source order. The map allocator owns
alignment/zeroing and actual allocation lifetime; returning/deleting this event
does not imply freeing its work.

| Work field | Meaning |
| --- | --- |
| `0/4/8/c` | Flags / phase / unsigned count / remaining flips |
| `10/14/18` | Live frame budgets converted from 6000/9000/1500 milliseconds |
| `1c/20/24` | Anchor XYZ; Y read from actor after frame-service calls |
| `28` | Radius |

Init publishes actor speed `+0x1b0` and mask `0x00400000`. It does not allocate
a pose, move a root or clear movement components. Main freshly acquires Mario,
work and phase each source invocation:

| Phase | Work performed |
| --- | --- |
| 0 | Choose movement budget, round down to a signed multiple of 20, turn by `irand(60)-30`, enter 1 |
| 1 | Boundary/contact/wall turning, decrement count, then gated component/sound work |
| 2 | Choose idle budget and small turn; optionally choose 2–4 flips, enter 3 |
| 3 | Decrement count; on exact zero choose flip budget and either return to 0 or flip visual yaw |

Budgets use `signed budget/2 + irand(signed budget/2+1)`. The budget is reread
after the RNG callback. Counts wrap unsigned; zero-before-decrement is not an
expiry shortcut. Idle flips update visual yaw `+0x144` while retaining movement
heading `+0x1ac`. Other turns write yaw then read it back into movement heading.

| Packed actor field | Owner/value used by wander |
| --- | --- |
| `8c/90/94` | Root XYZ; supplied to math and sound |
| `104` | Actual pose handle for readiness/material queries |
| `144/1ac` | Visual yaw / retained movement heading |
| `1a4/1b0` | Delta seconds / speed |
| `1b4/1bc` | Produced X/Z movement components |
| `1d8/1dc` | Signed sound-toggle halfword / elapsed milliseconds |
| `1e0/1e4/1e8` | Two sound names / period halfword |
| `2fc` | Wall token; nonzero triggers the boundary-facing turn |

Phase 1 captures horizontal distance once, but invokes reached readiness/math/
touch services separately. Callbacks may mutate admitted memory between them.
Boundary flags masks 1/2 latch correction. Contact comes from the prior Mario
radial hint, not collision contact; both strict angle comparisons `<90` and
strict distance `<radius` must pass for avoidance. Equality stays outside that
branch. Count expiry happens before component and audio work.

Movement is capped at 20 milliseconds of speed: start with `speed*dt`, replace
with `20*speed/1000` when greater or equal. `dt` is seconds. Sin/cos inputs each
use a fresh heading and the recovered binary32 tau factor; results stay double
until source rounding. Components X/Z are `step*sin` / `-step*cos`. **NPC-main
consumes them later**; this event does not integrate roots.

Sound queries precede fresh period/speed/elapsed reads. Sound mask `0x100` with
material mask `0x1800` clear adds modifier `0x10000`. Strict
`elapsed > period/speed` triggers source-name lookup and the full 3D request
`(ID|mask, root, 255, 255, 0, 8)`. The 255 operands select default volume/pan; zero is delay and eight is retained
distance metadata for positional processing. Even a missing sound name reaches elapsed reset
and toggle. The toggle stores the signed low byte of `1-old` into a halfword,
not Boolean inversion. Final elapsed uses fused `1000*dt + elapsed` only when
the second sound name exists or toggle is nonzero; zero speed is not guarded.

[npc_wander.cpp](../../source/field/npc_wander.cpp) is the typed compatibility
surface. It retains the four-phase mechanics, source shared LCG, units and
readiness ordering, but has no resumable false-provider contract. Token/contact
are caller identities. Its raw turn-ready difference does not wrap at 360.
Packed event routing uses the retained adapter above; do not run an approximate
second wander simulation over the same actor.

## Cold animation initialization

[animation_initialize.cpp](../../source/display/animation_initialize.cpp)
implements the cold `animInit` parent, not attachment/reset of live pools. The
live work pointer W comes from `0x80415dc0`. Reloads and captures follow source
publication points; services can change W or counts. Reentry may orphan prior
allocations: no local cleanup, rollback, null-allocation rejection or blanket
pool clear exists.

| Count / pointer fields in W | Heap-0 request derived from current count |
| --- | --- |
| `4 / 0` | Initial count 64 × 16-byte resource rows |
| `c / 8` | Initial count 64 × 12-byte secondary rows |
| `14 / 10` | Initial count 256 × `0x170`-byte pose rows |
| `f0 / ec` | Initial count 1024 × 12-byte auxiliary rows |

Sizes wrap 32 bits and survive a held allocator. Returned pointers publish
through freshly loaded W. `e8=0` is interleaved before auxiliary allocation.
Explicit scalar stores preserve untouched fields, including alternative output
pointers `f8/fc`. Even after writing `104=0`, the implementation retains the
fresh null guard before requesting `0x180000` scratch bytes.

The tangent writer captures W, factor, that W's scratch pointer, then one atomic
64-bit bias; it publishes through that captured W. It emits 178 binary32 entries
for signed degrees -89 through 88 at `0x803d9580`; index 89 is zero. The PPC
integer-bias conversion and binary32 rounding points are explicit. Factor bits
`0x3c8efa33` mean `0.017453288659453392`, not exact pi/180. A held tangent/store
retains captures and never repeats a completed tangent call. There is no finite
result guard.

`a/vivian.bin` uses a real file handle and holder: capture destination word
`0x80415db4`, then dereference the captured holder for the first `0xc00` copy.
Reload holder/payload before the 16-byte tail copy to `0x80415da0`; then release
the captured handle. Null skips copies/free. Archive type zero precedes the
`a/ag2tg.bin` request, whose returned handle, including zero, publishes at fresh
W `+0x100` without immediate release. File/reference/archive lifetime belongs to
the provider, not a direct-read shortcut.

[animation_tangent.hpp](../../include/display/animation_tangent.hpp) generates
a shared legacy native table once per process using the same single-rounded
factor and widened-angle libm call. It does not prove original libm equality,
source initialization cadence or aliasing with live guest tangent storage.

## Scratch overflow and pose reconstruction

[animation_buffer_refresh.cpp](../../source/display/animation_buffer_refresh.cpp)
retains five source parents plus two internal traversal continuations. The
public second argument is a byte count for `testAlloc` and a **pose address**
for `animPose_AllocBuffer`. Active cursors retain the original request and
nested child; false preserves earlier mutations.

W `+104/+108` hold scratch base/cursor, `+10/+14` pose array/count, `+10c` battle
selection. Rows have stride `0x170`. Nonzero pose flags admit the row; battle
selection additionally requires nonzero pose `+c`, while ordinary selection
excludes exactly 1. Selection does not establish visibility. Counts and W are
read at original points rather than captured for an entire traversal.

| MODEL count × stride | Pose buffer pointer |
| --- | --- |
| `f0 × 12`, twice | `48/4c` |
| `f8 × 12`, twice | `50/54` |
| `13c × 1` | `58`, aliased to `5c` |
| `140 × 4`, twice | `60/64` |
| `12c × 24` | `68`, aliased to `6c` |

Model access follows resource row → handle → holder → payload. Reservations
round up to 32 bytes with unsigned wrapping, advance the cursor **before** the
strict unsigned `cursor < base+0x180000` check, and keep the original reserved
pointer. Exact-end equality overflows. The caller must not replace this with a
transactional allocator or ring overwrite.

Recovery differs: pose allocation first resets/restores selected poses; the
standalone allocator first rebuilds them inline. Later attempts reset/restore,
then refresh and recursively allocate. Rebuild deliberately writes pose `+3c=-1`
twice. Nested cursors retain completed child effects and report the deepest
pending operation. Depth above 64 is a native stack-admission hold, not original
success or an original recursion limit. These helpers do not prove production
shared heap/output ownership or completed animation startup.
