# Retained original map group flag parents

`resumePackedMapGroupFlag` implements both complete US parents:
mapGrpFlagOff8001ecb8/118 and mapGrpFlagOn8001edd0/118. The read-only source
verifier pins each whole body. It does not replace the recursive mapFlgOff/On
children with a native visibility overlay.

## Source order

Both functions capture the caller's name/mask, then read active group8041e664
and global work8041e660 and calculate the current group at stride2f4. They inline
this calculation and object scan: **no GetWork or Search function is called**.
A nonnull name scans signed group entry count and each signed object count,
reloading those counts at the original loop boundaries. Entries stride178,
objects134; entry+154 is the actual object array, object+8 its original joint,
and joint word0 the original name. Real strcmp8026d464 compares it to the
captured requested name. Mask bits do not filter the scan.

A found object calls mapFlgOff8001eee8 or mapFlgOn8001f1fc with that exact
object, captured mask and zero. These complete recursive children remain linked
providers; a completed parent requires their actual readiness.

A null or missing name follows the original deferred-name path. It reads the
live index8041e674, addresses record803c97b0+index*24, stores word record+20=0,
then calls actual strncpy8026d5b8(destination,name,31). It does not invent NUL
termination at byte31 or initialize other queue bytes. In particular a null
name still reaches that real child: absence is not a successful no-op.
After copy it reloads the index, stores pending8041e670=1, stores index+1, then
stores the original shift/rotate signed-remainder result. Negative input indices
retain original 32-bit address/arithmetic behavior; no clamp or normalized
unsigned modulo is added. The parent does not store its caller mask in the queue.

## Retention and provider domain

The cursor retains source pointer/index/count progress and exact typed linked
requests with original callsite and argument count. Every memory read/store can
hold through the existing FieldMapSourceServices bool interface. False memory
operations must leave their transfer unaccepted. Held child providers own their
partial effects and return readiness only after their real operation completes.
A successful strcmp additionally supplies its actual scalar return. Void calls
do not require a fabricated result value.

Names/masks are captured once. On/Off function identity, source reader/store
identities, callback table, independent strong lifetime and current predicate
stay fixed. Reentry is rejected; observed retirement permanently invalidates
that attempt. Unknown queue bytes/source mappings remain unavailable. The owner
must preserve actual process memory across holds; this module allocates no guest
work, queue, objects or synthetic guest addresses.

## Existing owner integration and limits

Use the actual FieldMapSourceOwner process globals/heap arrays after complete
battle-map loading selects bank1. The group parent can use its VM read/store
services. Its original strcmp/strncpy operations and deep mapFlg children need
real providers; do not acknowledge them by StageMapObjects name lookup or CPU
array appends. Original queue storage803c97b0 and its history need the actual
process memory producer, separate from the owner globals. The existing field
continuation graph is unchanged by this new module.

Native Stage11 resources can mirror visibility only after actual source flag
mutations and exact mesh/hierarchy/allocation correspondence are established.
No normal Game binding, scene-ready flag, original GPU draw or full Crump battle
receipt is supplied here.

## Focused validation

Strict host and sanitizer fixtures compare every source read, store, child
operand and final word against an independent straight-line reference. Both
parents cover first/later/missing names, signed counts, queue indices0/31/-2,
actual global/count mutation during strcmp, and a changed live index during
strncpy. Every source transfer/child is held twice with no earlier replay.
Null-name copy stays unavailable, and reentry/retirement remain unavailable.
Source verification and ARM syntax are separate; fixture child acknowledgments
do not claim native recursive flag or libc/process ownership.
