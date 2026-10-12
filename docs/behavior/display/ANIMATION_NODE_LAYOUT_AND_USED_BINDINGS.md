# Loaded node layout,used builder bindings and native conversion boundaries

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete two original
builders and called SDK primitives,plus serialized a/a_mario metadata.
No original execution/visual measurement or scene reachability frequency.

## Reusable node data and branch ownership

Group record is88 bytes;word+50 is a FLOAT INDEX into pose's evaluated node
buffer+64 (address=buffer+4*index). Group word+54 nonzero selects joint builder,
zero selects transform builder. Available original node data has24 floats per
ordinary record;the two bodies read different subsets:

| Float indices | Byte offsets | Joint consumption | Transform consumption |
| --- | --- | --- | --- |
| 0..2 | 00..08 | Translation | Translation |
| 3..5 | 0c..14 | Own scale with asymmetric Z gate | Own scale with ordinary gate |
| 6..8 | 18..20 | Primary angles multiplied2 | Primary angles multiplied2 |
| 9..11 | 24..2c | Secondary angles before primary rotations | Not read |
| 12..14 | 30..38 | Not read | Rotation pivot |
| 15..17 | 3c..44 | Not read | Scale pivot |
| 18..20 | 48..50 | Not read | Rotation-pivot offset,admitted only if pivot nonzero |
| 21..23 | 54..5c | Not read | Scale-pivot offset,admitted only if pivot nonzero |

Joint parent compensation is supplied as separate f1/f2/f3 by group traversal;
it is not a node field. The traversal checks both current and parent joint
words before choosing that triple. Source/state/lifetime of evaluated node
buffer remains part of the earlier [animation evaluator](ANIMATION_MAIN_ENTRY_AND_TIME.md).

Actual direct DOL builder bindings are ONLY these four in the scoped census:

| Builder | Caller/site |
| --- | --- |
| Joint | evalProc80039ae4;dispProc8003c59c |
| Transform | evalProc80039af0;dispProc8003c5a8 |

This establishes the original [draw traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md)
route. It does not prove all authored groups are visible or reached on a given
US frame. Called SDK bodies' full instruction hashes/words are checked;no broad
SDK-reference count is used to inflate this four-site consumer evidence.

## Concrete original Mario source

Read-only a/a_mario hash/identity and M_Z_1 metadata remain pinned as in
[timeline sampling](ANIMATION_TIMELINE_SAMPLING.md). It has42 groups and1008
node floats. Group joint words:29 zero,13 one;no other word values in this
resource. Root41 uses node float index984,joint0;maximum group node index984.
Thus both builder types occur in the shipped source data. This is serialized
metadata,not an observed frame/state or proof of every group's visibility.

Verifier independently checks group-kind histogram,node count/root/max index,
resource pass/root metadata and original resource SHA. No original model/node
payload is exported to tracked documentation. Loaded relocation/pose-buffer
mapping and original render/material/texture composition remain open.

## Current native conversion boundary

At recovery time tools/character_mesh.py imports character_bake.geometry,whose
matrix(node,index,parent) uses one common pivot/secondary/primary formula and
unconditionally divides parent scale when supplied. That helper differs from
these recovered ORIGINAL branches:transform nodes ignore secondary angles;
joint nodes ignore pivot fields and have asymmetric scale admission/all-axis
zero fallback;transform pivot offsets are suppressed when their pivot iszero.
The parser's sibling parent convention and numeric matrix/trig helpers also need
reconciliation with the original traversal/SDK contracts before claiming full
animation or node-matrix fidelity. ID1 has not changed those native/asset tools.
Original source reuse,an idle mesh or matching bounds is not proof of all node
paths. This is a concrete implementation request,not an invented replacement
art policy or a claim that every current idle fixture visibly differs.

Detailed [joint](ANIMATION_JOINT_NODE_MATRICES.md),
[transform](ANIMATION_TRANSFORM_NODE_MATRICES.md),
[stack/affine](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) and
[rotation](ANIMATION_SDK_AXIS_ROTATION.md) contracts supply the literal behavior.
Original libm/FPSCR/GQR0,loaded resource/stack source invariants and render/
material/GX/original visuals remain separate. Final complete-game requirements
cannot be inferred from this isolated local closure.
