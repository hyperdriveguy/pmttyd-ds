# Bero initial-position locals and actor writes

US G8ME01 disc0 revision0; read-only static recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Complements
[selected-entry bank](../event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md),
[indexed info/length](BERO_INDEXED_INFO_LENGTH_AND_REGISTRATION_STATE.md),
[case allocation](BERO_CASE_POOL_IDS_AND_SWITCH_WRAPPER_LIFETIME.md) and
[source center/ray](OVERWORLD_ROAD_POSITION_AND_CONTACT.md).
Addresses/offsets/masks are hexadecimal; coordinates, counts and indices are
decimal. This supplies actual producers and required providers, not static
runtime positions or a completed16-local house05 bank.

## Original initial-position event and ordinary ROAD arm

evt_bero_initial_pos8030f060/278 first calls into_info at8030f060,
SETs LW0=LW4 at8030f068 and ANDs LW0 withfff at8030f074. Kind0's
original requests are:

| Address | Request |
| --- | --- |
| 8030f11c | evt_bero_get_info_length(LW1,LW10) |
| 8030f12c | bero_get_position_normal(1,LW10) |
| 8030f13c | READF(LW10,BeroSX8041e9f8) |
| 8030f148 | READF(LW11,BeroSY8041e9f4) |
| 8030f154 | READF(LW12,BeroSZ8041e9f0) |
| 8030f160 | evt_mario_set_pos(LW10,LW11,LW12) |
| 8030f174 | evt_party_set_pos(0,LW10,LW11,LW12) |
| 8030f18c | evt_party_set_pos(1,LW10,LW11,LW12) |
| 8030f2c8 | evt_cam_road_reset(),after the closed kind switch |

Kind1 uses the identical pattern at8030f1ac..8030f21c. Kindsf00/3 call
normal(0,FLOAT0) at8030f098 and publish/read/set at8030f0a8..8030f0f8.
Kind2 uses length then pipe_pure(1,LW10) at8030f23c/8030f24c,followed by
READF/setters8030f25c..8030f2ac. FLOAT0 literalf24a7a80 is encoded
-230000000, not a raw IEEE zero pointer. Pipe geometry/providers remain in
[the separate route contract](ROGUEPORT_UNDERGROUND_PIPE_ROUTE.md).

into_info writesLW1..8; this event then writesLW0 and,for admitted kinds,
LW10..12. Its inspected words do not writeLW9,LW13,LW14,LW15. Length first
publishes intoLW10,then READF overwrites that same word with starting X.
The local word must not remain length30 after the event finishes.

## Complete ordinary normal-position producer

bero_get_position_normal80079b74/310 captures event/args and Mario pointer,
resolves mode operand0 with evtGetValue,then distance operand1 with
evtGetFloat. It reads callerLW6 (event+b4) as pointer to exactly three signed
integer XYZ words and callerLW3 as hit-name pointer and LW5 as signed
direction index. Three coordinates are converted to binary32 and duplicated
into two stack tuples; it does not consume six independent source integers.
Conversion bias802c3058 is4330000080000000 (2^52+2^31).

If integer X==100000, hitObjGetPos(name,startingTuple) supplies the actual
named hierarchy center, using its runtime matrices/leaf-center traversal and
runtime80 name gate. The other copied tuple has not been updated by this
call yet. No native marker bounding-box substitution establishes this query
for an arbitrary hierarchy or transformed map.

For nonzero distance, the source performs these single-precision operations
without a heading wrap:

1. heading=single(single(360*direction)*.125); ifmode==1,
   heading=single(heading+180).
2. radians=single(single(twoPi*heading)/360),twoPi bits40c90fdb.
3. P.X=C.X-30*single(sin(radians));P.Z=C.Z+30*single(cos(radians)),
   using source fused negative multiply-subtract instructions.
4. Query hitCheckFilter8001577c(NULL filter,originP,direction(0,-1,0),
   inputlength1000000,distinct point/normal output pointers).
   Nonnull hit assigns P.Y=raw returned hit-point Y;miss retains C.Y.
5. Copy P to final tuple F. Re-evaluate source sin/cos;let
   D=single(30+distance). F.X=fma(D,single(sin(radians)),P.X);
   F.Z=fma(D,-single(cos(radians)),P.Z). F.Y=P.Y.

The fixed radius30 in step3 is not the queried distance. Mode1 adds180;
mode!=1 does not. Even an apparently equivalent cardinal heading wrap can
change original SDK trig/rounding; native cardinal adaptation must not claim
bit-identical Gekko results without numerical comparison. The floor query
starts at P.Y exactly,with no37-unit origin raise,player-footprint filter or
mandatory-hit fallback. Original source cull/winding and search order apply.

Distance0 skips steps1..5. Starting tuple is then the resolved hit center
when sentinelX selected hitObjGetPos,while final tuple still contains the
initial copied integer XYZ. Do not universally replace both zero-distance
tuples with the resolved center. This edge is outside current nonzero ROAD
length30 use but is visible in the complete local body.

The publication tail reloads actual stack tuples,then writes raw IEEE32:

| Store | Global/value |
| --- | --- |
| 80079e28 | BeroSX8041e9f8=P.X |
| 80079e2c | BeroSY8041e9f4=P.Y |
| 80079e30 | BeroSZ8041e9f0=P.Z |
| 80079e34 | BeroEX8041e9ec=F.X |
| 80079e38 | BeroEY8041e9e8=F.Y |
| 80079e3c | BeroEZ8041e9e4=F.Z |

Then call N_marioSetBottomlessRespawnPosOnBeroEntry80096de4 with actual
F.X/F.Y/F.Z at80079e40. Freshly read captured Mario+2e;if motion25,
call N_marioReloadMapOnBottomlessOn80096d34 at80079e50. Return2.
This helper itself does not publish LW10..12 or write Mario current XYZ.

## Recovery-point correction from complete register traffic

N_marioSetBottomlessRespawnPosOnBeroEntry80096de4/a8 retains input floats,
gets Mario,loads FPRf0=37 at80096e1c from80420aa4 (bits42140000),then
computes single(f0+inputY) at80096e28. Intervening lwz writes **GPRr0**,
not FPRf0. It therefore writes Mario+2e4=X,+2e8=single(Y+37),+2ec=Z,
with the source stack/integer-copy store order. It does not use a zero table
word as the Y addend. This explicitly corrects the older
OVERWORLD_BOTTOMLESS_EVENT_CHAIN interpretation which mixed GPR/FPR liveness.
N_marioReloadMapOnBottomlessOn80096d34/28 gets Mario and writes+2e0=1.
Neither function writes the original caller's event local bank.

The older OVERWORLD_BERO_NORMAL_PLACEMENT reading also misidentifies the
independent source XYZ count,mode branch and publication residues. The fresh
complete listing establishes the three duplicated XYZ values,mode==1 addition,
and explicit F.X/F.Y/F.Z reloads immediately before the recovery callback.
Use this contract plus the newer center/ray contract for implementation.

## READF quantization precedes current actor writes

Original opcode86 READF reads raw IEEE float from each absolute BeroS global
in X,Y,Z order,then calls evtSetFloat for its raw local destination. In the
finite conversion domain each lane becomes:
word=wrap32(truncSigned32(single(P*1024))-230000000).
This event float is not raw IEEE,an integer coordinate or a host double.
The subsequent setters call evtGetFloat and recover the quantized Q value;
source P versus Q may differ by event-float truncation. Use actual operand
routing for exceptional/out-of-range domains rather than invented saturation.

Thus LW10..12 contain the encoded **starting P tuple**,not final F. For
mode1 ROAD, ignoring independent Y resolution, P lies30 units outside C
along the original source heading and F lies queried distance inside.
Native final arrival coordinates cannot simply be copied into these locals.
Unquantized BeroS/BeroE/recovery publications and quantized current actor
position are distinct source stores.

evt_mario_set_pos800e7180/78 captures args,gets Mario once,then reads/resolves
rawX via evtGetFloat and immediately stores Mario+8c;rawY→+90;rawZ→+94.
All three resolution/store pairs are sequential;it does not resolve all
coordinates before the first write. It returns2 and ignores firstCall.
It does not update previous/root snapshots,heading,pose,control or floor state.
No event local is written by this setter.

evt_party_set_pos800eb558/98 reads **raw** selector operand0;zero chooses
global current index8041ead0,any nonzero chooses literal slot1. It does not
resolve selector as an event integer. partyGetPtr800bf1b4 resolves that
actual member;NULL returns2 **before any coordinate operand read**. Present
member:rawX operand1 evtGetFloat→party+58,rawY operand2→+5c,rawZ operand3→+60,
in sequential read/resolve/store order. Return2,ignore firstCall. No previous
position,heading/pose or event local update is supplied. Actual absent party
is a completed source branch;unknown registry/current-index is not proof of
absence. See [current-index source](../event/TROUBLE_CENTER_DIRECTION_CALLBACK_SOURCE_CORRECTION.md).

## Final camera request has real effects but no local outputs

evt_cam_road_reset800a785c/a8 gets Mario then camGetPtr(4),publishes camera
+94=X,+98=single(Y+.01),+9c=Z. Constant80421040 is3c23d70a
(single.009999999776482582),not10 or1 engine units. It copies those camera
words to+ac/+b0/+b4,then copies current MarioXYZ to Mario+ec/+f0/+f4 and
again to+f8/+fc/+100. It calls camRoadReset8000aa14 followed by
camShiftReset800f34d0,then returns2. It reads quantized Mario current values
after the setter chain. There are no event arguments/local outputs here.
Camera reset state/providers remain required before acknowledging complete
original event execution; omitting them merely because locals are unchanged
would fabricate successful world effects.

## Save-load LW13/14 are pointers from the last actual entry

System save-load domain of evt_bero_info_run invokes attached
bero_case_entry8030e608 instead of the ordinary placement child. Its indexed
callback80079e84 publishes LW13=entry+28 target-map pointer at8007a068,
then LW14=entry+2c target-entry-name pointer at8007a070. These source reads
require original retained record/string storage;they do not require entering
the target room,loading its art or inventing a native area ID.

For a completed positive-count loop,these words come from the last actual
indexed entry,count-1. Actual case allocation/index-save/switch providers
must still complete before claiming the child returned its final16-word bank.
Independent inline switch-on copies do not overwrite parent locals on return.
Do not predict static runtime case IDs or call the bank final while a source
case provider remains pending.

gor_02 actual table has count2. Last row3ee60 is dokan;its target-map pointer
relocates from file184c4 stringtik_01 and target-name pointer from184cc
stringdokan_2. In the existing explicit native REL section-address mapping
these are original-data guest pointers,not native numeric room5 or collision
node IDs. Other areas have their own ordered full tables;ROAD-only filtering
cannot identify the original last entry. The later inspected house door02/
03/roll children overwrite LW1..3/LW15,not LW13/14.

## Remaining16-word bank frontier and evidence

This closes producer semantics for ordinary LW10..12 and save-load LW13/14,
including actor setter boundaries. It does not assign static runtime values.
For actual ordinary event completion still require selected source state,
unfiltered transformed center/ray/trig providers,Bero globals/recovery,
actual Mario/party/current-index and camera services. For save-load require
the actual shared case pool and all source allocations/switches in order.
Outer gor_02 init USER_FUNCTION/transitive effects and initial event-bank
provenance remain distinct from these recovered children;do not substitute
zero high locals without that remaining closure.

Fresh ignored listings: artifacts/bero-local-evt_mario_set_pos.txt,
bero-local-evt_party_set_pos.txt,bero-local-evt_cam_road_reset.txt,
bero-local-bottomless.txt,bero-local-bottomless-on.txt;
complete original normal helper house05-bero_get_position_normal.txt and
event inventory house05-bero-subchildren.json. Original REL SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
No shared documentation/native code/assets/build/git changes made here.
