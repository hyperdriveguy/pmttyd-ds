# Original audience draw and native atlas clamp

US G8ME01 disc0 revision0, read-only static inspection,2026-10-14.
`battle_audience_draw.cpp` implements whole DispAudience801a38f8/92c and
GetFront801a2b28/98. The verifier pins9 whole bodies (including linked SDK/libm
and address getters),4 source data spans and r2. This is source behavior,
not measured Dolphin/GPU equality. See [population/animation ownership](AUDIENCE_POPULATION_SEATS_ENTRY_AND_ANIMATION_OWNERS.md).

Draw captures audience base and camera4 before fixed GX setup; member0 is queried
then200 retained records advance by134hex. Each iteration queries live map draw
mode. Mode2 admits alpha!=255; other modes alpha==255. Required flags are1/4;
there is no additional GetExist exclusion of2/100. Ordinary type<=11 uses retained
base+c normal bank; guests match two retained base18/19 type bytes and base10/14
handles. Texture lookup/load precedes width/height publications54/58. Each uses
original width/height U16, fresh member type/profile scale18/1c, source1/64 and
member60/64 in ordered single operations. Unmatched guests retain prior texture
and their own scale; unavailable initial texture context stays a frontier.

Scaled pivota8/ac/b0 is retained, SDK scale created and Euler9c/a0/a4 converted
with original radians literal. Double sin/cos results narrow once. The complete
source rotation matrix uses ordered multiply and fused add/subtract expressions,
not the actor/unit rotation helper's different axis convention. Member90..98
receives transformed original normal0,0,-1. Rotation*scale+pivot and translation
preserve source order; type2 alone adds130 hover. Camera view concat and matrix0
load precede copied32-float UV template and a fresh GetFront request.

GetFront queries current record/camera4, uses ordered camera at-eye/normal dot
(single Y product, fused X then Z), and returns0 for dot>=0,1 otherwise. Source
ordered/unordered behavior is represented by this comparison; invalid native
math providers do not invent finite values. Source selected animation+24 versus
profile table+24 chooses UV base8 or0. Material41..44 is retained BEFORE angle
normalization. Angle>=360 subtracts truncated quotient using source negative FMA;
angle<0 adds360*(1-truncated quotient). Negative exact multiples can produce360.
Original GPR27 flip retains its prior loop value in that endpoint; initially
unwritten remains unavailable. It is distinct from FPR27 negative half-width.
Ordinary0/180/360 comparisons and fresh front/animation exemptions1c/20/24 are
preserved. Flip adds4 UV pairs. Source positions are top-left/right at±20 plus
6c/70/74 upper warp, topY40 plus70; bottom±20,0,0. No resizing or center-pivot
replacement is applied to source vertices.

## Material, sampling and native geometry

Original fixed TEV color inputs ZERO/ONE/TEXC/RASC implement texture RGB plus
member RGB with clamp; alpha ZERO/RASA/TEXA/ZERO multiplies alpha. Black member
RGB is the neutral ordinary appearance. Native PICA uses RGB ADD and alpha
MODULATE, with original class1 opaque alpha>=128/depth writes and class2 alpha
blend/no depth writes; camera4 RGB mask suppresses alpha writes. Its independent
world adapter restores normal texture*raster state afterward. Fixed GX request
identities/arguments are validated before packet admission.

Both original TPL banks have CLAMP S/T and linear min/mag for all224 images.
End UV groups contain0.075/1.075 and must retain original interpolation. Merely
clamping four vertex UVs would compress the in-range image; mapping directly
into an atlas would sample neighboring cells. Native backend splits original
0-2 quad triangles at UV clamp boundaries, preserves ALL geometry, then clamps
UVs per resulting region and maps the original64x64 rectangle. The two-pixel
atlas gutter now repeats exact original edge texels. Normal frame92 has an
opaque border; transparent padding was insufficient. No frame pixels or original
indices change and no additional texture page is needed (4MiB still).

Native SDK/libm adapters use finite double trig and single FMA matrix arithmetic.
They preserve the supplied operation/concat shape, but paired-single/GPU rounding
is unverified. Packet vertices are separate owned storage; upper/lower projection
replay performs no packed writes, animation query or simulation advance.

## Live native ownership and remaining scope

Trio and harbor owners expose actual VM draw preparation. Native world renderer
loads private catalog/T3X before submitted frame ownership, prepares original
opaque/translucent passes once and copies geometry into its dedicated stable
linear arena. Trio's upper world pass consumes its actual source-created records.
The existing tutorial lower scene pass replays the SAME prepared audience vertices;
its current fresh source audience remains empty. No audience count or story grant
is added. This does not replace the independent trio lower menu with scene imagery;
that full lower-view/menu reflow remains unfinished.

Trio world preparation and original display-footer animation share a simulation-
timestamp guard/cursor around the existing original interpreter. First opcode can
be label0 and retain frame0; subsequent simulation visit supplies original frame172
for Goomba back idle. No inferred type-to-frame mapping or fabricated idle pose is
used. Both alpha passes at one timestamp leave script pointer/counters unchanged.
This is native display scheduling adaptation; missed preparation timestamps are
not caught up and full original main/control/display cadence remains unbound.
Harbor's packed audience has no live population producer yet and draws no members.

World scene camera is still the declared native adapter: identity view supplied
to source quad preparation, raw points scaled.01, existing world camera/projection
applied once on GPU; GetFront uses that rendered camera's eye/at. This is not
recovered original camera4 framing/projection fidelity. Actual kinopio/type state
controllers, reactions, positive target changes, item throw telegraphs, target
interruption/physical X/optional tapping and full class/turn/menu flow remain open.
Visible/source-normal battle admission and original raster equality require their
own evidence; an8-member host packet test is not a full gameplay proof.
