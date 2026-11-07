# Retained group traversal and resource requests

[dispProc](../../source/display/animation_group.cpp) walks the actual source
visibility, node and group graph. Its heap frames keep parent operands stable
across child holds and vector growth. The numeric phase labels and default
128 visits are native scheduling choices, rather than rendering frames or a
source depth limit. Resume with identical providers and request, including exact
fraction bits: +0 and -0 are different retained inputs. See
[the actual-VM contract](../behavior/display/NATIVE_ACTUAL_VM_GROUP_TRAVERSAL.md).

Offsets below are hexadecimal; sizes and strides are decimal. W+20 is MODEL,
W+18 is the current pose, and MODEL+1a8 points to 88-byte groups. Each group has
sibling40, child44, shape48, visibility index4c, node index50 and kind54.
Only group -1 is empty. There is no source cycle, depth or index clamp.

Visibility uses the current P+5c output bytes. Any nonzero byte is visible,
including ff. Zero skips this node's push, outputs, child and pop, but still
visits its sibling. Visible nodes use P+64 plus four times the node index.
Joint parents obtain scale from their node floats+c/10/14; absent or nonjoint
parents use the source ONE literal. SDK node builders remain explicit children.

After the node child, flags40, 80 and 20 each independently reload current global
W/pose. The captured group address stays unchanged through callbacks. Flag40
exports translation through the P+15c holder into 12-byte rows; flag80 copies
current modeling matrices through P+164 into 48-byte rows. Literal seed reads
still occur even though translation overwrites their local words.

A matched flag20 takes the special material/Vivian route. The table at
`0x80415db0` is indexed before kind classification; kind4 can alias the work root.
The saved matrix is inline P+124, not a loaded pointer. Material runs before
parameter-pair reads and the effect. After the effect, the code reloads current
W/pose, clears only flag20 and blends nine basis lanes using source .04f.
Each lane reads current matrix then saved matrix, rounds the subtraction and
requests fmadds. Earlier stores can affect later aliased reads. Translation
lanes+c/1c/2c remain untouched.

The ordinary path calls P+154 callback with P+158 data, group and modeling
pointer; then renders the freshly read captured-group shape and follows its
child. Pop decrements global level before publishing stack base+48×level, with
no underflow clamp. Finally both visible and invisible nodes follow the sibling.
Both child and sibling receive the CURRENT group as parent. Conventional tree
walkers that pass the incoming parent to siblings change this source behavior.

## Resource polling is a separate retained invocation

[animGroupBaseAsync](../../source/display/animation_group_request.cpp) pins its
name, mode, completion and providers across holds. Its default 8192 visits is a
native work budget. Modes0/1/2 choose archives1/2/0; other modes skip that setter.
Type5 requests `%s/%s` with prefix `a` and caller completion. Only after that
request does it read live W+100 → FileEntry+a0 → holder, whose first two words
are the ag2tg payload and signed byte length.

The scan advances in 32-byte name rows and selects the following row after the
first strcmp match. No match uses the source empty string. Type4 then always
requests `%s/%s-` with completion zero, so a miss requests `a/-`. Both requests
happen before readiness is chosen: MODEL -1 yields1, MODEL0 yields0, otherwise
TEXTURE nonzero yields1. Texture -1 counts as nonzero. Provider unavailability
is separate from a completed zero result; completion resets the cursor so the
next poll resolves fresh operands. Raw scanning supplies no native row bounds
repair. The file/native backing owner must admit the actual supported domain.

The [source graph contract](../behavior/display/ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md)
and [native prefetch contract](../behavior/display/NATIVE_ANIMATION_GROUP_PREFETCH.md)
separate these raw parents from bounded caches, file lifetimes and GPU children.
Complete local traversal or request readiness alone does not prove full renderer,
source scheduling or original visual parity.
