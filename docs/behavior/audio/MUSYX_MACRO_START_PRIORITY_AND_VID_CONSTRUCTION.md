# MusyX macro start and VID construction

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete macStart80283f54/438hex,
seqGetMIDIPriority80279f14/1chex, vidMakeNew80284850/114hex.
Addresses/offsets/masks hexadecimal; counts/argument indices decimal.
Static source request/state/identity order, not original allocation/audio execution.
Read [spawn ABI](MUSYX_MACRO_SPAWN_LINKS_AND_FX_CONTROLLER_CLONE.md),
[macro list transitions](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md),
[lookup](MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md),
[controller defaults](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
and [active initialization](MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md).

## Argument retention, lookup and priority/allocation admission

Use A1..A16 for raw arguments in the linked spawn ABI. GPR A1..A8 are retained
as words. Stack arguments are narrowed at entry: A9/A10/A13/A14 to low8,
A11/A12 to low16; A16 is a full word. A15.low8 is loaded later during pending
field publication. No whole input snapshot or global active/IRQ/type guard.

Request dataGetMacro(raw A1). Return0 immediately returnsffffffff without voice
allocation. Otherwise retain code pointer B. FX mode is A5 bit80hex. If clear,
request seqGetMIDIPriority(A9.low8,raw A8). Returned low16!=65535 replaces
priority A2 with returned low8;65535 preserves raw A2. FX mode skips this query.

Complete priority getter returns halfword at
803f8200+set.low8*20hex+channel.low8*2, without a set255 FX special path, bounds/
active/IRQ check or default assignment. Original table has128 halfwords (eight
sets/16 channels); it does not enforce those extents. The default65535 policy
belongs to macStart's caller-side comparison, not a missing-value repair here.

Request voiceAllocate(resolvedPriority,raw A3,raw A4,FXboolean). Exactffffffff
returnsffffffff. Other slot S selects C=synthVoice8041ee90+wrap32(S*458hex),
without slot-byte truncation/bounds/count verification. Retain C and request
vidRemoveVoiceReferences(C) BEFORE reading its fresh macro state word4c.
Allocator/reference-removal internals remain linked contracts, not inferred
from this admission graph.

## Old active/wait state cleanup

Fresh state2 skips old active/wait cleanup. For other states, state0 unlinks
C from active next3c/previous40 using fresh neighbor reads/root8041ef1c; other
states skip active unlink. Then read deadline HIGH98/LOW9c. Pair0 skips remaining
wait cleanup; nonzero/non-all-ones unlinks next44/previous48 from time root
8041ef18. All-ones skips that unlink. Any nonzero pair then clears deadline
LOW/HIGH, snapshots macro-clock HIGH then LOW into last-time LOWa4/HIGHa0,
and clears fresh low118 bits40004hex while rewriting high114 unchanged.
Set state4c=2 after this non-state2 path. No own-link wipe, general unknown-state
repair, duplicate/member/cycle guard or independent child success check.

Common preparation reads fresh low118 then high114; store low=(old&10hex)|2,
high=0. Request hwIsActive(raw S). Nonzero rereads low and ORs1. Clear deadline
LOW9c/HIGH98 unconditionally. These local writes prepare later active initialization;
they do not prove hardware break/start or audible voice state.

## FX versus MIDI pending-channel preparation

FX mode sets C.byte11d=1, truncates selected key to LOW7, requests
inpResetMidiCtrl(S.low8,255,1), then inpResetChannelDefaults(S.low8,255).
Store pending byte216=S.low8,217=255,218=0. Non-FX sets byte11d=0 and stores
pending216=A8.low8,217=A9.low8,218=A10.low8. No second priority query or channel/
set/rate bounds gate. Controller reset/default child effects precede the remaining
record publication; their results are ignored.

Construct internal ID J=(A1.low16<<16) OR (selectedKey.low8<<8) OR RAW S.
The slot is not masked before OR; valid slot bounds must prevent overlap with
key/macro fields. Store the following local fields in source order. A12 was
retained low16 but its byte store further truncates; A16 is tested as a full word.

| C field | Published source/value |
| --- | --- |
| 104 (width2) | A1.low16 |
| 100 (width4) | A4.raw |
| 110 (width4) | 75300000hex |
| 10e (width2) | 1024 |
| 34 (width4) | macro pointer |
| 38 (width4) | macro pointer+A11.low16*8 |
| 131 (width1) | selected key.low8 |
| 12e (width2) | selected key.low8 |
| 130 (width1) | 0 |
| 214 (width1) | A6.low8 |
| 215 (width1) | A7.low8 |
| 219 (width1) | A12.low8 |
| 8c (width1) | 0 |
| 8d (width1) | 0 |
| ec (width4) | ffffffffhex |
| f0 (width4) | ffffffffhex |
| 108 (width4) | ffffffffhex |
| 21a (width1) | A14.low8 |
| 21b (width1) | A15.low8 |
| 21c (width1) | A16.raw==0 |
| 442 (width1) | 0 |
| 441 (width1) | 0 |
| 440 (width1) | 0 |
| f4 (width4) | compound ID |

The table omits register-only intermediates but preserves these stores' order.
PC uses the retained code pointer plus A11.low16*8; no code-range/alignment check.
EC/F0 and child reference108 becomeffffffff, not a copied prior chain. The
compound ID is stored before priority/VID child work. No whole-voice memcpy,
controller/matrix initialization, sample upload or original playback guarantee.

## Priority/VID result, active publication and failure cleanup

Request voiceSetPriority(C,resolvedPriority), then vidMakeNew(C,A13.low8).
Retain raw VID return R. R!=ffffffff with fresh C.state4c==0 returns R without
another active insertion. Otherwise inspect fresh deadline: nonzero/non-all-ones
unlinks the time queue; all-ones skips unlink. Any nonzero pair requests
synthForceLowPrecisionUpdate(C), clears deadline, snapshots macro clock into
last-time and clears low flag bits40004hex/high unchanged, in that order.

Then load active root; C.next3c=root; if nonnull reread root and store root.prev40=C.
Set C.prev40=0, activeRoot=C, C.state4c=0, then return R. No ID/generation/C-pointer
recheck after priority/VID children, duplicate/root membership validation or
atomic list publication. Child effects can change which branch/neighbor is read.

R==ffffffff requests hwIsActive(raw S); nonzero requests hwBreak(raw S).
Then request voiceFree(C) and returnffffffff. No local VID counter/resource/
controller rollback is supplied; actual child cleanup owns those consequences.
Lookup/allocator failure occurs earlier without this selected-voice cleanup.

## Complete VID candidate selection and free-node admission

vidMakeNew(V,K) uses freeRoot8041ef28, activeRoot8041ef2c and public counter
8041ef30. Read counter into candidate; increment locally, skip exactffffffff,
store next counter. Counter advances BEFORE checking free-node availability.
Capture active root and traverse sorted word0 successors comparing full unsigned
node.word8 IDs. Stop at a node ID>candidate; equality consumes another fresh
counter candidate with the same sentinel skip. If that new candidate is below
the fresh current node ID (wrap), restart at the SAME captured root/previous0;
otherwise continue. No root reread, cycle/count limit or sortedness/uniqueness
validation. Counter exhaustion/reentrancy/alias invariants remain separate.

After locating insertion position, pop free root P. Null returnsffffffff with
counter updates retained. Nonnull: freeRoot=P.next; if nonnull next.prev4=0.
Insert P before the current active cursor using retained previous: write active
root or previous.next, P.prev4, P.next0, then nonnull next.prev4=P. Set P.word8
candidate public ID; read V.wordf4 and store P.wordc internal ID.

RAW K!=0 stores V.wordfc=P, otherwise stores0. Always store V.wordf8=P.
K!=0 returns candidate PUBLIC ID; K==0 rereads V.wordf4 and returns that fresh
INTERNAL ID. There is no V-null/full-ID sentinel/generation/count/bounds guard,
existing-V association removal, copied-reference validation or explicit rollback.
Mode0 return can differ from the earlier internal ID record store under external
mutation. macStart narrows A13 to a byte before this selector; direct constructor
calls use K's full-word nonzero test. The spawn handler passes0 and therefore
requests the internal-ID route, while arbitrary macStart callers can select public.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_start_and_vid_constructor_evidence.json)
pins three complete bodies/12 linked fingerprints:15 spans,234 instruction
anchors,15 direct calls. Six key-mode/five priority/four address/five compound-ID/
five state/four flag/four VID-counter/four return-mode vectors and24 ordered
field bindings are deductions, not original execution or provider validity proofs.
ABI entry hashes do not close complete save/restore fallthrough chains. Fresh
inspection ELFs stay `/tmp/inbox-macStart.elf`, `/tmp/inbox-seqGetMIDIPriority.elf`,
`/tmp/inbox-vidMakeNew.elf`; no original body/binary asset committed or code executed.

Remaining: voiceAllocate/voiceSetPriority/reference/free/resource services and
actual argument/registry/ID/VID-counter/list/pending/channel/rate/flag/clock writers,
valid slot/member/generation/sorted/cycle/alias/IRQ/reentrancy/concurrency/rollback
and public/internal-ID domains, remaining interpreter opcodes and original spawn/
allocation/refusal/activation/audio traces. Three local bodies are answered;
EVT-01 and full native/source/runtime/hardware equivalence remain OPEN.
