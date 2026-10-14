# Ordinary party entry: first appearance capture order

2026-10-13 US G8ME01 disc0 revision0 read-only DOL audit. This supplements
[constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md) with the exact missing
ordering between EntryMain completion and first appearance, and its caller's
index publication. Animation/HP initialization has separate
[admission](FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md); the placement helper's
complete local policy is [appearance2](FIELD_PARTNER_APPEAR_POSITION2.md).

## EntryMain return through first placement

`partyEntry`800bea68/1b8 calls partyEntryMain800be144 at800bea84, retaining
returned signed index in r29 at88. If nonnegative, it resolves the pointer
from table8041ead8[index] at800bea90..98; negative selects null atbeaA0.
Subsequent stores are unconditional: this wrapper provides no safe failed
constructor result. The valid allocated slot/owner domain is required.

The exact ordered prefix is:

| Instruction address | Request/read/write |
| --- | --- |
| 800beab4/bc/c8 | Read three WORDs from source vector802cb178 |
| 800beac0/c4/cc | Copy those WORDs to P70/P74/P78, respectively |
| 800bead0 | Store BYTE P38=10 |
| 800bead4 | Capture owner pointer M from P160 in r5 |
| 800bead8/dc | Read FLOAT M1ac; store FLOAT P110 |
| 800beae0/e4 | Read FLOAT SAME retained M1ac again; store FLOAT P10c |
| 800beae8 | partyGetAppearPos2_800c6718(P,&P58) |

Source vector802cb178/0c is raw3f8000003f8000003f800000, hence(1,1,1).
The two source angle loads are separate, although there is no intervening
function call. They use the pointer captured from this actor rather than
marioGetPtr. They are float copies without normalization. No callback,
partyChgRunMode, trig request or other service occurs between EntryMain return
and the first placement request. EntryMain's earlier kind initializer has
already run; ordinary kuribo_init initializes Pf0/f4 before these stores.

GetAppearPos2 independently captures M=P160 and reads/rereads M1ac as its
own body specifies. Caller P110/P10c are not replacements for those helper
reads. Its integer result is ignored. Even placement failure is followed by
raw P58/P5c/P60 copies to Pb8/Pbc/Pc0 at800beaf8/fc/beb04.

Only AFTER placement/copies, partyEntry processes the original pose selector:
P4 mask2 bypasses selection; otherwise signed kind<8 queries actual HP
marioGetPartyHP80140ea4 at800beb28, HP<=5 selects12, else1. Other kind/flag
branches remain in the original body. For ordinary kind1 the source keys are
PKR_D_2/PKR_S_1. A new/null or differing key writes P18, clears HALFWORD P20
and ORs P8 mask2000 at800bebdc..f0; same string preserves those fields.
At800bebfc it writes BYTE P34=1 and returns the retained original index.
No Hello request or Mario current-index publication occurs inside partyEntry.

## Binding the existing native angle/root owners

The [retained display direction authority](../overworld/OVERWORLD_MARIO_DISPLAY_DIRECTION.md)
separates M1a0 facing, M1ac current display turn and M1b0 target. In
include/display/mario_presentation.hpp these correspond to MarioPresentation.facing,
.display and .target respectively. Therefore the two caller M1ac loads bind
**raw .display**. Do not bind .facing, .target, moveHeading(), reflectZ(), or
wrap(display-offset) from drawYaw(); source caller applies no angle conversion.
The helper's own angle requests still operate on current raw M1ac as specified
in its separate contract. Current display can be negative or exceed180 during
source turning; replacing it with a canonical left/right value loses state.

M8c/90/94 are the unoffset root, distinct from render-root offset composition
[documented here](../overworld/OVERWORLD_ACTOR_RENDER_SCALE_AND_IDLE_BOUNDS.md).
Existing Game field service adapters at source/runtime/game.cpp (NPC talk, wander,
side-collision and camera bindings) provide engine units as
{progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100}.
This is the existing native coordinate adaptation, not an original source
writer or measured physical conversion. Ordinary dry field/trio restoration
may use that same actual persistent root owner. Game water-active camera
bindings use water.position[1] instead; that special owner must not be silently
covered by an ordinary ground+jump proof. Draw offsets, camera shifts and
sprite atlas pixel centers are not M8c/90/94. No new facing/root values or
battle-event coordinates are inferred by this binding audit.

## Caller publication and immediate second appearance

`marioPartyEntry`801412fc/110 captures its Mario owner at80141318/320.
For ordinary kind1 its source pouch row must already admit flags&1; existing
M245 removal precedes EntryMain. The used call/return tail is:

1. At80141384 call partyEntry(kind). At388 retain signed returned index;
   negative skips publication and Hello, returning that index.
2. At80141390 store LOW BYTE index into captured M245. This occurs after
   the entire first appearance, position copies and pose selection.
3. At80141394 call partyGetPtr(index)800bf1b4 using the returned integer.
4. At80141398 call partyHello(pointer)800bbc48, then return retained index.

There is no new global Mario capture between steps1/2. Current M246 and
slot1 Frankly remain separate owners. EntryMain has already published the
allocated actor in the registry before this first appearance; Mario245
publication occurs later. Thus the first helper may see the new registry
actor with the prior Mario245 value. Do not publish Mario245 early to simplify
native placement.

Fresh kind1 P8 lacks mask8. Therefore partyHello tests P8 at800bbc5c/60,
branches directly to800bbdc8 and calls partyHelloMain800bb544, without any
of Hello's conditional motion/scale/angle reinitialization. Original substate
P38=10 dispatches to800bb9ec: at800bb9f0 it calls GetAppearPos2 AGAIN into
P58, then copies XYZ to Pb8..c0 and performs its own current HP/pose selection.
The first helper cannot be deduplicated with this second helper; it runs
before Mario245 publication, and HP/trig/ray providers are called again.

A native implementation can bind this ordered scalar prefix without an
invented partyChgRunMode call. It must preserve P160/current owner angle and
source placement requests; guessed facing0 or constructor-success placement
is outside the closed source domain. The two independent placement results
remain ignored just as the original caller ignores them.

## Evidence

Complete raw DOL body SHA256, with original address/size(hex):

| Body | Address/size | SHA256 |
| --- | --- | --- |
| partyEntry | 800bea68/1b8 | 7b60eab9d944bb30cd847690a210ace406295d7301f8b0dee76d0096b666f831 |
| marioPartyEntry | 801412fc/110 | d3c9137c93eab1c7d3acfda0d3abacebd556d2118f7c9150c2bfd1378c58a641 |
| partyHello | 800bbc48/194 | 4603f3c14407f2d67a9379643d0044a8fceebc2a722f07954f31a3a02aec0aff |
| partyHelloMain | 800bb544/704 | 87025b990033f296bb28df701c7cb970e4f1d3fd07f013c81102778e75c210b0 |

Vector802cb178/0c SHA256
e852d0ceaa6a4d8ae6859337b6e3d25a56b1bef05a492d34283afabbdfe4db36.
Complete listings stored only in ignored artifacts/recovered-temp-evidence/
party-entry-order*. Original ISO opened read-only. Local branch/store/call
order verified against PPC; scoped diff check passed. Documentation only;
no native/test edits, shared docs changes, build rerun or commit.
