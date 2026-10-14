# Native Garf init and ordinary talk

Source: [actual quest1 actor and door copy](TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md)
and [complete talk/admission contract](GARF_ORIGINAL_TALK_AND_REGISTERED_HIDDEN_ACTOR_ADMISSION.md).

The PMNPC009 catalog now registers original gor_02 盗賊団２ (raw Shift-JIS
939091af92638251), c_monban, tribe row80333404. Native init profile2 is bound
to that exact resource/name pair; it cannot be attached to another model or
actor. Older profiles/layouts remain readable. Source Entry/setup/init flags
are40080603, original yaw270, idleS_1 and source dimensions30x30.

Init waits for known GSW1420. Quest1 places the retained actor at(445,0,120);
every other known quest uses(10,0,-460). The authored tagged placeholder remains
(0,-1000,0). Init runs once per field instance; later saved-state changes do
not replay it. The existing named-position service lets actual original Q1
start/end children move that same current/previous/movement body. They still
wait on the unbound house05 DoorParameter service afterward. No successful
door no-op or invented GW11 handle is provided.

For known1420!=1, ordinary talk uses the original story table: <56 unsuffixed
gor_02_011, then <164/_01, <215/_02, <261/_03, <339/_04, <373/_05,
<401/_06 and default/_07. There is no112 boundary. Original msg/US/gor_02
text, waits, key controls and actor attribution use the existing retained
FieldActorTalk owner. Unknown quest and active quest1 remain unavailable;
the source advice/key-selection/unlock branch is not replaced by ordinary talk.
Native quest dialogue and end-to-end house05 access are not claimed complete.

The original mesh/animation conversion retains Z_1 at index0 and S_1/T_1/W_1/R_1.
Its26 used texture cells use the existing wrap-aware atlas, reduced to512x256
RGBA8/512KiB with normalized UV ratios unchanged. Room-scoped model/texture
sharing and Citro3D actor batches need no character-specific renderer branch.

Host checks cover unknown/init branches, exact source resource/name, no init
replay, all ordinary story boundaries and actual original Q1 start/end position
writes. Isolated native capture shows the original green-bandana/purple actor
at the quest1 street position; exact artifacts and timing are in PROGRESS.md.
This verifies this native view, not source runtime/raster parity or hardware.

Next: real house05 mutable72-byte record/copy/GW11 lifetime and NPC-list
consumers, original key49 selection/unlock scene and ordinary-talk runtime
comparison. Source800 is a draw suppression bit, not actor deletion or a name/
collision/candidate gate. [Retained main flags and the late800 draw gate](NATIVE_RETAINED_NPC_FLAGS_AND_DRAW_SUPPRESSION.md)
are now bound. House05 itself skips the generic800 paths; its real list
consumer affects animation material1800 instead. Those material/copy owners
remain unbound; full registered lifetime across native coverage changes is open.
