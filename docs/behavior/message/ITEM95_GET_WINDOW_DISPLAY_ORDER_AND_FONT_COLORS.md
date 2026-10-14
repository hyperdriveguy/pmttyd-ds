# Item95 get window display order and font colors

US G8ME01 disc0 revision0, static read-only ISO recovery2026-10-13.
Supplements [get window definitions](../field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md),
[US title and acknowledgment](ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md)
and [shared manager sequence](ITEM_TABLE_SELECTION_WINDOW.md#main-lifecycle-and-manager-ordering).
These are original requests and state ordering, not executed GX/PICA pixels.

## Entry is a partial record initializer

winMgrEntry8023ec1c/9c scans ascending68-byte manager records until flags1
is clear. Finite source calls require an available slot: after the bounded scan,
the function unconditionally initializes the selected pointer, without a safe
full-pool failure return. A native owner can refuse unavailable slots without
presenting that refusal as an original successful allocation.

The writes are flags=0 then OR1, action0, clock0, definitionpointer+28,
X/Y/width/height copied from definition+c/10/14/18, priority+30=0,
parameter+2c=NULL, page+38=0 and linecount+3c=0. Entry does **not** clear
text+34, alpha+0c, scale+10 or rotation+14. Do not infer whole-record memset.
Original Open subsequently clears text/page/linecount and requests action1;
Seq0 initializes its transform/alpha. SetParam8023ebf8 stores the caller's
borrowed parameter only when active1. The item record must outlive closing
manager callbacks even when acquisition's original event has already returned.

## Frame submission precedes callback mutations

Fresh winMgrDisp8023f288/484 builds rotation and uniform-scale matrix, reads
current rectangle and definition RGB, replaces alpha with low8(manager+0c),
and calls windowDispGX2_Waku_col8007ad68 at8023f38c with border20.
For item95 both definitions have decorationkind0, so no extra decoration;
8023f6dc reloads definition, loads drawcallback+24, then invokes it.

Thus namewindow frame uses the rectangle from **before** winNameDisp on that
display. Width/X/height changes by the callback affect a subsequent display's
frame. Preserve frame-first ordering; precomputing expanded geometry before
frame submission removes an observable source display lag.

winNameDisp800acf34/1b4 always resolves original name and expanded template,
measures FontGetMessageWidthLine, and performs geometry writes **before**
its winMgrAction gate at800ad088. Let W be low16 measured width and L the
reported LF halfword. If W+20>currentwidth, write width=W+20 then
X=trunc(-width/2); otherwise retain current width/X. Always write
height=definitionheight+24*L. No shrink-to-measured-width write occurs.
Name callback can resize while flags4 suppress its text.

The width result excludes final spacing; glyph advances include it. Neither
this callback nor its direct font consumer wraps lines or width-fits X scale.
A native screen transform can adapt the source rectangle/coordinates without
rewriting original text or altering the source measurement semantics.

## Direct title consumer and distinct colors

When namewindow winMgrAction returns0, request in order:
FontDrawStart80077b4c, FontDrawColor800777e0 with a fresh copy of original
word804210a8, then **FontDrawMessage80075fcc**, a direct-coordinate consumer,
at(trunc(-W/2),managerY-8), with the original expanded byte stream.
A fresh ISO read pins804210a8 to **000000ff**. This is black, distinct from
FontDrawStart's palette0 initializer **1e1414ff**. No outline or matrix-font
wrapper is requested here. DrawStart resets scale1 and draw-alpha255;
manager frame alpha/transform is not applied to this admitted text call.

The original finite title contains `<col c00000ff>` around Box. Direct-message
col requests that whole RGBA word; `/col` restores current **palette0**, not
the caller's earlier black. Thus prefix starts000000ff, Box isc00000ff, and
final exclamation returns1e1414ff under initialized palette state. Retain
original tokens in the title owner. A finite parser may explicitly refuse
malformed/unbound markup; silently removing tokens or coloring the whole title
red does not reproduce these requests. See [direct consumer](WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md#fontdrawmessage-direct-coordinate-help)
and [font state](FONT_DRAW_STATE.md).

## Help text publication has a separate display lifetime

winHelpDisp800acd5c/6c first checks its original item's helpwindow action.
Only on0 does it resolve original description, write managertext+34, then
call winMgrHelpDraw8023e53c. Its helper independently checks flags4, requests
scissor(0,384,608,56), FontDrawStart and FontDrawMessage at
(-210,trunc(fusedSingle(28*page-144))), then resets scissor(0,0,608,480).
No explicit custom color: help uses FontDrawStart's palette0.

Manager update order is Seq, helpmain, display registration. HelpMain's worker
winMgrHelpMain8023e9b8/d8 consumes **previous display's** text+34. After Open,
textNULL/linecount0 persists through transitions. The first eligible help
main therefore still sees NULL; its following display publishes the description.
The next admitted manager main measures its LF and publishes LF+1. Item95's
original description has one LF: linecount2, page0, no paging icons.
Rendering does not execute HelpMain or advance sequence clocks.

For longer help, worker checks repeated key100, else repeated key200 via
keyGetButtonRep80005f94, independently from acquisition trigger300. Existing
native tickItemSelectionHelp accepts its page bits in the directionTrigger
field for the selector route; item-get reuse requires an explicit service input
adapter from its original **repeat** reader. Do not silently replace this with
directional input or change acquisition acknowledgment to repeat semantics.

## Evidence and limits

Fresh named disassemblies of Entry, Disp, NameDisp, HelpDisp, HelpMain and
HelpDraw from read-only ISO are ignored under artifacts/reference/goomba3-services/
window-*-review.{elf,txt}. Raw DOL definition bytes and804210a8 reread directly.
Existing font contracts supply direct col/reset and width-scan semantics.
No code, shared index/progress, emulator profile/save or asset package changed.
Full manager-slot pooling, itemGet/manager outer cadence, palette writers,
GX window/glyph backend, native display and hardware performance remain separate
providers. Static request ordering alone does not prove a complete quest.
