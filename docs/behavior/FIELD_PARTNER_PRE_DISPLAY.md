# Partner pre-display: marker consumption,animation update and draw registration

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyPreDisp0x800b7a98,size0x318. The used
[driver](FIELD_PARTY_DRIVER_DISPATCH.md) calls it at800bbf10 on the early
shadow route and800bc550 on the common tail. Input P is r3. Full display/animation/
camera queue lifetime and original visuals remain open.

## Facing gate and shared name markers

If P+4 mask0x10 clear AND P+0 mask0x200 clear,request
[partyMakeDispDir](FIELD_PARTNER_DISPLAY_FACING.md)(P). Otherwise skip that
request;do not rewrite facing/target fields locally as a replacement.

If P+8 has EITHER0x2000 or0x4000:

1. If0x2000 set,clear it. Reread P+8;then if0x4000 set,clear it too.
2. Request animPoseSetAnim(current P+c,current P+18,1)0x80041170 ALWAYS for
   this combined-marker path,even when only the PAPER marker caused admission.
3. Reread paper name P+1c. Nonnull requests
   animPoseSetPaperAnim(current P+c,paperName,1)0x80040ca4;null skips it.
4. Load UNSIGNED halfword P+20,convert to float using original unsigned double
   bias0x802cb348,and request animPoseSetLocalTime(current P+c,floatFrame)
   0x8004130c. No local increment or independent paper-frame counter occurs.

No markers skips those name/local-time requests. In BOTH cases request
animPoseMain(current P+c)0x8003ed2c. The flag/name producer contracts are
[name/selector requests](FIELD_PARTNER_POSE_REQUEST_SELECTION.md) and
[paper wrappers](FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md). Clearing markers
before callbacks and rereading names/IDs after them are observable order.
Underlying resource/animation/callback/frame/FPSCR behavior remains open.

## Post-animation rendering gate and depth request

After animPoseMain,reread P+4 mask1. SET returns immediately,skipping display
registration AND all trailing/shadow work. Animation/facing/marker work above
already happened. This is not an early skip of all presentation processing.

Otherwise copy current root XYZ raw words to a scratch vector. Request
camGetPtr(4)0x8000f2fc;pass its matrix at+15c and the scratch as BOTH input and
output to PSMTX44MultVec0x8029eb08. Read returned scratchZ and compute one
single FMA5000*Z+5000(word0x459c4000).
Ordered below0 clamps0;otherwise ordered above10000 clamps10000. Unordered
retains the result. Multiply by-1 with fmuls to form display sort depth.
This is literal matrix/output arithmetic,not measured view distance/near-plane
admission or a closed camera projection implementation.

Read SIGNED halfword P+2a. If positive AND its bit1(mask2) set,skip display
registration. Positive with bit1 clear,zero or negative registers:

dispEntry(current P+164,1,partyDisp0x800b7690,P,f1=sortDepth)
at800b7bfc or800b7c18. The callback receives P as its SECOND integer argument;
registration is not direct drawing. Its
[complete local transform/draw requests](FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md)
are separate from render queue/frame/camera lifetime.

Whether display registration occurred or its positive-halfword bit test skipped,
continue into [trailing/shadow work](FIELD_PARTNER_VIVIAN_PRESENTATION_AND_SHADOWS.md).
Only mask1's earlier return suppresses that whole tail. No frame-based timer or
ordinary unconditional blink policy is invented from these predicates.

## Verification

`python3 tools/verify_field_partner_predisplay.py` checks eight complete local
body hashes,515 instructions,32 constants,seven spans,two US lecture imports
and three direct pre-display/facing sites across seven DOL text sections.
Those sites bind actual driver calls and the facing helper. Callback construction
anchors pin both registration paths. Reports/ELFs/listings remain ignored.
Animation/display/camera/matrix/resource/source/alias/FP domains and original
presentation traces remain open after static checks and host tests pass.
