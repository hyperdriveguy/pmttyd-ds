# Native item-get audio validation — 2026-10-14

Committed `1a4a5ee0fb95611fe3a4374f35cb8a85c52968a2` (portable source
envelopes/native fanfare fade/tail) then
`ec3ee1fedbd797c3001e9d2ec9eed6677e394cd8` (retained field stream/native
secondary mute/resume and synchronous group updates). No story or pouch writes.
Source typed providers preserve publication-before-counter updates, D60/R24
quantization, signed16/low8 narrowing, repeat-request stability, normal primary
counters1, secondary endpoint handling and clean float20 tail/prezero stop.
Duration admission retains source positive low16 wrap, refuses nonpositive or
signed32-overflow results and introduces no guessed minimum source tick.

Native logical fanfare group detaches after final integer1 publication while its
existing physical slot/file/buffers retain the100ms tail. Source BGMMain precedes
SoundSSMain, including a possible first tail subtraction in that same outer call.
Native stream refill, query, tail tick, natural retirement, explicit release and
shutdown retain shared mutex/buffer ownership. Source On_f_d begin/NULLresume run
BGMMain synchronously without an extra SoundSSMain. Group0 publishes before1.
Original field metadata is caller-owned; ordinary plaza row67/base70/pan64/bypass0
and freshly admitted global100 produce final muted volume27. NULLresume never
opens/seeks/restarts the field stream. Source global live/imported percentage must
not be inferred from a native sound-toggle boolean.
[Control/native scope](PORTABLE_FIELD_BGM_ENVELOPES_AND_NATIVE_ITEM_AUDIO.md).

Exact validation:

- First step review tree `f5e778bf735592aea97b46ca5bf089225b5880c6`, baseline
  0501e87ca8f0c776b599e99d47c1dfafa29009ec;2335 tracked files matched
  (`artifacts/item95-envelope-source-audit.txt`). `make test`:211/211 CTests and
  135 Python tests; `make -j4 all` passed. Logs
  `artifacts/item95-envelope-reviewed-test.log`/`item95-envelope-reviewed-native.log`.
  Normal `artifacts/ttyd-item95-envelope-reviewed.3dsx` SHA256
  `d08529dc33e5207eb81b632ec4a1c2c6e38c852850e0148218bc7543290fd24e`.
- Final field step review tree `a1e12d9f79bc719a53ac9a62c7c76652db1cd72c`,
  baseline f5e778bf735592aea97b46ca5bf089225b5880c6;2335 files matched
  (`artifacts/item95-field-audio-source-audit.txt`). Same211/211 CTests and135
  Python tests passed, with a successful warning-free native build. Logs
  `artifacts/item95-field-audio-reviewed-test.log`/`item95-field-audio-reviewed-native.log`.
  Preserved normal `artifacts/ttyd-item95-field-audio-reviewed.3dsx` SHA256
  `3637be1cd863c64d356928c8bab69335b5f86431ece120340bf7e3bf65bcc843`.
- Independent source/ownership review found no concrete finite fade/tail/cursor
  defect. Tests cover every Off201 publication, repeated Off, R24/25/59/60,
  fast clamp/global quantization, base70/80, source A0 counters/synchronous
  publication and six float subtractions/seventh-call stop/gate refusal.
- Stable first-step private preview `artifacts/ttyd-item95-envelope-preview.3dsx`
  from `/tmp/item95-envelope-preview.py`, log
  `artifacts/item95-envelope-preview-build.log`; isolated/no-stretch Pulse/DSP
  capture via `/tmp/item95-envelope-audio-capture.py` into
  `artifacts/azahar-item95-envelope`. Begin tick60,Off201tick90; logicalvacancy150
  with tail still retained; new begin first logged157, then backend cleanup.
  Native polling happens after batched simulation updates/log I/O, so157 is an
  observation bound, not proof of exact tail stop count. Read-only ARM/VFP/startup
  disassembly confirms separate single ops and nearest rounding; ignored
  `tail-observation-diagnosis.json`/disassembly retain the evidence.
- Stable final field preview `artifacts/ttyd-item95-field-audio-preview.3dsx`
  from `/tmp/item95-field-audio-preview.py`; preserves normal artifact first and
  modifies only isolated main.cpp plus resume cursor logging. Build explicitly
  uses SOUNDTRACK=gamecube (`artifacts/item95-field-audio-preview-build.log`).
  `/tmp/item95-field-audio-capture.py` used its own no-stretch Azahar profile,
  private save/DSP copy and temporary Pulse sink; clean exits/module cleanup.
  `artifacts/azahar-item95-field-audio` logs retain/mute/silent success at60,
  fanfare start60,Off201/resume140 with producer cursor90112→90112, silent cleared,
  logicalvacancy199/tail retained and new begin/cleanup206. No grant95/5405 write
  or real event acknowledgment occurred. Actual stereo48000Hz capture is nonzero,
  peak21758/RMS2033.9 (`audio-statistics.json`). Viewed boot.png:field rendered
  without exception; Azahar57FPS/native displayed36FPS in that capture interval,
  CPU8.7ms/GPU0.5ms. This is not a physical3DS throughput proof or a stable60FPS claim.
- Final GameCube root/private packed plaza PCM hashes match stg_gor2 library
  (32000Hz/stereo/3284008 frames/hashd83abec5e38338a1740e5644523bcbddf34fc879d0af4a6906a6aeba7897fc6d).
  Final audio_map/config overrides build_assets' older gor1 intermediate.

Next: connect the real Goomba3 quest2 original print→entry→force/get→5405 graph
using these audio providers, retained item-main order/second active check,
Mario/control/camera/windows/effect services and once-only pouch mutation.
Then verify actual original branch/saves/acquisition, not a provider harness.
Source full sound flags/global slew/slot replacement, pause/combined tail modes,
exact SoundSS prefetch/IRQ/device schedule, late-story BGM branches and physical
performance remain open. Recheck steady normal performance when integrating the
simulation audio service. Overall remake/parent requests remain incomplete;
foreign working/staged changes were preserved by narrow commits.
