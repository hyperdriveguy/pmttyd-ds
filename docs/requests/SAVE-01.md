# SAVE-01 — Original cardMain save/load persistence and story switches (P1b)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

Native progress2026-10-13: the port now writes PM2D version4,40-byte records,
adding a scoped indoor-door mask at byte33 and retaining version1/2/3 loading.
See [native format](../behavior/format/NATIVE_SAVE_FORMAT.md). This does not close
any original card/switch persistence requests below.

- [x] Complete local cardWrite record assembly/additive checksum and
  write-side switch-bank inclusion — [record contract](../behavior/format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md).
  All187 words/six copied blocks/five bank extents pinned; read/load validation,
  field schemas and persistence lifetime remain open.
- [x] Complete local cardMain dispatcher/format/check-delete phases and
  eight completion callbacks — [dispatcher contract](../behavior/memory/CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md).
  Nine operation entries, fresh polls/publications and iteration caps pinned;
  delegated read/write/header/create services remain separate.
- [x] Complete local read_main header/two-copy validation, signed timestamp
  selection and resident-buffer loading — [single-slot reader](../behavior/memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md).
  Equal valid timestamps select secondary; header failure latches0400 without
  abort; both invalid mark resident record2. Live namespace restore remains open.
- [ ] Complete composed original save/load service and record: save-file format/layout, checksum algorithm, corruption detection/repair, backup recovery, error/recovery states, multi-slot management and card-operation timing/coordination.
- [x] Complete local swdrv access/init/reset and bank word counts; true
  rotate-by-five arithmetic — [corrected switch storage](../behavior/memory/SW_SET.md).
  Global256-word8192-bit and local16-word512-bit banks are proven; global byte0
  is a32-bit word, not a byte.
- [x] Concrete Monosiri/Tattle persistent switch4474+kind and group-aware
  battle-work binding — [flag owners](../behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md).
- [ ] Full index-to-story/namespace writer mapping and exact card/save/load
  inclusion/lifetime of these banks; no original round-trip claim.
- [ ] Persistent namespaces that must round-trip (party/inventory/status records noted open in `UNIT_STATUS_CLEAR_AND_POUCH_IMPORT`), so the native PM2D format can be replaced by original persistence semantics.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-12),
P1b save/persistence:source/save.cpp implements a fully native format(magic
PM2D,version3,40-byte record,FNV-1a checksum,room/x/z/hp/mushrooms/
crumpDefeated/sound/elevation and one opaque originalStory int)with its own
.tmp/.bak recovery;Progress.originalStory therefore cannot mirror original
persistence. CARD_MAIN.md and SW_SET.md are early sketches with explicit
remaining-gap lists and neither is in the inbox. Please recover the complete
original cardMain save/load state machine and record:save-file format/layout,
checksum algorithm,corruption detection/repair,backup recovery,error/recovery
states,multi-slot management and card-operation timing/coordination;plus the
swSet/swGet story-switch bank(bank word count,the index-to-story mapping,
whether the rotlwi5 bit computation is the true encoding or a compiler artifact,
and how the bank is saved/loaded). Request the persistent namespaces that must
round-trip(party/inventory/status records already noted as open in
UNIT_STATUS_CLEAR_AND_POUCH_IMPORT rows),so the native save can be replaced by
original persistence semantics. No lock reserved;no original save trace
claimed.

### 2026-10-13 — Save-block activation/lifecycle prerequisite

[Source head-contact and hit lifecycle](../behavior/field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
delivers original dynamic hit-box geometry, new-head-contact flag8 admission,
nonloop A_1 query/publication order and optional/system-event completion gates.
This closes a local activation dependency only. Original common save child,
cardMain format/error/timing and persistent switch namespaces remain OPEN.
Read-only DOL/model/table inspections; no native or original-runtime claim.

### 2026-10-13 — Switch bank extents, encoding and concrete Tattle namespace

[Corrected swdrv contract](../behavior/memory/SW_SET.md) answers bank counts and
rotlwi5: the preceding shift27 plus sign adjustment produces ordinary signed
remainder, so nonnegative bit index is index%32, not multiplied by5. All12
access/init/reset bodies are recovered. swInit clears global/local flag/byte
banks and full global byte0 word; swReInit clears only local banks. Global
byte0 getter/setter are full32-bit, while other global/local values use bytes.
[Tattle owners](../behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
provide persistent4474+kind and separate retained-BattleWork bitmap,48 original
groups/247 member occurrences and the policy/original-kind checker branches.

Validation:14 bodies, static pointer table and48 terminated list spans total63
DOL fingerprints with205 instruction anchors; source verifier passes. Signed
arithmetic recheck8951 cases covers all8192 global/512 local bits, Tattle range
and boundary values. Inspection ELF/listings stay `/tmp`; no native code,
original save trace or asset export. Complete cardMain/on-disk namespaces,
checksum/errors/backup/timing and bank/story writer/runtime mapping remain OPEN.

### 2026-10-13 — Common save UI/card request dependency

[Common save event/choices](../behavior/field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md)
records the complete local197-command UI/card request graph and its blocking
write/header/code leaves. Source saveblock normalmode0 supports original
Yes/No/B-cancel without a write on refusal, with GSF233/236 already updated;
success is gated by completed source code0. A native PM2D backend must remain
an explicit adapter. Original card record/slots/format/error mapping and global
persistent save semantics remain OPEN. Static read-only evidence only.

## 2026-10-13 — Complete write-side record and checksum preparation

Delivered [cardWrite](../behavior/format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md):
raw slot addressing, busy/state publication, live Mario/partner/time/counter/GP
changes, six exact copy blocks, version trailer and additive byte checksum.
First8800 bytes feed a sum/complement outside the covered range. GP copy includes
all five proven switch extents, including local banks and full-word special
byte0. No FNV/SDK-directory checksum substituted or original round-trip claimed.

Validation: read-only ISO manifest11 spans/187 anchors/16 calls; independent
all187 words/ten literal vectors/six copies/five switch extents/checksum loop/
masks/time/call sites/fresh pins passed. Link/scoped diff checks passed. Docs
only; no make test/all rerun; raw artifacts stay /tmp. SAVE-01 stays OPEN:
cardMain/async/read/version/corruption/backup/load and namespace/lifetime/native
binding need recovery and original execution; this child does not perform IO.

## 2026-10-13 — Complete card dispatcher and completion callbacks

Delivered [cardMain/eight callbacks](../behavior/memory/CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md).
Replaced inaccurate CARD_MAIN sketch with corrected size/source summary/link.
Optional probe/change/countdown, nine-entry operation dispatch, format/check/
delete substates, exact fresh result polls, bounded synchronous retries and
callback clear/store order are pinned. Busy1 differs from operation2; callbacks
clear1. No callback admission or physical timing assumed from caller bytes.

Validation: read-only ISO manifest24 spans/480 anchors/23 calls; independent
all471 words/nine table entries/19 literal vectors/eight callbacks/cap/probe/
result/substate/call sites/fresh pins passed. Links/scoped diff passed. Docs
only; no make test/all rerun; raw artifacts stay /tmp. SAVE-01 remains OPEN:
async children/SDK/error/read/backup/namespace/lifetime/original traces and
native round-trip binding not closed by complete dispatcher coverage.

## 2026-10-13 — Complete single-slot reader validation and selection

Delivered [read_main](../behavior/memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md):
15 phases,7750-byte header checksum, two8800-byte record length/version/sum/
inverse validations, signed64 saved timestamp comparison/tie-to-secondary,
resident read or invalid flag, cleanup/finish and fresh result polling.
Header failure alone does not abort; raw timestamps are copied even after
record validation failure. There is no live namespace memcpy restoration in
this body. Accepted scratch data is re-read into resident storage without a
second local validation, so original provider/IO mutation domains remain open.

Validation: manifest13 spans/565 anchors/17 calls; independent all550 words/
15 table entries/13 literal vectors/header+record sums/signed selection/call/
flag/stamp sites/fresh ISO pins passed; link/scoped diff passed. Docs only;
no make test/all rerun; raw artifacts stay /tmp. SAVE-01 remains OPEN: live
restore/read-all/backup repair/async write/SDK and original round-trip binding.
