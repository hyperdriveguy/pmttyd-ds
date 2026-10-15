# Native source class-menu numeric providers

`BattleClassNativeNumeric` binds concrete existing numeric owners to the original
DrawMainMenu/circle/bubble service tables. It is not a controller, a GX backend,
a scheduler, or permission to activate a class menu. It uses only the actual
admitted `BattleClassNativeFrame::camera()` bridge and its proven successful
CameraMain/Draw/queued-callback epoch. No fixture camera, identity fallback,
zero geometry, or physical-input substitution is installed.

The noncopyable adapter and its actual VM/provider contexts must outlive every
held invocation. Its constructor copies the actual actor services, native-camera
reservation base, optional source-word reader, pure current predicate, and
optional retained party/pinch child providers. Their bindings cannot change.
The real source/table resource owner must retain this context. Party/pinch
callbacks use their own copied user pointer; callers must not overwrite the
draw table's numeric `user`. Missing footers remain null/unavailable. Their
providers retain partial work themselves and the adapter checks current
ownership both before and after invocation.

## Concrete source operations

`draw()` supplies camera lookup, actual ring center/offset and source projection,
existing `cameraInterpolation`, and existing bridge SDK matrix operations:

| Source operation | Function |
| --- | --- |
| Translation | `8029e230` |
| Rotation by radians and source ASCII axis | `8029dff0` |
| Scale | `8029e2b0` |
| Concatenation | `8029dd64` |
| Identity, only when the raster source requests it | `8029dd04` |

The existing adapter preserves its finite native numeric policy and FMA/order;
this does not claim bit equality with PowerPC libm. Interpolation retains mode0/4
and all other existing supported source modes, including count0 returning B
before mode validation. Nonfinite results remain unavailable. Raster truncation
is toward zero only in the finite representable signed32 domain. NaN, infinity,
and overflow remain unavailable; no original exceptional conversion result is
invented.

`raster()` acknowledges only the actual Camera8 request and supplies identity,
truncation and SDK math. It never acknowledges an Init2D/GX/texture/icon/font
request. Those remain the existing real backend/resource owners. All optional
numeric/query calls recheck the current epoch before and after their work.

## Optional memory and asserted raster domain

Public `word(address)` routes the entire configured virtual camera reservation
to the bridge, including undefined/unaligned fields, and never falls through
there to source RAM. Ordinary words use the supplied optional actual VM/GP
reader. Missing reads and reads whose provider invalidates the epoch remain
unavailable. `camera()` and `project()` have the same epoch checks.

The original raster bodies use a total word interface. Their pinned source
queries Camera8, then later copies exactly twelve words at camera+11c. The
adapter admits that entire view snapshot at the actual Camera8 request using
optional bridge reads, and publishes no camera result until all twelve are
known under the same epoch. Their values stay valid because every subsequent
provider checks the unchanged actual camera matrices/queue/Main epoch. Their
source camera-word reads use only this complete typed snapshot.

The private total raster reader admits exactly those aligned twelve virtual
view cells and actual native-VM asserted-domain ordinary reads. It exposes no
stores, byte reader or lifecycle hooks. Undefined virtual fields, camera reads
before source Camera8 admission, or total reads outside a live epoch violate
this private programming contract and abort; they never produce a fabricated
zero or fall through to guest memory. Arbitrary readiness queries must use the
public optional API instead. This is not a resumable frontier for missing total
guest storage. Actual native VM domain admission is still required before
entering the original total-memory source bodies.

## Integration and validation

Construct the adapter with the actual Trio binding's actors, reserved-camera
base, resourceWord and current predicate. Give its `draw()` to
`classMenuFrameBinding(linked)` and its `raster()` to the existing native frame
binding. Retain its original party/pinch child context in the actual shared
resource owner. Source Camera8 captures the bridge view; source footer camera
queries can use `camera()` and `project()`, while common TPL queries use the
optional `word()`. The separate actual surface lease must remain current in the
frame/resource predicate. Font/message-icon lifetime and retained footer/GX
routing remain separate requirements. There is no new production callback,
normal activation, renderer edit or lower-screen UI change in this module.

Focused tests execute actual CameraManager Main/Draw and queued callback through
the native frame. They compare source interpolation and SDK matrices with their
existing implementations; cover all twelve admitted camera words, undefined
reserved words without guest fallback, actual VM word/half reads, unsupported
GX/camera requests, finite truncation boundaries, projection/ring providers,
and producer invalidation from an optional source read. Original radial and
raster body pins are rerun read-only. Host/ARM/sanitizer checks use a copied
consistent pre-surface-lease header/source graph and the matching frozen
`original-menu-validation/source` archive. Only the new translation units are
sanitizer-instrumented. Whole-project validation with the changed backend ABI
is required separately by the integrating owner.
