# Original position, billboard and scale icon services

US G8ME01 disc0 revision0. iconDispGx8001a918/24 supplies alpha255 and calls
iconDispGxAlpha8001a93c/2b4. Read-only complete listings, constants and hashes:
artifacts/stock-exp-display-evidence/. The alpha255 wrapper SHA is
140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d.
These are distinct from [matrix icon services](ORIGINAL_MATRIX_ICON_SERVICES.md);
merely calling that wrapper omits billboard/scale or duplicates the view multiply.

Capture raw flags, requested ID, alpha, scale and position-pointer identity;
call current-camera8000f2e4, retaining its camera. Read icon.bin8041e638, returning
if genuine0. Only if nonzero read icon.tpl8041e63c, returning if genuine0. An
unavailable provider holds and does not manufacture a loaded-buffer null.

Read caller XYZ at the translation call after those gates. Build T via
PSMTXTrans8029e230, uniform S via PSMTXScale8029e2b0 using captured scale. Then
freshly read retained camera yaw114, negate it, multiply by original radians
constant8041f8a0 (word3c8efa35), and request Y rotation8029dff0(axis121).
Concat T*Ry, then that*S, then retained camera's freshly read view11c*result.
Each request captures its own input at the original use point and retains it
across holds. Camera pointer remains captured; yaw/view content is not captured
at wrapper entry. Native matrix adapter uses the existing recovered SDK concat
ordering and explicit finite admission; native trig/FPSCR bit parity is not
established. No invented zero-yaw, identity view or additional current-camera
lookup substitutes this graph.

After matrices, read original colorWORD8041f860=ffffff00. Reread icon.bin,
then capture descriptor directory8041e634 EVEN IF that second bin was0. Replace
color low8 with captured alpha, narrow flags and original requested ID to16bits.
If second bin was nonzero, reread icon.tpl. A late bin/TPL0 does NOT gracefully
return: source sets descriptor pointer0, then reads width at ADDRESS2, height4,
texture8,delay10,count6. This is not descriptor ID0 at a valid directory. Native
unmapped memory legitimately holds this path; no empty icon is acknowledged.

With both late resources present, requested low16 IDs73/103 use fresh current
GP.story16c and original party-alias tables80415040/4c. Other IDs skip that query.
Descriptor=directory+U16(directory+2*(selectedLow16ID+1)), using source32bit wrap.
Read descriptor width2,height4,first texture8,first delay10,frame count6 in order.
Original requested ID remains the temporary icon ID even when descriptor aliases.

For count>1, sum all U16 delays in source order, then capture current GP pointer
and its signed clockWORD1c. Signed clock modulo total selects the first frame
whose duration exceeds remaining time; otherwise subtract duration and continue.
Retain source first delay in icon record; selected frame replaces texture only.
Native valid resource totals are positive and within signed32. Zero/nonpositive
sum and unsupported signed-divide domains hold explicitly; no guessed frame or
clock is supplied. Partial provider holds retain already-read metadata, duration
sum, captured GP/clock and per-frame scan progress.

Call original [iconGX800197f8](ORIGINAL_ICON_GX_DRAW_SERVICE.md) with the composed
matrix and initialized temporary icon. Reuse actual palette/frame/GX state.
Stock icon404 has original width/height0, frames418..421 and delay6 each;
iconGX queries the selected40x40 texture dimensions when both record sizes are0.
Stock EXP uses flags16: source flat material/fog-off, unchanged dimensions,
source RGBA modulation and its real source quad. Native FIFO acknowledgement
captures an atomic packet for the original whole-camera-pass publication/replay
boundary; it does not independently establish on-screen pixels when a later
callback still holds. Restore no extra source state that the original omits.

Native source-icons constant pack appends original8041f860/4 and8041f8a0/4 to
the former116-byte payload, giving124 bytes. Existing constant prefixes and all
613 original icon texture cells remain unchanged. Source artifact paths,
conversion choices and private original licensing are in [ASSETS](../../ASSETS.md).
Host tests check live position after current-camera call, captured scale/yaw,
late view, held geometry, low16/low8 inputs, initial-null return and late-null
ADDRESS2 hold using actual original icon404 metadata. Full runtime evidence and
unfinished scope are in [progress](../../PROGRESS.md).
