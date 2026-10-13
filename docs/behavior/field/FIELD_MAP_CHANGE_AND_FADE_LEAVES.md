# Map-change init and fade-mapchange leaves

US G8ME01 disc0 revision0; static contracts, no execution or gameplay claim.
Closes `mapChangeInit` and `evt_fade_mapchange` in the
[unaddressed-functions register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Both bodies are corroborated instruction-for-instruction by the pinned JP
decomp snapshot (`src/sequence/seq_mapchange.c`, `include/mario/mariost.h`);
snapshot member names are cited with that JP/US caveat, but every address,
constant, and call target below was re-verified in the US listings. Callee
identities: `strcpy` 0x8026d5fc, `strncpy` 0x8026d5b8, `strcmp` 0x8026d464
(MSL), `psndENVOff` 0x800d76b8 (pmario_sound.o, positional — no prose contract),
`badgeShop_bargainGeneration` 0x801fa1c8 / `badgeShop_bottakuruGeneration`
0x801faa50 (evt_badgeshop.o), `evtGetValue` 0x80031260 ([integer
operands](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md)).

## mapChangeInit (8000874c, 85 lines): next-map buffers and shop-gate flag

`us-symbols`/`us-usage` name it `seq_mapChangeInit` in effect (sequence
driver init for map change; the snapshot spells it `seq_mapChangeInit` at
`seq_mapchange.c:85`). Incoming r3 (retained in r30) is the sequence work
with mapName at +8, beroName at +12. Full order:

1. `strcpy` the SDA empty string (r2−32568) into three 0x20-char buffers:
   [r13+5832] (area — receives the 3-char copy below), [r13+5800] (map),
   [r13+5768] (bero). Buffer roles follow from step 2 and the snapshot's
   `_next_area/_next_map/_next_bero`.
2. If mapName ([r30+8]) nonzero: `strncpy(area, mapName, 3)` and
   `strncpy(map, mapName, 32)`. If beroName ([r30+12]) nonzero:
   `strncpy(bero, beroName, 32)`.
3. `psndENVOff(0x200)` then `psndENVOff(0x201)` (512/513).
4. Three `strcmp`s: gp (word at [r13−28496]) +300 = `currentMapName`
   (+0x12C in the snapshot header) against "" and "gor_01" (absolute
   0x802CF2D4), and next-map buffer against "gor_01".
5. If all three are nonzero: set bit 0 of the halfword at
   [[r13+7624]+276] (`ori`/`sth`; [r13+7624] is the snapshot's `bdsw`,
   +276 = +0x114).
6. Else, if the next-map compare is zero (equal "gor_01") AND the flag bit
   is set (`lhz` + `clrlwi.` low-bit test, taken-equal skips): call both
   badge-shop generators, then clear exactly the low bit (`rlwinm` keep
   16..30 + `sth` = &= 0xFFFE on the stored halfword). Any other combination
   leaves the flag untouched.
7. No return value is constructed (falls to the epilogue with stale r3).

## evt_fade_mapchange (80135c38, 71 lines): map/area fade parameter block

Event callback, returns 2. Resolves five event words (arg0..arg4) through
`evtGetValue`, then stores the non-−1 lanes (−1 = per-lane skip sentinel)
into gp fade fields ([r13−28496] + offsets from the snapshot header):

- Resolved arg0 != 0: arg1 → +256 (`mapFadeInType` +0x100), arg2 → +260
  (`mapFadeInDuration`), arg3 → +248 (`mapFadeOutType` +0xF8), arg4 → +252
  (`mapFadeOutDuration` +0xFC).
- Resolved arg0 == 0: the same four values go to +272/+276/+264/+268
  (`areaFadeOutType/Duration`, `areaFadeInType/Duration`).

So arg0 selects the map-fade versus area-fade parameter block. No calls
besides the five resolutions; no validation of the stored values.

## Not established

Buffer/flag lifetimes, `bdsw` identity beyond the snapshot's extern, sound
and shop internals, gp provenance, cadence, and any on-screen transition.
The "gor_01"/"" literals are snapshot-corroborated, not re-read from the US
binary's rodata here.
