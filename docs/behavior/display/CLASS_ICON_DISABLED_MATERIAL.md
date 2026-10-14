# Original disabled class-icon material

2026-10-14 static US G8ME01 revision0. The flag32 branch inside
iconGX800197f8/0x9a4 starts at80019a08 and joins80019e0c through80019c50.
[The verifier](../../../tools/verify_class_icon_material.py) pins the containing
body and source coefficient WORD8041f86c=555555ff; that does not imply the other
iconGX domains are ported. This extends the
[native class backend](NATIVE_CLASS_MENU_PACKET_BACKEND.md).
No original execution, measured color result or GX/PICA raster equality is claimed.

The branch sets three TEV stages. Swap tables1/2/3 repeat original texture R/G/B
respectively, retaining texture alpha. Color register0 contains(85,85,85,255).
Stage0 computes textureR*85/255. Stage1 adds textureG*85/255 to previous color;
stage2 adds textureB*85/255. No weighted-luminance coefficients are present.
The semantic normalized RGB result is the average of texture R/G/B replicated
across output RGB. Source stages retain their clamp/rounding behavior separately.

Source icon record color is loaded into register1, but its RGB is not a color
operand in this branch. Stages0/1 replace alpha with texture alpha; stage2
multiplies texture alpha by record alpha. Consequently disabled circle icons
do not apply their record's depth-gray RGB tint after averaging. A black record
RGB does not make the icon black. The native PICA mapping uses three stages,
replicated R/G/B operands, coefficient85, modulation then multiply-add and the
original final texture/record alpha relationship. Ordinary flags16 still use
normal texture/record modulation. All stages are reset before each quad and
Citro2D state is restored after the batch.

The packet backend now admits flags48 as well as16 and retains the source flag
for GPU dispatch. Other flags still require their own original transforms and
state. The renderer also refuses pre-initialization atlas work and requires the
batch to reference its admitted catalog, avoiding texture/catalog mismatches.
Original assets are reused; no gray bitmap variant, generated art or additional
ISO payload conversion is needed.

A portable rational-color reference checks primary colors, mixed colors, record
RGB independence and record alpha. This reference deliberately does not claim
original per-stage integer rounding or PICA sampler bit equality. Program checks
verify channel order, coefficient, accumulation and final-alpha ownership. The
native build compiles the actual shader-state calls, but an emulator-visible
class fixture/raster comparison is still required when the source ring producer,
projection and normal Game tutorial ownership are connected.

A subsequent native restore audit explicitly clears all six texture-environment
stages, restores ordinary modulation and then restores Citro2D after the batch.
This complements the per-quad reset; extra disabled stages cannot remain active
across the native backend boundary. Visual raster verification remains separate.
