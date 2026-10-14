# Native original advance-star indicator

Implemented2026-10-13 for the native dialogue renderers. Original resource
identity and draw requests come from US G8ME01 static evidence, not a matched
Dolphin trace. See[initialization](MESSAGE_RESOURCES_AND_LOOKUP.md),
[window drawing](MESSAGE_WINDOW_DRAWING.md) and
[draw-record admission](MESSAGE_DRAW_RECORD_ADMISSION.md).

`msgInit` selects original `a/hoshi_2` pose `HOS_C_1`; the window callback
requests its draw at(x+width-52,y-height+31,0),yaw180,scale5 after msgDisp returns
an indicator candidate. Native rendering retains that model/animation and these
ordinary-window position/scale requests. Source material/callback ownership and
original draw-dependent animation cadence are not claimed complete.

`MessagePlayback::canAdvanceText` admits full-opacity state3/key wait1, excluding
selection type5. Opening/typing, once_stop/script waits, selection, closed and
closing windows have no star. It disappears as soon as input consumes the
key wait and returns for the next ready page. Native small-window presentation
also uses this cue per the user's general advanceable-textbox request; the
original anchored-type1 callback discards its indicator candidate.

Top dialogue renderers share the original lower-right window inset; development
notice boxes also show the cue when advance is available. The separate opening/
demo bottom renderer places it in the lower-right of its current16,54..304,176
text area. That region remains a native frontend adapter, not the original
window geometry. Both retain uniform0.5 artwork scaling. The renderer restores
the correct top/bottom citro2d target after submitting the original mesh.

`tools/message_indicator_assets.py` reads `a/hoshi_2` and `a/hoshi_2-` from the
read-only ISO. Local PMANIM3 retains original positions, node animation, texture
changes and the80-frame loop. It uses the existing character converter's124px
texture cells/2px edge padding; unused atlas rows are removed to1024x128, with
UV-height scale8 during native drawing. The original animation is evaluated
against native60Hz simulation ticks. This phase/cadence is a native adapter;
no authored replacement star/bounce animation is used. Separate256-vertex
per-frame storage prevents overwriting simultaneous indicator GPU submissions.
Source callback alpha threshold128 is retained.

Assets and provenance remain ignored; paths/hashes/licensing are in
[asset provenance](../../ASSETS.md). Tests cover opening/ready/advance/closed/
selection opacity gates; original-data checks load HOS_C_1 and confirm its
animation changes between distinct phases. Isolated captures exercise waiting,
animated waiting and advancing on the harbor checkpoint. Original shared pose
lifetime/global-clock/active-bit/material fidelity, hardware rendering and
matched original images remain open. This is an advance cue implementation,
not complete message-service or full-game fidelity.
