# Horizontal battle camera shake request

Original US event wrapper evt_btl_camera_shake_h8012b3d0/f0 and helper
btl_camera_shake_h8012b4c0/60 are implemented by the shared portable
battleCameraShakeHorizontalEvent. Complete read-only DOL spans, including both
final return instructions, are SHA256-pinned in tools/verify_battle_camera_shake.py.

The wrapper captures its argument pointer and resolves priority, X amplitude,
Y amplitude, duration and cycle argument in source order. The amplitudes come
from signed integer GetValue, converted to single precision; GetFloat is not
used. Raw Y is loaded before converting X, and raw duration before converting
Y. Float-service callbacks therefore cannot change those captured operands.
The helper reloads the global battle pointer after the argument services.

Priority uses a signed comparison against battle275c. A lower priority returns
2 without camera writes or rumble. On admission, the helper ORs flag2 into
battle2754, stores float X/Y at2848/284c, cycle argument at282c, and duration at
both2834 and283c. It reloads flags and calls original padRumbleOn(0) only when
flag40 is clear. Other flag bits survive. Native bindings retain rumble requests;
New 2DS XL hardware does not implement the original GameCube motor.

Both tutorial and trio now use the shared request implementation. The trio's
old immediate success arm for this function was removed. Camera movement,
shake-wave consumption and full native camera framing remain separate providers;
request publication is not evidence of visible shaking.

The tutorial host fixture now executes the original ordinary Crump impact,
original shake request, Mario's timed reaction and original attack return.
HP7 remains6 without repeat impact; Mario returns to M_S_1, both attack/damage
roots finish, no unsupported callback remains, and installed Mario/Crump wait
handles are restored. It checks packed shake fields against the source event
operands and retains channel0 rumble requests. Guard/lethal boundaries remain
explicit; this is not the complete tutorial player-command or turn lifecycle.

Tests also cover priority rejection/equality, preservation of flags, signed
negative duration/amplitudes, large-integer float rounding, suppression by40,
operand mutation during float conversion, and global-pointer reload after
arguments. These are host/static checks, not Dolphin observations.

See [ordinary tutorial impact](TUTORIAL_ORDINARY_CRUMP_IMPACT.md). Next:
source camera main and shake presentation, original tutorial command/action/turn
ownership, and effect/audio/audience consumers with physical control paths.
