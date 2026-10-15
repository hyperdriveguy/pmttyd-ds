# Party removal controllers and native ownership

[Portable parents](../../source/field/field_party_lifetime.cpp) and their
[cursors](../../include/field/field_party_lifetime.hpp) implement Kill, Kill2,
clear/kill event wrappers and Mario's remove-all caller. The
[native adapter](../../source/field/field_party_native_lifetime.cpp) and
[API](../../include/field/field_party_native_lifetime.hpp) connect them to actual
party actors, Mario arrays and source animation/file owners. Constructor/story
integration and complete field return remain separate work.

The portable controller preserves unchecked source addressing. Negative index
or null initial table actor completes without teardown. Otherwise it captures
actor P and owner M, resolves the signed linked index and kind, publishes the
current definition, and conditionally invokes its exit hook. The source initially
reads linked+0x31 even when linked is null; native availability must hold rather
than invent a kind. Mode11 bypasses a nonzero hook; a zero hook skips mode lookup.

Kill rereads the owner index after the hook, null-tests the linked table, then
loads that table entry again before reading the saved kind. Kill2 directly saves0.
Both save kind, mark the captured owner index absent, clear the public table,
then read P's pose and release/free. They do not add HP, unlock or global-definition
cleanup. The captured actor must remain backed after table publication.

Memory false/empty returns have no effects. A retained child can make internal
progress before returning false: its own continuation must resume, with parent
arguments pinned. Completed parent primitives do not replay across a later hold.
Success resets the parent cursor. Outer transitions latch completion so a later
unrelated service hold cannot restart removal.

Event wrappers read actual Entry+0x18 and raw args[0], without operand evaluation.
Slot0 uses the captured signed primary byte and Kill; any nonzero raw slot uses
literal index1, with Kill2 only for clear. A negative byte completes immediately.
Result2 publishes only after the child completes. Mario's remove-all queries
Mario separately for primary and auxiliary, including an absent primary; it uses
the actual signed index, Kill, then an explicit ff store to that captured owner.
A negative byte skips the child and explicit store. This caller is void.

Native policy deliberately has narrower admission than the raw parents:

| Native boundary | Meaning |
| --- | --- |
| Negative/0/1 indices | Two actual backed actor slots; no upper-domain source rewrite |
| Actor tokens85800000/85800200 | Native address policy, stride0x200 over actual0x188-byte records |
| Kind1/8/13 definitions | Three unchanged source records, with separate original model identities |
| Live removal | Same ready native animation/pool, live pose, no competing release before publication |
| Event raw slot0/1 | Actual same-VM entry/argument admission, without fabricated event storage |

Definition import is cold-only. Already mapped complete records remain mutable;
recreating the adapter cannot restore a changed exit callback. A genuine nonzero
hook has no successful native substitute. Detached legacy pools cannot acknowledge
source fileFree, though negative/null removals need no live teardown.

At table clear, the actor unique_ptr moves into `retiring_`; public lookup becomes
null while captured reads still resolve to backed bytes. Actual source Release
can hold after model flags invalidate the typed pose. The typed pose wrapper
retires at that invalidation, including a held file child, before handle reuse.
A borrowed/group early return instead retains the live pose and wrapper. The final
heap0 free disposes of the actor only after Release finishes. This uses native
C++ actor allocation; it is not the original SDK heap implementation.

A shared/weak exclusion lease serializes native construction/removal consumers.
Its expiry lets whole-context teardown avoid accessing destroyed borrowed owners;
expiry does not complete or roll back Kill. Normal active cancellation/rebinding
is not provided. Keep adapter, registry, Mario, entry and argument backing stable
until completion. Entry address equality alone is not allocation-lifetime proof.
The native event adapter pins its memory until success, preserving result on hold.

See [portable source evidence](../behavior/partner/RETAINED_PARTY_REMOVAL.md),
[native ownership](../behavior/partner/NATIVE_PARTY_REMOVAL.md) and
[source pose release](../behavior/display/NATIVE_SOURCE_POSE_RELEASE.md).
These local controllers do not establish full authored-story or hardware parity.
