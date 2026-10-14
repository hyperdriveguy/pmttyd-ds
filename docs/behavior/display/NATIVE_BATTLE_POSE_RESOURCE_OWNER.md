# Native battle pose resource owner

2026-10-14. Implements a scoped native owner from the original US
[pool/Entry contract](BATTLE_POSE_POOL_PROVISIONING_AND_TYPED_NATIVE_ADMISSION.md),
[part entry/release contract](BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md),
[selection/restart contract](ANIMATION_SELECTOR_AND_RESTART.md), and
[source directory format](NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md).

## Real resource and workspace ownership

`BattlePosePool` retains 256 pose slots, 64 model slots and 64 texture slots.
It chooses ascending first-free slots by their whole flags word. A live pose
owns a genuine converted original bank and its own evaluated CPU positions,
normals, nodes, texture/visibility state, vertices and retained hierarchy
workspace. Admission fails explicitly when a default descriptor is unavailable
or capacity is exhausted; it does not invent an active handle or translate
original unchecked exhaustion into a successful sentinel.
Admission checks real position/node/hierarchy buffers, not current visible
vertex count: a fully hidden default still owns its pose and can later become
visible through a genuine animation selection.

Model reuse compares the original header name. Model references count live
poses; texture references count distinct loaded model cache entries. Texture
cache entries retain the bank containing real converted texture descriptors
and palettes. This is CPU ownership and source cache identity/count projection,
not an implementation of original SDK file handles or global GPU texture-cache
addressing. Native renderer texture objects retain their existing Tex3DS/
citro3d lifetime.

The immutable RomFS loader validates complete `PMPOSE01`/`PMPOSE02` names/order plus the
converted default geometry. It uses a weak cache keyed by both sidecar and
pack paths. Renderer and battle owners retain the same validated resource,
avoiding duplicate bank decoding/storage. The weak cache adds no independent
strong resource lifetime; changing a live immutable RomFS pack is outside this
loader's domain. Logical battle cache references are separate from renderer
ownership and from the field animation registry.

The offset view covers admitted scalar state within a 170-hex-byte native
record. It is not mapped original heap storage, output pointers or a full source
record ABI. Source mode/archive choices, default descriptor0, low-then-high
timestamp stores, cache reset fields, colors, local rate and identity matrix
are retained. Model-specific SDK pointer/sort/buffer services remain adapters.
Fresh native handles0..3 do not assert original global handles: the field/global
allocation history is separate.

## Selection, clocks and lifetime

Names resolve against the full original descriptor directory, preserving first
match and missing-name fallback to descriptor0. Numeric selection accepts an
already decoded nonnegative selector; raw negative original pointer selectors
must first resolve their name. Known source names missing from the converted
subset remain an unavailable evaluation provider, rather than becoming idle.

Force0 with an equal index keeps timestamps and caches. Force1 restarts even
an equal name. Selection does not change local rate or evaluate geometry.
The rate setter writes the incoming binary32 unchanged, including negative,
zero and nonfinite values, without rebasing clocks or advancing a frame.

Clock inputs are native millisecond quotients: mode0 uses GP40/44, other
admitted modes use GP38/3c. Field mode2 also uses the nonzero branch; these
branches are not separate field-versus-battle handle namespaces.
The ordinary CPU evaluator subtracts modulo
64bits, converts to binary32, multiplies60, divides1000 and then multiplies
the local rate with explicit intermediate rounding. Repeated calls at one
clock produce the same elapsed time. Converted sampling retains real CPU
workspace; it does not claim original paired-single, GX, facing or compressed
cache publication equivalence. The native owner now publishes elapsed/rate
lane20, ratio84, selected time24 and continuous sample lanes28/2c/30.
The [source time branches](ANIMATION_MAIN_ENTRY_AND_TIME.md) use binary32
span/offset/division, publish then reload ratio, and use a fused subtraction
for looping. Zero spans take precedence over loop mode; nonloop ratios remain
unclamped even after the selected time reaches the end. The converted sampler
accepts this selected time directly instead of applying a second loop fold.

Finite in-range truncation is the native integer-conversion adapter. NaN,
infinity and int32 overflow leave that conversion unavailable rather than
invoking an undefined C++ cast or guessing original FPSCR behavior. A selected
time below the first converted key also remains unavailable. Independent
source discrete sample lanes34/38/80 now use their independent retained
`PMPOSE02` timeline. Version1/synthetic resources without that data retain
their partial adapter domain; continuous keys never fabricate discrete state.
Five native cumulative buffers now retain actual replay state separately from
fractional position/auxiliary/node outputs. Rebuild, forward and same-current
paths publish the source intermediate and final3c/40 stores. Current>next
preserves earlier fractional outputs; discrete byte/texture state still advances.
Replay uses fused signed-byte increments and cubic output follows the source
operation order. ARM tangent/libm, distinct native buffers and decoded absolute
indices adapt the original GQR/copy/relative-stream services; exceptional,
aliasing and malformed-address domains remain unsupported.
The typed TestXLU query uses retained MODEL wordc4 bit8 and is unavailable
without model metadata. See the [native sidecar format](../format/NATIVE_BATTLE_POSE_DIRECTORY.md).

Unlinked SetEffect(handle,0,0) stores linked=-1 and clears only material-light
bit1. A live linked record requires its detach/release provider. Release retains
other raw state while clearing pose flags and dropping its owned CPU workspace.
Model/texture references decrement separately; final model release drops its
texture reference before model ownership. Borrowed record references follow
the signed post-decrement admission. Ordinary smart-buffer flags40/80 require
their actual provider and remain unsupported. Typed caller/lifetime checks
avoid double release; they are native admission checks, not original GetPtr
guards.

## Native player binding and current display boundary

The trio harness copies authored type28 names before Entry and registers each
of Mario's three original parts and Goombella's part independently in mode1.
Mario modelref3 shares textureref1; Goombella modelref1/textureref1 is separate.
Copied KRN_H_1 on additional Mario parts does not apply an animation or create a
pending bit; their real initial descriptor remains M_Z_1. Part-to-handle binding
is checked before rate/selection. Unregistered enemy/Frankly factories remain
pending at btlDispEntAnime801011fc before a constructor -1 can be treated as a
completed no-model result. They never alias Mario's valid numeric handle0.

The forced part branch executes rate multiplication, real SetAnim and source
bit1/200 consumption with force chosen from the entire resulting word. It then
implements the exact [pending mode gates](../battle/PART_PENDING_POSE_MODES_AND_AUTOMATIC_STATUS_SELECTION.md),
unsigned gravity/float phase counters, status mode dispatch, ordinary zero-effect
and material-light8 operation, and existing part-world-position service.
Automatic/status-specific pose and effect-pattern branches retain their real
providers; no lookup synthesizes a pending request.

Ordinary Main now completes for the admitted real resource, unlinked pose,
private output selector0 and flags8clear domain. The typed native work retains
its source animInit output selector; a nonzero selector requires its actual
alternate-output owner. Facing admission checks the real pose flags after
sampling, retaining its unavailable provider when bit8 is set. See the
[complete source audit](ORDINARY_BATTLE_POSE_MAIN_COMPLETION_AUDIT.md).

The trio continuation retains pre-Main world coordinates, computes original
part alpha/RGB, uses actual model TestXLU and holds at dispCalcZ8001086c with
the depth vector and camera4/class/part arguments retained. Suppressed draw
branches keep the enclosing display provider pending; remaining parts/queued
display, complete global cadence, teardown and field return remain incomplete.
The existing renderer still uses its presentation
clock; sharing immutable banks does not silently substitute that clock for
registered pose history. Full original battle/prologue parity is not claimed.

## Validation scope

Pool tests cover real workspace admission, original/subset index differences,
unknown versus unconverted names, forced and equal-index restarts, unchanged
rate timestamps, elapsed-time rate changes, repeated evaluation, 64bit wrap,
nonfinite rates, shared model/texture counts, borrowed releases, unsupported
smart/linked domains, stale/double release and first-free capacity reuse.
Private original-bank tests use all85 Mario and32 Goombella source names.
The native trio regression checks all four real bindings and finishing M_R_1
at source index4, plus pending flag consumption and the new Main frontier.
Exact test/build/emulator results belong in the session progress record.
