# Complete type5 nested resource-pointer setup

US G8ME01 revision0; ID2,2026-10-12. Complete type5 Set block8018ac80..8018ae54.
Read [clear](FILE_TYPE5_NESTED_POINTER_CLEAR.md),
[dispatch/flat routes](FILE_RELOCATION_TYPE_DISPATCH.md),
[type4](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md) and
[file move](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
`python3 tools/verify_file_type5_relocation.py` pins complete Set/Clear bodies
whose remaining handlers are supplied by those linked contracts. Format names/
consumers/panic/platform/source/runtime are separate;no original execution claim.

## Root first-field guard and25 ordered conversions

Retain payload B from Set dispatch. Read WORD B14c as FIRST. Compare UNSIGNED
FIRST to B. FIRST>=B skips the WHOLE type5 handler,including count/table/children.
Otherwise store B14c=retained FIRST+B with32-bit wrap. Visit remaining words
B150,B154,...,B1ac (step4),each read immediately before adding B and storing.
Thus25 root words14c..1ac are converted in order. No per-word null/range guard,
relocated byte,signature check or service call occurs.

COUNT at B148 is not part of this root field list. B1ac becomes the record-table
pointer used below. Field offsets/pointer roles are recovered accesses,not a
complete named model/animation schema. No arbitrary type5 asset enum is inferred.

## Captured signed count and full-word record offset

AFTER root conversion,read WORD B148 once into CTR. Interpret the same word as
signed for the initial admission:COUNT<=0 exits after root writes. Positive COUNT
executes exactly that captured number of iterations using decrement-and-branch.
It does not reread COUNT each iteration or use a low16 index as type4 does.

Initialize full-word record offset O=0. Each iteration processes field O+3c in
the live root table B1ac,then adds40hex to O with32-bit wrap. Records therefore
have64-byte stride under ordinary source provisioning. No count cap,table size,
record-null check,overflow check or cycle detection exists. A live COUNT change
inside a record/child alias does not change the already captured CTR.

## Parent link is root-relative;child fields are child-relative

Reread B1ac as TABLE. Read WORD(TABLE+O+3c) as Q. Add B with word wrap and store
to that SAME TABLE slot. There is NO Q==0 skip or pointer-form test:relative0
becomes B. Then reread B1ac and that slot as CHILD C. Aliases can change which
child is inspected;do not substitute the earlier arithmetic result for C.

Read WORD C24 as CHILD_FIRST and compare UNSIGNED to C. If CHILD_FIRST>=C,skip
all eight child-field conversions for this record. Otherwise store C24=
retained CHILD_FIRST+C. Then visit C28,C2c,C30,C34,C38,C3c,C40 in order,reading
each current word,adding C with word wrap and storing. Child data use C as base,
not root B or record-table base. No per-field zero preservation or size check.

Even when the child guard skips its fields,the parent slot was already converted.
Shared child pointers can cause later child-field guards to skip after an earlier
conversion;parent links are still visited for every admitted record. A child
aliasing root/table/other children can change subsequent live reads.

## Operation order and native requirements

Root25 stores precede count capture and all record work. Each parent store
precedes a fresh table/child lookup;child fields finish before O increments.
No user callback,heap/free,texture decode,cache flush or DVD request occurs in
this block. Root/record/child address identities must remain coherent with file
entry→holder→payload and the moving arena's original32-bit address provider.

Acceptance covers unsigned root below/equal/above B,zero/mixed/wrapped root
words,COUNT0/negative/positive/captured changes,record offset wrap,parent0,
parent/table aliases,child below/equal/above C,child0 words,shared/cyclic/aliased
children,retained first values versus later fresh fields,and root-base versus
child-base arithmetic. Invalid domains may be reported explicitly by a host
provider;do not label authored fixtures original source/runtime observations.

Reused original bytes remain outside source history. Full source format/type
producers/consumers,TPL/animation/model binding,panic/platform/worker/scene and
original movement/rendering traces remain EVT-01/JUMP-01/DMG-02 requirements.
All local type routes can now be implemented from linked contracts,but complete
resource loading/rendering and full-game requirements remain unproven.
