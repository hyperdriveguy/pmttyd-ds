# Native MarioJumpParam count preparation and write-back

Implementation: `include/battle/battle_mario_jump_parameter_event.hpp`,
`source/battle/battle_mario_jump_parameter_event.cpp`,
`tests/battle_mario_jump_parameter_event_tests.cpp`. Contract:
[BATTLE_MARIO_JUMP_PARAMETER.md](BATTLE_MARIO_JUMP_PARAMETER.md).
Original80109c68/19c inspected read-only at /tmp/pm-native-mario-jump-param.*;
no original binary/assets enter history. Verify original identity/callers with
`python3 tools/verify_jump_event_flow.py`.

The callback now registers original80109c68 and uses actual native event/unit/
work records. There is no first-call gate,unit motion store,speed fallback,
minimum count or GetFloat route. Every invocation resolves/translates actor,
integer X,ignored integer Y,integer Z and the raw output operand's requested count.
Private visible XYZ are read in order;distance uses only X/Z. Source unit pointer
is retained after current-work lookup. Its stored speed is queried only for
requested0 and only AFTER the distance request.

Source prefetch details:raw Y loads at80109ccc BEFORE X conversion80109ce0;
raw output/count destination loads at80109d10 BEFORE Z conversion80109d1c.
Both retained tokens survive numeric provider/source-argument mutations. The
same captured destination is then resolved as the requested count and used for
write-back,even if argument words/change of e.arguments occurs later. Current
work is loaded after count resolution,not captured before those services.

Nonzero requested count initializes candidate directly. Zero requested derives
trunc(single(distance/speed)). Original threshold60 at804222d4 is read after
that branch. Only ordered distance>threshold enters separate rounded subtraction,
then source coefficient804222d0=0x3e19999a load/multiply/truncate. Coefficient
load follows subtraction;word addition wraps before signed requested-bound test.
A zero requested accepts any candidate;nonzero accepts candidate only if greater.
If the RESULT equals-250000000,omit output. Other results still request SetValue
on a raw sentinel destination;this is deliberately different from GetMoveFrame.

Native services reuse explicit distance/signed conversion/divide/subtract/
multiply/truncate providers. No std::hypot/native-library numeric default is
synthesized. Test std::hypot/float arithmetic and forced exceptional conversion
results are authored providers,not proof of original frsqrte/FPSCR behavior.

Acceptance includes36→42 at authored D100,threshold/equal/short distance,
automatic26,automatic0 without minimum1,negative requested/speed values,word
wrap below requested retaining its bound,zero-request wrapped candidate,result-
sentinel skip/raw-sentinel write distinction,NaN ordered branch bypass,fresh
post-distance speed and post-subtraction coefficient,prefetched Y/destination/
current work,unit immutability and actual native event/registry byte RAM. A
native brother-creation fixture inherits updated local6=42 from the parent,
using existing source-owned creation services. Its minimal END-only test stream
is a creation harness,not invented story dialogue or actual stylish execution.

Ordinary source sites supply L6=36 before this callback. The updated count is
available for future native MarioJumpPosition and original stylish formulas;
those consumers must not freeze36 or its old windows. This increment does not
claim native airborne motion,bound Crump setup/tutorial,full Jump/outer battle/
Game flow,original executed positions/impact tick or hardware validation. Next:
complete native MarioJumpPosition with explicit numeric/audio/services and
original scene/source/transitive graph composition. Existing documentation
requests track remaining Monosiri/spatial playback/numeric/source evidence.
