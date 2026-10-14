# Native retained animation cache and ordinary Main

The native battle pose owner implements the admitted ordinary source Main
domain using separate typed cumulative and fractional buffers. This closes a
specific service path, not all animation/rendering or original SDK ownership.
Source authority is the [Main audit](ORDINARY_BATTLE_POSE_MAIN_COMPLETION_AUDIT.md),
[replay](ANIMATION_COMPRESSED_DELTA_CACHE.md) and
[output](ANIMATION_OUTPUT_INTERPOLATION.md) contracts.

`ActorAnimationCache` owns position/auxiliary triples, byte states, node floats
and texture indices/transforms. Entry allocates real per-pose buffers. Source
3c=-1 admits copying bases in position/auxiliary/byte/node/texture order,
then publishing3c=0 before replaying key0. Reverse/loop motion rebuilds;
forward motion applies only old+1..current; equal current skips replay. The
same-current fraction-decrease branch publishes positive-zero40 before the
final overwrite. Source3c/40 remains the controller history; the cache does not
maintain a competing index or presentation clock.

Each decoded key applies position, auxiliary, byte, node, texture streams in
source order. The asset parser already converts unsigned cumulative relative
indices into exact absolute destination indices; this native representation
does not reinterpret them as relative twice. Signed-byte float increments use
fused1/16 multiply-adds. Byte deltas wrap modulo256; texture shifts use separate
binary32 additions. Fractional output never overwrites cumulative buffers.

Current==next copies current position/auxiliary/node state. Current<next
copies then applies next sparse deltas, with source-rounded fraction scale and
the literal cubic operation order for node channels. Cubic OUT tangent±89
guard skips the update before IN lookup. Admitted IN angles use the runtime
tangent-table initializer's original rounded factor3c8efa33. Unknown adjacent
table domains remain unavailable. Current>next preserves earlier fractional
position/auxiliary/node output, including the loop seam; byte and texture
views still consume current cache state. After output, current3c then
fraction40 are published, before the native hierarchy/vertex assembly.

The native `main` entry requires actual complete discrete/model metadata,
an unlinked pose and private output selector0. Its work selector is explicit
state derived from animInit's actual W+e8 zero writer; alternate/paper output
needs a real provider. Optional facing is checked from actual pose bit8 after
sampling. Without that bit, this ordinary Main returns. Linked recursion,
nonzero output selector and facing admission retain unavailable providers.

ARM/GQR decoding, distinct vector buffers, host/ARM tangent math and typed
copies are declared platform/numerical adapters. This does not establish
original libm bit parity, FPSCR exception behavior, arbitrary aliases,
malformed pointers/indices or original global allocation history. No physical
hardware performance result follows from the retained-buffer design.

The trio display continuation retains its pre-Main world-position capture,
applies source alpha/RGB and TestXLU, and retains camera4/class/part/depth at
dispCalcZ8001086c. Suppressed draw paths keep enclosing btlDispMain pending.
Depth, queue callback, remaining parts/unit loop and footer remain open.
The renderer still uses its separate presentation clock until actual queued
draw ownership consumes these registered outputs.

Host tests exercise rebuild, forward, same-key fraction decrease, reverse,
loop-seam retention, nonloop copies, byte wrap, discrete texture shifts,
cubic guard order and unavailable IN lookup. Original trio regression checks
completed cache lanes and the opaque camera4 draw request after finishing
M_R_1. These are native tests, not original-game execution traces.
