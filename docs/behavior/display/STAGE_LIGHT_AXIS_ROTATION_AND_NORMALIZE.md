# Stage-light axis rotation and vector normalization

Read-only original US G8ME01 ISO; complete disassemblies and sdk-constants-and-bodies.json stay private under artifacts/stage-callback-evidence. No original instructions executed, no FPSCR/frsqrte/libm bit parity claimed. Existing contract camera/CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md already specifies Normalize's operation graph and no zero fallback; camera/OVERWORLD_CAMERA_ROAD_STATE.md identifies RotAxis as a real axis-normalizing owner. The following adds the internal rotation graph.

Notation S(expr)=source binary32 rounded scalar/paired lane result, F(a,b,c)=single fused multiply-add, N(a,b,c)=source single fnmsub = NEGATE the fused rounded result(a*b-c). Unary negation of that rounded result matters for zeros/NaN signs; replacing N with unrelated reassociated subtraction is not exact. frsqrte is original Gekko estimate (not exact inverse sqrt), scalar seed remains original register precision until each single instruction consumes it. Ordinary language `float seed=1/sqrtf(v)` is only an explicit adapter.

## Normalize8029ebe4/44

Input components x,y,z binary32, load pair(x,y) and pair(z,1) via W1 psq_l.

1. px=S(x*x), py=S(y*y).
2. zpx=F(z,z,px); len2=S(zpx+py). This is NOT S(px+py+pz) with independent pz.
3. seed=frsqrte(len2).
4. q=S(seed*seed).
5. h=S(seed*.5f), original .5 at80429394.
6. r=N(q,len2,3.f), original3 at80429398.
7. factor=S(r*h).
8. output=(S(x*factor),S(y*factor),S(z*factor)); XY stored before Z. Input=output alias supported by all source components loaded before output writes.

No zero, small-magnitude, Inf or NaN branch; no epsilon, substitute direction, identity or false return. A native finite-only rejection is an admission policy, not source behavior. Zero len2 runs through estimate/refinement and ordinarily produces nonfinite final vector; huge finite inputs can overflow len2 or seed² intermediate. A double-length safe normalize changes this original graph.

## Wrapper PSMTXRotAxisRad8029e1c0/70

Capture angle register, outputpointer and axispointer. Call original sinf80272888(angle) FIRST, retain return. Call original cosf802728ac(same captured angle) SECOND. Call internal8029e110(output,axis,sin,cos). No early angle0 return or null/degenerate axis gate. Internal rounds incoming cos and sin with frsp before arithmetic. Exact original libm implementation is a separate boundary; native sin/cos substitution is explicit adapter work.

## Internal8029e110/b0

Original .5=80429360 and3=80429364. Axis input loads pair(x,y), scalarz (scalarload mirrors z in paired lanes). Normalize axis by EXACT same steps1..8 above, with internal instruction order: roundcos,loadXY,roundsin,loadZ,XYproducts,two=S(.5+.5),zproducts/FMA,zero=S(.5-.5),len2,k=S(two-cos),seed,seed²,seed*.5,fnmsub,refinedfactor,then normalize XY and Z. Thus k computed before reciprocal estimate but independent. Zero is source+0 from .5-.5.

Let X,Y,Z be normalized axis components, C=roundedcos, T=roundedsin, K=S(1-C). Original separate products:

- XK=S(X*K), YK=S(Y*K), ZK=S(Z*K)
- XT=S(X*T), YT=S(Y*T)
- XXK=S(XK*X), XYK=S(XK*Y), YYK=S(YK*Y)
- XZK=S(XK*Z), YZK=S(YK*Z), ZZK=S(ZK*Z)

Matrix row-major3x4 (all displayed sums separately single, not fused):

```
m00=S(XXK+C)       m01=N(Z,T,XYK)    m02=S(XZK+YT)    m03=zero
m10=F(Z,T,XYK)     m11=S(C+YYK)      m12=S(-XT+YZK)   m13=zero
m20=S(XZK+(-YT))   m21=S(XT+YZK)     m22=S(ZZK+C)     m23=zero
```

Only m01 and m10 use fused cross term +/-Z*T; other cross terms use prior rounded products. Original paired stores order: (m02,m03), (m00,m01), (m10,m11), (m12,m13), (m20,m21), (m22,m23). There is no identity fallback for angle0 with zeroaxis; normalized NaN axis multiplied by zero still propagates nonfinite matrix values. Translation slots remain explicit sourcezero. Preserve axis input alias assumptions separately: output writes begin only after input axis loaded and all normalization completed.

The light callback then calls exact already-documented PSMTXMultVec8029e6a4, followed by Normalize above each of18iterations. Correct finite approximation should retain these graph boundaries rather than accumulating a single closed-form Rodrigues rotation across18segments.

## Existing packed stage ownership and live reads

Native original owner is EventNativeRuntime.memory via nativeBattleActorServices(runtime) in source/battle/battle_event_callbacks.cpp:121. Services.memory comes from runtime.memoryBinding.services(), byte callback invokes actual runtime.memory.readByte and asserts mapped. currentBattleAddress reads original global8041eb90; source callback captures it ONCE then W=B+163fc. Retain W across held child calls; do not recapture a new globalB for each alpha/flag unless original did.

source/battle/battle_stage_work.cpp original initializer memset(W,0,b3c), then explicit12 Eflags0, E+=e8 with E=W+4. Ambient darkness W+ae8=0, stage W+b08=140; ae4/b04 flags1 do NOT gate Dark callback. Main reads each live Eflags&1 and invokes activeEntry for true; inactive entries can own Ee4 scratch. Main counters can mutate live alpha W+ae8 and W+b08 through original interpolation after decrementing respective positive counters W+af0 and W+b10. Thus renderer must read actual current packed bytes, not hard-code initializer values. Original callback Dark reads W+b08 separately for its first TWO quads, then W+ae8; active light rereads12current Eflags as its loop advances.

TrioAttackRuntime::updateStageWork currently supplies activeEntry hold at80139150, so ordinary actual12inactive domain can be established from real bytes but active stage Main remains an honest later boundary. Current source-wide search finds direct ae8/b08 writes in initializer and Main interpolation; this is a native implementation census, not proof original event/REL/indirect writers are absent. Audience population also uses W+b34 etc, confirming W is shared stage state, not renderer-owned emptiness.

Native implementation in battle_stage_display.cpp substitutes float1/sqrt and native sin/cos while retaining these arithmetic boundaries. Nonfinite input/output and degenerate normalizations remain outside this native provider domain. No epsilon, substitute axis, identity or double-length rescue is used. This is not original frsqrte/libm/FPSCR/subnormal/NaN-payload equivalence.

## Original SDK body identity

| Function | Address | Bytes | SHA256 |
| --- | --- | --- | --- |
| PSMTXRotAxisRad | 0x8029e1c0 | 112 | 789894aeb53da26ce28254ec1478a0c6a266cac1418c10c269ee0af6a25271b4 |
| __PSMTXRotAxisRadInternal | 0x8029e110 | 176 | 659610b58bed54ac2a7f1b054f94c3ad0c7072598fa6453966b5d059309cf353 |
| PSVECNormalize | 0x8029ebe4 | 68 | 7fa0405d99623e73902a2542442048a5ea43816f98cab2e5c844dfa56b948088 |
