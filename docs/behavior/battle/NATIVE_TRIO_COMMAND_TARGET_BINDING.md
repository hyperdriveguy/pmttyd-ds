# Native setup4 command and target selection

2026-10-13 implementation adapter. Source authority:
[trio targets, turns and victory](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md),
[sampler](BATTLE_ENEMY_CANDIDATE_SAMPLING.md), and
[target confirmation](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md).
This does not claim complete original command input or combat execution.

`TrioBattleCommands` owns semantic registry slots1..6: Mario, Goombella,
Goomba, Spiked Goomba, Paragoomba and Frankly. Target sampling calls the existing
recovered sampler with the original default weapon admission/attribute words,
enemy belong1 and player direction+1. The four original setup records provide
part attributes, local coordinates, hit/sampling offsets and forward bias;
current positions come from the source battle scene. Frankly remains owned and
excluded through his source part flag. Stored status27 drives removal; zero HP
alone is not converted into death. Position/sampling views rebind on each call,
so copying the native Game cannot retain another owner's pointers.

The menu starts after Frankly's source phase event and message window both end.
Mario's Normal Jump/Normal Hammer and Goombella's Headbonk retain original
weapon addresses and original global message labels. Only Mario is currently
reachable through the native turn entry; partner selection awaits the turn
scheduler. This is a 3DS menu layout adapter rather than the original rotating
command UI. Touch selects and confirms; D-pad/Circle Pad select through the
existing physical producer's repeat publication. GC A/B retain0x100/0x200.

Target refresh retains unit/part through row changes, resetting to index0 if
absent. Navigation increments/decrements the fresh permutation and wraps;
both repeat masks execute. Cancellation precedes confirmation. Confirmation
retains actor, weapon, unit and part in an `AttackReady` request. It spends no
move, applies no damage and does not resume the field event. Original input
state29 effects, sounds, stage light, help/audience requests, attack event
consumer, turn/reaction barriers and field result remain unbound. The displayed
pending notice is a development notice. B returns from an unconsumed request
for continued menu inspection; that return is a development adapter.

Host tests use actual imported source records and labels to exercise source
trio geometry, spike admission, Hammer front occlusion, status27 ownership,
row-compaction selection, dual navigation, cancel priority, empty candidates,
owned copies, native physical/touch entry and retained dispatch tokens. Native
emulator captures are presentation checks, not original execution evidence.

Next: accept dispatch into original Jump/Hammer/Headbonk scripts, bind source
AC/motion/hit/counter services and source poses, then advance recovered move
counts/phase order, enemy actions, source death/fall reactions and result/field
return. Keep the documentation inbox requests open until runtime composition
and original comparisons are complete.
