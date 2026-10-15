# Retained original action-command manager

`resumePackedBattleActionCommandManager` implements one whole retained local
BattleActionCommandManager (`800fa5b0/7c`) invocation. Its synchronous reference
and original evidence remain in
[BATTLE_ACTION_COMMAND_MANAGER_REFERENCE.md](BATTLE_ACTION_COMMAND_MANAGER_REFERENCE.md).
The continuation does not select or approximate an unknown action callback.

The caller's actual BattleWork argument is retained independently of later
global changes. The private-pad provider receives `B+1d1c, channel0` first,
including when Main is null. Its successful completion is never repeated while
the action or display child waits. Only after pad completion does the cursor
read current `B1ca0`; a null pointer ends this invocation without display reads.
A nonnull callback and retained B survive all waits in that exact child.
The child provider owns its own partial work and must resume that work when
called again; this parent does not make a synchronous mutating callback safe.

A completed callback result0 clears only `B1ca0` at the original source point.
Result, display and delete slots `1ca4/1ca8/1cac` are not cleared by this manager.
The display pointer is read fresh after the callback and any Main clear. Null
ends without a priority read. Otherwise the original float word at `80422d68`
is read once and the exact deferred display registration is retained across
waits: camera8/class1/callback/current captured display/borrowed B/source priority.
Main and its successful pad are not repeated if registration is unavailable.

Both actual memory and providers must remain alive through completion. Exact
BattleWork argument and all service identities, including actor-memory services
and lifecycle observer, are fixed while held; a mismatch refuses progress before
guest reads or callbacks. Recursive resume or reset during provider execution
also refuses progress. Explicit reset between calls drops only native metadata.
A completed cursor starts a new invocation when called again; the actual
simulation owner supplies cadence and physical samples. `pending()` reports
manager identity for the pad frontier, captured callback identity for Main, and
`80010bc4` for deferred display admission.

Focused host tests compare complete guest read/write and successful child traces
and final memory against the unchanged synchronous body. They cover each child
held twice; callback result0/1/-1; callback mutations of Main/display/priority
and global BattleWork; null Main/display; fresh versus retained child operands;
service/observer/argument mismatch; missing-provider prefix; and recursive
resume/reset. Production and test sources pass strict devkitARM syntax.

## Active Main integration frontier

This module removes an actual prerequisite for ordinary source Main: a late
ActionCommandManager wait can no longer shift private pad histories again.
The native caller must delegate its pad exactly once here and retain each actual
TimingA/Hammer child independently before allowing a false response. Retrying a
partially mutating synchronous Hammer implementation is still unsafe.

The existing retained Main and SequenceManager already preserve their complete
local ordering. Ordinary bank2 must still execute the real all-unit pinch scan
(`8011efd8`, mode0) before the complete Turn controller (`8011c510`). The current
Trio active-path handoff/postAction helpers implement selected parts of that
controller and do not establish its complete original parent order. Current
Trio source Main/display/camera scheduling is reached only after ending; moving
that scheduler into ordinary active ticks requires those actual child owners,
proper once-per-simulation-pass input/event cadence, and genuine camera/queue
lifetime admission. This module neither activates that path nor bypasses a
missing handler, queue, sequence bank, population, or numeric provider.

## Native Harbor tutorial admission

The Packed tutorial now owns this retained manager, its original physical sample
and admitted simulation timestamp. It resets the display queue only when a new
physical sample starts an invocation, marks that sample completed only after all
children complete, and exposes no action icons while the manager is held. Same-
time retries preserve private-pad history and the queue reset epoch. Unknown
callbacks remain unavailable even if their live callback slot subsequently changes;
they are not acknowledged or replaced by a different action.

While held, the tutorial tick skips resampling, scene-to-VM publication and VM/
event advancement. Completion runs that event pass at the original admitted
timestamp. Message presentation may still advance while held; explicit external
menu/input APIs have separate admission. The supported TimingA child remains
synchronous. This does not establish complete original Main scheduling or make
other partially mutating action children resumable.

The added `harbor_tutorial_action_continuation_tests` uses the real native Packed/
VM/pad/queue owners with explicit provisioner constants and an unavailable callback
fixture. It verifies TimingA once per sample, repeated calls, missing physical
samples, retained unknown callbacks and suppression of incomplete icons. These
are integration fixtures, not observed original console timing.
