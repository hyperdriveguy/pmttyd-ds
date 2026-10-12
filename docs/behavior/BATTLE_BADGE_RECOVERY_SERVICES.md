# Badge recovery amount, resource writes and geometry event wrappers

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete _get_heart_suitoru_point80113950/78 and flower801138d8/78;
RecoverHp event8010fa1c/d4,RecoverFp8010f988/94;
GetBodyId8010ead8/74,GetStatusMg80111cf4/6c,GetHeight80111dd0/70,
GetPartsPos80112d98/10c and GetBodyPartsId80128c98/40. Read
[tail graph](BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md),
[geometry leaves](BATTLE_UNIT_PART_POSITION_QUERIES.md),
[resource arithmetic](BATTLE_REGENERATION.md) and
[badge import](UNIT_BADGE_COUNT_IMPORT.md).
Verification: `python3 tools/verify_badge_recovery_tail.py`. Static local bodies.

## Amount query's raw actor operand

Both point queries capture shared BattleWork W and raw output operand1.
Pass RAW operand0 directly to BattleTransID(event,rawActor); these wrappers
DO NOT call evtGetValue first. Fetch unit from captured W and translated ID.
If null,output0. Otherwise point initially=U8(unit+2f9) for heart,or+2fa for
flower. Read signed word D=unit+284. If signed point>D,set point=D. Write
through evtSetValue(rawOutput),return2. No output sentinel/null skip, no clamp
D>=0, no HP/FP availability or actual damage-result query.

This is signed min(unsigned badge byte,signed retained damage word). Negative D
can produce a negative amount; the event graph then skips its branch. Badge
import binds item295→2f9,item297→2fa for Mario's documented equip route. The
[damage application](BATTLE_DAMAGE_APPLICATION.md) adds clamped signed H to
attacker+284 before defender suppression/current-HP processing. The
[action controller](BATTLE_ACTION_CONTROLLER.md) clears+284 at action setup.
Those links do not establish every writer/cadence or permit a local reset after
recovery. Both point queries read the same retained word independently.

## Resource recovery event services

Both capture operand pointer and shared W BEFORE actor resolution. Resolve
actor0 with evtGetValue then BattleTransID; exact translated-1 returns2 BEFORE
unit fetch or resolution of the remaining operands. Otherwise fetch actor from
captured W;resolve operand1 (part) then operand2 (amount). Part result is discarded:
no part lookup, eligibility or body check. Actor-null is not guarded here.

RecoverHp tests word unit+104 bit0x40000000 AFTER both resolutions. If set,
call BtlUnit_RecoverHp(unit,amount) only. If clear, perform this exact order:

1. Read S16(unit+10c),store as word unit+208.
2. Call BtlUnit_RecoverHp(unit,amount).
3. Read S16(unit+10c),store as word unit+20c.
4. Copy S16(unit+202) as low16 into+200;copy S16(+206) to+204.
5. Read U16(+1fe),OR1,store16 to+1fe.

Return2. The attribute bypasses bookkeeping,not healing. No amount/fullness
validation,damage-counter reset or display/effect request locally.
RecoverFp simply calls BtlUnit_RecoverFp(unit,amount),return2;no corresponding
bookkeeping block and no0x40000000 gate.

HP core adds amount to signed currentHP+10c with word arithmetic,stores low16,
reloads signed16,then caps only if greater than S16(maxHP+108). No lower clamp.
FP core routes kind224..230 to Mario. It finds Mario,reads maximum through
GetMaxFp, narrows that result signed16,then finds Mario AGAIN to obtain current
storage+112. Other kinds read their own maximum and use their own+112. Add
amount,store low16,reload signed16,cap above captured signed maximum only.
No lower clamp/null fallback/pouch synchronization. Repeated Mario lookup and
maximum capture are required service ordering,not one assumed owner snapshot.
These complete core bodies are also fingerprinted by this slice.

## Body,scale and height readers

GetBodyId resolves actor then translates; only then captures raw destination,
fetches unit through its earlier captured W, calls GetBodyPartsId and writes
its returned word through evtSetValue. Core GetBodyPartsId returns-1 for null
unit or no matching linked part. Otherwise traverse U+14/P+0 in list order;
first P.flags1ac&1 returns word(word(P+4)+0) as part ID. There is no count bound,
HP/status check or fallback to a numeric part1. The event graph does not gate
its subsequent GetPartsPos on this returned ID.

GetStatusMg resolves actor, captures raw output,translates/fetches,then sends
F32(unit+114) to evtSetFloat(rawOutput);return2. It performs no status lookup.
GetHeight uses the same ordering then GetHeight leaf S16(unit+ce),writes through
evtSetValue and returns2. Neither supplies a destination sentinel or actor-null
check locally. Encoded floating storage is specified in the
[presentation follow-up](BATTLE_BADGE_RECOVERY_PRESENTATION.md).

## GetPartsPos output semantics

Resolve actor0,translate;resolve part1. Reload shared W,request unit fetch (its
return is discarded),then request BattleGetUnitPartsPtr(translatedActor,part).
Capture THREE raw output destinations only AFTER that lookup. Call
GetPartsWorldPos(part,&X,&Y,&Z) into stack floats. For each axis X/Y/Z in order,
independently skip raw destination-250000000;otherwise fctiwz its captured float
and write through evtSetValue. Conversion of later axes occurs after earlier
output callbacks,but all coordinates have already been captured. Aliased outputs
end with the last non-skipped axis. No evtSetFloat or extra hit offset here;
part lookup/world geometry follow the linked existing contracts.

Acceptance cases: raw actor semantics versus resolved actor in other wrappers;
null amount query0;negative damage minimum; skipped translated-1 preserving
remaining operand side effects; discarded part resolution; HP bookkeeping gate;
FP repeated shared owner lookup; narrowing before cap; body list order/-1;
float scale encoding; axis sentinel/alias/conversion order. Resource/body/scale
source writers,delayed validity,full VM and original execution remain open.
