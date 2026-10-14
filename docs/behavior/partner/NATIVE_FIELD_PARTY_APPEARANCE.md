# Native ordinary partner appearance and fresh Hello

2026-10-13 binding of [appearance2](FIELD_PARTNER_APPEAR_POSITION2.md),
[entry capture order](FIELD_PARTY_ENTRY_FIRST_APPEARANCE_ORDER.md),
[heading1](../overworld/OVERWORLD_DISPLAY_TO_MOVE_HEADING.md),
[heading2](FIELD_PARTNER_PLACEMENT3_MATH.md) and
[trig/movement](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md).

FieldPartyAppearanceServices reads actual persistent Game root/display/auxiliary
owners and current camera offset/field flags. Root uses the existing native
engine-unit bridge: X/Z from progress.position*100 and Y from
(groundHeight+jumpHeight)*100. Display is raw MarioPresentation.display,
independently read twice by the wrapper; facing/target/drawYaw are not
substituted. Dry ordinary field restoration is supported; water/other
transformed root ownership remains separate. Service records are copied into
the battle runtime and reference its owning Game, whose lifetime must persist.
Copying an active battle into another Game is not a callback rebind service.

The policy retains the initial revise request, current facing reads versus
retained dual facing, output reseeds, horizontal lengths30+Pf4 (56 for fresh
Goombella), Y+37 and down38. Single horizontal hit terminates; a miss admits
move-30 then down. Dual first horizontal miss admits only its first offset/down,
including when down fails. Only first horizontal hit tries the opposite side.
Any nonnull downward hit contributes raw pointY. Failure preserves seeded or
moved XYZ according to the original branch; no floor attribute/name gate,
100-unit ray, extra search, rollback or successful-result substitution.

Native heading/trig providers retain the known global override, Q1000/wrap,
signed offset, different pi/divisor pairs and fused move stores. Host/ARM libm
and geometry arithmetic adapt the source; Gekko FPSCR/libm/numeric parity is
unverified. Generic native fieldMapRay uses original current room triangle
geometry and current object triangles with wallPolicy=false. Existing grid/
bounds accelerate admission; narrow phase retains nearest geometry distance.
Native pointY is reconstructed from the real ray origin/direction/distance
without ground quantization. Full original hit-record/filter/owner ABI and
geometry tie/numeric parity remain separate. Providers do not mutate aliased
Mario/actor/output storage; original arbitrary pointer-alias domains are not
covered by this value-output projection.

After Main, wrapper stores scale(1,1,1), P38=10 and P110/P10c from two raw
display reads. It calls appearance2, copies output into P58..60 and Pb8..c0
regardless of the helper's result, rereads signed canonical HP for the original
known Goombella string comparison, then sets P34=1. Same name preserves the
frame/pending fields. Unknown borrowed names retain their comparison provider.
Only afterwards publish Mario245; then query actor/Hello. Fresh P8 lacks8 and
P38=10, so HelloMain calls appearance2 and reads HP again, copies positions and
sets mode1. The two placement calls remain distinct; no16-count entry jump.

Joined native ending now reaches current-party lookup and clears word4 bit1
through the real registered actor, then executes general rule judgment and holds at AudienceSoundStop8019fd68. Unjoined
source failure has no actor/index/unlock side effect and reaches the same
ending boundary. This constructor does not publish sequence2/result, restore
controls, advance story or acknowledge animation/GPU drawing. Actual actor
display/lifetimes, original join graph, general FbattlePost placement and
ending teardown still need integration.

Portable checks cover single/dual branch query order, floor misses retaining
moved coordinates, both-side obstruction, signed slot offsets, unfiltered raw
Y and live versus retained display reads. Original-bank battle regression
covers scale/substate/mode, both position copies, two distinct placement calls,
current-index publication and unchanged story/result/control hold. Source
contracts provide exact body hashes; runtime/Dolphin/hardware parity remains
unverified. No asset conversion or private payload is committed by this binding.
