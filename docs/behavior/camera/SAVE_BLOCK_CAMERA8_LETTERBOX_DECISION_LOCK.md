# Save-block camera8 flag0x200: automatic letterbox decision lock

US G8ME01 disc0 revision0;2026-10-13. Static read-only DOL evidence, no
original execution. Complements
[save callback](../field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md).

The save callback obtains camGetPtr(8), ORs0x200 into its flags HALFWORD+0
at80066fd4..80066fe0. Completion obtains camera8 again and clears exactly
that bit at800670a0..800670b0. No camera XYZ/mode/target request occurs in
those two local operations.

## Actual consumer and meaning

Complete `camLetterBox`8000e388/0x1dc directly loads
camPtrTbl803c95f8+0x20 (camera8), then at8000e3b8 tests flags0x200.
When set, branch8000e3bc goes directly to8000e464, skipping ALL automatic
letterbox enable/disable decisions8000e3c0..8000e460. This preserves existing
camera8 flag0x100, which owns enabled letterbox state. It does not force0x100
on, force it off, freeze player follow or stop the remaining letterbox work.

When0x200 clear, automatic decisions consult window/sequence/Mario/partner
services. The enable route sets0x100, makes the source icon/service request,
initializes alpha+0x1e0=255, top/bottom amounts+0x1e2/+0x1e4=0 and draw-
camera word+0x1e8=8. The disable route clears0x100 except its sequence3
exception. Exact global predicate/services remain separate scene owners;
this document's recovered save-specific effect is bypassing those decisions.

After the0x200 bypass BOTH enabled/disabled remaining routes still run:
if0x100 set, alpha255 and signed bars grow by3 to top49/bottom44; otherwise
alpha decrements by2*(255/GPword+4) and clamps below0. Separate flag0x400
requests immediate cap/zero handling. The helper then always queues
camLetterBoxDraw8000e564 via dispEntry, using stored cameraID+0x1e8 and
renderclass3. A0x200 save lock does not skip that draw request or substitute
its own scene-camera pose.

`camDraw` calls camLetterBox before display sorting at8000f46c. Source
camInit separately constructs camera8 through camEntryPersp and stores its
pointer at table+0x20 at80010538, with name literal `2d` at8041f73c.
Camera4 is a separate construction stored at table+0x10 at80010418, name
`3d`. The camera table is BSS/runtime publication, not a serialized array that
can safely be read as initialized pointers from disc.

## Native mapping boundary

Current native Game.fieldCameraFlags is consumed by field follow/road/shift
services corresponding to camera4. Its display flags track Mario camera
publication, also a distinct owner. Source camera8 bit0x200 must not be mapped
into either of those words as a guessed field-camera freeze. Preserve a separate
camera8/letterbox decision-lock owner; before the actual native letterbox UI
exists, recording this source request is a scoped unbound service, not evidence
that camera-follow behavior has been implemented. Drawing adapted 3DS bars and
source automatic predicates remain native/backend tasks.

## Validation

Fresh read-only complete camLetterBox,camDraw,camInit,camMain and cam3dMain
listings identify the exact0x200 consumer and camera8/4 publications; original
2d/3d literals were reread from DOL. Listings remain /tmp/sb-cam*.elf/.txt and
/tmp/sb-letterbox.*, not source history. No original payload/instructions are
redistributed. Docs-only: no make/build, Dolphin or hardware claim. Original
letterbox/icon/global predicate/camera render behavior and native UI remain open.
