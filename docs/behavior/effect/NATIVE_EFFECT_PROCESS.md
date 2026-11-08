# Native original effect process

`NativeEffectProcess` owns a retained facade over the actual `NativeUiStartup`
and field animation process, original SDK heaps and shared `NativeFileCache`.
`Game` calls original effInit immediately before Mario initialization, matching
US revision0 calls at8006fdf4 and8006fdf8. Room/checkpoint transfers retain the
same facade and refuse transfer while source operations are pending.

## Storage and lifetime

Cold attachment admits seven absent, byte-pinned ranges from
`assets/original/effects/process/memory.json`: format802c1f20/24,
manager pointer80415ee0/4, language table80415ee4/28, state pointer80419a40/4,
manager BSS8041e908/24, state BSS8041ecd0/8 and language strings80420180/28.
The existing UI root8041f494/4 is borrowed. A nonexecuting alias facade checks
all eight actual backing registrations; it is not an execution VM. Backing must
remain stable. Same-address/same-backing unobserved ABA remains a caller lifetime
contract. Successful attachment persists in the real VM after facade retirement;
recreation refuses occupied cold ranges rather than resetting history.

Initialization resumes the complete original effInit once: heap0 allocates512
records of40 bytes, source clear initializes20480 bytes and original manager,
selector and state64 writes execute. `poolReady()` certifies this bank only.
Primary effTexSetup and its manager+8/+c texture fields remain separate owners.
No null-allocation recovery or synthetic source heap is supplied.

## Source visits and file transfers

`main(owner, visit, update)` represents one source driver invocation. The owner
must be stationary and visit IDs strictly increase from nonzero. Held retries
and completed duplicate visits do not replay callback or cache side effects.
The source driver selects live context and records; reached nonnull callbacks
hold until a retained, current provider executes the complete original callback.
Empty records need no fake updater. Game does not yet schedule these visits.

The original language filename uses actual root, prefix table, format and live
GP selector. The native three-string formatter has a1024-byte including-NUL
admission capacity; this is native policy, not a measured original printf limit.
It publishes the actual shared filename at803e6e78 before requesting the real
shared cache, and retains the completed name across holds. Unsupported languages
hold on their original filenames; there is no forced US fallback. Filename
comparison preserves all unsigned bytes. Raw resource type4 is independent of
archive mode; source archive-mode history is preserved. `pumpFiles` exposes
cooperative native byte budgeting rather than claiming original DVD timing.

The immutable language payload `e/us/effect_us.tpl` is106880 bytes and differs
from primary `e/us/effect.tpl`. The normal asset pipeline stages it before cache
binding freezes the catalog. Original bytes remain editable through the checked-in
asset workflow; imports refuse nonempty destinations. Per-span/path provenance
and retained original rights accompany the assets.

## Release and remaining scene integration

`autoRelease(exactContext, retirement)` requires retained proof that constructors,
callbacks and queued display consumers have drained. It resumes original
AutoRelease, real heap3 free and flags clearing only. It does not cancel actors,
events or display queues. Release and Main cannot interleave while held.
`retire()` stops new admission without resetting/freeing source RAM.

The [Crump effect contract](CRUMP_FUKIDASHI_EFFECTS.md) and complete retained
fukidashi event wrapper are available. Connecting actual reaction EVT callers,
primary texture initialization, callback Main32, camera/actor aliases, original
render/audio queues and their retirement remains production work. Normal startup
bank admission is not a completed Crump field/battle/result/field-return scene.

## Validation

The actual Game startup/room/checkpoint fixture checks retained facade identity.
Native process tests cover once-only initialization, shared source allocations,
real cache relocation/refcounts, callback holds/live records, exact-context
release, alias replacement rejection, unsupported-language filename retention,
archive-mode preservation and unsigned filename bytes. Fresh read-only ISO import
roundtrip and effect driver verification cover14 bodies,186 instruction anchors,
eight data spans and four direct calls. Private evidence is under
`artifacts/effect-process/`; emulator boot and host tests do not establish original
pixel parity or physical New2DS XL performance.

The next concrete owner is whole-source effTexSetup8005ee0c/0x114 and its
completion callback8005efa8/0x48, staging primary `e/us/effect.tpl`266784 bytes
and setting actual manager+8/+c through original archive/DVD/unpack calls. See
[the existing texture contract](FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md);
these identities are documented and need implementation rather than a duplicate
request in the documentation inbox.
