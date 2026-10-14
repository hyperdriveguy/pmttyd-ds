# Field party entry camera/resource/animation admission

2026-10-13 US read-only constructor continuation. Complete local leaves and
animPoseEntry8004176c/4ec inspected; ignored listings
`artifacts/recovered-temp-evidence/entry-*`. This is scoped original input/
request/field ownership, not complete animation/file/shape rendering. No native,
shared PROGRESS/index edits or commit. Read
[full field constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md),
[camera ID leaves/callers](../camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md)
and [appearance2](FIELD_PARTNER_APPEAR_POSITION2.md).

## Mario camera ID exact owner and initialization

marioGetCamId801a94f0/24 calls marioGetPtr then reads raw WORDM134.
marioSetCamId801a94c0/30 retains supplied rawID BEFORE pointer query, then
stores returnedowner134; no range/context/camera-object validation.
marioInitCamId801a9514/30 retains literal4 BEFOREquery and stores134.
Fresh marioInit's fullmemset initially134=0, then call8005b628→InitCamId writes4.
There is no later DIRECT134 store in marioInit; transitive camera helpers and
later authored SetCamId callers retain their distinct bounded domains. ReInit
requests GetCamId before clearing the same2f8-byte record, then restores134
at8005b2ec. It does not always reset4 on re-entry. The linked camera contract
owns its direct caller census; setting the rendered viewport is not automatically
an original M134 write.

## EntryMain fields through original pose allocation

partyEntryMain800be144 retains suppliedkind, requests fieldMario pointer,
finds firstnull slot0/1, allocatesheap0/188, publishes and memsets188, then
looks it up and memsets188 AGAIN. Bothoccupied returns-1 before construction;
allocation-null has no guard. Aftersecondzero, in order:

* marioGetCamId→P164 (actualreturnedrawID, notalways4).
* WordsPc/P10/P14=-1; P0=1; P2f=slot; P160=retainedMario; P31=low8kind.
* SignedM247==0 copiesP31 toM247. Slot1 ORsP0mask4.
* Publish kindrecord803143e8+40*FULLinputkind at8041ead4.
* M0mask80000000 set propagates toP4mask8000, then resource selection.

Ordinarykind1/P48000clear uses record0 at80314410→string804213e0 `c_pkr`.
P48000set uses fixed802cb340 **`d_mario`**, not dot-Goombella guessed bank.
Kind4 color selection is another branch; do not apply it to kind1.
SignedoldposePc>=0 requests animPoseRelease8003b2d0; freshPc=-1 skips.
Call animPoseEntry(resource,2)8004176c; immediatelystore returnedPc; reloadPc
and call animPosePeraOff800414b0 WITHOUTnegative-return check.

Then request marioAnimeId800593fc; use its result toread M22c+4*index;
animPoseGetMaterialFlag8004069c(thatMarioPose), retain result, reloadPc and
SetMaterialFlagOn80040730(Pc,result). Request marioAnimeId AGAIN, select
currentM22c+4*secondindex; GetMaterialLightFlag80040684; reloadPc;
SetMaterialLightFlagOn800406f0. These are two live selector/owner captures;
do not cache oneMario pose or synthesize materialflags from tribe dimensions.
Later HP/pose/table/init callback remains full constructor contract.

## Animation entry usedmode2 and valid-resource domain

Mode2 first calls fileSetCurrentArchiveType80189d38(0); mode0 selects1,
mode1 selects2. Mode isn't a material bit, originalcameraID or party-slot index.
Posemanager=r13-7160, posearray=manager10,count14; records stride170hex.
Scan first pose whose flags0==0. Exhausted pool selectsnull/-1 then immediately
dereferencesnull before normal return: it is not a graceful negative result.
Mark selectedpose active1, clearwords4/8, then find/reuse loaded resource by
exact header name, incrementing its reference count. Otherwise select first
free16-byte resource record; no slot-exhaustion guard before writes there.

Newresource calls fileAllocf8018a61c withtype5 and originalprefix/name format.
Nullfile handle, or loaded file'sdata pointerzero, clearsresourceactive and
decrementsresource references, marks resourceindex-2. The latter case first
callsfileFree8018a260. Store selectedresourceindex→pose10; index-2 clears
poseflags and returns **-2**. CallerEntryMain doesnotrepairthisfailure and
stillrequestsPeraOff(-2). Validoriginalconstructor domain therefore requires
availablepose/resource/texturepool slots and loaded/validmodel bank, yielding
a nonnegativepose index. A native backend must retain an explicit resource
failure/pending boundary; it cannot claim constructor success by replacing a
missing bank with arbitrary handle0 or ignored negative indexing.

Successfulnewresource derives texture-name path from modeldata+44, reuses
matchingtexturecache byexactname/refcount or selectsfree12-byte record and
fileAlloc8018a5f8(type4). Texture-slot exhaustion likewise hasuncheckedwrites.
Successfulpose init uses mode2 GP38/3c ordinary timebase (mode0 usesGP40/44),
sourcehardware-frequency integer conversion, storesposemode2 at+c and calls
animPose_AllocBuffer80041ee0 withPOSEPOINTER. This large shape/buffer helper
is a transitive provider, not a renderer-ready result inferred from modelname.
Then defaults materialE8/EC=0, colorF0/F4=ffffffff, scalesF8/FC/44=1,
offsets100/E0/E4=0, identitymatrix124, normalbank sort/owner fields and154/158=0.
Source negative/overflow/slot/file/bootstrap/numeric domains remain explicit.

## Complete used flags/material leaves

All five compute posearray+170*RAWindex; no count/null/active/negative checks.
PeraOff clears poseflags mask8 only. GetMaterialFlag loads poseE8;
GetMaterialLightFlag loads poseEC. SetMaterialFlagOn ORs suppliedrawmask into
E8; SetMaterialLightFlagOn ORs intoEC. They don't overwrite flags0, select an
animation, altercamera/mode, remaplighting orresetpose clock. Since successful
newposeE8/EC initialized0, ordinary copiedflags become those exactMario masks;
reused/othercapture states still require original operations/capture order.
Negativepose/Mario sourcehandle or unknownmaterial/light backend is outside
the validnativeadmission until its actual producer exists.

Native next frontier: preserve published188-byteactor and camera/saved-kind/
resource request, then require actualc_pkr/d_mario bank/pose factory, original
material/read owners and buffer/shape/backend lifecycle before completing
animPoseEntry/PeraOff/materialcopy. A converted/prebaked renderer can be an
explicitnativebackend, but its positivehandle/resource/mode/field ownership
and supportedart mustbe real; sourceGP/time/shapes/rasterparity stayseparate.

## Exact original SHA256 evidence

| Body | SHA256 |
| --- | --- |
| GetCamId801a94f0/24 | 44aeb2104e604745f7bdf3e23c9dd7cd80355211832738edde34fde844bafcfd |
| SetCamId801a94c0/30 | d6d383eddf80be69e4bbc57ec57936634e3aa4b56f0c85a5473e5892a7c981f4 |
| InitCamId801a9514/30 | 3bb71c8239e37aa8973bef56fa964f5a78ad4354f2b848e5808d35633a385965 |
| EntryMain800be144/380 | 4564325d6f461dbfb4479609ce799ae466e5a158aa8a98451e6a8f2f1a1b8ec4 |
| animPoseEntry8004176c/4ec | 8d239cdb8bb0faf92f022b6f8289911fe399c160d7b769ed6cb39458a8da43c9 |
| PeraOff800414b0/1c | c97a7c2dfd8cdb00479eb6ada6379bdc59da74e7ca3a9fef5c93a3c955511028 |
| MaterialGet8004069c/18 | 23cb72ef00195bf5e23a2cc650f3730030d47b15ecfb683d0dbf20e7737f440e |
| LightGet80040684/18 | a898b38d911ab8c06288a02c83c6178caa78f6e2bca40a2445bbf254a5abd6ed |
| MaterialOn80040730/20 | f115c5e5cf50a18b35cc7ff50ad96091cd7a113140fcc4e178e330b9c9e2e65e |
| LightOn800406f0/20 | d0267ef39348de1f7d10bcf4b81165de78e82551a3f0e3c0b8a204df157a5209 |

Validation: read-only completeused localbodies, sourcepointers/constants and
direct constructor/camera capture order; scoped `git diff --check`. Onlythis
partner subsystem contract changed. No native tests/builds/sharedPROGRESS/
index edits or commit; transitive animation/file/buffer/material/raster/time
providers and actualexecution remain distinct requirements.
