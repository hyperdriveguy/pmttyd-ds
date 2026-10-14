# Native mode2 publication uses retained camera state

The trio's actual cam3dMain8000ea68 provider no longer requires the optional
`setCameraDepthLensAdapter` test hook before executing admitted mode2. The
retained CameraManager camera4 already owns FOV, near/far and projection type.
The hook is a native diagnostic adapter, not an original source prerequisite.

Existing callback ordering is unchanged: flag2 skips callback work; otherwise
unsupported mode or unavailable battle-frame production suspends. Admitted mode2
updates the battle frame, copies eye/at/up into the retained record, calculates
distance/yaw, and builds perspective from current FOV/near/far with the original
fixed aspect1.2666666507720947. Member aspect remains retained. The existing
orthographic branch remains unchanged. CameraManager Main subsequently publishes
LookAt plus roll/translation/extra in its own ordered continuation, then reaches
the distinct camDraw provider. A camera resume does not collect unit poses/shadows
or queue records again. See [entry/history](UNDERGROUND_BATTLE_CAMERA4_ENTRY_AND_INHERITED_LENS.md).

The integration test changes actual retained camera4 lens fields before the
ending camMain boundary. With no manual depth-lens hook, it expects projection
publication from those values, preserved member aspect, unchanged collected
queue/shadow counts, shared owner identity and subsequent original provider holds.
The later explicit diagnostic lens change must still leave the already published
projection unchanged until its real publication path runs.

This removes a false native gate at an actual ending-path producer. It does not
supply initial pre-display field matrix history, implement unsupported controller
modes/providers, invoke camMain from another cadence, or prove normal battle
framing. Initial dispCalcZ admission, full original view history, queued raster
fidelity and field return remain separate unfinished work. Inputs, story,
source controller math and original actor/asset values are unchanged.
