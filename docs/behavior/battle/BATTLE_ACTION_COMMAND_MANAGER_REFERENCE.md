# Original action-command manager and StartAC owners

US G8ME01 disc0 revision0. Complete BattleActionCommandManager800fa5b0/7c,
StartAC80107788/64, ActionCommandStart800fa450/c and help/window setters are
read-only source evidence. This replaces the older speculative GPU/index
interpretation: these fields are callback pointers and command state, not an
inferred GPU context or command-index loop.

## Complete manager order

[`battle_action_manager.cpp`](../../../source/battle/battle_action_manager.cpp)
implements the complete local manager against retained input BattleWork B.

1. Request BattlePadWorkUpdate(B+1d1c,0), even with a null main callback.
2. Read current B1ca0. Null returns without querying/registering display.
3. Invoke that callback with retained B. Exactly result0 stores B1ca0=0.
   Do not clear result1ca4, display1ca8 or delete1cac at this site.
4. Reread current B1ca8 after callback/clear. Nonnull requests
   dispEntry(camera8,class1,callback,userB,priority15.f). Priority comes from
   original binary32 word41700000 at80422d68. Null skips the request.

The manager does not invoke the display callback directly, tick an event root,
perform callback deletion, resolve result polling or imply a rendered frame.
Main can mutate the display pointer; the fresh pointer is passed to the display
queue. The retained B is never replaced by a new global lookup in this body.
Shared bool/optional returns signal provider readiness, not original return
values. Failure can follow prior effects and is not a retry-safe continuation.
Native unknown callbacks or missing pad/display owners must remain frontiers.

## Complete StartAC and help/window setters

[`battle_action_help.cpp`](../../../source/battle/battle_action_help.cpp)
provides source key/display setters and the complete StartAC wrapper. Capture
argument pointer then current B before resolving on. Store100 to retained B1c9c.
Only nonzero on loads retained B1c90 then actor27c; bit10-clear requests
HelpSetDispType(2). The wrapper returns2. On0 omits actor/help reads.

HelpSetDispType recaptures global B, stores raw mode at1900c, then always requests
WinSetActive(mode==0 ?1:0). That leaf independently resolves the current inline
window B2858+1375c and stores only its active BYTE0. A setter-induced global
change therefore separates help and window owners. Every nonzero mode, including
negative values, passes0. This is not a crowd-count/target adjustment. SetHelp
stores only raw key19008; it does not assign mode or fabricate text.

See [help bootstrap](SPOTLIGHT_MUTABLE_COLOUR_BOOTSTRAP_AND_AC_HELP_ADMISSION.md),
[audience inline owners](AUDIENCE_MAIN_INLINE_OWNERS_AND_ORDINARY_EMPTY_DOMAINS.md),
[action callbacks](BATTLE_ACTION_COMMAND_CALLBACKS.md), and
[Main order](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md).

## Harbor motion continuation and remaining integration

Harbor binds complete StartAC and actual help/window writes, timing-option
raw-mask updates and the adjusted success-window query. Its complete existing
MarioJumpPosition backend now has platform double arithmetic/FMA/fmod and a
runtime-owned service. Original sound requests retain the actual calling unit
as native metadata, key and position. Source eligibility and trajectory remain
the shared providers; no replacement arc or generated sound is supplied.

Host tests check manager pad/main/display order, null-main display skip, fresh
display publication, zero-return clearing only main, missing providers, captured
versus current owners, help modes and StartAC token/on gates. The native original
Jump graph prepares state100/help2, executes Mario motion and stops live at
ResultAC80107708 with unchanged HP/result and one payment. This is not an action
success, complete hit, full original scheduling or visual/audio playback proof.

The shared manager is not yet invoked automatically by Harbor. Complete native
private-pad/sequence/manager cadence, TimingA callback execution and display-queue
ownership must be composed next; do not substitute another rendering-frame tick
or silently clear the remaining callbacks. ResultAC/StopAC, pose consumption,
hits and post-action scheduling remain required. Game still needs the original
class/target menu activation and physical controls, extracted HUD/menu rendering
and second-screen scene/audience interaction. Source listings and validation
logs remain in ignored artifacts; no original-runtime measurement is asserted.
