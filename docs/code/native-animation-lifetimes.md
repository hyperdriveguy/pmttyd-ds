# Native field animation lifetimes

[The native field owner](../../source/display/native_field_animation.cpp) shares
one cold process, file cache and pose namespace across Mario and NPC services.
[Release](../../source/display/native_field_animation_release.cpp),
[NPC pass routing](../../source/display/native_field_npc_animation.cpp) and
[NPC Draw admission](../../source/display/native_field_npc_draw.cpp) retain
separate work. See [startup/adoption](animation-startup-publication.md),
[Main ownership](animation-sdk-draw-ownership.md) and
[native Inner](animation-native-render-owners.md).

Field offsets and guest addresses below are hexadecimal; sizes are decimal.
Native clock, address and work-budget choices are port policy, not measured
original timing or hardware memory consumption.

## One source namespace and explicit native timebase

Creation stages authentic files, initializes a cold process with GP0x81000000
and a12MiB arena at0x82000000, then installs borrowing callbacks into that exact
pool. Detached pool copies do not inherit them. The native-owned bus word,
window root, fallback filename RAM and modeling stack support the actual source
VM; their contents are not asserted to be ISO-initialized runtime values.

The bus policy is162MHz with a quarter-bus timebase. Each supplied simulation
visit advances raw time675000, exactly quarter-bus/60. Render/drop/I/O frequency
does not add time. Source retrace publication owns pause/freeze and mode-bank
arithmetic. The clock reader separately reads GP40/44 and38/3c and applies the
source derived divisor using signed division; reading clock does not tick.

ready() proves source alias/currentness, not global quiescence. entryPending()
covers retained Entry. producerPending() covers NPC lease, Mario/publication,
Main and selection; Release has its own cursor. APIs apply their shown guards
rather than sharing a universal mutex. For example tick and direct presentation
configuration use their Entry/producer checks; raw Main, new Mario, new Entry
and NPC admission also explicitly exclude Release. Do not infer a Release guard
merely from a producerPending() check.

## Entry pins source operand and canonical resource identity

A request validates supported mode, exact model name and staged MODEL/TPL.
An original-name operand must already map to those exact bytes including NUL,
without wrapping. That operand remains the source address. Other callers use
known original model tokens when available, or the native256-byte fallback name
reservation. Frankly's optional token requires mapped backing.

An existing active MODEL cache row chooses the canonical resource package,
including its atlas. Name equality must not silently replace its converted
resource with a differently packed package. The captured name/resource/mode/
original-address tuple must resume unchanged through held script calls.

Source Entry completion can precede native adoption. Original Entry preserves
source-2 failure, while the ordinary typed entry facade refuses negative pose
handles. An empty optional is native unavailability. These are different API
contracts, not a reason to manufacture a new valid handle. See
[source Entry](../behavior/display/NATIVE_SOURCE_ANIMATION_POSE_ENTRY.md).

## Release survives partially invalid typed lookup

Initial native admission uses a live typed pose, source buffers and cache
metadata, then pins generation. Once source Release has started, model flags
can clear before a file child acknowledges. Typed pose()/cacheMetadataReady()
can therefore fail legitimately. Retries use the actual retained slot/generation
and raw cursor, rather than rerunning initial typed admission or decrementing
references again.

A completed borrowed/group decrement can leave pose flags live. That success
keeps sidecars. Terminal source flags zero instead retire native buffers,
resource, shape and cache; inactive MODEL/TPL sidecars release their resource
references. Source words, scratch allocation and shared file bytes remain stale
as specified. Selected/sample sentinels invalidate only the released handle.
fileFree changes handle references, not payload collection or GPU residency.
Unsupported smartFree remains a real retained barrier. See
[native source Release](../behavior/display/NATIVE_SOURCE_POSE_RELEASE.md).

## Mario setup and publication form one retained request

Mario bank selection prioritizes event flag0x10000000, then rear0x80000000,
otherwise front. All three handles belong to the shared pool. Upload serial,
selected name, source start, yaw, reflection and root stay captured through
publication. Changing that identity cannot steal an active request.

On a changed handle/upload, retained selection runs once. The original frame/ms
coefficient converts source start time; finite, nonnegative and strictly below
2^64 protects the native integer conversion. Epoch rebasing stores LOW1c before
HIGH18, then paper timestamp8c/88. Later capture/assembly holds do not repeat
selection or setup. Up to three native pumps coalesce only Main-to-capture and
assembly-to-final-swap boundaries. Other holds return normally. Published root,
yaw and reflection change only with successful native shape publication.

## NPC lease spans the entire callback and queue

The owner token is identity, not ownership of a source actor pointer. A pure,
non-reentrant current predicate and camera/source owners stay valid through the
whole pass. The lease blocks competing native producers even between source
children. endNpcPass checks matching owner/pass/current and child quiescence;
the caller must first finish its entire outer callback and drain the queue.
False currentness retains the lease rather than cancelling or rolling back work.

The borrowed NPC service requires exact VM identity and Main configuration
generation. It routes original SetAnim/Main/Draw words directly to their retained
owners, without legacy fallback after false. A held Draw blocks unrelated
SetAnim/Main. Only its Inner phase12 can overlap same-root/same-pass Main.
Native generation/resource/buffer identity is captured at initial Draw admission.
Even an Inner backend reporting success cannot complete Draw while a process
child is still pending. Caller matrix payload is copied, not a fabricated guest
stack pointer. The wrapper's named unfinished linked/register frontiers remain
held. See [NPC lease](../behavior/display/NATIVE_NPC_ANIMATION_PASS.md) and
[unlinked Draw](../behavior/display/NATIVE_UNLINKED_NPC_DRAW_WRAPPER.md).

Destruction unhooks only callbacks still owned by this instance, then retires
Inner/process while their borrowed predicate/request context is alive. The caller
owns quiescent teardown; destructor execution is not source cancellation, GPU
completion, rollback or complete field lifecycle implementation.
