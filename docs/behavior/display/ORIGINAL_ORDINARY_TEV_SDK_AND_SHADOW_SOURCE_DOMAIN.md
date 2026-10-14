# Original ordinary TEV SDK and shadow source domain

2026-10-14. Read-only US G8ME01 disc0 revision0 DOL and MODEL inspection,
existing recovered contracts, and installed Citro3D/libctru headers. No original
CPU/GX execution, framebuffer comparison or hardware timing is claimed.
Private evidence is retained under artifacts/tev-source-evidence, including
original body hashes/bytes, LLVM disassembly, the SDK table and all three MODEL
source identities. This supplements the [base request contract](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md),
[constant/shadow stages](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md), and
[whole local request graph](MAP_TEV_LOCAL_REQUEST_GRAPH_AND_BACKEND_BOUNDARIES.md).

## Retail GXSetTevOp

The retail helper802b88d4,size0x8c uses a20-word table at803c4ec0. It does not
make nested ColorIn/AlphaIn/ColorOp/AlphaOp calls. Stage0/mode0 selects words
c008f8af and c108f2f0. The color write preserves the existing high8 BP register
address bits and replaces low24; the alpha write preserves high8 and low4 swap
selector bits, replacing the middle20. The caller's following explicit
GXSetTevSwapMode(0,0,0) clears those preserved swap selectors.

Those mode0 words decode to ColorIn(15,8,10,15), AlphaIn(7,4,5,7), with both
operations(0,0,0,1,0): texture times raster, ADD, zero bias, scale1, clamp enabled,
output PREV. Nonzero-stage mode0 selects c008f80f/c108f070 and uses PREV instead
of raster. Preserve the original high-level request; do not invent nested SDK
calls absent from this game's body. The older
[public SDK source](https://raw.githubusercontent.com/doldecomp/dolsdk2001/master/src/gx/GXTev.c)
corroborates the stage0-versus-PREV operand choice, but the retail bytes establish
this game's register-writing implementation. Exact GPU quantization is separate.

| Retail service | Address |
| --- | --- |
| GXSetTevOrder | 802b8e30 |
| GXSetTevOp | 802b88d4 |
| GXSetTevColorOp / AlphaOp | 802b89e8 / 802b8a50 |
| GXSetTevColorIn / AlphaIn | 802b8960 / 802b89a4 |
| GXSetTevKColor | 802b8b7c |
| GXSetTevKColorSel / KAlphaSel | 802b8be0 / 802b8c3c |
| GXSetTevSwapMode | 802b8c98 |
| GXSetNumTexGens | 802b5084 |
| GXSetNumTevStages | 802b8fcc |
| GXSetNumIndStages | 802b87ac |

## Original source draw domain and native material flags

Direct read-only MODEL census gives24 ordinary field Mario draws from a/a_mario,
24 battle Mario draws from a/b_mario,16 Goombella draws from a/c_pkr, and12
Frankly draws from a/c_kuri_h. Every draw has exactly one texture slot and mode0.
The actual trio converter uses a/b_mario,285640 bytes,SHA256
1f29b8e93280bcccf6a488c99a5f4addc35d5604e80d9afcbf5efcec64df51e4;
it is distinct from a/a_mario,215124 bytes,SHA256
61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293.
Both have the same N1/M0 draw-domain witness; neither name substitutes for the
other's exact resource identity. This proves metadata coverage of these banks;
actual original scene flags and execution frequency require separate evidence.

Native BattlePosePool initializes material flags P+e8 to0. The current packed
battle-part display ORs0x40, then clears0x800000 when status16 is false. Thus
its ordinary first-body path reaches F40 unless a separate callback/source
writer adds flags. Do not infer F4 from a character name or shadow-pool presence.

For N1/M0 without other admitted optional masks, the source configures base
stage0, then a constant stage1 unless F10000000 suppresses it. The constant
stage uses selectors12/28 (K0 RGB/A), previous output times K0, and increments
K from1 to2 without changing C1/I0. Final counts publish TexGens(C),TevStages(K),
IndStages(I), in that order, each truncated to low8. White K0 does not remove
this stage from the source request sequence.

## Character-shadow owner leaves

Retail getters freshly load cswp through r13-32320. The actual sdata symbol
is804150e0, initially pointing803c9e38. This corrects the older
[accessor note](SHADOW_DRIVER_ACCESSOR_LEAVES.md): its804150d0 global and
color-pointer description were incorrect. The newer constant/shadow contract
has the correct global and raw color semantics.

Projection getter8002bc7c returns word[owner+10c]. Color getter8002bc88 returns
the raw RGBA word[owner+100], not a color pointer. Matrix getter8002bc94 returns
owner+7c. Texture getter8002bca0 tests owner.flags&1: clear returns NULL; set
calls smartTexObj(owner+dc,holderPointer word[owner+fc]). The holder is not the
image address. Every getter reloads cswp, including the later second texture
getter; do not collapse both queries to one captured object.

Native ShadowWork.preparationGate names source+10c and starts1; flags starts0,
and Main clears bit1. Therefore an F4 material can legitimately query nonzero
projection and then NULL texture using this actual shared owner. The native
owner does not yet provide source projection-texture matrix+7c, embedded
GXTexObj+dc and smart-holder+fc. Enabled texture requests retain their provider
boundary. ShadowWork.projection is the camera matrix at+3c and cannot substitute
for the projection-texture matrix at+7c. Texture44 is a separate resource.

## PICA conversion constraints and current review

Installed libctru GPU_TEVSRC exposes GPU_CONSTANT for the current stage; it has
no operand selecting another stage's constant register. C3D_TexEnvColor is
stage-local. Retaining source K0 in reserved PICA stage5 does not let another
stage reference it: copy retained K0 into that consuming stage's own constant
register and preserve the independent source owner for later restoration.

The current native implementation splits source base RGB/alpha routing across
PICA0/1 to preserve separate channel material/vertex choices. Source constant
stage1 therefore maps to PICA2, multiplying PREVIOUS by its own copied K0 for
both channels; remaining stages pass through. This fits the admitted ordinary
N1/M0 topology. Fixed-mode portable request tables do not imply native fixed-mode
GPU support. Other projected/time/depth/callback branches remain dependencies.
Equivalent topology does not prove GX/PICA rounding or original image parity.

Read-only review of map_material_tev.cpp, native_shadow_program.cpp,
render_shadow_setup.cpp, render_finishing_camera.cpp and TrioAttack's material
bridge found no blocker in this N1/M0/current single-blocked-queue domain.
The source driver preserves request ordering, captured inputs, held GX requests,
constant-stage counter changes and callback K/C/I selective publication. It
retains original fixed tables while unsupported live branches hold. Native F4
projection-zero/texture-null queries use shared ShadowWork; a nonnull callback
holds with its actual identity. Native GPU publication requires every admitted
constant-stage field and known K0. Wider topology, concurrent material ownership,
real enabled projected resources, original pixel equivalence, geometry and field
return remain unfinished.
