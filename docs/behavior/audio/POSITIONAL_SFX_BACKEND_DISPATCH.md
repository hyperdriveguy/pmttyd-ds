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
800da204: lhz     0, 6(25)             ; slot[+0x06]
800da208: rlwinm  0, 0, 0, 30, 30      ; bit 30
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
| Spatial route | no | no | **bit 30** of `slot+0x06` |
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
   2048 but **not** bit 30, so where bit 30 comes from is open (see gaps).

## Remaining gaps

- **Who sets bit 30 of `slot+0x06`.** `__psndSFXOn` does not, and the
  `sth 0, 6(25)` at `0x800DA048` clears the whole field for engine-backed
  slots each frame. The route therefore depends on state set outside the
  positional-SFX family, and that writer was not located.
- The later half of the function (from `0x800DA330` to the epilogue): the
  per-frame volume/pan/pitch updates for already-playing sounds, which
  account for most of the remaining `SoundSSSetVolCh` /
  `SoundEfxSetVolume` / `SoundSongSetVolCh` call sites.
- The `row+0x0C`/`row+0x10` compatibility fields, read during reclaim.
- `row.word3` (offset `+0x0C`) is read in the update path but its role is
  not established.
- No original execution trace.