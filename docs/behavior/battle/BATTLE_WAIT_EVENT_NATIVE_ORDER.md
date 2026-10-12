# Wait-event replacement and guard polling: native integration order

US G8ME01 disc0 revision0,static original inspection2026-10-12.
Read [recovery wrappers](BATTLE_RECOVERY_EVENTS.md),
[wait/camera leaves](BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md) and
[dispatch helpers](BATTLE_EVENT_DISPATCH.md). No original execution/timing claim.

StopWaitEvent8010a1fc/70 resolves/translates actor then fetches current battle
unit. Nonzero unit.word290 is sent to DeleteID;AFTER return clear the SAME unit's
290. Zero skips both deletion and store. Return2. A missing/stale live event does
not suppress the clear;there is no separate ID-liveness or unit-null guard.

StartWaitEvent8010a26c/64 uses the same actor/lookup pipeline. Its outer unit28c
script test is BEFORE BattleRunWaitEvent801296e0/6c. Thus script0 skips the helper
and preserves any tracked ID. The helper called DIRECTLY instead always cancels
and clears nonzero290 first,then reloads28c. Nonzero script creates ROOT priority10,
flags0. AFTER constructor,read unit.word0 into event.owner160;THEN read event.id15c
and store to unit290. Helper returns0,wrapper returns2. No child wait flag,
firstCall gate,pose-only substitution or flags20 append is requested here.

WaitGuardMove8010a1c8/1c ignores event and firstCall. Read CURRENT battle8041eb90,
then wordef4. Numeric bit01 set returns0;otherwise2. Original seven instructions
mask bit0,negate it and AND-complement with2. No movement/HP/unit/RNG/clock request
or local flag write occurs. Owner of that flag supplies its later clear;the body
alone does not prove guard duration or how many simulation/render calls occur.

Original inspection-only ELFs remain `/tmp/pm-start-wait-event.elf`,
`pm-battle-run-wait.elf`,`pm-wait-guard-move.elf`,outside source history. Reproduce
with `tools/inspect_dol_function.py` and original US symbol index. Native integration
uses actual root constructor/DeleteID,shared counters/registry/unit bytes and
original-address callback bindings. Tests preserve live script/actor/ID reads,
zero outer gate versus direct cancellation,stale IDs,flags0 scheduling and real
USER_FUNC polling/resumption. Unit-script writers,source/lifecycle/outer timing,
original animation and complete Jump/Game composition remain separate requirements.
