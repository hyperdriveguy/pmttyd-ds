# Class-menu footers and actual audience item query

2026-10-14 static US G8ME01 revision0. Complete local bodies implemented:
DrawMenuPartyChangeButton801182a8/0x1d4, its Sub80118210/0x98,
DrawMenuPinchMark80117df0/0x58, N_DrawMenuPartyPinchMark80117e48/0x1e8,
N_DrawMenuMarioPinchMark80118030/0x1e0, BattleAudience_GetItemOn801a2d58/0x150
and three linked base/member/item getters. The
[verifier](../../../tools/verify_class_footer.py) pins these bodies, Init2D as a
linked identity, source scalar/palette spans and r2 base. This does not recover
all linked projection/SDK/sound/texture/GX bodies or prove runtime raster parity.
It serves [the original ring draw](ACT_SELECT_RING_DRAW_AND_ICON_ROUTING.md).

## Party and item cue

Capture current B and W=WORD(B+1c78) before camera8; get Mario then Party from
retained B. No Party: request GetItemOn with four private outputs and last item
output0. First output-1 resets W+120 to90 and exits. Otherwise decrement a
positive W+120 by5, or write0 for nonpositive values; project Mario+3c and
request button type5 at projectedX, projectedY-20 and fresh W+120.

With Party, project Mario then Party. Retain their mean X (sum times0.5) and
Mario Y before GetItemOn. A present index plays source sound802ef7a4 only when
W+120 equals90, then decrements and draws type5. No item resets90 and requests
the complete position-change gate; draw type4 at the retained midpoint/Y-20,
angle0 only when allowed. No partner roster or item presence is invented.

Sub rotates Y by angle*source radians coefficient, calls PSMTXTransApply to add
X/Y/0 to the captured translation, then requests btlDispTexPlane2 with actual
button type, original white color and last argument0. That texture helper remains
a linked provider; accepting a button request is not proof of converted art.

## Low-HP warning order and geometry

The parent captures Mario before Init2D and freshly compares its worldX to-120.
Above that threshold it calls Party then Mario warning; otherwise Mario then
Party. Each child captures fresh global B, requests camera8 and gets the actual
unit. Party null returns immediately. HP is signed16: <=0, >=maxHP, or >5 exits
before any warning texture request. These are complete healthy early exits,
not unconditional no-op providers.

HP1 uses the original critical table at80416d30 indexed by fresh GP.language;
GP.clock modulo50<=24 chooses255/233/64/255, otherwise233/82/47/255.
HP2..5 uses the danger table80416d24 with the first color. TEX object lookup
retains original resource pointer traversal and actual palette index. Then
project unit+3c and re-read worldX. Party anchors X=projectedX-20; its Z=10 on
right and0 otherwise. Mario anchors X-20 on right, X+20 otherwise, Z20.
Y=projectedY+60. Compose live retained camera+11c with translation, then fresh
nonzero language adds uniform1.5 scale. Load matrix0 and draw source rectangle
(-28,12,28,-12) with the selected color. Native warning textures, camera/GX state
and pulse visualization still require bindings; source constants are not measured.

## Whole GetItemOn producer and native no-match admission

Base=B+2858 is captured first. Member0 uses a fresh global B+2858+1bc and
subsequent members stride134 for exactly200 slots. Capture each flags word once:
require bit1, exclude100 and2. Signed held index at member+1c must differ from-1.
Its item getter uses fresh global B+2858+f25c+index*48; accept signed state<=4
or exactly7. Do not replace this with a caller-supplied item or a guessed empty
crowd/population count.

On match, a non-null index output stores retained-base+138bc and the member
index to the output. Cache member48/4c and member50 plus the original scalar at
804249cc into base+138c0/4/8 regardless of XYZ output nullness. Copy fresh cache
values to supplied outputs in X/Y/Z order. Optional final item output re-reads
signed held index: negative writes0, otherwise read retained-base+f26c+index*48.
On no match, unconditionally store-1 to the required first output only; leave
other outputs/cache intact. Output aliases and live read order remain observable.
No explicit original status scalar is substituted by this void implementation.

Native class, weapon and target-menu audience checks now call this complete
producer. Actual no-match returns ready and leaves physical X harmless, rather
than freezing at the formerly unbound getter. A real match retains its produced
value/cache effects and reports the interruption-consumer frontier; target
selection, menu end and lights are not falsely completed. This does not admit
throwing actors or implement optional audience taps. The backend Init2D finite
binding restores ordinary texture*raster color, so warning quads do not inherit
the previous class material tint. Original audience-event construction remains
separate from querying actual packed records.

Host checks cover both partner branches, timer crossing below0 and next-call
clamp, one sound gate, midpoint/position cue, healthy exits, critical palette/color,
button translation and GetItemOn no-match/eligible/excluded states/cache/outputs.
The native harbor fixture presses physical X on a ready class menu with actual
no-match data, then still selects Jump through physical A. Native GPU footer
resources, ring publication/normal Game activation, full battle scheduling and
[lower audience scene](../display/NATIVE_BATTLE_HUD_AND_SUBMENU.md) remain open.
