# Native slot initialization, idle pass, and array owners

2026-10-14 original US G8ME01. Source contracts: [slot/display admission](ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md)
and [heap lifetime and next gauge provider](SURVIVING_UNIT_HP_GAUGE_MAIN_AND_SLOT_HEAP_LIFETIME.md).

The trio constructor now calls actual BreakSlot_Init8020507c after AudienceInit
and before launching attacks. It retains the controller, copies all original
8/8/5-word arrays before clearing its134hex bytes, initializes reel ordinal/
state fields through the source current getter, then allocates each table and
stores/copies through the retained reel pointer. The original DOL data bank
already contains arrays802fe040/60/80; there is no invented native reel table
or new image conversion. Original map allocations align each payload to32,
so all three32/32/20-byte requests consume192 bytes including headers/padding.

Each native BattleAlloc request freshly reads the recovered root8041e80c and
uses the existing source _mapAlloc implementation. The original source has no
safe null/missing-allocation branch before memcpy; portable unavailable
providers suspend the bridge, and an actual null result is stored before that
unresolved copy domain. No fallback pointer or allocator is minted.

The engine now runs BreakSlot_Main80203aa0 after AudienceMain. Its used state0
prefix captures B, queries audience/controller, requests Mario/party lookup,
and scans stopped reel states before the common pass. Exactly two state2
reels with unequal reel1/reel0 result words publish controllerstate1,timer0
before the active branch remains pending. In the actual initialized idle
bank, the common pass clears only each flagsbit1 and writes BYTE58=ff in
ascending reel order. It preserves table owners/counts, ordinals and other
fields. Any admitted active controller/reel state remains an explicit
provider; earlier source writes are retained on suspension.

Portable Slot_End80203688 queries each reel freshly and calls Free on its
raw table pointer in ascending order. Native Free reads8041e80c freshly only
for nonnull arguments. End supplies no pointer clearing or controller reset.
This helper is ready for the source BattleEnd teardown order and is not
invoked as idle cleanup. Arrays remain allocated while the battle is held.

Bridge regressions now reach btlDispMain801024f0 without forced end/result/
story/control writes. Source survivors still require HpGaugeMain and later
status/part/display services; ordinary empty reels still register a real draw
callback and scissor state. Slot Main completion does not complete display
or field restoration.

Tests import the actual original arrays, check guarded controller clearing,
ordered allocation sizes and exact copied values, retained/fresh owners
across callback owner swaps, flag preservation, partial writes before active
admission, actual two-stopped transition and ordered frees without resetting
the pointer words. Trio regression checks native arena table owners and data,
idle fields and display boundary. Existing ending heap tests now follow the
actual EndingWork pointer and free-tail header: adding original slot arrays
legitimately shifts later allocations and leaves those arrays alive. The
prior spotlight-only fixed addresses are not an invariant of this history.
