# Field partner fly-main mode dispatch, splash marker and hazard guards

US G8ME01 disc0 revision 0; static read-only DOL inspection via
`tools/inspect_dol_function.py` (address `0x800c7ff8`, length `0x570`).
Evidence B: complete `flyMain` symbol body, `party_move.o`, size `0x570`
(348 instructions), per `artifacts/reference/us_symbols.csv`. The body
has no `us-usage.json` row and no prior prose mention; the symbol name is
the original symbol-table name, not an invented one. This is the outer
fly driver that the `FIELD_PARTNER_FLY_*` leaf contracts serve; it does
not call `partyMoveWalk` or `partyWalkMain`.

## Frame, fields and outer mode dispatch

Frame is `-48(1)`; the body saves `lr` at `+52(1)` and `r31`, with
`r31 = r3 = P` (the partner work record). It dispatches on the unsigned
mode byte `P+0x3b`:

| Mode byte | Arm |
| --- | --- |
| 0 | First-call arm, then the full common tail |
| 1 | Eight-entry substate jump table, then the full common tail |
| 2 | Four-column fly dispatch variant, position copy only |
| 3 | Four-column fly dispatch variant, position copy only |
| 4, 5 and 7+ | No service; return |
| 6 | Four-column fly dispatch variant, position copy only |

Modes 2, 3 and 6 copy float `P+0x100` to `P+0xfc` and return; they do
not run the splash marker, countdown or hazard guards. Modes 0 and 1
run the full common tail below.

## First-call arm (mode 0)

If word `P+8` **mask 0x20000000** is set, consume it (clear the mask),
then request `partyChgPoseId(P, r4)` at `0x800bee68` where `r4` still
holds the just-read `P+8` flags word residue — not a literal pose ID;
the pose-ID meaning of that residue remains open. `partyChgPoseId`
itself reads `r4` as its pose-ID argument and compares it against 1/2/3.
Then, if unsigned kind byte `P+0x31` equals 5, request
`unk_80187d74(P)` at `0x80187d74` (undocumented body; the sibling
contract `FIELD_PARTNER_MODE1_SUBSTATES.md` records the same call for
its own marker path). Then read unsigned substate byte `P+0x36` and
dispatch:

| Substate | Request/writes |
| --- | --- |
| 0 | Call `party_motion_stay(P)` at `0x8015164c`; then reread kind byte `P+0x31` and, if 5, request `unk_80187d10(P)` at `0x80187d10` |
| 1 or 2 | Three `P+0` flag clears (masks `0x0E000000`, `0xC0`, `0x20`), reset substate byte `P+0x36` to 0 and OR `2` into word `P+8` |
| 3 | Call `party_motion_fall(P)` at `0x8014cbb8` |
| 6 | Reset substate byte `P+0x36` to 0 and OR `2` into word `P+8`; the three `P+0` clears are skipped by the cached `>3` test |
| 4, 5, 7+ | No substate service |

The substate 1/2 clears are reachable here: substate 3 took the fall
arm and nothing rereads `P+0x36` before the cached `cmpwi 3` test, so
only substates 1 and 2 pass it. This differs from the sibling
`unk_800ca884`, whose own cached test cannot match locally.

## Mode 1 substate jump table

After consuming the `P+8` mask `0x20000000` marker (clear only, no
`partyChgPoseId` call in this mode), read unsigned substate byte
`P+0x36`; values above 7 skip the table. The eight-entry table at
`0x80315050` (read-only data in DOL text section 10) selects:

| Entry | Target body | Request |
| --- | --- | --- |
| 0 | `0x800c8228` | `party_motion_stay(P)` at `0x8015164c` |
| 1 | `0x800c8234` | `P+0x32 == 0`: `party_motion_homing_walk(P)` at `0x8014fd4c`; otherwise `party_motion_homing_fly(P)` at `0x8014f048` |
| 2 | `0x800c8258` | `party_motion_jump(P)` at `0x8014d2f0` |
| 3 | `0x800c8264` | `party_motion_fall(P)` at `0x8014cbb8` |
| 4, 5 | `0x800c8284` | No service; the table entry points directly at the common tail |
| 6 | `0x800c8270` | `party_motion_upstairs(P)` at `0x8014cb28` |
| 7 | `0x800c827c` | `party_motion_damage(P)` at `0x8014c858` |

## Modes 2, 3 and 6 fly dispatch variants

These arms consume the same `P+8` mask `0x20000000` marker (clear
only), then dispatch on unsigned substate byte `P+0x36` using the
fly-side counterparts of the walk-dispatch columns documented in
`FIELD_PARTNER_WALK_DISPATCH.md`:

| Substate | Mode 2 | Mode 3 | Mode 6 |
| --- | --- | --- | --- |
| 0 | `P+0x32 == 0`: `party_motion_behind_mario(P)` at `0x8014e8ac`; otherwise `party_motion_behind_mario_fly(P)` at `0x8014defc` | `party_motion_beside_mario(P)` at `0x8014da8c`, unconditional | `P+0x32 == 0`: `party_motion_behind_mario_continue(P)` at `0x8014e3dc`; otherwise `party_motion_behind_mario_fly(P)` at `0x8014defc` |
| 2 | `party_motion_jump2(P)` at `0x8014cf54` | same | same |
| 3 | `party_motion_fall(P)` at `0x8014cbb8` | same | same |
| 6 | `party_motion_upstairs(P)` at `0x8014cb28` | same | same |
| 7 | `party_motion_damage(P)` at `0x8014c858` | same | same |
| 1, 4, 5, 7+ | No substate service | No substate service | No substate service |

Each arm ends with the `P+0x100` to `P+0xfc` float copy and returns;
none of these modes runs the tail below.

## Common tail (modes 0 and 1 only)

1. Copy float `P+0x100` to `P+0xfc`.
2. If word `P+8` **mask 0x80** is set, consume it, copy floats
   `P+0x58`, `P+0x5c`, `P+0x60` into a stack vector and request
   **`marioSetSplash(0, &vector)`** at `0x8013b6e8`; write byte `20`
   to `P+0x4c`. The splash service's visual/audio behavior remains
   open.
3. Countdown byte `P+0x4c`: read, subtract 1, store the low byte and
   sign-extend. A negative signed result clamps the byte to 0; zero is
   retained, so a newly written 20 becomes 19 in the same invocation,
   raw 0 becomes 255 then clamps to 0, and raw 255 becomes 254 then
   clamps to 0. These literals do not establish measured splash
   duration or rendering cadence. The byte has the same expiry rule as
   the sibling contract's local countdown and is separate from the
   global action countdown in `FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md`.
4. If the reread mode byte `P+0x34` equals 13, return immediately,
   skipping the hazard guards.
5. Ordered hazard-pointer checks on `P+0x138`, then `P+0x12c`, then
   `P+0x140`. For each nonnull pointer `H`, call `hitGetAttr(H)` at
   `0x80014310` (complete body: returns word `H+4` if nonnull,
   otherwise 0). If the returned word has **mask 0x100000**, request
   `partyChgRunMode(P, 13)` at `0x800bcb2c` and skip the remaining
   pointer checks; a null pointer or a clear attribute continues to the
   next pointer. `partyChgRunMode`'s own old/new semantics are
   documented in `FIELD_PARTY_MODE_INITIALIZATION.md`. No local
   position, HP/FP or saved-kind write accompanies the request.

Then restore `lr`/`r31` and return. Unlike the sibling `unk_800ca884`,
this body makes no `vivianShadow(P)` call on any path.

## Open boundaries

- Callers are not identified from this body: no `us-usage.json` row,
  no prose mention, and the writers of mode byte `P+0x3b`, substate
  byte `P+0x36` and mobility byte `P+0x32` are not local. The outer
  update cadence and the mode-change conditions remain open.
- The meaning of `P+8` mask `0x20000000` (first-call marker), mask
  `0x80` (splash marker) and hazard attribute mask `0x100000`, the
  pose-ID residue passed to `partyChgPoseId`, and the `unk_80187d74` /
  `unk_80187d10` bodies remain open.
- The sibling `unk_800ca884` (`FIELD_PARTNER_MODE1_SUBSTATES.md`)
  shares this field layout and tail shape but documents its own
  distinct marker/attribute masks and a `vivianShadow(P)` call; the two
  bodies are separate contracts and their bit differences are recorded
  as measured, not normalized.
