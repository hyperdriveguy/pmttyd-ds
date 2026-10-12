# Partner live target-distance selection and repeated service reads

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyGetTargetDist3D0x800ccc94,size0x16c and partyGetTargetDist0x800cce00,
size0x190. They select **live Mario/party positions**,not records from the
follower-history bank. Their integer q argument can come from
[partyGetFootmarkId](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md).

3D receives(q,point-pointer) and returns PSVECDistance's floating result.
Planar receives(q,X,Z),captures its two floating arguments in nonvolatile
registers and returns distABf's floating result. The 3D point pointer is retained
and its coordinates are read by the final distance call;it is not copied into
an immutable input snapshot by this wrapper.

## Normalization and first candidate

Both first query global Mario as G0,normalize q via the32-bit negation/OR/sign
sequence to boolean **b=(q!=0)**,then request anotherPartyGetPtr(b). This
normalization includes negative and extreme32-bit q values;it is not an
unbounded pointer-table index. The earlier
[other-party helper](FIELD_PARTNER_HISTORY_WRITER.md) maps b0→table1 and
b1→table0. Let this first returned candidate be Q0.

Even q==0 performs that other-pointer request before the direct distance:

| q==0 route | Actual endpoint |
| --- | --- |
| 3D |A **fresh** marioGetPtr result G1,position+0x8c/+0x90/+0x94 |
| Planar |The **captured G0** X/Z,+0x8c/+0x94 |

For q!=0,when Q0 is nonnull,call nokonokoGetStatus(Q0). Status3 marks it
unusable without a distance query. Otherwise compare
PSVECDistance(G0 position,Q0 position) ordered>=150. Unordered does not
mark it unusable. Q0 null also selects the fallback route,without status/distance.

The fallback endpoints differ as above:3D gets fresh Mario immediately before
the final distance call;planar reads X/Z through captured G0. Neither wrapper
locally rewrites a history record or moves a partner.

## Usable first candidate triggers a second full selection

Only a nonnull,usable Q0 enters the expanded route:

1. Query fresh Mario **G1**,retain it as the later fallback pointer.
2. Query fresh Mario **G2**,retain it for a new separation check.
3. Normalize the **original q again**,request anotherPartyGetPtr(b) as Q1.
4. If Q1 nonnull,query its status. Status3 or ordered distance(G2,Q1)>=150
   makes Q1 unusable. Null skips both services.
5. If marked unusable,replace local Q1 with null. Select Q1's live position
   when nonnull,otherwise G1's live position.
6. Compute the final distance to the caller's input point/coordinates.

The two Mario reads serve different pointer roles. The second candidate is
not replaced with cached Q0. No local equality check establishes that any of
these pointers or coordinates remain identical across service requests. Under
stable state the expanded path usually selects Q1;this is a deduction,not an
executed invariant or permission to discard the repeated reads.

3D copies its chosen endpoint's three floats into a stack vector and calls
[PSVECDistance](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md) with that vector and
the retained caller point pointer. Planar calls distABf(inputX,inputZ,
selectedX,selectedZ) at0x8000722c. No epsilon,height offset or configurable
distance threshold is introduced. The planar numerical helper remains separate.

## Used callers and verification

[Stay](FIELD_PARTNER_STAY_MOTION.md) calls3D then planar on its ground-primary
route and retains the3D result,while the planar result is unused there. Its
secondary route uses the planar result to decide a motion change. These calls
perform their selection requests even when a result is not subsequently used.

tools/verify_field_partner_targets.py pins the two wrapper bodies,initial and
repeated normalization/status/distance branches,actual stay call sites,footmark
helpers and the SDK distance body. Six body hashes,55 instruction anchors,
three constants and two lecture imports pass against the read-only US image.
Reports and original corrected Gekko ELF/listings remain ignored.

Next:source status/ID writers and getter bodies,planar numeric helper,exact
paired/estimate environment,valid pointer/state domains and original live target
observations. Static selection closure does not complete follower locomotion,
collision or game equivalence.
