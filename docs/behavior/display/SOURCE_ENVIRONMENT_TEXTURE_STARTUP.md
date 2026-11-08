# Original environment startup and texture loading

Fresh read-only US G8ME01 disc0 revision0 DOL inspection and original symbol
identities establish three distinct complete bodies:

| Body | Address/size | SHA256 |
| --- | --- | --- |
| envTexSetup | 8019df84/0xd4 | 029c1a106da0972f86555a2f5c500116ceb848b76b174ee6ff5e28074d1c4e36 |
| envInit | 8019e09c/0x54 | 42f70180a6bdf676cbcbe9935ccd4fe353c7538b0862ced2e21cef8756e98cdb |
| DVD callback | 8019e0f0/0x40 | 8c485d49121a487fa3b466b9265f35bd30877273886e738a289ed008b98ccb65 |

## Original initialization and setup

`envInit` clears803e7278/0xf0 and803e7368/0xf0 in order using original memset,
then clears direct palette8041ec90 and readiness8041ec94. It allocates and loads
nothing. The loaded-DOL direct-call census finds it at marioStInit8006fe24.
This census does not exclude REL or indirect callers.

`envTexSetup` clears only the palette, preserving readiness history. It calls
arcOpen800782e4 with original name802f9c78 (`env.tpl`) and two zero operands,
compares the returned value, publishes that exact value and branches using the
captured comparison. An archive hit unpacks the retained return rather than a
fresh global palette, then writes readiness1.

On a miss it obtains the actual DVD root80007388 and privately formats
`%s/env.tpl` from802f9c80. There is no language selector. It opens type2 with
zero final operand, gets and captures logical length, allocates heap0 using
wrapping(length+31)&~31, publishes the allocation and stores the handle at
handle+6c. ReadAsync reloads the direct palette but uses the captured LOGICAL
length, not rounded allocation length. It queues callback8019e0f0 and returns
without waiting for readiness. Null source values are real results; native
unavailability must remain a distinct hold.

The callback ignores its first argument. It captures the handle at
secondArgument+2c FIRST, then reads direct palette8041ec90, unpacks, closes the
captured handle and finally stores readiness1. This ordering differs from the
primary/N64 effect callbacks' initial manager-pointer read.

## Resources and actual activation boundary

Original root `env.tpl` is37248 bytes,11 TPL descriptors, SHA256
365bce8eb8114c72636429a3cf32910594fa3ffdcdd8c84810343b3cba967744,
ISO offset947359696. New literals are802f9c78/8 and802f9c80/12. Original BSS
is8041ec90/8 and803e7278/0x1e0. The latter is adjacent to the shared file
filename buffer ending at803e7278; it does not overlap that owner. Do not reset
or reimport filename history when adding environment storage.

The direct texture caller is seq_logoExit80087488, immediately before effect
texture setup8008748c and marioPoseInit80087490. Original names at earlier
80087480 and80087484 are fadeTexSetup80013aa4 and iconTexSetup8001b694,
respectively; these calls do not imply whole fadeInit/iconInit/envInit.

These bodies/resources are documented but not yet implemented/imported by the
native effect texture work. A future shared-VM owner must preserve the single
palette clear, distinct initialization, logical read length and handle-first
callback. Include a nonmultiple32 logical-length fixture: the original asset's
alignment alone cannot detect an incorrect rounded DVD read. Existing archive,
root, finite path, DVD, source heap, memset and whole unpack providers cover the
bounded child identities; no duplicate unknown-function request is needed.
Environment movement, GX rendering and full original sequence activation remain
outside this contract. Private source evidence is in artifacts/effect-primary/
environment-frontier.txt and environment-evidence.json. No texture bytes or
new redistribution grant are introduced here.

See [native effect textures](../effect/NATIVE_EFFECT_TEXTURE_SETUP.md) and
[pose material provenance](../partner/FIELD_MARIO_POSE_HANDLES_AND_MATERIAL_PROVENANCE.md).
