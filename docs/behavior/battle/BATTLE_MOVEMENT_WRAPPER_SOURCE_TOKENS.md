# Original Jump movement requests and exact encoded float tokens

US G8ME01 revision0; ID2,2026-10-12. Read
[complete setter order](BATTLE_MOVEMENT_SETTER_EVENT_ORDER.md),
[float routing](../event/EVENT_FLOAT_OPERAND_ROUTING.md),
[Jump source graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) and
[Majinai child](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md).
Verify `python3 tools/verify_battle_movement_wrappers.py` read-only against ISO.
This is source bytes/decoded local numeric semantics,not original runtime timing.

## Eight complete USER_FUNC request spans

Each pinned16-byte span has header0003005b,original callback pointer,raw actor
fffffffe(-2),raw float token. Seven occur in ordinary Jump;one in Majinai nested
Jump. Addresses and exact outputs follow:

| Source address | Callback / source | Raw token | Binary32 value | Stored bits |
| --- | --- | --- | --- | --- |
| 80355f40 | SetMoveSpeed / ordinary | f24a8e80 | 5 | 40a00000 |
| 8035607c | SetMoveSpeed / ordinary | f24a8680 | 3 | 40400000 |
| 80356b6c | SetMoveSpeed / ordinary | f24a9280 | 6 | 40c00000 |
| 80356688 | SetFallAccel / ordinary | f24a7d4c | 0.69921875 | 3f330000 |
| 803566fc | SetFallAccel / ordinary | f24a7ce6 | 0.599609375 | 3f198000 |
| 8035690c | SetFallAccel / ordinary | f24a7d4c | 0.69921875 | 3f330000 |
| 80356ab8 | SetFallAccel / ordinary | f24a7ce6 | 0.599609375 | 3f198000 |
| 80363438 | SetFallAccel / Majinai nested | f24a7c80 | +0.5 | 3f000000 |

Float tokens are event encodings,not the IEEE bit patterns in the last column.
Retain original words in event packs and resolve them through evtGetFloat.
Do not round0.69921875/0.599609375 to0.7/0.6,or infer
physical units/frame duration from the numeric field alone.

## Explicit decode and Majinai sign correction

For these words,the documented literal range decodes
single(C32(wrapped signedToken+230000000)*binary32(1/1024)). The original
multiplier8041fa1c is bits3a800000. Used biased integers are5120,3072,6144,716,
614,512 respectively;all are exactly representable before multiplication.
The verifier pins the multiplier and reproduces binary32 results from the
original source tokens. This calculation does not execute the original event VM.

In particular f24a7c80 is signed-229999488;adding230000000 gives+512,and
multiplying by1/1024 gives+0.5. The previous Majinai prose's -0.5 was a sign
error and is corrected. This matters for the native port's fall-acceleration
write;the subsequent motion core determines how that positive field is used.
No negative-gravity or authored substitute token is justified by that old note.

## Scope of evidence and port acceptance

Verifier covers eight complete/linked body hashes(four wrapper/leaf complete,
four operand/actor/getter identities),64 instruction anchors,one startup span,
float coefficient and eight source spans. Existing operand/actor/getter contracts
supply their full semantics;these new documents close the requested wrappers.

Check actual packed source invocation with actor-2 and raw tokens through the
native resolver/lookup/leaf path. Separate authored mutation/null/float-edge
fixtures from original source witnesses. Count source sites rather than dynamic
calls;loops/branches/scheduling can repeat or skip them. The Majinai span is
outside the original three-stream callback census and has its own nested source.

Source actor/unit setup,registry/storage lifetime,callback registration/full
ordinary Jump/Game motion/action-command cadence/platform numeric domains and
original traces remain open. Original reused story/art/audio/data stay outside
source history;these contracts convert no assets and claim no complete-game
requirements or native trajectory fidelity.
