# Native battle VM message lookup cache

This native admission adapter resolves actual source key bytes against a retained
copy of the original US room/global resources. It binds the reached
`DrawHelp80117c1c` request for `msgSearch80081b8c` at `80117c80`. It supplies the
successful lookup domain; it does not claim complete original `msgSearch`
execution, localized-miss handling or battle scheduler activation.

## Original behavior and exact resource bytes

The whole US `msgSearch` body is `80081b8c/e0`, SHA256
`ee2ba7bc44a8d684cc45162e620790ba4698524a3d0205613bdd30cf4c83565e`.
`tools/verify_action_class_table.py` pins that complete original body read-only.
The [message resource contract](MESSAGE_RESOURCES_AND_LOOKUP.md) establishes
room slot0 before global slot1, bytewise keys and original raw text pointers.
Neither lookup nor this adapter changes text, IDs, attribution, control tokens or
story conditions. The adapter uses the existing `MessageAssets` precedence and
copies original bytes through and including their actual NUL terminator.

A source miss is **not** an original null result. Original `msgSearch` returns a
pointer from table `804160b4`, indexed by the actual GP language word at `+16c`.
That table and its localized text are not owned by this adapter. Missing keys
therefore hold without returning zero, the key, a guessed English string or a
successful empty result. Ambiguous duplicate entries also hold: the original
qsort/search tie behavior has not been established, and the adapter must not
choose a replacement line. This is an explicit admitted-domain restriction.

The initial snapshot comes from the genuine field `msg/US/global.txt` and actual
room resource via Game's existing field-message snapshot argument to
`TrioAttack::begin`. Runtime entry captures its own immutable copy once. The
lookup owner deep-copies that supplied snapshot before exposing guest pointers;
a later reload through an external mutable alias cannot change battle text.
No formatter or control parser is inserted into lookup.

## Per-VM cache and lifetime

`BattleVmMessageLookup` pins the actual VM weak control block for each call.
All memory callbacks are fixed at creation, and their supplied address-space
identity is metadata for that same VM. Native runtime callbacks lock a weak
`TrioAttackRuntime`; the runtime owns the cache. No runtime→cache→runtime strong
cycle or borrowed public `TrioAttack*` is introduced. The frame callback retains
the actual runtime and checks its own weak public-facade registration before
any cache or key read. Copies of the facade may share one actual VM/cache while
maintaining independent public registrations.

Each frame binding owns a separate retained key cursor. Only immutable entry
mappings are shared across callers. Destroying an abandoned frame therefore
discards its private incomplete key read without poisoning a subsequent genuine
frame; a cursor held by one cache cannot be rebound to another. No successful
registration is rolled back.

Native cache policy reserves `85b00000` through `85efffff` as a four-MiB admission
range. Capacity is not eagerly allocated or a measured original allocation.
Creation rejects any occupied span; each append checks its exact destination
again. `registerBytes` must be a synchronous actual VM registration, with false
performing no stores. Completed registrations remain VM-owned until VM teardown;
there is no unmap/rebind/destructor cleanup or writable guest-cache API. Arbitrary
external mutation of these permanent entries is outside this private VM policy;
this is not a generic mapping-generation or ABA-proof lease.

Existing original class-label mappings at `85a00000` are seeded only after each
actual ID/pointer pair is verified byte-for-byte against the same immutable
resource entry. Conflicting aliases or unknown/changed bytes reject admission.
The new general lookup therefore preserves the same guest pointer for an
already mapped class entry. Equal text from distinct original entries is not
merged. Newly requested entries retain their own original resource identity;
only repeat requests for that same entry share a pointer.

The native key admission limit is 4096 bytes, not an original `strcmp` bound.
A limit or address-space overflow holds without adding a terminator. Sequential
optional byte reads retain the successful prefix and actual NUL across holds;
changed held key pointers and recursive calls reject without consuming progress.
Mapping retries do not reread a completed key or repeat successful registration.
After a completed lookup the next call rereads the actual key, so reuse of a
source pointer for a different ID cannot return an address-keyed stale result.
The snapshot is immutable, so an absent/ambiguous key stays held until its parent
owner is retired; it is never silently rebound to new resources.

## Integration and validation

The unapplied native journal adds a trailing `messageLookup` callback to the
weapon frame binding, captures the original entry resources in the actual
runtime, and routes only the exact reached help request. It preserves
[retained font](../display/NATIVE_QUEUED_WEAPON_SOURCE_FONT.md), Window and icon
ownership. Source guest-key reads use the real Trio address space; unavailable
cells hold. It does not import or rewrite art and introduces no touch controls.
Original physical menu input remains unchanged.

Focused tests cover actual EventNativeRuntime registration, exact original
US help/control-rich entries, room precedence, all key-byte/NUL holds, changed
held pointers, independent/abandoned caller cursors, map holds without prefix replay, source loss after registration,
late overlap, capacity and key bounds, immutable snapshots, duplicates, alias
seeds and VM destruction. A separate test-only implementation view uses the
existing friend fixture to enter actual Trio `begin` and retained UI Init. It
checks real frame lookup, the original class pointer identity, shared per-runtime
cache across copied facades, and rejection after public owner reset. That test
uses an explicit original key input; it is not a Main/Camera/queued-draw receipt.

Strict host, ASan/UBSan and ARM checks use complete private copies of the
6966-input font/icon graph. The actual Trio test includes its production TU;
focused host linkage to the older immutable6944 dependency archive executes no
Game instance or old Trio object. Parent integration must rebuild the complete
new binding ABI and run `make test` and `make all` before acceptance.

Still missing: actual localized fallback ownership, unsupported width markup
`strcmp`/`sscanf`/icon children and any unbound original outer scheduler/provider.
Those children remain honest holds; this cache does not acknowledge them.
