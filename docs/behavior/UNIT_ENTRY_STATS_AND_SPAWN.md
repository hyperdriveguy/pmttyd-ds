# Unit entry HP/FP, getter ABI and tutorial actor spawning

US G8ME01 revision0,recovered2026-10-12. Evidence B:original stat/allocation/init
regions of BtlUnit_Entry0x80128fe0,size0x6b4;complete BtlUnit_Spawn0x80128d20,
size0xe8;btlevtcmd_SpawnUnit0x801133bc,size0x74;GetMaxHp0x801100cc,size0x78
and GetHp0x801101c4,size0x78 (getter names have btlevtcmd_ prefix).
The whole entry function is fingerprinted but **not fully documented**:the
intervening actor/part/geometry/resource initialization remains open. No original
execution or end-to-end tutorial equivalence is claimed.

## Entry slot, publication and kind pointer

BtlUnit_Entry receives a setup-record pointer S. Read shared BattleWork W and
scan unit IDs0..63 with BattleGetUnitPtr,stopping at the first null pointer.
If all64 slots are occupied,return null. Otherwise request BattleAlloc(0xb34);
null allocation returns null. Clear the allocated0xb34-byte unit U,then register
it via BattleSetUnitPtr(W,ID,U) **before** later initialization. Store ID at U+0.

Copy S+0 kind pointer to U+0x10;copy kind+0xc0 data table to U+0x18. The kind's
word0 is copied to both U+4 and U+8. Subsequent callers can therefore find a
registered unit while initialization is still in progress;do not publish it only
after its init script finishes if that changes original dependencies.
Other fields/callback initialization before the stat block are not closed here.

## Original signed stat copy

At0x80129244..80,read S->kind afresh for the original loads. Signed halfword
kind+8 supplies HP;kind+0xa supplies FP:

| Unit field | Original store order/source | Established consumer |
| --- | --- | --- |
| +0x10a | kind HP signed halfword | Loaded again for next store |
| +0x108 | copied from unit+0x10a | GetMaxHp reads signed halfword |
| +0x10c | kind HP reloaded | GetHp reads signed halfword |
| +0x110 | kind FP signed halfword | Loaded again for next store |
| +0x10e | copied from unit+0x110 | Other FP consumers remain separate |
| +0x112 | kind FP reloaded | Other FP consumers remain separate |

Thus unit current/max HP are initialized from the same kind value at these
stores. No local minimum1,nonnegative clamp or conversion to unsigned HP occurs
in this block. The extra HP/FP fields are retained independently;do not collapse
all three copies into one mutable value or give unverified names to them.

The checked harbor kind at REL0x645c8 has HP5,FP0. Entry therefore performs
HP5/FP0 writes above. This establishes the original data→runtime stat-copy link,
not a measured battle HP or proof of no subsequent init/status/bonus override.
The remaining entry helper calls and explicit init event can mutate other state;
validate actual initial/updated stats in original gameplay.

## GetMaxHp and GetHp callback policy

Each resolves actor operand0 using evtGetValue,then BattleTransID(event,value).
If translated ID==-1,return event2 **without writing the output operand**.
That is not the same as assigning0/-1 to the destination. Otherwise fetch the
unit through BattleGetUnitPtr(sharedBattle,translatedID),read signed halfword
+0x108 for maximum or+0x10c for current,and pass it to
 evtSetValue(event,rawOperand1,value). Return2. There is no additional null-unit
check or first-call guard on that branch.

The tutorial uses the current-owner selector-2. Its phase event reads maximum,
executes opcode56 integer DIV by2,and reads current HP before the comparison.
Original dispatch56 targets0x80031eb0;it resolves divisor first,then current
value of the destination operand,uses PowerPC divw,and stores the quotient via
 evtSetValue. For the verified unmodified positive maximum5,the quotient is2.
The one-shot HP message path therefore admits current HP<=2 when its unit-work
latch is0 and the exact feedback phase is active. This is a static deduction;
not every damage necessarily samples the phase immediately,and HP overrides
change that threshold. See [tutorial event ordering](HARBOR_BOSS_TUTORIAL_EVENTS.md).

Do not implement the supported threshold as floating-point2.5 or rounding up.
Zero divisor and signed overflow are outside this concrete caller's domain;
full VM arithmetic behavior remains a separate contract.

## Initialization event owner and stored handle

At the end of BtlUnit_Entry,clear U+0x288. Load kind+0xbc init-event pointer.
Null skips registration. Otherwise request evtEntry(script,priority10,flags0),
write the unit ID into returned event+0x160,read its word+0x15c and store that at
U+0x288. There is no local null-event check after evtEntry. Return U.

The boss kind+0xbc points to REL init_event0x64808,whose installed hooks and
helper spawn are [documented](HARBOR_BOSS_TUTORIAL_EVENTS.md). This registration
connects its negative-owner selectors to the created unit through event ownership.
It does not execute the whole init script synchronously inside the entry helper.
The remaining manager/init completion/cancellation/lifetime policies need recovery.

## Spawn wrapper and raw flags

btlevtcmd_SpawnUnit captures output operand0 raw. Resolve setup operand1 using
 evtGetValue,then read operand2 **raw** as spawn flags and call
BtlUnit_Spawn(setup,flags). A null return writes-1 via evtSetValue;otherwise
write returned unit word0 ID. Return2. The output is an integer unit ID,not a
pointer or event ID. No local yield/first-call gate exists.

BtlUnit_Spawn calls BtlUnit_Entry. On null,return null. On success:

- If flags bit1 (value2) is set,clear unit byte+0x20 and insert its ID at the
  front of the64-word ordering array W+0x120. Carry displaced IDs forward until
  an old-1 slot,carried-1 or64 iterations.
- Otherwise if flags bit0 (value1) is set,clear that byte and perform the same
  insertion from array index63 backward toward0.
- With neither bit,do no insertion or additional byte clear. Value2 wins when
  both bits are set. Other bits do not select these routes.

Each insertion writes before carrying the displaced word. A full array can
lose the final displaced ID;there is no growth or rollback. This is a separate
ordering array from the unit pointer registry populated by entry. Its full turn/
selection consumer must be recovered before naming it a general initiative
policy. Return the original U regardless of insertion termination.

The tutorial init's exact SpawnUnit(local3,entry_npc_christine,0) at REL0x64868
therefore takes the no-ordering-insertion path. Its setup pointer relocation is
at0x64874. The boss stores returned ID in unit-work0,then places the helper at
(-140,0,5);later message speaker operands read that stored ID. Preserve this
actor lifetime and distinguish it from a field NPC name or partner-menu slot.

## Evidence scope and next work

Five new whole-body fingerprints,32 instruction anchors and direct kind/getter/
spawn/DIV bindings pin these claims. Four local wrappers are complete;the large
entry body is only documented in the regions above. Original disassembly and
reports stay ignored;no executable code or story text is linked or rewritten.

Recover the remaining entry resource/part/position/status initialization,HP/FP
bonus/pouch overrides,unit registry/ordering lifetimes,SpawnUnit helper kind and
init completion,damage/reward/death/result consumers. Compare original HP/FP,
slot publication,owner IDs,spawn failures and phase feedback boundaries. Full
unit mechanics,tutorial scene and complete US game requirements remain open.
