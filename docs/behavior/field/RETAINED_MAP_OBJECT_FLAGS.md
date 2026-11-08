# Retained original recursive map-object flags

`resumePackedMapObjectFlag` implements the MapObj memory effects of complete US
mapFlgOff8001eee8/314 and mapFlgOn8001f1fc/440. The verifier pins both full
original bodies in the read-only ISO. Native product code contains typed
traversal frames, no instruction decoder, original opcode blob or invented guest
stack pointer.

## Actual source memory order

Off reads object word0 and writes oldFlags AND NOT(mask). On ordinarily writes
oldFlags OR mask. When On's mask includes100hex, each visited object first reads
word114; zero skips **that object's entire OR**, including any other mask bits.
It still traverses children/siblings. With that mask bit absent, word114 is not
read. No guessed visibility/eligibility test replaces this condition.

Each object reads child pointere4 after its own conditional flag mutation. It
completes that subtree before reading its sibling pointere8. Root siblings are
visited only if the captured third argument is nonzero. Descendant traversals
include their siblings. Pointers are captured only at their source load point:
a later callback cannot replace an already captured pointer, while a not-yet-read
child/sibling remains live source memory. A zero initial object is not silently
accepted as empty; it reaches its actual memory read and remains unavailable if
that source memory is unavailable.

The original compiler unrolls three levels before self-recursion. Native frames
preserve the exact MapObj load/store stream and traversal order without exposing
compiler register-save layout as a product ABI. Actual source MapObj allocations
must be distinct from the original call's private register-save stack. Native
frame storage does not claim original SP/history bytes or arbitrary stack alias
support. Internal pending PCs identify corresponding root transfer classes, not
which compiler-unrolled instance was reached.

## Retention

The existing FieldMapSourceServices word/store callbacks can hold each transfer.
False transfers leave that source transfer unaccepted. Flag read and write are
separate retained steps, so a held store does not reread flags or replay an
accepted earlier store. Frames pin captured object/mask/sibling policy and
provider/lifetime identity. Reentry, provider replacement and observed owner
retirement are unavailable. A transfer budget yields without resetting progress
or advancing simulation time. No node-count clamp, cycle repair, normalized
flag mask or synthetic source object is introduced.

## Composition and remaining native owner

The whole group parent may bind its real mapFlgOff/On request to this child,
passing actual MapObj address, mask and third argument while retaining that
same child cursor across waits. The composition test executes the frozen group
parent through a held real deep child and proves its earlier name comparison
runs once. It supplies no native overlay substitute for the source word stores.

A proper process map bank, heap, original joint/name arrays and deferred-name
queue remain separate actual providers. Existing FieldMapSourceOwner and source
bmapLoad are reusable producers; they are not instantiated by this module.
Native Stage11 geometry can consume flags only through a later proven matching
mesh/hierarchy/current-resource adapter. No Game activation, source scene-ready,
GPU draw, complete original map loading or Crump playthrough is claimed.

## Test evidence and its limits

The host test reads the exact two source bodies from the read-only US ISO
(`TTYD_ISO` overrides the local default) and runs a TEST ONLY bounded instruction
oracle. Every MapObj read/store is compared with native retained frames; source
register saves use isolated test stack storage and are excluded from the MapObj
comparison. The test decoder is never a production dependency. This is neither
Dolphin execution nor an original runtime/framebuffer observation.

Cases cross the compiler unrolling boundaries with deep and branched objects,
root sibling arguments0/1/-1, mask0/1/100/102/allbits, and conditional word114
zero/nonzero. Every admitted memory transfer is held twice, plus small-budget
yields, reentry, retirement and group-parent composition. Fully instrumented
ASan/UBSan, strict ARM syntax and independent whole-body hashes are separate
checks. Fixture memory/backend readiness is not normal process/GPU ownership.
