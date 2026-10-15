# Native party construction and position events

[NativeFieldPartyEntry](../../source/field/field_party_native_entry.cpp) and its
[API](../../include/field/field_party_native_entry.hpp) retain three original
constructor paths. [The position-event bridge](../../source/field/field_party_position_event.cpp)
and [API](../../include/field/field_party_position_event.hpp) resolve actual event
operands and publish the resulting slot. [Removal ownership](party-removal-ownership.md)
explains the separate Kill/release owner.

| Form | Admission and final behavior |
| --- | --- |
| Ordinary caller | Kind1 plus canonical pouch join; can remove an existing primary; appearance, previousXYZ, mode1, primary publication and fresh Hello |
| Ordinary Position | Kind1; no join/removal; supplied XYZ, two vector passes/display queries, pose choice, mode0; no previousXYZ or Mario slot publication |
| Secondary Position | Kind13; only slot1, occupied returns-1; no HP query; mode2, supplied XYZ/previousXYZ/unit scale; no ordinary appearance/Hello/publication |

Kinds remain distinct. Sharing c_pkr or an initializer does not turn auxiliary13
into an ordinary joined partner. The separate authored pouch-join callback owns
join state. Unknown kinds, d_mario and unbound cleanup/Hello branches hold rather
than completing substitute work.

The loader checks twelve unchanged span addresses/sizes for vectors, overwritten
stack seeds, names, dispatch, pose rows, dimensions and unit scalar. With a native
animation owner, it imports only absent complete spans. Existing live mappings
remain mutable; a new adapter cannot overwrite earlier consumers' state. Converter
source hashes establish original-byte provenance separately from loader layout
admission. Grouped vector/seed reads assume complete admitted mappings, without
instruction-level recovery from arbitrary partial RAM faults.

Ordinary caller checks join before any removal. It captures/requeries a current
primary index before retained Kill, then allocates once. The published actor and
entry lease survive resource, pose, paper, material and string waits. Slot/pool/
native-owner changes reject resume; caller-owned resources and providers remain
stable. Done stays terminal, preventing a held outer transition from replaying
construction. Disposal expires the lease without inventing teardown or touching
borrowed registry/Mario during destruction.

After paperOff, capture the actor's Mario owner. Two independent animation-ID
queries supply material flags and light flags. Each getter handle/mask remains
retained through the setter hold, with a fresh partner pose handle for each
setter. Release uses actual native source ownership; typed wrappers retire when
source validity ends, including a held file child.

Ordinary live signed HP<=5 chooses pose selector12; otherwise1. Auxiliary row1
skips HP. Main's two string comparisons stay distinct, comparing actual guest
NUL bytes even for different aliases. The second comparison still runs with
fresh0x8000, suppressing only its eventual name update. Missing string data holds
the captured operands. Current definition is reread for the reached initializer;
an unknown initializer does not acknowledge success.

The ordinary appearance tail performs two distinct display calls, placement,
previousXYZ and pose/mode before primary publication. Fresh Hello performs another
placement and live pose choice; it does not rerun construction or materials.
The appearance adapter publishes its value-output XYZ after the helper returns;
arbitrary reentrant observations of original intermediate output aliases remain
outside that adapter. The source boolean placement return is ignored, while
missing providers still hold.

Position tails differ deliberately. Ordinary Position reads an initial vector,
an overwritten seed, supplied XYZ, owner and a final vector before display and
pose selection. Auxiliary saves eligible old mode, then requires the bound fresh
old0-to2 inner leaf before publishing mode2 and its position/scalars. Other old-mode
cleanup remains unavailable.

The event bridge captures actual Entry pointer/id, args address, memory and
previous/explicit mode. It reads raw selector even for initial first=false; that
initial call completes2 without construction. Existing nonnegative signed Mario
index also completes before resolving kind/coordinates. A later VM first=false
cannot discard an invocation already waiting on native construction.

Explicit events resolve kind then X/Y/Z with original integer getEventValue.
Previous events resolve X/Y/Z first, then signed saved kind; saved0 skips creation.
Encoded event floats therefore truncate before signed32-to-float coordinates.
Selector0/1 is native admission; raw source arithmetic has a wider unchecked
domain. Provider identity pins once operand resolution starts. Nonnegative
constructor result publishes its low byte to the captured selector only after
full construction; negative result completes without slot publication.

See [ordinary evidence](../behavior/partner/NATIVE_ORDINARY_PARTY_ENTRY.md) and
[position/event evidence](../behavior/partner/NATIVE_PARTY_POSITION_EVENTS.md).
These local services are separate from complete authored event dispatch, all-kind
support, original numeric/visual parity and hardware performance.
