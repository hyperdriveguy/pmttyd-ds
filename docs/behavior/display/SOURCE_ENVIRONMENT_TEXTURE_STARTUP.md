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

The complete source Init/TexSetup/callback and original resources are now
implemented in `source_environment_startup.hpp/.cpp` and the shared
`NativeSourceTextureStartup` backend. `NativeEffectProcess` exposes explicit
initializeEnvironment/beginEnvironmentTextures/pumpEnvironmentTextures and
readiness methods on the actual UI/animation VM and SDK heap. Environment Init
can run before Window texture setup: it borrows the existing root and cold-maps
only the four original environment ranges. Texture admission later pins the real
shared archive descriptors, rather than installing a duplicate table.

The two source work clears execute once; importing BSS alone does not acknowledge
Init. Native repeated initialization and setup calls retain their completed
history. Setup preserves actual prior source readiness, while native readiness
also requires the real retained DVD job and complete original callback. Allocation
capacity and logical read length remain distinct in the shared worker. Partial
read budgets deliver only requested bytes; callback userdata, source unpack,
close flags and final readiness execute independently of job admission.

The finite native backend admits actual inactive archive exhaustion and the
original `./env.tpl` direct-DVD path. Active archives hold for a genuine provider.
Original37248-byte env.tpl is tracked with licensing/provenance under
`assets/original/display/environment-startup/`. Normal manifest recipes compile
`environment-startup/memory.pmevt` and copy `environment-startup/env.tpl`.
Imports open the ISO read-only and refuse nonempty destinations.

Portable tests include two held work clears, setup's retained archive return
instead of a fresh global, readiness preservation, a nonmultiple32 read length
(33 logical bytes versus64 allocation), and handle-first callback palette changes.
The actual-VM native fixture covers Init before archive mapping, filename/work
adjacency, once-only history, partial original TPL delivery, whole unpack and
callback, composition with both effect banks and equal-byte descriptor replacement.

Normal Game now executes this startup API after the actual fade owner and
before Effect/N64 setup, draining independent callbacks before native Mario
entries. Complete original source parents and raw Mario/logo-file tail,
environment movement/GX rendering, Crump effect update,
draw/audio/retirement and full battle/result/field-return are still open. Existing
archive/root/path/DVD/heap/memset/unpack children cover this bounded frontier;
no duplicate documentation request is needed. Fresh source evidence and final
validation artifacts are under artifacts/environment-startup/.

See [shared native texture startup](../platform/NATIVE_SOURCE_TEXTURE_STARTUP.md),
[native effect textures](../effect/NATIVE_EFFECT_TEXTURE_SETUP.md) and
[pose material provenance](../partner/FIELD_MARIO_POSE_HANDLES_AND_MATERIAL_PROVENANCE.md).
