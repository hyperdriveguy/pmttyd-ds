# Overworld speed, fall, landing and flag leaves

US G8ME01 disc0 revision0; static contracts, no execution, timing, or
gameplay claim. Closes ten rows of the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md): `movementSpeed`,
`walkSpeed`, `dashSpeed`, `mario_width`, `bottomlessOn`, `bottomlessPos`,
`checkLand`, `bero_length`, `fallSpeed`, and `marioGetFallSpd` (the last two
are duplicate extractions of one body — see below).

Common static facts: every body opens with `marioGetPtr` (0x8005e0c0, size 8,
[mario-pointer helper](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md))
and operates on the returned Mario pointer. Small-data (r2-relative) float
constants are cited by displacement only — their values are not recoverable
from relocatable objects. The 3025 (55²) stick-radius threshold below is the
dash/walk boundary in [Mario movement](../overworld/OVERWORLD_MARIO_MOVEMENT.md)
(strictly greater = dash, equality = walk); these bodies implement that
same comparison, and the field stick bytes Mario+594/+595 are the signed8
stick axes in that doc's offset table (+0x252/+0x253).

## movementSpeed (80095a8c, 33 lines): 0/1/2 stick classifier

Returns an integer in r3: 0 if either stick byte is zero (`extsb` + zero
branch on each); else x²+y² of the signed bytes (`mullw`/`add`, exact for the
±128 range); 1 if the sum ≤ 3025 (`cmpwi`, GT-clear branch), else 2. No
writes, no further calls.

## walkSpeed (80087d84, 45 lines) and dashSpeed (80087ce4, 46 lines)

Both return a float in f1: base × optional SDA scale × [Mario+552].

- walkSpeed: base = float at [Mario+388].
- dashSpeed: base = float at [Mario+392] when [Mario+0] bit 11 (0x00100000)
  is clear; when set, it reloads the **walk** base [Mario+388] and runs the
  stick test instead.
- Scale gate A (bit 11 set): if stick x²+y² > 3025, multiply by the SDA
  constant at r2−27572, resolved to 0.5 (`0x3F000000`) by the register's SDA
  resolution (r2 = 0x80427440).
- Scale gate B (bit 11 clear): if `marioBgmodeChk()` (0x8005c164, returns
  (Mario+0>>25)&1 per [party driver census](../partner/FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md))
  == 1, multiply by the same 0.5. Both gates therefore provably halve the
  base, re-deriving the movement doc's "background mode query equal 1 halves
  the selected base" from the binary rather than assuming it.
- Tail (both): multiply by float at [Mario+552], return in f1.

Bit 11 (mask 0x00100000, i.e. LSB 20) is `MARIO_FLAG_PAPER_MODE` (1<<20),
per `mario.h:74` of the pinned decomp snapshot — correcting this doc's
earlier "distinct from Paper-mode" note, which was wrong. (The 0x00200000
value in [Mario movement](../overworld/OVERWORLD_MARIO_MOVEMENT.md)
disagrees with both that header and the binary; left for that doc's owner.)

## mario_width (800916f0, 50 lines): +448 slew limiter

Adjusts the float at [Mario+448] toward a reference by SDA steps; all
comparisons are `fcmpo` with LT/GT-bit branches:

- Bit 11 of [Mario+0] set: candidate = [448] + 1.0 (SDA r2−27516, resolved
  by the register); reference = [Mario+440] if bit 7 is set, else SDA
  constant 18.0 (r2−27360);
  [448] = min(candidate, reference) — the store-then-conditional-overwrite
  sequence keeps the candidate unless it exceeds the reference.
- Bit 11 clear: slew [448] toward 6.0 (SDA r2−27300) by ±1.0 per call, clamping
  exactly at 6.0 from either side.

The meaning of [448]/[440] is not established here; the three constants are
resolved above via the register's SDA map.

## bottomlessOn (80096d34, 16 lines) and bottomlessPos (80096d5c, 40 lines)

- bottomlessOn: stores 1 to [Mario+736]. Complete setter, no reads.
- bottomlessPos: stores incoming float args f1/f2/f3 bit-patterns to
  [Mario+740/744/748]. The three words loaded from absolute 0x802C3EB4 are
  **dead**: each stack slot is overwritten by `stfs` before its `lwz` read,
  so the table contributes nothing to the stored triple. Offsets are
  positional; no prose meaning is claimed for 736–748.

## checkLand / marioChkLandon (8008dc70, 43 lines): three-way dispatch

`us-symbols` names 8008dc70 `marioChkLandon` (mario_hit.o, 148 bytes); the
[fall integration](../overworld/OVERWORLD_AIRBORNE_AND_LANDING.md) cites this
address as the landing query called with updated V and a height-output
pointer. Incoming f1 is forwarded as the float argument in all three routes;
r3 = Mario pointer; r4/r5 = two stack output slots. Return value is the last
callee's r3 (no local result).

- Byte at [Mario+60] (== +0x3c, the doc's "transform byte") == 2 →
  `marioLandonChk_old` (0x8008d4a0) with (Mario, f1). The doc's "char 2
  selects its own landing query" matches.
- Else if [Mario+0] bit 7 (0x01000000) set → `marioSearchGroundRoll`
  (0x8008de0c, mario_hit.o) with (Mario, f1, f1, out, out).
- Else (flag clear) → `marioSearchGround` (0x8008e694) with the same shape —
  matching the doc's clear-flag route with D=V and vertical input=V.

Naming note: the prose calls the set-flag selector "marioSearchGround4";
the symbol table names 0x8008de0c `marioSearchGroundRoll`. The discrepancy is
recorded, not resolved.

## bero_length / evt_bero_get_info_length (8007a76c, 67 lines)

`us-symbols`: `evt_bero_get_info_length` is cited by address in [Rogueport
roads](../overworld/OVERWORLD_ROGUEPORT_ROADS.md) ("length-1 resolves 30
original units"). Event callback, returns 2. Order: `evtGetValue(arg0)` →
index; entry = word at [0x803DC110 + 4*index] (absolute table base from
`lis`/`addi`); then:

- If word at [entry+24] != −1: flip its high bit (`xoris` 0x8000), convert to
  float with the 0x43300000-bias double trick against the table double at
  0x802C3058, and `evtSetFloat(entry, raw argword1, result)` (0x800304bc,
  complete /384 in [float routing](../event/EVENT_FLOAT_OPERAND_ROUTING.md)).
- Else ([entry+24] == −1): take low 12 bits (`lhz` + `clrlwi`) of [entry+4];
  select SDA constant K_a (r2−28456) when the value is 0, 3, or 3840, else
  K_b (r2−28448); `evtSetFloat(entry, raw argword1, K)`.

The `b 0x801063cc`-style filler is absent here; both paths converge on the
float call. Destination is the raw second event word in both paths.

## fallSpeed / marioGetFallSpd (80094098, 43 lines, one body, two names)

`diff` shows the listings differ only in the header line: same entry, same
43 lines, one `bl` (marioGetPtr). Only `marioGetFallSpd` has a
`us-usage.json` row (mario_motion.o, 148 bytes, 9 link sites). The body is a
three-lane float integrator over [Mario+124/128/132/136] with SDA clamp
constant K (r2−27572 — the same slot as `mario_width`'s step) and a data
table at [r13−28176] (cited positionally):

1. [132] = min([132] + [136], K).
2. [128] += [132]; [124] += [128].
3. If [124] ≤ float at [table+36] (compared with the `cror`-assembled ≤
   test): [124] = that float and [132] = [128] = [136] = K.
4. Return [124] in f1.

The [airborne doc](../overworld/OVERWORLD_AIRBORNE_AND_LANDING.md)'s
terminal-cap-and-zero reset in `marioFall` is a related pattern, not this
body. No claim about which lane is velocity/position/accumulator beyond the
update order above.

## Not established

Unresolved SDA slots (fallSpeed clamp r2−27296, hit const r2−28784, bero
K_a/K_b r2−28456/−28448), table contents, offset meanings not linked above, call
cadence, and any in-game effect. The duplicate extractions are reported as
observed facts; neither name is declared canonical.
