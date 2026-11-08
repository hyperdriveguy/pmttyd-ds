# Original KillUnit event

`battleKillUnitEvent` implements whole US `btlevtcmd_KillUnit`80113304/b8.
It resolves/translates actor0, captures the fresh global battle and registry
unit, then captures raw mode1 **after** lookup, including a null result. Null
unit returns2. Otherwise it stores current HP(+10c) zero and delegates to the
existing complete ClearStatus helper80127b58/54. No result or conclusion is
inferred from that HP store.

Mode bit1 (value2) reads unit+104, ORs20000 and returns2. Otherwise the actual
SetStatus(unit,27,1,1) branch writes byte+135=1; mode bit0 (value1) suppresses
the subsequent attribute write, while its absence reads unit+1c and ORs80000000.
The pinned SetStatus jump table80364268 maps index27 to80127f1c. The raw mode
is captured before status clearing, so mutations during clearing cannot change
the branch. These are source bits; no additional renderer visibility predicate
or native removal/completion shortcut is introduced.

The exact journal also corrects the previously ported ClearStatus charge16
subpath: the original stores the wrapped accumulated byte(+129), **rereads** it,
then conditionally stores0 for negative or99 above99. The prior helper folded
that store/read/clamp into one cached result. That produced the usual final
value but omitted a real transient write and could hide memory callback
mutation. The correction preserves the existing helper ownership and source
change-info loop. Tests cover replacements0/77/120/200 at the transient store;
unused change-info word8 stays untouched.

This is an atomic memory-only leaf with admitted total synchronous services.
Both service tables must address the same actual VM. Missing actual memory is
not a zero/default success. No asynchronous callback, renderer, Pouch or audio
acknowledgement is required or fabricated. The native adapter binds both tables
to its actual EventNativeRuntime. No guest stack or effect pointer is invented.

Focused validation checks the complete parent/linked-memory operation trace,
all low mode combinations and high bits, null-unit closed branch with unused
write providers absent, raw mode capture after registry and before clear, and
fresh status rereads. The separate coherent15584 actual lethal CPU fixture
includes previously frozen death/effect leaves. GPU/resource admission there
is explicitly TESTONLY. It reaches source conclusion1 with Crump0/Mario4,
turn4, actual EXP stock9 and coin stock0, with no pending service/attack/damage
or phase; scene.result remains0. It does not execute the original reward/End
sequence or authorize field return or Pouch award.

The verifier reads the ISO only and pins whole parent, registry, ClearStatus,
SetStatus and change-info bodies plus the actual dispatch table. Parent must
run fresh matching-header full `make test` and `make all`; focused fixtures do
not replace whole-graph/native gameplay checks.
