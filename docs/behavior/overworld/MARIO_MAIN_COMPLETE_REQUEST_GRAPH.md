# Complete Mario field update request graph

US G8ME01 revision0; recovered2026-10-13 for
[MAINLOOP-01](../../requests/MAINLOOP-01.md). Complete marioMain8005a7e0/9f8hex,
638 instructions. Addresses/masks/field offsets hexadecimal below; numeric
arguments/counts decimal unless explicitly prefixed. Static original code, not
measured retrace/pad/gameplay behavior. Children are requests; naming a child
never establishes its complete internals. Read
[outer update order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md),
[direction rate](OVERWORLD_DIRECTION_MOVEMENT_RATE.md).

## Corrections to the earlier sketch and request

801a938c is **marioPreCamera**, requested at8005a9dc; it is not motion dispatch.
8026d464 is **strcmp**: six short-circuit room-name comparisons, not six
subsystem updates. 8002e218 is **seqSetSeq**, not seqMain8002e228.
800d3d30 is **pouchGetHP**, not pouchAddHP. The eight calls to seqSetSeq locally
assign arguments(5,0,0) only when signed HP result<=0; no local write of
Mario state=5 or constructed battle parameter accompanies them.
Actual motion service marioMotion8009461c is requested at8005aacc for motion29,
or8005abf0 after ordinary input preparation. The original request text remains
verbatim; these corrections supersede its mistaken identities.

The entry tests M.word0 MASK1, not pointer-null/state-nonzero. M fields4c/50/54
are signed halfword counters/state, not stored position XYZ. Gravity is M228;
M174/178/17c are cleared to float0 by the finish blocks, not assigned gravity.
No local moon attribute flag or motion29 prerequisite selects half gravity.

## Entry, control/badge gate and room gravity

Capture M=[8041e900] into r30 at8005a800; retain rodata base802c18a0/r31.
If M.word0&1==0, exit immediately. No pointer/extent validation.
Load GP=[80415fd0], GP.word1338; low16&100f nonzero forces gate1, otherwise
use signed M.byte38. Gate0 skips only the badge/control block ending8005a934;
it does NOT skip the remaining field update or marioUpdateKeyData.

Gate nonzero: if signed retained M.byte3c==0, request
pouchEquipCheckBadge800d3300 with IDs327 then328, preserving first result across
second call. Build mode1(first nonzero/second zero),2(first zero/second nonzero),
3(both nonzero),0(both zero). Read Mario pointer FRESH and its signed byte3c;
if still0 and signed byte3d differs from computed mode, store low8 mode to3d,
request marioSetCharMode8005d740(0). No mode change request on equality.
Next capture Mario pointer FRESH/r29 BEFORE badge323 request. If that result
nonzero and captured motionU16+2e is0 or1, OR1000 into captured wordc. If motion2,
request marioChgMot8009455c(1), then OR1000 into that same captured wordc. Other
motion or badge result0 does no such write. Calls may mutate current globals.

Always request marioSetDirEventMain80096a08 at8005a938. Store float1.0 at retained
M228 from original bits3f800000/80420008. Compare FRESH GP+12c C string against
these original names, in order, rereading GP before EACH request:

| Call site | Original name / address |
| --- | --- |
| 8005a950 | moo_00 /802c1c00 |
| 8005a968 | moo_01 /802c1c08 |
| 8005a980 | moo_02 /802c1c10 |
| 8005a998 | moo_05 /802c1c18 |
| 8005a9b0 | moo_06 /802c1c20 |
| 8005a9c8 | moo_07 /802c1c28 |

First strcmp result0 skips later comparisons and stores float0.5
(bits3f000000/80420068) at retained M228. All six nonzero leave1.0. No prefix,
numeric room range, moo_03/04 inclusion, motion-ID or attribute-bit substitute.
Then request marioPreCamera801a938c, marioHitCheckWidth800916f0, in that order.

## Timers before the motion branches

Retained M.s16+4c>0: decrement/store low16; nonpositive stays unchanged.
M.s16+2d8>0: decrement/store low16, reread signed value. If its signed remainder
mod30==0 AND value>0, construct stack XYZ=(M8c, single(M90+30.0),M94), request
itemCoinDrop800aeaac(pointer). The30.0 bits41f00000 live80420048. No battle admission
or elapsed-seconds measurement; one decrement per admitted invocation.
M.s16+5e>0: decrement/store; if resulting signed low16<=0 clear MASK00100000
from retained M.word4. Nothing clears this bit on an initially nonpositive5e.

## Motion29, flags2/4 and character mode precedence

Read retained M.motionU16+2e. Exactly29 requests in order:

marioMotion8009461c; marioSearchGround8008e694(stack x/y/z outputs, f1=0,f2=0);
fresh Mario.word1e8=result; marioPreDisp80057878; partyMain800bc574;
marioCamMain801a924c. Reload current Mario into r30 afterward. Run the position
ring block below with this refreshed owner, then finish using retained r30.
No ordinary key/stick/use-party or final common suffix occurs on this branch.

Other motions read retained M.word0. If MASK2 set: MASK4 additionally set
requests marioCtrlOff2Main8005a634; either way jump directly to the common suffix
at8005b0dc. This bypasses input preparation and marioBoots.
If MASK2 clear, request marioUpdateKeyData80095cb0, then
marioGetStick80095af8(M+198,M+194), marioUseParty801414a0, marioMotion8009461c.
These use retained M for pointer arguments; no local post-child active recheck.

Then signed retained M.byte3c:2 requests kpaMain801fec90 and finishes with a
fresh Mario snapshot;1 requests peachMain801fc1d0 and exits directly WITHOUT
position copy/velocity clear/counter increment or common suffix. Other values
continue to motion selection, freshly reading retained M.motionU16+2e.

## Exact motion equality chains

The selector at8005ac80 uses equalities, not ordered thresholds. The low16
expression(motion-31)<=1 groups31/32, additionally matching24 and33 explicitly.
Later reads in children/tail remain fresh where shown; a stable model does not
freeze mutable motion through the entire invocation.

| Selected motion | Ordered requests, then local continuation |
| --- | --- |
| 24,31,32,33 | marioSearchUnder8009023c → marioPreDisp → marioBottomless80096e8c → partyMain → marioCamMain; clear retained M1f8,1e4,1e0,1f0,1f4=0; finish with fresh Mario |
| 28 | marioChkGnd8008f248 → marioChkToge8008dd04 → marioPreDisp → marioBalloonMain8012eff8 → marioBottomless → partyMain → marioCamMain; finish fresh |
| 20 | marioMove80059a78 → marioChkToge → marioPreDisp → marioBalloonMain → marioBottomless → partyMain → marioCamMain; finish fresh |
| 26 | marioChkToge → marioBottomless → marioPreDisp → marioBalloonMain → mario_kemuri8013597c → partyMain → marioCamMain; finish fresh |
| 8 | marioBottomless → marioPreDisp → marioBalloonMain → mario_kemuri → partyMain → marioCamMain; ring block, then finish fresh |
| Others | Flag arbitration below; then common suffix/ring/finish |

No local broad >=8/20/24/25/26/33 routing. Semantic names of individual motion
IDs require their producer/child contracts; this table records actual requests.

## Remaining flag arbitration and common suffix

Capture retained M.word0 into r4 at8005afc4. Test these masks in precedence:

- MASK00040000: marioMove → marioChkOverhead8008c904; if retained M.s16+50>100,
  marioChgMot(10) → marioSetFallPara8009412c. Then marioBoots80094680.
- Otherwise MASK00010000: marioJump80093d38 → marioChkOverhead → marioMove;
  fresh current Mario motion22 suppresses marioChkGnd2. Otherwise retained
  M.s16+50>5 requests marioChkGnd28008f0e4. Then marioBoots.
- Otherwise MASK00020000: captured selector motion25 bypasses this branch's
  requests directly to marioBoots. Other motion requests marioMove, then
  rereads retained M.motion. If16: marioChkOverhead; retained M1f8!=0 copies returned
  f1 to retained M90. Other motion calls marioChkOverhead without that write.
  Request marioFall80093910, reread retained M.motion:16/17 always request
  marioChkGnd2; other motion uses fresh current Mario motion22 exclusion and
  retained M.s16+50>5 condition. Then marioBoots.
- No masks: marioMove → marioChkOverhead → marioChkGnd → marioBoots.

MASK00020000 motion25 comparison uses r3 saved from selector; it is not a new
motion read. All three mask tests use the same captured r4. Children can alter
retained/current motion before the explicitly later reads.

Common suffix8005b0dc: marioForceMoveMain80095f64 → marioBottomless →
marioPreDisp → marioBalloonMain → mario_kemuri → partyMain → marioCamMain.
Run ring block, then finish with fresh Mario. Flag2 paths enter suffix without
marioBoots; regular arbitration requests marioBoots first. No local extra NPC,
collision, parts-delete or smartFree request in this complete body.

## Position ring and eight finish blocks

Ring sites8005ab00(motion29),8005aefc(motion8),8005b0f8(common): test retained
owner.word34 low1. If set, test owner.word4 MASK08000000; motion29 uses refreshed
r30, other two use FRESH current Mario for the second test/XYZ. If both pass,
load indexI=[8041e8f0], compute wrap32(803e9848+12*I), store raw XYZ8c/90/94 there.
Store wrap32(I+1) back to index. Signed new index>=8 subtracts8 ONCE and stores;
no initial index validation, modulo normalization or clamped ring domain.
No writes on either failed gate; other motion branches do not run this block.

Eight finish starts:8005ab5c,ac08,acd0,ad5c,ade8,ae74,af5c,b158. Only first uses
already refreshed retained r30; others capture current Mario into r29 ONCE at
start. Captured owner persists across pouchGetHP/seqSetSeq children:

1. Copy signed halfword50 to54 (low16), raw word1e8 to20c.
2. Request pouchGetHP. If signed returned r3<=0, request seqSetSeq(5,0,0).
   Positive result skips only that sequence request. No local battle parameter,
   state5 write, return-result guard or request to seqMain here.
3. Copy raw current owner8c/90/94 to110/114/118. Store original float0 bits
   at174/178/17c. Reload owner.word34, add1 wrap32/store; exit.

No fresh owner reload after HP/sequence children; post-child reads observe the
captured owner, which can differ from the current global pointer. Inactive entry
and character mode1 skip these finishes. No local timer/IRQ wait or 60Hz clock.
ABI:64-byte frame/GPR29..31/LR saved/restored. No defined independent scalar
result beyond child/register behavior is inferred.

## Validation and native port boundary

[Manifest](../../../tools/mario_main_complete_evidence.json):41 spans,
654 anchors (all638 body words/four float words/twelve room-name words),100
direct calls to35 distinct linked children, zero indirect calls. Original hashes,
source target identities, room names/constants, six gate/fourteen motion/ten
gravity/four wrap-counter mathematical vectors and eight finish sites checked.
Generic verifier checks pins/targets, not claims about complete children/runtime.
Raw ELF/listing/generator remain `/tmp`; no binary assets/source bodies committed.

For 2DS XL use simulation invocations to preserve these gates/order/captures;
render frames must not stand in for counters. The native fixed60Hz/max-five-step
accumulator remains an adapter until physical cadence/pause services and
original execution establish equivalence. MAINLOOP-01 now has its complete
local marioMain graph; child mechanics, current pointer/room/badge/flag/motion/
ring/input domains, pause/timebase, sequence5 ownership, original traces and
full native binding remain OPEN. No full-game completion claim.
