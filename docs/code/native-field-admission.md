# Native field animation, key and MODEL admission

These helpers connect typed Game state to retained original owners. Their checks
establish the stated native service boundary; they do not certify complete source
NPC drawing or field scheduler execution. See [runtime bridges](runtime-bridges.md)
and [atomic mesh publication](../behavior/display/NATIVE_ATOMIC_MESH_PUBLICATION.md).

## Game animation and party bridges

[Animation initialization](../../source/runtime/game_field_animation.cpp) requires
no existing original pool and no pending producer. Creation, pool configuration
and Mario initialization run in order. A failure is not a transaction that undoes
every earlier mutation. Callers must honor failure and the resulting owner state.

Preparation uses simulation-owned camera-clock milliseconds, motion, speed,
heading and HP. Pending Main/publication work blocks preparation and new selection.
The update binds the actual camera owner and captures a native-unit root: water
Y when active, otherwise floor plus jump height. Only secondary flag bits
0x90000000 come from the presenter's active-handle flags; all other bits survive.
Field Explore/Dialogue outside battle, harbor and professor locks admits Mario
submission. The publication budget counts native visits, not original frames.
Drain resumes the captured request without taking a new input edge or tick.

Room retention requires both Games to have no pending party or animation work,
and the previous animation process must be ready with a readable clock. Every
present candidate pose must match the retained source pose's generation,
resource and bytes. For Mario handles only, candidate material flags at 0xe8 may
be zero while light flags at 0xec equal the live word OR 2. Those eight reinit
bytes are the sole admitted difference. Successful adoption reconstructs candidate
role views over the retained source pool, then reinitializes Mario. Checkpoint
handoff instead moves the live pool, handles and party identities, source clock,
upload serial and NPC owner. It does not recreate the cold VM. Later sampler
holds belong to ordinary publication, after ownership admission has succeeded.

[Party binding](../../source/runtime/game_field_party.cpp) combines weak Entry/
Removal leases and the professor's pending child to serialize mutation. A native
animation owner enables bindings to the actual party/Mario/current-index, pouch,
asset root and appearance services. Resource bank 3 is `c_pkr` (Goombella), after
three Mario banks. A missing bank stays absent. Legacy harnesses keep their
explicitly partial path rather than gaining an invented shared namespace.

## Retained Mario key query

[The key cursor](../../source/field/field_mario_key_continuation.cpp) preserves
source order and completed children without guest writes. `nullopt` means a held
query/read; engaged 0 means a completed rejection. It captures the provider table
and actual registry/actor incarnation pins. Address equality alone cannot protect
against a retired slot being reused. An observed loss of current ownership
poisons that invocation until reset; provider replacement holds without replacing
the original table. A completed result remains cached until reset. Recursive
resume/reset from a callback is refused.

| Order | Admission condition |
| --- | --- |
| Party ID and actual lookup | Registry pin required even for a real null member; nonnull also needs incarnation pin |
| Sequence, Mario byte 0x39 | Original sequence 2 and zero key-depth byte |
| Ship getter twice | First result differs from 1; a separate fresh second result differs from 2 |
| Roll and Slit | Both nonzero; only now may a null party return 1 |
| Nonnull party abilities | Christine differs from 1; Bomhei and Yoshi equal zero |
| Fresh word 0, conditional byte 0x31 | If flag 0x100 is set, kinds 7, 5 and 6 reject |
| Held item | Zero admits |

A hold on byte 0x31 retains the earlier word-0 decision. The synchronous Ctrl
leaf independently returns 1 exactly when byte 0x38 is zero, guarding ownership
before and after the read. It does not return the depth itself. Source function
and callsite tokens distinguish repeated getter calls. See the
[key/control contract](../behavior/overworld/RETAINED_MARIO_KEY_AND_CONTROL_QUERIES.md)
for original evidence and still-unbound production getters/lifetime providers.

## Static MODEL closure

[The file-cache geometry check](../../source/platform/native_file_model_geometry.cpp)
requires a staged type-5 file at the exact guest base and an immutable draw
resource whose original bytes match. A weak identity cache avoids repeating that
immutable comparison; live geometry is checked on every call. Within a synchronous
raw-RAM lease, the VM must alias the actual payload backing. No lease escapes.

Header bytes before 0x14c remain identical. Every root word from 0x14c through
0x1ac must equal guest base plus its original file-relative offset. Selected
count/pointer/stride triples then compare shapes, runs, index streams, colors,
UVs, surfaces, texture slots/descriptors, draws and groups. Eight UV-index streams
use four-byte serialized words. Each 64-byte animation-directory record compares
only its 60-byte name; the last word is a relocated timeline pointer.

Base positions/normals, visibility/node data and timelines/deltas remain separate
dynamic consumers. The check establishes static inputs for trusted resource
assembly, not source draw completion or dynamic output equality. See
[shared file ownership](../behavior/file/NATIVE_SHARED_ANIMATION_FILE_CACHE.md).
