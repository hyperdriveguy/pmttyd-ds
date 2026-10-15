# Native command palette and tutorial registration

This connects the existing recovered
[CommandDisplay](BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md) to the
actual tutorial VM. It is native ownership/adaptation, not a new measurement of
original timing or a completed battle display scheduler.

`bindBattleCommandPaletteMemory` replaces exactly the four imported DATA bytes
at80416d20 with the existing process-owned `BattleCommandPalette::rgba`, and
maps its signed state byte at8041eba0. It requires the original DATA span to be
present and the BSS stage to be unmapped. Failed attachment preserves previous
mappings; neighboring DATA bytes remain intact. Successful attachment retains
the process color history rather than copying the new VM's cold initializer.
Guest byte/word reads and byte stores use the actual shared backing, including
alpha and invalid signed stages. This is not a source resource Init/End call.

The caller keeps that process owner alive for the address-space lifetime.
HarborTutorial retains a shared owner after explicit attachment and refuses a
different owner or repeated remapping. Normal native harbor entry attaches
Game's existing process palette, which already survives Game restore and is
shared with the later Trio battle. No battle-entry or per-display reset occurs.

`HarborTutorial::registerCommandDisplay` calls the recovered whole registration
helper with the actual tutorial BattleWork and actual VM services. It requires
that attachment. The caller owns the DisplayQueue and BattleFooterCursor across
holds. The global color step precedes unconditional HP callback registration;
every independent live B1720 mask then controls its own callback, as in the
source. A full queue retains the pending record without repeating the color
step. Successful registration queues callbacks only; it neither executes class
drawing nor writes menu readiness nor samples physical/touch input.

The simulation scheduler must call this once per original admitted display
pass after the required battle/camera work, dispatch every record through the
camera owner, and retain all missing HP/weapon/target/etc. callback providers.
Completing these APIs does not authorize skipping unconditional callbacks or
advancing on native render frames. Normal whole BattleInit/End, complete
btlDispMain, and the command/attack/exit lifecycle remain integration work.

Tests cover shared aliases in two separate VMs, live palette stores, imported
neighbors, invalid stages, failed/repeated attachment, and the actual tutorial
VM's full-queue hold followed by exact HP/class registration. The actual-asset
test proves registration leaves class readiness zero until source drawing.
