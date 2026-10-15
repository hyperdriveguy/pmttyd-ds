# Native NPC room reset and retained event VM

Implements the admitted `npcReset` body and the NPC/resource/event interval of
[original room teardown](ORIGINAL_NPC_ROOM_RESET_AND_TEARDOWN.md), using the
[retained registry owner](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md) and
[map preparation](NATIVE_MAP_NPC_HISTORY_BOOTSTRAP.md). It does not acknowledge
complete map unload or provide the currently missing world resource services.

`resetPackedNpcRegistry` uses the explicit signed argument: zero ordinary,
every nonzero alternate, independently of live GP mode. It reads capacity then
array, clears the captured byte span, stores selected active count0, freshly
reads GP/F and clears only F's mode halfword for ordinary reset, then clears
npcMainCount. No pose release, event deletion, history memset, auxiliary reset
or new F allocation occurs. Native `FieldNpcRegistryBinding::reset` pre-admits
initialized retained storage, actual selected capacity/array identity and
ordinary GP+168 pointing to that same owner's F before effects. Its full clear
uses contiguous retained allocation storage and native libc memset, avoiding
per-byte original-address dispatch. Corrupt/foreign destinations hold intact.

`FieldAreaEvent::retireMapNpcs` is an explicitly requested interval after the
caller's actual Mario/map teardown. It first runs source npcReset0 once, then
requires these synchronous source services in order:

| Function | Service |
|---|---|
|800d6b7c|extReset|
|800612f0|mobjReset0|
|80073394|offscreenReset0|
|80071828|imgAutoRelease0|
|8005ecd0|effAutoRelease0|
|8003b028|animPoseAutoRelease0 including real refresh|

The caller provides no-effect complete admission and stable service identity.
A false step means incomplete; the provider retains any internal progress and
must not repeat effects within that leaf on retry. Completed steps never run
again. Missing admission/step callbacks do not reset actors. Callback reentry
and retirement during a manager pass hold. Source ordinary GP mode is required,
and rechecked before context reset. A live unselected battle context is outside
this adapter's admitted ordinary-room transition domain.

Once reset starts, scheduler passes and new map registration/root construction
are suppressed until all six services complete. Original event-context ReInit
then clears selected entries without evtDelete, retirement callbacks, child
copyback or constructed completed-root results; actual case ReInit follows.
Native previous-party/NPC-setup cursors, old roots, pending diagnostics and the
parsed-tag mapping are invalidated separately. Actor binding and F stay alive.

The same VM survives. Its context work/flags/field94, next event ID, allocator,
entry allocation identities and unselected context are retained as source
ReInit requires. The next `begin` selects a supported room and registers its
original battle area in that VM, without repeating context Init or cold NPC
Init. It requires the same immutable loaded program; changing REL/data closures
needs its actual load/unload owner and currently holds. The caller still supplies
the next actual GP snapshot, including retained F identity. Unsupported room
selection leaves the retired state available for a supported selection.

The initial implementation review caught two lifetime hazards before final
validation: destroying/recreating the VM would have lost source-preserved banks
and IDs, and recursive external callbacks could replay a pending service.
Retaining the VM and guarding callback entry address both. No normal Game
bootstrap activation is claimed by this standalone area lifetime API.

Tests exercise pure source fresh GP/F after clear/count writes, argument-based
bank selection including negative selectors, zero-capacity memset invocation,
and native original actor/pose data. They verify full selected clears, retained
other bank/aux/F/history/poses, malformed admission with no partial clear,
retained alias rebinding, source service order and holds, manager/reentrant
admission, actual root invalidation without return publication, and next-room
context banks/ID preservation. The sequencing fixture deliberately uses external
resource test providers: it proves orchestration and no hidden pose release,
not the real image/effect/pose release/refresh implementations. Native ARM
fixtures have the same limitation. Normal-world teardown, original shared file/
pose ownership, source module/map/Mario cleanup, RNG interleaving and full
field/rendering correctness remain separate required owners.
