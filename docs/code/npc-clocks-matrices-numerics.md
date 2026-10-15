# NPC clock, matrix and inline numeric helpers

These retained helpers implement particular original NPC subgraphs. Their
completion does not imply a full NPC Main traversal, a rendered NPC, or normal
Game activation. [Clock behavior](../behavior/field/NATIVE_PACKED_NPC_CLOCK_SECTION.md),
[matrix behavior](../behavior/display/RETAINED_NPC_DRAW_MATRIX.md),
[blur callbacks](../behavior/display/RETAINED_NPC_OFFSCREEN_AND_BLUR_CALLBACKS.md)
and [automatic pose numerics](../behavior/field/NATIVE_NPC_MAIN_AUTOMATIC_POSE_SECTION.md)
contain source evidence. Constants here are recovered values or explicitly named
native adapters; they are not measured handheld/original timing.

The cursor conventions differ and matter to callers:

| Helper | Identity during a hold | Successful completion |
| --- | --- | --- |
| [Clock](../../source/field/field_npc_clock.cpp) | Rejects changed actor or service identity | Resets cursor; another call starts a new clock visit |
| [Matrix](../../source/field/field_npc_matrix.cpp) | Uses captured actor, raw slot and services; later arguments cannot replace them | Publishes matrix then resets |
| [Blur trig](../../source/field/field_npc_blur_trig.cpp) | Uses captured yaw, mode and word provider | Publishes scalar then resets |
| [Inline length](../../source/field/field_npc_inline_norm.cpp) | Rejects changed input bits or relevant providers | Retains terminal phase; repeat calls have no provider effects |

A false/empty provider result must have no effects. Earlier acknowledged reads,
stores and requests remain captured, so callers retain mappings, producer pass,
owner and actor lifetime until completion. Resetting a held cursor can replay
already completed stores. Room retirement must discard it before actor reuse.
The phase numbers are primitive continuations, not elapsed simulation ticks.

## Packed clock and actual VM binding

[Public clock services](../../include/field/field_npc_clock.hpp) separate guest
word access, signed division and signed-integer conversion. The
[binding](../../source/field/field_npc_clock_binding.cpp) and
[binding API](../../include/field/field_npc_clock_binding.hpp) require the exact
initialized registry owner, registry facade and VM memory. Only owned ordinary
actor slots are admitted. It performs no actor copying or Main gate traversal;
another facade sees completed partial stores directly.

The live GP pointer is read at 0x80415fd0, and GP+0x38/0x3c supplies the high/low
timestamp. The platform callback serves each source read of bus-clock word
0x800000f8; other words come from guest memory. It does not cache a bus value or
substitute the host/3DS frequency.

| Actor bytes | Role |
| --- | --- |
| 0x178/0x17c | Conditional accumulated high/low time |
| 0x180 | Conditional accumulation in float seconds |
| 0x188/0x18c | Elapsed high/low time |
| 0x190/0x194 | Delta high/low time |
| 0x198/0x19c | Anchor high/low time |
| 0x1a0/0x1a4/0x1a8 | Elapsed/delta/anchor float seconds |
| 0x1d4 mask0x10 | Clock-enabled state |
| Main word mask0x20000 | Conditional accumulator enabled |

With state mask0x10 clear, only anchor refreshes; stale delta and float outputs
remain observable. Enabled time subtracts wrapped 64-bit timestamps, with source
low/high lane ordering retained. Signed millisecond division uses
`high32(0x10624dd3*(bus>>2))>>6`. Only a positive quotient greater than500 caps
delta to low32(divisor<<4), high0: sixteen captured divisor units with
low-word wrapping. Exactly500 and negative values
do not cap. The conditional accumulator adds or zeros, elapsed always adds,
then a fresh GP timestamp refreshes the anchor. Stores publish low before high;
a hold between lanes does not undo the first store.

Four output conversions each reload bus/source and the original float1000000
literal. Source high/low bits shift left3 with 64-bit wrapping, divide signed by
`high32(0x431cde83*(bus>>2))>>15`, convert signed64 to double then float, divide
to seconds, and publish. Direct host duration arithmetic changes truncation and
rounding. The volatile double intermediate is deliberate: direct integer-to-float
conversion differs near halfway cases. Numeric services never invent a result
for unavailable data or a zero divisor.

## Source draw matrix

The [matrix API](../../include/field/field_npc_matrix.hpp) accepts a guest actor
and raw signed history slot. Actor layout uses root0x8c/0x90/0x94,
scale0xe0/0xe4/0xe8, X/Y/Z rotation0xec/0xf0/0xf4 and pivot0xf8/0xfc/0x100.
Slot -1 selects the root. Every other slot uses wrapping 32-bit
`actor+0xa4+12*slot`, with no added clamp; the word provider owns mapped admission.
Missing memory holds, rather than silently selecting the current root.

Flags0x02000000 or0x08000000 bypass camera correction. Otherwise a current-camera
query and two angle calls compare camera look direction with direction to the
actor; the second angle reloads camera X/Z. The revised difference is retained.
Additive yaw is separately revised twice; inclusive90..270 reflects Z scale.
Original zero/one/angle/reflection literals remain guest reads.

Transform order is identity, optional scale, negative pivot, Z rotation, X
rotation, Y plus camera correction, positive pivot, then root/history translation.
Each rotation left-concatenates the retained output. SDK axis tokens120/121/122
are ASCII x/y/z. Source radians-per-degree bits0x3c8efa35 stay explicit; exact
pi/180 is a different numeric input.

Short-circuit scale checks retain the first X, then reload Z/Y if scale applies.
Each pivot side performs separate short-circuit checks and fresh Y/Z reads.
Y rotation reloads yaw after earlier SDK calls, adds correction before loading
the radians factor, and skips only if both operands individually equal zero.
Providers may mutate bytes, so replacing these reads with a transform snapshot
changes behavior. NaN follows source comparisons into nonidentity/nonzero paths.

Numeric and SDK request payloads stay stable across holds. Function addresses
identify camera/angle/revise and identity/scale/translate/rotate/concat operations;
they are not executable host pointers. Matrix payloads are bounded native values.
Only final translation publishes the caller's result; a partial visit leaves it
unchanged. Native SDK adapters do not establish measured Gekko numeric parity.

## Blur polynomials and scalar length

[Blur API](../../include/field/field_npc_blur_trig.hpp) evaluates one original
sine or cosine graph. The callbacks invoke separate graphs with fresh camera and
literal reads. It forms tau*yaw before loading pi and the polynomial coefficients,
then divides by360. The source performs one quadrant reduction, not general
periodic normalization. The final quadrant subtracts threeHalfPi before reloading
halfPi. The nested binary32 FMAs precede sine's extra multiply and the final sign.
No finite-only gate or replacement libm call changes comparison outcomes.

[Length API](../../include/field/field_npc_inline_norm.hpp) consumes the caller's
already computed squared norm; it does not normalize a vector or rebuild the
caller-specific square/sum graph. Positive input, including positive infinity,
requests a reciprocal-square-root seed, then reads original double half/three.
Three Newton refinements preserve double multiply rounding and negated fused
subtraction. Final squared*seed rounds to double then float. The opt-in native
seed uses 1/sqrt(double); it is explicitly different from Gekko frsqrte.

The other branch reads original double zero before classification. Negative or
NaN input loads the canonical guest NaN word0x7fffffff; IEEE exponent/fraction
masks identify NaNs. Other nonpositive classes retain input, including signed
zero. No epsilon or finite fallback is added. Input bit checks preserve signed
zero and NaN-payload distinctions across retries. The terminal cursor stays active;
callers explicitly reset it for another sample. FPSCR, exception, subnormal and
original estimate parity remain outside the native numeric adapter claim.
