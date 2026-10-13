# MusyX macro spawn, voice links and FX controller clone

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdPlayMacro8027fa20/18chex,
synthFXCloneMidiSetup80279070/84hex and opcode8 stub80282d34..80282d44
(exclusive end). Addresses/offsets/masks hexadecimal; argument indices/counts
decimal. Static call/link order, not original polyphony/start/audio execution.
Read [dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[FX copy](MUSYX_INPUT_DESCRIPTOR_BUILDER_FX_COPY_AND_TRANSLATION.md),
[public/internal ID chains](MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md) and
[message suppression](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md).

## Opcode stub and key preparation

Opcode8 requests mcmdPlayMacro(parent V,&shared cstep8041ef08), ignores its
return and joins with interpreter stop marker0. The parent PC already advanced
by8; spawn success/failure does not itself stop this invocation. No private
command snapshot or parent-generation/active/IRQ check.

The handler captures command word0, reads unsigned V.byte131 and adds the
SIGNED8 byte at word0 bits8..15. Clamp the sum to0..127. Read V.byte11d;
nonzero ORs80hex into the clamped key. This is the exact key argument, not a
signed-note clamp or modulo wrap. Raw voice note bytes above127 still clamp.

Store V.byte11c=1 BEFORE constructing the remaining call arguments. This is
the byte checked by hardware message0; the setter is not a saved nesting count
or rollback guard. Null/alias/reentrancy/context policy remains separate.

## Complete macStart raw argument request

Retain command word1=E.word4, then construct the following raw arguments for
macStart80283f54. GPR arguments1..8 use r3..r10; remaining arguments are words
in ascending stack slots beginning SP+8:

| Argument | Exact value |
| --- | --- |
| 1 / r3 | Freshly reread E.word0.high16 |
| 2 / r4 | Captured word1 bits16..23 byte |
| 3 / r5 | Captured word1 top byte |
| 4 / r6 | Fresh V.word100 |
| 5 / r7 | Prepared key.low8 |
| 6 / r8 | (Fresh V.word158>>16)&255 |
| 7 / r9 | (Fresh V.word174>>16)&255 |
| 8 / r10 | Fresh V.byte121 |
| 9 / SP+8 | V.byte122 |
| 10 / SP+c | V.byte123 |
| 11 / SP+10 | Captured word1.low16 |
| 12 / SP+14 | V.byte120 |
| 13 / SP+18 | Literal0 |
| 14 / SP+1c | V.byte11e |
| 15 / SP+20 | V.byte11f |
| 16 / SP+24 | 1 exactly when V.byte197==0, otherwise0 |

Stack-source reads/stores occur in table order before the final fresh word0/
GPR source loads; key/clone flag were read earlier. No normalization of macro ID,
raw word100, channel/set/studio/step or pending voice fields beyond the explicit
byte/halfword selections. These register positions do not claim the complete
macStart parameter/provider contract. Its body is linked here, not recovered
as complete by this caller evidence.

After macStart returns raw J, store V.byte11c=0 BEFORE its success branch.
This is a fixed zero, not restoration of the prior byte or a child-written value.
No caller/parent identity/lifetime recheck. J==ffffffff is failure: store
V.word108=ffffffff and finish, leaving the parent EC chain unchanged locally.
Failure does not undo earlier suppression/flag reads or child effects.

## Complete success publication and chain splice

For J!=ffffffff retain displacement=J.low8*458hex. Load synthVoice8041ee90,
select child C using that displacement; read C.wordf8 as pointer P, then
P.word8 and store at parent V.word108. This is a VID-record word, not raw J;
no P-null/generation/public-ID validation.

Then reload synthVoice, read fresh V.wordf4, select C with retained displacement
and store C.wordf0=parent full ID. Read old O=V.wordec:

1. If O!=ffffffff, reload synthVoice and store C.wordec=retained O.
2. STILL within that admitted branch, reread V.wordec as O2, reload synthVoice,
   select voice O2.low8 and store its wordf0=RAW J.
3. Store V.wordec=RAW J, regardless of old-O branch.

The old-successor test uses O; successor selection uses fresh O2. No O2==O,
full selected-voice.wordf4 identity, new-child ID/count/member/alias/cycle check
or snapshot/rollback. If old O wasffffffff, C.wordec is not explicitly set here;
its value remains whatever macStart/other writers supplied. Parent publication
happens after predecessor/successor stores. A source-bound native implementation
needs the actual valid chain domain rather than silently repairing it.

Finally read FRESH V.byte11d. Nonzero requests
synthFXCloneMidiSetup(fresh synthVoice+retainedChildDisplacement,parent V).
This late admission can differ from the earlier byte11d used to set key bit80.
Child clone return ignored. No copied suppression byte, parent-PC/stop transition
or independent status. The interpreter stub ignores the handler's incidental
return, including the clone's register residue.

## Complete limited FX controller clone

synthFXCloneMidiSetup(DestinationVoice,SourceVoice) retains both pointers and
requests inpFXCopyCtrl(controller,destination,source) for controllers:

`7 → 10 → 91 → 128 → 132`.

Every request uses the complete FX-copy child's fresh full-ID.low8 row selection;
there is no captured common row ID across all five calls. Expanded stable byte
copy order is7,39,10,42,91,128,129,132,133. It is not a full134-byte controller
copy or a matrix/variable/last-note/channel-default clone. No dirty flag,
re-evaluation, cache store, hardware request, active/ID/IRQ/bounds validation
or refusal handling in this wrapper. Actual downstream invalidation and source/
destination identity/lifetime domains remain open.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_spawn_and_fx_clone_evidence.json) pins two
complete bodies,one complete opcode stub,two linked fingerprints (macStart and
prior FX-copy),dispatch table: six spans,86 instruction anchors,eight direct
calls. It records opcode8,seven key/seven returned-ID/three packed argument
vectors,five clone-controller/nine expanded byte order, suppression/chain store
order. These are source deductions, not original spawn/link/clone execution.
Whole child fingerprint does not close macStart semantics. Fresh full ELFs/
listings stay `/tmp/inbox-mcmdPlayMacro.elf`,
`/tmp/inbox-synthFXCloneMidiSetup.elf` and prior macHandleActive files.
No original bodies/binary assets committed or original code executed.

Remaining: complete macStart and allocation/resource/VID constructors, actual
command/pending/key/ID/public-node/EC/F0/clone/suppression writers and lifetime
owners, valid slot/member/generation/alias/cycle/IRQ/reentrancy/concurrency/
invalidation domains and original spawn/refusal/link/clone/audio traces.
Two local bodies/opcode stub are answered; remaining interpreter opcodes,
EVT-01 and full native/source/runtime/hardware equivalence remain OPEN.

### Start/VID child follow-up

[Complete macStart/MIDI-priority/VID construction](MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md)
now closes the exact start child and public/internal return selector. Spawn
passes selector0, requesting internal ID; allocator/priority/resource/free
providers and actual ownership/rollback/original audio domains remain open.
