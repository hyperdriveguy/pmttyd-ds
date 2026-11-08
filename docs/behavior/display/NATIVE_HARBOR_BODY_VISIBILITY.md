# Native Harbor body visibility

The manual Harbor renderer previously drew every registered actor regardless of live source part attributes. This bounded native projection now reads the actual current unit and primary-body attributes before emitting its flattened original body geometry. It owns no original display queue or shadow callback.

Read-only US ISO instruction spans independently establish the predicates:

- 80102c5c loads unit+104;80102c60 tests mask01000000;80102c64 skips ordinary part registration to80102d24 when set.
- 80102c68 loads part+1ac;80102c6c tests mask01000000;80102c70 takes that same skip when set.
- 80102d30..38 tests part+1ac mask02000000 and skips the later floor/shadow query to80102ecc. No-shadow alone does not suppress ordinary body drawing.

The original body selector80128c98/40 traverses unit+14 and live part+0 links, choosing part+1ac mask1. The native optional reader uses that same body identity, with an explicit maximum of three visited parts for the admitted manual Harbor representation: Mario has three, Crump/Goombella one. Unknown memory, missing body, cycles or a larger unsupported representation return unavailable. They never default visible. This bounded read is a presentation query, not original BtlUnit_GetBodyPartsId completion, and does not return its original part ID.

The facade getter first validates immutable admission-birth proof, then actual runtime and current B address. Its temporary current/lifetime services retain the real VM throughout the reads. No guest pointers, source defaults, copied mutable flags or synthetic part IDs are produced. No Game or actor field layout changes are needed.

The renderer applies the named ordinary-registration predicate only to body geometry. Pose sampling, animation clocks and animation-end acknowledgments continue for hidden bodies. The lower continuation uses the same simulation state and admitted upper geometry ranges. The existing manual renderer has no new original shadow owner; this change does not claim to implement or acknowledge shadow suppression beyond avoiding ordinary geometry when its separate source gate is set.

Primary inspection files are retained under artifacts/harbor-body-visibility/: prefix/consumer/body-selector ELF and LLVM disassembly. These are ordinary static LLVM observations, not REA retained Evidence or original execution. The reverse-engineering skill was consulted; connected REA analysis tools were absent, read-only doctor identified registration state, and the already available repository ISO/LLVM inspection tools provided the exact byte evidence without configuration changes. The source verifier pins all three relevant original instruction spans.

Focused tests cover ordinary/death attributes, independent no-shadow and extra-queue bits, three-part body selection, unavailable memory, retirement during a read and the bounded cyclic-domain rejection. Strict host, ASan/UBSan and ARM syntax checks validate the new reader and the exact facade/renderer journal postimages. Full make test/make all and native post-death visibility remain parent integration gates. No original display callback or full tutorial/return completion is claimed.
