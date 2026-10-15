# Quantized Maya curves and retained effect-pool continuations

US GameCube G8ME01 revision0, static read-only DOL analysis. This supplements
[Maya/recovery ownership](RECOVERY_EFFECT_LIFECYCLE_AND_MAYA_OWNERSHIP.md),
[pool acquisition](EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md),
[initialization/context release](EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md)
and [driver/cache ordering](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).
Original timing, raster output and hardware performance are not measured here.

## Quantization is signed half divided by64

`effCalcMayaAnimMatrix8005e4f4/3ac` forms literal `0x06070607` and writes
SPR917/GQR5 at8005e554. The scalar paired-single loads at8005e5c0..5d4 use
index5 and W=1. Load type occupies bits16..18 and is7 (signed16); load scale
occupies bits24..29 and is6. The decoded value is therefore signed-half times
2^-6. Type7 is not scale7. Both store fields also happen to have type7/scale6,
but the curve evaluator performs loads. The layout and dequantization can be
cross-checked against Dolphin's primary
[Gekko register definitions](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Gekko.h)
and [paired-load interpreter](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_LoadStorePaired.cpp).
These references explain the instruction encoding; the game-specific literal,
load operands and curve ordering come from the supplied ISO.

A channel descriptor contains signed key count at0 and key pointer at4. Counts
<=1 return without writing either output. Each key is0xb2 bytes: signed-half
time followed by22 eight-byte channel records. The scan chooses the first next
key whose signed time is ordered >= frame. If none qualifies it uses the last
pair with evaluation time equal to that next key's time. A NaN frame follows
the latter branch; a frame before the first key is not clamped up to its time.

For each channel, source reads previous+2/value, previous+4, previous+6/outgoing
slope, next+2/value, next+4/incoming slope and next+6, in that order. All six
loads occur before reading the signed hold byte at previous+9. This byte is
relative to the channel's eight-byte stepping base, so it is the final byte of
that channel's eight-byte record after the key time. A nonzero hold byte stores
the previous value. The unused previous+4 and next+6 loads are still part of the
source read order.

## Hermite operations preserve instruction boundaries

For an interpolated channel, reload signed previous/next times and subtract
them as signed integers before the signed-to-float conversion. Let d be that
converted difference. Only d==0 substitutes the source denominator epsilon
`0x322bcc77` at804201a4; negative d retains its sign. The numerator is evaluation
time minus converted previous time, followed by one single-precision divide.
Source constants are0, epsilon,3,-2,1,2 at804201a0..1b4.

With u the quotient, the instruction order is:

1. q=round(u*u); c=round(q*u); t=round(3*q).
2. a=round(-(2*q-c)) using one `fnmsubs`; b=round(2*c-t) using one `fmsubs`.
3. e=round(c-q); a=round(u+a); b=round(1+b).
4. f=round((-2)*c+t) using one `fmadds`.
5. a=round(d*a); e=round(d*e); a=round(previousOutgoing*a).
6. a=fma(previousValue,b,a), then a=fma(nextValue,f,a), then
   a=fma(nextIncoming,e,a), with one binary32 rounding per fused operation.

The mathematical Hermite polynomial is insufficient evidence for equivalent
rounding. In particular, splitting any fused product/add into two rounded
operations changes cancellation cases. Plain host multiply/add, a host `sin`
and a generic matrix library do not establish Gekko numeric parity. The original
SDK leaves and host/native numeric admission must remain explicit owners until
independently implemented and checked.

## Transform order and scalar publication

The22 outputs are translation0..2, X/Y/Z rotation3..5, scale6..8, pivotA9..11,
translationA12..14, pivotB15..17, translationB18..20 and scalar21. The source
constructs all translation matrices, including both negated pivots, before
concatenation. Using ordinary column-vector matrix notation, it calculates:

- B=((translationB * pivotB) * scale) * negativePivotB.
- A=(((((translationA * pivotA) * Rz) * Ry) * Rx) * negativePivotA).
- output=(translation * A) * B.

The parenthesization records actual concat call order. Temporary destinations
alias selected inputs in several calls, so the matrix backend must admit those
original alias patterns. Do not regroup the product, switch rotation order or
replace pivots with an unexplained host transform. Matrix publication happens
before scalar publication. Scalar>1 stores1; scalar<0 stores0; otherwise the
original scalar is stored, including NaN and signed zero under those comparisons.

## Retention is a native ownership adaptation

The source bodies are synchronous. A retained portable service cursor is an
adapter for unavailable host/native owners, not an invented GameCube event wait.
Acknowledged external allocations, clears, updates, frees and file requests
must occur once. A wait captures the original call arguments and the source
locals already computed; it must not restart a scan, replay earlier mutations,
read a replacement callback, or recapture a global that the source had captured
before that call. A completed callback may mutate guest globals and later
records; later source reloads must remain live. Ownership identity must reject
continuation through a different memory or service provider.

`effEntry8005ec68/68` searches ascending slots using exact flags==0, not merely
absence of active bit1. Source exhaustion leaves the pointer one record beyond
the captured array, and count<=0 writes at the base; neither path has a source
null/full check. A native bounded-storage policy must declare its admission
boundary instead of returning an invented successful null effect. Acquisition
writes flags1, then reads current GP14 and stores normalized context0/1; it
clears only record14,8,c,10 and byte18. Other bytes remain stale.

`effMain8005ea7c/170` retains signed count and array once, then freshly reads
GP14 for each record. It requires exact record context0 when GP14==0, exact1
otherwise, then active flags bit1 and a nonnull callback. Callback receives the
record only, and its source return is ignored. A later-slot allocation can run
in this same ascending traversal; an earlier-slot allocation waits for a later
invocation. A retained callback wait resumes that call before advancing.

After traversal, the cache tail reloads current manager and GP before comparing
cached selector with GP16c. A nonnull old handle is released, then current
manager.cache10 is cleared. Async and sync requests each independently reload
GP selector, prefix-table word and root. Async return0 finishes without tag
publication; any nonzero value, including-1, admits the sync request. The sync
returned handle, including0, is stored into a freshly selected manager; then
fresh GP and manager reads publish the selector. Async rejection and an
unavailable native async service are different outcomes. Prefix indexing is raw
wrapping selector<<2, not a validated language enum. Original file/cache/TPL
backends remain separately owned.

## Integration limits

These local bodies do not provide a native renderer, texture-object cache,
original sound macro player, terminal stardust lifetime, scene-camera adapter or
a complete battle effect scheduler. An addressed resource pack supplies original
bytes and pointer aliases; it does not prove effect admission. Test callbacks
which acknowledge math, rendering, audio or allocation prove only their stated
fixture boundaries. HP/FP gift consumption must keep its retained effect wait
until a real surviving constructor, driver, numeric, resource, display and sound
owner are admitted. Source scene4 display ownership may support top-screen and
lower-screen scene continuation, but does not itself prove audience interaction.
Optional audience tapping and physical-button parity remain independent battle
requirements; this work must not introduce touch-only control.
