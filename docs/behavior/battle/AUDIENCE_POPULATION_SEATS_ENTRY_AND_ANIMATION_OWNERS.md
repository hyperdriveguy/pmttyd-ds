# Original audience population, seat entry and animation ownership

US G8ME01 disc0 revision0, read-only static source inspection,2026-10-14.
Implementation: `battle_audience_population.hpp/cpp`. This extends the
[initial population audit](AUDIENCE_SETUP4_INITIAL_POPULATION_AND_POUCH_GATE.md)
and preserves the [verified empty setup4 graph](AUDIENCE_SETUP4_ZERO_COUNT_EXACT_OPERATION_ORDER.md).
Constants are source data, not Dolphin measurements. Whole local bodies and
nine data spans are pinned by `tools/verify_audience_population.py` (17 bodies,
including the existing supplied rand/irand providers and address getters).

## SettingAudience801a60c4/a4c

Captures battle for override words, audience owner, fresh battle's setup pointer
and pouch, then copies all65 level values. Rank0..3 publishes50/100/150/200;
other ranks retain capacity. A fresh pouch8c zero overrides capacity0. Retained
pouch level selects min(level-1,64); negative local index remains unavailable.
Baseline, desired override upper clamp or the two original random10 requests,
fresh second audience amount and single FMA remain ordered. Setup0/1 publishes
extra0;4 uses original ratio;2/3/5/6 uses capacity-desired; other modes retain
extra. Weight selection rereads setup mode.

Mode2 selects types1/7;5 selects1;6 uses original paired flags to select weights
2/1. Default branch preserves cardinality thresholds,24 swaps, successful
nonzero-weight admission, signed setup bytes, fresh byte after each random10,
paired doubling and type override. Override1..12 maps to original types
0,10,8,4,7,2,1,3,5,6,9,11; >12 retains the last scanned type as source r28 does.
It never substitutes an arbitrary category.

The original signed truncated desired+extra is clamped below0 and above
capacity. Allocation uses wrapping multiply and signed divide, paired truncation
and doubled physical-seat accounting. Zero divisor/int-overflow division remain
unavailable provider domains. Original index-list sorting compares the original
weight array; equal pairs request random2. Residual allocation cycles the
resulting indices and stops after12 paired single-seat remainder failures.
The optional guest branch consumes random20 before its battle40000 test, runs
24 fresh swaps and replaces one allocated member with type12. Paired replacement
does not invent a second guest or adjust the retained source allocated count.

Mode2 repeatedly scans its rank seating table uniformly and enters left-half
slot type7/right-half type1 until actual allocator-1, independent of a guessed
capacity. Other modes expand13 allocation counts, consume150 pairs of
random(length), then call front/back allocation and Entry in source order.
Length0 still requests random0 twice150 times; unwritten stack swaps have no
later observable read and no members are manufactured. Entry rejection0 still
advances the list. Only allocator-1 ends it early. Final12 original weights
publish after entry. The typed list capacity is200; no stack overflow or invalid
float conversion is replaced with successful population.

## Seat allocation and placement

NoUsedFCHaitiRand801a21a0/40 uses the retained cached audience count at13784:
>=20 selects back directly; otherwise front, with back fallback only for-1.
It does not recalculate this count after every Entry. Weighted front/back whole
bodies801a21e0/200 and801a23e0/200 copy their respective10 row and20 column
weights; rank table is8039cc20(front) or8039cb80(back),40-byte stride. Eligible
seats lack occupied1 and reserved100; getters requery the current battle for
each flags check. Weights are row*column+1 with source cumulative ordering.
Raw rand is requested even for an empty candidate list. Draw uses separately
rounded single multiply/divide by original32767 float and supplied truncation.
Exact raw32767 can reach the exclusive cumulative endpoint and return-1 even
with free seats. Uniform801a25e0/f4 uses back table and random(count), with no
random request on an empty list. Original immutable table widths0..20 are the
admitted array domain; invalid tables do not fabricate indices.

Entry801a3400/1d0 captures its record before raw rand. Type low8 selects original
paired flag; positive odd paired slots reject before occupancy. Both occupancy
checks use the captured flags. Accepted single/pair writes5/85 before raw rand,
then source column*25 and integer jitter(rand*10/32767) into X48/b4. Paired X
retains separate12.5/column/230/jitter/5 float operations. Sub801a31a8/258
requeries stage/current record, publishes original appearance/type/item-1,
profile animation table and forced animation1. It resets only authored fields,
uses actual stage+b34 for Y/b8, source FMA for row/column Z/bc, special type11
Z+.2, appearance11 Y-2000, copies source transform templates, sets flip270 and
central-seat bit8 ranges4..15,24..35,44..55. Paired Entry reserves captured
record's neighbor101 only after Sub. There is no generic memset or invented
actor schema. Callers supply original valid slot/type address domains.

SetAnim801a30ac/ac ignores absent/reserved records; same nonzero animation
with force0 retains progress. Otherwise it copies original pointer to24/28,
resets timer2c, three loop counters34..3c and depth40; frame byte1a is retained.
The existing [display interpreter](../display/BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md)
advances these original scripts; no copied-frame animation cadence is invented.
GetAnimEnd801a3064/48 checks opcode-1 and the actual cntlzw/rotate timer result:
zero or MSB-set timer returns1, other positive timers0. ChangeStatus801a2a24/104
routes reserved member to its previous slot recursively, resets only source
color/transforms/counters/motion and writes state19. Delete801a3158/50 clears
flags and paired neighbor flags only, retaining the rest of each record.

## Immutable data and native binding limits

The182 original `battle_audience_kind_data.o` sdata symbols occupy8041c638/14e8
through8041db20, SHA256
`27efa23577efb6deb3204baf406fdda61546d5700ec4c25f04e8b59fad724cc4`.
Exact scripts/item tables now enter both harbor and trio private PMEVT packs.
The general mutable sdata tail remains excluded; native globals are not imported.
No story text, frame indices, delays, loops or item weights are rewritten.

Trio's real Init Setting provider now invokes the general producer with inherited
RNG, raw rand and finite single conversion. A supplied canonical pouch8c=0 still
has the original empty setup4 result. The explicit positive test publishes8
members and original animation pointers, advances them through the existing
interpreter and resolves resulting original frame indices in the prepared atlas.
Whole Init reaches correct count/window publication and original known-key noise
request. Native nonempty startup still stops at existing Sound1/8019fcec: actual
positional stream creation is unbound. No known-key-1 failure, voice or completion
is substituted. Active audience state-control, positive target adjustment, GPU
texture/draw binding, source story pouch8c admission and throw interruption remain
separate unfinished work. This is not proof of a populated live battle display.
