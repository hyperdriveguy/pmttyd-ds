# RESULT-01 — Tutorial battle result publication, victory and save-unblock

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)); full
game playability remains the end goal. The documentation slice is explicitly
requested; no independent research-agent ownership or completion claim is made.

## Open todos

- [x] Complete InterruptStop flag10/20 callback and bounded original Escape
  flag10 route — [escape publication](../behavior/battle/BATTLE_INTERRUPT_STOP_FLAGS_AND_ESCAPE_RESULT_ROUTE.md).
  Raw gate/resolution/fresh owner, AC result-bit2 exclusion and terminal0 call
  pinned;736 annotated DOL streams contain one call. Other4/5 domains open.

- [x] Scoped ordinary ending admission from two turn wait gates and first-act
  index3 — [ending gates](../behavior/battle/BATTLE_END_ADMISSION_GATES_AND_CONCLUSION_CALLERS.md).
  Event barrier, ordered bank writes, audience request, dispatch bindings and
  ten-site direct DOL conclusion-query census pinned; full controllers open.

- [x] Complete local BattleCheckConcluded group publisher —
  [fresh conclusion reads](../behavior/battle/BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md).
  Two ordered scans/config reread/status27/attribute snapshots, group result
  writes and count>=2 OR flags10/20 return pinned; actual producers remain open.

- [x] Exact local ending phase0 result publisher and complete set/get/
  enemy-belong leaves — [result priority](../behavior/battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md).
  Flag40→4,20→5,own conclusion→3,opponent→1,10→4/no-write priority and
  captured record/getter order pinned; full ending and4/5 meanings open.

- [ ] The original battle-end result record producer: which owner writes the
  result, in what order relative to unit death/end, and which values mean win,
  loss, flee and timeout.
- [ ] Victory/defeat determination and the story-advance write (the value or flag
  that admits the post-battle field scene).
- [ ] Reward publication (EXP/coins/items/drops) for the tutorial encounter, if
  any, and the field-return handoff.
- [ ] The exact condition that unblocks saving after the encounter, and the
  original save-point existence/placement at this stage.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone: the native battle result is a development token.
`Game::returnFromBattle` in `source/runtime/game.cpp` writes
`fieldBattleWork.result = harnessResult` with the comment "Development outcome
tokens only; original outcome producers are unrecovered"; victory is synthesized
from the harness HP check (`if (enemyHP==0) scene=Scene::Victory`); and
[the opening playtest chain](../behavior/story/NATIVE_OPENING_PLAYTEST_CHAIN.md)
records that "result0 is not victory, story5 is not synthesized and saving remains
blocked during this unfinished battle". An older deliverable already bound the
field-side wait condition (`include/field/field_battle.hpp`: "evt_npc_wait_battle_end
(8004eed8): sequence2 AND any nonzero result"), but the producing side — what
writes that result, how victory is decided, what story advance it publishes and
when saving is unblocked — is unrecovered. Please recover the original
battle-result publication, victory/defeat determination, story advance,
reward/field-return and save-unblock contract for the tutorial encounter. No lock
reserved; the cross-scene persistence contract is [DEMO-01](DEMO-01.md) and the
flag/save-block map is [GSF-01](GSF-01.md).

## Answers and delivery notes (append-only history)

No delivery yet.

## 2026-10-13 — Verified common result publisher and priority

[Phase0 publication](../behavior/battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
registers the common producer already identified by underground recovery under
RESULT-01, with fresh complete leaf/scoped source verification. Result1 requires
opponent conclusion after override/own-loss priority; nonzero field-wait result
is not victory proof. No-write retains prior record. Ready bit/sequence/field/
story/save-unblock remain separate. Flag10/20/40 providers must establish flee/
timeout semantics before4/5 receive those labels.

Validation:10 spans/91 anchors/12 scoped calls;15 leaf+71 prefix words,
16 mathematical vectors/masks/capture/five setter/get/table sites and exhaustive
nine-site direct DOL census/fresh pins checked. Links/scoped diff passed.
Docs only; no make rerun; raw artifacts stay /tmp. RESULT-01 stays OPEN:
full conclusion/death/ending/override/reward/tutorial field/story/save and
original-runtime/native real-result binding remain unproved.

## 2026-10-13 — Complete conclusion group publication/fresh reads

[Conclusion contract](../behavior/battle/BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md)
strengthens existing barrier/conclusion evidence for RESULT-01. Second scan
requires40000 before status, then rereads10000000 after; no40000 retest or20000
exclusion there. Config bit2 rereads after first scan. Count>=2 or fresh10/20
forces Boolean1;40 alone does not. No HP-sum/result/ready/story write here.

Validation:3 spans/all96 words/four calls;20 literal vectors/group writes/
fresh config/attributes/return-mask/call sites checked; read-only ISO pins pass.
Links/scoped diff pass. Docs only; no make rerun; raw artifacts stay /tmp.
Actual tutorial config/status/death/override/end-bank admission and original
execution/native result binding remain OPEN; parent stays OPEN.

## 2026-10-13 — Scoped ending admission and conclusion caller census

[Ending gates](../behavior/battle/BATTLE_END_ADMISSION_GATES_AND_CONCLUSION_CALLERS.md)
bridge scoped conclusion callers to bank0=3/bank7=07000000. Two turn gates wait
first; first-act index3 calls audience before ending writes and has a different
zero-result continuation. No immediate end redispatch or result/story/save write.
The ten-site direct census distinguishes other queries from proven admission.

Validation:18 spans/215 anchors/17 scoped direct calls; literal instructions,
branches, retained owners, returns,93 table words,35 relevant dispatch bindings,
ten mathematical models and exhaustive ten-site DOL census checked; read-only
ISO pins pass. Links/scoped diff pass. Docs only; no make rerun. Full controller/
tutorial/death/override/ending/rewards/field/story/save/original execution remain
OPEN. Raw listings/ELFs/generator stay /tmp.

## 2026-10-13 — Escape-domain override publication

[InterruptStop/Escape](../behavior/battle/BATTLE_INTERRUPT_STOP_FLAGS_AND_ESCAPE_RESULT_ROUTE.md)
proves literal0 publishes flag10 with a nonzero raw call gate; operand1 publishes
20 but its gameplay domain is not established. Escape's mask2-set AC result arm
ends before publication; mask-clear continuation retains presentation/child
waits before the terminal request. Result4 is reached only if higher-priority
result branches do not override10. This does not label every4 escape or5 timeout.

Validation:7 spans/80 anchors/one callback service; all30 callback words, scoped
script boundaries/order,16 mathematical vectors and736-stream/one-call original
DOL event census checked. ISO pins and existing IF/dispatcher verifiers pass;
links/scoped diff pass. Docs only; no make rerun. Raw artifacts stay /tmp.
Full Escape/tutorial admission/other writers/ending/reward/field/story/save and
original execution stay OPEN; parent stays OPEN.

## 2026-10-13 — Native session verification: result transport setters/getters

Independently re-read the two 8-byte transport functions the field
[battle-entry](../behavior/field/FIELD_BATTLE_ENTRY_AND_RETURN.md) and result
deliveries cite, using `tools/inspect_dol_function.py --address` against the
read-only US ISO. `0x80114508` disassembles to `stw 4,16(3); blr` (store the
value at `[ptr+0x10]`) and `0x80114500` to `lwz 3,16(3); blr` (load `[ptr+0x10]`).
This confirms the cited addresses, the 8-byte sizes and the `+0x10` semantics of
`BattleInformationSetResult`/`BattleInformationGetResult`. It does not establish
the result-value meanings, the victory/defeat producers, or the story/save
writes, which remain OPEN.

Validation: two DOL spans (8 bytes each) disassembled read-only; artifacts in
`/tmp`. No source change.

## 2026-10-13 — Native session verification: phase0 result priority

Independently disassembled the
[phase0 publication](../behavior/battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
priority region (`0x802152b0`..`0x80215350`) from the read-only US ISO. Every
ordered alternative reproduces exactly:

- flags read once as `lwz 5,0xef4(26)`; `rlwinm. 0,5,0,25,25` (mask`0x40`) →
  else `li 4,4`/`bl 0x80114508` at `802152cc`.
- `rlwinm. 0,5,0,26,26` (mask`0x20`) → `li 4,5` at `802152e4`.
- `slwi 0,28,3`+`lwz 0,12(4)` = `B.word(0xc+8*ownBelong)` → `li 4,3` at
  `80215308` (ownBelong = GPR28 sign-extended Mario byte, `extsb 28,28`).
- `slwi 0,3,3`+`lwz 0,12(3)` = enemyBelong word → `li 4,1` at `8021532c`.
- `rlwinm. 0,5,0,27,27` (mask`0x10`) → `li 4,4` at `80215344`.

All five setters pass captured `I` (`mr 3,27`), both side words load only when
reached, and the reread is `bl 0x80114500` (GetResult) at `8021534c`. This
confirms the priority order (40→4, 20→5, own→3, enemy→1, 10→4), result4's two
distinct flag branches and that 5 precedes the side-3 conclusion. It does not
establish the flag10/20/40 writer domains or the tutorial outcome, which remain
OPEN. Validation: scoped DOL region disassembled read-only; artifacts in `/tmp`;
no source change.

## 2026-10-13 — Native session verification: InterruptStop and Escape route

Independently disassembled two ISO regions behind the
[Escape delivery](../behavior/battle/BATTLE_INTERRUPT_STOP_FLAGS_AND_ESCAPE_RESULT_ROUTE.md):

- `btlevtcmd_InterruptStop` `0x80105368` (0x78 hex): `mr 31,4` retains the raw
  gate; `lwz 5,0x18(3)` then `lwz 4,0(5)` reads operand0 from `WORD(event+0x18)`;
  `bl 0x80031260` (evtGetValue); `lwz 4,7280(13)` = `*(r13+0x1c70)` = `0x8041eb90`
  is the post-resolution battle owner. Branch decode (`bt 2`=`beq`, `bf 0`=LT
  false) yields exactly: gate0→no write; gate!=0 & operand==1→`ori 0,0,0x20` at
  `B+0xef4`; gate!=0 & operand==0→`ori 0,0,0x10` at `B+0xef4`; else no write;
  `li 3,2; blr`. No `evtSetValue`/result/sequence write.
- The Escape script tail at `0x8034f1d4` is `0002005b 80105368 00000000` — the
  lone `InterruptStop` USER_FUNC(91) with literal operand 0 — followed by
  `00000002` END_EVENT at8034f1e0 and `00000001` END_SCRIPT at8034f1e4. The
  mask2 region near8034eba8 shows `0002001f fe363c86 00000002` (LW6 mask2).

Not re-run: the 736-stream "exactly one site" census and the mask-set arm's
`WaitEventEnd(LW15)`@8034f038/END_EVENT@8034f044 ordering; those remain the
manifest's pinned evidence. Verified bytes only; no original execution, no source
change.

Addendum: the mask-set arm tail is also verified. `0x8034f038` is
`0002005b 8011389c fe363c8f` = WaitEventEnd(LW15) (`0xfe363c8f` = LW15), followed
by `00000002` END_EVENT at8034f044 before `00000021` END_IF at8034f048 — so the
mask2 arm ends the event and does not fall through to the terminal
`InterruptStop(0)`.
