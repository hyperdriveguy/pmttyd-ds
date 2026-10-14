# Native Goomba4 quest13 hand-in

`GoombaQuest13` binds the original first and continuing arms of the
[Goomba4 quest graph](ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md), through the
existing native NPC arbitration and composite `GoombaHandIn` owner. Source
[control and speaker rules](GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md)
remain authoritative; numeric speaker0 inherits the Goomba4 type6 owner.
This is native binding evidence, not an original execution trace.

Admission requires the original actor, known quest1420=13, known5406=true,
live count107>0, known5407, original pose/message/selector resources, and the
finite native pouch revision domain. Unsupported nearest candidates remain
unavailable without falling through to ordinary or farther NPC dialogue.

The event allocates one native signed-word table with count+1 entries. Each
positive update fills duplicate107 entries and a terminating-1 in the same
allocation. Zero-count update leaves its previous bytes unchanged. The original
map allocator is represented by this retained native allocation; selector rows
are separate native window storage. Neither selection sorting nor quantity
collapse changes the authored table. Count growth beyond that retained capacity
is a native admission frontier, not a fabricated source allocation/overflow.

The first arm uses unchanged `irai_13_001`, requests original `M_N_5B`, waits
500ms in simulation time, requests `M_N_C`, waits500ms, then requests `M_S_1`
and prints `irai_13_002`. It updates, selects, removes107 through the source
Mario/party revision wrapper, and updates again. Remaining positive count prints
`irai_13_003` before another selection; zero ends the loop. Any non107 selection,
including-1 cancel, also ends it. Every first-loop exit writes5407=true.

The continuing arm updates, prints `irai_13_005` while count remains positive,
selects and removes107, then repeats. After either arm, a fresh count0 prints
`irai_13_004`. Final print completion frees the retained table once. Original
removal result0/-1 does not gate subsequent statements; native unknown-domain
failure remains pending. Neither story, quest1420,5405 nor5406 changes.

Talk depth1 survives every print and authored wait. Selector entry adds depth2;
ready-only deletion restores depth1, and event return restores depth0. The Game
adapter keeps dialogue/control ownership throughout. Message bytes, pages,
disabled-key regions and waits are original US assets; no dialogue is invented.
Native v8 [checkpoint persistence](../format/NATIVE_POUCH_CHECKPOINT_V8.md)
retains the complete packed pouch and explicit5407 knowledge across reloads.
The overlay follows the event through pose waits, so a temporarily inactive
message does not produce the generic harness dialogue fallback.

Pose assets use original a/e_mario with its a_mario texture binding and original
a/a_mario stay pose. A separate21-cell atlas preserves the existing field pack
and avoids exceeding its64-cell limit. The unused lower half is cropped:
1024x512 RGBA8,2MiB decoded GPU storage, source UV scale2. Existing124px cells
and2px duplicated borders remain. Citro3D binds this texture only for the two
quest effect poses, uses the existing linear vertex buffer and animation/root
transform, then restores ordinary character drawing. Resources load once and
are freed after GPU shutdown synchronization. No per-frame texture conversion
or new quest-specific rendering allocation is introduced.

Field entry retires unused legacy/trio battle maps before staging field
resources. Battle entry retires the field map and NPC textures before loading
the battle maps; returning reloads the original field room. Area changes retire
the previous room before staging another and restore it if staging fails. The legacy stage loads on its actual battle render path; the two
trio maps load when the trio owns the world, then retire on field return. These
native residency adapters retain original geometry and texture data. Allocation
and retirement occur before frame submission with Citro3D synchronization;
failure remains an explicit unavailable resource rather than a successful map.
This fixes an observed Azahar field load refusal at Darkly texture import with
only686592 bytes of linear memory free. It does not establish original map-heap
or New2DS memory behavior.

Isolated native resource-provider probe: baseline985728 bytes free; legacy
map admission succeeded with9471104 free, field return succeeded and restored
985728. Trio pair admission succeeded with9724672 free, then field return
restored985728. Before field retirement both admissions failed. This private
probe exercises allocation/retirement and final field drawing, not battle event
execution or battle raster correctness.

Host checks cover exact first/continuing message sequences,500ms pose boundaries,
duplicate slots0/60/120, one table address/capacity, unchanged zero-count bytes,
ready-only deletion, all hand-ins, first/continuing cancellation, flag timing,
unchanged damaged HP/FP, and v8 round-trips. Native Game checkpoint fixtures use
actual source proximity/arbitration, pose publication, control retention and
cancel/reload state. Those fixture conditions and steering counts are harness
inputs, not observed original task acceptance or timing.

Upstream task writers are documented in the
[Trouble Center recovery](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md);
their native acceptance/acquisition binding remains pending.

Remaining: native task acceptance/progression, full switch banks,
original card format/driver, shared event/window/status/SFX/RNG ownership,
matched Dolphin raster and New2DS hardware performance. Pose geometry and GPU
availability do not by themselves establish original lighting/material parity.
