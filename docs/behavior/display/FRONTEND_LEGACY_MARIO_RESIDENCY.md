# Frontend legacy Mario texture residency

The native renderer's characters-mesh atlas is1024x1024 RGBA8:4MiB of linear
memory, independent of its compressed file size. Renderer startup/ordinary
legacy draws may have admitted it, but storybook, marketplace, title and file
selection draw no actor from this texture. Marketplace Peach, merchant and
chest retain their own original banks. Book final-adjust overlap retains its
original desktop, cover, current illustration, page back and active capture.
Dropping those visible resources or delaying the chest changes the scene.

beginFrontend synchronizes and retires the unused legacy atlas, alongside the
existing native field Mario retirement. Before FrameBegin, renderFrontend
computes whether the reached House/HouseReturn/MapDisplay or Arrival draw
actually needs legacy Mario and admits its original texture if necessary.
House capture-only map display does not draw Mario. Non-consuming scenes
retire a resident legacy atlas only after synchronization. CPU animation packs
remain available; the original pixels, UVs, poses, story order and dialogue do
not change. Field scene preparation retains its existing independent admission.

House and Arrival drawing explicitly reject an absent legacy atlas. Resource
failure cannot acknowledge a successful scene submission or silently omit
Mario. The normal scene admission/clock receipts still require completed GPU
submission. The native policy trades cold admission at a reached consumer for
bounded GPU residency; physical timing and the complete prologue remain open.

The failure investigated here was an actual demo-chest import failure with327KB
linear space free in a long isolated run after successful constructor fixtures.
The normal40s boot did not reach the failing stage. Matched long uninstrumented
validation and memory evidence are recorded in [progress](../../PROGRESS.md),
separately from original-game observations. The independent source/asset review
found that the required final book overlap already retires inactive pages
correctly; capture storage is VRAM and does not reclaim this linear atlas.
