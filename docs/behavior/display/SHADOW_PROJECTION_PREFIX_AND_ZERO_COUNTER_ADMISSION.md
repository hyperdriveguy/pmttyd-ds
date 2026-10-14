# Shadow projection prefix and zero-counter admission

US G8ME01 disc0 revision0; read-only recovery2026-10-14 for EVT-01.
Companion to [camera2 class5 suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md).
This lists source GX arguments, not equivalent PICA enum numbers.

## Projection8002caf4 captures and ordered unconditional prefix

Entry captures primary cswp as ownerO, its pool pointer and profile row
R=80304a60+72*O.word8, then queries camGetCurPtr8000f2e4 and captures its
view+11c. After that query it freshly loads cswp and captures alpha byte103
as A. This alpha is retained across all prefix calls. Do not merge the initial
owner/profile capture with later global pointer reads.

The following requests execute before any record/count admission:

| Order | Call and exact arguments |
| --- | --- |
| 1 | GXSetNumChans(1),8002cb5c |
| 2 | GXSetChanCtrl(4,0,0,0,0,2,2),8002cb7c |
| 3 | Fresh cswp.word100 captured as RGBA; GXSetChanMatColor(4,capturedRGBA),8002cb94 |
| 4 | GXSetNumTexGens(0),8002cb9c |
| 5 | GXSetNumTevStages(1),8002cba4 |
| 6 | GXSetTevOrder(0,255,255,4),8002cbb8 |
| 7 | GXSetTevColorOp(0,0,0,0,1,0),8002cbd4 |
| 8 | GXSetTevAlphaOp(0,0,0,0,1,0),8002cbf0 |
| 9 | GXSetTevColorIn(0,15,15,15,10),8002cc08 |
| 10 | GXSetTevAlphaIn(0,7,1,5,7),8002cc20 |
| 11 | GXSetFog(0,+0,+0,+0,+0,word80429538),8002cc44 |
| 12 | GXSetBlendMode(1,4,5,0),8002cc58 |
| 13 | GXSetZMode(1,3,0),8002cc68 |
| 14 | GXSetZCompLoc(1),8002cc70 |
| 15 | GXSetAlphaCompare(7,0,0,7,0),8002cc88 |
| 16 | GXSetCurrentMtx(0),8002cc90 |
| 17 | GXSetCullMode(0),8002cc98 |
| 18 | GXSetArray(9,80304b00,12),8002ccac |
| 19 | GXClearVtxDesc(),8002ccb0 |
| 20 | GXSetVtxDesc(9,2),8002ccbc |
| 21 | GXSetVtxAttrFmt(0,9,1,4,0),8002ccd4 |
| 22 | GXSetBlendMode(0,4,5,0),8002cce8 |
| 23 | Fresh GP.word14 query,8002ccec..ccf8; choose color below |
| 24 | GXSetChanMatColor(4,selectedRGBA),8002cd0c or8002cd38 |
| 25 | GXSetZMode(0,3,0),8002cd48 |

Fog word80429538 is retained startup-cleared .sbss2, not on-disc texture
metadata. Treat unknown inherited mutations as unavailable rather than assuming
a white fog color. Source +0 literal is float8041f9d4.

GP.word14 is freshly read **after** the second blend request. If nonzero,
selected RGBA is original word8041f9c4=ffffffff. If zero, start from
word8041f9c8=000000ff and replace RGB bytes with the previously captured A:
RGBA=(A,A,A,255). Thus fresh source alpha110 selects6e6e6eff in this branch;
it does not set draw alpha110. Neither Game.Scene nor a guessed battle bool
proves the GP.word14 value. Its producer/import is a separate provider.

For a retained continuation, each successfully emitted request advances once;
hold at an unavailable request without replaying preceding source requests.
Capture colors/branch values at their actual reads. Material mutations between
request3 and request24 must not replace the previously captured A. Native GPU
state restoration on frame re-entry is distinct from source request replay.

## Record and counter admission after prefix

The primary pool pointer captured at entry advances5c per record. Loop bound
is **fresh global cswp.word104**, signed comparison at8002cdf8, on each test.
Radius==0,type!=3 or flag2clear skips a record. There is no signed record
camera equality gate in this loop. An admitted record concatenates the
captured current camera view with its prepared matrix; loads position matrix0;
emits GXBegin(152,0,6) and six index bytes0,1,2,3,4,5; only afterward
wrap-increments captured ownerO.word4 at8002cddc..cde4.

After scanning, captured O.word4 is read at8002ce00. Zero returns directly
to function epilogue. Nonzero admits the remaining image/shadow compositing
suffix: first C_MTXOrtho8029e8b8 into stack70 from captured R values, then
GXSetProjection(matrix,1) at8002ce34 and subsequent actual GX drawing.
An all-type2 live pool leaves zero only when its entering counter was zero;
this does not suppress the unconditional prefix. Imported/stale nonzero
counters reopen that genuine suffix even when this invocation adds no type3.

## End restoration precedes its counter gate

projShadowEnd8002b834 captures its **passed owner**, then:

1. camGetCurPtr8000f2e4; PSMTXCopy(saved8041e6e0,current.view11c), at8002b85c.
2. GXSetViewport(saved six floats8041e720), at8002b87c.
3. GXSetScissor(saved four words8041e710), at8002b894.
4. Read passedOwner.word4 at8002b898; zero returns, nonzero admits actual
   image allocation/copy/texture/matrix publication.

depthShadowEnd8002b4d8 has the analogous prefix but inserts
GXSetColorUpdate(1) at8002b528 **between view restoration and viewport
restoration**. Its counter read at8002b564 likewise follows all restoration.
Neither End helper restores the projection here. Zero ownerflags are not
an entry predicate for these restores; do not skip them.

## Camera2 classes0/2/7 after a zero class5 counter

Original jump table80304fb0 contains:
0→8002d868,1→return,2→8002d9e0,3→return,4→return,
5→8002d658,6→return,7→8002db6c. Out-of-range class>7 also returns after
the common dispGetCurWork query. No additional ownerflag gate exists in
shadowDisp before any admitted target.

- Class0 captures pswp; GXInvalidateTexAll,viewport/scissor save+set,
  source lookat/projection, current view backup/replacement all execute.
  It returns after that setup; flag/counter0 do not skip it.
- Class2 first calls projShadowEnd(camera2,fresh pswp). Even counter0
  restores view/viewport/scissor. Then it captures dswp, invalidates textures,
  disables color update, saves viewport/scissor, sets square projection and
  replaces current view. Again no zero ownerflag/counter admission skips setup.
- Class7 calls depthShadowEnd(camera2,fresh dswp), whose query/view restore,
  ColorUpdate(1),viewport/scissor restores execute before its counter gate.

Therefore reset primary/auxiliary flags and counters after shadowMain do not
produce genuine no-service class0/2/7 callbacks. Only their admitted image
suffixes can be excluded by actual counter0. A no-op would lose camera view,
viewport/scissor/color state and is a different execution path.

## Source profiles and evidence

Mutable shadowConfig80304a60 is exactly two72-byte rows, source startup values:

| Row | Followword0 | Eye | Up | At | row28,row2c,near,far | Kind | Size index |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 0 | -100,256,335 | 0,1,0 | 0,0,0 | -20,20,356,741 | 0 | 0 |
| 1 | 1 | 135,40,41 | 0,1,0 | 0,0,0 | -240,240,80,10000 | 1 | 0 |

Primary fresh profile1 follows camera4 in class5. Auxiliary fresh profile0
retains row0. ShadowSizeTbl unsignedhalf values are decimal400,256,128,64.
Follow mutation and shared mutable rows must be retained, not recomputed from
native screen size. Profile writers/imports remain separate from this snapshot.

Fresh ISO metadata/listings are under ignored
artifacts/reference/shadow-projection-prefix/ and shadow-class5/. Complete
Projection/678 SHA25645d3da8fc34ce3ec08cb7e01bf75d05b5f96cfbe6794bef0770490b3d34ce3e1;
projShadowEnd/294 d5b52533c33a488880d068d80e68a2e658a770618bbd8a88a10d3328118735ca;
depthShadowEnd/35c43ab8f516923e9765ddf4a9fd7c3aed3a00f84e0f21551f9b05b7b012eadde44.
Table90-byte SHA256b01f90b7b4e7213c09322f2c3319369f4f1c0a4b8b11dd2033163d94dc731c46.
Documentation only; no code/index/shared progress edits or runtime/hardware
validation. Full nonzero image suffix and native GX-to-PICA fidelity remain open.
