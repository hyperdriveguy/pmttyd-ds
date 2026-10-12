# Complete type5 nested resource-pointer clearing

US G8ME01 revision0; ID2,2026-10-12. Complete type5 Clear block8018b320..8018b4f0.
Verify `python3 tools/verify_file_type5_relocation.py`;read
[setup](FILE_TYPE5_NESTED_POINTER_SETUP.md),
[entry/type dispatch](FILE_RELOCATION_TYPE_DISPATCH.md) and
[file payload movement](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
This is full local behavior,not a complete resource-format or original-runtime
trace. Clear dispatch resolves file entry/type/holder/payload before this block.

## Unsigned root guard precedes captured signed count

Retain payload B. Read WORD B14c as ROOT_FIRST. If ROOT_FIRST<B unsigned,skip
entire handler. Otherwise read WORD B148 COUNT once into CTR;test signed COUNT.
COUNT<=0 skips record traversal but STILL proceeds to all25 root conversions.
Positive COUNT executes that captured number of iterations with full-word
O initially0,advancing40hex after each record. No live count reread,low16 index,
count cap,signature/version/size check or local call occurs in this handler.

Unlike Set,root pointers remain absolute during traversal. Root25 subtraction
occurs only AFTER all record/child operations. The guard's initial value is not
retained for the later root14c subtraction;that field is freshly read at the end.
This distinguishes type5 Clear from the flat handler's immediate first update.

## Retain absolute child before making the parent link relative

For each iteration,reread CURRENT B1ac as TABLE;compute RECORD=TABLE+O with
word wrap. Read WORD RECORD3c as absolute CHILD C and RETAIN it. Store
RECORD3c=C-B with word wrap BEFORE child inspection. No null skip or pointer-form
check protects the parent link. C remains the original absolute address even
though the table slot has just changed to a relative word.

Then read WORD C24 as CHILD_FIRST. Compare UNSIGNED to retained C. If
CHILD_FIRST<C,skip all child-field subtraction. Otherwise store C24=
retained CHILD_FIRST-C;visit C28,C2c,C30,C34,C38,C3c,C40 in order,reading each
current word,subtracting C with word wrap and storing. Child fields use C as
base,not root B. No fresh table lookup substitutes another child after parent
publication;physical aliases can still change the fields reached at retained C.

Parent conversion occurs regardless of the child guard's outcome. Zero C still
encounters C24 dereference after RECORD3c=-B publication;no graceful null-child
path exists locally. Valid source domains/format constraints remain separate.
Shared children can be converted on first visit then skipped by a later child
guard;there is no visited-object map or duplicate-record rejection.

## Root block is converted last using fresh words

After the captured loop ends (or initial COUNT<=0),read CURRENT B14c,subtract
B with word wrap,store. Continue B150,B154,...,B1ac in ascending4-byte order,
reading each live word before subtraction/store. There are25 root stores.
No second unsigned root guard is performed after child traversal;aliases may
have changed the first field or other root words since initial admission.

This root-last order preserves the table during traversal and deliberately
converts B1ac last. It does not validate pointers,lengths,type tags or remaining
raw bytes. Child operations may alias and change root/table data;do not normalize
those cases with a root snapshot or reversed child/parent store order.

## Set/Clear/move composition and implementation acceptance

Set converts root words first,then parent links using root B and children using
child C. Clear converts parent links first with retained absolute C,then child
words,then root words last. File movement requests Clear(E),memmove,new holder
publication and Set(currentPayload,signedType) in its separately recovered order.
Implement original32-bit byte coherence/modular arithmetic through those address
levels;stable header identity does not mean stable payload/child addresses.

No heap allocation/free,texture upload,cache/DVDClose or user callback is added
by type5. Resource contents are original reused bytes outside source history.
Native adapters require actual format/source domains and provider bindings;
local instruction evidence does not establish successful source-shaped movement.

Acceptance includes root guard equality,COUNT0/negative/captured mutation,
record stride/order/wrap,parent publication before child read,retained C despite
table changes,child-relative arithmetic/guard/shared aliases,zero fields,
root14c fresh reread after child mutation and root1ac last. Compare source-shaped
round trips separately from authored malformed/alias fixtures.

The complete Set/Clear local handler graph is documented across dispatch,flat,
type4 and these type5 contracts. Full source type/format provisioning/consumers,
platform panic/cache/thread/worker/scene,original traces and complete-game system
requirements remain open. No final resource-engine fidelity claim is made.
