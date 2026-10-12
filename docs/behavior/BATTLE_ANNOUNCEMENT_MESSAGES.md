# Battle announcement callback, timer and draw protocol

US G8ME01 revision0, inspected 2026-10-12. B evidence is original instructions
and data. Public function names annotate services. See [baseline](../ORIGINAL_BEHAVIOR.md),
[event runtime](EVENT_RUNTIME.md) and [default action events](BATTLE_DEFAULT_ACTION_EVENTS.md).
Original executable/message extracts remain ignored. No original execution or
Switch comparison was measured. This covers the local announcement service;
full message lookup, control-token parsing and font rendering remain separate
contracts. Exact original US text/IDs/tokens must be reused, never paraphrased.

## Arguments, ownership and first-call state (B)

btlevtcmd_AnnounceMessage at0x801abdd0,size0x1b0 receives(event,firstCall).
Its five script operands are(mode,x,y,messageKey,duration). Every invocation
resolves all five in order through evtGetValue, before any null-key or first-call
test. Capture global battle B before resolving operands. If resolved key=0,
return2 immediately, without display registration, timer decrement or initialization.

On firstCall≠0:

1. Write duration word to event+0x78 (per-event continuation storage).
2. Store low16 bits of x/y at B+0x18c8c/+0x18c8e.
3. msgSearch(messageKey), obtaining the original localized template/text pointer.
4. Format into shared text buffer at B+0x18ca0 according to mode below.

Later calls do not reset duration or format/relookup the text, even though their
resolved operands are read again. x/y and text buffer are global battle storage,
while the timer belongs to the event. Two concurrent announcements can overwrite
each other's shared text/position; no local queue, owner lock or separate buffer
is established by this callback. The outer scheduler must preserve actual
service call order rather than assuming independent per-event banners.

## Text formatting branches (B)

If mode≠1, call sprintf(sharedBuffer,localizedText) without an explicit
substitution argument. Original text/control data must meet this path's contract;
there is no local length bound or synthesized replacement message.

If mode=1, use parameter word B+0x18c90 as numeric item-table index at
itemDataTable0x803108a8+40*index. Its word+4 supplies the item-name message key.
No local index validation occurs. Inspect word+0x16c through the global pointer
stored at r13−0x6f50:

- Word=0: msgSearch(itemNameKey), then sprintf(sharedBuffer,localizedTemplate,
  localizedItemName).
- Word≠0: strcpy(sharedBuffer,localizedTemplate); msgSearch(itemNameKey); call
  unk_800d1364(sharedBuffer,localizedItemName). Its control/substitution algorithm
  remains unrecovered, so it is not equated with ordinary printf substitution.

The pointer/word's gameplay configuration meaning remains a data-binding task.
Both branches use the original localized item name. No hardcoded English or
rewritten plot text is introduced. Message search fallback/encoding/control
semantics are open; a helper name alone does not establish them.

## Parameter setter (B)

btlevtcmd_AnnounceSetParam at0x801abc10,size0x60 resolves(index,value) through
evtGetValue and stores value at `B+0x18c90+4*index`, using word arithmetic.
Return2. It does not clamp index, validate an item ID, initialize a message, or
reset event timers. Index0 sets the word used by mode1 above. Other parameter
slots and their consumers remain open; negative/out-of-range accesses require
original caller invariants, not invented legal gameplay behavior.

## Display registration and optional input (B)

After initialization, or on every resumed call, register
`dispEntry(8,1,callback0x801abc70,parameter0,depth900.0f)`.
The constant is US0x80424bc8. This happens before timer/input processing and
also on the final invocation. There is no local delete/unregister call on finish;
display-list lifetime is a separate service contract.

Only mode exactly2 performs trigger dismissal. Call BattlePadCheckTrigger(0x100)
first. If zero, call BattlePadCheckTrigger(0x200). A nonzero result from either
writes event+0x78=0. Then, for every nonnull-message mode, decrement the timer
word by1 and store it **before** selecting the callback result.

The final instructions return1 when the post-decrement timer word is zero or
has its high bit set; otherwise0. This is the exact count-leading-zeros/rotate
outcome. For ordinary signed positive timers it is `remaining<=0 ? 1 : 0`.
Word wrap still matters:0 decrements to0xffffffff and yields1;0x80000000
decrements to0x7fffffff and blocks0. Do not replace source word arithmetic with
unbounded signed arithmetic on edge inputs.

Result0 leaves USER_FUNC blocked on the same command. Result1 yields and allows
the next command on the next eligible event invocation; it is not the callback
result2 immediate same-invocation completion. A mode2 trigger writes0 then
subtracts1, reaching the high-bit/yield case. No message typing-complete gate is
consulted by this local timer path. Rendering/control parsing may have separate
behavior that must be recovered.

For the zero-gravity script's(mode0,0,0,key,120), absent external timer writes:
first invocation stores120 then decrements119; invocation120 reaches0 and
returns1. Its next camera-mode command runs later under event scheduler rules.
This is a derived call count, not120 measured render frames or two seconds.
Mode0 does not consult dismissal triggers in this callback.

## Trigger helper used here (B)

BattlePadCheckTrigger at0x8011a06c,size0x28 calls
BattlePadMultiCheckTrigger(port0,mask) and propagates the result.
Multi helper at0x8011a054,size0x18 returns
`U32(globalBattle+0xf44+0x1fc*port)&mask`, with no normalization or port bounds
check. It does not consume/clear input or add a repeat policy. The two announcement
checks can observe the same word when called sequentially. Producer/edge-history
semantics and adapted controller prompts remain separate tasks.

## Draw callback local geometry (B)

The local `_disp` symbol in battle_message.o is0x801abc70,size0x160.
It uses current global B, signed x/y halfwords and the shared text buffer; it
receives no per-announcement text/timer payload. Start:
`textY=float(S16(B+18c8e)+120)`, `textX=float(S16(B+18c8c))`.

Call FontGetMessageWidthLine(buffer,&lineWord). Let W be its returned word;
compute halfWidth=`(W>>1)&0x7fff`, width=`W&0xffff`.
Set textX−=float(halfWidth). Let L=U16(lineWord),
nextLines=U16(L+1), windowWidth=width+20,
windowHeight=29*nextLines+3 (word arithmetic).
The font helper's width/control semantics are not inferred here.

Copy the four-byte constant0xffffffff at0x80424bb8 to a local color value.
Call windowDispGX_Waku_col with integer argument0, pointer to that color and
float arguments:

- x=textX−10;
- y=textY−29+3;
- width=float(windowWidth);
- height=float(windowHeight).

Then FontDrawStart(), followed by
`FontDrawMessage(truncToWord(textX),truncToWord(textY)−29,buffer)`.
The positioning centering uses the original integer halfWidth calculation,
not width/2.0f. The draw callback updates only its temporary line/color storage;
it does not decrement timer, clear input or change the global message buffer.
Font/window internals, screen coordinates and port reflow remain dependencies.
Float constants at0x80424bbc/bc0/bc4 are10,−29,3 respectively.

## Differential obligations and remaining work (P)

Compare first versus resumed arguments, null key returning2, mode1 item branches,
shared buffer overwritten by a second event, timer1/0/negative/wrap, mode2 first
trigger short-circuit, display registration on the final invocation, result1
scheduler yield, odd text width centering, line count16-bit narrowing and x/y
argument narrowing. These are proposed original/native comparison cases, not
executed-original observations.

Remaining: msgSearch resource/fallback/encoding, unk_800d1364 substitution,
original US text/control-token parsing, display-list lifetime, font/window
rendering, parameter/configuration producers and other announcement callers.
This callback is not the complete story-dialogue service. Full message and
full-game fidelity remain unfinished.

Local msgSearch/resource loading and command tokenization are now specified in
[message resources](MESSAGE_RESOURCES_AND_LOOKUP.md); full control/glyph parsing
and substitution remain open.
