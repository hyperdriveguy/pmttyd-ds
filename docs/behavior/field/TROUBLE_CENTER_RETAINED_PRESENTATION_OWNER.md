# Trouble Center retained presentation owner

TroubleBoardPresentation composes the [original event adapter](ORIGINAL_TROUBLE_CENTER_EVENT_RUNTIME_ADAPTER.md)
with retained MessagePlayback, independent MessageSelection, and the existing
[original task menu](TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md). It executes
original message IDs/tokens through MessageAssets; no rewritten story text or
synthetic dialogue is introduced. World callbacks retain the original0/2 protocol.

The stable, noncopyable owner binds externally owned event/font/message resources.
A visit has an explicit monotonically increasing simulation ordinal. Duplicate or
older ordinals do nothing, while different visits at identical milliseconds remain
separate. Visit order is EVT callbacks, independent iconMain, ordinary message,
then selection. Rendering has no advancement API. Selection effects are routed as
original numeric IDs; menu effects are routed as original menu rows. Missing cue
providers hold the associated selection/menu admission rather than choosing a
substitute sound. The provider owns actual audio playback.

Toge state is retained per event ID, consumed once after successful Print creation.
Source selector2 becomes mode7 with fixed WORLD coordinates; lecture anchor is
150,12,-156. These are engine coordinates, not pixels. Native renderer binding
must scale/project the supplied anchor using the actual world camera. Selector1
becomes mode0. Speaker operand0 remains unattributed. Append and Continue mutate
once per callback and then poll window bit2; stopped retained prints remain alive.
The separate selection publishes only after its zero-opacity closing visit retires.
Delayed resource admission can start after original firstCall has become false.

MenuInit owns one stable TroubleQuestPresentation; LW10 receives owner-scoped
opaque handle1, never a host pointer or an invented GameCube address. MenuMain
translates that handle and preserves the same list/cursor across No/Continue.
MenuEnd releases the owner without changing sourceLW10. This is a portable work
surrogate: original Cmap16748-byte allocation, zeroing, binary offsets/cache ABI,
global icon pool, and allocator failure semantics remain unimplemented. Do not
claim the original allocation contract is fulfilled by this surrogate. Menu flags
read the event's last completed visit snapshot; activeQuest reads the live callback.
No concurrent global-bank writer is bound here. Singleton print/selection slots
are scoped to this original closure; arbitrary simultaneous talkers are unsupported.
Toge models the packed source calls1/2 and their integer anchor coordinates, not
arbitrary floating operands or the complete original high-flag word.

## Validation scope and next binding

Host integration tests run actual packed original EVT words, original font/icons,
and exact original US messages through the real retained presentation classes.
They exercise lecture-only contact, fixed anchor/append serial retention,
selection fade retirement, delayed room resource arrival, task2 diary/help,
decline/reopen with retained cursor, later acceptance, source cue requests,
duplicate-visit suppression and missing world provider yielding. World Key and
direction callbacks in these tests are fixtures, not native gameplay evidence.

The owner is not yet installed in Game's native board/counter contact dispatch or
renderer. Exact contact nodes, deferred case9 lifetime, shared switch-bank merge,
Mario/party direction services, pouch/cancellation, Q1/Q4 NPC/door children and
original audio execution remain providers. Frozen native compilation establishes
build compatibility only; no interactive board capture or physical3DS performance
measurement is claimed. Next bind the actual case contact, Game owner and existing
Renderer originalMessage/troubleQuestMenu paths before expanding additional areas.
