# Progress and next-session handoff

## 2026-10-12 — Porting-critical large function documentation

Added detailed documentation for high-priority functions needed for porting,
as identified in `docs/behavior/meta/US_COMPLETION_AND_PRIORITIES.md`. All
new files follow the per-subsystem folder requirement from AGENTS.md.

### Overworld category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioMain` | `0x8005a7e0` | 2528 B | `overworld/MARIO_MAIN.md` |

Primary field player update: motion dispatch, gravity, subsystem ordering,
battle transitions, and save-slot management.

### Battle category

| Function | Address | Size | File |
|----------|---------|------|------|
| `battleMain` | `0x800f904c` | 144 B | `battle/BATTLE_MAIN.md` |
| `BattleCalculateDamage` | `0x8005f958` | 1692 B | `battle/BATTLE_CALCULATE_DAMAGE.md` |
| `BattleSetStatusDamage` | `0x800fcf04` | 4220 B | `battle/BATTLE_SET_STATUS_DAMAGE.md` |
| `BattleDamageDirect` | `0x800feaa8` | 2852 B | `battle/BATTLE_DAMAGE_DIRECT.md` |
| `BattleCheckDamage` | `0x800fb9cc` | 2560 B | `battle/BATTLE_CHECK_DAMAGE.md` |
| `BattleCheckCounter` | `0x800fabcc` | 1444 B | `battle/BATTLE_CHECK_COUNTER.md` |

Battle system entry, damage calculation pipeline, status damage application,
direct damage, damage validation, and counter checking.

### Event category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evtmgrCmd` | `0x800316b8` | 5048 B | `event/EVTMGR_CMD.md` |
| `btlseqTurn` | `0x80042b94` | 7896 B | `event/BTLSEQ_TURN.md` |
| `btlseqAct` | `0x8011bb38` | 2220 B | `event/BTLSEQ_ACT.md` |
| `btlseqMove` | `0x8011c264` | 1680 B | `event/BTLSEQ_MOVE.md` |

Event VM command interpreter, battle turn/action/move sequence controllers.

### Message category

| Function | Address | Size | File |
|----------|---------|------|------|
| `msgAnalize` | `0x80081de0` | 5796 B | `message/MSG_ANALIZE.md` |
| `msgDisp` | `0x80050b88` | 2900 B | `message/MSGDISP.md` |
| `msgWindow_Main` | `0x8011bb38` | 2052 B | `message/MSGWINDOW_MAIN.md` |
| `msgMain` | `0x8008460c` | 1832 B | `message/MSG_MAIN.md` |
| `msgWindow_Disp` | `0x800808ec` | 1756 B | `message/MSGWINDOW_DISP.md` |
| `_evt_msg_print` | `0x800d2d60` | 1244 B | `message/EVT_MSG_PRINT.md` |

Message text analyzer, dialogue display, window update, message main loop,
window rendering, and event-driven message printing.

### Input category

| Function | Address | Size | File |
|----------|---------|------|------|
| `btlevtcmd_ResultACDefence` | `0x80107694` | 144 B | `input/BTLEVTCMD_RESULT_AC_DEFENCE.md` |

Defense action-command result handler.

### Field category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evt_bero_mapchange` | `0x8007aad0` | 176 B | `field/EVT_BERO_MAPCHANGE.md` |
| `evt_mario_jump_pos` | `0x800e4c54` | 1984 B | `field/EVT_MARIO_JUMP_POS.md` |

Room transition event command and scripted Mario movement.

### Memory category

| Function | Address | Size | File |
|----------|---------|------|------|
| `swSet` | `0x800858fc` | 64 B | `memory/SW_SET.md` |
| `cardMain` | `0x800b2508` | 1692 B | `memory/CARD_MAIN.md` |

Story switch setter and save/load state machine.

### Unit category

| Function | Address | Size | File |
|----------|---------|------|------|
| `pouchGetItem` | `0x800d5520` | 1188 B | `unit/POUCH_GET_ITEM.md` |
| `BtlUnit_Entry` | `0x80129644` | 1716 B | `unit/BTL_UNIT_ENTRY.md` |

Item retrieval from pouch and battle unit initialization.

### Format category

| Function | Address | Size | File |
|----------|---------|------|------|
| `__dec2num` | `0x80266834` | 4224 B | `format/DEC2NUM.md` |
| `__strtold` | `0x8026e68c` | 4108 B | `format/STRTOLD.md` |
| `__sformatter` | `0x8026cdf4` | 2476 B | `format/SFORMATTER.md` |
| `__ieee754_pow` | `0x8027067c` | 2096 B | `format/IEEE754_POW.md` |
| `__pformatter` | `0x8026ab78` | 1908 B | `format/PFORMATTER.md` |
| `float2str` | `0x8026b294` | 1820 B | `format/FLOAT2STR.md` |
| `__minus_dec` | `0x800d2d60` | 1276 B | `format/MINUS_DEC.md` |

Decimal conversion, string-to-float, printf/scanf formatters, power function,
float-to-string, and decimal subtraction.

### Platform category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioStMain` | `0x8006faec` | 176 B | `platform/MARIO_ST_MAIN.md` |

Main field system update orchestrator.

### Validation

All functions inspected with `tools/inspect_dol_function.py` against read-only
US G8ME01 ISO. Documentation records exact address, byte size, disassembly,
called functions, and data offsets. No original executable code linked or
copied.

### Limitations

- Many called helper functions remain undocumented
- Full structure layouts partially documented via field references
- Motion ID semantics and subsystem chains need further recovery
- Battle transition conditions require original event script analysis
- Shared helpers across commands need individual documentation

### Next tasks

- Document remaining medium-size functions (500-1500 bytes) by priority
- Recover full structure layouts for battle work, unit, and event contexts
- Cross-reference shared helper functions across command implementations
- Document remaining event command handlers and their argument processing
- Recover complete motion ID table and associated behavior chains
