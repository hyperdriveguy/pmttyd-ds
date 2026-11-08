# Retained field Mario preparation and return

`field_mario_battle_transfer` implements whole US `marioFBattlePrepare8005c3cc/6c`
and `marioFBattlePost8005c30c/c0`, including whole `partyUsePost800bcf34/5c`
and both-slot `partyCtrlOff800bed44/38` / `partyCtrlOn800bed0c/38` bodies.
The read-only verifier pins these five bodies and the complete current-party
lookup/getter bodies. Existing instruction listings are in ignored
`artifacts/recovered-temp-evidence/field-mario-prepare.dis`, `return-post.dis`,
`field-party-usepost.dis`, `field-party-off.dis`, `return-party-control.dis`,
`native-field-party-id.dis` and `native-field-party-ptr.dis`. This adds no
original binary assets. See [original ownership and call order](MARIO_OWNER_CONTROL_DEPTH_AND_BATTLE_PREPARE.md)
and the [field return handshake](../battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md).

Prepare captures the actual global Mario pointer once. It reads both position
X/Y words before either saved-position store, then reads Z after both stores.
The direction child finishes before the fresh motion20 test and conditional
main1000 store. The original current-party ID/pointer children precede the
whole use-post body; mode3 reads signed kind, then the actual table callback
and calls that exact nonnull function with the captured party. Both global
party slots are visited independently and in original order.

Post clears main1000 before its motion20 test. Only motion20 restores saved
XYZ; X/Y are paired reads and Z remains late. The motion26 check is fresh,
as is the final motion18/20 check after party control. A nonnull current
party reaches the real appearance child before source stores to party58/5c/60.
Its private native vector begins unknown. Missing output components hold at
their actual consumption points, with paired X/Y and late Z reads; no guest
stack address or default zero is invented.

Each successful read, store and child is retained exactly once. A false store
must have performed no write; a child may partially execute before false and
must retain its own continuation. The same typed request and vector persist
across retries. A child reporting completion without required output is not
called again. Service user, callbacks, shared lifetime pointer and control-block
identity are fixed at first admission. Recursive resume and swapped services
are rejected. Losing the current-owner predicate permanently rejects that
cursor before further accesses; resurrection of an address does not restore
admission. Destruction and backing replacement are single-threaded and
quiescent; destroying the owner during its own callback is unsupported.

## Concrete native field adapter

`native_field_mario_battle_transfer` binds actual `FieldMarioBattleState`,
`FieldPartyRegistry`, `MarioPresentation` and `FieldMovement` owners. The host
supplies actual live motion and engine-space XYZ word readers/stores. These
are narrow typed native field views; they do not certify a complete original
Mario allocation, original constructor execution or a full source address
space. Encoded native Mario85700000 and party85800000/stride200 match existing
native field-owner policy. The only internally owned source globals are the
three saved-position cells; they remain unknown until actual Prepare stores.

Host and facade registration proofs are checked before the external current
callback or borrowed pointers. The callback must also prove the actual current
Game host slot and Encounter facade/runtime before borrowing Game. Copies or
restores must obtain fresh proofs and cannot reuse another facade's binding.
The host retains all typed owners stationary for the adapter lifetime. An
expired proof latches stale; no borrowed object is dereferenced afterward.

Native current-party lookup uses the real signed Mario index bytes and actual
party records, including main8 suppression. Source nonnegative indices outside
the bound two-slot registry hold rather than reading invented pointers.
Both-slot control changes actual party main20000000. Mario control services
use the existing exact wrapping depth operations, without resetting depth.

Ordinary direction adjustment uses the existing native source helper and
publishes its actual travel heading vector. It checks finite input/composition
and the supported signed conversion domain before any direction mutation.
Special main00100000 remains unavailable. This is the established native
numeric adaptation, not a claim of Gekko/libm bit equality. The host must bind
the authoritative actual field main word, not an unrelated display projection.

A genuine null current party avoids ability and appearance domains; no visible
Goombella sprite is treated as a constructed party registry entry. Mode3 ability
table words remain unavailable in this sparse native view. Nonnull appearance
and motion26 / motion18/20 actions require actual supplied child providers.
Those missing services retain the source prefix; they are not boolean success
acknowledgments. Source start/wait, sound flags and BGM retirement remain the
separate field sequence/backend owners. The native sequence policy does not
claim original battle Main's additional control off/on pair.

## Validation and integration limits

Direct original-body references compare complete ordered successful memory and
child traces across Prepare/Post, null/nonnull parties, motion0/18/20/26 and
source mutations. Every boundary is held twice. Tests cover captured Mario,
paired X/Y/late Z, negative ability kind, fresh motion rereads, partial appearance
work, unknown private output, recursive calls, owner replacement and owner death.
Native tests exercise real typed field/party effects, null-party prepare/post,
both slots, genuine special-child holds, finite rejection and proof-before-borrow
after actual host destruction. No Game/Renderer/Audio production hookup or
whole battle End/field return is asserted by this package.
