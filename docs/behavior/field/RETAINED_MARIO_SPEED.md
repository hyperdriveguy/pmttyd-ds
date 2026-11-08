# Retained original walk and dash speed

`SourceMarioSpeed` implements whole original `marioGetDashSpd80087ce4/a0` and
`marioGetWalkSpd80087d84/9c`, including whole reached
`marioBgmodeChk8005c164/10`. Source single multiplies remain typed numeric
services with their real callsites. These are source getter results, not Game
movement, force-count completion or a whole MarioMain receipt.

Both capture actual Mario via8041e900, read main flags and then initial base:
dash+188 or walk+184. Main bit00100000 takes the form branch, which reads actual
signed stick bytes+252/+253. Dash then reloads walkbase+184. The integer squared
stick magnitude at or below55*55 selects original half factor8042088c=.5;
strictly greater magnitude keeps the base. Signed-128..127 bytes are preserved,
not clamped/normalized native Circle Pad estimates.

Other forms take BgmodeChk, which independently reloads8041e900 and reads that
fresh actor's main flags, extracting bit02000000. If set, speed is multiplied by
the original half factor. Both branches then read actual captured original
actor+228 scale and perform a separate source single multiply. A child can see
a different Mario while the final scale still comes from the parent's captured
actor. No neutral input or default scale1 is fabricated.

Root binds the ForceMoveMain reached speed callback to a persistent owner with
the actual requested getter function, holding the containing source simulation
visit until the optional result is engaged. Bind canonical actual main flags,
base184/188, scale228, and signed stick252/253 only when reached. A current
source constructor's initialized bytes can supply genuine initial history;
subsequent state needs one real owner. Unsupported/unreadable fields hold before
their first source operation. No source field is written by these getters.

Function, callbacks/user and shared lifetime pointer/control-block identity are
fixed through waits. Actual source current is checked before each step, loss
poisons, recursive resume/reset rejects, completed repeated resume has no effects.
A numeric service that returns unengaged must retain its own partial work.
Original memory/service state is stationary during each single-thread callback.

Independent immediate reference tests compare all reads/multiplies with each
external boundary held twice for both getters, both branch flags and signed
stick threshold/extreme cases. Late pointer changes and scale mutations prove
captured/fresh ownership. Missing byte service does not block ordinary form;
actual form branch holds at its first byte. Strict host, ASan/UBSan, strict ARM
syntax and full-body/GetPtr/real-half-factor read-only source pins pass. Root
owns actual Game/native state and production routing; no shared file edits or
normal field activation are included.
