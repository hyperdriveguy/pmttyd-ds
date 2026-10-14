# Star-power gauge storage (AP add/set leaves)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the storage half of STAR-01 todo 1 (`docs/requests/STAR-01.md`):
the star-power gauge lives at pouch+122/+124 (`currentSP`/`maximumSP`,
+0x7A/+0x7C in the snapshot `PouchData`), written only through these two
leaves. Fill sources, thresholds, and consumers stay open.

Fingerprints: `pouchAddAP` entry 800d3c00 (23 lines, `.elf` present);
`pouchSetAP` entry 800d3bc4 (21 lines). `us-usage.json`: mario_pouch.o,
68/60 bytes, fingerprinted; AddAP has 2 direct link sites (owners not
expanded in evidence — open). `evidence.json` pins both sha256 rows.

## pouchAddAP(delta): add with two-sided clamp

[122] += delta; negative results floor to 0; results above [124] cap to
[124] (`bclr`-return when in range on each side, store on breach — read
branch-if-false carefully: fallthrough stores). No other validation; the
pouch pointer is reloaded per phase rather than retained.

## pouchSetAP(value): raw store with the same clamp

[122] = value outright, then the identical floor-0 / cap-max sequence.
Unclamped intermediate values are never observable (single-threaded stores;
no hook between write and clamp).

## Not established

Fill callers (the 2 link sites), threshold/level semantics, max-SP writers,
save mapping, audience/stylish linkage, and any gameplay effect. Do not
present AddAP/SetAP as the fill policy — they are the storage mechanism.
