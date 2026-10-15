# Trio field return and original pose resources

The ending suffix in [TrioAttackRuntime](../../source/battle/trio_attack.cpp)
connects the canonical pouch, borrowed Mario/party owners and retained field
constructors. See [ending](trio-ending.md), [native entry](party-construction-events.md) and [removal](party-removal-ownership.md)
and [cadence](trio-cadence.md). This guide also covers the
[enemy resource catalog](../../source/display/trio_enemy_pose_resources.cpp).
All numeric offsets/masks below are hexadecimal; kinds/counts are decimal.

## Three completed prefixes

`endingExported` protects unit-resource export, exact canonical pouch byte copy,
end-work free, pointer clear and source sequence change. It does not rebuild
inventory or heal beyond the source's explicit setup/status policy.
`endingMarioRestored` independently prevents Mario restore from replaying after
party/provider waits. It is set only after the real borrowed Mario owner exists
and restoration runs. Party availability is checked afterward.

`endingPartyReady` protects a third prefix. With a battle partner, its original
kind goes through `battlePartyIndex` and one retained `NativeFieldPartyEntry`.
The root, registry, Mario, pouch and trace belong to that retained owner. A false
step holds the exact constructor; a completed scalar -1 remains a legitimate
source no-entry result. The outer ending intentionally ignores that scalar.
Without a battle partner, one retained `NativeFieldPartyLifetime` runs the actual
whole Mario-party removal controller, including live pose/file waits.

Only whole entry/removal completion sets the party latch. Current-party flag
clear, general rule judgment and audience stop follow separately. If audience
stop holds, that suffix can be revisited, while pouch export, Mario restoration
and party construction stay completed. None of these latches acknowledges the
still-unbound engine/sequence/exit owners. A completed fade, exported pouch or
constructed actor does not prove whole field return.

## Ordinary and positioned field entry

[NativeFieldPartyEntry](../../source/field/field_party_native_entry.cpp) uses
sixteen original spans with exact addresses and byte lengths. Twelve-byte vectors,
60-byte pose rows and the 124-byte Hello dispatch table are different shapes;
loader layout checks complement the converter's original-byte hashes. Existing
complete native mappings stay live; partial spans are refused. A new adapter
cannot overwrite an earlier consumer's mutable source state.

Ordinary kind 1 has the source pouch join gate, primary-slot removal, construction,
placement, index publication and fresh Hello. Position kind 1 and Secondary kinds
11/13 have different source tails. Secondary tests only slot 1; an occupied slot
completes with -1. It does not substitute a free ordinary slot. Raw XYZ bits bind
the positioned invocation, including signed zero. Source state, partial effects
and fixed process/pool/actor identities survive holds.

Model token `0x802cb4bc` is Professor `c_kuri_h`. Its resource first comes from
an actual live same-pool model, then from the retained root's original full bank.
It must have the actual MODEL/TPL backing and name. Entering mode 2 produces the
exact pool handle wrapped by the typed field pose. Sharing the source initializer
with Goombella does not alias Professor's resource or kind. The direct APIs leave
Mario index publication to their caller/EVT wrapper. Terminal Done caches the
original result and is not a reusable constructor.

[NativeFieldPartyLifetime](../../source/field/field_party_native_lifetime.cpp)
checks both native process and pool identities. Four original 40-byte definition
records cover kinds 1,8,11,13; recreating the adapter preserves existing mutable
records. Removal can publish a null public table slot before releasing the pose
or freeing actor storage. The captured actor stays readable through `retiring_`.
Keep the adapter through completion; abandoning its lease cannot roll back source
stores or stand in for Kill. Whole Mario removal can legitimately nest its own
Kill children, while unrelated public Kill/event calls are excluded.

## Enemy catalog admission

`TrioEnemyPoseResources` loads the shared source directory/CPU bank/raw resource
and requires model and texture names `c_kuribo`. MODEL flags at `0xc4` must match
the resource flags. Descriptor count at `0x148`, supplied names and discrete
metadata must agree and be nonempty. Every source-order descriptor must have a
CPU pose with original array dimensions: positions `0xf0`, normals `0xf8`, nodes
`0x140`, visibility `0x13c` and groups.

Texture header count `0x134` indexes 64-byte rows at `0x198`. Header +4 identifies
a TPL image; +8 must be zero in this admitted domain. Every referenced image must
exist, even if the initial pose does not reach it. The wrapper checks the whole
supplied directory rather than hardcoding 45; the original fixture separately
verifies the actual 45-descriptor bank. Equal sizes and names establish this
resource domain, not original CPU/GPU numeric or pixel parity.

`find(model,initialName)` requires exact model/name membership and returns the
same shared resource. It never chooses by visual actor index, enters a pose or
creates a current draw lease. The runtime separately owns pose generations,
allocation/camera checks and GPU submission. See
[menu/pose admission](trio-menu-frame-bindings.md). This focused review does not
prove completeness of every numeric field in the large Trio runtime.
