# Partner vector ray wrapper and asymmetric quantization

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partyHitCheck0x800c464c,size0x280,called by
[support appearance](FIELD_PARTNER_SUPPORT_APPEARANCE.md) and
[out-of-screen recovery](FIELD_PARTNER_SCREEN_RECOVERY.md). This is a general
partner ray,not a floor-only service. Backend hitCheckVecFilter is fingerprinted
with selected result-layout evidence;its full traversal is separate.

Inputs are(P,origin*,direction*,pointOutput*,normalOutput*,lengthInOut*),in
r3..r8. Copy origin/direction words and input length into local query Q at
stack+14,using origin Q+c,direction Q+18,length Q+3c. The origin/direction
buffers are input-only;writes to the point output do not implicitly update
origin. There are no output-pointer null checks and not every Q header/context
field is explicitly initialized by this caller.

Choose the callback by fresh yoshiGetStatus:nonzero global status selects
chkfilterYoshi;otherwise unsigned P kind2 with P+0&0x100 selects
chkfilterNokotaro;otherwise chkfilter. Those complete local
[filter/status leaves](FIELD_PARTNER_FRONT_WALL_FILTERS.md) are already recovered.
Call hitCheckVecFilter(&Q,callback). A null result returns null with NO caller
output stores,including no length store.

For nonnull H,publish in this order:

1. Quantize Q+3c distance and store to length pointer.
2. Quantize Q+24 pointX and store pointOutput+0.
3. Quantize Q+28 pointY and store pointOutput+4.
4. Quantize Q+2c pointZ and store pointOutput+8.
5. Copy Q+30/+34/+38 normal float values to normalOutput+0/+4/+8 without
   the thousandth quantization.

Every quantizer independently chooses+.5 for ordered value>=0 and-.5 otherwise,
uses fused single-result1000*value+bias,fctiwz,original signed-word conversion
and single-result divide by1000. NaN/int-conversion and FP environment domains
remain target-CPU requirements. Do not quantize normals,leave point unquantized,
or infer distance from point displacement in this wrapper. Its behavior differs
from [hitCheckFilter](FIELD_SCALAR_RAY_WRAPPER.md),which publishes raw outputs.

Return H unchanged after stores. Output aliasing observes distance,point XYZ,
normal XYZ store order;the point and normal are not swapped. Copying input
vectors before backend work supports ordinary output/input aliases,but any
external callback mutation or invalid alias must retain the actual ordering.
Do not invent a full alias/domain guarantee from these local copies alone.

The tool pins all five quantization/store regions,normal copies,selector calls,
null path and backend result addresses. Shared scalar constants and the
conversion double at0x802cb878 are pinned. No root position,partner cache,
motion or contact pointer is directly published here;callers own those effects.
Full backend geometry/header/cull/resource lifetime and original ray outputs
remain open. Deduced tests should distinguish miss preserving every output,
five independent signed quantizers,raw normals and output alias store order.
