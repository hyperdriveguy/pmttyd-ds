# Shared native animation and menu file owner

NativeFileCache generalizes the existing common-menu backing into ONE retained
process file manager. BattleMenuFileCache delegates its unchanged narrow public
menu API to that owner and exposes files() for shared animation staging. It does
not construct a second manager at the same original addresses.

## Original operations and native boundary

Actual source fileInit creates the1024-entryB0 linked pool. Actual retained
_fileAlloc performs filename comparisons, pending waits, state2/3 reactivation,
S16 reference increments and fresh manager/list publications. Original type4
Set relocates TPL records, complete type5 Set relocates MODEL root/record/child
fields, and type0 Set is a genuine no-op. Actual fileFree changes shared refs/
state while retaining loaded payloads. Read [acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md),
[type5](NATIVE_TYPE5_MODEL_RELOCATION.md) and
[menu ownership](../display/NATIVE_CLASS_MENU_FILE_OWNERSHIP.md).

This remains a serialized staged-ROMFS backend, with empty native archives,
stable backed native address tokens and no original SDK/smart heap placement,
DVD worker, garbage collection, file movement or arbitrary concurrent formatting.
Original mode1/2 still takes its actual empty archive probes before the disc
adapter. File-init allocation returns a real0x2c000-byte vector; per-file holder
storage contains payload/backlink words, not a complete source smart heap object.
Disc reads copy the original file bytes into a32-byte-rounded retained vector.
Native padding remains zero and is not certified as original DVD tail bytes.
The relocation admission below proves actual handler accesses use original bytes.
Missing path/type/domain/providers are unavailable, never fabricated handles.

## Staging and immutable lifetime

stagePrologueAnimationFiles stages12 original files alongside the existing menu:
6models (a_mario,a_mario_r,e_mario,b_mario,c_pkr,c_kuri_h),4textures and2auxiliary
payloads. MODEL/TPL inputs are unchanged draw-model/draw-textures.bin, not PMANIM,
pose-directory or converted GPU data. Type5 paths are a/model; type4 paths use
loaded MODEL textureName plus a/ prefix and trailing hyphen. a_mario/e_mario/
b_mario share a/a_mario-. Aux paths are exactly a/vivian.bin and a/ag2tg.bin,
type0. Common-menu key remains ./battle/common/battle_common.tpl. There is no
case/path canonicalization or invented e_mario-/b_mario- file. Read-only source/
ISO equality evidence: artifacts/animation-file-path-evidence/.

A catalog batch validates and prepares all files before publishing any; failure
leaves existing catalog/addresses untouched. Duplicate keys and oversized/NUL
names are rejected. Current source filename capacity admission is127bytes plus
NUL, before effects. First payload uses87100000 below87200000; holders use
87200000+index*32; later payloads occupy87500000..<88000000. These are explicit
native virtual-address limits, not measured GameCube or3DS heap sizes.

Admission checks writable model root/table/child access extents and excludes
ordinary header/table/child write aliasing; shared identical children are allowed.
Converted but undereferenced MODEL words may contain original wrapped unused
sentinels (observed in e_mario/b_mario); they must NOT be forced into payload
bounds or rewritten. Raw source handlers retain their broader alias domains.
TPL admission checks signature/table/descriptor extents and data-start offsets,
original relocation
flags and conflicting descriptor spans. Count>65535 is refused because the source
low16 loop index cannot terminate against that full-word count. This is native
admission, not a new source loop guard or count clamp.

First successful VM bind or acquisition freezes the catalog. Backing never moves
or reloads in place. Every VM aliases the SAME manager, pool, holders and loaded
payload vectors. A failed bind removes only its successful mappings; earlier VMs
and conflicting RAM remain intact. Owner must outlive all borrowing VMs. Multiple
VM lifetimes do not reset references, lists, archive mode or relocated data.

The native API serializes one retained request. Another path/type cannot replace
its filename/child cursor while held. Pending entry waits require an explicit
yield provider configured before freezing; missing/unready yield holds at the
original Yield child, even after a pending gate is cleared. No default OS yield
success is supplied. Typed staging metadata is not permission for external
mutation/collection/reassignment of live holders or used-chain identity; those
source lifecycle domains need their real providers before admission.

Cache hits still use actual source filename-only lookup and ignore requested
type/archive mode, with S16 refs and state rules intact. The legacy menu facade
retains its prior narrow path/type gate. New-file type must match staged data.
Release preserves payload/holder identity for state2 reacquisition. Unsupported
GC/move/reload domains remain unavailable rather than resetting loaded bytes.

## Evidence and remaining startup

Integration tests use the actual shared owner in two real event VMs, all6MODEL
and4TPL originals, shared Mario texture refs, actual cold parent Vivian copies/
free and ag2tg retained file/holder/payload, filename-only changed-type reuse,
explicit yield holds/resume, freeze/atomic stage and mapping rollback. Fixture W,
heap returns, constants, literal formatting and aligned word-copy providers are
explicit test inputs; cold FILE manager/holders/payloads/ref effects are genuine
production cache services. No header-only/nonzero fake file identity is supplied.

Normal common-menu loading now uses this shared implementation. Animation catalog
staging is explicit and must precede VM bindings. Normal source cold bootstrap,
real source Entry→typed model/file metadata synchronization, SDK heap/tangent/
output owners, formatted-wrapper shared-buffer/thread semantics, acquisition
teardown/movement and complete prologue/rendering/performance fidelity remain
unfinished. Availability of raw files does not certify those owners.
