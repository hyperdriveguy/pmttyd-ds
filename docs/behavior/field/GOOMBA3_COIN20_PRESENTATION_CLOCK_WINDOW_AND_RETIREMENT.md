# Original Goomba3 coin20 presentation, clock, window and retirement

US G8ME01 revision0; read-only source recovery2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md). Complements
[quest2 caller](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md) and
[earlier service frontier](GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md).
Addresses, offsets and masks are hexadecimal; amounts/states/counts decimal.
Static source evidence, not original runtime observation or playback parity.

## Invocation, placement and first-call ownership

evt_sub_get_coin80054280/4f8 resolves amount using evtGetValue on argument0
EVERY invocation, including state100 grant. It captures current Mario root
XYZ8c/90/94 into two stackvectors, obtains camGetPtr(4),reads heading114 and
computes all placement before testing firstCall. It does not retain an amount
snapshot merely because firstCall displayed it. Finite quest2 argument20 is
literal and stable; mutable generic arguments remain service inputs.

Let F mean single-precision operation and FM mean source fused single op.
Original constants at8041fe40..78 are float words:

    40a00000 42c80000 40c90fdb 43b40000
    3bf95d4f 3e2a0903 3f800000 40490fdb
    3fc90fdb 4096cbe4 41200000 3d17c1be
    3efe4f76 42780000 42500000

Compute angle=reviseAngle(F(FM(100,heading,5)/100)), then
r=F(F(6.2831854820251465*angle)/360). reviseAngle80007328 uses original
fmod(doubleangle,double360),rounds tofloat,NaN→0; negative adds360 and caps
rounded>=360 to0. This is source polynomial placement, not ordinary libm
sin/cos replacing the authored instructions.

Let PI=3.1415927410125732,H=1.5707963705062866,
T=4.71238899230957. Define

    S(x): u=F(x*x); return F(FM(FM(.00761000020429492,u,-.1660500019788742),u,1)*x)
    C(x): u=F(x*x); return FM(FM(.037050001323223114,u,-.4966999888420105),u,1)

Here inner FM(a,u,-b) corresponds to fmsubs(a,u,b).
Source sector order and inclusive boundaries:

| r | sin polynomial | cos polynomial |
| --- | --- | --- |
| r<=H | S(r) | C(r) |
| H<r<=PI | S(F(H-F(r-H))) | -C(F(H-F(r-H))) |
| PI<r<T | -S(F(r-PI)) | -C(F(r-PI)) |
| r>=T | -S(F(H-F(r-T))) | C(F(H-F(r-T))) |

Compute dx=F(10*sin),dz=F(10*cos). Effect creation XYZ is
(F(MarioX+dx),F(MarioY+62),F(MarioZ-dz)); icon initialXYZ is
sameXZ withF(MarioY+52). Source arithmetic542e4..544a4 retains exact
operation order. Current camera heading and Mario root are sampled even
on later polls, but these new positions are only consumed by firstCall effect
entry and state0 icon position; no recurring icon/effect follow write appears.

firstCall resets event.work78=0, calls effItemGetEntry(type7,effectXYZ),
stores effectpointer7c,zeros80/84, allocates8 bytes with map allocator and
stores pointer88. No null/allocation fallback guards appear locally.
Subsequent switch reloads effect7c and parameter88. Owned event fields:

| Offset | Role |
| --- | --- |
| 78 | Service state |
| 7c | Effect entry pointer |
| 80 | Wait threshold milliseconds |
| 84 | Window entry ID |
| 88 | Allocated parameter pointer |
| 198/19c | Retained GP38/3c timestamp high/low |

## Exact finite states and audio/Mario requests

State0 creates named icon **iraiCoin** using descriptor403,sets itsXYZ,
ORs icon flags102, sets effectpayload+38=5, requests MarioGetItemMotion,
Entry/Open/SetParam(windowdefinition80415e84,param88),then writes param[0]
=resolvedamount,param[1]=0. It requests BGMOff3800,then BGMOn201 with literal
**BGM_FF_GET_ITEM1**,storeswait200,snapshotsGP38/3c,setsstate1,returns0.
It does not also execute state1 on this call.

State1 computes elapsed low32 rawticks as wrap32(GP3c-saved19c). Divisor is
unsignedinteger floor((hardwareword800000f8>>2)/1000), implemented with
mulhwu10624dd3 and>>6. elapsedMs=unsigneddivision(ticks,divisor); compare its
32-bit result **signed** against wait80. Strict elapsedMs>200 is required.
The retained highword198 is written but not read in either wait predicate.
On admission clear iconflag2,writeparam[1]=1,storewait500,newGP38/3c,
state2;return0. No fixed simulation/render-frame count follows from this clock.

State2 uses same exact predicate with strict>500, setsstate10 and returns0.
State10 reads keyGetButtonTrg(0); bit100(A) setsstate100;stillreturns0.
There is no B-trigger acknowledgment, window-action-ready predicate,
fanfare-vacancy wait or automatic grant here. Early A does not bypass waits;
state10 publication does not process A until another invocation.

State100 order at800546d4..724:

1. pouchAddCoin(current resolved amount).
2. effSoftDelete(retained effect),iconDelete(iraiCoin).
3. marioChgStayMotion.
4. BGMOn(a0,NULL),then BGMOff201.
5. winMgrCloseAutoDelete(window ID).
6. Write param[1]=0,state101,**return2 immediately**.

Coin callback does not call KeyOff/On,CtrlOff/On,SystemLevel,psndSet/ClearFlag80,
SFXOn or parameter revision. Those source services must not be imported from
item95's different callback. MarioGet/Stay preserve their request/post/pending
owners, not an instant pose override. The BGM order also differs from item95:
resume field before fading group1. Both wrapper durations are1000ms; On201
has no force-replace10 and preserves original backend reuse/admission.

State101, if explicitly invoked again: wait winMgrAction(window)==0;
when0 free param8bytes using map heap,return2;otherwise0. Other statevalues
fall through return2. Standard VM return2 completes USER_FUNC at state100;
there is no automatic callback call101 or local parameter-free at100. The
window retains the borrowed pointer, but param[1]=0 makes display return
before reading amount. Do not invent a VM continuation delay before the
quest caller's5405/5376/1420/story operations; any native resource retirement
that differs must state the ownership adaptation.

pouchAddCoin800d3e18 reads signed16 pouch78,adds amount,narrows16 FIRST,
then clamps negative0 and>999 to999. Finite20 is granted only atstate100.
Quest2 caller clears1420,sets5376 and prints sys_request_finish after callback
return2; callback does not perform these event writes.

## Original window definition and US text

Definition80415e84 raw10words:

    0 0 8 ffffff51 78 15e 2a e1d79bff 0 80054778

Sequence0,camera8,XY(-175,120),width350,height42,frameE1D79BFF,
mainNULL,displaycoingetDisp80054778/12c. Uses existing
[manager open/close ordering](../message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md),
including borrowed parameter and closeAutoDelete flag8. No separate help window.

coingetDisp obtains param fromwindow2c;param[1]==0 immediately returns.
GP16c==0 uses sprintf(template,amount);USselector1 copies original template
and calls unk_800d0a4c(buffer,amount). Exact msg/US/global.txt payload:

    msg_window_coin_get = You got <col c00000ff><NUM></col> coin<S>!

Finite US helper searches firstNUM,replaces its5-byte token with signed%d
format, then selector1 branch800d0b14 searches firstS. Amount exactly1 adds
no suffix; **any other amount**,including0/negative,adds original's' at804217fc.
Only first matching tokens are replaced; markup is preserved. Finite20 yields
`You got <col c00000ff>20</col> coins!`. No new congratulatory dialogue.

FontGetMessageWidthLine narrows returned widthlow16; expand windowwidth only
whenW+20>currentwidth,thenX=trunc(-newwidth/2). Height=definitionheight42+
24*reportedlinecount. FontDrawStart→FontDrawColor(original000000ff)→
FontGetMessageWidth(buffer)low16→FontDrawMessage(X=trunc(-W/2),Y=windowY-8).
Window flags/action and font/provider clocks remain separate owners;
param[1] does not depend on open-animation completion.

## Assets and bounded backend frontiers

Original icon.bin403 offset178a descriptor halfwords:

    1 0 0 4 414 6 415 6 416 6 417 6

This is the four-frame original icon.tpl414..417 sequence,perframe duration6;
retain original icon animation owner/clock rather than choosing one substitute
texture. Descriptor403 is not TPL403 or itemtable95's descriptor307.

Effect7 entry has one60bytepayload; initialsize table8041b8b0[7]=10 gives
payload14=single(.75*single(10/10))=.75;payload10 initially0,life1000,age0,
mode0,alpha255. Callerpayload38=5. Shared draw palette8041b888[7] is
**e85bd0ff**. The [type6 family bank/update/draw](../effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md)
pins shared bodies/assets42/44, but native type7 admission/render geometry
must explicitly retain kind7 and its source scale/color rather than relabel6.

Fanfare BGM row219 at80316878 is
802ce248/80000000/802ce25c/50400001: BGM_FF_GET_ITEM1,
pathsound/stream/ff_get4_32k,volume80,pan64,surround0,globalpercentagebypass1.
Original stream resource lookup/loop/retirement for this distinct file still
needs its own evidence; keyitem row217/fileff_get2 must not be substituted.
Shared wrapper/envelope semantics are [already recovered](../audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).

## Evidence and implementation boundary

Complete callback sourceSHA256
26923485bb7646cc25d0a8c917b85823961030c5e9ff4a6e3b0584d2f80194b2.
Fresh named extraction: evt_sub_get_coin,coingetDisp,reviseAngle,
unk_800d0a4c. Raw definitions/constants/switch/BGM row/icon descriptor and
original US message were reread from ISO. Ignored artifacts are in
artifacts/reference/goomba3-services/coin*. No original code executed.

Finite portable service is implementable with these explicit clock,position,
window,icon,effect,Mario,audio,pouch and resource-lifetime providers. Remaining
native dependencies: authored icon clock/rendering,type7 effect presentation,
row219 stream playback/loop completion,source window/font rendering and
safe borrowed-parameter retirement after event completion. Exact SDK/Gekko
polynomial parity and actual native/original/hardware cadence remain unverified.
No shared request/index/progress/code edits were made.
