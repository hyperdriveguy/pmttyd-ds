# Retained ordinary NPC draw callbacks

Complete local US G8ME01 disc0 revision0 `npcDisp8004c50c/2d4` and
`npcDisp_xlu8004c7e0/2a0` are implemented by `advanceFieldNpcDraw`.
Source SHA256: ordinary
`bab2a0d86dbae6988235568fa45bee22b47ed0df8b53ce982b03a06c2cd8a912`,
translucent
`ace5eb0d7680e999966c84f0d30618b4d4e79b457aa7bf8317d951a37f93ff87`.
This is local source control flow plus a VM/shared-pose binding, not activated
normal Game NPC rendering or proof of original GPU/numeric parity.

## Retained source work

Both callbacks negate fresh event yaw144 and call reviseAngle80007328, retaining
incoming A. They run the complete [shared matrix](RETAINED_NPC_DRAW_MATRIX.md)
with history slot-1. The source-supplied callback camera parameter is unused;
the matrix queries current camera only under its own original flags.

Fresh state1d4 and color114 are read in that order, including color when the
darkening gate is clear. State00400000 halves copied unsigned RGB bytes, keeps
alpha, loads fresh pose handle104, sets copied material event color, reloads
handle104 and ORs material flag40. There is no material restore at return.

Ordinary then queries actual current queue work80010864 and reads byte+1.
Class0 runs the facing-name graph, Main and pass1. Class1 requests pass2;
class2 requests pass3. Neither class1 nor class2 runs Main or the name graph.
Other classes finish after the shared prelude. Translucent does not query queue
class: it runs the facing-name graph, Main, then passes1/2/3 consecutively.

The facing-name graph reads fresh main flag00800000. When set, it loads
handle104 for GetCurrentAnim80039c44, retaining the call but discarding its
return; reloads handle for GetAnimPosePtr80039cf8; reloads handle again for
GetCurrentAnim, retaining the actual string pointer. It queries literal camera4
and loads atX18,eyeXc,atZ20,eyeZ14 in that order. Single-precision differences
are negated and passed to double atan2. Its result narrows to float BEFORE
multiplication by original factor8041fcbc. After reviseAngle, negative results
receive original360. It reads retained pose pointer+74, subtracts, revises again,
and classifies rear by `(d>90 && d<=270)||(d<-90 && d>=-270)`. NaN takes front.

The source `luigi_pose` table803066f8 contains14 pairs, stride8. Rear compares
left strings then chooses right; front compares right then chooses left.
strcmp uses source pointers, visits until first match or14 failures, and never
paraphrases or normalizes names. Pair5 is the unusual original J_1A→J_1RR.
On a match, fresh handle104 and fresh opposite table pointer go to SetAnim with
force0. The source no-match path has no setter. The table's original .data
classification is not a hardware read-only guarantee; packed source-backed
input views preserve all fresh pointer reads and remain VM-owned after import.

Main and each draw independently reload handle104. Every draw reloads scale2
at8041fd34, preserving captured A and matrix across all passes. Request args
represent r3/r4/r5; the guest matrix stack pointer in r4 is replaced with copied
`matrix` storage, so args[1] is0 and args[2] is the source pass. Scalars are f1/f2.
Every acknowledged primitive is retained across holds; false/empty has no effects.
Changing callback, actor or providers while held cannot replace the invocation.
Successful completion resets the cursor. Unsupported callbacks hold untouched.

## Native ownership

`FieldNpcMainEntryBinding::advanceDraw` borrows the same VM/animation alias as
Main, retains its registry owner, rejects another binding's suspended cursor and
holds if actual animation pool mappings change. VM reads and raw shared
material/pose getter leaves are built in. They do not add an active/resource
gate: inactive source slots remain addressable. Native bounded string admission
uses actual VM bytes, with no host pointer fabricated as an original string.

GetCurrentAnim is the exact no-effect source pointer graph: read global80415dc0
W; W+10 posepool; W+0 modelcache; P=pool+handle*170; P+10 resourceIndex;
P+14 animIndex; modelcache+(resourceIndex<<4)+8 file; file+a0 holder; holder
MODEL; MODEL+1ac descriptors; return descriptors+(animIndex<<6). Arithmetic
wraps to low32 bits. Missing mappings hold; native resource-name metadata is
not substituted for the original descriptor pointer.

Camera, angle/revise, SDK matrix, actual queue-work pointer, Main, SetAnim and
draw remain explicit native providers. In particular typed pool Main alone
cannot acknowledge the full camera-facing NPC path. SetAnim force0 returns
before clock lookup when already selected in the original, whereas current
typed selection resolves a clock earlier; the bridge therefore delegates the
complete setter rather than imposing that extra dependency. Providers must own
the same real field camera/queue/animation pass and retain resources through
holds and deferred dispatch. Missing providers never acknowledge empty work.

The cursor uses fixed bounded storage and no per-call allocation. The native
rendering backend still needs actual field Main/draw composition, source
animation buffers and GPU resource lifetimes. Frame-time/hardware benefits and
original numeric parity are not yet measured.

## Validation and next work

Tests check explicit ordinary trace suffix, all queue classes, RGB/alpha,
discarded getter, first/middle/last/no-match table searches, asymmetric and NaN
bounds, double→float-before-factor rounding, fresh handles/scales, exact matrix,
unsupported callbacks, missing providers and every primitive hold in eight
source graphs. Native fixtures use actual VM/registry/shared-pose aliases and
original table/string data; explicit fixture Main/draw callbacks are not
production renderer acceptance. They cover true descriptor pointer traversal,
material persistence after a hold, inactive slot255 and changed mapping holds.

Source evidence and closure hashes are ignored under
`artifacts/npc-main-parent-evidence/`. The original table, strings and numeric
constants are privately extracted and hash-verified; no original source bytes
or assets enter history. See [EVT-01](../../requests/EVT-01.md), which remains
open. Next implement complete offscreen synchronization/GPU bounding boxes and
blur history/material restoration, then compose actual field producers before
activating this path in normal Game. Original runtime traces, field visual
comparison, smoke occlusion/admission and New 2DS XL performance remain open.

The complete local [offscreen and blur callbacks](RETAINED_NPC_OFFSCREEN_AND_BLUR_CALLBACKS.md)
now join ordinary/xlu, superseding those implementation tasks above. Real
field Main/SetAnim/draw, camera/queue and GPU offscreen owners, normal activation
and original runtime/visual/numeric/hardware parity remain open.


## Native animation service availability

The [native NPC animation pass](NATIVE_NPC_ANIMATION_PASS.md) now supplies
retained original SetAnim/Main using the same cold process as Mario. This
optional provider has a separate context, owns these calls exclusively and may
return false after source stores. The callback retains its pending request and
resumes without replay. Camera, queue, original Draw/Inner and GPU owners remain
required; the service is not yet ordinary Game NPC rendering activation.
