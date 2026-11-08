# Crump crowd snapshot, texture and draw lifecycle

US G8ME01 disc 0 revision 0. This is static recovery from the read-only original
ISO, not an original runtime observation or a native rendering completion claim.
It supplies the snapshot and ext-rendering requirements for the
[opening integration](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
[EVT-01](../../requests/EVT-01.md) remains open for production owners and reference
observations.

## Evidence and scope

The complete bodies below were rehashed directly from the ISO; all twenty body
identities in ignored `artifacts/crump-crowd-resources-evidence.json` matched.
Ordered witnesses are `artifacts/crump-crowd-*.disasm`. Additional constants and
checks are in `artifacts/crump-crowd-snapshot-extra-evidence.json`; the detailed
review is `artifacts/crump-crowd-snapshot-lifecycle.txt`. These are shell/ISO
inspection results, not REA runtime evidence. Lengths and offsets below are
hexadecimal unless explicitly identified as decimal.

| Complete body | VA | Bytes | SHA256 |
| --- | --- | --- | --- |
| `animPoseDisp_MakeExtTexture` | `80039f0c` | `00000784` | `0748a36441e9ac4a7cfa22c2d994099c15f5972f486e1575fe31daafd49fc658` |
| `extDraw` | `800d6558` | `000000f8` | `21ef93a4033475cec862573526717908a3fcf126af1c5e5d981d7fce4b9246b5` |
| `extEntry` | `800d6fe8` | `0000035c` | `bf5a77d1ca98d0543c6dfc53c3bd7b4b230a0643df074bd9f969bf1a50c0e652` |
| `extLoadTexture` | `800d6334` | `00000110` | `d1807c27b5717b5279c8e10e06d912bfb6b88eeb9c2fec984d2da75d508f7b64` |
| `extLoadVertex` | `800d6444` | `000000b4` | `d9d91a04153b4ad8115e57b7293527a45103745d101a6b45fdbc4bb4caa80a9a` |
| `extMain` | `800d67cc` | `000003b0` | `2ad4b5ab70f9c0921887c0de88a7d484cd26adf1eef3be6e4cb0246dd0bebe49` |
| `extMakeTexture` | `800d6c58` | `00000390` | `622ef09707d384baf3f7fd05672f70c11a30214c0379f588af269cb455bc7a58` |
| `extPoseDraw` | `800d6650` | `00000108` | `11c653f9c59dcc85cf548b8e3086dedafeda9d48ca85201ab6f35c7de54e7a37` |
| `extReset` | `800d6b7c` | `000000dc` | `a0f2aa2861a4fdd112cb7c8b473081f4880cfd2f4cb5c83d87161ed7dd4e2fe3` |
| `nannpc_ext_disp` | `80130fb8` | `000002b4` | `16f285eaa5dca795e2ad4235f9aa89557219fc113038dba18411348e8f194d9b` |
| `nannpc_ext_dispent` | `80130c08` | `000000a0` | `306cf5d55c7dd09512f6c92ec256a0a7421b24ad5f6bdc5abf554f9960dee0f2` |
| `nannpc_zcompare` | `8013126c` | `0000002c` | `df724403ff60c94b9a43d186c9b366cf93b61751bcb9b4646aaa0e158acbeeee` |
| `_unload` | `80007ab4` | `00000178` | `e217052388ebb30ee45513043dab0e0a1008cddcb6ab92c8c3b78f117a575099` |
| `seq_mapChangeMain` | `80007c2c` | `00000af8` | `517ca2e7b145ff0a315ba6a3c11312e083faced219eeed728b21c54dbadd66eb` |
| `evt_ext_reset` | `800d7370` | `00000024` | `849e2a7c4176781f3c4bde5efcf4ff686e944d1fc6d23e546d7ebfa42fac8cbf` |
| `battle_exit` | `800736ec` | `000001c0` | `500c637c3049e79b92269464c281a9064a518f136293594ba39c3aae63979c42` |

## Work, templates and deferred camera ownership

Each ext helper independently reads `GP = word[80415fd0]`. `GP+14 != 0` selects
work bank `803df5f8`; otherwise it selects `803df588`. Each bank is `70` bytes.
Do not retain one bank selection across separately reached original helper calls.

| Work offset | Recovered owner |
| --- | --- |
| `00`, `04` | Instance count, template count |
| `08` | Instance records, `20` bytes each |
| `0c` | Template records, `34` bytes each |
| `10` | Instance matrices, `30` bytes each |
| `14` | Template quad XYZ array |
| `18`, `1c`, `20` | Shared source attribute arrays |
| `24` | Sort records, eight bytes each |
| `28`, `2c` | Display lists, `80` bytes per template, and recorded lengths |
| `30`, `34` | Optional callbacks |
| `38` | Last loaded template index |
| `3c` | Affine matrix storage initialized to identity |
| `6c` | Texture-preload byte accounting |

`extEntry800d6fe8` counts twelve-byte descriptors until a null model pointer.
It allocates seven heap-2 arrays: instance records, template records, instance
matrices, template quads, instance sort records, template display lists and
lengths. It initializes borrowed matrix pointers and sort indices, then registers
`dispEntry80010bc4(0, 1, extMakeTexture800d6c58, descriptors, 0.0f)` at
`800d7300..7314`.

Snapshot generation therefore belongs to a deferred **camera-0** callback.
Descriptor storage, actual source poses, ext work and output allocations must
survive that callback. It generates one snapshot per template, rather than
rendering the full model separately for every crowd instance. Actual camera
publication follows the [camera manager contract](../camera/CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md);
field camera 4 cannot silently replace camera 0.

## Snapshot source pose and three render passes

`animPoseDisp_MakeExtTexture80039f0c` receives a model name, animation selector,
frame in `f1`, draw scale in `f2`, output texture object, output smart-header
holder and four crop-output pointers. `extMakeTexture` supplies each descriptor's
frame and scale `4.0f`.

The helper searches current animation work's pose array for active flags bit 1,
context `pose+0c == Boolean(GP+14 != 0)` and matching model name. Failure returns
without initializing outputs. An unavailable original pose is not permission
to publish a fabricated transparent snapshot.

On success, it saves the complete `170`-byte pose, writes `pose+90=-1`, resolves
the requested animation and evaluates the actual pose. Negative signed selectors
are name pointers; nonnegative selectors below the animation count are indices.
A missing name or out-of-range index selects index zero. Existing animation
clock, lookup, link and Main children retain their own source requirements.

The helper obtains render-mode framebuffer width and EFB height through
`8002eb30`. Reference crop origin is `(0.5*width, 0.75*height)`. At `8003a35c`
it calls `sysWaitDrawSync80005ce4`, then `GXClearBoundingBox802b69d0`. A temporary
translation is `(0, -0.25*height, 0)`.

Before drawing, it saves pose flags, material flags and both colors; clears
flags mask `8`, sets material flags `e8=0` and colors `f0/f4=ffffffff`.
It calls `animPoseDrawInner8003bc68` with that translation and original pass
selectors **1, then 2, then 3**, using `f1=0` and the captured draw scale.
Those four pose fields are then restored. A real snapshot needs the original
geometry, textures, materials and alpha behavior of all three passes; a successful
placeholder draw callback is insufficient.

After capture/publication, `8003a64c..a660` restores all `170` pose bytes. This
restores neither evaluated output buffers nor GPU, global work, bounding-box or
copy state. A native continuation must retain the saved pose and incomplete
children until completion. Original code supplies no asynchronous cancellation
or rollback API for a partially suspended capture.

## Bounding box, RGB5A3 copy and inherited filter state

After the three passes, the helper synchronizes again and calls
`GXReadBoundingBox802b6a08`. The source processes the four halfwords as follows:

1. If left exceeds right, substitute `(0,0,16,16)`.
2. Clamp left and top to at least 2; clamp right to framebuffer width minus 2.
3. At `8003a4cc..a4f0`, compare **top** with EFB height minus 2 and clamp top.
   The complete original body does not clamp bottom at this point.
4. Round left/top downward to even coordinates and right/bottom upward to even.
   Width/height are the low-halfword differences.

Preserve that top/bottom asymmetry; do not silently correct it as a presumed typo.
There is no local generic positive-span or maximum-size guard.

The exact publication chain is:

- `GXSetTexCopySrc802b5ee8(left,top,width,height)`;
- `GXSetTexCopyDst802b5f98(width,height,5,0)`;
- `GXGetTexBufferSize802b708c(width,height,5,0,0)`, accumulating the result in
  `8041e868`, then the same size query again for allocation;
- `smartAlloc8002f540(size,1)`, storing its returned **header** in the output
  holder, then dereferencing the current header payload;
- `GXCopyTex802b6844(payload,1)` and `GXPixModeSync802b5658`;
- `GXInitTexObj802b72b0(object,payload,width,height,5,0,0,0)`.

Format 5 is original **RGB5A3**. The clear argument is 1. There is no local null
allocation guard, no CI palette and no local LOD/filter override. Crop outputs
are the aligned coordinates minus truncated reference-origin coordinates.

Neither this complete body nor `extMakeTexture` sets copy filter, copy-clear
values, pixel format, projection, viewport or scissor. These states are inherited
from the actual frame/camera owners. Their absence is not evidence that filtering
is disabled. Exact filter weights, EFB alpha, clear color, edge alpha and camera-0
framing still need a bounded caller/writer audit and original reference images.
Native RGBA8 storage is an adaptation and must account explicitly for RGB5A3
color/alpha quantization; it does not prove original copy behavior.

## Template quad and display-list publication

`extMakeTexture` resets `8041e868`, saves the shared sixteen-byte UV rectangle
at `8042e0c0` and temporarily writes `(0, 4/templateCount, 0, -1/templateCount)`.
The saved rectangle is restored after its ascending template loop. The source
performs those divisions even for a zero template count.

For each snapshot, its quad is:

```text
(left/2,  -top/2,    0)    (right/2, -top/2,    0)
(left/2,  -bottom/2, 0)    (right/2, -bottom/2, 0)
```

The emitted corner order is top-left, top-right, bottom-right, bottom-left.
A template record contains smart header at `+00`, texture object at `+04` and
preload region at `+24`. `GXBeginDisplayList802b9e10` uses its `128`-byte slot;
`GXBegin802b5a6c(0x80,0,4)` records an original quad. Position/UV INDEX16 pairs
are `(4*i,0),(4*i+1,1),(4*i+2,2),(4*i+3,3)`. The actual size returned by
`GXEndDisplayList802b9edc` is stored separately.

Header, texture object, quad, source arrays, display-list bytes and recorded
length must remain a consistent generation. A partially generated list cannot
be acknowledged as a completed template.

## Stable smart headers and real texture preload

The holder is a smart-header pointer; payload is `word[header]`. The
[smart texture patch](ANIMATION_SMART_TEXTURE_DATA_PATCH.md) reads that current
payload and patches the same texture object. It neither allocates nor pins
payload memory. [Smart compaction](../memory/SMART_HEAP_COMPACTION_AND_PENDING_FILES.md)
may move ownerless snapshot bytes and update the stable header. Retaining an old
payload address across compaction bypasses original ownership.

`extLoadTexture800d6334` invalidates the texture cache, walks templates, queries
object format/height/width and actual buffer size, initializes each preload region,
patches its texture object through `smartTexObj`, and calls
`GXPreLoadEntireTexture802b7bc4`. If the current TMEM offset is below `80000`
and its end reaches or passes `80000`, it moves the start to `80000` before
that load. Final offset is retained at `work+6c`. Generator setup is
`(0,1,4,60,0,125)`. `extLoadTextureExit800d6314` calls
`GXTexModeSync802b567c`.

These calls establish real cache/preload state requirements, not a texture-name
lookup. Preserve [allocation failure and retries](../memory/SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md),
header identity and actual GPU retirement independently of template CPU storage.

## Instance matrices and the two actual sort orders

`extMain800d67cc` invokes the first optional callback only when query `8006f7bc`
returns zero; it then invokes the second optional callback independently. Its
camera-4 yaw calculation, facing interpolation and sine-table writes update
actual borrowed instance matrices. Position-only billboards lose this source
matrix behavior.

The ordinary ext sort stores template index as its active-instance key and sorts
ascending using `compare800d67a0`. `extDraw800d6558` walks sorted records from
the array start while its loop counter decrements. Its actual traversal is
therefore **ascending template-key order**, not reversed record order.

For an active instance, a changed template patches the texture object through
its current smart header and loads its preload region. Drawing composes the
**current camera view** with that instance's matrix, uploads position/current
matrix 0 and calls the recorded template display list. Ordinary `extDraw` does
not upload a normal matrix. `extPoseDraw800d6650` additionally performs inverse
transpose and normal upload; singular behavior remains an explicit child.

The crowd alpha path has a separate order. `nannpc_ext_disp80130fb8` builds keys
from each crowd actor's Z at `+20`; `nannpc_zcompare8013126c` sorts descending Z.
Its actual draw loop then walks the sorted array backward, producing
**ascending-Z draw order**. Before `extPoseDraw(instanceID)` it calls
`GXSetTevColor802b8ab8(1, actorColorAtBc)`. Equal-key stability is not established;
no actor-index tie order may be invented from the comparator alone.

`nannpc_ext_dispent80130c08` registers camera-4 crowd and shadow callbacks at
source sort key zero. When crowd work `+10` mask 1 is clear their types are
1 and 2; when set both are type 2. The real display queue owns execution order.

Ordinary ext drawing selects `extLoadRenderMode`, `extLoadVertex`,
`extLoadTexture`, `extLoadTev`, `extDraw`, then `extLoadTextureExit`.
The alpha crowd branch selects its custom per-instance TEV path instead.
The ordinary render mode has exact tuples:

```text
BlendMode(0,1,0,0)
ZCompLoc(0)
AlphaCompare(6,128,1,0,0)
ZMode(1,3,1)
```

`extLoadVertex800d6444` uses only INDEX16 position 9 and UV 13, with strides
12 and 8, and cull 0. It does not emit normal/color indices. The
[raw animation render parent](NATIVE_ACTUAL_VM_RENDER_PARENT.md) and its
three-common-lane fan stager cannot be substituted unchanged for this interface.

## Reset, tag-1 release and cancellation ownership

`extReset800d6b7c` frees the seven nonnull heap-2 arrays, then clears the selected
`70`-byte work bank. It calls no smartFree, texture-cache flush, queue deletion
or draw synchronization. Snapshot headers were allocated with tag **1**.
Their release belongs to a separate smart-tag/arena lifecycle owner, not to the
local array reset. `smartAutoFree(2)` at battle exit does not match tag 1.

The independent tag-1 release caller is now established: field-map
`_unload80007ab4` supplies literal 1 to `smartAutoFree8002f9cc` at `80007c04`.
It executes this source order:

1. Reinitialize Mario/camera and unload the map, then `npcReset(0)`.
2. Call `extReset` at `80007b10`.
3. Reset objects/offscreen/images/effects/poses, event manager, cases, items,
   icons, windows/status/background and window manager.
4. Perform the conditional fade-reset block, stop rumble channels 0 through 3,
   and conditionally call `swReInit` when the area-prefix comparison differed.
5. Call `memClear80030154(1)` at `80007bf4`, then `memClear(2)` at `80007bfc`.
6. Call `smartAutoFree(1)` at `80007c04`, then clear crowd-work pointer
   `8041ebb8` at `80007c0c`.

The prefix and fade branches rejoin before those final heap/tag operations;
none bypasses tag-1 release. The tag release scans the global smart-header chain,
not the already freed per-template array. Follow
[automatic tag release](../memory/SMART_AUTOMATIC_TAG_RELEASE.md) and
[header release](../memory/SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md) for its
retained-next and normal-recycle effects. Do not add per-template frees inside
`extReset` or replace this owner with a full smart-arena reinitialization.

`seq_mapChangeMain80007c2c` reaches `_unload` at `80007e78`, after waiting for
`fadeIsFinish`, invoking the old module's `+38` callback, unlinking it and
conditionally map-freeing its allocation. It passes the old-area string at
`GP+12c` and next-area string `8041e5e8`. The
[room teardown contract](../field/ORIGINAL_NPC_ROOM_RESET_AND_TEARDOWN.md)
describes the shared actor/event lifetime. Module epilog, array reset, heap clears
and smart-tag release are distinct ordered owners.

The direct-call census covers all seven DOL text sections and all 29 `.rel`
files' original import relocations. It finds three direct `smartAutoFree` calls:
`_unload(1)`, first-allocation `smartAlloc(3)`, and `battle_exit(2)`. No REL import
targets `smartAutoFree` or `smartInit`; `gor.rel` has no reference to the scanned
release/reset target set. `evt_ext_reset800d7370` calls only `extReset` and returns
2; it does not release smart snapshots. These are bounded direct-reference facts,
not proof about all computed calls or alternate carriers. The complete census,
body identities and enclosing branch order are recorded in ignored
`artifacts/crump-crowd-tag1-release-owner.txt` and
`artifacts/crump-crowd-tag1-call-census.json`.

This closes the independent release-caller identity. Actual native reachability,
source GP/bank selection and queue/GPU drain chronology remain unverified.
`_unload` itself has no local synchronization or callback-cancellation API;
`fadeIsFinish` alone does not establish GPU retirement.

A native transition must quiesce deferred capture/draw callbacks and submitted
GPU consumers before freeing borrowed matrices, lists and texture backing.
Source reset alone supplies no cancellation guarantee. A native suspended
capture also needs an explicit recovery policy for its saved live pose; abandoning
it cannot silently publish success or leave the temporary pose installed.

## Proposed 3DS adaptation and acceptance requirements

Original model, animation and texture art is available. A suitable native design
can retain one snapshot per original template, convert its original copy result
into GPU-supported storage, and batch instances while preserving source matrices,
alpha and actual sort order. This is an architectural proposal, **not original
source behavior or measured performance**. Resource generations must own smart
headers, texture allocations, templates and queued consumers together.

Acceptance needs exact child/primitive hold tests, moving smart payloads, retained
pose restoration, separate capture/draw camera ownership, RGB5A3 alpha/crop
reference comparisons, ordinary versus alpha sort goldens, reset with pending
consumers, and actual execution of the verified map-unload/tag-1 release owner.
Fresh native images and physical New2DS XL timing/memory measurements are still required. Static body hashes,
original art availability and bounded staging do not establish rendered parity
or hardware performance.
