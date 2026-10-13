# SAVE-01 — Original cardMain save/load persistence and story switches (P1b)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

Native progress2026-10-13: the port now writes PM2D version4,40-byte records,
adding a scoped indoor-door mask at byte33 and retaining version1/2/3 loading.
See [native format](../behavior/format/NATIVE_SAVE_FORMAT.md). This does not close
any original card/switch persistence requests below.

- [ ] Complete original `cardMain` save/load state machine and record: save-file format/layout, checksum algorithm, corruption detection/repair, backup recovery, error/recovery states, multi-slot management and card-operation timing/coordination.
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
