# Native shared original camera CPU bootstrap

Normal Game runs complete original camInit8000fd9c/ac8 and camEntryPersp8000fac8/
2d4 in the existing animation VM, after UI/effects/texture startup and before
Mario entries. The fixed continuation also contains complete original
C_MTXPerspective8029e7e8/d0, C_MTXOrtho8029e8b8/98 and the8-byte render-mode
getter8002eb30. A separate explicitly bounded DEMO selected-copy contribution
contains8002ed34..edb4. The verifier pins996 original instructions: five complete
functions and one bounded contribution, not six complete source functions.

Native libctru/Citro graphics boot supplies the platform. Its declared US logical
render policy chooses original interlaced descriptor8030c2a8; progressive8030c2e4
is also supported/tested. Both original60-byte inputs are unchanged assets.
The CPU contribution executes original paired loads/stores into rmodeobj803c9f58
and publishes rmode8041e7b4 only after all60 bytes. It stops at the original branch
to the common suffix. OS/DVD/VI/PAD initialization, OS reset-code selection,
framebuffer reservations, memInit and GX/VI setup are separate unimplemented
whole-source contributions; no no-op acknowledgment is supplied. Whole DEMOInit
must not rerun its memInit over these live heaps. Native logical608x480 metadata
is independent of the3DS physical screens.

All thirteen0x260-byte records are real shared SDK heap0 allocations, individually
isolated at unchanged guest addresses before borrowed publication. SDK headers
remain in the actual heap and allocated-list membership is checked. The original
source clears, constructor order, callbacks, lens fields, names and table stores
execute. Actual table is803c95f8/34; source slot2 is shadow and slot3 is bg.
The earlier semantic table/prose had these two names reversed. Default vectors,
constant matrices, names, binary32/binary64 literals and SDK constants retain
original bytes; runtime camera records/table/render-mode BSS are never imported.

The dedicated source frame preserves binary64 FPR bits across lfd/stfd, source
frsp/single arithmetic boundaries and paired spills. Integer-to-double bias
subtraction occurs before binary32 narrowing. The source SDK perspective tangent
result remains binary64 until the actual fdivs; early float conversion changes
results and is tested. Libm tangent and reciprocal-square-root estimates are
explicit native numerical providers, not measured Gekko/FPSCR/pixel parity.
Unavailable providers retain the exact child/cursor and do not replay allocations.

A real camera4 slot/record borrow binds the existing FieldCameraRoadOwner in the
same VM, with original road/shift literals and independently owned BSS. Road
requests retain their original source behavior; their pending state prevents
Game room/checkpoint/restore handoff. This availability is not map publication.
Native camera/source stacks at8d020000/8d030000 are32KiB frame policy, not measured
original stack budgets. Ownership teardown preserves foreign replacements.

One cold publication copies actual raw camera values into the existing semantic
CameraManager, using validated static names to avoid dangling raw-name pointers.
The weak publication relationship avoids a process/animation/camera cycle. It
never replays over live state, even after a target expires or is explicitly reset.
Raw view/extra matrices remain their actual zero bytes and viewPublished stays
false: this supplies no LookAt, camMain, road solver, projection-extra or draw
receipt. Room/checkpoint ownership retains the same raw and semantic bank.
Subsequent semantic field camera publication remains an explicit native adapter;
ordinary source camMain and raw/semantic update authority still require composition.

Camera allocation isolation also exposed palette backing moving when a later
arena suballocation was extracted. Original effect/fade/environment/N64 texture
startup now isolates each actual palette before publishing its identity. The
source addresses, SDK metadata, contents and original read/callback order remain;
full Game tests require these texture profiles to stay ready after camera startup.

Next compose same-VM source map loading with this actual camera/road owner and
the retained shadow process. Complete material/GX display-list capture, actual
source camera Main/draw/outer-display epochs, GPU shadows, file movement and
field/battle/return scheduling remain open. G0 must remain0 until whole mapLoad
returns. CPU boot is not full prologue, original numerical/raster parity or
physical New2DS XL performance acceptance.

See [camera boot/Main/draw](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md),
[road loading](NATIVE_SOURCE_CAMERA_ROAD_LOADING.md) and
[shared shadow bootstrap](../display/NATIVE_SHARED_SHADOW_BOOTSTRAP.md).
