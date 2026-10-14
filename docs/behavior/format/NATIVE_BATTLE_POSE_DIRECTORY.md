# Native battle pose directory and discrete state sidecar

This private conversion format retains original descriptor identity beside a
converted animation subset. It is a native file format, not the original loaded
model pointer ABI. Generated files stay outside source history.

Both versions begin with eight-byte ASCII magic, model and texture names in
zero-padded64-byte ASCII fields, little-endian uint32 descriptor count, then
that many zero-padded64-byte descriptor names in original order.

`PMPOSE01` ends there. It supplies no model flags or independent discrete
timeline; native consumers must keep those services unavailable.

`PMPOSE02` appends original MODEL wordc4 as little-endian uint32, then one
timeline for each descriptor in that same order. Each timeline has a
little-endian uint32 count followed by count records of binary32 time and
uint32 value, both little-endian. Conversion reverses each original word
independently and preserves its bits, entry order and value. Empty, duplicate
and unordered timelines are not rewritten into a sorted sequence.

The original serialized descriptor name entry+60 locates animation T.
Signed source count is T20; serialized relative offset T40 locates its
eight-byte discrete entries. The importer bounds original header and timeline
extents and admits at most65536 entries per timeline. The native loader bounds
the descriptor count to4096, validates names/padding and requires end-of-file
after the final record. These are native admission limits, not source rules.

The owner scans the source timeline independently of the continuous geometry
keys, publishes34 then38 and applies the source nonloop successor clamp. It
loads the selected original value into80. An empty/pre-first timeline leaves
the predecessor-1 fields published and evaluation unavailable rather than
inventing a state value. See the [original timeline contract](../display/ANIMATION_TIMELINE_SAMPLING.md).

The typed TestXLU adapter tests modelwordc4 bit8; pose flags, part colors and
animation selection do not replace that model metadata. A version1 or
synthetic resource without flags yields an unavailable query. The [post-Main
contract](../battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md)
describes its original caller and the next camera depth provider.

The read-only b_mario extract has85 descriptors, each with one time0/value0
entry; c_pkr has32, also one time0/value0 entry each. Both modelwordc4 values
are1. These are actual source properties of these two resources, not constants
for other models or future animations. Synthetic host tests use independent
multiple-key discrete values to verify the consumer rather than hard-coding0.
