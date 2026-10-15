# Retained whole original JUT font draw-start setup

`source_font_draw_start.hpp/.cpp` owns JUTFont_DrawStart80074bfc/294.
The read-only source verifier pins the complete US G8ME01 revision0 DOL body,
34 direct call sites (two mutually exclusive scale sites), its initialized
texture-scale/zero constants and original DOL BSS extent. A reached invocation
contains33 child operations, including camera, scale and fog. The source
[setup contract](../message/FONT_GLYPH_REQUESTS_AND_SETUP.md) documents all
original GX tuples. No source/body bytes or original assets are redistributed.

## Complete local graph and actual late captures

Capture actual current camera first. Issue every original indirect-mask,cull,
depth,alpha,blend,vertex-descriptor/format,channel and texgen request in source
order. Only after the initial18 setters read the supplied color word and
request TEV color(index1). The public typed input supports an actual guest
pointer or a borrowed native equivalent of the original private caller word;
no guest stack address is invented. The supplied word is never modified. A
mutation before that source read is visible; a mutation after it cannot replace
the captured request through a wait.

Continue all original stage,order,color/alpha-op/input requests, then load the
actual **static object address8041e97c** at texture slot0. There is no word-load
through that address to obtain a texture token. This is not an arbitrary atlas
index; the actual source font-setup/resource/GPU owner must recognize and prove
its initialized/resident object.

After texture-load completion freshly read GP and GP+16c. Zero selects actual
scale constants(804203ec,804203f0,804203e4); any nonzero selects
(804203ec,804203f4,804203e4). Do not infer readiness or replace these with guessed
font dimensions. Retain the complete SDK scale request and its output, then
issue LoadTexMtx(30,1) and TexCoordGen2(0,1,4,30,0,125).

Read all12 words of the originally captured camera's view **at the later
LoadPosMtx(0)**, after those preceding children. A different current-camera
query value does not replace the captured pointer. Source writes to the view
before its actual late consumption are visible. Then SetCurrentMtx(0), read
actual positive-zero804203f8 and actual fog-color word80429578, and issue the
whole final Fog(type0,zero,zero,zero,zero,color) request. These captures are not
moved to entry or replayed after a held suffix.

## Retained providers and owner boundary

Ordinary GX requests reuse unchanged SourceFontStateCall/Services from
[shared font state](RETAINED_SHARED_FONT_STATE.md). SDK scale uses its actual
matrix callback. SourceFontDrawStartServices adds a typed actual camera query
and typed fog callback; the fog's four float lanes remain distinct from integer
lanes. No existing shared source/header ABI is extended by this implementation.

Every request object, operand, successful effect and query result survives
partial waits. False means actual unavailable/held work; recording a request in
a vector does not acknowledge the original GPU operation. A linked child must
own all its partial effects. A real finite native backend may interpret source
state into a validated,resource-owned ordered draw batch only when its complete
submission/publication owner requires actual GPU acknowledgment before the
original queued callback completes. There are no successful empty GX handlers.

The source camera/color/BSS inputs and mapped storage are required. A strong
actual context/resource owner, every provider/actor-memory/lifecycle identity
and the original color pointer identity are pinned across holds. The current
predicate is checked around providers and each memory read. Invalid lifetime
latches stale without issuing suffix work, even if ownership later appears
valid again. Recursive resume/reset fails. Explicit reset is metadata only,
with no original End or guest cleanup. Borrowed private words outlive the held
invocation. Pointer0 retains its genuine source guest-memory domain; no null
color or camera fallback is created.

## Actual BSS evidence

Original DOL header fields at d8/dc give BSSbase803c5360,size645ac,
end8042990c. Fog80429578 and the matrix font's selected-color80429598 are inside
that original BSS. Thus a real cold BSS producer can establish their initial
zero through source loader semantics, although neither has initialized file
bytes. This is not permission to return a constant0 per read: actual persistent
storage and subsequent writes remain observable. The font global object and
pointers also require their actual source Init/Setup ownership.

## Validation and integration

Strict host tests check the full source call-site census, literal static object
identity, all GP branches, guest/private color paths, late color/view/fog and
fresh GP mutations, every-child-twice holds with successful-prefix counters,
stable payload addresses, owner/table/lifecycle replacement, reentry/reset,
missing providers and immediate stale rejection during a memory read. Source
constants come from the original event pack; camera,BSS and GPU effects are
explicit fixture contracts, not native or original-hardware proof. ARM syntax,
ASan/UBSan and read-only source pins provide additional focused validation.

The common request router for SourceFontState's80074bfc child can pass its
stable q.color by reference to this owner and reuse its real GX/SDK providers.
Retain one cursor per whole invocation. The typed fog/camera callbacks must bind
actual native owners. Nothing registers another display record or activates a
normal menu in this batch.
