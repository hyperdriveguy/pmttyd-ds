# Display records, sort requests and per-camera dispatch

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
dispGetCurWork0x80010864,size8; dispCalcZ0x8001086c,size94;
dispDraw0x80010900,size25c; dispSort0x80010b5c,size34;
_sort@dispdrv.o0x80010b90,size34; dispEntry0x80010bc4,sizea0;
dispReInit0x80010c64,sizec; dispInit0x80010c70,size64.
Pinned Japanese dispdrv.c structurally corroborates these routines; contracts
below come from US instructions/constants. Sorting helper internals are now
covered in [sort implementation](DISPLAY_SORT_IMPLEMENTATION.md); outer camera
cadence and executed images remain dependencies. See
[window manager](../message/MESSAGE_WINDOW_MANAGER.md) and [selection drawing](../message/MESSAGE_SELECTION_DRAWING.md).

## Allocation, records and count-only reset (B)

dispInit calls __memAlloc(0,0x4000) for1024 records of16 bytes and
__memAlloc(0,0x1000) for1024 four-byte sort pointers. Store at r13+0x16f0/
0x16f4 (0x8041e610/0x8041e614); memset both full buffers to0; set count at
r13+0x16f8 (0x8041e618) to0. No allocation failure guard or prior-buffer free.
Current-work pointer at r13+0x16fc (0x8041e61c) is not cleared locally.

dispReInit writes only count0. No buffer clearing, callback cancellation,
userdata release, sort, current-pointer clear or generation marker. Next entries
reuse record storage from index0; old pointers/data persist outside active count.

| Record offset | Stored role |
| --- | --- |
| 0 | Low8 camera argument |
| 1 | Low8 render class argument |
| 2..3 | Padding, not written by registration |
| 4 | Binary32 transformed sort key |
| 8 | Callback pointer |
| c | Borrowed userdata word |

## Registration and class-dependent key (B)

dispEntry receives camera in r3, signed class in r4, callback in r5, userdata
in r6, requested order in f1. Compute current record address from count*16.
Compare f1 to itself with fcmpu; unordered returns before record writes or count
increment. This rejects NaN requested order; infinities and signed zero pass.

Store camera/class bytes and callback/userdata words. Use the full class word
to index the float table at0x80414fd8; class is not narrowed before indexing or
sign/direction selection. Classes2/5/9/10 store f32(offset+order), others store
f32(offset-order). Append record pointer to sortPointers[count]; increment count
with word arithmetic. No1024-entry capacity guard, callback-null check, camera/
class validation or userdata lifetime handling. Invalid full class values can
index outside the table even though their stored class byte is narrowed.

| Class | Original offset | Key direction |
| --- | --- | --- |
| 0 | 400000 | offset-order |
| 1 | 400000 | offset-order |
| 2 | 600000 | offset+order |
| 3 | 100000 | offset-order |
| 4 | 100000 | offset-order |
| 5 | 300000 | offset+order |
| 6 | 900000 | offset-order |
| 7 | 1100000 | offset-order |
| 8 | 1000000 | offset-order |
| 9 | 500000 | offset+order |
| 10 | 700000 | offset+order |

US symbol metadata declares offset_tbl size0x28, omitting its final element.
Direct US reads establish all11 floats over0x2c bytes, including class10 at
0x80415000; public source also lists that retail element. Do not truncate this
table using annotation size alone. Finite order arithmetic can overflow to an
infinite key; there is no second validity test after key calculation.

## Sort wrapper and comparator (B/P)

dispSort calls qqsort0x800064fc(sortPointers,count,4,_sort@dispdrv.o), then
returns. This is the game helper qqsort, not MSL qsort0x8026c1b8. It passes all
active pointers together, not separate arrays per camera/class.

Comparator receives pointers to pointer-array elements, dereferences each once,
loads record keys and uses ordered fcmpo. Return1 for first>second, -1 for
first<second, otherwise0, including equal keys and unordered comparisons. No
camera/class/index tie breaker or original registration ordinal. Requested order
is ascending by transformed key. Exact qqsort algorithm, tie permutations and
scratch/reentrancy are now covered in [sort implementation](DISPLAY_SORT_IMPLEMENTATION.md),
including a three-entry equal-key reversal. Actual caller and anomalous-key
executions remain open; there is no host stable-sort guarantee.

Window callbacks register camera8,class0,order=400-float(s16(layer)). Their key
is f32(400000-order), approximately399600+layer. Under the requested ascending
comparator, larger window layers draw later within this key range. This is a
derived request relationship, not measured compositing or a global priority rule;
arbitrary requested orders can cross nominal class ranges.

## Live per-camera traversal and callback lifetime (B)

dispDraw(cameraWord) starts index0 and visits sort pointers while signed index
<current count. Reload count on each loop test and sort-array pointer for each
record; this is not a snapshot of the count/entries at entry. Compare unsigned
record camera byte to the full supplied word. Mismatch skips all GX/callback
work. There is no camera-byte narrowing of the draw argument or early termination
on an unmatched camera; all current records are traversed.

For matches, apply common/class-specific state below, store current-work pointer
to that record, then invoke its callback with r3=draw cameraWord and r4=userdata.
Callback-null is not checked; return value is ignored. No automatic queue deletion,
count reset, resort, userdata validation or projection-camera setup here.

Callback registrations can extend the count and be encountered later in this
same traversal, without a new sort. Count reset can end traversal on its next
test; sorting/mutating pointers can change later visits. Repeated dispDraw for
the same camera re-invokes its retained callbacks. These are static consequences,
not observed callback mutation traces or a claim about normal callers.

dispGetCurWork simply returns the global current pointer. It retains the last
invoked record after draw/reset; a draw with no matches leaves it unchanged.
There is no nested-dispatch save/restore. This pointer is not proof of an active
callback or owning reference. Queue borrowing does not cancel a queued W when
window flags/handle change; actual update/event/draw lifetimes must be traced.

## Per-record GX requests (B)

Every matching record first requests GXSetColorUpdate(1). GXSetAlphaUpdate is0
for draw camera4/5/7 and1 for all others, including6. Then request
GXSetZScaleOffset(1.0,0.0). Render class byte0..10 dispatches through the11-target
table at0x80304460; >10 skips class-specific settings but still calls callback.

The following ordered calls follow those common settings. Numbers are the exact
integer request values, without unverified final GPU-state/rendering claims:

| Classes | GXSetBlendMode | GXSetZCompLoc | GXSetAlphaCompare | GXSetZMode |
| --- | --- | --- | --- | --- |
| 0 | (0,1,0,7) | (1) | (7,0,0,7,0) | (1,3,1) |
| 1 | (0,1,0,0) | (0) | (6,128,1,0,0) | (1,3,1) |
| 2,5,9,10 | (1,4,5,0) | (1) | (7,0,0,7,0) | (1,3,0) |
| 3 | (0,1,0,0) | (1) | (7,0,0,7,0) | (0,3,0) |
| 4 | (0,1,0,0) | (0) | (6,128,1,0,0) | (0,3,0) |
| 6,7,8 or >10 | No local class-specific calls | No call | No call | No call |

Classes6..8 therefore inherit those four settings from previous work after
the common requests; they do not receive a complete neutral reset. No prior GX
state restoration after callback. Shader/TEV/texture/fog/vertex settings are not
universally initialized here and may be established by each renderer.

## Position-derived order helper (B/P)

dispCalcZ(positionPointer) obtains camGetPtr(4). Call PSMTXMultVec with camera+0x11c,
input position and a local result; then PSMTX44MultVec with camera+0x15c and the
local vector as both input/output. Matrix helper arithmetic, camera producers
and projection/division conventions remain separate; names alone do not prove
their full numerical behavior.

Read resulting z. Compute single-precision fused 5000*z+5000. Ordered compare
against0: if less, replace with0. Otherwise compare against10000: if greater,
replace with10000. Multiply the result by-1 with fmuls and return. Thus ordinary
numeric output is in[-10000,0]; unordered z bypasses both clamp replacements.
The final multiplication retains the original signed-zero/environment semantics.
This helper always uses camera4, independent of dispEntry camera arguments.

Differential obligations: NaN admission, eleven offsets/metadata-size discrepancy,
key direction and float rounding, pointer sorting/ties, byte versus full-word
camera/class values, count-only reset/current pointer, live traversal mutations,
common/class GX order and inherited settings, callback borrowing, camera4 matrix
calls, fused depth mapping and clamps. Remaining: actual sort/tie execution, camera/
matrix/GX services, actual reset/sort/draw cadence, message renderer bodies and
original execution/images/story callers. Full rendering/game equivalence remains
unproven; the complete-game requirements document is not yet ready.
