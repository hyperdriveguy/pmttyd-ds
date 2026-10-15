# Original class-menu message ownership

The source ClassTable calls `msgSearch(80081b8c)` with seven original DOL
key pointers. `BattleClassMenuMessages` maps those exact keys to the corresponding
entries of an immutable, actual `MessageAssets` resource snapshot. It supplies
labels to the retained [ClassTable](RETAINED_ACTION_CLASS_TABLE.md); it does not
replace its row order, gates, icons, cursor behavior, or message-call boundaries.

The key table covers Jump, Hammer, Items, Tactics, Special, partner techniques,
and lecture completion. `tools/verify_battle_class_menu_messages.py` checks every
address and message ID against the read-only original US ISO. All seven reside
in `msg/US/global.txt`; the native global resource must match that complete
original file. Message bytes, NUL terminators, high bytes, and control tokens
are copied without rewriting. An unknown key, missing entry, or ambiguous
entry returns unavailable. Equal text in distinct source entries retains
distinct guest addresses; actual pointer aliases share their mapped address.

## Actual VM allocation policy

The caller supplies an explicit unused guest base and the actual stable
`EventAddressSpace`. A single `registerRam` admits the complete copied bank;
overlap, zero base, overflow, and incomplete resources fail before registration.
The facade owns those copied bytes until VM teardown, independently of the
wrapper metadata. The ClassTable binding strongly retains the wrapper and its
immutable source snapshot. Calls against a different address-space identity
are rejected.

This is a private battle-VM lifetime policy: callers must not unregister,
rebind, remap, or write this cache, and the VM must outlive every use. There is
no claim of registration-generation or ABA detection. No EventAddressSpace
API or generic allocation contract is changed. The immutable snapshot must not
be reloaded through another mutable alias while bound. The native integration
constructs its own snapshot and exposes only `shared_ptr<const MessageAssets>`.

## Native integration journal

`artifacts/battle-class-menu-messages-native-replacements.json` contains exact,
unapplied replacements, dependent on the ClassTable native journal:

* `snapshotFieldMessageAssets(roomAsset)` loads actual configured field
  `assetRoot/messages/global.txt` and the current room file into an owned,
  immutable snapshot. It uses existing `MessageAssets` room/global lookup,
  including ambiguity handling. Missing files remain unavailable.
* A final optional `TrioAttack::begin` argument accepts that snapshot. On fresh
  actual runtime creation it registers cache `0x85a00000` and binds the actual
  retained ClassTable message provider before initialization. Failure does not
  publish the temporary runtime. No resource is detached during its lifetime.
* `hasBattleRuntime()` observes the real runtime owner. Game requests the real
  current-room snapshot only at fresh `AttackReady` entry. Subsequent attacks
  reuse the existing VM and binding. Game does not admit a fresh attack without
  an actual snapshot. Older narrow begin callers may omit the argument; their
  native ClassTable then honestly holds at its missing message provider.

The integration does not publish original menu rendering or audience tap
rectangles. Physical controls and source X interruption remain unchanged.
Remaining UI providers retain their own original unavailable boundaries.

## Focused validation

Strict host and ARM syntax checks pass for the new owner and tests. ASan/UBSan
passes with leak detection disabled for the new owner, retained ClassTable, and
test translation units; the matching frozen core archive is not instrumented. Actual
`EventNativeRuntime` tests cover overlap, wrong VM, unavailable keys, room
priority and ambiguity, raw control bytes, distinct-entry identity, resource
retention, and facade-owned bytes surviving wrapper retirement. The original
US seven-label integration runs the retained ClassTable with real VM-backed
actor memory and compares every mapped byte and terminator with the actual
resource. It links a matching frozen core dependency graph; its recorded
closure hashes are in `artifacts/battle-class-menu-messages-core-match.json`.
The journal was applied only to a private complete-header copy and all three
changed native translation units passed host syntax checks. Whole-project
host/native checks and applying shared replacements remain the integrating
owner's responsibility.
