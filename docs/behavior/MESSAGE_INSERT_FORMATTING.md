# Print insertion formatting and append insertion

US G8ME01 revision 0, recovered 2026-10-12. B evidence is original static DOL
instructions and bounded read-only US resource metadata. See [common print](MESSAGE_PRINT_EVENTS.md),
[variants](MESSAGE_PRINT_VARIANTS.md), [parser](MESSAGE_PARSER_RECORDS.md) and
[event runtime](EVENT_RUNTIME.md). No original execution or Switch equivalence
was measured. Original formats, message IDs and dialogue bytes remain reused
assets; no text is paraphrased or replaced with fabricated dialogue.

## Two callbacks and instruction-derived insertion count (B)

evt_msg_print_insert at 0x800d2460,size 0x18c and evt_msg_print_add_insert at
0x800d2250,size 0x174 receive(event,firstCall). Read A=event.argumentPointer
at +0x18 and the word at A-8. Arithmetic-shift that word right16 as signed
32-bit, then subtract5 for print_insert or3 for add_insert to obtain N.
This is the encoded event call's argument-count route, not a count of `%`
characters or decoded message controls. Keep original signed word arithmetic.

| Callback | Fixed resolved script operands | Insertion operands begin |
| --- | --- | --- |
| print_insert | 0 flags,1 key/raw format,2 callback,3 speaker operand | A+16 |
| add_insert | 0 flags,1 key/raw format | A+8 |

Resolve fixed operands in order through evtGetValue. Then for i=0 while signed
i<N, resolve the next insertion operand with evtGetValue and store its word at
stack+8+4*i. This happens on every invocation before the firstCall check,
including resumed polling calls. No local N<=4 guard protects the argument
area, and negative N simply skips this loop. Validity depends on original event
encoding/callers, not automatic clamping by a proposed port.

Both use stack frame size 0xa40. Four prepared argument words occupy offsets
8..0x17. Output starts at stack+0x18; the next saved-register region starts
at0xa18, leaving0xa00 (2560) bytes for ordinary formatted output. More than four
resolved insertion words begin overwriting that output region, and larger counts
can reach saved state. No allocation/resize/count-error fallback occurs locally.

## Format source and local sprintf dispatch (B)

Only on nonzero firstCall, choose format:

- Input flags mask1 set: resolved operand1 is the raw format pointer.
- Mask1 clear: msgSearch(resolved operand1) returns original resource format.

N=1/2/3/4 selects one sprintf call at US0x8026a0b8, passing the chosen format
and exactly that many resolved word values in integer argument registers.
Each call clears the varargs floating-register condition bit. No type descriptor,
format validation or floating argument promotion is supplied by these wrappers.
Do not reinterpret script words as typed C++ format objects, parse angle-bracket
commands first or normalize `%` text before recovering the original library ABI.

The return value/output length is unused. This is sprintf, not a capacity-limited
call. The local output buffer is not zeroed first and no output bound is passed.
N=0, N<0 or N>4 skips every sprintf case; the following first-call consumer still
uses the output buffer. There is no zero-insertion copy of the source format,
no universal safe-empty result and no alternate large-argument formatter here.
Do not declare those cases valid behavior from a guessed fallback.

The original sprintf implementation's conversion/padding/sign/escape behavior,
string pointer interpretation, `%n`, invalid formats, numeric corner cases and
output overruns remain library/caller obligations. Local calls and inspected
resource lexemes establish usage evidence, not a complete printf interpreter.

## New-window insert consumer and lifetime (B)

After the firstCall formatting branch (or directly on resumed calls), print_insert
always invokes _evt_msg_print(event,firstCall,inputFlags OR1,outputBuffer,
resolvedCallback,resolvedSpeaker). Return its result unchanged.

Forced mask1 means the common helper treats the formatted buffer as direct text;
its first call assigns original static DIRECT identity, creates/parses a window,
stores owning event and applies normal speaker/placement handling. The constructor
copies temporary text and parses synchronously before this stack buffer expires.
The identity is not a pointer to this stack output.

On resumed calls this wrapper resolves arguments but does not format, so its
stack output can be uninitialized. Common print's firstCall=0 path polls the
saved window without consuming that passed text pointer or reassigning identity.
Moving formatting into every resumed call would change local library side effects
and failure behavior, even if ordinary displayed text seemed unchanged.

## Append-insert consumer and polling (B)

add_insert formats only on firstCall as above, then calls
msgWindow_Add(outputBuffer,event.word[0x178]) even if N skipped formatting.
There is no second msgSearch or common-helper identity/speaker/placement setup
after this pass. Synchronous Add parses the buffer into existing records.
Identity, callback/owner/speaker and geometry remain governed by prior creation
and append semantics. The wrapper does not retain an output pointer in work.

Every invocation polls windowCheckID(savedIndex): nonzero returns0 (event yield),
zero returns2. No selection result, deletion or allocation-flag completion check.
On resumed calls Add is skipped, while all encoded insertion operands are still
resolved as described above.

## Static US format-like resource census (B)

Schema3 of tools/message_resource_evidence.py scans loader-visible text bytes
with an explicitly lexical percent-directive pattern. Across260 resources and
13,186 pairs,93 pairs contain119 matches:

| Lexeme | Occurrences |
| --- | --- |
| %d | 58 |
| %s | 55 |
| %% | 2 |
| %04d | 2 |
| %3d | 1 |
| % o | 1 |

The last match illustrates why this is not formatting-call evidence: a regex can
classify literal percent prose as a directive. None of these counts proves that
a specific message reaches an insert callback, receives a matching argument
count/type, or is substituted just once. Some `%d` matches occur inside control
arguments (including earlier documented selection placeholders); the formatted
output is passed to the ordinary control parser afterward.

Metadata retains per-pair match counts and aggregate lexemes, offsets/key hex
and text hashes, not original dialogue text. Call-site mapping and generated/
non-US format sources remain open. No parser token is discarded because its
format-like text appears in this census.

## Differential obligations and remaining work (P)

Compare signed encoded N extraction, fixed operand order and all resumed operand
resolution, raw/key format routing, one through four register arguments, no local
zero-count copy, word/string ABI and varargs flags, format-before-control order,
static identity versus transient output, first-call-only library call/Add,
retained append identity and poll completion without deletion. These are derived
obligations, not executed printf/message traces.

Remaining: exact original printf library behavior, insert call-site/format/
argument-type census, other replacement helpers/callback bodies, generated text,
renderer/manager and all original story attribution/order/branches/conditions.
These two local formatting wrappers are documented; full substitution/dialogue
and complete-game equivalence remain unfinished.
Output termination, byte sink, argument slots and local formatter consumers are
now specified in [format runtime](FORMAT_OUTPUT_RUNTIME.md); directive grammar,
numeric/wide conversion and original call-site coverage remain open.
