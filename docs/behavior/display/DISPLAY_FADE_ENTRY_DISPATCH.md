# Display fade entry dispatch

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `fadeEntry` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 80013368, `.elf` 2044 bytes, 469-line `.disasm`, 12 `bl`
(9 distinct same-TU targets + 3 absolute). `us-usage.json`: fadedrv.o (no
row for this spelling; the snapshot names it `fadeEntry` in
`src/driver/fadedrv.c:81`). Args: r3 = type, r4 = duration, r5 = color
pointer (incoming GXColor word stored through it during normalization).
No return value is constructed.

Jump tables and rodata below were read from the read-only ISO through the
DOL section map (same verified method as the SDA backfill).

## Normalization prologue (matches the snapshot line-for-line)

- type 0 → return immediately (tail epilogue, no stores).
- 16→9, 17→10, 18→11, 19→12, 20→14, 21→15, each rewriting the color word
  through r5 to 0xF0F0F0FF (six SDA white words, all read 0xF0F0F0FF).
- Types 1–2 and (3–5 or 6): duration < 1000 → duration = 1000 (signed).
- 7→3, 8→4.

## Dispatch tree on the normalized type

- < 1 (negative — 0 already returned): straight to the common tail.
- 1..6 and 9..15: slot-0 init block (below).
- 7..8: straight to the tail (no init).
- 16..21: unreachable (normalized away); fall into the tail range.
- 22..40 and 51: slot-1 init block.
- 41: slot-0 block; 42..48: slot-2 block; 49..50 and 63..64: slot-3 table
  (see jump tables); 52..62: slot-3 init block; 65+: tail.
- Table 0x80304860 (types 0..14): odd types → duration-default block
  (duration 0 → 2, plus [slotbase+164] = 2); even types → tail.
- Table 0x8030483c (types 52..58): 52,55,56,58 → slot-3 init; 53,54,57 →
  tail.
- Table 0x80304790 (types 22..64): 29 distinct small blocks plus tail for
  33 and 39. Each small block stores a ([96]=X, [100]=name-base,
  [104]=2) triple with X in 72..288 and one of five packed name-table bases
  (r2−31772/−31796/−31768/−31764/−31760, holding A_1/Z_1/Z_2/A_3-style
  entries read from the ISO). Full slot→type map: 22→80013848,
  23→80013864, 24/27/63→80013880, 25/28/64→800138b8, 26→8001389c,
  29/35→800138d4, 30/36→800138f0, 31/34/37/40→8001390c, 32/38→80013928,
  41→800137a0, 42→800137bc, 43→80013768, 44→80013784, 45→800137d8,
  46→800137f4, 47→80013810, 48→8001382c, 49/52→80013944, 50/53→80013960,
  51→8001397c, 54→80013998, 55→800139b4, 56→800139d0, 57→800139ec,
  58→80013a08, 59→80013a24, 60→80013a40, 61→80013a5c, 62→80013a78.

## Slot init blocks and common tail

Five 168-byte fade entries at [fadebase+8/+176/+344/+512/+680] (fadebase =
word at [r13+5888]); slot index 0..4 stored at [fadebase+848]. Each init
block: conditional releases of [slot+452]/[slot+112]/[slot+140] (via
0x8003b2d0/0x8003b1f4/0x800712cc when nonzero), `memset`-shaped
(0x800050b4 called with (slot, 0, 168)) clear, [slot+108] = −1, SDA
0.0/1.0 floats to +28/+32/+120/+124/+144, and a duration/flag word near
the slot base. Common tail (reached by every path): sign-extends duration
(`srawi`), stores halfword 0 marker, type, sign-extended duration, color
word, SDA floats to the slot, sets bit 0 of the slot's halfword flag,
skips the [slot+156] zero-store only for types 50/53 (`cmpwi` pair), sets
[type 34/40 → 3.0 float at +144], and routes types 22..64 (minus 33/39)
through the triple table above.

## Not established

Entry lifetimes, release-target ownership, name-index semantics, heap
allocator internals, cadence, and any on-screen fade. The snapshot stops
where the switch begins, so block roles rest on the listing alone.
