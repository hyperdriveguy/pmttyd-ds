# Native retained shadow profile and target tracking

Implements the conditional profile update in the recovered
[class5 suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md), after independent
collision preparation and before GXInvalidateTexAll802b7b54.

`ShadowWork.profileIndex` represents primary word8; its default1 selects the
tracking orthographic row. It is not an enable boolean. Both mutable72-byte
profiles retain recovered scalar startup settings, including opaque word3c
and raw float44 bits. The four unsigned size entries retain400/256/128/64.
These are engine initialization values; no ISO/image binary is embedded or
committed. Startup-table hash and original addresses are in the source contract.
Main resets requests/counters/flags independently of these mutable settings.
The native semantic owner does not reproduce original packed primary offsets.

Camera2 class5 captures the selected row and exact word0==1 decision after
collision. A held camGetPtr(4) preserves both; changing profileIndex or tracking
selector during that wait cannot redirect the admitted update. Its returned
camera4 target supplies eyeX, rounded float(1000+targetY), eyeZ in store order;
up becomes(+0,+0,-1), then targetXYZ are read again for profile.at. Camera4
**eye** does not supply these values. Native TrioAttack binds indexed camera
queries to its retained CameraManager, independently of current camera2.
After completion, repeated GPU waits preserve this update and prepared records.

ProfileIndex outside the owned two-row domain remains unavailable, with no
clamp or invented source fallback. Other original profile/table writers and
primary pointer replacement remain unbound. The next provider is actual
GXInvalidateTexAll; no texture-cache invalidation, viewport readback,400-square
setup, matrix/image publication or callback completion is claimed here.
Empty/type2-only pools still require the unconditional source GPU operations,
as recovered in [projection and counter admission](SHADOW_PROJECTION_PREFIX_AND_ZERO_COUNTER_ADMISSION.md).

Host regressions cover target versus eye, held row/decision, nontracking row,
invalid index, no replay, Main retention, and actual TrioAttack camera binding.
Native compilation is separate from emulator/hardware or original execution
fidelity; no new shadow image or battle-to-field return is established.
