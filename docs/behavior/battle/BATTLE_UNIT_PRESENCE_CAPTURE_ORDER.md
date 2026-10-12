# Status and data-membership event leaves: capture-order refinement

US G8ME01 disc0 revision0;static read-only-ISO inspection2026-10-12.
Refines [unit-query summary](BATTLE_UNIT_QUERY_EVENT_LEAVES.md),using the complete
[status getter/presence contract](BATTLE_STATUS_STORAGE.md) and actual CheckData
instructions. No original execution or Game unit-writer/lifetime claim.

CheckStatus80105f0c/94 retains E/A;resolves actor A0 and translates it. Read RAW
status A4,then RAW output A8 BEFORE current battle read/unit lookup. Narrow status
LOW8 as SIGNED8;do not GetValue(status). Call CheckStatus on selected unit,then
SetValue with normalized1/0 and return2. There is no event-layer ID-1 guard;
callee CheckStatus itself returns0 for null unit. Anchors:raw status80105f3c,
output80105f44,battle80105f48,narrow80105f4c,lookup80105f50,check80105f58.

CheckDataOfDataTable80109f88/88 retains E/A;resolves/translates actor,then CURRENT
battle/unit lookup. AFTER lookup,capture RAW output A4;resolve key A8 separately.
Only now call CheckData on retained unit,so a key resolver can mutate its table
pointer. SetValue captured output to membership result and return2. Anchors:
battle80109fbc,lookup80109fc0,output80109fc4,key resolve80109fd4,check80109fe4.
Neither wrapper uses firstCall or invents index/actor/null validation.

BtlUnit_CheckData801272fc/40 reads unit.word18;null TABLE returns0. For each8-byte
record,read key:first exact0 terminates with0,otherwise matching word key returns1.
It does not read the value word. Thus an existing key with value0 is PRESENT;
key0 is always terminator,not a found entry. This differs from GetData's returned
pointer/value and must not be implemented as GetData!=0. Null UNIT is not guarded.

Packed GetStatus follows BATTLE_STATUS_STORAGE. Single-byte cases read once and
copy to both result bytes. Poison/regen read duration then strength. Shared signed
pairs10..15 read STRENGTH first and only load duration when its sign admits the
requested side;rejected sides produce zero without duration read. Both source
bytes are computed before optional output writes;duration precedes strength,
including aliased output addresses. CheckStatus alone has a null-unit guard and
Electric's Zap Tap byte2f7 fallback when returned duration is0.

Inspection-only ELFs remain `/tmp/pm-unit-check-status-event.elf`,
`pm-unit-check-data-event.elf`,`pm-unit-check-data.elf` and earlier
`pm-move-get-status.elf`/`pm-move-check-status.elf`,outside history. Reproduce with
`tools/inspect_dol_function.py` and original US symbol index. Native tests cover
exact lookup/output/key capture,raw signed IDs,zero-valued membership,table
terminators,pair read order,Zap Tap and precomputed output aliases. Native storage
assertions express unsupported provider domains,not source recovery behavior.
