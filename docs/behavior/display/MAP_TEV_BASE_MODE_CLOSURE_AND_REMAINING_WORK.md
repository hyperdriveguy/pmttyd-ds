# TEV base-mode closure, cadence and remaining material-system work

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or observed image/clock trace.
`tools/verify_map_tev_time.py` checks the containing TEV identity as PARTIAL
semantics,266 reviewed words,six binary32 constants,four original spans,two US
lecture imports and six direct DOL sites. Modes8/9 are complete local base blocks;
six later middle branches and original backend/source/runtime remain open.

All positive-N base dispatch targets now have local contracts:

| Mode | Local contract | Source/usage boundary |
| --- | --- | --- |
| 0 | [Ordinary base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) | Original Mario24 authored draw records all0 |
| 1..7,12 | [Fixed programs](MAP_TEV_FIXED_STAGE_PROGRAMS.md) | Two actual procedural renderer/material N2/M12 requests;other executed scenes unproven |
| 8 | [Pulse/raw clock](MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md) | GP+1c/color source/clock cadence and actual scene selectors open |
| 9 | [Shared scroll/matrices](MAP_TEV_MODE9_SHARED_SCROLL_AND_MATRICES.md) | Four original startup phases/increments pinned;runtime writers/call cadence/scenes open |
| 10,11;unsigned>12 | [Generic base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) | Same request target,no label-based distinct semantics invented |
| N<=0 | [No-texture base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) | Signed fallback before mode dispatch;B/C preserve inputN |

This closes local base REQUEST behavior,not full mapSetMaterialTev semantics or
visible hardware shader equivalence. Full six-site DOL census establishes
static users in mapSetMaterial,renderProc,materialProc,imgDisp_ProjPlane. It does
not prove mode8/9 runtime reachability/frequency in the US content. Original
static Mario mode0 source and explicit mode12 argument preparations remain the
stronger scoped usage witnesses. Scene flag/selector/material producers still
need source/runtime binding;do not assign unused modes from English names.

## Native implementation implications

Mode8 consumes a live raw GP word but does not mutate it;mode9 mutates shared
phases per invocation. Capture those different dependencies/cadences explicitly.
Maintain exact color-register versus KColor distinction,raw seed bytes,low32
shift/single/fused/truncation order,shared phase state and strict one-wrap gates.
Mode9 requests two translated UV matrices regardless of positive N;counter B/C
publication retains N. Original alias/cache/lifetime and backend request order
must be adapted knowingly instead of inferring a two-texture counter reset.

Startup phase/increment metadata is original data,not an observed gameplay
speed. SBSS2 color source80429528 is not a serialized constant palette asset.
The pulse's sin call is a double libm boundary;source sinf wrappers recovered
elsewhere do not prove this full numeric result. FPSCR/libm/GX/memory/runtime
remain separate even with complete local expression/control-flow recovery.
No original assets/poses/images or game instructions are exported/executed
by the verifier. Binary hashes/anchors/census support manual review only.

## Remaining work before shader and full-game closure

Six middle branches remain open:flags0x400,0x10000,0x20000,0x40000,
0x80100000(any),0x800000. Exact entry/skip tests and final callback/publication
are in [the gap register](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md).
Their internals need complete projected/depth/indirect/source requests and
counter mutation recovery. Recover actual material/pose/group/override source
writers/callbacks,normal/texture/shadow cache/resource/pool/link lifetimes,
full SDK/GPU packing/arithmetic and original frame comparisons. Actor shadow
admission/capture/update is a separate system from this material branch.

Original models/textures/text/audio remain reused;portable adaptations require
recorded provenance and fidelity/performance evidence. The requested final
complete-game system requirements document remains deferred until all systems
have adequate behavior/source/validation closure,including gameplay/story/event/
message order/branches/conditions and persistent results. Base-mode coverage is
concrete progress and is not a completion percentage for the game.
