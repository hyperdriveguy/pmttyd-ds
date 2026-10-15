# Retained original action-class table

`battle_action_class_table_continuation` owns the complete US G8ME01 revision0
`_btlcmd_MakeActClassTable`, `80124dc8/5b8`. Its 366 instructions and seven
original NUL-terminated message keys are pinned by the existing read-only
[verifier](../../../tools/verify_action_class_table.py). Full-body/callsite
evidence is recorded in `artifacts/battle-action-class-table-source-evidence.json`.
Inspection opens the original ISO read-only. No original execution, hardware,
pixel-parity or complete battle result is claimed.

The exact branch/storage contracts are
[class admission/publication](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)
and [class gates/keys](ACTION_CLASS_TABLE_ELIGIBILITY_AND_MENU_KEYS.md).
This owner preserves their complete local body; it does not delegate whole
class cases or an unavailable suffix to the synchronous implementation.

## Invocation and providers

`resumePackedBattleActionClassTable(B,services,cursor)` reuses the typed
`BattleActionClassServices`. The only external children are:

| Child | Function / callsite | Retained original operands |
| --- | --- | --- |
| Unit | `800f88cc / 80124de8` | Passed B and actor ID read from B420 |
| Pouch | `800d5eb0 / 80124df4` | No arguments; original global getter |
| Jump message | `80081b8c / 80124eec` | `802f0018` |
| Party message | `80081b8c / 80124fd0` | `802f002c` |
| Hammer message | `80081b8c / 801250c8` | `802f0044` |
| Item message | `80081b8c / 801251f0` | `802eff0c` |
| Special message | `80081b8c / 80125278` | `802f0058` |
| Operations message | `80081b8c / 801252d8` | `802f0074` |
| Lecture end message | `80081b8c / 80125320` | `802f008c` |

An engaged result is the actual raw source word. A completed message result
zero is stored as zero; it is distinct from an unavailable provider. `nullopt`
alone holds the current original child. The ordinary completed return is the
freshly read published cursor count. When wrapped `B+171c==0`, the completed
return is the actual pouch pointer, after Unit/Pouch/setup/definition reads.
Neither scalar is converted to a success bool or EVT completion2.

Providers own their partial child work. A message provider that has completed
some file/mapping work before returning unavailable must retain that work;
the parent only redrives that same message key. It never repeats completed
Unit/Pouch/Message requests, already emitted row stores, or previous denial
checks. No source rollback or whole-unit/table snapshot is used.

The cursor pins B, callback user/functions, all actual guest memory callbacks,
byte reader and audience lifecycle observer. Changed bindings are refused
without reads or stores. Recursive resume/reset during a child is refused.
Explicit reset otherwise abandons the owner without undoing prior writes and
does not make restarting the synchronous producer safe. Actual VM allocations,
pouch, definition and resource owners must outlive the invocation. Source
registry or current globals may change where original fresh reads allow it;
they must not invalidate already retained guest allocations.

## Exact storage and freshness

U, P, definition D and the first-kind-selected class cursor C remain captured.
The setup and nested definition are read after the completed Pouch call, even
on wrapped-M0. The first kind read selects C and the initial Jump admission;
later kind reads stay independent. A changed global pouch pointer does not
redirect captured P. Updated bytes within P remain fresh at their source uses.

Each admitted row writes the original type/enabled/reason sequence, then waits
only at its actual message lookup. Its row address and count survive that wait.
After message acknowledgement, its label word is written at the source point;
fresh boot/hammer levels select original icons334..339. Party/Item/Operations
and other row count increments retain their original relative icon-store order.
All seven label keys and icons334..343 remain original. No English text,
replacement image, generated tone, source pointer fallback or guessed row is
introduced.

Gates preserve signed kinds/levels/counters, unsigned halfwords and every fresh
battle flag read. Operations and Lecture are independent tests: message-visible
mutation can produce both or neither. Message callbacks can also change later
kind admission, producing all seven rows under explicit fixture mutation; the
owner does not impose a stable-state row limit. Class descriptor padding,
unused rows, the cursor scroll word, menu type and other unowned fields survive.

Final publication stores the computed count, reads current index and then
current count, and applies only signed `index>=count ? count-1` correction.
It returns that live count even when a source-visible store mutation changed it.
There is no lower-index clamp and no alternate native class list.

## Original US message and native binding ownership

`BattleActionClassMessageBinding` describes an actual immutable US message
resource plus its mapped guest text owner, stable callback user and lookup.
It is an ownership requirement, not an implementation of `msgSearch` or a grant
to invent label pointers. Lookup must preserve the actual original key/text and
control bytes. Returned label addresses remain borrowed by the completed class
descriptors and UI after this producer returns, so their allocation lifetime
extends through the whole visible UI/VM borrow, not just the held message call.

`artifacts/battle-action-class-table-native-replacements.json` is an unapplied
journal which depends on the retained menu journal. It binds actual VM actor
lookup and `pouchGetPtr` through source cell8041eb00, adds the persistent cursor,
and connects actual menu ClassTable requests to this whole owner. It does not
force actor420 to Mario or invoke Harbor's acting-Mario adapter.

The proposed `bindOriginalClassMessages` admits a real resource/mapping once per
VM. Native resource admission occurs before initial source entry; without it the
child holds at the original message frontier and emits no speculative native
labels. Once begun, the same binding is additionally captured through the held
invocation. The VM-owned binding remains alive for all descriptor/UI borrows.
The factory forwards actual lookup results, including zero, and does not replace
an unavailable result with a key, translated string, placeholder or fabricated
guest address.

Current source ownership investigation: `MessageAssets` owns original US
room/global bytes and performs room-before-global key lookup, rejecting ambiguous
room keys. FieldActors exposes a real messages owner; Professor/Harbor story and
tutorial modules keep their own resident resources, mostly private. Harbor's
`cacheMenuLabel` copies actual looked-up text into its explicit MenuRam arena,
but its builder forces Mario and uses a transient stack Context. That does not
establish a Trio VM message/mapping lifetime. Trio currently has no admitted
`msgSearch` resource-to-guest text owner. Supplying that actual owner is the
next coordinated integration step; missing messages remain honest holds.

## Validation

Focused tests compare the complete ordered read/write/provider trace and final
state against the existing synchronous whole source reference. Every Unit,
Pouch and Message child is held twice at each reached boundary, then its entire
suffix is compared. Matrices cover Mario/party/other signed kinds, all original
level branches and icons, masks/definition gates, signed counters, status bytes,
halfword restrictions, source zero message values and signed index correction.

Mutation fixtures exercise all seven rows, retained first-kind cursor, fresh
post-message icon levels, captured pouch/definition owners with later byte/flag
changes, independent final admissions, live published-count return, actor-ID
capture before a Unit wait, setup read after a Pouch wait, and no replay of row
stores after a missing label. Wrapped-M0 preserves the raw Pouch result. User,
callback, lifecycle and passed-B changes plus recursive entry/reset are rejected.
Fixture labels are explicit scalar service outputs, not native story text or
resource ownership proof. Strict host execution, strict ARM production/test
syntax and the read-only original verifier pass.

This batch changes no shared runtime/build/index files and does not activate
normal battle UI, touch-only controls, or the complete original Main scheduler.

## Integrated original message ownership

The earlier investigation is superseded by the
[actual original label snapshot/mapping owner](NATIVE_CLASS_MENU_MESSAGES.md).
FieldActorTalk's message storage is private; the narrow field snapshot helper
loads the actual configured room/global assets without borrowing that private
storage. Fresh Trio entry maps copied original labels once and retains their
binding. Missing/ambiguous source keys stay unavailable; no authored fallback.
Normal class drawing and active source Main are still unfinished.
