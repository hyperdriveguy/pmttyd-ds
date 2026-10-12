# Complete flat file-pointer relocation handlers

US G8ME01 revision0; ID2,2026-10-12. Complete Set/Clear local handlers for types
1/2/3/6/7/8/9/10. Verify `python3 tools/verify_file_relocation_dispatch.py`;read
[dispatch and argument domains](FILE_RELOCATION_TYPE_DISPATCH.md),
[moving payload contract](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
No original relocation/runtime trace or complete resource-format schema.

## One unsigned first-field guard controls the whole selected block

Retain payload base B supplied by dispatch. Read first field WORD(B+FIRST).
Set compares it to B UNSIGNED. If first>=B,skip the WHOLE handler. Otherwise,
visit every listed field in ascending offset order:read current word,add B with
32-bit wrap,store word. The first update uses the value retained from the guard read;later fields
are read immediately before their own arithmetic/store. No per-field guard exists.

Clear compares the first field to B UNSIGNED. If first<B,skip the WHOLE handler.
Otherwise visit every listed field in ascending order:read current word,
subtract B with32-bit wrap,store word. The first update again uses the retained guard value.
Equality is asymmetrical:Set skips;Clear processes. No calls,loops,allocation,
flush/state/tag or relocation-marker writes occur in these flat blocks.

| Type | FIRST | All visited word offsets in order | Word count |
| --- | --- | --- | --- |
| 1 | 14c | 14c,150,...,1ac (step4) | 25 |
| 2 | 24 | 24,28,2c,30,34,38,3c,40 | 8 |
| 3 | 48 | 48 | 1 |
| 6 | 48 | 48 | 1 |
| 7 | 64 | 64,68,6c | 3 |
| 8 | c8 | c8,cc,...,120 (step4) | 23 |
| 9 | 58 | 58,5c,60,64 | 4 |
| 10 | 48 | 48 | 1 |

All offsets are hexadecimal. These are raw words treated as offsets/pointers by
arithmetic;no field names,asset kind or bounds/validity schema is proven here.
The first field acts as the local relative/absolute heuristic,but later fields
are not checked individually. Unknown remaining bytes are untouched by these
blocks unless raw address aliases overlap visited words.

## Null/mixed/wrapped field behavior must not be normalized

No per-field zero preservation exists. For an admitted Set with B!=0,a later
word0 becomes B. For admitted Clear,a later word0 becomes -B modulo2^32.
A first word0 with positive B admits Set;Clear skips. First=B skips Set but
admits Clear,which writes its first field0. Mixed absolute/relative words still
receive uniform arithmetic according to the FIRST guard. A null B is not
rejected;its Set guard always skips and its Clear branch adds no difference
while performing its literal reads/stores.

The heuristic need not remain reliable for arbitrary wrapping/malformed source
states. Do not strengthen it into an explicit relocated Boolean,null-safe
pointer resolver,range validation or per-field idempotence rule. The original
source may constrain these inputs;those domains remain to be recovered.

## Moving resource composition and implementation acceptance

A ready smart file move calls Clear with its ENTRY argument,resolves B through
its live holder,converts admitted words to relative form,memmoves the bytes,
publishes the new holder.word0 and calls Set on its current payload/type. That
composition depends on actual selected fields/source domains and preserved
addresses. It does not justify rewriting the payload into a different schema or
assuming each raw word is always a valid native pointer.

Implement these operations in the original32-bit address space. Native backing
memory can use an explicit relocation/address provider,while retaining modulo
arithmetic,first-field guard/retained first value,field order and later live reads for aliases. Reuse original
asset data outside source history;this arithmetic is not texture scaling/model
reduction or a changed story resource policy.

Acceptance includes each type/all fields,first below/equal/above B,unsigned-high
values,Set/Clear retained first value,zero later fields,mixed states,overflow/underflow,
base0,field aliases and bytes outside selected blocks. Verify source-shaped
clear→move→set chains separately from authored edge fixtures. Instruction/hash
checks establish local bodies,not successful original resource execution.

Types4/5 nested traversal,source format/type producers/consumers,TPL/animation/
platform/worker/scene and original traces remain explicit EVT-01/JUMP-01/DMG-02
requirements. No complete relocation engine or full-game requirements claim.
