# Trio action, turn and frame scheduling

This covers the committed legacy handoff, action/retained-Main composition and
camera pipeline in [TrioAttackRuntime](../../source/battle/trio_attack.cpp).
See [events/entry](trio-events.md), [ending](trio-ending.md),
[display prefix](trio-display-prefix.md), [retained battle owners](battle-retained-owners.md)
and [camera](battle-camera.md). Public draw/material wrappers and newer concurrent
Turn/pinch/sequence bindings still need a separate complete audit.

## Action and legacy phase ownership

Source banks `B+0xf14/0xf18/0xf1c/0xf20` hold turn/phase/actor/attack state.
`postAction` visits `0x06000007..0e` and checks real event, message, audience and
slot readiness. Sequential conditions can advance several banks in one call;
one bank is not inherently one displayed frame. Actor cleanup and move accounting
can precede an unavailable reservation service. Post-action script `0x2c8` owns
running handle `0x2cc`; starting it cancels wait `0x290`. Its retirement and final
barrier precede the actor-bank increment. Native `done` marks an action/handoff
boundary, not completed story or whole-battle lifetime.

The legacy `handoff` combines several source function domains. Its `emptyTurnPass`
admits only branches with no active effect/recovery work; positive HP, status 27,
state-specific status/badge and timer domains still gate admission. The original
23-status recovery order remains literal. A real effect stays pending under its
function token rather than becoming guessed healing or damage. Do not use this
legacy composition as an acknowledgement of a complete source Turn callback.

Legacy phase work starts installed `0x294` roots, then consumes the ordered phase
queue at `B+0x220`, stride 8, one admitted script at a time. Root retirement and
conclusion precede further actors. Audience phase state has separate flags,
200-record population, category queue and break-slot turn ownership. Its native
admitted category subset is 4/9/10/11/12/13. Nonempty actor/effect domains are not
silently emptied. Turn-end chance and category writes may precede unavailable
target/crowd work, so this immediate parent is not generally restart-safe.

Actor preparation resets the supported accounting/sequence/report work before
calling the source phase selector. Explicit launch copies candidates, class and
weapon/cursor state, then requires actual attack selection before registration.
Goombella class 6 differs from Mario's Jump/Hammer row indices. Source status can
suppress root creation after successful registration; no replacement is invented.
Broken-unit retirement cancels actual events, repairs tables/drop storage and
publishes a null registry pointer last. Guest bytes survive for diagnostics;
shared pose/file lifetime is owned separately, not proven freed by this prefix.

## Pads and action commands

A source pad block is `0x1fc` bytes: two 12-byte snapshots, four 30-word histories
(trigger/repeat/held/released at `0x18/0x90/0x108/0x180`), then repeat state at
`0x1f8/0x1f9`. BE buttons, signed sticks and analog bytes remain separate fields.
General and private histories are distinct. Native channel 0 has real input;
other source channels explicitly receive inactive native samples.

TimingA and Hammer share packed work under different active callback tokens.
Their typed projection helpers do not themselves tick. Hammer includes charge,
period, suppression, result/classification, 11 scratch words and 12 sound handles.
A missing Hammer branch can leave its typed tick unavailable before writeback.
The native sound sink records original keys and returns failed handle -1; it does
not claim MusyX playback. Cleanup of all four slots under `mainLive=false` differs
from the outer manager's Main-only clear on zero.

## Retained Main and native tick

`completionPass` pins one physical sample across an entire retained Main
invocation. Each child must use the actual runtime Battle owner. General pads,
sequence, private action, stage, auxiliary icons, help, audience, reel and display
are separate services. Sixteen auxiliary icon records must actually be inactive
when their complete animation provider is unavailable. Never clear work simply
to satisfy admission. A display child completed outside its parent acknowledges
exactly once through `completionDisplayReady`; the parent still performs final
checks and waits on next outer provider `0x8000f704`.

`tick` first resumes held camera/depth/footer work, then admits new sampled work.
Repeated screen drawing does not run another action-command tick. Legacy
trigger-only input is an explicit fallback when no physical sample is supplied.
The VM manager is outside the sample-update sub-block when pending is clear;
the sample guard does not prove all event work is once per millisecond. Pending
source Main/event work and icon/PointGet/fade clocks have distinct gates. Clock
updates use their stated native visit policy, not elapsed-time catch-up or an
assertion of original scheduler/hardware parity. Maps and scene projections run
after these branches; renderer publication does not advance story.

## Camera publication and outer draw

Camera-frame update attempts once per native millisecond and caches availability.
A failure marks source history incomplete; an explicit numerical depth snapshot
does not recover it. Rumble-off is a New 2DS XL hardware no-op because no motor
exists. Camera Main uses retained actual camera lens/history; mode 2 is not gated
on a manual depth-test lens adapter. Source numerical aspect, orthographic angle
and 608×480 constants remain explicit adapters, not measured Gekko parity.

Whole camera Main success increments a native publication generation. Overflow
poisons this owner instead of wrapping into an old valid HP epoch. Camera Draw
is separate: actual backend requests, near/far, queue dispatch and letterbox can
hold. New icon-pass identity begins only on fresh draw invocation. Letterbox
queries actual party slot 0, sound flags, bero/sequence and an explicit native
rate; missing Key39/Ctrl38 producers remain unavailable.

Only whole Draw completion marks the icon pass complete, clears source map
allocation counter `0x8041e810` and requests outer Main `0x8006f904`. Diagnostics
follow the deepest held callback/cursor. Recording its function address does not
execute that callback, complete GPU work, free battle state or restore the field.
