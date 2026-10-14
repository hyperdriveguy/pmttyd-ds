# Native original item95 window providers

This implements name/help definitions80313dc8/80313df0, preserving original
US global message IDs and control tokens. Source contracts:
[display order/colors](../message/ITEM95_GET_WINDOW_DISPLAY_ORDER_AND_FONT_COLORS.md),
[template/acknowledgment](../message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md),
[original definitions](../field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md).

`ItemGetWindows::prepare` retains an immutable US resource bank and caller-owned
font, expands original msg_window_item_get with item95's original name, and
validates title/description before admission. The font must outlive the windows.
New entry is refused while both finite slots are active. Unsupported definitions
(including the failure window) return-1; native acquisition must not treat this
finite provider as a complete full-storage service.

`enter` allocates the first free local slot, retains the entry-name parameter,
and reproduces partial entry initialization followed by Open. It preserves old
transform/alpha until Seq initializes them. `managerTick` visits active slots in
ascending order, runs the existing source sequence0 transition helper, runs
help main using prior display text and **button-repeat** bits100/200, then
publishes visible display slots. Refresh/interpolation are explicit services;
no render-frame clock or fabricated fixed delay is introduced. It does not run
an item-get event or decide its acknowledgment input.

`display` returns a frame snapshot from before the callback's geometry writes.
Name measures/mutates width/X/height even during transition, growing width only;
text becomes eligible when transitionbit4 clears. Help publishes description
only after its own transition gate. The next manager invocation measures that
retained text; the initial eligible display still has linecount0. AutoDelete
requests set action2,clock0,flags8; source Seq retires active/visible bits at
closure. Text/parameters remain retained through this independent lifecycle.
The event's successful restoration must not wait for deleted-slot actionbit4.

`Renderer::itemGetWindows` uses existing original window-frame Tex3DS resources,
C3D frame transforms, batched font glyph quads, and projected PICA scissor.
Name frame RGB=e1d79b, help=f0f0f0; frame alpha/rotation/scale come from Seq.
Text starts separately at source alpha255/scale1, unaffected by frame rotation
or scale. Name begins black000000ff; Box's original col token appliesc00000ff;
/col resets palette0(initial1e1414ff) for the exclamation mark. Help uses original
(0,384,608,56) clip and existing source help icon plan.

The finite direct-message font adapter accepts valid col/red/reset commands,
suppresses the immediately following LF as the source consumer does, and retains
raw title text in the owner. Color command streams are compiled once at prepare; native draw reuses cached
plain-byte glyph input and color requests without parsing or allocating strings
per frame. Original raw tokens remain retained separately. Width scanning ignores their style effect while
preserving source glyph spacing/halfword arithmetic. Glyph draw delegates to
the established direct-coordinate quad/RNG service with per-glyph color.
Unsupported/malformed commands are rejected before any glyph/RNG requests;
this explicit admission bound does not claim complete original font parsing.

Native quest/event binding, global manager slot sharing, failure/discard window,
outer field update cadence, original effect shader/audio/control and hardware
raster/performance remain pending. Isolated display checks prove this provider's
finite rendering, not actual quest progression or a complete item-get sequence.
