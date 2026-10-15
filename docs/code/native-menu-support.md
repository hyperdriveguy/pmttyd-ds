# Native menu queries, presentation, poses and labels

These adapters preserve actual original memory operations while using explicit
native private outputs. Actual VM/actor/resource lifetime and synchronous mapping
serialization are required. See [menu controllers](battle-menu-continuations.md)
and [ActMenu](battle-act-menu-continuation.md) for their parents.

## Audience item query and physical X

[ItemOn](../../source/battle/battle_audience_item_on_native.cpp) captures the
cache base and member start from separate current-battle reads. It scans 200
0x134-byte members, requiring flag 1 and excluding 0x100/2. Signed held halfword
+0x1c rejects only -1. Its 0x48-byte item lookup uses another fresh global B;
signed item state <=4 or exactly 7 admits, including the source negative-state
domain. No harmful-item-only filter is added.

The first captured cache base receives selected index +0x138bc and XYZ
+0x138c0/4/8. Z is fresh member Z plus original offset 3. Index-and-position
output rereads cached XYZ after these writes, rather than reusing detached member
values. Actual source-pointer/global changes remain observable. No match returns
private index -1/absent XYZ while preserving old cache words. Supported signatures
have disjoint native outputs; arbitrary guest-output aliasing is not admitted.

[Physical-X interruption](../../source/battle/battle_menu_audience_native.cpp)
captures B before Trigger 0x400, applies original battle/Mario blockers and uses
that first-eligible query, including good items. The private selected index survives
cache changes. SetTarget performs its own fresh-global operations; actual End
must finish before retained-B previous/type/input/flag writes. A held End cannot
repeat query/cache or target effects. The original OffLight leaf returns only;
acknowledging it is not GPU submission. Engaged false/true are completed results,
while `nullopt` retains the child. Allocation proof guards synchronous groups and
external boundaries; observed loss poisons until reset. Reset does not roll back.
See [physical-X ownership](../behavior/battle/NATIVE_PHYSICAL_AUDIENCE_INTERRUPT.md).

## Original class messages

[Class-message mapping](../../source/battle/battle_class_menu_messages.cpp) pairs
seven original DOL key pointers with exact US message IDs. Its list order is
mapping metadata, not ClassTable row order. The immutable MessageAssets snapshot
supplies actual room/global lookup; no generated English label or key fallback
replaces missing bytes. Original bytes, control tokens, high bytes and NUL survive.

The complete bank is built before one registerRam call at an explicit nonzero
unused guest base. Wide arithmetic rejects overflow while admitting an exact
end at 2^32. Actual source-pointer aliases share a guest pointer; identical text
in distinct entries remains distinct. Lookup requires the same EventAddressSpace
object, but that is not registration-generation/ABA proof. Callers must not write,
unregister or rebind this cache. The facade owns copied RAM until VM teardown;
wrapper destruction deliberately leaves it mapped. Binding pins wrapper/resources,
and the actual VM must outlive every lookup. See
[message lifetime policy](../behavior/battle/NATIVE_CLASS_MENU_MESSAGES.md).

## Atomic command-pose copy

[CommandPose](../../source/battle/battle_command_pose_native.cpp) reuses the whole
source selector and actual packed status services. Its normal name-copy route
preflights extra reads before the first guest mutation: 1..63 non-NUL bytes plus
a terminator, nonwrapping part extent and disjoint actual copied spans including
NUL. Destination part+0x1c4 is a 64-byte name; the required contiguous 0x44-byte
span also covers the following flags through +0x208.

After admission, original sequential source rereads/writes, including NUL, run
before the selector's remaining flag stores. The caller retains immutable name
backing and fixed actual mappings throughout. If that guarantee fails, abort
marks a violated native invariant, rather than retrying a partial copy. Empty,
missing, overlong or overlapping names fail admission without source writes.
These are native domain guards, not recovered strcpy checks or proof that a GPU
pose/animation has been presented. See
[atomic pose admission](../behavior/battle/ATOMIC_NATIVE_COMMAND_POSE.md).

## PresentItem recovery and effect barrier

[PresentItem](../../source/battle/battle_present_item_native.cpp) captures B before
separate fresh-global item/target queries. Signed item >=339 or target outside
[-1,63] excludes; actual unit lookup still precedes item-zero/negative rejection.
Generic items publish pending 1 (2 for Input states 12/13), apply the original
0xffeeeeee menu mask, preserve previous type, publish type 12/Input 29 and return
1. They leave the present item for later source execution.

Items 123/124 apply original HP/FP +1 once, then retain raw XYZ, signed height,
conversion bias, unit scale and original constant 5. The actual subtraction-double/
single-round result precedes private XYZ consumption. FMA(scale,height,5) and
separate addition of old Y remain distinct required providers. Their operands
and the resulting owned position survive waits.

Actual effRecoveryEntry receives kind 0/1, amount 1 and the same XYZ object.
Missing math/effect holds without repeating recovery, clearing the item or
inventing an effect. Only completed effect permits the fresh-global present-item
clear and local return 0. Actual allocation proof is checked around groups and
children; observed loss poisons until reset. Native disjoint XYZ does not claim
arbitrary guest alias or original FPSCR equivalence. See
[typed PresentItem](../behavior/battle/TYPED_NATIVE_PRESENT_ITEM.md).
