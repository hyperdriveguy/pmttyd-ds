# Battle display footer registration and local mutation

2026-10-14, original US G8ME01 disc0 revision0, read-only ISO/DOL inspection.
EVT-01 continuation after [live unit loop/StockExpDisp](BATTLE_DISPLAY_LIVE_PART_LOOP_UNIT_TAIL_AND_FOOTER.md).
Addresses/offsets/masks are hexadecimal; decimal priorities/counts are marked
by ordinary prose. This is static instruction evidence, not original execution,
rendered fidelity or native provider completion. Queue requests below defer
their actual callbacks; finishing these wrappers does not execute their GX.

## StageDisp: first real provider after StockExp

BattleStageDisp80139084:60 captures fresh global BattleWork B, reads B420,
then calls BattleGetUnitPtr800f88cc(B,index) at80139098. It ignores the returned
unit and has no null-unit gate. Queue these two unconditional requests in order,
all userdata0:

| Camera | Class | Callback identity | Requested priority |
| --- | --- | --- | --- |
| 4 | 2 | 80138da8 | 5000,80422b34 |
| 4 | 2 | 801389a0 | 6000,80422b38 |

They are actual stage callbacks, not a pose snapshot or synthetic background.
The getter is a real provider even when its ignored return happens to be null.
No other stage work/mutation occurs in this wrapper.

## CommandDisplay: global color mutation before registration

BattleCommandDisplay80122870:168 uses passed/captured B. Compute C=B171c,
not WORD(B171c); if the resulting address iszero, return before any service.
This guard admits every ordinary valid native BattleWork. First actual callee
at8012288c is80119e24:124, an original global color-cycle owner.

It reads signed state BYTE8041eba0 and manipulates RGBA bytes at80416d20.
Original initialized DATA bytes are ffc0c0cf; this is an image initializer,
not proof no subsequent controller resets/writers occur. All arithmetic stores
low8 wrapping byte and tests the NEW unsigned byte; invalid signed state outside
0..5 returns without stores. Its six states are:

| State | Mutation | Transition equality |
| --- | --- | --- |
| 0 | increment G | newG==255→state1 |
| 1 | decrement R | newR==192→state2 |
| 2 | increment B | newB==255→state3 |
| 3 | decrement G | newG==192→state4 |
| 4 | increment R | newR==255→state5 |
| 5 | decrement B | newB==192→state0 |

Alpha remains untouched. The state is global retained lifetime, not reset to0
on every display pass, and the routine is not a no-op when menus are hidden.

Next unconditional dispEntry is camera8/class0, callback8011fde0,userdataB,
priority902.0 (8042265c=44618000). Then independently reload WORD(C4)=B1720
for each of the following gates, in this exact order. Each admitted registration
is camera8/class0, userdataB, priority500.0 (80422660=43fa0000).

| B1720 mask | Callback identity |
| --- | --- |
| 1 | 8012210c |
| 10 | 80121ce8 |
| 100 | 80121638 |
| 1000 | 80121074 |
| 20 | 80121330 |
| 10000 | 80120a44 |
| 100000 | 80120844 |

Flags are not mutually exclusive; no else-if menu winner or current input-state
skip exists here. Registration callbacks own their original local rendering.
Source wrappers do not substitute a single native command UI renderer.

## AudienceDisp: animation process then independent queue/window gates

BattleAudience_Disp801a6c18:d0 first calls AudienceBaseGetPtr801a86a0, then
AnimProcess801a4224, then calls BaseGetPtr AGAIN and reads fresh A.flags.
First getter's pointer is not retained across AnimProcess for this gate.
If A.flags20000 clear, register these four requests in order, all userdata0:

| Camera | Class | Callback | Priority |
| --- | --- | --- | --- |
| 4 | 1 | DispAudience801a38f8 | +0,80424988 |
| 4 | 2 | DispAudience801a38f8 | +0,80424988 |
| 4 | 1 | DispItem801a37f0 | +0,80424988 |
| 8 | 0 | DispApSrc801a36c8 | 499,80424a7c |

Then ALWAYS request pouchGetPtr800d5eb0, irrespective of A.flags20000.
Unsigned HALF(pouch8c)!=0 registers camera8/class1/DispWin801a35d0/userdata0/
priority499. A.flags20000 suppresses only the first four, not this window gate.
Pouch8c0 suppresses only the fifth, not animation process or the four ordinary
records. Empty audience records do not skip callback registration.

## Complete local AnimProcess scalar interpreter

AnimProcess801a4224:1e4 first calls BaseGetPtr, then AudienceGetPtr801a8664(0).
Starting with that pointer, scan exactly200 records at stride134, ascending
indices. It does not re-query GetPtr for every index. Each record's live flags
WORD0 must have BOTH masks1 and4; otherwise no animation state stores.
For admitted record R, capture signed timerR2c and script pointerR28. Timer>0
stores wrapping timer−1 and advances to next actor WITHOUT reading script.
Otherwise read script WORD0 as opcode. Unsigned `(opcode+8)>7` skips any
opcode outside−8..−1 without advancing its pointer. The source switch table
8039ccc0 gives these complete local operations, one opcode per actor per pass:

| Opcode | Source operation |
| --- | --- |
| −8 | R2c=script WORD4; R28+=8 |
| −6 | write script WORD4 to R34+4*U8(R40); increment BYTE R40; R28+=8 |
| −7 | decrement WORD at R30+4*U8(R40), reload; if signed<=0 decrement BYTE R40 and R28+=4; otherwise scan R28 backward4 per word, depth1, decrement depth on−6 and increment on−7, until depth<=0, then R28+=8 |
| −5 | capture script WORD4 label ID; set R28=R24; scan forward4 per word until opcode−4 AND following word==label ID; retain pointer on that label |
| −4 | R28+=8 |
| −3 | R28=R24 |
| −2 | BYTE R1a=low8(script WORD4); R2c=script WORD8; reload R28 and add12 |
| −1 | no local state store |

All pointer/byte/word arithmetic retains original wrapping domains. The stack
has no bounds/underflow guard; label/backward scans have no termination budget.
Scan marker tests inspect every word, including operands; do not rewrite this
as a parsed high-level loop with different marker recognition. Script pointer
is not null-checked after active flags admission. A native invalid/unavailable
script domain must remain unavailable rather than set a synthetic first frame.
For source zero flags on all200 actors, the full original scan has no animation
mutation; it remains a real scan and getter lifetime, not an empty-crowd global
shortcut. Positive actors require this scalar interpreter before display.

## BreakSlotDisp is unconditional registration

BattleBreakSlot_Disp80203a68:38 always registers camera8/class0, callback
BattleBreakSlotDispReel802036d4,userdata0,priority490.0 at804268ac=43f50000.
No reel/controller/vertex availability gate exists here. Its actual idle callback
still owns scissor setup/restoration; see [reel display](../battle/ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md#registered-slot-display-is-not-a-no-op-even-without-reel-vertices).

After all footer wrappers return, enclosing btlDispMain performs its separate
Bef4 clears10000 then20000 with fresh rereads, as in the live-loop contract.
Do not apply those clears before the registrations or omit them after admitting
the callbacks through a typed native queue adapter.

## Palette bootstrap lifetime and actual resource Init

Fresh DOL-section mapping establishes BYTE8041eba0 is not in any loadable
text/data section. It is within boot BSS row800055b8: start8041e580,lengthEA4.
Original __init_data8000542c traverses the table800055b0; at800054c4 it calls
memset(start,0,length) for each nonempty BSS row. This establishes state0 once
at process bootstrap. DATA color80416d20 lies in section11 (base80414f20,
length9660), with initializerffc0c0cf, and is not within that BSS row.

A fresh aligned instruction census of all seven original DOL text sections
finds direct SDA stage stores only at80119e80/ea4/ecc/ef4/f18/f40, all six
color-process transitions. Direct palette SDA references additionally include
consumer8011856c/570. This is a bounded direct-reference census, not proof
that arbitrary REL/event pointers or indirect alias writers cannot exist.
It supplies no authorization to reset palette at battle entry or per display.

The supposed palette Init80119da0 is actually a distinct resource constructor,
complete body58: capture global B, call80007388, format a source pathname with
sprintf8026a0b8, call fileOpen8018a5f8(path,4), retain returned resource at
B163f8. It does not store palette bytes or stage. Source btlDispInit801030f8:3c
calls8012dee8, then this resource constructor80119da0, then ORs Bef8 bit2.
Neither body locally resets palette; native resource completion remains separate
from authentic retained palette bootstrap/history.

## Ordered reads/stores for active AnimProcess scripts

The scalar table above must retain source live/alias order, not snapshot an
actor/script structure. After reading one flags word, load R2c FIRST, then
R28 even if the positive timer branch will only decrement. Active zero/nonpositive
timer loads opcode through that captured script pointer. For the following used
opcodes the precise order is:

* −6 stack push: read unsigned BYTE R40; compute destinationR34+4*depth;
  read captured script WORD4; store to that destination; RELOAD BYTE R40,
  increment/store byte; RELOAD current R28, add8/store. A stack destination can
  alias depth or pointer state in an invalid/exceptional domain; source has no
  independent stack-bounds validation.
* −7 stack end: read BYTE R40; computeR30+4*depth; load/decrement/store its
  count word. RELOAD BYTE R40, deriveR30+4*newDepth AGAIN, read that word and
  compare signed>0. For<=0, store depth−1 byte then reload R28,add4/store.
  For>0, local nesting=1; each iteration RELOAD R28,subtract4/store,RELOAD
  current R28,load opcode. Marker−6 decrements nesting; marker−7 increments.
  Repeat while signed nesting>0, then reload R28,add8/store.
* −5 label jump: load captured script WORD4 label; load R24 and store R28.
  Each scan iteration reload R28 and read opcode; if opcode−4, freshly read
  following WORD4 ID and compare to captured label. Otherwise reload R28,
  add4/store and continue. Matching leaves pointer at marker; next pass can
  process its separate−4 skip. No pre-parsed table or one-pass branch target
  replaces the real memory scan/alias behavior.
* −2 frame: read captured script WORD4, store LOW8→R1a; THEN read captured
  script WORD8, store R2c; THEN reload current R28,add12/store. Script words
  are not all captured before the first actor store.

The final actor pointer advances134 from its retained actor base after every
opcode/skip, regardless of scripts mutating another actor or flags. No function
calls occur inside the opcode branches; native retry cursors cannot justify
repeating completed stores. Valid typed/native nonaliased scripts can declare
a narrower adapter domain, but that declaration must not be presented as proof
of arbitrary source alias/invalid-stack semantics.

## Caller after display returns: state publication precedes later exit

Uppercase BattleMain800f8e28 calls btlDispMain at800f8e6c. After return it
restores its OWN captured B and calls BattleGetSeq800f89a0(B,0) at800f8e78.
Exactly4 returns explicit0. Other banks follow the complete
[Main late gate](../battle/BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md)
through source global word tests and possible forced end-bank writes; display
completion alone is not whole battle completion.

Lowercase battleMain800f904c captured its own outer B before calling Main.
Zero Main return at800f906c admits read/OR/store retained outer Bef4 mask80000000
at800f9074..7c. Then this hook returns. It performs NO BattleEnd, pointer clear,
field scene switch or queued callback drain. In actual marioStMain outer order,
next direct service after this hook is camMain8000f704 at8006f944, followed by
window/map/hit/event/field owners. Next rendering's camDraw dispatches retained
registered records through the queue's real lifetime. Returning directly from
display to field teardown bypasses these owners.

The later field-exit context is [seq_battleExit→battle_exit](../battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md#required-exit-services-before-field-waiter):
first actual exit provider BattleEnd800f8da4, then map/free/smart/registry/event
and paused-field recovery services in the documented order. This is a separate
sequence-manager admission after completion publication, not a transitive call
inside display/Main/outer hook. Deferring unsupported draw providers is honest;
claiming complete scene restoration from these local footer returns is not.

## Original evidence and boundaries

Fresh ignored disassembly: artifacts/battle-display-footer-evidence. All body
hashes below use actual return-delimited original ranges, not the containing
larger evidence spans used during exploratory disassembly.

| Original body | SHA256 |
| --- | --- |
| resource Init80119da0:58 | 9ad746186b635d7f0fe7bcd0555f354625ddf80e48cc4e71c806612d2bcfd8e8 |
| btlDispInit801030f8:3c | 5804a4be99cfe95361eb72dd0ac12d93746eebc064217a2b6bfbd9f29557dbf2 |
| BattleMain800f8e28:dc | bf6aee153d46d818476c124121653739778fdcc2d73a958962b95251be990f96 |
| outer battleMain800f904c:48 | 748302b617d70e9e5e45de1a2f9248cd2100f499a5f5a04ff9a281acaa96e28f |
| StageDisp80139084:60 | 27bbfd61453a6984c71419f4057807daa76f7ccf630f18228c16a596ee3dcb06 |
| CommandDisplay80122870:168 | d298f29ca100edef1bfafa877a84206800d5c87c8d0434b442ca3a7ef7cfb7dd |
| color process80119e24:124 | b28291e95852d770d0052a2cd2cc966b5af503cf30fd2229240d5392d9bbf47b |
| AudienceDisp801a6c18:d0 | ea5d361d0e98cb572fb836c4af44376ace3ba2dc62e88b0423d0e139a06455f6 |
| AnimProcess801a4224:1e4 | 2f4994c631bcc9568f3b37f681116fa201f5c78009b611299f03ac1aafba3215 |
| BreakSlotDisp80203a68:38 | d005872c21ba4197dab3578e40ad704b751669737989af62ee828ca1b778e7e6 |

No native implementation/tests, executed original callbacks or complete setup4
rendering is claimed. Global palette state/init/writers, actor scripts/lifetime,
actual queue sorting/dispatch, every deferred callback and its GPU dependencies
remain retained owners. EVT-01 and full prologue parity remain open.
