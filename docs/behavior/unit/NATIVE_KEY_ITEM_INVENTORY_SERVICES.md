# Native key-item inventory and event services

Source contracts: [inventory storage](STATE_AND_INVENTORY.md),
[removal/compaction](INVENTORY_REMOVAL_AND_RESERVATIONS.md) and
[Goomba4 inventory wrappers](../field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md).
This implementation binds only the original key-item array and generic event
wrapper protocol; other item categories, parameter revision, full saved storage
and positive-item quest playback are separate owners.

`packedKeyItemCount` and `removePackedKeyItem` admit original IDs1..120 and
operate on all121 signed halfwords at pouch+a0. Counting includes copies after
zero gaps. Removal finds the first match, clears it, then copies/clears every
remaining active slot including zeros. It never scans/changes the carried array
beginning immediately after the key array, nor packs nonzero entries together.
The low-level return is0/1; valid original-address extents are the caller's
provider domain. Native invalid access policy is not an original bounds check.

`checkItemEvent` caches the original argument stream, resolves item, delegates
the count, and then reads/writes its destination with event return2.
`removeItemEvent` delegates removal; only result1 invokes Mario then party
parameter revision and writes0. Other results write-1 without those revisions.
It retains late destination reads across callbacks. Revision providers must be
supplied before this callback is used; missing revision is not a success no-op.
These wrapper services can delegate other source categories when implemented;
they do not force every item through the bounded key-array primitive.

The live ProloguePouch object exposes count and low-level removal adapters over
its retained original big-endian bytes. Low-level removal does not run revision
or write quest flags. Current field availability only queries counts95/107 for
the exact Goomba4 profile after matching known quest/true-flag gates. A known
zero live count can exclude that source quest arm and reach unchanged ordinary
US dialogue. Missing original storage remains unknown even when the native
mushroom quantity is0 or99; positive counts keep unsupported quest playback
blocked. Other actor profiles do not scan this key array.

Inventory observations are transient and separate from persisted saved-switch
knowledge. Existing native checkpoint files do not serialize this original pouch;
no fake import or checkpoint format change is introduced here. Future complete
quest binding must preserve actual inventory across saves/rooms, provide both
parameter-revision owners and use the
[item-selection core](../message/NATIVE_ITEM_TABLE_SELECTION_CORE.md), rather
than substituting native resource counters or message Yes/No choices.

Validation and limitations are recorded in [the handoff](../../PROGRESS.md).
Tests cover gaps/duplicates/first-match compaction/write order/active extent,
wrapper0/-1 and revision order, live original pouch guards and missing storage.
Hardware profiling and complete source inventory/runtime parity remain open.
