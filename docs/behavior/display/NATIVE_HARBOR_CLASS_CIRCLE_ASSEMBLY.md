# Native Harbor class circle assembly

The manual native command presentation uses the original common-menu palette
index0. Its40x40 image is one quarter-disc mask. Drawing that whole image once
as a36x36 backing produced the white corner shapes in the native physical probe.
The texture, compiled atlas and editable source pixels are unchanged.

`DrawMainIconCircle801191a8/4a0` copies XY802ef650 and UV802ef6d0, then emits
four black alpha128 shadow quads followed by four gray alpha255 face quads.
The original face gray is the byte of192 minus truncated1.5 times positionZ.
The source icon color uses the same gray. This native flat reflow explicitly
uses source Z0 and angle0: gray192, shadow X4/1.3 and Y-3.076923131942749.
It maps the original diameter40 to the existing native diameter36. Those are
presentation choices within the existing native reflow, not original animated
ring-state or source GX completion receipts. The actual source row enable bit
still selects the existing normal/disabled icon material.

The corrected native plan retains the exact four source XY/UV sequences.
Shadow quarters precede face quarters and the actual icon. The original mask
alpha shapes every quarter; no replacement circle image or generated art is
introduced. Selected bubble art, label strings, physical controls, frame
presentation admission and the lower scene remain on their existing paths.

Focused tests cover all four selections, every quadrant position and mirrored
UV, all shadow/face colors and offsets, icon tint, list/target pages, missing
resources and stale facade/model rejection. Host tests use the consistent
16788 source graph and its terminal archive. The changed menu planner and test
are also sanitizer instrumented; the borrowed unchanged archive is not. The
planner compiles to an ARM native object with the same frozen headers. A later
full native probe must inspect the assembled circles; CPU geometry tests alone
do not prove raster output or original framebuffer parity.

Source pins and native probe receipts remain private artifacts. Original asset
licensing/provenance remains with the existing original common-menu import;
this fix adds no asset or license grant. Parent integration owns the exact
unapplied journal, fresh whole build and progress record.
