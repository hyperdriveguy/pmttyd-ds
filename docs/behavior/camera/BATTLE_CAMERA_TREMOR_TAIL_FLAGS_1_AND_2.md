# Battle camera horizontal and vertical tremor tail

US G8ME01 revision0 read-only DOL recovery2026-10-14. Refines the formerly
unexpanded tail in [Main/init/victory](BATTLE_CAMERA_INIT_DEFAULT_AND_VICTORY_MODE17.md)
and [native frame boundary](NATIVE_BATTLE_CAMERA_FRAME_OWNER.md). B is retained
BattleWork; C is camera4. All B offsets below are absolute BattleWork offsets.
This is complete local tail arithmetic/order, not an original execution trace.

## Position in Main and independent rumble gates

After mode handler, eyeZ cap, axis freeze and baseline/delta publication,
8012dc34 reads flags wordB2754. If bits1/2 BOTH clear, request
padRumbleOff80005f28(0). Freshly read flags again; bit40 independently requests
the same service. Both calls can occur in one invocation. Read flags freshly
again before horizontal admission. Do not fold these reads into one snapshot.

Horizontal flag1 runs at8012dc70..dd90, then freshly read flags at8012dd94
before vertical flag2 runs8012dda0..dec0. No timer admission precedes their
interpolation requests. A zero/negative timer must not be used to skip that
interpolation, temporary speed mutations or timer decrement.

## Per-axis fields

| Meaning | Horizontal, flag1/X | Vertical, flag2/Y |
| --- | --- | --- |
| Curve mode signedword | B2828 | B282c |
| Total signedword | B2830 | B2834 |
| Remaining signedword | B2838 | B283c |
| Interpolation start float | B2840 | B2848 |
| Interpolation end float | B2844 | B284c |
| Published amplitude float | B2850 | B2854 |
| Saved/restored movement speeds signedhalf | B277c andB277e | B277c andB277e |

## Exact axis continuation

1. Capture/sign-extend speed277c, then speed277e. Read remaining then total;
   store speed277c=3. Compute wrapping32 phaseDifference=total-remaining and
   phase=phaseDifference&2 (bit1, **not modulo2**). Store speed277e=3.
2. Freshly reread total and remaining. Read curve mode/start/end from the axis
   fields. Compute wrapping32 elapsed=total-remaining, then signed elapsed/2
   and signed total/2, each rounded toward zero. Source srawi/addze implements
   this; C++ signed division is valid for divisor2, while subtraction requires
   defined wrapping. Phase uses the earlier captured values.
3. Request intplGetValue80006c8c(mode,elapsed/2,total/2,start,end). Store returned
   single amplitude in2850 or2854 immediately. Original has no local rejection
   of total0 or negative mode/timing. Exceptional/unsupported interpolation is
   an explicit provider boundary, not amplitude0 or an early successful return.
4. Freshly load signed remaining. If >0, construct the six MoveTo coordinates
   from CURRENT C.eye0c/10/14 and C.at18/1c/20. For horizontal, modify only eyeX
   and atX by single addition of amplitude if saved phase==0, otherwise single
   subtraction. For vertical, modify only eyeY and atY likewise. The other four
   coordinates remain current camera values. Request battleCameraMoveTo8012c128
   with integer arguments(0,0), the six floats and the six ordinary Main local
   delta output pointers. Both speed fields are3 during the request.
5. If remaining==0, freshly read flags and clear ONLY this axis bit. It makes
   no MoveTo request. If remaining<0, neither clear bit nor call MoveTo.
6. Restore speed277c then277e from the previously captured signedhalf values.
   Freshly read remaining, wrapping-subtract1 and store to its word field.
   This decrement happens for positive, zero and negative remaining alike.

The float shifts occur separately and are binary32 add/subtract operations;
do not accumulate both axis shifts in double and then narrow. MoveTo's genuine
speed3 subtract/add pathway can also have rounding distinct from assignment
of the requested target, so reuse its source implementation. The source provides
six local delta addresses even though these results are not published again.

## Retained baseline and sequential axes

B2780..2788 eye, B2798..27a0 target and B27bc..27d0 delta snapshots were
published BEFORE these tremor blocks. There are no repeated baseline/delta
stores after either MoveTo. Tremor changes C.eye/target used by outer camMain
without overwriting those retained baseline snapshots. The second axis reads
the camera already shifted by the first; do not reconstruct both from old B
snapshots or discard horizontal mutation on vertical entry.

The axis freeze bits4/8/10 were applied before this tail. No second freeze or
eyeZ cap occurs after tremor. Up-vector changes made by ordinary MoveTo speed3
remain its actual writes. Source interpolation does not itself restore camera
up or its lens/projection state.

A missing interpolation or MoveTo provider must retain stage, saved speeds,
phase and prior axis effects. In particular speeds have already become3 before
interpolation; restarting whole Main would repeat baseline/earlier axis work.
Returning Main unavailable is valid; claiming completion after clearing flags
or replacing unsupported interpolation by0 is not source behavior.

## Validation and implementation boundary

Fresh ISO reads matched all164 instructions in8012dc34..dec4 to existing
ignored complete listing artifacts/recovered-temp-evidence/calcdepth-battleCameraMain.dis.
Complete Main8012c8b0:1638 SHA256:
1bc682b107a1dac6a1134f51d34cd2482ecec868c805424840d35e584c6c0edd.
Tail8012dc34:290 SHA256:
4f22fdcd55fd6cabe03cc3aa8d55c8bef9f6b6f061ba9236fa60e1be337200d1.

Existing native intpl adapter and genuine MoveTo speed3 can provide supported
curve modes; arbitrary source function-pointer curves remain explicit missing
services. Original rumble maps to the declared no-motor New2DS XL adapter.
Native source history, callback cadence, exact Gekko math and rendered parity
remain separate verification. Documentation only: no build/emulator claim.
