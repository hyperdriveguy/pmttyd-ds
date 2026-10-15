# Trio draw and material bindings

[TrioAttack facade wrappers](../../source/battle/trio_attack.cpp) connect actual
VM/camera/queue state to retained source draw helpers. See [public API](trio-api.md),
[display prefix](trio-display-prefix.md), [part/HP helpers](battle-part-presentation.md),
[draw pipeline](draw-pipeline.md), [class raster](battle-class-raster.md) and
[provider ownership](trio-providers.md).

## Invocation and provider ownership

A queued request is insufficient: each draw wrapper requires the source callback
to be **currently invoking**, with matching camera and userdata/part. Most bridges
borrow raw runtime and external service pointers during a synchronous call; their
retained cursors live in the runtime. Keep those owners stable across callback
reentry and held work. The modern HP path additionally pins and rechecks explicit
runtime, allocation, renderer lifetime and camera publication ownership.

| Original callback | Facade/retained owner |
| --- | --- |
| `0x8000e564` | Letterbox |
| `0x8011fde0` | Enemy HP (legacy or modern provider overload) |
| `0x80100258` | Stock EXP plus nested source-icon position cursor |
| `0x802036d4` | Break-slot/reel |
| `0x80138da8` / `0x801389a0` | Two independent stage cursors |
| `0x801a38f8` | Audience callback with actual current queue record |
| `0x801a37f0` / `0x801a36c8` | Two independent audience auxiliary cursors |
| `0x80101774` | Actual source part, inner model, group traversal and shape owners |
| `0x8002d3d8` | Shared shadow/camera/collision owners |

Optional zero/false can be an actual result; absent optional denotes unavailable.
An excluded model pass can complete without drawing, which differs from a missing
resource/provider. Boolean completion does not grant guarantees belonging to a
different overload or cursor.

## Read routing and numeric adapters

Letterbox resolves shared startup color `0x80429510`, original read-only fog word
`0x8041f6b0=0xffffffff`, then GP-address view before general VM memory. Camera
signed fields `0x1e0/0x1e2/0x1e4` are alpha/top/bottom. Actual pass/GX/begin/vertex
callbacks retain external ownership.

HP resource reads admit allocation first, then camera-owned synthetic words,
actual VM and GP view. Legacy HP draw uses published camera-4 projection and
actual VM/status/saved-switch queries, forwarding numbers/bars externally. It
has no modern allocation-epoch/begin/publish binding. The modern overload requires
camera 8/Battle and its explicit current publication; its stronger contract is
explained in the provider guide.

Break-slot word/half reads try VM before caller fallback; bytes are VM-only.
The sourceIcon service preserves its original user/GX providers, while the legacy
icon callback is null. Declared native numeric matrix adapters do not substitute
for real GPU work. Stage/audience wrappers replace only their stated VM/camera/
request services; nested copied icon/numeric providers keep their own users.

## Part and inner-model dispatch

Part scalar adapters admit finite source angle/heading inputs and original phase
offsets. Status reads actual 30-byte storage. Material setters update real pose
color word `0xf0` and flag word `0xe8` through original replace/OR/AND semantics.

Draw requires an actual pose, source field `0x90==0xffffffff`, original resource
flags and an admitted model pass. Pass bit 5 suppresses the ordinary bit shift;
pass 2 also admits model flag 1. Missing resources report source function tokens,
not successful empty draws. A supplied outer draw callback takes precedence.
Otherwise the recovered inner graph uses actual linked group/GX/callback/cache
flush/invalidation providers and retains captured context for later processing.

Within it, a supplied whole traversal takes precedence; otherwise wrapped group
traversal preserves original provider users. A supplied shape renderer takes
precedence before retained native shape fallback. Inner, traversal and shape
cursors own distinct pending operands. Diagnostics forward the deepest admitted
source hold; completed outer matrices do not complete the model or GPU pass.

## Shadows and materials

Shadow draw uses the actual current queue record/camera, mutable camera resolved
by index and live GP+`0x14`. Pass/GX/texture/geometry remain external. Collision
prefers an external ray provider when supplied, else actual BattleHitRegistry;
unknown context writers must still invalidate that registry.

Texture, color, light and TEV each retain a separate cursor. An unavailable helper
publishes its real pending token. Success does not clear the caller's pending
variable here. Loaded original battle lighting/group data is required; no empty
field-bank substitute makes a missing material complete.

External TEV branches take precedence. Native shadow branch `0x8002bc7c` latches
its preparation gate; enabled texture holds at `0x8002bca0` until real embedded
object/projection services exist. Null source map callback branch `0x800248a8`
still publishes indirect count zero; a non-null callback stays pending. Missing
shader/material/texture providers are not silently skipped.
