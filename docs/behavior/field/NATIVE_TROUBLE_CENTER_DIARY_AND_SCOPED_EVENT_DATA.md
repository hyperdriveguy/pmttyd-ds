# Native Trouble Center diary and scoped original event data

[Source board provider boundaries](TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md)
pin the first lecture, retained print/selection/Add/Continue lifetimes and all30
start children. The board's help messages use original diary/type7; this was a
missing renderer dependency, not ordinary bubble art.

## Original diary presentation

[Original type7](../display/ORIGINAL_DIARY_TYPE7_WINDOW.md) uses TPL6 from
read-only f/msgWindow.tpl. tools/message_diary_assets.py decodes actual560x176
CMPR pixels into RGBA8 without resizing, pads a1024x256 atlas, and adds one
CLAMP edge texel on every side/corner. Content starts(1,1); native UVs sample
that exact region, preserving the linear-filter footprint at its edges.
The preloaded texture costs1MiB and shares the canonical flat-window quad
adapter with kanban. Citro2D performs the retained texture draw/tint/scaling;
Citro3D owns linear/CLAMP texture state. No frame-time file IO, heap allocation,
asset conversion or new geometry lookup is introduced. Shutdown frees both
texture import metadata and GPU texture. Kanban's existing origin0 is unchanged.

Type7 explicitly selects its dedicated original frame, forces direct placement
and retains text visibility at all opacity. Shared messageTextClip records the
source clip bands; default diary text origin(-218,189) and sourceY43/height126
become native top21.5/bottom84.5. General ordinary/sign/system/selection bands
are preserved. The parser already recognizes diary and preserves ordinary text
speed25; kanban's speed0 behavior is not applied. Original US bytes, keys/pages,
waits, retained stops, selection and append/continue ownership are unchanged.
Fresh normal text opacity is supported by original FontDrawStart_alpha; history
emphasis/direct color overrides and arbitrary window geometry remain separate.
Original diary open/close SFX34/35 are documented but not newly bound here.

## Data-only event closure for later native binding

tools/rel_data_pack.py accepts explicit original data roots, recurses local
ADDR32 data dependencies against the existing US symbol inventory, and packs
only bounded data-section spans. It retains original words/strings except local
ADDR32 native section-base relocation and original module0 imported addresses.
Event spans include the original trailing word required by the label scanner.
Executable sections are excluded; callback code pointers remain external native
provider identities. Unbounded/ambiguous data, foreign dependencies, executable
roots or section-crossing extents fail before output. This is a scoped data
exporter with ADDR32 coverage, not a full REL loader; BSS/code remain providers.

trouble_board_event_assets.py pins the actual US gor.rel hash and selects
board/counter installer, board main, lecture, counter main, and the client,
contents, confirmation and cancellation-item tables consumed by C callbacks.
Transitive closure retains exposition/active-task/start/end tables, both
nonnull quest1/4 start children, null children and original message IDs/NPC
names. Native section mapping matches the existing event adapter policy.
Result:205 objects across5segments,5464-byte PMEVT001 pack. Extracted REL,
metadata and compiled pack stay in ignored local-assets/trouble-board and
RomFS; no original executable, ISO or extracted payload enters this commit.
Nintendo/Intelligent Systems resources have no redistribution license.

The packed data is not yet admitted to a native Board/Counter event owner.
Original handler calls, allocator/work pointer, saved-bank admission, case pool,
priority0/typeef constructor, deferred EVT visit and full NPC/callback services
remain required. No acceptance/completion/item grant is inferred from packaging.

## Validation and next binding

Reviewed tree2dffa7c1920d15777ed922a30b8e6f5a84216eaa passes isolated
make assets-ready SOUNDTRACK=gamecube (artifacts/trouble-board-prereq-assets-private.log),
make test (275/275 C++ and164/164 Python, artifacts/trouble-board-prereq-test.log),
and normal make all -o assets-ready SOUNDTRACK=gamecube
BUILD=build-trouble-board-prereq TARGET=ttyd-trouble-board-prereq
(artifacts/trouble-board-prereq-build-final.log and post-harness
trouble-board-prereq-build-restored.log). The restored3152-file checkout has
zero tracked mismatches (artifacts/trouble-board-prereq-audit-restored.log).
Normal binary SHA2562568825b3273277d433f0daca4242c85e176bfd8d4f721fdd5c6c5096dda94fe.

An isolated DEBUG_SHORT_BOOT=1 direct-presentation harness opens unchanged
irai_help_02 over a declared room2/mask4/story23 fixture; it does not dispatch
the board. Actual capture artifacts/azahar-trouble-diary/original-diary-help02.png
shows original paper/help text/advance star,60 emulator AppFPS/frame9.66ms.
The script verifies copied checkpoint bytes unchanged and no detected native
exception. User emulator profile/saves were not used. Harness main.cpp was
removed and the normal build recompiled/audited afterward. This proves this
native diary presentation, not board acceptance or hardware timing.

The first isolated asset preparation failed from misplaced symbol references;
correct private reference directory fixes it. Initial native helper compile
failed because Texture is a private nested Renderer type; deduction through
shared template helpers fixes that without exposing the type. Failed logs
trouble-board-prereq-assets.log/build.log remain and are not counted as passing.
Source review confirms normal type7 opacity, exact CLAMP gutters and data-only
closure; saved-bank/case/EVT/render/audio/hardware parity remains open.

Next: run the actual board/counter event roots through EventNativeRuntime,
bind retained print/selection/Add/Continue and source switch providers, reuse
TroubleQuestPresentation for init/main/check/end, retain original reset/start
child/write1420 order, and implement quest1/4 NPC/door children. Bind actual
A_i04_keizi/A_i04_kunter contacts through source case9 pool/lifetime scheduling,
then source door-scoped c_monban and original window/menu/door sound services.
Source rendering/event/audio parity and physical3DS performance remain open.
