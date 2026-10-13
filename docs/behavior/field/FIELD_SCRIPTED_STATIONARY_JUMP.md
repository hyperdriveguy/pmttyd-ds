# Scoped stationary scripted Mario jump

US G8ME01 revision0 static source at evt_mario_jump_pos0x800e4c54..0x800e5154,
**0x500 bytes /320 instructions**, body SHA256
`522113ffc62aced79e344638364afe2ca12fc74577775314637bc07e4fe9aadd`.
The earlier [function sketch](EVT_MARIO_JUMP_POS.md) incorrectly described its
size, callees, argument count and mode meaning; this scoped contract supersedes
those claims for the before-battle caller. The ordinary function resolves six
operands (XYZ floats, milliseconds integer, direction-policy integer, height
parameter float). Internal modes0/1/2 are event continuation states, not four
caller-selectable movement modes. Mario comes from marioGetPtr8005e0c0;
`marioMakeJumpPara`80094020 advances the derivative tuple; distABf8000722c and
angleABf8000703c provide horizontal queries. Completion returns1, while active
continuation returns0. The event VM must consume/yield on1, not repeat that call.

The harbor caller at gor0x267e0 supplies current Q1024 XYZ,200,0,-1000.
Height parameter<=-1000 selects source20. When abs(targetY-rootY)<1, normal
non-roll handling uses half=0.5*sysMsec2Frame(milliseconds), denominator
fmsubs(half,half,half), coefficient=(height+height)/denominator, V=coefficient*half,
A=-coefficient, J=S=0. Counter=trunc(0.5+half). Each ascending callback adds V to Y,
then runs the derivative update and decrements the counter. At transition it
sets V=0 and A=2*(targetY-currentY)/denominator, with the same counter.

Descending callbacks update derivatives BEFORE testing/adding Y. An admitted
support with Y+V<=supportY triggers original landing cleanup and count0; the
shared decrement then consumes the command. Otherwise Y+=V and the counter
still decreases. Exhaustion clears the tuple/speed and ends. Current native
coverage is stationary XZ, abs(Y delta)<1, normal flags/divisor1, duration with
at least4 frames. The runtime's normal60Hz bridge converts200ms to12 frames;
original video/pause cadence is not claimed as measured behavior.

Binary32 matters: root/target15, parameter-1000,12 frames peaks at
42.9999961853 after6 callbacks; descending sample11 is15.0000019073, above
support15. Landing therefore occurs on callback12, not on the idealized real-
arithmetic sample11. The portable helper retains source float operations and
external admitted support. Source heading queries, horizontal travel, motion8
initialization/presentation/cleanup, roll and 30Hz flag branches remain native
backend dependencies; no general jump or collision equivalence is claimed.

`tools/verify_harbor_scripted_jump.py` independently checks20 source instruction
anchors and5 constants directly in the read-only DOL: sentinel-1000, default20,
half0.5, near-equality1 and zero0. Source marioMakeJumpPara updates J+=S/divisor,
A+=J/divisor, V+=A/divisor. This is distinct from ordinary field jump/release
physics. Host tests cover the binary32 trajectory, admitted landing and rejection
of unsupported height domains; original scene playback must exercise the actual
caller's operands. No Dolphin/hardware observation was performed.
