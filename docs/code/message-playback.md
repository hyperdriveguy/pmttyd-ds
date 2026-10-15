# Native message parser, playback and selection

Implementation: [playback](../../source/message/message_playback.cpp),
[API](../../include/message/message_playback.hpp),
[selection](../../source/message/message_selection.cpp),
[selection API](../../include/message/message_selection.hpp).
Original evidence: [parser records](../behavior/message/MESSAGE_PARSER_RECORDS.md),
[playback](../behavior/message/MESSAGE_PLAYBACK.md),
[window states](../behavior/message/MESSAGE_WINDOW_STATES.md),
[selection](../behavior/message/MESSAGE_SELECTION.md).
These notes describe the implemented native scope, not complete original message
execution or measured original timing.

`open()` builds a fresh temporary playback from original ID/text bytes. Unsupported
controls and malformed operands fail without changing resident playback. Native
admission guards include 64 KiB text, tag length below 128 bytes, finite positive
scale <=8, valid single-byte glyphs and a record count below 2560 before terminal
append. Unknown controls are never silently removed or rewritten. Successful open
replaces prior state with defaults; callers apply opt-in presentation settings to
the new playback. A fresh process serial identifies a native draw-cache lifetime,
not the message ID or a story event.

Parser X/Y/key/scroll/baseline retain signed-halfword narrowing. Schedule uses
wrapped U32 arithmetic, default speed 25. Ordinary advance is scaled glyph width
plus spacing 2. `scaleX` preserves vertical scale and marks style `0x10000`;
style bits 1/2/4 carry dynamic/shake/wave. These style flags differ from playback
acceleration flags. Control records reuse fields as opcode-specific payloads.

| Record code | Reached role |
| --- | --- |
| `0xffff` | Terminal, cursor remains on it |
| `0xfffe` | Key ordinal gate, parser adds 100 to schedule |
| `0xfffd` | Scroll request, parser adds 50; retains original S-3/Y-6/Y-3 arithmetic |
| `0xfffc` | once_stop, parser adds 50; playback advances cursor then stops |
| `0xfffb` | Line feed |
| `0xfff8` | Icon ID/placement/scale/aux payload |
| `0xfff7` | Color payload; `/col` is RGBA `000000ff` |
| `0xfff5` | Input-acceleration disable toggle |

`pos` second operand 999 leaves Y/scroll unchanged. One LF immediately after a
control is consumed without a line record. Ordinary LF uses pitch 32 times the
line's maximum vertical scale, with factor .7 for small spacing. Selection LF
also grows choice count and tracks automatic width; native signed-byte and shift
guards do not establish arbitrary option-count rendering support.

Supported icon names are STICK `0x81`, PAD_Y_ON `0x73`, otherwise decimal IDs.
The original descriptor's first frame provides size; parser retains fused
placement, halfword narrowing, aux low16 and advance +40. Missing icons reject
instead of substituting artwork. This record path differs from the separate
plain-font consumers described in [drawing](message-window-drawing.md).

`append()` reparses exact accumulated raw bytes to rebuild record style/schedule/
coordinates. It replaces records/raw and resumes playback, while preserving the
existing identity, geometry, serial and pose state. Temporary parsed metadata is
not assigned wholesale. `resume()` also reopens playback/poll state without
resetting records, cursor or schedule. Neither API authors dialogue.

Playback consumes caller-normalized time, not the original timebase division
service. It implements a finite subset of source callbacks/history/audio behavior.
Scroll takes precedence over waits and records. The original binary32 factor
`0x3eb851ec` uses fused addition followed by truncation; acceleration repeats that
addition/truncation, rather than multiplying scroll by 100. Wait 1 preserves last
time; acknowledged wait 2 increments key ordinal and adds 100 instead of elapsed
delta. Ordinary schedule advance uses low32 elapsed and wrapped 100x acceleration.
A/B mask `0x300` latches acceleration unless disabled by flag `0x80`.

The unsigned schedule loop may consume multiple records in one call. Setting key
wait or scroll does not itself break that loop. Native play results are 0 terminal,
1 once_stop, 2 wait, 3 progressing and 4 opt-in terminal-input adaptation. Ordinary
window states are 0 inactive, 1 playing, 2 stopped, 3 key wait, 5 opening start,
6 opening fade, 7 closing start, 8 closing fade. Native state 9 is the optional
handheld terminal gate, **not** original history state 9. It adds no original token
and avoids a duplicate gate if a source key already follows the last glyph.

Open/close envelopes use elapsed*255/200, with strictly >255 opening completion.
Closing is retired on the next invocation after alpha reaches zero. Talk countdown
runs before state dispatch, is set to 15 by eligible glyphs, and publishes separate
Talk/Stay request serials. No measured frame cadence is implied. Backwards `now`
causes no tick update.

Selection has a different state machine: 4 complete, 5 opening, 1 input, 7 closing.
It starts at the terminal record to expose choice text, with constructor cursor Y
`windowY-(31*choice+58)`. Opacity changes by 25 per selection invocation, with an
extra zero-opacity closing call. Decrement and increment blocks run independently;
new triggers wrap, repeats stop at endpoints. Mask 8/`0x1000` decrements and
4/`0x2000` increments. Net change queues sound `0x20005`.

Cancel `0x200` precedes confirm `0x100`, even if cancel is disabled (-1). Cancel
-2 produces signed result -1; other cancel values copy unchanged. Cancel/confirm
queue `0x20013`/`0x20012` and start closing; they do not immediately complete the
selection lifetime. The caller consumes the retained result and owns event/story
branch conditions. Native selection does not establish the full original window
manager's unconditional msgMain/display-registration protocol.
