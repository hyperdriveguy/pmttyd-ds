# Partner pose requests: name admission,selector tables and source getters

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyChgPose0x800bedec(size0x7c),partyChgPoseId0x800bee68(size0x164),
partyGetHp0x80140ea4(size0x44),pouchGetPartyColor0x800d2e44(size0x14).
These are used pose requests,not completed animation playback. Pending markers
are consumed by [pre-display](FIELD_PARTNER_PRE_DISPLAY.md).

## Name request partyChgPose(P,newName)

If P+4 mask2 set,return without comparing/updating names. Otherwise read old
name P+18. Nonnull old name requests strcmp(old,new);equality returns without
reset. Null old name or inequality next checks P+4 mask0x8000:SET returns
without publication. That inhibit check occurs AFTER any requested comparison.

Admitted publication stores the NEW borrowed name pointer to P+18,clears
halfword P+20,and ORs0x2000 into P+8. No pose-engine call or owned string copy
occurs. Existing matching names retain frame/markers. No null-newName guard
is added;source/name/alias/string lifetime domains remain external.

## Numeric request partyChgPoseId(P,I)

P+4 mask2 set immediately returns current P+18 pointer,without HP/query/table
work. Otherwise sign-extend kind K=P+31. If K<8,request partyGetHp(K);result
signed<=5 enables remap I1→12,I2→13,I3→14. Every other I is unchanged.
K>=8 skips HP and remap. Negative K is not locally excluded from that<8 route.

Now choose table using CURRENT P+4 mask0x8000. Unlike the name request,this
flag selects dot resources;it does not inhibit numeric publication:

| Condition | Selected address |
| --- | --- |
| Flag clear | 80314708+60*signed current kind+4*I |
| Flag set,current signed kind!=4 | 80314bb8+60*kind+4*I |
| Flag set,current signed kind==4 | Request pouchGetPartyColor(4)=C;80314d98+60*C+4*I |

Load selected pointer S. Existing P+18 nonnull requests strcmp(old,S).
Equality returns S with no reset. Otherwise store S to P+18,clear unsigned
halfword P+20,OR0x2000 into P+8,and return S. No selected-pointer null guard,
kind/index clamp or fallback name exists. Kind is reread for table addressing,
not necessarily the same byte captured before the HP service.

The normal table's pinned0x4b0 bytes comprise20 rows of15 words;dot table
0x1e0 bytes comprise8 rows. The named yoshiDotPoseTbl0x1a4-byte span contains
SEVEN15-word rows. Color getter can return0..7;row7 addresses adjacent data
at0x80314f3c,which is pinned separately rather than invented as an eighth valid
pose row. Shipped color/index source domains still require proof. Do not silently
clamp7 to6 or add a new row under this local contract.

## Source leaves

partyGetHp(K) returns0 for signed K>=8. Otherwise request pouchGetPtr
0x800d5eb0 and load SIGNED halfword at returnedBase+14*K+6. There is no lower-
bound/null check;negative indices address preceding data with original32-bit
arithmetic. This is a resource getter,not an HP update or recruitment predicate.

pouchGetPartyColor(K) multiplies K by14,loads the global pouch pointer from
SDA+1be0,loads UNSIGNED halfword at base+14*K,and shifts right13. Return0..7
from those three top bits,without kind/null/range validation. The numeric-pose
caller passes literal4. Full pouch fields/writers/recruitment/save/color lifetimes
remain separate even though these local loads are closed.

`tools/verify_field_partner_predisplay.py` pins these four complete local bodies,
remap/table/current-name requests and original table/adjacent spans alongside
pre-display/facing/drawing/tail predicate. Exact resource strings remain reused
original assets;metadata hashes are not playback or original visual validation.
