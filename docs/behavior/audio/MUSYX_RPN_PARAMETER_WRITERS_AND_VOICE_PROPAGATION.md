# MusyX RPN parameter writers and voice propagation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete inpSetRPNHi8028dff0/1e0,
inpSetRPNDec8028e1d0/224 and inpSetRPNInc8028e3f4/224hex. Addresses/offsets/
masks hexadecimal; scalar values decimal. Static selector/arithmetic/memory
order, not original MIDI/controller/audio observations. Read
[MIDI setters/call order](MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md),
[default initialization](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
and [default getter](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md).

## Raw inputs, selector order and unchecked table addresses

Inputs Hi=(RAW S,RAW C,RAW V); Dec/Inc=(RAW S,RAW C). Narrow set/channel to
s=S.low8,c=C.low8. Every body uses MIDI row80409748+s*860+c*86, EVEN s255;
no FX row substitution, channel255/set255 rejection, range/active/IRQ/pointer
check or explicit success result. The parent setter rejects channel255, but
direct callers need their own argument domains.

Selector=byte100|(byte101<<8), with full bytes and no7-bit mask. Hi reads100
then101; Dec/Inc read101 then100. Not an atomic pair snapshot. Exact recognized
selectors:0→packed defaults.byte0,7f7d→word1,7f7e→word5. All others return
without any default/voice writes. Default address D=8040da48+s*90+c*9.
Packed word1/word5 offsets remain unaligned; do not pad the nine-byte record.
These source formulas do not establish valid original table extents/domains.

## Selector0 byte operations and two-field publication

Hi takes U8(RAW V), clamps that unsigned byte to24, stores D.byte0. Thus RAW
256 narrows to0 before clamping. Dec reads old D.byte0: nonzero decrements1,
zero remains0. Inc reads old byte: if old<24 increments1, otherwise leaves old
unchanged. Inc does not repair an old25/255 to24. Each body ALWAYS stores its
result byte, including unchanged boundary cases.

Then scan ascending voice ordinals under freshly read synthInfo803f8540.byte210
before EVERY iteration, stride458. Reload synthVoice8041ee90 for each ordinal,
match voice.byte122==s then voice.byte121==c. For a match:

1. Store retained result byte at voice.byte1db using the matching captured pool.
2. Reload synthVoice AGAIN and store it at same ordinal.byte1da.

Both fields receive the same byte in this order, but pool reload can separate
writes under unsupported mutation. No macro-active/identity/generation/protection/
FX-kind gate,64-slot cap, callback/job request or field-pair atomicity. Stored
byte result stays captured while later count/pool/voice fields are read fresh.

## High-byte packed word replacement

For selector7f7d/7f7e, Hi reads D.word1/5 and computes:

`newWord = (oldWord & 000001ff) | (U8(RAW V) << 9)`

The source keeps low9 old bits and inserts EIGHT input bits9..16. It does not
clamp V to127, preserve high word bits or use the MIDI low-byte setter's fe00
mask. Values128..255 can set bit16. Store new word to packed default first.
Then scan ascending fresh count/current pool, matching the same set/channel;
store RETAINED new word at each matching voice.word1e0 (word1 route) or1e4
(word5 route). No repeated default-word load per voice in Hi propagation.

## Exact whole-word decrement/increment guards

Dec/Inc read full unsigned D.word1/5 using an update-form load to the packed
address. Dec first checks exact0: zero skips the DEFAULT store; otherwise
reread that word, subtract1 wrap32 and store. Inc first checks exact16383:
equality skips the DEFAULT store; otherwise reread, add1 wrap32 and store.
This is equality guarding, not a general valid14-bit range clamp:

- Inc old16383 stays16383; old16384 becomes16385.
- Inc oldffffffff becomes0; Dec old0 stays0.
- Dec old16384 becomes16383, with no packed high/low field reconstruction.

Both bodies STILL run the voice propagation scan when the initial boundary
skips the default store. For each matching voice, reload DEFAULT WORD FRESH
and store it to voice.word1e0/1e4. They do not retain one result word for all
voices. Default mutation/aliasing between matches can therefore publish
different values. Loop count and synthVoice also remain fresh. Whole-word
arithmetic can carry/borrow through packed components; source does not modify
only one extracted component or normalize unexpected stored values.

## Parent ordering and limits of these bodies

None of these three bodies calls another function, writes controller bytes,
word220, dirty flags, hardware or jobs, or clears an identity/macro/voice pool.
The [complete MIDI setter](MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md)
requests Hi for controller6, Dec for96, Inc for97, ignores return, forces
changed marker1, THEN publishes the7-bit controller byte and performs its
matching key-state jobs/dirty writes. Thus parent notification also occurs for
unrecognized selector/no matching voice/unchanged guarded default. Separate
calls and scans are not one atomic controller/default/voice transaction.

The parent FX branch passes raw set255 into these same MIDI/default formulas.
That source behavior is retained here; runtime reachability/address ownership
cannot be established by replacing them with inferred FX defaults or bounds.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_rpn_parameter_writers_evidence.json) pins three
complete spans,219 anchors/zero child calls. Six high-mask/seven byte/six whole-word/
six selector vectors check arithmetic/route deductions, not original execution.
Full fresh inspection ELFs/listings stay `/tmp`; no original bodies/binary assets
committed. Source identity does not prove valid tables/caller/alias ownership.

Remaining: actual selector/controller/default/voice/count/caller writers and
consumers, valid set/channel/packed-value/address/matching/alias/reinit/IRQ/
concurrency domains, other macro opcodes and original RPN/default/voice/controller/
audio traces. EVT-01 remains OPEN.
