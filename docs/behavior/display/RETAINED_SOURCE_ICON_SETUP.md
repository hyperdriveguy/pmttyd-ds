# Original IconInit and completion callbacks

`source_icon_setup` retains the complete US `iconInit8001b698/178`,
`_callback_bin8001b810/2c`, and `_callback_tpl8001b83c/40`. The source verifier
pins all three bodies/calls and the actual DVD rounding evidence. It also checks
the existing whole [palette/SDK object graph](RETAINED_PALETTE_TEXTURE_OBJECT.md).

The two work banks allocate1792 bytes each. Each count32 store precedes its
allocation; the count is freshly read afterward for the original count*56 clear.
Neither allocation failure nor an actual child return0 is an unavailable return.
The parent clears the four pointer/ready words, formats the two original paths,
and independently skips either failed DVD open. It retains source unsigned
length rounding `(length+31)&~31`, the userdata store at handle+6c, the fresh
palette pointer read, and the original callback address.

The globals are8041e630 rawTPL,8041e634 rawBIN,8041e638 TPL-ready, and8041e63c
BIN-ready; the last two are flags1, never resource pointers. BIN completion
captures secondArgument+2c, closes that actual handle, then stores BIN-ready1.
TPL completion captures the same handle before its fresh palette read, retains
whole Unpack80037b6c, closes the captured handle, then stores TPL-ready1.
Callback argument1 is ignored. A separate callback cursor permits a real callback
while the parent's async child is held. Init itself never calls a callback.

Each successful memory operation and whole-child return is retained across waits.
Function/input, every provider, userdata and strong lifetime/control block remain
fixed. Recursive entry/reset is rejected. The current predicate is checked around
source providers; observed retirement latches stale. Reset drops native metadata
and cannot undo source effects. Partly completed children must own their own
continuations; this controller does not turn an arbitrary mutating provider into
an atomic child.

The sparse private sprintf destination preserves unknown/unwritten bytes and
its actual identity across both source uses. There is no guest stack address,
empty fake filename, synthetic directory/palette, index-as-object handle, atlas
readiness, SDK/GX acknowledgment, queue completion, or LogoExit claim.

Focused tests compare an independent direct source translation's observable
memory/call order across both open outcomes and count mutation, hold every
reached boundary twice, reject replaced services/lifetimes/input and reentry,
and relocate the actual613-image original TPL. The whole Unpack child has its
separate existing relocation tests. Full native integration is described by the
[native icon owner](NATIVE_SOURCE_ICON_PROCESS.md).
