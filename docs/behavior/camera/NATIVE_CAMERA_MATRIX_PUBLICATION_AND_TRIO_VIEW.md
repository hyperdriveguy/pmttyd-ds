# Native matrix publication and trio view

The native depth adapter now runs camera matrix publication in the recovered
order: Z-roll times LookAt, positive translation times that result, then the
optional extra times projection. Both view multiplications execute for zero
roll/offset. Each product/store and subsequent fused multiply-add follows the
[US publication arithmetic](../display/BATTLE_CAMERA4_MATRIX_PUBLICATION_ARITHMETIC.md).
`CameraMatrixPublication` retains roll, translation, flags and extra independently
of its supplied snapshot. Flags mask8 clears after multiplication; other flags
and the stored extra survive. Draw-disabled bit1 does not inhibit this helper.
It does not execute callbacks, field zoom, camera-bank traversal or draw setup.

Perspective construction now retains the two single angle conversions, double
tangent/inverse before narrowing, and the single far*near product before the
reciprocal multiplication. Finite/degenerate admission, LookAt normalization,
native double trig and ARM FMA remain declared numerical adapters. They do not
reproduce Gekko reciprocal-square-root seeds, paired-single/FPSCR/GQR behavior or
original transcendental results. The upright depth adapter explicitly supplies
zero roll/offset and no extra request; complete field-camera history is still
unavailable and is not certified by these primitives.

While the trio presentation is active and its depth adapter is available,
Renderer uses the retained battle-controller eye/target/up. Engine positions
scale by .01 into the native renderer. Its two world actor-facing queries use
the same selected camera rather than the old field eye. Lens values remain the
retained native field frame, with handheld aspect/tilt through citro3d. This is
an interim presentation binding; it does not consume queued part/material/shadow
callbacks or publish completion of camMain8000f704. The existing finishing wait
remains in place. Other cameras retain their current renderer adapters.

Portable tests cover noncommuting roll/translation, draw-disabled publication,
extra multiplication and one-use clearing/rearming, preservation of unrelated
flags/extra storage, and perspective product ordering. See
[camera-manager source evidence](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md)
for the thirteen records and missing ordered draw services. Emulator acceptance
and hardware fidelity must be reported separately in PROGRESS.md.
