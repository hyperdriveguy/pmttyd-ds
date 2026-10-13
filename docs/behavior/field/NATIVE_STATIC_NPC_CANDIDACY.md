# Native stationary NPC candidacy and simulation presentation

Scope: the original US Rogueport east Chu3 and Darkly records in the current
story0..55 native coverage envelope. Source evidence is the
[ordinary candidate and interrupt contract](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md).
This binding does not establish later quests, all actor lifetimes, original
scheduler cadence, matched original rendering or New 2DS hardware performance.

`updateNpcTalkTurn` represents the visited local candidate result separately
from nearest selection. First admission saves the current yaw, sets state
20000000, stops the regular event through the supplied service and turns toward
Mario unless mainflag01000000. Repeated admission retains that first yaw.
Local loss clears the state. With mainflag00400000 it restores saved yaw and
starts the retained regular event; otherwise it posts secondaryflag10000000
and retains yaw. Skipped global/actor gates do not invoke local loss. Other
state and secondary bits are preserved. Native wandering callbacks adapt
StopID/StartID to retained work pause/resume; the complete EVT service remains
unbound. Current callbacks only change the pause Boolean; instance fields are
published from the helper result after callbacks return.

Chu3 and Darkly have proven null original regular and all interrupt scripts.
The native registry supplies no event callbacks for these profiles. Their
next flags1/2-admitted NPC collision/update pass calls
`consumeNpcNullInterrupts`, preserving only secondary00000f00 and clearing
state00001000. This occurs before the new candidate scan, so a local-loss
request remains observable until the next admitted pass. There is no yaw
restore or event/work reset. The current native consumer is explicitly guarded
by room2 and source-area/name profile IDs525f654d/e4c10e3d (Chu3/Darkly),
not by reusable character models. Future profiles must retain and bind their
own original script metadata rather than inheriting
this null-script assumption. General interrupt priority/event replacement is
still unbound.

Ordinary stationary NPCs now own their yaw, message-driven pose clock and
paper presentation in the simulation instance, just as the wandering actor
already does. The original local Main/Inner preparation runs once per native
simulation update; rendering reads its prepared matrix and pose time without
advancing retained facing. Original animations and room model staging/texture
batches still use the existing citro3d/tex3ds path. Scripted Frankly retains his
separate event presentation owner. Native60Hz clock and host/ARM numeric
services remain adapters, not measured original-runtime cadence.

Talk event start retains the original no-turn mainflag and publishes the
source turn toward Mario into the instance. Message presentation uses the
unchanged original IDs/control tokens, attribution and per-actor branch
providers. No new dialogue, quest fallback or story writes are added here.
Source talk-return/event lifetime beyond existing bounded playback is still
partial.

Validation is recorded in [the session handoff](../../PROGRESS.md). Portable
regressions distinguish repeated admission, wandering restore/start, static
loss/request/next-pass consumption and no-turn actors; original-data native
tests exercise Darkly candidacy, skipped gates and retained yaw. Azahar checks
use isolated development fixtures and never the user's profile or saves.
