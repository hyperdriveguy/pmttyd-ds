# Rogueport Darkly static actor and original talk bindings

US G8ME01 disc0 revision0, read-only tagged-map/REL/DOL/US-message evidence.
Scoped expansion answer to [OW-INTERACT-01](../../requests/OW-INTERACT-01.md),
using [ordinary NPC catalog](NATIVE_AREA_ACTOR_CATALOG.md)'s existing early
story0..55 coverage boundary. This is static source evidence, not Dolphin
observation, all-story lifetime closure or original hardware rendering parity.

## Actor, source placement and admission

Exact original actor/tribe key is Shift-JIS `クラガリさん` (Darkly). In original
`m/gor_02/s`, position(-335,0,-335),direction270,aitypeなし,
territory_typeNPC_TERRITORY_TYPE_NOTHING, zero territory/search/homing values,
BTLNO_INVALID. This tagged setup resource is Shift-JIS generic-close `</>`
grammar, not XML. Preserve original identifiers losslessly.

REL NPC setup row0x45444, actor-name pointer0x19720, flags0x80600,
secondaryflags0; init+12 points0x446a8, regular+16 null, talk+20 points0x44d90.
The gor_02 init unconditionally calls evt_npc_setup at0x4d630. Darkly init
0x446a8 requests evt_npc_flag_onoff(1,"me",0x40000000), then RETURN/END;
no position override, deletion, movement or branch. NPCEntry publishes3, so
fresh initialized main flags0x40080603. Null regular plus return-only init
apart from source mask makes this a bounded stationary source actor.

Darkly occurs after Chu3 in tagged-map and REL setup order (Chu row0x452d4),
so adding him after Chu preserves their original relative registry order for
strict nearest talk ties. A census of all648 annotated gor.rel EVT streams and
all self ADDR32 relocations finds the actor literal only at the setup row:
no direct named door/fade/delete/move references were found. This supports the
ordinary early static slice, but does not close dynamic operands, foreign
scripts or general room-loader/NPC lifetime ownership. Do not infer an all-game
visibility guarantee from that bounded non-finding.

The native early0..55 envelope is an implementation coverage boundary, not an
original init visibility predicate. Explicit story is required; missing story
must not be inferred. Unlike tavern Chu2/sailor, there is no recovered tavern
fade owner for this selected actor. Existing room geometry controls occlusion;
do not relocate him to improve native convenience.

## Exact bank, poses, dimensions and source draw metadata

DOL tribe row0x80333110: namepointer0x802e5270,modelpointer0x802e5280=
`c_f_mura`. Pose pointers+8/+c/+10/+14/+18/+1c/+20/+24 resolve
S_1,Z_1,S_1,T_1,W_1,R_1,Z_1,Z_1. Width/height+28/+2c are30/30,
separate from model scale. Source ordinary draw scalar2 and NPC unit scaleXYZ
follow [ordinary matrix/facing](../display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md).
Talk anchor uses rootY+30, not sampled bounds. Source direction270 is an
animation-facing request; do not interpret it as a direct world90 rotation.

Model `a/c_f_mura`, original texture companion `a/c_f_mura-`. Model resource+c4
is drawmask1; preserve original shape culling/material/pass metadata through the
existing converter. Source animation directory order:
Z_1,S_1,W_1,R_1,T_1,K_1,W_2,T_2,F_1,T_3,A_1A,A_1B,A_1C.
Export descriptor0Z_1 alongside idle/talk to preserve original case-sensitive
missing-name fallback. Bounded source evaluator admits all three:

| Clip | Original loop / key interval / keys | Sparse updates |
| --- | --- | --- |
| Z_1 | no loop,0..0,1 key | no changed streams |
| S_1 | loop,0..120,17 keys |67 node records,8 instant-node records |
| T_1 | loop,0..12,6 keys |34 node records,2 instant-node records,2 texture records |

Model bbox(-5.1595082283,-0.1359241754,-0.6000000238) to
(6.8404917717,18.7694892883,0.6000000238) is source metadata only;
it must not replace width/height or root anchor. Texture scaling, sampler/
material adaptation and source clock/render cadence retain the existing native
converter/runtime limits. No generated replacement art or audio is requested.

## Original story-first-match talk graph

Talk stream0x44d90,size0x1b0,SHA256
8b6c8d655eb42a4c929b5b86b5bd4cefc1249c5b5b3976b1d34d808753c34704
switches raw story operand0xf5de0180. **First arm is CASE_EQUAL175**, before
ordinary thresholds. It checks flag0xf84060bd:0 prints gor_02_034_03 with
speaker"me";otherwise RUN_CHILD kuragarisan_write_name0x446c4. That child
has original name/inventory/state dialogue behavior and cannot be replaced by
the ordinary one-print family.

Subsequent ordered CASE_LESS requests:

| Selector/source site | Original US key / evt_msg_print(0,key,0,"me") |
| --- | --- |
| <56 /0x44dd4 | gor_02_034_00 at0x44ddc |
| <112 /0x44df4 | gor_02_034_01 at0x44dfc |
| <172 /0x44e14 | gor_02_034_02 at0x44e1c |
| <176 /0x44e34 | gor_02_034_03 at0x44e3c |
| <215 /0x44e54 | gor_02_034_04 at0x44e5c |
| <215 /0x44e74 | gor_02_034_05 at0x44e7c |
| <261 /0x44e94 | gor_02_034_06 at0x44e9c |
| <339 /0x44eb4 | gor_02_034_07 at0x44ebc |
| <373 /0x44ed4 | gor_02_034_08 at0x44edc |
| <401 /0x44ef4 | gor_02_034_09 at0x44efc |
| >=401 /0x44f14 | gor_02_034_10 at0x44f1c |

The second<215 is shadowed by the first under source first-match semantics;
retain the original stream instead of repairing its duplicate threshold.
Opcode41 terminal is CASE_GREATER_EQUAL, so story401 prints10. Story175
selects its leading equality arm and never reaches the later<176 arm. These
rules differ from Chu's generic nine-key family: importing prefixgor_02_034_
with Chu's thresholds for all stories would be wrong.

For scoped0..55 admission, original gor_02_034_00 alone is exact and sufficient.
Its payload is111 bytes,SHA256e392cfb5af659d8f8b452f5bb72a4df8cdf68a3e589510b64b1ab9f7cd39f674,
control sequence wait250,wait250,wait250,k. No select token/branch exists in
this arm. All11 original keys exist uniquely in msg/US/gor_02.txt. Preserve
raw text/tokens and actual Darkly attribution; no story/state write exists in
this early print arm. Future all-story binding requires an exact per-actor rule
or original VM stream, including the leading175 branch.

## Rejected alternative and open ownership

East Goomba4 is not stationary despite tagged aitypeなし: init0x43904
sets(-370,0,130), overriding tagged(-375,0,150), and ORs0x40000000.
Regular0x43924 readsXYZ then calls urouro_init_func and urouro_main_func.
Talk0x43978 has request/inventory branches before ordinary story cases. Source
wandering/quest owners must be bound before presenting it as a faithful static
alternative; do not omit its regular event because the art bank is convenient.

## Reproduction and source provenance

Read-only source resources:

| Path | ISO offset / bytes | SHA256 |
| --- | --- | --- |
| a/c_f_mura |874046096 /23436 |149edfc1b25c6644c482257c856a2a1cb278bfd0497a8b2b7514ccd221018d2a |
| a/c_f_mura- |874069532 /22560 |340ad6568f6b6a58f659e2a596b3da55ca008d9270826b2c00e7d3ae8b9691d4 |
| m/gor_02/s |993344684 /9322 |dd1dd8400fa0b94dbd6d266155dd1e9caba82176e7eec38e46acf0f3328ef2ec |
| msg/US/gor_02.txt |1177698108 /128972 |1958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16 |

rel_event_inventory.py freshly re-extracted selected actor events directly
from ISO; self relocation/setup checks and full annotated actor-reference census
pin this bounded static graph. DOL tribe pointers/floats and original descriptor/
loop/sparse metadata were read directly; no extracted original asset/text copied
into source history. Inspection artifacts remain /tmp. Original Nintendo/
Intelligent Systems rights remain, with no redistribution license inferred.
Docs-only; no native make/test/emulator or hardware claim. Exact model surface/
clock/lighting/runtime comparison, dynamic actor lifetime,175 name quest and
broader all-story interaction remain separate outstanding work.
