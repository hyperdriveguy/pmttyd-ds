# Harbor retained source command UI

`HarborSourceCommandUi` composes the existing complete retained Input
`801229d8`, Act Main/Setup/End, ClassTable, Weapon Main/Setup/End, their actual
UI constructors/destructors, and physical-X audience check. This is a native
provider composition, not another battle sequence controller. Its engaged
return completes one original Input invocation; the simulation owner decides
when to admit the next invocation and sample physical input.

The Packed registration journal adds `bindSourceCommandUi` once per actual
runtime, `resumeSourceCommandInput(scene)`, and `sourceCommandInputPending`.
The Main/Act scheduler must delegate the original Input child to that method.
The old `commandInput`, `openClassMenu`, `openWeaponMenu`, and Presented helpers
remain legacy harness interfaces and are not used by this owner. In particular,
this owner never writes an acting ID, menu state, selected class, or readiness
on behalf of a missing source child.

## Original ownership

Actual registry/status/cursor/help/restore and pad primitives operate on the
supplied actual VM. Trigger, repeat, and held words are B+`f44`, B+`fbc`, and
B+`1034`; directional controls and A/B/X remain original physical paths. No
command touch rectangles or synthesized X input are introduced. The original
physical-X check includes good-item holders, as before; optional targeted
harmful audience taps are a separate source interruption entry.

The ClassTable producer has its own retained rows/message cursor. A message
callback must return an actual VM-mapped original US string, preserving its ID
and control bytes. Unknown original message lookup stays unavailable. Empty or
fabricated labels cannot complete that child. Class/weapon UI construction
uses the original MapAlloc `8002ff58`/MapFree `8002fdc4` children, not Harbor's
old SDK-heap substitution or fixed MenuRam weapon projection. Class constructor
callbacks capture the real heap root from `8041e80c` before holding. The native
Map provider installs a fresh allocation token after real allocation and retires
it before real free. It requires genuinely admitted allocator history; it does
not construct that history. Allocation0 remains the original unchecked return,
not a manufactured unavailable result or repaired allocation.

The new source bodies are complete TargetSetup `80120fe4/58` and TargetEnd
`801209e8/5c`. Setup resolves the acting unit, stores M4 OR `0x10000`, resolves
body ID and part at their actual source points, and retains the real
SetAnimationType(part,40) child. A held pose copy cannot repeat the preceding
flag store. End performs the original cursor lookup whose private result is
unused, retains CameraMode(0,0), then independently reloads and clears
`0x10000` and `0x01000000` in M4. No synthetic guest stack pointer is supplied.
The complete display/light leaves acknowledged locally are verified original
`blr` bodies. These are distinct from queued drawing and registration.

## Lifetime and waits

The Packed object owns the UI; the UI only weakly registers its actual runtime.
Each entry locks that registration before inspecting actor/provider userdata,
and holds it through all callbacks. Its immutable binding is not replaceable.
A separate pure current predicate proves actual source storage ownership. Lost
registration/currentness poisons the owner. Recursive entry is rejected before
any memory/provider access. No reset can replay previously successful source
work. Actual child providers retain their own partially completed state;
returning unavailable is not a promise that the caller can restart them.

Parent controllers retain function/site/arguments and native private outputs.
Class and weapon cursors, UI work, allocation identities, and the audience
cursor are separate. Unsupported cursor lookups complete without writing the
native optional output; an actual later consuming source site holds if it is
still unwritten. Written0 remains known original data.

## Native integration frontier

Root-owned cold battle entry must provide real Map allocator state, original
UnitSetup/Init/FirstAct history, shared source resources, and source numeric
providers. Its stable individual-child callback must bind actual PresentItem,
command/target pose, weapon table/update/decision, event creation/query,
camera, message lookup, and menu sound as they are reached. Successful source
sound means the original logical pending-slot start completed, including real
failure returns; appending a request vector is insufficient. The former Harbor
weapon table/decision wrappers are not safely retryable after partial effects
and must not be installed as a generic retry adapter. Retained closures are a
separate next increment.

The Packed adapter projects class/weapon/target visibility only from actual
source M4 flags and copies actual existing rows. Original queued drawing must
own W+14 readiness. A real Harbor camera/frame/resource binding remains
required: the existing Trio frame cannot be relabeled as Harbor, and drawing
or GPU import cannot certify missing source Main/Draw. This batch does not
claim normal tutorial command activation or a completed Crump battle.

## Validation

Focused host tests compare immediate composition with every reached external
child held twice for source29 finalization, physical radial navigation, and
weapon cancel/class reconstruction. They compare complete memory/write/call
traces and reject recursive entry. Additional tests cover source-readiness0,
retained TargetSetup pose40, fresh TargetEnd flag reads after camera holds,
weak runtime retirement and currentness loss. These use explicitly supplied
TEST actor/table/audio children, not production activation receipts.

Strict host, ASan/UBSan (LeakSanitizer disabled because this executor uses
ptrace), ARM syntax including the narrow Packed postimage, and the read-only
ISO source verifier are recorded under `artifacts/harbor-source-command-ui`.
The focused execution does not instantiate the changed Packed/Game ABI from
an old archive. Actual factory/runtime execution and full `make test`/`make all`
require root's complete fresh graph after applying the header journal.
