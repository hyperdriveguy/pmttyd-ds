# MusyX macro external key-off, pedal and event traps

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete macSetExternalKeyoff80283ab4/f0,
macSetPedalState80283ba4/108hex; complete opcode40 trap-install802832dc/78 and
opcode41 trap-clear80283354/44 regions. Addresses/offsets/masks hexadecimal;
scalar/opcode literals decimal. Static flag/event/activation order, not original
pedal/key-off/sample timing/audio. Read [activation](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md),
[wait flag consumers](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[existing sample-end notification](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md),
[macro lookup](MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md) and
[message handler](MUSYX_MACRO_MESSAGES_VID_AND_KEY_GROUP.md).

## Fields and common event restoration

Voice V: macro base34, PC38, state4c; event macro bases50/54/58 and respective
PCs5c/60/64; common enable byte68. Flags H=word114, L=word118. High100hex is
pedal-held/defer admission, high400hex pending deferred key-off; low8hex external
key-off publication, low4hex key-off wake test. These roles follow local branch/
mutation uses; they do not establish every writer, hardware or timing meaning.
No sndActive/IRQ/null/identity/member/state-domain guard in these new bodies.

Where the following branches admit the KEY-OFF event: byte68!=0 AND word50!=0,
perform exactly PC38=word5c, base34=FRESH word50, word50=0, then request
macMakeActive(V). Child return ignored; branch considers the event consumed
without checking child success. It does not clear enable68 or event PC5c.
This differs from sample-end's state1/word54/PC60 event and message enqueue's
word58/PC64 event; source fields and admissions must remain distinct.

## Complete external key-off publication/defer/wakeup

macSetExternalKeyoff(V) first reads L then H (initial H read contributes no
subsequent store), stores L|8 to low word118. If fresh macro base34==0, return
AFTER that flag publication. Otherwise freshly read H then L:

- H&100!=0: store captured L unchanged, then H|400; return. No event restoration
  or activation, even if event50 exists or low4 is set.
- H&100==0: attempt key-off event restoration as above. If consumed, finish.
  Otherwise reread H then L; high AND0 contributes nothing; ONLY L&4!=0
  requests macMakeActive(V). Else finish.

No deferred400 clear, low8 clear, forced whole flags reset, sample stop/hardware
key-off/free request, return status or event success predicate. The already-set
low8 can exist on an inactive/base0 voice. A held pedal defers at flag level;
these local instructions do not establish duration or when a job observes it.

## Complete pedal set and release ordering

Input RAW P. P!=0 (FULL word comparison) reads H, stores H|100 and returns.
No macro-base/activity check, low-word store, event request or pending400 clear.
RAW256 therefore sets the held bit just as1 does; do not narrow P to a byte.

P==0: read macro base34. Nonzero base freshly reads H/L; only H&400!=0 admits
attempted key-off event restoration. Consumed event skips fallback; otherwise
fresh flags are read and ONLY L&4!=0 requests macMakeActive. Base0 or pending400
clear skips both event and fallback, even if low4 is set.

After these branches/child calls, ALWAYS reread L THEN H; store L unchanged
(source ANDffffffff), then H&fffffaff. This clears high100 AND high400, retaining
other high bits. Release clearing occurs AFTER activation/event restoration,
so that child sees prior pedal/deferred flags; final values are fresh post-child
reads, not a captured earlier pair. No atomic64-bit flags write, low8 clearing,
physical controller sampling or audible release guarantee.

## Complete opcode40 event installation and special deferred marking

Retained captured command W0 in r26 supplies U16(W0.high16) macro lookup ID;
request dataGetMacro. Lookup0 joins with no event/enable/flag writes. Nonzero
returned code base B:

1. Reread SHARED cstep8041ef08.word0, index i=U8(word>>8), address displacement4*i.
2. Store B at V+50+4*i.
3. Read SHARED cstep.word4; PC=B+U16(word.low16)*8 wrap32; store V+5c+4*i.
4. Enable byte68=1.
5. Only if the fresh index byte i==0, read H then L. If H&100!=0 AND L&8!=0,
   store captured L unchanged then H|400. Otherwise no flag writes.

Index is NOT limited to0..2: all256 byte indices address raw voice offsets here.
No event-code bounds/alignment check, source-PC rewind, activation/job request,
whole event reset or sample/message wakeup. The high captured lookup ID and
fresh shared index/step can come from different commands under unsupported
nested mutation. Installation while key-off already published AND pedal held
marks deferred400; it does not consume the event immediately.

## Complete opcode41 selected clear and three-slot enable scan

From captured W0, i=U8(W0>>8); store0 at V+50+4*i, with no0..2 check. Leave
corresponding event PC untouched. Scan ONLY base words50/54/58 in that order:
first nonzero joins, leaving enable68 unchanged. All three zero sets enable68=0
then joins. No base0-null fallback check, matching ID removal or flags reset.
Clearing an out-of-range i can change other voice memory, while enable decision
still tests the three original event slots; valid index domain remains open.

Both inline regions join80283894 without modifying initial stop marker0.
Normal fetch already advanced PC; no implicit wait/stop is added. Whole
macHandleActive and other opcode paths remain incomplete.

## Sample-end reuse, evidence and remaining bindings

The existing [sample-end contract](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
already closes macSampleEndNotify: state1 admission, event54/PC60 restore first,
otherwise low40000hex wake, with no flag/event-enable clear. Fresh full inspection
and its source manifest revalidated here; no duplicate completion claimed.

[New manifest](../../../tools/musyx_macro_keyoff_pedal_traps_evidence.json) pins
two bodies/two regions/two linked identities/dispatch table: seven spans,127
anchors/five direct calls/two registrations. Six key-off/seven pedal/five trap-
address/five displacement vectors are deductions, not original execution; pedal
vector post-flags assume no child mutation and do not replace fresh-read order.
Full fresh inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.

Remaining: actual flag/event/enable/command/code/caller/pedal/job writers and
consumers, valid event indices/voice state/member/null/alias/shared-cell/reinit/
IRQ/reentrancy/concurrency domains and original key-off/pedal/sample/message/
event/audio traces. EVT-01 remains OPEN.
