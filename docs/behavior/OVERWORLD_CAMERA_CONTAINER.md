# Original camera-road container parsing

US G8ME01 revision0, read-only m/gor_00–04/c; recovered2026-10-12.
Verify `python3 tools/verify_overworld_camera.py`: complete owner fingerprints,
source relocation/version/dereference anchors, all five container hashes,
header words, lens pointees, main collection spans and 44 local path ranges.
Read [lifecycle](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md) and
[state/zone consumers](OVERWORLD_CAMERA_ROAD_STATE.md).

Native extraction may preserve each authentic raw container in an ignored asset
pack, retaining all records/raw fields for later owners. Do not commit extracted
camera assets or replace them with authored native clamp tables. The layout below
is big-endian source; portable field decoding/bounds validation is an adapter,
not a claim the original loader validates these ranges.

## Header and raw file offsets

Raw header occupies0x10c bytes. Word+0 equals complete file byte length in all
five shipped files. Bytes+4..43 contain the NUL-terminated export identifier
MarioSt_CameraRoadExport; +44..83 version; +84..c3 date. All five versions are
exactly ASCII `1.01`. These are fixed64-byte header string spans.

| Header field | Shipped collection/range role | Stride / pointer field |
| --- | --- | --- |
| +c4 | Literal1 in all five; single header metadata triplet present | Pointee at+e8,12bytes; no runtime count-loop established |
| +c8 | Road record count | 184bytes / +ec |
| +cc | Zone descriptor count | 104bytes / +f0 |
| +d0 | Polygon descriptor count | 8bytes / +f4 |
| +d4 | Path-vector count (both point and tangent/parameter rows) | 12bytes / +f8 |
| +d8 | Zone vertex count | 12bytes / +fc |
| +dc | Polygon index word count | 4bytes / +100 |
| +e0 | Extra curve collection count, zero in these five files | +104; source extra-curve indexing uses104bytes |
| +e4 | Opaque absent collection count, zero in these five files | +108; stride/consumer not closed |

These counts match the shipped contiguous collection spans exactly. The main
consumer directly uses record-local counts and signed32 loop comparisons; do not
turn source unchecked indices into a claimed generic validated file format.
Field+c4's general semantic role beyond the shipped single metadata triplet is
unclosed. Preserve it, and retain +e4/+108 opaque until their actual owner exists.

Every raw pointer word at+e8,+ec,+f0,+f4,+f8,+fc,+100,+104,+108 is an encoded
file-base-relative value before runtime relocation. There is no map-model
32-byte bias. **+e8 is a pointer word, not the beginning of inline lens floats.**
In all five raw files +e8=0x10c, and the three IEEE32 floats at file0x10c are
(25,.01,1000). calcCurrentCamState8000df54–70 loads the container pointer,
loads WORD(container+e8), then reads FLOAT(pointee+0/+4/+8). The state contract
explains why returning those floats does not establish writes to camera4's lens.

| Room | Bytes / road / zone / polygon / path-vector / zone-vertex / index counts |
| --- | --- |
| gor_00 | 2048 /3 /3 /7 /36 /24 /32 |
| gor_01 | 7700 /11 /11 /45 /132 /127 /196 |
| gor_02 | 11316 /14 /14 /101 /168 /209 /418 |
| gor_03 | 11392 /14 /24 /71 /168 /191 /291 |
| gor_04 | 1304 /2 /2 /2 /24 /9 /9 |

## Actual runtime relocation and absent collections

camRoadSetup8000e104 first clears its0x210-byte runtime work, loads the original
camera file using source archive/DVD services, and retains allocated container
baseB in work+0. It compares **only** unsigned WORD(B+e8) against B at8000e284–8c:
if that first pointer is below B, it adds B to **all nine pointer words** listed
above, unconditionally, with32-bit wrap (8000e290–334). Otherwise it skips all
nine relocations. This is not a per-pointer threshold, a null-pointer check, or
an index-count loop. Portable readers should keep raw offsets and resolve them
against their own immutable buffer instead of reproducing original VA mutation.

The two absent fields raw+104/+108 both contain0xffbf3a10, notzero, in all five
files; their counts+e0/+e4 arezero. Source relocation still adds B to those raw
values under its first-pointer gate. Do not dereference the sentinel as a raw
file offset, infer a valid extra collection from its nonzero value, or silently
convert it into a guessed pointer convention. Preserve raw words and use the
actual zero count/local admission to leave absent collections unused. Main
collections and the required header pointee are nonzero/in-range in this slice;
missing main/lens pointers do not acquire a source-defined zero default.

After relocation, source strcmp(B+44,"1.01") writes runtime version101 on exact
match, otherwise100 (8000e338–36c). It does not parse arbitrary numeric version
text. That runtime number controls later record flag gates and Y overrides.

## Road points and tangent/parameter rows

Each road record is184bytes. Its first32 bytes precede the interpreted flag+20;
all shipped road names NUL-terminate within those32 bytes. Preserve exact full
strings such as `|MarioSt_CameraRoad|C_mae`, plus record index/offset. Names can
repeat in one room; source pointer identity is the record position, so a lookup
that merges records by string changes selection. Remaining raw words must stay
available even before every flag is interpreted.

For mode0 collisionCurve (8000c548–590):

- Record+a0 is a signed32 vector index into container+f8.
- Record+a4 is the **combined** row count. Source computes H=truncSigned(count/2)
  using sign-aware integer division. The first H rows are pointXYZ. The next H
  rows are `(tangentX,parameterU,tangentZ)`, not positions or surface normals.
- Points base = pathVectorCollection +12*start. Tangent/parameter base =
  pathVectorCollection +12*(start+H). Source loops consecutive segments H-1.
  Do not divide the raw collection's overall count by2 or split it globally;
  each road defines its own slice. All44 shipped local counts are12 (H=6).
- Road+40/+44 are the start/end indices used for cap rows, both2/3 throughout
  the current five-room records. Record+48/+4c are side-distance caps;
  +84 controls end-cap behavior. These are solver inputs, not universally valid
  room bounds or a reason to ignore intermediate path rows.
- Road+a8/+ac index/count the104-byte zone collection. Road+b0/+b4 index/count
  extra curve records at header+104. Their source indexing stride is104;
  their general admission/geometry is separate from the main path owner.

In mode1, collisionCurve takes a104-byte descriptor instead of a road record,
reads its+60/+64 as start/combined count, and still takes vector data from the
**regular +f8 collection** (8000c598–5e0). It does not simply redirect to the
opaque header+108 collection. Preserve that mode's actual caller/type and counts.

## Zone and polygon indexing

A104-byte zone descriptor has vertex-start index+58 into container+fc,
polygon-start index+60 into container+f4, and polygon count+64. The selector
adds12*vertexStart to the vertex base, then iterates the indexed8-byte polygon
descriptors. Each polygon holds a32-bit start index and count into header+100.
Each index is a32-bit word selecting a vertex **relative to that zone's vertex
base**, not an absolute global vertex number. Fan order uses first vertex fixed
and successive neighboring vertices, under signed32 count loops. Zone+5c's
local span metadata is preserved; the inspected selector does not use it as a
runtime bounds-validation gate.

Word and float interpretation must follow the owner for each field. Index
multiplication/addition is original32-bit address arithmetic; host overflow and
range checks are portable safety adaptations. Preserve source float words and
units through extraction, then convert authored coordinates to engine/native
coordinates only at their documented service boundaries.
