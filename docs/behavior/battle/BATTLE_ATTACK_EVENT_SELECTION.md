# Complete attack-command event selection and record classification

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Static evidence: complete `_btlcmd_SetAttackEvent` at 0x80120268, size 0x5b4,
three original dispatch tables, and the called local data/audience leaves.
Verify with `python3 tools/verify_battle_attack_selection.py`: 11 complete or
linked body identities, 301 instruction anchors, eight spans and seven scoped
DOL direct calls. No original execution or complete menu/event lifecycle is
claimed. Read [cursor selection](BATTLE_SELECTED_TARGET_AND_WEAPON.md),
[weapon classification and data leaves](BATTLE_WEAPON_ATTACK_CLASSIFICATION.md),
[byte counters](BATTLE_ACTION_RECORD_BYTE_UPDATES.md) and
[turn-rule consumers](BATTLE_ACTION_RECORD_TURN_RULES.md).

## Inputs, capture order and writes

Inputs are unit pointer U in r3 and command pointer M in r4. Retain both.
Read U8(M+0x551), capture global battle pointer B from 0x8041eb90, then read
word T=M+0x540. These reads precede the early override. The B capture remains
the owner of every record call in this routine, even if a called helper reads
a different global battle pointer. No unit, command or battle null guard exists.

The outputs are word U+0x2a0 (event pointer), word U+0x2ac (execution category)
and, on selected branches, byte U+0x20. They are separate stores, with branch
specific order. This routine does not create, run or wait for the selected
event; it does not pay cost, produce damage, choose a target, validate input or
clear the action-record bank. It has no defined scalar return contract.

If the captured override byte is nonzero, write byte U20=0, event pointer
U2a0=0x80416b40 (`btldefaultevt_Wait`), then word U2ac=1 and finish. The
T value is still read, but no table lookup or record increment occurs.

Otherwise unsigned T>13, including negative word values, uses the default:
write event Wait, then category 0. Preserve U20 and all records. Dispatch table
0x80364004 contains exactly the following 14 entries:

| T | Branch entry | Behavior |
| --- | --- | --- |
| 0, 1, 2, 4, 6 | 0x8012053c | Selected weapon branch below |
| 3, 7, 8, 10 | 0x801207f4 | Default Wait/category 0 |
| 5 | 0x801202d4 | Subcommand table below |
| 9 | 0x80120694 | U20=1, ChangePosition event 0x80351754, category 7, in that order |
| 11 | 0x801206b4 | Audience attack branch |
| 12 | 0x80120768 | Audience move branch |
| 13 | 0x801207dc | LectureEnd event 0x80416b54, then category 3 |

These numbers are actual table indices, not inferred UI labels. Their producers,
reachability and menu presentation remain open.

## T=5 subcommands and counter stores

Read unsigned word S=M+0x548. Unsigned S>8 uses Wait/category 1, event store
before category store; no U20 or record change. Table 0x80363fe0 is:

| S | Entry | Event and category | Counter offset in captured B |
| --- | --- | --- | --- |
| 0 | 0x80120494 | ChangeParty 0x8034de14, category 1 | 0x16f45 if U.kind==222, otherwise 0x16f46 |
| 1 | 0x801203ec | Charge event from weapon table, category 0 | 0x16f4f / 0x16f50 |
| 2 | 0x80120440 | SuperCharge event from weapon table, category 0 | 0x16f51 / 0x16f52 |
| 3 | 0x801203ac | Escape 0x8034e7a4, category 1 | 0x16f53 / 0x16f54 |
| 4 | 0x801204f4 | ProtectPartner 0x80416b10, category 1 | None |
| 5 | 0x8012032c | Appeal 0x8034e41c, category 1 | 0x16f49 / 0x16f4a |
| 6 | 0x801202f8 | Defend 0x80416b6c if kind222, otherwise DefendParty 0x80416b88; category 1 | None |
| 7 | 0x8012036c | Wait 0x80416b40, category 1 | 0x16f49 / 0x16f4a |
| 8 | 0x8012050c | `BtlUnit_GetData(U,2)` result, category 3 | None |

The slash means kind222 versus EVERY other signed word kind U8. There is no
224..230 partner range gate in this routine. Each counter is one call to
AddCount, after its listed event/category writes; its >=100 no-write guard is
the linked byte contract. Selecting S5 and S7 increments the same byte pair.
A missing data key for S8 publishes event pointer zero, with no Wait fallback.

S6 writes category 1 BEFORE reading kind and publishing its chosen event.
S1/S2 write category 0 BEFORE kind lookup and their event write. For S1,
kind222 reads word(0x803550a8+0xb0)=0x80416cb0 (NormalCharge); other kinds read
word(0x80355168+0xb0)=0x80416ccc (NormalChargeP). For S2 the analogous source
records are 0x80355228/0x803552e8, whose b0 fields contain 0x80416ce8/0x80416d04
(SuperCharge/SuperChargeP). These are actual memory reads, not hardcoded script
substitution; neither branch tests the loaded event pointer for zero.

S0 writes its event, category 1 and word M+0x4a4=0, in that order. It then reads
word at FIXED address 0x80354208 (within `marioWeapon_UltraHammerFinish`, +0x60).
If that word's low bit is set, write byte U20=3; otherwise preserve U20.
Only afterward read kind and increment the selected counter. The unusual fixed
address follows `lwz 0,0x63f4(r3)` with r3=0x8034de14 at 0x801204b4. Do not
replace it with a battle/global flag or a ChangeParty-relative semantic field
without evidence of its writers and purpose. S8 calls GetData before either
output write. Other S rows write event then category then their optional count.

## Selected weapon event and classification

Pass (M,T,&cursor) to `_btlcmd_GetCursorPtr`; the stack cursor output is
uninitialized and its scalar return is ignored. Read cursor.index, compute
wrap32(M+28*index), then load weapon W from that address+0x80. Read word W+b0
as event pointer. Write U2a0=event, then U2ac=0. Reload U2a0; if zero, write
Wait to U2a0 only. W is retained for classification; fallback does not convert
it to a Wait weapon. W itself is dereferenced without a null check.

Next the seven-entry table 0x80363fc4 dispatches captured T. A defensive
unsigned T>6 would finish without counting; all reachable selected modes here
are <=6. Its entries are:

| T | Entry | One AddCount request |
| --- | --- | --- |
| 0 | 0x801205a8 | B+0x16f38 unconditionally |
| 1 | 0x801205b8 | B+0x16f39 unconditionally |
| 2 | 0x801205f4 | Special pointer/owner/classifier branch below |
| 3 | 0x80120808 | None; not selected by the outer table |
| 4 | 0x801205c8 | B+0x16f3a if classifier(W)!=0, otherwise B+0x16f3b |
| 5 | 0x80120808 | None; outer table selects subcommands instead |
| 6 | 0x80120668 | B+0x16f55 if classifier(W)!=0, otherwise B+0x16f56 |

For T2 compare retained W with EXACT pointer 0x8036aa84
(`ItemWeaponData_Naniga_Okorukana`). Equality finishes with no count or kind
read. Other pointers read U.kind: kind222 calls classifier(W) and increments
B+0x16f41 for nonzero, B+0x16f42 for zero. EVERY non222 kind instead increments
B+0x16f43 or B+0x16f44 for the same classifier outcomes. Pointer identity is
required: a byte-identical clone at another address does not take the exemption.
No resource eligibility, actual hit, successful action command, executed damage
or event completion predicate participates in these counts.

## Audience branches and signed byte normalization

T11 and T12 first read S8(U20). Both replace 6 with 5, 7 with 6, 9 with 8,
and EVERY other signed value (including 8 and negative bytes) with 4, writing
one byte before their helper call. There is no default preservation branch.

T11 reads word U0 and calls `btlataudGetAttackEventPtr(U0)`. Write its result
to U2a0, then category 3. Read U.kind and call AddCount(B+0x16f47) for kind222,
otherwise AddCount(B+0x16f48). After that call, read word B+0x19090. If bit0
is zero, finish without reading the adjacent handle. If set, read word
B+0x19094 and call `msgWindow_Continue(handle)`, ignoring its result. See
[message continuation](../message/MESSAGE_WINDOW_LIFECYCLE.md). The count precedes this
flag/handle read. It can therefore be observed before presentation resumes.

T12 reads word U0 and passes it to `btlataudGetMoveEventPtr`, whose local body
ignores that argument. Write returned event to U2a0, then category 3 and finish;
there is no record increment or message continuation on this branch. The
helpers' global recapture, data key and present-item choice are in the linked
classification/data contract. A category-3 null result from S8 receives no
fallback, whereas the T11 helper supplies a fallback for data-key result zero.

## Call evidence, implementation checks and remaining boundaries

A scan of the seven DOL text sections finds seven aligned direct linked calls
to the scoped targets: BattleCommandInput calls this selector at 0x801231ac;
selector calls classifier at 0x801205c8, 0x80120610, 0x8012063c and 0x80120668;
and audience helpers at 0x80120714/0x801207c8. This proves local direct call
sites, not full BattleCommandInput state, REL/indirect callers or call frequency.
The earlier byte verifier separately pins all 24 AddCount sites in this body.

Acceptance fixtures should cover override-before-dispatch with preserved reads;
all 14 modes and nine subcommands; signed negative/out-of-range words; stale
U20 preservation versus explicit writes; every kind split including kind223;
output-store order and captured B versus helper global recapture; invalid cursor
output and null W as adapter preconditions; null event fallback versus retained
weapon classification; exact special-weapon pointer; classifier short circuit;
counts at99/100/255; all signed normalization cases; and T11 message flag after
count versus T12's absence of both services. Recording “an attack occurred” only
at event completion would violate this selector's bookkeeping boundary.

General rule judgment, record raw writers/reset/save ownership, full menu and
command state producers, selected source scripts, event category consumers,
audience/message backends, numeric/address alias services, turn/scene cadence
and original executed traces remain open under JUMP-01/DMG-02/STATUS-01/EVT-01.
