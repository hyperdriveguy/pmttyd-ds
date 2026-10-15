# Trio source display prefix and footer

[The runtime display prefix](../../source/battle/trio_attack.cpp) composes actual
source unit/part work with native pose/queue/resource owners. See
[scheduling](trio-scheduling.md), [part/HP helpers](battle-part-presentation.md),
[draw pipeline](draw-pipeline.md), [class display](battle-class-raster.md) and
[provider ownership](trio-providers.md).

## Continuation limits

`displayActive`, unit/part indices and footer stages retain progress across
provider holds. They are not a cursor around every source operation. Alpha,
gauge, pose/effect and phase stores can precede unavailable work. A retry can
repeat operations within that unit/part unless an explicit latch or source flag
protects them. Do not promise whole-pass rollback or complete exactly-once
presentation from these indices alone.

The unit prefix rejects kind 223's unbound display service. Source alpha target
`0x311` and current `0x312` rise by 12 or fall by 2 toward the target, marking the
transition in unit flag 2. Actual HP gauge and ordered status-11/10 scale factors
run before the unit-prefix acknowledgement. Five effect domains map source
status IDs 1/6/8/9 plus faint work to handles `0x324/0x330/0x328/0x32c/0x334`.
Active work or a live old handle needs its actual creation/retirement provider;
zeroing the handle or recording an effect request is not completion.

## Part pose, effects and depth

Parts follow the real linked list. Each requires a native binding; source handle
`0xffffffff` skips its pose work. Automatic pose priority comes from the original
status table. The first exact or type-28 fallback name wins. Player status flags
select special types 57/56/45; statuses 19/20 change the rate. Actual pose handle
identity and resource methods remain required.

Part dirty bit 1 and restart bit `0x200` clear before the remaining flag word is
used for force; **any** remaining nonzero bit can force source selection. Flags
are not interchangeable with a single native restart boolean. Effect-selection
stores follow original status/transparency precedence. Byte `0x214` can publish
before `clearEffect` acknowledgement, so it is not a general retry-safe cursor.
Unsupported effect mode groups retain distinct source function tokens.

Gravity/floating phase halfwords `0x1a6/0x1a8` use periods 120/90 in source visits.
Inactive phase first takes modulo and then settles toward a boundary using the
half-period rule; active phase increments with unsigned narrowing. They are not
millisecond timers. World position is captured before actual pose Main. Queue
depth adds part `0x4f8` afterward, preserving that read order.

Source alpha narrows after each multiply/divide by 255; status 18/unit attribute
8 introduces the separate factor 128. RGB copies from source part color bytes.
Default class is translucent 2. Only alpha 255 queries actual pose translucency
to select class 1/2. Hidden/no-draw paths still run their required shadow/sound
suffix. Queue registration needs an admitted camera depth snapshot; registration
is separate from eventual callback execution and GPU publication.

## Footer ownership

| Native stage | Source work and retained boundary |
| --- | --- |
| 0 | Stock EXP mutation once, then actual icon callback registration |
| 1 | Acting-unit getter once, then two stage callbacks in order |
| 2 | Process palette and retained command-display registration |
| 3 | Audience animation initialization, captured flags and retained registration |
| 4 | Retained break-slot registration |
| 5 | Ordered report-flag clears and complete display prefix |

Stock EXP counter increments while report exceeds displayed value. Once counter
is at least 4, displayed value grows by one per eligible visit; this leaf does
not reset that counter. `stockExpMutationDone` prevents repeating that source
mutation when registration later holds. Stage callbacks use camera 4/class 2 and
source priorities 5000/6000; `stageRegistration` advances only after admission.
A held second callback must not re-register the first.

Command display needs the actual process palette; audience window query uses the
real pouch capacity. Animation-start and captured audience flags have distinct
latches. Each registration helper owns its own pending suffix. Completion clears
source report flags `0x10000/0x20000` in order. None of these registrations alone
proves actual rendering, source audience touch admission or hardware parity.

Final scene publication resolves registry IDs through the visual-slot map. Null
registry entries hide the scene actor while retaining guest bytes. Rotation adds
two source vectors; uniform scale applies only X/Y before separate axis scales.
The draw projection does not claim original teardown or change story state.
