# Native process UI startup: original font prefix

`NativeUiStartup` owns one cold `NativeFieldAnimation` process's original US
language contribution and whole `FontManager`/font setup chain. Its explicit
`resumeFont()` completion requires actual source heap allocation, real native
file delivery, original BFN mutations and both original GX texture-object
constructors. It does not acknowledge `seq_logoExit`, initialize WindowReady,
submit a source display callback, or authorize a native font GPU lease.

## Source and port entry

Original US revision0 instructions at `8006fd0c..8006fd18` load GP through
`80415fd0`, put `1` in r4 and store that word at GP+`16c`. The intervening
`li r0,0` has no memory effect in this contribution. The exporter verifies all
16 instruction bytes against `806d90b038800001380000009083016c`. The native
prefix executes this contribution once before calling retained original
`FontManager` (`80077bdc`), which is the first child of `seq_logoExit`
(`8008746c`). This is an explicit port startup ordering; neither whole
`marioStInit` nor later logo fade/icon/environment/effect children are claimed.

Cold admission requires no held Entry/Release and no allocated/generation-bearing
pose row. Cold source binding already constructs 256 empty row objects, so
pointer presence is not an allocation test. Once font setup completes, ordinary
Mario Entry, pose reinitialization, source Release and time publication may
continue. They do not cause font setup or language initialization to rerun.
`language()` reads actual current GP+16c, including later real writes.

The UNAPPLIED integration journal adds a separate
`Game::initializeNativeUiFieldAnimation`, called explicitly by normal native
main. It performs this prefix between process creation and the existing
configure/Mario Entry order. The animation-only method remains a distinct
legacy/mechanics entry. Normal UI startup has no fallback to that method.

## Original cold memory and native DVD delivery

`tools/source_ui_startup_assets.py` imports ten exact initialized-data/BSS spans
into ignored `romfs/source-ui-startup/memory.pmevt` and `memory.json`.
Initialized DOL sections take precedence over the source loader BSS interval.
The pinned pack SHA-256 is
`7577bb75184fd6e6fe81d5e291fccd76919ef25b62a454b21523232a5053a9fe`.
Mapping requires every span to be unoccupied; it never imports over restored
history. A failed factory rolls back its own new mappings. Successful process
startup retains them, preventing another cold import into the same process.

The actual revision0 FST has only `f/papermarioset_{EU,JPN,US}.bfn`, and no
`font/` entries. Only the three exact original `./font/` probes are admitted
catalog misses. Unknown paths remain unavailable. Actual selected
`./f/papermarioset_US.bfn` opens the original raw file staged at
`romfs/source-font/papermarioset_US.bfn` (66560 bytes, ISO extent948050384,
SHA-256 `bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45`).
Factory validation reads the immutable raw pack as an asset preflight; it
delivers no source bytes and is not a source DVD request. The separate delivery
`FILE*`, backed native DVD entry and delivery `fread` are admitted only by the
corresponding actual source requests. Delivery checks all observed bytes against the
immutable original before modifying source RAM. Entry fields and actual close
are coherently owned by this finite port adapter; this is not a complete
original DVD scheduler or disc-hardware implementation.

## Actual heap and aliases

Heap0 allocation runs through the existing source `eventHeapAllocate` over
this very process VM. Its real memset/flush/list/header writes execute once.
A held transfer retains the returned source allocation. Only under a quiescent
scope may its actual known RAM payload detach and become strongly backed shared
font storage. Source heap headers remain at their original guest addresses.
Failed alias adoption restores captured RAM and retries without allocating again.

The separate UNAPPLIED `NativeSourceFontMemory` extension adds
`adoptHeapAllocation` and `deliverHeapRead`. The former preserves a captured
known prefix, including actual allocator writes; unread capacity remains owned
but unavailable. The latter admits one actual later DVD overwrite only after
all bound VM aliases preflight actual backing identity under a raw-RAM borrow.
A byte-identical external replacement is insufficient; no host pointer escapes
the approved pure `RawRamAccess::matchesBacking` query. It changes neither original unknown adoption
nor `installRead`. Raw-borrow/mapping conflicts hold before any overwrite.
Whole original setup then writes glyph0 width7 and9 and runs the genuine
InitTexObj/LOD children. Missing actual truncate remains pending at `802b7580`.
Native main supplies toward-zero conversion only for the SDK owner's already
validated finite signed32-bit domain.

## Lifetime and integration limits

The startup lifetime pins the real field process, VM, source heap backing, file
records and texture binding. `fontMemory()`/`fontObject()` remain unavailable
until the entire source font prefix completes. `retire()` rejects further
admission. Same-process room/checkpoint retention must transfer this same
owner; detached candidates expose no owner until they reconnect to that exact
process. A genuine new process requires its own cold initialization. No old
heap is re-labelled as a new process allocation.

Future Trio binding must alias the actual startup GP language lanes and bind
this actual shared font memory into its VM; copying language1 or selected BSS
values is insufficient. Window asynchronous delivery/callback, whole LogoExit,
source GX global setup, native font residency, and normal queued weapon
presentation remain separate requirements. Font CPU completion alone proves
none of them.

## Validation

Focused strict host tests use actual original private packs and the accepted
root archive plus explicit frozen formatter/approved memory overlays. They
execute whole source setup, missing truncate/resume, allocation and real-read
holds without replay, failed alias transfer/rollback, cross-VM mutable aliases,
unknown tails, actual Mario's three Entries/tick/reinit/source Releases,
language persistence, retirement and a distinct fresh process. Owned-code
ASan/UBSan passes; the already validated root archive is not instrumented in
that focused run. Strict ARM syntax covers the new owner, memory postimages
and exact Game/restore/main integration. Python tests verify original source
pins, BSS origin, true catalog absence and preservation of modified outputs.

A separate `native_ui_startup_game_tests` target is prepared for the fresh
integrated build (not linked against the old root archive). It exercises the
new Game entry, detached candidate rejection, room retention, restore and
checkpoint ownership transfer with actual language history.

Normal native checkpoint validation must use a fresh consistent integrated
Game ABI/build and show completed font/GP/source owner before Mario. The prior
isolated Window/font raster probe used explicit startup fixtures and is not
evidence for this newly executable production startup route.

Original Nintendo/Intelligent Systems rights are retained. Extraction and
tracking grant no redistribution license. ISO/raw payloads/generated packs
remain outside source history.
