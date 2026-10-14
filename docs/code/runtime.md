# Runtime ownership and timing

[main.cpp](../../source/runtime/main.cpp) owns libctru/Citro3D services and
resource/file operations. `Game::tick` advances simulation; it does not own
wall-clock catch-up. [game.hpp](../../include/runtime/game.hpp) declares the
60 Hz `TickRate`. These scheduling choices belong to the native port, rather
than measurements of original execution.

HID is polled once per outer iteration. `merge` retains the newest analog and
held values, ORs button edges, and retains an observed touch until a simulation
tick consumes it. The first catch-up tick consumes edges; later ticks keep held
values. At most five ticks run per iteration. A wall-clock delta above 250 ms
resets accumulated time and pending input and admits one tick. Excess backlog
is dropped and counted. Loading/restoring rooms also resets the accumulator,
so IO cannot fast-forward an action command. Notice durations are UI policy:
180/240 ticks mean three/four seconds. Frontend story clocks convert ticks to
integer milliseconds using `ticks * 1000 / 60`.

Circle Pad mapping uses an 18-unit dead zone and divisor156, then adapts to the
original +/-70 PAD range. These two coordinate spaces are different. The
frontend slot hitbox and four 37-pixel rows are native bottom-screen layout.
Story controls use original PAD masks after native key mapping.

[PhysicalInputProducer](../../source/runtime/physical_input.cpp) models four
source ports and their repeat/edge history. The
[physical-input contract](../behavior/input/PHYSICAL_INPUT_PRODUCER.md) supplies
its original masks and errors. A transfer error (-3) clears newly computed
edges/deltas but retains the previous raw sample/direction. Repeat can still
fire from held history. Initial/subsequent repeat delays are24/6 at60 Hz,
scaled by the caller's rate. X+Y+Start suppresses published gameplay channels
without clearing the retained DemoPad sample. Input history survives frontend
handoff and checkpoint restore so a held dialogue button does not become a
fresh jump press. Battle also owns private history; global publication alone
does not implement all battle polling.

## Room and checkpoint transactions

Collision is temporarily staged by `CollisionTransition`. A separate candidate
`Game` checks floor and wall clearance against the destination. Renderer room
resources must then load successfully before collision and game state commit.
The transaction destructor restores old collision on rejection. The same
pattern covers checkpoint loads and pipe/road transitions. GPU staging belongs
to the renderer; simulation requests identify a destination and wait for the
outer runtime to accept it.

The save-block `SaveWorker` deep-copies the original pouch, runs one write on a
joinable libctru thread, and returns a presentation ticket plus success. Its
16 KiB stack, priority0x31 and default core affinity(-2) are platform settings.
The atomic release/acquire flag publishes the worker's result. Ordinary writes
wait while that worker owns the path; shutdown joins any accepted write before
services close. Deep copying is necessary because `Progress` normally shares
pouch storage. [Checkpoint layout](save-format.md) explains the on-disk bytes.

## Audio and frontend boundaries

[audio.cpp](../../source/runtime/audio.cpp) streams decoded original PCM16,
with three music slots, each with three4096-frame buffers sized for stereo.
NDSP channels1/0/18 belong to these slots; channels2..9 hold eight primary-sample
cues. Channel19 is the opening-map sound. A sample frame contains one sample
per channel; PCM16 uses two bytes per sample. The constrained WAV header is44
bytes; `.loop` holds two32-bit frame indices, start inclusive/end exclusive.
These channel allocations and buffer sizes are native service policy.

The NDSP callback only signals a separate refill worker; it must not block on
file IO or the music lock. Fixed buffer addresses survive overlapping music
owners because NDSP borrows their wave descriptors. Music is stopped before
buffers are freed; shutdown disables callbacks, wakes/joins the worker and
then closes the backend. Harness gains and direct sample playback are adapters,
not the complete original MusyX macro/spatial mixer.

[frontend.cpp](../../source/runtime/frontend.cpp) retains four native save slots.
Slot0 is `progress.bin`; later slots are `progress2.bin` through `progress4.bin`.
Wrapping to the previous slot adds3 modulo4. An invalid file cannot silently
start New Game; the demo/opening owners publish their own completion rather
than advancing field simulation behind the frontend.
