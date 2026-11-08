# Native Inner, group and raw renderer owners

[Native Inner](../../source/display/animation_native_inner.cpp) binds the source
[Inner parent](animation-parent-ownership.md) to one current Draw lease.
[Group binding](../../source/display/animation_group_binding.cpp) supplies the
[group parent](animation-group-traversal.md), and
[render binding](../../source/display/animation_render_binding.cpp) guards the
[raw renderer](../../source/display/animation_source_render.cpp).
Field offsets and guest addresses below are hexadecimal; sizes are decimal.

## Provider selection persists through holds

Inner accepts only its actual source VM and current producer ownership. Optional
owned groups are exclusive once selected; their false result never falls through
to external traversal. Group's explicit whole-node and whole-render callbacks
are likewise exclusive. Only absence selects the owned CPU node or raw renderer.
Material, Vivian, dynamic callbacks, GPU and cache effects need real providers.
Missing providers keep the reached operation pending.

The backend structs are copied, but their user contexts remain borrowed. Inner
may pump late Main only for its enclosing Draw root handle and lease pass.
Backend replacement waits for all producers, Entry and Release to quiesce.
Camera/group/SDK owners refresh before a new Inner operation when camera-bank
or Main-configuration ownership changes; held work retains its local identities.
The camera token reservation is 13 slots at0x81910000 with stride0x280, shared
by the facing/render adapters and required to stay unmapped.

Inner's matrix Copy starts from the copied caller matrix and retains all48
input bytes. It stores six forward8-byte pairs. Group Copy instead reads each
VM source pair immediately before its matching destination pair. Earlier stores
can therefore affect later source values under overlap. Neither is an eager
VM-to-VM memmove. A held pair retains captured bits; changing the old source
cannot replace them. Complete raw backing admits both lanes together. These
finite-domain GQR0 adapters do not establish general paired-single parity.
Group's blend explicitly rounds source-saved then applies FMA(factor,delta,saved).

## Raw renderer admission follows earlier matrix effects

Only shape=-1 exits before providers. Other inputs first query the current
camera, capture modeling, Concat view/model, upload position matrix, request
InvXpose, upload normal and select matrix0. InvXpose status is ignored, so an
absent/stale normal slot must not become an identity substitute.

| Loaded source field | Meaning |
| --- | --- |
| W+20 → D | Current MODEL |
| D+14c → H | Shape table,168-byte rows |
| H+98 / H+9c | First draw index / signed draw count |
| D+19c → B | Draw table,108-byte rows |
| H+40/48/50 | Position/normal/color base indices,expanded by12/12/4 |
| H+58..90 | Eight UV bases,expanded by8 through D+18c |

Admission uses shape mode0 with effective pass2, exact mode/pass equality, or
nonopaque pose alpha bytes f3/f7. Mode0 can read effective pass twice. The cull
selector indexes the actual source table without a source clamp. UV bases are
captured in order58,60,68,70,80,78,88,90, rather than sorted by address. Even a
nonpositive draw count reaches this setup before skipping per-draw work.

Each draw captures default slots before the pose+e4 guard. A zero guard admits
seed/MultVec, Light, Texture, LastStageBlend, fresh camera4/current equality for
Fog, and TEV. A nonzero guard skips defaults, not later overrides/arrays. Colors
remain raw secondary then primary captures; global pose pointers reload at the
source usepoints. Child requests preserve tagged local matrix/vector/color IDs
and ordered arguments without fabricated guest stack RAM.

## Texture matching and rebind preserve priority

The entire P+4 word admits both primary scans. Kind1 has priority over kind2.
Each scan walks reverse draw slot IDs;8-byte base texture records plus unsigned
frame-extension bytes from24-byte state rows select64-byte descriptors. Signed
out-of-range indices request the actual printf child with the captured FRAME
byte, then descriptor0. Slot dereferences precede this fallback.

The first matching kind ends the scan even with a null override object. Primary
smartTexObj returns the pointer passed to LoadTexObj; the extra-object request
instead discards that return and loads its captured object. Texture matrix IDs
start30 and advance3 per coordinate;60 is identity and125 post-identity.
Extra work can replace GX state and hardcodes two TEV stages. Reached matrices
have no added null guard, and no state restore follows. Independent pose flag10
admits a new kind1 rebind even when P+4 skipped both primary scans.

The [source material contracts](../behavior/display/ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md)
and [last-stage contract](../behavior/display/ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md)
define these exact requests. A matching flag means matching texture kind, not
successful GPU binding.

## Vertex calls emit indices one effect at a time

Current MODEL/run metadata mixes with captured shape array bases at its specified
reads. W+e8 chooses current shared W+f8/fc or current pose P+4c/54 outputs.
Run records are8 bytes, while draws advance108. The descriptor/format/array calls
configure attributes9/10/11 as position/normal/color0, then13+ as texture
coordinates. Descriptor3 is index16. Types4/5 mean float32/RGBA8; row strides
are12/12/4 and UV8 bytes.

Signed UV selectors use reverse order except the one-slot route. The durable
native UV table has eight entries; an escaped captured selector holds before
SetArray. This is an implementation frontier, not an original bounds branch.
Repairing the old byte cannot undo its captured value.

GXBegin uses primitive0xa0 (triangle fan),format0 and count truncated to LOW16.
Zero still calls Begin. One-UV streams are fixed before Begin; each vertex emits
position,normal,color,UV low-half indices from big-endian4-byte slots. Multi-UV
instead captures W after Begin, even at zero vertices, then reloads D through
that retained W per coordinate. Four-coordinate bulk lanes and remainders retain
different D/selector read order. UV word loads contribute only LOW16.

Each input read alternates with one retained halfword effect at FIFO0xcc008000.
A held FIFO write does not reread input or replay prior indices. The parent
adds no GXEnd, stack pop, state restoration or lease retirement. Fixed local
storage survives holds without per-vertex allocation. See
[arrays/FIFO](../behavior/display/ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md) and
[raw renderer](../behavior/display/NATIVE_ACTUAL_VM_RENDER_PARENT.md).
Completed local control is not GPU, normal-game scheduling, original numeric,
matched-image or hardware-performance proof.
