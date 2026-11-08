# Native queued weapon-menu source font

This composition supplies actual font children to the retained weapon callback
`80121ce8`. It neither registers that callback nor activates battle Main. Original
physical directional/A/B input remains owned by the menu controllers; this module
adds no touch command regions.

## Original source and retained children

`BattleWeaponSourceFont` composes the existing complete source owners:

| Entry | Owner |
|---|---|
| `80077b4c`, `800777e0`, `80077844`, `80077758`, `80077aa8`, `80077ab4` | [FontState](RETAINED_SHARED_FONT_STATE.md) |
| `80074bfc` | [DrawStart](RETAINED_JUT_FONT_DRAW_START.md) |
| `80075528` / `8007554c` | [width wrapper / line](RETAINED_SOURCE_FONT_WIDTH.md) |
| `80076a10` | [centered string](RETAINED_CENTERED_STRING_FONT.md) |
| `80076cdc` | [matrix string](RETAINED_MATRIX_FONT_RASTER.md) |
| `800773d0` | [scalar string](RETAINED_SCALAR_FONT.md) |

The read-only `verify_source_font_state`, `verify_source_font_draw_start`,
`verify_source_font_width`, `verify_source_string_font`,
`verify_source_matrix_font`, `verify_source_scalar_font` and
`verify_battle_font_gx_state` scripts pin those original bodies, children and
literal data. The new composition contains no replacement PPC body or native
width estimate. Its tests exercise the actual original BFN through these owners;
static pins alone are not executed-original or pixel-equivalence evidence.

One stable table of source services and one cursor per retained child belong to
the composition. A held request pins entry, integer operands, binary32 operands,
matrix bytes and private-text owner identities. A different request or recursive
resume cannot consume its cursor. SDK matrices, text reads, width arithmetic,
static texture admission and glyph reservation can hold independently. Completed
source stores, matrices, glyphs and RNG requests are never restarted. Retiring or
losing the actual source/publication/font owner permanently rejects this owner.

## Actual memory, text and rendering ownership

The native integration uses `BattleWeaponSourceMemory` first-refusal routing:
actual shared Font/Window process resources, the real Trio VM and the genuine
four-byte GP language alias. Owned unknown/stale cells never fall through to
immutable bytes or zeros. Camera words come from the actual weapon camera8 bridge
through the numeric owner; total source readers require the admitted finite
source domain and fail on violated ownership assumptions. The module itself does
not map guest RAM or invent stack addresses.

Text is exactly one actual guest pointer, a known native two-byte digit buffer,
or a retained native `SourceMatrixFontText` stream. A private buffer remains
separate from guest addressing. An unknown byte or NUL remains unavailable;
already emitted glyphs survive a later missing terminator. The DrawHelp stream
pins its native byte-owner descriptor across holds. Original command/markup
children of width are optional explicit providers: a reached unbound child holds,
without skipping markup, fabricating a label or estimating a result.

The runtime requires actual US language history and completed source font Setup,
the original BFN glyph data, native font lease and actual static texture-object
witness. The native raster view already contains the current camera8 projection,
viewport and scissor; it does not multiply the source view matrix a second time.
Source FontState/DrawStart setters update the real process GX history only after
the corresponding provider succeeds. Glyph admission captures compatible actual
inherited GX evidence; immutable long proof survives subsequent genuine setters
and camera completion while rejecting source-process retirement.

The atomic [glyph sink](RESIDENT_BATTLE_FONT_AND_GLYPH_PACKETS.md) reserves the ordered
slot before either original `irand(64)` request. The Trio factory binds the actual
runtime RNG and original trace identity `80005da0`. After two RNG calls, commit
cannot fail. A blocked reservation consumes no RNG. Ordinary offscreen,
transparent and finite singular glyphs retain the original two-call RNG behavior;
invalid composed/clip coordinates fail before reservation. Submission remains the
outer [ordered publication](ORDERED_WEAPON_MENU_PUBLICATION.md), which seals and
preflights the entire mixed batch before GPU effects.

The renderer integration retains the existing Window bridge for `8007b44c` and
routes only the original font entries above. Weak context callbacks and the
independent long resource proof avoid a context/publication ownership cycle. The
Trio RNG closure captures the actual runtime, without dereferencing its retired
public facade. Existing per-facade registration still rejects destruction,
replacement, reset and old allocation/resource identities before source reads.

## Validation and remaining frontiers

The focused actual-resource test executes width, default start, color, centered,
matrix and scalar strings with every admitted callback held twice. It compares
successful operation traces, copied packet operands and RNG counts. Additional
cases cover original four-shadow-plus-final glyphs and restored color, texture
holds after source stores, reserve-before-RNG, recursive resume rejection,
changed held operands, unknown guest text, late native NUL and sticky retirement.
Strict host, ASan/UBSan and ARM syntax checks cover the composition. A separate
complete-header ARM check applies the exact native journal over the immutable
6944-input accepted graph, including actual Trio and Window bridge code.

The native journal is an integration proposal, not a visible normal-battle
activation receipt. Actual original message lookup/format helpers, unsupported
width-markup children, source icon startup/GX and unreached outer battle scheduler
providers remain explicit holds. No source ready flag, camera pass, source
sequence, original stack value or whole GPU state is synthesized to bypass them.
No art is converted or imported by this package; it reuses tracked original
resources under the existing asset provenance and licensing policy.
