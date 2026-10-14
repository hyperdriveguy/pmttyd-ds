# Stage spotlight work initialization and Main admission

US G8ME01 disc0 revision0, 2026-10-13 read-only DOL evidence. Next used
StageMain provider after the [stage object service](SETUP4_STAGE_OBJECT_CACHE_AND_NAMED_MAP_REQUESTS.md).
Closes original spotlight work/record allocation, entry, Main and lifetime.
Active drawing's numerical/GX backend remains separate; no guessed pointer0.

## Global owner: separate from BattleWork

Original small-data pointer wp804175a0 has value8041ebdc, addressing12-byte
SBSS symbolwork in battle_stage.o. BatSpotGetPtr801381a0/8 returns thatpointer.
It is not B180xx or StageWorkW=B163fc. Recordbank is allocated mapheap memory:

| Work offset / width | Used source role |
| --- | --- |
| 00 signedWORD | Record capacity/count |
| 04 WORD pointer | Allocated recordbank |
| 08 signedWORD | Display admission gate, retained across Init |

Fresh boot SBSS initialization supplies work8=0; batSpotInit itself DOES NOT
write work8. A source reentry after another event changed it must retain it.
Do not bind this owner by assuming globalpointer/recordpointer null.

## Exact allocator/entry order and five records

batSpotInit80138068/138:

1. Read wp; store work0=5 at8013809c.
2. Read wp AGAIN; read its capacity; call BattleAlloc800f8934(count*2c)=dc
   at801380b0 (220decimal payload bytes).
3. Read wp AGAIN; store returnedbankpointer work4 at801380bc.
4. Read wp AGAIN; reread count/pointer; memset(bank,0,count*2c) at801380d0.
5. Five batSpotEntry80137a88 calls at801380e8, with per-record values overwritten
   after each return. No prior bankfree or allocation-failure guard locally.

Entry rereads wp and capacity/bank, scans fromslot0 for first record U16flags0
bit1 CLEAR. It preserves other flagbits and writes defaults from original
802f37a8 table, then ORs flagsbit1 at80137b14. It returns the recordpointer;
there is no exhaustion check after reaching count. Valid Init's fivecalls
follow a clearedbank and fit exactly. Entry does not increment work8.

Init overrides each entry in order:

| Record field | Original initialized values |
| --- | --- |
| U16+00 flags | 1 (Entry OR1 after bankmemset; bit2 remains clear) |
| FLOAT04/08/0c position | (-100+50*i,0,-50), i0..4 |
| FLOAT10/14/18 rotationXYZ | (0,0,0) degrees |
| FLOAT1c/20/24 scaleXYZ | (20,20,150) |
| WORD28 colourRGBA | ffffffff |

PositionX uses source singleFMA(50,float(i),-100) at8013811c, not a lookup or
source crowd position. Other triples copied as rawWORDS fromtable+30/+3c/+48.
Literal50 at80422b1c,-100 at80422b18, colourffffffff at80422ad4. Entry's own
colour is WORD804295d4 before Init overwrites it; those two writes are distinct.
Record stride2c, bankextentdc. Unlisted bytes (HALF02) remain bankmemsetzero.

## Main used fresh domain and actual active-writer evidence

batSpotMain8013800c/5c reads wp, then signed work0. If positive, its bdnz loop
runs that many times WITHOUT any recordread, update or callback. It then reads
signed work8; <=0 returns. >0 registers dispEntry80010bc4(camera6,class0,
callbackbatSpotDisp80137d20,user0,priority-1000FLOAT80422b14). Registration is
not drawing itself and no activebankrecord is tested before it.

A census of all loaded DOL wp-relative loads (r13+a680) and direct calls to
BatSpotGetPtr identifies Entry, Disp, freehelper, Main, Init, getter and exactly
three level-up helpers: _lvup_spot_off80213844, _lvup_spot_move8021396c,
_lvup_spot_on80213f38. The On helper writes work8=1 at80213f68. This census
does not exclude REL/indirect accesses; it is a scoped original caller fact.
The ordinary no-level-up10EXP graph bypasses those level-up helpers. Combined
with actual fresh bootwork8=0 and no admitted laterwriter, it provides the
bounded inactive Main domain WITHOUT erasing the real5 allocatedrecords.
Source Init does not reestablish that domain after a prior level-up.

Native may own a real12-byte persistent work record and originaldc-byte bank,
run this exact allocation/entry/Main branch, and retain work8=0 only from its
actual source bootstrap/history. A successful nonexistent/globalpointer0 stub
cannot substitute for it. Positivegate/history remains a renderer dependency.

## Active display: procedural geometry, no texture asset

Complete batSpotDisp80137d20/2c4 sets vertexXYZ/RGBA direct formats,
GXSetNumTexGens(0), oneTEV/onecolourchannel and then walks capacityrecords.
A record draws ONLY when U16flags has BOTH bits1 and2. Source Init supplies1,
so merely changing work8 positive does not fabricate visiblecones. For admitted
records it invokes drawCone80137b1c three times with distinct GX passes:

| Pass | Cull | AlphaUpdate / ColorUpdate | ZMode(enable,compare,write) | Blend(type,src,dst,logic) |
| --- | --- | --- | --- | --- |
| 1 | 2 | 1 /0 | (1,3,0) | (0,4,5,0) |
| 2 | 1 | 1 /0 | (1,3,0) | (0,4,5,0) |
| 3 | 2 | 0 /1 | (1,3,0) | (1,6,7,0) |

Pass1 uses vertexalpha; pass2 sets materialcolour WORD804295d0;
pass3 uses recordWORD28 materialcolour. GXSetChanCtrl colourchannel4 inputs
also differ; complete GX setup/listings are retained, and equivalentCitro3D
alpha/depth/cull composition is not established by this table alone. There is
no external TPL/icon/model/file request in Disp/drawCone, so inventing a
spotlight texturebank is unnecessary. Its proceduralvolume is distinct from
hidden bti mapnode namedspotlight.

drawCone takes(cameraID,recordpointer), requests camGetPtr(cameraID), builds:
Camera.matrix11c * Translate(record04xyz) * Rz * Ry * Rx * Scale(record1cxyz).
Angles use source degree-radian literal80422b08. It uploads to matrixindex0,
then GXBegin(TRIANGLE_FAN a0,format0,count32). Centervertex(0,0,0) has blackRGB
and alpha recordBYTE2b. Thirty-one perimetervertices: for i0..29 angle
single(single(6.2831854820251465 * float(-i))/30); i30 explicitlyangle0 toclose.
Position=(singlecos,singlesin,-1). PerimeterblackRGB with integer alpha/3.
Source sin/cos and FP rounding remain numericalproviders, not measured native
pixel parity. NoUV/texture is emitted. Matrix/camera GX state and framebuffer
alpha pass ownership are required before rendering an activecone faithfully.

## Free and preserved lifetime fields

unk_80137fe4/28 reads wp, work4 and calls BattleFree800f8908; it does not clear
pointer, count, displaygate or bankbytes. BattleStageEnd801390e4/6c calls that
freehelper at80139100 FIRST, then separately frees/clears up to12 StageWork
scratch pointers. The originalmap allocator owns actual dc bankfree; freed
bytes/pointer do not imply another valid Main pass afterwards. Subsequent
Init overwrites bankpointer but leaves work8 unless another writer changes it.

StageWorkW+b28=200 is independently initialized by BattleStageInit and used
for escape_wall Y; it is not this spotlightwork+8 admission gate or positionY.

## Evidence / delivery scope

Complete original raw DOL SHA256:

| Body | Address/size(hex) | SHA256 |
| --- | --- | --- |
| batSpotInit | 80138068/138 | 83c505917c70536a9cf15978489fae9b16d50e4283ca1102567720df79e9c347 |
| batSpotMain | 8013800c/5c | 77dc679a036faccbf36ef87f0d47a03eb91a25b9298e4282d95e4fd51a060f65 |
| batSpotEntry | 80137a88/94 | 37526fcda4e6a9438e9a4f826d293522f454888bc04ee712bf5e3cf0f390bcbd |
| batSpotDisp | 80137d20/2c4 | d7e1d126ec5bdd59f2ad8671ad03833542c233daf95d6a29e8f099b1a4ba714b |
| drawCone | 80137b1c/204 | 0cab14f7f6f78bb512a0284ef786d52b6a2ea8b68817c23cf7d3b9f102b3c5b0 |
| freehelper | 80137fe4/28 | 15f806e48424f713059dd04a946322c13bfc7615c7a94ac1daae08899a342443 |

Ignored complete listings under artifacts/recovered-temp-evidence/spot-used-* and
ending-top4-batSpot*. Local init/allocator/record/Main/teardown order and scoped
DOL owner/caller census checked. Documentation only; no native/shared docs
edits, build rerun or commit. Scoped diff check passed. Positive drawgate/
level-up/live GX camera-alpha backend remains separate from fresh inactiveMain.
