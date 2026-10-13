# Native original demo map visibility

US G8ME01 disc0 revision0, static source evidence; no Dolphin/hardware timing
claim. The [demo scene](NATIVE_OPENING_DEMO_PRESENTATION.md) retains the
original map flag requests and now has a dedicated map pack, rather than
applying ordinary Rogueport field visibility to its cutscene.

## Original map alias and scope

Original `_load` at0x800075e0 compares requested name dmo_00 using source string
0x802bf2cc; equality loads gor_01 from0x802bf2d4 at0x80007678..0x8000767c.
The ordinary branch at0x80007684..0x80007688 loads the requested map. Source
background naming still uses the requested dmo_00; this map alias does not
justify replacing b/dmo_00.tpl with the ordinary gor_01 background.

Scope1/action1 mapobj flag requests set bit1 on a named visual subtree and its
descendants; action0 clears bit1. See the [original flag callback and recursive
ownership](../field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md). The native pack targets
source hierarchy names/offsets, not material names or camera occluder IDs.

| Original subtree | gor_01/d node offset | Demo visibility |
| --- | --- | --- |
| S | 0x158128 | Hidden during book; revealed after book24 |
| S_ie_01_naka | 0x15c938 | Permanently hidden in marketplace |
| S_ie_02_f1_naka | 0x161fa0 | Permanently hidden in marketplace |
| S_ie_02_f2_naka | 0x165648 | Permanently hidden in marketplace |
| S_ie_03_naka | 0x1687b0 | Permanently hidden in marketplace |
| S_ie_04_naka | 0x16e600 | Permanently hidden in marketplace |
| S_gai_kabe_k | 0x159440 | Hidden until post-op_09 request |
| S_gai_kabe_k_01 | 0x159bf8 | Hidden until post-op_09 request |

The last two are separate targets. S_gai_kabe_k owns8 nodes/7 mesh nodes;
S_gai_kabe_k_01 is a distinct1-node/1-mesh subtree under
polySurface440_q00159, outside the first root's descendants. Omitting the
second request leaves source wall geometry visible too early.

The init hides S at dmo REL0x4070. After book24, source0x2dac reveals S;
0x2dc4/0x2ddc/0x2df4/0x2e0c/0x2e24 hide the five interiors and
0x2e3c/0x2e54 hide the two wall roots. After op_09, the1000ms wait, source
camera/tribe requests and then0x34d4/0x34ec reveal both walls;300ms precedes
op_10. Original payload and speaker order remain unchanged.

## Local export and renderer binding

`tools/demo_map_visibility_assets.py` reads m/gor_01/d and m/gor_01/t from the
ISO read-only. It compiles ignored opening-demo-market.vertices/.batches/
.uvanim/.meshanim and opening-demo-market-0..59.t3x. The local metadata JSON
retains provenance, exact hierarchy roots/descendants and eight wall batch IDs.
Five permanently hidden interiors are omitted; both wall roots inherit scene
batch event group1. Other world geometry has group0. Material batching splits
at these boundaries, preserving unrelated geometry sharing wall materials.
GX layer/cull/cutaway/wrap fields remain independent of bits26..31 event group.
Original converter geometry,0.1 map scale, source material colors/texture order
and existing ≤128px texture conversion remain unchanged.

The original pack has60,453 vertices,60 textures and8 wall batches. A wall flag
change toggles batch admission without reload or extracting another map copy.
This is a cutscene-only room; its group1 must not be interpreted through the
ordinary field helper, where group1 means an initially hidden harbor ship.
OpeningDemoScene.visibleMapGroups returns mask1 (group0) before the reveal,
then mask3 (groups0/1). mapGroupVisible rejects unknown groups. Render the new
opening-demo-market room using this predicate for both solid/translucent draws.
No persistent story value is written or fabricated to select the scene flag.
The parent renderer owns the predicate binding; these commits do not edit it.

## Evidence and limits

The extended original demo verifier checks392/12/63 event command hashes/counts
plus51 source scene/input/wait/float/tribe/visibility/map-alias anchors. Host
checks compare all tagged vertex records with independent source subtree
omission before/after the wall reveal; collision/head/wall export remains
byte-identical. The source forest test verifies both distinct wall roots, and
portable scene tests verify hidden-at-op_09/revealed-at-op_10 plus unknown-group
rejection. Cache validation requires all map companions and every texture slot.

This closes the selected visual bit1 requests only. Original live map-object
flag storage, full map display/material/lighting/animation, background transforms,
ambient actor loops, paper/book capture/page turns, crystal/radiation consumers,
fade/letterbox/audio owners and original runtime image/timing remain unbound.
The source map alias and subtree partition do not imply those services are
implemented. No source dialogue, generated art or substitute effects are added.
