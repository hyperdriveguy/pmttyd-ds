# Native last-stage color and fog continuation

US G8ME01 revision0. Implementation follows the recovered
[last-stage color/fog graph](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) and
[primary battle-map ownership](BATTLE_PRIMARY_MAP_MATERIAL_OWNERSHIP.md).
Static source evidence and host tests do not establish original image parity.

mapMaterialLastStageColor starts white, applies admitted C1, multiplies alpha
by C2 RED, then applies the three admitted map colors in source order. Every
byte multiplication truncates separately with /255, exhaustively equivalent
to the recovered multiply-high expression. Flags800/1000 suppress the exact
original groups. Zeroed disabled map payloads are preserved as zero, not white.

The retained helper captures its input register values at entry. Blend resolves
current map group before arithmetic and reads its flags after the C2 stage;
color bytes are only read for admitted stages. No provider yields within those
arithmetic stages. Fog obtains and retains its OWN current-camera pointer before
resolving map state, including disabled fog. Near/far are read from that retained
pointer when constructing an enabled request. A held GX request retries identical
captured arguments without repeating getters. Missing providers remain pending;
invalid native fog type indices hold before indexing the five-entry source table.

Trio runtime now owns one primary map-material record shared by translucency,
last-stage color and fog. It comes from fresh bmapLoad clearing and primary
bti_01 fog in the unchanged PMLTSC01 pack. Background stg_00_1 fog never substitutes
for primary character fog. Later map color/fog writers are not fabricated; their
unbound event callbacks still hold. Arbitrary map reload semantics are not added.

The native state owner admits GXSetTevKColor register0, retains it independently
of TEV routing and restores the RGBA value into reserved PICA constant register5.
That register remains unused by the shadow/audience stages. The
[ordinary material TEV backend](NATIVE_MAP_TEV_PROGRAM_AND_ORDINARY_COMBINER.md)
now copies retained K0 into its own active PICA constant stage and multiplies
PREV there. Merely storing K0 does not itself acknowledge a completed material
program. Dummy state does not erase this source register. Fog-off uses the
existing actual Citro3D backend.
Enabled fog requests are produced correctly by the portable helper but remain
unavailable in the native backend pending actual fog equations/LUT support.

Host checks compare all65536 byte products to the recovered signed multiply-high
expression, distinguish C2 red/alpha, check stage truncation and suppression,
retained input/GX state, camera-before-group order, retained camera identity and
all five enabled fog types. Altered records are synthetic tests, not original
execution observations. Native trace/build validation and next boundaries are
recorded in PROGRESS. Original TEV setup, indexed fans, frame comparisons and
battle-to-field return remain unfinished.
