# Retained animation Main evaluation

The four portable stages implement contiguous regions of the original nonnull
Main body: [time](../../source/display/animation_main_time.cpp),
[timeline](../../source/display/animation_main_timeline.cpp),
[replay](../../source/display/animation_main_replay.cpp) and
[interpolation](../../source/display/animation_main_interpolation.cpp).
The composed Geometry cursor retains successful earlier stages through later
holds. It does not acknowledge Main's resource resolution, linked recursion,
platform setup or optional facing tail. See [memory ownership](animation-memory-ownership.md)
and [selection/restart](animation-selection-ownership.md).

R means the captured pose, D the loaded MODEL and T the timeline. Field offsets
and guest addresses below are hexadecimal; byte sizes and counts are decimal.
Every stage pins its input and provider identities. A false controller result
can follow committed stores or a cooperative yield. Preserve the cursor and
producer pass; restarting cannot undo earlier source writes. Primitive
unavailability, in contrast, acknowledges no effects for that operation.

## Time conversion preserves stores and numeric boundaries

Time captures mode and loop selector before clock work, reads GP then hardware
bus clock, and divides the selected signed-64 clock by the original derived
millisecond divisor. The epoch subtraction wraps modulo 64 bits; its result
then converts as UNSIGNED to float. Underflow can become a large positive value.
There is no elapsed-time clamp or render-driven increment.

Source 60 and 1000 convert that value to authored animation frames. The code
publishes R+20, reloads it, applies fresh R+44 local rate and publishes again.
The source literal ZERO is captured between the multiply and division. These
reads, stores and binary32 rounding points are separate observable boundaries.
Changing rate affects elapsed time rather than an accumulated animation phase.

A fresh control pointer supplies END then START, while the initial loop selector
and earlier ZERO stay captured. Zero span selects start and publishes offset
rather than a ratio. Negative looping offset selects start. Nonloop selection
clamps endpoints, but the published ratio can exceed one. The looping conversion
loads a whole double bias, publishes/reloads ratio, then calls truncate. Signed
integer reconstruction uses the actual captured bias, followed by the original
single fused subtract/negation helper. Reversing FMA operands can lose exact -0.
Selected R+24 publishes last. These are source arithmetic operations, not a
claim of complete Gekko FPSCR, exception or NaN-payload emulation.

## Timeline scans retain independent captures

Continuous keys are 44 bytes, with count/base at T+8/28. Discrete rows are
8 bytes, with count/base at T+20/40. Each scan captures its own selected R+24
once. Only ordered keyTime>selected breaks; equality and NaN continue.
Nonpositive signed counts still read selected time and table pointer.

Each scan publishes predecessor=index-1, rereads it, then publishes successor.
Nonloop upper clamps use fresh counts and predecessors. No lower clamp is
introduced: a predecessor -1 reaches actual pre-table memory when the state or
key is read. Missing backing holds rather than substituting state zero.

The fraction numerator is captured before a possible loop-wrap store. On wrap,
successor becomes zero and fresh current/control/table/END form the denominator;
the old numerator remains. A fresh ZERO comparison chooses the literal on exact
equality, otherwise single division. Scan backedges yield cooperatively without
adding a source count cap.

## Replay expands five cumulative streams

Replay captures current/old/next/fraction, then all five MODEL counts before
branching. Old=-1 or signed current<old rebuilds caches from bases and replays
keys 0..current; forward movement applies old+1..current. Equal current/old
skips replay and can reset old fraction on a reversed fraction. Final old-key
and fraction publication belongs after interpolation.

| Channel | MODEL count/base | Pose cache | Key offset/count | Timeline stream | Stream/cache row bytes |
| --- | --- | --- | --- | --- | --- |
| Position | f0/154 | 48 | 4/8 | 2c | 4/12 |
| Normal | f8/15c | 50 | c/10 | 30 | 4/12 |
| Visibility | 13c/1a0 | 58 | 1c/20 | 38 | 2/1 |
| Node | 140/1a4 | 60 | 24/28 | 3c | 4/4 |
| Texture | 12c/190 | 68 | 14/18 | 34 | 12/24 |

Sparse indices are unsigned cumulative byte increments within each channel/key.
Vector deltas capture all three GQR4 loads before the first destination read.
Main sets GQR4 to signed8 scale0; the raw child requires that actual owner rather
than assuming a decoder. Replay uses FMA(scale, delta, old) with source 1/16.
Visibility and texture selector additions wrap bytes. Texture float+4/+8 use
unscaled additions, preserving other row fields. The +8 read order differs for
bulk groups and remainders; aliases make the difference observable.

Copy lengths and pointer arithmetic wrap 32 bits. Word-copy runs forward and
its shifted zero count still enters the do/while body before wrapping. Byte-copy
returns on zero and copies backward whenever source<destination. A held store
retains its loaded value. The optional bulk provider may decline only without
effects, before the scalar copy begins. Derived zero loop counts after admitted
ranges can represent 2^32 entries; cooperative yielding does not turn them into
empty success. Source pathologies remain distinct from native admission limits.

## Interpolation consumes the captured replay result

Equal current/next copies position, normal and node caches to output. Increasing
indices copy bases then apply next-key vectors and cubic node updates. Decreasing
indices, including the loop seam, preserve old output. Visibility and texture
have no interpolation copies in this region. A zero MODEL count suppresses a
base copy, not a nonempty stream or its headers.

The next-key pointer survives earlier copies; node duration alone reloads the
time table. Vector FMA order is delta first, scaled fraction second, old third.
Equal-key node setup reads output before cache; increasing-key setup reverses
those reads. Do not homogenize the observable graph or replace captured output
pointers with current pose fields.

A node reads outgoing byte3 before sparse byte0, and advances output even when
outgoing >=89 or <=-89 rejects it. Accepted incoming byte2 has no source bounds
check. Tangents use 0x803d9580+0x164+4*signedAngle, so degree zero is row89.
Out-of-table incoming angles can reach adjacent mapped work bytes. Missing
backing is a hold, not a host-table clamp. Constants and tangents reload per
admitted entry, with explicit single operations and ordered FMAs for cubic terms.

All branches publish captured current to R+3c then fraction bits to R+40.
A hold between those stores must not replay geometry or the first publication.
Only afterward can the parent reach its optional facing tail. The retained
[time](../behavior/display/RETAINED_ANIMATION_MAIN_TIME.md),
[timeline](../behavior/display/RETAINED_ANIMATION_MAIN_TIMELINE.md),
[replay](../behavior/display/RETAINED_ANIMATION_MAIN_CACHE_REPLAY.md) and
[interpolation](../behavior/display/RETAINED_ANIMATION_MAIN_INTERPOLATION.md)
contracts contain the detailed source evidence and native-domain limitations.
