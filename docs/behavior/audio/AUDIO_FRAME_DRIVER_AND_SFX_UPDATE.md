# Audio frame driver and `psndSFXMain`

**Address:** `0x800dc48c` (`psndMain`, 84 bytes) and `0x800d9e74` (`psndSFXMain`, 2964 bytes)
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow) — `psndMain` is a **complete body**; `psndSFXMain` is **structure level**

## Overview

`psndMain` is the per-frame audio driver. It owns the global enable gate
and the **cooldown decrement**, then runs four services in a fixed order.
`psndSFXMain` is the third of those: the per-frame updater for the 40
`pssfx` positional slots.

This contract corrects a claim made earlier in this session. The
`__psndSFXOn` cooldown at `global+0x5c` is decremented by **`psndMain`**,
not by `psndSFXMain`.

## `psndMain` — complete body

```asm
800dc498: lwz     0, 7144(13)          ; init_f  (r13 = 0x8041cf20 -> 0x8041eb08)
800dc49c: cmpwi   0, 0
800dc4a0: bt      2, 0x800dc4d0        ; init_f == 0 -> return, services skipped
800dc4a4: lis     3, -32706            ; 0x803e0000
800dc4a8: addi    4, 3, -2328          ; r4 = 0x803df188 (pmario_sound global)
800dc4ac: lbz     3, 92(4)             ; global[+0x5c] cooldown
800dc4b0: extsb.  0, 3
800dc4b4: bt      2, 0x800dc4c0        ; == 0 -> skip decrement
800dc4b8: addi    0, 3, -1
800dc4bc: stb     0, 92(4)             ; cooldown -= 1   (SIGNED byte)
800dc4c0: bl      0x800db7cc           ; psndBGMMain
800dc4c4: bl      0x800d7b64           ; psndENVMain
800dc4c8: bl      0x800d9e74           ; psndSFXMain
800dc4cc: bl      0x800e22f4           ; SoundMain
```

### Gate

The driver runs only when **`init_f != 0`**. `init_f` is a named symbol at
`0x8041eb08` (reached as `r13 + 7144`). When it is zero the four services
are skipped entirely and the function returns immediately.

### Cooldown decrement

`global+0x5c` is a **signed byte** (`extsb`). Each audio frame that passes
the gate decrements it by one if it is nonzero; zero is left alone, so it
saturates at 0 rather than wrapping negative.

`__psndSFXOn` sets it to **6** on every accepted start, and rejects any
start while it is nonzero. Combined, that permits **six positional-SFX
starts per audio frame**, then blocks until the counter drains.

### Service order

| Order | Address | Symbol | Size |
|-------|---------|--------|------|
| 1 | `0x800db7cc` | `psndBGMMain` | 1172 |
| 2 | `0x800d7b64` | `psndENVMain` | 5764 |
| 3 | `0x800d9e74` | `psndSFXMain` | 2964 |
| 4 | `0x800e22f4` | `SoundMain` | 168 |

The cooldown decrement happens **before** all four, so a start issued
during a service in this frame sees the already-decremented value.

## `psndSFXMain` — structure level

Stack frame: 48 bytes. Working registers:

| Register | Role |
|----------|------|
| `r25` | slot cursor, `pssfx` = `0x803df988` |
| `r24` | second-phase cursor (starts as `r25`) |
| `r26` / `r27` | `pssfx` global = `0x803df188` |
| `r30` | `pssfxlist` row pointer |
| `r31` | slot index counter |
| `r29` | count of slots seen alive |

### Phase 1 — stale-voice reaping (`0x800d9e9c`..`0x800d9f3c`)

Walks the slots and, for each occupied slot whose `slot+0x06` low 5 bits
are set, indexes `pssfxlist[slot.key & 0x1FFF]` and tests
`word1` bit 0. If that bit selects the stream type, the handle at
`slot+0x18` is passed to `SoundSSCheck`:

```asm
800d9edc: lwz     3, 24(24)           ; slot[+0x18]
800d9ee0: bl      0x800dd968           ; SoundSSCheck
800d9ee4: cmplwi  3, 0
800d9ee8: bf      2, 0x800d9f40        ; still playing -> keep
800d9eec: li      0, -1
800d9ef0: stw     0, 0(24)            ; slot.key    = -1
800d9ef4: stw     0, 24(24)           ; slot.handle = -1
```

This is the missing counterpart to
[`__psndSFXOn`](POSITIONAL_SFX_SLOT_PROVISIONING.md): sounds that end on
their own in the engine have their slots reclaimed without an explicit
`psndSFXOff`. Without this pass the 40 slots would leak.

Two `stb 0, 89(4)` writes in this phase clear the flag byte at
`global+0x59`, which the later phases also test (`lbz 0, 89(27)` at
`0x800da418`, `0x800da478`, `0x800da4d0`).

### Phase 2 — per-slot update (`0x800da010`..`0x800da9dc`)

The loop bound is a literal **`cmpwi 31, 40`**, independently confirming
the 40-slot table size that `pssfx`'s 1600-byte size also implies.

```asm
800da010: lwz     4, 0(25)            ; slot.key
800da01c: bt      2, 0x800da9d0       ; key == -1 -> next slot
800da020: clrlwi  5, 4, 19            ; key & 0x1FFF  (LOW13)
800da028: mulli   5, 5, 20            ; row = 80316b28 + 20*(key & 0x1fff)
800da030: clrlwi. 0, 3, 31            ; slot+0x06 low 5 bits
800da038: add     30, 4, 5            ; r30 = pssfxlist row
800da03c: addi    29, 29, 1           ; count live slots
800da040: bt      2, 0x800da398        ; low 5 clear -> local/software route
800da044: rlwinm  0, 3, 0, 24, 22
800da048: sth     0, 6(25)            ; slot+0x06 = 0   <-- cleared
800da04c: lhz     3, 14(25)           ; slot+0x0e (lifetime counter)
800da050: cmplwi  3, 0
800da058: addi    0, 3, -1
800da05c: sth     0, 14(25)           ; decrement
800da060: b       0x800da9d0          ; next slot
```

For an engine-backed slot the routine **clears `slot+0x06` outright and
decrements the lifetime counter at `slot+0x0e`**, skipping the slot once
the counter reaches zero. That is a countdown lifetime in frames, distinct
from the engine's own voice end that phase 1 reaps.

### Channel dispatch

The remaining body dispatches on the pssfxlist row and slot flags to
`SoundSS*`, `SoundSong*` and `SoundEfx*` services. Observed call sites:

| Service | Call count | Purpose |
|---------|-----------|---------|
| `calc3D` (`0x800dc6d4`) | 2 | positional attenuation |
| `SoundSSSetVolCh` | 6 | stream volume |
| `SoundEfxSetVolume` | 6 | effect volume |
| `SoundSongSetVolCh` | 6 | song volume |
| `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop` | 4 / 3 / 3 | stops |
| `SoundSSSetPanCh` | 3 | stream pan |
| `SoundEfxPlayEx` | 2 | effect playback |
| `SoundEfxSetPitch` | 2 | effect pitch |
| `SoundEfxSetPan` / `SoundEfxSetSrndPan` | 2 / 2 | effect pan |
| `SoundSSPlayCh` / `SoundSongPlayCh` | 1 / 1 | playback start |
| `SoundSSCheck` / `SoundSongCheck` / `SoundEfxCheck` | 4 / 2 / 1 | liveness |
| `SoundSSGetVolCh` / `SoundSongGetVolCh` / `SoundEfxGetVolume` | 1 / 1 / 1 | volume readback |
| `SoundSSSetSrndPanCh` | 1 | surround pan |
| `SoundEfxSetAux1` | 1 | auxiliary |
| `seqGetSeq` (`0x8002e210`) | 1 | current sequence |

`calc3D` is called **twice** from distinct sites (`0x800da264` and
`0x800da3f8`), so the distance/pan model is computed on two different
routes rather than once and reused. The slot-flag bit tests
`rlwinm 0, 3, 0, 24, 22` at `0x800da044` select among these routes.

### Epilogue (`0x800da9d0`..`0x800da9dc`)

```asm
800da9d0: addi    31, 31, 1
800da9d4: addi    25, 25, 40
800da9d8: cmpwi   31, 40
800da9dc: bt      0, 0x800da010
800da9e0: lis     3, -32706
800da9e4: li      0, 0
800da9e8: addi    3, 3, -2328
800da9ec: stb     29, 91(3)           ; global[+0x5b] = live slot count
800da9f0: sth     0, 84(3)            ; global[+0x54] = 0
```

Two epilogue writes:

- `global+0x5b` receives the count of live slots seen this pass.
- **`global+0x54` is zeroed every frame.** This is the preference cursor
  that `__psndSFXOn` advances by `+8` per same-key allocation. Because it
  is reset here, the cursor is a **within-frame** round-robin, not a
  persistent one across frames.

## Correction to an earlier claim in this session

An earlier entry in this session's `__psndSFXOn` contract, in the EVT-01
request, and in `docs/PROGRESS.md` stated that "`psndSFXMain` owns the
cooldown decrement" and that omitting it "would stop all further starts
permanently".

**That is wrong.** `psndSFXMain` never reads or writes `global+0x5c`. The
decrement is in `psndMain` at `0x800dc4ac`..`0x800dc4bc`, gated on
`init_f`. Those three places have been corrected to name `psndMain`.

The corrected practical dependency: a port needs **`psndMain`**, including
its `init_f` gate and service order — not `psndSFXMain` — for the
positional-SFX start cooldown to drain.

## Global block at `0x803df188` — writer summary

| Offset | Size | Written by | Read by |
|--------|------|-----------|---------|
| `+0x50` | 2 | `__psndSFXOn` (increment) | — |
| `+0x52` | 2 | — | `psndSFXMain` |
| `+0x54` | 2 | `__psndSFXOn` (+8), **`psndSFXMain` (zero)** | `__psndSFXOn` |
| `+0x56` | 2 | `psndInit` | `__psndSFXOn`, `psndSFXMain` |
| `+0x58` | 1 | `psndInit` | — |
| `+0x59` | 1 | `psndSFXMain` (clear) | `psndSFXMain` |
| `+0x5a` | 1 | `psndInit` | — |
| `+0x5b` | 1 | `psndSFXMain` (live count) | — |
| `+0x5c` | 1 | `__psndSFXOn` (= 6), **`psndMain` (-1)** | `__psndSFXOn` |

`psndInit` (`0x800dc4e0`, 368 bytes) initialises this block and is the only
writer of `+0x58`, `+0x5a` and the initial `+0x5c`.

## Porting notes

1. **The enable flag is `init_f` at `0x8041eb08`.** If the remake's
   equivalent is not set, no audio service runs at all and the cooldown
   never drains — sounds start once and then stop permanently. This is the
   single most important thing to get right in this file.
2. Cooldown is a **signed** byte decrement that saturates at 0. Preserve
   `extsb`.
3. Service order is fixed and the cooldown decrement precedes all four
   services.
4. `global+0x54` resets every frame; do not treat it as persistent state.
5. Phase 1's reaping and phase 2's countdown are **two different
   end-of-life mechanisms**. A port that implements only the countdown
   will leak slots; one that implements only reaping will hang slots whose
   engine voice outlives their frame budget.

## Remaining gaps

- The `psndSFXMain` per-channel dispatch bodies: which pssfxlist field or
  slot flag selects each of the ~20 `SoundSS*` / `SoundSong*` /
  `SoundEfx*` services, and the argument packing for each. Structure and
  call sites are recorded; the routing conditions are not.
- `calc3D` (1268 bytes) — the distance/pan model that feeds
  `slot+0x0e` and `slot+0x10`.
- `psndENVMain` (5764 bytes) and `psndBGMMain` (1172 bytes) run before
  `psndSFXMain` and may also touch `pssfx` or the global block; not audited.
- Writers of `pssfx` fields `+0x0a`, `+0x0b`, `+0x14`, `+0x16` after
  provisioning.
- `psndInit`'s initial values for the global block.
- No original execution trace of the audio frame.

## Two corrections to earlier work in this session

**Row indexing.** Earlier deliveries in this session read
`clrlwi rA, rS, 19` as masking the **top** 13 bits (`key >> 19`). That is
wrong: it retains the **low** 13 bits, so the pssfxlist row is
`80316b28 + 20 * (key & 0x1FFF)`. The allocator proves it —
`__psndSFXOn` computes `r31 = clrlwi(r18, 19)` where `r18` is a plain
0..4241 scan index, then tests `cmplw 0, 31`. Under the top-13 reading
`r31` would always be `0` and the slot match would be vacuous; under the
low-13 reading it is an exact match. A related "unresolved conflict"
about `slot+0x00` being an index versus a packed key was an artifact of
the same error, not a real conflict. The corrected delivery is
[POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md).

**Handle packing on wrap.** An earlier claim that `slot+0x04` and the
returned handle diverge once the 16-bit channel counter wraps is also
wrong. `0x800d9e50` **reloads** the counter from memory after the `sth`,
so both the stored channel and the shifted handle use the same truncated
value and always agree:

```asm
800d9e48: sth     0, 80(4)             ; global[+0x50] = counter
800d9e4c: sth     0, 4(29)             ; slot[+0x04] = counter
800d9e50: lhz     0, 80(4)             ; RELOAD (16-bit, post-wrap value)
800d9e54: slwi    0, 0, 8
```

The reload is the point of the second load; reading it as the pre-truncation
counter was my error.
