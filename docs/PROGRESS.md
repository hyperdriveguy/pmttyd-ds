# Progress and next-session handoff

## 2026-10-12 — Action-class table builder contract

Closed _btlcmd_MakeActClassTable (35/43): kind-gated record regions with
tags 6/1/2/4/13, message ids 334–343, (0,3) flag-pair overwrites, count
publication and selected-index clamp; duplicate R3a/R3b blocks proven
identical by diff; msgSearch single-loaded-arg hazard noted. Callee
pouchGetPtr/msgSearch identities from symbol rows. doc:
docs/behavior/battle/BATTLE_MAKE_ACT_CLASS_TABLE.md.

Validation: mechanical re-checks (call counts, bases 1176/1188, all seven
msg offsets, all ten ids, all five flag masks); diff --check clean.
Docs-only, no make rerun. Next: party_flyMain, bero_normal, checkLand2,
hitVec, fadeEntry. Incomplete.

## 2026-10-12 — Porting-critical function documentation

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

Battle system entry: delegates to `0x800f8e28` dispatcher; sets battle-active
flag when dispatcher returns 0.

### Input category

| Function | Address | Size | File |
|----------|---------|------|------|
| `btlevtcmd_ResultACDefence` | `0x80107694` | 144 B | `input/BTLEVTCMD_RESULT_AC_DEFENCE.md` |

Defense action-command result handler: resolves unit, calls defense result
routine at `0x800fa1b8`, returns 2 (complete).

### Field category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evt_bero_mapchange` | `0x8007aad0` | 176 B | `field/EVT_BERO_MAPCHANGE.md` |
| `evt_mario_jump_pos` | `0x800e4c54` | 1984 B | `field/EVT_MARIO_JUMP_POS.md` |

`evt_bero_mapchange` handles room transitions via seqMain. `evt_mario_jump_pos`
performs scripted movement to a target position with 4 modes and collision
integration.

### Memory category

| Function | Address | Size | File |
|----------|---------|------|------|
| `swSet` | `0x800858fc` | 64 B | `memory/SW_SET.md` |

Story switch setter: bit-packed with unusual 5-bit rotation; 258 direct DOL
callers, 15 REL references. Critical for persistent story state.

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

- Document battle turn sequence (`btlseqTurn` @ `0x80042b94`, 7896 B)
- Document event command interpreter (`evtmgrCmd` @ `0x800316b8`, 5048 B)
- Document message display (`msgDisp` @ `0x80050b88`, 2900 B)
- Document damage calculation pipeline (`BattleCalculateDamage` @ `0x8005f958`)
- Document save state machine (`cardMain` @ `0x800b2508`)
- Recover full motion ID semantics and subsystem call chains
