# Disc thread system-message routing and exact local rendering requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete admitted presentation region within gcDvdCheckThread8006f1e0/224,
romFontGetMessage80251858/54 and romFontInit802517f8/60. Font width/draw/make,
GX projection,SDK matrix and render/OS internals are REQUEST boundaries here,
not complete callees merely because their caller is recovered. Verify:
`python3 tools/verify_event_readiness_gate.py` pins body/data/resource evidence.
Read [gate/state admission](EVENT_DISC_THREAD_READINESS_GATE.md),
[outer update](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) and
[physical input](../input/PHYSICAL_INPUT_PRODUCER.md). Static original DOL evidence;
no measured message display,text width,color/font fidelity or original trace.

## Exact admitted request order

Gate thread admits only live GP10!=0 and retained count>10. Then:

1. DEMOBeforeRender8002ec1c.
2. If retained word V==0,skip text setup/draw but keep later requests.
3. Otherwise romFontGetMessage(V);REPLACE retained V with returned pointer.
4. romFontGetWidth(V),using returned pointer still in r3. Interpret returned
   word as signed integer;convert through double bias0x4330000080000000 at
   802c2a80,`fsubs` SINGLE then `fctiwz`;retain resulting word W. This conversion
   can alter very large integer widths;no local width validation/clamp.
5. C_MTXPerspective(output at stack+c,fovy25.0,aspect binary32 0x3fa22222,
   near1.0,far10000.0),then GXSetProjection(matrix,type0).
6. Compute x=singleMultiply(singleConvert(S32(wrap32(-W))),0.5). Store color
   word0xffffffff at stack+8. Call romFontPrintGX(&color,V,x,y60.0,scale1.0),
   with floating varargs condition bit6 SET.
7. DEMODoneRender8002eb38;makeKey80005ff8;L_gcResetCheck8006edbc.
8. Return to the thread's unconditional OSYieldThread and next iteration.

Scalar helper returns are ignored except message pointer/width. No previous
projection restoration or local GX state save exists. The input/reset requests
happen even when V0 skips text. This is a separate error-mode input path while
normal marioStMain is waiting on GP10. Native implementations need explicit
media-status/thread/render/input adapters rather than guessing all work stops.

## Complete message table routing

Load font-config pointer from8041df28=r13+1008;read its word+8. If nonzero,
read current GP.word16c language. If language==0,choose table slot1 directly.
Otherwise (config8==0 OR language!=0),read CURRENT GP16c again and use that
word as table index. No language clamp/normalization or config pointer-null gate.
Language can be reread independently on this route.

Table-pointer array is8041df2c=r13+100c,with six INITIAL DOL entries:
803b9340,803b9394,803b93e8,803b943c,803b9490,803b94e4. Read selected table
pointer,then read message pointer at table+wrap32(selector<<2). No selector
range/null/encoding check or allocation. This is a direct resource pointer,
not a story msg/US ID lookup. The original US startup stores GP16c=1;therefore
slot1 is the US-default table under that local initialization,with later language
writers/aliases remaining open. Do not silently substitute current locale text.

romFontInit clears config words0 and4,requests OSGetFontEncode80298b08,uses its
LOW16 result:exact1→config8=0,every other value→config8=1. Then romFontMake
802514a8. It returns without a scalar contract. Actual console-ROM font texture,
encoding/glyph construction,OS source and GX draw behavior are outside this
local routing contract;reuse of a bitmap font cannot be claimed extracted from
the ISO merely because its system-message strings live in the DOL.

## US-default codes10..13:reusable original data locations

The verifier fingerprints each complete NUL-terminated resource below;no
extracted message body is committed. Paths are read-only ISO ranges with DOL
virtual addresses,not filesystem message files. Length includes terminator;
retain original bytes/newlines and language-table association on extraction.
These are system error notices,not story dialogue. Project US story/message
fidelity and adapted-prompt presentation constraints remain unchanged.

| Code | Virtual address | ISO offset | Bytes including NUL | SHA256 |
| --- | --- | --- | --- | --- |
| 10 | 803019a0 | 31eca0 | 87 | c0cbf73c27ced1347f17833b0fc947342e0fd29d2ef77debb8f352eb147f6b7d |
| 11 | 803019f8 | 31ecf8 | 67 | c0306d3df8e3fdebf9c846e338d8f0efe2a20bd1e8c8b8ad44e2fbf7168e1ab3 |
| 12 | 80301a3c | 31ed3c | 109 | 74f65a824ac2ef30c25a8a50d5ac8cd758bb6452c7bf85fd5ca0327d16a895b4 |
| 13 | 80301aac | 31edac | 123 | 67296b685da4f3c790ec7ea1e889195b2f7a19f3564cd6d8aeb74d5e71521e8a |

These offsets apply to US G8ME01 disc0/revision0 only. Compiled/extracted assets
stay outside source history;licensing/conversion records belong in docs/ASSETS.md
when implementing extraction. No original system notice is paraphrased here.

V's pointer replacement is retained across loop iterations;status1 can preserve
it and the gate. The next admitted iteration would feed that pointer word as
selector. GetMessage adds no repair,so valid status/cadence/selector source domain
must be established. A host implementation that caches original selector
separately changes this local behavior. Normal mapped error statuses overwrite
V with10..13 again before each display,according to the producer contract.

Acceptance:count10/11 admission,V0 skip with rendering/input tail,V pointer
replacement,width conversion/negation/single rounding,exact projection/color/
position/scale/CR6,request ordering/no restore,config/font encoding/language
routes,US initial table association/four whole resources and stale V boundary.
ROM font/renderer/matrix/media/thread/selector/language/width/interrupt domains
and original message/recovery/input/audio traces remain EVT-01/platform work.
