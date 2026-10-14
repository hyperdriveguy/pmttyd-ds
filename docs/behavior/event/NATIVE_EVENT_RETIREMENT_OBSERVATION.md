# Native event retirement observation

EventCreationServices and EventNativePlatform optionally supply a read-only
retiring callback. deleteEventEntry invokes it after recursively deleting
children/brothers and performing actual attached-child local-bank copy-back,
immediately before clearing the event and decrementing allocation. The callback
must not mutate ownership or recurse into event deletion. A null observer leaves
source event behavior unchanged. This is native diagnostic/result observation,
not an emulated original world callback.

FieldAreaEvent uses this point to capture actual final locals. Opcode RETURN
marks returned=true; forced retirement is returned=false. Retirement alone does
not establish successful story completion. Tests verify grandchild/child/parent
observation order, forwarding through EventNativeRuntime, copied locals and
clearing afterward. The [area parent contract](../field/NATIVE_ORIGINAL_AREA_PARENT_AND_RETIREMENT.md)
explains why callback-time snapshots were insufficient for late child outputs.
