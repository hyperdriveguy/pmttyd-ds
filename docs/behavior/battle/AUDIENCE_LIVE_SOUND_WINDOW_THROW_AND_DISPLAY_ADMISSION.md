# Audience live sound, window, throw, and display admission

2026-10-13 US G8ME01 disc0 revision0, read-only original ISO. Source continuation
after [actual setup4 zero-count initialization](AUDIENCE_SETUP4_ZERO_COUNT_EXACT_OPERATION_ORDER.md)
and [nonzero target adjustment](AUDIENCE_NONZERO_TARGET_ADJUSTMENT_CHUNKS_AND_ROUNDING.md).
This closes used Main leaves without treating flags0 or numeric sound handle0
as a global shortcut. No native/shared files or commit are part of this delivery.

## Actual initial sound history and public slot ownership

The actual Audience_Init memset supplies24 records R=A133fc+i*24(hex).
All begin flags WORD0=0, handle WORD4=0, lifetime WORDc=0 and other bytes0.
Native unavailable/unassigned-key failed request adapter for Init NoiseAlways
then supplies record2 handle-1. It does not make other handles-1. SetVol2
still installs its180-tick ramp fields; the latter can remain unadvanced when
the public handle checker rejects-1. Later real source sound requests and
SetVolAll overwrite fields according to their existing contracts.

The authoritative public accessor is named **psndSFXChk**, 800d95a0/58:

```
if rawHandle == -1: return -1    // before any slot access
slot = 803df988 + (rawHandle & 0xff) * 0x28
if ((rawHandle >> 8) & 0xffff) != U16(slot+4): return -1
return S32(~(wrapping(slot.key+1) | wrapping(-1-slot.key))) >> 31
```

The result is-1 iff the matching-generation slot key is-1; otherwise0.
Handle0 selects public slot0, generation0. Its meaning depends on the actual
slot key/generation owner, not the audience record's flags. Source liveness
checks no backend engine handle and no audience active flag.

Source public-slot initialization is explicit inside psndInit800dc4e0/170.
After its first backend SoundInit800e239c call returns,800dc524..590 initializes
40 public records at803df988, stride28hex, through5 unrolled groups of8:
WORD0 key=-1, WORD18 enginehandle=-1, HALF6 flags=0. It **does not write**
HALF4 generation; initial boot BSS supplies0, later initialization preserves
the existing generation. Other public-slot fields also retain their existing
contents. A source-initialized free public slot0 therefore rejects handle0
regardless of generation. It must be a retained real global registry; do not
run this initialization at Audience_Init or erase unrelated current effects.
Whole psndInit also has other backend/global requests; this is a bounded
public-slot write contract, not proof the entire audio bootstrap is implemented.

## Complete SoundMain record pass

SoundMain8019f97c/1e8 calls BaseGetPtr, then NoiseMain8019f7b4, then
SoundGetPtr(0). For each of24 records in ascending order, with no flags gate:

1. psndSFXChk(R04). Result-1 rereads R00 and clears **only bit1**, then skips
   all remaining work for this record. Handle/lifetime/ramp fields remain.
2. Otherwise if signed R0c==0: psndSFXOff800d95f8(R04), reread/clear R00 bit1,
   then skip. This call may change the public registry before checking later
   audience records which contain the same raw handle0.
3. Otherwise if signed R18>0: wrapping increment/store R14; call
   intplGetValue(mode0, step=R14, duration=R18,
   start=float(U8 R1f), end=float(U8 R20)). Truncate returned float toward0,
   store LOW8→R1e. Request psndSFX_vol800d97d0(handle R04,
   LOW8(signedDiv(wrapping(U8 R1d*U8 R1e),100))). Then freshly compare signed
   R14>=R18 and if true store R18=0. No R14 clamp/reset accompanies that store.
4. Fresh signed lifetime R0c>0 admits the following independent fade.
   If R0c==R10: BYTE R1f=R1e, BYTE R20=0. If R0c<=R10 call
   intplGetValue(mode0, step=wrapping(R10-R0c), duration=R10,
   start=float(U8 R1f), end=float(U8 R20)), then store LOW8 truncated→R1e and
   call the same volume-product request. Finally wrapping decrement/store R0c.
   Negative lifetimes, including-1, do not decrement or enter this fade.

Both explicit ramp and positive-lifetime fade can run in one record pass, in
that order. Neither is guarded by sound active bit1. Mode0 source interpolation
is single(start+single(single(float(step)*single(end-start))/float(duration))).
Duration0 returns end immediately before conversion/dispatch. The public
volume accessor rejects handle-1/key-1, otherwise stores LOW8 slot8; other
backend cadence remains its audio subsystem provider.

With actual free publicslot0 and record2handle-1, all24 Check requests return-1
and clear each record bit1. Ramp clocks do not advance: for example slot2's
180 or subsequent source fade30 remains stored. This is proven from the
specific initialized/live public registry, not a SoundMain no-op substitution.

## NoiseMain before the record pass

NoiseMain captures current B, A, record2, and the source live count801a2144.
It checks retained A.word0 mask10000 against B.ef4 mask80:
old clear/current set requests SetVol(2,60,80); old set/current clear requests
SetVol(2,0,80). Both tests occur before count-category updates.

Live count0 with R.byte8!=0 requests Off(R04), then BYTE8=0. Counts1..49,
50..99,100..149,150..200 select category BYTE8=1/2/3/4; a changed category
calls NoiseAlways801a0b88 and then stores the category. Count0/BYTE8=0 calls
no NoiseAlways or Off. Outside the stated ranges, category remains unchanged.
Finally A.flags2000 clear **or** livecount0 stores BYTE R1d=0. Otherwise it
stores LOW8(truncate(single(63+single(single(63*float(count))/200)))). Original
constants804249b4=63 and804249b8=200. No count-cache-only surrogate is used.

## WinCtrlProcess and exact built-in mode4 arithmetic

Window owner V=A1375c; source BaseGetPtr→WinGetPtr captures actual fields.
Signed A13784 converts to single C; single D=C-V18. D==0 skips amount change.
D>0 adds max(single literal.1,single(.1*D)) to V18. D<0 subtracts
max(.1,single(-.1*D)). Every multiply/add/subtract rounds to single. Freshly
read signed actualcount is converted again after the store; overshoot clamps
V18 to that fresh count. Do not replace this follower by an unconditional copy.

BYTE V0 nonzero wrapping increments WORD14 and upper-clamps signed>15 to15.
BYTE0 zero wrapping decrements WORD14 and lower-clamps signed<0 to0. Source
does not apply the opposite bound on either branch. Then always:
V08=intplGetValue(4, signed V14,15,-500,-280); FLOAT V0c=-160; FLOAT V10=+0.

Original intplGetValue80006c8c/3b0 dispatch table8030434c[4]=80006e90.
This used branch has no trigonometry or callback request. Let f round once to
single, s=f(signed step), n=f(signed duration), a=start,b=end:

```
if duration == 0: return b
d = f(b-a)
r = f(n-s)
squareR = f(r*r)
squareN = f(n*n)
return f(f(a+d) - f(f(squareR*d)/squareN))
```

Source orders r=n-s, d=b-a, squareN=n*n, squareR=r*r, a+d, squareR*d,
division, final subtract. Independent expressions have no writes/services;
the rounding points above preserve the used numeric dependency order.
No generic endpoint/clamp shortcut or integer easing helper appears. Normal
window timer0 producesX-500, timer15 producesX-280. Original constants:
804249e8=3dcccccd (.10000000149);80424a2c=bdcccccd (negative.1);
80424a30=c3fa0000 (-500);80424a34=c38c0000 (-280);
80424a38=c3200000 (-160);80424988=00000000.

## Throw tail after actual Init: exact zero-record rejection

Source Init SetThrowItemMax consumes irand100, sets max A137f4 to0/1/2/3
for draws<80/<90/<95/<100, and initializes A137f0=0,A137f8=0. Preserve actual
random output. CtrlProcess's throw tail first sets max10000 when B19078&20000.
B.ef4&80 clear then skips; otherwise signed counter>=max skips. Counter0/1/2
uses RNG bound1800/6000/30000, override20000 replaces it with60. Nonzero draw
skips. Draw0 initializes200 seat indices, performs1000 swaps (twoirand200
requests each), then attempts ItemOn801a5aac(index,0,1) in permuted order;
first return1 increments counter once. There is no empty-count shortcut.

ItemOn is its actual symbol name (earlier prose called it ThrowItemEntry).
The used zero-record body captures actor by original GetPtr. It first rejects
A.flags40000; otherwise uses actor BYTE1b type and original type row32bytes.
Zero actor type0 points to item table803b4c04, whose first WORD is121 (nonzero),
so the table gate passes. Actual arg4 item0 lies outside121..235 and skips
the badge gate. seat=index%20 outside4..15 returns0. Seats4..15 call
GetExist(index); actual WORDflags0 returns0 and ItemOn returns0. Thus all200
attempts faithfully fail before state checks/item allocation, with no actor/
item writes and counter still0. This closes the used empty throw path if RNG
admits it; positive occupied actors remain the real item/state providers.

## Display dependencies remain separately admitted

BattleAudience_Disp801a6c18/0d0 always calls AnimProcess801a4224 first. Its
200-record scan enters animation only for actor flags both1 and4; original
zero actors skip all animation mutations. Then A.flags20000 clear queues:

- camera4,class1,priority0: DispAudience801a38f8;
- camera4,class2,priority0: the same DispAudience;
- camera4,class1,priority0: DispItem801a37f0;
- camera8,class0,priority499: DispApSrc801a36c8.

Actual pouch half8c0 skips **only** the final camera8/class1/priority499
DispWin801a35d0 request. Other queued callbacks can have GX-state prefixes
even with no vertices/actors; their real render consumer is not implied by
AudienceMain completion. Window Main writes are still required independently
of this display registration gate. Break-slot and broader battle display
owners remain their existing subsystem contracts.

## Evidence and validation

Ignored new listings live under artifacts/recovered-temp-evidence/audience-live-*.
Complete body hashes (address/size hexadecimal):

| Body | Address/size | SHA256 |
| --- | --- | --- |
| SoundMain | 8019f97c/1e8 | f09a66ac8c4e0784251d8efd78676ea0d83d5148daae30c1b8e169f2ad9ddf9a |
| NoiseMain | 8019f7b4/1c8 | 3d686cd2fe740541c737d780b7f3a510acde9f5c3efa4f9a1fae49a6c5758371 |
| psndSFXChk | 800d95a0/58 | 1070b5fd16d79e26ece4205c7004df688d26669ddc9ff3dd4914274c88afd6fc |
| psndInit | 800dc4e0/170 | 3e0abb3a40bb3770ab7ad17952694b9d29c945ff9219102ebe5d53fdbe1e0483 |
| WinCtrlProcess | 801a4570/1e4 | 9d34c9d5dc54db93371924e8b874cd5780adf3463e8cac38d050708b24e1093a |
| intplGetValue | 80006c8c/3b0 | 0d800da046dc09ba1fe07a6933b4a910553c1fbe7ea2f6a874395018f4b6a4ae |
| ItemOn | 801a5aac/1fc | 732ac58f704352a0fa32ce40edf6da5dedafd0674c28c86f66d8389769b96a21 |

Linked audio authority for exact public field widths:
[corrected SFX accessors](../audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md),
[source provisioning](../audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
[audience sound history](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md).
Read-only complete source bodies, field/load/store tracing and scoped diff
check. No native test/build claim, shared documentation edits or commit.
