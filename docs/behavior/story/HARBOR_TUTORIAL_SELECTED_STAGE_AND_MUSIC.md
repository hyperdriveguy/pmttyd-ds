# Original setup2-selected harbor battle stage and music

US G8ME01 disc0 revision0, static original-data recovery and local conversion.
No original-runtime image/timing or hardware performance comparison is claimed.
See [setup2 configuration](HARBOR_TUTORIAL_CONFIGURATION.md) and
[stage consumer/transform recovery](../battle/UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md#initial-hide-then-selected-descendant-reveal).
The common consumers apply to this separately selected gor stage, not the trio's
stage1 scenery. All REL locations below are file offsets.

Record2+14 at60ecc selects weighted list60dd0. Weight50, stage pointer60dd8→608f8
and the zero next weight remain original. The stage's name pointers select
bti_01 and stg_00_0; count+8 is4, object pointer+c selects60868. Complete source
object-array SHA256 is d3e951f0a360221dc2bb15d922ac34b2676f38057a9f6bb439e6189ac127bb55;
stage first16 bytes SHA256 is56ae268ef08d7137cef01fcd73902eda64b27612acb1338d7e808c73a9ed9dd0.
Whole gor REL identity is checked before pointer/name/record interpretation.

Each object record is24 bytes: group name pointer+0, signed kind+4/layer+6,
float XYZ+8/c/10, retained halfwords+14/+16. The admitted records are:

| Record | Root | Kind/layer | Source extra XYZ | Native extra XYZ |
| --- | --- | --- | --- | --- |
| 60868 | A1 | 2/0 | (33,0,-48) | (.33,0,-.48) |
| 60880 | A2 | 2/1 | (0,0,71) | (0,0,.71) |
| 60898 | B | 2/2 | (0,0,0) | (0,0,0) |
| 608b0 | C | 2/3 | (0,0,0) | (0,0,0) |

These four names each occur once in the original stg_00_0 joint graph. Their
ancestor is S, under world_root. Authored S translation is
(4.934720993,0,-15.019821167); A1 translation is(-4.422432899,0,15.019821167),
A2(-4.934720993,0,15.019821167), B(-4.934720993,0,7.019821167), and
C(-4.934720993,0,15.019821167). These named nodes have source scale(1,1,1) and
rotation(0,0,0). Descendant mesh joints retain their own original transforms.
The manifest records every authored node SRT/parent/offset, including ancestors.
Original source DOL mapObjTranslate divides engine XYZ by10, concatenates an
additional translation and retains authored SRT. Native map conversion scales
map units by.1, so objectXYZ*.01 is added once after the authored hierarchy.
Zero extra translations do not recenter B/C or their descendants.

Common stage init first hides A1/A2/B/C, then reveals source-selected objects.
The native static admission pack selects all four source roots directly with
retained ancestor matrices. Base bti_01 hides escape_wall. None of the four
selected records has layer6, so the source no-ironframe branch also hides
if_body/if_wire. Other curtains, frame, floor and theater surfaces remain present.
The adapter admits full static scale; original entrance scale/camera animation,
fall/aftereffect object lifecycle and dynamic visibility are not implemented here.

The converter outputs ignored `harbor-tutorial-bti_01` and
`harbor-tutorial-stg_00_0` map packs with original vertex colors, cull and
opaque/translucent batches, plus texture-indexed T3X files. Materials use the
existing first-texture converter. Texture dimensions are powers of two capped
at128px with Lanczos resampling. Native camera/lighting/order are separate
renderer adaptations. Source maps produce3943 and2665 triangles respectively;
these are conversion counts, not performance measurements.

Setup2 record+40 at60ef8 points to BGM_CHUBOSS_BATTLE1. The original DOL BGM
row126 at803162a8 has flags80000000 and alias sound/stream/btl_cbs1_32k.
The helper checks all three bindings against original gamecube_tables before
reading sound/stream/btl_cbs1_32k.stm. Source SHA256 is
fa1cf68fb5ea63b6e3a61697cc843c2d7b23fbb9fa89e0a5eecd2df3cf87940d.
DSP ADPCM converts to unchanged-rate32000Hz/stereo PCM16,1048768 frames,
with original half-open loop[117824,1048768). The ignored WAV and little-endian
loop sidecar are `harbor-tutorial-music.wav` and `.wav.loop`.

`tools/harbor_tutorial_stage_assets.py` is invoked by the existing tutorial asset
converter. Independent source/helper fingerprints and SHA256 of every cached
output protect stage/music reuse; a missing or altered output invalidates its
cache. Verification checks unchanged product modification times on a repeat,
WAV sample format/count and exact loop sidecar. No originals are committed;
Nintendo/Intelligent Systems ownership and no redistribution rights remain.
Game/Renderer/audio activation, original battle scheduler/combat/end and full
scene fidelity stay separate from this resource conversion.
