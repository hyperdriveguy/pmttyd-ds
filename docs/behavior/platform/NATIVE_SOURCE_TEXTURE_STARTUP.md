# Shared original native texture startup

`NativeSourceTextureStartup` shares one implementation of actual-VM heap/DVD
ownership between the original effect primary/N64 and environment startup
profiles. `NativeEffectTextures` remains an alias preserving the established
effect API; create selects the effect profile, createEnvironment the environment
profile. Source cursors stay separate and preserve each original driver's
initialization, path, size, live reads and callback order. This is process startup,
not a Crump-specific sprite loader or a per-area duplicate texture bank.

## Shared ownership and transfers

Both profiles retain the actual NativeUiStartup/field-animation process, source
SDK heap0 and actual VM registries. Cold admission pins exact original new
initialized/BSS ranges and refuses occupied mappings; borrowed UI/root/manager
state is never reimported/reset. Each profile's nonexecuting identity facade
checks actual backing, including DVD entries and allocated palette heaps. Source
state may change legitimately; byte-equal replacement of the owner is rejected.
Successful RAM remains source process history even after native facade teardown.

Each admitted direct-DVD job owns an open ROMFS file and a checked-unmapped
152-byte VM-owned entry at a native backend token. Source open/length/heap calls,
ReadAsync stores and separate flag reloads, progress/remaining/destination,
userdata, callback and close history stay retained. Source and heap pointers are
actual backed addresses. Native entry placement, finite path formatting, file IO
and cooperative byte budgeting are explicit backend policies, not recovered
GameCube heap placement or DVD thread timing.

Allocation bytes and readBytes are distinct. The environment reads original
logical length even when its allocation rounds to32; effect drivers request
rounded reads. A pump delivers at most its actual byte budget, uses a synchronous
raw-memory borrow for word/byte copies and runs whole original completion only
after verified transfer. No raw pointer escapes, callback runs or yield occurs
inside that borrow. Original texture hashes admit the supported source domain;
unsupported files/providers hold rather than receiving synthetic source results.

Native readiness requires completed retained jobs AND actual original readiness
words. Source readiness alone cannot retire pending IO. Busy independently tracks
held source cursors and queued jobs, so Game's existing process retention/restore
guards see actual pending work. Callers must retain the owner through callbacks;
teardown does not acknowledge source cancellation/free or queued GPU consumers.

## Distinct original profiles

The [effect profile](../effect/NATIVE_EFFECT_TEXTURE_SETUP.md) borrows the actual
effect manager/N64 state/prefixes/root/archive owners, imports five new literal
ranges and runs complete primary plus mandatory N64 setup. Their callbacks have
manager-pointer-before-handle capture ordering. Both jobs must complete.

The [environment profile](../display/SOURCE_ENVIRONMENT_TEXTURE_STARTUP.md)
imports four new literal/BSS ranges and borrows root at creation, allowing actual
envInit before Window's archive mapping. Begin later pins the actual shared
archive table without overwriting it. Init clears two work banks and direct
palette/readiness. Texture setup clears only palette, uses no language selector
and reads logical length. Its callback captures handle before direct palette.
Only its one genuine job is required for readiness.

The native archive adapter supports actual four-inactive-descriptor exhaustion;
active descriptors hold for their whole archive provider. Exact original source
paths are authoritative. No forced language fallback, shared-filename mutation,
private execution VM, fabricated allocator, callback receipt or placeholder art
is introduced. These process resources can be retained by later areas through
the existing Game effect-process owner instead of copying per-room banks.

Normal source sequence activation, actual effect/environment render/audio queues,
source math/pixel fidelity and physical New2DS XL performance remain separate
acceptance work. The common transfer implementation adds no per-frame renderer
work or performance claim.
