# Save-event retained window Continue/Add and raw control waits

US G8ME01 disc0 revision0;2026-10-13. Static read-only DOL evidence, no original
execution. Scoped dependency for
[common save event](../field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Existing
[window lifecycle](MESSAGE_WINDOW_LIFECYCLE.md),
[print variants](MESSAGE_PRINT_VARIANTS.md),
[parser](MESSAGE_PARSER_RECORDS.md),
[playback](MESSAGE_PLAYBACK.md) and
[window states](MESSAGE_WINDOW_STATES.md) establish the local service bodies.
This binding makes the original save script's retained-window ownership explicit.

## evt_msg_continue is resume plus polling, not force-close

Complete `evt_msg_continue`800d21fc/0x54 takes(E,firstCall). It has no resolved
script operands. If firstCall!=0,load saved message window indexE+0x178 and
call msgWindow_Continue8008156c. Every invocation then loads that saved index
AGAIN and calls windowCheckID8007ad50. Nonzero returns event handler0/yield;
zero returns2/complete. FirstCall-only mutation must not be repeated during
polling. There is no local deletion, force-close, skip-text, key-wait release,
new-window creation, identity/speaker reset or replay request.

Complete `msgWindow_Continue`8008156c/0x50:
windowGetPointer(index); set statehalfword+0=1; OR flagshalfword+2 with2;
load retained handleW+0x28 and itsdata; refresh lastTimestampLOWD+0x34 from
GP+0x3c, then HIGHD+0x30 fromGP+0x38. Playback index, E/schedule, scroll,
parser records, wait state, key ordinal, history, identity and callback survive.
These unchecked source helpers assume a valid retained window.

`windowCheckID`8007ad50/0x18 returns ONLY flags&2, exactly0 or2. This is the
message poll bit, distinct from allocationbit1. It does not mean “slot exists”
or “window was deleted.” Continue sets pollbit2 before its first poll, so a
valid ordinary first invocation yields pending. Subsequent window processing
owns when that bit clears.

A native source bridge must resume the saved window and await the same source
progress condition. Force-closing/discarding the active message on Continue
changes the source path even when that message was near its terminal. Continue
can reach a terminal through ordinary playback and close through the normal
window states; it does not directly select those closure states.

## evt_msg_print_add flags0 versus flags1

Complete `evt_msg_print_add`800d23c4/0x9c resolves flagsoperand0 through
evtGetValue, then inputoperand1 through evtGetValue on EVERY invocation.
It captures firstCall before those calls. Only on firstCall!=0:

- Flags&1==0:call msgSearch(resolvedInput), then
  msgWindow_Add(returnedText,E+0x178).
- Flags&1!=0:call msgWindow_Add(resolvedInput,E+0x178) directly; source input
  is a raw text pointer, not a message ID to be looked up.

No other flags bits are used locally. All invocations then poll savedwindow
via windowCheckID; nonzero returns0,zero2. It does not resolve an arbitrary
output parameter or consume a selection result. Resumed calls do not append
the same payload again even if flags/input are re-resolved.

Flags0 in the save event means original US keyed messages such as
`msg_mcard_save_select`, `msg_mcard_save_warnning`,
`msg_mcard_save_finish` and source error IDs. Flags1 uses exact raw controls
`<scrl_auto><once_stop>` from the original DOL literal. Do not display that
literal as authored text or look it up as an unavailable resource ID.

Complete `msgWindow_Add`800815bc/0x74 receives(text,index), not(index,text).
It resolves the same window/handle, refreshes D.lastTimestampLOW/HIGH from
GP+0x3c/+0x38 BEFORE msgAnalize(existingHandle,text), then sets W.state1 and
ORsflags2. Parser appends over its previous terminal slot, retaining emitted
count, coordinates, style and parserT/V. It does not resize or reset the entire
work, create a new window, overwrite identity, assign a speaker/position or
replace callback. Add therefore continues the accumulated original message
window, including key/page history and parser timeline.

## Exact raw-control result and retained-stop lifetime

Under the source parser, `<scrl_auto>` shares the p-token route: when retained
S!=0 emit record0xfffd with currentT and row-adjustment payload; whenS==0
emit no scroll record. It is not guaranteed to emit a fixed record count
independent of prior text. `<once_stop>` always emits0xfffc at currentT,
then increments parserT by50. These parser schedule values are source units,
not a new scene-level WAIT_MSEC or native 50-tick delay.

Playback0xfffd updates scroll target/history andE, then advances through the
record loop. Playback0xfffc saves cursorI+1, setsE=recordSchedule, clears
message-data flag4 and returns1. General windowMain maps return1 to W.state2,
clears W.flags2 and restores source stay pose. The window/handle remain
allocated and visible. The event poll completes because flags2 cleared,
**not** because the window closed. State2 itself does not run more playback.

A subsequent keyed Add appends over the retained terminal, setsstate1/poll2
and resumes from the saved cursor into appended records. In the source
first-hit path this enables the same system window to scroll/stop and then
show the save prompt. Independent `evt_msg_select` uses savedE+0x17c and its
selection window; it does not replace the retained print windowE+0x178.
Child constructors inherit those metadata fields along with locals/flags;
see [child metadata](../event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md).

Source `<o>` and `<once_stop>` both emit0xfffc; source `<k>` emits its separate
key-wait record. Their equivalence as parser opcodes is not permission to
skip a source wait or to collapse the print and selection windows. Multiple
commands can observe distinct retained stops as the script advances.

## Ordinary terminal versus key/page waits

Only W.state1 calls msgMain. Return2 requests general state3/key wait; input
on a later state3 invocation requestsstate1 and sets waitresume2. Continue
itself does not change that wait state. Return1 requests retainedstate2 and
clears poll2 as above. Return0 from terminal0xffff requestsstate7/stay pose;
closing sound/timestamp/fade begin on subsequent processing. State8 deletes
only when opacity was ALREADY0 on entry, so closure acknowledgement has a
separate lifetime from discovering the terminal. Return3/other retainsstate.

Consequently source Continue/Add can remain pending for original keys, page
scrolls, explicit retained stops and closure processing. Do not treat “text
revealed,” “choice answered,” “callback issued” and “window poll clear” as the
same event completion. Terminal handling preserves source callback and window
main order; a native backend may adapt the rendering/fade but must state that
adapter and retain the script's actual message/control ownership.

For normal save cancellation, the script's evt_msg_continue resumes the
original prompt window after selection closes. For success, the source appends
save_finish and returns after that Add's poll condition; it does not insert a
new extra Continue or force-close callback. Native cleanup must follow the
actual source window outcome rather than add a script command to conceal a
retained-window discrepancy.

## Validation and remaining work

Fresh read-only `inspect_dol_function.py` listings independently inspect the
five complete bodies: evt_msg_continue,evt_msg_print_add,msgWindow_Continue,
msgWindow_Add and windowCheckID. Their branch/argument/timestamp/publication
order agrees with the linked lifecycle/variant contracts. Original parser and
playback/window-main control branches were checked against existing recovered
local contracts. Inspection ELF/text remains /tmp/sb-message-* and
/tmp/sb-window* outside source history; no original payload/code is added.
Original Nintendo/Intelligent Systems asset rights remain; no redistribution
is implied. Docs-only: no native test/build, Dolphin or hardware claim.

Remaining: full native retained-window/poll adapter, capacity/lifetime/lookup
failure handling, original parser/schedule/selection animation rendering,
matched first-hit/yes/no/error window captures and original card persistence.
