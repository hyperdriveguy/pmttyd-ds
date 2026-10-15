# Original map NPC registration and pose owners

US G8ME01 disc0 revision0, read-only static source evidence for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[EVT-01](../../requests/EVT-01.md). This separates actual map registration
from [NPC setup](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md),
[gor setup0 battle data](GOR02_SETUP_ZERO_BATTLE_INDEX_AND_ORIGINAL_TRIBE_DESCRIPTORS.md),
and [ordinary pose/display ownership](../display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).
It does not claim native registration, complete original execution, or raster parity.

## Exact inspected bodies

Original ISO DOL bytes, with symbol boundaries from
`artifacts/reference/us_symbols.csv`; hashes cover each complete body below.
Fresh inspection used anonymous in-memory ELF disassembly, without executing
the source code or changing the ISO.

| Body | Address / size | SHA256 |
|---|---|---|
| npcInit | 8004bb70 / cc | ce07ec3389326b49ad38bb7630c3eed66d768327c46e54f2118a27a790e7a184 |
| npcEntry | 8004b894 / 250 | 1fe2a84bd4518b2f398d4febd28d4a01508f5d5c3165428a6ca408b584e748fa |
| npcGetTribe | 8004b828 / 6c | 741c64ee7ca08f239903f39c6b621adc93a15f12b0d7ee56bfcab5a1f104cc68 |
| npcNameToPtr_NoAssert | 80049934 / ac | 873ef842b09186bb24b901bde93600cafb1ffa9cb27a82b49e3220315702031b |
| npcNameToPtr | 800499e0 / 9c | db277558457058e2be1a3bb4589857e47de0eafaab9f914e55f5c76e0fe0a04b |
| setupDataBase | 80084f24 / 668 | 02b8acf7fce8e711405179cfb88e87e8a7cb8cde7906ec119977ce62be3c8594 |
| animPoseEntry | 8004176c / 4ec | 8d239cdb8bb0faf92f022b6f8289911fe399c160d7b769ed6cb39458a8da43c9 |

Existing detailed pose listing:
`artifacts/recovered-temp-evidence/battle-pose-owner-animPoseEntry.dis`.
Existing event tribe setter listing: `artifacts/outer-bank-evt_npc_set_tribe.txt`.

## Registry allocation and lookup

npcInit stores ordinary capacity36 at registry+4, allocates36*0x340 actor
bytes with __memAlloc800300f0(0,size), publishes registry+c, and clears
that actor allocation. It also allocates36*0x94 auxiliary bytes. Alternate
capacity16 is published at registry+18; its16*0x340 allocation is published
at+20 and cleared. Finally it allocates/clears0x580 field-battle bytes,
publishes their pointer to fresh GP+168, and stores mode halfword0.
The ordinary registry is SDA r13+1994 (8041e8b4); alternate work is+14.
Selection in npcEntry/name lookup depends on current GP+14, not room number.
This lists actual writes, not guessed defaults for uninspected registry fields.

npcNameToPtr_NoAssert selects that registry, scans signed capacity+4 slots
from array+c at stride0x340, and compares actor+8 name with strcmp only
when actor mainflag1 is set. It returns the first matching active actor or0.
Visibility, mainflag800 and queryflag2 do not participate in this lookup.
Source names are exact Shift-JIS bytes, including full-width digits.

## npcEntry constructor and duplicate names

npcEntry captures name/model arguments. At8004b8dc..b908 it scans active
names with strcmp and may stop on a duplicate. Regardless of that result,
at b90c it starts another scan from the first actor slot for the first
inactive slot. Thus the inspected release body does not reject duplicates
or reuse the matching actor; the duplicate search has no admission effect.
Later named lookup still returns the earliest active matching slot.

The inactive slot is cleared in full (0x340 bytes) at b944, then mainflags3
are written and strcpy copies the supplied name into actor+8. Capacity
exhaustion is not safely handled in this body; native bounded-capacity
admission must precede effects rather than copying the source overrun.
Source name buffers likewise have no local length check.

Next, read fresh GP+14, reduce nonzero to1 and call
animPoseEntry8004176c(model,selector). Publish its handle at actor+104.
Only result-2 triggers another fresh GP selector read and an original
`hoshi` resource fallback (string8041fd6c). Then call animPosePeraOn on
the resulting handle. No local asynchronous success stub exists here.

The constructor copies source zero vector802c11f8 into current+8c..94,
then copies those actual words into previous+98..a0. It queries animation
height800407b4 into+14c and radius80040818 into+150. Source unit vector
802c1204 initializes+e0/+e4/+e8. Verified explicit later writes include:

- +154=1; +108=0; +10c points to original string M_I_2 at8041fd74.
- Fresh GP+38/+3c words are copied into actor+198/+19c.
- +18c/+188=0; +1cc/+1c8=1; +1d0=0.
- +110=-1; +114=-1; +1f4=4; +2f8=0; byte+317=1.
- Nonzero animPoseGetVivianType8003980c sets main04000000.
- Model strcmp against original c_luigi802c1468 sets main00800000.
- animPoseSetMaterialLightFlagOn800406f0(handle,2), then registry+0 is
  freshly read/incremented; return the allocated slot index.

Constants above were checked from source r2=80427440: float1 at8041fc94,
float0 at8041fc7c and wordffffffff at8041fc6c. Unlisted bytes are the
constructor's actual cleared bytes unless a called service writes them;
they are not inferred gameplay defaults. In particular associated actor
pointers+330/+334/+338/+33c, setup IDs and descriptor+28 start cleared.
npcEntry itself never installs the tribe descriptor.

## Actual tagged-map producer

setupDataBase is the upstream tagged-map parser. The recovered NPC span
at80085334 resolves tagged tribe through npcGetTribe8004b828, captures
the returned descriptor, and calls npcEntry(taggedName,descriptor+4).
It then performs asserting named lookup at85350 rather than using the
returned slot index. Duplicate names can therefore direct later setup to
the earlier actor; replacing this with a unique-name dictionary changes
the source's duplicate domain.

At85358 publish parsed map ordinal byte+314. OR mainflag4 at85368..70.
Copy parsed XYZ into current+8c/+90/+94, parsed yaw into+144, publish
captured descriptor at+28 (85394), copy descriptor width+28 and height+2c
to actor+14c/+150, then clear mainflag2 at853a8..b0. The shown map span
does not also copy tagged current XYZ into previous; the earlier npcEntry
copy used its initial zero root. A later actual owner may update history.

Consequently constructor mainflags3 are not final map registration flags:
map setup deliberately has active1 and pending4 with query2 clear. The
producer of later query readiness must be bound separately; do not infer it
from rendering or installed talk scripts.

The following map setup scans npc AI rows at80338270, stride0x24, by the
parsed AI name. Matching data ORs AI flags and installs actor+120,+124,
+12c,+130,+134,+138,+13c. It resolves territory type through table80416580
and installs territory/ranges at+1f8..22c. Tribe sounds+40/+44 and fields
+4c/+50/+48 go to actor+1e0/+1e4/+1ec/+1f0/+1e8; it sets halfword+1d8=1
and float+1dc=0. Source anchors853b4..854d8 establish those ordered writes.

At854dc areaDataPtr resolves the actual map area; original parsed battle
name is translated through area+0c unless it is the invalid sentinel.
npcSetBattleInfo80043d80 is called at85510 with that actual index.
This map-construction battle request precedes the separate npcEnt row's
battle request; they must not be collapsed into one presumed index0 call.
Descriptor+10/+14 pose strings are then copied to actor+4c/+6c; descriptor
+8 supplies actor+2c and the initial animPoseSetAnim request. Exact tagged
values must come from original m/gor_02/s, not native visibility envelopes.

## Native integration frontier

Current PMNPC catalog models and FieldNpcInstance body/wander records can
provide recovered rendering, motion and named mutations for their finite
profiles. They do not own this source registry, descriptor/resource handles,
all map AI metadata, independent init/regular event IDs, or pending-readiness
transition. Their initialized boolean cannot acknowledge source setup.

A genuine binding needs stable addressable registered actor records, exact
names and descriptor pointers, loaded original animation resources with
source pose-handle effects, both map and npcEnt battle services, AI/default
script ownership, actual event IDs/context+170, and deletion/reinitialization
lifetimes. Native complete pre-admission or retained suspension must prevent
partially repeated allocations/resource requests. Ordinary/alternate pools,
resource failure/fallback, duplicate/full-capacity domains, later queryflag2
writer, complete parser admission and original runtime traces remain explicit
dependencies. This contract does not authorize a fabricated actor-registration
or pose-readiness success path.

The original packed allocator/constructor/name lookup is now implemented; see
[mechanics and native admission](PACKED_NPC_REGISTRY_ALLOCATION_CONSTRUCTOR_AND_LOOKUP.md).
Fresh release-body inspection corrects the conventional asserting-name label:
npcNameToPtr returns the exhausted scan pointer when missing, not0 and not an
executed assertion. NoAssert alone returns0. Complete registered-name admission
is mandatory for native unchecked calls. Actual
[field pose leaf/resource owners](ORIGINAL_FIELD_NPC_POSE_LEAF_SERVICES.md) remain
separate; this does not complete Game construction or tagged-map registration.
