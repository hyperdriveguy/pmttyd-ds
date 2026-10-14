# Native action registration and status-window requests

The native trio now uses the shared complete registerBattleAttackState6 for
confirmed player commands and inhibited enemy action state6. Selected command
publication remains separate. Registration follows original status/category/
zero-gravity selection, RNG and root-construction rules; suppression still
advances channel6 without creating or deleting a wait root. This replaces the
manual player wait deletion before construction and the reduced enemy path.

The native backend resolves the actor twice around the required status-window
request, creates the real ROOT through EventNativeRuntime, retains its actual
ID, then lets the shared helper store U2a8, bind E.owner from U0, and delete/
clear U290. It increments sequence channel6 through the source sequence helper.
No immediate script execution, guessed success or count consumption is added.
Existing registration fixtures check owner/ID/wait ordering, actor recapture,
status/category/gravity routes, qualifying Electric/Confuse draws and null
creation. Native trio regression checks its actual registered attack path.

The new shared status-window checker preserves blocker273c, blocker2740,
show274c and the2748/2744 conjunction in that order. It queries current state
once and requests open/close only when needed, returning the desired source Q.
Event OnOff captures the battle before resolving its argument, writes2748 for
0/1 or2744 for2, then checks the current global battle; unsupported arguments
still recompute and return2. System/Event setters write their source field
before recomputation. Complete source bodies are pinned by
 tools/verify_battle_status_window.py (six bodies, read-only original US ISO).

The harness projects original Init state5 EventOn then SystemOn requests at
entry; full startup sequencing/resources remain separate. Normal registration
uses SystemOff; post-action state9 restores SystemOn only if Bef4 lacks30.
The renderer reads the resulting native visibility state and keeps the existing
original icon/font HUD. Native open/close projection is immediate; original
status-window main/slide animation is not implemented or measured. Request
visibility is not evidence that the complete original window renderer runs.

Tests cover conflicting priority fields, all desired/current combinations,
whole-word nonzero blockers, event arguments0/1/2/unsupported, captured callback
store versus global recapture for the checker, and native attack hide/restore.
Original registration/root verifiers also pass. No Dolphin observations or
complete native battle/menu/tutorial claims follow from these host checks.

See [original action controller](BATTLE_ACTION_CONTROLLER.md),
[registration graph](NORMAL_JUMP_ROOT_REGISTRATION.md),
[status arbitration](BATTLE_STATUS_WINDOW_ARBITRATION.md), and
[native command selection](NATIVE_ATTACK_COMMAND_SELECTION.md).
Next: original tutorial menu/table/input and player action ownership, complete
action/turn scheduler, camera/effect/audio/audience consumers and original
window animation. Physical controls remain required; touch is optional.
