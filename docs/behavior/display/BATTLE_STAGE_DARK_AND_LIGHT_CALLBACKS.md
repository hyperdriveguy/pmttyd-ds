# Original stage dark and light display callbacks

Source: /home/carson/games/roms/G8ME01.iso opened rb; US G8ME01 disc0 revision0. Complete disassemblies and original constant evidence stay private under artifacts/stage-callback-evidence. This contract extends [stage registration](BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md). This is static source recovery, no original runtime/raster/paired-single equivalence claim.

## Dark callback80138da8/208

1. Capture B from global8041eb90 (r13+1c70 using verified init r13=8041cf20). Retain B+10000 in r30, call camGetPtr8000f2fc(4), retain camera pointer. Original queue callback arguments are unused.
2. Unconditionally issue GX setup below. No flag, mode, ending, alpha or active-entry early gate.
3. SetZMode(1,6,0), then DarkBySquare(alpha=U8[B+16f04]=U8[W+b08], x0=-640,y0=640,x1=640,y1=-640,z=105).
4. SetZMode(1,1,0), then DarkBySquare(alpha=FRESH U8[W+b08], x0=640,y0=640,x1=-640,y1=-640,z=105).
5. SetZMode(1,1,0), then DarkBySquare(alpha=U8[B+16ee4]=U8[W+ae8], bounds matching step3).
6. Return; no restore and no source Work writes. Camera matrix captured original pointer is used during setup before quads. W=B+163fc. Initializer sets W+b08=140,W+ae8=0, so fresh stage definitely has real dark geometry.

DarkBySquare80138fb0/ac calls GXBegin802b5a6c(0x80,0,4) then four DarkByPoint8013905c calls: (x0,y0,z),(x1,y0,z),(x1,y1,z),(x0,y1,z). Each point writes XYZ f32 then RGBA U8(0,0,0,alpha) to FIFO. No zero-alpha skip. First/second quad orientation differs and both keep original cull/depth setup; do not merge them solely because bounds coincide.

## Shared GPU prefix (in exact order)

Dark and Light801389a0/408:

- GXSetCullMode802b5c8c(2)
- GXSetZCompLoc802b940c(1)
- GXSetAlphaCompare802b8d60(7,0,0,7,0)
- GXSetBlendMode802b932c: Dark(1,4,5,5), Light(1,2,1,5)
- Light ONLY: GXSetZMode802b93d8(1,3,0) here.
- GXClearVtxDesc802b4520()
- GXSetVtxDesc802b3c08(9,1), then(11,1)
- GXSetVtxAttrFmt802b4558(0,9,1,4,0), then(0,11,1,5,0)
- GXSetNumChans802b6f3c(1)
- GXSetChanCtrl802b6f78: Dark(4,0,1,1,0,2,2), Light(2,0,1,1,0,2,2)
- GXSetNumTexGens802b5084(0)
- Dark ONLY: GXLoadPosMtxImm802ba2fc(camera+11c,0) here.
- GXSetNumTevStages802b8fcc(1)
- GXSetTevColorOp802b89e8(0,0,0,0,1,0)
- GXSetTevColorIn802b8960(0,15,15,15,10)
- GXSetTevAlphaOp802b8a50(0,0,0,0,1,0)
- GXSetTevAlphaIn802b89a4: Dark(0,7,7,7,5), Light(0,7,7,7,7)
- GXSetTevOrder802b8e30: Dark(0,255,255,4), Light(0,0,0,4)
- Light ONLY: GXLoadPosMtxImm802ba2fc(camera+11c,0) here.

No texture/TPL/image resource lookup, texture coord vertex attributes, lighting object/resource acquisition or memory allocations occur in either body. Texture binding inherited by Light order0,0 cannot simply be treated as a new owned image request; active color/alpha inputs consume raster/zero in this local program.

## Light local active-entry loop

Capture B once, retain W=B+163fc. camGetPtr(4) and GPU prefix ALWAYS execute even with all inactive entries. Loop exactly12 indicesi0..11. Let R=W+i*e8 (original real entryE=R+4). Test WORD[R+4]&1; clear advances without per-entry data/GPU emissions. Set flag does following, no additional early gate:

- Axis[R+88..90]-[R+58..60], component single subtractions. Temporary literal vector copied from802f37a8 (threezero words) is overwritten by those3differences before use.
- Read widthFLOAT[R+40]. Copy original vectorFLOAT[R+b8..c0].
- Center=singleFMA(2, end-start, start) per component, where start=[R+58..60],end=[R+88..90]. Source2 literal80422b20.
- At the start of EACH segment, previous=center+currentVector*width (singleFMA each component), using that segment's freshly reread center and the unrotated local vector. It is recomputed, not simply the preceding triangle's endpoint when center changes.
- For each18segments: recompute start/end/differences and center from live retained R; call PSMTXRotAxisRad8029e1c0(localMatrix,axis,.3490658402442932f literal80422b24), then PSMTXMultVec8029e6a4(localMatrix,vector,vector), then PSVECNormalize8029ebe4(vector,vector).
- Current=center+normalizedRotatedVector*width via source singleFMA. Read RGB U8[R+28..2a] for this segment, alpha0.
- GXBegin(0x90,0,3), FIFO vertices start,previous,current with same RGBA(RGB,0) each. The normalized rotated vector becomes next segment's local vector; previous is recomputed with fresh center before the next rotation. Exactly18 triangles/54vertices per active entry.
- The axis and width are initialized once per active entry before segment loop; start/end/center are reread per segment, as disassembly shows. Only local scratch vector evolves. Calls may expose external mutations, so do not snapshot every R field once as a claimed exact continuation.

Source R offsets above avoid inventing unknown semantic field names; for E-relative offsets subtract4 (flagE0,widthE3c,startE54,endE84,vectorEb4,RGBE24). No persistent Work stores in callback. Native current original initializer clears all12 Eflags; proving they remain inactive requires actual live storage/writer evidence, not guessed emptiness. Even proven inactive only removes triangle loop, not GPU setup. Full active numeric rotation/normalize and raster behavior remain explicit provider work.

## Native binding implications

- Existing original packed stage owner W and camera4 projection/view owner are inputs. No new original art imports required for these two local bodies.
- Implement retained ordered requests and real direct colored-vertex backend; indexed animation-fan backend is not itself an appropriate GXBegin direct descriptor protocol.
- Shared GPU state must record blend/depth/cull/alpha/channel/TEV/descriptor changes because original leaves them applied for subsequent queue callbacks.
- Dark first original depthcompare6=GEQUAL and following1=LESS must use established GameCube→PICA depth convention; do not numerically assume Citro3D enum equality. Dark blend factors4/5 mean source-alpha/inverse-source-alpha. Light factors2/1 mean source-color/one, distinct from ordinary alpha blend.
- Exact frame visual parity remains unverified. Native Black ending fade may conceal actual geometry; successful GPU telemetry/queue advancement can prove service progress but cannot alone prove source image parity.

## Original body identity

| Function | Address | Bytes | SHA256 |
| --- | --- | --- | --- |
| BattleStageDispDark | 0x80138da8 | 520 | 2713efc4c601346a10ce963e5479b3cd5d677fa5827a91b533cc071f7e40187e |
| BattleStageDispLight | 0x801389a0 | 1032 | 07bec02c118d5ec0e2152b071e9ac39c9b0f70e41dcfc9bd792667a48b6f7cf9 |
| DarkBySquare | 0x80138fb0 | 172 | 3ccdbe1adc7affa5c778adccb3c2251a7dc415be6d82882f86fdfc5c0f2ed16e |
| DarkByPoint | 0x8013905c | 40 | dbac5394311ca34bd858295de790ed45e12b43755faff7eec373429fdfeb4a6a |
