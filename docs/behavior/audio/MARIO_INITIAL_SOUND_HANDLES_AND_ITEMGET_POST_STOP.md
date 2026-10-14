# Original Mario initial sound handles and item-get post stop

US G8ME01 revision0; read-only DOL reinspection2026-10-14. Bounded fresh
native item95 post-provider provenance for [ITEM-01](../../requests/ITEM-01.md).
Complements [motion15/post table](../field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md),
[idle cue/handle ownership](../overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
and [complete SFX stop](BATTLE_POSITIONAL_SFX_OFF.md). Addresses, offsets and
masks hexadecimal unless marked decimal. This is static source evidence,
not observed playback or native backend parity.

## Fresh empty-handle admission

marioInit8005b5c8/488 allocates760(decimal;0x2f8) bytes through800300f0 at
8005b5f0 and stores the Mario global pointer. It calls memset800050b4 with
value0,length2f8 at8005b600. Then reloads Mario and uses li−1 at8005b608,
explicitly storing−1 in this order:

| Address | Mario word offset |
| --- | --- |
| 8005b60c | 27c |
| 8005b610 | 280 |
| 8005b614 | 284 |
| 8005b618 | 288 |
| 8005b61c | 28c |

Thus three ordinary idle/fall post handles begin−1, rather than zero from
memset. The remainder of this complete Init body does not write27c/280/284;
child services and later cue producers have their separate ownership.
A finite native fresh Mario with no subsequent voice producer may initialize
these handles−1 without inventing cues. It must retain actual later writes;
room entry/pose selection alone does not prove an imported active handle is
empty. Cue IDs and NDSP channel IDs are not interchangeable with these words.

marioReInit8005b1d8 likewise calls memset on its2f8-byte work at8005b2b0,
reloads Mario, loads−1 at8005b2b8,then stores27c/280/284/288/28c at
8005b2c0/c4/c8/cc/d0. This is explicit reset provenance, not a post callback.
The reset instructions do not call psndSFXOff for these overwritten handles;
other owner cleanup must not be inferred from this assignment block.

## Post order and pointer lifetime

For newly installed motion15, mot_stay_post800a6b10/ac takes the ordinary
branch (new current19/20 are the separate retain280 exception).
At800a6b38 it obtains Mario; retains that pointer while reading word27c.
If27c!=−1, call psndSFXOff at800a6b50,then store27c=−1 at800a6b58.
Next obtain Mario **again** at800a6b5c,retain this pointer,read280 and,if!=−1,
call Off at800a6b74 then store280=−1 at800a6b7c. Sentinel−1 skips both
call and store. The second stage does not freeze the first Mario pointer
across a stateful stop provider. Same-pointer native adapters can still
preserve this source call order without claiming the service is immutable.

mot_fall_post8009aed4/48 obtains Mario once at8009aee4, retains that pointer,
reads284 and,if!=−1, calls Off at8009aefc then stores284=−1 at8009af04.
The motion table installs this post for10;11 shares its main but has no post.
Posts see the alreadyinstalled new motion and do not restore an old animation,
reset movement or provide a sound cue request.

## Original handle-stop admission

psndSFXOff800d95f8/c8 checks raw input−1,then resolves
P=803df988+40(decimal)*(handle&ff). It does **not** validate generation or
compare allocation halfword+4 with the handle's upper bits. The source Chk
service owns a different comparison; do not import it into Off.

If P.word0==−1, return unchanged. Otherwise P.half6 flag1 means queued:
clear P.word0 then P.word18 to−1 with no backend stop. In the started path,
P.word18 supplies actual backend handle;−1 skips backend call but still
clears the two words. Descriptor high bits choose SoundSSStopCh first,
SoundSongStopCh second,otherwise SoundEfxStop. Call precedes ordered slot
identity/backend-handle clears. Full branch/table semantics are linked above.

For the bounded fresh−1 idle/fall fields, no stop provider is called. A native
provider for a nonempty source handle needs retained slot/backend ownership,
including queued versus started state. Strengthening generation validation,
using cue-name stops or clearing Mario's field before invoking Off changes
this original contract and must be identified as an adapter behavior.

## Verification and remaining boundary

Fresh named extraction covered marioInit,marioReInit,mot_stay_post and
psndSFXOff; fall post was independently extracted in the preceding task.
Ignored artifacts are under artifacts/reference/goomba3-services/voice_*.
Complete source bodies and explicit store/call anchors were inspected; no
original execution or native physical audio timing was measured.
Original later idle/fall voice producers, full source sound-slot admission,
backend playback and lifetime import remain separate dependencies. A fresh
empty-handle implementation does not establish those providers or authorize
new idle cue playback.
