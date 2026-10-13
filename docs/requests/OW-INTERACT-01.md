# OW-INTERACT-01 — Mario A/X/B interaction arbitration and playback

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Mario A/X/B interaction arbitration and priority against jump/movement,
  actor/door/object proximity, hammer/first-strike, selected message/speaker/
  control-token playback and persistent scene conditions. Native development
  notices/menu and Crump proximity are harness behavior.

## Request text (verbatim)

- OW-INTERACT-01: Mario A/X/B interaction arbitration and priority against
  jump/movement, actor/door/object proximity, hammer/first-strike, selected
  message/speaker/control-token playback and persistent scene conditions.
  Native development notices/menu and Crump proximity are harness behavior.

## Answers and delivery notes (append-only history)

No answer has arrived for this request yet.


### 2026-10-12 — First harbor encounter predicates and payload census

Partial answer from the implementation session's explicitly delegated
documentation worker. Independent read-only US ISO recovery supplements
[harbor scene bindings](../behavior/story/STORY_HARBOR_OPENING_BINDINGS.md)
with the exact position polls: story3's `christine_osoware` admits integer
decoded Z<=-100; story4's beforebattle admits integer decoded Z<=-100 AND
X>=-25. Both poll with WAIT_FRAME(1), then disable Mario key before dialogue.
The predicates resolve 1/1024-quantized float locals through integer
`evtGetValue` truncation; comparing raw floats changes boundary behavior.
Neither poll requests A/X/B or actor proximity. Resume positions and actor
placement requests are recorded separately from fresh entry.

The story operand is now composed with the recovered namespace service:
`0xf5de0180` = GSW(0); first admission writes GSF(1179)=1. All 20 original
message calls `mac_0_014`..033 and attribution remain verified. No `<select>`
token exists in those payloads and no `evt_msg_select`/`evt_msg_continue`
call exists in the five scene streams. Story3's 016..021 and story4's 022..024
execute unconditionally after their polls. Original wait/page/key/dynamic/
scale/shake tokens must remain in local message assets and playback.

Validation: `rel_event_inventory.py` re-extracted five scene spans; direct
ISO checks pinned 13 command records, two get_pos imports and raw operands,
20 ordered IDs, two resource hashes and no-select census;
`python3 tools/verify_behavior_contracts.py` passes. No native code changed.
OW-INTERACT-01 stays OPEN: full interaction arbitration, control/key helpers,
general proximity/hammer behavior, postbattle wait predicates, actual playback
timing and original execution remain unclosed. No delivery-log owner/lock is
invented: its protocol requires a user-assigned numeric agent ID, which this
delegated worker has not been assigned.

Native implementation breadcrumb (2026-10-12): complete US message library
and `MessageAssets` resource lookup shipped in `0a94b04`. The original
`evt_mario_get_pos` callback and the story3/story4 integer position predicates
are now portable implementations in `field_story_position.cpp`, using existing
event operand services and the callback registry. They retain encoded float
quantization and original pointer/operand capture order, independently checked
against a fresh US DOL disassembly. This does not activate story scenes in
`Game`; parser/window/camera/NPC services and REL event execution remain open.
