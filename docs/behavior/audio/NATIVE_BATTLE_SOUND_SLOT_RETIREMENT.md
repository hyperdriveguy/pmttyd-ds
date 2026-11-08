# Native battle-owned public sound retirement

This is native lifetime metadata and teardown policy layered over the recovered
public PSND routines. It is not original `btlseqEnd`, `BattleEnd`, `psndMain`, or
proof of audible playback. See [the original voice/public-slot closure](RETAINED_BATTLE_VOICE_AND_PUBLIC_PENDING_SOUND.md).

`startPositionalSfx` publishes a new native incarnation token immediately after
its original successful allocation-generation store. Rejections, missing children,
and merely finding an existing public slot do not mint tokens. The token is not
the original 16-bit generation: source handles wrap and source Off intentionally
ignores handle generation. A native consumer must retain the actual registry and
compare `slotLifetime(index)` against its captured token. Registry copying does
not transfer actual bank identity. Manually populated slots have unknown native
births. Direct foreign slot writes are outside the source-On ownership policy.

Actual public initialization retires all native slot identities at its existing
committed source initialization boundary. Actual Off retires the identity only
after any required backend stop succeeds and source key/engine clearing occurs.
Native metadata adds no original memory reads/writes and does not change stale
source handle behavior, source slot selection or allocation-generation wrapping.

`NativeBattleSound` records only tokens changed by its own actual On calls. Its
bounded table holds at most one current incarnation per public slot. External
Off, public initialization, stealing and foreign allocation do not allow native
teardown to stop a replacement sound, even if its numeric handle is identical.
The registry is strongly retained by the sound binding; the actual VM/process
proof is checked before descriptor reads. No public facade pointer is captured.

`retireOwned` is an explicit serialized native teardown step. The caller first
stops the actual battle event roots and retires presentation outside a render
frame. On the first admitted retirement attempt it freezes new sound starts and
pins backend identity across holds. Pending source slots and engine `-1` need no
hardware acknowledgement, as in original Off. A started engine requires a real
backend stop; descriptors come from actual original rows in the bound source VM.
Earlier successful stops are retained when a later stop holds. Successful backend
work is not replayed if process lifetime is lost immediately afterward. Recursive
retirement or new voice/menu/position work during teardown is rejected.

Completion clears native retained voice-cursor metadata only after all owned
slots have been handled. This is cancellation of an already stopped native
battle scheduler, not successful completion of a held original voice event.
Destruction alone does not call foreign providers or infer teardown success.
The external stop callback must not mutate/replace the same actual registry while
it is stopping an engine; this follows the existing serialized source Off
contract. Hardware playback, music retirement and field-wait acknowledgement
remain separate actual owner operations.
