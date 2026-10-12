# Confusion selector: weapon candidates,rechecks,and target policy

Companion for complete [BattleSetConfuseAct](CONFUSION_CLASS_SELECTION_AND_MENU_TYPE_WRITE.md),
static US G8ME01 revision0. [Verifier](../../../tools/verify_confusion_action.py)
pins the entire body. Use retained B/U/M and shared SP08/list representation
from the entry contract. This routine returns event pointers;it does not invoke
CommandGetWeaponAddress or BattleGetSelectWeapon directly.

## Weapon builder and candidate filter

For admitted type0/1/2/4/6/7/8,request MakeSelectWeaponTable(B,currentType).
Ignore its scalar return,including0. Reload current M540;request
GetCursorPtr(M,currentType,&SAME SP08),ignore return. Capture signed count
WORD(C+8). Empty builder results preserve old cursor count in the
[complete builder](WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md);
this caller does not reset it or terminate solely on builder return0.

Use the common low-byte source-ordinal scan. For each Q=M+28*Low8(J),read
enabled WORD(Q+0x84) then weapon pointer WORD(Q+0x80),even when enabled0.
Enabled0 skips weapon-field dereferences. Nonzero requires in order:

1. WORD(weapon+0x74) &0x00010000 must be SET.
2. Read WORD(weapon+0x64);bit0x02000000 must be CLEAR.
3. On the SAME flags word,bit0x01000000 must be SET.

Only then append ordinal to private halfword candidates. There is no weapon
null guard on enabled rows. Emit terminal-1 as usual. No eligible low-byte
candidate count returns NULL0,not flustered and not a default weapon.
Otherwise irand(candidateCount) once selects source ordinal and writes it to
current SP08 cursor index.

Reload cursor/index,derive current Q,read enabled. Zero returns flustered.
Nonzero retains that CURRENT index as the third argument to
SelectWeaponDecide(B,current M540,index). Type is freshly read at the call.
[Complete decision](WEAPON_MENU_SETUP_AND_DECISION.md) rebuilds the table,
then samples live targets;it can change row contents between the next reads.

## Post-decision weapon and flags revalidation

After decision,reload cursor/index and read current weapon pointer,retaining it
as V. This overwrites the register previously holding actor U;no later actor
query is used in this weapon branch. Check WORD(V+0x74) bit0x00010000 AGAIN.
Clear returns flustered. Then read WORD(V+0x64);bit0x02000000 set returns
flustered,or bit0x01000000 clear returns flustered. These are actual repeated
checks after table rebuild/sampling,not merely an assertion that the first
candidate filter succeeded. Failed checks leave earlier type/cursor/target
writes intact.

Read S8(B+0x0e94) candidate count. <=0 returns flustered. Positive count:
request irand(that signed positive count),store LOW8 result at BYTE(B+0x0edf),
then read live WORD(B+0x0ef4),OR0x4,store it. Finally read CURRENT WORD(V+0xb0)
and RETURN that address unchanged,including0. This call-site uses B+e94/edf
rather than assuming it is the same currently selected target field at
BUF+ab7. Preserve exact addresses and sign extension.

Do not manufacture a target record lookup,index clamp,actor target-ID output,
or script constructor from this branch. Full sampling/choice target ownership
is documented by its own callers;this selector makes the visible random/store
requests above. [Sampling](BATTLE_ENEMY_CANDIDATE_SAMPLING.md) and
[eligibility](WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md) provide
linked local bodies,without a global source-count invariant.

## RNG accounting and implementation requirements

The successful class→weapon caller path requests class irand once,weapon irand once,
then target irand once,with builders/cursor services between. Early failures
omit later local draws;additional callee draws require their own contracts. Native replay must preserve shared RNG bounds/order and
source-table mutations;selecting a host-random weapon followed by a default
target changes the recovered behavior. Keep distinctions between null candidate
result,flustered script fallback,and null event pointer from an admitted weapon.

Weapon/source availability,dynamic counts,full event graph/selected-weapon query
reachability,Game/platform/original runtime remain open. Local type writes and
rechecks do not prove all subsequent events have supported private cursors.
