# Selection window state, input order and event result

US G8ME01 revision 0, recovered 2026-10-12. B evidence is original static
instructions/constants, not executed input or rendering. See [lifecycle](MESSAGE_WINDOW_LIFECYCLE.md),
[parser](MESSAGE_PARSER_RECORDS.md), [playback](MESSAGE_PLAYBACK.md) and
[event runtime](EVENT_RUNTIME.md). Original selection text remains reused data;
options, ordering, branches and story attribution must not be invented.

## Selection fields and update entry (B)

selectWindow_Main at 0x8007ff3c, size 0x324, receives W pointer. Cache message
data D from W+0x28 smart handle's first word, with no local null guard. Always
call msgMain(handle) before inspecting W.state, including terminal/closing state.
Ignore its return code; selection handling is controlled by W.state, not by a
playback-complete condition. No local callback is installed by evt_msg_select.

| Field | Established selection role |
| --- | --- |
| W.u16[0] | state |
| W.u16[2] | pool flags; poll-completion mask 2 |
| W.s16[4] | layer used to derive display order |
| W.s16[6] | opacity-like value used for opening/closing |
| D.s8[f240] | option/line count, populated by parser line processing |
| D.s8[f241] | initial option, copied to f243 by constructor |
| D.s8[f242] | cancel configuration |
| D.s8[f243] | current option/result |
| D.float[f248] | constructor cursor coordinate; this main does not update it |

Storage operations retain low bytes/halfwords. Signed interpretation occurs at
comparisons/results. Counts outside normal small positive ranges are not locally
validated; do not turn them into arbitrary-precision indices or supported UI.

## States, opening/closing and display registration (B)

- State 5: add 25 to W.s16[6] with halfword storage. If resulting signed value
  is strictly greater than 255, clamp to 255 and write state 1. Register display
  on this call, including the transition call. Input starts on a later call.
- State 1: perform ordered navigation/cancel/confirm below, then register display.
- State 7: if W.s16[6] is already zero on entry, set state 4, clear flags mask
  2 and return without display. Otherwise subtract 25, retaining halfword;
  if signed result <=0, clamp to zero. Register display even when just clamped
  to zero. Poll flag clears on the subsequent state-7 invocation.
- State 4: return without display. msgMain has already been called.
- Other states: no local selection transition; register display.

Display registration is dispEntry(8,0,400.0-float(W.s16[4]),
selectWindow_Disp at 0x8007fdf4,W). This is per-invocation registration, not a
new drawing allocation/ownership claim. Increments are per callback invocation,
not measured rendering-frame or simulation-tick durations. Window-manager cadence
and renderer use of W+6/f248 still need tracing.

## Snapshot readers and navigation masks (B)

All navigation reads use controller index 0. Existing keyGetButtonTrg reads
global work +0x1338+4*i. Three additional 0x14-byte helpers are literal loads:

| Function | US address | Global-work word offset |
| --- | --- | --- |
| keyGetDirTrg | 80005f94 | 1388+4*i |
| keyGetButtonRep | 80005fa8 | 1348+4*i |
| keyGetDirRep | 80005fbc | 1398+4*i |

No helper generates repeat timing or checks index bounds. Input snapshot/repeat
producers and physical-controller mapping remain dependencies; exact masks below
are the observed admission contract.

In state 1 save original option byte. Execute the decrement block, then the
increment block independently; simultaneous opposite input is not an else-if.

Decrement block:

1. If button trigger mask 8 or direction trigger mask 0x1000 is set, decrement
   current option byte without an endpoint guard. Skip decrement-repeat handling.
2. Otherwise, if button repeat mask 8 or direction repeat mask 0x1000 is set,
   decrement only when signed current option is nonzero.

Increment block:

1. If button trigger mask 4 or direction trigger mask 0x2000 is set, increment
   current option byte without an endpoint guard. Skip increment-repeat handling.
2. Otherwise, if button repeat mask 4 or direction repeat mask 0x2000 is set,
   increment only when signed current option < signed count-1.

After both blocks, normalize in order: if signed current option<0, write low
byte of count-1; then if signed current option>=signed count, write zero.
New presses therefore wrap for ordinary counts; repeat input at valid endpoints
does not wrap. This distinction depends on the snapshot reader used, not on a
single shared up/down boolean. Opposite inputs can cancel after both mutations.

If signed final option differs from signed saved original, call
psndSFXOn(0x20005). No movement sound when the net result is unchanged. Navigation
sound occurs before cancel/confirm handling, so one invocation can also play a
closing sound. No f248 cursor-position write occurs here.

## Cancel precedence and confirm (B)

After navigation, test button trigger mask 0x200 first:

- Signed cancel configuration -1: ignore cancel and go directly to display.
  Do not subsequently test confirm in this invocation.
- Configuration -2: write result/current option=-1 as byte; state=7; play
  psndSFXOn(0x20013); display.
- Every other value: copy the cancel configuration byte to result/current
  option, without option-count normalization; state=7; same sound/display.

Only when cancel trigger is absent, test button trigger mask 0x100. If set,
write state=7, play psndSFXOn(0x20012) and display. Preserve current option.
No additional validity, playback, message flag, debounce or duration gate exists
on these selection input branches. Other directions/buttons are ignored here.

## evt_msg_select: creation, identity and result lifetime (B)

At 0x800d2090, size 0xe8, receive(event,firstCall). Every invocation resolves
script operands 0 and 1 through evtGetValue before checking firstCall. Operand
0 is flags; operand 1 is key/raw text input.

On nonzero firstCall:

1. If resolved flags numeric mask 1 is clear, call msgSearch(input).
   Use returned text for creation and retain resolved input key as identity.
2. If mask 1 is set, use resolved input directly as text. Identity is static
   NUL string DIRECT at 0x802cc71c; do not use raw text as identity.
3. Call msgWindow_Entry(text,callback=null,layer=20). Save returned index in
   event.word[0x17c], distinct from print repeat/continue's +0x178.
4. Dereference window/handle/data and write identity pointer to D.word[0].
   No local creation-failure or full-pool fallback.

Every invocation polls windowCheckID(savedIndex), i.e. window flags mask 2.
If nonzero, return 0 (event yield). Once zero, obtain the window/data again,
sign-extend D.byte[f243] and write it to event.word[0x9c] (event local work 0).
Then windowDeleteID(savedIndex), ignore its result and return 2. The callback
does not set an arbitrary destination operand. Read the result before freeing
message work; no result pointer to freed data is returned.

Resolved operands on resumed calls are not used to recreate or replace identity.
The stored window index and window main state determine progress. Because closing
mask 2 clears only after an already-zero-opacity state-7 call, the event result
is delayed until that poll point, not merely until cancel/confirm sets state 7.

## Window deletion helpers (B)

windowDeleteID at 0x8007fa54, size 0x6c, computes unchecked pool+0x48*index.
If allocation mask 1 is clear, return 0 without callback/mutation. Otherwise
call W.deleteCallback(W) if nonnull, then set W.flags=0 and W.handle=null,
return 1. It does not zero every window field or explicitly clear W.state.
For selection this invokes msgWindow_Delete and smartFree as documented in
[lifecycle](MESSAGE_WINDOW_LIFECYCLE.md).

windowDelete at 0x8007fac0, size 0x4c, instead receives W pointer and has no
allocation-mask admission guard. Call delete callback if present, clear flags
and handle and return 1. Callback return values are unused in both helpers.
No manager-level display-list retirement or stale-index protection is proven.

## Differential obligations and remaining work (P)

Compare unconditional playback before state dispatch, strict >255 opening,
zero-opacity extra closing call, allocation versus poll flag, trigger wrap versus
repeat endpoint guards, opposing inputs/net sound, cancel-before-confirm including
disabled cancellation, -2 versus arbitrary cancel result, direct/key identity,
first-call-only creation, +17c saved index, signed LW0 result before delete and
callback cleanup order. These are derived obligations, not executed observations.

Selection callback geometry/cursor interpolation is now covered in
[selection drawing](MESSAGE_SELECTION_DRAWING.md), pool/main traversal in
[window manager](MESSAGE_WINDOW_MANAGER.md).
Remaining: inner selection/text highlighting, input snapshot
and repeat cadence, outer/display scheduling, print callback/wait-state producers,
all resource/message callers and original story branch attribution/conditions.
Selection local main/event/deletion contracts are documented; full dialogue and
complete-game equivalence remain unfinished.
