# Native milestones: first demo and full game

This is the native port's own roadmap. It is **not** original-game evidence and
**not** a documentation-agent deliverable; recovered behavior stays in
`behavior/`, and provenance is still requested only through
[the request inbox](DOCUMENTATION_REQUESTS.md). This file fixes the *next step*
the requests are scoped to and the *end goal* they must ultimately serve.

## End goal — G1: full-game playability

A native New 2DS XL remake that reproduces the original **US G8ME01 revision 0**
game: every chapter and story scene from the Rogueport prologue to the finale,
all partners, and all field/battle/event/message/save/audio/render systems, with
original US English dialogue, character attribution, order, branches and event
conditions intact, and with original assets, music and sound effects. G1 is
reached only when systems close end-to-end, never when helpers merely exist; no
completion percentage is claimed while the coverage register in
[the completion assessment](behavior/meta/US_COMPLETION_AND_PRIORITIES.md) still
lists full-scope rows as open.

G1 is why every subsystem is being recovered. A demo that hard-codes around an
unbound system carries that debt into every later chapter, so D1 below may not
bake in demo-only shortcuts that contradict a recovered contract.

## Next step — D1: the Rogueport opening demo

One complete original playable sequence, from new-file entry through persistent
results and repeat entry, using original content only:

1. **Boot → new file.** The original opening demonstration/title flow and the
   original new-file opening message payloads (house/letter/map/voyage/prologue/
   wake/arrival) with source speakers and the voyage input gate.
2. **Arrival.** Save source story3 at the harbor and admit the field owner.
3. **Encounter.** Story3 Goombella/Crump encounter messages (016..021); story4
   original before-battle event, messages (022..024), HP minimum and `setup2`
   battle-start request.
4. **Tutorial battle.** Original tutorial init/phase and selected stage/music;
   ordinary Jump and Hammer action commands on the recovered TimingA history
   checks; defense sampling; damage/status application; turn progression; enemy
   defeat — with the original first encounter's units, parts and command IDs.
5. **Result.** The original battle-result record published by a real battle
   owner; victory determination; story advance; field return.
6. **Persistence.** The post-battle checkpoint saved; saving unblocked only by
   the real result; reload restores the post-battle state and re-entry is stable.

### D1 acceptance criteria

- No development notice or authored dialogue appears anywhere in the sequence;
  every on-screen line is the original US message payload for the correct
  speaker and order.
- The sequence is driven by the recovered event graph and the recovered
  action-command timing, not by native timers or injected outcomes.
- Original assets, music and sound effects are used; no generated tones or
  redrawn stand-ins.
- Saving is gated by the real result; a save/reload round-trip at the checkpoint
  reproduces the field state, and re-entering the harbor does not replay a
  finished scene incorrectly.
- Host tests and an isolated Azahar run cover the chain; the validation states
  what is proven and what remains static-only.

### D1 out of scope (deferred on the G1 path)

Chapters after the prologue, other partners, inventory/shop/party menus beyond
the first encounter, pipes/interiors/story-gated exits, optional content and
hardware performance. Deferring them must not introduce shortcuts that
contradict the recovered contracts.

## Request map

| Request | Serves | Note |
| --- | --- | --- |
| DEMO-01 | D1 | The integration contract the completion assessment calls missing: entry conditions, persistent writes, branch/join and re-entry. |
| RESULT-01 | D1 | Battle-result publication, victory, story advance and the save-unblock producer. |
| GSF-01 | D1 + G1 | Opening story-flag map and save-block story switches; the same system gates every later chapter. |
| TRACE-01 | D1 + G1 | Matched original execution trace — the validation oracle D1 acceptance needs, and the first of the whole-game trace corpus. |
| ITEM-01 | D1 + G1 | Field inventory/pouch and field item use (the demo's items; every later chapter's inventory). |
| PARTNER-FULL-01 | G1 | Field lifecycle and abilities of the six non-Goombella partners. |
| MENU-01 | G1 | Original field pause menu entries, navigation, pause ownership and saving (the demo only opens it once; every later session uses it). |
| PROGRESS-01 | G1 | Star points, level-up bonus selection, BP and badges, and their save persistence (the demo's first encounter has no-level-up tail). |
| FIELD-ENEMY-01 | G1 | Roaming field enemies, contact battle entry and post-battle respawn (the demo's encounter is scripted). |
| STAR-01 | G1 | Crystal Stars, star power and special moves (the prologue grants no star; every chapter 1+ battle uses it). |
| SERVICE-01 | G1 | Shops, inns and NPC item/coin services (no shop appears in the demo). |
| AC-01, AC-02, AC-03 | D1 | Jump/Hammer/defense ownership and impact cadence. |
| DMG-01, DMG-02 | D1 | Weapon power, part defense, damage dispatch/application. |
| STATUS-01 | D1 | Status lifetime and turn ownership. |
| INPUT-01 | D1 | Physical input producer and WorkInit. |
| EVT-01 | D1 | Event scheduler/dispatch closure (drives sequence steps 1–5). |
| JUMP-01 | D1 | Ordinary Jump follow-up and per-hit event flow. |
| FIELD-RECOVERY-01 | D1 | Water/void recovery, pose domains and pipe paper. |
| MAINLOOP-01 | D1 | `marioMain` field dispatch and pause cadence. |
| SAVE-01 | D1 + G1 | `cardMain`/`swSet` save/load format and story-switch storage. |
| OW-MOVE/WALL/SUPPORT/PARTNER/ROAD/INTERACT/CAMERA-01 | D1 | Field movement, collision, Goombella follow, Rogueport roads, interaction and camera. |
