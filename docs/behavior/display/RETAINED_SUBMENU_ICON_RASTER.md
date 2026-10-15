# Retained original submenu icon raster parent

`battle_sub_icon_raster.hpp/.cpp` owns the whole DrawSubIconSub8011847c/ba8
body called by [DrawSubIcon](../battle/RETAINED_SUBMENU_ICON_PARENT.md).
`verify_battle_sub_icon_raster.py` checks the complete original US G8ME01
revision0 DOL body, all 74 direct calls and five initialized constant/key spans
from the read-only ISO. These are static source pins, not executed-PPC proof.
No original data files or generated images are changed by this batch.

## Captures, colors and surfaces

Capture current B and W0 cursor before requesting actual camera8. Capture W98
and truncate it once; source double-bias conversion determines fractional scroll.
Row0 derives alpha from255*(1-fraction), row6 from255*fraction; other rows retain
255. These are original unchecked source conversions, not clamped fade values.
Compare original converted scroll+row with a fresh cursor selection. The
selected path reads the three actual highlight bytes80416d20..22 and retains
full alpha as shade. The other path reads the original unsigned conversion bias,
scales the low8 alpha by the original shade factor and truncates it. Do not
replace either path with a guessed UI color.

Run real Init2D, Concat(camera+11c,input matrix), LoadMatrix. Source palette is
freshly obtained for each texture from captured B+163f8, owner+a0, then that
word's target. Query original TPL index2, load its opaque object and issue two
original Quad calls; independently query/load index3 and issue the third Quad.
An opaque texture object token is native provider metadata, never a fabricated
guest stack or real texture pointer. A returned0 is an acknowledged scalar;
missing descriptor/residency providers hold. Existing class assets0/1 are not
substitutes for these genuine original textures2/3.

## Icon, label and numeric order

Perform the original icon translation/scale and both concatenations. Icon0
falls back to original403. Source argument r6 selects icon flags16 versus48
and the two original RGBA constants; only the alpha byte is replaced. Then run
FontDrawStart, original FontDrawColor, label translation and source scale.
Source stack flag1 enables FontGetMessageWidth: compare its low16 result to150,
and call it **again** if greater. The second, fresh low16 result supplies the
original floating width division. This is not a cached width or a new clamp.

Historical parent slot names are register lanes: `input.label` is source r6
(the color/enabled selector), while `input.enabled` is source r8 (the actual
message string pointer). The interface preserves those exact lanes; it does not
reinterpret the r6 value as a label pointer. The other lanes are r7 icon,
r9 FP cost, r10 BP, first stack word extra-table pointer and second stack word
width flag. They remain distinct across all branches and holds.

The selected r6==1 font-color path really reads r2+0x2158, address80429598.
This address is outside initialized DOL spans: a real runtime mapping/value is
required. The tests explicitly declare their fixture value. It is not an
imported original constant and must never be fabricated by a native binding.
The other font-color path reads its actual original constant.

FontDrawStringMtx80076cdc is a distinct original function from existing
FontString80076a10. Each call retains its matrix and actual guest message
pointer or its owned two-byte private digit string. A caller must supply a
whole retained or explicitly atomic owner for this exact entry point. A partial
font/RNG/GPU prefix cannot be acknowledged or replayed as a new invocation.

Nonzero FP and BP costs follow their complete independent source branches.
Signed division by100 and10 builds all three digit/remainder cells; positive
hundreds/tens select original count/spacing arrays. Units are drawn first,
then tens, then hundreds. Native private strings are exactly the source digit
byte+48 followed by NUL, not substitute numeric formatting. Thus a negative
source value can produce punctuation, and a hundreds value above9 is not
clamped. Each group independently captures its scale and three original
constants/bias words, then performs every original translation/concat/font
call. The matching source message-key lookup and matrix-font call follow the
digits. Null message lookup is a real0 and still reaches the original font call.

An actual nonzero extra-table pointer enables the complete later path. Reload
live W98, truncate it, then load both signed16 fields at actual
pointer+24*(row+converted scroll)+14/16. Issue the first heading, its signed digit
sequence, the second heading and its digit sequence. GP and GP+16c language are
fresh for **each** digit and for the intervening heading position; a language
change inside an earlier font child is therefore visible later. No field or
language read is moved into the initial capture.

## Retention and honest linked boundaries

The controller retains the actual input object and immutable copied operands,
strong provider/context owner, all provider identities, actor-memory/lifecycle
identity, completed effects, native private strings and actual query results.
Every matrix/request/conversion boundary is stable through waits. Required
`current` must prove the real parent allocation/pass/camera/queue lease; false
permanently invalidates the cursor until explicit metadata reset. Provider
callbacks and mappings stay stable through synchronous reads. Reset is metadata
only, refuses recursive entry and never performs source End or guest cleanup.

Typed raster requests name exact source function and callsite. SDK matrix
requests use the established actual translation/scale/concat request type.
Compatible existing Init2D/Quad/texture/IconColor/FontStart/FontColor providers
can be bound directly or adapted without changing their function identities.
The source body adds no successful empty handlers, fake camera, fake palette,
placeholder glyph, synthesized position or unbound GPU acknowledgment. Its
remaining linked boundaries are those actual SDK, resource, icon, message and
matrix-font children. Numeric conversion remains an explicit native policy;
exceptional FPSCR/NaN equivalence is not claimed.

For integration, route the stable `BattleSubIconRasterRequest` from the retained
row parent's8011847c request into one persistent `BattleSubIconRasterCursor`.
Retain the actual provider owner and compose all emitted raster work into the
same ordered whole-weapon publication. Nothing may publish or advance command
input after a partial surface/font prefix. Normal native activation remains
unbound pending genuine80121ce8 camera admission, original surface2/3 ownership
and the exact retained matrix-font/GPU provider.

## Validation

Strict host tests use actual imported original constant/bias words with explicit
abstract child contracts and declared fixture camera/live memory. They cover
source no-cost call census, selected/unselected and edge alpha branches, icon0,
width low16/150 boundary/repeated query, signed1/2/3-digit and negative values,
actual texture2/3 query identities, extra signed fields, language branches,
late W98/extra-field mutation, stable private strings, every-child-twice holds,
callback/output/lifecycle/strong-owner replacement, reentry/reset, missing
providers and stale lease rejection. Immediate versus held traces compare every
actual memory read and provider operand. This is source-branch/retention
validation, not an independent executed-PPC or native-pixel oracle. ARM syntax
and ASan/UBSan checks use the same new source/header graph without mixing live
headers with stale core archives.
