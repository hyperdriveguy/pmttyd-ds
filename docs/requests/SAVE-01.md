# SAVE-01 — Original cardMain save/load persistence and story switches (P1b)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

Native progress2026-10-13: the port now writes PM2D version4,40-byte records,
adding a scoped indoor-door mask at byte33 and retaining version1/2/3 loading.
See [native format](../behavior/format/NATIVE_SAVE_FORMAT.md). This does not close
any original card/switch persistence requests below.

- [ ] Complete original `cardMain` save/load state machine and record: save-file format/layout, checksum algorithm, corruption detection/repair, backup recovery, error/recovery states, multi-slot management and card-operation timing/coordination.
- [ ] `swSet`/`swGet` story-switch bank: bank word count, index-to-story mapping, whether the `rotlwi`5 bit computation is the true encoding or a compiler artifact, and how the bank is saved/loaded.
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
