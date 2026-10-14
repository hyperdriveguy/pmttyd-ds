# Original trio field-return handshake and missing native providers

2026-10-13 read-only US source follow-up to
[ending1f](UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md) and
[field battle protocol](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).
Complete local listings retained ignored `artifacts/recovered-temp-evidence/return-*`.
No native changes/commit. Existing ProfessorStory/Game inspected read-only;
current battle ending, persistent pouch and preserved underground VM have
separate owners. Ending1f is not immediate scene completion.

## Actual sequence polling and fade-in

seq_battleMain80073b24/3dc uses table8030c3e0. State5 target80073dac polls
battleSeqEndCheck800f903c, which reads actual Bflags80000000; it does not call
BattleMain itself. Ready clears sequence counter+10 and increments state+4
to6. GP+c nonzero overrides state11 (alternate ending). State5 still executes
its ordinary camera/sound-location tail and returns. No same-call state6 count.

State6 target80073e3c increments counter+10 first. Only signed result>30
admits seqSetSeq8002e218: GPword0 both1000 AND2000→(5,1,0), otherwise(2,1,0).
Do not change >30 to>=30 or erase the alternate GP+c/flag branches. Uppercase
BattleMain topbank4→return0, lowercase battleMain publication, sequence driver
and field event waiter are distinct ordered services.

Entering sequence2 calls seq_gameInit8000753c (time-base publication and
GP18clear10); seq_gameMain800073a4 state0 issues fadeEntry. GP0mask10 uses
special18/900 path plus event start and clears that mask. Ordinary GP118==0
uses type/time at GPf8/fc, otherwise GP108/10c. After requesting the selected
fade, set BOTH incoming presets GPf8=9/fc=300,GP108=9/10c=300, then state1.
There is no local sequence0→1 fallthrough. Frame-loop sequence publication/
exit/init cadence remains its original sequence-manager provider.

marioStInit8006fb3c constructor writes GPf8/fc=9/300,GP100/104=10/300,
GP108/10c=9/300,GP110/114=10/300 at8006fca8..d8. Ending's fade10/200 only
calls fadeEntry; it does not overwrite these GP presets. Fresh ordinary
retained presets therefore admit source fade9/300. Authored
evt_fade_set_mapchange_type80135ccc/5d10 and continue/load producers can
override them; preserve their actual values instead of always assigning9/300
at battle finish. A bounded direct-store/nearby-GP-load census identifies those
writers, not a complete whole-program alias proof.

## Original Goombella unlock belongs to joining history

Fresh initializePackedPouch clears every party row flag. Actual authored
producer is `christine_nakama@gor_01.o` (Rogueport plaza joining scene), root
RELfile37458/be0, SHA256
`28feaf1c480e1eadbff8fd701e48800bef6b1356354ea0f7649db4d46d68ef3b`.
At374ac it opens an inline event;374b0 calls evt_pouch_party_join800f0a8c(1)
before374bc evt_mario_set_party_pos(0,1,25,0,625), facing/movement requests.
This is an original script callback, not battle1f or a generic story-threshold
side effect. The unchanged joining graph/caller admission must own it.

Wrapper800f0a8c/30 reads/evaluates operand0, calls partyJoin80140f74/44,
returns2. partyJoin takes signed kind; kind<8 reads pouch U16[14*kind], OR1,
stores same U16. No lower-bound check, HP/FP/reset, level change, actor entry
or save write locally. Used1 writes P0e|1 only. Negative kinds are outside this
used caller domain. Native ordinary source-join execution must mutate the same
Progress-owned pouch used by later battle export. A declared story11 checkpoint
snapshot can preserve that authored history, but battle ending cannot silently
unlock an unjoined new-game pouch.

marioPartyEntry801412fc kind1 requires current P0e&1; otherwise returns-1.
Existing signed fieldMario245>=0 is removed,245=-1; successful partyEntry
registry index is stored245 then partyHello. At ending1f, current battle party
kind224 translates to field kind1. Party constructor failure is not permission
to create a substitute actor. Field constructor/animation/placement providers
remain distinct from existing native field billboard or battle actor memory.

## Required exit services before field waiter

seq_battleExit80073f00/20 delegates battle_exit800736ec/1c0, in this order:
BattleEnd800f8da4; N_battleMapFree80030048; smartAutoFree8002f9cc(2);
itemReInit800aeb20; iconReInit8001b64c; extReset800d6b7c;
bmapUnLoad80028aa8; camera4 mode based on its flags4;
npcReset8004bafc(1);mobjReset800612f0(1);offscreenReset80073394(1);
imgAutoRelease80071828(1);effAutoRelease8005ecd0(1);
animPoseAutoRelease8003b028(1);evtmgrReInit800377b0;
npcRecoveryFiledNpc8004cd38; GP14=0;evtStartAll80035bd4(4).
If r13+1a48==0: marioCtrlOn,partyCtrlOn, then fieldMarioword0 OR1.
Next musicstop, psndClearFlag(80), original light/audio restoration; source
story109..110/402/nextseq5 select their own later audio branches.

These resource/context/event services must preserve paused field roots and
original NPC placement/lifetimes through field recovery. Current Game's
trioBattleActive and ProfessorStory pending callbacks cannot replace this
entire chain with a finished boolean. Smallest explicit unknown at exit is
BattleEnd800f8da4 plus transitive resource/context preservation; existing
per-subsystem release/event contracts should be reused, not success stubs.

## Control reference counts, party slots and post-battle placement

evt_npc_wait_battle_end8004eed8 admits seq2 AND I10!=0 (any nonzero result),
then marioCtrlOn; current party lookup/nonnull partyCtrlOn; L_psndBGM_stop;
marioFBattlePost;return2. No local result clear/first-call gate or fade-finish
poll. Preserve original sequence fade and event-manager cadence rather than
adding an invented wait requirement.

marioCtrlOn8005c290/34 decrements Mario byte38 with8-bit wrapping; if signed
new byte==0, clear Marioword0mask2. Return signed newbyte. It is a nested
control counter, not a setter that forces controls on. partyCtrlOn800bed0c/38
ignores passed actor and clears word0mask20000000 in BOTH nonnull registered
party slots r13+1bb8[0] and[1], so Frankly slot1 is included.

marioFBattlePost8005c30c/c0 clears fieldMarioword0mask1000. Action20 copies
retained XYZ globals toMario8c..94; action26 requests its own motion service.
Current nonnull party calls partyGetAppearPos800c6c9c(P,out), copies returned
XYZ→P58/5c/60; then calls global partyCtrlOn AGAIN. Mario actions18/20 also
request mode service8009455c(0). This is distinct from ending1f clearing
Mario/fieldparty word4mask1. It does not universally choose idle/teleport/reset.

partyEntry800bea68 begins with partyEntryMain800be144 allocation/definition provider,
sets placement via partyGetAppearPos2, updates retained XYZ and derives pose
including currentHP<=5 branch. partyHello800bbc48 may immediately call
partyHelloMain800bb544; its mode/owner branches retain their original fields.
Full used appearance800c6c9c, allocation/definition and HelloMain providers
are bounded native-frontier requests; do not substitute a guessed fixed offset
or retain battle coordinates as original field placement.

## Unchanged authored continuation

The preserved161-command root resumes its wait, deletes the three original
movement eventIDs, places NPCs(-65,0,65),(-55,20,35),(-55,0,85), restores
MarioM_S_1 and source NPC damage poses, sets original camera then waits500ms.
Only later exact093→094_kur→094_01/extparty messages, two shared completion
flags/escapes, camera-off, party_run0/1, story12, hit-offa_rec, keys-on and
original underground music occur. See
[complete graph/dialogue](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md).
Do not publish story12 or finish the field scene at battle export. Native
ProfessorRuntime cases60/61 now use the supplied FieldBattleServices owner
for the original start/wait protocol. Unbound standalone scenes retain their
service barrier; case62 previous-party placement remains independent. The
native Game supplies the owner only for the underground scene and launches
the trio on the exact start callback8004ef54, without a second initialization.

The host scene regression reaches the original093 print only after sequence2
and nonzero result. It checks the original NPC1 speaker, story11, PTK_D_1
and field Para-Goomba XYZ(-55,20,35). This exposed a movement adapter lifetime
bug: deleting the original movement event left its interpolation active.
Movement now retains its creating eventID and stops when that event is gone,
preserving the root's postwait placement. Linear movement remains an adapter;
this does not claim original ease/collision stepping. The test supplies a
field outcome; native ending remains suspended at field getter8005e0c0, so
actual native sequence2/result publication is still unfinished.

Validation: read-only complete used bodies/tables, exact original join record,
current native APIs inspected, `git diff --check`. No native tests/builds/commit;
source release/appearance/sequence/pouch-save lifetime and original execution
comparisons remain explicit implementation boundaries.
