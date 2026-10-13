# Native front-end asset lifetime

This is a native resource ownership contract, not a recovered GameCube loader
or a hardware memory/performance equivalence claim. Normal boot has a separate
front-end owner through demonstration, title/files and fresh-file opening.
Field and battle simulation remain paused during that ownership.

`beginFrontend` synchronizes the preceding GPU frame and frees field/plaza,
ordinary battle, trio background and trio stage rooms. Shared shader, font and
base actor resources remain available. A front-end phase owns only its current
raster images, cinematic actor banks and scene map. The storybook is one bounded
phase: its seven original rasters and rune-back texture remain resident while
page capture consumers need earlier images. Market entry and opening part
changes release that phase after GPU synchronization. Title and file selection
share one raster phase. The small original paper geometry stays CPU resident;
its original textures follow the raster cache lifetime.

A scene-map change releases the preceding map before staging its successor.
Failure frees the partial staged room and retains an explicit development load
notice. Original scene bytes, geometry, textures and dialogue are unchanged.
This avoids simultaneously reserving old/new map vertices and textures alongside
field battle rooms in the native linear allocator. Raster/actor texture pointers
remain valid until the submitted GPU work has completed.

Field admission releases the front-end cache, then restores ordinary/trio battle
rooms as a transactional bundle. A partial failure frees all pending rooms and
leaves the front-end ownership flag set for retry. Field collision and checkpoint
restore retain their existing separate transaction. This remains an early slice:
phase-specific loading of all field actor/animation resources and actual New2DS
memory/fragmentation profiling are future work.

The regression was observed in isolated Azahar normal fresh-file boot: title and
file menu worked, but the house/voyage scene repeatedly failed to load, rendered
black and slowed simulation. Original map companions, batch ranges, UV/mesh
headers and texture presence were independently checked and valid. Releasing
field battle rooms and the previous front map restored the house scene and the
fresh-file harbor checkpoint. This is evidence for the scoped native fix, not a
measured original load schedule or complete performance trace.
