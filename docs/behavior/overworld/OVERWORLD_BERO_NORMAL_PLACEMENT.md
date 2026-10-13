# Bero normal entry placement

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `bero_normal` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 80079b74, `.elf` 976 bytes, 202-line `.disasm`, 11 `bl`
(5 distinct absolute targets + 6 same-TU). No `evidence.json`/`us-usage.json`
row. Callee identities: `marioGetPtr` 0x8005e0c0, `evtGetValue` 0x80031260
([integer operands](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md)),
`evtGetFloat` 0x80030840 ([float operands](../event/EVENT_FLOAT_OPERAND_ROUTING.md)),
`hitObjGetPos` 0x8001449c, `hitCheckFilter` 0x8001577c (hitdrv.o, 296 bytes,
positional), `sin` 0x80272410 / `cos` 0x80271ea8 (MSL double, `frsp`-rounded
per [axis rotation](../display/ANIMATION_SDK_AXIS_ROTATION.md)),
`bottomlessBero` 0x80096de4 and `bottomlessOn` 0x80096d34 ([bottomless
chain](OVERWORLD_BOTTOMLESS_EVENT_CHAIN.md)).

SDA constants below were read from the read-only ISO through the DOL section
map (r2 = 0x80427440; method self-checked against five register-resolved
slots, all exact): −28504 = 0.0, −28508 = 360.0, −28464 = 0.125,
−28460 = 180.0, −28512 = 2π (6.2831854820251465), −28468 = 1000000.0,
−28456 = 30.0, −28472 = −1.0. The −1.0 load is dead (f5 is reloaded with
30.0 before any fp use); the 34464 `cmplwi` result is likewise unconsumed
except by its own branch.

## Setup: six int→float lanes

Entry retained; arg block = [entry+24]. `evtGetValue([block+0])`,
`evtGetFloat([block+4])` → f31 (preserved for the whole body). Six
consecutive integers at [entry+180]+0..20 are each flipped (`xoris`
0x8000) and converted with the 0x43300000-bias double pair against the 2^52
constant at 0x802C3058, into stack lanes. The `(first − 0x10000) == 34464`
unsigned test (`addis`/`cmplwi`, consumed 33 instructions later by the gate
branch — no CR0 setter intervenes) selects a
`hitObjGetPos([entry+168], lane0)` call. No null guards.

## Zero gate and trig section

If `fcmpu(0.0, f31)` is equal, control skips to publication: the six lanes
publish unmodified. Otherwise, with X = resolved arg0:

1. Degree pre-scale: f1 = 360.0 · (X − biasdiff) · 0.125, where the biasdiff
   term is the 2^52-pair difference already in f1 on entry to the section;
   if X != 1, f1 += 180.0.
2. Radians: angle = (2π · f1) / 360.0 (`fmuls` + `fdivs`, exact order).
3. s1 = single(sin(angle)); projection A = lane72 − 30.0·s1 (`fnmsubs`).
4. s2 = single(cos(angle)); projection B = lane80 − 30.0·(−s2)... precisely
   `fnmsubs 3, 5, 3, 2` with f5 = 30.0, f3 = −s1-form, f2 = lane80 — see
   listing lines 80079d40–80079d54 for the exact operand flow.
5. `hitCheckFilter(0, seven stack pointers)`; if nonzero, lane40 = lane12.
6. Second sin(angle)/cos(angle) pair; f30 = 30.0 + f31; two `fmadds`
   accumulations into the 60/64/68 lanes (listing 80079dac–80079e0c).

The algebra above is a reading aid; the instruction order in the listing
governs. No branch in this section depends on the trig results.

## Publication and bottomless chain

Six floats publish to GP-relative words [r13+6860/6856/6864/6852/6868/6872]
(from lanes 60/64/68/72/76/80 respectively — note the rotated order), then
`bottomlessBero(f1, f2, f3)` runs with fp residues: f2 provably 30.0 (last
f2 write), f1 = −(lane56 float), f3 = second-sin residue, f30 = 30.0 + f31.
Then `lhz [Mario+46]`; if == 25, `bottomlessOn()`; return 2 in all paths.

## Not established

Lane/angle/offset meanings, table at [entry+180], `hitCheckFilter` argument
semantics, `hitObjGetPos` output use, cadence, and any on-screen placement.
The road doc's entry-crossing account ([Rogueport
roads](OVERWORLD_ROGUEPORT_ROADS.md)) is the narrative context, not a proof
of these lanes' roles.
