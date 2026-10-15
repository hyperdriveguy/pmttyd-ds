# Queued NPC draw callbacks

[The draw controller](../../source/field/field_npc_draw.cpp),
[public payloads/cursor](../../include/field/field_npc_draw.hpp) and
[native binding](../../source/field/field_npc_draw_binding.cpp) implement five
original callback graphs. [Main registration](npc-main-detection-display-tail.md)
and actual dispatch remain separate. Local completion requires real camera,
SDK, animation and GPU acknowledgements; it does not activate normal Game drawing.

| Callback | Body routing |
| --- | --- |
| Ordinary8004c50c | Queue class0: facing/Main/pass1; class1: pass2; class2: pass3; others: prelude only |
| Translucent8004c7e0 | Facing/Main, then consecutive passes1/2/3 |
| Offscreen8004bf28 | Ordinary body between synchronization/coverage operations |
| Offscreen translucent8004c234 | Translucent body between synchronization/coverage operations |
| Blur8004bc3c | One Main, four historical matrices, three draws per sample |

The core captures callback, actor, camera and providers only at admission.
Changed retry arguments cannot replace that invocation. Source child payloads
remain stable across false/empty provider holds, and acknowledged operations
never replay. Success resets the cursor, so another call starts a new draw.
The native binding rejects another binding's suspended cursor and requires the
same actual VM/shared-pose alias. Producer pass, current queue record/generation,
resources and borrowed actors remain alive through holds and dispatch.

Ordinary/translucent prelude negates event yaw0x144, revises it, builds the
[current-root matrix](npc-clocks-matrices-numerics.md), then reads state and color.
Darkening halves copied unsigned RGB while preserving alpha. It writes color,
reloads the handle, then sets material0x40. There is no material/color restoration
at ordinary return. Ordinary queries current queue work and class byte+1;
translucent ignores that query and owns its three-pass sequence.

Facing-name work requires main0x00800000. The first current-animation getter is
required even though its result is discarded. Separate fresh handles supply pose
and actual name address. Camera4 loads atX/eyeX/atZ/eyeZ in source order; negated
float differences go to double atan2. Its result narrows to float before the
source degree factor. The retained pose angle then contributes to asymmetric
rear bounds: `(d>90 && d<=270)||(d<-90 && d>=-270)`. NaN selects front.

Fourteen original string pairs at0x803066f8 are searched in order. Rear compares
left/selects right; front compares right/selects left. A match reloads the opposite
table pointer and handle before force0 selection. No match performs no setter.
Do not normalize names or replace the table with an inferred animation alias.
Main and every draw independently reload handle0x104; each draw reloads original
scale2 while retaining incoming angle and matrix. Copied matrix storage replaces
only the original guest stack argument, never a fabricated guest address.

Offscreen variants synchronize and clear GPU bounds before their ordinary body.
Every body exit synchronizes again, reads four unsigned16 bounds, reloads alternate
handle0x110 and forwards the copied bounds. Draws still use primary0x104. There
is no new active/-1 gate or model-space coverage fallback. Missing GPU completion
or actual offscreen ownership stays pending.

Blur saves material flags, queries its dispatched camera ID twice, and runs
independent source sine/cosine graphs with fresh yaw/literals. X offset is2*sin;
Z is2*(-cos), with source load order retained. Main runs once. History4/3/2/1 each
builds its full matrix and receives the retained offset translation with fresh zero
Y. Original copied color gets alpha28/53/78/103; each sample runs passes1/2/3,
for twelve draw calls. There is no history0 draw or extra Main per sample.
The footer clears0x40, reloads the handle and ORs saved flags, preserving unrelated
new flags. Final color remains alpha103. Assignment of saved flags or color restore
would change behavior.

The binding's built-in leaves use real pose-row material bytes, bounded source
strcmp and exact getter arithmetic. Current-animation lookup traverses actual
W/pool/model/file/holder/MODEL/directory words and returns the source name address.
It does not substitute native metadata, add active/resource gates, or eagerly
resolve clocks for force0 selection. Other Main/SetAnim/draw/camera/GPU services
remain explicit providers.

See [ordinary evidence](../behavior/display/RETAINED_NPC_ORDINARY_DRAW_CALLBACKS.md)
and [offscreen/blur evidence](../behavior/display/RETAINED_NPC_OFFSCREEN_AND_BLUR_CALLBACKS.md).
Original numeric/GPU/runtime parity and hardware performance remain unverified.
