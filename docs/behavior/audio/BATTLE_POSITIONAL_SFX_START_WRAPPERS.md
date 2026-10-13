# Positional SFX start wrappers and the `__psndSFXOn` ABI

**Address:** `0x800d98bc`, `0x800d9808`, `0x800d9848`
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow — **all three bodies complete**)

## Overview

The three public start entry points all forward to the internal
`__psndSFXOn` (`0x800d9934`, 1344 bytes). Each supplies a fixed set of
defaults, so the eight-argument calling convention of `__psndSFXOn` is
fully determined by comparing them. This contract pins that ABI; the body
of `__psndSFXOn` itself is **not** decoded here.

| Function | Address | Size | Purpose |
|----------|---------|------|---------|
| `psndSFXOn` | `0x800d98bc` | 60 | non-positional start |
| `psndSFXOnEx_3D` | `0x800d9808` | 64 | positional start, full argument set |
| `psndSFXOn_3D` | `0x800d9848` | 60 | positional start, defaults |

## Shared shape

All three are 16-byte frames whose entire body is register shuffling
followed by a single `bl 0x800d9934`. They contain **no logic, no
branches and no validation**:

```asm
stwu 1, -16(1)
mflr 0
stw  0, 20(1)
  ... argument shuffling ...
bl  0x800d9934              ; __psndSFXOn
lwz  0, 20(1)
mtlr 0
addi 1, 1, 16
blr
```

The return value of `__psndSFXOn` passes through untouched in `r3` — that
is the handle later consumed by
[`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md),
[`psndSFX_pit`](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) and
[`psndSFX_vol`](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md).

## `psndSFXOn` — non-positional (`0x800d98bc`)

```asm
800d98c4: li      4, 255             ; arg1 = 255
800d98c8: li      5, 255             ; arg2 = 255
800d98d0: li      6, 0               ; arg3 = 0
800d98d4: li      7, 0               ; arg4 = 0   <-- NULL position
800d98d8: li      8, 0               ; arg5 = 0
800d98dc: li      9, 16              ; arg6 = 16
800d98e0: li      10, 0              ; arg7 = 0
```

`r3` (the key) is the caller's only input.

## `psndSFXOn_3D` — positional, default arguments (`0x800d9848`)

```asm
800d9850: mr      7, 4              ; arg4 = caller's r4 (position pointer)
800d9854: li      4, 255            ; arg1 = 255
800d985c: li      5, 255            ; arg2 = 255
800d9860: li      6, 0              ; arg3 = 0
800d9864: li      8, 0              ; arg5 = 0
800d9868: li      9, 16             ; arg6 = 16
800d986c: li      10, 0             ; arg7 = 0
```

Caller signature: `psndSFXOn_3D(key, positionPtr, arg5)`. The caller's
`r5` is **not forwarded** — `arg5` is forced to `0`. In
[BtlUnit_snd_se_pos](BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md) that dropped
argument is the derived pitch/variation id, so `psndSFX_pit` supplies it
after the start instead.

## `psndSFXOnEx_3D` — positional, full set (`0x800d9808`)

```asm
800d9810: mr      9, 8              ; arg6 = caller's r8
800d9814: li      8, 0              ; arg5 = 0
800d981c: mr      0, 7              ; save caller's r7 in r0
800d9820: mr      7, 4              ; arg4 = caller's r4  (position)
800d9824: mr      4, 5              ; arg1 = caller's r5
800d9828: mr      5, 6              ; arg2 = caller's r6
800d982c: mr      6, 0              ; arg3 = caller's r7  (via r0)
800d9830: li      10, 0             ; arg7 = 0
```

Caller signature: `psndSFXOnEx_3D(key, position, a1, a2, a3, a6)`. Five of
the seven forwarded arguments; `arg5` is forced to `0` and `arg7` is
always `0`.

Note the round trip of `r7` through `r0`: `mr 0, 7` at `0x800d981c` saves
the caller's `r7` because `r7` is reassigned two instructions later.

## Recovered `__psndSFXOn` ABI

Comparing the three wrappers fixes the convention completely:

| Register | Name | `psndSFXOn` | `psndSFXOn_3D` | `psndSFXOnEx_3D` |
|----------|------|-------------|----------------|------------------|
| `r3` | key | caller's | caller's | caller's |
| `r4` | arg1 | `255` | `255` | caller's `r5` |
| `r5` | arg2 | `255` | `255` | caller's `r6` |
| `r6` | arg3 | `0` | `0` | caller's `r7` |
| `r7` | arg4 (position) | `0` | caller's `r4` | caller's `r4` |
| `r8` | arg5 | `0` | `0` | `0` |
| `r9` | arg6 | `16` | `16` | caller's `r8` |
| `r10` | arg7 | `0` | `0` | `0` |

Two invariants hold across all three entry points:

- **`r8` (arg5) is always `0`.** No public wrapper sets it.
- **`r10` (arg7) is always `0`.**

`arg1` and `arg2` default to `255` and `arg6` to `16`; only
`psndSFXOnEx_3D` overrides them.

`r7` is the position pointer — `0` for the non-positional entry. This is
consistent with the accessor family, which publishes position into
`slot+0x1c..0x24` only after a start has allocated the slot.

## Porting notes

1. These wrappers contain no logic. A port should not add validation,
   clamping or null checks here; the defaults are the contract.
2. `psndSFXOn_3D` **discards the caller's `r5`**. Passing a pitch id there
   has no effect — it must go through `psndSFX_pit` afterwards, which is
   exactly what `BtlUnit_snd_se_pos` does.
3. `arg1 = arg2 = 255` and `arg6 = 16` are the defaults every in-tree
   caller sees. Do not change them when porting `psndSFXOnEx_3D`'s
   overrides.
4. The handle returned is the one the accessors expect, packing slot index
   in bits 0..7 and channel allocation in bits 8..23.

## Remaining gaps

- **`__psndSFXOn` `0x800d9934` (1344 bytes) is not decoded.** Its body is
  where the slot at `0x803df988` is allocated and where `slot+0x00`
  (key), `+0x04` (channel), `+0x06` (channel bits) and `+0x18` (handle)
  are written, and where the handle is packed from bits 0..23. That is
  the single largest remaining gap in the positional-SFX family and it
  blocks the whole audio chain.
- Names for `arg1`, `arg2`, `arg3`, `arg6` (plausibly volume, priority,
  flags, distance/rolloff) are inferred only from the `255`/`16` defaults
  and from `psndSFX_vol`/`psndSFX_dist` being separate calls. Not
  established.
- Callers of `psndSFXOnEx_3D` — none are documented in this repository.
- No original execution trace.
## Original evt_snd_sfxon raw event wrapper

2026-10-13 follow-up fromordinaryHammer usedsource pending. Complete US
`evt_snd_sfxon8015235c/64` andpsndSFXOn800d98bc/3c inspected read-only;
/tmp/trio-hammer-sfxon.dis/.elf andtrio-hammer-psnd-on.dis/.elf are ignored
inspection evidence. This closes eventABI, notunderlyingkey/macro/sample/
voice playback completion.

Twoeventoperands are **both RAW**:
`(rawSoundKeyPointer,rawOptionalIntOutputToken)`.
Readentry18argumentpointer; capturewordarg1 asrawtoken, wordarg0 askeyinput.
Thereisno evtGetValue/evtGetFloat, actortranslation, firstcallgate, nullkeyskip,
positionlookup, delay, voicehandleclassification, allocation or RNGinwrapper.
CallpsndSFXOn(key) onbothbranches. If **rawarg1==0**, discardregisterreturn
andperformno eventwrite. Ifrawarg1!=0, passreturnedword unchangedasintto
`evtSetValue(event,rawarg1,returnedHandle)`. Return2.

Arawlocalvariabletoken whose currentvalue0 isstillnonzeroRAWandrequestsoutput;
thisisnotconditional onresolvedlocalvalue. Neither keynoroutputtokenisresolved
beforeplayback. UnderlyingpsndSFXOn passeskey,255,255,0,NULLposition,0,10hex,0
to__psndSFXOn800d9934. Itsresult/voiceidentity/resources areownedbythatservice,
notinventedbyeventwrapper. Thereisno Off orretainedvoicefieldwritehere.

UsedordinaryHammercommand8035e128 haskey802ef2dc
SFX_MARIO_HAMMER_SWING1 and **literaloutput0**. A scopednative retainedoriginal
soundrequestneednotfabricate avoicehandlefor thisdiscarded-return branch;
callbackmaycompleteevent2 whileactualaudiointegrationremains explicitly
unboundpresentation. Itmuststillrecord/requestthisauthoredkey. Fornonzerooutput,
provideanactualnativevoicehandle adapterorretainpending ratherthanstoringan
unownedfake0/−1. Sourcewrapperitselfdoesnotnormalizefailedhandles. Existing
movement/chargevoice owners aredistinctfromthisnonpositionaloneshotwrapper.
