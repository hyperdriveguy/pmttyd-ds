# Terminal TEV color stage, context capture and cosine alpha

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX calls executed or images observed.
`tools/verify_map_tev_terminal.py` checks the containing TEV identity as PARTIAL
semantics,255 reviewed words,eleven binary32 constants,five original spans,
two US lecture imports,six direct DOL sites and65536 host byte-product cases.
Two earlier depth branches and backend/source/runtime remain open.

Complete block800244ac..80024654 exclusive belongs to mapSetMaterialTev
80021b80/0x2dbc,after flag0x40000 and before0x800000. Retain F,model and running
K=TEV stages,B=texture maps,C=coordinate generators,I=indirect stages.
If F&0x80100000==0,skip. Otherwise append one untextured color stage.
This is an ANY-bit test,not a requirement that both admission bits are set.

## Two distinct color sources

If F&0x100000 SET,load context pointer word[8041e66c],then U8 context+0c,+0d,
+0e,+0f as R,G,B,A. Capture these four bytes before any stage GX calls;do not
reread them later. No context-null/valid-color or source initialization guard.

If F&0x100000 CLEAR (admitted via0x80000000),read GP=word[80415fd0],then raw
word t=GP+1c. Convert uint32 t to single q using double0x43300000:t minus
bias802bfc20=0x4330000000000000 with fsubs. Unlike
[base mode8](MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md),this branch does NOT double t.
Compute theta=R32(R32(0x3fc90fdb*q)/20),using1.5707963705062866 and0x41a00000.
Call double cos80271ea8,FRSP result s. Compute v=R32(64*R32(1-s)),then
z=F32(v,.5,64),using binary32 constants1/.5/64. FCTIWZ truncates z toward0;
A=low8(integer result). Set R=240,G=240,B=100.

R32/F32 specify original single-rounding/fused operation order,not host-libm or
GPU parity. Raw GP word units/writer/cadence remain open. No elapsed-time
adjustment,frame/tick gate,float clipping or semantic fade-name is supplied by
this body. If both admission bits set,the raw context path wins.

## Stage request order and KColor arithmetic

At stage K,request in order:

1. GXSetTevOrder(K,255,255,255).
2. GXSetTevColorOp(K,0,0,0,1,0).
3. GXSetTevAlphaOp(K,0,0,0,1,0).
4. GXSetTevColorIn(K,0,15,15,14).
5. GXSetTevAlphaIn(K,7,7,7,0).
6. GXSetTevSwapMode(K,0,0).

Then initialize local raw KColor from8041f8f4=000000ff. Replace only RGB:
Rout=floor(R*A/255),Gout=floor(G*A/255),Bout=floor(B*A/255).
Retain seed alpha255;**do not store source A as output alpha**. The byte products
use the original signed multiply-high magic0x80808081/add/arithmetic-shift7/
sign-correction/low8 sequence. The previously decoded bounded arithmetic proof
checks all65536 pairs;no original game execution is implied. This is truncation
of each byte product,not float premultiplication or divisor256.

GXSetTevKColor(1,&local),then GXSetTevKColorSel(K,13). No KAlphaSel request in
this block. Finally K++;B/C/I unchanged;fall through to0x800000 test80024654.
The local stage uses retained input register/selector state per the original
raw enums;actual GX packing/equation/quantization/images remain backend work.
No context/source/global color write or prior GX restoration occurs here.

Final callback/count publication is separately specified in
[the postlude contract](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md).
Actual US context/GP/flag writers and scene admission,SDK/libm/FPSCR/color/GX
lifetime/cache and original frame comparisons remain necessary for full
material/system fidelity. Neither static default RGB nor raw context pointer
constitutes a measured scene color/clock trace.
