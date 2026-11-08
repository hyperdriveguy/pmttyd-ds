# Whole Mario constructor and native fresh owner

The source graph executes the complete US `marioInit` body at8005b5c8/488 and
fourteen reached constructor leaves. `NativeMarioConstructor` supplies a fresh
Mario and pouch in the existing UI/animation process, using its actual SDK heap0,
file cache and address space. This closes a constructor ownership boundary;
it does not establish original field Main, card loading, battle result export,
field return or the complete Crump opening.

Implementation: [source constants](../../../include/field/source_mario_constructor.hpp),
[native owner](../../../source/field/native_mario_constructor.cpp),
[public lifetime/API](../../../include/field/native_mario_constructor.hpp),
[fixed graph compiler](../../../tools/compile_mario_constructor.py),
[whole-body pins](../../../tools/mario_constructor_bodies.json) and
[data pins](../../../tools/mario_constructor_assets.json).

## Source execution and ordering

The checked-in graph contains1247 original instructions across15 complete bodies.
Builds execute generated native continuations; they do not fetch original code,
decode instructions at runtime or require the ISO. The extractor verifies complete
source SHA256 before generating each instruction. The shared
[FieldMapSource engine](../../../source/field/field_map_source.cpp) retains integer,
condition, link/count, scalar/paired registers and real VM stack stores.

The parent allocates heap0 size2f8, publishes8041e900, clears the returned760-byte
record and writes its five sound/resource handles27c..28c=-1. It calls fresh
pouchInit, gravity reset and camera-ID initialization. It sets variant byte3d=0,
then makes three `animGroupBaseAsync(name,2,0)` requests, each with the original
fresh owner/signedvariant/table read. A completed source polling return0 is a
completed child call; it does not prove the assets are ready.

The remaining parent preserves signedcharacter3c speed selection, original
pose handles/party bytes, scale/vector fields and inlined initial pose request,
including display locks, actual name comparisons and dot-mode table mapping.
It calls motion change0, balloon/rub initialization, initializes blur history,
resets camera shift/rates/follow, checks item9 and conditionally grants it,
then initializes actual party pointer globals8041ead8/dc. The current-party
selector8041ead0 is a separate owner.

Motion change executes the original previous-motion post callback. For the cold
ordinary owner the callback is `mot_stay_post`800a6b10/ac. Known compiled callback
entry addresses enter the same source frame after setting the original LR;
unknown targets retain the external child boundary. The source post checks
exact-1 handles before SFXOff. Initial handles skip audio through those actual
gates; an unsupported reached audio child holds.

Blur history is803d9848/60. The parent captures one owner at8005b99c, reads X/Y,
stores the first X/Y, then reads Z at8005b9b0. Seven further rows reuse those
retained words. A hold on Z must neither replay the first stores nor reread X/Y.

The private floating frame supports the reached binary32 scalar/paired save
ABI with unquantized GQR0. Binary64 stack saves promote those scalar values;
this is not arbitrary GameCube FPR/FPSCR state. Split-word loads/stores retain
completed halves through holds. Stack addresses and work budgets are native
policy; budgets do not advance simulation ticks.

## Fresh pouch and bounded native children

Complete `pouchInit`800d59c8/4e8 allocates heap0 size5d4, publishes8041eb00,
clears and initializes the actual packed record. Its only external children
are allocation, memset, `msgSearch("name_party3")` and strcpy. It loads source
float0 at8042187c before its later store. After msgSearch completes, it freshly
reads8041eb00 and derives destination+5c0; the source graph preserves this
read ordering even if the portable provider changes the selected owner.

The native facade admits one finite message case: UI language1, original key
`name_party3`, and the original US text loaded from `messages/global.txt`.
Factory admission requires its NUL-terminated length below20 bytes. A native
32-byte name buffer supplies a stable VM address. Other language/key cases
hold. This is an adapted constructor message provider, not the whole original
message catalog, binary search, language fallback or message lifecycle.

Native strcmp reads bounded mapped strings of at most127 bytes plus NUL and
returns the unsigned-byte comparison sign. The constructor uses that sign or
equality. Native strcpy admits only the retained name token into the actual
published pouch+5c0, copies ordered bytes including NUL and returns the
destination. It does not reproduce optimized GameCube word overreads. Mapped
memory writes are preflighted; these finite leaves do not insert an artificial
hold halfway through a successful native copy.

The graph also contains whole `pouchCheckItem`800d4ed4/1f8 and
`pouchGetItem`800d50cc/4a4. Reached item9 first stores byte[pouch+99]=1, then
freshly fetches the pouch for common key insertion. It finds the highest empty
slot120..0 in the121 halfwords beginning+a0 and shifts the preceding entries
right. A full array returns0 while retaining the earlier upgrade-byte write.
This is not a plain insertion without item dispatch. Other source item paths
remain in the graph; unsupported reached children such as swSet or party/audio
services hold instead of receiving fabricated results.

Fresh constructor pouch initialization is distinct from the subsequent
stg0_00 grants/coins/revisions in the
[prologue pouch adapter](../../../source/inventory/prologue_pouch.cpp).
See also [fresh packed pouch](../battle/BATTLE_FRESH_PACKED_POUCH.md) and
[key insertion mechanics](../unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md).

## Shared process admission and lifetime

Creation requires a current `NativeUiStartup`, its ready field animation
process and actual file-cache aliases. The importer requires an empty
destination and preserves original initialized bytes/BSS provenance. Runtime
admission validates exact span shape and fingerprints before installing missing
fragments. It refuses any already mapped cold Mario/pouch/party/history/rub
BSS, including equal zero bytes. It does not reset an existing loaded owner.

Immutable initialized inputs may borrow matching existing memory. In particular,
animation boot already owns model triplet80415eb0/c and model-name prefix
802c18a0/1c; only missing suffixes are installed. Each real contiguous backing
is separately pinned into a nonexecuting identity facade. Currentness checks
UI/process/cache identity, every admitted backing and, after publication, the
actual Mario/pouch pointer values. Replacing RAM with equal bytes retires the
owner. The finite native facade therefore holds on published-owner substitution,
although the portable source graph retains the original fresh-global reads.

Allocation uses actual `eventHeapAllocate(0,size)` and original SDK headers.
Before source publication, each returned payload is extracted into a standalone
VM mapping at the same guest address and pinned. Mario uses owned VM RAM; the
pouch uses the stationary shared typed byte array described in
[shared pouch backing](NATIVE_SHARED_SOURCE_POUCH.md). This prevents later Window
heap extraction from moving the live Mario/pouch backing. Native stack32KiB and
name32 bytes use private tokens8d080000 and8d088000; they are not original heap
placements. Group acquisition uses the existing
[animation group request binding](../display/ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md),
with the genuine empty literal8041fb30 also pinned.

`initialize(visits)` resumes the retained source cursor, refuses reentrant calls
and pending animation Entry/producer work, and marks ready only after the full
parent returns with ownership still current. Once initialized it does not rerun
the constructor. A failed admission or partial initialization is not a
transactional rollback: process RAM/heap work may already exist. Keep the shared
process and child lifetimes stationary; do not retry a new cold owner over its
partial state. There is no source destructor/free lifecycle acknowledgment in
this facade.

## Corrected static inputs and evidence

Actual camera reset templates are802fa748/18, two initialized zero vec3s.
At801a95bc, `lis r4,8030` followed by signed `addi ...,a748` yields802fa748;
the rate reset uses signed a754 at801a9560, yielding802fa754. Unrelated8030a748
contains different data and must not be imported for this owner. Camera float
inputs remain80424aa4/4 and80424adc/10. The
[camera leaf contract](../camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md)
provides the remaining reset order.

Selected source pins below are SHA256; full pins and imported-span provenance
remain authoritative in the linked manifests and asset provenance.

| Original span | Size | SHA256 |
|---|---:|---|
| marioInit8005b5c8 |488|e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce|
| pouchInit800d59c8 |4e8|c6e29f88e574c1d5caa1aaf5c7a73a53bc2d952cbad00a9e21ab2bfd3113def1|
| pouchCheckItem800d4ed4 |1f8|42deab825c1637d84ab4ad27431a9571fbb92f349716b88a6eba7b736779ba57|
| pouchGetItem800d50cc |4a4|7abf7872b6aedf92f82286b20aebcbad5b613f1f83616e7ee8cb661d7e47998c|
| camera templates802fa748 |18|9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0|
| model triplets80415eb0 |30|93f4206ad5303f1097f627986b329d800b96585f2eedba0592987e7744cd16de|
| dot pose table80309748 |60|dc0223caabc4e9aeedab6ff9f6fab1cecab5b1588a421cc8348cc901615d05bf|

All sizes in this table are hexadecimal. Further signed-address witnesses are
rub803e0000+c228→803dc228, blur803e0000+9848→803d9848,
dot table80310000+9748→80309748 and key802d0000+c728→802cc728;
the low halves here are signed16. Blur storage starts exactly after the
animation boot tangent span, not within it. Constructor data is adjacent to,
not overlapping, the effect table beginning80415ee0. Original rights/provenance
and local-use restrictions remain in [ASSETS](../../ASSETS.md).

## Snapshots, proof and remaining boundary

After genuine initialization, `adoptPouchSnapshot` copies a supplied packed
5d4-byte native checkpoint into the same live allocation; it does not change
pointer identity or call Init. `capturePouchSnapshot` copies current bytes into
a candidate before publishing the host snapshot. These are explicit native
handoff helpers, not original save validation, card loading, battle export,
field-return acknowledgment or a complete Mario state snapshot. Neither copies
the Mario owner, heap graph, animation state or continuation stack.

[Portable tests](../../../tests/source_mario_constructor_tests.cpp) cover a
held group request without repeated allocation, first blur stores before a held
late Z read, fresh pouch destination after a message hold, and item9 upgrade
before full-array failure. Their allocator/message providers are test fixtures.
[Native tests](../../../tests/native_mario_constructor_tests.cpp) use the actual
shared UI/animation/cache/SDK heap, original imported constructor data and US
name; cover language hold/resume, budget retention, final packed fields,
checkpoint adoption without reinitialization, later Window extraction, actual
MarioPoseInit composition, cold-admission refusal and equal-byte owner retirement.
These are scoped fixtures, not a receipt for normal Game scene execution.

Full source MarioPoseInit is a separate
[borrowed-owner boundary](SOURCE_MARIO_POSE_STARTUP.md). The owner here makes
that genuine raw record available; it does not replace normal field producer,
NPC/effect ownership or battle services. Remaining opening obligations stay in
[Crump integration requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md)
and the [documentation inbox](../../DOCUMENTATION_REQUESTS.md). No new unknown
source request is needed for the fifteen pinned local bodies.


## Normal native Game startup and retention

Normal `Game::initializeNativeUiFieldAnimation` now runs this whole constructor
in the existing UI/animation process before its environment/texture startup
contribution. It then drains the same cache's actual pending ROMFS group jobs
with an explicit unlimited cold IO budget and calls the retained
`initializePoses` facade, which owns one `NativeMarioPoseStartup`. Source holds
retain that same wrapper, including its original unconditional handle stores.
`FieldPartyRegistry::adoptMarioSourcePoses` preflights all three existing source
handles, resource identities, mode2 and archive0, then creates typed role views.
It performs no Entry, allocation or default writes. Negative source handles
remain raw source results and fail native rendering admission. The legacy
non-UI/harness initialization path remains separately named.

`Game::nativeMarioConstructor()` exposes the ready constructor/pose owner only
when its UI process is the same admitted field animation process. Detached Game
copies do not acquire that receipt; room/checkpoint ownership handoff explicitly
retains it. Restore and handoff refuse a held constructor/pose wrapper. Actual
Mario/pouch addresses, three handles and backing remain stable across the tested
room, native restore and checkpoint transfer. `native_ui_startup_game_tests`
checks exactly three occupied source pose slots after startup, raw/typed handle
agreement, shared process identity and retained owners. The standalone native
constructor fixture also repeats the completed facade without new Entries.

An existing packed `progress.originalPouch` can be copied after genuine fresh
constructor completion during cold native saved-state admission. This is explicit
native snapshot policy, not original card-load or field-return acknowledgment.
Normal Game now aliases an explicitly supplied canonical pouch to the actual
source backing; see [shared pouch admission](NATIVE_SHARED_SOURCE_POUCH.md).
Later typed control/position/party writes remain separate source views. Binding
the complete Mario/pouch field/battle lifecycle,
whole Mario Main, Prepare/Post and the source sequence return remains required.
Normal boot success and these ownership checks do not prove a Crump round trip.


Native backing admission first tests each complete span, then binary-searches
the largest backed prefix only when a span crosses allocation boundaries. The
contiguous-prefix predicate is monotone for the fixed address. This preserves
all fragment identities and refusal behavior while removing the32,768-probe
private-stack walk; no source values, aliases or currentness checks are cached.
Per-instruction ownership checks and full rechecks across children remain.
