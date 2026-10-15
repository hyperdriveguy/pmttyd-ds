# Atomic native command-pose admission

US G8ME01 original CommandAnimPose801264cc/8c and its complete linked helper
contracts are already implemented in `battle_command_pose.cpp`. The new
`nativeBattleCommandPose(EventNativeRuntime&,unit)` binds that existing owner;
it does not duplicate its source graph or require rendering acknowledgement.
[Source command-pose recovery](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
records status priority, actual type67 lookup, strcpy and individual flag writes.

Actual `checkPackedBattleStatus` performs total source reads. All possible
unavailable decisions in this binding precede the existing helper's first guest
write. Status branches that use PoseAnime have no unavailable child. The normal
command branch resolves the actual pose-name pointer, then the native copy
provider preflights original source bytes through the terminating NUL, requires
a nonempty name fitting the64-byte part buffer, rejects source/destination
overlap, and admits the actual contiguous writable buffer plus flags. It then
performs original sequential byte reads/copies including NUL. The remaining
original SetPose/CommandPose flag operations cannot suspend. No scene actor,
GPU residency, animation playback or invented pose name substitutes for that
source memory operation.

Name/destination admission failure returnsfalse before any guest mutation.
The caller freezes actual mapped unit/part/name/table storage through this
synchronous leaf. Losing a preflight guarantee during the copy is a violated
native memory invariant, not a retry-safe partial completion. The nonempty,
bounded, disjoint and contiguous requirements are explicit native policies;
they do not claim original strcpy has those guards or supports every overlap
case. Actual original command names meet this domain. This leaf performs no
story, input, timing, sound or rendering operation.

The real VM fixture imports the accepted original tutorial event pack, runs the
existing native provisioner and source-data actor registration, and resolves
original Mario's type67 to actual `M_C_3` bytes. It verifies copied bytes and
individual resulting source flags, the status-flag PoseAnime route without GPU
work, and missing actual name backing refusing the copy/flags. It uses a frozen
core/header/RomFS graph consistently; live headers mixed with an older archive
are invalid because independent work changes native class layouts.

Strict host actual-VM execution and ARM GNU++17 syntax pass against the root's
frozen Phase/Act validation tree. Existing read-only `verify_command_pose.py`
pins eight complete source bodies and original priority27,2,9,1,4,5,3. No source
asset, original pose table or image is changed. The unapplied native journal
binds Input's actual801264cc child to this atomic owner and preserves missing
admission as the genuine source-function frontier. Full runtime validation and
build registration belong to root.
