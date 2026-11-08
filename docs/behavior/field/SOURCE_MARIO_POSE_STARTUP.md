# Original raw Mario pose startup

Whole US revision0 marioPoseInit8005ba50/0x98 is implemented by
`initializeSourceMarioPoses` and its retained cursor. SHA256:
08c3dce0e69c0c1e23422a3dbdfb3aabe5fc117761965731e76f5c30260f304a.
[Source owner and selector evidence](../partner/FIELD_MARIO_POSE_HANDLES_AND_MATERIAL_PROVENANCE.md)
remains authoritative.

The initial global Mario pointer8041e900 is captured once for all result stores.
The first selector uses captured Mario byte3d. The second and third reread the
global pointer, then signed byte3d, then resource table80415eb0+12*kind+4*index.
Each whole animPoseEntry8004176c(name,2) result is stored unconditionally into
captured Mario+22c/+230/+234, including signed negative results. There is no
prior release, rollback or negative-handle guard. A hold retains each accepted
read, request and store; a completed Entry is never repeated after a held store.

`NativeMarioPoseStartup` borrows an already initialized, fully backed raw
0x2f8-byte Mario owner in the actual NativeFieldAnimation process. It does not
create that owner, run marioInit or convert the typed FieldMarioBattleState
token into a fabricated full source record. It pins the original global/table/
name mappings and captured owner, then each later reached selector record.
Backing replacement invalidates use; the global pointer value may legitimately
change after initial capture. Three ordinary resources resolve through their
actual source name operands and the same pool/VM/SDK heap/file cache's whole
Entry implementation. Unavailable/unsupported resource variants hold. Native
admission currently has the ordinary table row/name range, while the portable
body preserves general signed/wrapping selection without substituting row0.

Portable tests mutate later globals/table/selector bytes, return signed-2, hold
Entry and result stores, and verify exact capture/no replay. Native tests execute
all three genuine resource Entries over a TEST-only actual heap record and
reject equal-byte backing replacement. That test record is explicitly not proof
of whole marioInit or production raw-owner availability.

Normal Game still uses the typed canonical three-entry adapter. Complete original
marioInit8005b5c8/0x488 and its actual pouch/motion/camera/group/party children must
produce the persistent source owner before this wrapper replaces that adapter.
See [Logo startup](../display/SOURCE_LOGO_STARTUP.md) and the
[Crump requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
No source dialogue/event branches or renderer cadence change here.
