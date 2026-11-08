# Animation Main, Inner and VM provider ownership

[Main](../../source/display/animation_main.cpp) and
[Inner](../../source/display/animation_inner.cpp) retain different source captures.
The [time binding](../../source/display/animation_main_time_binding.cpp) and
[replay binding](../../source/display/animation_main_replay_binding.cpp) expose
actual VM operations to the retained [evaluation stages](animation-main-evaluation.md).
Field offsets and guest addresses below are hexadecimal; sizes are decimal.
A cursor preserves progress, not the lifetime of its borrowed memory. Callers
keep the producer pass, VM, aliases and child owners stable through holds.

## Main captures before linked work

Main resolves W through `0x80415dc0`, pose pool W+10, MODEL cache W+0, pose
resource/animation indices, FileEntry+a0 holder, loaded MODEL and its descriptor
directory. Pose stride is 368 bytes, MODEL cache stride 16 and descriptor stride
64. The original parent supplies no handle, active, pointer or index validation
before those reads; native admission is a separate owner responsibility.

It then requests GQR2..5 writes through SPR914..917, with mirrored load/store
words `00040004`, `00050005`, `00060006`, `00070007`: unsigned8, unsigned16,
signed8 and signed16 at scale zero. Platform services must implement those
semantics. Main neither saves nor restores earlier GQR values.

Next it captures output pointers: W+e8 chooses shared W+f8/fc, otherwise pose
P+4c/54. Linked P+90=-1 skips recursion; otherwise mask2 in the linked P+4 can
skip that child. Linked flags use the already captured pool. No cycle filter,
recursion limit or once-per-tick filter is inserted. Only after linked work can
a null captured timeline return, so null does not bypass platform setup.

Evaluation receives the original W/pool/pose/MODEL/timeline/output captures.
Linked writes and later holds do not refresh them. Successful evaluation advances
to facing permanently; a facing hold cannot replay sampling. Provider identities,
including the complete facing interface, stay pinned. See the
[retained Main contract](../behavior/display/RETAINED_ANIMATION_MAIN_ENTRY.md).

## Inner captures samples before a possible late Main

Inner receives a pose pointer and copied caller matrix, angle, scale and pass.
Identity uses all 48 matrix bytes and scalar float bits, including signed zero
and NaN payloads. Its initial reads interleave file/descriptor resolution with
P+28 current, P+2c next and P+30 fraction. A null timeline exits after that prefix.
Initial W+10c and one P+c read supply exact mode gates. Pass2 has a resource
mask1 exception; otherwise the source shift uses low six pass bits, with shifts
32..63 producing zero.

Pf4 secondary color precedes Pf0 primary. Their low BE bytes are alpha.
Primary alpha applies only under Pe8 mask40; secondary always applies. Two
optional map queries independently read INDEX `0x8041e664` then BASE
`0x8041e660`, stride `0x2f4`, halfword flags and an admitted alpha byte.
Translucency changes effective pass to 3 without repeating resource admission.

A Pe0 callback replaces both built-in GX calls and W+e0 publication. Otherwise
effective passes0..3 issue the original Z/blend/depth-compare-location/alpha
requests; other values skip GX calls but still publish fresh W+e0. The current
GX request remains captured through a held provider. These source requests do
not prove GPU pixels or restore earlier GX state.

After facing, fresh P+3c=-1 requests late Main. The handle conversion is
`high32(0xb21642c9 * low32(P-pool)) >> 8`; aligned poses produce their 368-byte
row index, with no added source alignment check. P+4 mask2 enables ring setup.
Capacity is captured before completing MODEL count resolution. Position+normal
and cursor+total sums wrap32 and compare as signed; exact equality fits.
Oversize skips allocation but still calls Main and clears the fresh selector.

Ring publication reloads W at its source use points: set W+e8, publish
W+f8=base+12*cursor, advance by captured positions, then W+fc using current
cursor and advance by captured normals. Late Main does not replace the original
MODEL, descriptor, sample indices or fraction retained by Inner.

Modeling level/pointer reset, effective matrix Copy and separate W+18/1c/20
publications precede flushing. The output selector is retained across both
flushes, but each flush independently resolves its resource, output pointer and
count. Count×12 wraps; zero does not suppress the flush child. Invalidation
precedes a fresh group-count read through the captured MODEL. Traversal receives
parent=-1, groupCount-1 and captured sample values, without a pass argument.
See [actual-VM Inner](../behavior/display/NATIVE_ACTUAL_VM_NPC_INNER.md),
[draw state](../behavior/display/ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md) and
[group traversal](animation-group-traversal.md).

## VM bindings admit operations without detached fallbacks

Both bindings retain the shared pool and borrow VM/alias ownership. Their pure,
non-reentrant current predicates must prove the same producer/lifetime at each
primitive. Alias checks require the same pool and VM. They add no independent
active-handle gate, so mapped inactive source rows remain readable/writable.
A current check supplies neither synchronization nor a producer lock.

The time binding reads its double bias synchronously as HIGH then LOW, without
a suspension between halves. The caller keeps mapping ownership exclusive.
Signed clock division holds on a zero divisor. Elapsed conversion is unsigned
through binary64 then binary32; truncation admits finite [-2^31,2^31), holding
for unsupported values instead of providing zero. Timeline shares the exact
same guarded word/store identity.

Replay can expose ordinary word/byte services even while GQR is unavailable.
Each quantized usepoint requires the actual GQR4 word `00060006` and decodes
one signed byte at scale zero. Earlier raw reads or copies can already have
committed when quantization holds.

Optional bulk copies default off. Word helper `0x80005634` floors byte length
to whole words; byte helper `0x8000519c` uses full length. Zero declines to the
scalar continuation, preserving source wrapping do/while behavior. The bulk
path also excludes the W root and requires an active raw borrow, complete raw
spans, nonwrapping guest/host nonoverlap, owned destination and no external-owner
alias. Decline has no effects. Success counts one diagnostic bulk copy, not a
frame or source reference. The helper retains no raw pointer or borrow across
callbacks/yields. Numeric/native acceleration domains remain distinct from
original instruction, scheduling, visual and hardware parity.
