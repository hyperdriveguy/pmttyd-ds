# Native party removal ownership

`NativeFieldPartyLifetime` binds the [retained source removal controllers](RETAINED_PARTY_REMOVAL.md)
to actual `FieldPartyRegistry`, `FieldMarioBattleState`, `NativeFieldAnimation`
and its source file/cache owners. It implements both Kill variants, the original
Mario remove-all caller, and clear/kill event wrappers over a live EventEntry.
This is not a complete party constructor or an authored story conversion.

## Original data and admitted owners

`tools/field_party_definition_assets.py` reads the original US revision0 DOL
read-only. The ignored `romfs/field-party-definitions.pmevt` contains exactly
three unchanged40-byte records (168-byte pack including its directory):

| Kind | Original record | Model token | Original model | SHA256 |
| --- | --- | --- | --- | --- |
| 1 | 80314410 | 804213e0 | c_pkr | 66223e5e99847e374a78c965305df7da505db3794f40503a107dbd8c5e2e0e3c |
| 8 | 80314528 | 802cb47c | c_babyyoshi | 0736c199924450fd0f59d4b42682e20fdd5086e0d8c6920f37fc3a4195aebb9d |
| 13 | 803145f0 | 804213e0 | c_pkr | 8f4f2986bff8e23c7b9fd34a86ee6079a4672253779644f67188345792f3f6d9 |

All three records have init800c4fa0 and genuine exit+24=0. Sharing that init
callback does not make kind8 a Goombella model or kind13 a canonical kind1 join.
Complete record bytes, pointers and other callbacks remain unchanged. Model
strings and constructor dependency graphs are not added by this small pack.

The loader validates layout and critical fields. With a native animation owner,
it imports records once into that actual process VM. Already mapped complete
records remain mutable; a later adapter does not restore modified callback data.
Partial mappings reject import. A real nonzero exit callback remains a genuine
unavailable child; no successful noop replaces it. Mode11 still skips it through
the original source controller.

Native adapters expose semantic actor/table/Mario views to their controllers;
they do not install a complete global party address facade for unrelated VM code.
Native actor tokens85800000 and85800200, stride200(hex), encode two currently
backed native slots; they are not recovered SDK heap addresses. Actor fields are
actual188(hex)-byte big-endian records. Mario's original245/246 and247/248 views
alias the typed arrays, currentDefinition aliases the registry word, and
original definitions/file/pose data reside in the actual animation VM.

The native index domain is negative inputs plus0/1. Raw source controllers retain
their original unchecked domain. Live actor removal requires the same real native
animation/pool owners captured by the adapter, a live typed pose and no other
active source Release. This admission occurs before actor publication; an admitted
removal resumes its raw source state across later typed invalidity. Detached legacy cache-only pools
cannot acknowledge source fileFree; empty negative/null removal still completes.

## Ownership through publication and holds

The initial actor is captured without freeing it. At the source table-clear
store, its unique_ptr moves from the public registry into `retiring_`; getters
then return null while the captured native actor remains backed. The later pose
ID read therefore still accesses P after table publication.

The child calls the real [native source Release](../display/NATIVE_SOURCE_POSE_RELEASE.md),
including actual fileFree. A source borrowed/group early return keeps the live
pose and its typed FieldPartyPose wrapper intact for other owners. During actual
teardown, the wrapper retires as soon as model flags make typed lookup invalid,
including a held file child; terminal success retires it in the same native call
before any new Entry can reuse the handle. Its direct word/shape proxies therefore
cannot adopt a new pose lifetime. This native view retirement does not free the
actor or replace source animation retirement. A wait after model.flags clears
preserves the actor, source release cursor, captured pose argument and remaining
sidecars. Other pool consumers still own retirement of any surviving borrowed
role views when they release that handle; no general collector observer is added. The final
`__memFree(0,P)` child resets the captured unique_ptr only after Release completes.
Allocation and disposal here use the same native C++ actor heap policy; this is
not the original SDK heap implementation.

An expiring native exclusion lease blocks other adapters and allocateFirst while
removal waits. It is not an original party flag or source index check. A copied
idle registry does not inherit it. The adapter owns a shared token, the registry
a weak token; default adapter destruction never accesses borrowed registry or
Mario. This matters because whole Game teardown destroys some field members
before the battle wrapper. Lease expiry on context disposal does not complete
an original Kill. Normal callers keep adapter, registry and Mario stationary
until completion; active cancellation/room rebinding is not provided.

## Actual EventEntry and ending composition

`event` accepts EventEntry+18/args reads from the same live native animation VM.
Native admission bounds the raw slot to0/1 before starting; it does not evaluate
an operand or create a fake entry. The original wrapper then captures Mario,
entry arguments and child index in its normal order. Held events keep result
unchanged; successful completion publishes2. Wrong entry/clear mode/VM or
interleaved direct/outer requests cannot replace a held invocation. The actual
EventEntry and its argument backing must stay alive across holds; a raw address
is not allocation-lifetime proof. Entry deletion/reuse and callback cancellation
remain the caller/scheduler's responsibility.

The battle ending's no-party path now owns an adapter and resumes the complete
original marioPartyKill80141198 parent instead of stopping at an unconditional
Kill barrier. Source Mario restore and a fully completed party constructor/
removal are each latched while later battle services remain held. This prevents
completed publication from being interpreted as a fresh old-party removal.
The adapter pins the resource root when the runtime is created; later global
asset reloads do not retarget that runtime.

The joined existing-primary branch still stops at its Kill frontier: its ordinary
constructor must become restartable at every resource/material/appearance hold
before removal can safely be bound there. Auxiliary Entry2Pos(kind13), authored
harbor/plaza conversion and complete battle field return remain open. No source
unlock/story/message write, early canonical join or fabricated scene is added.

## Evidence and validation scope

Native fixtures use actual cold animation VM/pool/file owners and original
records, explicit raw actor fixtures, and real mapped EventEntry storage. They
cover shared c_pkr resources, borrowed early return and later teardown, a genuine file-manager hold after table clear,
retained actor/proxy ordering, competing leases, wrong requests, twenty warm
sample/release cycles, stale source readers, raw kind preservation, unknown exit
hooks/mode11, lease expiry on adapter disposal, actual event operand mutation,
remove-all through its second child hold, another pool consumer already holding
Release, Mario survival and detached copies.
Kind8 fixtures test raw kind/removal semantics with an explicitly retained pose;
they do not claim a c_babyyoshi constructor or renderer.

Existing Trio regression checks retain one completed party entry/constructor
across110 later visits. The definition extractor has independent read-only ISO
hash/byte/layout/metadata checks. Source and native graphs were independently
reviewed without edits. Hardware performance, original Gekko execution/numeric
parity, full scene scheduling/cancellation and complete prologue parity remain
unproven.
