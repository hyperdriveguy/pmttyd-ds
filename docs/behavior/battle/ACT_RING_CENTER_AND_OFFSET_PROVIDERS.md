# Original class-ring center and offset providers

2026-10-14 static US G8ME01 revision0. Complete local GetRingCenter80119cf0/0x84
and GetRingOffset80119c7c/0x74 bodies are implemented with typed projection/libm
providers. [The verifier](../../../tools/verify_act_ring_position.py) pins the
helper bodies, linked unit accessor and original scalar spans/r2 base. They
serve [the ring draw lifecycle](ACT_SELECT_RING_DRAW_AND_ICON_ROUTING.md).
Static recovery is not a measured camera/timing or original-libm bit equality claim.

Center captures current-global B and WORD(B+420), calls the existing original
unit getter and requests projection of position at retained unit+3c. It does
not substitute Mario's visual slot, field position or home position. Once the
projection returns, add original22.5 to X and105 to Y, store Z0, then clamp X
to-180 when X<=-180. No right-edge clamp, scene-font reflow or input gate occurs
in this body. A projection provider changing current-global B does not retarget
the already retained unit or trigger a second lookup. Missing projection leaves
the output untouched. Native actor/camera projection remains a separate binding.

Offset retains float angle=(original4.71238899230957-inputTheta). Call original
cos identity with that angle promoted to double; round its returned result to
float, multiply by fresh original80 and write X. Then call sin with the retained
angle; round to float, multiply by fresh original35 and write Y. The cosine and
sine provider sequence is retained. An unavailable sine after successful cosine
leaves the preceding X write visible; no zero Y or fabricated trig result is
supplied. Missing cosine leaves both outputs untouched. Native stdlib/PICA SDK
numerical equality with original PowerPC routines is not inferred from the API.

Host checks cover actual active-unit lookup/position address, a projection
provider mutating the global battle pointer, original left clamp and Z write,
ordinary center shifts, zero-phase ellipse offset, provider order and partial
output behavior. No renderer or Game scheduling is automatically activated by
these helpers. Next bind projection from the admitted battle camera/actual acting
unit, then connect the existing original ring/consumer/native GPU backend once
per simulation tick. Keep the user-requested upper command presentation and
[lower audience continuation](../display/NATIVE_BATTLE_HUD_AND_SUBMENU.md).
