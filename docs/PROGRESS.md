# Progress and next-session handoff

Newest entries first under this single heading. Keep entries dated, write link paths relative to `docs/`, and record exact validation, limitations and next tasks in each entry. Append-only: do not rewrite earlier entries.

## 2026-10-12 — Native cross-check fixes: sink guard + default lanes

The native session's independent re-read falsified my water doc's sink
guard direction ([124]==0.0 initializes −6.0, not !=; fixed and credited)
and flagged the undocumented r4-sourced default fall lanes — added to the
bottomless doc ([r13−28176]+16 vs absolute 0x80416208+16 by [46]==5,
verified by direct reads). Both fixes verified against the listings. This
is the inbox cross-check loop working as designed: independent re-reads
catch single-bit errors that self-review missed.

Validation: direct-instruction re-checks, diff --check clean. Docs-only.
Next: triangle interiors, pose domains. Full-game incomplete.

## 2026-10-12 — HitVec region-graph correction (false asymmetry withdrawn)

Re-verified the traversal doc's region claims against the listing and found
my own region-1 asymmetry finding was backwards: R1 allocates counter=r30 /
pointer=r31 (inits `lwz 31,172` / `li 30,0`), regions 2–4 the reverse — same
loop semantics, no executability consequence. Withdrew the pointer-size
claim; kept the verified cascade edges (R1-hit→R2/R3-gateway, drains) and
the airtight R4-unreachable finding (no entry edge, dead fallthrough).
Two crashed verification scripts produced zero passing asserts this turn —
all claims above are re-verified by direct reads instead.

Validation: direct-instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains. Incomplete.

## 2026-10-12 — Water/void recovery: apply recovered bottomless drop-back lanes

Reviewed the
[water](behavior/overworld/OVERWORLD_WATER_RECOVERY_NUMERICS.md) and
[bottomless](behavior/overworld/OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS.md)
numerics deliverables against the `WaterRecovery` prototype. Independently
re-read the ISO DOL and confirmed the mot_bottomless drop-back lanes
(`0x80091bd4`: `[124]/[128]/[132]/[136] = 0.34/-0.002/-0.01/0.0`), so replaced
the prototype's authored `-.001/-.002/+.00024/-.00004` cascade (which matched
only `-0.002`) and pinned it in `tests/field_camera_tests.cpp`. Did **not**
apply the `-6.0` sink velocity, the `0.08` travel step or the `state==15`
handling: the deliverable leaves them unestablished, the `0.08` constant has no
SDA source, there is no original stage-15 slot, and the prologue guard reads as
`[Mario+124]==0.0` on the DOL (the contract states `!= 0.0`). The prototype
still omits the `[Mario+552]`-mismatch condition and the `r4` default lanes.
Breadcrumb: `requests/FIELD-RECOVERY-01.md`.

Validation: `make test` 87 CTest / 90 Python pass, `make all` 3DSX pass,
warning-free compile of the changed TU. Docs mid-restructure (inbox split into
`requests/`); breadcrumbs went to the per-request file. Full-game incomplete.

## 2026-10-12 — Merge the overworld request inbox into docs/requests

Migrated `OVERWORLD_DOCUMENTATION_REQUESTS.md` into the standard inbox style:
seven per-request files under `requests/` (OW-MOVE-01, OW-WALL-01,
OW-SUPPORT-01, OW-PARTNER-01, OW-ROAD-01, OW-INTERACT-01, OW-CAMERA-01), each
with a status line, an Open-todos checklist, verbatim request text and an
append-only answer/delivery history. The seven rows join the status table in
`DOCUMENTATION_REQUESTS.md`, which also absorbs the story-integrity and
static/runtime rules that headed the separate file; the "tracked separately"
pointer is gone. Cross-links in `behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md`
and `behavior/battle/BATTLE_HIT_VEC_TRAVERSAL_ORDER.md` now target the
per-request files.

Validation: `python3 tools/check_doc_links.py` (686 markdown files, 0 broken)
plus a line-by-line comparison of the old file against the new files — every
original line survives; only heading levels, the three header provenance lines
and the relative link prefixes differ. Docs-only, no make rerun. Limitation:
overworld deliveries stay recorded in each request's history and are not
back-filled as `requests/DELIVERY_LOG.md` rows, because the original inbox
never named a numeric documentation-agent ID for them. Next: answer the OW-*
open todos and register new deliveries/locks in the delivery log.
Full-game incomplete.

## 2026-10-12 — EVT-01 accessors, start wrappers, Monosiri, and an address correction (ID 3)

Second EVT-01 pass as documentation agent ID 3, continuing the inbox queue.

**A correction that outranks the new work.** I had published three data
bases computed **without the sign extension `lis` performs**, so all three
were wrong by `0x3B0000`:

| Published earlier | Correct | Symbol now confirmed |
| --- | --- | --- |
| `0x8002F988` | **`0x803DF988`** | `pssfx` (`.bss`, 1600 B = **40 slots** x 40) |
| `0x80016B28` | **`0x80316B28`** | `pssfxlist` (`.data`, 84840 B) |
| `0x8004CF40` | **`0x8036CF40`** | `monosiriMessageTable` (`.data`, 5544 B = **exactly 231 records** x 24) |

The symbol names and sizes are the decisive part: `1600/40` and
`5544/24` both divide exactly, which independently confirms both strides
and turns the slot count (40) and table extent (231) into facts rather than
guesses.

The same error had reached seven field-collision contracts from the previous
session, where `lfs` float constants were given as `0x800494xx`. Those were
internally inconsistent with each other, which is what exposed them. All are
now in the authoritative `N(r2)` form, read from the instruction bytes
(a contiguous pool from `-27544` to `-27380`), and the `lis`-derived globals
correct to `0x80343B78` / `0x80343D88`, both resolving into `.data`. A
correction note heads each affected document.

I also **reverted one of my own "corrections"**: the three values
`0x80091434` / `0x80091504` / `0x800915e4` are not debug strings but the
symbol entry points of `chkfilterVecVivian`, `chkfilterVecRoll` and
`chkfilterVec`, passed to `hitCheckVecFilter` as filter labels. Two were
already right in the original text; I had briefly "fixed" them wrongly.

**New complete bodies**, all in `docs/behavior/audio/`:

- [Slot accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) —
  `psndSFX_get_vol`, `psndSFXChk`, `psndSFX_dist`, `psndSFX_pit`,
  `psndSFX_vol`. Completes the accessor family. Key finding: the handle
  packs **slot index in bits 0..7 and channel allocation in bits 8..23**,
  validated against `slot+0x04` with a `-1` return. Also `stb`/`sth`
  truncation with no clamp, and the local `slot+0x14` vs `SoundEfxSetPitch`
  pitch split gated on the same low-5 bits `psndSFXOff` uses.
- [Start wrappers](behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) —
  `psndSFXOn`, `psndSFXOnEx_3D`, `psndSFXOn_3D`, plus the **derived
  eight-argument `__psndSFXOn` ABI**. `arg5` and `arg7` are always `0`;
  `psndSFXOn_3D` silently discards the caller's `r5`.
- [Monosiri lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md) —
  `battleGetUnitMonosiriPtr` 8012ef20, the item EVT-01 called
  unclaimed. Pure signed-multiply address computation with no bounds,
  no negative-kind check and no null return. Noted that the caller's
  `== null` test is therefore reading a **record field**, not a failure.

Also noteworthy: **40 slots exist but the handle masks with `& 0xFF`**, so
indices 0..255 are all reachable and nothing bounds them.

Updated [behavior/README](behavior/README.md) (its rows were overwritten
twice by concurrent commits and were re-added each time), appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md), marked the Monosiri todo
answered with the residual work, and registered three validated delivery
rows.

Validation: bodies re-disassembled per function with ELFs in `/tmp` only;
every callee resolved against `us_symbols.csv`; `lis`/`addi`/`addis`
arithmetic re-derived in Python and cross-checked against named symbols;
repo-wide sweep for the four wrong address patterns returns clean;
`git diff --check` clean; link audits clean. Docs-only, so `make test` /
`make all` were not rerun. Full-game incomplete.

**Process lesson worth keeping:** the fabricated addresses survived a
commit and a README row because they were plausible-looking. The two
cross-checks that caught them were (a) does the address land in the
section the symbol table says it should, and (b) do claimed strides divide
the claimed symbol sizes evenly. Both are cheap and should be applied to
any future address claim before it is written down.

Next tasks:

1. **`__psndSFXOn` 800d9934 (1344 bytes)** — now the only writer of
   `pssfx` fields `+0x00`/`+0x04`/`+0x06`/`+0x18` and the place the
   handle is packed. With `pssfx`/`pssfxlist` named and the layout fixed,
   this is the last structural gap in the audio chain.
2. **`psndSFXMain` (2964 bytes)** — reader of volume, distance and the
   published position; closes the update/stop loop.
3. Monosiri record layout beyond `+0x04`, and its table writers
   (`battleAcMain_MonosiriTarget`, the `monosiriExit` call at `8017225c`).
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` is a contended file: three concurrent commits
   have dropped rows from it. Worth an owner check before it is treated as
   a reliable index.

## 2026-10-12 — `__psndSFXOn` slot provisioning decoded (ID 3)

Third EVT-01 pass as documentation agent ID 3, taking the top item from
the previous queue: `__psndSFXOn` (`0x800d9934`, 1344 bytes), the last
structural gap in the positional-SFX chain because it is the **only**
writer of the `pssfx` fields the accessors read.

Delivered [POSITIONAL_SFX_SLOT_PROVISIONING](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
decoded end-to-end. Evidence class B; the `make test` / `make all` gates
were not rerun because this is docs-only.

Recovered:

- **The `key` argument is a string.** Name resolution is a `strcmp` scan
  over `pssfxlist` (stride 20, up to **4242** entries) against
  `word0`. Key `0` skips the scan; a scan that runs off the end resolves to
  index 0. There is no not-found failure.
- **Three pre-allocation gates**: global `+0x56` bit 29 as a mute that
  applies only when `arg7 != 0`; a signed-byte **cooldown of 6** at global
  `+0x5c` that rejects while nonzero; and for index 869 an event flag at
  `0xF5DE0180` that must equal 2. Indices 517 and 869 are the only
  per-index specials.
- **Two-stage slot search**: 20 same-key slots first (with a `+8`
  preference cursor at global `+0x54`), then a 40-slot reclaim pass that
  **inlines a full copy of `psndSFXOff`** — same low-5 gate, same
  `key >> 19` table index, same two-bit SoundSS/Song/Efx stop — and
  additionally requires `slot+0x0c == pssfxlist[+0x10]`. Exhaustion
  returns `-1`.
- **Channel bits are a pure function of the pssfxlist index**: base
  `0x101`, plus bits 5/13/14/15 of the index and per-index/per-field
  overrides. This is the routing mechanism.
- **Handle packing confirmed**: `channel = ++global[0x50]`,
  `slot[+0x04] = (u16)channel`, return `slot_index | (channel << 8)`.
  Independently confirms what `psndSFXChk` validates. Recorded asymmetry:
  `slot+0x04` gets the full 16-bit counter but only `<<8` reaches the
  handle, so the two diverge when the counter wraps.
- **Two unreachable volume clamps** at `0x800d9c60`/`0x800d9c70` — the
  `clrlwi ...,24` then `cmplwi ...,255` can never be greater. Recorded as
  observed rather than "fixed" into live clamps.

**One conflict deliberately left open.** `__psndSFXOn` writes `slot+0x00`
as the bare pssfxlist index (0..4241), but `psndSFXOff` and `psndSFX_pit`
derive their pssfxlist row from `(slot+0x00) >> 19`, which is always 0 for
that range. Read literally the accessors always resolve `pssfxlist[0]`.
The bytes are unambiguous (`stw 18, 0(29)`) and no second writer is
visible, so **no reconciliation was invented**. Flagged in the contract and
in the EVT-01 reply as something to settle before either side is relied
on.

**The practical dependency this created:** nothing in `__psndSFXOn`
decrements the cooldown. If `psndSFXMain` is ported without its decrement,
`global[0x5c]` stays nonzero and every later start fails permanently. That
makes `psndSFXMain` (2964 bytes) the single remaining item in the chain.

Registered one validated delivery row and appended the reply to
[requests/EVT-01.md](requests/EVT-01.md).

Next tasks:

1. **`psndSFXMain`** (2964 bytes) — cooldown decrement, preference-cursor
   expiry, and the reader of volume, distance and the published position.
   This is the last item in the positional-SFX chain.
2. Resolve the `slot+0x00` index-versus-packed-key conflict above.
3. Monosiri record layout beyond `+0x04` and its table writers.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` remains a contended file; verify rows survive.

## 2026-10-12 — OW-MOVE-01 input remainder: key-data update

Re-extracted marioUpdateKeyData (0x80095cb0, ISO read-only) and contracted
it in behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md: six GP pad
bytes with alternating negation into +594..599, [600/601] zeroing, flag
halfwords, fixed-order per-flag pair zeroing, key-check gate and the
0x803DA398 rising-edge word. No deadzone/clamp exists in-body (native .15
is authored). Reference pair placed under ignored artifacts/.

Validation: instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains, deadzone
ownership elsewhere. Full-game incomplete.

## 2026-10-12 — Full-tree gap re-audit plus lightSort alias

Re-ran the name-mention audit over all 664 reference binaries against the
current docs tree: exactly one unnamed file (`lightSort`), verified as alias
of contracted `lightdrv _sort` @8001bccc (three-way float compare plus
`sorter800064fc` caller in behavior/display/MAP_CHARACTER_LIGHT_QUERIES.md)
and recorded in the register. Re-added the plane-motion README row dropped
by the recent restructure. Zero unnamed-and-uncontracted binaries remain.

Validation: audit script, listing-vs-doc structural match, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains,
pipe lifecycle. Full-game incomplete.

## 2026-10-12 — Inbox request: EVT-01 positional SFX chain (ID 3)

Worked the [request inbox](DOCUMENTATION_REQUESTS.md) as documentation
agent **ID 3** (the ID the user assigned; ID 1 and 2 were already taken).
Picked the priority-0 EVT-01 audio chain, which had **no dedicated contract
at all** and was explicitly listed as open by three existing documents.

First surveyed the inbox: `docs/requests/` per-request files plus the
[delivery log](requests/DELIVERY_LOG.md) lock table. Only one lock was
active (ID 2, audience target accumulator), so EVT-01's SFX subtask was
free. Confirmed the manager loop and constructor policies making up the rest
of EVT-01 todo 1 are already complete in `EVENT_MANAGER_COMPLETE_LOOP.md`,
`EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md`,
`EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md` and
`EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md`; the genuinely unclaimed work was
the audio chain.

Delivered three **complete bodies** in `docs/behavior/audio/`:

- [Unit positional SFX trigger](behavior/audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md)
  — `BtlUnit_snd_se_pos` 80126088/114
- [Positional SFX position update](behavior/audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)
  — `psndSFX_pos` 800d96f8/4c
- [Positional SFX stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md)
  — `psndSFXOff` 800d95f8/c8

New facts, none previously recorded anywhere in the repo:

- The shared **positional-SFX slot record**: base `0x803df988`, **stride
  40**, selected by the **low 8 bits** of the handle. Fields `+0x00` key,
  `+0x06` channel bits, `+0x18` handle, `+0x1c/+0x20/+0x24` position.
- The **channel table** at `0x80316b28`, **stride 20**, indexed by
  `(slot.key >> 19)` — by the key, not the handle. Two bits of `word0`
  select `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`.
- **Stop-without-stop**: `psndSFXOff` clears the record without calling any
  stop service when `(halfword6 & 0x1F) == 0` or `handle == -1`; and an
  already-free slot returns *without writing*, so a double stop is a true
  no-op rather than a re-clear.
- `psndSFX_pos` writes **exactly three words** and never the key or handle,
  so it cannot start or stop a sound.
- `BtlUnit_snd_se_pos` status 10 **overrides** status 11 when both are set
  (sequential `if`s overwriting one register), and its start id is
  `(int16)(irand(41) + belong + offset - 20)`.
- The volume predicate is `r0 < 0x4D80` unsigned, where
  `r0 = (T & 0xFFFF0000) + 0xE7700000`. For the sentinel the movement-sound
  callers actually pass, `250000000`, this is the **preserve-current-volume**
  path. Derived from the compared words, not an observed trace.
- The wrapper's return is the **handle**, confirming the existing R18 store
  in `BATTLE_MOVEMENT_SOUND_CONTROL`.

Updated [BATTLE_MOVEMENT_SOUND_CONTROL](behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)
and [OVERWORLD_MARIO_IDLE_SOUND_CUES](behavior/overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
to link the new bodies instead of listing them open; no contradiction found
with the existing low-8-handle-bits claim. Registered and then **released**
the lock in the [delivery log](requests/DELIVERY_LOG.md), marked the EVT-01
todo answered with an explicit list of what stays open, and appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md).

Validation: all three bodies disassembled from the read-only ISO with ELFs
written to `/tmp` only; every callee resolved against `us_symbols.csv`;
disassembly re-read against each written instruction block; `git diff
--check` clean; README, cross-link and request-file link audits all clean.
Docs-only, so `make test` / `make all` were not rerun. Full-game incomplete.

Note: while this session ran, commits ec92b10/990b12e/488983a landed from
other work and prepended entries above this one. No conflict; this session's
field-collision entry is intact at its original position.

Next tasks, in the order the inbox implies:

1. **`__psndSFXOn` provisioning** (`0x800d6934`, 1344 bytes) — the slot
   allocation and macro/voice/resource chain that fills the record layout
   recovered above. Now the biggest blocker for the audio chain, and the
   known layout makes it tractable.
2. **`battleGetUnitMonosiriPtr` 8012ef20** — still explicitly unclaimed,
   with no dedicated contract. Small and bounded.
3. **`psndSFXMain`** (2964 bytes) — the reader of the published slot
   position, which closes the update/stop loop from this session.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, `NormalJump`
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, and the `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. The constant block `0x80049468`..`0x8004950c` from the field-collision
   session is still unextracted.

## 2026-10-12 — Re-extracted placeholders: plane flyMain contract, two aliases

Re-extracted three zero-byte placeholders read-only via
tools/inspect_dol_function.py into behavior/: mot_plane_flyMain (mot_plane.o
flyMain @8009c9b8, contracted in behavior/overworld/OVERWORLD_PLANE_FLIGHT_MOTION.md
with ISO-read SDA set; fixed my own hex/decimal stage confusion pre-commit),
btlevtcmd_AnnounceMessage (matches contracted announcement doc — alias),
PSVECDotProduct (9-instruction leaf contracted in the traversal doc).
Remaining placeholders (_announceMessage/BattleGetFirstAttackTarget/
partyGetOtherPtr/partyGetTargetNum cases) have no DOL text symbols and cannot
be extracted by name.

Validation: extraction outputs verified (ELF identity, entry/size match),
stage immediates re-checked after the hex fix, link audit, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains.
Full-game incomplete.

## 2026-10-12 — Fifth gap pass: guard wrapper, act-status publisher, small-data setter and float-to-unsigned

- [Small unit/runtime leaves](behavior/unit/SMALL_UNIT_RUNTIME_AND_MOTION_LEAVES.md):
  `BtlUnit_CanGuardStatus` (801272dc) is a frame-only forwarder to
  `BtlUnit_CanActStatus` (8012724c), so a `bl 801272dc` site is a `bl 8012724c`
  site for call-graph purposes. `btlevtcmd_CheckActStatus` (80105cb4) resolves a
  unit id, calls that same `BtlUnit_CanActStatus`, and publishes a fixed literal
  1 or 0 through `evtSetValue` using the **raw** second operand word as the
  destination. `unk_80151d20` (party_motion.o) is a three-instruction setter
  that writes the constant `60.0` (0x42700000 at 0x8042318c) to the small-data
  slot at `0x8041ec10` and returns. `__cvt_fp2unsigned` (80264860, runtime.c)
  maps `f1 < 0` to 0, `0 <= f1 <= 2^32` to `0xFFFFFFFF`, `f1 > 2^32` to
  `trunc(f1)` and NaN to `0xFFFFFFFF`; the 2^31 constant at `0x80303a30` is
  loaded but dead on every reachable path because the `f1 > 2^31` branch is
  always true once execution passes the 2^32 test.

Register state after this pass: 360 reference listings have no snapshot
definition; 31 of them still have no substantive prose contract. The largest
remaining unaddressed bodies are `seq_battleMain` (253 lines), `partyReInit`
(461), `_getSickStatusParam` (286), `JUTFontSetup` (196),
`FontDrawStringVecPitch` (211), `pouchRemoveItemIndex` (183),
`npcSetupBattleInfo` (166), `vivian_move`/`cloud_move`, the pad readers
(`DEMOPadRead`/`DEMOPadCopy`), `btlevtcmd_ConsumeItemReserve` and
`btlevtcmd_AnimeChangePoseType`. `marioLandOn` and `marioSearchHead` are now
carried by multiple contracts and are no longer counted as open.

Validation: docs-only; `make test` and `make all` were run for the previous pass
and are unchanged by this one (logs `/tmp/pm-gap3-test.log`,
`/tmp/pm-gap3-build.log`). `python3 tools/check_doc_links.py` reports 0 broken
links. No native/test/asset conversion change, hence no runtime, cadence,
emulator or original-execution claim. EVT-01/JUMP-01/DMG-02 and the full-game
requirements remain incomplete.

## 2026-10-12 — Native JUMP-01 correction: packed Mario jump SFX

Verified the JUMP-01 SFX-provenance answer against the read-only ISO and found
it analysed the generic `btlevtcmd_JumpPosition` (0x8010d208/0x4c4), not the
body the native port binds, `btlevtcmd_MarioJumpPosition` (0x801093a8/0x8c0).
Reproduced the Mario body from the DOL (`tools/inspect_dol_function.py`):
r30=0x802ee458, so the only 0x802E reads are the dead 24/28/32(r30)=
0x802EE470/474/478 loads (overwritten by unit floats before the call); one
`psndSFXOn_3D` (0x801098fc) keyed on `word[unit+0x18c]`, position
unit+0x154/158/15c; heading/distance from `system_angleABf` 0x8000703c /
`distABf` 0x8000722c and sine from MSL `sin` 0x80272410. Applied the one
native fix that survives: removed the three dead `0x802ee470` reads in
`source/battle_packed_mario_jump.cpp`; kept the single `play` call (the "model
both records" follow-up came from the generic body and is withdrawn). Filed the
correction breadcrumb in `docs/requests/JUMP-01_HISTORY.md`.

Validation: `make test` 87 CTest / 90 Python pass; `make all` 3DS build pass;
`diff --check` clean. Docs are mid-restructure (inbox split into
`docs/requests/`), so the breadcrumb went to the per-request history file, not
the index. Full game still incomplete.

## 2026-10-12 — Medium-size field collision functions (mario_hit.o)

Continued the medium-size sweep requested for a US port. First verified
that the 359-function fingerprint set in
`artifacts/reference/behavior/evidence.json` is now fully referenced by
existing docs (its `documented` flag is stale and was not used), then
widened the search to the full `us_symbols.csv`: 1098 game-text functions
in the 500-1500 byte band, 810 of them not referenced anywhere.

Worked the Mario field-collision family, picked because it is directly
needed for movement fidelity. Five new contracts in `docs/behavior/field/`:

- `MARIO_SLIT_CHK_WALL_AROUND.md` - complete structural read of the 8
  direction slit probe: two sweeps, three-way per-direction
  classification, and the two-group triple-run rule that returns 0 on
  three adjacent blocked directions.
- `MARIO_CHECK_WALL_ROLL.md` - the roll wall check, including the 14-tick
  wall-hold re-arm record at `mario+0x1cc..0x1d4`.
- `MARIO_SEARCH_HEAD_ROLL.md` - the 5-iteration indexed direction dispatch,
  with the `frsp` roundings between trig and scale called out.
- `MARIO_SEARCH_FRONT_WALL_STAYING.md` and `MARIO_CHECK_WALL.md` - both
  documented as **complete bodies** (every branch and call site).
  The staying search contributes the `mario[+0x2e] == 19` veto, the
  field-name gate and the triple `vivianGetStatus` gate; `marioCheckWall`
  contributes the 5-char hit-name hysteresis (thresholds 10/20) on a
  **signed** byte at `mario+0x3f` and the in-place `movePos` slide.
- `SEARCH_FRONT_WALL.md` and `MARIO_CHK_WALL_AROUND.md` - the two largest
  callees (2404 and 2460 bytes), documented at **structure level only**:
  entry gates, probe grouping, SDK vector math. Their probe sequences and
  result reduction are explicitly not decoded.

Cross-references in the earlier three docs were corrected where they listed
these two callees as undocumented, including one `marioChgWallAround` /
`marioChkWallAround` transcription error.

Newly recorded facts: the field literal at `0x800c3d88` gates both
`searchFrontWall` and stage 3 of `marioSearchFrontWall_staying`, so those
two paths are tied to one specific field; the 5-char hit-name literal
compared in `marioCheckWall` lives at `0x80049504`.

Validation: all callees resolved against `us_symbols.csv`; disassembly
regenerated per function into `/tmp` only; every `git diff --check` clean;
README link audit and field cross-link audit both clean. Docs-only, so
`make test` / `make all` were not rerun. Full-game work remains
incomplete.

Limitations: no Dolphin observations were taken this session, so every
claim is class B. The two structure-level docs must not be read as
equivalent to the original implementations.

Next tasks:

1. Decode the 9-probe reduction in `searchFrontWall` and the 12-probe
   reduction in `marioChkWallAround` - these block full movement parity.
2. Read the two unresolved literals from the ISO: `0x800c3d88` (the
   gated field name) and `0x80049504` (the hysteresis hit-name prefix).
3. Extract the `lfs` constants at `0x80049468`..`0x8004950c` - they are
   referenced by nearly every function documented in this session and are
   currently the largest single source of provisional numbers.
4. Continue the 810-function medium backlog; `party/*` and
   `npcCheckHitMarioSide` / `mobjHitEntry` are the next highest-value
   targets after collision.
5. `docs/PROGRESS.md` tables should be reconciled against actual file
   contents if any further drift is found.

## 2026-10-12 — Native request 3 slice: pose call-site survey

Delivered OVERWORLD_POSE_CALL_SITE_SURVEY: 18 marioChgPose sites with
ISO-read literals (KPA_D_1/D_2 table pair, M_D_2/M_U_3 recovery pair,
M_J_1B/M_J_1C, M_W/M_O/M_R alternation) plus gate/ordering notes; all
surveyed sites are marioChgPose (no EvtPose). Remaining request-3 items
(named domains, pipe-paper asset work) explicitly scoped out with a
non-finding (no reference body) for the pipe lifecycle.

Validation: all 18 call sites re-checked, literal strings ISO-read, link
audit, diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — Native request 2 remainder: bottomless numerics

Delivered OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS: first-tick gate,
win_03/win_01/tik_01 lane selection (rodata-read), angle setup, stage
machine via ISO-read table (0/2/10/11/12 live, 13 terminal-by-gate),
fallSpeed-shape integrator, min −10.0 snap, land/HP chain, and the
0.34/−0.002/−0.01 drop-back triple (port's jerk cascade matches one lane).
Dead −2000.0 load and SDA set pinned from ISO. Request 2 fully answered
except float-chain interiors.

Validation: call/SDA/stage-write re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: pose-table slice (request 3). Incomplete.

## 2026-10-12 — Native request 2: water recovery numerics

Delivered OVERWORLD_WATER_RECOVERY_NUMERICS for field/AC-03: full
stage/counter machine ([68]=stage via ISO-read table 0x8036ED4C, [72]=
counter), 12-slot ISO-read SDA set, sink −6.0 init, 40-count stage-1 hold,
two-tick stage-10 arm, HP-penalty tick at stage-31 expiry, 12-count stage-32
window, and named gaps where the port authored ratios/jerk (no 0.08-class
constant, no jerk lane in-body). Stale "stage15" has no slot (no-op).

Validation: dispatch/stage-write/SFX-ID/HP-call re-checks with int/float
register-file care; link audit; diff --check clean. Docs-only, no make
rerun. Next: bottomless motion33/34, pose-table slice. Full-game incomplete.

## 2026-10-12 — Native JUMP-01 request 1 answered (SFX provenance)

Fulfilled the packed-Jump provenance request from the native inbox with
BATTLE_JUMP_SFX_PROVENANCE: vec3 scratch proven zero-bytes (not a key row),
U+0x18C key domain with two-call psndSFXOn_3D record (second inherits r3),
and no-PS-math finding for 801093a8. Recorded the answer in
DOCUMENTATION_REQUESTS.md without touching the request text. Water/motion
and pose-table requests (2/3) remain open — mot bodies located for next.

Validation: dual-offset ISO reads, call-site arg traces, PS-op sweep,
link audit, diff --check clean. Docs-only, no make rerun. Full-game
incomplete.

## 2026-10-12 — Third and fourth gap passes: event leaves and the shadow driver

- [Weapon/status/wait leaves](behavior/battle/BTLEVTCMD_WEAPON_STATUS_WAIT_LEAVES.md):
  six `battle_event_cmd.o` bodies with no snapshot definition.
  `btlevtcmd_GetWeaponActionLv` publishes `lbz [resolved+0x6E]` through
  `evtSetValue` with a **raw** destination word; `btlevtcmd_AcSetDifficulty`
  drives `BattleActionCommandSetDifficulty(B, unit, value)`; and
  `btlevtcmd_AnnounceSetParam` writes into an 8-byte-stride record array at
  `BattleWork+0x18C8C` at the `+4` value word of `4*index`, with no bounds check.
  `btlevtcmd_AfterReactionEntry` resolves operands into
  `BattleTransID`/`evtGetValue` and calls the queue helper from the
  after-reaction contract. `btlevtcmd_StatusWindowOnOff` is a signed four-way
  dispatch writing `word[B+0x2748]` (modes 0/1) or `word[B+0x2744]` (mode 3) and
  then always calling `BattleStatusWindowCheck`; those are exactly the two words
  whose nonzero state forces that checkpoint's result to 0, which is recorded as
  a static inference. `btlevtcmd_StartWaitEvent` calls `BattleRunWaitEvent` only
  when `word[unit+0x28C]` is nonzero, pairing with the existing stop leaf.
- [Shadow driver leaves](behavior/display/SHADOW_DRIVER_ACCESSOR_LEAVES.md):
  the whole fifteen-body `shadowGet*` family in `shadowdrv.o`, previously absent
  from every prose file. Three SDA words at `0x804150c8/cc/d0` select the
  depth/projected/character shadow records; two static texture objects live at
  `0x8041e738`/`0x8041e758`; the three `ProjTexMtx` getters share relative
  `+124`; the two `Config` getters index a 72-byte-stride table at `0x80304a60`
  from `word[record+8]` with no bounds check; the three `TexObj` getters gate on
  bit 0 of `word[record+0]` and tail-call `smartTexObj(record+220, word[record+
  252])`, returning NULL when the gate is clear. `shadowGetDepthShadowColor` is
  the only value-producing body: it packs `(255-c)*alpha/255` for the three
  component bytes at `record+256..259` using the `0x80808081` signed-division
  idiom into one word with a zero high byte.

`BattleAfterReactionMain` was checked against the existing
[waits and conclusion](behavior/battle/BATTLE_WAITS_AND_CONCLUSION.md)
contract instead of being rewritten: the queue scan, `BattleConsumeReserveItem`,
`BattleCheckConcluded`, `BtlUnit_GetData`, `evtEntry` and the recursive restart
on a missing unit all match that prose, so the queue contract now links to it.
`btlevtcmd_AcSetParamAll` was likewise left alone: it stores eight resolved
operands into `word[B+0x1CC8+4*i]`, skipping the `0xf1194d80` sentinel, which the
action-command callback contract already documents.

Validation: `make test` passes and `make all` passes; logs
`/tmp/pm-gap3-test.log`, `/tmp/pm-gap2-build.log` (docs-only pass, no native
change). `python3 tools/check_doc_links.py` reports one pre-existing broken link
owned by another agent (`partner/FIELD_PARTNER_FLY_MAIN_DISPATCH.md`); every link
added by this pass resolves.

Next: remaining unaddressed rows are larger bodies — `BattleCheckPikkyoro`
(112 lines), `battleMakePhaseEvtTable`/`battleSortPhaseMoveTable`,
`partyDoWork`, `_getRegistStatus` and `marioLandOn`/`marioSearchHead`.
EVT-01/JUMP-01/DMG-02 and the full-game requirements remain incomplete.

## 2026-10-12 — Source audit: documentation requests for packed Jump and recovery

Observed the implementations under `source/` and filed three native
documentation requests in docs/DOCUMENTATION_REQUESTS.md (appended at the end;
no docs-agent lock reserved). Reviewed the recent native commits that shipped
without leaving requests (b79bbe2 packed Mario Jump services, 81c143d
water/void/pipe recovery poses, 778add7 idle/low-HP pose scheduling,
e1257d8/357a656 field shadows).

- JUMP-01: the packed Mario Jump position binding (commit79bbe2) requests the
  0x802ee470/474/478 sound-key block, the original jump psndSFXOn_3D
  invocation record (timing/vector/return/landing cue), and the PS-family
  sin/cos identity plus FPSCR entry state. Local bodies were already recovered
  (BATTLE_MARIO_JUMP_MOTION/NUMERICS, BATTLE_UNIT_SOUND_MODE,
  BATTLE_DAMAGE_ORCHESTRATION for the mode1 AC-possibility fallback).
- field/AC-03: `source/water_recovery.cpp` water/void return-fall parameters
  (mot_damage motion31 / mot_bottomless motion33 stages) are authored and
  source-derived per include/water_recovery.hpp; pose timings are recovered,
  the numeric stage contract is not. Requested measured original sink/rise/arc/
  travel/return counters, HP-penalty tick, and bottomless placement/fall.
- field/AC-03: recovery pose-table domains 4..40 and pipe paper (p_dokan_x)
  lifecycle requested per the OVERWORLD_MARIO_RECOVERY_POSES open tail.
- P0c main-loop/field-dispatch: the native fixed-60Hz accumulator and
  Game::tick/move/startBattle substitutes have no original marioMain
  (8005a7e0) request; asked for the motion-ID→subsystem dispatch chain,
  battle-transition construction, input gate and marioStMain retrace/pause
  cadence (MARIO_MAIN.md sketch gaps), leaving existing pause/RNG asks open.
- P1b save/persistence: source/save.cpp uses a native PM2D format and one
  opaque originalStory int; asked for the complete original cardMain save
  record/state machine and the swSet/swGet story-switch bank, since CARD_MAIN.md
  and SW_SET.md are early sketches with explicit remaining gaps and no inbox
  entry.

Address-coverage check: of every original 0x8xxxxxxx address referenced by
source/, only 0x802ee470 (sound-key block, requested) lacked a contract; the
memory-bootstrap table, game-pointer clock block and sampling constants are
already documented.

Follow-up: the documentation agent answered the JUMP-01 request in commit
044ba5c with behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md. It corrects two
premises — 0x802EE470/474/478 is `vec3_802ee470` zero scratch (not a key row),
and 801093a8 has no Gekko PS math (trig via system_angleABf/distABf). Recorded
the resulting native action items (drop the spurious 0x802ee470 reads, bind the
same-TU angle/dist helpers, model both psndSFXOn_3D calls) as a ledger
breadcrumb; no native code changed.

Validation: docs-only change; no native/test/CMake edits and no make rerun
(latest recorded host state remains 87 CTest/90 Python and make all passing).

Next: documentation agent answers the five requests; native closures for
packed Jump sound cadence, water/void physics replacement, pose-table driven
recovery presentation, the original field main-loop/pause cadence and original
save persistence; full-game story/encounter closure stays open.

## 2026-10-12 — Getter plus status dispatch; case-table rescue

Closed BattleGetUnitPtr (NULL-domain table, −1-first, asymmetric setter,
487-site US row; snapshot definition with JP/US caveat) and BtlUnit_GetStatus
(28-slot ISO-read jump table, paired sign gates, nullable outputs).
Verification caught a badly misaligned case table in my own draft (wrong
lanes for statuses 0/2/3/16–24/27, inverted pair directions); rewrote it
from a machine dump of every block and re-verified all 20 lane blocks plus
6 paired blocks. Also resolved a phantom read-tool/listing discrepancy by
re-reading bytes from disk (disk won).

Validation: full re-verification, link audit, diff --check clean. Docs-only,
no make rerun. Next: zero-byte placeholder re-extraction; triangle/math
interiors. Full-game incomplete.

## 2026-10-12 — OW-WALL-01 traversal half: hit-vector skeleton

Delivered BATTLE_HIT_VEC_TRAVERSAL_ORDER for the overworld wall inbox:
4-region/call-census (49 bl all resolved: PSVEC family, checkTriVec_xz,
mapGetWork)/return skeleton with ISO-read SDA values, NaN-safe gate
patterns, and a verified region-1 counter/pointer asymmetry (r30+1/r31+84
vs r31+1/r30+84 elsewhere) reported without inference. Triangle interiors,
chunk ownership, and callback identity stay open per the request's own
remainder. Fixed two of my own early assertions (stride count, uniform-loop
claim) during verification.

Validation: machine-extracted census/edges/strides/counts, link audit,
diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — New denomination: flagged-callee contracts (CheckStatus, filter, search)

Survey found the register's flagged open callees now have reference pairs but
no dedicated contracts. Closed three: BATTLE_UNIT_CHECK_STATUS (null gate,
GetStatus byte, 6-compare, +759 test, exact {0,1}), BATTLE_HIT_CHECK_FILTER
(8-int/6-float marshal, hitVec call, six outputs, residue return),
MESSAGE_MSG_SEARCH (two-table binary search with gp fallback, no miss-null).
Caught two wrong relative links pre-commit (battle/→overworld, message/→battle).

Validation: full instruction re-checks (incl. corrected stfs count and
output-opcode proof), link audit, diff --check clean. Docs-only, no make
rerun. Next: BtlUnit_GetStatus, BattleGetUnitPtr dedicated contracts;
zero-byte placeholder re-extraction. Full-game incomplete.

## 2026-10-12 — Register complete: landing query 2, hitVec alias (43/43)

Closed checkLand2 as marioChkLandon2 (five-probe loop, variant dispatch,
lane quantization, angle tail; doc overworld/OVERWORLD_LANDING_QUERY_2.md)
and resolved hitVec as alias of partially-contracted hitCheckVecFilter
(SLOPES doc covers orientation/normalization; triangle phases stay open).
Final tally: 38 behavior contracts + 5 alias resolutions = all 43 register
rows addressed. Residual open work outside the register: angleABf/
system_angleABf bodies, hitVec triangle phases, and the usual lifetimes/
cadences. Fixed two of my own errors pre-commit (wrong bl census, wrong
callee-share claim).

Validation: instruction re-checks, ISO SDA reads, link audit incl. a
repaired register link; diff --check clean. Docs-only, no make rerun.
Full-game goal stays incomplete.

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
| `evt_npc_wait_battle_end` | `0x8004efbc` | 184 B | `event/EVT_NPC_WAIT_BATTLE_END.md` |
| `evt_npc_set_battle_info` | `0x8004f0a4` | 100 B | `event/EVT_NPC_SET_BATTLE_INFO.md` |
| `evt_npc_battle_start` | `0x8004efbc` | 184 B | `event/EVT_NPC_BATTLE_START.md` |

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
