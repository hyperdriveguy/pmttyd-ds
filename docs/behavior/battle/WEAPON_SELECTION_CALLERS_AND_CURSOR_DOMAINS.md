# Weapon selection callers, cursor domains and type writers

US G8ME01 revision0; recovered 2026-10-13. Evidence B: original DOL bodies for
`btlevtcmd_CommandGetWeaponAddress` (801057e8/0x84), `BattleGetSelectWeapon`
(8012021c/0x48), `_btlcmd_SelectWeaponDecide` (80123210/0x14c),
`_btlcmd_UpdateSelectWeaponTable` (80123db0/0x10c),
`_btlcmd_MakeSelectWeaponTable` (80123ec0/0xd3c) and concrete event sources.
This answers the EVT-01 static request for menu-type writers and cursor
reachability. No original runtime trace or complete Game binding is claimed.

## Event sources invoking CommandGetWeaponAddress

`btlevtcmd_CommandGetWeaponAddress` is invoked exclusively through event USER_FUNC
(opcode 91) operands in original attack streams. Two ordinary attack streams
call it; the lecture Jump/Hammer streams call it four times each.

| Stream | Address | Command offset | Callback operands after function pointer |
|--------|---------|----------------|------------------------------------------|
| `marioAttackEvent_NormalJump` | 80355cf0 | 0x80355e80 | (-2, 1, 1, 0) — SetupAC |
| `marioAttackEvent_NormalJump` | 80355cf0 | 0x8035600c | (-2, 1, 1, 0) — SetupAC (first-attack brother) |
| `marioAttackEvent_NormalHammer_Core` | 8035d6d4 | 0x8035d988 | (-2, 3, 1, 0) — SetupAC |
| `marioAttackEvent_NormalHammer_Core` | 8035d6d4 | 0x8035dc1c | (-2, 3, 1, 0) — SetupAC |
| `lecAttackEvent_Jump` | REL | 431104/192/708/876 | (-2, 1/3, 1, 0) — lecture parameter branches |
| `lecAttackEvent_Hammer` | REL | 433500/434588/434764 | (-2, 3, 1, 0) — lecture parameter branches |

The wrapper at 801057e8 resolves the calling event's arguments as follows:
1. Operand 0 → `evtGetValue` → `BattleTransID` → `BattleGetUnitPtr` (actor)
2. Operand 1 → `evtGetValue` (command ID, 1 or 3)
3. Operand 2 → **raw** (option word, becomes `battle+0x1c94`)
4. Operand 3 → `evtGetValue` (becomes `battle+0x1c98`)
Then calls `BattleActionCommandSetup(battle, ID, unit, rawOperand2, resolvedOperand3)`.

For the four ordinary stream sites, raw operand 2 is literal `1`. The lecture
streams pass the same literal `1` for both Jump (ID 1) and Hammer (ID 3).

## BattleGetSelectWeapon — private cursor allocation

`BattleGetSelectWeapon` at 8012021c allocates its **own private stack slot** for
the cursor output. It does **not** initialize this slot before querying the
current TYPE from `M540` (`battle+0x1c5c`). The function body:

1. Captures battle work pointer at `r13+0x14a0` → battle base `B`
2. Computes `M = B + 0x171c` (wrap-to-0 returns 1)
3. Adds offset 0x171c (5916) to form pointer `M`
4. Calls `_btlcmd_MakeSelectWeaponTable(M, 8)` at 80124cb8
5. Reads table count from stack offset 8 (the output count)
6. Multiplies count by 28 (descriptor stride) and adds to table base
7. Loads descriptor+128 (weapon address) into r3 and returns

The private stack slot for cursor (at stack offset 8) is **uninitialized** on
entry. If `MakeSelectWeaponTable` produces zero rows, the slot retains whatever
stack residue existed. There is **no default cursor value written** by this
function. The 16 direct DOL call sites pinned in the call-site census are all
event-driven; no direct DOL caller passes a pre-initialized cursor.

## _btlcmd_SelectWeaponDecide — Input state 11 same-invocation reuse

`_btlcmd_SelectWeaponDecide` at 80123210 is called from `BattleCommandInput`
state 11 (weapon selection confirmation). Its cursor behavior:

1. Entry captures battle `B`, computes `M = B + 0x171c`
2. Calls `_btlcmd_GetCursorPtr(M, 14, &newSP08)` at 80122a14 — **initializes
   stack word `newSP08` with type-14 cursor** (class cursor selected by
   acting kind 222 vs other)
3. Calls `_check_present_item` and other services
4. State 11 branch at 80122d20 calls `_btlcmd_GetCursorPtr(M, currentType,
   &THE SAME newSP08)` at 80122d34 — **reuses the identical stack word**
3. Later reads that stack word's cursor index to select a weapon-table row

If `currentType` is unsupported (9, 11, 12, 13, or unsigned >14) and no
intervening operation changed `newSP08`, the helper **leaves the type-14 cursor
intact from the same invocation**. This is concrete initialization and reuse of
a **local stack slot owned by Input's invocation frame**. It is **not** a
fallback for `BattleGetSelectWeapon`'s DIFFERENT private uninitialized slot.

## _btlcmd_UpdateSelectWeaponTable — type-dependent descriptor field

`_btlcmd_UpdateSelectWeaponTable` at 80123db0 updates the descriptor field at
offset +152 (type-dependent value) for each weapon-table row. The mapping:

| Weapon kind (r31) | Value written to descriptor+152 |
|-------------------|--------------------------------|
| 0 (Jump) | 4 |
| 1 (Hammer) | 5 |
| 4 (Partner) | 3 |
| Other | 1 or 2 (specific branches) |

The function iterates the table built by `MakeSelectWeaponTable`, computes the
kind-dependent value, and stores it at `descriptor+152`. It does **not** modify
cursor state or type words.

## _btlcmd_MakeSelectWeaponTable — table builder with late overrides

Complete builder at 80123ec0. Key behaviors relevant to cursor/type domains:

- **Input**: battle work `B`, base descriptor address
- **Output**: weapon-table rows at `B+0x171c+...`, count at stack offset 8
- **Branch on acting unit kind (r31)**:
  - Mario (kind 222): builds Jump + Hammer + badge + item + Special + party rows
  - Partner: builds partner weapon rows via delegate
  - Other: returns 0 rows
- **Late first-row override**: if `battle+0x1c9c` (setup state) is 1000, the
  first row's weapon address is overridden with `battle+0x1c98` (option word)
- **Empty count preservation**: if no rows selected, count stays 0; table base
  is still written but no descriptors are populated
- **Type 12 writer**: the audience-item branch at 801201a8 explicitly writes
  `M540=12` (TYPE) and `M544=previous TYPE`, then `M00=29` (input state)
- **Type 13 writer**: `BattleSetConfuseAct` writes `M540=13` (flustered)

## Type/previous-type/Input-state word map (M = B+0x171c)

| Offset | Word | Purpose | Writers |
|--------|------|---------|---------|
| +0x00 | `M00` | Input state (0..29) | Input dispatch, audience-item, confuse, ActMenu |
| +0x540 | `M540` | Current TYPE | audience-item(12), confuse(13), ActMenu(9), Input restore(11/12) |
| +0x544 | `M544` | Previous TYPE | audience-item (copies current→previous) |
| +0x540 | `M540` | TYPE restore | Input state 0: if byte `M574`≠0 and TYPE∈{11,12}, restores `M544→M540` |

## Cursor query reachability — what is and isn't established

### Established (static recovery)
- **Input state 11**: same-invocation initialization of `newSP08` with type-14
  cursor, then reuse on same `newSP08` for current TYPE read. This stack slot
  is owned by `BattleCommandInput`'s frame.
- **Audience-item branch**: explicit TYPE 12 write with previous-type capture.
- **Confuse branch**: explicit TYPE 13 write.
- **ActMenuMain**: two TYPE 9 writes at 80122380/8012254c/80122754.
- **16 direct DOL call sites** for weapon selection functions (pinned in census).
- **No default cursor** written by `BattleGetSelectWeapon`, `_btlcmd_SelectWeaponDecide`,
  `_btlcmd_UpdateSelectWeaponTable`, or `WeaponSelectMenuSetup`.

### NOT established (requires runtime/event trace)
- **BattleGetSelectWeapon's private slot**: uninitialized; original stack
  residue at its call sites unknown. No evidence proves it is ever called while
  TYPE is in {9, 11, 12, 13, >14} with a valid cursor already present.
- **WeaponSelectMenuSetup's private slot**: also uninitialized; same issue.
- **Query-specific reachability**: whether `CommandGetWeaponAddress` callers
  (event streams) ever invoke `BattleGetSelectWeapon` while TYPE is an
  unsupported value, or whether the event context guarantees a supported TYPE.
- **Global valid-type invariants**: the explicit TYPE 12 writer in audience
  branch does not prove TYPE 12 is unreachable or irrelevant to other query
  callers. Safe query-specific reachability remains unproven.
- **Physical stack aliases**: Input's `newSP08` and `BattleGetSelectWeapon`'s
  private slot are **different stack words** in different call frames. No
  evidence they alias or that one initializes the other.

## Direct DOL call-site census (16 sites)

| Function | Direct DOL callers |
|----------|-------------------|
| `BattleGetSelectWeapon` | 6 (event sources) |
| `_btlcmd_MakeSelectWeaponTable` | 4 (table builder) |
| `_btlcmd_SelectWeaponDecide` | 3 (decision) |
| `_btlcmd_UpdateSelectWeaponTable` | 3 (table update) |
| `WeaponSelectMenuSetup` | 3 (setup) |
| `btlevtcmd_CommandGetWeaponAddress` | 0 (invoked via event USER_FUNC operands) |
| **Total** | **16 aligned bl/bla sites** |

REL event operands, indirect/raw-pointer calls and dynamic counts are excluded.

## Remaining blockers for Game binding

1. **BattleGetSelectWeapon cursor initialization**: either prove its call sites
   always have a valid cursor in the private slot (by tracing event context), or
   add a native adapter that initializes it (but this must not replace original
   stack residue behavior).
2. **WeaponSelectMenuSetup cursor initialization**: same issue.
3. **Query-specific type invariants**: for each of the 6 `BattleGetSelectWeapon`
   callers, determine the TYPE value at call time. Current evidence only covers
   audience-item (12), confuse (13), ActMenu (9), Input state 11 (reuses 14).
4. **Unsupported TYPE handling**: types 9, 11, 12, 13, >14 leave helper outputs
   untouched. The native adapter currently reports/terminates unsupported domain
   and supplies no default weapon. Original behavior for these cases at the
   specific query callers is unknown.

## Linked contracts

- `WEAPON_MENU_SETUP_AND_DECISION.md` — complete setup/decision functions
- `WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md` — type domains, Input reuse
- `BATTLE_SELECTED_TARGET_AND_WEAPON.md` — cursor/query contract
- `ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md` — class table producer
- `ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md` — ActMenu TYPE writers
- `AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md` — TYPE 12 writer
- `CONFUSION_CLASS_SELECTION_AND_MENU_TYPE_WRITE.md` — TYPE 13 writer
- `WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md` — full builder
- `NORMAL_JUMP_ROOT_REGISTRATION.md` / `NORMAL_JUMP_CALLBACK_REGISTRY.md` — Jump callback census

## Verification

```sh
python3 tools/verify_weapon_menu_domains.py
python3 tools/verify_weapon_selection_cost.py
python3 tools/verify_jump_source_registry.py
```

ISO reads are read-only. Extracted assets remain outside source history.