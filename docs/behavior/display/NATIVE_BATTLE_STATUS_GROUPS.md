# Native partner HP and Star Power status groups

This native presentation reads complete original
[statusGetValue](RETAINED_SOURCE_STATUS_VALUES.md) invocations from the actual
Trio/Harbor VM. It adds original partner portraits/HP and acquired-star/AP
art to the compact top-screen HUD. It preserves existing native status-visible
gates and controls. It does not acknowledge original StatusWinMain/StatusWinDisp,
slide/blink choreography, source Main scheduling or a source GX display callback.
The lower audience scene and its physical input alternatives are unchanged.

`NativeBattleStatusReader` retains the whole frozen original value controller,
actual private output cells, captured child operands and optional source reads.
It publishes only a complete invocation plus its presentation data. Native
runtime members own the reader before destroying their VM, pin the actual battle
allocation and check the live global/packed battle backing. Reentry rejects;
retirement latches. Unknown source cells retain the reached operation and never
become default HP, absent partner, language or AP.

The linked battle children follow complete original GetMarioPtr800f87a8 and
GetPartyPtr800f8728 scans of the actual64-slot registry, in original field/predicate
order, including party flag8 exclusion and alliance byte0. Kind translation
reuses existing source-backed `battlePartyIndex` (original800f8444), including
nonsequential kinds228→6,229→3. Each pouch leaf captures the actual8041eb00 pointer
then reads its signed halfword. In particular `pouchGetMaxAP800d3bb8/c` is exactly
`lwz r3,8041eb00; lha r3,+7c; blr`. Its returned value remains output14; this is
not the computed acquired-star capacity. Field/background-partner children are
not guessed: the native battle readers bind only their actual battle owners;
if source GP+14 selects an unsupported field suffix it holds there.

Partner ID08 selects the original table8041767c:
428,428,429,430,431,438,439,440. ID4 uses the actual whole
pouchGetPartyColor800d2e44/14: wrapping14*ID, fresh pouch pointer, unsigned
halfword row flags then arithmetic shift13. Actual color0..5 select431..436;
color6/7 select437, matching the original display branch. The three original
partner rows at80417658 provide HP banner472, portrait and slash480.
The renderer's adapted positions preserve the original art and numeral style.

The AP presentation follows the selection semantics recovered from original
gaugeDisp8013d440/214. It captures a fresh pouch pointer and reads unsigned8c
for each of eight capacity bits; every set bit adds100. Current AP is clamped
to this computed capacity then0. Full groups use AP/100. A nonzero remainder
selects floor(remainder*15/99), with original minimum1, through table456..471;
the source magic-multiply arithmetic is equivalent on this admitted0..99 domain.
The partial icon precedes acquired-star icons. The second loop freshly queries
pouch8c for each bit: it exits at the first unset bit, so holes must not be
compressed into a contiguous count. Full acquired groups use table447..454;
remaining groups use original empty455. Source full-star y offset is12 while
the partial uses0. Native placement reflows these original offsets to400x240.
A zero acquired mask produces no native AP group. No AP producer or maxAP value
is invented from audience count, stars earned or an assumed initial capacity.

`tools/verify_native_battle_status.py` pins the original whole value producer,
GetMario/GetParty/registry/party translation, complete original display/gauge,
all eight exact signed pouch getter bodies, color leaf and the partner/gauge
tables. This is static primary evidence from the read-only US revision0 ISO,
not a Dolphin timing/framebuffer observation.

Exact unapplied journals under `artifacts/native-battle-hud-status/` add actual
Trio/Harbor `statusSnapshot()` APIs and the native renderer groups. The existing
HP/FP/counter presentation uses this complete snapshot when available, replacing
its fixed-Mario-record projection for those invocations. New partner/AP fields
are omitted when the whole snapshot is unavailable. Parent integration registers
the frozen Status5 files once, then the new reader/tests, and owns full
`make test`, `make all` and native field/battle smoke validation.

Focused tests cover actual initialized Trio and Harbor resources (supplied
HP/FP/partner/AP/flags), all seven partner aliases, all eight Yoshi colors,
all256 acquired masks and AP edges, unknown held registry reads without
recapture, source retirement and repeated fresh snapshots. They also retain the
separate complete parent-controller tests. The new owned code is checked with
strict host, ASan/UBSan and ARM syntax; the accepted archived core is not
instrumented by the focused sanitizer run. Original atlas conversion and
independent pixel/UV admission checks are recorded in the frozen manifest.

All art remains unchanged original icon.bin/icon.tpl pixels and descriptor
frames. The shared atlas grows from512x512 to512x1024 RGBA8:2MiB total,
+1MiB of main-memory texture storage (the existing native import requests
non-VRAM backing). No additional texture owner or draw-state switch is added. Generated products stay ignored. Original Nintendo/Intelligent Systems
rights remain; extraction/tracking grants no redistribution rights.

The isolated ARM/Azahar atlas probe verifies actual512x1024,2097152-byte
RGBA8 import and original art display, physical A synchronized real texture
retirement/reimport, stable top-screen pixels and START cleanup with no
unmapped accesses/native exceptions. Imported memory preserves every alpha and
every nonzero-alpha pixel exactly. The existing tex3ds conversion normalizes
hidden RGB in12,963 fully transparent atlas pixels tozero; full raw RGBA equality
is therefore not claimed. Original source PNG cells remain byte-exact. This
standalone budget/import proof does not measure full renderer peak memory,
normal battle callback activation or original framebuffer parity.
