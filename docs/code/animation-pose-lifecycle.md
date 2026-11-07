# Source pose Entry and Release

[Entry](../../source/display/animation_pose_entry.cpp) and
[Release](../../source/display/animation_pose_release.cpp) retain original guest
operations across unavailable services. Their cursors represent one invocation;
keep its memory and child owners alive until completion. A `false` return is a
hold, and earlier acknowledged stores remain visible. It does not roll back the
invocation or mean the source function returned a failure value. See
[memory aliases](animation-memory-ownership.md) and
[selection](animation-selection-ownership.md).

Offsets and addresses below are hexadecimal; strides and capacities are decimal.
The Entry switch uses original instruction addresses. Its decimal constants are
translated PPC immediates, including signed offsets from SDA/SDA2. The source
bases are `r13=0x8041cf20` and `r2=0x80427440`. Register copies preserve semantic
stack tags so a filename can survive a child hold without a fabricated guest
stack pointer. The five child arguments correspond to r3..r7.

| Current work owner W | Record layout |
| --- | --- |
| W+0 / W+4 | MODEL array / count; 16-byte rows: flags0, refs4, file8, texture index c |
| W+8 / W+c | Texture array / count; 12-byte rows: flags0, refs4, file8 |
| W+10 / W+14 | Pose array / count; 368-byte rows |

The root cell `0x80415dc0` supplies W. These are actual source tables, rather than
an independent renderer handle namespace. Native callers own capacity and
lifetime admission; the portable bodies preserve source arithmetic and missing
validation.

## Entry publishes a prefix before resources are ready

Modes 0, 1 and 2 select archives 1, 2 and 0. Other modes skip the archive setter.
Entry scans for the first pose with a zero flags WORD, marks it active and clears
its two auxiliary words. Exhaustion selects pointer zero/index -1 and reaches
the original null access. It does not supply a graceful native allocation error.

An active MODEL compares its loaded header name through FileEntry+a0 → holder →
payload+4. Reuse increments the model reference; a new row requests type 5 with
`%s/%s`, prefix `a`, and the requested name. Null handle or payload returns source
-2 after the original model cleanup. Payload-null first calls fileFree. The
stale file word survives. Publication of model index -2 clears the pose active
word before return.

A new MODEL derives the texture filename from payload+44 using `%s/%s-`.
Texture reuse compares the real FileEntry+20 filename and increments that row's
reference. A new texture requests type 4. Null texture has no equivalent source
rollback: Entry continues to timestamp and AllocBuffer. The native adoption
layer must require a genuine resource instead of repairing the source result.

Timestamp conversion reads bus clock `0x800000f8`, quarters it, applies unsigned
multiply-high by `0x10624dd3`, then shifts six bits to obtain ticks/millisecond.
Signed-64 division uses GP+40/44 for mode zero or GP+38/3c otherwise, with GP
loaded through `0x80415fd0`. It publishes LOW at P+1c before HIGH at P+18.
Cache index -1, zero interpolation/discrete state, mode and linked-pose -1 precede
the AllocBuffer child. That child receives the pose pointer, not its index.

Only after AllocBuffer finishes does Entry publish material/color/rate defaults,
inline P+124 identity matrix, sort kind/owner and cleared callback/data. The four
sort names select live Vivian-tail pointers; the default owner is `0x80415dc4`.
Unmentioned stale fields remain. There is no whole-pose memset. Entry pins
name/mode but does not compare callback identities on resume; stable providers
and exclusive producer ownership are caller requirements.

## Release preserves shared resources and fresh reads

Release captures pose address with wrapping handle×368 arithmetic. A nonzero
P+4 chooses the borrowed/group branch: decrement P+8, store, reread, and return
while the signed result is nonnegative. That early return retires no resources.
A negative result enters the common suffix without the ordinary smart frees.

The ordinary branch tests flags40 and requests smartFree(P+15c), then rereads
flags before testing flags80 and requesting smartFree(P+164). A held child keeps
its captured argument even if another operation changes the original field.

The common suffix reloads W and the pose's model index. It decrements, stores
and rereads the model reference. Signed positive skips resource retirement.
Otherwise model flags clear first, then its texture index is loaded and current
W/texture base resolved. The texture reference also decrements, stores and
rereads. Signed nonpositive requests fileFree(texture.file), then clears texture
flags. The model file is read only after those child effects, then released.
Pose flags clear last. Other words and file handles stay stale.

These decrements wrap; they neither saturate nor use native unsigned-positivity
rules. fileFree does not imply payload collection, GPU deletion or scratch arena
retirement. Complete native sidecar retirement and unsupported smartFree owners
are described by the [native Release contract](../behavior/display/NATIVE_SOURCE_POSE_RELEASE.md).
See the [Entry contract](../behavior/display/NATIVE_SOURCE_ANIMATION_POSE_ENTRY.md)
for resource adoption after the retained parent finishes. Static reconstruction
and host fixtures do not establish original execution timing or visual parity.
