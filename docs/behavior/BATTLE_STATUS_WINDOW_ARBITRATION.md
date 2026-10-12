# Battle status-window arbitration and lecture suppression

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B: complete
BattleStatusWindowCheck0x800f7514,size0xb8;btlevtcmd_StatusWindowOnOff0x80103e34,
size0x7c;BattleStatusWindowEventOn0x800f7650,size0x2c and EventOff0x800f75f8,
size0x2c. The underlying statusWinCheck/Open/Close services and rendering/animation
remain dependencies. No original image,frame timing or Switch behavior claimed.

## Desired-state priority

BattleStatusWindowCheck loads shared W and computes desired show result Q in
this exact order,using whole-word nonzero tests:

| Priority | Condition | Desired Q |
| --- | --- | --- |
| 1 | W+0x273c != 0 | 0;skip remaining tests |
| 2 | W+0x2740 != 0 | 0;skip remaining tests |
| 3 | W+0x274c != 0 | 1;skip remaining tests |
| 4 | W+0x2748 != 0 and W+0x2744 != 0 | 1 |
| Otherwise | Neither show route applies | 0 |

These fields are independent. Do not replace them with one visibility boolean
or let a lower-priority show request override either blocker. The body does not
mutate these five words or test physical input,actor HP,event type or battle
sequence directly. Positive/negative nonzero words both meet its tests.

If Q is1,call statusWinCheck0x8013c1f0. A zero return requests statusWinOpen
0x8013c3b8;nonzero makes no open/close request. If Q is0,call the same checker;
a nonzero return requests statusWinClose0x8013c390,otherwise no request. Return Q
in r3,not a fresh query of the resulting visible/animated state. No wait/poll or
repeated open/close request occurs within this body. Inner service lifetime,
actual state bit semantics,rendered layout and transitions remain open.

## Event callback writes versus recomputation

StatusWindowOnOff captures W before resolving its argument with evtGetValue.
Its original signed integer dispatch is:

| Resolved argument | Store before checking |
| --- | --- |
| 0 | W+0x2748=0 |
| 1 | W+0x2748=1 |
| 2 | W+0x2744=1 |
| All others | No local field store |

Every path then calls BattleStatusWindowCheck and returns **callback2**,ignoring
that checker's Q as an event result. It does not implement argument2 by setting
2748,clear2744 on argument0,or treat arbitrary nonzero arguments as1. Because
checker reloads shared W while the callback stores through its earlier capture,
stable W ownership is a caller invariant rather than a local validation.

EventOn and EventOff each load shared W,store1 or0 at+0x2748,then call the checker.
They add no physical/event delay or separate field reset. Other system/AP-recovery
setters and all field producers remain separate contracts;their names alone must
not determine precedence or numeric field meaning.

## Concrete lecture writer and misleading restore requests

[Lecture mode setup](CHRISTINE_HELPER_ROLES.md) writes W+0x2740=1 before spawning
its professor. This is a proven priority2 blocker in the checker. A later
StatusWindowOnOff(1) sets2748 but cannot override2740 while it remains nonzero.
Likewise274c's show request remains lower priority. Thus the coordinator script's
hide/restore-like requests do not themselves prove that a status bar becomes
visible during the lecture. Interpret them as requests through this arbitration.

The [lecture end stream](LECTURE_PLAYER_ATTACK_FLOW.md) has no clear of2740.
The previously recovered BattleInit clears the allocated whole work block,which
covers this field;later battle-end/free/next-battle initialization ownership and
other writers must still be closed. Do not clear2740 early just to make a copied
StatusWindowOnOff(1) appear to work,or import this lecture writer into Crump's
separate NPC init. W+0x2740 was not read in the checked default weapon substitution
regions;selection uses battle flags09/0a plus professor ID/data instead.

## Derived acceptance obligations

Test each priority with conflicting fields:273c/2740 override274c and the2748+
2744 conjunction;274c alone can request show when2748 is0;2748 alone cannot show
without2744 when274c is0. Test four Q/current-state combinations to ensure only
needed open/close services are requested. Verify callback argument2 modifies2744
only,unsupported values still recompute,and callback return remains2 independently
of Q. Lecture suppression must survive a lower-priority event-on request until
an original matching writer/lifetime change removes it.

These are controlled local-contract checks,not observed UI behavior. Original
comparison must log all five words,Q,inner status state and open/close requests
at the same scene/tick,including the lecture's setup/message/end paths. Exact
presentation/assets/audio and full status window service implementation remain
required for a strongly equivalent port.
