# Native original map-object head collision

This binds the original mode0/default-object-scale cube described by
[save-block source hit lifecycle](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
and [shared hit registry order](DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md).
It is the collision part of the save-block implementation; the hit callback,
save event and card persistence are separate pending owners.

## Catalog and geometry

PMNPC003 retains PMNPC002 actor fields and appends a32-byte original source
name to each actor row (156 bytes). Model rows are64 bytes:32-byte asset stem,
float UV scale, uint32 hit mode, then six float model minXYZ/maxXYZ bounds.
Mode0 means unbound; mode1 means recovered mobjHitEntry mode0 with default
object scale and original actor position/Y rotation. Other hit modes, pitch/
roll and changed object scale require explicit future binding, not inference
from the model draw scalar. Loading retains PMNPC001/002 support and rejects
invalid modes, nonfinite/degenerate bounds, unsafe source names and collision
models on ordinary NPC records. Failure preserves the prior catalog.

The converter reads model floats at0xd0..0xe4 directly from the read-only
original bank. Native source dimensions use10*abs(minX-maxX),10*abs(maxY),
10*abs(minZ-maxZ); source bbox midpoint offsets retain forcedY0. Original eight
unit vertices/six strips emit12 triangles with alternating original winding,
source cull1 and shared0x80000000 owner attribute. Original source XYZ transform
precedes the single engine-to-native*.01 conversion. Animation deformation and
model draw scale do not alter the cube.

## Head query and lifetime

The existing five-probe upward search evaluates static triangles then registered
object triangles. Current area packs represent one original map record: static
slots precede its appended MOBJ slots. Equal lengths retain the first winner;
the far ray endpoint rejects. Multi-map-record source chain reconstruction
remains pending. Dynamic hits publish a separate catalog owner key; their
GroundContact sourceNode remains0, preventing native FNV IDs from being used as
static REL offsets or door nodes. A near miss publishes no owner. Original
head contact, inclination, quantization and ceiling response remain shared.

Game owns a reusable triangle vector. Queries rebuild the current visible
object subset from room/story/interior/system admission, retaining capacity.
The current plaza object contributes12 triangles; source name `mobj_save` is
available for the later name-based manager cache. No global pose namespace or
room-specific query branch is introduced. Object transforms are currently
static-init data; moving instances need runtime transform ownership.

## Validation and limits

Original-data tests check source bounds/dimensions,12 faces, bottom winding,
owner namespace, actual/near-miss/backface/far-endpoint queries and room/story/
system suppression. A full native Game jump beneath the original cube produces
head contact, clamps root height, falls and lands. Catalog tests retain V1/V2
coverage and add malformed V3 mode/bounds atomicity.

Top support and side walls from this same cube are still unbound. Name-contact
cache, callback stages0..4, nested keys, previous-published pose ratio, original
save event/choices and persistence remain next. Normal head search float math
is an ARM/host adapter; source collision traces and hardware performance remain
unverified. This is not a complete object driver or full-save fidelity claim.
