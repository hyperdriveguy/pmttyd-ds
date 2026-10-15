# Retained seven-row weapon subicons

[The subicon parent](../../source/battle/battle_sub_icon_continuation.cpp)
retains the actual current-battle/registry lookup even though its unit result is
unused locally. Work+0x98 supplies scroll. The first fctiwz child uses captured
initial scroll, then captures work+0 cursor and computes a fixed fractional offset.
Each of seven physical rows separately reloads scroll and cursor count and invokes
its own fctiwz. Reusing the initial conversion would change source-visible behavior.

The signed conversion constructs double bits `0x43300000:(n XOR 0x80000000)`,
subtracts the source double bias 2^52+2^31, then rounds to float. Fresh r2-relative
constants use base 0x80427440: offsets -20416/-20284/-20296/-20500 give spacing
24, base 35, X 100 and Z 0. Bias words are read high then low at 0x802ef780/784.
These literal values were checked in the read-only original DOL; they are not
measured animation or hardware constants.

A row is reached only if signed wrapped `(truncated fresh scroll + row) < count`
and word(work + 0xa4 + 4*row) has bit 1. It computes Y as a source FMA, negation and separate
float add: `-fma(24,row,-35) + initialFraction`. It then requests translation and
concat with the actual supplied row matrix. Do not algebraically fold those
operations or precompute every row ahead of holds.

The copied input and borrowed output address must remain identical, including
optional-value bit patterns. The actual current predicate guards lifetime; the
caller freezes mapped memory within synchronous read groups. Missing matrix or
private label/icon/enabled/cost/BP values hold only when their source consumption
is reached. A real zero in any optional is known data, not absence.

Finally the parent copies one complete DrawSubIconSub request, function 0x8011847c
at source site 0x80119170. Holds retain its payload and do not repeat conversion,
math or completed rows. That child owns all partial raster effects. Its engaged
zero acknowledges completion; only `nullopt` holds. Parent completion does not
acknowledge the whole queued weapon draw or native ordered GPU batch. See
[queued weapon source ownership](../behavior/battle/RETAINED_QUEUED_WEAPON_DRAW.md).
