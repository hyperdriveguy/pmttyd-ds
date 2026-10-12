# Message window allocation, creation and lifecycle controls

US G8ME01 revision 0, recovered 2026-10-12. B evidence is original static DOL
code/data. See [lookup](MESSAGE_RESOURCES_AND_LOOKUP.md), [parser](MESSAGE_PARSER_RECORDS.md),
[objects](MESSAGE_EMBEDDED_OBJECTS.md), [playback](MESSAGE_PLAYBACK.md) and
[event runtime](EVENT_RUNTIME.md). No original execution or complete window/
story equivalence is claimed. Callback names annotate services, not measured
behavior or final enum identities.

## Window pool and poll helpers (B)

windowEntry at 0x8007fb0c, size 0x88, scans seven 0x48-byte records from the
global pool pointer at r13+0x1b08. Choose first slot whose halfword flags at +2
lacks numeric mask 1. Zero all 0x48 bytes, set flags=1 and write low
halfword of its argument to +4. Return index 0..6. All allocated returns -1.
No eviction, retry or allocation fallback occurs here.

windowGetPointer at 0x8007ad40, size 0x10, returns pool+0x48*index without
checking range or allocation. windowCheckID at 0x8007ad50, size 0x18, returns
`flags[index]&2`, exactly 0 or 2, with no bounds check. It does not test numeric
allocation mask 1. Repeat/continue event wrappers use this second flag to poll.

| Window offset | Established local role |
| --- | --- |
| 0 | state halfword used by message lifecycle/main |
| 2 | flags halfword; allocation mask 1, message poll mask 2, wpos mask 4 |
| 4 | layer/priority halfword initialized from entry argument |
| 8 | signed message type word |
| c/10/14/18 | float geometry values |
| 28 | smart handle to message work |
| 3c | main callback pointer |
| 40 | delete callback pointer |

Other pool initialization, update/display ordering and callback invocation remain
window-manager dependencies; these offsets are not a complete manager contract.

## msgWindow_Entry allocation and initialization (B)

At 0x80081688, size 0x394, inputs are (textPointer,callback,layerArgument).
Call windowEntry(layerArgument), then windowGetPointer(returnedIndex). No local
-1/full-pool check. Install main=msgWindow_Main(0x80080c20), delete=
msgWindow_Delete(0x80081630), state=5, type=-1 and OR flags with 2.

Allocate temporary text through __memAlloc(0,strlen(textPointer)), then strcpy
it into that allocation. The local requested size omits the terminating byte;
allocator rounding/slack is a required dependency before judging actual memory
safety. Do not silently describe the original request as strlen+1. Neither
temporary allocation nor input pointer receives a local failure/null guard.

Call smartAlloc(0xf258,1), dereference its first word D, zero all 0xf258 bytes.
No local smart-allocation failure fallback. Initialize:

- D.f24c=window pointer; D.flags=3; D.f1fc=callback.
- D.timestamp[0x30/0x34]=current global work +0x38/+0x3c.
- D.timestamp[0x18/0x1c]=another read of those current clock words.
- D.f228=0, D.byte[f22c]=127, parser T/X/Y/K/S=0, parser V=25,
  history count f040=0 (also zero from whole-buffer initialization).

Call msgAnalize(handle,temporaryText). This parses raw text; it does not perform
msgSearch in this routine. If parsed window type=1, OR D.flags with 0x200.
Free temporaryText with __memFree(0,pointer). Attach the smart handle at W+0x28,
then explicitly set work words f250/f254=0. Return original window index after
geometry setup below. Raw record count, schedule and all initially untouched
fields start zero because of the full memset, resolving the normal creation
case for record schedules left unwritten by particular parser branches.

D.word[0] also starts zero; this constructor does not assign a message identity
string there. ForceClose's identity therefore depends on later event/callback
initialization, not just the text passed here.

## Geometry by resulting message type (B)

For types 0,2,3,4,6,7,8,9,10, use (-280,225,560,176) at W+c/10/14/18 unless
window flags mask 4 is set by wpos. Type 11 changes main to msgWindow_Clear_Main
(0x8008093c), then takes that same conditional default geometry path.
Other types outside these branches receive no geometry assignment here.

Type 5 installs selectWindow_Main (0x8007ff3c) and always derives geometry,
regardless of wpos mask. Using signed halfwords width=D.f244 and y=D.f246,
and signed byte lines=D.f240:

- W+c=float(s16(trunc((-width)/2))), with integer division toward zero.
- W+10=float(y), W+14=float(width).
- W+18=float(s16(36*lines+40)); this path retains halfword narrowing.
- Copy D.byte[f241] to f243. Set D.float[f248] to
  `W.float[10]-float(31*s8(D.byte[f243])+58)`.

Type 1 writes low16(layerArgument+10) to W+4. Obtain texture object 3 with
getWakuTexObj and its unsigned-halfword GX width/height; set W+14/18 to each
dimension multiplied by binary32 0x3f666666 (approximately 0.9). It does not
assign W+c/10 on this branch; they retain pool-zero or parser-supplied values.
Texture asset bindings and actual window drawing remain unmeasured.

## Add, Continue and Repeat (B)

All use unchecked windowGetPointer and assume a valid message handle.

msgWindow_Add at 0x800815bc, size 0x74, inputs (additionalText,windowIndex).
Set D.lastTimestamp at +0x30/+0x34 from current global clock, then call
msgAnalize(existingHandle,additionalText). Existing parser count, persisted
coordinates and parser T/V are retained, so the parser appends over its prior
terminal slot. There is no local resize, capacity guard, clear, lookup, string
copy or geometry reconfiguration. Then set W.state=1 and OR W.flags with 2.

msgWindow_Continue at 0x8008156c, size 0x50, receives windowIndex. Set W.state=1,
OR flags with 2 and refresh D.lastTimestamp +0x30/+0x34. It does not locally
reset playback index/E/scroll, parser data, key ordinal or wait state.

msgWindow_Repeat at 0x800814f4, size 0x78, receives windowIndex. Set state=1,
OR flags with 2. Refresh both timestamps (+0x30/+0x34 and +0x18/+0x1c), then
zero E(+0x20), target/current scroll(+0x24/+0x28) and playback cursor(+0x10).
It does not reparse text or reset parser count/coordinates, wait/key fields,
message flags, sound settings, history, callback or window type/geometry.
Whether external state changes permit intended replay still needs main/caller
tracing; Repeat is not a whole-message reset.

## Deletion and identity-based force close (B)

msgWindow_Delete at 0x80081630, size 0x58, receives W pointer. If W+0x28 is null,
return. Otherwise read signed D.recordCount at +0x38. For positive count, the
binary executes an empty decrement-counter branch loop count times; it does
not visit glyph records or release embedded poses in that loop. Call smartFree
on the handle and null W+0x28. No local state/flag reset, NPC restoration or
individual embedded-pose deletion. Window-manager/smart allocator and other
callbacks determine remaining ownership and cleanup.

msgWindow_ForceClose at 0x80081424, size 0xd0, receives an identity string.
Scan indices 0..6. Skip if W.flags=0, W.state=0, handle=null or D.word[0]=null.
Exact strcmp(D.word[0],identity) must match; no identity null guard. On match
write W.state=7. If D.flags bit 8, call BtlUnit_ChangeStayAnim(D.word[f224]);
otherwise npcSetStayPose(D+f204). Continue scanning, allowing multiple matches.
It neither clears W.flags mask 2 nor frees the handle locally. Message closure
and completion require the main callback to process state 7.

## Event bridge return and polling protocol (B)

evt_msg_close at 0x800d2178, size 0x30, loads operand 0 from event argument
array (+0x18), resolves it with evtGetValue(event,operand), passes result to
msgWindow_ForceClose and returns 2 immediately. It does not wait for deletion.

evt_msg_repeat at 0x800d21a8, size 0x54, and evt_msg_continue at 0x800d21fc,
size 0x54, have the same protocol. On nonzero firstCall only, read windowIndex
from event word +0x178 and call the corresponding lifecycle helper. This index
is stored event work, not a newly resolved script argument. Every invocation
calls windowCheckID(event.word[0x178]); nonzero returns 0 (event yield), zero
returns 2 (callback complete). No local handle validity or stale-index check.
See [event runtime](EVENT_RUNTIME.md) for callback result interpretation.

## Remaining lifetimes and differential obligations (P)

Compare pool first-free order/full failure, allocation versus poll flags,
creation zeroing and timestamp reads, type/wpos geometry precedence, signed
selection dimensions/lines, small texture-derived dimensions, append over the
terminal record, Continue's preserved playback, Repeat's partial reset,
null/positive-count deletion, multiple identity matches, force-close state
versus free, and first-call-only wrapper mutation. These are static-derived
comparison cases, not measured original execution.

Remaining: msgWindow_Main/Clear/selection state machines and renderer, message
event print creation/identity/callback producers, wait-state inputs, allocator
rounding/failure and embedded-pose cleanup, pool update/delete ordering and all
story script attribution/order/conditions. The lifecycle helpers are locally
documented; full dialogue and complete-game behavior remain unfinished.
Selection state/input/result and manager deletion helpers are now recovered in
[selection](MESSAGE_SELECTION.md).
General/clear state transitions, wait/history input and local closure are now
specified in [window states](MESSAGE_WINDOW_STATES.md); rendering and manager
cadence remain open.
