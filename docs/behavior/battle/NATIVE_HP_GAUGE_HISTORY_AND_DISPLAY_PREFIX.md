# Native HP gauge history and display prefix

2026-10-14 original US G8ME01. Authority: [gauge and damage producers](SURVIVING_UNIT_HP_GAUGE_MAIN_AND_SLOT_HEAP_LIFETIME.md),
[status effects/icons](SURVIVING_UNIT_STATUS_EFFECT_ICON_AND_PART_FRONTIER.md),
[alpha/status lanes](UNIT_ALPHA_CONSTRUCTION_AND_DISPLAY_SIZE_STATUS_LANES.md),
and [forced-pose rate frontier](FORCED_PART_POSE_DISPLAY_RATE_AND_ANIMATION_FRONTIER.md).

Portable HpGaugeInit preserves ordered signed HP reads, ratio fields, source
mode reset and20/50 durations. Main preserves remaining-clock semantics,
linear/cubic single-rounded interpolation, callback-sensitive max/current
captures, ordered completion copies, independent hide timer and flags/latch
clear. MaxHP0 retains source IEEE arithmetic; no invented divisor1 or ratio0.
An unavailable interpolation provider retains earlier stores and suspends.

The custom trio constructor now initializes gauges before pouch/resource
import, supplies original255/255 alpha bytes, and restores part208/20c speed
defaults1 and4f0 colorffffffff. It retains original later writers. The existing
portable damage calculator already implements timer/latch production; native
readDamage now imports200/202/204/206,1fe and208/20c, while writeDamage exports
the applied200/204 timers. No duplicate guessed HP application or post-battle
gauge reset was added. Resource import does not reseed initial ratios.

The native adapter advances gauges once per distinct unblocked simulation
timestamp after its existing event-manager pass, independently of rendering.
This is a declared integration order for the current harness, not proof of
whole original event/engine/render cadence. Complete btlDispMain ownership
still needs binding; its ending prefix performs its own actual gauge update.

Ending btlDispMain now runs the ordinary-unit prefix: actual alpha flags/slew,
gauge Main, original size statuses11 then10, ordered inactive effect leaves
with their delete wrappers, and actual icon counter/gate. Active effects or
nonzero old handles remain their source creation/deletion providers. An
admitted icon gate remains pending even if all status bytes are0. No live
status, handle, flag or part bank is erased to satisfy this pass.

The actual first surviving part carries204bit2 from changePose. Thus it skips
_GetStatusPoseType and admits btlDispAnimeSpeed:208=1 then local-rate request
8004142c. This is the next concrete boundary; the original pose pool/handle
owner is not yet bound. Existing native original-asset animations continue
through the separate presentation adapter. Legacy raw part handle0 is not
proof of successful original pose registration, and no fake local-rate write
or animation completion is supplied. Installing sentinel-1 solely to skip
this live animation branch would conceal the missing constructor/provider.

Tests cover linear versus cubic curves, signed countdowns, asymmetric ratio
owners across provider mutations, source completion stores, independent flag
clearing, unavailable provider retention and exceptional initial ratios.
Trio regression retains actual damage/timer history and reaches8004142c with
engine result/field control still held. Original pose registration/time-rate/
SetAnim/Main, remaining units, draw consumers and teardown remain unfinished.
