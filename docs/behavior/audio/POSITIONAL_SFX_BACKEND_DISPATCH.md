# `psndSFXMain` backend dispatch: channel allocation and start routes

**Address:** `0x800d9e74`, dispatch region `0x800da064`..`0x800da330`
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow)

## Overview

This contract answers the open item from
[AUDIO_FRAME_DRIVER_AND_SFX_UPDATE](AUDIO_FRAME_DRIVER_AND_SFX_UPDATE.md):
which condition selects each backend service in `psndSFXMain`.

The dispatch is **two bits of one field**, and the three backends have
different channel-allocation strategies and different resource fields.

## The type field

```asm
800da064: lwz     7, 4(30)             ; row = pssfxlist[key & 0x1fff]
800da068: rlwinm. 0, 7, 0, 0, 0        ; bit 0
800da06c: bt      2, 0x800da160         ; bit0 set -> test bit 1
800da160: rlwinm. 0, 7, 0, 1, 1        ; bit 1
800da164: bt      2, 0x800da204         ; bit1 set -> effect
```

`r30` is the `pssfxlist` row (stride 20, base `0x80316B28`), selected by
the **low 13 bits** of `slot+0x00`.

| bit 1 | bit 0 | Backend family | Route |
|-------|-------|----------------|-------|
| 0 | 0 | **Stream** `SoundSS*` | `0x800da070` |
| 0 | 1 | **Song** `SoundSong*` | `0x800da168` |
| 1 | 1 | **Effect** `SoundEfx*` | `0x800da204` |

This is the **same two-bit encoding** that
[`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md) uses to choose
`SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`, and it is read from
`row.word1` (offset `+0x04`) in both cases. Stop and start agree on the
encoding by construction.

## Stream route (`0x800da070`..`0x800da15c`) — 3 channels

Three sequential loops, all bounded by `cmplwi 24, 3`:

```asm
800da070: li      24, 0                ; loop A: find a free channel
800da074: mr      3, 24
800da078: bl      0x800dd968           ; SoundSSCheck(r24)
800da07c: cmplwi  3, 0
800da088: addi    24, 24, 1
800da08c: cmplwi  24, 3
800da090: bt      0, 0x800da074
800da094: li      24, 0                ; loop B: stop stale channels
800da09c: bl      0x800dd968           ; SoundSSCheck
800da0a0: rlwinm. 0, 3, 0, 26, 26      ; bit 26 = "in use"
800da0ac: bl      0x800ddff8           ; SoundSSStopCh(r24)
800da0b4: cmplwi  24, 3
800da0bc: li      24, 0                ; loop C: re-probe for a free channel
800da0c4: bl      0x800dd968           ; SoundSSCheck
800da0d8: cmplwi  24, 3
800da0e0: li      24, -1               ; exhausted
800da0e4: stw     24, 24(25)           ; slot[+0x18] = handle
```

Order is **find-free → sweep-stale → find-free-again**, and the handle is
`-1` if the second probe also fails. Stream backend therefore has
**3 channels (0, 1, 2)** and performs an explicit stale-stop sweep before
allocating.

Start and follow-up:

```asm
800da0f8: lwz     4, 8(30)             ; row.word2  <-- resource id
800da0fc: bl      0x800ded7c           ; SoundSSPlayCh
800da108: bl      0x800dddb4           ; SoundSSSetPanCh   (slot+0x0a)
800da158: bl      0x800ddc1c           ; SoundSSSetVolCh
```

The stream resource id is **`row.word2` (offset `+0x08`)** — a different
field from the type.

## Song route (`0x800da168`..`0x800da200`) — 4 channels

```asm
800da168: li      24, 0
800da16c: mr      3, 24
800da170: bl      0x800e0f94           ; SoundSongCheck(r24)
800da180: addi    24, 24, 1
800da184: cmplwi  24, 4                ; <-- 4, not 3
800da18c: li      24, -1
800da190: stw     24, 24(25)           ; slot[+0x18] = handle
800da1a4: lwz     0, 4(30)             ; row.word1  <-- same word as the type
800da1a8: clrlwi  4, 0, 4              ; & 0xF        <-- resource id
800da1ac: bl      0x800e124c           ; SoundSongPlayCh
800da1fc: bl      0x800e102c           ; SoundSongSetVolCh
```

Differences from the stream route:

- **4 channels (0..3)**, not 3.
- **No stale-stop sweep** — only a free-channel probe.
- The resource id is `row.word1 & 0xF`, i.e. the **low 4 bits of the same
  word that carries the type**. Since bit 0 is 1 for songs, the id is
  odd-valued in `0..15`.

## Effect route (`0x800da204`..`0x800da330`) — no channel probe

```asm
800da204: lhz     0, 6(25)             ; slot[+0x06]  (SIGN-EXTENDED)
800da208: rlwinm  0, 0, 0, 30, 30      ; mask 0x40000000, on a sign-extended halfword
800da20c: bt      2, 0x800da2bc         ; clear -> non-positional route
800da210: lbz     4, 8(25)             ; slot[+0x08] volume
...
800da25c: lhz     6, 18(25)            ; slot[+0x12]
800da260: clrlwi  5, 0, 24
800da264: bl      0x800dc6d4           ; calc3D
800da268: stw     3, 8(1)              ; stage the PAN word
800da270: lwz     0, 4(30)             ; row.word1
800da27c: clrlwi  3, 0, 16             ; & 0xFFFF     <-- resource id
800da280: bl      0x800e0ca0           ; SoundEfxPlayEx
800da284: stw     3, 24(25)            ; slot[+0x18] = handle
```

Differences again:

- **No channel allocation loop at all** — effects allocate from the effect
  pool inside `SoundEfxPlayEx`, and the returned handle is stored directly.
- The resource id is `row.word1 & 0xFFFF`, a **16-bit** mask rather than
  4 or a whole word.
- **Bit 30 of `slot+0x06`** selects the positional route: when set,
  `calc3D` runs and its `PAN` record is staged before `SoundEfxPlayEx`.
  When clear, control goes to `0x800da2bc`, a route that does not call
  `calc3D`.

### What "bit 30" actually tests

The mask `0x40000000` is applied to a value just produced by `lhz`, which
**sign-extends** a 16-bit halfword into bits 16..31. Bit 30 lies entirely
inside that sign-extension region, so it is set exactly when bit 15 is —
i.e. when the halfword is **negative as `int16`**.

```cpp
// (int16)slot->flags & 0x8000   --  nothing to do with 0x40000000
if ((int16_t)slot->flags < 0) { /* calc3D spatial route */ }
```

A reader taking "bit 30" literally would hunt for `0x40000000` in the
provisioner, find nothing, and wrongly conclude the route is dead. This
note exists to prevent that.

The field is **not** cleared before this test. The preceding pair
`rlwinm 0, 3, 0, 24, 22` / `sth 0, 6(25)` at `0x800DA044` has mask
`0xFF7FFFFF` — MB > ME, a wrapped range that **includes** bits 0..15 — so it
normalises a sign-extended value to a zero-extended one and preserves the
low 16 bits. The genuine clear is `rlwinm 0, 0, 0, 16, 30` at `0x800DA390`
(mask `0x7FFF0000`, MB <= ME, so bits 0..15 fall outside the mask), and it
sits in the **live** pass at `0x800DA38C`, *after* this startup test.

### Both `calc3D` call sites are unreachable

Both sites in `psndSFXMain` are gated by the same bit-15 test, and in both
cases the gate is provably false.

| Site | Gate | Why it is never taken |
|------|------|------------------------|
| `0x800DA264` | `lhz`+`rlwinm 0,0,0,30,30` at `0x800DA208` | needs bit 15 of `slot+0x06`; nothing in the binary sets it |
| `0x800DA3F8` | same test at `0x800DA39C` | the field is **cleared four instructions earlier** |

The second case is straight-line and needs no cross-file argument:

```asm
800da38c: lhz     0, 6(25)          ; slot+0x06
800da390: rlwinm  0, 0, 0, 16, 30  ; mask 0x7FFF0000 -> low 16 zeroed
800da394: sth     0, 6(25)          ; slot+0x06 = 0   <-- CLEARS it
800da398: lhz     0, 6(25)          ; re-read: 0
800da39c: rlwinm. 0, 0, 0, 30, 30  ; bit 30 of 0 -> 0
800da3a0: bt      2, 0x800DA528     ; ALWAYS taken
```

`bt 2` branches when the tested bit is clear, so control always leaves to
`0x800DA528` and the block containing `calc3D` at `0x800DA3F8` is skipped
unconditionally. Contrast the startup path, where the preceding store at
`0x800DA044` uses `rlwinm 0, 3, 0, 24, 22` (mask `0xFF7FFFFF`, MB > ME,
bits 0..15 inside the mask) and therefore *preserves* the field — which is
why that site needs the cross-file argument instead.

### What this does and does not imply

`__psndSFXOn` already computes a distance at provisioning time and stores
it into `slot+0x0E` and `slot+0x10`, and `psndSFXMain` counts `slot+0x0E`
down each frame. So attenuation is **not** dependent on `calc3D`; the
provisioner's own distance formula covers it, and `calc3D` is the richer
per-frame spatial model that this build never reaches.

That said, this is **class B static analysis with no execution trace**. A
decompilation-based reading can be defeated by a path I have not traced —
for instance if `0x800DA528` or the reclaim loop can re-enter the startup
block with a field value set elsewhere. The startup site in particular
depends on the claim that nothing sets bit 15, which rests on a 22-site
census rather than on control flow alone.

**Recommendation for a port:** do not delete either path on this evidence.
Implement both, or gate them the same way, until an execution trace
confirms that `calc3D` is never entered in ordinary play.

Both effect follow-ups follow the `-1` handle guard at
`0x800da28c` (`addis 0,3,1; cmplwi 0,65535`), so a failed `SoundEfxPlayEx`
skips `SoundEfxSetVolume`.

## Backend comparison

| | Stream | Song | Effect |
|---|---|---|---|
| Type bits (`row.word1`) | bit0=0 | bit0=1, bit1=0 | bit0=1, bit1=1 |
| Channels | **3** (0..2) | **4** (0..3) | none probed |
| Stale-stop sweep | **yes** | no | no |
| Resource field | `row.word2` | `row.word1 & 0xF` | `row.word1 & 0xFFFF` |
| Exhaustion | handle `-1` | handle `-1` | handle from backend |
| Spatial route | no | no | **sign** of `slot+0x06` as int16 |
| Services | `PlayCh`, `SetPanCh`, `SetVolCh`, `SetSrndPanCh`, `GetVolCh`, `StopCh`, `Check` | `PlayCh`, `SetVolCh`, `GetVolCh`, `Check`, `StopCh` | `PlayEx`, `SetVolume`, `GetVolume`, `SetPitch`, `SetPan`, `SetSrndPan`, `SetAux1`, `Check`, `Stop` |

## Porting notes

1. The type field is **`row.word1` bits 0..1** and the same field supplies
   the song and effect resource ids in its low bits. Mask exactly as
   written: `& 0xF` for songs, `& 0xFFFF` for effects, `row.word2` for
   streams.
2. Channel counts differ per backend: **3 streams, 4 songs, effects
   unpaged here**. A port that gives effects the song's loop would
   misbehave.
3. Only the stream route sweeps stale channels before allocating. The
   order **find → sweep → find** matters; a port that only finds will
   return `-1` while channels are technically occupied.
4. The three routes converge at `0x800da36c`, which tests
   `slot+0x18`: an exact `-1` clears `slot+0x00` and `slot+0x18` and skips
   the slot, otherwise it clears the pending flag and enters live
   processing.
5. Bit 30 of `slot+0x06` is what routes an effect through `calc3D`.
   `__psndSFXOn` sets bits 1, 2, 4, 16, 32, 128, 256, 512, 640, 1024 and
   2048 (maximum `0xBB7`) but **not** bit 15, so what makes the field
   negative is open (see gaps).

## Remaining gaps

- The live `calc3D` site at `0x800DA3F8` and its `PAN` consumer chain.

## The startup spatial route is dead code — resolved

This replaces the earlier "not determined". It **is** determined, and the
answer is that nothing in the binary can make the guard true.

**Step 1 — the guard needs bit 15.** `lhz` sign-extends, and the mask
`0x40000000` lies wholly in the sign-extension, so the condition is
`(int16)slot+0x06 < 0`, i.e. bit 15 (`0x8000`).

**Step 2 — the provisioner cannot set it.** `__psndSFXOn` builds the field
from a `0x101` base plus conditional ORs of 2, 4, 16, 32, 128, 512, 640,
1024 and 2048. The maximum reachable value is

```
0x101 | 16 | 32 | 640 | 512 | 1024 | 2048 | 128 | 2 | 4  ==  0xFB7
```

`0xFB7 < 0x8000`, so bit 15 is never set there.

**Step 3 — no other writer exists in the whole binary.** Every access to the
`pssfx` array (`0x803DF988`) is inside `pmario_sound.o`. A binary-wide scan
for the `addi rD, rA, -1656` that forms the address returns **22 sites in 13
functions, all in that one file**:

`psndENV_stop` (4), `psndSFX_get_vol`, `psndSFXChk`, `psndSFXOff`,
`psndSFX_dist`, `psndSFX_pos`, `psndSFX_pit`, `psndSFX_vol`, `__psndSFXOn`
(2), `psndSFXMain` (4), `L_psndBGM_stop` (2), `psndSFXAllOff`,
`psndMapChange`, `psndStopAllFadeOut`.

A second scan for the alternate route — an `addi` from the audio global with
`imm = 0x806 + 40*i`, which would reach `slot+0x06` directly — returns only
4 candidates, all in unrelated files (`effStageClearMain`, three
`stg3_20_*` event initialisers) and all using immediates against a
different base register. No external writer exists.

**Step 4 — the remaining in-file writers do not set bit 15.** Inside
`pmario_sound.o` the only stores to `slot+0x06` are the ten `__psndSFXOn`
ORs above, the normalisation/clear pair in `psndSFXMain` (which preserve or
zero the low 16 and cannot invent bit 15), and `sth 5, 6(6)` in `psndInit`
during global initialisation.

**Conclusion:** bit 15 is never set, so the guard at `0x800DA208` is never
true and the `calc3D` call at `0x800DA264` is unreachable. The second site,
at `0x800DA3F8`, is unreachable for the simpler local reason given above.

**This supersedes an earlier statement in this file that `0x800DA3F8` was
"the live site" carrying all positional attenuation.** That was wrong: the
clear at `0x800DA394` precedes its own test at `0x800DA39C` in the same
straight-line path. The caveat about it being class B without an execution
trace applies to both sites, and the porting recommendation is to keep both
paths rather than delete either.
- The later half of the function (from `0x800DA330` to the epilogue): the
  per-frame volume/pan/pitch updates for already-playing sounds, which
  account for most of the remaining `SoundSSSetVolCh` /
  `SoundEfxSetVolume` / `SoundSongSetVolCh` call sites.
- The `row+0x0C`/`row+0x10` compatibility fields, read during reclaim.
- `row.word3` (offset `+0x0C`) is read in the update path but its role is
  not established.
- No original execution trace.