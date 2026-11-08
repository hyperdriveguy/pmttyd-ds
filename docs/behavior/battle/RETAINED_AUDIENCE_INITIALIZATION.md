# Retained original audience initialization

`battle_audience_init_continuation` owns complete original US
`BattleAudience_Init801a83f8/1c4` and
`BattleAudienceSettingAudience801a60c4/a4c`. It supplies no battle outcome,
invented crowd or resource readiness. See the [original initialization
contract](AUDIENCE_INIT_ORDER_RESOURCES_AND_LIVE_DOMAIN_BOOTSTRAP.md) and
[population contract](AUDIENCE_SETUP4_INITIAL_POPULATION_AND_POUCH_GATE.md)
for source context. The original setup4 fixture is not a Harbor setup2 proof.

## Execution and private ownership

The implementation is a finite C++ translation of these two pinned bodies,
with explicit source program counters. It has no instruction input, binary
payload or arbitrary PowerPC execution service. The source's branches, scans,
private stores, independent global captures and arithmetic sites belong to
this owner. Original instruction addresses identify retained operations and
unavailable operands; they are not elapsed time or a simulated frame count.

The native private frame retains typed words and private offsets, independently
of guest memory. Private pointers never become fabricated guest SP addresses.
Incoming saved registers and unwritten cells remain unknown. Native return
linkage and unused restored registers remain opaque; the module does not claim
original stack placement, FPSCR or GQR equivalence. A value needed for an actual
memory address, branch, floating operand or output must be known. For example,
source level zero indexes an unwritten private word and holds at that reached
operand instead of reading an invented table element or clamping to level one.
The list with length zero still consumes every original shuffle request and
propagates unknown private cells through its swaps; it is not filled with zero.

The source final `memcpy8000519c/50` chooses byte-copy direction by comparing
its private pointer and actual guest destination. The typed linked request
therefore carries a stable private 48-byte view and destination; its second
integer argument has no guest-word meaning. A genuine child must own this
whole operation. A native adapter may explicitly admit an ordinary-RAM,
nonaliasing atomic-copy domain where direction has no observer; this is a
native observational policy, not proof of original SP ordering. It must reject
unknown bytes, mapping changes, MMIO and concurrent/prefix observers. It must
retain any acknowledged copy prefix and must release the borrowed private view
when the child completes. The parent neither invents SP nor substitutes twelve
word stores for the original bytewise copy.

## Exact initialization order

The initializer captures its audience and window through separate fresh source
getters, retains the discarded pouch lookup, clears its actual `13914`-byte
bank and observes Reset immediately after that clear. Setting runs once next;
its retained completion is required before ordinary and guest TPL children,
throw-limit setup, counter resets and the two 100-slot scans. Each existence
check retains its discarded BaseGetPtr lookup and its own ActorGetPtr lookup;
only accepted existence then makes the separate actor lookup/state-byte read.
States 11 and 12 are excluded exactly where the original excludes them.

The right and left sums remain stores into the initializer's captured bank,
while the actor helpers continue making their own current-global captures.
The original double bias is loaded **before** publishing the summed count.
Its effective address is `802fa328`: `lis 8030` plus signed D-form displacement
`a328`, not `8030a328`. The final window store uses its independently captured
window. Noise, volume `(2,60,180)`, counter clear and target adjustment remain
in source order. Reset, prior children, RNG and scans never replay after a hold.

The setting owner retains the captured battle/status owner, independently
captured audience, setup parameters and pouch, all 65 baseline words, rank and
fresh pouch gate reads. It owns every mode branch, threshold/cardinality scan,
24-swap permutation, signed weight selection, paired-species doubling,
source override jump table, thirteen allocations, source weight-sort behavior,
residual pair-fit loop, optional guest substitution and the final uniform or
front/back entry path. It preserves the source sort's use of weight positions
rather than silently sorting weights by the permutation's current values.

The setting's private array layout is the original layout: thresholds at `08`,
allocations at `34`, twelve weights at `68`, permutation at `98`, baselines at
`c8` and seat list at `1cc`. Bounds admission and unknowns describe the typed
native private domain; they do not invent original range checks. Unsupported
escaping private operands remain unavailable.

## Providers and lifetime

Services retain actual actor-memory accessors, complete callback identity,
strong lifetime control block and a current-owner predicate. Recursive
entry/reset and changed providers are rejected. Retirement latches a held
invocation unavailable until explicit metadata reset, with no guest writes or
fake End. A successful child/numeric result advances its source counter once;
missing results retain operands, stable request identity and all prior effects.
Engaged zero remains an original return value.

Pure Base/Window/Actor/Exist/Pouch getters execute their full original reads
inside this module. Other linked requests carry actual function, source site
and only its real arguments. In particular, positive seat/Entry, original TPL,
GuestTPL, throw maximum, noise, volume and target helpers require actual whole
owners, or a genuinely atomic admitted leaf. Synchronous helpers that can
partially mutate before returning unavailable cannot simply be retried.

Inline floating instructions and signed division use distinct optional numeric
requests. Original binary64 bias operands are captured before subtraction and
binary32 rounding; add/multiply/FMA operands retain binary32 bits. Truncation
retains its actual low integer word and leaves the unobserved high FPR word
unknown. Native providers must bind real SDK arithmetic and reject unsupported
division/float domains; no divide-by-zero, NaN or overflow success is supplied.
This is a finite host numeric policy, not original exception-state emulation.

Actor memory accessors are total. Native callers must admit every reached real
source and destination mapping, preserve its topology/ownership through each
synchronous sequence, and hold externally on unknown storage. A matching atlas,
empty actor bank or source resource file is not such admission.

## Evidence and limits

Read-only ISO hashes pin both parents, used pure/linked leaves and source data.
All 772 original instruction addresses in the two parents have explicit cases.
REA CLI attempts are preserved in ignored validation artifacts: the extracted
ELF was unsupported and the DOL Ghidra bridge failed after import. Findings use
ordinary ISO/LLVM static evidence, not successful REA output or observed PPC
execution. No tool/provider installation or configuration changes were made.

Host differential tests compare the existing whole population helper over all
seven modes, eight RNG seeds and supplied counts zero, seven and 77. Cold
entry exercises every linked/numeric callback occurrence twice; positive modes
also hold every distinct reached source site twice. Tests cover all thirteen
nonzero override selectors, rank-capacity preservation, retained partial-child effects,
service/control-block replacement, retirement, reentry, unknown private inputs
and zero-divisor holds. The original Harbor catalog/configuration is processed
through the actual field setup helper separately: the supplied cold-pouch-valued
fixture keeps capacity zero while preserving setup RNG, all population RNG and
300 bound-zero shuffle requests. This does not claim that a current story VM
reached that state or produced a completed native audience resource owner.

Mapless fixture RAM, audio/target/TPL completions, copy policy and positive
atomic child admission are explicitly test providers. Focused tests are source
controller evidence; native cold-start integration, live audio/resources and
playable tutorial verification remain parent work. The matching prebuilt host
core is not sanitizer-instrumented; new owner/test code is.
