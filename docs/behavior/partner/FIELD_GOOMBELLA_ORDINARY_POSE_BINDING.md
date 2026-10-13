# Goombella ordinary stay, walk and run resource binding

US G8ME01 disc0 revision0, read-only static recovery, 2026-10-12.
This supplements [numeric pose requests](FIELD_PARTNER_POSE_REQUEST_SELECTION.md)
with the decoded kind1 row and the complete moving selector
`unk_800cbb10` at `0x800cbb10`, size `0x120`. It does not establish original
visible performance, recruit/story ownership, or the follower simulation.

## Original kind1 normal resource row

Normal table `0x80314708 + 60*1`, SHA256 of its 60-byte row:
`7a458d068ea5e3d0cb5ba6c6079b3443adf63402ff7ad2d5f39ae775f757e2c7`.
These are actual decoded pointer targets, not names inferred from asset spelling.

| Selector | Pointer address | Original name |
| --- | --- | --- |
| 0 | 0x802cb4d4 | PKR_Z_1 |
| 1 | 0x802cb4dc | PKR_S_1 |
| 2 | 0x802cb4e4 | PKR_W_1 |
| 3 | 0x802cb4ec | PKR_R_1 |
| 4 | 0x802cb4f4 | PKR_J_1A |
| 5 | 0x802cb500 | PKR_J_1B |
| 6 | 0x802cb50c | PKR_N_1 |
| 7 | 0x802cb514 | PKR_T_1 |
| 8 | 0x802cb51c | PKR_D_1 |
| 9 | 0x802cb524 | PKR_D_4 |
| 10, 11 | 0x802cb4dc | PKR_S_1 |
| 12 | 0x802cb52c | PKR_D_2 |
| 13 | 0x802cb534 | PKR_W_2 |
| 14 | 0x802cb53c | PKR_R_2 |

Ordinary stay requests **selector1 / PKR_S_1**, including the actual
[stay entry](FIELD_PARTNER_STAY_MOTION.md) at `0x801516a8` and walk-mode marker
consumption at `0x800c85c8`. `PKR_Z_1` is selector0, not the ordinary stay request.
Normal kind1 HP<=5 remaps selectors1/2/3 to12/13/14 as already documented.
The dot-table admission remains separate; these decoded names describe normal
table selection only.

## Moving selector: exact conditions and request order

Input P=r3, speed=f1. The source reads P+4 mask **0x100** first. Set requests
`partyChgPoseId(P,3)`, then `unk_800cbcc0(P,1)`, and returns without owner or
input-class queries. The latter is a sound/movement presentation helper whose
full local behavior remains outside this resource binding.

Otherwise read owner M=P+160 and its float M+228. Exact equality to1 selects
B=`0x3fd33333` (1.649999976158142) and D=`0x3f4ccccd`
(0.800000011920929); any other value selects B=2 and D=0. Original constants
are respectively `0x804215f4`, `0x804215fc`, `0x804215cc`, `0x804215b8`;
equality compares against1 at `0x804215f8`. R(x) means binary32 rounding.

1. If ordered `R(B-D) < speed` **and** ordered `speed < R(speed+D)`, request
   global `N_marioGetMovementSpeed()` at `0x80095a8c`. Result==1 requests
   selector2 then `unk_800cbcc0(P,0)`; every other result requests selector3
   then `unk_800cbcc0(P,1)`.
2. Otherwise ordered `speed >= B` requests selector3 then sound helper1;
   all other comparisons request selector2 then sound helper0.

The second comparison is literally speed versus **speed+D**, at
`0x800cbb8c–94`, not B+D. Do not replace it with an invented hysteresis upper
bound. For ordinary finite scale1 values, the first boundary is exactly
0.8499999642372131. Values at/below that boundary select2; larger finite values
normally use the global Mario class even above B. Exceptional binary32 rounding
and unordered comparisons follow the literal sequence. For scale!=1, D=0
prevents the first branch, and finite speed>=2 selects3, otherwise2. NaN speed
falls to selector2 unless the force flag selected3 first.

This helper does not request stay on speed0. Its ordinary ground caller
`party_motion_homing_walk` at `0x80150980` passes current P+104 as speed when
that field compares unequal0; equal0 requests motion0 separately. Six direct
DOL callers were found: beside `0x8014ddc4`, behind-fly `0x8014e214`,
behind-continue `0x8014e75c`, behind `0x8014ec04`, homing-fly `0x8014f80c`,
homing-walk `0x80150980`. These use the current movement speed field, not draw
delta or rendered frame count.

`N_marioGetMovementSpeed`, size0x6c, first requests global Mario via
`marioGetPtr` at `0x8005e0c0`. Sign-extend bytes M+252 and M+253. Both zero
returns0; otherwise compute their integer squared norm. Norm>3025 returns2;
norm<=3025 returns1. This is the processed original stick state, not a query
of P+104. Source byte production and native Circle Pad conversion remain
separate input-adapter work.

`marioMain` publishes M+228=1 at `0x8005a93c–44` from literal
`0x80420008` (`3f800000`). Its subsequent map-name comparisons substitute.5
at `0x8005a9d4–d8` from `0x80420068` (`3f000000`) for moo_00, moo_01,
moo_02, moo_05, moo_06 and moo_07 (strings `0x802c1c00..28`, eight-byte stride).
Ordinary Rogueport therefore uses1 under this source owner. This field is also
used by jump/speed owners; calling it only gravity would omit those consumers.
Other writers, alternate partner owner pointers and driver cadence remain open.

## Original animation descriptors, scale and facing

Read-only asset `a/c_pkr`, 120932 bytes, SHA256
`d712df83d7b70b64a90d45551b39ecf7adf277b9572f6931daae7792b52d69e7`.
Every resource below has one loop descriptor. Times are original animation
frame-domain data, not measured duration or a rendering-frame increment policy.

| Resource | Loop word | Start | End |
| --- | --- | --- | --- |
| PKR_Z_1 | 0 | 0 | 0 |
| PKR_S_1 | 1 | 0 | 224 |
| PKR_W_1 | 1 | 0 | 28 |
| PKR_R_1 | 1 | 0 | 12 |
| PKR_D_2 | 1 | 0 | 92 |
| PKR_W_2 | 1 | 0 | 28 |
| PKR_R_2 | 1 | 0 | 12 |

[Partner pre-display](FIELD_PARTNER_PRE_DISPLAY.md) consumes changed-name
markers and requests selector restart/local time before Main; same-name
requests retain their state. [Partner drawing](FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md)
sets the local rate **after** both draw calls:1 ordinarily, or.75 when P+0
mask0x80 is set. No speed-proportional animation rate exists in those local
owners. Clock units and original scheduling must compose with the animation
backend contracts; these loop values alone do not prove temporal parity.

[Original actor scale](../overworld/OVERWORLD_ACTOR_RENDER_SCALE_AND_IDLE_BOUNDS.md)
supplies ordinary scalar2 and initialization scaleXYZ=(1,1,1), hence model*.02
under the native engine*.01 adaptation. Partner draw reflection is a Z sign
selected from wrapped P+10c in inclusive[90,270]; yaw is wrapped(P+10c-P+f8).
It is not unconditional X mirroring. Animated model groups and visibility must
remain source-owned; static selector0 geometry cannot substitute for stay1.

## Verification and open work

Independent read-only disassembly and `/tmp/goombella-verify.py` check the15
pointer strings,7 asset loop descriptors,21 opcode anchors,5 constants and
the asset fingerprint. Moving selector body SHA256:
`3b58a16d181d5bb5a331ab13193174df6a9572c3537b65940761a4312492f391`.
Global movement-class body SHA256:
`2bd04d98c0fffd3d0be3c917260322f85afe38341c0c9c106bece3e17c50b558`.
`tools/verify_field_partner_predisplay.py` independently passes8 bodies,
515 instructions,32 constants,7 data spans,2 imports and3 caller sites.
Temporary inspection script/listings stay outside source history.

This is a narrow answer for [OW-PARTNER-01](../../requests/OW-PARTNER-01.md).
Original follow path, room/battle/story/recruitment lifecycle, full terrain
binding, processed input production, dot modes, sound helper, material/shadow
rendering, hardware performance and Dolphin-visible verification remain open.
