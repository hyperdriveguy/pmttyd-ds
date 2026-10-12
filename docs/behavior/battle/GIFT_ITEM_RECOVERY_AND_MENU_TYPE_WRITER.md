# Present item handling: recovery branches and complete type12 writer

Complete `_check_present_item`80120024/0x1f8,static US G8ME01 revision0.
Together with [audience writer](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md)
and [restore/getters](MENU_INTERRUPTION_RESTORE_AND_AUDIENCE_FIELDS.md),this
closes the local native interruption request. [Verifier](../../../tools/verify_menu_interruptions.py)
pins every instruction and used scalar constants. It does not recover the
entire audience present producer,effect renderer,or original event execution.

## Capture before queries and asymmetric admission

Capture B=WORD(0x8041eb90) first. Request BattleAudience_GetPresentItemNo(),
retain signed word I. I>=339 returns0 immediately,without target query.
Negative I is NOT rejected here. Request GetPresentTargetUnitId(),retain D;
accept signed -1<=D<64. D<-1 or>=64 returns0. Request
BattleGetUnitPtr(retainedB,D),retain U. Its bounds rule returns0 for D=-1,
but this caller does not reject that value or add a unit-null guard.

Then dispatch retained I:exactly0 returns0;123 selects HP recovery;124 FP
recovery;all others go to generic signed0<=I<339 admission. Negative I
therefore fails only AFTER the target/unit queries. Neither rejected item nor
rejected target is cleared by this function. Do not replace these ordered
branches with one early item/target range guard or assume D=-1 is harmless
on the unchecked recovery paths.

## Recovery item123/124:position and numeric request order

For123 request BtlUnit_RecoverHp(U,1);for124 request BtlUnit_RecoverFp(U,1).
Ignore return. Then request BtlUnit_GetPos(retainedU,&SP08,&SP0c,&SP10),
then BtlUnit_GetHeight(retainedU). The complete position leaf loads/stores
x from U3c,y from U40,z from U44 in that order. Complete height leaf returns
SIGNED halfword Uce. Both are unchecked;separate outputs permit alias-sensitive
ordering and are not a preloaded vector copy.

Convert returned signed integer H using the original xoris/high-word sequence:
constructed binary64 word `(0x43300000, uint32(H)^0x80000000)`,subtract
binary64 bias0x4330000080000000 at802efe78 using `fsubs` (single result).
This yields signed-H conversion with original single rounding. Then read
binary32 scale from U114,constant5.0 from804225f8,and preloaded private
position y. Preserve:

- `t = fmadds(scale, convertedH, 5.0)` (one fused single operation).
- `yEffect = fadds(originalY, t)` (separate single operation).
- Store yEffect to privateSP0c with stfs before the effect request.

Request effRecoveryEntry(0,1,x,yEffect,z) for123 or (1,1,x,yEffect,z) for124.
The two integer arguments are literal request values;effect type/resource/render
semantics remain a separate contract. Do not rewrite fused operations as an
unfused multiply-plus-add,or infer observed geometry from the formula.
Startup r2=80427440 makes r2-20040=804225f8;the verifier pins startup,bias,
and5.0 bytes. FPSCR/subnormal/platform fidelity remains separate.

After the effect request,request BattleAudience_SetPresentItemNo(0),which
reacquires its own current global owner. Then return0. These branches do NOT
write menu type/state/previous type/pending fields,even though recovery/effect/
item clear occur. A port must not equate return0 with “nothing happened.”
Target ID is not cleared by this setter;retainedU survives services throughout.

## Other admitted items:type12/state29 after AllEnd

For generic I with signed0<=I<339 (excluding earlier0/123/124):

1. Store BYTE(B+0x1c74)=1.
2. Read CURRENT Input state WORD(B+0x171c). Exactly12 or13 stores pending
   byte2 instead;these are Input states,not menu-type comparisons.
3. Request BattleCommandDisplay_AllEnd(retainedB).
4. AFTER AllEnd,read current menu TYPE WORD(B+0x1c5c),retain it.
5. Store previous TYPE WORD(B+0x1c60),then TYPE12,then Input state29.
6. Return1. No present-item clear,inventory insertion,item-use event creation,
   or target output write occurs in this generic caller.

The complete AllEnd leaf only reads WORD(B+0x1720),ANDs0xffeeeeee,and
stores it;it does not invoke every UI End service. Its mask clears0x00111111.
The mask comes from lis0xffef plus SIGNED addi-4370;0xffefeeee is wrong.
Its name must not be used to invent additional calls or resets.

## Downstream and native boundary

[Input entry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md) requests this
once before its progress loop;nonzero requests HelpSetDispType(0). Captured B
here can differ from passed Input B if global ownership changes. State29 invokes
[attack selection](BATTLE_ATTACK_EVENT_SELECTION.md),whose type12 route is
AudienceMove. The earlier type12-only scoped documentation is now expanded
by this complete body,but original gift/event/query reachability remains open.
Implement ordered typed reads,retained owners,recovery/result distinction,
exact effect arithmetic,route-specific clearing,pending1/2,and post-AllEnd type
capture. Reuse original assets;no dialogue or plot scene is invented here.
