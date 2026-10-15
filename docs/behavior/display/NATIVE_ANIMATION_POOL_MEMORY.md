# Authoritative native animation pool memory

The native typed animation pool now owns the same big-endian record bytes that
an explicitly attached event address space reads and writes. This is a bounded
memory/ownership bridge, not complete original animation startup.

## Original reference and scope

Original US GameCube animInit80042854 allocates64 model records of16 bytes,
64 texture records of12 bytes, and256 pose records of0x170 bytes. W at the
current80415dc0 root supplies array pointers/counts at0/4,8/c,10/14 and the
output selector at e8. See [cold initialization](NATIVE_COLD_ANIMATION_INITIALIZATION.md)
and the original listings/evidence under ignored artifacts/animation-live-cache-evidence/.
Original Entry and release references were checked against their complete source
listings. The original release decrements a word and retires on signed<=0;
its complete fileFree graph is not implemented by this native bridge.

## One record owner

AnimationCacheRecord stores flags0, references4 and opaque file8 as BE bytes;
model records additionally store texture index c. Native immutable resource and
texture-name metadata are separate C++ members, never exposed using native
struct stride or pointer reinterpretation. VM byte/halfword/word writes,
including accesses crossing two adjacent rows, reach the actual cache records.
Pose state remains the actual BattlePose.state0x170 bytes. All256 stable pose
backing objects exist after admission; inactive backing does not assert an
original Entry or allocated resource. Existing pose object identity is retained.
The single W e8 bytes are used by both VM and native outputSelector consumers.
Native copies own independent record/work/pose bytes and omit attachment guards.

Native cache entry/release admits positive signed reference counts, matching
resource metadata and consistent active pose model indices. It holds on zero,
high-bit counts, stale metadata or a changed P10, rather than executing an
unsupported source file/metadata lifecycle. Entry refuses count overflow.
Textures count model owners; models count pose owners. Release preserves stale
file/index source words; a genuinely new native resource clears its unbound
file word to0. Reusing the current resource preserves current source file bytes.
No native pointer or invented nonzero file handle stands in for file ownership.

## Admission and lifetime

AnimationPoolMemory::bind requires the caller's actual existing readable W root,
complete RAM spans and source capacities64/64/256. It rejects null, wrapping,
overlapping spans and spans covering the root cell. Before replacing RAM,
source model/texture/pose bytes must equal the current typed record bytes;
absent inactive native pose backing requires all-zero source pose bytes.
First W ownership transfer accepts previously unknown work fields but requires
unchanged native e8 history. Later transfers require complete W equality.
No default W, capacity shortcut, fake allocation or active-pool reset is supplied.

Admission extracts the four actual RAM spans transactionally. Failed extraction
restores earlier spans and preserves sparse native object identity. Only after
all extractions succeed are missing stable pose objects prepared and individual
rows registered as external RAM. Failed registration removes successful aliases,
restores RAM/work and removes newly prepared backing. The source guard is
published only when the complete mapping succeeds. A second attachment is refused.

matchesWork checks the current root and actual pointer/count layout. Native
resource-dependent pose/cache operations hold while that layout changes. Raw
original machines retain their own captured pointers and fresh-read semantics;
the bridge does not redirect those source operations to a guessed replacement.
An active pose's actual P10 must identify its exact native model resource before
Main/select/evaluate uses the corresponding CPU output workspace.

Detach copies CURRENT authoritative bytes back to RAM; it never rewinds the
pre-attachment snapshot. Reattachment requires current matching state. The
attachment exclusively owns these mappings and must retire before its VM;
external owners must not unregister or replace its rows during its lifetime.
The pool is shared-owned by the bridge. Explicit BattleSequenceBinding attachment
requires the actual Game player/NPC shared pool and precedes sequence entry/init.
There is no automatic acknowledgment of missing source startup.

## Validation and remaining boundary

The host/strict ARM fixture runs the complete source cold parent with explicit
fixture-owned W, heap allocations and null-file branches, then attaches the
actual typed records. It covers same bytes, all256 slots, cross-row writes,
W e8, unsupported signed counts, changed valid/invalid model indices, mismatch
refusal, partial extraction rollback, live root/count guards, copy isolation,
current-byte detach and reattachment. These fixture providers do not represent
normal SDK heap or file/archive initialization.

The integration fixture uses actual original Mario/Goombella/Frankly resources,
the genuine shared player/NPC pool, source door event data and the real battle
sequence binding. Explicit test file/holder/MODEL-header views permit actual
mode2 Refresh and allocation, retaining its subsequent NPC-release wait without
replaying completed buffers. Headers are original0x1b0-byte views, not complete
loaded/relocated type5 models or real file-manager history. Native evaluation
then updates the same source pose cache words. Production supplies no fabricated
nonzero file identities.

Normal source cold bootstrap before Game's existing Mario entries remains open.
Real SDK heap/file/holder/loaded MODEL ownership, live source tangent/output
owners and CPU-vector/guest-scratch aliasing remain required. Normal Game flow
is not switched to this bridge without those providers. Full original renderer,
field/battle parents, authored events and prologue fidelity remain unfinished.
