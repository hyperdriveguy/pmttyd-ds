# Harbor actual unit construction owner

`HarborSourceUnitConstruction` binds the retained
[unit/display constructors](BATTLE_UNIT_ENTRY_CONTINUATION.md) to the actual
Harbor VM and [source map allocator](HARBOR_SOURCE_MAP_POOL.md). It captures the
published BattleWork allocation token and each later unit/part allocation token;
replaced source globals, freed allocations and retired native owners reject
further resumes. One completed source call must be explicitly advanced with
`next()` before the next constructor invocation. This does not free its units.

Registry access, memset, status/gauge helpers, body/pose lookup, pouch import,
vector setters and original root event creation use the real shared VM.
Current rotation uses the exact store-before-clamp source leaf. Source math
comes from the actual native jump numeric services; there is no dummy event
request or fabricated pose handle. Status-effect Init's actual `blr` remains a
complete no-op; it grants no renderer/effect readiness.

Actual nonnull model banks resolve through the immutable
[original CPU resource catalog](../display/HARBOR_TUTORIAL_UNIT_ENTRY_RESOURCES.md)
and the existing NativeFieldAnimation source pool. The requested original model
string address is preserved. When that original-address RAM is absent from the
animation process, a view shares the unchanged exact source string bytes at the
same guest address. Conflicting mappings hold. Actual Entry retains that source
name, mode and resource; existing same-model cache entries preserve their
canonical resource/atlas. A missing animation/catalog/heap provider holds the
actual source Entry call, after accepted constructor writes, without replaying
those writes or using a detached test pose pool.

The adapter retires its still-shared name views before the source VM/map owner.
The caller must preserve that destruction order and keep both VMs' mappings
stable across holds. Actual native animation retains its own file/cache/buffer
continuations. Constructor completion grants neither GPU publication nor a
battle outcome, and does not replace original init script execution.

The original-name bridge adds only the required source-name entry route and
same-address RAM borrowing to the frozen source authority. Its implementation
matches the existing committed native APIs; all users are rebuilt with one
consistent class layout. No older archive may be linked to headers containing
the added BattlePosePool callback field.
