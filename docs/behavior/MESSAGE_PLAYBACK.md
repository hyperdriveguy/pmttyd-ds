# Message playback, clock advancement and record side effects

US G8ME01 revision 0, recovered 2026-10-12. B evidence is static original
instructions/data, not executed timing. msgMain at 0x80083f34, size 0x728,
has no implementation in the pinned Japanese source. See [parser records](MESSAGE_PARSER_RECORDS.md),
[embedded objects](MESSAGE_EMBEDDED_OBJECTS.md) and [lookup](MESSAGE_RESOURCES_AND_LOOKUP.md).
Original dialogue and control records remain original assets. Switch behavior
is not established by this contract.

## Playback work and callback protocol (B)

Input is a smart handle. Cache D=handle.firstWord with no local null guard.
The routine sometimes reloads handle.firstWord after callbacks, while retaining
cached D for other accesses. Do not assume callbacks can relocate/free D safely
without tracing the allocation and caller contract.

| Offset in D | Observed playback role |
| --- | --- |
| 4 | flags, described below |
| 8 | unsigned halfword accepted key ordinal |
| a | unsigned halfword wait state |
| c | unsigned halfword set to 1 when wait state 2 resumes |
| 10 | word playback record cursor |
| 20 | word current schedule coordinate E |
| 24/28 | signed word scroll target/current position |
| 30/34 | last clock timestamp, high/low words |
| 38 | parser count; not used as this loop's record bound |
| 3c onward | 24-byte record array |
| f040 | signed history count |
| f044 onward | history words |
| f1fc | callback function pointer |
| f204 | string used for default NPC lookup |
| f228 | word written by sound-ID record |
| f22c | sound volume byte |
| f24c | window pointer |
| f250 | word countdown set to 15 by some generic records |

At entry clear the global sound latch at r13+0x1b18 (0x8041ea38), once for
this invocation. If D.flags bit 2 is set, invoke callback(0,handle) if present,
then clear bit 2 on cached D. If freshly loaded D.flags lacks 0x200 and its
signed history count is less than 110, append zero when count is zero or the
latest history word is nonzero. Append writes oldCount+1 to f040 and zero to
f044+oldCount*4. No local negative-count protection or callback result check.

Callback events observed in this routine are:

| Event argument | Trigger |
| --- | --- |
| 0 | entry flags bit 2 |
| 1 | terminal opcode 0xffff |
| 2 | matching key-wait opcode 0xfffe |
| 3 | wait state 2 resume path |
| 4 | line opcode 0xfffb |
| 5 | generic record path, including some control opcodes |

All receive the original handle as second argument; return values are unused.
Callback bodies and other producers of flags/wait state remain separate work.

## Elapsed clock conversion and input acceleration (B)

Current 64-bit timestamp is read from global work +0x38/+0x3c. Subtract D's
last timestamp with carry/borrow. Build a denominator from the word at
0x800000f8: `q=(word>>2)` then
`den=(high32(q*0x10624dd3)>>6)`. Pass signed 64-bit delta and (0,den) to
__div2i at 0x80264ad8. This is the original integer conversion route, annotated
as a millisecond quotient; clock producers, runtime division corner cases and
zero denominator policy are not implemented or dynamically measured here.

Input helper keyGetButtonTrg(0), at 0x80005f80 size 0x14, reads global work
word +0x1338. For general index i it reads +0x1338+4*i, with no local bound.
If cached D.flags lacks 0x80, use mask 0x300, expanded to 0x700 when freshly
loaded D.flags has 0x100. A nonzero trigger intersection sets cached flags bit
4. This routine does not clear that bit just because a later trigger is absent.
Acceleration is active only when bit 4 is set and bit 0x10 is clear.

## Scroll has precedence over waiting and playback (B)

If signed target at +0x24 is greater than signed current at +0x28 on entry,
take the scroll branch before inspecting wait state or records.
Convert the entire elapsed quotient through __cvt_sll_flt (0x80264e70), then
set current to `trunc(fma(binary32(0x3eb851ec), elapsedFloat, float(current)))`.
The factor is approximately 0.36, an original binary32 constant, not a measured
scroll rate. Check input/set bit 4 as above. If acceleration is active, perform
a second elapsed calculation and the same addition again: two additions and
two truncations, not multiplication by 100 and not one doubled fused expression.
Signed-clamp current to target if it overshoots. Save the current global timestamp
into D+0x30/+0x34 and return 3 without visiting records.

If target is equal to or below current at entry, no correction or scroll animation
occurs locally; execution proceeds to wait/schedule handling.

## Wait states and ordinary schedule advancement (B)

After the scroll gate:

- Wait state 1 returns 2 immediately. No timestamp update, input polling or
  record processing occurs on that branch.
- Wait state 2 writes D.halfword[c]=1, increments the unsigned key ordinal
  with halfword storage, sets wait state zero, calls psndSFXOn(0x2003d), adds
  100 to E, and invokes callback(3,handle) if present. It does not apply ordinary
  elapsed advancement on this resume branch.
- Other wait states, including zero, calculate elapsed quotient. Retain only
  its low word for the schedule increment. Poll input as above. If acceleration
  is active, multiply that low word by 100 with word arithmetic. Add to E.

For both resume and ordinary paths, save current global timestamp into D+0x30/
0x34, then start record processing. Resume and ordinary paths can therefore
reach multiple records in this same invocation.

## Record admission, bounds and return codes (B)

Start local index I=D.word[0x10], pointer R=D+0x3c+24*I. Before each record,
require signed I<2560. There is no lower bound and no comparison to parser
count D+0x38. Require unsigned R.word[c] <= unsigned E, even for opcodes whose
parser branch left +c untouched. A future schedule stops the loop before opcode
or side-effect inspection. Allocation/initialization invariants are needed for
animation, paper-end and audio records with untouched schedule fields.

Dispatch opcodes 0xfff2..0xffff through a 14-entry table at 0x8030f630; all
others use the generic path. Most cases increment I and advance R by 24, then
continue. A callback or newly set wait state does not itself break this loop.
At bound/future-schedule exit save I at +0x10 and return 2 if wait state is
nonzero, otherwise 3. Early returns are specified below; no universal finished
boolean or one-record-per-call rule is equivalent.

| Opcode | Target | Local side effect |
| --- | --- | --- |
| ffff | 80084310 | callback(1,handle); save I without increment; return 0 |
| fffe | 8008433c | Key ordinal gate, detailed below |
| fffd | 8008438c | Scroll target/history update, detailed below |
| fffc | 8008440c | Save I+1; E=record schedule; clear flags bit 4; return 1 |
| fffb | 80084430 | callback(4,handle); advance |
| fffa | 80084454 | D.word[f228]=R.word[0]; advance |
| fff9 | 80084464 | D.byte[f22c]=low8(R.word[0]); advance |
| fff8 | 8008461c | Advance only in msgMain; icon rendering belongs elsewhere |
| fff7/fff6 | 8008454c | Generic path, including callback/audio flag inspection |
| fff5 | 80084474 | Trigger disable toggle, detailed below |
| fff4 | 8008461c | Advance only in msgMain; pose rendering belongs elsewhere |
| fff3 | 800844b4 | Enable paper pose, detailed below |
| fff2 | 8008450c | Paper end with wait-dependent index handling |

The terminal cursor remains on its terminal record. If called again with the
same state it can invoke the terminal callback again; caller ownership must
establish whether that happens. A terminal record is inspected despite not
being included in the parser's emitted count.

## Key, scroll and trigger controls (B)

For 0xfffe, compare signed R.halfword[6] to unsigned D.halfword[8] promoted
to a word. If unequal, just advance. If equal, set wait state 1, E=R.word[c],
clear D.flags bit 4 and invoke callback(2,handle). Then advance and continue
processing; later admitted records may still run before a loop exit.

For 0xfffd, subtract signed R.halfword[8] from scroll target using word
arithmetic and set E=R.word[c]. Using freshly loaded handle data, if flags lacks
0x200 and signed history count<110, append the new cached-D target unless a
nonzero count's latest history word already equals it. No immediate scroll
branch re-entry occurs; it is tested on the next invocation. Continue records.

For 0xfff5, nonzero payload sets flags 0x80, clears bit 4 and sets E to this
record schedule. Zero payload clears 0x80 only. This implements dkey's input
gate locally; it neither advances E by elapsed time nor resets every flag.

## Paper pose changes and wait-dependent cursor (B)

For 0xfff3, take R.word[0] as NPC pointer. If null, npcNameToPtr(D+0xf204).
Dereference NPC+0x104 as pose ID with no local lookup/null rejection. Call
animPoseSetPaperAnimGroup(pose,"p_b_st",1), animPosePaperPeraOn(pose), then
animPoseSetPaperAnim(pose,paperPoseTable[R.word[c]],1). The six entries at
0x804160d0 are PM_Y_1B, PM_Y_2B, PM_F_1B, PM_F_3B, PM_C_1B, PM_C_2B.
Index is an unchecked word; no local supported-index validation or cleanup.

For 0xfff2, if wait state is nonzero, decrement local I, then take the common
increment and pointer advance. I is unchanged but R advances by 24: do not
model this as a break or retry with an unchanged pointer. If wait state is zero,
resolve null NPC as above, call animPoseSetPaperAnimGroup(pose,0,1), then advance.
Restoration effects and invalid NPC/pose behavior belong to those services.

## Generic callbacks and throttled text sound (B)

First invoke callback(5,handle) if present. Read R.word[0] afterward as flags,
even for 0xfff6/fff7 payload records dispatched here. If bit 0x10 is set,
skip sound for window type 1; otherwise, if the invocation-global latch is zero,
call psndSFXOnVol(0x20044,D.byte[f22c]) and set latch 1. This branch takes
precedence over bit 8.

Otherwise if bit 8 is set, write D.word[f250]=15 unless window type is 3.
Skip sound for window type 1; otherwise apply the same sound ID/volume/latch
rule. No current glyph sound ID is read from D.f228 in this routine. Do not
assume the se opcode immediately plays audio or changes this literal glyph
sound; downstream callbacks/rendering may use that stored word separately.
Window pointer, volume byte and countdown writes in these generic branches use
freshly reloaded handle data, unlike the cached record pointer. Callback-induced
handle relocation therefore cannot be reduced to one uniform D access rule.

The latch resets per msgMain invocation, not per record or globally per rendered
frame. Multiple message calls in one simulation step can each play a sound.
Actual callback scheduling, sound-bank binding and audio voice lifetime remain
unmeasured dependencies.

## Differential obligations and remaining work (P)

Compare scroll-before-wait precedence, two rounded scroll additions, elapsed
quotient low-word schedule arithmetic, latched acceleration, disabled/expanded
trigger masks, wait 1 unchanged timestamp, wait 2 resume in the same record loop,
unsigned schedule admission, terminal outside count/repeated callback, stop
cursor, matching/mismatching signed key ordinals, history deduplication/capacity,
paper-end pointer/index divergence, generic color/noise payload flags and
one-sound-per-invocation admission. These are derived cases, not executed traces.

Remaining: message creation/callback and state producers, timestamp/key snapshot
cadence, original runtime numeric helper details, window/render/selection input,
pose/NPC restoration, font/icon services and sound-bank/event bindings. This
routine's local branches are documented; original/native dialogue execution and
full story attribution/order/conditions remain unfinished.
Window allocation, creation zeroing, Add/Repeat/Continue and identity close
helpers are now specified in [window lifecycle](MESSAGE_WINDOW_LIFECYCLE.md).
Their main callback and wait/input state transitions remain open.
