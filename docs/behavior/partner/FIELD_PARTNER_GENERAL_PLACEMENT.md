# General partner placement: entry,primary attempt and admission

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyGetAppearPos0x800c6c9c,size0x135c. This is the general writer called by
[placement4 exhaustion](FIELD_PARTNER_PLACEMENT4.md) at0x800c61e0. It also
has seven other direct DOL callers listed below. The complete local algorithm
is split here and in [fallback/output](FIELD_PARTNER_GENERAL_PLACEMENT_FALLBACK.md).
Called ray backend,trig/numeric/resource/source domains and original execution
remain open;full local recovery does not certify end-to-end equivalence.

## Entry ownership and headings

Inputs are partner P and output vector V. Capture owner M=P+160;retain that
pointer throughout. Capture maximum horizontal query length L=R(30+P+f4).
No null guard exists for M or V. Choose first base heading B and retained
facing F before initializing output:

- If unsigned owner motion M+2e==24,request marioPlaneGetFlyingDir. Result1
  sets B=0,F=180;every other result sets B=180,F=0.
- Otherwise B=revise360(R(180+current M+1ac)),then load F from M+1ac AGAIN.
  Do not replace that second load with the value read before revise360.

Write M+8c/+90/+94 to V in XYZ load/store order. This always happens before
slot gating,so failure does not preserve input V. Arbitrary source/output aliases
must preserve the interleaved order rather than copy an invented immutable XYZ.
With placement4's fall output pointer,V aliases the live partner root;this seed
replaces the output accumulated by that caller before general placement starts.

Sign-extend M+245. Negative skips the primary attempt and goes to the
fallback dispatch,which rereads this byte. Otherwise sign-extend M+246:
negative selects the single-slot primary path;nonnegative selects the dual-slot
primary path. These are current party indices described in
[slot ownership](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md),not a boolean count
or P's own slot. They are reread later;do not freeze them into one entry enum.

## Primary horizontal request and plane shortcut

Both primary paths first request toMovedir(B),then
[sincosf](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md). Cast direct
hitCheckVecFilter(&Q,null callback),origin(current V.x,R(37+V.y),current V.z),
direction(sin,0,-cos),length L. The direction's Y is exactly
the original copied +0 template;there is no computed signed-zero policy.
The common query Q begins at frame+130:origin+0c,direction+18,length+3c,
point+24,normal+30. No partner-specific callback filter is requested.

AFTER the horizontal query,reread captured-owner motion M+2e. If==24,
return1 immediately regardless of ray hit. No movePos,no downward query,
no floor classification and no floorY write occurs on this shortcut. V remains
whatever the seed and any permitted callback/alias effects have produced.
The initial motion24 heading path and this later motion24 check are separate
reads;one does not imply the other.

Otherwise nonnull horizontal H rejects the primary attempt and proceeds to
fallback dispatch. Null H requests an output move:

| Primary path | Angle request for movePos | Signed movement distance |
| --- | --- | --- |
| M+246 negative | toMovedir(F) | -30 |
| M+246 nonnegative | toMovedir2(F,A),A=-20 if signed P+2f==0,else30 | -30 |

Both change current V.x/V.z without another reseed. The
[movePos contract](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md) preserves original
angle conversion,trig rounding and fused X/Z writes. Primary dual-slot
horizontal and movement angles are intentionally different requests.

Cast downward directly with null callback from(V.x,R(37+V.y),V.z),direction
(0,-1,0),length150. This is150,not placement4's87. The common floor admission
below either snaps raw pointY and returns1,or continues to fallback dispatch.
Rejected primary movement remains in V until a later actual reseed.

## Exact floor admission,shared by all eight compiled attempt blocks

A call attempts at most one primary plus three fallback blocks;the eight blocks
cover both slot alternatives. All primary/fallback downward queries use the same inlined gate:

1. Null H rejects. Nested null tests after a proven nonnull are unreachable
   local checks;they do not request extra rays.
2. Request hitGetAttr(H);either0x200 or0x800 rejects.
3. Request hitGetAttr(H) AGAIN. If0x80000000 is set,request map-object owner,
   then compare owner+15 against the exact25 names/null table0x80314fe8 from
   [ground support](FIELD_PARTNER_GROUND_POSTPASS.md). Any match rejects.
   No map-owner null guard exists. The scan is inlined at each attempt;
   preserve query/classification order and live reads.
4. Read the downward query's CURRENT length at frame+16c. Accept only when
   ordered length<=100. The compiled cror combines LESS or EQUAL;greater
   and unordered reject under nontrapping continuation. This is clipped ray
   length admission,not a normal/slope angle or root-to-floor distance recompute.
5. Write raw downward pointY at frame+158 to V.y and return1.

No NPC scan,no normal inclination filter,no thousandth point quantization,
no surrounding-wall correction and no owner-height minimum are requested by
this local general writer. Placement4 adds its own minimum AFTER this call;
do not move that caller adjustment into this function.

## Used call sites and verification boundary

Seven-text-section opcode18 census finds eight direct linked DOL sites,no
nonlink references to this target. Annotation names identify containing bodies;
the census itself does not prove feasible execution or cover indirect/REL uses.

| Call site | Containing annotated body |
| --- | --- |
| 8005c378 | marioFBattlePost |
| 800b840c | party_damage_return |
| 800b8aa4 | partyRideYoshiMain |
| 800b9488 | partyRideMain |
| 800bb5c8 | partyHelloMain |
| 800c1260 | bomhei_use_main |
| 800c1a8c | bomhei_use_post |
| 800c61e0 | partyGetAppearPos4 |

Only placement4's argument/output consumption has been closed by this delivery;
other caller bodies remain separate contracts. Run
`python3 tools/verify_field_partner_general_placement.py` to check nine containing
hashes(eight complete local bodies,one selected placement4 caller),198 instruction
anchors,18 constants,three spans,two lecture imports and the exact eight-site
census. Hashes include already documented angle helpers;this adds no original
execution oracle. Generated original listings/ELFs/reports remain ignored.
