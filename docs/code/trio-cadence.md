# Trio retained cadence and partial source dispatch

This documents `resumeSourcePinch`, `resumeSourceAction`, `completionPass` and
`tick` in [the runtime](../../source/battle/trio_attack.cpp). See
[sequence-provider composition](trio-sequence-bindings.md),
[legacy scheduling](trio-scheduling.md) and [draw binding ownership](trio-menu-frame-bindings.md).
The original source controller and a native runtime binding have separate scopes:
a complete reusable controller does not make every runtime child available.

## Pinch and action owners

Pinch admits only the runtime's actual Battle address and retains the original
mode. Its status callback uses packed VM actors. Float-to-int admission requires
a finite value in [-2^31,2^31) and the real platform truncate callback. These are
native supported-domain checks, not full original FPSCR behavior. No sound
callback is bound in this adapter. A warning can therefore hold after its source
countdown writes; resetting native metadata cannot undo those writes. Fixed
provider identities are required throughout an invocation. See
[pinch/recovery](battle-pinch-recovery.md).

The retained action manager has a different private pad at B+`0x1d1c` from the
general pad at B+`0xf2c`. Only channel zero and TimingA Main `0x8012aae0` are
bound here. Its packed projection copies state `0x1c9c`, finish countdown
`0x1f54`, flags `0x1f50`, classification `0x1ce8` and eleven scratch words at
`0x1f20`. TimingA state 1002 owns all four callback clears; an ordinary Main
return zero lets the retained manager clear only Main before rereading Display.
Legacy Hammer execution does not make another callback available here.

Display admission creates an actual deferred queue record. It does not execute
the UI callback, prove raster/resource availability or complete GPU submission.
The sample held by the outer Main also feeds this private pad, so a later input
sample cannot leak into a suspended source invocation.

## Main and sequence completion

At fresh Main entry, `completionPass` copies the physical sample and resets its
local display acknowledgment. Held Main keeps that sample. `completionFunction`
deduplicates adjacent diagnostic notices only; it neither caches service results
nor suppresses actual source calls. Every request must name the actual Battle
allocation. A numerically aliased owner has no lifetime proof.

General input updates four `0x1fc`-byte source blocks starting at B+`0xf2c`.
Channel zero gets the retained physical sample; the other three get explicitly
inactive native samples. Channel-zero history is then retained for command use.
This is a native input adapter, not evidence of original multi-controller input.

The real retained Sequence manager (`0x8011f1ec`) binds only pinch
`0x8011efd8` and Turn `0x8011c510`. Init, FirstAct and End requests stay pending
under their actual symbols. The separate legacy `handoff`/`ending` functions are
not acknowledgments of those missing children. Stage, audience, action help,
auxiliary icons and display remain separate source requests. ActionHelp can
complete its actual no-display route, but a requested `dispEntry` stays pending.

A display prefix completed outside Main is acknowledged exactly once through
`completionDisplayReady`; `finishDisplayContinuation` asserts that the retained
Main still exists and resumes it with the same sample. Whole Main then requests
`camMain` (`0x8000f704`). Camera Draw and `marioStMain` (`0x8006f904`) are later
outer owners. These transitions do not free battle storage or restore the field.

## Native tick gates

`tick` first retries held camera draw/Main, pose depth and display-footer work.
Only then can a fresh `BattleMain` (`0x800f8e28`) sample run at a new native
millisecond. Real shadow Main (`0x8002db8c`) precedes it. The guest timestamp is
milliseconds times `eventClockRate(243000000)`; 243 MHz is an explicit nominal
clock conversion, not a measured New 2DS XL or original-CPU rate.

The pending-clear legacy block has several cadences, despite sharing one compact
line of implementation. `sampledAt` gates pad sampling, action/legacy turn work
and stage updates. Camera update and VM manager are outside that sample gate;
repeated visits at the same timestamp can still call the VM manager. HP gauge
updates have their own once-per-timestamp guard and run only while no pending
provider remains. A screen redraw is not an action-command simulation tick.

Icon, PointGet and ending-fade clocks are outside that entire pending-clear
block and use their own gates; fade uses explicit native rate 60. Map transforms
and scene projection also run after the branch. None of these independent visits
acknowledges a missing source event/Main child or invents elapsed-time catch-up.
They must not be described as a single global once-per-frame scheduler.

This focused audit leaves the large runtime partial. Newer field return/lifetime
work and any unreviewed changes since earlier snapshots still require inspection;
no complete original battle, story return, hardware timing or GPU parity follows.
