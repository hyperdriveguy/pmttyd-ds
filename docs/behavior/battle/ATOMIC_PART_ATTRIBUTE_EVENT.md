# Original atomic part-attribute event

This owner implements complete OnPartsAttribute8010e23c/84 with existing complete
BtlUnit_GetPartsPtr80128cd8/40 and registry800f88cc/3c helpers. All three bodies
are pinned read-only against the original US ISO; no executable payload is
embedded or tracked as an asset.

Resolve/translate argument0, resolve argument1 as a signed part ID, then capture
argument2 as a raw mask without a getter. Only afterward read fresh Battle,
lookup the actual unit and traverse its actual part list. Read part+1ac, OR the
captured raw mask and store that same word; even a zero mask performs the
original read/store. This helper does not interpret those attribute bits as
visibility, readiness, actor deletion, victory or reward conditions.

The existing linked-part helper remains authoritative: negative IDs return the
head without reading its next pointer. A single-part head whose next is null
returns for any requested ID, without reading a definition. Multi-part lists
scan actual definition IDs and next links. No visual-slot array, null/cycle
fallback or fabricated part pointer replaces it. Complete reached memory and
part-domain admission remain the caller's obligation.

The source body is atomic under the existing synchronous total EVT/memory/actor
services. No resource or async child can partially acknowledge it. Native Harbor
uses those actual Packed services; missing mapped data is never an executed
zero. Tests check exact source read/getter/translation/registry/store order,
negative/single/multi-part lookup, raw high/zero/all-bit masks, late Battle/mask
captures and source mutations between getters and lookup. Strict host, ASan/
UBSan and source/test/Packed ARM syntax pass; only the existing Packed TU's
baseline indentation warning is demoted. An actual lethal CPU fixture checks
progress beyond this source frontier using coherent15584 headers/core plus
frozen death/StarPoint/return-item overlays. Native GPU/presentation predicates
there are explicitly TESTONLY. Fresh root make test/make all and real native
validation remain required; no complete original end/field return is asserted.
