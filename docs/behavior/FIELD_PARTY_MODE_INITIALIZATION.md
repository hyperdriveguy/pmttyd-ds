# Field party mode transitions and local cleanup

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
N__partyChgRunMode at0x800bcb98,size0x39c;partyChgRunMode at0x800bcb2c,
size0x6c;partyClearJumpPara at0x800cc0a0,size0x20;
L_partyForceSlitOff at0x8014c448,size0xc0;revise360 at0x80096564,size0x34.
This closes local writes and ordered calls,not every mode's update engine or
original movement/visual execution. Addresses are DOL virtual addresses.

## Wrapper order and old-mode dispatch

partyChgRunMode(P,new) reads byte P+0x34. If the old byte is neither9 nor3,
copy it to P+0x35. It calls N__partyChgRunMode(P,old,new), then writes the
requested new mode's low byte to P+0x34 and ORs8 into word P+8.
Do not interpret an intermediate mode write in the called routine as the
wrapper's final mode,or assume the old/new arguments have been swapped.

The inner routine switches **signed old mode**,not new mode:

| Old mode | Local request path |
| --- | --- |
| 1 or2 | Call L_partyForceSlitOff(P) |
| 3 | Read signed P+0x31 kind;call kind record+0x10 with P if nonnull |
| 4 | Restore vectors/owner parameters,jump cleanup,pose selection,clear flag0x10 and write intermediate mode1 |
| 11,new==8 | Return without that mode's restore path |
| 11,all other new values | Restore vectors/owner parameters,jump cleanup,pose selection and flag clears |
| 12 | If P nonnull,clear P+4 mask0x100000 |
| Others,including0 and5..10 | No local work |

The kind record for mode3 is0x803143e8+signed-kind*0x28. The pinned kind1
record binds its+0x10 callback to kuribo_use_post;that body remains a
dependency. A null callback skips the call. Other cases do not use the new
mode except old11's exact8 guard.

## Old11 restore path

With new!=8,let M=P+0x160. Copy vector at0x802cb310 to P+0x70..0x78,
then vector at0x802cb31c to P+0x7c..0x84. The pinned values are respectively
(1,1,1) and(0,0,0). Copy M+0x1a0 into P+0xfc,M+0x1a4 into P+0x100
and M+0x1b0 into P+0x110. Clear mask **0x70000** in word P+0.
Then call partyClearJumpPara(P).

If P+4 mask0x2 is clear,select a pose using the
[constructor selector/table rules](FIELD_PARTY_POSITION_CONSTRUCTORS.md):
selector1,or12 when signed kind<8 and queried HP<=5;normal versus alternate
tables selected by mask0x8000,with the special kind4 color lookup. If the
existing string pointer is null or strcmp differs,store the selected pointer,
clear halfword+0x20 and OR0x2000 into word+8. If mask0x2 is set,skip the
selection and comparison entirely.

Next clear mask0x10 in P+4,then clear mask1 in a second load/write. The
following P-nonnull test controls a third clear of mask0x100000. That late
test does **not** make the earlier dereferences null-safe. No local mode write
occurs on this path;the wrapper still stores its requested new mode afterward.

## Old4 restore path

Copy vector0x802cb28c,(1,1,1),to P+0x70..0x78 and clear mask0x10 in
P+4. Copy M+0x1a0→P+0xfc,M+0x1a4→P+0x100,M+0x1b0→P+0x110,
and additionally **M+0x1ac→P+0x10c**. Call partyClearJumpPara(P),then
perform the same conditional pose selection/change requests described above.
Clear P+4 mask0x10 again and write **mode byte1** at P+0x34. The outer
wrapper subsequently overwrites this byte with its requested new value.

This path does not copy old11's second vector,clear P+0 mask0x70000,or
clear P+4 masks1/0x100000. Do not merge the two paths into an identical
reset-all operation.

partyClearJumpPara itself clears **mask0x70 in P+0** and writes binary32
positive zero into P+0x114,+0x118,+0x11c. There is no null check or change
to the enabled mask0x1,mode byte,position vector or EVT entry in this helper.

## Old1/2 slit cleanup

L_partyForceSlitOff returns without work unless **P+0 mask0x1000** or
**P+4 mask0x100** is set. Otherwise:

1. Compute revise360(R(P+0x100 - P+0xf8)). Choose angle-base0 if the
   result is ordered>=180;otherwise choose180. Store it at+0x10c,then+0x110.
2. Compute revise360(R(270-angle-base));store it at+0xfc,then+0x100.
3. Clear mask0x3c0 in P+4 and call **partyPaperOff(P)** at0x800beff4.
4. Reread word P+0 and clear mask0x3000,then clear byte+0x37.
5. Request **partyChgPoseId(P,1)** at0x800bee68.

R is the fsubs single-precision result rounding. The ordered comparison's
greater/equal CR OR does not classify unordered as>=180. Pose/paper service
bodies remain dependencies;their names do not prove original rendering.

revise360 repeatedly performs fadds(value,360) while ordered value<0,
then fsubs(value,360) while ordered value>=360. Constants are exact0 and360.
For ordinary finite angles this returns a value in[0,360). It uses neither
division nor a remainder instruction. Huge values for which adding/subtracting
360 stops changing the single-precision result can fail to progress;infinities
and exception state are not sanitized. Do not replace this loop with a
different numeric-domain policy without recording the equivalence limit.

## Used field run/stop composition and verification

The [field callbacks](FIELD_PARTY_EVENT_CALLBACKS.md) stop calls this wrapper
with new0;run calls it with new1 only if current mode is0. Consequently an
ordinary old1→stop0 transition requests slit cleanup,while old0→run1 has no
inner cleanup. Both retain P+0 enabled mask0x1 through these local paths. They
do not locally stop an EVT entry or remove P from
[partyMain's update eligibility](FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md).
Whether driveParty actually advances movement in mode0 is a separate body
contract,not established by the word stop.

tools/verify_field_party_modes.py checks eight whole body hashes,44 instruction
anchors,six constants,three data spans,two used lecture imports and the scoped
control-index census. No original execution,measured timing or visual/audio
trace is claimed. Next:all mode update bodies,mode3/all-kind hooks,pose/paper
internals,global/owner-slot writers and original lecture transition observations.
