# Fresh shared field and battle CPU poses

Fresh native sessions load original player resources before constructing Game.
FieldPartyRegistry owns one BattlePosePool, injected into FieldPosePool and
TrioAttackRuntime. Actual global handles therefore include Mario, partner,
registered NPC and native battle poses. This is a native ownership composition,
not completion of original animInit or the original global W/file/heap graph.
Existing legacy fixtures retain their independent path. No live namespace is
migrated or silently replaced; configureOriginalPool rejects a second pool.

Original resource tokens802c18a0/802c18a8/802c18b4/804213e0 resolve to
`a_mario`, `a_mario_r`, `e_mario`, `c_pkr`. Original descriptor0 is respectively
`M_Z_1`, `M_Z_1R`, **`M_S_5`**, `PKR_Z_1`. Entry uses these original descriptors,
not the old visible workspace seeds. Shared entry requires mode2 and original
resources; it does not require a legacy animation bank. Field NPCs retain mode0.
Original source/default inspection is recorded privately under
artifacts/shared-animation-entry-evidence/; original MODEL/TPL extraction is
read-only and provenance remains in the generated metadata.

FieldPartyPose aliases live flags0, materialE8, lightEC, timestamp18/1c and CPU
shape, with immutable animation resources shared by identity. VM external views
can alias the same original big-endian record. Timestamp writes update both
words; material reset acts on those bytes. This is not a claim that CPU vectors
alias all original guest scratch/output ranges. See the
[refresh contract](NATIVE_ANIMATION_REFRESH_AND_SCRATCH_RECOVERY.md).

Battle runtime retirement tracks only its own handle plus native generation.
A later allocation reusing that handle survives destruction. This is native
facade lifetime management, not the complete original BattleEnd graph. The
shared partner resource is canonical; existing field Frankly resources remain
canonical when the model cache already owns them. Matching GPU packages are
selected by resource identity, preserving each converter's atlas layout.

Registry copies clone the entire pose namespace once and retarget role wrappers;
immutable resources remain shared. A Game copy does not reconstruct an existing
NPC owner's external VM graph: the battle binding holds mismatched ownership.
Game::restore retains a live shared namespace and original NPC owner, then applies
existing Mario material reset. Callers must retire VM facades first. This does
not reconstruct checkpoint history or substitute original room/event retirement.
Party wrappers must not outlive manual release of their own pose handles.

Player conversion retains complete original MODEL/TPL and descriptor metadata,
with a bounded set of CPU clips plus actual descriptor0. Unconverted descriptors
remain unavailable. GPU admission for matching Goombella/Frankly packs follows
the existing battle texture lifetime. Ordinary field drawing still uses its
existing original animation presenter. Full clip census, original cold startup,
W/file/scratch/output ownership, field facing/Main/render/collision and authored
battle entry remain open. Full playable original prologue is not established.
