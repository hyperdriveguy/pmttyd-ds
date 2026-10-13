# Original opening-demo golden-chest effects

Scope: US G8ME01 disc0 revision0, original dmo marketplace only. This binds
existing scene requests to original resources and a portable effect consumer.
It changes no message IDs, dialogue, branches or final input guard.

## Request ownership and update order

Original demo_evt gets hako position, adds10 to Y at0x3988, then requests
crystal_n64 types2/6 at0x3994/0x39d4 with scale0.5 and durations0/60.
Local eff_cam assigns both data+0x38 camera5. After WAIT_MSEC500,0x3a24
requests radiation_n64 type3 at the captured position, scale0.5, duration0;
eff_cam1 assigns data+0x98 camera5. This is independent of later Peach poses
and the final WW10/A|START guard. Request records retain these source addresses
and values; the portable scene schedules500ms as30 simulation ticks.
The [scene verifier](../../../tools/verify_opening_demo_scene.py) pins all
392/12/63 event command hashes and58 explicit anchors including these requests.

The [original outer loop](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
runs evtmgrMain before effMain. The
[effect driver](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md) scans newly active
slots in the same update. Creation therefore performs the first Main update
immediately; subsequent simulation ticks perform one Main update each.
Repeated rendering or same-tick advance calls do not age effects. Missed ticks
replay every update. This scopes fresh-demo ownership; the full effect pool,
contexts and other producers remain separate.

## Source resources and used effect branches

[Exporter](../../../tools/demo_effect_assets.py) reads the ISO/DOL directly,
checks six source function hashes, and exports only used original data:

| Source | Address | Used behavior |
| --- | --- | --- |
| Crystal Disp/Main/Entry |801b8a38/801b8ea4/801b9484|Types2 and6 only|
| Radiation Disp/Main/Entry |801e1280/801e1878/801e1e0c|Type3 only|
| Crystal quad arrays |8039f130/8039f168|Original signed-short XYZ/ST/RGBA|
| Radiation triangle |80419b60|Original three vertices and alpha gradient|
| Radiation r_data |803a7888|Sixteen deterministic five-word rows|
| Crystal/radiation constants |80425040/80425e18|Original float words|

effSetVtxDescN64 at801d771c establishes14-byte source vertices, XYZ signed16,
fraction5 ST and RGBA8. PMDEFX01 exports11 vertices,16 ray rows,25 constants
and two source color words. No replacement geometry is introduced.
Original e/us/effect_n64.tpl textures109/110 remain64×32 I8 and64×64 I4;
conversion retains dimensions and intensity alpha. GX intensity expands to
RGBA equally, as corroborated by the primary
[Dolphin decoder](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/TextureDecoder_Generic.cpp).

Type2 has indefinite sentinel100, original215+15*sin alpha modulation and
16-update fade-in. Its large/small quads rotate age/−8age degrees; texture109
uses S clamp/T mirror with source ST factors0.03125/0.0625. Type6 starts60,
decrements on creation, grows scale by original1.1 each Main, fades with
remaining lifetime and deletes only after remaining becomes negative.
Texture110 mirrors both axes, ignores texture RGB and modulates mask alpha
with original grey ENV color. Crystal matrices use Camera4 yaw while the
source display queue is Camera5.

Radiation type3 does not consume request scale0.5 in its used Main/Disp branch.
Sixteen ray lives begin32+2*i; age<5 supplies alpha=age*50, life<5 fades,
life<=0 resets before the same update's angle-Z velocity addition. Source
row angles and velocity are retained; this branch has no RNG. Disp uses
T*RX*RZ(angleZ+45)*RY*S. X/Z scale is0.25*(3+alpha/255); Y uses alpha/255
times0.3/0.5/0.9/1 by source ray ordinal. ENV white replaces vertex RGB;
the original vertex alpha255-to0 triangle gradient remains.

## Native consumer and evidence boundary

[Portable consumer](../../../source/demo_chest_effects.cpp) exposes requests,
lifetimes and transformed original draw vertices.
[Native helper](../../../source/render_demo_effects.cpp) imports two masks
lazily, binds source wrap/TEV color-versus-alpha behavior, and draws original
quads/rays. Frontend release synchronizes and frees both textures.
Absent source data fails the opening controller instead of substituting art
or acknowledging an effect request without a consumer.

Native view bridges Camera4 yaw and Camera5 display to scene eye/at view.
Trigonometric/affine numeric parity, complete camera/display queue ownership,
original parent GX depth/blend state, lighting and hardware performance are
not established. Native uses existing0.01 world scale, RGB-only writes,
GEQUAL depth, source-alpha blending and no culling: explicit rendering
adapters, not recovered global GX service behavior. No original-runtime
capture or native visual comparison was performed for this isolated branch.
Parent integration must apply the small frontend hook to its newer actor/paper
renderer and compare the final chest/title guard.

Tests cover creation-update timing, source requests/anchor/delays,
fade/lifetime/delete boundaries, radiation reset/update order, original mesh
and UV ranges, sparse-versus-dense tick progression, effects continuing under
the final title guard, missing assets, and I4/I8 alpha decoding. Source packs,
original disassembly and generated reports stay ignored outside source history.
