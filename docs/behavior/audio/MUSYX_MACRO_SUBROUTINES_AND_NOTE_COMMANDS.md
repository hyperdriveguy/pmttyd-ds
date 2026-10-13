# MusyX macro subroutines and note commands

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdGosub8027f7fc/d0,
mcmdAddKey8027fbac/d8, voiceIsLastStarted80285ffc/74hex and five inline regions
below. Addresses/offsets/masks hexadecimal; opcodes/scalars decimal. Static
ring/note/request order, not original command/audio/timing observations. Read
[fetch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[lookup](MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md),
[wait](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[last-note setter](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md),
[VID removal](MUSYX_VID_LIFECYCLE_AND_LAST_STARTED_RESET.md) and
[voice free](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md).

## Complete gosub: lookup, ring writes, saturated byte count

Inputs retained voice V/command C. Capture C.word0, request
 dataGetMacro(U16(word.high16)). Lookup0 requests vidRemoveVoiceReferences(V),
then voiceFree(V), ignores returns, returns1. No ring push or code store on miss.

Lookup nonzero B: read index byte8d, store (index+1).low2. Reread index, store
fresh macro base34 at V+6c+index*8. Reread index AGAIN, store fresh PC38 at
V+70+index*8. Then read count8c, increment/store LOW8, test that stored low8>4;
if greater, overwrite count4. Thus incoming255 wraps to0 before saturation.
No full-stack refusal or separate overflow signal; a fifth ordinary push rotates
and overwrites an old entry while count stays4.

Store B to base34; reread C.word4, store PC38=wrap32(B+U16(word.low16)*8).
Return0. Lookup ID and step are not a single post-child command snapshot.
No stack/code/null/member/generation/IRQ guard, returned-pointer extent check,
whole stack clear or restore rollback. Fetch already advanced PC8 before this
call, so the saved PC normally points after the calling command.

## Complete opcode36 return and opcode37 caller

Return region80283274/54 first reads count8c. Count0 joins unchanged. Nonzero
reads RAW index8d, loads V+6c+index*8 into base34; rereads RAW index8d, loads
V+70+index*8 into PC38. Then reads index again, stores(index-1).low2; rereads
count, decrements/stores low8. Own entries stay uncleared. No count<=4/index<=3
check BEFORE raw addressing or success/failure return. Unsupported aliasing can
change fresh reads across base/PC/index/count stores.

Gosub stub802832c8/14 passes V,&shared cstep8041ef08, assigns returned0/1 to
local stop marker, joins. Miss therefore ends this interpreter invocation;
return opcode leaves marker initially0 and normally continues. No caller's
separate handling of free/VID child failure is inferred.

## Complete add-key arithmetic and last-note admission

mcmdAddKey(V,C) captures W0. Delta=S8((W0>>8).low8). W0.high byte!=0 selects
BASE=V.byte131; otherwise BASE=unsigned V.half12e. Add and FIRST store low16 at
half12e. Reread that half, interpret S16: negative clamps0, otherwise unsigned
>127 clamps127; store clamped half. Thus a prior raw32767 plus1 wraps to signed
-32768 and clamps0; this is not a wide sum clamped directly to127.

Reread C.word0, extract bits16..23 via high16/byte store to V.byte130. Request
voiceIsLastStarted(V). Nonzero reads fresh note half/channel121/set122 and requests
inpSetMidiLastNote(channel,set,note.low8); return ignored. No extra channel255
guard here, but the predicate rejects it. Then OVERWRITE C.word0=4 and request
mcmdWait(V,C), preserving word4; return that child's result. This mutates the
shared command for the interpreter caller and uses the ordinary wait handler's
own tick/flag/return rules. No dedicated pitch/sample/hardware/job request here.

## Complete last-started predicate

voiceIsLastStarted reads identityf4; exactffffffff returns0. Channel byte121==255
also returns0. Retain slot=identity.low8, read set122. Set255 reads FX last-started
80406a40+SLOT (not channel), otherwise MIDI80406a80+set*16+channel. Return1 iff
stored byte==slot, else0. No full generation/voice identity-table match,64-slot/
8-set/16-channel bound, mutation or active-system gate. Channel gates FX even
though slot indexes it. This matches the reset writer's exact source domain.

## Other note regions and their distinct last-note policies

| Opcode | Complete region (end exclusive) | Operation |
| --- | --- | --- |
| 24 | 80282f58..f6c | Call add-key; result becomes stop marker |
| 25 | 80282f6c..fc0 | Direct note; conditional last-started publication; wait |
| 26 | 80282fc0..3040 | Base132+signed delta; channel-only publication; wait |
| 36 | 80283274..2c8 | Ring return |
| 37 | 802832c8..2dc | Gosub; result becomes stop marker |

Opcode25 stores (captured W0>>8).low7 at half12e, then rereads SHARED word0,
stores bits16..23 at byte130. Request last-started predicate; true publishes
fresh note/channel/set through last-note setter. Opcode26 starts from byte132
plus S8(captured W0 bits8..15), stores low16 then S16/unsigned clamps exactly as
add-key. Reread shared W0 to store byte130. This route instead reads channel;
channel!=255 publishes fresh note/set, WITHOUT last-started predicate.

Both inline note regions finally overwrite shared W0=4, call mcmdWait(V,&cstep),
assign returned value to stop marker and join. No word4 reset, controller clone,
whole note-state zero, bounds or inferred sound onset. Their publication gates
are distinct; do not apply the last-started test to every note change.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_macro_subroutines_and_note_commands_evidence.json)
pins three bodies/five regions/five linked identities/table:14 spans,162 anchors/
13 calls/five registrations. Six push/four return/six note/six predicate vectors
are mathematical deductions, not original execution. Fresh full ELFs/listings
stay `/tmp`; no original bodies/binary assets committed.

Remaining: actual stack/note/base131/132/last-started/code/command/caller writers,
valid ring/count/index/slot/generation/table/alias/shared-cell/IRQ/concurrency
and original subroutine/note/wait/audio traces; other opcodes and full native/
runtime binding. EVT-01 remains OPEN.
