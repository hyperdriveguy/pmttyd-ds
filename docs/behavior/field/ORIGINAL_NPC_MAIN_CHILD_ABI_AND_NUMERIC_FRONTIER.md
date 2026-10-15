# Original NPC Main child ABI and numeric frontier

Evidence: artifacts/npc-main-parent-evidence/npcMain.txt, body SHA256 decc7aa26c3e7990624bbb9204ed0ce286ecabc49405d3140390a301e59a6b7f.
A=current packed NPC; GP is the separately fresh global pointer; F=retained tail field pointer; E=F+550. Offsets hexadecimal. Pointer scratch must remain addressable across suspension and provider output writes. Results ignored means parent consumes no return value; provider side effects still matter.

## Complete 39-child input/output table

| Child / first call anchor | Inputs | Consumed outputs |
|---|---|---|
| marioGetPtr 49ad8 | no arguments | r3 real Mario pointer; entry writes collision+1dc=0; later repeated calls separately capture different Mario owners |
| animPoseGetAnimPosePtr 49b20/38 | r3=A104 handle | r3 actual pose pointer; parent reads/writes pose word0 bit100 |
| __div2i 49be4 | r3:r4 signed64 numerator high:low; r5:r6 signed64 divisor=0:Dms | r3:r4 quotient for signed64 >500 comparison; four later calls numerator source<<3, divisor0:Dus |
| __cvt_sll_flt 49cc0 | r3:r4 quotient from immediately preceding div | f1 binary32 through rounded binary64; divide by1000000 and store1a0/180/1a4/1a8 |
| strcmp 49e04 | r3=fresh GP+12c; r4=802c1394 kpa_03 | r3==0 |
| psndSFXOn_3D 49e48 | r3=802c139c SFX_KUPPA_3RD_WATER_FALL1; r4=A+8c | ignored |
| npcHitCheckSide 49f20/5c | r3=A; f1/f2 retained dx/dz; r4=&scratchX stack20,r5=&scratchZ stack24 initially current X/Z; r6=A+1ac | first r3 contact stored2fc then freshly reloaded; scratch X/Z outputs. Second f1/f2=first output minus fresh current, r4=A+8c,r5=A+94,r6=A+1ac; return discarded |
| npcMoveG 49f84 | r3=A (parent already zeroed1b4 then1bc) | ignored; child mutates A |
| npcCheckHitMarioSide 49fa8 | r3=A | ignored |
| PSVECSubtract 49fb8 | r3=A+8c,r4=A+98,r5=stack+c4 | output XYZ. At4a520 r3=A+98,r4=A+8c,r5=stack+b8; at4a9d4 same inputs,r5=stack+ac (this last output is unused by parent but call must remain) |
| animPoseSetAnim 4a0f0/12c/154 | r3=A104; r4=descriptor+10/+18/+1c string pointer; r5=1 | ignored; parent writes phase108 AFTER child returns |
| evtGetPtrID 4a16c | r3=A11c | r3 event pointer; read byte8 stopped2 then byte10 type |
| cloudGetBreathPower 4a1f4/250/2a8 | r3=&separate scratch vec stack60/6c/78; f1=fresh A14c width | f1>0; each next point freshly reloads actor XYZ/height after prior call. MidY=fnmsubs(height,.5,currentY), bottomY=fsubs(currentY,height) |
| seqGetSeq 4a678/aa40 | no arguments | r3==2; retained Mario from prior marioGetPtr remains owner of later halfword2e read |
| vivianGetStatus 4a6a0/6ac/aa68/aa74 | no arguments | first r3==0 passes; otherwise separate second request r3==1 passes |
| angleABf 4a814/aad0 | f1=0,f2=0,f3=retained Mario-NPC deltaX,f4=retained deltaZ | f1 angle |
| compAngle 4a824/aae0 | f1=retained facing; f2=angleABf result | f1 fabs, compare to freshly loaded half-angle |
| kpaGetLevel 4a9a4 | no arguments | r3 index into FOUR raw words copied from802c1210 to scratch9c..a8 BEFORE child. No bounds clamp. Radius is read from retained local table after child |
| reviseAngle 4a9dc/aee8 | f1=fresh A144 yaw | f1<180 chooses90 else270. Proximity retains this facing; shadow passes selected literal to sinfd |
| npcCheckInterrupt 4acc8 | r3=A | ignored |
| camGetPtr 4acdc/acf4 | r3=4 | first r3+11c view pointer; separately fresh second r3+15c projection pointer |
| PSMTXMultVec 4acec | r3=view,r4=A+8c,r5=stack+c4 | scratch projected XYZ; source paired dot grouping |
| PSMTX44MultVec 4ad04 | r3=projection,r4=stack+c4,r5=SAME stack+c4 | in-place divided XYZ via retained reciprocal, not raw clip coords |
| animPoseSetMaterialFlagOn 4ada8 | r3=fresh A104,r4=40 | ignored |
| animPoseSetMaterialEvtColor 4adbc | r3=fresh A104,r4=stack28 COLOR WORD freshly copied from A114 AFTER flag-on child | ignored; four-byte scratch, not actor+114 alias |
| animPoseSetMaterialFlagOff 4adcc | r3=A104,r4=40 | ignored |
| dispEntry 4ae50 etc | r3=camera id,r4=class,r5=retained callback identity,r6=A or E,f1=retained depth | ignored; main camera is freshly reloaded A1f4 for EACH of3 opaque queues; alternate camera1; effect camera9/class2/depth+0 |
| sinfd 4af04 | f1=90 or270 selected from reviseAngle | f1 factor retained for shadow X offset |
| shadowEntry 4af7c/b040 | f1/f2/f3 copied XYZ,f4 radius; ONLY4 float args | r3 token retained directly for optional shadowSetCollision; ordinary radius R(1.33*R(A154*R(R(width/3)*scaleX))); Vivian omits1.33 |
| shadowSetCollision 4af90/b054 | r3=shadow token,r4=0 | ignored |
| hitCheckFilter 4aff8 | r3=0; f1=rootX,f2=R(rootY+1),f3=rootZ,f4=0,f5=-1,f6=0; r4=stack44,r5=stack40,r6=stack3c,r7=stack38,r8=stack34,r9=stack30,r10=stack2c | r3 nonzero => read output stack40 hitY into Vivian scratchY. stack38 initialized100; others uninitialized outputs. Keep 7 individually addressable float slots |
| animPoseVivianMain 4b014 | r3=fresh A104,r4=&scratchVec stack84 | ignored; original-root shadow is freshly loaded afterward, not scratch hitY |
| intplGetValue 4b22c/b268 | r3=5,r4=incremented count,r5=20,f1=-600,f2=0; second r3=1,r4=count-80,r5=20,f1=0,f2=+600 | f1 stored E10; original8041fce4=-600 (c4160000),8041fce8=+600 (44160000); earlier60 documentation corrected |
| fbatEncountCheck 4b31c | no args | ignored; GP then freshly reloaded to capture F2 for remaining score/fire tail |
| kpaJumpChk 4b344 | no args | r3 int retained jump result; called only first retained Mario+3c read==2 |
| kpaFireAttackCheck 4b358 | no args | r3 int retained fire result; second FRESH retained Mario+3c read==2 controls call independently of first |
| kpaAddScorePos 4b3ac | r3=F2+57c value loaded before eligibility check; r4=&scratchXYZ stack48, Mario root fresh words and SP Y+60 | ignored; then clear F2+578 then57c |
| fbatTalkMode 4b3d0 | no args | ignored |
| fbatBattleMode 4b3d8 | no args | ignored |

## Scratch lifetime and comparison subtleties

- Separate persistent scratch groups: side20/24; color28; hit outputs2c..44; score48..50; breath base54..5c and individual60..80; Vivian84..8c; ordinary shadow90..98; copied Kpa table9c..a8; otherwise-unused subtractac..b4; movementb8..c0; display/auto-posec4..cc. A source-address scratch arena makes outputs and in-place aliases explicit; zero-filling previously uninitialized hit outputs is an adaptation, not source behavior.
- 49e18 branches out only on Y>365: NaN Y enters special state! Do not rewrite as Y<=365, which rejects NaN.
- Float branch complements matter. dt 'not greater than0' skips movement, including NaN. Simple outside checks test strictly < or >, so unordered usually does not trigger failure. Culling explicitly requires ordered >/< and >=/<= so NaNs fail ordinary cull. Main40 can force those NaN projected values through material and depth.
- 4addc depth uses SINGLE fused fmadds(5000,z,5000). Clamp sequence is if(depth<0)0 else if(depth>10000)10000, then fmuls by-1. NaN survives both clamps; +0 becomes-0. std::clamp/min/max do not automatically provide matching NaN operand semantics.
- Autopose: NaN speed is neither <.1 nor <descriptorThreshold and selects run. NaN phase threshold similarly selects run if speed not idle. No descriptor-null guard exists in source auto pose.
- T2 quarter gate is if(distance>quarter) execute other tests; unordered skips range/height/angle tests and leaves avoidance success. T1/4 quarter gate similarly bypasses range/angle but NOT later vertical. Direction rejection uses abs(compAngle)>halfWidth, so NaN angle does not reject. Inner circle compare rejects only strict >, so NaN remains admitted; rectangle strict outside checks also preserve admission on NaN.
- State1/2 effect count increment is wrapping word, subsequent comparisons SIGNED. E+4 completion clear does not clear E+0. State2 start interpolation is-600, not60/+600; second ends+600.
- Tail state is retained F signed halfword read once; F+54d clear always uses retained F, even after services replace GP168. New F2 captured only after fbatEncountCheck is separately retained across jump/fire/score callbacks. Mario pointer retained across both type reads; don't call marioGetPtr again in between.
- The source's loop bound is signed cmpw(index,count); index starts0. Negative counts do no actors. MainCount increment wraps32 and blur history parity reads global fresh after all display children.

No shared implementation/docs edited. This audit is not a claim of implementing or validating the transitive providers.

## Follow-up exact clock capture order

49b94: capture GP; actor.low19c; GP.low3c; GP.high38; actor.high198. Subtract wrapped64 and store low194 then high190. Read bus only after stores; read delta high190 then low194 and divide by0:Dms. Retain Dms (r24) across call, signed quotient>500 replaces low194 withDms<<4 then high190=0.

Fresh mainflags0 controls accumulated update. Read accumulated.low17c,delta.low194,accumulated.high178,delta.high190; add wrapped64; store low17c then high178. Else clear17c then178. Elapsed always reads low18c,delta.low194,high188,delta.high190, then stores low18c and high188. FreshGP read then GP.high38,GP.low3c; store anchor low19c then high198.

Each conversion separately reads bus, source high, source low. Wrapped leftshift3, divisor0:Dus; div returns high:low directly into cvt. After conversion read1000000 literal then fdivs/store. Next bus/source reads only after preceding publication. No GP reload inside4 conversions. Stopped branch49dac freshGP, high38 thenlow3c, lowstore19c thenhigh198; all elapsed/delta/float fields remain untouched.

Extracted numeric bodies __div2i80264ad8 and __cvt_sll_flt80264e70 in this artifact directory. Parent denominators are nonnegative; max for32bit bus Dms1073741,Dus8590, so signed INT64_MIN/-1 overflow is impossible. Zero divisor has no safe native C++ quotient equivalence; do not execute undefined C++ division or invent saturation. Source leftshift/accumulations are bitwise wrapping64, potentially yielding negative signed values.

__cvt_sll_flt first builds nearest-even binary64 integer conversion (discarded11bits4ecc..ef0), then lfd+frsp4f14/18. An explicit int64-to-double-to-float reproduces source double-rounding order. Counterexample to direct int64-to-float: N=2^54+2^30+1. Source double rounds to midpoint2^54+2^30, then float ties-even down2^54; direct conversion rounds up2^54+2^31.

This static audit supports the [clock subsection](NATIVE_PACKED_NPC_CLOCK_SECTION.md)
and [complete parent graph](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md).
It does not complete the39 native child providers or full Main integration.

Original helper bodies inspected read-only:

- __cvt_sll_flt at80264e70, 180 bytes, SHA25669207aa0a8e4377cd39df889250f0a6e3366c4ad873f80e607d8458641906a26.

Original helper bodies inspected read-only:

- __cvt_sll_flt at80264e70, 180 bytes, SHA25669207aa0a8e4377cd39df889250f0a6e3366c4ad873f80e607d8458641906a26.
- __div2i at80264ad8, 312 bytes, SHA256b8acc70386f7831b292e561c133ce6abb4a2881bb22e97780f09d2e0bbeee660.

### 2026-10-14 Tail interpolation sign correction

Read-only instruction/constant audit pins first -600->0 and second0->+600;
[history/traversal evidence](NATIVE_NPC_MAIN_HISTORY_AND_TRAVERSAL.md) records
addresses/hashes. Earlier prose reversed both signs. Field tail remains open.
