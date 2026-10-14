# Camera road manager copies and timebase composition

US G8ME01 revision0; recovered2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md). Complete camRoadMain8000ab48/
4f0hex (316 instructions), getV60FPS80005a00/d8hex (54), plus scoped signed
64-bit divider ABI. Static source, not observed framing or measured cadence.
Addresses/fields hexadecimal; counts/arithmetic decimal. This strengthens the
existing [finite transition contract](OVERWORLD_CAMERA_ROAD_TRANSITIONS.md)
with complete body coverage, fresh/captured owners, copy order and clock branches.
Read [comparison](FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md),
[interpolation/current construction](FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md).
Their child contracts remain separate; complete caller coverage does not close
candidate geometry/math/library/provider/original execution.

## Incoming coordinates and sequential transition gates

Retain supplied output O/r3 into GPR31. Publish supplied f1..f6 individually to
fresh G=[80414fa8] fields1ec/1f0/1f4/1f8/1fc/200. Each publication reloads G.
Divide each supplied float separately by original10.0 bits41200000/8041f644;
request calcCamRoad8000b6cc(G+68,G+d8,f1..f6) at8000abb4 using one freshly loaded
G for the two pointer arguments. All six original inputs are published before
that request. Incoming seventh FLOAT f7 is forwarded without division here; calcCamRoad
captures it into f22 at8000b798. This is separate from integer register r7.
Child return ignored; no local input/active/extent guard.

After child, fresh G.c8 nonzero admits compareCamRoad(G+d8,G+68,G+d4) at8000abd4.
Nonzero result K starts/restarts the transition in order:

1. Fresh G.198=K; another fresh G.c8=1.
2. Capture GP=[80415fd0] and another fresh G. Copy GP40(high)/44(low) to G1c8/1cc,
   loading high then low, storing LOW first, then HIGH.
3. Another fresh G.d0=original float0.
4. Copy fresh G+08 to same owner's G+138 (96 bytes); then separately fresh
   G+68 to same owner's G+d8 (96 bytes).
5. Recompute G.cc admission described below, using another freshly loaded G.

Zero comparison instead copies fresh G+68 to G+d8 only. It does not locally
reset c8,cc,d0,d4,198,timestamp or fixed-origin138. Comparison's own possible
d4 output store remains its contract; a return0 leaves that output unchanged.

NEXT test fresh G.c8 again. If0, request reverse compare(G+68,G+d8,G+d4) at
8000ad54. Nonzero K has the same publication/timestamp/progress order, but its
first96-byte copy is G+d8→G+138, then G+68→G+d8; recompute cc. Zero does no
copy here. These are sequential fresh tests, not mutually exclusive branches
on one retained initial c8; children/other owners can change globals.

## Anchor-blend admission after start/restart

One freshly loaded G owns this calculation. G.worde0!=0 OR G.word140!=0
immediately selects cc=1. Otherwise, if G.word1d0==101, inspect nullable G.fc
and G.15c pointers: either pointed word20!=0 selects1. Other versions skip
these word20 checks. Then, irrespective of version, inspect nullable G.fc and
G.15c again: pointed float7c unequal/unordered versus0 selects1. All paths
without such admission select0. Store literal0/1 into that captured G.cc.

G.fc/15c are reread at their later tests. No nonnull record extent/type/lifetime
or finite check. ±0 compare equal; NaN is unequal/unordered. Source worde0/140
are state+8 in the existing record layout, not state+4. The mask198 is not
consulted by this gate. No universal record7c-as-duration policy follows.

## Progress and interpolation owner boundaries

Read fresh G.c8; nonzero:

- Capture GP; use captured G.d4 in f1, previous G1c8/high and1cc/low in r3/r4,
  current captured GP40/high,44/low in r5/r6 for getV60FPS at8000aeb4.
- AFTER helper return, reload G, read G.d0 FRESH, single-add returned increment,
  store d0. Reload G AGAIN, reread d0; ordered>1 stores source1.0. No lower
  clamp or NaN completion fallback.
- Reload GP and G, load GP40/high,44/low; store G1cc/low BEFORE G1c8/high.
  This need not use the previous helper's captured owners/time snapshot.
- Fresh G supplies mode=G.cc,A=G+138,B=G+d8,r6=G+68(unused locally by child),
  O=G+08,progress=G.d0 to calcHokanCamRoad at8000af14.
- AFTER child, fresh G.cc nonzero sets G.word10=1, otherwise0. This is
  current state+8, written after interpolation; do not copy the whole state
  prematurely or freeze pre-child cc.
- Another fresh G.d0 compares EQUAL to1 using fcmpu. Exactly1 copies G+d8→G+08
  (96 bytes), then fresh G.c8=0. Otherwise leave c8 unchanged locally.

This is not one increment per render frame. Fixed-step native time must be
an explicit adapter to the supplied GP timestamp owner, with quantization kept
separate from original observations. Newly captured equal timestamps ordinarily
yield0; clock/global mutation can change later snapshots.

## No-transition path and exact copy semantics

At the fresh c8 test above,0 selects G+68→G+d8 with retained G7; then reload
G for DESTINATION ONLY, copying retained G7+d8→fresh G+08. This mixed owner
boundary is visible at8000afa8..b4. Do not silently redirect the second source
to the refreshed destination owner. Under stable G, it matches the earlier
finite contract's N→D→C; stable pointer identity remains a provider domain.

All eight copy loops use12 iterations, EACH ordered as: load first word, load
second word with source+8 update, store first word, store second word with
destination+8 update. They copy96 bytes forward in pairs, without memset,
whole-record snapshot, memmove/overlap handling or IRQ masking. Copy bases:

| Loop start | Source offset | Destination offset | Owner |
| --- | --- | --- | --- |
| 8000ac2c | 08 | 138 | Same freshly captured G |
| 8000ac54 | 68 | d8 | Separate fresh G |
| 8000ad24 | 68 | d8 | Same fresh G, active compare0 |
| 8000adac | d8 | 138 | Same fresh G, start |
| 8000add4 | 68 | d8 | Separate fresh G |
| 8000af60 | d8 | 08 | Same fresh G, completion |
| 8000af94 | 68 | d8 | Retained G7, no transition |
| 8000afbc | d8 | 08 | Retained G7 source/fresh G destination |

Each loop's first-word read is base+4 from the artificially backed-up iterator,
so the table records ACTUAL first copied byte. Interruption/alias/other writes
may affect later pair reads; valid nonoverlapping stable-domain copies are not
proof of those broader domains.

Always request calcCurrentCamState(retained output O) at8000afd4. Then read and
single-multiply its O18,1c,20,0c,10,14 individually by10.0 and store, in that
order. O0/4/8 unchanged by this scaling tail. No null/result guard or further
camera publication. ABI16-byte frame/GPR31/LR; no independent scalar result.

## Complete getV60FPS: division before timestamp difference

Arguments: supplied factor R/f1, previous timestamp high/low r3/r4, current
high/low r5/r6. Preserve R in f31 and current pair in GPR27/26. Read bus word
B=[800000f8]; unsigned Q=B>>2, K=highU32(10624dd3*Q)>>6. Preserve K in GPR28.
No zero-divisor/bus/time validity guard in this body.

Request __div2i80264ad8(previousHigh,previousLow,divisorHigh0,divisorLowK) at
80005a48; retain returned pair P. Then same division for CURRENT pair at80005a64,
retain returned pair C. This is the **signed divider entry**, not __div2u.
Scoped sign-normalization and quotient-sign-restoration words are pinned; the
full restoring loop/zero-divisor/overflow cases remain a library contract.
For nonnegative representable timestamps/positive K the ordinary mathematical
model gives each timestamp's integer quotient before subtraction; do not
substitute divide(current-previous) or infer unsigned handling for bit63-set
inputs from the original finite increasing-time examples.

Compare returned pairs as UNSIGNED64 using subtraction/borrow. If C>=P,
D=C-P. If C<P, compute wrap64((UINT64_MAX-P)+C), WITHOUT adding1. Thus supplied
quotients P=UINT64_MAX,C=0 produce0, while ordinary modular subtraction would
produce1. This is the actual helper branch, not corrected clock rollover.
Values are returned quotient words; mathematical examples do not establish
which exceptional timestamps produce them through the signed child.

Request __cvt_ull_flt80264f24(Dhigh,Dlow) at80005aa4. With returned f1 F, compute
single multiply(source60.0,R), single multiply(result,F), single divide by
source1000.0; return f1. Source bits42700000/447a0000 at8041f450/44c. Conversion/
rounding/FP domains are child-owned. No OSGetTick request, negative-factor clamp,
60Hz physical observation or tick-delta cap. ABI48-byte frame/GPR26..31/f31/LR
and paired-single state restored; three direct calls, no indirect calls.

## Evidence and remaining work

[Manifest](../../../tools/camera_road_manager_time_evidence.json):13 spans/
399 anchors (all370 body words/five constants/23 divider ABI words/one f7 capture),nine
local calls, no indirect sites. Eight pair-copy layouts, six quotient-delta/five
bus-divisor/six progress mathematical vectors, exact publication/capture/child/
branch/source pins checked independently. Existing finite transition manifest
reverified separately; its positive-time model does not prove the wider domains.
Raw ELF/listings/generator stay `/tmp`; no bodies/binary assets committed.

OW-CAMERA-01 now has complete local manager/time-helper composition. Remaining:
calcCamRoad/collisionCurve authored selection/contact/geometry/limits; actual
work/state/record/GP/bus/time/callback/alias/lifetime/IRQ/FP domains, complete
math/division/conversion implementations and original room/control/pause/cadence/
visual/native binding. The parent request and full-game port remain OPEN.
