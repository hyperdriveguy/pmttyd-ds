# Trouble Center state, menu and board events

This guide covers the finite quest-state store, working-switch reset/cancellation,
original menu producer, window cache/draw requests and retained board/counter/
installer event presentation. [Field items](field-items.md) covers actual Goomba
quest delivery/rewards; [field entry](field-entry.md) covers the shared case pool.
Recovered source evidence remains in [behavior contracts](../behavior/README.md).
These owners do not complete every Trouble Center quest or the whole game.

## Saved-state knowledge and reset boundaries

`TroubleQuestExtraState` stores knowledge separately from values for flags
5374..5421 and bytes 1469..1474. Four existing optional fields remain canonical:
5376 and 5405..5407. Their relative bits are excluded from the extra mask, avoiding
duplicate owners. Known-clear differs from unknown; unsupported bits, values
outside known bits and nonzero storage for unknown bytes are invalid encodings.
Representation validity does not mean every required switch is known.

Quest1420 retains its existing optional byte. The six extra byte setters narrow
to U8 with source wrapping rather than saturation. Explicit fresh initialization
establishes known-zero state, but an older save omitting the extension must not
acquire invented zero values. Board entry requires the full admitted state.

Start/cancel services reset 17 working flags (5405..5421), then six bytes
(1469..1474), in source callback order. They do not themselves select quest1420
or clear completion flags. Cancellation admits every item ID before resetting,
then removes each original listed key item until a fresh count reaches zero.
List order, duplicates and ignored remove return values remain source behavior.
Actual pouch providers own progress; there is no guessed iteration cutoff.

## Menu rows, input and publications

The 30-entry array contains original one-based quest IDs. It is filled in source
row order and counts entries without sorting or compacting them. Story 261 puts
quests 20/21 in rows 18/19; Story 281 later puts quest 19 in row 20. Other chapter
thresholds add fixed triples. Row number, quest ID, completion flag (5374+ID)
and active-quest byte are distinct domains.

State 0 creates five original named icons and yields. State 1 processes input.
Accepted/canceled states 100/101 publish and delete icons on the following
invocation before returning 2; the caller still owns MenuEnd. Source A is tested
before B. Unknown active/completion knowledge holds before later input; an active
quest or completed row cannot accept. These are source button masks, not raw
libctru numbering.

Down then up process independently. Repeat stops at an endpoint unless its edge
trigger permits wrapping. Ten visible rows retain 7/3 cursor margins; L/R page
by up to 10 rows. Counter94 resets only when cursor changes. Scroll eases one sixth
of the remaining difference per admitted visit, with source row spacing 30 and
centered 608x480 coordinates. Drawing does not advance these values.

Icons are original pointer 504, arrows 524 and L/R 134/136. Pointer flip-X and
up-arrow flip-Y are retained source flags, while L/R scale is .8. The pointer's
10 original 40x40 frames use delay 2 except final 28; static resource dimensions
are verified before admission. Hidden active icons still animate. Resource
unavailability advances no icon state. Sound rows 42/43,18/19 and5 remain original
open/close,accept/cancel and move requests to their actual provider.

## Text cache and ordered window requests

The cache retains four original header pointers,30 client copies of21 bytes and
30 contents copies of31 bytes, including NUL. Limits are bytes, not glyph counts.
All 64 U16 font widths are prepared once with immutable font spacing 2. Width
lookup uses retained pointer identity; equal text at another address is not a hit.
A complete candidate replaces the cache only after every string measures.

Drawing emits source commands in order: frames,font state,titles,clip,rows,clip
restore. It is not a transaction. Early command failure can leave already emitted
state; row-loop failures attempt saved-scissor restoration. Providers must own
that admission/failure boundary instead of assuming automatic rollback.

Colors are RRGGBBAA. Completed gray and ordinary black are overridden by active
red. Each row queries completion then active state separately. Header matrix
strings ignore the prior shared .85 scale, cap X independently and center using
U16-narrowed width. Row caps 180/260 apply before their .85 factor. The list clip
is source top-left 175/98/410/304, while text/frame/icon coordinates use centered
source UI space. Native rendering adapts that space later.

`TroubleQuestPresentation::invoke` clears its display pointer/result before the
menu producer. A provider hold does not reuse a prior draw publication. Self-owner
and menu identity prevent copied presentation state from advancing borrowed
callback pointers.

## Original event closure and service routing

The private program pins board/counter/installer roots, the confirmation table
and 21 semantic callback tags to original relocated addresses. Tags are native
dispatch identities; guest REL tokens are neither host function pointers nor
executed PPC. Cancellation lists come from all 30 original pointers and retain
-1 termination; a quest number does not imply an item identity.

The VM snapshots locals,story,active quest,raw operands and resolved values for
each supported request. Text remains original ID/name bytes. The shared case
pool handles registration; Check selects the original confirmation pointer and
LW11, with -1 cancel producing null. Start performs the working-switch reset
through actual VM saved-state services. Other callbacks use explicit providers.

Status 0 holds and 2 continues. Only direction providers may return 1 to clear and
yield at the source endpoint. MenuInit must publish a nonzero retained work token;
MenuMain/Select publish LW0. Coin/item-check returns route through the original
raw destination operand. A provider must retain its operation even after source
`first` becomes false during an earlier resource hold.

Board construction admits all required saved state before VM effects. It owns
native GP/heap support and original bus-clock timestamp conversion. Diagnostic
formatting counts calls without failing the VM; native cache/syscall hooks do not
prove original device scheduling. Saved-state synchronization refreshes the same
retained VM without restarting events or clearing unknown state.

Root IDs survive case-slot reuse and are distinct from total live allocations.
Children may outlive a root; installer case records may remain after the VM has
no events. Contact marking and second-pass case dispatch can reactivate it.
Explicit root reentry reserves the original immediate constructor burst and
retains existing children/cases/IDs. Priority20 dispatch requires a real NPC-stop
provider rather than a skipped side effect.

## Event/UI visit ordering and borrowed owners

A stable `TroubleBoardPresentation` owns one pending operation per event ID and
callback token. It opens exactly once when resources become available, even if
`first` is already false; completion erases the key so the same callback may run
again later. Toge state is per event and is consumed by Print. Type 2 uses a fixed
anchor with original placement override 7. Message pending state controls callback
completion, separately from rendering or whether some text remains active.

Each accepted simulation visit runs eventMain before presentationMain. The latter
advances icons,message,selection,then drains selection sounds. UI acknowledgment
therefore becomes visible to EVT on a later visit. Visit serials are monotonic
and independent of milliseconds/FPS; a missing UI pass blocks a second event pass.
Rendering reads retained publications without acknowledging callbacks.

Menu handle 1 is scoped to this owner. Active quest refreshes from each VM request;
completion flags read the last published event-switch snapshot. They are not raw
VM aliases. Known return 2 also needs a contained menu result; -1 is valid cancel.
MenuEnd separately destroys the retained menu. Actual world/direction/audio,
message-window lifetime and full original field/case integration remain explicit
owners and unfinished broader requirements.
