# Native plaza Goomba3 and Trouble Center working services

The existing per-area catalog now includes original Goomba3 in room0 using
[recovered setup](GOOMBA3_SOURCE_SETUP_AND_TALK.md): tagged position(-400,0,-325),
yaw90, original c_kuribo_n/S_1 and tribe dimensions, Entry/setup flags80603.
The native story0..55 envelope is a coverage boundary, not original visibility.
Shared model/texture/linear vertex batching handles this actor without a new
renderer branch. Tagged registry order remains the candidacy tie-breaker.

Known quest1420!=2 permits the original source-order ordinary story switch.
Known quest2,5406false,5405true permits unchanged irai_02_01, speakerme. Unknown
quest/required flags and the acquisition/reward arms stay unavailable; no
farther actor or fabricated dialogue replaces this candidate. Message IDs,
control tokens and US text remain original. The waiting arm writes no switches,
quest, coins or pouch. Talk availability avoids string allocation during scans.

The regular movement owner remains unbound: this NPC lacks the direct-XZ flag
that admits the east Goomba's bounded wander adapter. Current presentation
retains its initial S_1/position and ordinary candidacy-facing service. This is
provisional native presentation, not original stationary behavior. Full side
collision/gravity and shared radius100/speed30 wander integration remain next.

`startTroubleQuest` implements original irai_start_func: clear5405..5421 in
ascending order, then set1469..1474 bytes to0 in ascending order, return2.
It does not set quest1420 or accept a task. `cancelTroubleQuest` admits recovered
original quest2=[95,-1] and quest13=[107,-1] table domains. After the same resets,
repeatedly count/remove/recount each item until count<=0, ignoring removal result
and using low-level pouch removal without Mario/party parameter revision.
No quantity caching, synthetic count clamp or quest1420 mutation is introduced.
Unknown task tables/missing providers return native nullopt before writes;
contained2 is the original callback result. Original callbacks do not have
these native admission checks. Providers own complete switch/pouch storage.
These portable callbacks are ready for the original acceptance/cancel caller;
current native UI and checkpoint v8 do not own all17 flags/six bytes.

Source contracts: [writers and tables](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md)
and [get-item/coin service frontiers](GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md).
Direct inventory insertion would bypass itemseq_GetItem and is not implemented
as the source acquisition event. Task menu filtering/acceptance/cancellation,
item95 presentation, coin acknowledgment and full switch persistence remain
unbound. This binding does not claim task acceptance or matched source raster.
