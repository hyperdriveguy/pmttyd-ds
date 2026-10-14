# Native item-get visual resources

This finite implementation supplies original US item95 presentation resources.
It does not yet connect the native Goomba3 acquisition or implement the complete
original window/effect shader services.

`tools/field_item_get_assets.py` reads the ISO without modifying it. It exports
original `a/a_mario` pose M_I_U (index39, nonlooping static0..0) and its
`a/a_mario-` texture into a separate animation pack/atlas. The atlas contains18
124px cells with2px borders, is1024x512, and has UV scale2. Texture scaling and
unused-height cropping follow the existing actor compiler. Native Tex3DS/C3D
loads this2MiB texture only while the selected pose is M_I_U; synchronized
retirement covers pose return, room change and shutdown. Existing atlas poses
remain in their established bank. No per-frame extraction or texture upload.

Original `e/us/effect.tpl` images42(128x128),43(16x128),44(16x16) are exported
without resampling or tinting. I4 intensity becomes equal RGBA components;
source and decoded-pixel hashes are recorded in the ignored generated manifest.
These texture packs are prepared resources; the native effect draw shader is
still pending.

`FieldItemGetEffect6` owns the source-driver clock, startup scale curve,
indefinite lifetime1000, soft-delete16-call fade and deletion ordering. Draw
requests leave age/lifetime unchanged and expose the two source64x32 quads,
matrices, texture IDs and TEV color registers. The pass0 RGB value remains an
explicit provider argument. Host trig/matrix operations are numeric adapters,
not measured GameCube instruction or PICA raster equivalence.

`expandUsItemGetText` reproduces the finite source raw-first-byte vowel choice
and first-occurrence AN/ITEM replacement, preserving message control tokens.
It neither invents item names nor discards source message IDs.

See [original effect](../effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md),
[quad/startup color verification](../effect/FIELD_ITEM_GET_TYPE6_STARTUP_COLOR_AND_QUAD_VERIFICATION.md),
[US text/window acknowledgment](../message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md),
and [portable receive owner](../field/PORTABLE_ITEM95_RECEIVE_SEQUENCE.md).

Next providers: original window action/close service, GX-to-PICA effect shader,
camera/layer ordering, original audio/control and item-name registry teardown.
Connect those to the portable receive owner before enabling actual acquisition;
retain the failed-receive discard frontier rather than fabricating success.
