# Plane-flight motion main (mot_plane)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Newly extracted for this contract (the old `behavior/flyMain.disasm`
placeholder is 0 bytes and the name collides with party_move's flyMain):
`behavior/mot_plane_flyMain.elf` (3648 bytes) / `.disasm` from
`flyMain@mot_plane.o` at 0x8009c9b8, size 0xd80, via
`tools/inspect_dol_function.py` (ISO read-only). Distinct from
[party fly dispatch](../partner/FIELD_PARTNER_FLY_DISPATCH.md), which is
`flyMain@party_move.o` — the two same-named symbols must not be merged.

SDA constants read from the ISO: −26412 = 90.0, −26404 = 0.0,
−26392 = 2.0, −26388 = 1.0, −26384 = 270.0, −26372 = 60.0,
−26368 = 0.1, −26364 = 150.0, −26360 = 330.0, −26356 = 210.0,
−26352 = π, −26348 = 180.0, −26344 = −0.001, −26340 = 0.5,
−26336 = 6.0, −26332 = −80.0, −26328 = −22.0, −26324 = 1000.0.

Callees (`us-symbols` rows): `marioGetPtr`, `psndSFXOn_3D` (ID 383 site),
`hitGetAttr`, `distABf`/`system_angleABf`, `sin`, `sincosf`, `revise360`,
`marioSearchUnder`, `chkCollision` (mot_plane.o, 1364 bytes — open),
`sysMsec2Frame`.

## Entry and pre-main

`marioGetPtr` → Mario. [Mario+68] == 13 → dedicated early block;
== 12 → fallthrough region; == 15 → separate block; else the main
sequence at 8009ca98. Pre-main: if [Mario+704] vs [144] compare selects
it, stage = 14. A `marioChkInScreen`-gated SFX 383 site and the
[660]+40/65535 compare feed the same region. [156] −= 2.0 while ≥ the
table lane, with [180]/[144] +1 accumulations, else stage = 15.

## Stage machine ([68] values written: 14, 15, 40, 50, 51, 60, 70)

- Main gate: [68] == 15 → [588] bit23/bit22 tests select stage 51 (with
  a [144]→[708] snapshot) versus falling into angle setup (`distABf` +
  `system_angleABf` over the [660]-table lanes) into the shared core.
- The core runs stick-594-gated sin chains (degree→radian via π/180 with
  the 0.1/2.0-class SDA factors), `hitGetAttr` bit26–28 gates, the
  0x80310030 + [46]·12 indirect callback, [384]/[144] accumulators, and
  `sincosf`/`revise360`/`marioSearchUnder`/`chkCollision` requests in
  fixed order; thresholds −80.0/−22.0 and caps 1000.0/6.0/0.5/−0.001 gate
  named sub-blocks (exact addresses in the listing).
- Stage values written in-body: 14, 15, 40, 50, 51, 60, 70 (verified by
  `li` census; entry compares against 12/13/15 without writing them).

## Not established

`chkCollision` internals, table/lane ownership, stage semantic names,
cadence, camera/pose bindings, and any on-screen flight. Float-chain
interiors beyond the pinned constants/branches, and the 12/13/14-entry
prologue details, remain open.
