# Retained original shared font state

`source_font_state.hpp/.cpp` implements fourteen complete US revision0 helper
bodies from8007773c through80077bdc. These are the shared CPU-state parents
needed by submenu matrix text and scalar weapon help, rather than a separate
style initialized for each native label. See the recovered
[font-state behavior](../message/FONT_DRAW_STATE.md) and
[original delegated GX setup](../message/FONT_GLYPH_REQUESTS_AND_SETUP.md).

| Original entry | Complete operation |
| --- | --- |
| 8007773c | Read all three caller scale words, then store all three |
| 80077758 | Store the original binary32 scalar bits three times |
| 8007776c | Return the raw shared color word |
| 80077774 | Reissue current raw color, with effective alpha in a local copy |
| 800777e0 | Read caller alpha/word, store raw color, mutate caller alpha, reread caller word, request TEV color |
| 80077844 | Wrapped table index, raw color store, local effective color |
| 800778b4 | NoiseOff's two original ordered requests |
| 800778e0 | Noise's complete twelve linked requests including actual matrix service |
| 800779e4 | RainbowOff's single original request |
| 80077a08 | Rainbow's six original ordered requests |
| 80077aa8 | Clear edge word |
| 80077ab4 | Set edge word |
| 80077ac0 | Alpha-start's complete source reads/stores, then whole JUT setup |
| 80077b4c | Default-start's distinct source reads/stores, then whole JUT setup |

The two start bodies preserve their different read and store order. Their three
scale words come from actual original802c2b6c storage. Alpha-start narrows its
argument to a byte. Default-start stores255. Both clear edge, restore palette0
and scale, then request whole JUTFont_DrawStart80074bfc using a retained native
local color. This request alone never acknowledges missing font/GX setup.

The color-pointer argument can reference genuine guest memory or an explicitly
borrowed original caller local. A native private local is not assigned a fake
guest address. Its parent/resource lifetime must retain the exact cell through
every suspended child. Guest memory keeps alias behavior: a pointer to the raw
global changes the global again when its alpha byte is stored. Reusing a
mutated caller color scales its alpha again on the next invocation. The indexed
helper uses original32-bit shift/address wrap, with no invented three-entry
bounds check. The implementation exhaustively checks the byte-product/255 rule
over65536 pairs in host tests; this is arithmetic validation, not original
execution evidence.

Noise's texture object address is freshly read from actual8041ea24 plus640 at
the original request boundary. It is not a pointer inferred from atlas pixels.
The scale request retains the original1/64,1/64,1 operands across a held matrix
child. It then passes that child's actual matrix to GXLoadTexMtxImm33,1 and
the exact original texture-coordinate request. Rainbow/noise off operations
do not restore a saved state or invent CPU effect booleans.

Each cursor copies its source/provider identity and pins a genuine external
lifetime. Native invocation guards reject recursive resume/reset on that cursor.
An unavailable child retains its exact request object and captured local values;
CPU prefix writes are not repeated. Service or argument replacement rejects a
held invocation. Lost allocation/context currentness poisons it until explicit
reset. These are native lifetime/admission policies, not original guest fields.
The actual mapped memory services are total over source domains, including
state/constant/caller aliases. Missing source memory must not be represented by
zero-valued successful reads. Guards reject completion after retirement.

GetDrawColor returns its original word. Native completion0 for void helpers is
only a typed completion signal; it is not a claim about unused PPC return
registers. Unsupported functions are not acknowledged and allocate no cursor.

Host tests cover raw/effective color, all byte alpha products, repeated and
aliased caller mutation, private locals, negative palette indexing, scale bit
patterns/overlap, exact alpha-start memory order, start reset, every linked
request wait, held texture/matrix capture, no prefix replay, reentry, replacement,
absent providers and irreversible stale retirement. Source verification pins all
fourteen whole bodies,26 linked-call sites and four original constant/base spans
from the read-only ISO. No original body, BFN, texture, ISO or generated native
pack is committed by this work.

Actual whole JUT setup, mapped shared font storage/initialization history,
loaded font texture/TEV/matrix ownership, genuine native GPU admission and the
whole outer weapon queued callback remain separate required owners. Recording
these requests is not real rendering. This module does not activate original
battle entry, change story/dialogue bytes, fabricate source readiness, or change
physical/touch control availability. No hardware/original pixel parity claim.
