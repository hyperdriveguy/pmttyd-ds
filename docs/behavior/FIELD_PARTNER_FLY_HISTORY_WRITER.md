# Flying history writer: slot sources,append guards and ordered publication

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyFlyMain0x800c8ad8,size0x3f0. Actual callers are the
[Cloud/Vivian callbacks](FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md),which then
consume its movement/presentation effects. Every writer path ends by requesting
[flyMain@party_move.o](FIELD_PARTNER_FLY_DISPATCH.md)(P) at0x800c8eac,
including rejected appends. No original executed history cadence is measured.

## Two shared banks and three source paths

These are the same buffers used by
[walking history](FIELD_PARTNER_HISTORY_WRITER.md),not separate flying arrays.

| Bank | Record base | Read word | Write word |
| --- | --- | --- | --- |
| 0 | 803ec2c8 | 8041eae8 / SDA+1bc8 | 8041eaf0 / SDA+1bd0 |
| 1 | 803ec7c8 | 8041eaec / SDA+1bcc | 8041eaf4 / SDA+1bd4 |

Each bank has80 records,stride16:flag byte+0,padding+1..3,float XYZ words
+4/+8/+c. The writer reads SIGNED slot byte P+2f,not the consumer footmark bit
or initializer bank bit. Slot==0 selects primary path;EVERY other signed value
selects secondary selection,no range check. Source reads are:

| Path | Source/flag/requests | Written bank |
| --- | --- | --- |
| Slot0 | marioGetPtr G0:word+0 mask0x00010000→F=1 else0;fresh G1 raw XYZ+8c/+90/+94;fresh G2 queried but its pointer result unused | 0 |
| Nonzero slot,fixed party0 exists | anotherPartyGetPtr(signed slot) requested,return UNUSED;partyGetPtr(0) gives Q;Q word+0 mask0x10→F=1 else0;raw Q rootXYZ+58/+5c/+60;global Mario queried but its pointer result unused | 1 |
| Nonzero slot,fixed party0 NULL | Same two party pointer requests,then G0/G1/G2 pipeline as primary | 0 |

The preliminary anotherPartyGetPtr result does not become the chosen Q. No
status3 or distance150 rejection is requested by this writer. Secondary null
fallback writes bank0,unlike bank1 initialization's Mario-coordinate fallback.
Preserve the unused global/pointer requests and their order;do not cache all
Mario pointers into one identity without proving source/ABI equivalence.

## Append admission

After capturing source words and F,load old write index W and locate its record.
The first guard is literal:if old flag bit0 set AND captured F bit0 set,reject
WITHOUT coordinate comparisons. Otherwise compare old XYZ versus captured
XYZ with fcmpu,equality in X then Y then Z order. All three equal rejects.
Any unequal or unordered component reaches capacity admission;there is no
position epsilon or forced append on every invocation. Signed zeros compare
numerically equal despite different raw words;NaN does not satisfy equality
under nontrapping continuation.

Load selected read index R and compare it to32-bit W+1 BEFORE wrapping.
Equality rejects. If not equal,reload CURRENT write word,add1 with original
32-bit wrap,store it,then compare the new value SIGNED with80. Values>=80
are replaced by0;negative values are not lower-clamped. Reload write word
again to locate the destination record.

The full-buffer guard is not conventional modulo capacity:W=79,R=0 compares
0 to80 and permits the later wrapped0 write. This is static branch composition,
not proof that this index pair occurs in retail play. Do not wrap W+1 before
that equality or add a capacity policy absent source-domain proof.

## Publication and flag4 is locally always cleared

Write captured raw X,Y,Z words to the destination at+4/+8/+c in XYZ order.
Then reread flag byte and set/clear ONLY bit0 according to F bit0. Reread it
AGAIN and set/clear ONLY bit2(mask4) according to F bit2. Other flag bits and
padding remain unchanged. Do not overwrite the whole flag byte as F.

Every source path initializes F=0 or1 and never locally ORs4 into it. Therefore
the compiled bit2-set branches are unreachable in this writer under its local
register definitions,and every admitted append CLEARS destination flag4.
The branches still exist in the instruction graph;no guessed airborne/height
flag producer is substituted. Other history writers can have different flag
producers and ownership. If append rejects,old flags/coordinates remain unchanged.

No read index advances in this body. After append or rejection,request flyMain(P)
once. The caller's root can then change through that executor's bound handlers;
this writer's snapshot/source words are captured before that movement request.
The full source/index/flag/alias/FP domains and original trajectories remain open.

Run `python3 tools/verify_field_partner_fly_history.py`:seven complete local
hashes,467 anchors,ten constants,two kind-record spans,two US lecture imports
and four actual direct writer/initializer call sites. Original listing/ELF/report
stay ignored. This supersedes the earlier selected-tail-only writer coverage;
no complete flying subsystem or original-runtime parity is claimed.
