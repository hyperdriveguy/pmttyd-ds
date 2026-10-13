# Mario key-data update (pad bytes, flags, edge detect)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the OW-MOVE-01 input-producer remainder
([OW-MOVE-01](../../requests/OW-MOVE-01.md)): `marioUpdateKeyData` body with
the GP+0x13B8 pad source and per-flag zeroing order. Newly extracted for
this contract (`behavior/marioUpdateKeyData.elf` 672 bytes / `.disasm`,
0x80095cb0 size 480 via `tools/inspect_dol_function.py`, ISO read-only).

Callees (`us-symbols` rows): `marioGetPtr` 0x8005e0c0, `marioChkKey`
0x8005c47c (mario.o, 316), `marioGetMotSlitCancel3` 0x800a601c (mot_slit.o,
100). No return value is constructed (falls to the epilogue).

## Pad-byte publication (no deadzone in this body)

`marioGetPtr` → Mario (r31). Six bytes copy from gp words to Mario stick
lanes, with negation on alternating lanes:

| gp byte | Mario lane | Sign |
| --- | --- | --- |
| +5048 (+0x13B8) | +594 | direct |
| +5052 (+0x13BC) | +595 | negated |
| +5056 | +596 | direct |
| +5060 | +597 | negated |
| +5064 | +598 | direct |
| +5068 | +599 | direct |

[600/601] are zeroed outright. No clamp, deadzone, or magnitude scaling
occurs here — the `.15 radial deadzone` the native port applies is authored,
and this body contradicts no part of it either way. r4 carries [Mario+656]
or −1 when that word is zero (downstream register only).

## Flag-halfword publication

Words at gp+4904/4920/4936/4968 masked with 4 publish to halfwords
[586/588/590/592]. Later, if `marioChkKey()` (called under a [Mario+0]
bits-28..30 gate) returns nonzero, all six stick lanes zero; if [656] is
also nonzero the four flag halfwords take r3 (0 at that point); then
`marioGetMotSlitCancel3()` nonzero plus [Mario+4] bit 7 writes 32 to [586].

## Per-flag stick-pair zeroing

Under the same [Mario+0] gate, [Mario+12] bits 4/5/6/7 each zero one lane
pair when set: bit4 → 594/595, bit5 → 596/597, bit6 → 598/599, bit7 →
600/601. Order is fixed 4→5→6→7; each test is independent (no else chain).

## Rising-edge word

r3 = 0x803DA398; new = halfword [r3+16]; [Mario+608] = new & (new XOR
[Mario+604]); [Mario+604] = new. Only rising 0→1 bits publish. The source
word's producer is not traced here.

## Not established

Pad-byte producers (GP+0x13B8 domain), flag-word meanings, callee
internals, cadence, deadzone ownership (explicitly not here), and any
gameplay effect.
