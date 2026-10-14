# Fresh opening GW8 provenance and resolved ship jump operand

Scope: original US G8ME01 disc0 revision0, a freshly initialized field event
context and the opening route through gor_00 arrival. This is bounded static
source evidence. It does not establish complete callback/store or original
runtime parity, and it says nothing about loaded/legacy checkpoints.

## Initial bank value and retained room ownership

[Complete initialization/reset](../event/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md)
pins evtmgrInit8003781c:118 and evtmgrReInit800377b0:6c. Field context
C0=803d8d30 holds32 GW words at+4..83. Init explicitly memsets that entire
0x80-byte bank to0, so GW8 atC0+0x24 initially has integer word0. ReInit clears
the selected event-entry array and updates clocks/manager globals; it preserves
both work/flag banks. A room change cannot be described as clearing GW8 again.

The used encoded variable operand is decimal4244967304/hexFD050F88,
signed−49999992=−50000000+8. This is GW8, not a literal Y coordinate.
Native fresh-flow initialization can therefore explicitly establish a retained
GW bank with initial GW8=0. That native state must retain actual subsequent
writes and resolve the operand when its callback reads it; it must not replace
all occurrences with a hardcoded zero.

## Reproducible selected-event prefix census

[Verifier](../../../tools/verify_opening_gw8_prefix.py) and
[pinned source manifest](../../../tools/opening_gw8_prefix_evidence.json)
read the ISO without mutation and verify original event hashes, all literal
operands, local event-reference closure/root membership, initializer/reset
body hashes, original nohit jump body/−4 height constant and the actual
arrival operand/getter/write/jump anchors.
No original program bodies or message payloads are copied into that manifest.

Starting roots are original dmo demo/button/init; aaa init/prologue/prologue2;
gor10 init/letter/mezame/init; sys heading init/start; and muj20 init/prologue.
Following every annotated local REL event reference conservatively includes
later/epilogue branches too. This is a superset census rather than a claim that
all59 events execute on the fresh path. Counts are:

| Module | Local referenced events | Commands | Literal GW8 operands |
| --- | ---: | ---: | ---: |
| dmo | 7 | 511 | 0 |
| aaa | 7 | 496 | 0 |
| gor10 | 4 | 154 | 0 |
| sys | 2 | 121 | 0 |
| muj | 39 | 1419 | 0 |
| Total | 59 | 2701 | 0 |

The original gor_00_init_evt's174 commands also contain no literal GW8 operand.
In original enter_gorotsuki_town, every command before0x25920 likewise has
none. Thus these authored event operands do not directly read/write GW8 or
pass literal GW8 as a callback output destination before that first use.

This does **not** prove absence of a hardcoded/aliased bank write inside opaque
C++ callbacks, Title/file-load services, a synthesized operand alias, imported
DOL events or service-driven event launches. Those transitive owners are the
exact missing dependencies for an unconditional original-runtime claim that
GW8 is still0 at0x25920. The verifier explicitly reports
`callback_store_closure: false`. No broader SDK/title closure is implied.

## Actual source arrival flow

Original enter_gorotsuki_town uses this ordered flow:

| Source command | Used request |
| --- | --- |
| 25920 | evt_npc_set_position(shipdummy,0,GW8,0): read incoming GW8 |
| 2595c | Concurrent evt_npc_get_position(shipdummy,LW0,LW1,LW2) |
| 25974 | SETF GW8,LW1: publish the actual dummy's Y through source float assignment |
| 259d8 | evt_npc_jump_position_nohit(shipdummy,0,GW8,0,1800,100): resolve current GW8 |

The called source nohit body at80051868:364 has SHA256
`eb67fa0707263fcd321df7e977c9f034517a71bfcd835ca96454feacd088a003`;
its height constant at8041fdfc isC0800000/−4. Those used identities are pinned
separately from event-bank provenance; they do not turn GW8 into a literal.

The monitor is a live inline loop gated by WW10; it can continue publishing
actual dummy Y while the root's jump owner runs. Even when the incoming fresh
bank value is0, that does not make GW8 a constant across the animation.
The native source helper accepts the resolved Y and the native dummy-position
provider must publish its actual current position. Pinning the Init bank word
is separate from pinning NPC getter/physics/scheduler ownership.

The bounded conditional conclusion is: **if no earlier used provider writes
GW8 after fresh Init, the first set_position receives Y=0**. The getter/monitor
then supplies the actual value used by the jump. A native bridge with explicit
fresh bank0, no earlier native GW8 writes, retained bank ownership and actual
dummy-position publication legitimately implements that native fresh-flow
state. It does not prove the entire original callback graph, force arbitrary
resumed states to0 or allow substituting0 for a resolved operand.
