# Native original sequence owner

[Source table, lifecycle and hashes](ORIGINAL_SEQUENCE_TABLE_AND_FIELD_BATTLE_LIFECYCLE.md)
are the implementation contract. `OriginalSequence` owns current/requested/
previous, two requested arguments and32-byte work independently of native
Scene and the legacy field-battle harness word. A request only changes next
and arguments. Main captures the original callback identities and preserves
completed sync/Exit/work reset/current publication/Init across provider waits.
Callbacks own their transitive continuations. All eight recovered table rows
are present; out-of-table memory stays unavailable rather than being clamped.

The portable constructor represents source boot with all three IDs-1 and zero
work. Game explicitly imports a **native field snapshot** with current/next2,
previous-1, work.word0=2 and other work words zero. Checkpoint/room restore
retains the shared owner identity and reimports this adapter, discarding unsaved
callback history. This does not execute or certify original field Init/Main,
logo/title startup or map-change history. Existing checkpoint formats do not
serialize the original sequence owner.

The original field battle-start adapter requests(4,0,0). Native main, outside
Citro3D frame recording, consumes pending transitions before the next gameplay
tick. A blocking `C3D_FrameBegin(0)` waits the prior GX command queue;
`C3D_FrameEnd(0)` closes the empty admitted frame before ordinary rendering.
This is a library adapter, not a claim of original GameCube fence timing.
`C3D_FrameSync()` alone is a VBlank wait and is deliberately not used as a fence. The complete
field Exit leaf clears only mask2 in the retained native GP.word0 adapter
`Game::fieldSystemFlags`. Current4 and reset work/arguments are then published
before the callback80073f20. That battle Init has no success provider and stays
pending without replaying sync/Exit/publication. No whole Init no-op is used.

Existing demo gameplay continues through its prior native adapter while this
source callback is held. This is **not** execution of the complete original
outer sequence/battle lifecycle; original Init sound/event/control/resource
providers, Main and Exit/field return are still required. Native main does not
run an invented successful field Main when current equals requested.

Trio finishing letterbox reads this exact shared current owner. The original
current4 decision advances from sequence query to the independent sound-clear
provider800dc0f8 after known Bero bit1 is clear. Missing sound remains pending;
explicit test providers resume it to the GPU viewport wait without replaying
sequence reads or adding duplicate queue records. Key39/Ctrl38, sound owners,
full deferred GPU callbacks and original outer return remain separate work.

Host tests exercise boot, request/current separation, missing sync/Exit/Init/
Main, fresh reads after mutating callbacks, captured callback/arguments across
waits, all table rows, unmapped accesses, exact field leaf and restore ownership.
The finishing integration test uses its actual original battle-start request,
then injected synchronization plus the real field Exit leaf. Native emulator
coverage is recorded separately in the progress archive and does not certify
original timing, pixel parity or physical performance.
