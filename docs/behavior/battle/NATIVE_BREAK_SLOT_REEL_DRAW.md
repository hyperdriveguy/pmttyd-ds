# Native original break-slot reel display

Original US callback `BattleBreakSlotDispReel` (`802036d4`, 0x394 bytes) is
implemented by the retained `BattleBreakSlotDrawCursor`. Read-only ISO recovery
is retained privately in `artifacts/break-slot-display-evidence/`; body SHA256
`14bf5dae73a27777b8e2310f1b1fee061b3935a020022a15a628a8c9e8da3b64`.

The three ReelGetPtr(0) calls independently read current global battle pointer
`8041eb90`; each captures base B+16184 and walks three records of stride5c.
First-pass flags precede signed-state1..4 admission, translation and late color
read. The original background uses icon476 through the existing matrix/color
icon wrapper. Second-pass controller state selects original small/big texture
tables and initial scissor. State11 omits that initial scissor. State0 alone
is not an empty callback: third-pass active flags still admit reel work.

Active records retain original GX blend/depth/alpha requests and the13-operation
translation/scale/rotation/pivot matrix chain. Radian constants precede fresh
angle reads; positive and negative pivot loads remain separate. Primary and
optional next-symbol calls retain late category, color, alpha and state reads.
Original big/small tables are battle_common.tpl indices70..74 and65..69, not
icon.bin identifiers. Missing real `btlDispTexPlane2` (`8010323c`) is a pending
service; no successful placeholder quad or invented texture is emitted.

The final scissor always uses current GP U16+170 then U16+172, independent of
the earlier scissor. Those bytes share a mutable CameraManager-owned backing
with every bound EventGpAddresses facade. Native boot admits original NTSC
render-descriptor dimensions608x480 once. Repeated camera initialization does
not reset subsequent mutations; checkpoint transfer copies the retained owner.
This is an explicit native boot adapter, not execution of full marioStInit.
GP byte/half/word aliases and GP retirement snapshots see the same backing.

Source constants add40 bytes at80426884 and24 bytes at80419f30 to the private
source-icons pack. The existing124-byte prefix and all613 image cells remain
unchanged. Source paths and local-use provenance are in [ASSETS](../../ASSETS.md).

Host fixtures check inactive reads/scissors, held scissor arguments, fresh
battle owners, active matrix order, independently reread pivots/state/alpha,
retained Plane2 requests and absent-provider waits. Original queued native
execution and whole-camera icon publication require separate runtime evidence.
Full active Plane2 rendering, ordered battle teardown, field restoration and
prologue event/story parity remain open; see [EVT-01](../../requests/EVT-01.md).

Native ordinary ending now completes phase39/index3/pending0 with all three
live reel flags0/state0. Both native scissors acknowledge; final rectangle is
0,0,608,480. Whole camera draw completes and reaches outer Main8006f904.
This does not implement teardown. The original [outer loop order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
requires sequence Main before lowercase battleMain and subsequent event/world
services; source bank4 observation and sequence exit must retain that order.
