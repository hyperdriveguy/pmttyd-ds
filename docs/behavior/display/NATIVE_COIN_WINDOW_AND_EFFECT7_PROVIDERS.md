# Native original coin window and effect7 providers

Committed `f82d32cc`. Source owners:
[coin callback/window](../field/GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md),
[effect family](../effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md),
[original coin icon](COIN403_ICON_ANIMATION_VISIBILITY_AND_CAMERA5.md), and
[portable reward](../field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md).
This supplies presentation providers; native reward admission remains unbound.

## Coin window ownership and display

CoinGetWindow loads original msg_window_coin_get from the existing immutable
US global resource bank. It expands original NUM/S tokens and compiles original
color commands/glyph input/measurement at prepare or when the borrowed amount
changes. Repeated display calls reuse the compiled text and measured geometry;
no per-frame expansion, parsing or allocation is introduced for finite20.
Palette0 remains a live draw input. Original20 is redC00000FF; surrounding
text begins black000000FF and closing col restores original palette0.

Only definition80415e84 and a valid borrowed two-word parameter are admitted.
Entry retains old alpha/rotation/scale, then sets original sequence0/open,
rectangle(-175,120,350,42),decoration0. Manager advances the existing original
sequence helper with injected GP rate/interpolation. Display registration is
independent from source callback execution and physical acknowledgment.

Display snapshots the frame before callback writes. Parameterword1==0 returns
before amount/measurement/rectangle mutation. When enabled, title content can
appear even with transitionbit4 set: coin display has no item95 transition gate.
Width grows only when low16 measured width+20 exceeds current width, centers X
with truncation and restores height42+24*linefeeds. Text X is trunc(-width/2),
Y=windowY-8, colorblack, alpha255/scale1 independent of pane transforms.
Native Renderer reuses the existing original Citro3D/Citro2D source frame/font
path for parcel and coin displays; it does not duplicate a new frame renderer.

CloseAutoDelete sets action2/clock0/flag8. Manager clears the borrowed pointer
only after active1 is removed. The source deleted slot still retains transition4,
so action can remain1 after deletion. Native coin parameter retirement must
observe actual window deletion (!retained), separately from source state101;
waiting for action0 through an unmodified state101 adapter can retain it forever.
Do not delay the original reward's quest writes to solve this lifetime boundary.

## Effect7 and camera correction

The existing retained effect-family class supports beginCoin kind7 explicitly:
sizebyte10 gives .75 scale factor, paletteE85BD0FF. Kind6 remains size12/.9 and
orangeEB7D06FF. Original42/44 textures, quad geometry, scale/fade tick driver,
shadow and PICA projected-pattern/alpha-mask pipeline are shared. beginCoin
initializes source camera4; later caller payload38=5 selects camera5.

The source review caught a prior native parcel adapter error: EffectPayload5
was treated as a no-op, leaving effect6 on camera4. Game now applies that typed
request to the retained effect camera. Native field rendering selects the actual
camera4 or fresh camera5 frame for the effect; camera5 copies current camera4
vectors and retains its independent25-degree lens/1..100 native clip and .2.. .3
depth. The Box icon uses that separate camera5 owner too. Host tests cover the
actual Game request, preserving kind7 while selecting5. The historical class
name FieldItemGetEffect6 does not authorize treating kind7 as kind6.

## Validation

Final isolated source tree4dc27d8aaf31a8e55c75e46efce4bdc60194f87f passed
make test:244/244 C++ and142/142 Python; make all passed. Exact logs:
artifacts/coin-window-scoped-test.log and coin-window-scoped-build.log.
Normal artifact artifacts/ttyd-coin-window-reviewed.3dsx SHA256
1136ad7461f9bc70c5b901a51feea0686a7fca798b73c72fd0062007720bcaba.
An earlier private snapshot accidentally included incomplete foreign camera
calls and failed compilation; the final scoped snapshot excludes those calls,
with the shared working edits preserved. This was a snapshot correction.

Tests verify original US text/color/RNG glyph emissions, parameter-before-
geometry gate, transition-independent content, stable compiled inputs, amount
refresh, prior-frame rectangle, only-growing width, retained closing parameters,
deleted-slot action flag, and separate kind6/7 source scales/colors.

Clean DEBUG_SHORT_BOOT artifact SHA256
65df7a5f1c5a7c98e3b7825d6547b44eb6e481072d74f61fbd6ddb4ecc0b020c
passed isolated Azahar parcel dialogue/get/return/save twice. Logs:
artifacts/coin-window-camera5-smoke.log and coin-window-camera5-held-smoke.log.
Held window capture:
artifacts/azahar-coin-window-camera5-held/parcel-open-window.png.
Save proves quest2,5405true/5406false, one95, HP10/FP5 unchanged. Fixture supplies
room0/story8/known quest2 and private DSP firmware; no user profile/saves touched.

A private fixed-pose raster diagnostic shows original coin title/red20 and
pink effect7 at camera5 in artifacts/azahar-coin-window-raster/boot.png.
Log: artifacts/coin-window-raster-smoke.log. Diagnostic artifact SHA256
3a2d593e39a5e3a700b760cebe360b28d694232c669dca2cd1165234f2e1415a.
This preview adds no reward/coin/switch writes, icon403 or row219 audio. Its
render-driven preview window clock and fixed effect age are private diagnostics,
not source scheduling evidence or committed runtime behavior. Source was restored
and audit matched the final review tree. A harness script initially used the
wrong cwd and was corrected before the successful raster build/capture.

Emulator preview showed60 appFPS/100% speed (11.89ms) and native HUD sim2.09ms/
CPU7.5ms/GPU.5ms; held parcel showed62 appFPS/102% speed/11.00ms. These are
individual software-emulator frames, not a stable60 or physical New2DSXL
benchmark. No matched Dolphin pixel/audio parity or new hardware measurement.

Remaining: native coin403 resources/counter/projection/shadow owner, row219
PCM/NDSP binding, separate borrowed-parameter retirement, flag5376 persistence,
Game reward admission and original quest acceptance/full-chain validation.
Original [coin icon animation](COIN403_ICON_ANIMATION_VISIBILITY_AND_CAMERA5.md)
now pins hidden animation and authored four-frame durations for the next binding.
Full global window/display managers and later areas remain open.
