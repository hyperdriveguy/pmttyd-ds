# Mario-specific JumpParam: integer positions and count write-back

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete btlevtcmd_MarioJumpParam at0x80109c68,size0x19c,
SHA256`bf128edee4331114b3d4d4cd147fb2d0bb2580f6ea16f5add0eb11acd6d399d5`.
Original distance helper identity and numerical constants are pinned separately.
No original executed movement or measured count/impact trace is asserted.

## Argument resolution and service order

The [ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) supplies
(-2,L0,L1,L2,L6) at0x80356310 and0x80356484,after setting L6=36. This callback
can update that same count destination;36 is not guaranteed to remain36.
It has no first-call/polling gate and always completes with callback result2.

Resolve in this exact order:

1. Operand0 via evtGetValue,then BattleTransID;retain translated actor ID.
2. Operand1 via evtGetValue,convert the signed32 result to binary32 X using
   the inspected integer-bias/fsubs sequence.
3. Operand2 via evtGetValue;**discard its returned Y value**.
4. Operand3 via evtGetValue,convert signed32 result to binary32 Z.
5. Capture operand4 raw as output destination,then resolve it via evtGetValue
   as signed32 requested count N.
6. Load current shared BattleWork,fetch U for the translated actor,and request
   BtlUnit_GetPos(U,&currentX,&currentY,&currentZ).
7. Call distABf(currentX,currentZ,X,Z),using the visible X/Z values and the
   integer-converted destinations. Current Y was read but is not a distance input.

No operand uses evtGetFloat here. Even ignored operand2 must be resolved in
order,because resolver reads/callback boundaries cannot be deleted from the
contract. Unit and work pointers are not locally null-checked. BtlUnit_GetPos's
ordered load/write semantics are in the [generic jump services](BATTLE_JUMP_EVENT_CALLBACKS.md).
This callback does not store unit position,target position,speed or remaining
integration count;its sole direct output is the event destination write below.

## Distance and count algorithm

Let D be the binary32 return from distABf. The helper first forms binary32
Z-difference squared plus a **fused** X-difference square/add. Its positive
distance path uses reciprocal-square-root estimate and three double-precision
refinement steps,then rounds the distance to binary32. The inspected full
helper body is fingerprinted,but original frsqrte/FPSCR execution and exceptional
input behavior remain numeric dependencies;do not claim host sqrt equality from
the helper name. The parameter callback uses its returned D exactly as follows:

1. If requested N!=0,set candidate C=N. Otherwise compute binary32 D/U170
   (current horizontal speed),then fctiwz truncation to signed32 C.
2. Compare D to exact binary32 **60** (0x42700000). Only ordered D>60 enters
   correction:binary32 subtraction D-60,then binary32 multiplication by
   **0x3e19999a** (=0.15000000596046448),then fctiwz to integer E;
   C=wrapped32(C+E). These are separate rounded subtraction/multiplication,
   not a fused expression or a decimal-exact0.15 constant.
3. If requested N==0 or signed C>N,replace N with C. Otherwise retain the
   original N. Thus a nonzero requested value is a signed lower bound against
   the candidate after correction;it is not an absolute fixed motion duration.
4. If resulting N equals resolved sentinel **0xf1194d80** (-250000000),omit
   the output write. Otherwise evtSetValue(event,rawOperand4,N). Return2.

The sentinel test is on the **resulting count**,not on raw output operand4.
No count positivity clamp,minimum1,maximum,counter reset,RNG or AC option write
occurs. Zero speed and exceptional/out-of-range conversion outcomes require
the original floating/convert contract;do not add an invented divide-zero
success result. Signed wrap matters before the C>N test. A wrapped candidate
below a nonzero requested N leaves the original count in place.

Original register initialization sets r2=0x80427440;therefore the constant loads
r2-0x516c/r2-0x5170 bind60 at0x804222d4 and0.15 at0x804222d0. The signed
integer conversion bias is0x4330000080000000 at0x802ee608. The verifier pins
these addresses and the startup instructions rather than guessing the r2 base.

## Concrete ordinary caller consequences

Both parent paths request N=36. For finite D<=60,the callback writes36 to L6.
For a representative exact D=100,the correction uses40*binary32(0.15),
truncates to6 and writes42. These are **derived local cases**,not an original
scene observation or measured Mario jump time. For N=0,D=100,speed5,the
derived candidate starts20 and becomes26.

The parent then passes updated L6 to MarioJumpPosition. In the subsequent-pass
branch,the attached stylish brother is created **after** JumpParam,so it copies
the updated count. Its formula yields ACRStart(-2,N/2-10,N/2+2,N-4,0) after
WAIT3,using original integer division/order. The earlier8/20/32 example is valid
only if N remains36;N42 derives11/23/38. Do not freeze those window operands
while otherwise implementing distance-adjusted motion.

The final return stylish block sets32 directly and uses generic JumpPosition;
it does not call this MarioJumpParam service. Keep that caller separate.
The subsequent [MarioJumpPosition contract](BATTLE_MARIO_JUMP_MOTION.md) now
closes its local mode0/3/remaining-count/landing behavior separately. Its numerical
helpers/audio/domain and original execution remain dependencies;none of those
are inferred from parameter preparation or generic JumpPosition.

## Scoped requirements and verification

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| MJP-PARAM-01 | Preserve signed integer X/Z resolution,ignored Y resolution and service order | Y resolves but does not change D;GetPos uses visible coordinates;output operand stays raw |
| MJP-PARAM-02 | Preserve binary32 correction,word addition and upper comparison | N36,D60 retains36;N36,D100 yields42;N0,D100,speed5 yields26;wrapped candidate below requested leaves requested intact |
| MJP-PARAM-03 | Preserve destination/sentinel policy and absence of unit mutation | Result-250000000 skips output;other counts write through evtSetValue;no position/speed/remaining-count/AC state field write |
| MJP-PARAM-04 | Bind updated count into parent motion and copied stylish locals | Parameter42 reaches MarioJumpPosition and subsequent brother11/23/38;return stylish32 is independent |

tools/verify_jump_event_flow.py now additionally checks this complete body,
distance-helper fingerprint,19 instruction anchors and three numeric constants.
Whole distance-helper fingerprint is an identity anchor,not complete original
numeric execution evidence. Inspection outputs stay under /tmp or ignored
artifacts;ISO reads are read-only. Original caller position/domain,conversion/
FPSCR and parameter/motion/input traces remain necessary before asserting
faithful ordinary or tutorial impact timing.
