# Original late NPC history and complete actor traversal

Source8004b10c..8004b1b4 is an allocation-free raw-word history controller.
The native all-actor composition joins the [shadow suffix](NATIVE_NPC_MAIN_SHADOW_SUFFIX.md)
to the existing final clear/index/actor step8004b1b4..8004b1d0. Returned FieldTail
is an unfinished frontier, never whole npcMain completion. Normal Game's
separate field path is not replaced or automatically activated.

## Exact late captures

Read currentX8c then currentY90, write previousX98 then previousY9c, THEN read
fresh currentZ94 and write previousZa0. These are scalar lwz/stw word copies,
not a paired load, float conversion, memcpy or a detached actor snapshot. NaN
payloads and signed zero remain exact bits. Every successful read/store is
retained across a later primitive hold; false/empty has no effects.

After this unconditional reached snapshot, load fresh mainflags. Only flag100
loads fresh MainCount8041e8ac. Odd count skips motion history. Even count shifts
five XYZ vectors oldest first: c8->d4, bc->c8, b0->bc, a4->b0, current8c->a4.
EVERY vector uses readX, readY, storeX, storeY, fresh readZ, storeZ. CurrentX/Y
for the last vector are reread, not reused from previous-position capture.
A late root/count/flag mutation can therefore change later captures without
rewriting earlier successful stores. No active/query flags are rechecked here.

## Parent composition and retirement

The new native actor cursor retains the original entry and all movement,
automatic pose, detection, Mario detection, display, shadow and history cursors.
After full history only, resume original entry at phase78: fresh state1d4,
clearONLY10, store, increment index and actor by340, compare against captured
signed count. Initial !(active1 and query2) skips both history and final clear;
post-clock query loss runs final clear but skips body/history. Hidden/cull paths
after body admission still run history. Late query loss in a held body does not
cancel its remaining source work.

Bank/count/array remain the original whole-pass captures. Native storage bounds
36 ordinary/16 alternate and negative-count skip policy are unchanged. The
clock helper ALREADY retires its own cursor on both successful paths; no new
clock reset is needed. A source audit initially missed that reset and the test
incorrectly expected an active completed clock. Both were corrected from the
full helper body. Source clock logic is unchanged.

Once an actor body finishes, its child captures remain retained while final
clear or the next actor's entry is held. Retire them only after entry reaches
another Movement frontier or FieldTail. A suspended next clock cannot replay
previous history. The aggregate stays bound to its original native binding;
alien owner/VM/pose aliases hold. Same mappings, producer pass, queue generation,
and all borrowed actors/resources remain live through holds/deferred dispatch.
No source tail work, callback dispatch or whole-Main acknowledgment is invented.

## Evidence and remaining work

Pure tests enumerate the raw ordinary/even history trace, hold every primitive,
exercise fresh/odd/missing100 gates, scalar XY vs fresh Z, fresh final-current
rereads, NaN/signed-zero bits and owner identity. Native fixtures scan actual
owned slots0/2 with inactive1/3, suspend the next clock AFTER first history,
mutate the old root while held, then suspend second gravity and mutate live
bank/count/array and MainCount. Earlier captures do not replay; captured scan
still retires four slots, second history uses fresh odd MainCount, inactive
state10/history remain untouched, and late cleared query does not skip reached
history/final clear. Real same-VM/pool storage is used; gravity is an explicit
fixture. These are not original execution/visual traces.

Ignored source audit artifacts/npc-main-parent-evidence/history-traversal-review-
notes.txt records original history/traversal body b10c/c4 SHA256
aeeaf97ad878a2cfab2a0b9464cbc304a3c3fa80fe8fe59ff1734d0ca13612ef.
It also corrects the next tail's interpolation signs: first -600->0, second
0->+600. b21c uses8041fce4 rawc4160000; b25c uses8041fce8 raw44160000.
Read-only ISO span fce4/8 SHA256
4d3b57e63f076c32b53d1b26264b1a272d87587d5b4dfef339ca8c9eb350ae2c.
The parent and ABI contracts now agree. This is static source evidence, not
measured Dolphin behavior; the field tail is still unimplemented.

No new assets/constants, texture/geometry/lighting/audio conversion or original
message/story change. Existing native pools/memory primitives and ARM integer
instructions are reused. Field tail, real movement/collision/math/camera/shadow/
interrupt/resource/audio producers and normal Area activation remain required.
No full field rendering or physical New2DSXL performance claim.
