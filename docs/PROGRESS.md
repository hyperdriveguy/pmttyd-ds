# Progress and next-session handoff

Newest entries first under this single heading. Keep entries dated, write link paths relative to `docs/`, and record exact validation, limitations and next tasks in each entry. Append-only: do not rewrite earlier entries.

## 2026-10-13 — Inbox priority-zero DVD lifecycle/worker

Completed DVDMgrInit/Delete/dvdmgr.o proc_main; checked EVT-01 portion.
[Lifecycle/worker](behavior/file/DVD_MANAGER_INITIALIZATION_CANCELLATION_AND_WORKER.md)
pins queue-before-thread, zero-create self-loop, flag-before-resume, cancel-only
cleanup and infinite main-yield ordering. SDK/providers/original lifecycle/IO/
audio/native binding remain open.

Validation: read-only ISO manifest passed ten spans,57 anchors,seven calls.
Five creation/four deletion/four finite worker-prefix vectors/thread args and
publication/backedge sites checked; main pins reverified. Link audit passed936
Markdown files/zero broken links; scoped diff check passed. Mathematical source
deductions separate from original execution. Docs/metadata only; no make test/
all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/assets committed.
Concurrent native changes excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 SDK DVDReadPrio/Close/path/fileInfo/thread/cancel/yield,
actual lifecycle/queue/callback/request/providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains, original failure/cancel/reinit/yield/IO/audio
traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero DVD manager sorted service

Completed DVDMgrMain/DVD-specific comparator; checked EVT-01 portion.
[Sorted service](behavior/file/DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md)
pins occupied snapshot/low8 sorting/full-priority signed cap, result-3/negative80
block/nonnegative requested-count advancement, fresh callback/close and first
nonnegative scan break. Init/Delete/worker/SDK/providers/original IO/audio/native
binding remain open.

Validation: read-only ISO manifest passed six spans,170 anchors,five calls.
Seven cap/seven result/five advancement/six comparator/five completion-flag/seven
service-gate vectors, three indirect sites/comparator binding/publications checked.
Link audit passed934 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELFs/listings/generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 DVDMgrInit/Delete/proc_main/SDK/read/error recovery,
actual queue/thread/request/callback/providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains, original sorted service/short-read/error/
close/callback/IO/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero DPCM decoder/table

Completed ssDecodeDPCM/16-value dtbl; checked EVT-01 portion.
[DPCM decoder](behavior/audio/GAME_STREAM_DPCM_MARKERS_PREDICTOR_AND_BOUNDARIES.md)
pins marker/no-output, packed nibble/state, signed scale/saturation, output/count,
buffer/retry/switch/loop predictor reset and source-pointer return. Providers/
manager/original decoded audio/native binding remain open.

Validation: read-only ISO manifest passed two spans,132 anchors,zero calls and
16 signed scale values. Six nibble/six marker/seven predictor/six packet/seven
boundary/three zero-request vectors and publication sites checked; PCM/producer
pins reverified. Link audit passed932 Markdown files/zero broken links; scoped
diff check passed. Mathematical deductions separate from original execution.
Docs/metadata only; no make test/all rerun. Fresh full ELF/listing/generator stay
/tmp; no bodies/assets committed. Concurrent native changes excluded. Approved
push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 deferred DVD manager/providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/AI/DSP and
original packet-boundary/saturation/starvation/loop/refill/callback/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero PCM and whole stream producer

Completed PCM continuation/whole local _sscallback; checked EVT-01 and kept
ssDecodeDPCM/manager/providers/original/native binding open.
[PCM/composition](behavior/audio/GAME_STREAM_PRODUCER_PCM_AND_WHOLE_COMPOSITION.md)
pins initial alignment/per-copy count, boundary-before-position, retry skipping
position/end and copied-count early-stop return. All local producer routes closed.

Validation: read-only ISO new manifest passed two spans,136 anchors,zero calls.
Prior dispatch/encoded manifests reverified; all521 disjoint words cover complete
824-byte callback, checked against ISO. Six alignment/seven step/four return
vectors/publication sites checked. Link audit passed931 Markdown files/zero
broken links; scoped diff check passed. Mathematical deductions separate from
original execution. Docs/metadata only; no make test/all rerun. Fresh full callback/
helper ELFs/listings/scoped generator stay /tmp; no bodies/assets committed.
Concurrent native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 ssDecodeDPCM/deferred DVD manager and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/
AI/DSP and original starvation/buffer-switch/loop/early-stop/refill/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero producer live encoded region

Completed _sscallback subrecord/decode-call/ADPCM byte-copy region; EVT-01 checked.
[Live encoded](behavior/audio/GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md)
pins tag/format/helper-return chain, rounded bytes/fresh cursor/count, boundary
rollback/switch/total restart and second-span/requested-return ordering. PCM/
decoder/whole callback/providers/original refill/audio/native binding open.

Validation: read-only ISO manifest passed two spans,172 anchors,two calls.
Ten size/seven tag/five route/six boundary/four total/four return vectors checked;
prefix pins reverified. Link audit passed929 Markdown files/zero broken links;
scoped diff check passed. Mathematical deductions separate from original
execution. Docs/metadata only; no make test/all rerun. Fresh full callback/helper
ELFs/listings/scoped generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback PCM/whole composition and ssDecodeDPCM,
deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/ABI
and original starvation/loop/early-stop/short-span/refill/audio traces. Other
inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero producer dispatch/silence prefix

Completed _sscallback dispatch/silent-fill prefix and return ABI; EVT-01 checked.
[Producer silence](behavior/audio/GAME_STREAM_PRODUCER_DISPATCH_AND_SILENT_FILL_PREFIX.md)
pins low8 row/all-zero/gates/latch, PCM per-store count and ADPCM whole-block
zeroing with original count return. Whole live refill/helpers/providers/original
mute/refill/audio/native binding remain open.

Validation: read-only ISO manifest passed two spans,213 anchors,zero calls.
Ten encoded-count/seven tag/ten dispatch/six PCM/five all-zero vectors checked.
Link audit passed928 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution; raw large loop words
are models, not actual fills. Docs/metadata only; no make test/all rerun. Fresh
full ELF/listing/scoped generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback live refill/helpers, deferred DVD manager/
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains and original
mute/block-boundary/short-span/refill/callback/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header whole callback

Completed allocation/zeroing/mix/cache tail and whole SoundSSPlayChEx_main;
checked EVT-01 local completion while keeping original/provider binding open.
[Tail/composition](behavior/audio/GAME_STREAM_HEADER_ALLOCATION_CACHE_AND_COMPOSITION.md)
pins fixed zeroing, raw allocation failure publication, deferred callbacks,
fade/mix recapture and fresh cache setup. _sscallback/manager/providers/original
startup/audio/native binding remain open.

Validation: read-only ISO new manifest passed eleven spans,253 anchors,ten calls.
Prefix/initial-read manifests reverified; all627 disjoint words cover complete
9cc-byte callback, independently checked against ISO. Six zero/five allocation/
five secondary/six cache/four mix vectors, ABI/callback/publication sites checked;
allocation/pan child pins reverified. Link audit passed927 Markdown files/zero
broken links; scoped diff check passed. Mathematical deductions separate from
original execution. Docs/metadata only; no make test/all rerun. Fresh full ELF/
listing/scoped generator stay /tmp; no bodies/assets committed. Concurrent native
work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 _sscallback/deferred DVD manager and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive SDK/ARAM/AI/
DSP and original allocation-failure/cache/short-read/callback/startup/audio traces.
Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header initial reads

Completed SoundSSPlayChEx_main initial-read region; checked EVT-01 portion.
[Initial reads](behavior/audio/GAME_STREAM_HEADER_INITIAL_READS_AND_PENDING_PUBLICATION.md)
pin cache gate, full unsigned chunk/strict32 advance, independent storage-path
reads/fresh post-primary type and request-before-count/pending. Whole callback
allocation/zeroing/cache/providers/original IO/audio/native binding open.

Validation: read-only ISO manifest passed six spans,189 anchors,six calls.
Twelve linked chunk/seven admission/four type/six pending vectors, four callback
bindings/publication sites checked; prefix/chunk pins reverified. Link audit
passed926 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh full ELF/listing/scoped generator stay /tmp;
no bodies/assets committed. Concurrent native work excluded. Approved push
remains pending valid GitHub credentials.

Next: priority-zero EVT-01 same callback allocation/zeroing/cache continuation,
deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/ABI
and original initial-read/callback/type-change/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream header matching/format prefix

Completed SoundSSPlayChEx_main matching/fields/format/reset prefix and exit ABI;
checked scoped EVT-01 portion and added explicit continuation todo.
[Header prefix](behavior/audio/GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md)
pins unmatched third-row/index3 fallthrough, raw mappings/type narrowing,
sentinel preserving prior200, formats/copy/reset. Whole allocation/initial IO/
cache continuation/providers/original header/playback/native binding open.

Validation: read-only ISO manifest passed two spans,185 anchors,zero calls.
Seven match/five sentinel/five format/five type vectors, twelve field mappings/
two coefficient copies/eighteen reset stores checked. Link audit passed923
Markdown files/zero broken links; scoped diff check passed. Mathematical source
deductions separate from original execution. Docs/metadata only; no make test/
all rerun. Fresh full callback ELF/listing/scoped generator stay /tmp; no bodies/
assets committed. Concurrent native work excluded. Approved push pending valid
GitHub credentials.

Next: priority-zero EVT-01 same callback allocation/initial-read/cache continuation,
then deferred DVD manager/provider/value/extent/alias/lifetime/IRQ/concurrency/
ABI domains and original header/cache/playback/audio traces. Other inbox/native/
full-game work remains open.

## 2026-10-13 — Inbox priority-zero public stream play wrappers

Completed SoundSSPlayCh/Ex/getMarioStDvdRoot; checked EVT-01 portion.
[Play wrappers](behavior/audio/GAME_STREAM_PLAY_WRAPPERS_REPLACE_SLOT_AND_HEADER_READ.md)
pin raw caching argument, replace-before-validation, slot reservation/failure
retention, ordered defaults/shared path/two opens/deferred header request.
Header callback/providers/manager/original playback/native binding remain open.

Validation: read-only ISO manifest passed15 spans,182 anchors,twelve calls;
23 ordered writes/header binding and six slot/four caching-outcome/five raw-
caching vectors checked; stop pins reverified. Link audit passed922 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELFs/listings/generator stay /tmp; no bodies/assets committed. Concurrent
native edits excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSPlayChEx_main header callback, deferred DVD
manager/providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains,
transitive SDK/ARAM/AI/DSP and original replacement/slot-exhaustion/open-failure/
header/cache/playback/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream main

Completed SoundSSMain; checked EVT-01 portion.
[Main fades/stops](behavior/audio/GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md)
pins initial stop priority, sequential40/20/10 mode processing, zero-before-
subtraction, recaptured rows/three mix/two stop blocks and counter/close-free/
marker order. Providers/FP/original cadence/fade/stop/audio/native binding open.

Validation: read-only ISO manifest passed eight spans,398 anchors,21 calls.
Thirteen finite stable-frame/six stop-gate/five counter vectors plus three mix/
two stop groups checked; pan/stop pins reverified. Link audit passed921 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELF/listing/generator stay /tmp; no bodies/assets committed. Concurrent
native work excluded. Approved push remains pending valid GitHub credentials.

Next: priority-zero EVT-01 game stream play/deferred DVD manager and actual
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive
SDK/ARAM/AI/DSP and original cadence/fade/stop/mix/audio traces. Other inbox/
native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream cache callbacks

Completed _ssDVDReadAsync_cache_next/cache_aram/DVDMgrGetLength; EVT-01 checked.
[Cache callbacks](behavior/audio/GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md)
pin fixed32768 transfer despite short logical count, serial/low8 owner, ordered
cursor/progress, fresh length/continuation, resets/type and initial playback IO.
Providers/SDK/original cache/DMA/startup/audio/native binding remain open.

Validation: read-only ISO manifest passed nine spans,294 anchors,eleven calls.
Six owner/four next/seven continuation/six branch/six index/twelve linked chunk
vectors and six callback bindings checked; interrupt/completion pins reverified.
Link audit passed920 Markdown files/zero broken links; scoped diff check passed.
Mathematical deductions separate from original execution. Docs/metadata only;
no make test/all rerun. Fresh ELFs/listings/generator stay /tmp; no bodies/assets
committed. Concurrent native changes excluded. Approved push remains pending
valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/deferred DVD manager and actual
providers/value/extent/alias/lifetime/IRQ/concurrency/ABI domains, transitive
SDK/cache/ARAM/AI/DSP and original cache/error/short-read/DMA/callback/startup/
audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero stream completion callback

Completed _ssDVDReadAsync_activeChk; checked EVT-01 portion.
[Completion callback](behavior/audio/GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md)
pins first-match/negative exit/requested count/terminal flags/latch, independent
ready-row scan, activation-before-inline mix-before-old flag8 and counter order.
Providers/SDK/FP/original callback/IRQ/audio/native binding remain open.

Validation: read-only ISO manifest passed seven spans,262 anchors,seven calls.
Eight completion/four match/seven result/eight ready/five mix-gate/five counter
vectors checked; linked pan manifest reverified. Link audit passed919 Markdown
files/zero broken links; scoped diff check passed. Mathematical deductions
separate from original execution. Docs/metadata only; no make test/all rerun.
Fresh ELF/listing/generator stay /tmp; no bodies/binary assets committed.
Concurrent native edits excluded; approved push pending valid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/cache-next/cache-ARAM callbacks,
deferred DVD manager and actual providers/value/extent/alias/lifetime/IRQ/
concurrency/ABI/FP domains, transitive SDK/ARAM/AI/DSP and original callback/
short-read/error/startup/IRQ/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream interrupt engine

Completed SoundSSMainInt/DVDMgrReadAsync/cache_flush; checked EVT-01 portion.
[Interrupt engine](behavior/audio/GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md)
pins three-row gates, unsigned chunk/strict32 advance, storage-path and fresh
buffer reads, request-before-pending, companion copy/shared latch and deferred
DVD/ARQ callback bridge. Callback/manager/providers/SDK/original IRQ/IO/audio/
native binding remain open.

Validation: read-only ISO manifest passed six spans,302 anchors,seven calls.
Twelve chunk/ten stable gate/six latch/six copy vectors and two callback bindings
checked. Link audit passed918 Markdown files/zero broken links; scoped diff
check passed. Mathematical source deductions separate from original execution.
Docs/metadata only; no make test/all rerun. Fresh ELFs/listings/generator stay
/tmp; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/play/three _ssDVDReadAsync callbacks,
deferred DVD manager consumers and actual providers/value/extent/alias/lifetime/
IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/AI/DSP and original IRQ/
IO/callback/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero game stream volume

Completed SoundSSSetVolCh; checked EVT-01 portion.
[Volume setter](behavior/audio/GAME_STREAM_VOLUME_CACHE_AND_REQUESTS.md)
pins low8 float10-before-floatc, fresh old/new gates, pool recapture and ordered
mono/dual requests using new-row data. Providers/FP/original volume/audio/native
binding remain open.

Validation: read-only ISO manifest passed four spans,102 anchors,three calls.
Six cache/eight gate/seven finite split/four type vectors and pool annotations
checked. Link audit passed917 Markdown files/zero broken links; scoped diff
check passed. Mathematical deductions separate from original execution. Docs/
metadata only; no make test/all rerun. Fresh ELF/listing/generator stay /tmp;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials; local commits retained.

Next: priority-zero EVT-01 game stream engine/play/callbacks and actual providers/
value/extent/alias/lifetime/IRQ/concurrency/FP domains, transitive ARAM/AI/DSP and
original volume/mono/dual/audio traces. Other inbox/native/full-game work open.

## 2026-10-13 — Inbox priority-zero game stream pan/surround

Completed SoundSSSetPanCh/SoundSSSetSrndPanCh; checked EVT-01 portion.
[Pan/surround](behavior/audio/GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md)
pins low8 halfword caches, raw mono pan, divisor63 split, every non-one type's
ordered dual requests and old/new pool rows in surround setter. Providers/FP/
original pan/surround/audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,185 anchors,six calls.
Seven finite split/four type/five cache vectors and pool bindings checked.
Link audit passed916 Markdown files/zero broken links; scoped diff check
passed. Mathematical deductions separate from original execution. Docs/metadata
only; no make test/all rerun. Fresh ELFs/listings/generator stay /tmp; no bodies/
binary assets committed. Concurrent native edits excluded. Explicitly approved
push of all commits attempted again; GitHub rejected username/token. Local
commits retained; push requires valid credentials.

Next: priority-zero EVT-01 game stream volume/engine/play/callbacks and actual
provider/value/extent/alias/lifetime/IRQ/concurrency/FP domains, transitive ARAM/
AI/DSP and original mono/dual/pan/surround/audio traces. Other inbox/native/full-game work remains open.

## 2026-10-13 — Inbox priority-zero game stream frequency

Completed SoundSSSetPlayFreqCh; checked EVT-01 portion.
[Game frequency](behavior/audio/GAME_STREAM_FREQUENCY_CACHE_AND_REQUESTS.md)
pins halfword-before-fresh-flag8, shared counter, primary/conditional secondary
post-child frequency/type rereads and restore order. Engine/providers/IRQ domains/
original frequency/audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,53 anchors,four calls.
Six narrowing/seven gate/four fresh-request/six counter vectors checked. Link
audit passed914 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical source deductions are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials.

Next: priority-zero EVT-01 game stream pan/volume/engine/play/callbacks and actual
channel/base/flag/frequency/type/ID/counter/saved-state providers and valid
value/extent/alias/lifetime/IRQ/concurrency domains, transitive ARAM/AI/DSP and
original frequency/dual-stream/IRQ/audio traces. Remaining macro/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero game stream fade/stop

Completed SoundSSFadeoutCh/StopCh/SoundSetFadeTime/DVDMgrClose; checked EVT-01
portion. [Fade/stop/close](behavior/audio/GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md)
pins signed GP/duration/minimum step, nested counter wrap/fresh decrement,
flags-before-deferred-DVD/stream-free and global invalidation order. Consumers/
GP/counter/IO/provider/FP domains/original stop/audio/native binding remain open.

Validation: read-only ISO manifest passed nine spans,117 anchors,six calls.
Seven finite fade/six counter/five stop/three time-set/four deferred-close vectors
checked. Link audit passed913 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 game stream engines/play/frequency/pan/volume and
GP/time/counter/marker/base/flags/table/handle/stream-ID/type providers/deferred
DVD owner and valid value/extent/alias/lifetime/IRQ/concurrency/FP domains,
original fade/stop/IO/audio traces. Remaining decoder/DVD/DMA/macro/song/backend,
lower-priority inbox/full native/game work remain open.

## 2026-10-13 — Original Goomba4 ordinary talk with explicit quest knowledge

Committed91a091e: [native known-state talk](behavior/field/NATIVE_GOOMBA4_ORDINARY_TALK.md)
retains all11 source-order ordinary kuribo4_talk message rules, including exact
164/175/176 and multi-suffix boundaries. Original US message text/control tokens
and source speaker me use the existing actor/anchor/paper presentation; no
ordinary story/quest write or fabricated fallback is added. Saved state now
retains optional GSW1420/GSF5405/GSF5406 knowledge. Both source quest arms must
be excluded by known byte/flag values before ordinary talk becomes available.
Legacy unknown remains unavailable and retains source candidacy/A suppression;
possible item-count arms remain unbound rather than treating native mushroom
quantity as original key-item storage. Guard scope is exact gor_02/Goomba4
profile2b0a9f83, independent from reusable character art.

Explicit fresh-file opening applies recovered swInit zero to those three fields;
room rebuilds retain them. Native40-byte checkpointv7 packs value/knowledge bits
into the existing native quantity word, validates reserved/value-without-known
bits and quantity range, and preserves knownfalse. Wholly unknown state still
writesv6; v1..6 loads clear any prior quest knowledge. This is a partial native
saved-switch adapter, not the complete original switch/inventory/card format.
Fixture tool --quest/--flag5405/--flag5406 supplies explicit development state,
with omission remaining unknown. No product quest guesses or implementation UI.
Original model/message assets are reused; catalog provenance/licensing updated.

Documentation agent recovered [full quest/item-window graph](behavior/field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md)
and OW-INTERACT-01 breadcrumb from read-only ISO, including original reset
banks, fixed95 table, dynamic107 table, check/remove wrappers and item-window
entry/poll/delete/key ownership. Item selection is not message Yes/No selection.
Both quest arms and their original item services remain next native work.
Independent review found no blocking binding/save defect and requested source
boundary/legacy clearing tests; those are included.

Validation: final shared `make test` passes151 CTest/103 Python,
`/tmp/goomba-talk-shared-test-complete.log`. Isolated snapshot
`/tmp/pm-goomba-talk-fzkn9v87` passes150 CTest/103 Python,
`/tmp/goomba-talk-isolated-test.log`. Tests cover all ordinary thresholds,
unknown/knownfalse/quest2/13 guard combinations, original raw US message and
return without story write, v7 optional-value roundtrips/reserved corruption,
quantity overlap rejection and explicit v1..6 clearing of known prior state.
Shared/isolated `make all` pass without warnings/errors:
`/tmp/goomba-talk-shared-native-complete.log`,
`/tmp/goomba-talk-isolated-native.log`. Intermediate shared attempts hit a
concurrent reward narrowing/type mismatch and full /tmp; final runs supersede
those failures. Applied one-line std::min<int32_t> ARM portability fix inside
concurrent uncommitted trio reward work, preserving its feature scope for its
owner. Removed only obsolete build outputs/duplicated romfs packs from earlier
owned live-wander/static snapshots to recover /tmp space; their source snapshots
and final project artifacts remain. Future snapshots should use ignored project
artifacts storage to avoid the16GiB /tmp limit.

Final isolated binary `artifacts/ttyd-goomba-ordinary-final.3dsx` passes corrected
field-ready `tests/azahar_static_npc_talk.py` with both explicit quest0 and legacy
unknown fixtures. Captures/profiles `artifacts/azahar-goomba-ordinary-final` and
`artifacts/azahar-goomba-unknown-final`; logs
`/tmp/goomba-ordinary-emulator-final.log` and
`/tmp/goomba-unknown-emulator-final.log`. Actual original Goomba field/window/
return inspected. Known ordinary talk opens original window without onset save;
unknown opens no substitute window; return admits save with room2/story8,
coordinates, exact knowledge bits and checksum unchanged. Earlier shared native
binary also passed original talk at `artifacts/azahar-goomba-ordinary`.
Software captures show60fps field/return and53fps window; not controlled New2DS
hardware, dense-area benchmark or matched original-runtime fidelity evidence.
Link audit passes912 Markdown/zero broken links; whitespace and script checks
pass. 2a5ac1a removes accidentally staged concurrent EXP ledger subsection from
feature history while preserving its working entry. ISO/assets/packs/captures
remain local/ignored; user emulator profile/saves untouched.

Next: bind original item-window tables and native item selection, key-item
count/removal/revise owners, both quest graphs and full switch/pouch persistence;
then extend original actor/event/object profiles and additional areas, complete
scheduler/shared RNG/clock and lifetimes, and measure Dolphin/New2DS rendering/
performance. The full requested goal remains active; early Rogueport is a slice.

## 2026-10-13 — Inbox priority-zero game stream leaves

Completed SoundSSMuteOn/Off/Check/GetVolCh/ContinueCh; checked EVT-01 portion.
[Game stream leaves](behavior/audio/GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md)
pins raw channel/flags/zero gates, mute-off zeroing, signed volume conversion
and clear/copy/fresh-OR continue order. Engine/writers/callbacks/provider/FP
domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,59 whole-body anchors,
zero calls. Seven flag/four fresh-continue/seven volume/five address vectors
checked; invalid FP/conversion outputs unresolved. Link audit passed911 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELFs/listings/generator stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 SoundSSMain/Int/play/fade/stop/frequency/pan/volume
writers and producer/decoder/DVD/DMA callbacks, actual channel/base/flags/float/
index/IRQ/providers and valid extent/value/alias/lifetime/concurrency/FP/
conversion domains, original mute/check/continue/fade/audio traces. Remaining
macro/song/backend, lower-priority inbox/full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream mix/local inventory

Completed sndStreamMixParameter/ABI helpers and audited all17 tagged stream.c
local functions; checked EVT-01 portion.
[Mix/local inventory](behavior/audio/MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md)
pins AuxB0/three publication routes/fresh state2/retained raw argument recursion/
IRQ order. All17 local tagged functions answered; provider domains/transitive
ARAM/DSP/original audio/native binding remain open.

Validation: new read-only ISO manifest passed15 spans,361 anchors,16 calls.
Six byte/mode/gain vectors/fifteen stable chain models checked. All17 CSV-tagged
functions matched name/address/size to full fingerprints and complete/composed
contracts;16 inventory/prerequisite manifests reverified. Link audit passed910
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source/inventory checks are separate from original execution and full runtime
coverage. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Previously approved push remains pending
invalid GitHub credentials.

Next: priority-zero EVT-01 actual stream caller/state/ID/parameter/mode/voice/
companion/IRQ/resource providers and valid value/index/extent/encoding/alias/
lifetime/concurrency/stack/ABI/termination/FP domains, transitive ARAM/DSP and
original startup/refill/mix/frequency/free/chained failure/audio traces.
Remaining macro/song/backend, lower-priority inbox/full native/game work open.

## 2026-10-13 — Inbox priority-zero stream frequency chain

Completed sndStreamFrq/ABI helpers; checked EVT-01 portion.
[Frequency](behavior/audio/MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md)
pins raw frequency-before-fresh-state2, reference/voice reads, exact single
multiply-before-divide/signed conversion, retained-frequency companions/recursion
and four-stage enables. Mix/FP/chain/IRQ/providers/original audio remain open.

Validation: read-only ISO manifest passed11 spans,422 anchors,16 calls. Eight
finite/exception-domain pitch vectors/fifteen stable chain models checked for
writes/state2 requests/partial progress/recursion/IRQ counts. Link audit passed907
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution; invalid FP/conversion
outputs unresolved. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original ELF/listing/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream mix and actual frequency/reference/ID/state/
voice/companion/IRQ/providers and valid value/index/extent/alias/lifetime/
concurrency/stack/ABI/termination/FP/conversion domains, transitive ARAM/DSP
consumers and original frequency/chained partial-update/audio traces. Remaining
macro/song/backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream free/reverse clear

Completed sndStreamFree/ABI helpers; checked EVT-01 portion.
[Free/reverse clear](behavior/audio/MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md)
pins five stages, deactivate-before-fresh-selector/link, parent buffer exits
before descendants and deepest-first retained-slot state0/enables. Frequency/
mix/resource/chain/IRQ/provider domains/original free/audio/native binding open.

Validation: read-only ISO manifest passed ten spans,453 anchors,24 calls.
Fifteen finite-chain models checked for stable deactivation/exit/recursion/reverse
clear/missing links/retained fields/request counts. Link audit passed903 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream frequency/mix, actual ID/state/voice/buffer/
companion/IRQ/resource providers and valid value/index/extent/alias/lifetime/
concurrency/stack/ABI/termination domains, transitive ARAM/DSP consumers and
original chained free/deactivation/failure/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero stream deactivation chain

Completed sndStreamDeactivate whole local body; checked EVT-01 portion.
[Deactivation](behavior/audio/MUSYX_STREAM_DEACTIVATION_AND_COMPANION_CHAIN.md)
pins five IDs before recursion, fresh state1/2 unblock-before3, retained buffer/
ID/voice/cursor, partial progress and exact enables. Free/lifecycle/chain/IRQ/
provider domains/original deactivation/audio/native binding remain open.

Validation: read-only ISO manifest passed five spans,453 whole-body anchors,
seventeen calls. Five state/fifteen finite acyclic-chain models checked for
lookup/gates/unblock/state/retained fields/missing links/recursion/IRQ counts.
Link audit passed902 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical/source checks are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 stream free/frequency/mix and actual ID/state/voice/
companion/IRQ/providers and valid value/index/extent/alias/lifetime/concurrency/
stack/ABI/termination domains, transitive ARAM/DSP consumers and original
chained deactivation/reactivation/failure/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero activation companions/composition

Completed sndStreamActivate companion continuation and composed whole local
body; checked EVT-01 portion.
[Companions/composition](behavior/audio/MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md)
pins four stages before recursion, fresh companion/partial voice/state writes,
recursive return propagation and exact enables. Local source answered; chain/
IRQ/provider domains/original activation/audio/native binding remain open.

Validation: new read-only ISO manifest passed six spans,272 anchors,fourteen
calls. Prior primary manifest reverified; disjoint union392+14 external ABI
words,total406/658hex,and all body anchors covered. Fifteen finite acyclic-chain
models checked for success/failure/missing links/partial writes/recursion/counts.
Link audit passed899 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical/source checks are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELF/listing/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 stream free/deactivate/frequency/mix and companion
writers/acyclicity/ID/state/priority/voice/IRQ/providers and valid index/extent/
value/alias/lifetime/concurrency/stack/ABI domains, original chained activation/
failure/audio traces. Remaining macro/song/backend, lower-priority inbox/full game.

## 2026-10-13 — Inbox priority-zero stream activation primary

Completed sndStreamActivate primary lookup/voice/state/terminal-return regions
and ABI; checked EVT-01 portion.
[Primary activation](behavior/audio/MUSYX_STREAM_ACTIVATION_PRIMARY_ADMISSION.md)
pins64-row lookup/fresh state3, voice publication before failure, cursor/state1
stores and companion-sentinel return. Companion continuation/providers/IRQ/
domains/original activation/audio/native binding remain open.

Validation: read-only ISO manifest passed six spans,134 anchors,four calls.
Five lookup/seven primary/four slot vectors checked. Link audit passed897 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 sndStreamActivate companion continuation/recursive
requests/enables/return composition; actual state/ID/priority/voice/companion/IRQ
providers and valid value/index/extent/alias/lifetime/concurrency/ABI domains,
stream deactivation/free/frequency/mix and original activation/failure/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Original stationary candidacy and simulation paper facing

Committed eee81fa: [stationary NPC binding](behavior/field/NATIVE_STATIC_NPC_CANDIDACY.md)
adds portable visited-candidate transitions and binds original Chu3/Darkly
first-admission turn, retained yaw, skipped-gate ownership and next-pass null
interrupt consumption. The documentation agent recovered original
npcCheckInterrupt and verified REL raw null words plus absent relocations for
both exact source setup rows. Static loss posts secondary10000000; the next
flags1/2-admitted NPC pass preserves only secondary00000f00 and clears
state00001000 without restoring yaw or creating an event. Source profile
admission is scoped to room2 and exact area/name IDs525f654d/e4c10e3d,
so future areas reusing their character models do not inherit null scripts.
Full interrupt/event metadata/providers remain future work.

Native static NPC instances now own original yaw, message-driven pose clocks
and paper Main/Inner preparation in simulation; the renderer reads their
prepared matrix/pose clock without advancing retained facing on repeat draws.
Scripted Frankly retains his event owner. Original artwork, source poses,
message IDs/control tokens/attribution and room model batching through
citro3d/tex3ds are retained. Talk start respects source no-turn flags and
publishes its facing to the live instance. The shared candidate helper retains
existing Goomba stop/resume callbacks and source no-turn/restore distinctions.
Independent review caught angle-table rounding to360; candidate/talk headings
now normalize through source reviseAngle with a boundary regression. No new
story dialogue, branch fallback, story writes or replacement art/audio.
No asset conversion/licensing change; existing local asset ledger applies.

Validation: final shared `make test` passes141 CTest/103 Python,
`/tmp/npc-static-candidate-shared-test-latest.log`. Isolated source snapshot
`/tmp/pm-static-candidate-7a5rc5ru` with only owned implementation overlays
passes138 CTest/103 Python, `/tmp/npc-static-candidate-isolated-test-final.log`.
Tests distinguish first/repeated candidacy, wandering restore/start, static
loss/request/next-pass consumption, no-turn actors and normalized near-Z
heading. Original-data Darkly fixture settles the camera before approach,
then verifies saved/retained yaw, skipped keys, local loss and later consumption.
Original talk and existing Goomba/side-collision checks remain passing.
Shared and isolated `make all` pass without warnings/errors, logs
`/tmp/npc-static-candidate-shared-native-final.log` and
`/tmp/npc-static-candidate-isolated-native-final.log`. Intermediate shared
runs caught incomplete concurrent battle build registrations, missing pouch
test and a battle-drop memory assertion; the final141-test run supersedes them.
The isolated overlay excludes concurrent pouch edits. Link audit passes896
Markdown files/zero broken links; whitespace and emulator-script compilation pass.

Final isolated binary `artifacts/ttyd-static-candidate-final.3dsx` passes
`tests/azahar_static_npc_talk.py` in separate Azahar/Xvfb profiles/captures
`artifacts/azahar-static-candidate-chu-verified` and
`artifacts/azahar-static-candidate-darkly-verified`; logs
`/tmp/npc-static-candidate-chu-emulator-verified.log` and
`/tmp/npc-static-candidate-darkly-emulator-verified.log`. Corrected script
requires field HUD plus shaded Rogueport geometry before talk; Darkly requires
original contact approach. Window onset performs no save; return admits save,
retaining room2/story8, player contact bounds and checksum. Inspected actual
original actors, field and speaker-anchored windows. Earlier fixed-delay script
mistook title artwork for a window; those earlier green results are discarded.
Captures show60fps exploration and51..58fps dialogue with two simultaneous
software-emulator instances; these are smoke observations, not controlled
hardware/dense-area benchmarks or matched original-runtime visual parity.
ISO is read-only; binary packs/captures/profiles remain ignored, and user profile
and saves are untouched. Concurrent battle/pouch/audio work is preserved.

Next: original Goomba4 quest/switch/item-table/window binding, general actor
script metadata and interrupt replacement/regular lifetime, complete ordinary
talk return/cadence, original shared RNG/clock, additional original area actors/
events/objects and matched Dolphin/New2DS rendering/performance measurements.
The full requested remake goal remains active; this is an early Rogueport slice.

## 2026-10-13 — Inbox priority-zero stream allocation/length

Completed sndStreamAllocEx/sndStreamAllocLength/ABI helpers; checked EVT-01
portion. [Allocation/length](behavior/audio/MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md)
pins first free/collision ID, wrapped sizing, partial record/coefficients/mode/
filter/state3-before-buffer/deferred activation/state-only failure and IRQ order.
Child lifecycle/providers/domains/original allocation/audio/native binding open.

Validation: read-only ISO manifest passed11 spans,349 anchors,eight calls.
Thirteen length/six ID/four slot/five format/six completion vectors and16 copy
rows checked. Link audit passed896 Markdown files/zero broken links; scoped
`git diff --check` passed. Mathematical source deductions are separate from
original execution. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream activation/deactivation/free/frequency/mix,
actual callers/flags/count/ID/state/length/source/callback/voice/default/resource
providers and valid extent/value/encoding/alias/lifetime/IRQ/concurrency/ABI
domains, transitive ARAM/DSP consumers and original allocation/failure/stream/
audio traces. Remaining macro/song/backend, lower-priority inbox/full game open.

## 2026-10-13 — Inbox priority-zero stream footer/composition

Completed streamHandle footer/loop-PS/ABI and composed whole local source body;
checked EVT-01 portion. [Footer/composition](behavior/audio/MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md)
pins fresh gates/raw translated read/loop byte/helper fallthrough order. Whole
local handler answered; lifecycle/providers/domains/transitive ARAM/DSP/original
stream/audio/native binding remain open.

Validation: new read-only ISO manifest passed seven spans,44 anchors,three calls.
Four prior region manifests and ABI helper prerequisite reverified. Six disjoint
regions cover558 semantic+11 ABI words,total569/8e4hex. Seven gate/five source/
four leaf vectors and region composition checked. Link audit passed895 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution; address vectors do not prove valid
loads. Documentation/metadata only; no `make test`/`make all` rerun. Fresh full
original handler/leaf ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 stream allocation/activation/deactivation/free/frequency/
mix lifecycle; actual producer/delay/count/state/format/descriptor/position/cursor/
length/flags/source/selector/voice/loop-PS providers and valid value/divisor/extent/
alignment/encoding/address-map/cache/alias/lifetime/IRQ/concurrency/ABI/FP domains,
transitive ARAM/DSP consumers and original startup/refill/stream/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Inbox priority-zero stream nonzero-position refill

Completed streamHandle nonzero-position wrapped refill; checked EVT-01 portion.
[Nonzero wrap](behavior/audio/MUSYX_STREAM_NONZERO_POSITION_WRAPPED_REFILL.md)
pins two-span callback, strict returned-count split, down-versus-up block sizes,
zero-length second post, fresh source reads and uploads-before-cursor publication.
Footer/ABI/providers/domains/original refill/audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,152 region anchors,eight
calls; two indirect callback sites recorded. Six gate/fourteen ordered-refill
vectors checked, including equality/mutation/split/zero-size/bypass/invalid length
unresolved. Link audit passed894 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh full
original handler ELF/listing/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle common footer/ABI and whole-body
composition, actual callback/position/format/cursor/length/flags/source/selector/
voice/count providers and valid divisor/value/extent/alignment/encoding/alias/
lifetime/IRQ/concurrency domains, original wrapped-refill/stream/audio traces.
Remaining stream lifecycle/macro/song/backend, lower-priority inbox/full game open.

## 2026-10-13 — Inbox priority-zero stream zero-position refill

Completed streamHandle zero-position wrapped refill; checked EVT-01 portion.
[Zero-position refill](behavior/audio/MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md)
pins callback gates/fresh modulo, byte-versus-logical length distinction, retained
block offset and flush-before-cursor order. Nonzero wrap/footer/ABI/providers/
domains/original refill/audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,113 region anchors,four
calls; two indirect callback sites recorded. Six dispatch/five callback/nine
refill vectors checked, including mutation/length distinction/bypass/invalid
length unresolved. Link audit passed892 Markdown files/zero broken links;
scoped `git diff --check` passed. Mathematical source deductions are separate
from original execution. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh full original handler ELF/listing/generator stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded. Previously approved
push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle nonzero-position wrap/footer/ABI and
callback/position/format/cursor/logical-versus-byte-length/flags/source/selector/
voice/count providers and valid divisor/value/extent/alignment/encoding/alias/
lifetime/IRQ/concurrency domains, original refill/stream/audio traces. Remaining
stream lifecycle/macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Inbox priority-zero stream forward refill

Completed streamHandle position/forward refill region; checked EVT-01 portion.
[Forward refill](behavior/audio/MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md)
pins /14 position/block mapping, callback return/state gate, fresh modulo versus
retained pre-callback offset and flush-before-cursor publication. Wrapped refill/
footer/ABI/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,126 region anchors,five
calls; two indirect callback sites recorded. Nine block/seven route/six callback/
nine refill vectors and65540 division cases checked. Link audit passed890 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical source
checks are separate from original execution; invalid divisor output unresolved.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
handler ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Previously approved push remains pending
invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle wrapped-refill/footer/ABI, actual callback/
position/format/cursor/length/flags/source/selector/voice/count providers and valid
value/divisor/extent/alignment/encoding/alias/lifetime/IRQ/concurrency domains,
transitive ARAM/DSP consumers and original refill/stream/audio traces. Remaining
stream lifecycle/macro/song/backend, lower-priority inbox and full native/game open.

## 2026-10-13 — Inbox priority-zero stream startup

Completed streamHandle throttle/dispatch/state1 startup/row tail regions; checked
EVT-01 portion. [Startup](behavior/audio/MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md)
pins partial descriptor fields, ordered filter/pitch/mix/start, state2-before-
optional-flush and fresh count. Refill/ABI/providers/domains/original audio open.

Validation: read-only ISO manifest passed14 spans,151 region anchors,nine calls.
Five throttle/five state/four format/five finite pitch/four flush vectors and
exact descriptor/argument bindings checked. Link audit passed888 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 streamHandle state2 refill/ABI, actual delay/count/state/
format/descriptor/voice/frequency/filter/start/flags/source providers and valid
extent/value/alias/lifetime/IRQ/concurrency/FP domains, transitive DSP/ARAM/hardware
consumers and original startup/refill/stream/audio traces. Remaining stream
lifecycle/macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero output-mode propagation

Completed SoundSetOutputMode/sndOutputMode/streamOutputModeChanged/hwDisableHRTF;
checked EVT-01 portion. [Mode propagation](behavior/audio/MUSYX_STREAM_OUTPUT_MODE_PROPAGATION.md)
pins game write-before-request, public flags/HRTF/fresh change gate, voice dirty
before stream, requested/effective mode/state3 skip and live count/inline mix/IRQ
order. UI/providers/domains/stream engine/original audio/native binding open.

Validation: read-only ISO manifest passed nine spans,152 whole-body anchors,
eight calls. Eight public-mode/eight state/four count/six game-gate vectors
checked. Link audit passed881 Markdown files/zero broken links; scoped `git diff
--check` passed. Mathematical source deductions are separate from original
execution. Documentation/metadata only; no `make test`/`make all` rerun. Fresh
full original ELFs/listings/generator stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Previously approved push remains
pending invalid GitHub credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 streamHandle/AllocEx/Free/Activate/Deactivate/Frq/
MixParameter and actual game/UI/preference/flags/count/base/HRTF/state/voice/
resource providers and valid extent/value/alias/lifetime/IRQ/concurrency domains,
transitive DSP/hardware consumers and original mode/stream/audio traces.
Remaining macro/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Live original Goomba4, source pause ownership and NDSP footsteps

Committed live [Goomba4/wandering binding](behavior/field/NATIVE_LIVE_NPC_WANDER.md)
in acf058b. PMNPC008 retains original source talk-event presence separately
from playback, adds188-byte typed wander/pose/sound metadata and preserves
original tagged relative registry order. Source init(-370,0,130),direction270,
radius70/speed30,30/30 dimensions and40480603 regular flags remain exact.
Actor state owns retained work, previous root, previous NPCdt/contact, yaw,
automatic S_1/W_1/R_1 phase and simulation-owned paper presentation. Rendering
reads its live matrix/pose clock without a second facing update; model resources
still stage/batch through tex3ds/citro3d. Source fresh zero-vertical/no-gravity
profile takes the proven no-ground-probe path rather than inventing a floor ray.

Regular USER_FUNC executes before Mario using prior NPC publications; NPC-main
publishes current interval, applies direct displacement, clears components,
responds to Mario, chooses pose from actual speed and snapshots. Candidate
admission is independent of native quest playback and strict nearest victory;
fully admitted wandering actors pause even when not nearest. Stop/resume
retains work/countdown; skipped global/actor gates do not resume. Yaw turns
only on first candidacy, saves/restores source yaw without heading mutation.
Unknown nearest quest playback cannot redirect A to farther Chu/Darkly.
Original Goomba quest/flags/items remain unavailable: A jumping is suppressed,
no substitute dialogue/story write is generated, and a labeled development
notice explains missing playback. Source/native contracts and inbox retain
that boundary. Native seed1 and simulation60Hz clock remain explicit adapters
pending full shared seed/scheduler/clock binding.

Original c_kuribo_n descriptor0Z_1/S_1/T_1/W_1/R_1 uses14 cells,
1024x256RGBA8/UV4,1MiB nominal texture payload. Original step names request
source primary samples993/994 through libctru NDSP channels8/9; decoded
16000Hz monoPCM16 lengths540/672 and tablevolume45/127 are retained, with the
existing0.65 master gain. Named cue lookup uses the loaded original clip table.
Original name/mask/root requests remain inspectable; full macro/child voices,
material-mask routing and spatial attenuation remain adapters. No generated
replacement art/tones. ISO/raw resources/packs/captures remain ignored.

Validation: final shared `make test` passes134 CTest/103 Python at
`/tmp/live-wander-shared-test-complete.log`; isolated snapshot
`/tmp/pm-live-wander-4wc3cmxh` passes133 CTest/103 Python at
`/tmp/live-wander-isolated-test-complete.log`. Shared and isolated `make all`
pass without warnings/errors, logs `/tmp/live-wander-shared-native-complete.log`
and `/tmp/live-wander-isolated-native-complete.log`. Original-data tests verify
freshdt0/prior-publication movement, bounded1600tick walking, run/idle/sound
requests, retained pause/resume and skipped gates, first-turn retention, room
invalidation and unsupported-nearest/no-farther-dialogue with real message
providers. Parser tests reject incomplete moving profiles/malformed v8 data;
older formats and object support remain covered. Intermediate shared links
raced library rebuilds; final complete shared/isolated runs are the validation.
Independent source/runtime/render review caught repeated candidacy turning and
playback-winner mismatch; both corrected and regression-tested.

Final binary `artifacts/ttyd-live-wander-final.3dsx` passes isolated Azahar/Xvfb
`tests/azahar_live_wander.py` with goomba/goomba-observe fixtures from the fixture
tool. Profiles/captures `artifacts/azahar-live-wander-pause` and
`artifacts/azahar-live-wander-observe`; logs `/tmp/live-wander-emulator-pause.log`
and `/tmp/live-wander-emulator-observe-final.log`. Strict original-hat visibility/
motion assertion finds centers357.32,357.29,377.38,395.35 across captures;
stationary player, room2/story8, save admission/checksum and unavailable-quest
no-window assertion pass. Inspected original moving/paused art and development
notice; captures run60fps in software emulator (observed sim~1.0ms,CPU~7.0ms,
GPU~0.5ms). This is not New2DS hardware speed, full audio-runtime fidelity or
matched original-runtime pixel/trajectory measurement. Link audit passes867
Markdown files/zero broken links; whitespace passes. f371744 removes an
accidentally duplicated asset-ledger chapter, preserving the original foreign
entry. Concurrent battle/opening/audio work excluded from feature commits.

Next: bind Goomba4 quest/switch/item-table/window services before ordinary talk;
complete shared RNG/clock/event-category scheduling and all-story lifetimes;
static-NPC candidate turn/secondaryflag140 ownership; additional original area
NPC/events/objects and full sound/spatial/shadow/backend services. Compare
original traces and dense-area New2DS performance. Early Rogueport remains an
incremental remake slice; the full requested goal is still active.

## 2026-10-13 — Inbox priority-zero stream lookup/output mode/mix

Completed GetPrivateIndex/CheckOutputMode/SetupVolume/SetupVolumeAndPan/SetHWMix;
checked EVT-01 portion. [Lookup/mode/mix](behavior/audio/MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md)
pins64-row first-active-ID scan, requested/effective pan writes, exact unclamped
byte scaling and fixed studio0 hardware request. Stream engine/lifecycle/
providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed eight spans,157 whole-body anchors,
one call. Five lookup/64 individual-slot/six mode/four finite gain vectors checked.
Link audit passed880 Markdown files/zero broken links; scoped `git diff --check`
passed. Mathematical source deductions are separate from original execution.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 streamHandle/AllocEx/Free/Activate/Deactivate/Frq/
MixParameter/output-mode ownership, actual ID/state/voice/volume/mode/resource
providers and valid extent/value/alias/lifetime/IRQ/concurrency domains, transitive
DSP/hardware consumers and original stream/audio traces. Remaining macro/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision final pitch/composition

Completed LowPrecisionHandler output/requeue/ABI helpers and composed whole
local source body; checked EVT-01 portion.
[Final pitch/composition](behavior/audio/MUSYX_LOW_PRECISION_FINAL_PITCH_AND_COMPOSITION.md)
pins narrow lookup/fraction/wrapped addends/scaling, half212 before hardware,
requeue and complete GPR helper fallthrough. Whole local handler answered;
providers/domains/transitive consumers/original voice/audio/native binding open.

Validation: new read-only ISO manifest passed11 spans,53 anchors,seven calls.
Four prior region manifests reverified. Eight disjoint regions cover454 semantic
words plus11 anchored ABI words,total465/744hex. Six lookup/six doppler vectors
and bit-extraction/composition checks passed. Link audit passed879 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 actual voice/note/fraction/sample/ADSR/periodic/
controller/index/job providers and valid value/extent/alias/lifetime/IRQ/
concurrency/stack/ABI domains, transitive hardware/DSP services and original
pitch/voice/audio traces. Remaining macro/stream/song/backend, lower-priority
inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision portamento

Completed LowPrecisionHandler controller transitions/interpolation; checked
EVT-01 portion. [Portamento](behavior/audio/MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md)
pins unchanged-controller exception, strict threshold, raw init/flag/cache order,
wrapped signed step versus unsigned endpoint and elapsed-only completion.
Remaining handler/providers/domains/original glide/audio/native binding open.

Validation: read-only ISO manifest passed three spans,115 whole-region anchors,
one call. Twelve transition/eight interpolation vectors checked, including
threshold/ff/mode/zero-divisor/overshoot/wrap. Link audit passed877 Markdown files/
zero broken links; scoped `git diff --check` passed. Mathematical source deductions
are separate from original execution. Documentation/metadata only; no `make test`/
`make all` rerun. Fresh full original handler ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded. Previously
approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler final ADSR/pitch/scaling/hardware/
requeue/ABI and body composition; actual channel/set/controller/cache/mode/flags/
duration/current/time/note/voice providers and valid index/extent/value/alias/
lifetime/IRQ/concurrency domains, original glide/pitch/audio traces. Remaining
macro/stream/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero low-precision pitch preparation

Completed LowPrecisionHandler note/cents/bend/modulation region; checked EVT-01
portion. [Pitch preparation](behavior/audio/MUSYX_LOW_PRECISION_PITCH_PREPARATION.md)
pins retained note, signed base versus unsigned modulation cents, cached/query/
ff-skip bend and fresh scale flag/wrapped product/signed shift order.
Remaining handler/providers/domains/original pitch/audio/native binding open.

Validation: read-only ISO manifest passed four spans,96 whole-region anchors,
two calls. Seven base/six gate/five bend/six modulation vectors and512 byte
/100 cases checked. Link audit passed873 Markdown files/zero broken links;
scoped `git diff --check` passed. Mathematical source deductions are separate
from original execution. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh full original handler ELF/listing/generator stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded. Previously approved
push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler portamento/controller transition,
final pitch/ADSR/scaling/hardware/requeue/ABI, actual note/cents/cache/range/factor/
depth/oscillator/flag/controller/voice providers and valid value/index/extent/
alias/lifetime/IRQ/concurrency domains, original pitch/audio traces. Remaining
macro/stream/song/backend, lower-priority inbox and full native/game work open.

## 2026-10-13 — Inbox priority-zero integer sine lookup

Completed sndSin/original1024 numeric coefficients/two oscillator bindings;
checked EVT-01 portion. [Integer sine](behavior/audio/MUSYX_SINE_QUARTER_WAVE_LOOKUP.md)
pins low12 phase,1023 mirror/narrowed negation, bounded indices and source
peak4095. Runtime table/caller domains/remaining handler/original audio remain open.

Validation: read-only ISO manifest passed three spans,37 anchors (35 body/two
caller),two bindings. Eighteen boundary/high-bit vectors/all4096 phase checks and
numeric coefficients' big-endian serialization/hash passed. Link audit passed872
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution. Documentation/metadata
only; no `make test`/`make all` rerun. Fresh original ELF/listing/generator stay
`/tmp`; no binary bodies/table bytes/assets committed. Concurrent native edits
excluded. Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 LowPrecisionHandler pitch/controller/portamento/
requeue/ABI, actual table/phase/period/divisor/time/depth/dirty/flag/voice providers
and valid value/index/extent/alias/lifetime/IRQ/concurrency domains, original
oscillator/audio traces. Remaining macro/stream/song/backend, lower-priority inbox
and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision modulation/pan ramps

Completed LowPrecisionHandler oscillator/periodic/pan ramp region; checked
EVT-01 portion. [Modulation/pan ramps](behavior/audio/MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md)
pins wrapped unsigned phase requests, changed-half dirty OR, separately flagged
oscillator, distinct elapsed masks, periodic reset and raw-expiry versus clamped
continuing pan paths. Sine/handler remainder/providers/domains/runtime remain open.

Validation: read-only ISO manifest passed three spans,146 whole-region anchors,
two calls. Seven phase/five dirty/six periodic/eight pan-ramp source vectors
checked, including invalid-divisor outputs left unresolved. Link audit passed871
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source deductions are separate from original execution. Documentation/metadata
only; no `make test`/`make all` rerun. Fresh full original handler ELF/listing/
generator stay `/tmp`; no bodies/binary assets committed. Concurrent native edits
excluded. Previously approved push remains pending invalid GitHub credentials.

Next: priority-zero EVT-01 sndSin and LowPrecisionHandler pitch/controller/
portamento/requeue/ABI, actual phase/period/rate/countdown/target/dirty/flag/time/
voice providers and valid index/extent/value/divisor/alias/lifetime/IRQ/concurrency
domains, original oscillator/pan/audio traces. Remaining macro/stream/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset finish/composition

Completed ZeroOffsetHandler filter/priority/requeue/dirty/ABI finish and composed
whole local body; checked EVT-01 portion.
[Finish/composition](behavior/audio/MUSYX_ZERO_OFFSET_FILTER_REQUEUE_AND_COMPOSITION.md)
pins wrapped unsigned filter delta/conversion, priority first store/signed clamp,
raw wrapped requeue delay and post-requeue dirty replacement. Whole local source
body answered; providers/domains/transitive consumers/original audio remain open.

Validation: new read-only ISO manifest passed13 spans,106 anchors,nine call sites.
All four prior region manifests reverified. Union audit covers461 semantic+31 ABI
words,total492/7b0hex,without gaps/overlap. Six filter-gate/six filter-math/six
priority/six requeue/three dirty vectors checked. Link audit passed870 Markdown
files/zero broken links; scoped `git diff --check` passed. Mathematical/source
checks are separate from original execution. Documentation/metadata only; no
`make test`/`make all` rerun. Fresh full original handler ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials; no repeat
push attempted this turn.

Next: priority-zero EVT-01 LowPrecisionHandler remainder and actual voice/time/
gain/controller/filter/priority/dirty/index/source providers and valid value/
address/extent/alias/lifetime/IRQ/concurrency/ABI/FP/conversion domains, transitive
hardware/DSP/job consumers and original filter/requeue/voice/audio traces.
Remaining macro/stream/song/backend, lower-priority inbox and full native/game work.

## 2026-10-13 — Inbox priority-zero zero-offset volume/aux output

Completed ZeroOffsetHandler volume/aux output region; checked EVT-01 portion.
[Volume/aux output](behavior/audio/MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md)
pins dirty admission/six-call order, exact separate single main/pre/post/byte
terms, post-child fields and half454 publication before hardware ABI reads.
Filter/requeue/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed11 spans,97 whole-region anchors,six
calls. Eight admission/seven finite single-output vectors checked, including
zero main/nonzero AuxA and negative convertible snapshot. Link audit passed869
Markdown files/zero broken links; scoped `git diff --check` passed. Mathematical
source checks are separate from original execution. Documentation/metadata only;
no `make test`/`make all` rerun. Fresh full original handler ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.
Previously approved push remains pending invalid GitHub credentials; no repeat
push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler filter/requeue/epilogue and whole
body composition, LowPrecisionHandler remainder, actual gain/controller/byte/
studio/dirty/voice providers and valid value/extent/alias/lifetime/IRQ/concurrency/
FP/conversion domains, original output/audio traces. Remaining macro/stream/song/
backend, lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset pan/surround

Completed ZeroOffsetHandler pan/surround region; checked the EVT-01 portion.
[Pan/surround](behavior/audio/MUSYX_ZERO_OFFSET_PAN_AND_SURROUND.md) pins
captured/fresh mode, dirty admission, flag-before-request, post-child base reads,
wrapped signed pan versus unsigned surround clamps and fixed/clean indicator.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed four spans,82 whole-region instruction
anchors,two direct calls. Nine pan/eight surround/eight gate vectors and65536
low16 rotate-mask cases checked. Link audit passed868 Markdown files/zero broken
links; scoped `git diff --check` passed. Mathematical/source deductions are not
original execution. Documentation/metadata only; no `make test`/`make all` rerun.
Fresh full original handler ELF/listing/generator remain `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded. Previously approved push
remains pending invalid GitHub credentials; no repeat push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler volume/aux/filter/output/hardware/
requeue/epilogue and LowPrecisionHandler remainder, actual mode/dirty/voice/base/
controller providers and valid value/extent/alias/lifetime/IRQ/concurrency domains,
original spatial/audio traces. Remaining macro/stream/song/backend, lower-priority
inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset gain/attenuation

Completed ZeroOffsetHandler live fader/track/base gain and modulation/tremolo
attenuation region; checked EVT-01 portion.
[Gain/attenuation](behavior/audio/MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md)
pins row21/22 selection, unsigned conversions, post-child halfword reads,
signed half transform, exact single constants/step/reload and update order.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed six spans,128 whole-region instruction
anchors,two direct calls. Five selection/five gain/eight attenuation finite model
vectors and65536 signed16 half-transform cases checked. Link audit passed867
Markdown files/zero broken links; scoped `git diff --check` passed. Source checks
and mathematical deductions are separate from original execution. Documentation/
metadata only; no `make test`/`make all` rerun. Fresh full original handler ELF/
listing/generator remain `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded. Previously approved push remains pending invalid GitHub
credentials; no authentication changes or repeat push attempted this turn.

Next: priority-zero EVT-01 ZeroOffsetHandler pan/surround/aux/volume/hardware/
output/requeue and LowPrecisionHandler remainder, actual fader/track/voice/
controller providers and valid index/extent/value/alias/lifetime/IRQ/concurrency/
FP domains, original gain/audio traces. Remaining macro/stream/song/backend,
lower-priority inbox and full native/game work remain open.

## 2026-10-13 — Inbox priority-zero zero-offset admission/ramp

Completed ZeroOffsetHandler admission/timestamp/ramp region; checked the EVT-01
portion. [Admission/ramp](behavior/audio/MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md)
pins retained voice, low elapsed/shift, wrapped product before signed clamp,
fresh high-word gate clear, word158 publication and carried update indicator.
Remaining handler/providers/domains/original audio/native binding remain open.

Validation: read-only ISO manifest passed three spans,79 whole-region anchors,
one direct call. Four admission/five elapsed/fourteen ramp source-operation vectors
checked, including equality, zero step/rate and signed wrap. Link audit passed
866 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Fresh full original
handler ELF/listing/generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Approved push of all current commits attempted;
GitHub rejected configured credentials (invalid username/token). Push remains pending.

Next: priority-zero EVT-01 ZeroOffsetHandler remaining table/gain/controller/aux/
hardware/output/requeue, LowPrecisionHandler remainder, actual voice/time/rate/
target/flag/accumulator/IRQ/concurrency domains and original voice/audio traces;
remaining macro/stream/song/backend, lower-priority inbox and full native/game work.
Retry the authorized push once GitHub credentials work.

## 2026-10-13 — Inbox priority-zero cache syscall vector/installation

Completed SystemCallVector/__OSInitSystemCall/ICInvalidateRange/memcpy and
pinned direct OSInit call; checked the EVT-01 portion.
[Vector/installation](behavior/audio/GEKKO_CACHE_SYSCALL_VECTOR_AND_INSTALLATION.md)
pins28-byte source copy/256-byte cache lengths, temporary SPR1008 bit8/isync/
sync/restore/rfi, IC barrier/range and original overlap byte direction. Actual
boot/vector residency/exception/cache/hardware/original DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (four bodies/two linked),67
whole-body/caller anchors/four direct calls. Four SPR/six finite overlap-copy/
six IC range-barrier vectors and copy source/end/destination/hash/cache lengths
checked. Link audit passed864 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh ELFs/listings stay `/tmp`; no vector/bodies/binary assets
committed. No live vector/processor/emulator state modified. Concurrent native
edits excluded.

Next: priority-zero EVT-01 complete boot/vector/handler residency/patch owners,
original exception/MSR/SRR/HID0/address/privilege/cache/IRQ/ordering/trap/coherency
domains, actual copy/caller ranges/alias/lifetime/DSP consumers and original
syscall/cache/voice/audio traces. Remaining precision/macro/stream/song/backend
work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero cache ranges/source corrections

Completed five DC cache-range bodies; checked the EVT-01 portion.
[Cache/source corrections](behavior/audio/GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
pins wrapped line count/unconditional first loop/opcodes/post-loop sc. Corrected
six earlier contracts/four linked-body pins: e0 DCStoreRangeNoSync,ab4
DCFlushRangeNoSync,a54 DCFlushRange,a28 DCInvalidateRange. Prior prose names
were wrong; historical entries remain intact and this correction supersedes
those attributions. Syscall/hardware/domain/original cache/DSP/audio remain open.

Validation: new read-only ISO manifest passed five spans/five complete bodies,
57 whole-body instruction anchors/zero direct calls. Eleven range/zero/threshold/
wrap vectors and8224 small-domain count checks passed. All ten composed-builder
prerequisite manifests reverified, including corrected child pins; source region/
call/address evidence unchanged. Link audit passed863 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original syscall vector/handler and cache/address/
IRQ/coherency/ordering/trap/ownership domains, actual caller range/alias/lifetime/
partial publication/DSP consumers and original cache/voice/audio traces. Remaining
precision/macro/stream/song/backend work, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero startup finish/composed DSP builder

Completed startup loop/fallback endpoints/filter/volume seeds/start-slot/active
publication and composed the local whole salBuildCommandList body; checked the
EVT-01 portion. [Startup finish/composition](behavior/audio/MUSYX_DSP_STARTUP_ENDPOINTS_AND_COMMON_FINISH.md)
pins raw endpoint/default-register behavior and exact field/mode/publication
order. Providers/ABI/value/alias/IRQ/concurrency/cache/DSP/original audio/full
native binding remain open; local source composition is not runtime fidelity.

Validation: new read-only ISO manifest passed two spans (region/whole body),286
anchors (275 region/11 ABI),zero calls within new region. All ten prior region
manifests reverified against ISO. Union audit covers all2419 non-ABI instruction
words, plus11 anchored ABI words,total2430/25f8hex. Eight loop/six fallback/six
initial-enable vectors,nine seed bindings/all256 start-byte predicates checked.
Link audit passed861 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
fresh builder ELF/listing/coverage generator stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 descriptor/address/count/offset/loop/type/zero-buffer/
filter/mode/gain/start-slot/register/flag/list/PB/callback providers and valid
extent/residency/encoding/index/overflow/alias/lifetime/ABI/IRQ/concurrency domains;
transitive cache/DSP/command consumers and original startup/endpoints/voice/audio
traces. Remaining precision/macro/stream/song/backend work, lower-priority inbox
and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero startup callback/ITD/format prefix

Completed builder startup callback/ITD/type0..6 descriptor/direct/rounded-seek/
cursor prefix; checked the EVT-01 portion.
[Startup format prefix](behavior/audio/MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md)
pins callback return versus fresh length gate, original copy/request order,
wrapped seek rounding and unsupported type's stale/incoming-register fallthrough.
Loop/endpoints/finish/provider/domain/original sample/DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (one region/five linked),256
instruction anchors/five calls. Nine dispatch/seven rounded-seek/six initial-
cursor/five callback-length vectors and sixteen descriptor-copy binding rows
checked. Link audit passed859 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh builder ELF/listing stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 word8c/type-specific startup loop/endpoints and common
finish, actual type/callback/address/count/descriptor/offset/history/ITD/flag/
record/register-state providers and valid extent/residency/encoding/alias/lifetime/
IRQ/concurrency domains; cache/DSP consumers and original startup/seek/sample/
audio traces. Remaining precision work, lower-priority inbox/full native/full-game
remain open.

## 2026-10-13 — Inbox priority-zero existing voice endpoints/completion

Completed existing-voice type4/5/6 field/pending-buffer endpoint/completion
region; checked the EVT-01 portion.
[Existing endpoints/completion](behavior/audio/MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md)
pins captured/fresh type dispatch/latch/c2/raw wrapped address/count formulas and
independent unsigned progress/length/message-before-deactivate order. Startup/
provider/encoding/domain/original endpoint/DSP/audio execution remains open.

Validation: read-only ISO manifest passed four spans (one region/three linked),
72 instruction anchors/two calls. Eight endpoints/seven gates/seven completion
vectors and69638 unsigned high-multiply quotient/remainder14 cases checked;
algebra proves the quotient formula across unsigned32. Link audit passed857
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original fresh builder ELF/
listing stays `/tmp`; no bodies/binary assets committed. Concurrent native/staged
edits excluded.

Next: priority-zero EVT-01 startup/sample-format bodies, actual type/latch/PB.c2/
bytea0/address/count/word78/word8c/progress/length/callback/record writers and
valid encoding/index/overflow/residency/alias/lifetime/IRQ/concurrency domains;
cache/DSP consumers and original endpoint/completion/voice/audio traces. Remaining
precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP budget/rejection/voice chain

Completed three initial/cost/rejection/PB-chain/command02/03 gate regions;
checked the EVT-01 portion.
[Budget/rejection/chain](behavior/audio/MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md)
pins original cost tables/wrapped formulas/unsigned limit and current/tail/future-
studio depop/deactivation/callback ordering, fresh pointer/captured successor
choices and admitted PB-chain publication. Startup/sample/provider/domain/
original performance/DSP/audio remain open; system-word fixtures unmeasured.

Validation: read-only ISO manifest passed ten spans (three regions/five linked/
two numeric tables),197 instruction anchors/ten calls plus initial comparison.
Seven initial-budget/eight mixer-index/six SRC-index/six rejection/three chain
vectors checked. Link audit passed856 Markdown files/zero broken links; scoped
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 startup/sample/voice-format regions, actual D/system
word/flags/mode/type/count/new-setup/result/table/array/list/PB/callback writers
and valid index/overflow/member/cycle/alias/lifetime/IRQ/concurrency domains;
cache/DSP consumers and original cost/rejection/chain/voice/audio traces.
Remaining precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP studio payloads/fade bindings

Completed ten studio/input/aux/mode/surround/compressor/voice-chain-tail/fade/
loop regions; checked the EVT-01 portion.
[Studio payloads/fades](behavior/audio/MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md)
pins exact fields/raw indices/fresh frame pairing/mode gates/modulo/nine fades/
cache/outer-loop order. Compressor table fingerprint is identity only; remaining
voice/budget/admission/domain/cache/DSP/original audio remain open.

Validation: read-only ISO manifest passed14 spans (ten regions/whole-builder/
fade/cache/table),323 instruction anchors/11 calls. Six auxiliary-index/five raw-
frame/seven mode/six surround-gate/one compressor-layout vectors, nine fade
bindings and all256 auxiliary-byte quotient/modulo cases checked. Link audit
passed854 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original fresh
builder ELF/listing stays `/tmp`; no bodies/binary assets committed. Compressor
table stored only as hash identity. Concurrent native edits excluded.

Next: priority-zero EVT-01 preceding command03 admission, full voice/sample/
budget/command paths, actual studio/input/count/reference/frame/aux/mode/buffer/
coefficient/block/alias/lifetime/IRQ/concurrency domains, cache/DSP/table consumers
and original studio/input/aux/surround/compressor/fade/command/audio traces.
Remaining precision work, lower-priority inbox/full native/full-game remain open.

## 2026-10-13 — Inbox priority-zero DSP command rollover/finalization

Completed command-cell initialization/twelve rollover blocks/final command/cache
publication; checked the EVT-01 portion.
[Command rollover/finalization](behavior/audio/MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md)
pins unsigned fit/low16 aligned sizes, deferred previous-link size/flush, fresh
pointer publication and final0e/0f/cache order. Other payload/admission/budget/
domain/cache/DSP/original command/audio execution remain open.

Validation: read-only ISO manifest passed16 spans (fourteen regions/whole-builder/
cache child),533 instruction anchors/14 calls. Whole aligned body census found12
exact li0,0d rollover sites; twelve threshold/equality/wrap,seven size/three final-
command vectors checked. Link audit passed853 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 pending command payload/admission/budget/studio/voice
regions, actual pointer/cell/X/size/allocation/cache/DSP consumers and valid total
extent/wrap/alias/lifetime/IRQ/concurrency domains; original chunk/link/flush/
command/DSP/audio traces. Remaining precision work, lower-priority inbox and
full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP sorter/collection

Completed SortVoices and builder list collection/reverse iteration regions;
checked the EVT-01 portion.
[DSP sorting/collection](behavior/audio/MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md)
pins unsigned keys/wrapped midpoint/pivot stores/exact recursive order/unstable
ties, unchecked64-slot array and reverse consumption. Actual list/priority/index/
alias domains, budget/admission/original DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (sorter/two regions/three
linked),180 instruction anchors/seven calls plus reverse-load binding. Eight
identity/order/six midpoint vectors and3280 source-order mathematical model
cases checked for unsigned ascending/permutation invariants, lengths0..7/three
key values. Link audit passed852 Markdown files/zero broken links; scoped `git
diff --check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native/staged edits excluded.

Next: priority-zero EVT-01 actual priority/list/count/index/record/array writers,
valid signed range/overflow/member/cycle/duplicate/alias/lifetime/IRQ/concurrency
domains, surrounding builder budget/admission/cache/commands and original
collection/sort/voice/DSP/audio traces. Remaining precision work, lower-priority
inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero depop/fade/studio prepass

Completed HandleDepopVoice/DoDepopFade and builder studio two-list prepass;
checked the EVT-01 portion.
[Depop/fade/prepass](behavior/audio/MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md)
pins nine accumulator gates, wrapped-then-clamped addition/residual fade and
captured versus post-child successor/callback/sentinel/active/break/root order.
Fade callers/list providers/domain/original depop/DSP/audio remain open.

Validation: read-only ISO manifest passed six spans (two bodies/one region/three
linked),291 whole-body/region instruction anchors/four calls. Seven accumulation/
clamp,thirteen fade-boundary/residual,six admission/sentinel/deactivation vectors
and nine field bindings checked. Link audit passed850 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 fade callers/studio output/cache/DSP consumers,
surrounding builder admission/sorting/command/budget paths, actual accumulator/
PB sample/gate/latch/active/break/sentinel/root/link providers and valid studio/
list/member/cycle/index/value/alias/lifetime/IRQ/concurrency domains; original
list/depop/fade/callback/DSP/audio traces. Remaining precision work, lower-priority
inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero paired sound cosine/root helpers

Completed sndCos/sndSqrt; checked the EVT-01 portion.
[Paired cosine/root helpers](behavior/audio/MUSYX_PAIRED_COSINE_AND_SQRT_HELPERS.md)
pins original constants/paired reduction/signed conversion/Gray bits/fused
polynomial and estimate-before-zero/three double refinements. GQR/FPSCR/estimate/
caller-domain/original math/filter/audio execution remain open.

Validation: read-only ISO manifest passed five spans (two bodies/three numeric),
74 whole-body instruction anchors/zero calls. Nine ordinary phase/eight odd-Gray/
five synthetic-seed/five zero-gate vectors checked. Paired/scalar lane semantics
cross-checked against Dolphin's primary interpreter source (links in contract);
no emulator/original instruction execution claimed. Link audit passed849 Markdown
files/zero broken links; scoped `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Full original fresh ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original GQR0/FPSCR/scalar-paired lane/estimate/
rounding/trap/NaN/denormal execution, actual angle/frequency/coefficient/caller
value domains, filter/DSP command consumers and original math/filter/audio traces.
Remaining DSP/precision work, lower-priority inbox and full native/full-game work
remain open.

## 2026-10-13 — Original wandering-NPC driver core and lifecycle evidence

Implemented the [portable wander core](behavior/field/NATIVE_NPC_WANDER_CORE.md)
in commita8d09d6, compiled for host/3DS. It retains original44-byte work,
four phases, live frame conversion, RNG ordering/multiple20 countdown,
separate paper-facing readiness calls, strict radius/contact conditions,
minimum displacement cap, preserved components on early exits, idle yaw flips
without heading writes and exact sound threshold/toggle/FMA requests. Source
LCG and wrapped frame helpers consume caller-owned seed/rate, not an invented
private original trajectory. Original work allocation, event lifetime, roots,
NPC-main component consumption and platform providers remain separate owners.

The inbox yielded [full original driver/Goomba4 evidence](behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md):
initial XYZ(-370,0,130) overrides tagged placement; radius70/speed30; initflags
40080603 become40480603 through regular initialization. Source corrections
confirmed min(speed*dt,20*speed/1000), idle speed threshold0.1 and distinct
main00400000 versus auto-pose00000400. Fresh Goomba vertical0/no gravity takes
npcMoveG's zero exit before floor probes or Y writes. NPCdt begins0 and source
clock anchor begins current time. Regular event executes before Mario/NPC,
consuming prior dt/contact/Mario touch. Candidacy stops regular events beforeA,
including eligible non-nearest actors; candidate loss/resume preserves work,
phase/countdown rather than rerunning init. Full sound tail and original quest
branches before ordinary story text are recorded. Unknown quest state cannot
be treated as quest0 or replaced by generic dialogue.

Prepared original c_kuribo_n Z_1/S_1/T_1/W_1/R_1 locally through the existing
read-only converter. Ignored field-goomba manifest records14 cells,
1024x256RGBA8/UV4; raw/provenance/compiled resources stay ignored. Asset ledger
records paths, conversion and Nintendo/Intelligent Systems rights. This bank
is not yet registered as a live moving actor, and no stationary Goomba substitute
was added. The native core contract/inbox explicitly retains this boundary.

Validation: final shared `make test` passes130 CTest/103 Python at
`/tmp/npc-wander-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-wander-6f1t0umc` passes129 CTest/101 Python at
`/tmp/npc-wander-isolated-test-final.log`. Targeted core checks pass at
`/tmp/npc-wander-target-final.log`; final suite adds mixed readiness/three-versus-
four-query order, signed-byte sound toggles and cap equality. Shared/isolated
`make all` pass at `/tmp/npc-wander-shared-native.log` and
`/tmp/npc-wander-isolated-native.log`. Isolated build has no warnings/errors;
shared build has an unrelated concurrent trio_attack.cpp:365 indentation
warning. Concurrent battle/audio changes excluded from this scoped commit.
Independent source/disassembly/core review passed; suggested meaningful
ordering/byte/interface coverage was added. Link audit passes848 Markdown files,
zero broken links; whitespace passes. Static deductions are not Dolphin
observations; core not activated, so no new emulator/gameplay/hardware claim.
ISO read-only, no original executable bodies/assets/dialogue in source history.

Next concrete native step: instantiate source regular work per actor, run its
USER_FUNC before Mario with prior published dt/touch/map contact, apply current
NPC-main direct-horizontal/zero-vertical/side-response/automatic-pose services,
and expose live root/yaw/pose to existing rendering. Preserve original stopped
regular work on candidate/message transitions. Bind Goomba4 quest/flags/items
or explicitly leave that event unavailable; do not publish an assumed ordinary
message. Then use original bank/step-sound resources and verify native walking,
radius reversal, contact, pause/resume and rendering/performance with isolated
fixtures and matched original observations. Shared clock/RNG/backend/FPSCR,
all-story lifetime and broader event/NPC/object/area work remain open. The full
goal remains active; this is an incremental Rogueport foundation, not completion.

## 2026-10-13 — Inbox priority-zero filter coefficients/DSP parameters

Completed hwSetFilter/hwLowPassFrqToCoef and the builder's five parameter-copy
gates; checked the EVT-01 portion.
[Filter/DSP parameters](behavior/audio/MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md)
pins exact low-byte1 mode admission/raw active-byte preservation, unsigned/single
math child/conversion/output order and fresh ITD/SRC/polyphase/filter gates.
Math/FPSCR/domain/whole builder/command/original filter/DSP/audio remain open.

Validation: read-only ISO manifest passed13 spans (two bodies/three linked/one
region/seven numeric literals),80 anchors/two calls. Nine setter-state/seven
consumer-gate/five input-scaling/six synthetic final-clamp vectors checked. Link
audit passed847 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed. Concurrent
native edits excluded.

Next: priority-zero EVT-01 sndCos/sndSqrt/source math/FPSCR/transitive execution,
actual frequency/mode/coefficient/flag/record/PB/type/value/alias/lifetime/IRQ/
concurrency domains, surrounding builder setup/cache/command consumers and
original filter/parameter/DSP/audio traces. Remaining DSP/precision work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP volume-ramp consumers

Completed salBuildCommandList three-triplet prepare/correct/mask region;
checked the EVT-01 portion. [DSP ramp consumers](behavior/audio/MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md)
pins nine component bindings/dirty versus residual paths/fresh level-status
flag decisions/studio suppression/cross-triplet C dependency on B2. Remaining
builder/provider/domain/command execution and original ramp/DSP/audio stay open.

Validation: read-only ISO manifest passed four spans (one region/whole-builder/
two helpers),422 anchors/22 calls. Nine dirty-pair/nine residual-pair/four alternate-
C flag vectors, six instruction-decoded low-half clear masks/nine component
binding rows checked. Link audit passed846 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 surrounding DSP builder admission/list/budget/sample/
setup/cache/command regions, actual desired/host/PB flag/mode/mask/index/record/
alias/lifetime/IRQ/concurrency domains and original ramp/mask/command/DSP/audio
traces. Remaining precision work, lower-priority inbox and full native/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero IEEE mixer remainder

Completed __ieee754_fmod used by the mixer phase requests; checked the EVT-01
portion. [Original IEEE remainder](behavior/audio/IEEE_REMAINDER_USED_BY_MUSYX.md)
pins exceptional gate/double multiply-self-divide, finite exponent/significand
normalization, subtract/shift loop, normal/subnormal reconstruction and signed
zero. Original FPSCR/NaN/trap/runtime/mixer/DSP/audio evidence remains open.

Validation: read-only ISO manifest passed two spans (complete body/numeric signed-
zero table),94 anchors/zero calls. Twelve finite bit-pattern/seven exceptional-
gate vectors checked; deterministic4096 finite exact integer-rational versus
host math.fmod bit-pattern crosschecks passed. These are mathematical deductions,
not original instruction execution or exception/FPSCR experiments. Link audit
passed843 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELF/listing/crosscheck generator stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 original FPSCR/rounding/trap/NaN/denormal execution,
actual gain/coordinate/index/table/output/alias/lifetime/IRQ/concurrency domains,
DSP ramp/command consumers and original mixer/audio traces. Remaining DSP/
precision work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero mixer interpolation/output branches

Completed both salCalcVolume output regions and composed the whole local
mixer body with the prior preparation contract; checked the EVT-01 portion.
[Interpolation/outputs](behavior/audio/MUSYX_VOLUME_INTERPOLATION_AND_OUTPUT_BRANCHES.md)
pins original tables, exact single interpolation/products/nine-store order,
nonzero mode's ignored third gain/zero slots and cross-table extra pan pairing.
IEEE math/FP/index/alias/runtime domains, DSP consumers/original audio remain open.

Validation: read-only ISO manifest passed16 spans (whole body/two regions/four
linked/four numeric tables/five literals),337 anchors/19 calls. Eight binary32
finite/nonalias formula vectors checked, including paired E changes;266 original
volume/pan floats retained. Link audit passed842 Markdown files/zero broken
links; scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 IEEE remainder/math/FPSCR/transitive execution,
actual gain/coordinate/index/table/output/alias/lifetime/IRQ/concurrency domains,
DSP ramp/command consumers and original mixer/volume/DSP/audio traces. Remaining
DSP/precision work, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero mixer coordinate/phase preparation

Completed salCalcVolume coordinate/table/sentinel/compression/phase region and
fmod wrapper; checked the EVT-01 portion.
[Coordinate/phase preparation](behavior/audio/MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md)
pins low-byte table selection/raw sentinel/unsigned floor/single conversion,
optional pre-compression phases and common child-request order. Both table/output
branches and IEEE remainder/math/domain/original mixer/audio remain open.

Validation: read-only ISO manifest passed13 spans (one region/one wrapper/three
linked/two numeric tables/six literals),108 anchors/13 calls. Seven raw-coordinate/
sentinel/carry,six binary32 compression and six table-selection vectors checked;
258 original table floats retained. Link audit passed841 Markdown files/zero
broken links; scoped `git diff --check` passed. Documentation/metadata only; no
`make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 both mixer interpolation/output branches, original
pan-table/literal providers, IEEE remainder/math/FPSCR/transitive execution,
actual coordinate/volume/table/output/alias/lifetime/IRQ/concurrency domains and
original mixer/volume/DSP/audio traces. Remaining DSP/precision work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Original Darkly and reusable per-actor talk rules

Added source Darklyクラガリさん to the east-side early0..55 slice in
commit3b08537. Read-only tagged/REL/DOL evidence retains XYZ(-335,0,-335),
direction270,30/30 dimensions,flags0x40080603, drawmask1 and original
c_f_mura descriptor0Z_1/idleS_1/talkT_1.
[Source contract](behavior/field/ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md)
pins original static setup/null regular, lifetime census, asset provenance
and complete later talk thresholds/quest exception. Goomba4 was investigated
but requires original wandering/quest owners; no stationary replacement added.

[Native binding](behavior/field/NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md)
uses existing actor matrix/camera-facing, source pose/message clocks, dimensions,
collision and talk-ray services without a renderer character branch. Original
US gor_02_034_00 bytes,wait/key tokens and speaker attribution remain unchanged;
no story write. Darkly's later175 flag/name child remains unbound. Source
thresholds differ from Chu, so PMNPC007 adds per-actor source-order/full-ID
rules instead of reusing one global table. Current Chu keeps all nine original
ordinary requests; Darkly binds onlyLESS56. Empty/unmatched bindings do not
fallback or suppress A jump. Generic room prefixes and model metadata support
future area resources without model-index or gor_-only exporter coupling.
Legacy1..6 remain readable; staged parser rejects malformed counts/comparisons/
IDs/room namespaces/truncation. Rule tests include equality, greater/equal,
unconditional and duplicate first-match cases, plus future tik_04 namespace.

Original a/c_f_mura/a/c_f_mura- resources convert locally to13 wrap-aware124px
texture cells,1024x256RGBA8 atlas (1MiB),UV4 and original sparse hierarchy/
geometry/pass/cull. No generated replacement visuals/audio/dialogue. Source
positions and scalar2 remain separate from *.01 native mapping. Asset ledger
and documentation inbox record source paths/conversion/rights and remaining
quest/lifetime domains. Packs, manifests, raw assets and captures are ignored.

Validation: shared `make test` passes126 CTest/101 Python,
`/tmp/darkly-shared-test.log`; isolated snapshot `/tmp/pm-darkly-si2xzi8u`
passes125 CTest/101 Python, `/tmp/darkly-isolated-test.log`. Shared/isolated
`make all` pass without warnings/errors at `/tmp/darkly-shared-native.log`
and `/tmp/darkly-isolated-native.log`. Snapshot excludes foreign working edits;
count difference is concurrent test coverage outside that snapshot. Original
asset tests verify model/descriptor/idle/talk, metadata, contact and unchanged
message/story through closure. Initial side fixture used the wrong local stick
for the alley's source camera90; corrected approach uses right/world+Z and
retains original geometry/placement. Independent parser/model/source review
passed; its ordering/unknown-talk-admission followups were implemented.

Isolated Azahar/Xvfb `tests/azahar_darkly.py` with fixture from
`tools/make_field_actor_fixture.py --actor darkly` passes source contact at
native(-3.350000,-3.625000), visible original message, onset-save suppression,
return/save, unchanged room2/story8 and checksum. Binary `artifacts/ttyd-darkly.3dsx`,
profile/captures `artifacts/azahar-darkly`, log `/tmp/darkly-emulator.log`.
Inspected original model/room/contact/message/return captures; contact capture
shows60fps,simulation3.23ms/CPU6.7ms/GPU0.5ms in software emulator. This is not
New2DS hardware speed or matched original-runtime pixel proof. Link audit
passes839 Markdown files/zero broken links; whitespace passes. ISO read-only;
no assets/original prose committed, foreign battle/audio work preserved.

Next: original Goomba4 urouro/quest and other per-area source creation/event
owners; Darkly's later175 switch/name child and remaining lifetimes; original
NPC timer/movement/registry/auto-turn and full interaction/encounter composition.
Expand source rules only for verified one-print branches; complex events need
original state/service graphs. Continue inbox documentation/best-effort binding,
matched original trajectories/rendering and dense-area New2DS measurements.
The deliverable remains an early Rogueport slice, not a finished full remake.

## 2026-10-13 — Inbox priority-zero hardware pitch/volume/DSP ramps

Completed hwSetPitch/hwSetVolume/sal_setup_dspvol/salCheckVolErrorAndResetDelta;
checked the EVT-01 portion. [Pitch/volume/DSP ramps](behavior/audio/MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md)
pins duplicate/event-slot/dirty publication, float triplets/ITD table, signed
coarse division and residual correction/mask/reset order. Mixer internals,
DSP ramp callers/consumers and valid index/FP/alias/runtime domains remain open;
source constants do not establish measured cadence or audible playback.

Validation: read-only ISO manifest passed eight spans (four bodies/one linked/
two float literals/one numeric table),121 anchors/one call. Seven pitch/six
ramp/eleven correction/four triplet-gate/five ITD-index vectors and all131071
signed divide160 differences checked;128 ITD halves retained. Link audit passed840
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original inspection ELFs/
listings stay `/tmp`; no bodies/binary assets committed. Concurrent native edits
excluded.

Next: priority-zero EVT-01 full salCalcVolume/math/table providers, DSP ramp
callers/command consumers, actual pitch/pan/float/flag/marker/record/studio/time/
mask writers and valid index/FP/alias/lifetime/IRQ/concurrency domains; original
volume/pitch/ramp/DSP/audio traces. Remaining precision/builder work,
lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP initialization/activation

Completed salInitDspCtrl/salActivateStudio/salInitHRTFBuffer/salActivateVoice/
hwStart/hwKeyOff; checked the EVT-01 portion.
[DSP initialization/activation](behavior/audio/MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md)
pins allocation/partial failure/cache/record/studio bindings, unchecked per-voice
buffers, fresh count/base reads, head insertion/start time byte/indexed key-off.
Allocator/cache/DSP children and valid domain/original init/start/audio remain
open; source local requests do not prove successful audible playback.

Validation: read-only ISO manifest passed15 spans (six bodies/nine linked),305
anchors/33 calls. Six count/size,six activation/root,four indexed-event vectors,
eight studio slice offsets/seven explicit failure-stage cases checked. Link audit
passed839 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded.

Next: priority-zero EVT-01 allocator/ARAM/cache transitive behavior, full DSP
builder/consumer, actual count/base/time/studio/record/link/descriptor providers,
valid static extent/index/null/value/alias/partial-init/reinit/lifetime/IRQ/
concurrency domains and original init/allocation/start/key-off/DSP/audio traces.
Remaining precision-handler sections, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero DSP ADSR setup/command loop

Completed four salBuildCommandList setup/start/envelope-command regions and
three host-position/message/deactivation leaves; checked the EVT-01 portion.
[DSP ADSR loop](behavior/audio/MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md)
pins all seven local ADSR call sites, release precedence/mask/delta commands,
previous-result/repeated callback completion, flush range and retained playback
position/unlink order. Other builder/provider/consumer/domain/cadence/original
voice/DSP/audio execution remains open.

Validation: read-only ISO manifest passed14 spans (three bodies/four regions/
six linked/nine-halfword numeric table),309 anchors/18 calls and alternate index
source. Six loop/eight release-precedence/five delta/five host-position vectors
checked. Link audit passed838 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 DSP builder sample/mixer/budget/mask/index/record/
setup producers and branch admission, callback registration/transitive services,
cache/DSP command consumers, valid studio/list/stack/type/value/alias/lifetime/
IRQ/concurrency domains and original voice/envelope/command/audio traces.
Remaining precision-handler sections, lower-priority inbox and full native/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero low-precision ADSR caller/census

Completed four LowPrecisionHandler admission/clock/envelope/pitch/dirty
regions and the six-service DOL direct-call census; checked the EVT-01 portion.
[Caller/census](behavior/audio/MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md)
pins retained voice/elapsed low, stack outputs/status-bit clearing and independent
fresh descriptor-level pitch contribution. Fourteen direct calls identified;
seven DSP builder sites remain semantic work. Remaining precision/DSP sections,
indirect/REL/domain/cadence/original envelope/audio stay open.

Validation: read-only ISO manifest passed15 spans (four regions/nine linked
bodies/two nonempty DOL text sections),80 anchors/15 calls. Fresh opcode18/LK/
AA/displacement scan of all seven text slots found14 exact calls to six entries;
five elapsed/five flag/five signed pitch vectors checked. Link audit passed836
Markdown files/zero broken links; scoped `git diff --check` passed. Documentation/
metadata only; no `make test`/`make all` rerun. Full original inspection ELFs/
listings/census generator stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded.

Next: priority-zero EVT-01 DSP builder envelope setup/release/update/command
regions, remaining precision modulation/controller/portamento/pitch/requeue,
actual descriptor/flag/depth/time providers and valid value/alias/stack/IRQ/
concurrency/lifetime domains, indirect/REL callers and original voice/DSP/audio
traces. Lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Inbox priority-zero ADSR advancement/wrapper

Completed adsrHandle/adsrHandleLowPrecision for EVT-01; checked the inbox
portion. [Advancement/output order](behavior/audio/MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md)
pins two domains, pre-update level publication/fresh delta, count wrap/exact-zero
transition and fifteen-call output reuse/early return. Caller cadence/value/index/
alias/DSP/original envelope/audio remain open; no measured timing inferred.

Validation: read-only ISO manifest passed four spans (two complete bodies/one
linked state-change/one numeric table),83 anchors/three calls. Ten quantization,
five linear/count,seven log-index/five wrapper formula vectors checked;194 table
halves retained. Link audit passed834 Markdown files/zero broken links; scoped
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded. Authorized push of all committed
changes failed: GitHub rejected configured username/token; no push succeeded.

Next: priority-zero EVT-01 actual envelope callers/cadence and type/phase/count/
level/log/step/selector writers, valid table/value/alias/lifetime/IRQ/concurrency
domains, macro/hardware/DSP consumers and original output/audio traces. Other
opcodes, lower-priority inbox and full native/full-game work remain open.

## 2026-10-13 — Original NPC side collision and accelerated wall queries

Implemented [native ordinary NPC side collision](behavior/field/NATIVE_NPC_SIDE_COLLISION.md)
in commits74a28cf/c398f21. Portable source response preserves original
sphere/body admission, compAngle one-adjustment ties, finite binary32 source
polynomials, previous-XZ rollback/recheck, retained-travel40-degree branch,
source amplification/cap and required wall retry. PMNPC006 retains original
Chu0x80603/Frankly0x40080603 flags and source dimensions. Scripted creation,
explicit placement generations, continuous movement, flag OR/CLEAR ownership
and draw hiding remain distinct. Current contact/root/flags feed talk/render;
source8000 snap binding now reaches presentation100. Source Around saves the
uncapped first correction and admits negative/unordered second dot. Road case
checks follow NPC ownership. Source-derived inbox evidence appended to
[OW-INTERACT-01](requests/OW-INTERACT-01.md); no new Dolphin observations.

Instances reset by room/catalog generation; native mechanics remain independent
of3DS services. An initial unconditional engine/native position rescale caused
exact-coordinate tests to fail; corrected to publish MarioXZ only after actual
contact. Tests now cover original Chu walking contact, Professor collision
mask clears, invisible active bodies, deletion, explicit placement versus
continuous rollback and rejected-slot history. Original US message content,
IDs/control tokens and story state remain unchanged.

A warmed emulator run exposed contact slowdown: around retry visited4,816
static grid candidates. Cached expanded XYZ bounds now reject unreachable
triangles before the unchanged narrow phase, preserving source slot order,
filtering, cull, nearest/equal-hit and dynamic-object policy. HEAD serialized
prefix remains48 bytes; derived bounds stage/swap with geometry. gor_02 adds
82,800 cache bytes for3,450 triangles. Native margin is an acceleration adapter,
not measured original behavior. Five-room full-scan parity covers800 horizontal/
vertical/oblique queries plus16 contact rays and boundary/unsupported cases.

Validation: final shared `make test` passes126 CTest/101 Python at
`/tmp/npc-ray-bounds-shared-test.log`; isolated snapshot
`/tmp/pm-npc-collision-u6mshs4d` passes123 CTest/101 Python at
`/tmp/npc-ray-bounds-isolated-test.log`. Shared and isolated `make all` pass,
no warnings/errors, logs `/tmp/npc-ray-bounds-shared-native-final.log` and
`/tmp/npc-ray-bounds-isolated-native-final.log`. Independent numerical/history/
rendering/cache review found no blocker. Link audit passes833 Markdown files,
zero broken links; scoped whitespace checks pass. Difference in test counts
reflects concurrent additions outside the isolated source snapshot.

Isolated Azahar/Xvfb `tests/azahar_npc_collision.py` with original story8 native
fixture passes contact position(1.225000,-4.000000), unchanged room2/story8,
checksum, visible original message, onset save suppression and return/save.
Final binary `artifacts/ttyd-npc-collision-bounds.3dsx`, test-only profile/captures
`artifacts/azahar-npc-collision`, log `/tmp/npc-ray-bounds-emulator.log`.
Inspected contact/US talk/return captures retain original art and message window.
Warm baseline before bounds showed contact7fps and simulation47.42ms; final
contact capture shows60fps and simulation3.70ms in the software emulator.
Baseline captures remain `artifacts/azahar-npc-collision-baseline`; this is not
New2DS hardware performance or a matched original-runtime pixel measurement.
ISO read-only; extracted resources/packs/binaries/profiles/captures ignored.
Concurrent battle/opening/audio edits excluded from these scoped commits.

Next: extend original per-area NPC/event/object creation from source bindings;
full NPC timer/speed/gravity/pose/regular-event and alternate registry ownership;
transformed Mario dimensions and remaining retained-heading writers; complete
encounter/object/door/pipe arbitration. Verify matched original trajectories,
near-degenerate Gekko/SDK/backend numerics and dense-area New2DS performance.
Continue documentation inbox/best-effort bindings and incremental commits.
The current deliverable remains an early Rogueport slice, not the full remake.

## 2026-10-13 — Inbox priority-zero ADSR transitions/release

Completed salChangeADSRState/adsrStartRelease/adsrRelease for EVT-01; checked
the corresponding inbox portion.
[ADSR transitions/release](behavior/audio/MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md)
pins zero-stage fallthrough/two domains/count/step/table/terminal and explicit/
stored release. Linear-zero preserves level; converted-zero clears level/log.
Advancement/actual type/index/divisor/FP/alias/caller/runtime domains remain open;
no common zero handling/table repair or measured cadence inferred.

Validation: read-only ISO manifest passed nine spans (three bodies/one linked/
five numeric data),217 anchors/two calls. Five linear/five log-count/five release/
seven index-formula vectors and domain/mask/width/table/order deductions checked.
Link audit passed833 Markdown files/zero broken links; scoped `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 envelope advancement and actual type/phase/count/
level/log/step/duration/sustain/selector/caller writers, valid table/index/divisor/
float/value/alias/IRQ/concurrency domains, macro/hardware/DSP consumers and
original transition/release/audio traces; other opcodes/full native binding.
Lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero resource/pitch ADSR commands

Completed mcmdSetADSR/mcmdSetPitchADSR/adsrSetup and opcodes12/32 for
EVT-01; checked the corresponding inbox portion.
[Resource/pitch ADSR](behavior/audio/MUSYX_RESOURCE_AND_PITCH_ADSR_COMMANDS.md)
pins LE layouts/sentinel coefficients/single-FP operation order, signed command
pitch adjustment, sustain/time/release/setup and post-child flags. Setup wrapper
is closed; state engine/pow/actual resource/index/FP/caller/runtime remain open.

Validation: read-only ISO manifest passed19 spans (three bodies/two regions/six
linked/seven data/dispatch table),164 anchors/12 calls/two registrations. Seven pitch/
five LE/four index/four sentinel vectors, all256 signed fractional divisions and
byte/mask/FP/field/order deductions checked. Link audit passed832 Markdown files/
zero broken links; scoped `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 salChangeADSRState/pow/transitive math, other opcodes,
actual resource/command/volume/base-note/coefficient/descriptor/envelope/flag/
caller writers, valid length/type/index/FP/value/alias/shared-cell/IRQ/concurrency
domains and original ADSR/pitch/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller ADSR/hardware layouts

Completed mcmdSetADSRFromCtrl/hwSetADSR/adsrConvertTimeCents and opcode22
for EVT-01; checked the corresponding inbox portion.
[Controller ADSR/layouts](behavior/audio/MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md)
pins four fresh queries/stack descriptor, three hardware layouts/unsupported
control-bit publication and signed time-cent/pow/FP conversion wrapper. Original
129/129/1024-entry tables retained; unchecked input indices can exceed extents.
Other ADSR/pow/actual caller/index/float/DSP/runtime remain open.

Validation: read-only ISO manifest passed16 spans (three bodies/one region/three
linked/eight numeric data),122 anchors/11 calls/one registration. Seven index/
seven mode/four sustain vectors and descriptor/mask/width/table/order deductions
checked. Link audit passed829 Markdown files/zero broken links; scoped `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 ordinary/pitch ADSR and remaining opcodes; pow/
transitive math, actual controller/command/table/descriptor/mode/ordinal/flag/
caller providers, valid index/float/value/alias/shared-cell/IRQ/concurrency domains,
hardware/envelope/DSP consumers and original ADSR/controller/audio traces.
Full native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro pan/volume/envelopes

Completed pan/surround/scale/envelope/fade/sndConvert2Ms/curve lookup/comparator
and opcodes13..15/20..21 for EVT-01; checked the corresponding inbox portion.
[Pan/volume/envelopes](behavior/audio/MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md)
pins raw versus converted pan duration,127 versus128 scale/ramp arithmetic,
curve interpolation/sentinel and signed step/publication. Pre/post-child source
captures retained; no unified divisor/post-curve clamp or immediate job inferred.
Actual command/divisor/value/curve/mix/DSP/runtime domains remain open.

Validation: read-only ISO manifest passed19 spans (eight bodies/five regions/
five linked/table),210 anchors/19 calls/five registrations. Five pan/six scale/
four target/four curve/four step vectors and division/mask/width/address/order
checked as deductions. Link audit passed828 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual command/rate/pan/volume/
curve/registry/flag/caller writers, valid raw-versus-converted divisor/value/
alias/shared-cell/IRQ/concurrency domains and envelope/mix/job/DSP consumers;
original pan/fade/audio traces. Full native/runtime binding, lower-priority inbox/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero random notes/pitch lookup

Completed mcmdRandomKey/sndGetPitch/sndPitchUpOne/unsigned conversion helper
and opcode23 for EVT-01; checked the corresponding inbox portion.
[Random notes/pitch](behavior/audio/MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md)
pins interval/cents/note RNG/shared rewrite/publication/wait and single-precision
table/scale/divide/conversion. Distinct one-step/up-table constants and ignored
opcode23 wait return retained. Actual seed/float/table/command/runtime domains open.

Validation: read-only ISO manifest passed18 spans (four bodies/one region/six
linked/six numeric data),95 anchors/nine calls/one registration. Six interval/
five selection/five cents/six route/seven conversion vectors, all65536 cents
quotient cases and exact float tables/literals checked as deductions. Link audit
passed826 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual seed/RNG/command/note/
cent/reference/rate/table/caller producers, valid interval/table/float/divisor/
shared-cell/alias/IRQ/concurrency domains, pitch/DSP consumers and original
random-note/pitch/audio traces. Full native/runtime binding, lower-priority inbox/
full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro vibrato/portamento

Completed mcmdVibrato/mcmdPortamento/synthInitPortamento and opcodes27/28
for EVT-01; checked the corresponding inbox portion.
[Vibrato/portamento](behavior/audio/MUSYX_MACRO_VIBRATO_PORTAMENTO_AND_INITIALIZATION.md)
pins conversion/zero duration versus signed-byte/phase, controller65 write/query/
mode gates and initializer early-return/state stores. Mode2 does not clear400
on low controller; initializer skip still permits caller flag publication. Actual
command/provider/consumer/shared-cell/runtime domains remain open.

Validation: read-only ISO manifest passed ten spans (three bodies/two regions/
four linked/table),151 anchors/ten calls/two registrations. Eight vibrato/seven
mode/five initializer vectors and byte/mask/order deductions checked. Link audit
passed825 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual command/duration/rate/
amplitude/byte133/base132/flags/controller/caller producers, modulation/pitch/job
consumers and valid shared-cell/alias/IRQ/concurrency/initialization domains;
original vibrato/portamento/controller/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero hardware sample playback/pitch

Completed hwInitSamplePlayback/DoSetPitch/SRC/filter/ITD for EVT-01; checked
the corresponding inbox portion.
[Hardware playback/pitch](behavior/audio/MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md)
pins inclusive slot clear/preserved bit, full descriptor/padding copy, field/
SRC/filter/ITD writes and unsigned pitch normalization/strict backward table scan/
interpolation. Stable slot255 never ends; exact octave reaches pre-table reads.
Actual input/extent/alias/DSP/runtime remain open; no arithmetic repair inferred.

Validation: read-only ISO manifest passed nine spans (five bodies/three tables/
preceding halfword),146 anchors/three calls. Eight copy bindings, four finite slot/
nine pitch vectors and mask/width/divisor/table/address/order deductions checked.
Diagnostic stops are not original returns. Link audit passed823 Markdown files/
zero broken links; scoped `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 actual hardware pool/time-slot/descriptor/padding/
flags/SRC/ITD/pitch/caller providers, valid ordinal/table/ratio/divisor/preceding-
memory/alias/IRQ/concurrency domains and DSP/format/loading/consumer/original
playback/pitch/audio traces; other macro opcodes. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro sample start/resource lookup

Completed mcmdStartSample/dataGetSample/smpcmp and opcode16 for EVT-01;
checked the corresponding inbox portion.
[Sample start/lookup](behavior/audio/MUSYX_MACRO_SAMPLE_START_AND_RESOURCE_LOOKUP.md)
pins referenced-row search/shared scratch/ordered descriptor output, wrapping
mode offset/extent clamp, eight hardware arguments and fresh post-child voice/
pitch/flag/job publication. Length0 retainsffffffff offset. Actual residency/
descriptor/domain/hardware/pitch/runtime remain open; no fallback or guard inferred.

Validation: read-only ISO manifest passed11 spans (three bodies/one region/six
linked/table),138 anchors/eight calls/one registration. Eight output bindings,
seven offset/six extent/five flag vectors and comparator/unsigned division/address/
order deductions checked. Link audit passed821 Markdown files/zero broken links;
scoped `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 hwInitSamplePlayback/DoSetPitch and actual resident
registry/descriptor/ID/ref/output/command/word158/length/flags/metric/pitch/caller
providers, valid sorted/duplicate/member/zero-length/shared-cell/alias/IRQ/
concurrency domains and original start/pitch/sample/audio traces; other opcodes.
Full native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro subroutines/note commands

Completed mcmdGosub/mcmdAddKey/voiceIsLastStarted and opcodes24..26/36..37
for EVT-01; checked the corresponding inbox portion.
[Subroutines/notes](behavior/audio/MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md)
pins rotating push/raw return/miss cleanup, low16/S16 note clamp, route-specific
last-note gates and shared command rewrite/wait/result propagation. Actual ring/
note/table/command/caller/domains/runtime remain open; no index repair inferred.

Validation: read-only ISO manifest passed14 spans (three bodies/five regions/
five linked/table),162 anchors/13 calls/five registrations. Six push/four return/
six note/six predicate vectors and mask/address/order deductions checked. Link
audit passed820 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. Earlier authorized push failed GitHub username/
token authentication; no publication claimed. Latest objective removes periodic
pushes, so this delivery is committed locally.

Next: priority-zero EVT-01 other macro opcodes and actual ring/note/base131/132/
last-started/code/command/caller writers, valid count/index/slot/generation/table/
alias/shared-cell/IRQ/concurrency domains and original subroutine/note/wait/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero key-off/pedal/event traps

Completed macSetExternalKeyoff/macSetPedalState and opcode40/41 for EVT-01;
checked the corresponding inbox portion.
[Key-off/pedal/traps](behavior/audio/MUSYX_MACRO_KEYOFF_PEDAL_AND_EVENT_TRAPS.md)
pins low publication/high defer, event versus wake, full-word pedal and post-child
release clears, fresh shared trap install and selected clear/three-base enable
scan. Existing sample-end contract revalidated; no duplicate completion. Actual
flag/event/index/caller/shared-cell/runtime domains remain open.

Validation: read-only ISO new manifest passed seven spans (two bodies/two regions/
two linked/table),127 anchors/five calls/two registrations; existing notification
manifest16 spans/201 anchors revalidated. Six key-off/seven pedal/five address/
five displacement vectors and masks/order checked as deductions. Link audit
passed819 Markdown files/zero broken links; scoped `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded. User requested periodic upstream pushes;
commit/push result is verified separately after this delivery is committed.

Next: priority-zero EVT-01 actual flag/event/enable/command/code/caller/pedal/job
writers/consumers, valid event index/state/member/null/alias/shared-cell/reinit/
IRQ/concurrency domains; other opcodes and original key-off/pedal/sample/message/
audio traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero macro messages/VID/key groups

Completed mcmdSendMessage/mcmdGetVID/mcmdSetKeyGroup and opcodes42..44/89
for EVT-01; checked the corresponding inbox portion.
[Macro messages/VID/groups](behavior/audio/MUSYX_MACRO_MESSAGES_VID_AND_KEY_GROUP.md)
pins variable payload/macro-versus-VID/callback routing, queue/trap/pop/copy and
clear-own-group-before-key-off/kill order. Repeated mapped delivery/raw index
access before normalization/empty-zero receive retained. External key-off/callback/
actual command/queue/ID/group/domain/runtime remain open; no ring repair inferred.

Validation: read-only ISO manifest passed14 spans (three bodies/four regions/
six linked/table),183 anchors/11 calls/four registrations. Five enqueue/four
receive/six group/five index vectors and raw queue/mask/address/order deductions
checked. Link audit passed818 Markdown files/zero broken links; `git diff --check`
passed. Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 external key-off/callback provider, queue/trap/base/ID/
group/command/caller/count/pool writers and valid ring/slot/member/generation/
null/duplicate delivery/alias/IRQ/reentrancy/concurrency domains; other macro
opcodes and original message/activation/group/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Original harbor before-battle event execution

The ordered opening now continues from the original six harbor messages into
`gundan1000tai_beforebattle`, loaded from read-only US `gor.rel` with all132
commands and relocated source operands intact. Native bindings retain original
integer coordinate admission, fresh/resume branches, actor/speaker ownership,
022→023→024 messages, independent charges, camera requests, HP floor5 and
Crump setup2. Existing native movement is an explicitly limited presentation
adapter. The stationary200ms surprise jump uses the recovered normal binary32
recurrence, including same-call ascent/descent transition and floor admission;
its scoped verifier checks20 instruction anchors and five SDA constants.
Original event bytes/messages/poses remain local and ignored; provenance and
conversion choices are in [assets](ASSETS.md), [the encounter contract](behavior/story/NATIVE_HARBOR_BEFORE_BATTLE.md)
and [the scoped jump contract](behavior/field/FIELD_SCRIPTED_STATIONARY_JUMP.md).

Validation: isolated `/tmp/pm-harbor-continuation` `make test` passes105 CTest
and100 Python tests; `make all` builds the native3DS executable. Tests cover
signed integer poll edges, fresh/resume coordinates, original message order,
HP2→5/HP10 retention, stationary jump samples, and integrated story3→4 control
and save suppression. The original-source verifier passes against the read-only
ISO. Isolated Azahar `artifacts/azahar-harbor-fresh-chain` was inspected at the
original Goombella bubble and the pending battle boundary after actor charge.
No user profile/save was modified. Link audit724 Markdown files/0 broken links
and whitespace checks pass. An earlier resume smoke did not cross the X admission
threshold; it is not counted as successful native encounter validation.

Limits/next: the real tutorial battle-start service is unbound. The source VM
stays blocked at that call with a development notice, retains story4, suppresses
saving and never fabricates a battle result or dispatches the postbattle child.
Bind the original setup2/tutorial/return services before story5 and original
025..033 continuation. Original effect/audio request consumers, numeric NPC
movement, roll/horizontal jump branches and complete opening scene choreography
remain open. Keep the user goal active; the earlier title/files/opening adapter
plus this encounter are incremental playtest milestones, not a complete remake.

## 2026-10-13 — Inbox priority-zero RPN parameter writers

Completed inpSetRPNHi/Dec/Inc for EVT-01; checked the corresponding inbox portion.
[RPN writers](behavior/audio/MUSYX_RPN_PARAMETER_WRITERS_AND_VOICE_PROPAGATION.md)
pins selector routes/byte clamps and steps, high-word nine-preserved/eight-input
bits, equality-only word guards and distinct retained/fresh propagation. Boundary
cases still scan; step defaults can differ across matches under unsupported
mutation. Actual selector/caller/table/value/alias/runtime domains remain open.

Validation: read-only ISO manifest passed three complete spans,219 anchors/zero
calls. Six high-mask/seven byte/six whole-word/six selector vectors and packed-
address/rotate/scan-order deductions checked. Link audit passed815 Markdown files/
zero broken links; `git diff --check` passed. Documentation/metadata only; no
`make test`/`make all` rerun. Full original inspection ELFs/listings stay `/tmp`;
no bodies/binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcodes and actual selector/controller/
default/voice/count/caller producers/consumers, valid set/channel/packed-value/
address/matching/alias/reinit/IRQ/concurrency domains and original RPN/controller/
audio traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero MIDI setters and notification

Completed inpSetMidiCtrl/inpSetMidiCtrl14/synthKeyStateUpdate for EVT-01;
checked the corresponding inbox portion.
[MIDI setters](behavior/audio/MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md)
pins byte stores/change versus forced marker, RPN/default-before-store propagation,
matching fresh voice scans/job requests, MIDI aux dirty replacement and unclamped
14-bit pair order. FX special38 retains set255 MIDI/default formulas. RPN children/
actual caller/table/count/job domains/runtime remain open; no address repair inferred.

Validation: read-only ISO manifest passed nine spans (three complete/six linked),
213 anchors/18 calls. Eight store/six parameter/six pair/four address vectors and
mask/address/matching/order deductions checked. Link audit passed814 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 RPNHi/Dec/Inc and actual controller/selector/default/
voice/count/dirty/job/caller providers, valid table/voice/matching/alias/IRQ/
concurrency/reinit domains; other macro opcode paths and original pair/publication/
pedal/audio traces. Full native/runtime binding, lower-priority inbox/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero macro variable commands

Completed mcmdVarCalculation/mcmdIfVarCompare/inpSetExCtrl and opcodes96..101/
112..113 for EVT-01; checked the corresponding inbox portion.
[Variable commands](behavior/audio/MUSYX_MACRO_VARIABLE_CALCULATION_COMPARE_AND_SET.md)
pins S16 arithmetic/saturation versus signed32 comparison/inversion/PC, immediate
assignment and14-bit controller setter request order. Incoming result registers
for unsupported modes, fresh command reads/raw indices/reused channel CR retained.
Downstream MIDI setter/commands/domain/runtime and other opcode paths remain open.

Validation: read-only ISO manifest passed17 spans (three bodies/eight regions/
four linked/two tables),224 anchors/24 calls/eight registrations. Eleven arithmetic/
seven comparison/eight clamp/six index vectors and PPC comparator/rotate/mask/
address/order deductions checked. Link audit passed813 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 complete downstream MIDI controller setter, other
macro opcode paths and actual command/source/variable/controller/base/PC/incoming
register/reachable mode/domain writers, shared-cell/alias/IRQ/concurrency and
original calculation/comparison/controller/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Original ordinary NPC talk selection and retained owner order

Commit `0203759` binds [native ordinary talk selection](behavior/field/NATIVE_NPC_TALK_SELECTION.md)
to recovered source dimensions, Mario display/main flags, key-off admission,
live viewYaw versus rounded camera snapshot/display getter, inclusive vertical/
radius checks, center/+halfwidth/-halfwidth halfplanes, null-callback map
obstruction and strict nearest/first tie. Portable NpcTalkPlayer/NpcTalkScan
separate mechanics from native geometry/registry services. Source nearActor
r25 remains distinct from chosen candidate; native registry/auto-turn ownership
is not invented. Current static entries map source Entry flags3 and known area/
story lifetimes. Source20/37 dimensions cover current ordinary Mario only;
transformed dimensions/modes/party and nested-key/forced providers remain open.

The retained prior NPC candidate blocks following Mario A jump without a fresh
facing/radius scan. Later NPC scan publishes current candidate; A commits pending
selection and the next NPC invocation starts the original simple Mouser talk.
Wait/return clears without rescanning in that invocation. Talk yaw turns toward
Mario and catalog saved yaw restores after closure. Source US IDs/text/control
tokens/story branches remain unchanged; no story write added. Prompts consume
retained simulation state rather than running map rays while rendering. Catalog/
original angle-table staging is atomic. Resident source-winding head meshes,
spatial indices and dynamic object geometry are reused; null talk rays bypass
ordinary Mario wall attributes/ordinarySolid. ARM object inspection confirms
VFP float arithmetic and vsqrt.f32 for the ordinary finite norm, with fmaf/trig
library adapters preserving their stated numeric boundaries. Existing citro3d
material/pass/cull/texture rendering remains.

Delegated [side-response numerics](behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
and [talk null callback/display getter](behavior/field/FIELD_NPC_TALK_RAY_AND_DISPLAY_DIRECTION.md)
pin exact polynomial/FMA/cap/constants, direct NPC-main gates, null callback
policy, getter and next-invocation ordering. Earlier scope0 terminology was
corrected. [OW-INTERACT-01](requests/OW-INTERACT-01.md) remains open for full
registry/encounter/regular-event/auto-turn/collision/backend/runtime ownership.

Validation: final shared `make test` passes117 CTest/100 Python at
`/tmp/npc-talk-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-talk-qt8g5ba7` passes116 CTest/100 Python at
`/tmp/npc-talk-isolated-test-final.log`. Difference is the existing private
professor fence-reference test excluded from the snapshot. Shared and isolated
`make all` pass, no warnings/errors, logs `/tmp/npc-talk-shared-native-final.log`
and `/tmp/npc-talk-isolated-native-final.log`. Independent source/predicate/
geometry/order/render review passes; its identified live Mario flag gap was
fixed and covered. Tests include full original camera/head geometry, source
boundary/width/flag/nearest/nearActor vectors, retained candidate and pending/
return ordering, old out-of-range jump and an attributes4 polygon proving
filtered wall miss versus null-callback talk obstruction.

An initial emulator fixture failed to open a message despite passing weak
return/save assertions. Full-camera host warmup proved the authored view settles
from0 to180, reversing eligible world side. Corrected fixture is original
room2/story8 at native(1.2,-4); tests now warm source camera and explicitly
require a visible original message cloud plus no onset checkpoint write.
Final isolated binary `artifacts/ttyd-npc-talk-final.3dsx`, profile
`artifacts/azahar-npc-talk-cached`, script `tests/azahar_npc_source_presentation.py`
passes; inspected idle/talk/return show original NPC/window and restored field,
unchanged story/checksum and return/save assertions pass. Final emulator log
`/tmp/npc-talk-emulator-cached.log`; final image runs60fps in software emulator,
which does not prove New2DS XL speed or original-runtime pixel equality.
Rejected `artifacts/azahar-npc-talk` is not validation; corrected intermediate
`artifacts/azahar-npc-talk-final` also passes strict window assertions.
Documentation links pass812 Markdown files/zero broken links; whitespace passes.
ISO read-only, artifacts/compiled packs/profiles ignored; concurrent battle/audio
work preserved through scoped Git indices.

Next: bind original stationary NPC side response with the now-pinned polynomial/
FMA/quadrant/cap graph, active1+2/state10 and bypass8/20/4000 gates, previous-NPC-XZ
rollback and ordered Mario wall retry; preserve Mario→objects→NPC scheduling.
Then complete dynamic registry/auto-turn/regular events and source encounter/
door/pipe/A composition, expand original per-area NPC/event/object coverage,
finish display/material/shadow services and profile New2DS XL hardware. Full
remake and broader goal remain unfinished.

## 2026-10-13 — Inbox priority-zero macro metric/priority inline state

Completed macHandleActive opcodes48..57 for EVT-01; checked the corresponding
inbox portion. [Metric/priority inline state](behavior/audio/MUSYX_MACRO_METRIC_PRIORITY_AND_INLINE_STATE.md)
pins metric add/set/scaled-byte/hardware publication, priority signed16-sum clamp,
rate division/truncation and global/aux/tremolo/flag writes. Captured versus fresh
shared command reads retained; no generic saturation or measured age/time unit
inferred. Actual command/provider/domain/consumer/runtime and other opcodes open.

Validation: read-only ISO manifest passed13 spans (ten regions/two linked/table),
112 anchors/five calls/ten registrations. Seven metric-add/six priority-add/six
scale/six rate vectors and rotate/mask/address/order bindings checked as source
deductions. Link audit passed812 Markdown files/zero broken links; `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELF/listing stays `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 other macro opcode paths and actual commands/metric/
rate/priority/word158/global/aux/tremolo/flag producers/consumers, valid table/ID/
member/shared-cell/alias/IRQ/concurrency domains; original metric/priority/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero VID lifecycle and last-started reset

Completed vidInit/vidRemoveVoiceReferences/vidMakeRoot/vidGetPublicId and
voiceResetLastStarted for EVT-01; checked the corresponding inbox portion.
[VID lifecycle](behavior/audio/MUSYX_VID_LIFECYCLE_AND_LAST_STARTED_RESET.md)
pins112-node initialization, conditional table reset, non-root detach/root
promotion/terminal recycle and root/public-chain lookup. Exact repeated pointer/
identity reads and FX slot-index reset retained. Actual caller/graph/null/member/
identity/concurrency/runtime domains remain open; no null repair/atomicity inferred.

Validation: read-only ISO manifest passed five complete spans,252 anchors/one
call. Seven branch/six last-started vectors,112-node links/endpoints and global/
field addresses checked as source deductions. Link audit passed811 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 actual chain/node/last-started/identity/caller writers,
remaining macro opcodes/metric producers and valid null/equality/member/slot/
generation/sorted/cycle/alias/reinit/IRQ/concurrency/rollback domains; original
promotion/removal/reservation/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero voice block/unblock and allocation peek

Completed voiceBlock/voiceUnblock/voiceAllocatePeek for EVT-01; checked the
corresponding inbox portion. [Block/unblock/peek](behavior/audio/MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md)
pins fixed FX search, free/count bookkeeping, protected FX/group/synthetic
identity publication, raw unblock hardware/recycle/final pool reload and peek's
group-word output admission. Miss/free peek leaves output unwritten; raw ordinal
and low8 identity indexing remain distinct. VID/callers/domains/runtime open.

Validation: read-only ISO manifest passed nine spans (three complete/six linked),
180 anchors/ten calls. Seven counter/seven identity/four peek vectors and global/
table addresses checked as source deductions. Link audit passed810 Markdown
files/zero broken links; `git diff --check` passed. Documentation/metadata only;
no `make test`/`make all` rerun. Full original inspection ELFs/listings stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 VID reference removal and actual block/unblock/peek/
priority/metric/kind/identity/count callers/providers, valid raw ordinal/synthetic
ID/member/sorted/cycle/alias/reinit/IRQ/concurrency/accounting/rollback/stream/
resource domains; remaining macro opcodes and original reservation/free/audio
traces. Full native/runtime binding, lower-priority inbox/full-game work open.

## 2026-10-13 — Inbox priority-zero priority-list and voice-free writers

Completed voiceSetPriority/voiceFree/hwSetPriority for EVT-01; checked the
corresponding inbox portion. [Priority/free writers](behavior/audio/MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md)
pins early return/unlink/prepend/sort insertion, unconditional inactivity/free
append/wrapping counters and packed hardware priority memory publication.
Corrected reversed next/previous priority-sort labels in the earlier kill
contract using freshly inspected source; exact previous source stores retained.
Actual caller/metric/identity/member/hardware domains/runtime remain open.

Validation: read-only ISO manifest passed four spans (three complete/one linked),
150 anchors/two calls; existing kill manifest revalidated. Six packed-priority/
five admission/six counter vectors and sort/address bindings checked as source
deductions. Link audit passed809 Markdown files/zero broken links; `git diff
--check` passed. Documentation/metadata only; no `make test`/`make all` rerun.
Full original inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 actual priority/metric/kind/identity/caller providers,
other list/count writers and coherent slot/member/sorted/cycle/alias/reinit/IRQ/
concurrency/accounting/rollback domains; hardware/DSP consumers, remaining macro
opcodes and original allocation/free/replacement/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero allocation-aid initialization

Completed synthInitAllocationAids for EVT-01; checked the corresponding inbox
portion. [Aid initialization](behavior/audio/MUSYX_ALLOCATION_AID_INITIALIZATION.md)
pins four count reads, free-link/endpoints, membership-only/head reset and ordered
FX/root/Music writes. Zero endpoint count retains root0/marker255 and a store
before the free table. Valid counts/call order/other writers remain open; no
empty-root repair,64-slot guard or voice/resource cleanup inferred.

Validation: read-only ISO manifest passed one complete span,142 anchors/zero
calls. Eight stable-count/six free-row vectors and exact global/table addresses
checked as source deductions. Link audit passed808 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELF/listing stays `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from this commit.

Next: priority-zero EVT-01 voiceSetPriority/free/recycle writers and actual
count/caller/argument/limit/marker/metric/ID providers, valid slot/member/sorted/
cycle/alias/reinitialization/concurrency/accounting/rollback domains; remaining
macro opcodes and original initialization/allocation/audio traces. Full native/
runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero voice selection/allocation bookkeeping

Completed voiceAllocate/voiceAllocateFind for EVT-01; checked the corresponding
inbox portion. [Selection/allocation](behavior/audio/MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md)
pins limits/group counts/protection/type/priority/minimum-metric/free choices and
free-record unlink/wrapping running counters. Matching protected voices count
against group cap; no eviction/hardware/macro/VID cleanup occurs in these bodies.
Actual aid/priority/free/accounting providers and valid domains remain open;
no unlimited cap0, saturation or original replacement/audio policy inferred.

Validation: read-only ISO manifest passed four spans (two complete/two ABI
entries),168 anchors/three calls. Six limit/seven counter/seven group eligibility/
three metric/five cap vectors and exact table/global addresses checked as
source deductions. Link audit passed807 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 allocation-aid initialization/voiceSetPriority/free/
recycle writers and actual argument/limit/count/marker/metric/ID providers,
valid slot/member/sorted/cycle/alias/concurrency/accounting/rollback domains;
remaining macro opcodes and original allocation/audio traces. Full native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro start/priority/VID construction

Completed macStart/seqGetMIDIPriority/vidMakeNew for EVT-01; checked the
corresponding inbox portion. [Start/priority/VID](behavior/audio/MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md)
pins lookup/priority/allocation requests, old state/wait cleanup, pending fields/
compound ID, public VID construction and active publication/failure cleanup.
VID selector0 returns a fresh internal ID; nonzero returns public ID. Counter
advances before free-node admission. Allocator/priority/resource/free providers
remain linked/open; no complete rollback/lifetime or original playback inferred.

Validation: read-only ISO manifest passed15 spans (three complete/12 linked),
234 anchors/15 calls. Six key/five priority/four address/five compound/five state/
four flags/four counter/four return-mode vectors and24 field bindings checked
as deductions. Link audit passed805 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 voiceAllocate/voiceSetPriority/reference/free/resource
services and actual argument/registry/ID/VID/list/pending/flag/clock lifecycle,
valid slot/member/generation/sorted/alias/concurrency/rollback domains; remaining
macro opcodes and original activation/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro spawn/links/FX clone

Completed mcmdPlayMacro/synthFXCloneMidiSetup and opcode8 stub for EVT-01;
checked the corresponding inbox portion.
[Spawn/links/FX clone](behavior/audio/MUSYX_MACRO_SPAWN_LINKS_AND_FX_CONTROLLER_CLONE.md)
pins signed key clamp/raw16-argument request, temporary suppression/fixed clear,
public-node and predecessor/successor publication, fresh clone admission and
five limited controller requests. macStart itself remains linked/open. No
identity/lifetime/bounds/chain repair, prior-byte restoration, full clone or
implicit dirty invalidation is inferred.

Validation: read-only ISO manifest passed six spans (two complete/one stub/two
linked/one table),86 anchors/eight calls. Seven key/seven ID/three argument
vectors, clone controller/byte and suppression/chain order checked as deductions.
Link audit passed803 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 complete macStart/allocation/resource/VID constructors,
actual pending/ID/link/clone/suppression writers and identity/lifetime/alias/
cycle/concurrency/invalidation domains; remaining macro opcodes and original
spawn/clone/audio traces. Full native/runtime binding, lower-priority inbox and
full-game work remain open.

## 2026-10-13 — Original NPC descriptor0 fallback and collision/talk research

Commit `b5b1242` retains original animation descriptor0 in NPC subsets and
selects it on case-sensitive missing-name requests: Chu CHU_Z_1, Frankly Z_1,
including original lowercase tribe d_1. Canonical base/talk names reach retained
pose clocks before change detection, so two unknown names selecting one pose
preserve restart0; source message generations still force same-name Talk/Stay
restarts. Empty-bank rejection and map-object exact-name lookup remain. Packs
retain original material/pass/cull/UV/color data and existing citro3d submission;
source descriptor directory/provenance/rights are recorded in [assets](ASSETS.md)
and the [native contract](behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).
Numeric selectors, unexported valid source poses and repeated same-name scripted
Frankly restart requests remain separate coverage adapters.

Delegated documentation pins [NPC sphere/side response](behavior/field/FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md)
and [ordinary talk selection/lifecycle](behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md):
source vertical-segment query, inclusive side geometry/NPC rollback/Mario push/
wall retry; Mario20/37 dimensions, NPCEntry flags3, camera-relative center/width-
edge halfplanes, obstructing map ray, strict nearest/first tie, original A state
transition and Mario→objects→NPC update order. [OW-INTERACT-01](requests/OW-INTERACT-01.md)
remains open. These recovered predicates are not yet the native interaction
binding; the current .65-native-unit shortcut remains explicitly provisional.

Validation: final shared `make test` passes116 CTest/100 Python at
`/tmp/npc-fallback-shared-test-final.log`; isolated snapshot
`/tmp/pm-npc-fallback-gxc4y_oe` passes115 CTest/100 Python at
`/tmp/npc-fallback-isolated-test-final.log`. Difference is the existing private
professor fence-reference check excluded from the snapshot. Shared and isolated
`make all` pass at `/tmp/npc-fallback-shared-native.log` and
`/tmp/npc-fallback-isolated-native.log`, no warnings/errors. Independent read-only
fallback/cache/material review passes. Isolated binary
`artifacts/ttyd-npc-fallback.3dsx`, profile `artifacts/azahar-npc-fallback`,
original Mouser idle/talk/return script passes; inspected talk image shows the
original NPC and message window visible, and unchanged-story/checksum/save-return
assertions pass. Log `/tmp/npc-fallback-emulator.log`; this software emulator
check does not establish New2DS XL performance or original-runtime pixel parity.
Final documentation link audit passes801 Markdown files/zero broken links;
`git diff --check` passes. ISO read-only; assets/profile/build artifacts ignored;
concurrent battle/audio/source-history edits preserved through scoped indices.

Next: replace provisional ordinary NPC search with recovered motion/flag/
vertical/radius/directional/occlusion/nearest rules and preserved candidate
ownership across Mario/NPC update order; adapt fixture positions/facing to source
radius. Bind source stationary/dynamic side response after exact polynomial/wall
and caller domains, retain distinct partner scan. Then expand original per-area
NPC/event/object lifetimes, collision and dialogue branch coverage; complete
pre-display/material/shadow services and hardware profiling. Full remake and
broader goal remain unfinished.

## 2026-10-13 — Inbox priority-zero macro loop/jumps/registry lookup

Completed four loop/registry/binary-search/comparator bodies and three control-
flow regions for EVT-01; checked the corresponding inbox portion.
[Loop/jumps/lookup](behavior/audio/MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md)
pins shared countdown/random/gates/loop PC, header/scratch lookup and exact
conditional/direct miss/termination requests for six opcodes. Zero random divisor
is unchecked; binary lookup assumes valid sorted/provider domains. No private
loop stack, registry/code bound repair or full resource/free semantics inferred.

Validation: read-only ISO manifest passed14 spans (four complete/three regions/
six linked/one table),153 anchors/15 calls. Nine loop/five offset/seven bucket/
six search vectors/six registrations and comparator binding checked as deductions.
Link audit passed801 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macro opcodes, registry/assets/resource/
reference/free providers and actual command/counter/PC/flags/ID writers, valid
nonzero-divisor/sorted/member/code-range/alias/reentrancy/concurrency domains;
original loop/branch/termination/audio traces. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro wait/queue/time conversion

Completed five macro wait/queue/random/conversion local bodies and two opcode
stubs for EVT-01; checked the corresponding inbox portion.
[Wait/queue/time conversion](behavior/audio/MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md)
pins staged flags/early-return effects, random remainder/indefinite sentinel,
four exact clock bases, due-deadline publication and equal-before queue insertion.
Tick conversion retains division-before-multiply/wrap. Source clock units are
not converted into measured original milliseconds or renderer-frame counts.

Validation: read-only ISO manifest passed11 spans (five complete/two stubs/three
prior children/one table),185 anchors/nine calls. Six RNG/six conversion/five
rate/four base/five deadline/five queue/five remainder vectors checked as
deductions. Link audit passed798 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macro control-flow/opcode paths and actual
command/flag/seed/rate/index/clock/timestamp/list writers, nonzero-rate/member/
sorted/cycle/alias/concurrency/indefinite domains; original wait/random/queue/
event/audio traces. Full scheduler/native/runtime binding, lower-priority
inbox/full-game work remain open.

## 2026-10-13 — Original NPC facing, dimensions, talk clocks and surface culling

Commit `1e959c7` binds [ordinary NPC local presentation](behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md)
and records newly delegated [dimensions/anchor/display owners](behavior/display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).
Original event yaw is now separate from the NPC matrix: source angleABf table/
ordered camera correction, scale/pivot/Z-X-Y/root composition, Main sign then
Inner front/rear target/unwrap/per-draw fused0.1 smoothing and Z reflection replace
literal yaw-based mirroring. Fresh angle0/flags9 state is retained per room actor;
sign is published by Main, not invented as a source Entry+78 initializer. Host/
ARM SDK/trig/matrix rounding and complete original pass scheduling remain adapters.

PMNPC005 exports original model+c4 masks, tribe30/30 dimensions and CHU_T_1/T_1
names. Old1..4 remain readable; dimensions do not change geometry scalar2.
Original Chu/Frankly NPC talk anchors use root+height once, not mesh bbox/scale.
MessagePlayback now publishes source Talk/Stay generations and timestamps, and
host-testable NpcPoseClock preserves/restarts pose timing from simulation time,
including same-name requests and clock reset. Native ordinary Chu uses original
CHU_T_1 while talking. Explicit anchor overrides, other party/battle NPC anchors,
full NPC name/flags/pre-display/collision/arbitration and scripted pose owners
remain partial. No source dialogue/text/story branches are rewritten.

Original Chu/Frankly now retain source PMANIM4 RGBA, shape-cull/pass and UV/sampler
state via the existing surface compiler. Resource mask1 uses original special
pass2 admission and existing citro3d depth/alpha-test services. Correct facing
changes winding: an initial emulator capture exposed that older PMANIM3 fallback
had dropped original no-cull metadata and made Chu invisible. The final pack keeps
actual source no-cull indices/material data; the final NPC is visibly rendered.
Original texture124px conversion/sampler-aware gutters and source table/dimension
hashes/rights are recorded in [assets](ASSETS.md). Extracted/compiled artifacts
remain ignored; ISO read-only; no original executable payload is exported.

Validation: final shared `make test` passes116 CTest/100 Python at
`/tmp/npc-final-clock-shared.log`; final isolated snapshot
`/tmp/pm-npc-presentation-r_n1cotc` passes115 CTest/100 Python and `make all`, logs
`/tmp/npc-final-clock-isolated-test.log` and `/tmp/npc-final-clock-native.log`.
Only the existing private professor fence-reference check is absent in the
isolated snapshot. No native warnings/errors. Committed owned implementation
matches that tested source snapshot; shared renderer/platform changes were
staged as scoped NPC deltas so concurrent battle work remains preserved.
Original-data tests cover angle cardinal/quadrant/orientation/table migration,
Main-before-Inner/snap sign/camera call count, matrix scalar/root, source anchors,
pose persistence/same-name restart/message generation/reset, catalog1..5 atomic
rejection/original dimensions/talk poses, original shape no-cull/pass/color and
unchanged US story talk. The catalog upgrade initially broke a hidden-object
filter test's hardcoded96-byte model stride; schema-aware100-byte access restores
all three original hidden-name collision cases. An intermediate shared run failed
an in-progress unrelated trio-attack expected callback; final shared run above
passes. No unrelated source behavior was reverted to obtain that result.

Isolated Azahar final binary `artifacts/ttyd-npc-source-final.3dsx` and profile
`artifacts/azahar-npc-source-final` pass original east Mouser talk/return and
versioned checkpoint/story/checksum assertions with
`tests/azahar_npc_source_presentation.py`; log `/tmp/npc-final-emulator.log`.
Idle/talk/return images were inspected and show the source NPC, original message
and source-height bubble anchor. Earlier invisible capture in
`artifacts/azahar-npc-source` is rejected evidence; corrected surface and final
captures are the accepted verification. Warm captured emulator reports60fps,
simulation4.57ms/CPU5.6ms/GPU0.5ms; transient talk capture53fps is also visible.
These are emulator diagnostics, not New2DS XL hardware performance or original
runtime/pixel comparisons. Final link audit796 Markdown files/0 broken;
owned commit whitespace passes. User emulator profile/saves were not modified.

Next: bind source field NPC collision and A/X/B interaction priority using actual
inbox owners/dimensions, add remaining source actors/constructor-specific objects
and areas, recover wander/tavern fades before claiming those NPCs. Complete
pre-display/layers/color/blur/shadow/pose producers, case-sensitive missing-name
index0 fallback against original bank order, nonordinary anchor/override services,
full field/shared VM/pause/lifetime and matched original-runtime/hardware checks.
Continue source coverage from the documentation inbox; do not treat data packaging
or this local presentation milestone as complete NPC/full-game fidelity. The
broader user goal remains active and incomplete.

## 2026-10-13 — Inbox priority-zero active macro initialization/last-started

Completed the macHandleActive initialization prefix and two last-started voice
local contracts for EVT-01; checked the corresponding inbox portion.
[Active init/last-started](behavior/audio/MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md)
pins admission/break/pending/input/note/default/reset/flags/local-variable/clock/
job-start order and exact table setter/reset behavior. Initialization clears
known selector first-use flags before dispatch, but actual lifecycle/reinit
writers remain open. No whole-record reset, atomic timestamp, valid ID/table
bounds or original audible-start guarantee is inferred.

Validation: read-only ISO manifest passed13 spans (two complete/one region/ten
linked),244 anchors/ten calls. Six flag/five pending/four previous-note/six
setter vectors,128+64 table addresses and22 suffix fields checked as deductions.
Link audit passed797 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Full original
inspection ELFs/listings stay `/tmp`; no bodies/binary assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 remaining macHandleActive opcode paths/children and
actual pending/ID/note/channel/flag/PC/clock writers, last-started consumers and
valid bounds/alignment/alias/IRQ/concurrency/reinit/first-mode/r28 domains;
original initialization/selection/job/audio traces. Full native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro scheduler/scoped dispatch

Completed whole macHandle outer contract and four macHandleActive dispatch
regions for EVT-01; checked the corresponding inbox portion.
[Scheduler/scoped dispatch](behavior/audio/MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md)
pins wait/active/event/captured-next/deadline and macro-clock order, shared
command-budget fetch/PC publication,17 selector registrations and common join.
Normal unmodified counter admits32 commands; attempt33 returns before fetch.
Remaining active initialization/other opcodes are open; no whole-interpreter,
original execution or rendering-frame timing claim from a function fingerprint.

Validation: read-only ISO manifest passed29 spans (one complete body/four regions/
23 linked/one table),158 anchors/24 calls.17 registrations/six wait/six clock/11
opcode/five budget/five event vectors checked as deductions. Link audit passed796
Markdown files/zero broken links; `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Full original inspection ELFs/listing stay
`/tmp`; no bodies/binary assets committed. Concurrent native edits excluded.

Next: priority-zero EVT-01 complete remaining macHandleActive initialization/
opcodes/children and actual command/flag/list/debug/time writers; valid sorted/
member/cycle/alias/reentrancy/IRQ/concurrency/clock/first-mode/r28 domains and
original macro scheduling/audio traces. Full scheduler/native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero macro input selectors/first-use/dirty

Completed18 macro input selector/global dirty setter local contracts and four
mask tables for EVT-01; checked the corresponding inbox portion.
[Selectors/first-use/dirty](behavior/audio/MUSYX_MACRO_INPUT_SELECTORS_FIRST_USE_AND_DIRTY.md)
pins15 voice/two aux builder callers, conditional first-use mode0/reset,
separate signed coefficient decoding and post-builder dirty publication.
Existing selectors still use command mode; shared aux descriptors have per-voice
flags. This proves a conditional first-row/r28 chain, not global reachability/
flag lifecycle/shared ownership or original interpreter/audio timing.

Validation: read-only ISO manifest passed23 spans (18 complete/one prior builder/
four tables),480 anchors/19 calls.15 schemas/four first-use/four field/four global-
dirty/nine coefficient vectors, common voice-body regions and all65536 high-half/
all256 fraction-byte division cases checked as deductions. DOL scan found17
builder calls in these selectors. Link audit passed795 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from the commit.

Next: priority-zero EVT-01 interpreter/command and first-use-flag lifecycle,
shared aux ownership/other descriptor/dirty writers, actual caller/global first-
mode/r28/count/bounds/alias/concurrency domains; original selection/recompute/
audio traces. Full scheduler/native/runtime binding, lower-priority inbox and
full-game work remain open.

## 2026-10-13 — Inbox priority-zero descriptor builder/FX copy/translation

Completed inpAddCtrl/inpFXCopyCtrl/inpTranslateExCtrl local contracts and one
jump table for EVT-01; checked the corresponding inbox portion.
[Builder/copy/translation](behavior/audio/MUSYX_INPUT_DESCRIPTOR_BUILDER_FX_COPY_AND_TRANSLATION.md)
pins zero-low8-mode reset, four-row append cap, count-before-row publication,
variable flag/alias translation and ordered FX byte-copy routes. Builder can
admit nonzero first mode; it does not universally establish evaluator r28.
FX copy adds no dirty invalidation or full voice-ID guard. Actual callers must
supply those domains; no automatic recomputation or bounds repair inferred.

Validation: read-only ISO manifest passed six spans (three complete/two ABI
entries/one table),58 anchors/three calls. Nine builder/17 translation/256 copy-
route/five ID-row vectors and exact publication order checked as deductions.
Documentation link audit passed793 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Full original inspection ELFs/listings stay `/tmp`; no bodies/binary
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual builder/copy/translator callers and dirty
invalidation owners, other descriptor/controller writers, first-row/r28 and
FX bounds/lifetime/alias/concurrency domains; original build/copy/recompute/audio
traces. Full scheduler/native/runtime binding, lower-priority inbox/full-game
work remain open.

## 2026-10-13 — Inbox priority-zero matrix cache/aux/external wrappers

Completed17 matrix cache/aux/external-controller local contracts and three
tables for EVT-01; checked the corresponding inbox portion.
[Wrappers/dirty/cache/external](behavior/audio/MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
pins14 voice wrapper schemas, aux A/B shared masks, pre-evaluation dirty clears
and external remap dispatch versus original-index fallback. Wrappers do not
initialize r28 or restore dirty flags after the evaluator. No boolean result
conversion, matrix-special fallback substitution or bounds repair inferred.

Validation: read-only ISO manifest passed22 spans (17 complete/two prior children/
three tables),224 anchors/17 calls.14 schemas/eight masks/nine jump targets/nine
dispatch/16 dirty/five aux-address/five signed-half vectors checked as deductions.
Documentation link audit passed792 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/binary assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 descriptor construction/dirty/index/channel/set writers
and actual wrapper callers, incoming-r28/first-mode/count/numeric/bounds domains,
cache/voice/filter/aux consumers and original recompute/audio traces. Recover
inpAddCtrl/FX-copy/translation mutations next. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero input matrix/variable domain

Completed _GetInputValue/varGet local contracts for EVT-01 and checked the
corresponding inbox portion. [Matrix evaluator/domain](behavior/audio/MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md)
pins live descriptor count/source markers/wrapped coefficient and combine
arithmetic/cache and variable reads. The evaluator does not initialize incoming
r28 interpretation state; valid caller/first-row domains remain a requirement.
Recorded different first-mode2 residue results without assigning a provisional
flag default, widening integer arithmetic or inferring original audio timing.

Validation: read-only ISO manifest passed six spans (two complete/four linked),
191 anchors/six calls.256 sources/64 combines/three residue/eight scaling/six
elapsed/six variable vectors checked as deductions. Documentation link audit
passed791 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full
inspection ELFs/listings stay `/tmp`; no bodies/assets committed. Concurrent
native edits excluded from the documentation commit.

Next: priority-zero EVT-01 wrappers/descriptor construction/count/first-mode/
coefficient/index/flag writers and incoming-r28/reachable numeric domains,
external-controller child and voice/variable/clock/cache consumers; original
matrix/audio traces. Full scheduler/native/runtime binding, lower-priority
inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller/default/last-note reads

Completed three controller/default-pointer/last-note local contracts for EVT-01
and checked the corresponding inbox portion.
[Controller/getter reads](behavior/audio/MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md)
pins all256 controller routes, raw paired-byte OR, byte>=64 thresholds,
constant-zero RPN range and packed/default/last-note table getters. Only the
controller getter rejects channel255. Static cold/warm read compositions are
recorded without clamping raw bytes, inventing shared gates or claiming
original controller/audio timing.

Validation: read-only ISO manifest passed three complete spans/67 anchors,
including both threshold carry/mask sequences/no direct calls.256 routes/eight
pair/eight threshold/six single/six destination vectors checked as deductions.
Documentation link audit passed790 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 _GetInputValue/matrix/filter/dirty/last-note consumers,
actual read/reset/write callers and valid bounds/raw-byte/alias/concurrency
domains; original controller/audio traces. Full scheduler/native/runtime
binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero controller initialization/defaults

Completed four controller initialization/reset/default/last-note contracts and
two default-table interpretations for EVT-01; checked the corresponding inbox
portion. [Controller init/reset/defaults](behavior/audio/MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
pins58 per-voice stores, global aux-byte/dirty-word resets, common frequency
defaults, cold copy/warm255 preservation, last-note request and packed9-byte
channel defaults. Global inpInit does not reset all controller values; warm
reset stores only ten positions. No blanket clear, bounds repair, padded host
record or original filter/controller timing inferred.

Validation: read-only ISO manifest passed nine spans (four complete/three linked/
two tables),223 anchors/four calls.58 decoded stores/64 aux/128 dirty writes,
all134 classified bytes per table/six destination/five preserve vectors checked
as deductions. Documentation link audit passed789 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Original full inspection ELFs/listings stay `/tmp`; no bodies/
binary assets committed. Concurrent native edits excluded from the commit.

Next: priority-zero EVT-01 actual reset/init callers and controller/matrix/aux/
dirty/last-note/filter consumers/other writers, valid bounds/alignment/alias/
IRQ/reinit/concurrency domains and original controller/audio traces. Full
scheduler/native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Execute original NormalJump through first damage request

Mario's confirmed setup4 Normal Jump now executes the unchanged268-command
DOL stream in a native battle VM. Shared source-address unit/part/menu/candidate
storage feeds the recovered child, targeting, takeoff, approach, position,
MarioJumpParam and MarioJumpPosition services. Original empty datakey7 and
Majinai child events execute. Used facing/pose/declaration/AC setup callbacks
follow the new source contract; setup reset scope remains separate from TimingA
initialization. Fresh precheck and physical trigger history reach source timing
failure(flags100) or first success(flags20000). The live attack root is retained
at attackerU2a8 for the forthcoming reaction barrier. Source coordinator facing
is preserved when publishing the native battle registry.

Original b_mario supplies seven used poses and its own correctly bound battle
atlas. M_A_5 has a single source key over range0..2; import/loader now preserve
that constant pose without generating a terminal key. Original data/constant
sections are retained with live native manager/global ranges excluded. See
[native scope](behavior/battle/NATIVE_ORIGINAL_JUMP_RUNTIME_BINDING.md),
[used callbacks](behavior/battle/NORMAL_JUMP_USED_POSE_FACING_DECLARE_AND_AC_SETUP.md)
and [new reaction/death evidence](behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#original-used-hit-death-and-variant-lifecycle-2026-10-13-follow-up).

Validation: `make test` passed115 CTest/100 Python tests
(`/tmp/trio-attack-tests-final.log`), including actual source approach/airborne
motion, failed timing, single valid presses for all three enemy selections,
source per-hit flags/counter/HP/pose flags/attack-handle ownership, constant
original pose and Game retaining field position/story11/result0/control lock.
`make all` passed(`/tmp/trio-attack-all-final.log`); isolated debug-short build
passed(`/tmp/trio-attack-native-final.log`). Azahar helper
`tests/azahar_trio_jump.py` passed in
`artifacts/azahar-trio-original-jump-ready` with48second boot wait; manually
inspected battle scene and original Mario landing pose at the explicit damage
boundary. Earlier10second run was still black at boot and missed the input;
its later field-only captures do not verify the attack. Final coordinator-facing
preservation was host-tested/build-checked after the native capture. No original
execution, full attack/turn parity or hardware performance claim.

Limits/next: CheckDamage8010a78c remains genuinely waiting; no HP, turn, reaction,
battle result or story progress is fabricated. Camera/SE/voice requests are
retained without complete original providers. Unit entry/equipment/seed/private
pad/helper/prize/display clocks remain bounded adapters. Bind source damage/
counter/calculator/direct application with actual nonlethal/death/wing-fall and
Frankly warning event lifetimes, then bounce/tail/second hit, Hammer/Headbonk,
source move counts/enemy phases, rewards/end/result and field093/094/094_01/
story12. The prologue deliverable and full-game fidelity remain unfinished.


## 2026-10-13 — Inbox priority-zero virtual table provider/DOL census

Completed a scoped configuration-writer and DOL entry census for EVT-01 and
checked its inbox portion. [Provider/census](behavior/audio/MUSYX_VIRTUAL_TABLE_PROVIDER_AND_DOL_ENTRY_CENSUS.md)
consolidates known count/size/generation/callback writes,28 bounded literal-
address constructions owned by the five complete virtual bodies, and seven
exact entry calls. Those bodies supply no positive count/loop-size/nonzero
callback installation. Wider indirect/REL/pointer/alias providers remain open;
no original unused-feature/reachability/runtime conclusion is inferred.

Validation: bounded predicate scan and independent exact DOL direct-call decode
ran read-only. Manifest passed10 spans (two whole text segments/five prior bodies/
three callers),66 anchors/seven calls. Generic verifier checks bytes/hashes/
targets; scan completeness is limited to the explicitly documented predicate.
Documentation link audit passed788 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Scanner/generator stay `/tmp`; no original bodies/assets committed.
Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 wider indirect/REL/alias configuration/callback search,
actual transfer/result/provider contracts and original playback traces; continue
other open scheduler/audio/controller/source requirements. Full native/runtime
binding, lower-priority inbox/full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero virtual-sample refill

Completed the whole vsUpdateBuffer local contract for EVT-01 and checked the
corresponding inbox portion. [Refill descriptor/cursor](behavior/audio/MUSYX_VIRTUAL_SAMPLE_REFILL_DESCRIPTOR_AND_CURSOR.md)
pins type5/6 offset encoding, forward/tail/wrapped two-segment descriptor order,
six tag1 callback sites and nonzero-return advancement using fresh cursor/size
and a wrapped32-bit sum. Zero return adds no parent cursor write. No invented
callback-null/bounds repair, result cap or original byte-transfer/audio timing.

Validation: read-only ISO manifest passed one complete span/141 anchors,
including six indirect sites/no direct calls. Ten offset/13 stable descriptor/
nine post-callback vectors plus all65536 low-input type5 encoding cases checked
as deductions. Documentation link audit passed787 Markdown files/zero broken
links; `git diff --check` passed. Documentation/metadata only; no `make test`/
`make all` rerun. Original full inspection ELF/listing stays `/tmp`; no body/
assets committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual callback installation/implementation and byte
transfer, slot/cursor/type/size/result providers and valid range/nonzero-size/
alias/reentrancy/concurrency domains; original contiguous/wrapped/partial/
zero-return refill/audio traces. Full scheduler/native/runtime binding,
lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero virtual init/update/position

Completed five virtual initializer/updater/hardware position/state local
contracts for EVT-01 and checked the corresponding inbox portion.
[Virtual init/update/position](behavior/audio/MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md)
pins partial table reset,64-map scan independent of configured count, fresh
identity/state/position/remaining/signed-threshold/cleanup order and hardware
position type routes. Corrected the prior notification prose table base from
803f6b00 to80406b00; original instruction hashes/relative ordering were correct.
No whole-slot reset, position fallback or physical underrun timing inferred.

Validation: read-only ISO manifest passed12 spans (five complete/seven linked),
149 anchors/11 calls; prior notification manifest reverified.64 reset addresses/
six state/ten rounding/five remaining/six threshold/five startup/15 position
vectors plus all65536 low-input rounding cases checked as deductions.
Documentation link audit passed786 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 complete vsUpdateBuffer, table/count/size/callback/
remaining/type/pitch/hardware providers and valid bounds/generation/numeric/
reinit/concurrency domains; original refill/underrun/playback traces. Full
scheduler/native/runtime binding, lower-priority inbox/full-game work remain open.

## 2026-10-13 — Inbox priority-zero sample identity/hardware fields

Completed six sample-start child contracts for EVT-01 and checked the
corresponding inbox portion. [Identity/hardware fields](behavior/audio/MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md)
pins public-ID internal voice chains, active-only sequence/note association
with eight whole-list retries and exact hardware type/ID/extra/loop fields.
The sequence counter is not an eight-node cap and has no paused fallback;
lookup services add no voice identity revalidation. Loop setter reloads base
between raw stores. No original hardware output or association traces inferred.

Validation: read-only ISO manifest passed seven spans (six complete/one prior),
50 anchors/no calls. Eight index/eight width/seven stable-sequence/seven stable-
VID vectors checked as deductions, including ten-node first-pass association.
Documentation link audit passed783 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 actual voice/sequence/note/ID and hardware field
providers, backend consumers and bounds/generation/cycle/alias/concurrency
domains; original sample association/playback traces. Full scheduler/native/
runtime binding, lower-priority inbox and full-game work remain incomplete.

## 2026-10-13 — Live original save-block events, System graphics and asynchronous SD persistence

Commit `0675712` binds [live save blocks](behavior/field/NATIVE_LIVE_SAVE_BLOCK.md)
by original model identity/catalog source owner and name. Actual source bbox head
contact triggers flag8; per-instance S_1/A_1 clocks and source previous-published
ratio control the original stages. Native raw input/windows/events precede Mario;
object callback and animation publication follow Mario. Neutral-input physics
still lands Mario while nested source key/system ownership is active. Camera8
bit200 has a separate owner; field camera4 does not receive a follow-freeze guess.
The same catalog/registry path serves future rooms with source save-block records.

Original System pattern5, shape42..45/ring46..49 and selection index0 now draw
with original source dimensions/masks, four-quad/nine-slice geometry, source UV/
phase and PICA texture interpolation/alpha stages. Original type3 scissor and
no-tail routing, separate retained-message effect caches, original cursor1f8 and
independent choices are live. Source221-command event graph/US first-hit/dialogue/
branches are unchanged. Renderer samples per-instance raw X with source clip
starts/clamps and caches time as well as pose/tick; it never decides completion.
Cue431 now schedules the original sample550 on resident NDSP channel7. This is
an original primary-sample fallback; MusyX macro/3D/group parity is still unproven.
[Asset provenance](ASSETS.md) records unchanged texture/PCM conversion and rights.

Accepted writes use a libctru worker with an owned checkpoint/path,16KiB stack,
priority31/default permitted core, release/acquire completion and the existing
atomic v6 SD/temp/backup writer. No enqueues no write. Harness writes suppress
while source saving owns input, and exit autosave is limited to legacy harness
state without originalStory. Other harness autosaves remain adapters. Original
GC card/header/error classification, shared field VM/category/pause ownership,
optional custom source object programs and camera8 letterbox UI remain partial.
Native provider fault recovery is an adapter, not invented source behavior.

Validation: shared `make test` passes114 CTest/100 Python. Isolated source/romfs
snapshot `/tmp/pm-live-save-cp7b1_2f` passes113 CTest/100 Python and `make all`;
only the existing private professor fence-reference fixture check is excluded.
All committed implementation files match the validated isolated source snapshot.
Logs: `/tmp/live-save-shared-test.log`, `/tmp/live-save-isolated-test.log`,
`/tmp/live-save-native.log`, `/tmp/live-save-isolated-native.log`. Game tests use
actual jump/head physics, original first-hit/No/Yes windows and delayed real file
writes; they check landing, nested keys, camera separation, unchanged No files,
source switch persistence, restored S_1 and previous ratio at250ms. No compiler
warnings/errors were emitted by the isolated native build. Source clock samples
and test timestamps are adapter inputs, not measured original behavior.

Isolated Azahar binary `artifacts/ttyd-live-save.3dsx` passes repeat-hit No in
`artifacts/azahar-live-save-no2` (byte-identical checkpoint, no temp/backup), Yes
in `artifacts/azahar-live-save-yes` (original success window, atomic v6 file and
backup), and all three first-hit explanation pages followed by No in
`artifacts/azahar-live-save-first` (original flags0 file unchanged). Scripts are
`tests/azahar_save_block_no.py`, `tests/azahar_save_block_yes.py` and
`tests/azahar_save_block_first_hit.py`; fixtures remain ignored in artifacts.
All final choice/return/page/success images were inspected. First No smoke used
an input before the selection finished opening, so its file-only assertion did
not prove cancellation; inspected screenshots exposed that error and the final
scripts wait for the completed source choice. The final No captures show the
cursor on No and return to exploration. Sandboxed Xvfb failed its display socket;
scoped isolated-profile escalation completed the final runs without modifying
user emulator profiles/saves. Warm captured emulator reports60fps, simulation
2.47ms/CPU8.4ms/GPU0.5ms in the idle return; this is not hardware profiling or a
New2DS XL performance guarantee. Final link audit782 Markdown files/0 broken;
owned commit whitespace check passes. Original ISO stays read-only, and extracted
assets/compiled packs/binaries remain outside history. Concurrent battle/audio
research and pending shared edits are preserved through isolated scoped staging.

Next: ordinary NPC source pre-display/facing/tribe dimensions/talk-anchor services
and source interaction/collision arbitration, then remaining constructor-specific
objects and area catalog expansion. Keep using OW-INTERACT-01/other inbox contracts
rather than proximity or fabricated story. Verify source runtime/pixel/audio parity,
full field pause/category/shared VM/save state, narrow System frame cases and
source3D/macro sounds; profile actual New2DS XL hardware. Original S_1/A_1 assets,
source events and reusable live save infrastructure now work; the full-game/area
coverage and overall user goal remain active and incomplete.

## 2026-10-13 — Inbox priority-zero hardware message/sample notifications

Completed five hardware-message/sample notification local contracts for EVT-01
and checked the corresponding inbox portion.
[Message/start-end order](behavior/audio/MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
pins the installed four-message dispatch, hardware virtual-ID leaf, virtual
end matching/callback cleanup, macro sample-end restoration/wakeup and virtual
start reclaim/reserve/map/generation/payload/callback order. Callback return0
accepts a start. End cleanup reads fresh slot voice after callback; message0
checks full voice identity only after virtual end. No original audio timing,
callback mutation guarantee or post-callback identity repair is inferred.

Validation: read-only ISO manifest passed16 spans (five complete/11 linked),
201 anchors/20 calls. Nine message/eight ID/15 generation/eight wakeup/four
callback/five stable-slot vectors checked as deductions. Documentation link
audit passed782 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full
inspection listings/ELFs stay `/tmp`; no bodies/assets committed. Unrelated
concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 virtual table/map/count/counter/callback and voice/
event/flag providers, hardware/VID/sequence-instance children, actual callbacks
and bounds/alias/IRQ/reentrancy/concurrency domains; original start/end/refusal/
reuse/message/audio traces. Full scheduling/native/runtime binding, lower-
priority inbox and full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero synthesis initialization

Completed synthInit/synthSetMusicVolumeType/sndVolume/hwSetMesgCallback local
contracts for EVT-01 and checked the corresponding inbox portion.
[Initialization/type/jobs/callback](behavior/audio/MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md)
pins raw voice allocation/count and exact initial fields, partial fader resets,
row types/current overrides, aux/global/job resets, child request order and
callback publication. The type setter gates on sndActive but has no direct-row
bounds check. No universal fader reset, reinitialization repair or original
audio timing is inferred; linked children remain separate requirements.

Validation: read-only ISO manifest passed19 spans (four complete/13 linked/two
literals),279 anchors/13 calls.32 fader rows/96 job words/34 constant+three ordinal
voice fields/seven count/six setter/six reset-domain vectors checked as deductions.
Documentation link audit passed780 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Original full inspection listings/ELFs stay `/tmp`; no bodies/assets
committed. Concurrent native edits excluded from the documentation commit.

Next: priority-zero EVT-01 controller/allocation/voice/callback children, other
row/bitmap/aux/job writers and actual caller bounds/reinit/concurrency domains;
original initialization/scheduling/audio traces. Full native/runtime binding,
lower-priority inbox and full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero synthesis volume producer

Completed synthVolume/sndConvertMs source contracts for EVT-01 and checked
that inbox portion. [Selectors and fader publication](behavior/audio/MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md)
pins six row-type scans/direct indexing, four numeric literals, low16 duration
conversion, ordered fader/mode/ID stores, zero-duration sequence dispatch and
fresh bitmap OR. Zero duration preserves earlier start/coefficient fields yet
sets active; direct selectors beyond31 derive unchecked table writes. These
are static semantics, not original executed corruption or audible fade timing.

Validation: read-only ISO manifest passed11 spans (two complete/five linked/four
literals),192 anchors/15 calls.256 selector/eight duration/eight target/ten mode/
six row-type vectors checked as deductions. Documentation link audit passed779
Markdown files/zero broken links; `git diff --check` passed. Documentation/metadata
only; no `make test`/`make all` rerun. Original full listings/ELFs stay `/tmp`;
no bodies/assets committed. Unrelated concurrent native work excluded.

Next: priority-zero EVT-01 fader initialization/other bitmap/row writers,
caller group/ID/mode/time/numeric domains, sequence deferred/track consumers and
original duration/audio traces. Full scheduling/native/runtime binding and
lower-priority inbox/full-game work remain incomplete.

## 2026-10-13 — Inbox priority-zero sequence speed/volume

Completed four local sequence speed/volume/public-wrapper contracts for EVT-01
and checked their inbox portion. [Speed/volume/group forwarding](behavior/audio/MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md)
pins16 speed halfword stores, tagged flag/payload writes, immediate primary
volume request then64 fresh group comparisons without deduplication, four tagged
volume modes and raw public IRQ forwarding. Speed has no lookup-miss guard and
derives writes before seqInstance; volume returns on miss. No original audible
tempo/fade behavior is inferred from local field/request semantics.

Validation: read-only ISO manifest passed nine spans (four complete/five linked),
137 anchors/ten calls. Eight halfword/seven address/16 mode/15 flag vectors,
16 speed offsets and three64-byte group-order examples checked as deductions.
Documentation link audit passed777 Markdown files/zero broken links;
`git diff --check` passed. Documentation/metadata only; no `make test`/`make all`
rerun. Fresh original full listings/ELFs stay `/tmp`; no bodies/assets committed.

Next: priority-zero EVT-01 complete synthVolume and deferred/track consumers,
caller lookup-success/private-ID/group domains and original tempo/volume/miss/
audio traces. Full scheduling/native/runtime binding, lower-priority inbox work
and the full remake remain incomplete.

## 2026-10-13 — Inbox priority-zero sequence continue

Completed the local seqContinue/sndSeqContinue source contracts for EVT-01 and
checked their inbox portion. [Continue and missing-ID domain](behavior/audio/MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md)
pins paused→active publication, tagged pause-bit clearing and the raw public
IRQ wrapper. Continue has no lookup-miss guard: source arithmetic derives a
byte write at803eb432 before seqInstance. Runtime contents/execution are not
claimed; caller lookup-success/private-ID requirements remain open.

Validation: read-only ISO manifest passed four spans (two complete/two linked),
53 instruction anchors and three resolved calls; seven ID/tag/address, five
flag-clear and five state-route vectors checked. Documentation link audit
passed776 Markdown files/zero broken links; `git diff --check` passed.
Documentation/metadata only; no `make test`/`make all` rerun. Original full code
listings/ELFs stay `/tmp`; no original bodies/assets committed.

Next: continue priority-zero EVT-01 caller/private-ID, track/note/deferred-field
and audio provider contracts; original resume/missing-ID/audio traces and full
scheduler/native/runtime binding remain open. Lower-priority inbox work and
the full remake remain incomplete.

## 2026-10-13 — Original setup4 command and target menu

Bound a native command/target menu after Frankly's original battle introduction
and window closure. Source default Normal Jump/Normal Hammer/Headbonk weapon
IDs, admission/attribute masks and unchanged US menu/unit labels are imported
from the read-only ISO. PMBUNIT2 adds original local/hit/sampling geometry and
setup forward bias. The existing recovered sampler owns admission/occlusion:
Jump admits all three enemies, including Spiky Goomba; Hammer initially admits
only the front Goomba. Target refresh retains unit/part across row changes,
uses stored status27 rather than HP as death, wraps navigation and gives B
priority over A. Native D-pad/Circle Pad repeats and touch selection operate.
Owned views rebind after copies; confirmations retain actor/weapon/unit/part
without damage, move consumption or field resume. Partner Headbonk targeting
is host-tested but awaits the native turn scheduler. See
[native binding](behavior/battle/NATIVE_TRIO_COMMAND_TARGET_BINDING.md) and
[source follow-up](behavior/battle/UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md).

Validation: `make test` passed113 CTest/100 Python tests
(`/tmp/trio-commands-tests-final.log`). `make all` passed
(`/tmp/trio-commands-all-final.log`); isolated development build with
`DEBUG_SHORT_BOOT=1 BUILD=build-trio-battle TARGET=artifacts/ttyd-trio-battle`
passed (`/tmp/trio-commands-native-final.log`). Native smoke helper passed
`tests/azahar_trio_battle_targets.py` in
`artifacts/azahar-trio-battle-targets`; manually inspected Jump's three targets
with Spiky selected and Hammer's single Goomba. The target label received a
contrast panel afterward. Source tests additionally cover pair compaction,
HP-versus-status27, empty lists, cancellation, copied owners, retained weapon
IDs, physical/touch Game entry and partner targeting. Fixed an ARM int32_t
callback return mismatch in concurrent save-object work; that work stays
separate. No original execution/hardware/full combat parity claim.

Limits/next: confirmation ends at the explicit original-attack-pending notice;
B returns to inspect targets while dispatch remains unconsumed. Execute the
original Jump/Hammer/Headbonk event streams with motion/AC/hit/counter services
and source poses, then source move counts/phases, enemy actions, death/wing-fall
reactions, rewards/result publication and field093/094/094_01/story12. Keep the
inbox requests open for source death/event barriers and remaining composition.
Full playable prologue remains unfinished.






## 2026-10-13 — Priority0 sequence pause/mute/deferred field contracts

Delivered [pause/mute](behavior/audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md).
Three complete bodies pin inline lookup, state1 active→paused before note kill/
recycle, high-bit pause/mute deferred fields, raw mute words and public IRQ
forwarding. No timing/silence/private-index repair inferred. Marked local
EVT-01 portions complete; deferred consumers/provider/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),160 anchors/
five resolved calls passes.10 pause/6 mute/5 flag vectors and three-head order
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original execution.
Link audit775 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: ID/state/list/note/deferred-byte/word/mute-field providers/consumers,
resume/controller/voice scheduling, index/member/alias/IRQ/concurrency and original
pause/mute/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 synthHandle fade/aux region and local body closure

Delivered [fades/aux](behavior/audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).
Complete final region pins zero-offset gate, master/pause coefficient/bitmap
publication, mode requests/early exit and8-studio A→B/tag1/control-halfword
callbacks. Prior queue/voice/clock plus this contract closes local whole
synthHandle. Marked local portion complete; child/provider/runtime EVT-01 OPEN.

Validation: generic read-only verifier9 spans (region/6 linked/whole fingerprint/
zero literal),150 anchors/six resolved calls passes.6 modes/5 bitmap/16 studio-side
rows are source deductions, not original execution. Full listing/ELF remains
`/tmp`, source reverified. Link audit773 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: fade/bitmap/mode/coefficients and queue/voice/time providers, PPC/
alias/IRQ/concurrency domains, sequencePause/Mute/precision/mac/ADSR/aux children,
callback installation and original scheduling/fade/audio traces. Continue
priority0 source/backend then priority1 damage. No hardware/full-game completion.

## 2026-10-13 — Priority0 scoped synthesis job consumer and clock tail

Delivered [consumer/clock](behavior/audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md).
Two complete regions/no-op leaf pin category0→2→1 captured-next/marker/root
order, voice flag/pedal/hardware requests, fresh index advancement and64-bit
raw-input time addition. Whole synthHandle fingerprint does not close its
master-fade/aux middle. Marked local portions complete; parent EVT-01 OPEN.

Validation: generic read-only verifier13 spans (2 regions/1 leaf/9 linked/whole
fingerprint),163 anchors/10 direct calls passes.3 queue/5 index/5 clock vectors
and65536 pedal cases are derived checks; original code not executed. Full fresh
ELF/listing stays `/tmp`. Link audit772 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: synthHandle zero-offset/master-fade/aux middle, precision/zero/mac/
pedal/ADSR/hardware handlers, queue/voice/flags/time providers and member/alias/
IRQ/concurrency domains, original scheduling/audio traces. Continue priority0
source/backend then priority1 damage. No hardware/playback/full-game completion.

## 2026-10-13 — Retained original save windows and ticketed persistence services

Commit `bd61acc` adds [portable save presentation](behavior/field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md).
The original221-command graph now runs with real original global US message
payloads, retained main windows, separate source selection windows and nested
key ownership. Continue preserves cursor/schedule/key wait/identity rather than
force-closing; Add respects raw versus keyed operands. Window poll flags are
separate from playback data flags and survive ordinary waits/closing until
source deletion. Original system parser type/color and state7-to8 fallthrough
are implemented. No save dialogue is authored or rewritten.

Write requests carry one-use tickets and four source switches captured after
same-invocation VM writes. Simulation performs no filesystem I/O; an external
provider supplies delayed success0 or provisional generic error-1. Stale,
duplicate, untaken, finished and moved-from acknowledgements reject. Header's
one-update yield adapts combined native SD serialization; original card hardware,
classification, format/create/IPL and real worker binding remain absent. Missing
resources/unsupported operations fault and yield for owner recovery. Save v6
retains40 bytes/checksum, stores GSF1207/233/236 in byte34 and GSW4 in byte35,
and preserves legacy1..5 decode/reserved-byte checks. Live propagation is next.

Inbox documentation independently recovered camera8's
[letterbox decision lock](behavior/camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
and [original cue431](behavior/audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md).
Bit200 must not freeze native field camera4 following; it preserves camera8's
automatic letterbox choice while fades/draw continue. Original cue431 maps
FX618/group5/macro745/sample550; the ignored source WAV exists but is absent
from the current five-cue native fallback pack. Source runtime residence is
unproven; no generated sound replaces it.

Validation: final shared `make test` passes112 CTest/100 Python. Isolated source/
romfs snapshot `/tmp/pm-save-presentation-v2zjcjj3` passes111 CTest/100 Python and
`make all`; only the existing professor_fence_animation_original check is absent
because its private `artifacts/professor-fence-reference.bin` was not copied.
Logs: `/tmp/save-presentation-final-shared.log`,
`/tmp/save-presentation-final-isolated-test.log`,
`/tmp/save-presentation-final-isolated-native.log`. Real original-data tests cover
first-hit pages, raw scroll/stop, Yes/No/B cancellation, delayed success/failure,
retained main/choice ownership, nested keys, same-tick save-mode snapshot,
stale/duplicate/moved acknowledgements and window lifetime. Save tests cover all
three flag combinations, full mode bytes, v5 migration, older versions and
atomic file/backup recovery. An initial original first-hit test failed because
system parser control was unsupported; the documented type/color implementation
resolved it. Final Markdown link audit771 files/0 broken; owned whitespace checks
pass. No emulator/hardware capture is claimed: presentation remains unbound in
Game, so this milestone does not activate the block's live interaction. ISO
remains read-only and generated/extracted assets remain outside history.

Next: bind SaveBlockPresentation to live source catalog owner/name hit publication,
MapObjectState callback and independently timed S_1/A_1. Publish animation results
after object callback, use original clip starts, and key renderer cache by pose
plus per-instance time. Bind event processing before Mario, nested key/system
ownership while preserving falling physics, separate camera8 letterbox lock,
retained windows/choice drawing and an asynchronous libctru SD worker. Propagate
GSF233/236/GSW4 into progress/write snapshots; suppress automatic harness writes
while original save flow owns persistence and ensure No never writes. Then run
isolated Azahar head-hit/No/Yes/save-file checks and measured hardware profiling.
Other objects/NPCs/areas and complete source field/category/pause/resource/audio
ownership remain open. The full user goal remains active.

## 2026-10-13 — Priority0 synthesis job insertion and start timestamps

Delivered [jobs/start time](behavior/audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).
Two complete bodies pin32-slot arithmetic, distinct category duplicate/reschedule
rules, ordered node/root writes and two real-time captures before0/1 requests.
Unsupported categories have no guarded failure; source type precondition remains.
Marked local EVT-01 portions complete; queue consumers/runtime remain OPEN.

Validation: generic read-only verifiertwo spans/75 anchors/two direct calls passes.
21 slot/3 category/9 duplicate/two timestamp maps checked as deductions; original
code not executed. Full fresh ELFs/listings stay `/tmp`. Link audit771 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: queue/table/index/markers/time init and consumer advancement, category/
member/null/alias/IRQ/concurrency source domains, actual job/macro/voice scheduling
and original queue/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 macro active/inactive/wait/init lifecycle

Delivered [macro lifecycle](behavior/audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).
Four complete bodies pin state equality gates, active/time unlink, two-word wait
sentinels/clear/time and literal flags, activation's two-job request and live-count/
per-field pointer init. No scheduler/resource completion inferred. Marked local
EVT-01 portions complete; jobs/interpreter/source/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),125 anchors/
three resolved calls passes.6 wait/5 mask/7 state vectors and ordered job metadata
are deductions; no original code executes. Full fresh ELFs/listings stay `/tmp`.
Link audit768 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: synthAddJob/macro interpreter/wait-time producers, state/list/voice/
count/pointer providers and member/alias/IRQ/concurrency domains, original job/
macro/voice/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 stream kill/init and hardware break flags

Delivered [stream/hardware leaves](behavior/audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).
Four complete bodies pin64-record first-match kill/callback, configured-prefix
stream reset with fresh tail count and exact DSP break/active flags. Record
publication is separate from hardware/voice/audio completion. Marked local
EVT-01 portions complete; deeper providers/consumers/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),78 anchors/
one direct call passes.10 initialization/10 kill-route/16 break cases are derived
source checks. Full fresh ELFs/listings stay `/tmp`; no original code executes.
Link audit767 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: voiceUnblock/callback/stream-state/count providers, valid index/offset/
reset/concurrency domains, streamHandle/DSP consumers and original shutdown/audio
traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Original setup4 units, stage and Frankly battle introduction

[Native introduction](behavior/battle/NATIVE_SETUP4_BATTLE_INTRODUCTION.md)
imports original tik setup4 four-unit kind/part/group data: HP2 normal/spiked/
airborne Goombas, FranklyHP10, defense arrays and exact unit/part/counter flags.
Actual Frankly init15/entry7/phase39 commands run in a separate VM while the
original field scene remains suspended at battle_start. Source fresh lectureID6,
work0/maxmoves0,900msentry, counted45-step R_1 move, phase04000001/turn1 gate,
five original092_01..05 messages, status/camera requests and actor facing execute.
Native field-battle services publish sequence4/result0/controls-off; field
checkpoint/story stay unchanged. Full original scheduler/unit setup/player turns
are separate dependencies, not replaced by the Crump harness.

Imported original bti_01/stg_00_1 and source selected five object subtrees.
Recovered visibility excludes alternate A2/B geometry while revealingselected
isi1/isi2/B_mati descendants; default escape_wall/if_body/if_wire are hidden.
Authored ancestor matrices and source additionaltranslations remain; opaque/
translucent passes now filter separately. This fixes observed floor/beam occlusion
without invented actorY lifts. Source homes are Mario(-100,0,-5),Goombella
(-140,0,5); enemy positions retain grouprecords. Imported original PAD_Y_ON115
56x56icon; used atlas512x256/0.5MiB. Bound battle commonprint mode3 and used
one-shot toge selector1/mode0. Closing window continues after event completion.

Validation: final make test111 CTest/100Python and make all pass; separate
DEBUG_SHORT_BOOT artifacts/ttyd-trio-battle build passes. Tests check rawunits/
flags/stats, init/entry/firstturn/laterturn behavior, exact five messages,
status/camera/windowclose, retained field owner/control/result and checkpoint
coordinates. Final isolated Azahar artifacts/azahar-trio-battle-stage-final
captures original dialogue/cameras and complete intro; inspected cleanstage
shows allsix characters and explicit pendingplayercommands. Prior native capture
exposed whole-map visibility/pass occlusion and a generic dialogue fallback;
both were corrected. Initial092_05 parse failed for missing PAD_Y_ON, then used
source icon import fixed it. No user emulator profile/save touched.

Limits/next: bind original player target/Jump/Hammer/Headbonk graphs and private
pad/impact clocks, enemy turns, spike conditional092_06/07, airborne damage/fall,
result/loss/escape and source wait_battle_end return. Then execute original
093/094/094_01/story12 field continuation. Current introduction sequences
Frankly init→entry→phase; complete concurrent firstAct/phase scheduler, wait-event
and unit registries remain open. Stage spring scales/curtain/dynamic lifecycle,
full material/depth/lighting/shadows, camera defaults, effects/SFX and original
runtime traces remain partial. Source fixed-group RNG consumption and full
starting pouch/party setup are pending. Original setup3 lecture and ruins route
still required; fullprologue/hardware parity is not achieved. Goal remains active.

## 2026-10-13 — Priority0 sound-chain kill and voice allocator recycling

Delivered [sound/voice kill](behavior/audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
Two complete bodies pin external mapping/identity/captured successor, stale skip,
internal priority/free-list/count recycle and stream/hardware tail. Requested kill
and allocator state changes are separate from actual hardware/audio completion.
Marked local EVT-01 portions complete; child/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/4 linked),124 anchors/
five resolved direct calls passes.5 mask/4 counter/6 index/4 chain-route vectors
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original code
executes. Link audit766 Markdown files/0 broken; whitespace checks pass. Docs/
metadata only: no make rerun, native change or original body/asset export.
Isolated index excludes unrelated staged/worktree work.

Next/limits: map-reference/macro-inactive/stream/hardware children, identity/
priority/free/count providers and valid chain/index/member/IRQ/concurrency domains,
original note/voice recycle/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence initialization and free chains

Delivered [sequence free-chain init](behavior/audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md).
Complete seqInit pins empty active/paused roots,8-instance chain/IDs0..7/state0,
128 priority halfwordsffff,256 notes and next-ID0. Cross-record +18 writes are
next.previous within the20-byte note layout; payloads are not zeroed. Marked
local EVT-01 portion complete; constructors/voice/resource/runtime remain OPEN.

Validation: generic read-only verifierone body span/88 anchors/no calls passes.
Exact extents/chain metadata and emitted32*8 note-store simulation512 link
assertions/all writes within1400hex checked as deductions. Full fresh ELF/listing
stays `/tmp`; no original execution. Link audit763 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or
original body/asset export. Isolated index excludes unrelated shared work.

Next/limits: sequence constructors/ID/state/list/note/payload providers and MIDI
priority consumers, voice/resources/reset/concurrency and original initialization/
reuse/audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence validity and stop lifecycle

Delivered [sequence validity/stop](behavior/audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).
Four complete bodies pin active-before-paused key/private-ID lookup, validity
boolean, public IRQ wrapper and high-bit field-clear versus untagged state/note/
instance recycle. Source lacks private-index/unknown-state repair; no audible
validity guarantee. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (4 complete/5 linked),157 anchors/
8 resolved direct calls passes. Nine validity/five width/five state vectors and
original8-instance extent checked as deductions. Full fresh ELFs/listings stay
`/tmp`; no original execution. Link audit762 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: key/private-ID/state/instance/note/free/efc providers and valid index/
list/cycle/alias/concurrency domains, voice/resource/sequencer execution and
original validity/stop/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Source save lifecycle and original221-command event core

Commit `a0905a6` adds the portable source save-block callback, owned hit-name
cache and uint8 nested key leaves. The callback preserves exact stage0..4
request order, previous-published ratio/ordered NaN comparison, optional versus
system children, entry priority30/type26, distinct running predicates and
retained eventID. These mechanics are compiled for host/ARM and do not yet
change the Game's live save-block interaction.

Original system/wrapper/common-child arrays retain221 commands/14 callback
identities/89 sites in ignored local packs. The builder validates exact source
hashes and reads only DOL data spans/operand strings plus actual label-scanner
lookahead words. No original executable body is exported or dialogue invented.
The existing interpreter executes the original child/branch/switch/flag graph;
typed services preserve raw/resolved arguments, source speaker/coordinates,
ID-versus-raw controls, firstCall/yielding semantics and output references.
See [native core](behavior/field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md)
and [asset provenance](ASSETS.md).

Documentation inbox work recovered
[restart/event identities](behavior/display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md)
and [retained Continue/Add ownership](behavior/message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md).
Continue resumes/polls, not force-closes; raw scroll/once_stop appends preserve
the retained main window while selection has its own handle. Actual native
message/card providers remain next; test callbacks are not presentation/I/O.

Validation: final shared `make test` passes110 CTest/100 Python tests. Isolated
source/romfs snapshot `/tmp/pm-save-flow-jikuwdrm` passes107 CTest/100 Python
and `make all` after final request/move changes; concurrent underground work
is excluded from that snapshot. Original-data tests check No/success flags and
mode/story effects, exact ID/raw-control order, source attribution/arguments,
yielding header/write, all normal-mode error families, accepted format/create,
format/create/write failure repolls, guard break and moved runtime ownership.
Lifecycle tests cover suppression, callback stage/order, nested key wrap, NaN,
optional children and helper-versus-wait predicates. An initial test backend
accepted every later creation choice while retaining a persistent error and
entered the source retry loop; the owned process was stopped and test responses
corrected. Source loop was preserved. An earlier shared run failed a concurrent
unfinished underground test syntax error; the final shared run above passes.
Documentation links758 files/0 broken and whitespace checks pass. No emulator
run is claimed for this library-only milestone; current rendering is unchanged.
ISO remains read-only and compiled packs/assets remain outside history.

Next: bind actual source instances/head-name publication, independently timed
S_1/A_1 with Main publication after callback, original431 audio when available,
event manager before Mario, nested Game key/system/camera ownership, retained
message windows and original selection. Then connect a yielding native persistence
adapter and preserve GSF233/236/GSW4 alongside existing source flags. Avoid
force-close on Continue or an extra Continue on success; No must not write.
Source pause/category/camera consumers, shared field VM/state, full lifetime,
original execution traces, other areas/actors and hardware profiling remain open.
The full user goal remains active.

## 2026-10-13 — Priority0 MusyX listener update and cross/inverse math

Delivered [listener/cross/inverse](behavior/audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md).
Three complete bodies pin four-vector publication, cross-product order, stack
matrix layout, inverse cofactor/translation stores and low-byte volume/127.
No normalization, singular rejection or in-place snapshot guarantee added.
Marked local EVT-01 portions complete; numeric/source/consumer/runtime OPEN.

Validation: generic read-only verifier10 spans (3 complete/4 linked/3 literal),
229 anchors/six resolved direct calls passes. Derived stack/store maps and two
admitted matrix/one cross examples remain static deductions; no original code
executes. Full fresh ELFs/listings stay `/tmp`. Link audit761 Markdown files/
0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: listener/basis/allocation providers, null/nonalias/finite/invertible/
PPC numerical domains, CalcEmitter/consumer transforms and original geometry/
audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game effect main and outer sound updates

Delivered [effect/listener/completion main](behavior/audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).
Three complete bodies pin listener displacement/basis arguments, separate40-record
update/completion passes, captured traversal/fresh pool cleanup, double voice
checks/countdown continuation, exact sequence-valid1 and effect/DVD/SS outer
order. No measured velocity or physical audio cadence. Marked local EVT-01
portions complete; provider/child/runtime remains OPEN.

Validation: generic read-only verifier21 spans (3 complete/14 linked/4 original
literals),135 anchors/25 resolved direct calls passes.40 pool offsets/four countdown
boundaries/double-query metadata are derived source cases. Full fresh ELFs/listings
stay `/tmp`; no original execution. Link audit759 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or original
body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: listener update/transform and S position/heading providers, pool/
record/sequence/countdown writers and alias/concurrency invariants, DVD/SS/sequence/
controller/voice and original completion/audio traces. Continue priority0 source/
backend then priority1 damage graph. No hardware/playback/full-game completion.

## 2026-10-13 — Priority0 emitter vector/volume update and 3D reset

Delivered [update/reset](behavior/audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md).
Two complete bodies pin ordered two-vector copies, low-byte volume/127 target,
current-volume one-sided clamp and exact root/studio/flag reset. Update does not
check emitter membership; reset does not clear old emitter flags/voices. No
smoothing/cadence/release inferred. Marked local EVT-01 portions complete;
construction/consumers/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 IRQ/2 literal),47
anchors/two resolved direct calls passes. Whole-DOL direct census2 sites;8 volume/
7 flag/5 finite clamp vectors derived only. Full fresh ELFs/listings stay `/tmp`;
no original execution. Link audit758 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: emitter construction/list/vector/volume consumers, actual SoundEfxMain
provider/gate graph, listener/studio setup, alias/null/finite/reset/concurrency and
original update/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 emitter removal and FX unload lifecycle

Delivered [emitter lifecycle](behavior/audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md).
Three complete bodies pin active-system admission, exact neighbor/root unlink,
all-high16 flag clear, retained links/voice and key-off request. Bulk/FX-array
traversal preserves captured next and first-match break even without mutation.
Marked local EVT-01 portions complete; full voice/unload/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),117 anchors/
9 resolved direct calls passes. Whole-DOL aligned caller census9 sites;6 clear/
12 gate vectors checked as deductions. Fresh full ELFs/listings stay `/tmp`;
no original code executes. Link audit757 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: emitter construction/active/list/FX providers, member/cycle/array/
IRQ/concurrency source domains, full caller unload/silence and macro/voice/key-off
execution, original lifetime/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 effect spatial controls and emitter query leaves

Delivered [spatial/emitter leaves](behavior/audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md).
Five complete bodies pin early pan/surround record stores, fresh pool/voice lookup,
raw/controller width and two surround requests, retained volume read and active/
flag-gated emitter voice lookup. No rollback on voice miss or inferred audible
liveness. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier10 spans (5 complete/5 linked),75 anchors/
11 resolved direct calls passes. All256 surround mappings,9 raw-width and15
active/flag vectors checked; original functions not executed. Fresh full ELFs/
listings stay `/tmp`. Link audit756 Markdown files/0 broken; whitespace checks
pass. Docs/source metadata only: no make rerun, native changes or original body/
bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: effect-pool/emitter flags/voice providers and valid ordinal/null/
lifetime/concurrency domains, controller/MIDI/voice/backend services and original
pan/surround/fade/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 remaining positional SFX live/fade/mute graph

Delivered [SFX live/fade/mute](behavior/audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md).
Two complete regions pin global fade/percentage, spatial/nonspatial controls,
backend scaling, mute→fade→fresh stop/clear→final mute. Previous sweep/startup/
loop/tail plus this delivery close the local whole psndSFXMain body. Calls remain
ordered even when final mute follows key/engine clear. Marked local EVT-01
portions complete; backend/source/original audio remain OPEN.

Validation: generic read-only verifier18 spans (2 regions/15 linked/whole body
fingerprint),446 anchors/35 resolved direct calls passes.65536 division127 byte
products and65536 division100 unsigned×signed byte products checked;6 fade wrap/
decrement vectors are derived only. Fresh full ELF/listing remains `/tmp`;
no original code executes. Link audit754 Markdown files/0 broken; whitespace
checks pass. Docs/source metadata only: no make rerun, native changes or original
body/asset export. Isolated index excludes unrelated shared work.

Next/limits: SS/song/effect child controls/stop/query semantics, G/slot/descriptor
writers and flag/fade/denominator/alias/concurrency/lifetime domains, original
live/fade/audio observations. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 listener setter and sequence camera source bindings

Delivered [listener/camera source](behavior/audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md).
Two complete bodies/two scoped argument regions pin alias-sensitive XYZ stores,
heading publication, unchecked camera table lookup, field position/repeated-heading
queries and battle fixed XYZ/flag100 Z adjustment. Exact direct argument bindings
are closed; whole camera/sequence gates/cadence remain open. Marked local EVT-01
portions complete; parent OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 scoped/1 linked/
1 original literal),46 anchors/8 resolved direct calls passes. Whole-DOL aligned
setter census2 sites; original constants0/400/200 decoded. Alias example is a
static deduction, not original execution. Full fresh ELFs/listings stay `/tmp`.
Link audit752 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original code-body/asset export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: camera pointers/position/heading producers and finite/null/source
invariants, whole sequence admission/cadence, battle flag/shared-result writers,
indirect/REL/dynamic routes and original spatial/audio traces. Continue priority0
live SFX/source then priority1 damage graph. No hardware/full-game/audio claim.

## 2026-10-13 — Original underground approach, dialogue and Goomba animations

[Native trio scene](behavior/story/NATIVE_UNDERGROUND_TRIO_SCENE.md) runs actual
161-command gorotuki_sanninngumi and10-command footstep child in the existing
VM. Story<=11 roomrestore admits sourceinitial placement/flags and MarioX>=-100
crossingpoll, then original keys/cameras/party positioning and seven exact
086..092 messages with correct named speakers/Goombella suffix. Parallel actor
run/hop/move blocks preserve original local/shared event IDs. Imported actual
c_kuribo KUR/PTK/TGK stay/talk/walk/run/damage sparse poses and Marioe_mario
M_N_7. Native citro3d batches draw allthree variants; Frankly now follows/renders
as temporary partner underground and accepts original party1 placement.

Scoped source callbacks cover facing towardNPC, Mario/party positions and NPC
run/damage poses. Original script requests battleinfo4 and remains atbattle_start
until its actual backend exists; no postbattle messages/story12 are synthesized.
Documentation inbox delivered full setup4 contract: threeHP2 variants PLUS
Frankly coordinator, five firstturn messages and two conditional spike messages,
variant weapons/flags/counter/fall and source wait/result ownership.

Validation: staged source snapshot /tmp/pm-underground-trio-dbe8scf9 passes
make test108 CTest/100Python. Shared make all and separate DEBUG_SHORT_BOOT
artifacts/ttyd-underground-trio build pass; focused underground/professor tests
pass. Host original-data checks pin placements, passive poll, inclusive crossing,
message order, source pose/party positions, four cameras, setup4 and retained11.
Isolated Azahar artifacts/azahar-underground-trio captures original086, allthree
variants/Frankly, camera/dialogue pages and final pendingbattle notice; images
inspected. It is manual presentation evidence, not a full nativebattle pass.
Initial unusedZ_2 imports rejected non-endpoint topology, so only actualused
poses are admitted. InitialM_N_7 bank assumption rejected; corrected to original
e_mario. First shared make test encountered an unrelated build-host concurrent
archive race; isolated staged build excludes unfinished concurrent save-event
work and preserves its files. No user emulator profile/save touched.

Limits/next: bind setup4 battle/coordinator and original attack/counter/fall/result
services, then original093/094/094_01/GSW0=12 continuation. Full party stop/run,
force-restart pose clocks, NPC terrain/ease/hop arithmetic, effects/SFX, source
cadence, original lighting/material/shadow sorting and original-runtime traces
remain incomplete. Source first-arrival main map services/dynamic hit toggles and
actual pipe BOTH-party restoration also remain partial. Live scene saves remain
suppressed because native event state is not serialized. Complete setup3 lecture,
prologue route/ruins and New2DS hardware validation still required; goal active.

## 2026-10-13 — Priority0 calc3D spatial result and angle table

Delivered [calc3D/angle-table contract](behavior/audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
Two complete bodies pin flag/capture precedence, original91-row angle table,
literal196 pan fold, PPC sqrt refinement/cos/sin surround, distance falloff and
byte-first volume clamp. Shared output preserves byte3. No host math replacement,
measured numeric/audio behavior or guessed listener invariant. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (2 complete/2 linked libm/1 table/
4 literal spans),161 anchors/four resolved direct calls passes.91 original table
rows/literal bytes checked;15 signed fold vectors remain static arithmetic.
Full fresh ELFs/listings stay `/tmp`; no original execution. Link audit750 Markdown
files/0 broken; whitespace checks pass. Docs/source metadata only: no make rerun,
native changes or original body/bank export. Isolated index excludes shared work.

Next/limits: listener/shared-byte source writers, valid heading/flags/finite and
PPC fused/frsqrte/fctiwz/libm domains, live SFX/backend/original spatial/audio
traces. Continue priority0 live/source owners then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 audio frame and aux callback order

Delivered [frame/aux graph](behavior/audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
Seven complete bodies pin one-time active gate, DSP submission before five
seq/synth slices, frame-byte/live voice clears, final3D/stream/virtual-sample
order and direct OS critical wrappers. Aux A/B callback/buffer capture and fresh
B gates/count reads remain explicit; no measured physical cadence/action ticks
inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier17 spans (7 complete/10 linked),127 anchors/
29 resolved direct calls passes. All256 byte arithmetic cases, five-slice order,
three-channel extent and6 aux-gate vectors checked; no original execution. Full
fresh ELFs/listings remain `/tmp`. Link audit748 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change
or original body/bank export. Isolated index excludes unrelated shared work.

Next/limits: seq/synth/stream/3D/virtual-sample/command-builder engines, actual
voice/studio/aux providers and pointer/format/IRQ/reentrancy domains, original
frame/DMA/aux/audio observations. Continue priority0 source/live SFX/calc3D then
priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 DSP task callbacks and command-mail source

Delivered [DSP task/callback/mail](behavior/audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md).
Six complete bodies pin task publication/init wait, distinct ready/init/resume/
thread-wake callbacks, pending-wait consumption before active gate, command
pointer/size captures, two-mail polls and raw wrapping clock arithmetic. Original
DSP program identity is pinned without decoding/export. Marked local EVT-01
portions complete; parent and actual audio execution remain OPEN.

Validation: generic read-only verifier21 spans (6 complete/13 linked/2 original
program and length),98 anchors/16 resolved direct calls passes. Length1ec0 equals
program span;9 resume-gate/5 mail/6 clock-domain vectors are derived source checks,
not original execution. Fresh full ELFs/listings remain `/tmp`. Link audit747
Markdown files/0 broken; whitespace checks pass. Docs/source metadata only: no
make rerun, native change or original DSP/bank/code-body export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: SDK/DSP task/mail execution, command builder/control/voice/frame
engine and state/thread/clock/pointer/concurrency domains, original startup/resume/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Original east pipe and first underground map

[Native underground route](behavior/story/NATIVE_UNDERGROUND_PIPE_ROUTE.md)
packages original gor_02/dokan↔tik_01/dokan_2 records and hit centers/identities.
Case6 retains strict signedmainY>30/contact gating; case10 uses frontcontact,
without A. Original11-command gor pipe departure executes in the existing VM,
sets actual GSF1207, preserves GSW0 and retains party requests. Already-setflag
returns without repeat requests. Expanded metadata4 contains329commands and
64callbacks. Native transactional map staging reaches original tik_01 as room5,
with original geometry/textures/collision/camera and source underground music.
Prologue zero-flag wall display hides are extracted; source hit toggles remain
unbound. Source horizontal endpoint20/start-40 offsets replace provisional0.65.

Inbox deliveries recover exact pipe event/geometry and full161-command Goomba
trio/init/arrival contracts. Original scene waitsMarioX>=-100, uses three actual
c_kuribo tribes and original086..094_01 payloads (last Frankly text in tik_04),
requires battle setup4 and only then advancesstory12. This room import/route
preservesstory11; binding that event/actors/battle is the next milestone.

Validation: isolated staged implementation snapshot /tmp/pm-east-pipe-1mezum8k
passes make test107 CTest/100Python after final pipe ground-query change. Shared
make all and separate DEBUG_SHORT_BOOT artifacts/ttyd-east-pipe build pass.
Focused original pipe and professor tests pass. Initial new pipe landing query
failed because ordinary groundAt has a narrow support window; corrected to
source-start downward-ray bridge. Shared make test initially hit unrelated
in-progress save-block event assertion, then hung in that test; that own run was
stopped and is not reported passing. Snapshot excludes that concurrent unfinished
feature and retains its files. Isolated final Azahar artifacts/azahar-east-pipe-final
passes contact/stick travel to underground, save(room5/story11/flag1), reverse
frontcontact return and save(room2/story11/flag1). Both map captures inspected;
Frankly is correctly hidden on east-room return. Repro fixture tool is
make_east_pipe_fixture.py. No user emulator profile/save touched.

Limits/next: case6 Mario+1e8 producer is not proved identical to native ground
contact; source mode/Vivian/nonride/contact lifecycle, pipe paper poses/SFX/camera
and BOTH-party restoration remain incomplete. Direct emergence/fade and ground
support are native adapters; temporary Frankly underground rendering is pending.
Bind original Goomba trio scene/setup4 and later badge/door/ruins route, actual
lecture setup3, dynamic wall flags/collision and original environment/reverb.
NDSP streams original underground music; audio/device and New2DS performance
were not measured here. Full requested prologue/full-game fidelity remains open.

## 2026-10-13 — Priority0 AI buffers, callback and IRQ initialization

Delivered [AI/IRQ/compressor source](behavior/audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md).
Six complete bodies pin initial IRQ ownership, four-slot allocation/null failure,
state/registration/initial DMA/rate publication, callback rotation/tick-before-gates,
OS enable/dispatch/disable ordering, CPU destination and compressor flag. These
are local source requests, not measured DMA/frame/audio behavior. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier15 spans (6 complete/9 linked),83 anchors
and11 resolved direct calls passes. All256 byte-index arithmetic cases, exact
four-slot extent and9 DSP/logic gate vectors checked; original code not executed.
Fresh complete ELFs/listings remain `/tmp`. Link audit746 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native changes or
original code-body/bank export. Isolated index excludes unrelated shared work.

Next/limits: DSP/control/frame callback services, actual state writers and SDK/
physical coherence/alias/concurrency/reinit domains, original interrupt/callback/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game allocator providers and MusyX bootstrap

Delivered [game allocator/bootstrap](behavior/audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Four complete bodies and scoped SoundInit startup prefix establish original
hook table/providers, fixed game initialization inputs, low-byte configuration,
hardware admission/failure and successful default-cache/resource/child init order.
Game prefix ignores sndInit result; no child failure unwind or residency/playback
proof inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier29 spans (4 complete/1 scoped prefix/
1 original hook table/23 linked),134 anchors/25 resolved direct calls passes.
Independent8-byte table matches actual provider symbols; nine derived clamp/width
vectors checked. Full fresh ELFs/listings remain `/tmp`; no original execution.
Initial link audit742 Markdown files/0 broken; later shared audit743 files found
one unrelated new underground link pending. All6 delivery Markdown files have
0 broken links; whitespace checks pass. Docs/metadata only:
no make rerun, native change or code-body/bank export. Isolated index excludes
unrelated worktree/staged work.

Next/limits: whole SoundInit effects/loader/settings tail, child OS/AR/AI/DSP/IRQ/
frame/seq/synth/stream/virtual-sample/3D, provider/size/reset/concurrency/failure
and original startup/audio traces. Continue priority0 source/live SFX/calc3D,
then priority1 damage graph. No hardware/audio/full-game completion claim.

## 2026-10-13 — Priority0 default cache, resource reset and hook installation

Delivered [cache/reset/hook producers](behavior/audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md).
Seven complete bodies pin gsDefault base/end/cursor and root/current publication,
six U16 registry-count resets, whole macro main-table clear, second-argument-only
ARAM initialization, ordered hook copy, ordinary allocator and callback forwarding.
Reset counts do not erase all registries or prove stopped voices; alias-sensitive
hook copy is preserved. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (7 complete/2 linked documented
children),87 anchors and4 resolved direct calls passes. Four derived cache/wrap
vectors,800hex macro-clear extent and3 hook-alias vectors checked. Full fresh
ELFs/listings remain `/tmp`; no original code executes. Link audit741 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original code-body/bank export. Isolated index excludes unrelated work.

Next/limits: actual setup callers/B/N/allocator/callback providers, per-kind
registries, alias/reset/concurrency/residency domains, voice/macro/MIDI/SDK and
original setup/audio/runtime traces. Continue priority0 source and live SFX/calc3D,
then priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 generic ARAM upload and hardware stream bridge

Delivered [upload/stream bridge](behavior/audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).
Five complete bodies pin raw upload/full-word priority normalization/user callback,
high/low queue post-before-count/index and hardware stream offset/length alignment,
DCStoreRange and exact init/exit/address delegation. The flush ignores returned
active size; play-buffer wrapper reads an address, not hardware playback position.
Marked local EVT-01 portions complete; full stream/source/runtime remains OPEN.

Validation: generic read-only verifier16 spans (5 complete/11 linked),114 anchors,
14 resolved direct calls passes. Independent whole-DOL aligned caller scan25
sites (17 Flush/1 Get in streamHandle,1 Init in sndStreamAllocEx,5 Exit in
sndStreamFree,1 upload in Flush), each instruction anchored. Eight priority and
42 offset/count vectors remain arithmetic deductions, not original execution.
Full inspection ELFs/listings stay `/tmp`. Link audit739 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change,
original body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: actual streamHandle/AllocEx/Free caller ownership, source/providers,
valid callback/index/count/offset/extent/lifetime/concurrency domains, OS/cache/
ARQ/DMA and original audio traces. Continue priority0 source/live SFX/calc3D
owners, then priority1 damage graph. No playback/hardware/full-game completion.

## 2026-10-13 — Original lecture choices, talk requests and story11 reload

[Native lecture binding](behavior/story/NATIVE_LECTURE_CHOICES_AND_RELOAD.md)
executes original selection/add/partykill/setup requests. Declining practice via
source results0/1 advances GSW0 to11 and retains the original question window
across084_03/084_04. Both accepted branches preserve Goombella NPC attribution,
visibility/hop and setup3, then stop at the unbound original battle_start.
Source choice opening/closing, press/repeat/cancel/result semantics and separate
window geometry run with original US text. Imported original STICK/cursor pixels
and frame delays; sound requests remain unbound. Glyph-driven talk countdown
replaces window-active pose inference; added original Goombella PKR_T_1.

Story10 reload admits a fresh lecture crossing poll; story11 reload restores
Frankly's source outside position/yaw or flag1207-dependent hidden state.
Save version5 retains actual GSF1207 in byte34 and reads native versions1..4.
Documentation inbox deliveries cover speaker talk/stay ownership, exact flag
and map reentry, NPC visibility and distinct lecture backend ownership.

Validation: make test passes106 CTest/100 Python tests; make all passes. Separate
DEBUG_SHORT_BOOT build-lecture-verify/artifacts/ttyd-lecture-choices build passes.
Tests exercise accepted and declined original branches, retained append,
selection gates/geometry, source icon records, glyph countdown, story10 reentry,
story11 visible/hidden restore and save-v5/legacy compatibility. Isolated Azahar
final run artifacts/azahar-lecture-choices-final passes both choices and verifies
saved originalStory11; captured original windows and returned field inspected.
Separate story11 reload capture artifacts/azahar-lecture-story11-reload/boot.png
shows Frankly outside with ordinary field restored. A prior OCR attempt failed missing English data/incompatible older data, then
was replaced by pixel window detection plus independent native save assertion.
An earlier native pixel run also passed. No user emulator profile/save touched.

Limits/next: implement actual setup3 teaching battle/coordinator and field return,
original pipeout/GSF1207 writer/underground maps, complete NPC/party conversion,
force-restart pose ownership, icon transforms/depth, original selection sounds,
GX fence capture and source event/animation cadence. Native renderer uses source
assets through citro2d/citro3d; physical New2DS performance and full prologue
parity remain unverified. The full-game/prologue goal remains active.

## 2026-10-13 — Priority0 ARAM initialization and stream-buffer owners

Delivered [initialization/hooks](behavior/audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md)
and [stream-buffer lifecycle](behavior/audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md).
Eight complete bodies pin zero allocation/fill/post/wait/free, limits/queue resets,
hook requests,64-descriptor idle chain, cached/downward allocation, low-byte
getter/default pointer and exact free traversal. Larger-candidate predecessor
and global-head cleanup writes are preserved; source list/order/membership
invariants remain required. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
251 anchors and14 independently resolved direct calls passes. Derived64-node
chain,8 alignment,7 index and7 cached-search vectors accompany full listings;
no original functions execute. Fresh complete ELFs/listings remain `/tmp`.
Link audit738 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native changes or original bank/code-body export. Isolated index
keeps unrelated staged/worktree changes out of this commit.

Next/limits: actual default-cache and allocator-hook producers, valid stream-list/
cycle/predecessor/order/membership domains, upload/SDK/OS/ARQ/DMA and original
sample/voice/runtime traces. Continue priority0 source/backend and live SFX/calc3D
owners, then priority1 damage graph. No original playback/hardware/full-game claim.

## 2026-10-13 — Original object top/side collision and supported checkpoint restore

Commits `4e12f64` and `1d853a6` bind the unchanged original12-face save-block
cube to top support, ordinary Mario forward/around wall queries and checkpoint
validation. PMNPC004 retains original model names so the source hidden-model
floor/wall exclusions remain explicit; older catalogs remain loadable. Dynamic
floor/wall owners stay separate from static REL offsets/array indices. Existing
five-foot support, two-height wall rays, source corrections and landing response
remain shared. Static object geometry caches catalog generation and room/story/
interior/system admission, avoiding rebuilds across unchanged queries/frames.
Successful catalog replacement invalidates reuse; failed loading retains it.
See [native collision](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md).

Source documentation through OW-INTERACT-01 establishes
[ordinary Goombella save-cube admission](behavior/partner/FIELD_PARTNER_SAVE_BLOCK_COLLISION_BINDING.md),
including distinct floor/front/around filters and tie rules. Goombella's current
native floor adapter includes the cube; its complete original wall driver remains
unbound. The file menu/reload path previously rejected on-block checkpoints via
static-only validation. Both now use the same object-aware candidate service
and existing transactional area replacement. Native save elevation quantization
stores the top fixture as.9, then source support resolves actual top.85.

Validation: final isolated source/romfs snapshot
`/tmp/pm-object-surfaces-mkwzez51` passes `make test`104 CTest/100 Python tests
and `make all`. Original-data tests check top/side normals/backfaces, dynamic
owner metadata, hidden-name floor/wall rejection with head geometry retained,
standing, full top jump/landing/walk-off, malformed V4 name atomicity and cache
invalidation after catalog replacement/system gating. The final checkpoint
regression is covered by that complete run. An initial Azahar top capture
exposed static-only checkpoint rejection; it was corrected before acceptance.
Immutable final binary `artifacts/ttyd-saveblock-checkpoint.3dsx` loads the
`--on-block` fixture in isolated `artifacts/azahar-saveblock-top2`; inspected
`saveblock-head-landed.png` shows Mario on the original block after jumping.
No user emulator profile/saves touched. These host/emulator checks establish
this native slice, not matched original execution or New2DS performance.
Documentation links731 files/0 broken before the handoff; whitespace checks pass.
Unrelated concurrent lecture/message/save/audio changes remain outside these commits.

Next/limits: connect source name-contact cache and save callback0..4 with
independently timed S_1/A_1 poses, previous-published loop ratio, nested keys,
system/camera ownership, original US save choices and a native persistence
adapter retaining source switch namespaces. Hit animation/prompt remain unbound.
Source partner body-wall driver, moving objects, dynamic shadow queries,
multiple-map-record hit ordering, full area/event content, original lighting/
translucent ordering and hardware profiling remain open. Keep the full goal active.

## 2026-10-13 — Priority0 ARAM sample store and queue completion

Delivered [sample store/cursor/configuration](behavior/audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md)
and [queue completion/synchronization](behavior/audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
Eight complete local bodies pin32-byte wrapping alignment/capacity, direct versus
callback chunks, retained return/fresh destination/cursor, post-before-count/index,
full callback scan and low-only busy sync. Removal only rewinds caller cursor,
ignoring the supplied address. No timeout, invalid-completion guard or fabricated
free/voice-stop semantics. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
207 instruction anchors and16 independently resolved direct calls passes.
Derived8 alignment/rewind vectors,256 byte-index arithmetic cases and5 callback
count boundaries checked; these are static arithmetic evidence. Fresh complete
inspection ELFs/listings remain `/tmp/inbox-aram*`. Link audit734 Markdown files/
0 broken; whitespace checks pass. Documentation/source metadata only: no make
rerun, native changes or original code-body/bank export. Isolated commit index
excludes unrelated staged/worktree native work.

Next/limits: aramInit/stream/cache producers and other queue upload paths, actual
callback providers/removal order/queue concurrency, SDK flush/interrupt/ARQ/DMA,
voice/macro/MIDI/resource ownership and original audio/runtime traces. Continue
priority0 initialization/source and live SFX/calc3D, then priority1 damage graph.
No hardware/original playback/full-game equivalence claim.

## 2026-10-13 — Priority0 sample directory lifetime and storage bridge

Delivered [directory/reference lifetime](behavior/audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
and [sample hardware bridge](behavior/audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md).
Eight complete local bodies pin preferred-pointer publication, duplicate/capacity
behavior, mutable low16 refs and resolved addresses, acquisition preconditions,
release/removal/compaction and raw hardware/cache requests. First acquisition
requires a matching preferred inactive candidate; no source not-found fallback.
Encoded transfer sizing is proved over the24-bit count domain; it is not audible
length or successful residency. Marked these EVT-01 portions complete; parent OPEN.

Validation: read-only generic verifier13 DOL spans (8 complete/5 linked),256
anchors,10 independently resolved calls and1 original directory resource hash
(no map nodes) passes. Independent original .sdir parse1247 records, all refs0/
formats0; Hammer775/776 source transfers3016/872 bytes.54 derived boundary vectors
and exact-domain arithmetic proof remain static evidence. Fresh full ELFs and
listings remain `/tmp`. Link audit731 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make test/all rerun, native changes or original
asset/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: ARAM store/remove/sync/cache/allocation/DMA services, per-kind pool
registries, bank/source initialization, macro/voice/MIDI stops and actual runtime
residency. Continue priority0 audio source owners and live SFX/calc3D requests,
then priority1 damage/counter graph. No original audio/hardware/full-game claim.

## 2026-10-13 — Original map-object bbox head collision

Commit `12d4b56` connects the original save-block cube to the existing native
five-probe head query and ceiling response. PMNPC003 adds original model bbox
metadata and source object names while retaining PMNPC001/002 loading. The
converter reads original model0xd0..0xe4; source dimensions, forcedY0 midpoint,
eight vertices/six alternating strips, source cull1 and owner attribute are
retained. Dynamic owner keys remain separate from static REL node offsets.
Actual contacts publish the owner; near misses/backfaces do not. Original far
ray endpoint rejects and equal hits retain the earlier static/object slot.
Reused per-Game storage bounds the current plaza object to12 triangles.
See [native binding](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md) and
[asset provenance](ASSETS.md).

Documentation inbox work recovered the
[original shared hit registry/order](behavior/field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md):
source map/slot traversal, dynamic insertion, strict upward ties/endpoint,
whole-box ground/wall eligibility and borrowed name/delete lifetime. Current
native packs cover one map record; multiple-record chain reconstruction remains
explicitly pending. Model draw scalar and S_1/A_1 deformation do not change the
source collision cube.

Validation: independent source/romfs snapshot
`/tmp/pm-object-collision-er9tx9p7` passes `make test`104 CTest/100 Python tests
and `make all` after the final endpoint change. Tests cover source bounds,
12 faces/bottom winding, head actual/near-miss/backface/far-endpoint queries,
room/story/system gates, malformed V3 atomicity and a full Game jump that
publishes the owner, clamps, falls and lands. Initial shared full run failed an
unrelated concurrently modified professor lecture assertion; it is not reported
as passing. Snapshot excludes that unfinished lecture/choice work and preserves
its shared edits. Immutable binary `artifacts/ttyd-saveblock-head.3dsx` boots
with `--under-block` fixture in isolated Azahar; inspected contact/landed images
are `artifacts/azahar-saveblock-head/saveblock-head-contact.png` and
`saveblock-head-landed.png`. The images support presentation/landing, while
host assertions establish the collision owner/correction. Emulator samples
are not stable hardware performance evidence. No user profile/saves touched.
Documentation link check727 Markdown files/0 broken; whitespace checks pass.

Next/limits: bind this same source cube into top support and side-wall queries;
connect name-contact caching and callback0..4 with independently timed poses,
previous-published loop ratio, nested key/system/camera ownership, original
save-choice graph and explicit native card persistence adapter. Hit animation
and save prompt are not yet triggered by the new collision. Moving object
transforms, full event/area content, source observations, original lighting/
translucent ordering and New2DS profiling remain open. Full user goal stays active.

## 2026-10-13 — Priority0 group lifecycle and game audio loader source bridge

Delivered [complete group push/pop/resource lists](behavior/audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md)
and [game warm-up/loaded-bank/free](behavior/audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
Group push publishes stack pointers before resources, preserves typed pool/list
ranges and FX insertion; pop decrements first, requests voice stops, removes
samples backward and remaining lists forward. Game explicit-list push counts
attempts while sequential mode counts exact successes; warm-up ignores push
returns. Ready and return values are not proof of actual residency. Marked those
local EVT-01 portions complete, leaving child/source/runtime ownership open.

Validation: generic lifecycle manifest29 spans (6 complete/23 linked),476
anchors,56 resolved direct calls and1 original project resource hash passes
(no map nodes). Independent project parse12 groups0..11/2998 IDs; reverse
samples match forward reversal in all12; group6 contains Hammer775/776 samples
and994/995 macros. Fresh complete ELFs/listings and derived report remain `/tmp`.
Link audit726 Markdown files/0 broken links; whitespace checks pass. Docs and
metadata only: no make rerun, native code or original asset/code-body export.
Unrelated staged/worktree edits remain excluded from this commit.

Next/limits: sample-directory/reference/per-kind pool registry producers,
actual file/sample/project/bank initialization and residency, voice-stop/MIDI/
macro/OS/hardware/source/runtime still open. Continue priority0 these source
owners and calc3D/live audio/query bindings, then priority1 damage/counter graph.
No original executed/audio/hardware/full-game completion claim.

## 2026-10-13 — Priority0 FX registry insertion/removal and live resource mutation

Delivered [complete FX registry producers](behavior/audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md).
First insert references the caller's10-byte entry array and writes every byte9
to31 before publishing the fresh group count. Duplicate registration retains
existing pointer/count, increments16-bit refcount and returns0; full refusal
also returns0. Removal decrements refs, compacts12-byte records when zero and
leaves unused tail/entry arrays/voices untouched. Linked synth-start extra
operand therefore comes from live-mutated data, not immutable .proj bytes.
Marked the local EVT-01 producer portion complete; no full residency claim.

Validation: fresh complete two-body listings under `/tmp`; generic registry
manifest5 fingerprints/87 anchors passes. Derived reference boundary metadata
retains0/1/2/65535 wrap. Link audit722 Markdown files/0 broken links; whitespace
checks pass. Docs/metadata only; no make rerun, native/resource conversion or
original ISO/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: sndPushGroup/sndPopGroup and actual project residency, sorting,
resource/voice lifetime on unload, macro/synthStartSound/controller mutations,
calc3D/live audio and original scheduler/encounter/runtime remain open. Continue
priority0 source/core/query binding before lower-priority integration. No original
executed/audio/hardware/full-game equivalence claim.

## 2026-10-13 — Original save-block surfaces and material passes

Commit `c3e36cb` adds the original plaza `mobj_save` presentation through the
shared per-area catalog. PMNPC002 retains original `(185,60,-55)`, scalar10,
S_1, story341 exclusion and constructor system mask0x20. PMANIM4 preserves
original indexed RGBA, untextured draws, discrete texture shifts, material wrap
overrides, source pass/cull and nonzero animation start. Portable triangle
partitioning preserves texture/color interpolation across clamp/repeat/mirror
boundaries. Citro3d submits original opaque/translucent admission and flushes
CPU-written linear vertex data. PMANIM1/2/3 and PMNPC001 remain loadable.
Conversion/resources stay local and ignored; provenance/rights are in
[assets](ASSETS.md) and [surface format](behavior/display/NATIVE_ACTOR_SURFACE_PACK.md).

Documentation work through OW-INTERACT-01/SAVE-01 recovered
[material/UV binding](behavior/display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md),
[head cube and callback lifecycle](behavior/field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
and the [complete common save event](behavior/field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
These establish original contact, five-state callback, previous-published
animation ratio, key ownership, exact original choices, flag namespaces,
cancellation and card error/retry requests. Story text is retained from
msg/US; no replacement dialogue is introduced.

Validation: shared-worktree `make test` passed103 CTest/100 Python tests and
`make all` built the native3DS binary before concurrent fence changes. Isolated
feature snapshot `/tmp/pm-object-verified-j_rg9rov` independently passed103
CTest/100 Python tests and `make all`; its source/romfs were isolated from later
concurrent edits. Dense barycentric tests check all nine wrap combinations,
geometry area and RGBA interpolation; original S_1/A_1 tests check discrete
scrolling, timing, pose wrap and malformed-pack retention. Idle peak402
vertices. Its immutable binary `artifacts/ttyd-saveblock-verified.3dsx` passed
isolated Azahar capture `artifacts/azahar-saveblock-verified2`: original colored
block and animated glow were inspected in the plaza. An earlier3-second cold
launch stopped at title; the successful12-second startup reached the field.
Single emulator HUD sample60fps/sim3.43ms/CPU8.3ms/GPU0.5ms is not a percentile
trace or hardware-performance result. Documentation links721 files/0 broken;
whitespace checks pass. User emulator profile/saves remain untouched.

Limits/next: bind the recovered source collision cube and head-name cache,
then the hit lifecycle with independently timed per-instance poses and original
common save event/card adapter. Save-block hit/save interaction is not yet
connected. Original lighting, global translucent sorting, NPC collision/search,
wandering, area event completeness and New2DS hardware profiling remain open.
Keep the full user goal active; this is an incremental Rogueport milestone.

## 2026-10-13 — Title/file menu and fresh-file opening order adapter

Normal boot now plays the original storybook/market demonstration, title and
four-file menu before starting an empty file's opening. Native controllers own
input separately from field mechanics. A/START skips the demo to title and
opens file selection; D-pad/touch chooses a file, B returns to title. Original
US messages are preloaded and parsed before the new-file sequence starts:
37 demo payloads and11 opening payloads preserve IDs/raw bytes/control tokens
and explicit speaker operands. Diary sets source type7. New-file order is
house → letter → house return/map display → voyage/input gate → original
prologue heading → awakening → arrival → saved harbor story3 at(0,15,225).
Map display retains the source +2/clamp255 counter over128 simulation updates;
source GSW0 writes2/3 are explicit. Existing original saves are not a priority
per the user's steering; slot0 keeps the existing path as a low-cost convention.

Original title/desk/book/letter/map/prologue raster assets, house/voyage/wake
geometry, four character banks and six music streams are converted locally.
House outside/interior snapshots use distinct visibility roots; Luigi retains
idle/talk/read clips. Front-end caches release before field loading and empty
actor-atlas rows are cropped with checked UV scaling. No generated art, rewritten
story dialogue, synthesized tones or redistributed ISO assets were added. See
[asset choices](ASSETS.md) and [opening contract](behavior/story/NATIVE_OPENING_FLOW.md).
`make debug-short` creates a separate `ttyd-2ds-debug-short.3dsx` and build directory
that starts in the prior checkpoint/plaza harness without the opening.

Incremental commits: controller `d47d21f`, test/index registration `e19eda1`,
native/assets/order adapter `a096639`, committed-professor API compatibility
`433a001`. Concurrent work was changing the shared field actor packs and source
APIs during testing. Final reproducible validation used isolated checkout
`/tmp/pm-opening-validation-a096639` with the exact433a001 main.cpp correction,
without incorporating unrelated pending edits. `make test` passes **101 CTest /
96 Python tests** (`/tmp/opening-isolated-test.log`); `make all` passes without
warnings (`/tmp/opening-isolated-build.log`); `make debug-short` passes
(`/tmp/opening-isolated-debug-build.log`). The primary working tree additionally
passed103 CTest/100 Python tests at `/tmp/opening-preload-test.log`, but that
mutable-worktree result is not the reproducible feature baseline. Original-data
playback tests verify all48 IDs, source speakers, automatic demo completion,
source-story writes and voyage input admission. Document audit checks720 Markdown
files with0 broken links; whitespace check passes.

Isolated Azahar fresh-file `tests/azahar_opening_flow.py` passes the full sequence
and verifies a native40-byte saved room1/story3 checkpoint
(`/tmp/opening-isolated-smoke.log`). Reviewed title, file menu, voyage and final
harbor exploration captures; copies are under ignored `artifacts/azahar-opening-isolated/`.
Separate debug-short Azahar boot passes directly into the plaza
(`/tmp/opening-isolated-debug-smoke.log`); reviewed debug-short-boot.png.
Local verified artifacts are `ttyd-2ds-opening-verified.3dsx` and
`ttyd-2ds-debug-short-verified.3dsx`. The isolated source/profile/assets stay under
/tmp; original extracted assets/binaries remain ignored. No user emulator profile
or saves were modified. Xvfb required display-socket escalation. Earlier shared
runs exposed initialization/resource failures and are superseded by this stable
checkout validation; no original-runtime/Dolphin or hardware measurement is claimed.

Limitations/next: this is an **event-order/presentation adapter**, not complete
source cutscene execution. Book/page transforms, original camera interpolation,
NPC movement/yaw/pose ownership, ship transitions, fades, title curtains, diary
border/glyph effects, environment/SFX services and full new-game pouch/card
initialization remain unbound. Preserve original concurrent/branch/control semantics
when replacing snapshots with REL execution. The new-file path reaches story3;
the existing six-message harbor encounter reaches4, but original022..024 battle
entry, tutorial/full Jump continuation,025..033 postbattle/crowd joins and the
subsequent story progression still need native binding before fresh-file playtests
can reach all later events in order. Continue with those source graphs and the
remaining opening choreography; do not call the full opening or remake complete.
The user's goal remains active.

## 2026-10-13 — Original house-exit fence event and live lecture entry

[Native fence/lecture entry](behavior/story/NATIVE_PROFESSOR_FENCE_AND_LECTURE_ENTRY.md)
executes the original63-command wall-removal event through its story9 close-door
contact+A callback. Door-end wait, temporary-party-to-NPC conversion, original
083/084 messages, cameras/movement/hops and source GSW0=10 run in the existing
VM. Expanded original REL catalog preserves318 commands/63 callbacks across
first-talk, fence and lecture. The source lecture crossing poll remains live
with field controls enabled, then runs084_01/084_02 through its original pending
choice service. Added source `<o>`/once_stop retained-window behavior.

Imported actual OFF_d_saku A_1 nonloop140-frame deformation/seven keys. Native
capture bounds unblock the original integer/aspect commands; original78 fence
vertices/materials deform in a frozen camera-plane adapter. Original map hide
and image display ownership remain distinct from actual A_saku_04 collision
removal. Source story>=10 restores collision removal on reload. Saves/exit
saves remain suppressed while the live event lacks persistence. Asset paths,
adaptations and rights are recorded in [assets](ASSETS.md).

Validation: final shared-worktree `make test` passes104 CTest/100 Python tests;
`make all` builds the3DS binary. Separately exported staged source compiles
host/native and passes102 CTest tests after regenerating its field actor format
in `/tmp/professor-fence-stage`; workspace assets are untouched by that rebuild.
Original-data tests complete first-talk, walk to the close-door contact, wait
for capture/animation, compare collision before/after, reach story10, walk the
opened gap and reach the pending original lecture choice. Independent23-paper-
animation samples pass0.00025 local-unit tolerance, max error0.000213623; the
initial generic0.0002 check rejected that large-fall sample, and ordinary default
tolerance remains unchanged. Key/boundary interpolation sampling was expanded.

The first Azahar run overflowed the native checkpoint-restore stack. Main game,
validation/warp candidates and full reset temporaries now use heap storage;
retry `artifacts/azahar-professor-fence-heap/` restores the checkpoint and shows
original083/084, fence gap and restored controls without that exception. Native
`artifacts/azahar-professor-fence-gap/` replay walks through the gap and reaches
original084_02 retained text and the pending choice service. Earlier straight
replays hit the pipe; corrected navigation uses the opened gap. Initial staged-tree
compile raced the file export and lacked main; rerun after export completes
passes. No physical3DS or original runtime trace was obtained.

Next/limits: bind original selection/add services, both lesson branches, actual
lecture battle/return and story10 reload admission. Source offscreen/GX capture,
rectangle clipping/depth/shadow, image clock and SFX/backend/flag consumers are
still partial; current map-material deformation is an explicit handheld adapter.
Earlier harbor/prologue gaps, temporary-party persistence and hardware
performance still prevent full playable prologue parity.

## 2026-10-13 — Priority0 synth FX resource and chained control cores

Delivered [complete synth request cores](behavior/audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md):
loaded-group/shared-scratch lookup, comparator/binary search, default255 and
all16 sound-start operands, voice-generation checks and fresh chain reloads.
Controller cores stop at stale identity; active-gated KeyOff skips stale nodes
and continues. Returns report requests rather than confirmed hardware changes.
Marked the local EVT-01 portion complete; registry/macro/voice/MIDI/resource/
source/runtime dependencies remain explicit and open.

Validation: generic musyx_synth_effect_core_evidence.json verifier12 spans
(8 complete/4 linked identities),192 anchors and independently resolved15 direct
call sites pass. Fresh new full body inspections remain `/tmp`; existing complete
voice lookup identity is revalidated. Link audit719 Markdown files/0 broken
links; whitespace checks pass. Docs/metadata only: no make rerun, native or
original asset/code-body export. Unrelated staged/worktree edits excluded.

Next/limits: FX registry insertion/removal and project residency, actual
synthStartSound/MIDI/external-keyoff/voice-chain producers, calc3D/live audio and
original scheduler/encounter/audio observations remain open. Continue priority0
producer/source/query binding, then priority1 consumer/counter graph. No original
executed/hardware/full-game fidelity claim.

## 2026-10-13 — Priority0 MusyX effect API and voice/IRQ closure

Delivered [eight complete API/lookup/IRQ bodies](behavior/audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Public effect wrappers retain raw inputs and core returns; StartEx loads a
studio-indexed default byte with no bounds guard. Check returns the original
external ID when internal mapping exists, not the internal index. The original
repeated backend checks retain fresh lookup/IRQ boundaries. Internal mapping
is an unsigned-key linked-list scan with early miss; MusyX IRQ count/state is
separate from SoundEfxPlayEx's nesting pair and retains16-bit wrap semantics.
Marked the local EVT-01 portion complete while leaving core/runtime work OPEN.

Validation: generic musyx_effect_api_evidence.json verifier12 body hashes
(8 complete/4 linked identities) and77 anchors passes. Eight fresh complete
listings remain under `/tmp`; IRQ boundary vectors distinguish0/1/2/65534/
65535. Link audit717 Markdown files/0 broken links and whitespace checks pass.
Docs/metadata only; no make rerun, native code or original asset/code-body
export. Shared unrelated staged/worktree changes excluded from this commit.

Next/limits: core synthFXStart/controller/KeyOff, voice-map/default-table
producers, resources/macros and hardware/OS/source/runtime need recovery.
Continue priority0 calc3D/live audio and query-specific weapon/menu source
bindings, then priority1 damage/counter consumers. No original audible/executed
trace, hardware or full-game completion claim.

## 2026-10-13 — Priority0 pending SFX startup and effect backend contracts

Delivered [complete pending region](behavior/audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md),
[effect PlayEx](behavior/audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md) and
[five control leaves](behavior/audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md).
Queued delay reaches0 before a later start visit; successful startup enters
live processing the same update. Three backend routes retain original volume,
spatial packet, aux and cleanup order. Effect engine uses a separate40-record
pool with first-free/priority<=steal, raw start/controllers, interrupt nesting,
stored narrowed fields, fresh-pool lookup and repeated liveness checks. Check,
Volume,Pitch,Aux1 and Stop preserve their different checks and field widths.
Marked those local EVT-01 portions complete; no full audio-chain completion.

Validation: fresh current SFXMain and six complete effect listings in `/tmp`.
Generic DOL verifier sfx_startup_backend_evidence.json passes25 spans/143
anchors; independent direct-call census50 sites resolves all targets. Arithmetic
check65026 products proves division127;195078 offset/clamp cases pass. Final
link audit716 Markdown files/0 broken links; whitespace checks pass. Metadata/
documentation only: no make rerun, native code, generated tone or asset export.
Unrelated staged/worktree edits remain excluded from the commit.

Next/limits: full calc3D/listener/live volume/fade, stream/song services and
MusyX/resource/emitter/interrupt/hardware/source/runtime still open. Continue
priority0 spatial/core/backend and query-specific weapon/menu source domains,
then priority1 consumer/counter graph. Original execution/audio/hardware/full
remake fidelity remains unverified; no provisional timing is original evidence.

## 2026-10-13 — Tattle flag owners and switch encoding/storage answer

Delivered [complete Tattle flag graph](behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
and rewrote [swdrv storage](behavior/memory/SW_SET.md). Actual rotlwi5 plus
preceding shift27/sign correction implements ordinary signed remainder; valid
nonnegative bit index is modulo32. Init proves global256-word8192-bit and
local16-word512-bit banks,2048/1024-byte banks, and full32-bit global byte0.
ReInit clears only local banks. Tattle setter scans48 lists/247 member occurrences,
writes persistent4474+kind and retained battle bits; checker has policy bypass
and asymmetric original-kind local check. These are not table-population writes.
Marked corresponding EVT-01 and SAVE-01 portions complete; preserved request text.

Validation: fresh14 bodies and table/48 list spans total63 fingerprints;
205 source anchors pass generic verifier with switch_monosiri_flags_evidence.json.
Independent signed arithmetic8951 cases cover global/local/Tattle indices and
signed boundaries. Source groups/kinds and first-match order independently
checked. Link audit712 Markdown files/0 broken links; whitespace checks pass.
Docs/metadata only; no make rerun, native save/code or original asset export.
Inspection outputs remain `/tmp`; unrelated staged/worktree edits excluded.

Next/limits: complete cardMain record/checksum/error/backup/load/timing and
full namespace/writer/scene/save lifetime still open. Continue priority0 SFXMain
used startup/spatial backend, query-specific weapon menu source domains and
original scheduler/encounter traces, then priority1 consumer/counter graph.
No original execution, save round-trip, hardware or complete-game parity claim.

## 2026-10-13 — Temporary Frankly follows the party

Bound [temporary Frankly follow](behavior/partner/NATIVE_TEMPORARY_FRANKLY_FOLLOW.md)
after the original first-talk. He now advances independently through imported
terrain and door travel with original W_1/S_1 poses. Source kind11 uses the same
partyMoveWalk callback; recovered secondary source selection follows Goombella
when ordinary and within150 original units of Mario, otherwise Mario. Added
[used lifecycle/fence admission](behavior/partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md)
and inbox evidence. This does not advance story9 or enable unsupported saves.

Validation: `make test` passes103 CTest and96 Python tests; `make all` builds
the3DS binary. Original-data test completes29-message first-talk then walks100
ticks, checks independent Frankly history/movement, story9 and save suppression.
Selection tests cover the150-unit threshold, missing/special party state and
unordered floating comparison. Fixed the actor test's closed catalog count to
allow additional models while asserting required original model identities;
the initial full test run exposed concurrent save-block catalog expansion.
No new emulator capture or hardware test for following.

Next/limits: bind the source story9 house close callback to fence removal,
original OFF_d_saku paper/image/offscreen animation and A_saku_04 collision
removal, then source story10 lecture/choices/battle. Current portable follower
uses adapted trail/terrain stepping and explicit ordinary Goombella mode/status;
complete source ring/flags/pose timing and temporary save lifecycle remain open.
Earlier harbor story gaps and measured handheld performance still block full
prologue parity.

## 2026-10-13 — Original professor first-talk runs natively

[Native scene](behavior/story/NATIVE_PROFESSOR_FIRST_TALK.md) executes the
original180-command gor REL event in EventNativeRuntime, preserving brothers,
shared waits, actual GSW0 writes, all29 original message IDs/control bytes and
119 callback starts. The original story8 house door starts the event before
its door-end wait; completion reaches story9 and restores field controls.
Bound source cameras, seven Mario event poses, original Frankly/Goombella
animation assets, source map icon314/TPL319 and Frankuri1 stream. Corrected
the source hop to225ms/height30 and scripted travel facing. Save/exit-save
guards suppress unsupported temporary-party checkpoints. Asset provenance and
handheld atlas resampling are recorded in [assets](ASSETS.md).

Validation: full shared-worktree `make test` passes101 CTest/96 Python tests;
`make all` builds the3DS binary. Independent staged-source host/native compile
passes; after regenerating staged-format field actor assets in a separate
`/tmp/professor-stage-verify` tree, all100 staged CTest tests pass. The first
staged test run exposed the concurrent PMANIM-format cache mismatch, rather
than a scene failure; the isolated rebuild does not modify workspace assets.
Original-data tests check29 ordered messages,119 callbacks,17 cameras, source
item/hop/music, door admission, story9 and save suppression. Independent Mario
animation comparison passes102 samples, maximum error8.58307e-06. Azahar
isolated replay/captures at `artifacts/azahar-professor-scene-memory/` shows
original attributed dialogue and restored field HUD. Initial4MiB scene atlas
run failed to restore the east checkpoint; retry with1MiB succeeds. Final
scripted-facing/save-guard changes were rebuilt and host-tested after capture.

Limits/next: temporary Frankly currently remains at his saved source position;
follow/lifecycle/persistence and original story9 door-close fence event are
next. Paper fence animation, reaction/SFX/display-flag consumers, exact turn
interpolation and source movement/collision stepping remain unfinished. Story10
lecture/choice/battle continuation, earlier harbor gaps and hardware performance
remain required. This increment is not complete prologue or original parity.

## 2026-10-13 — Priority0 Monosiri table, consumer and census answer

Delivered [Tattle table/consumers](behavior/battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md)
and rewrote [lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md). Corrected
spatial-sound misclassification, lis arithmetic, impossible-null and safe-caching
claims. Original sampling actually tests pointer0, reloads kind and looks up
again; arbitrary invalid kind can wrap to0. Initialized231-record table supplies
six name/help/log/model/pose/additional-key fields. Complete message-key callback
resolves its unused second argument then raw output before fresh unit lookup,
without actor translation. Direct census:17 DOL calls,2 REL24 imports.

Validation: generic metadata verifier13 spans/40 anchors; sampling verifier7
bodies/48 anchors/14 constants passes. Independent DOL scan and US usage audit
agree17 linked/0 nonlink/0 data-word candidates plus2 imports across29 RELs.
All231 table records/column counts and original example identifiers inspected;
wrapped-zero derivation checked. Inspection files and reports remain `/tmp`.
Link audit708 Markdown files/0 broken links; whitespace check passes. Docs and
metadata only: no make rerun, native change or original asset/dialogue export.
Unrelated staged/worktree edits are preserved outside the commit.

Next/limits: table mutation/alias/valid-kind and full Tattle/log/AC/REL/event/
save/runtime binding remain open. Separately observed flag setters/readers use
persistent switches and battle work, not table-population writes. Recover that
shared flag graph and correct the SW_SET rotate-by-five interpretation, then
continue priority0 source/menu/audio/runtime requests. No original gameplay/
hardware/full-game equivalence claim.

## 2026-10-13 — Original Hammer charge cue resource answer

Delivered [two charge cue bindings](behavior/audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md)
and checked that portion of AC-02. Actual callers8012a4b0/4d8 resolve PI1/PONE1
through table ordinals2619/2620, FX862/861, group6/macros995/994 and original
samples776/775. Directory rate16000 and nonloop1526/5276 frame lengths are
static dependencies, not a complete cue duration or substitute WAV playback.
Three original bank asset hashes and derived metadata stay in a source manifest;
no original macro bytes/PCM/ISO or new asset conversion enters history.

Validation: `/tmp/verify_hammer_evidence.py` reparses original ISO rows/project/
pool/directory, passing3 assets/2 cues/2 macro bindings/2 sample records and
all1247 directory records. Generic DOL verifier passes14 spans/85 anchors,
including full Hammer main/two caller instructions; complete behavior verifier
passes (`/tmp/inbox-hammer-behavior-check.log`). Both existing ignored PCM
counterparts exist. Link audit706 Markdown files/0 broken links; whitespace
check passes. Documentation/metadata only; no make test/all rerun. Unrelated
staged/worktree edits preserved outside this commit.

Next/limits: full SFXMain spatial/start/volume/effect backend/MusyX execution,
EVT-01 query-specific menu/source/global domains and original scheduler/audio
traces remain open. AC-02 lifecycle/display/forced/suppression writers and
original charge playback remain unchecked. Continue by inbox priority; no
original runtime/hardware/full-game equivalence claim.

## 2026-10-13 — Priority0 positional-SFX correction and update producers

Reinspected and rewrote original [provisioning](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
[stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md) and [accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md).
[Update/correction delivery](behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
closes full psndMain cooldown producer and scoped SFXMain completion/pending/
preference-reset paths. Corrected low13 table index, pending flag direction,
row+4 high backend bits, numeric/name ABI, mute override,40-slot free search,
strict reclaim priority, live255 defaults, independent delay/offset stores and
coherent16-bit generation wrap. Prior conflicting historical deliveries are
explicitly superseded; original request text remains unchanged.

Validation:9 complete body listings independently regenerated under `/tmp`.
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`
passes13 DOL body/table/numeric spans and78 instruction anchors. Link audit
checks704 Markdown files with0 broken links; `git diff --check` passes.
Metadata only; no tool/native code change, so make test/all not rerun. Read-only
ISO and user emulator profiles/saves untouched; no original code/assets exported
into history. Commit excludes unrelated staged/worktree changes.

Limits/next: SFXMain middle/spatial/startup and Sound/MusyX resources/macros remain
partially recovered; original execution and other source writers stay open.
Next recover full used effect startup and two Hammer charge cue macro bindings,
then continue EVT-01 menu/global/source and original scheduling evidence.
Full game documentation/remake and hardware parity remain incomplete.

## 2026-10-13 — Original east Mouser talk and per-area actor resources

Feature commit `8cdab4a` adds stationary east Chu3/Mouser using original
`m/gor_02/s` XYZ/tribe/direction, `a/c_tyusan` / texture bank and `CHU_S_1`
sparse animation. Its simple original talk event selects CASE_LESS branches
at56/112/164/215/261/339/373/401; original US IDs/payloads/control tokens and
actual actor attribution are retained with no story write. Current runtime
coverage is explicitly story0..55, not a recovered visibility rule. Native
proximity0.65/height0.25 and grounded A-before-door/jump priority remain
provisional until original search/arbitration binds. No original story is
inferred from absent native save data. Original Frankly story8/10 init positions
move into the same catalog; his complete scenes remain separate owners.

The portable catalog validates model namespaces, story/interior gates and
whole records atomically. Destination-area models stage with geometry; failed
imports retain the resident area. Animation samples persist per model and cache
simulation ticks/poses; hierarchy arrays retain capacity, recursive dispatch
avoids std::function allocation, and root trig runs once per instance. GPU
texture batches use citro3d linear buffers and whole-mesh capacity admission.
Visible actor/vertex/sample-failure/overflow counters expose bounded submission.
See [native scope](behavior/field/NATIVE_AREA_ACTOR_CATALOG.md),
[source bindings](behavior/field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[provenance](ASSETS.md). Initial transform helper commit is `c46ee29`.

Validation: isolated source snapshot `/tmp/pm-field-verified-at8uf8ls` of the
feature changes passes `make test`: **99 CTest / 96 Python tests**
(`/tmp/goal-field-isolated-test.log`), and `make all` without warnings
(`/tmp/goal-field-isolated-build.log`). Snapshot excludes concurrent opening/
professor additions and uses local ignored original resources. Original-data
tests cover18 branch-boundary stories/all9 unchanged US payloads, actual talk
entry/closure, speaker identity, no story/save mutation while talking, original
room fixture ground/wall admission, and sampled model streams. Catalog tests
cover malformed/unsafe/overlapping records and retained state on failed reads.
Earlier shared runs also passed100 CTest/96 Python; shared native builds briefly
failed during in-flight frontend/audio/professor edits. Those intermediate
failures are not claimed as feature validation; the isolated build is passing.

Immutable ignored `artifacts/ttyd-field-actors.3dsx` is the validated snapshot
binary. Isolated Azahar run uses `tools/make_field_actor_fixture.py` and
`tests/azahar_field_actor_talk.py`, output
`artifacts/azahar-field-actors-verified/`, log
`/tmp/goal-field-verified-smoke.log`. Reviewed original-Mouser talk and final
return captures: source art, speaker-reaching tail, restored exploration/menu
and actual manual save with unchanged story8/checksum. The script requires a
new save timestamp after source-window closure, avoiding repeated-A reopening
as false success. Xvfb required display-socket escalation; user profiles/saves
and ISO were untouched. This is native scene validation, not Dolphin or hardware
parity/performance evidence. No new Dolphin observations were taken.

Documentation worker recovered ordinary NPC and save-block bindings and added
inbox breadcrumbs. Save-block original spawn `(185,60,-55)`, scalar10 and
hit/system-event graph are documented in
[source binding](behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).
Conversion exposed S_1 UVs outside0..1 plus scrolling texture keys; the ordinary
compiler correctly rejects that domain. The incomplete probe pack was removed
and no save-block record ships. Bind original animated texture matrices and
clamp/material services before adding its visual/hit/save implementation.

Next: close original NPC search/priority/facing/collision and shadow/material
services; bind Goomba wandering and tavern NPC fade; integrate professor/fence/
lecture scenes through their source owners; implement save-block texture/hit/
card lifecycle. Gather dense-room/movement timing traces on New2DS hardware and
matched Dolphin observations. Broader prologue/game fidelity remains incomplete;
the user's goal stays active. Other concurrent worktree/staged changes are
preserved outside this feature commit.

## 2026-10-13 — Priority1 initialization/count and priority2/3 inbox portions

Delivered [DMG-01 initialization/count composition](behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md):
full local Entry/UnitSetup, signed HP/FP copy and later imports, distinct body-part
selectors, setup count0/result1, four encoded TimingA increment routes and
read-only shared count query. Independently corrected result-reader battle/output
capture order and pending bit0/mask1 wording. Marked scoped delivered portions
in DMG-01, AC-03, AC-01 and the shared STATUS-01 initialization prerequisite;
request text remains unchanged and parent integration/runtime work stays OPEN.
Priority4 AC-02 full lifecycle/display/macro bindings remain pending.

Validation: full Entry verifier33 bodies/441 anchors/11 spans; UnitSetup8 bodies/
190 anchors/5 spans; Jump graph268 commands/19 bodies/57 anchors/5 concurrent
blocks; input cadence11 hashes/154 anchors/28 calls/3 data bindings; boss Jump
10 fingerprints/55 instructions/9 constants; lecture helper roles and complete
general behavior verifier pass. Setup/TimingA/count/result reader listings
regenerated to `/tmp/inbox-*.elf`, read-only ISO. `python3 tools/check_doc_links.py`:703 Markdown files,0 broken links;
`git diff --check` passes. Documentation only:
`make test`/`make all` not rerun. Unrelated staged/native edits remain excluded.

Limitations/next: actual loadout/stat/bonus and remaining counter writers, full
consumer/reward/defeat/tutorial eligibility graph, query-specific cursor domains,
source/platform services and original multihit/input/RNG/pause/turn traces still
need recovery. Continue priority0 remaining source/runtime acceptance, then
priority1 consumer/counter owners, priority2 global defense/forced-counter gates,
priority3 tutorial writers, priority4 Hammer display/audio and priority5 turn
lifetime. No new Dolphin observations, hardware or complete-game fidelity claim.

## 2026-10-13 — Priority0 documentation inbox delivery

Consolidated [EVT-01's local service contract](behavior/event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md)
and corrected the [six unit-query leaves](behavior/battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md).
Independent read-only US ISO inspection confirms HP's retained early battle,
ID-1-before-lookup/output gate, work's late lookup, and distinct status/data
capture order. Split EVT-01 todos into checked static portions and open runtime/
source requirements; preserved request text and registered the delivery.

Validation: nine source verifiers pass: event manager/root/descendants/dispatch
 tail/run-controls, weapon menu domains, ActMenu, menu interruptions and Jump
source registry. Registry confirms3 streams,156 direct sites and72 callbacks.
Six complete leaf listings were regenerated to `/tmp/inbox-*.elf` only.
`python3 tools/check_doc_links.py`:701 Markdown files,0 broken links.
`git diff --check` passes. Docs only; no make rerun or native code changes.
Existing unrelated worktree edits were preserved and excluded from this commit.

Limitations/next: EVT-01 stays OPEN for original pause/multicredit/creation/
deleting/wait traces, query-specific unsupported cursor slots, global/dynamic/
transitive sources and backend domains. No original observation or hardware/
full-game equivalence claim. Continue priority1 DMG-01 allocation/stat/init and
attack-count ownership, then AC-03 defense integration in inbox order.

## 2026-10-13 — Rebuilt movement timing sample

Added the renderer declaration required by in-flight front-end code so the
current tree builds; no performance behavior changed. `make all` succeeds. The
rebuilt 3DSX capture under `artifacts/azahar-perf-current-plaza/` holds60 FPS
after a camera move and reports0.79ms simulation,5.1ms CPU and0.5ms GPU. The
ordinary plaza scene includes Mario, Goombella and nearby NPCs. This is a single
software-emulator sample, not a percentile trace or hardware measurement.

An east-room fixture attempt under `artifacts/azahar-perf-current-east-valid/`
was rejected by native restore validation and displayed “Saved room unavailable;
started in plaza”; it is not an east-actor timing sample. No substantial
optimization is indicated by the valid capture. Next: construct a verified
current-format east-room fixture, then gather repeated timing samples across
animated harbor/east scenes and cutaway-active movement. Hardware profiling is
still required; performance review remains open.

## 2026-10-13 — Current renderer workload and build limitation

After the field actor/frontend source update, the current `romfs/manifest.json`
records plaza at40,714 triangles/150 batches, harbor22,922/87, east33,444/145,
west34,885/127 and station41,084/54. Plaza has26 cutaway groups with1,425
triangles total (maximum186 in one group). The renderer performs bounds checks
before triangle intersection tests, so cutaway testing is a bounded workload;
full map geometry and batch submission are more plausible GPU/driver costs.
This is a static workload assessment, not a measured bottleneck. Existing
128-pixel map texture caps and atlas reductions remain in effect. No behavior or
asset changes are justified without profiling evidence.

Ran `make all` against the current shared worktree; it fails in
`source/render.cpp:794` because the new `Renderer::renderFrontend` definition has
no declaration in `include/platform.hpp`. This frontend integration is in-flight
work outside the performance review; no source files were changed to work around
it. Because the build failed, the prior Azahar captures exercise an earlier
executable and cannot verify this updated renderer. Next: once the interface
integration is complete, rebuild and repeat moving-scene measurements with the
timing overlay visible, then profile on the target 2DS XL before selecting any
substantial optimization. Do not mark performance complete yet.

## 2026-10-13 — Emulator performance review

Reviewed the renderer's per-frame costs: map animation vertex updates/cache flushes,
material batch submission, actor pose evaluation, and triangle-level exterior
cutaway checks. Existing size reductions are documented in [asset provenance](ASSETS.md):
source maps cap textures at128 pixels, and original character atlases trim unused
regions. No code optimization was justified by this pass; preserving original
geometry, animation, culling and gameplay behavior takes priority until a measured
hotspot is found.

Isolated Azahar software-renderer captures use separate profiles under
`artifacts/azahar-perf-plaza/` and `artifacts/azahar-perf-professor/`. The plaza
boot overlay showed60 FPS,1.63ms simulation,5.1ms CPU and0.5ms GPU. The professor
story8 fixture showed60 FPS,4.15ms simulation,5.3ms CPU and0.5ms GPU. The captures
are brief overlay samples, not percentile/frame-time traces. The attempted
walkthrough completed, but its movement capture hid the timing overlay behind a
development notice, so it provides no moving-camera timing result.

No `make test` or `make all` was run because this was a read-only performance
review and the shared worktree contains unrelated in-flight implementation edits.
The emulator reports steady60 FPS with CPU/GPU headroom in these scenes; no
reproducible emulator dip was observed. Software Azahar timing does not establish
real 2DS XL performance. Next: collect repeated per-frame timings on hardware and
capture cutaway-active movement without overlays before changing hot paths. If
original-equivalent behavior and measured hardware performance conflict, track
the affected behavior as a high-priority fidelity/performance task rather than
silently changing it.








## 2026-10-13 — Original Frankly animation and first-talk message prerequisites

Added the original c_kuri_h model/texture bank and six real clips
Z_1,S_1,T_1,W_1,R_1,D_1 to the local animation pipeline. Native story8 professor
interior draws original animated S_1 at source init(-135,0,-240). The12-cell
RGBA8 atlas crops unused rows to1024x256 without resizing those cells or changing
colors, saving3MiB of texture memory; a checked V scale adapts the existing
animation pack UV domain. Original animated geometry/hierarchy/visibility stays
intact. Actual bank D_1 versus tribe literal d_1 is explicitly recorded with no
invented alias. See [assets](ASSETS.md) and
[complete scene evidence](behavior/story/PROFESSOR_FIRST_TALK_AND_FENCE.md).

Message parser/playback now binds dkey and its close: source0xfff5 payload1/0,
nonzero sets0x80/clears acceleration/anchors elapsed to record schedule; zero
clears0x80 only. Timed source waits stay active under repeated A input. Tests now
parse/play all29 original first-talk payloads, including resolved party `_kur`
keys, original malformed-looking `</dynamic 3>`, styles/colors and unchanged
raw bytes. No narrative text or substitute scene is authored.

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/professor-tests.log`); `make all` builds native3DSX with no warnings
(`/tmp/professor-build.log`). Explicit original-data message test passes20
harbor+29 professor payloads. Independent animation reference generation and
`actor_visibility_tests romfs/professor.pmanim artifacts/professor-animation-reference.bin`
pass45 source samples, maximum host model error1.90735e-06. Native input-gate
regression verifies acceleration reset, real elapsed wait and timed reopening.
Document audit checks695 Markdown files with0 broken links; diff whitespace
check passes. Isolated Azahar story8/interior2 fixture under
`artifacts/azahar-professor-idle/` boots inside the original house with Frankly
visible; reviewed boot.png. Fixture is a native development save, not original
GameCube progress. User emulator profiles and read-only ISO are untouched;
all original/generated assets stay ignored and outside source history.

Limits/next: this is character/message prerequisite work, NOT complete
first-talk/fence execution. NPC yaw/clock/scalar and init visibility are scoped
native adapters awaiting producer/backend binding in OW-ROAD-01. Source
first-talk's initial position override, concurrent brothers/shared wait, actor
movement/poses, cameras/music/effects/item display, temporary Frankly party1 and
story8→9 remain to execute. Fence removal must proceed9→10 and dispatch the
original lecture, whose common return is11. Consider the existing EventNativeRuntime
for the original REL script rather than manually flattening concurrent commands;
DOL-only pack_event_scripts needs REL-section relocation/data provisioning first.
Broader arrival/tutorial/story progression and hardware performance remain
unfinished. Goal stays active.


## 2026-10-13 — Harbor partner fall-through and original follow distances

Reproduced the user's harbor ledge complaint on original converted geometry:
the old center/crossing landing query could reject valid support and fall below
the dock until recovery copied Mario's root. Read-only original contracts prove
five-probe support BEFORE fall integration, current-velocity drop admission,
normal homing floor-loss acceleration-1 versus stay-1.5, velocity cap-16 and
horizontal damping0.2 followed by1.3*retained speed/direction. Native support now
uses Goombella's original width26/height24, ordinary partner attribute filtering,
Q1000 pointY and highest admitted support. Wall occupancy no longer rejects
valid landing; only Mario's jump flag marks history jumps.

Corrected an initial distance interpretation during validation:45/50 are speed
thresholds, not a fixed trailing gap. Original q0 activity normally remains
nonzero through dash-base2.25 fallback even at idle. Replaced fixed0.5 trail
spacing with width/speed history reach13+speed, separate wake-up50/idle100,
source speed selection/thresholded bonus and ordinary overlap box/corrections.
Restore now uses a0.3 leftward native seed based on general placement's30-unit
magnitude instead of the provisional0.8; full facing/search placement is unbound.
Evidence/contract commit `41b0e6d`, implementation/fixtures `9672cd6`; see
[ground-follow distances](behavior/partner/FIELD_PARTNER_GROUND_FOLLOW_DISTANCES.md)
and [native binding scope](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md).

Validation: final `make test` passes **95 CTest / 96 Python tests**, log
`/tmp/partner-fall-final-test.log`; final `make all` builds3DSX without warnings,
`/tmp/partner-fall-final-build.log`. Actual harbor regression uses two deck
positions,90 idle ticks, a verified admitted Mario jump and300 following ticks;
partner minimumY>=0.149, zero resets and grounded0.15 arrival. Portable tests
cover support-before-gravity, side-probe landing independent of wall occupancy,
retained speed damping, history reach, overlap, dense jump marks and blockages.
The old idle catch-up test's100-tick dash assumption was changed to260 ticks
for the original1.5-unit idle bonus cap. `core_tests --original` passes18,315
indexed/full-scan probes and all original-room scenarios, log
`/tmp/partner-fall-original-final.log`. Extended homing verifier passes64
instructions/12 constants; ground, direction, fall, jump, lifetime and front-wall
verifiers pass against the read-only ISO.695 document links and diff check pass.
One intermediate Python visibility test failed during concurrent map changes;
the final full run passes after that concurrent fix. Door/save/message work was
preserved and excluded from the staged partner hunks.

Isolated Azahar final capture passes with15-second startup, original converted
harbor geometry and `tests/azahar_partner_harbor_fall.py`, under ignored
`artifacts/azahar-partner-harbor-final/`; log `/tmp/partner-fall-final-smoke.log`.
The input script samples the short fall window and then reveals Goombella by
moving Mario sideways. Inspected `partner-dock-side-view.png` confirms both
characters grounded on the dock. Earlier straight-line pictures hid Goombella
behind Mario; temporary isolated tracing proved minimumY0.15 and zero resets.
All temporary instrumentation was removed before the final build. A reproducible
native fixture generator is `tools/make_partner_harbor_fixture.py`; no original
save/ISO or compiled asset is committed. All profiles/saves are isolated; Xvfb
needed display-socket escalation and the user's emulator state was untouched.

Limitations/next: this fixes the reproduced dry harbor descent using source
physics and distance rules, not full instruction-exact party locomotion. Native
vector history, exact smoothing-call multiplicity/trig/rounding, ray/cache/flag
ownership, forced/fly/ability modes and dynamic surfaces remain adapters. Bind
full80-record ground homing, partner wall services and placement3/4 (+37 for fall
recovery), then compare original Dolphin jump/fall/cadence and partner distances.
No new Dolphin observations or hardware claim; full prologue/game fidelity remains
incomplete. Original art and existing original sound assets were retained.

## 2026-10-13 — Stretch bubble tails to the visible speaker

User requested a reaching tail rather than the original fixed window-relative
length. Native story drawing now extends the tail from its existing bubble
base to four screen pixels before the projected talk anchor. Original sheared
rows, base/width, placement and suppression gates remain; too-close anchors
suppress the tail. White/shadow geometry independently compensates the original
48x64 masks' transparent margins, so both last opaque texel centers reach the
same visible endpoint and the shadow does not overshoot the head. Original
raster assets, IDs/raw dialogue/controls/timing and4:3 proportions are unchanged.
The four-pixel gap is an authored native adaptation, not measured original
behavior. See[native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and[asset conversion notes](ASSETS.md).

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/tail-stretch-test.log`); `make all` passes without warnings
(`/tmp/tail-stretch-build.log`). Original-data20-payload playback and six-speaker
harbor integration tests pass. Added left/right/inverted stretch checks for
retained base, four-pixel gap, white/shadow endpoint equality and close-anchor
suppression. Document audit checks694 Markdown files with0 broken links;
`git diff --check` passes.

Isolated `tests/azahar_textboxes.py` capture with the native story3 fixture and
immutable `artifacts/ttyd-tail-stretched.3dsx` completes and saves story4 under
`artifacts/azahar-tail-stretched/`. Reviewed Goombella page08 and X-Naut page14:
the visible tails now extend from the bubble toward their heads. No diagnostic
marker remains; user emulator profiles/saves and original ISO are untouched.

Limits/next: this tail extension follows the user's presentation preference,
not original fixed-endpoint parity. Pose/image-derived talk anchors and
handheld aspect remain adapters. Original NPC heights/projection, full window
services/indicator and matched Dolphin/hardware verification remain open.
Broader dialogue/story/full-game fidelity remains incomplete.


## 2026-10-13 — Save and reload playable interiors

Native PM2D version4 adds the completed inn/professor interior mask while
retaining40-byte records and version1/2/3 loading. Story remains a separate
optional signed word; no original story is inferred or changed. Unknown masks,
room/mask mismatches and reserved bytes reject atomically. Native temporary-file,
backup recovery and checksum behavior is preserved. This is a native format,
not a recovered original card format; [SAVE-01](requests/SAVE-01.md) stays open.
See [format contract](behavior/format/NATIVE_SAVE_FORMAT.md).

Game restore applies saved door collision/display state before floor/wall
validation in the native startup transaction. Completed interiors permit the
existing manual save; in-flight door choreography still suppresses saves. A
room transition resets the previous room's door mask. Original-data tests enter,
save/reload and exit both buildings through their actual close contacts.

Validation: final `make test` passes **96 CTest / 96 Python tests**
(`/tmp/indoor-save-final-tests.log`); `make all` builds native3DSX
(`/tmp/indoor-save-final-build.log`); `build-host/field_door_tests original`
passes both source rooms after save/reload. New native-save tests cover old
versions, invalid masks/reserved bytes, actual version3 file loading, version4
writing/loading and backup recovery without mutating decoded state on failure.
Document audit checks694 Markdown files with0 broken links; diff whitespace
checks pass. Isolated Azahar `tests/azahar_indoor_save.py` asserts version4,
room0/interior1 in the actual saved file. A second launch with that isolated
save reloads inside the original inn; reviewed
`artifacts/azahar-indoor-save/boot.png`. User profiles/saves remain untouched.
Original assets/packs remain local and ignored. Validation runs on the current
shared worktree; concurrent partner/message changes are kept separate from
this incremental save commit.

Next: implement the original professor story8 first-talk with recovered
ordered US message keys (party print expands to `_kur`), original Frankly
`c_kuri_h` art/poses and source camera/actor chronology. Source `dkey` parser/
playback behavior is documented but not yet bound; `col` is already supported.
Then implement story9 close/wall-removal and its story10 lecture dispatch, not a
fabricated terminal story state. Door animation/light/audio, indoor NPC/item
activation and upstairs doors remain unfinished, as do the broader prologue
arrival/tutorial/story transitions and hardware performance. Goal stays active.


## 2026-10-13 — Correct tail direction and preserve original4:3 bubble proportions

Fixed the observed Goombella tail miss in79c15b1. Tail masks now draw through
source sheared quads (two textured triangles per pass), retaining horizontal
base/end edges and avoiding rotated transparent-margin expansion. A dedicated
12-vertex buffer preserves the separate shadow/white submissions. Flush and
restore citro2d around the tail GPU pass. Bubble/glyph scaling is uniformly0.5,
fitting original640x480 presentation into centered320x240 with40-pixel margins
on the400-wide top display. World drawing still fills the wide viewport.

Speaker projection now reuses the actual tilted world transform and converts
its clip coordinates to the 2D screen plane. Goombella talk position uses her
evaluated original pose bounds; Crump/X-Naut use opaque bitmap top/center
instead of padded billboard tops. Original print-entry roots/placement caching,
speaker/message attribution/order and control playback remain intact. Ignored
PMHNPC2 actor metadata stores the new offsets; source paths/conversion/rights
are recorded in[assets](ASSETS.md). See the updated
[native bubble contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md).

Validation: latest shared `make test` passes **95 CTest / 96 Python tests**
(`/tmp/tail-fix-test.log`); final production `make all` passes without warnings
(`/tmp/tail-fix-final-build.log`). Original-data playback20-payload and harbor
six-speaker integration tests pass. Added steep left/right/inverted tail cases
verify horizontal edges, width, mask coordinates and base-to-speaker direction.
Document audit checks693 Markdown files with0 broken links; diff check passes.
An earlier shared run failed the concurrently changing partner core check.
A clean archived baseline plus the tail host changes independently reproduced
partner/core failures (`/tmp/tail-fix-isolated-test.log`); message tests passed.
The later shared run after partner/test updates is the passing full validation.

Isolated captures in `artifacts/azahar-tail-fix-aspect/` complete story4.
`artifacts/azahar-tail-anchor-diagnostic/` temporarily marks the talk anchor:
reviewed Goombella's marker on her visible head and the white mask centerline
aiming there. Original shadow artwork extends beyond the white mask. The marker
was removed before the final production build. Final marker-free captures use
`tests/azahar_textboxes.py`, native fixture and immutable
`artifacts/ttyd-tail-fixed-final.3dsx` under `artifacts/azahar-tail-fixed-final/`;
script checks story4 persistence and overlay restoration. User emulator profiles
and saves are untouched; ISO input stays read-only and packs/images ignored.

Limits/next: pose/image-derived anchors are native adaptations; original NPC
tribe height+150 and original getScreenPoint services remain unbound. Original
shared RNG, full window types/indicator/history/selection/icon/pose/paper/audio/
noise and matched Dolphin/hardware comparison remain open. Check additional
actors/cameras as more story content is bound. Full dialogue/full-game fidelity
is not complete.


## 2026-10-13 — Original same-map inn and professor door access

Bound the plaza inn/bar ground-floor and east professor house as original
same-map doors, preserving the five existing room IDs. The documentation
agent delivered [door evidence](behavior/overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
and appended the [request inbox](requests/OW-ROAD-01.md). Source admission uses
registered forward hit identity plus A and ordinary motion/common case gates,
with no invented radial door trigger. Extracted first hit-triangle normals and
original hit centers produce40-unit-offset, EVT-integer destinations. The native
fixed-tick bridge uses the original135-unit/sec request and500ms waits; it
consumes A before the native jump harness and preserves the story word.

Original inner subtrees start hidden and are revealed on entry. The professor
interior collision subtree enables/disables across wall, floor, overhead and
shadow queries, with a cached mask change rather than per-frame asset rebuilds.
Inn's original null collision field stays null. Source geometry is retained;
outer subtrees use an explicit provisional indoor cutaway until the original
flag2/0x800 fade consumers are documented. See [conversion/licensing](ASSETS.md).
The ignored doors.pmdoor pack and doors.json manifest contain only locally
converted original data; neither the ISO nor extracted assets are committed.

Validation: final `make test` passes **95 CTest / 96 Python tests**
(`/tmp/interior-final-tests.log`); final `make all` builds the native3DSX
(`/tmp/interior-final-all.log`). `build-host/field_door_tests original` tests
actual original contact admission, both buildings' entry/exit, unchanged story,
and interior-save suppression. Geometry partition tests preserve all vertex
records and byte-identical collision packs. Document link audit checks693
Markdown files with0 broken links; working/index diff whitespace checks pass.

An initial default-binary Azahar run displayed a generic load error and is NOT
counted as a gameplay pass. An isolated build then loaded and revealed an
exterior-wall occlusion problem; the explicit cutaway fixes it. Reviewed
`artifacts/azahar-inn-cutaway/inn-inside.png` and `inn-exited.png` under the
isolated profile: original bar interior, visible Mario/Goombella and restored
exterior on exit. Professor access is original-data host tested, not yet visually
compared against Dolphin. Captured cutaway build precedes the final central save
suppression guard; final `make all` and tests include that guard. User emulator
profiles/saves remain untouched. Concurrent partner/message refinements remain
separate working-tree work; this increment stages only its own changes.

Limits/next: original door rotation/light/dark/sound/letterbox services, indoor
NPCs/items, inn upstairs callbacks, professor story8 first-talk and story9 wall
removal are not executed yet. Camera and relative partner movement are explicit
native adapters. Interior saves are suppressed to preserve the existing outside
save until a versioned door-state extension is implemented. Next bind those
services and original dialogue, expand door registration to the other source
records and verify professor captures. Harbor arrival/tutorial/plaza/east story
transitions and hardware performance remain incomplete; the goal stays active.


## 2026-10-13 — Original-style speaker bubbles and dialogue overlay suppression

The harbor checkpoint now renders the original `f/msgWindow.tpl` ordinary
border/shadow/tail masks, with source default(-280,225,560,176) geometry adapted
to the400x240 top display. Print-entry speaker roots and cached left/right/
offscreen placement use the recovered contracts; source tail endpoints aim
at the attributed speaker through the current native camera. Bubble opening/
closing interpolates from the speaker and ordinary anchored glyphs draw at
full opacity. See [native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and [local asset conversion/licensing](ASSETS.md).

Expanded parser controls with pos, scaleX, wave, col and closing controls,
small, clear and wpos. Existing key/page/wait/speed/scale/dynamic/shake tokens
and original ID/raw-byte preservation remain intact. Shake is now rendered;
wave uses unaccelerated simulation-clock time and dynamic follows original
draw-time relaxation. Partial glyph UV cropping keeps enlarged effects visible
at the window boundary without leaking GPU scissor state. Ordinary source
vertical admission precedes effects; clipped dynamic records still relax.
Unsupported service controls reject atomically instead of dropping tokens.

Top HP/location/interaction prompts and bottom menu/captions/buttons/notices/
FPS disappear for ordinary dialogue and the entire locked harbor scene.
A/B/touch still advance the original scene, and overlays restore on completion.
Development notices use the original ordinary border without fabricated NPC
attribution. No original dialogue was authored or rewritten.

Validation: final `make test` passes **94 CTest / 96 Python tests**
(`/tmp/textbox-test.log`); final `make all` builds3DSX without warnings
(`/tmp/textbox-build.log`). Original-data `message_playback_tests original`
parses/plays all20 opening payloads; `harbor_story_tests original` passes six
source speakers/order/positions and story4 completion. Source contract verifier
passes (`/tmp/textbox-source-contracts.log`); document audit checks691 Markdown
files with0 broken links; `git diff --check` passes. Tests include control layout,
rollback, source geometry/selector/equality/offscreen/tail gates, partial clipping
and speaker snapshots. An initial new test incorrectly assumed right-side
placement implied an endpoint on the right of its base; corrected it to check
the recovered base-to-speaker interval. The initial assertion failure is not
counted as a passing validation.

Final isolated Azahar capture runs `tests/azahar_textboxes.py` using the native
story3 fixture under `artifacts/azahar-textboxes-final/`. Reviewed opening,
Crump page/Goombella responses, and final story4 overlay restoration; the script
checks saved story4. The earlier rapid walkthrough also passes under
`artifacts/azahar-textboxes/`. Sandbox Xvfb first failed display-socket access;
the approved isolated run succeeds. User emulator profiles/saves are untouched.
Original ISO/extracted raster assets/packs remain read-only-source/local/ignored.

Limits/next: native eye/aim/FOV projection, Goombella0.55 height, extracted
Crump/X-Naut billboard tops and separate shake RNG are explicit adapters,
not measured source behavior. The tail uses a rotated original mask rather
than the exact sheared GX quad. Complete NPC tribe heights/getScreenPoint,
original shared RNG, full window types/history/selection/icon/pose/paper/audio/
noise, original star indicator and matched Dolphin image comparison remain
open. Arrival/tutorial/full-game story services and hardware performance remain
unfinished. Next bind those consumers and compare original captures; do not
claim complete dialogue or game equivalence from this slice.


## 2026-10-13 — Combined dialogue-window support and isolated validation

Concurrent work committed the harbor/font foundation as48d8c73 and extended
the shared message renderer while this session was preparing its commit.
ff225fa picked up that extension; bd4abf8 adds its required
`message_window.hpp` and `message_window_assets.py` support files, so the
committed dependency set is complete. Original ordinary bubble/tail masks now
replace the plain native panel. Source-derived anchor/box/tail behavior and
additional retained style controls improve presentation; specialized window,
NPC anchor/pose and original shared RNG/material fidelity remain unfinished.

Validation of the combined snapshot: **94 CTest / 96 Python passing**,
original20-payload and six-speaker harbor integration checks pass. A shared
native build raced another linker and produced an invalid-ELF packaging error;
isolated `make BUILD=build-harbor-verify
TARGET=artifacts/ttyd-harbor-verified all` passes. The immutable output at
`artifacts/ttyd-harbor-verified.3dsx` completes the original story3 checkpoint
and saves story4 in `artifacts/azahar-harbor-combined-verified/`; manually
inspected first-dialogue image shows original font/bubble masks. New
`emulator_smoke.py --binary` supports such isolated build verification without
replacing another workstream's artifact or touching user emulator profiles.
Current concurrent uncommitted refinements are separate from that validated
snapshot. Next: validate those refinements, then continue arrival/tutorial,
original NPC/event/window services and indoor access. Full prologue parity
remains incomplete.

## 2026-10-12 — Original harbor dialogue checkpoint and US font

Added a source-derived story3 harbor sequencer for original mac_0_016..021,
with the recovered position gate (integer decoded Z<=-100), exact message IDs/
raw payloads/speaker order, independent delayed actor moves, source camera
requests and terminal story4 write. It is enabled only by explicit native
originalStory=3 in harbor; legacy/new harness state does not invent a story
checkpoint. The gate has no added A/grounded condition, and neutral field
physics continues an existing jump while the scene blocks new player actions.

Opening glyph/key/page/wait/speed/scale/dynamic records and finite ordinary
window fade/acceleration/scroll behavior are portable. Original US BFN glyph
masks, mappings, width patches and row offsets render through citro2d, retaining
all original message bytes/tokens. Speaker captions are presentation labels,
not rewritten dialogue. Original c_kanbu_ibt/c_zako_n setup art and the exact
BGM_EVT_DANGER4 stream are packaged locally; see
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md) and
[source choreography](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md).

This remains a development checkpoint, not complete scene/REL VM fidelity:
NPC numeric stepping/collision/automatic poses use a documented temporary
speed adapter; Crump/subordinates use original static setup art. Mario
set_dir/reardir, nested key/GSF persistence, environmental/NPC/text SFX, crowd,
original bubble/history/selection/paper/shake/noise services remain unbound.
Story2 arrival/resume and story4 tutorial continuation/restore are unfinished.
Fresh launch remains the plaza harness. No new fabricated dialogue was added.

Native checks parse/play all20 opening payloads with their exact source font
and retained bytes; integration checks the six speaker/ID order, position-only
and airborne admission, independent movement/owner completion, save suppression
and story3→4 boundary. Isolated Azahar captured original font/dialogue and
persisted story4. The first test checkpoint was on water and lost1 HP; the
final helper places it on independently probed dry harbor ground. Source camera
and NPC timing are static-derived/adapted, not Dolphin observations.

Validation: `make test` **93 CTest / 96 Python passing**, `make all` builds
3DSX without warnings; original-data `message_playback_tests original` and
`harbor_story_tests original` pass. `check_doc_links.py` checks690 files with
0 broken links. Final isolated checkpoint capture is
`artifacts/azahar-harbor-story3-physics/`, driven by
`tests/azahar_harbor_story.py`; final story4 persistence passes. An airborne
regression test first exposed a missing air-motion dispatch in the dialogue
physics bridge; adding that ordinary dispatch lets the jump land while the
original key wait remains active. User emulator profiles/saves were untouched.
No original-runtime or hardware performance observation was made.

Next: bind original arrival and tutorial continuation, full message/window/NPC
services, original event runtime composition and indoor entrances/maps. Full
prologue parity and hardware performance remain incomplete.


## 2026-10-12 — Partner 3D following, jumps, falls and wall/path recovery

Recovered selected missing `party_motion_homing_walk` branches directly from
read-only US G8ME01: grounded history flag1 admits jump preparation, repeated
wall contacts use a signed byte counter greater than8, and ground homing calls
screen recovery. Added pinned body identity/26 instruction anchors and
[native contract with explicit fidelity boundaries](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md)
in `432293c`. Independent existing source verifiers still pass.

Replaced the ground-only XZ follower in `55d00bb` with portable 3D history,
independent ground/jump/fall state, source-derived ordinary jump initialization
and fall coefficients, swept wall/axis sliding, supported stale-path pruning,
and screen candidate recovery. The partner can traverse air without floor,
land independently, and recover without the old queue-size teleport. Original
`PKR_J_1A` now joins the existing original Goombella pack; provenance is in
[assets](ASSETS.md). Native motion uses actual post-collision Mario output.
Refinement retains stationary-hop spacing, continues over gaps, handles a jump
behind dense short trail marks and caps append pressure without teleporting.
Concurrent harbor/message work was preserved and excluded from partner commits.

Validation: final `make test` passes **93 CTest / 96 Python tests** in
`/tmp/partner-validation-test.log`; `make all` builds3DSX without warnings in
`/tmp/partner-validation-build.log`. `build-host/core_tests --original` passes
all original-room scenarios and **18,315** indexed/full-scan collision probes
(`/tmp/partner-original-room.log`), including partner stair support. New partner
scenarios cover stationary/dense-history jumps, retained spacing, gap crossing,
ledge fall, thin walls, stale path, persistent wall/capacity nonteleport, screen
recovery and Game binding. Homing/jump/fall/recovery/history static verifiers
pass;690 document links and `git diff --check` pass.

Initial full runs exposed a concurrent harbor test failure, independently
reproduced with pre-partner Game sources under `/tmp/partner-preexisting-check`;
that concurrent work subsequently resolved it. One rebuild hit a transient
unrelated field_battle_tests link failure; targeted relinking and the final
full run pass. Neither intermediate run is claimed as a passing validation.

Isolated Azahar movement/jump/save capture passes with15-second startup and
`tests/azahar_partner_follow.py`. Final files are in ignored
`artifacts/azahar-partner-follow-spacing/`, log `/tmp/partner-smoke-final.log`.
Inspected `partner-air-early.png` and `partner-follow-landed.png`: Goombella
uses her original air pose and returns to the floor with following space.
Earlier captures exposed an overlapping stationary landing, fixed and covered
by a regression. Xvfb required display-socket escalation; all profiles and
saves are project-isolated. No user's profile/save was modified.

Limitations/next: this implements equivalent native behavior categories,
not full instruction-exact party locomotion. Native vector scheduling, scale1,
terrain landing/sweeps and path lookahead remain adapters. Complete original
homing targets/ring indices, five-ray partner floors/front walls, near-Mario
placement3, fall placement4/+37, dynamic platforms, ability modes and partner
sound playback remain unbound. Record original Dolphin jump/ledge/wall/warp
trajectories before claiming measured parity; see [mechanics](MECHANICS.md).
Hardware behavior and full prologue/full-game fidelity remain incomplete.


## 2026-10-12 — Prologue map object visibility from original events

Implemented recovered ordinary-entry visibility for harbor ship subtrees and
wakes, east interior/gate and west alternate foliage in commit `8c6c5fa`.
The pirate ship group `S_fune_c` is hidden in prologue states2..6; original
init reveals it only for172..229 except the earlier separate217 case.
The later `S_fune_b` group uses>=268 except402/404. Other initialized-hidden
ship/wake groups remain hidden until their scripted overrides are bound.
See [visibility contract](behavior/field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md)
and [asset provenance](ASSETS.md). Source subtrees inherit visibility into
children without affecting siblings; batches split by event group, retaining
geometry for subsequent story states. Both drawing passes use the same policy.
Missing native story uses a conservative story0 rendering envelope without
changing persistent story storage. Pre-existing concurrent prologue work was
preserved and excluded from this feature commit.

Validation: `make test` passes **92 CTest / 96 Python tests** (log
`/tmp/pm-visibility-test.log`); final `make all` builds3DSX without warnings
(`/tmp/pm-visibility-build.log`). New tests cover source geometry partition,
prologue subtree omission, unchanged collision packs, metadata fields and
story boundaries/special cases. `check_doc_links.py` checks689 files with0
broken links; `git diff --check` passes. Isolated Azahar captures use synthetic
native story3/6 saves at harbor(0,-2), with18/15-second startup windows;
`artifacts/azahar-prologue-visibility/boot.png` shows source dialogue and
`artifacts/azahar-prologue-visibility-6/boot.png` shows unobstructed dock/water.
The initial5-second capture was too early (black); no visual pass was claimed
for it. Xvfb needed display-socket escalation. All profiles/saves are isolated.

Limitations/next: this is recovered map initialization, not complete event VM
activation. Bind story2 arrival, ship motion/departure/wake overrides, remaining
plaza/NPC/item/door conditions and original collision activation; verify scene
transitions against Dolphin. No original-runtime observation or hardware
performance claim. Full prologue/full-game fidelity remains incomplete.

## 2026-10-12 — Goombella original animations, visibility and facing correction

Field Goombella now renders original `PKR_S_1`/`PKR_W_1`/`PKR_R_1` idle,
walk and run clips from `a/c_pkr` + `a/c_pkr-`, using original scalar2 and a
separate texture atlas. Static zero-span `PKR_Z_1` is also packaged for setup.
PMANIM3 adds original visibility bases, group indices and sparse full-key
modulo256 updates. Hidden ancestors suppress descendants while preserving
siblings; shapes hidden at frame0 remain available for later keys. Corrected
the offline bake's old `delta==1` assignment. PMANIM1/2 remain readable.
Source clips replace her field billboard/procedural bob. See
[asset provenance](assets/MARIO_AND_POSES.md) and
[Goombella binding](behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).

The user caught reversed travel-facing in the initial native adapter. Replaced
that snap mapping with the recovered ordinary `partyMakeDispDir` target ranges,
four interpolation/latch branches and wrapped Z-reflection. Positive X uses
source heading90 / target180; negative X targets0. Original moving pose
selection keeps partner speed, owner scale1 and Mario stick class distinct.
Native displacement still adapts source speed/heading; original homing, special
turn flags, sound helper, HP/dot-table and event/battle poses remain unbound.
This is animation progress, not completed partner/story fidelity.

Validation: `make test` **90 CTest / 93 Python passing**; `make all` builds
3DSX without warnings. Independent original-data comparison passes **39
Goombella samples**, max model error5.53811e-6, and **251 Mario samples**,
max error3.05176e-5, including texture/visibility boundaries. Synthetic tests
cover byte wrapping, hidden descendants/siblings/cycles, rewind, invalid pack
rollback, zero-duration poses and both facing endpoints. Source checks pass
47 scale anchors/13 DOL spans/2 converter models and515 partner instructions/
32 constants/7 spans;688 document links pass. Refreshed a previously stale
converter-derived Mario numeric baseline without changing source fingerprints.

An initial isolated west-road capture passed movement/jump/save. A later short
hold failed its0.1-unit distance assertion (actual0.072); the longer dedicated
`azahar_partner_animation.py` capture passed, saving room0 X=-3.20559 from
-5.05. These are software-emulator fixture checks, not measured original timing
or hardware performance. Final facing capture under `artifacts/azahar-partner-facing`
passes the expanded right/left movement/jump/save script; manually inspected
`partner-run-later.png` and `partner-idle-left.png` confirm correct facing in
both directions. All emulator profiles/saves are isolated from the user's profile.

Next: native original message parser/window playback, harbor REL scene
activation and indoor doors/maps; remaining partner motion/poses and prologue
visibility conditions. The playable prologue / original-game parity goal
remains incomplete.

## 2026-10-12 — Harbor position gates and original Mario position callback

Documentation cross-check recovered original story3's integer Z<=-100 gate,
and story4's nested Z<=-100 / X>=-25 gate, with original message IDs, speakers
and control-token inventory. Added `fieldMarioGetPosition` with callback-registry
binding at US0x800e71f8 and `harborPositionGate`; both compose with the existing
event operand services, preserving 1/1024 float writes and integer truncation.
Independently re-disassembled the complete US callback: argument pointer is
captured before marioGetPtr, Mario pointer before mode resolution, and each
destination/position write remains ordered. All nonzero modes subtract1 from Y.
See [harbor bindings](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md) and
[inbox breadcrumb](requests/OW-INTERACT-01.md).

Validation: `make test` **89 CTest / 92 Python passing**, `make all` builds
3DSX; `verify_field_lecture_callbacks.py` verifies15 bodies/54 instruction
anchors/6 constants/10 imports; `verify_behavior_contracts.py` passes;
`check_doc_links.py` checks687 files with0 broken links. Tests cover fractional
gate boundaries, mode behavior, quantization, pointer capture, ordered field
reads and registry dispatch. No emulator run: these portable services are not
yet invoked by `Game`, so current development notices/trigger remain unchanged.

Next: original message parser/window presentation, REL harbor scene activation,
NPC poses/choreography and indoor entrances. Goombella's PKR_Z_1 pack probe
encountered original animated visibility, which PMANIM2 explicitly rejects;
implement that domain before replacing its baked idle image. The playable
prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native original US message library and resource lookup

Imported all 260 `msg/US` resources / 13,186 key/text pairs byte for byte
into ignored `romfs/messages/`, with hashes, source offsets and licensing in
its local manifest. `make all` now packages this library. Portable
`MessageResource` retains original IDs/control tokens/high bytes, builds an
offset index, and atomically rejects invalid files. `MessageAssets` provides
room-before-global lookup. Missing/ambiguous keys deliberately return null;
original fallback and duplicate-key qsort behavior remain unbound. See
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md).

Validation: `make test` **88 CTest / 92 Python passing**, `make all` builds
3DSX, `build-host/message_assets_tests romfs/messages/gor_00.txt` verifies
328 actual harbor pairs and all 20 opening IDs. No story playback or emulator
behavior changed in this increment. Next: original-token presentation and
story-call bindings, position-triggered harbor scenes, indoor entrances.
The playable prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native cross-check fixes: sink guard + default lanes

The native session's independent re-read falsified my water doc's sink
guard direction ([124]==0.0 initializes −6.0, not !=; fixed and credited)
and flagged the undocumented r4-sourced default fall lanes — added to the
bottomless doc ([r13−28176]+16 vs absolute 0x80416208+16 by [46]==5,
verified by direct reads). Both fixes verified against the listings. This
is the inbox cross-check loop working as designed: independent re-reads
catch single-bit errors that self-review missed.

Validation: direct-instruction re-checks, diff --check clean. Docs-only.
Next: triangle interiors, pose domains. Full-game incomplete.

## 2026-10-12 — HitVec region-graph correction (false asymmetry withdrawn)

Re-verified the traversal doc's region claims against the listing and found
my own region-1 asymmetry finding was backwards: R1 allocates counter=r30 /
pointer=r31 (inits `lwz 31,172` / `li 30,0`), regions 2–4 the reverse — same
loop semantics, no executability consequence. Withdrew the pointer-size
claim; kept the verified cascade edges (R1-hit→R2/R3-gateway, drains) and
the airtight R4-unreachable finding (no entry edge, dead fallthrough).
Two crashed verification scripts produced zero passing asserts this turn —
all claims above are re-verified by direct reads instead.

Validation: direct-instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains. Incomplete.

## 2026-10-12 — Water/void recovery: apply recovered bottomless drop-back lanes

Reviewed the
[water](behavior/overworld/OVERWORLD_WATER_RECOVERY_NUMERICS.md) and
[bottomless](behavior/overworld/OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS.md)
numerics deliverables against the `WaterRecovery` prototype. Independently
re-read the ISO DOL and confirmed the mot_bottomless drop-back lanes
(`0x80091bd4`: `[124]/[128]/[132]/[136] = 0.34/-0.002/-0.01/0.0`), so replaced
the prototype's authored `-.001/-.002/+.00024/-.00004` cascade (which matched
only `-0.002`) and pinned it in `tests/field_camera_tests.cpp`. Did **not**
apply the `-6.0` sink velocity, the `0.08` travel step or the `state==15`
handling: the deliverable leaves them unestablished, the `0.08` constant has no
SDA source, there is no original stage-15 slot, and the prologue guard reads as
`[Mario+124]==0.0` on the DOL (the contract states `!= 0.0`). The prototype
still omits the `[Mario+552]`-mismatch condition and the `r4` default lanes.
Breadcrumb: `requests/FIELD-RECOVERY-01.md`.

Validation: `make test` 87 CTest / 90 Python pass, `make all` 3DSX pass,
warning-free compile of the changed TU. Docs mid-restructure (inbox split into
`requests/`); breadcrumbs went to the per-request file. Full-game incomplete.

## 2026-10-12 — Merge the overworld request inbox into docs/requests

Migrated `OVERWORLD_DOCUMENTATION_REQUESTS.md` into the standard inbox style:
seven per-request files under `requests/` (OW-MOVE-01, OW-WALL-01,
OW-SUPPORT-01, OW-PARTNER-01, OW-ROAD-01, OW-INTERACT-01, OW-CAMERA-01), each
with a status line, an Open-todos checklist, verbatim request text and an
append-only answer/delivery history. The seven rows join the status table in
`DOCUMENTATION_REQUESTS.md`, which also absorbs the story-integrity and
static/runtime rules that headed the separate file; the "tracked separately"
pointer is gone. Cross-links in `behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md`
and `behavior/battle/BATTLE_HIT_VEC_TRAVERSAL_ORDER.md` now target the
per-request files.

Validation: `python3 tools/check_doc_links.py` (686 markdown files, 0 broken)
plus a line-by-line comparison of the old file against the new files — every
original line survives; only heading levels, the three header provenance lines
and the relative link prefixes differ. Docs-only, no make rerun. Limitation:
overworld deliveries stay recorded in each request's history and are not
back-filled as `requests/DELIVERY_LOG.md` rows, because the original inbox
never named a numeric documentation-agent ID for them. Next: answer the OW-*
open todos and register new deliveries/locks in the delivery log.
Full-game incomplete.

## 2026-10-12 — EVT-01 accessors, start wrappers, Monosiri, and an address correction (ID 3)

Second EVT-01 pass as documentation agent ID 3, continuing the inbox queue.

**A correction that outranks the new work.** I had published three data
bases computed **without the sign extension `lis` performs**, so all three
were wrong by `0x3B0000`:

| Published earlier | Correct | Symbol now confirmed |
| --- | --- | --- |
| `0x8002F988` | **`0x803DF988`** | `pssfx` (`.bss`, 1600 B = **40 slots** x 40) |
| `0x80016B28` | **`0x80316B28`** | `pssfxlist` (`.data`, 84840 B) |
| `0x8004CF40` | **`0x8036CF40`** | `monosiriMessageTable` (`.data`, 5544 B = **exactly 231 records** x 24) |

The symbol names and sizes are the decisive part: `1600/40` and
`5544/24` both divide exactly, which independently confirms both strides
and turns the slot count (40) and table extent (231) into facts rather than
guesses.

The same error had reached seven field-collision contracts from the previous
session, where `lfs` float constants were given as `0x800494xx`. Those were
internally inconsistent with each other, which is what exposed them. All are
now in the authoritative `N(r2)` form, read from the instruction bytes
(a contiguous pool from `-27544` to `-27380`), and the `lis`-derived globals
correct to `0x80343B78` / `0x80343D88`, both resolving into `.data`. A
correction note heads each affected document.

I also **reverted one of my own "corrections"**: the three values
`0x80091434` / `0x80091504` / `0x800915e4` are not debug strings but the
symbol entry points of `chkfilterVecVivian`, `chkfilterVecRoll` and
`chkfilterVec`, passed to `hitCheckVecFilter` as filter labels. Two were
already right in the original text; I had briefly "fixed" them wrongly.

**New complete bodies**, all in `docs/behavior/audio/`:

- [Slot accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) —
  `psndSFX_get_vol`, `psndSFXChk`, `psndSFX_dist`, `psndSFX_pit`,
  `psndSFX_vol`. Completes the accessor family. Key finding: the handle
  packs **slot index in bits 0..7 and channel allocation in bits 8..23**,
  validated against `slot+0x04` with a `-1` return. Also `stb`/`sth`
  truncation with no clamp, and the local `slot+0x14` vs `SoundEfxSetPitch`
  pitch split gated on the same low-5 bits `psndSFXOff` uses.
- [Start wrappers](behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) —
  `psndSFXOn`, `psndSFXOnEx_3D`, `psndSFXOn_3D`, plus the **derived
  eight-argument `__psndSFXOn` ABI**. `arg5` and `arg7` are always `0`;
  `psndSFXOn_3D` silently discards the caller's `r5`.
- [Monosiri lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md) —
  `battleGetUnitMonosiriPtr` 8012ef20, the item EVT-01 called
  unclaimed. Pure signed-multiply address computation with no bounds,
  no negative-kind check and no null return. Noted that the caller's
  `== null` test is therefore reading a **record field**, not a failure.

Also noteworthy: **40 slots exist but the handle masks with `& 0xFF`**, so
indices 0..255 are all reachable and nothing bounds them.

Updated [behavior/README](behavior/README.md) (its rows were overwritten
twice by concurrent commits and were re-added each time), appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md), marked the Monosiri todo
answered with the residual work, and registered three validated delivery
rows.

Validation: bodies re-disassembled per function with ELFs in `/tmp` only;
every callee resolved against `us_symbols.csv`; `lis`/`addi`/`addis`
arithmetic re-derived in Python and cross-checked against named symbols;
repo-wide sweep for the four wrong address patterns returns clean;
`git diff --check` clean; link audits clean. Docs-only, so `make test` /
`make all` were not rerun. Full-game incomplete.

**Process lesson worth keeping:** the fabricated addresses survived a
commit and a README row because they were plausible-looking. The two
cross-checks that caught them were (a) does the address land in the
section the symbol table says it should, and (b) do claimed strides divide
the claimed symbol sizes evenly. Both are cheap and should be applied to
any future address claim before it is written down.

Next tasks:

1. **`__psndSFXOn` 800d9934 (1344 bytes)** — now the only writer of
   `pssfx` fields `+0x00`/`+0x04`/`+0x06`/`+0x18` and the place the
   handle is packed. With `pssfx`/`pssfxlist` named and the layout fixed,
   this is the last structural gap in the audio chain.
2. **`psndSFXMain` (2964 bytes)** — reader of volume, distance and the
   published position; closes the update/stop loop.
3. Monosiri record layout beyond `+0x04`, and its table writers
   (`battleAcMain_MonosiriTarget`, the `monosiriExit` call at `8017225c`).
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` is a contended file: three concurrent commits
   have dropped rows from it. Worth an owner check before it is treated as
   a reliable index.

## 2026-10-12 — `__psndSFXOn` slot provisioning decoded (ID 3)

Third EVT-01 pass as documentation agent ID 3, taking the top item from
the previous queue: `__psndSFXOn` (`0x800d9934`, 1344 bytes), the last
structural gap in the positional-SFX chain because it is the **only**
writer of the `pssfx` fields the accessors read.

Delivered [POSITIONAL_SFX_SLOT_PROVISIONING](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
decoded end-to-end. Evidence class B; the `make test` / `make all` gates
were not rerun because this is docs-only.

Recovered:

- **The `key` argument is a string.** Name resolution is a `strcmp` scan
  over `pssfxlist` (stride 20, up to **4242** entries) against
  `word0`. Key `0` skips the scan; a scan that runs off the end resolves to
  index 0. There is no not-found failure.
- **Three pre-allocation gates**: global `+0x56` bit 29 as a mute that
  applies only when `arg7 != 0`; a signed-byte **cooldown of 6** at global
  `+0x5c` that rejects while nonzero; and for index 869 an event flag at
  `0xF5DE0180` that must equal 2. Indices 517 and 869 are the only
  per-index specials.
- **Two-stage slot search**: 20 same-key slots first (with a `+8`
  preference cursor at global `+0x54`), then a 40-slot reclaim pass that
  **inlines a full copy of `psndSFXOff`** — same low-5 gate, same
  `key >> 19` table index, same two-bit SoundSS/Song/Efx stop — and
  additionally requires `slot+0x0c == pssfxlist[+0x10]`. Exhaustion
  returns `-1`.
- **Channel bits are a pure function of the pssfxlist index**: base
  `0x101`, plus bits 5/13/14/15 of the index and per-index/per-field
  overrides. This is the routing mechanism.
- **Handle packing confirmed**: `channel = ++global[0x50]`,
  `slot[+0x04] = (u16)channel`, return `slot_index | (channel << 8)`.
  Independently confirms what `psndSFXChk` validates. Recorded asymmetry:
  `slot+0x04` gets the full 16-bit counter but only `<<8` reaches the
  handle, so the two diverge when the counter wraps.
- **Two unreachable volume clamps** at `0x800d9c60`/`0x800d9c70` — the
  `clrlwi ...,24` then `cmplwi ...,255` can never be greater. Recorded as
  observed rather than "fixed" into live clamps.

**One conflict deliberately left open.** `__psndSFXOn` writes `slot+0x00`
as the bare pssfxlist index (0..4241), but `psndSFXOff` and `psndSFX_pit`
derive their pssfxlist row from `(slot+0x00) >> 19`, which is always 0 for
that range. Read literally the accessors always resolve `pssfxlist[0]`.
The bytes are unambiguous (`stw 18, 0(29)`) and no second writer is
visible, so **no reconciliation was invented**. Flagged in the contract and
in the EVT-01 reply as something to settle before either side is relied
on.

**The practical dependency this created:** nothing in `__psndSFXOn`
decrements the cooldown. If `psndSFXMain` is ported without its decrement,
`global[0x5c]` stays nonzero and every later start fails permanently. That
makes `psndSFXMain` (2964 bytes) the single remaining item in the chain.

Registered one validated delivery row and appended the reply to
[requests/EVT-01.md](requests/EVT-01.md).

Next tasks:

1. **`psndSFXMain`** (2964 bytes) — cooldown decrement, preference-cursor
   expiry, and the reader of volume, distance and the published position.
   This is the last item in the positional-SFX chain.
2. Resolve the `slot+0x00` index-versus-packed-key conflict above.
3. Monosiri record layout beyond `+0x04` and its table writers.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` remains a contended file; verify rows survive.

## 2026-10-12 — OW-MOVE-01 input remainder: key-data update

Re-extracted marioUpdateKeyData (0x80095cb0, ISO read-only) and contracted
it in behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md: six GP pad
bytes with alternating negation into +594..599, [600/601] zeroing, flag
halfwords, fixed-order per-flag pair zeroing, key-check gate and the
0x803DA398 rising-edge word. No deadzone/clamp exists in-body (native .15
is authored). Reference pair placed under ignored artifacts/.

Validation: instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains, deadzone
ownership elsewhere. Full-game incomplete.

## 2026-10-12 — Full-tree gap re-audit plus lightSort alias

Re-ran the name-mention audit over all 664 reference binaries against the
current docs tree: exactly one unnamed file (`lightSort`), verified as alias
of contracted `lightdrv _sort` @8001bccc (three-way float compare plus
`sorter800064fc` caller in behavior/display/MAP_CHARACTER_LIGHT_QUERIES.md)
and recorded in the register. Re-added the plane-motion README row dropped
by the recent restructure. Zero unnamed-and-uncontracted binaries remain.

Validation: audit script, listing-vs-doc structural match, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains,
pipe lifecycle. Full-game incomplete.

## 2026-10-12 — Inbox request: EVT-01 positional SFX chain (ID 3)

Worked the [request inbox](DOCUMENTATION_REQUESTS.md) as documentation
agent **ID 3** (the ID the user assigned; ID 1 and 2 were already taken).
Picked the priority-0 EVT-01 audio chain, which had **no dedicated contract
at all** and was explicitly listed as open by three existing documents.

First surveyed the inbox: `docs/requests/` per-request files plus the
[delivery log](requests/DELIVERY_LOG.md) lock table. Only one lock was
active (ID 2, audience target accumulator), so EVT-01's SFX subtask was
free. Confirmed the manager loop and constructor policies making up the rest
of EVT-01 todo 1 are already complete in `EVENT_MANAGER_COMPLETE_LOOP.md`,
`EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md`,
`EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md` and
`EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md`; the genuinely unclaimed work was
the audio chain.

Delivered three **complete bodies** in `docs/behavior/audio/`:

- [Unit positional SFX trigger](behavior/audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md)
  — `BtlUnit_snd_se_pos` 80126088/114
- [Positional SFX position update](behavior/audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)
  — `psndSFX_pos` 800d96f8/4c
- [Positional SFX stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md)
  — `psndSFXOff` 800d95f8/c8

New facts, none previously recorded anywhere in the repo:

- The shared **positional-SFX slot record**: base `0x803df988`, **stride
  40**, selected by the **low 8 bits** of the handle. Fields `+0x00` key,
  `+0x06` channel bits, `+0x18` handle, `+0x1c/+0x20/+0x24` position.
- The **channel table** at `0x80316b28`, **stride 20**, indexed by
  `(slot.key >> 19)` — by the key, not the handle. Two bits of `word0`
  select `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`.
- **Stop-without-stop**: `psndSFXOff` clears the record without calling any
  stop service when `(halfword6 & 0x1F) == 0` or `handle == -1`; and an
  already-free slot returns *without writing*, so a double stop is a true
  no-op rather than a re-clear.
- `psndSFX_pos` writes **exactly three words** and never the key or handle,
  so it cannot start or stop a sound.
- `BtlUnit_snd_se_pos` status 10 **overrides** status 11 when both are set
  (sequential `if`s overwriting one register), and its start id is
  `(int16)(irand(41) + belong + offset - 20)`.
- The volume predicate is `r0 < 0x4D80` unsigned, where
  `r0 = (T & 0xFFFF0000) + 0xE7700000`. For the sentinel the movement-sound
  callers actually pass, `250000000`, this is the **preserve-current-volume**
  path. Derived from the compared words, not an observed trace.
- The wrapper's return is the **handle**, confirming the existing R18 store
  in `BATTLE_MOVEMENT_SOUND_CONTROL`.

Updated [BATTLE_MOVEMENT_SOUND_CONTROL](behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)
and [OVERWORLD_MARIO_IDLE_SOUND_CUES](behavior/overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
to link the new bodies instead of listing them open; no contradiction found
with the existing low-8-handle-bits claim. Registered and then **released**
the lock in the [delivery log](requests/DELIVERY_LOG.md), marked the EVT-01
todo answered with an explicit list of what stays open, and appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md).

Validation: all three bodies disassembled from the read-only ISO with ELFs
written to `/tmp` only; every callee resolved against `us_symbols.csv`;
disassembly re-read against each written instruction block; `git diff
--check` clean; README, cross-link and request-file link audits all clean.
Docs-only, so `make test` / `make all` were not rerun. Full-game incomplete.

Note: while this session ran, commits ec92b10/990b12e/488983a landed from
other work and prepended entries above this one. No conflict; this session's
field-collision entry is intact at its original position.

Next tasks, in the order the inbox implies:

1. **`__psndSFXOn` provisioning** (`0x800d6934`, 1344 bytes) — the slot
   allocation and macro/voice/resource chain that fills the record layout
   recovered above. Now the biggest blocker for the audio chain, and the
   known layout makes it tractable.
2. **`battleGetUnitMonosiriPtr` 8012ef20** — still explicitly unclaimed,
   with no dedicated contract. Small and bounded.
3. **`psndSFXMain`** (2964 bytes) — the reader of the published slot
   position, which closes the update/stop loop from this session.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, `NormalJump`
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, and the `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. The constant block `0x80049468`..`0x8004950c` from the field-collision
   session is still unextracted.

## 2026-10-12 — Re-extracted placeholders: plane flyMain contract, two aliases

Re-extracted three zero-byte placeholders read-only via
tools/inspect_dol_function.py into behavior/: mot_plane_flyMain (mot_plane.o
flyMain @8009c9b8, contracted in behavior/overworld/OVERWORLD_PLANE_FLIGHT_MOTION.md
with ISO-read SDA set; fixed my own hex/decimal stage confusion pre-commit),
btlevtcmd_AnnounceMessage (matches contracted announcement doc — alias),
PSVECDotProduct (9-instruction leaf contracted in the traversal doc).
Remaining placeholders (_announceMessage/BattleGetFirstAttackTarget/
partyGetOtherPtr/partyGetTargetNum cases) have no DOL text symbols and cannot
be extracted by name.

Validation: extraction outputs verified (ELF identity, entry/size match),
stage immediates re-checked after the hex fix, link audit, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains.
Full-game incomplete.

## 2026-10-12 — Fifth gap pass: guard wrapper, act-status publisher, small-data setter and float-to-unsigned

- [Small unit/runtime leaves](behavior/unit/SMALL_UNIT_RUNTIME_AND_MOTION_LEAVES.md):
  `BtlUnit_CanGuardStatus` (801272dc) is a frame-only forwarder to
  `BtlUnit_CanActStatus` (8012724c), so a `bl 801272dc` site is a `bl 8012724c`
  site for call-graph purposes. `btlevtcmd_CheckActStatus` (80105cb4) resolves a
  unit id, calls that same `BtlUnit_CanActStatus`, and publishes a fixed literal
  1 or 0 through `evtSetValue` using the **raw** second operand word as the
  destination. `unk_80151d20` (party_motion.o) is a three-instruction setter
  that writes the constant `60.0` (0x42700000 at 0x8042318c) to the small-data
  slot at `0x8041ec10` and returns. `__cvt_fp2unsigned` (80264860, runtime.c)
  maps `f1 < 0` to 0, `0 <= f1 <= 2^32` to `0xFFFFFFFF`, `f1 > 2^32` to
  `trunc(f1)` and NaN to `0xFFFFFFFF`; the 2^31 constant at `0x80303a30` is
  loaded but dead on every reachable path because the `f1 > 2^31` branch is
  always true once execution passes the 2^32 test.

Register state after this pass: 360 reference listings have no snapshot
definition; 31 of them still have no substantive prose contract. The largest
remaining unaddressed bodies are `seq_battleMain` (253 lines), `partyReInit`
(461), `_getSickStatusParam` (286), `JUTFontSetup` (196),
`FontDrawStringVecPitch` (211), `pouchRemoveItemIndex` (183),
`npcSetupBattleInfo` (166), `vivian_move`/`cloud_move`, the pad readers
(`DEMOPadRead`/`DEMOPadCopy`), `btlevtcmd_ConsumeItemReserve` and
`btlevtcmd_AnimeChangePoseType`. `marioLandOn` and `marioSearchHead` are now
carried by multiple contracts and are no longer counted as open.

Validation: docs-only; `make test` and `make all` were run for the previous pass
and are unchanged by this one (logs `/tmp/pm-gap3-test.log`,
`/tmp/pm-gap3-build.log`). `python3 tools/check_doc_links.py` reports 0 broken
links. No native/test/asset conversion change, hence no runtime, cadence,
emulator or original-execution claim. EVT-01/JUMP-01/DMG-02 and the full-game
requirements remain incomplete.

## 2026-10-12 — Native JUMP-01 correction: packed Mario jump SFX

Verified the JUMP-01 SFX-provenance answer against the read-only ISO and found
it analysed the generic `btlevtcmd_JumpPosition` (0x8010d208/0x4c4), not the
body the native port binds, `btlevtcmd_MarioJumpPosition` (0x801093a8/0x8c0).
Reproduced the Mario body from the DOL (`tools/inspect_dol_function.py`):
r30=0x802ee458, so the only 0x802E reads are the dead 24/28/32(r30)=
0x802EE470/474/478 loads (overwritten by unit floats before the call); one
`psndSFXOn_3D` (0x801098fc) keyed on `word[unit+0x18c]`, position
unit+0x154/158/15c; heading/distance from `system_angleABf` 0x8000703c /
`distABf` 0x8000722c and sine from MSL `sin` 0x80272410. Applied the one
native fix that survives: removed the three dead `0x802ee470` reads in
`source/battle_packed_mario_jump.cpp`; kept the single `play` call (the "model
both records" follow-up came from the generic body and is withdrawn). Filed the
correction breadcrumb in `docs/requests/JUMP-01_HISTORY.md`.

Validation: `make test` 87 CTest / 90 Python pass; `make all` 3DS build pass;
`diff --check` clean. Docs are mid-restructure (inbox split into
`docs/requests/`), so the breadcrumb went to the per-request history file, not
the index. Full game still incomplete.

## 2026-10-12 — Medium-size field collision functions (mario_hit.o)

Continued the medium-size sweep requested for a US port. First verified
that the 359-function fingerprint set in
`artifacts/reference/behavior/evidence.json` is now fully referenced by
existing docs (its `documented` flag is stale and was not used), then
widened the search to the full `us_symbols.csv`: 1098 game-text functions
in the 500-1500 byte band, 810 of them not referenced anywhere.

Worked the Mario field-collision family, picked because it is directly
needed for movement fidelity. Five new contracts in `docs/behavior/field/`:

- `MARIO_SLIT_CHK_WALL_AROUND.md` - complete structural read of the 8
  direction slit probe: two sweeps, three-way per-direction
  classification, and the two-group triple-run rule that returns 0 on
  three adjacent blocked directions.
- `MARIO_CHECK_WALL_ROLL.md` - the roll wall check, including the 14-tick
  wall-hold re-arm record at `mario+0x1cc..0x1d4`.
- `MARIO_SEARCH_HEAD_ROLL.md` - the 5-iteration indexed direction dispatch,
  with the `frsp` roundings between trig and scale called out.
- `MARIO_SEARCH_FRONT_WALL_STAYING.md` and `MARIO_CHECK_WALL.md` - both
  documented as **complete bodies** (every branch and call site).
  The staying search contributes the `mario[+0x2e] == 19` veto, the
  field-name gate and the triple `vivianGetStatus` gate; `marioCheckWall`
  contributes the 5-char hit-name hysteresis (thresholds 10/20) on a
  **signed** byte at `mario+0x3f` and the in-place `movePos` slide.
- `SEARCH_FRONT_WALL.md` and `MARIO_CHK_WALL_AROUND.md` - the two largest
  callees (2404 and 2460 bytes), documented at **structure level only**:
  entry gates, probe grouping, SDK vector math. Their probe sequences and
  result reduction are explicitly not decoded.

Cross-references in the earlier three docs were corrected where they listed
these two callees as undocumented, including one `marioChgWallAround` /
`marioChkWallAround` transcription error.

Newly recorded facts: the field literal at `0x800c3d88` gates both
`searchFrontWall` and stage 3 of `marioSearchFrontWall_staying`, so those
two paths are tied to one specific field; the 5-char hit-name literal
compared in `marioCheckWall` lives at `0x80049504`.

Validation: all callees resolved against `us_symbols.csv`; disassembly
regenerated per function into `/tmp` only; every `git diff --check` clean;
README link audit and field cross-link audit both clean. Docs-only, so
`make test` / `make all` were not rerun. Full-game work remains
incomplete.

Limitations: no Dolphin observations were taken this session, so every
claim is class B. The two structure-level docs must not be read as
equivalent to the original implementations.

Next tasks:

1. Decode the 9-probe reduction in `searchFrontWall` and the 12-probe
   reduction in `marioChkWallAround` - these block full movement parity.
2. Read the two unresolved literals from the ISO: `0x800c3d88` (the
   gated field name) and `0x80049504` (the hysteresis hit-name prefix).
3. Extract the `lfs` constants at `0x80049468`..`0x8004950c` - they are
   referenced by nearly every function documented in this session and are
   currently the largest single source of provisional numbers.
4. Continue the 810-function medium backlog; `party/*` and
   `npcCheckHitMarioSide` / `mobjHitEntry` are the next highest-value
   targets after collision.
5. `docs/PROGRESS.md` tables should be reconciled against actual file
   contents if any further drift is found.

## 2026-10-12 — Native request 3 slice: pose call-site survey

Delivered OVERWORLD_POSE_CALL_SITE_SURVEY: 18 marioChgPose sites with
ISO-read literals (KPA_D_1/D_2 table pair, M_D_2/M_U_3 recovery pair,
M_J_1B/M_J_1C, M_W/M_O/M_R alternation) plus gate/ordering notes; all
surveyed sites are marioChgPose (no EvtPose). Remaining request-3 items
(named domains, pipe-paper asset work) explicitly scoped out with a
non-finding (no reference body) for the pipe lifecycle.

Validation: all 18 call sites re-checked, literal strings ISO-read, link
audit, diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — Native request 2 remainder: bottomless numerics

Delivered OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS: first-tick gate,
win_03/win_01/tik_01 lane selection (rodata-read), angle setup, stage
machine via ISO-read table (0/2/10/11/12 live, 13 terminal-by-gate),
fallSpeed-shape integrator, min −10.0 snap, land/HP chain, and the
0.34/−0.002/−0.01 drop-back triple (port's jerk cascade matches one lane).
Dead −2000.0 load and SDA set pinned from ISO. Request 2 fully answered
except float-chain interiors.

Validation: call/SDA/stage-write re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: pose-table slice (request 3). Incomplete.

## 2026-10-12 — Native request 2: water recovery numerics

Delivered OVERWORLD_WATER_RECOVERY_NUMERICS for field/AC-03: full
stage/counter machine ([68]=stage via ISO-read table 0x8036ED4C, [72]=
counter), 12-slot ISO-read SDA set, sink −6.0 init, 40-count stage-1 hold,
two-tick stage-10 arm, HP-penalty tick at stage-31 expiry, 12-count stage-32
window, and named gaps where the port authored ratios/jerk (no 0.08-class
constant, no jerk lane in-body). Stale "stage15" has no slot (no-op).

Validation: dispatch/stage-write/SFX-ID/HP-call re-checks with int/float
register-file care; link audit; diff --check clean. Docs-only, no make
rerun. Next: bottomless motion33/34, pose-table slice. Full-game incomplete.

## 2026-10-12 — Native JUMP-01 request 1 answered (SFX provenance)

Fulfilled the packed-Jump provenance request from the native inbox with
BATTLE_JUMP_SFX_PROVENANCE: vec3 scratch proven zero-bytes (not a key row),
U+0x18C key domain with two-call psndSFXOn_3D record (second inherits r3),
and no-PS-math finding for 801093a8. Recorded the answer in
DOCUMENTATION_REQUESTS.md without touching the request text. Water/motion
and pose-table requests (2/3) remain open — mot bodies located for next.

Validation: dual-offset ISO reads, call-site arg traces, PS-op sweep,
link audit, diff --check clean. Docs-only, no make rerun. Full-game
incomplete.

## 2026-10-12 — Third and fourth gap passes: event leaves and the shadow driver

- [Weapon/status/wait leaves](behavior/battle/BTLEVTCMD_WEAPON_STATUS_WAIT_LEAVES.md):
  six `battle_event_cmd.o` bodies with no snapshot definition.
  `btlevtcmd_GetWeaponActionLv` publishes `lbz [resolved+0x6E]` through
  `evtSetValue` with a **raw** destination word; `btlevtcmd_AcSetDifficulty`
  drives `BattleActionCommandSetDifficulty(B, unit, value)`; and
  `btlevtcmd_AnnounceSetParam` writes into an 8-byte-stride record array at
  `BattleWork+0x18C8C` at the `+4` value word of `4*index`, with no bounds check.
  `btlevtcmd_AfterReactionEntry` resolves operands into
  `BattleTransID`/`evtGetValue` and calls the queue helper from the
  after-reaction contract. `btlevtcmd_StatusWindowOnOff` is a signed four-way
  dispatch writing `word[B+0x2748]` (modes 0/1) or `word[B+0x2744]` (mode 3) and
  then always calling `BattleStatusWindowCheck`; those are exactly the two words
  whose nonzero state forces that checkpoint's result to 0, which is recorded as
  a static inference. `btlevtcmd_StartWaitEvent` calls `BattleRunWaitEvent` only
  when `word[unit+0x28C]` is nonzero, pairing with the existing stop leaf.
- [Shadow driver leaves](behavior/display/SHADOW_DRIVER_ACCESSOR_LEAVES.md):
  the whole fifteen-body `shadowGet*` family in `shadowdrv.o`, previously absent
  from every prose file. Three SDA words at `0x804150c8/cc/d0` select the
  depth/projected/character shadow records; two static texture objects live at
  `0x8041e738`/`0x8041e758`; the three `ProjTexMtx` getters share relative
  `+124`; the two `Config` getters index a 72-byte-stride table at `0x80304a60`
  from `word[record+8]` with no bounds check; the three `TexObj` getters gate on
  bit 0 of `word[record+0]` and tail-call `smartTexObj(record+220, word[record+
  252])`, returning NULL when the gate is clear. `shadowGetDepthShadowColor` is
  the only value-producing body: it packs `(255-c)*alpha/255` for the three
  component bytes at `record+256..259` using the `0x80808081` signed-division
  idiom into one word with a zero high byte.

`BattleAfterReactionMain` was checked against the existing
[waits and conclusion](behavior/battle/BATTLE_WAITS_AND_CONCLUSION.md)
contract instead of being rewritten: the queue scan, `BattleConsumeReserveItem`,
`BattleCheckConcluded`, `BtlUnit_GetData`, `evtEntry` and the recursive restart
on a missing unit all match that prose, so the queue contract now links to it.
`btlevtcmd_AcSetParamAll` was likewise left alone: it stores eight resolved
operands into `word[B+0x1CC8+4*i]`, skipping the `0xf1194d80` sentinel, which the
action-command callback contract already documents.

Validation: `make test` passes and `make all` passes; logs
`/tmp/pm-gap3-test.log`, `/tmp/pm-gap2-build.log` (docs-only pass, no native
change). `python3 tools/check_doc_links.py` reports one pre-existing broken link
owned by another agent (`partner/FIELD_PARTNER_FLY_MAIN_DISPATCH.md`); every link
added by this pass resolves.

Next: remaining unaddressed rows are larger bodies — `BattleCheckPikkyoro`
(112 lines), `battleMakePhaseEvtTable`/`battleSortPhaseMoveTable`,
`partyDoWork`, `_getRegistStatus` and `marioLandOn`/`marioSearchHead`.
EVT-01/JUMP-01/DMG-02 and the full-game requirements remain incomplete.

## 2026-10-12 — Source audit: documentation requests for packed Jump and recovery

Observed the implementations under `source/` and filed three native
documentation requests in docs/DOCUMENTATION_REQUESTS.md (appended at the end;
no docs-agent lock reserved). Reviewed the recent native commits that shipped
without leaving requests (b79bbe2 packed Mario Jump services, 81c143d
water/void/pipe recovery poses, 778add7 idle/low-HP pose scheduling,
e1257d8/357a656 field shadows).

- JUMP-01: the packed Mario Jump position binding (commit79bbe2) requests the
  0x802ee470/474/478 sound-key block, the original jump psndSFXOn_3D
  invocation record (timing/vector/return/landing cue), and the PS-family
  sin/cos identity plus FPSCR entry state. Local bodies were already recovered
  (BATTLE_MARIO_JUMP_MOTION/NUMERICS, BATTLE_UNIT_SOUND_MODE,
  BATTLE_DAMAGE_ORCHESTRATION for the mode1 AC-possibility fallback).
- field/AC-03: `source/water_recovery.cpp` water/void return-fall parameters
  (mot_damage motion31 / mot_bottomless motion33 stages) are authored and
  source-derived per include/water_recovery.hpp; pose timings are recovered,
  the numeric stage contract is not. Requested measured original sink/rise/arc/
  travel/return counters, HP-penalty tick, and bottomless placement/fall.
- field/AC-03: recovery pose-table domains 4..40 and pipe paper (p_dokan_x)
  lifecycle requested per the OVERWORLD_MARIO_RECOVERY_POSES open tail.
- P0c main-loop/field-dispatch: the native fixed-60Hz accumulator and
  Game::tick/move/startBattle substitutes have no original marioMain
  (8005a7e0) request; asked for the motion-ID→subsystem dispatch chain,
  battle-transition construction, input gate and marioStMain retrace/pause
  cadence (MARIO_MAIN.md sketch gaps), leaving existing pause/RNG asks open.
- P1b save/persistence: source/save.cpp uses a native PM2D format and one
  opaque originalStory int; asked for the complete original cardMain save
  record/state machine and the swSet/swGet story-switch bank, since CARD_MAIN.md
  and SW_SET.md are early sketches with explicit remaining gaps and no inbox
  entry.

Address-coverage check: of every original 0x8xxxxxxx address referenced by
source/, only 0x802ee470 (sound-key block, requested) lacked a contract; the
memory-bootstrap table, game-pointer clock block and sampling constants are
already documented.

Follow-up: the documentation agent answered the JUMP-01 request in commit
044ba5c with behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md. It corrects two
premises — 0x802EE470/474/478 is `vec3_802ee470` zero scratch (not a key row),
and 801093a8 has no Gekko PS math (trig via system_angleABf/distABf). Recorded
the resulting native action items (drop the spurious 0x802ee470 reads, bind the
same-TU angle/dist helpers, model both psndSFXOn_3D calls) as a ledger
breadcrumb; no native code changed.

Validation: docs-only change; no native/test/CMake edits and no make rerun
(latest recorded host state remains 87 CTest/90 Python and make all passing).

Next: documentation agent answers the five requests; native closures for
packed Jump sound cadence, water/void physics replacement, pose-table driven
recovery presentation, the original field main-loop/pause cadence and original
save persistence; full-game story/encounter closure stays open.

## 2026-10-12 — Getter plus status dispatch; case-table rescue

Closed BattleGetUnitPtr (NULL-domain table, −1-first, asymmetric setter,
487-site US row; snapshot definition with JP/US caveat) and BtlUnit_GetStatus
(28-slot ISO-read jump table, paired sign gates, nullable outputs).
Verification caught a badly misaligned case table in my own draft (wrong
lanes for statuses 0/2/3/16–24/27, inverted pair directions); rewrote it
from a machine dump of every block and re-verified all 20 lane blocks plus
6 paired blocks. Also resolved a phantom read-tool/listing discrepancy by
re-reading bytes from disk (disk won).

Validation: full re-verification, link audit, diff --check clean. Docs-only,
no make rerun. Next: zero-byte placeholder re-extraction; triangle/math
interiors. Full-game incomplete.

## 2026-10-12 — OW-WALL-01 traversal half: hit-vector skeleton

Delivered BATTLE_HIT_VEC_TRAVERSAL_ORDER for the overworld wall inbox:
4-region/call-census (49 bl all resolved: PSVEC family, checkTriVec_xz,
mapGetWork)/return skeleton with ISO-read SDA values, NaN-safe gate
patterns, and a verified region-1 counter/pointer asymmetry (r30+1/r31+84
vs r31+1/r30+84 elsewhere) reported without inference. Triangle interiors,
chunk ownership, and callback identity stay open per the request's own
remainder. Fixed two of my own early assertions (stride count, uniform-loop
claim) during verification.

Validation: machine-extracted census/edges/strides/counts, link audit,
diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — New denomination: flagged-callee contracts (CheckStatus, filter, search)

Survey found the register's flagged open callees now have reference pairs but
no dedicated contracts. Closed three: BATTLE_UNIT_CHECK_STATUS (null gate,
GetStatus byte, 6-compare, +759 test, exact {0,1}), BATTLE_HIT_CHECK_FILTER
(8-int/6-float marshal, hitVec call, six outputs, residue return),
MESSAGE_MSG_SEARCH (two-table binary search with gp fallback, no miss-null).
Caught two wrong relative links pre-commit (battle/→overworld, message/→battle).

Validation: full instruction re-checks (incl. corrected stfs count and
output-opcode proof), link audit, diff --check clean. Docs-only, no make
rerun. Next: BtlUnit_GetStatus, BattleGetUnitPtr dedicated contracts;
zero-byte placeholder re-extraction. Full-game incomplete.

## 2026-10-12 — Register complete: landing query 2, hitVec alias (43/43)

Closed checkLand2 as marioChkLandon2 (five-probe loop, variant dispatch,
lane quantization, angle tail; doc overworld/OVERWORLD_LANDING_QUERY_2.md)
and resolved hitVec as alias of partially-contracted hitCheckVecFilter
(SLOPES doc covers orientation/normalization; triangle phases stay open).
Final tally: 38 behavior contracts + 5 alias resolutions = all 43 register
rows addressed. Residual open work outside the register: angleABf/
system_angleABf bodies, hitVec triangle phases, and the usual lifetimes/
cadences. Fixed two of my own errors pre-commit (wrong bl census, wrong
callee-share claim).

Validation: instruction re-checks, ISO SDA reads, link audit incl. a
repaired register link; diff --check clean. Docs-only, no make rerun.
Full-game goal stays incomplete.

## 2026-10-12 — Porting-critical large function documentation

Added detailed documentation for high-priority functions needed for porting,
as identified in `docs/behavior/meta/US_COMPLETION_AND_PRIORITIES.md`. All
new files follow the per-subsystem folder requirement from AGENTS.md.

### Overworld category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioMain` | `0x8005a7e0` | 2528 B | `overworld/MARIO_MAIN.md` |

Primary field player update: motion dispatch, gravity, subsystem ordering,
battle transitions, and save-slot management.

### Battle category

| Function | Address | Size | File |
|----------|---------|------|------|
| `battleMain` | `0x800f904c` | 144 B | `battle/BATTLE_MAIN.md` |
| `BattleCalculateDamage` | `0x8005f958` | 1692 B | `battle/BATTLE_CALCULATE_DAMAGE.md` |
| `BattleSetStatusDamage` | `0x800fcf04` | 4220 B | `battle/BATTLE_SET_STATUS_DAMAGE.md` |
| `BattleDamageDirect` | `0x800feaa8` | 2852 B | `battle/BATTLE_DAMAGE_DIRECT.md` |
| `BattleCheckDamage` | `0x800fb9cc` | 2560 B | `battle/BATTLE_CHECK_DAMAGE.md` |
| `BattleCheckCounter` | `0x800fabcc` | 1444 B | `battle/BATTLE_CHECK_COUNTER.md` |

Battle system entry, damage calculation pipeline, status damage application,
direct damage, damage validation, and counter checking.

### Event category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evtmgrCmd` | `0x800316b8` | 5048 B | `event/EVTMGR_CMD.md` |
| `btlseqTurn` | `0x80042b94` | 7896 B | `event/BTLSEQ_TURN.md` |
| `btlseqAct` | `0x8011bb38` | 2220 B | `event/BTLSEQ_ACT.md` |
| `btlseqMove` | `0x8011c264` | 1680 B | `event/BTLSEQ_MOVE.md` |
| `evt_npc_wait_battle_end` | `0x8004efbc` | 184 B | `event/EVT_NPC_WAIT_BATTLE_END.md` |
| `evt_npc_set_battle_info` | `0x8004f0a4` | 100 B | `event/EVT_NPC_SET_BATTLE_INFO.md` |
| `evt_npc_battle_start` | `0x8004efbc` | 184 B | `event/EVT_NPC_BATTLE_START.md` |

Event VM command interpreter, battle turn/action/move sequence controllers.

### Message category

| Function | Address | Size | File |
|----------|---------|------|------|
| `msgAnalize` | `0x80081de0` | 5796 B | `message/MSG_ANALIZE.md` |
| `msgDisp` | `0x80050b88` | 2900 B | `message/MSGDISP.md` |
| `msgWindow_Main` | `0x8011bb38` | 2052 B | `message/MSGWINDOW_MAIN.md` |
| `msgMain` | `0x8008460c` | 1832 B | `message/MSG_MAIN.md` |
| `msgWindow_Disp` | `0x800808ec` | 1756 B | `message/MSGWINDOW_DISP.md` |
| `_evt_msg_print` | `0x800d2d60` | 1244 B | `message/EVT_MSG_PRINT.md` |

Message text analyzer, dialogue display, window update, message main loop,
window rendering, and event-driven message printing.

### Input category

| Function | Address | Size | File |
|----------|---------|------|------|
| `btlevtcmd_ResultACDefence` | `0x80107694` | 144 B | `input/BTLEVTCMD_RESULT_AC_DEFENCE.md` |

Defense action-command result handler.

### Field category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evt_bero_mapchange` | `0x8007aad0` | 176 B | `field/EVT_BERO_MAPCHANGE.md` |
| `evt_mario_jump_pos` | `0x800e4c54` | 1984 B | `field/EVT_MARIO_JUMP_POS.md` |

Room transition event command and scripted Mario movement.

### Memory category

| Function | Address | Size | File |
|----------|---------|------|------|
| `swSet` | `0x800858fc` | 64 B | `memory/SW_SET.md` |
| `cardMain` | `0x800b2508` | 1692 B | `memory/CARD_MAIN.md` |

Story switch setter and save/load state machine.

### Unit category

| Function | Address | Size | File |
|----------|---------|------|------|
| `pouchGetItem` | `0x800d5520` | 1188 B | `unit/POUCH_GET_ITEM.md` |
| `BtlUnit_Entry` | `0x80129644` | 1716 B | `unit/BTL_UNIT_ENTRY.md` |

Item retrieval from pouch and battle unit initialization.

### Format category

| Function | Address | Size | File |
|----------|---------|------|------|
| `__dec2num` | `0x80266834` | 4224 B | `format/DEC2NUM.md` |
| `__strtold` | `0x8026e68c` | 4108 B | `format/STRTOLD.md` |
| `__sformatter` | `0x8026cdf4` | 2476 B | `format/SFORMATTER.md` |
| `__ieee754_pow` | `0x8027067c` | 2096 B | `format/IEEE754_POW.md` |
| `__pformatter` | `0x8026ab78` | 1908 B | `format/PFORMATTER.md` |
| `float2str` | `0x8026b294` | 1820 B | `format/FLOAT2STR.md` |
| `__minus_dec` | `0x800d2d60` | 1276 B | `format/MINUS_DEC.md` |

Decimal conversion, string-to-float, printf/scanf formatters, power function,
float-to-string, and decimal subtraction.

### Platform category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioStMain` | `0x8006faec` | 176 B | `platform/MARIO_ST_MAIN.md` |

Main field system update orchestrator.

### Validation

All functions inspected with `tools/inspect_dol_function.py` against read-only
US G8ME01 ISO. Documentation records exact address, byte size, disassembly,
called functions, and data offsets. No original executable code linked or
copied.

### Limitations

- Many called helper functions remain undocumented
- Full structure layouts partially documented via field references
- Motion ID semantics and subsystem chains need further recovery
- Battle transition conditions require original event script analysis
- Shared helpers across commands need individual documentation

### Next tasks

- Document remaining medium-size functions (500-1500 bytes) by priority
- Recover full structure layouts for battle work, unit, and event contexts
- Cross-reference shared helper functions across command implementations
- Document remaining event command handlers and their argument processing
- Recover complete motion ID table and associated behavior chains

## 2026-10-13 — Native original Jump damage and owned Goomba reactions

`TrioAttack` now imports installed tik damage/wait wrappers and original kind/
part/data pointers. Fresh ordinary Goomba NormalJump calls the recovered power,
HP/FP calculation and direct application services, writes packed accounting,
and starts the original owned hit root with exact cancellation order. Original
generic damage/subset children execute their source waits and pose requests.
A missed first timing deals1 damage (2→1), completes Mario recovery/home movement,
and returns Goomba to automatic type28/KUR_S_1. Successful timing starts the
source second hit; two successful timings preserve20000/20100 and HP2→1→0.
First-success/second-miss also reaches0 with source full code10717. Source
status27 is written by the death child after the reaction and WAIT_FRAME30,
not by the HP setter. Lethal presentation currently suspends at AddRotate801115f8.

StopAC now persists the original1f54 finish countdown and clears all four
callback registrations only when Main returns0. Original stylish window/result
helpers, generic recovery jump integration and used reaction/event-wait leaves
are bound. Missing source callbacks preserve the first pending address. A completed
action retains its battle resources and locked field control pending original
turn scheduling; the development overlay now describes that boundary and stops
advertising a cancel action that cannot run. Source battle result remains0 and
story remains11. No win, reward or field return is inferred from HP0.

Validation: `make test` passed116/116 CTest checks and100/100 Python tests
(`/tmp/trio-hit-tests-final.log`). Focused original Jump checks cover first miss,
first-success/second-miss, both successes, exact per-hit flags/accounting, owned
reaction, source death status, alive idle and AC cleanup. `make all` passed
(`/tmp/trio-hit-all-final.log`), and `make debug-short` passed
(`/tmp/trio-hit-native-final.log`); no compiler warnings in those final logs.
`git diff --check` passed. Read-only asset conversion produced a1,351,884-byte
local event/data pack and48-byte PMTHIT1 mapping; no original files were staged.

Isolated Azahar run: `tools/emulator_smoke.py --binary
ttyd-2ds-debug-short.3dsx --save-fixture
artifacts/fixtures/underground-trio-story11.bin --seconds48 --input-script
tests/azahar_trio_jump_reaction.py --output-dir
artifacts/azahar-trio-jump-hit-reaction-final` passed launch/capture. The extended
30-second post-input capture shows original Goomba damage pose, Mario at home
and the pending reaction service. The first shorter run captured target selection
and the setup interval, so it alone does not verify damage. Native HP/control
words were verified by host integration tests, not emulator memory inspection.
The setup delay needs profiling; emulator FPS is not a hardware result. The
profile/saves remain isolated.

Limitations/next: bind original AddRotate/rotation publication, death sound/effect
requests, rewards and deferred removal; then install Mario idle and original turn
scheduler. Source Spike counter/Frankly warning and Paragoomba wing-fall remain
pending at CheckDamage8010a78c. Feedback/audience/effect/audio providers retain
requests but full presentation remains unbound. Difficulty/pouch level1, empty
equipment and seed0 remain scoped fresh-prologue adapters; original seed/math/
clock parity and other statuses need binding. Continue the original battle-end
and field return ownership after turns; the prologue demo goal remains active.

## 2026-10-13 — Original Goomba death, rewards and deferred removal

The native NormalJump now runs the original Goomba death core through all48
Y-spin requests,30 X-tip requests, reward bookkeeping, packed status clearing
and KillUnit. Rotations preserve integer inputs, raw SetRotate sentinel handling
and original angle normalization. The recovered fresh c_kuribo draw path now
uses Rx*Rz*Ry, zero pivot, the camera-dependent inclusive mirror gate, depth−5
and direct incomingA0/180/model scalar2. Original pose initialization confirms
flag8/100clear and unlinked−1 for this battle branch. Other paper/linked/status
rendering domains and hardware numeric/material equality remain separate.
Per-actor matrix/trig setup is shared across vertices; zero angles skip it.

PMTHIT2 adds the original relocated opponent group pointer. The native source
chain is Battle2738→FieldInfo, FieldInfo+c→NpcInfo, NpcInfo18→group26460.
This corrects the earlier stage-pointer interpretation. GetExp uses group count4
(factor1.3), original kind+e→unit+d level7 and the explicit fresh pouch level1,
yielding3 exp. GetCoin keeps its one irand100 draw and original chance60.
Stores retain FieldInfo14/Battlef00 owners, pending later pouch export.

KillUnit marks the unit during eventpassN; the next native battle call scans64
slots before action/event services, deletes wait/phase/hit IDs, clears matching
Battle120 IDs and unregisters last. The unit remains visible/registered on its
marking call. HP0 alone does not hide it. Native units/parts are preallocated,
with shared immutable animation packs; retired bytes remain diagnostic storage.
Packed status clearing preserves each change-info record's last word. Mario
now installs original wait_event80417c38 and returns to automatic M_S_1.

Validation: `make test` passed117/117 CTest checks and100/100 Python tests
(`/tmp/trio-death-tests-final.log`). Focused Jump tests verify both hit outcomes,
HP0 preceding status27,48/30 rotation counts, numeric axis/pivot/mirror boundaries,
reward accounting, retained registration during KillUnit, retirement exactly on
the next call, original Mario/Goomba idle, and unchanged pending variant targets.
`make all` passed (`/tmp/trio-death-all-final.log`); `make debug-short` passed
(`/tmp/trio-death-native-final.log`). Final build logs contain no warnings.
`git diff --check` passed.

Isolated Azahar: `tools/emulator_smoke.py --binary
ttyd-2ds-debug-short.3dsx --save-fixture
artifacts/fixtures/underground-trio-story11.bin --seconds48 --input-script
tests/azahar_trio_jump_reaction.py --output-dir
artifacts/azahar-trio-death-removal` completed. The early capture shows original
Goomba damage pose; the extended final capture shows Mario idle, Goomba absent
and the other two foes present. It reaches the turn-scheduling development
boundary rather than the previous pending death callback. Exact HP/reward/event
words are host-verified, not emulator memory observations. Captures do not prove
Dolphin image parity or every intermediate spin angle; no hardware FPS claim.
Original profiles/saves were untouched.

Remaining/next: source turn scheduler and repeated command ownership, Hammer/
Headbonk, Spike counter/Frankly warning and Paragoomba wing-fall. Death smoke,
star-point effects and original sound requests remain retained without full
presentation providers. Original RNG seed/earlier draw owners, equipment/item
rolls, pouch/field reward export and full resource teardown are not bound.
Battle result stays0 and story11 stays locked pending original conclusion/end/
field return; do not synthesize victory from HP. Profile native event-bank setup
latency separately. The full playable original-fidelity prologue goal remains
active and incomplete.


## 2026-10-13 — Persistent player turns and original Goombella Headbonk

The trio battle retains native battle/unit/AC resources between player actions.
Original packed PhaseFirst ordering, selection, phase-end checks and actor move
consumption are host-testable helpers. Normal post-action states wait for owned
reaction/event IDs and source message/reservation/audience/BreakSlot gates before
spending the move. Move10/8 hands Mario's completed turn to Goombella without
rebuilding the phase, replaying Frankly's lecture or resetting target HP.
Command sampling uses the live registry and retained HP/status.

Goombella installs original kind/part/damage/wait definitions and executes
Headbonk weapon80393ea8/root803941a8. Used CP wrappers, source power lookup,
raw position/rotation sentinels, scale and XY-angle trajectory are bound for the
fresh ordinary ground path. Original c_pkr idle/run/walk/jump/attack/talk assets
are locally extracted and batched with citro3d; per-pose clocks and battle
rotations apply to her action. Original data/script bytes stay ignored/read-only.

Focused tests pass: source stable priority/home ordering, positional tables,
deferred/dead actor selection, signed move counts, bypass flags/state chains,
Mario-to-Goombella live menu handoff, retained Goomba HP1 then Headbonk HP0,
once-only reward/retirement, original idle and both spent moves. `make test`
initially passed121/121 CTest and100/100 Python tests. With concurrent opening/
NPC work added, the latest full CTest run passes122/123; the remaining failure
is `field_actors_original`, tests/field_actors_tests.cpp:133 (scripted inactive
NPC contact assertion). This turn's battle checks pass. An earlier opening
pose failure was resolved by regenerating stale local opening assets; the
latest opening tests pass. Exact latest logs: `/tmp/trio-turn-tests-final.log`
and `/tmp/trio-turn-python-final.log`. `make all` and `make debug-short` pass
(`/tmp/trio-turn-all-final.log`, `/tmp/trio-turn-native-final.log`), with no
compiler warnings. ARM's int32_t/std::min deduction issue was corrected.
`git diff --check` passes. Concurrent native asset conversions caused one
truncated-image run; sequential rebuild passed.

Isolated Azahar capture `artifacts/azahar-trio-headbonk` uses the debug-short
binary, underground-story11 fixture,48-second boot and
`tests/azahar_trio_headbonk.py`. It shows original tutorial dialogue, Mario's
Goomba defeat/removal, Goombella Headbonk menu/target selection and her original
jump/rotation pose. This input run then attacks Spiked and correctly reaches
the retained pending damage service. It does not prove completed ground
Headbonk recovery; the host integration test covers that path. Native profiles/
saves are isolated; no hardware FPS or Dolphin-image parity claim.

Limits/next: phase11 onward/enemy AI, Spike counter and Frankly warning,
Paragoomba wing-fall, full first-turn/command init, original audience/BreakSlot
producers, equipment/pouch ownership, RNG and sound/effect/stylish playback.
Documentation inbox work now records the phase2→empty3→enemy4 transition and
variant callback/part-replacement owners. After both player moves the native
runtime stays locked at Move11 with result0/story11; victory is not inferred.
Fresh partnerHP10/level0 and empty equipment remain explicit adapters. The
original-fidelity prologue demo goal is active and incomplete.


## 2026-10-13 — Original player-to-enemy phases and Spiked approach

The native trio controller advances through source phase2→empty3→enemy4.
Move0 rebuilds the actor/event tables; selector1 hooks fan out and selector0
hooks run serially, preserving pair holes and keys. Frankly's original phase
script runs in3/4 and returns at its phase1 gate without replaying dialogue.
Move10 observes conclusion and actual remaining counts before phase end;
Move11 uses actual audience mask8/owned-event readiness; Move12 drains empty
message work before advancing. Original turn counts are not reset/incremented
at the player-to-enemy boundary. Native phase2 audience inventory remains an
explicit empty presentation adapter; mask8-set reaction and phase4-end audience
providers remain pending.

Enemy Move8 uses source selection/bookkeeping and Act0→3→6 launch ordering.
The installed Spiked attack root retains owner/action ID, then deletes its wait.
EnemyItemUseCheck binds the source held-item0 early return. Original signed side,
weapon masks, packed sampling and deterministic candidate weighting choose its
target. No fixed Mario target is substituted. Its original approach, jump and
rotation run through the verified relocated tik krb_get_dir93105b2c ABI and
original angle table. The native action reaches ResultACDefence80107694;
guard/impact services remain the next unbound boundary. Player menus now close
when the enemy root owns the action.

Typed pose lookup follows the original first requested-or-band-fallback scan,
including force1 only for29..39; Spiked43 correctly becomes TGK_S_1. Unit live
scale precedes outer rotation and display offset translates in world axes
afterward, following the documented source matrix order. Original scale/display setters are
bound; source requests/effect/camera playback and later impact remain incomplete.
PMTHIT3 (68bytes) retains original kind/damage/wait/attack, group and Frankly
phase pointers. Extracted assets and immutable capture binaries remain ignored.

Validation: `make test` passes126/126 CTest and101/101 Python tests
(`/tmp/trio-enemy-tests-final.log`). Integration verifies actual phases2/3/4,
two gated Frankly roots/no dialogue replay, phase-ID cleanup, original Spiked
root/Act3/owner, packed sampler actor4, player-menu exclusion, original fallback
pose and preserved field lock/story11/result0 at the guard boundary. Earlier
Jump/Headbonk HP/reward/deferred-removal checks still pass. `make all` passes
(`/tmp/trio-enemy-all-final.log`); `make debug-short` passes
(`/tmp/trio-enemy-native-final.log`); final logs contain no compiler warnings.
`git diff --check` passes. The previous session's NPC test failure is now fixed
by its own workstream; the complete current host suite is green.

Isolated Azahar final capture: `artifacts/azahar-trio-enemy-phase-final`,
immutable debug-short snapshot, underground-story11 fixture,48second boot,
`tests/azahar_trio_enemy_phase.py`. It shows Mario's missed Jump leaving Goomba,
Goombella menu/ground target, subsequent Goomba removal and Spiked approaching
Mario in its original airborne/rotated pose at the pending impact boundary.
It does not prove enemy damage or completed rounds. The capture precedes the
last menu-title cleanup; that change is host state/build verified. Original
profiles/saves are untouched, and no hardware FPS/Dolphin parity is claimed.

The first native capture stopped during Mario setup. A symbolized isolated
GDB run caught abort from renderer animation-string allocation. Actor layout
had changed while one native compilation was in flight; its object timestamp
masked an inconsistent layout. Forcing a fresh trio_attack object build
resolved the stop in the immutable final capture. Do not change shared data
layouts during active compilation; finish edits before launching final builds.
The event-bank setup delay remains unprofiled.

Next: bind private defence WorkUpdate/result/reset ownership, original Mario/
Goombella damage roots/poses, enemy CheckDamage and recovery; then Para dive,
Spike counter/warning, wing-fall, phase4 audience/end and next-turn/conclusion/
field-return owners. Inbox evidence now pins fresh no-input result2, Spike
power2/Para1, code10117 and original player reaction scripts. Held-item,
equipment/status/guard/lethal/pouch/effect/audio branches remain incomplete.
The full original-fidelity playable prologue goal remains active and unachieved.

## 2026-10-13 — Original trio enemy impacts, private defence and recovery

Spiked Goomba and Paragoomba now execute their original impact/recovery scripts
through the native event VM. Enemy CheckDamage admits the fresh ordinary source
weapons and uses original power2/1, damage calculation, direct application,
player-owned reaction roots and source guard codes. Mario/Goombella take source
selected targets, not a fixed Mario target: the no-input host fixture ends this
round at Mario8HP/Goombella9HP. Source default waits restart after damage; both
enemy attacks and tracked reaction barriers drain. Original damage/guard poses
are packaged from the read-only ISO.

Private battle pad WorkUpdate now receives full physical samples, independently
of general action triggers and without requiring an installed main AC callback.
Packed snapshots/history/reset and ResultACDefence use the recovered portable
logic in source sequence→private-pad→AC→event order. A guard fixture yields4,
reduces Spike damage to1 and completes the round. B superguard yields5, leaves
Mario10HP, reduces Spike to1HP, increments A280 once, retains codes8012a/137
and cancels the original enemy attack. WaitEventEnd and the original 40-sample
mode5 spin/float-output interpolation are bound. Superguard subsequently stops
at original TransStageFloorPosition8010510c; its exact three-operand ABI and
embedded-stage90/-50 producer are now documented for the next step.

Paragoomba DivePosition supports the used mode0/curves0,1,4, constant count,
source elapsed/intermediate/visible positions, signed amplitude and source
endpoint/sound ordering. Portable interpolation preserves low-word signed
integer squares for intpl_sub. Native 3D sqrt and sine are math adapters, not
bit-identical Gekko estimates/library results. Movement sound keys/counters are
retained and requests collected; playback handles/position/off remain adapters.

Phase4 end retains its mandatory RNG draw/reset/mask8 initialization. The
existing explicitly empty native audience follows source polls0→13→15, clears
mask8 and decrements/clamps BreakSlot turn before advancing phases5/6. Turn
bank4c→4d and phase reset preserve turn count1. Native player level now reads
low byte of the existing fresh pouch level1. Original next-turn audience owner
8019f298 remains pending; no new turn, victory, dialogue or field return is
fabricated. Nonempty audience/setup4 crowd/SP/bingo/throw mechanics remain open.

Validation: `make test` passes130/130 CTest and103/103 Python tests
(`/tmp/trio-impact-tests-final.log`). Integration covers earlier Jump/Headbonk,
phase order2/3/4/5/6/0, source audience polling, target HP, reset/guard/counter
codes, attack cancellation and unchanged story11/result0/field lock. `make all`
and `make debug-short` pass (`/tmp/trio-impact-all-final.log`,
`/tmp/trio-impact-native-final.log`); final native logs have no compiler warnings.
`git diff --check` passes. Numeric bit fidelity, complete successful player
follow-ups, lethal player damage, statuses/equipment/items, full presentation,
next-turn/conclusion/field-return owners and hardware performance remain open.
The full original-fidelity playable prologue goal remains active and unachieved.

Isolated Azahar capture: `artifacts/azahar-trio-enemy-impacts`, immutable
`artifacts/trio-impact-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and `tests/azahar_trio_enemy_impacts.py`. Manual review
shows Goombella's menu/Headbonk, ordinary Goomba removal, Spiked airborne
recovery with Mario's original damage pose (sample04), then both remaining
enemies and player waits restored (sample15/final). One-second captures do not
prove every Para/Goombella reaction frame or guard timing; host assertions cover
HP and ownership. The sequence reaches the explicit next-turn boundary and
keeps story11/field lock. Profiles/saves outside the isolated folder are untouched;
no hardware FPS or Dolphin parity is claimed.

Next: provision the source embedded-stage threshold90/lowfloor−50, bind the
three-operand TransStageFloorPosition and finish superguard recovery; bind
original next-turn audience/turn-first owners with their RNG/order before
opening another player turn. Then successful player follow-ups, Spike warning,
wing-fall, conclusion and original field/story return. Do not bypass these
owners with fabricated completion or reset turn bookkeeping at phase boundaries.

## 2026-10-13 — Superguard recovery and original fresh next-round control

TransStageFloorPosition now captures the three source operands, resolves X/Z,
uses original embedded-stage threshold90/lowfloor−50 and writes only raw Y.
Original B-superguard counter bounces/recovery and owned-event barriers drain.
No unconditional zero floor or fabricated completion is used.

TurnEnd retains original four-unit group metadata (including defeated Goomba),
sum23/player-level1/threshold46, chance increment, mandatory RNG, unique category
queue and target-count arithmetic. The empty native crowd follows actual
zero-amount/capacity gates; known chance categories filter through the no-SysCtrl
record path without invented RNG. Nonempty crowd allocation/movement,
threshold target-size changes and other category graphs remain explicit boundaries.

Fresh initialization now calls the original portable TurnFirst processor from
count0 to1; next round calls it to2. Rule/fortune/fog/record/move/priority and
unit U24 ordering reuse native shared-memory services. Frankly's original kind
is resolved from the fourth source group unitset, including level/priority/data
instead of the old synthetic kind row. The integration confirms U24=2 despite
his count0 and the source turn record counter2.

Original Turn2→3→5 and ordered zero-status/zero-Happy-badge scan branches,
word/halfword countdown tails and Turn4a/4b return run in the original logical
order. Admitted nonfresh periodic damage/recovery/Happy-effect paths remain
pending at their actual provider; they are not skipped. Original get_turn feeds
Frankly's phase1/turn gate. Phase0→1→2 then uses existing original phase hooks,
event barriers and actor selection to open Mario's next command menu. The
first-turn lecture does not replay; HP remains Mario8/Goombella9 in the no-input
fixture, Mario9 after A guard, and Mario10 after B superguard (SpikedHP1).
Field HP/story11/result0/control lock remain unchanged until original field
return is implemented. The demo is still incomplete.

Validation: `make test` passes133/133 CTest and103/103 Python tests
(`/tmp/trio-next-turn-tests-final.log`). Focused integration verifies full phases
2/3/4/5/6/0/1/2, count2, source Frankly pointer/U24, record counter2, chance3,
queue0, actual Mario command state, retained HP, guard/counter codes, attack
cancellation and recovered hit-event cleanup. `make all` and `make debug-short`
pass (`/tmp/trio-next-turn-all-final.log`, `/tmp/trio-next-turn-native-final.log`),
without compiler warnings. `git diff --check` passes. Full nonfresh status,
audience/crowd, successful player follow-ups, Spike warning, Hammer, wing-fall,
conclusion/field return, original numeric/cadence parity and hardware performance
remain unfinished. The original-fidelity playable prologue goal remains active.

Isolated Azahar capture: `artifacts/azahar-trio-next-turn`, immutable
`artifacts/trio-next-turn-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and updated `tests/azahar_trio_enemy_impacts.py`. Manual
review shows original Mario damage pose/Spiked recovery (sample04), then
`source-next-player-command.png` shows restored idle actors, surviving Spiked/
Paragoomba and Mario's Normal Jump/Normal Hammer menu. Host assertions establish
turn2, HP and event ownership; screenshots do not prove native guard windows or
every reaction frame. Other emulator profiles/saves are untouched. No hardware
FPS or original-runtime parity is claimed.

Next: bind original ordinary Hammer wrapper80416c40/core8035d6d4 and W80353de8,
its held-left command3 packed/general-pad ownership, source Hammer power and
original M_H poses. Preserve actual approach/waits/result branches instead of
inventing an impact schedule. Then conditional Spike warning/counter, successful
Jump/Headbonk follow-ups, Para wing-fall, conclusion and original field/story
return. Documentation work has recovered the Hammer dependency map from the
request inbox/read-only original; full prologue completion remains unproven.

## 2026-10-13 — Original ordinary Hammer event and held-left native command

Original weapon80353de8 launches wrapper80416c40/core8035d6d4 through the
persistent VM. Its approach/ftomsec windup, M_H_1A/B/C poses, StartAC/ResultAC
poll, swing WAIT7 and source damage/result/recovery branches run unchanged.
No fixed impact tick is introduced. AcSetParamAll preserves resolved sentinels,
mode remains raw, and the level helper reads signed pouch99. Fresh pouch98/99
are explicitly initialized1; imported field upgrades remain unbound.

ID3 now round-trips every documented packed Hammer field, including source
registration, shared signed forced counter, stage/countdown/scratch/handles,
actor automatic/options/difficulty and suppression. General pad atBf2c receives
full native samples independently of private guard B1d1c. Held-left comes from
B1034, not an A/B edge. Source manager return0 clears main only; original1007
clears all four pointers. Stop writes1005 and the original60-step cleanup runs.
Raw-output0 evt_snd_sfxon retains its original sound key without writing a fake
voice result. Charge cue handle0 is a presentation adapter; actual cue/macro
playback and original gauge drawing remain unfinished. Circle Pad prompts are
presentation adaptations, not story dialogue.

Hammer power now reads the selected source weapon's words, actual pouch98/99
and U2ff/U300 bonuses through recovered callbacks/calculators. Fresh Hammer
hits ground Goomba(kind1) and Spiked(kind3); W78 resistance2 prevents the early
spike counter in this admitted scope. Para's original flying kind is2. No-input
and early release cause H1/code10117, leaving Spiked1HP. Stage5 held-left release
produces result2/class3/H2/code30717, original Spiked death/reward/registry and
visual removal, then Goombella's command menu. Earlier guard/counter/turn
bookkeeping remains green. HP0 does not bypass original death or manufacture
victory/field return.

Original M_H_1A/B/C are imported from read-only ISO a/b_mario/a/a_mario-, using
the existing atlas/mesh pipeline. Data-only script banks already contained the
source Hammer. Extracted assets/packs and binaries stay outside source history.
No new dialogue is authored, and no generated Hammer artwork or tones are used.

Validation: `make test` passes134/134 CTest and103/103 Python tests
(`/tmp/trio-hammer-tests-final.log`). Focused integration covers source target
occlusion (Spiked only after Goomba removal; airborne Para excluded), timeout,
early/full-stage release via full physical snapshots, exact HP/result/class/code,
original death/removal and Goombella handoff, four-pointer cleanup, story11/
result0/field lock. `make all` and `make debug-short` pass
(`/tmp/trio-hammer-all-final.log`, `/tmp/trio-hammer-native-final.log`) without
compiler warnings. `git diff --check` passes.

Isolated Azahar: `artifacts/azahar-trio-hammer`, immutable
`artifacts/trio-hammer-snapshot` debug-short binary/ELF, underground-story11
fixture, 48second boot and `tests/azahar_trio_hammer.py`. Reviewed screenshots
show Hammer menu/Spiked target, original hammer geometry/windup/swing,
Circle Pad prompt and restored Goombella Headbonk menu after no-input recovery.
Native early/full charge-window success is host-verified, not captured/measured
on hardware. Other emulator profiles/saves are untouched; no hardware FPS or
Dolphin parity claim is made.

Next: bind original Para wing-fall/damage graph and ground replacement parts/
poses/scripts. ChangeKind writes U8 while retaining original U10; source model
bank remains c_kuribo, so do not invent a replacement resource. Then Jump/
Headbonk successful follow-ups, Spike warning/counter, full conclusion and
original field/story return. Gauge/voice, optional stylish/teaching/mask2,
nonfresh status/crowd/equipment and original timing/numeric parity remain open.
The full original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Native original wing fall, grounded attack and final removal

A fresh failed Headbonk on flying Para(kind2) now adds source reaction400000
only through weapon4000/part800/per-hit100 admission, yielding410117/HP1.
Original installed damage/root/child owns the complete fall: attribute clears,
PTK_D_1/wait1000, raw pose/data tables, actual part replacement, KUR_D_1,
JumpPosition10 toY0/home update, wait500, ChangeKind1, original grounded init,
wait1 and final pose. Integer SetHomePos preserves raw XYZ truncation; it is not
a float/sentinel setter. No independent fall timer, teleport or HP reset is used.

ReplaceParts preserves the specified old fields and imports the original ground
record. Native resource recreation reuses the preloaded one-part c_kuribo bank;
full multiparts/resource lifetime remains an adapter. Current kind1 changes
without replacing original kind-definition93521b48, identity, level or HP1.
Original ground wait/attack/damage/confusion/sound pointers install through
source event setters. The grounded attack executes its real root, shared
krb_get_dir93105a14 angle-table ABI and default power1; native generic-jump
numeric services remain adapters. The next player menu follows source phase/
barriers and retains HP. Live part attributes/current kind refresh command
sampling and original US name: grounded unit5 is Goomba and admits Hammer.

Original c_kuribo now packages KUR_R_2 and keeps original descriptor0 KUR_Z_2
first. Authored PTK_R_2 is absent from the original bank; original battle uses
shared missing-name fallback to descriptor0. No replacement animation is
invented. Native source assertions preserve exact tables/part/event pointers,
original definition and floor/homeY0. All asset extraction remains read-only;
originals, compiled packs and snapshots remain ignored.

The host scenario now defeats and removes all three opponents through source
hits, death/reward scripts and deferred registry retirement. After wing fall/
grounded attack, Mario's missed Hammer kills the remaining HP1 Goomba with
10717. Original conclusion sets bank7=07000000. Runtime now exposes the exact
next ordinary owner btlseqEnd80215234 instead of silently remaining active.
It does not fabricate result1, export rewards/stats or release field controls;
story11/result0/control lock remain until original ending/return is implemented.

Validation: `make test` passes134/134 CTest and103/103 Python tests
(`/tmp/trio-wing-tests-final.log`). Integration covers original Hammer and prior
guard/turn behavior, full wing transition, HP/definition retention, exact part/
pose/data/ground event pointers, grounded attack's sampled one-damage target,
correct name/Hammer admission, final original death/removal and ending boundary.
`make all` and `make debug-short` pass (`/tmp/trio-wing-all-final.log`,
`/tmp/trio-wing-native-final.log`) without compiler warnings. `git diff --check`
passes. Capture helper Python syntax check passes after suppressing xdotool
verbosity; that edit changes only logging, not the captured input sequence.

Isolated Azahar: `artifacts/azahar-trio-wing-fall`, immutable
`artifacts/trio-wing-snapshot` debug-short binary/ELF, underground-story11 fixture,
48second boot and `tests/azahar_trio_wing_fall.py`. Reviewed captures show the
flying target/Headbonk, wings present before replacement, grounded KUR_D_1
(sample10), restored ground idle/menu and original Goomba name alongside Spiky
Goomba after the enemy round. Screenshots do not prove every intermediate frame,
exact HP, charge success or final enemy kill; those are host assertions. Other
emulator profiles/saves are untouched. No hardware FPS or Dolphin parity claim.

Next: bind original btlseqEnd victory/result/audience/reward/pouch/export owners
and original field-result1 gorotuki continuation. Source first-ending audience
services do not become no-ops merely because the current crowd adapter is empty.
Retain the original FieldInfo/NpcInfo callback producer; do not force itnull or
skip ending presentation to fabricate field return. Successful Jump/Headbonk
follow-ups, Spike warning/counter, gauge/voice, optional stylish/teaching,
nonfresh status/crowd/equipment and full story/hardware parity remain open.
The original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Native initial original trio victory stages

After original enemy death/deferred removal and conclusion, the native trio
now executes btlseqEnd's initial result selection through the existing portable
source-priority helper. Packed battle information I10 becomes1; the field
sequence/result consumer remains unchanged until real teardown/publication.
Story11 and field control lock remain. ResetMoveColorLvAll scans all64 slots
and writes only U311=ff to surviving records, including Frankly.

The source original group, retained after enemy deletion, owns Great/Fast
victory audience checks. Both real AddTarget requests execute the exact zero
Sub early return and outer bias/target clamps with original packed constants.
Nonzero Sub remains pending801a18cc. Original Koura deletion stream803b0c50
runs as an actual owner-Mario priority10 root; the fresh no-guard leaf resolves
its actor/output and returns-1. End work uses existing SDK heap0 allocation
adapter, initializes only the original used fields (including separate E30=0),
and retains the required irand120+60 request. Display/clap requests are traced;
full source crowd/audio/end display remains an adapter.

Original state05 uses ordered SetStatus(U,id,0,0), party then Mario for IDs0..24,
rather than ClearStatus: it preserves Charge16 and IDs25..27 and retains the
portable move-count/opposite-status semantics. Original owned-event barrier and
all-unit data3f lookup follow; the existing original zero-terminator query is
reused. The precise next pending owner is DropMaterial801141bc at bank7 state
07000006. Full drop RNG/equipment/pouch ownership, rewards/EXP/victory art,
fade/export/sequence publication and preserved field dialogue return remain
unfinished. Nonfresh callback, alternate party flag/body and Mario10 paths
remain explicit pending providers; no false field completion is produced.

Documentation agent recovered and persisted the full original ending state
map, local audience/status/Koura ABI and bounded fresh FieldInfo callback
constructor evidence, with remaining work in DMG-02/EVT-01/JUMP-01 inboxes.
No original assets or binaries were added to source history.

Validation: make test passes137/137 CTest and103/103 Python tests
(/tmp/trio-ending-tests-final.log). Extended source scenario removes all three
enemies, verifies packed result1 versus retained field result0, exact pending
DropMaterial/state06, initialized end fields/RNG range, reset color bytes,
two audience adjustments, real Koura callback and clap request. Prior attacks,
wing fall, enemy rounds and guard checks remain green. make all passes
(/tmp/trio-ending-all-final.log); git diff --check passes. No new emulator
capture was run for this controller-only increment; visible victory and field
return are not yet implemented or claimed. No hardware/Dolphin timing parity.

Next: bind source DropMaterial and correct live pouch/resource ownership,
then EXP/victory poses and presentation, fade/export/field sequence return,
and original gorotuki continuation. The full playable prologue goal remains
active and incomplete.

## 2026-10-13 — Native opening chain, original scene consumers and combined verification

Integrated the normal demo → title → four file slots → fresh-file opening →
source story3 harbor checkpoint, with separate debug-short target. Existing
native-save compatibility is not required by the user; automated testing still
uses isolated profiles/fixtures only. Original37 demo/11 opening payloads,
message IDs, tokens, attribution and input gates remain source-owned.

House rendering now consumes original actor roots/poses/local simulation clocks,
source cameras, door curve11/Q1024 requests and original hinge/postbox geometry.
Fixed queued actors overwriting one another's GPU vertex ranges. Original demo
marketplace rendering consumes Peach/merchant/chest choreography, original opaque
pass1/color/cull surfaces and the dedicated gor_01 map with five hidden interiors
and both source wall subtrees. Identical selected actor samples are cached,
avoiding repeated wrapped-chest tessellation while its pose is static.

The original OFF_d_meku_right A_1 page mesh and rune back now render. Texture
admission/final180 sample acknowledgements gate the four turns; original scale,
third-message timing and curve11 book request are retained. Native orthographic
outer projection/depth is explicitly an adapter. A first smoke exposed paper
behind its2D underlay; corrected depth layering is visible in focused captures.
No substituted mesh/art or rewritten story dialogue was added. Golden-chest
forensics separately proved the colorful rug objects are original S_roten stock;
independent original conversion matches native chest PMANIM4/T3X/crop bytes.

The harbor source event owns admission, messages022..024, charges/HP minimum and
blocking setup2 request. Native tutorial entry runs original init/phase/t1_000
on source-selected bti_01/stg_00_0 and original BGM126. Packed three-unit/part,
work/HP/hook/target ownership and source Crump approach/jump/precheck services are
integrated. ResultACDefence now consumes explicit physical samples; abstract
story A/B edges cannot stand in for guard input. Original fresh pouch, Mario
init/wait hooks and shared counter/hit-event cancellation/priority services are
bound. Actual CheckDamage, legitimate first-turn/command/attack ownership,
turn/end/reaction/reward and genuine field return remain unfinished.
Result0/story4/save blocking are retained; no victory or source story5 is faked.

Validation:
- Combined committed tree4ed0b6c plus408df97/faa0184 in
  /tmp/pm-opening-merged-4ed0b6c: make test135/135 CTest +103/103 Python;
  make all passed (/tmp/opening-merged-mario-{test,build}.log).
- Fresh normal Azahar route completed and saved source story3;
  artifacts/azahar-opening-merged-fresh contains title/files/house/harbor captures.
- Original tutorial intro reached its pending combat boundary with three distinct
  actors; artifacts/azahar-harbor-packed-intro and
  artifacts/azahar-harbor-fresh-mario-intro (fresh pouch/Mario init). This is not
  a fight result; screenshots supplement the host initialization assertions.
- Original unskipped37-message demo completed its final input gate before the
  paper depth fix; focused corrected fold/rune/back/narration captures are in
  artifacts/azahar-original-paper-depth. The corrected full230-second demo now
  reaches its final input guard; B retains the demo and A enters title, captured
  in artifacts/azahar-corrected-full-demo. Screenshots supplement host exact-ID
  and guard assertions; they are not full original-image/timing parity.
- make debug-short passed; isolated25-second boot in
  artifacts/azahar-opening-debug-short-long shows direct development field.
  The earlier5-second capture was still blank during initial loading.
- Read-only source verifiers:15 house object/matrix anchors,51 demo scene/map
  anchors and53 book/capture constants; independent10-sample chest UV/byte check
  passed. Source checks are static evidence, not Dolphin observations.
- Isolated defense integration129/129 CTest +103/103 Python and native build passed;
  its bridge was then rechecked in the combined132-test tree. No user profile,
  original ISO or user saves were modified. Originals/artifact packs are ignored.

Local playtest binary: artifacts/ttyd-opening-faa0184.3dsx (normal opening chain,
verified checkpoint; combat remains pending). New contracts live under story/
and display/, indexed by docs/behavior/README.md. Full-game and original-image/
New2DS hardware parity are not claimed.

Next: bind legitimate source startup/first-turn/command/Jump owner and full
damage/reaction services; retain fresh pouch through the actual field/save path; then real battle end and
original field continuation. Finish
original cover geometry/outer projection, fade/light/tree/environment, sound and
crystal/radiation consumers; profile actual New2DS memory/performance. The goal
remains incomplete while the real harbor fight/return and these scene services
are still pending.

## 2026-10-13 — Original harbor field battle construction

Fresh harbor admission now constructs source NPC battle information and original
FieldInfoInit/battle_init fields. Source setup preserves its seventeen RNG
requests on an explicit seed0 adapter and validates setup2 group/stage pointers.
Fresh no-party/no-field-attack state yields firstAttack0; configuration20000010
is not substituted for a first strike. The packed Fbat/NPC/FieldInfo allocations
are explicit native carriers; field internals outside those source fields remain
unbound. FirstAct/resource/fade/command ownership remains the next startup work.

Validation in /tmp/pm-harbor-integrated-fa27933: make test134/134 CTest and
103/103 Python, make all passed (/tmp/opening-field-info-{test,build}.log).
Original packed tests check actual group/stage/firstAttack values; the read-only
verifier retains source addresses. No victory/field return is published.

## 2026-10-13 — Native original trio drops and supplied prologue pouch

BattleInformationSetDropMaterial801141bc now runs its complete valid source
HP→FP→item algorithm with typed services: signed current-unit ratios, upper
cap only, original threshold/outer/inner draws, story<=19 weights, both actual
badge314 queries, original weighted first-unit/back-selected item routes and
25-word Item Hog fallback (including zero outcomes). It writes only setup
S10/S14/S0c; original death coin I14 and selected/back/returned slots remain
distinct. It does not deposit drops or coins into the pouch. The original
source data addresses/tables come from the existing unchanged local pack.
Original encounter-selected item setup and shared field RNG ownership remain
separate unbound inputs; the used native fresh no-item admission is preserved.

Added an explicit source-prologue initial pouch snapshot from recovered pouch
Init and initial-stage grants6/32hex/9, tier writes1/1, source insertion order
[9,32,6], coins100 and fresh zero-equipment resource revision. The original
localized default partner name is copied from msg/US/global.txt; only the name
stays resident, not a second complete global bank. This implements the pouch
part of the authored initial-stage snapshot, not proof of ordinary original
title/new-game callback reachability or the full original stage initialization.

Native fresh-file handoff now owns this block in Progress via shared storage,
retaining it through Game/room rebuilds. Mario's actual field HP bridges into
entry once. TrioAttack accepts the supplied live block, initializes actual
Mario/Goombella kind HP/FP copies, clears status then imports source resources
and initialized seven-party snapshots. Supplied current HP7/FP2/partyHP6 stay
7/2/6; neither import nor a later turn heals them. Mario maxFP/currentFP no
longer start incorrectly at0. Fresh snapshots restore Goombella scale1 and
preserve kind FP5 through the HP-only partner import.

Legacy40-byte checkpoint files still omit this block; their development entry
uses the explicit initial-stage snapshot plus saved Mario HP. This is not
original inventory/equipment/save reconstruction. Original later item/equipment,
full field/player/harbor shared pouch services and serialization remain open.
The same source-owner block is preserved during live new-file room traversal;
normal loading of a legacy checkpoint is a separate limited adapter.

Ending06 now computes actual drops and advances08. Fresh FogForceStop's exact
no-fog early return is bound. Original signed stockEXP clamp/fortune-byte/
level99/upper-cap/halfword copies prepare E8/Ea, while sourceflagsE0c/Bef8
writes retain original roles. The all-three scenario yields10 stock/remaining
EXP and reaches real _GetExpIcon_Init802149ec; pouchEXP stays0, sourceIresult1
is distinct from fieldresult0, story11/control lock remain. No icon/effect,
reward counting, victory/fade/export or field completion is manufactured.

Validation: make test passes141/141 CTest and103/103 Python tests
(/tmp/trio-drop-tests-final.log). Focused DropMaterial tests cover signed ratios,
upper-only cap, draw order/count, early-story mandatory item roll, weighted
back/selected slots, badge multiplication and fallback. Prologue pouch tests
verify source fields/grant order/resources and retained import. Native trio
integration verifies supplied HP/FP/partnerHP, shared pointer survival through
Game restore, original all-enemy death/removal, actual drop fields/two badge
queries/3-or5-or7 draws, original10EXP preparation and exact icon boundary.
Prior attack/wing-fall/guard/round tests stay green. make all passes
(/tmp/trio-drop-all-final.log) without compiler warnings; git diff --check passes.
No new emulator capture or hardware/Dolphin timing claim for this state/owner
increment. Original assets/packs/binaries stay outside source history.

Documentation agent also recovered the next actual19icon404 frames and
positions/alpha policies, original US PointGet texture strips and source08→0b
fallthrough/counting. Next: import/render those original reward assets, bind
source count/victory children/fade/export/sequence and preserved gorotuki field
continuation, then finish shared field pouch/save/encounter input ownership.
Full original-fidelity playable prologue goal remains active and incomplete.

## 2026-10-13 — Source startup controllers, native readiness and original curtain

Integrated original Init/SequenceManager, US six-effect readiness row, ordinary
firstAttack0/shared FirstAct states1..14, mandatory system/Mario UnitSetup and
complete portable UnitEntry storage/control bodies. Actual fresh source order is
system kind220/ID0, Mario kind222/ID1, then Crump/ID2 and spawned helper/ID3.
These controllers retain required service providers. They do not replace the
current bounded intro with an unconditionally successful initialization or enable
an internal attack diagnostic as a legitimate command/turn owner. Packed literal
registry IDs and public three-actor visual slots still need coherent migration.

Actual9 original US startup banks/25 descriptors/23 supported clips and original
GPU atlases now admit source readiness. Two unsupported dormant accordion clips
retain their raw descriptors/bank; admission does not acknowledge their playback
or paper field capture. The original p_jyabara texture owner is p_slit. Wrapped
UV/pass/cull/RGBA remain in explicit PMANIM4 conversion; default ordinary capture
kind rejection is retained. Configured ISO overrides pass through all exporters.

Bound original fade23/context2 OFF_d_maku_left_right A_2, actual14 keys/216
triangles/0..45 span and original atlas. Portable fade polls resource admission,
uses source pre/post-main progress queries, RGB truncation*.98 afterprogress.5,
then original release/flag2 atprogress1. Duration200 is retained rather than used
as a substitute timer. Native completion uses actual evaluated pose and simulation
time. Unique queued ranges retain all actors. Native image-plane mapping remains
an explicit adapter; admission occurs after field GPU rooms are released.

Validation: isolated integrated make test144/144 CTest +103/103 Python and
make all pass (/tmp/opening-startup-unit-final-{test,build}.log); focused curtain
original-pack and callback-order checks pass. Read-only original verifiers pin15
startup/UnitSetup/UnitEntry spans,10 US resource/row anchors and12 curtain provider
hashes/dispatch/constants/keys. Isolated temporary curtain component smoke in
artifacts/azahar-original-curtain-projection caught and corrected a100x image-unit
error. Reviewed sample129 shows original pleated curtain moving off screen and
sample130 restores stage/actors. That temporary trigger is removed before the
normal build. This proves the bounded native consumer, not source FirstAct
choreography, complete fade/camera/material parity or a battle result. User
profiles/saves and the source ISO remain untouched; original packs are ignored.

Next: bind required UnitEntry helper services and literal registry/visual mapping,
then actual Init/FirstAct/Turn/command/damage/reaction/end/reward/field owners.
Retain fresh pouch through actual field/save ownership. Full source harbor fight
and return remain pending; result0/story4/control/save barriers remain intact.
Original cover/fade/environment/effects/audio consumers and hardware parity remain
ongoing. The opening playtest goal remains active and incomplete.

## 2026-10-13 — Original ten-point ending tail source delivery

Documented [ordinary trio ending tail](behavior/battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md):
exact0c..1f timer/invocation order, independent effect softdelete/pointer
release, voice/bodytype42 conditions, source fade10 refresh/strict completion,
Rule0 and ordered persistent-pouch/field-party export. Original field wait and
161-command scene remain the postbattle owners; no fabricated heal/story write.

Validation: read-only US DOL complete bodies/tables/constants; scoped
`git diff --check`. Documentation only; no native edits, builds or commit.
Next bind actual PointGet presentation/counting/tail and original icon creation
clock/window geometry, then full sequence/field restoration; alternate EXP/
level-up/fortune/audio/GPU/field constructor services remain open.

## 2026-10-13 — EXP icon clock and original frame source delivery

[Icon clock/window frame](behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md)
closes per-record source404 frame0/delay6/reset/Main/hidden advancement and
same outer-pass creation/decrement ordering. Original type0 window texture
72x72CMPR and36-vertex nine-slice geometry/UV/color material are documented.

Validation: read-only original bodies/TPL header,180float FIFO symbolic census
and `git diff --check`. Documentation only; no native edits/tests/builds/commit.
Next native providers can replace borrowed clock/frame placeholders; actual
icon draw, camera/GX/palette lifecycle and original raster comparison remain.

## 2026-10-13 — Original US PointGet complete layout delivery

[PointGet source layout](behavior/effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md)
closes original US phrase/digit widths/cap, foreground-derived shadow geometry,
ordered matrices/quads/UV and effect entry/Main/softdelete lifetime for10EXP.
Fresh line half-width322 and actual scaling200/322; original assets preserved.

Validation: complete read-only original bodies/constants and quad/matrix paths;
`git diff --check`. Documentation only; no native edits/tests/builds/commit.
Next bind original PointGet graphics/counting; source camera/display dispatcher
state/pool/texture scheduling and actual numeric/raster comparisons remain open.

## 2026-10-13 — Original EXP icon bottom anchor correction evidence

[Used404 icon draw](behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md#2026-10-13-used404-draw-anchor-follow-up)
confirms sourcecenterX/bottomY quad and actual1011 geometry/flip/scale branch.
BigY25 spans25..61; smallY0 spans0..18 before declaredviewport scaling.
Validation: read-only complete iconDisp/iconGX/constants; git diff --check.
Documentation only; no native edits/tests/build/commit. Native renderer can
replace centerY assumption; sourcecamera/GX/runtime raster comparison remains.

## 2026-10-13 — Native trio EXP icons and victory events

Implemented actual ending08→09→0a with original19-entry EXP setup, source
positions/scales, alpha64 increments/cap255, hidden behavior and per-record
six-update frame clock (including hidden records). Creation-pass120 wait
decrements to119 before presentation. Source data40 creates the original
Mario/Goombella victory roots with source priority/owner/U2a8 tracking.
Original404 frames418..421 and M_V_1/M_V_2/PKR_Y_1 are locally packed;
Citro2D preserves source texture RGB/alpha, bottom anchors and type0 nine-slice
frame geometry using original descriptor0 and a declared half-size viewport.
Native reward UI uses a layer independent of stage depth; existing original
victory PCM selects during this positive-EXP prefix.

Validation: make test passed151/151 CTest and103/103 Python tests
(`artifacts/trio-exp-tests-final.log`). Final make all and make debug-short
passed (`artifacts/trio-exp-bottom-all.log`, `artifacts/trio-exp-bottom-debug.log`).
Original assets regenerate locally; Python capture syntax and git diff --check
passed. Host checks pin ten-point setup, alpha/visibility, source wait/result
boundary and original victory pose names; icon metadata checks pin all four
original40x40 frames, delay6 and wrap. Integrated workspace also includes
concurrent workstreams; this commit stages this increment only.

Actual attacks/removal in isolated Azahar reach the original reward prefix.
Earlier diagnostic captures caught white texture tint and incorrect center-Y
anchoring; both corrected from native/source evidence. Final immutable binary
and matching ELF are `artifacts/trio-exp-bottom-snapshot/`; final isolated
capture evidence is `artifacts/azahar-trio-exp-bottom/`; reviewed final capture
and enlarged diagnostic confirm the original frame, bottom-anchored animated
star and victory poses after actual enemy removal. The original ISO and
user emulator profile/saves remain untouched. Full original frame/camera/GX
raster parity and hardware performance remain unverified.

The native stop remains actual PointGetEntry8021b70c, bank0700000a, before
counting. PouchEXP remains0, fieldresult0/story11/control lock remain intact;
this is not a completed victory or prologue. Next implement original US
PointGet strips/lifecycle, bank0b counting and ordinary0c..1f fade/export/field
return, then resume unchanged scene dialogue. Original camera/crowd/voice,
WIN2 routing, shared RNG/equipment/save ownership, alternative rewards/level-up
and broader prologue owners remain incomplete. Goal remains active.

## 2026-10-13 — Source book composition, captured paper and scene consumers

Implemented original procedural cover/page-base/illustration composition, source
final adjust over the newly visible market, and actual native GPU page capture.
Four independent RGBA8 textures acknowledge only a completed, successful old
composition copy. Native readback crop/rotation/padding/Morton layout has host
coverage. Focused isolated Azahar first exposed vertically inverted page fronts;
the actual display-transfer row mapping is corrected and subsequent fold captures
show upright original artwork. Screenshots also exposed opaque blend state leaking
into bottom-screen text; book consumers now restore alpha blending.

Original book6000ms fade, marketplace start after3000+1000ms and750ms retiring
tail now feed two fixed original-PCM stream owners. Original crystal/radiation
subtypes2/6/3 use original source geometry/masks and independent simulation
clocks. Original house sai_ki2/sai_ki1 geometry and finite4000ms opposite-X
parallax are restored. Source UnitEntry helpers now cover initial fields,
vectors, equipment, pouch import/revision, pose-name lookup/display admission;
source Init state4 returns after setting5, rather than falling through. These
helpers remain distinct from still-pending active startup/turn ownership.

Validation: integrated make test155/155 CTest and105/105 Python passes
(/tmp/opening-composite-capture-corrected-test.log). Native make all passes
(/tmp/opening-composite-alpha-restored-build.log). Focused isolated captures
artifacts/azahar-source-book-capture-corrected show upright old-page faces and
original fold geometry. A subsequent complete demo run is pending at this
commit; no original raster, audible/hardware parity or complete battle proof is
claimed. Original source hashes/anchors and asset converters remain read-only;
all raw resources/compiled packs/captures are ignored local products.

Also verified the clean combined primary ca0565a checkout:150/150 CTest,103/103
Python, native make all and isolated fresh-file route to saved source story3
Rogueport checkpoint pass (artifacts/opening-current-ca0565a-validation). Initial
combined build/test ran out of temporary-disk space; after archiving only our
generated outputs/caches to ignored project artifacts, the full rerun passed.
User emulator profile/saves and source ISO are untouched.

Next: complete native unskipped demo and new-file regression, integrate literal
harbor registry/entry/local-frame providers and original Init/FirstAct/Turn,
then command/damage/reaction/end/reward/field return. Original fade/camera/glide/
environment services and hardware comparison remain open. Result0/story4/save
barriers remain intact. The opening goal remains active and incomplete.

Follow-up: the first complete new-composition run raised a native exception
at the book-to-market transition, so it did not pass. Azahar's log showed
unmapped stack writes in original actor hierarchy evaluation; native default
stack is32KB and frontWorld reserved roughly16KB for its Game scratch snapshot.
That renderer-only snapshot now has scene-owned heap storage. Smoke captures
now detect native exception dialogs even while Azahar remains running. Two
focused harness checks pass; complete native rerun is pending.

## 2026-10-13 — Independent source EXP count/end audit

[EXP count follow-up](behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md#2026-10-13-independent0a0b0c-audit)
rechecks sourceheldpad/fastcounter latch, exactone-point/sound/levelguard
ordering, next-invocation0c transition, and icon-end retained bytes/pointers.
Validation: read-only complete used bodies; git diff --check. Docs only, no
native edits/tests/build/commit. Next native provider uses source count ticks
and soundrequests; actualaudio/raster/update/pouch-save lifecycle remains open.

## 2026-10-13 — Native original PointGet and Star Point counting

Bound actual source0a→0b→0c→0d→0e: used PointGet creation, original US
phrase/digit/shadow strips, signed packed EXP counting, held-pad fast latch,
source count-cue requests, exact wait/deletion/softdelete and five-call pointer
release. Separate host-testable effect retains entry/depth capture, source45
enter/hold/60leave ticks and original width-cap/cursor/UV/shadow geometry.
Native cosine is an explicit numeric adapter. Effect update follows battle/
event/icon work once per simulation timestamp, including creation pass.
Victory music ownership stays active through icon deletion independently of
icon visibility. GPU submission reuses the existing original UI atlas.

Validation: final integrated make test164/164 CTest +107/107 Python
(`artifacts/trio-pointget-tests-integrated.log`), make all and make debug-short
(`artifacts/trio-pointget-all.log`, `artifacts/trio-pointget-debug.log`) pass.
Earlier integrated runs caught concurrent opening-effects registration and
item-selection build-state failures; current full suite passes without changes
to those unrelated implementations/assertions. Original local asset generation
passes. Host checks cover original image dimensions, phrase/digit/shadow layout,
phase/softdelete timing, actual normal40-call and one-press/released fast10-call
payout, count-cue totals, delays and pointer release before effect disappearance.
Python capture syntax and scoped git diff --check pass. Tests/builds cover the
integrated workspace; commits exclude concurrent workstream edits.

Native evidence uses immutable binary/ELF `artifacts/trio-pointget-snapshot/`
and isolated capture `artifacts/azahar-trio-pointget/`: reviewed frame32 shows
original You got10 Star Points! art, source-scaled digits/shadow and victory
poses; final original-pointget-counted-return-boundary capture shows the effect/
icons gone and the development return notice. Actual original attacks/removal
reach this boundary; no shortcut result was injected. User emulator profile/
saves and read-only ISO remain untouched; extracted/compiled assets are ignored.
Original camera/dispatcher/GX raster and hardware performance are not verified.

Frontier is bank0700000e, before full victory export and field restoration.
Ten Star Points reach the battle's actual packed pouch; fieldresult0/story11/
control lock remain intact, and persistent pouch/save export is still pending.
Next implement ordinary0e→10→19..1f body/fade/export and actual sequence/field
wait publication, then resume unchanged scene dialogue. Source shared RNG,
equipment, camera/crowd/voice/count sound playback and alternative rewards/
level-up/fortune branches remain incomplete. Full prologue goal remains active.
