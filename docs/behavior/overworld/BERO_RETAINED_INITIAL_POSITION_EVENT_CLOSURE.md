# Original retained Bero initial-position event closure

US G8ME01 disc0 revision0, read-only static recovery for
[EVT-01](../../requests/EVT-01.md). This defines a finite ordinary ROAD
execution boundary; it does not establish completion of the outer area init,
Bero arrival movement, attached entry event or full camera service.

## Immutable root and address model

The complete original DOL data object is evt_bero_initial_pos at8030f060,
size278, exclusive end8030f2d8, DOL file offset30c060. SHA256 is
214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519.
There are45 commands, including terminal RETURN2 at8030f2d0 and END1 at
8030f2d4. Retain the complete original words, including unadmitted branch
words; do not synthesize a truncated ROAD program or replace branch targets.
A pack loader that fetches beyond END must provide its own bounded fetch policy
rather than claim additional bytes belong to this object.

DOL absolute addresses are already linked: this root needs no REL relocations.
USER_FUNC addresses identify native callback dispatch services, not executable
PPC bytes to package. READF addresses identify mutable globals, not immutable
asset data. This object has no direct event-script pointer operand or child
creation request. Source selected-entry records/names/XYZ storage are separate
catalog dependencies: retain their actual relocated guest addresses when
into_info publishes pointers. In a combined catalog, keep original DOL guest
addresses disjoint from the explicit REL section guest mapping.

The six mutable words8041e9e4..8041e9fc are BeroEZ,EY,EX,SZ,SY,SX, in that
ascending-address order. Provide mutable reads/writes outside immutable script
storage. Do not put startup-zero copies in the immutable pack and subsequently
read stale values from those copies.

## Actual control words and ordinary requests

Operands LWn have raw wordfe363c80+n. The root begins USER_FUNC91
into_info8007a214 at8030f060; SET50(LW0,LW4) at8030f068;
AND78(LW0,fff) at8030f074; SWITCH34(LW0) at8030f080.
These opcodes, and READF86, CASE_EQ36, END_CASE_GROUP46, END_SWITCH49,
RETURN2 and END1, suffice for the entire script control domain.

| Source address | Original command in kind0 ROAD arm |
| --- | --- |
| 8030f114 | CASE_EQ36(0) |
| 8030f11c | USER_FUNC91(8007a76c,LW1,LW10), get_info_length |
| 8030f12c | USER_FUNC91(80079b74,1,LW10), normal-position helper |
| 8030f13c | READF86(LW10,8041e9f8), starting X |
| 8030f148 | READF86(LW11,8041e9f4), starting Y |
| 8030f154 | READF86(LW12,8041e9f0), starting Z |
| 8030f160 | USER_FUNC91(800e7180,LW10,LW11,LW12), Mario current XYZ |
| 8030f174 | USER_FUNC91(800eb558,0,LW10,LW11,LW12), current party |
| 8030f18c | USER_FUNC91(800eb558,1,LW10,LW11,LW12), party slot1 |
| 8030f2c4 | END_SWITCH49, reached after source case skipping |
| 8030f2c8 | USER_FUNC91(800a785c), camera-road reset |
| 8030f2d0 | RETURN2 |
| 8030f2d4 | END1 |

A matched CASE_EQ marks the switch done; the following CASE_EQ skips to its
END_SWITCH rather than executing the next kind. Kind0 therefore does not also
execute kind1 or pipe placement. Do not interpret the absent conventional
break after kind0 as C-style fallthrough.

Other source case labels f00 and3 precede the normal-helper block with mode0
and encoded FLOAT0=f24a7a80 at8030f098, followed by publication/setters and
END_CASE_GROUP46 at8030f110. Their grouped control admission remains outside
this ROAD-only execution boundary. Kind1 uses the same operations as kind0
at8030f1a4..8030f234. Kind2 requests
bero_get_position_pipe_pure800797d0 at8030f24c. An unsupported kind reaches
the common camera reset without placement. A ROAD-only native owner must
admit an actual selected kind0 entry before execution or explicitly remain
pending on unsupported providers; it cannot silently acknowledge pipe effects.

## Callback closure and world effects

The ROAD arm has six distinct callbacks, all original leaf callback returns2:
into_info8007a214, length8007a76c, normal80079b74, Mario setter800e7180,
party setter800eb558 and camera reset800a785c. Dispatch with the actual
EventEntry and first/poll protocol; generic success for a missing callback
would falsely complete world effects. No wait or requested child is present,
so ready providers can complete this root in one source manager dispatch.
That is not authorization to schedule it earlier than its actual parent.

Use [selected-entry semantics](../event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md)
and [length/registration state](BERO_INDEXED_INFO_LENGTH_AND_REGISTRATION_STATE.md)
for the first two callbacks. into_info needs actual retained INFOARR/count,
GP incoming-name alias/flags and selected original record/string storage. It
publishes LW1..8 including the script pointer inLW7; it does not execute that
pointer. Neither this root nor the normal helper requests attached scripts.
Later evt_bero_info_run children own entry-event execution separately.
Length must resolve operand0, obtain the selected record, calculate the source
float and publish through actual evtSetFloat operand1 intoLW10. READF then
overwrites LW10 with encoded starting X; leave no length residue there.

The normal helper captures its Mario pointer, resolves mode then distance,
reads actual LW6 XYZ pointer/LW3 name/LW5 direction and performs the complete
[center, ray and numerical producer](BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md).
Its dependencies include evtGetValue/evtGetFloat, readable signed XYZ words,
hitObjGetPos for sentinelX100000, NULL-filter hitCheckFilter floor query,
source sin/cos and float/FMA behavior, and real mutable Bero S/E globals.
Successful geometry calculation alone is insufficient callback completion.

Its publication order is SX at80079e28, SY at9e2c, SZ at9e30, EX at9e34,
EY at9e38, EZ at9e3c. All are raw unquantized IEEE32. It then calls
N_marioSetBottomlessRespawnPosOnBeroEntry80096de4 at80079e40 with final F.
That service gets Mario afresh and writes current recovery XYZ in source
order: +2e4=F.X at80096e54, +2e8=single(37+F.Y) at80096e5c,
+2ec=F.Z at80096e60. Constant80420aa4 is42140000, float37.
Only after this call does the helper freshly read its originally captured
Mario halfword+2e at80079e44. If exactly25, call
N_marioReloadMapOnBottomlessOn80096d34 at80079e50; that service gets Mario
again and writes word+2e0=1 at80096d48. Other motion values preserve+2e0.
Recovery stores precede READF and current actor setters; retain unquantized F,
not quantized starting P or the final native movement endpoint surrogate.

READF uses real evtSetFloat, so each starting P lane is quantized as
wrap32(truncSigned32(single(P*1024))-230000000) before setters resolve it.
The actor setter and reset order is detailed in the linked position contract.
Current-party selector0 needs known global8041ead0; any nonzero raw selector
chooses slot1. An actual null actor returns2 before coordinate reads. Unknown
native registry/current-index is an admission gap, not proof of null.

The camera callback consumes current Mario root after setters, updates raw
follow/look and Mario retained target/contact words, then calls road reset
and shift reset. Preserve independent event view/target blends and clock;
see [reset ownership](../camera/OVERWORLD_CAMERA_MAP_LIFETIME_AND_DUPLICATES.md).
Native unit conversion and host trig/FMA remain explicit numerical adaptations.

## Evidence and boundaries

Fresh complete word inventory: artifacts/bero-retained-initial-pos-closure.json,
produced by tools/dol_event_inventory.py with the original US symbol CSV.
Complete function evidence: artifacts/outer-bank-bero_get_position_normal.txt,
artifacts/bero-local-bottomless.txt, artifacts/bero-local-bottomless-on.txt,
artifacts/bero-local-evt_mario_set_pos.txt,
artifacts/bero-local-evt_party_set_pos.txt,
artifacts/bero-local-evt_cam_road_reset.txt. The root's original symbol entry
and exact size/file mapping are independently checked by the inventory tool.

This contract supplies source closure, not static runtime local values. It does
not claim parent scheduling, entry-event completion, generic pipe service,
full original allocator/memory ABI, arbitrary hierarchy transforms or original
raster/runtime observation. No original assets or executable code are embedded
in this documentation.
