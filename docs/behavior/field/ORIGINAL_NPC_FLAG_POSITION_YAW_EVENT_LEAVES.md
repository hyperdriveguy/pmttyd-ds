# Original NPC flag, position and yaw event leaves

Read-only static US G8ME01 revision0 DOL evidence for the next
[real NPC setup world](NATIVE_NPC_SETUP_WORLD_ADMISSION_REQUIREMENTS.md).
Original actor storage is described in
[retained registry ownership](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md).
These callbacks do not create actors, implement query/Main/facing, or map
unregistered idle-render models into original NPC identities.

All five callbacks capture EventEntry+18 argument-array pointer once and
return handler2. They do not branch on the first-call argument or implement
an animation/resource wait. Native missing-provider holds must retain progress
or pre-admit complete synchronous effects; source return2 must not acknowledge
an unbound actual actor/world provider.

## Common evtNpcNameToPtr resolver and actual absence domains

All five leaves call evtNpcNameToPtr8004f5d8/168 with the current event and
resolved name-string pointer. It captures both arguments and performs strcmp
in this exact order, stopping at the first equal string:

| Literal string / original address | Result |
|---|---|
|me /8041fd80|fresh event+170|
|party /8041fdcc|ffffffff (-1)|
|(^x^)party /802c14f0|ffffffff (-1)|
|extparty /802c14fc|fffffffe (-2)|
|mario /8041fdd4|fffffffd (-3)|
|master /802c1508|fresh event+170, then actor+32c|
|slave_0 /802c1510|fresh event+170, then actor+330|
|slave_1 /802c1518|fresh event+170, then actor+334|
|slave_2 /802c1520|fresh event+170, then actor+338|
|slave_3 /802c1528|fresh event+170, then actor+33c|

For every other string, call release npcNameToPtr800499e0. Registry selection
inside that helper follows fresh GP+14 and lookup uses active1, independent
of query2 and render-hiding flag800. Missing release lookup returns its
exhausted scan pointer, not0 or a defined assertion result; see
[constructor/lookup source](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md).
The special comparisons precede ordinary lookup, even if a registered actor
happens to have one of those literal names.

The resolver itself does not validate null name strings, event+170, associated
pointers, sentinel results, or source ownership. These five leaves subsequently
dereference the result as an NPC pointer; they contain **no** Mario/party
negative-result dispatch. Native admission must therefore require a genuine
owned NPC target for these operations, including context/association modes.
Do not silently substitute Mario/current party for negative sentinel results
or turn missing pointers into successful no-ops. Other original callbacks
may interpret the sentinels differently; that is outside these five bodies.

The actual init/regular roots created by npcSetup have context+170 set to the
registered actor and use the original literal me. That publication is a real
producer, unlike a finite catalog's idle model or guessed current actor.
String comparisons and associated-pointer reads happen at the stated source
points; do not cache event+170 before an earlier provider call if exposing
service-visible mutation semantics.

## evt_npc_flag_onoff800500b0/9c

Argument evaluation and request order:

1. Read raw operand0 from captured args; evtGetValue80031260(event,raw),
   capture signed on/off selector.
2. Read raw operand1; evtGetValue, capture name pointer.
3. Read raw operand2; evtGetValue, capture mask word.
4. Call evtNpcNameToPtr(event,captured name), capture actor pointer.
5. Selector0: read fresh actor+0 flags, AND complement mask, store actor+0.
   Selector1: read fresh actor+0, OR mask, store actor+0.
   All other signed selector values: no actor flag read/store, still perform
   argument resolution and name resolver, then return2.

There is no implicit active/query publication, flag filtering, pose effect or
collision-owner update here. A source mask can include any bit; an adapted
renderer must separately consume the appropriate actual stored flags. Flags
at actor+4 are untouched by this leaf.

## evt_npc_set_position80053158/e4

Read/evaluate raw operand0 through evtGetValue and retain name pointer.
Then separately read operand1, call evtGetFloat80030840 and retain X; read
operand2, call evtGetFloat and retain Y; read operand3, call evtGetFloat and
retain Z. Only after all three float requests call evtNpcNameToPtr.

The source then reads the zero-vector constant802c1470 as words0/4/8 into
stack8/c/10. All three words are subsequently overwritten with retained
X/Y/Z using stfs; this constant read is real even though its values do not
survive the ordinary result. Original vector words are all00000000.

Exact source publication order (A is the returned actor):

1. Round/store retained X into stack8; load stack8 into captured X word.
2. Round/store retained Y to stackc and retained Z to stack10.
3. Load stackc Y word; store captured X to A+98 (previous X).
4. Load stack10 Z word; store captured Y to A+9c, then captured Z to A+a0.
5. Read fresh A+98 and A+9c, **both before stores**; store them to current
   A+8c then A+90.
6. Read fresh A+a0; store it to current A+94.

The initial stack zero stores interleave with these float stores in assembly;
they do not provide a new actor default. The source words copied into current
position come from fresh actor previous fields, so an observable store
provider mutating previous Y/Z must affect the later current copy at the
stated paired/late read boundaries. Do not replace this with a bulk host
struct assignment or write current before previous.

This leaf does not set/clear flags, movement components, contact pointers,
velocity, gravity, collision floor, yaw, actor elapsed time or pose state.
Native side-effect adapters may need to invalidate their own derived spatial
cache, but must distinguish that policy from extra original actor stores.
Original NaN/infinite or float exception domains are not source finite guards;
a bounded native numeric domain must name such admission separately.

## evt_npc_get_position80052e98/84

Resolve raw operand0 via evtGetValue first. Then capture raw operands1,2,3
(destination tokens) in order **without evtGetValue**. Call evtNpcNameToPtr
only after capturing all three destinations. Retain returned actor pointer.

Read float current A+8c; call evtSetFloat800304bc(event,captured destinationX,
value). Then read fresh A+90 and call evtSetFloat for destinationY. Then read
fresh A+94 and call evtSetFloat for destinationZ. Return2. Do not snapshot
XYZ before the first output callback: destination resolution or an injected
setter can alias/mutate actor storage and later source reads must observe it.
The destination tokens are captured before lookup and are not reread between
outputs. Use genuine float operand routing, not integer evtSetValue or guessed
LW-only stores. Local/global/float namespace semantics belong to the existing
[event float contract](../event/EVENT_FLOAT_OPERAND_ROUTING.md).

## evt_npc_set_ry80050988/64

Read/evaluate operand0 through evtGetValue, retain name. Read operand1 and
resolve through evtGetFloat, retain yaw in the FP register. Call
evtNpcNameToPtr only then. stfs retained yaw to actor+144; return2.
No reviseAngle, turn interpolation, pose paper-facing request, normalized
range requirement or previous yaw write occurs here. The separate
[ordinary yaw/facing owner](../display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md)
consumes it later; writing a pose target here would add unrequested effects.

## evt_npc_set_ry_lr8005088c/fc

This is a camera4-dependent left/right selector, not a literal90/270 helper.

1. Resolve operand0 through evtGetValue, retain name pointer.
2. Resolve operand1 through evtGetFloat, perform PPC fctiwz toward-zero signed
   conversion, capture low32-bit integer selector. Conversion occurs before
   name lookup. Native signed finite conversion admission must preserve this
   order without invoking C++ undefined overflow for exceptional source inputs.
3. Call evtNpcNameToPtr; retain returned actor pointer A.
4. Call camGetPtr8000f2fc with literal4. Read returned camera+114 heading h.
5. Load binary32 constant100 at8041fde4, then binary32 constant5 at8041fde0.
   Compute SP fused multiply-add t=roundF32(100*h+5), then SP division
   u=roundF32(t/100). Do not simplify to h+0.05 or substitute current native
   camera yaw without proving the source camera+114 mapping.
6. Call reviseAngle80007328(u), then fctiwz its result to signed integer n.
7. Compute both signed PPC truncating remainders using divw/mullw/sub:
   L=(n-90)-trunc((n-90)/360)*360;
   R=(n+90)-trunc((n+90)/360)*360.
   These are signed remainders, not always-positive modulo.
8. Convert L and R to float by signed PPC integer-to-double construction
   with constant4330000080000000 at802c14b0, subtract and round as source
   fsubs. Selector==0 stores L to A+144; any nonzero selector stores R.
   Return2. Only one actor yaw store occurs.

reviseAngle80007328/60 calls double fmod802725c0 with double360 from802bf240,
rounds result to binary32, replaces NaN with float0, and if negative adds
float360. If that rounded addition is >=360 it returns0. Constants float0
and360 are at8041f448/8041f45c. Retain its float rounding/negative-zero behavior
rather than using a different generic angle function. fmod resource/numeric
provider semantics and actual camera4 ownership remain explicit transitive
requirements; this leaf does not call any party/facing interpolation service.

## Minimal real native providers and evidence

The actual retained registry can supply original ordinary named lookup,
context+170/association reads and full actor stores; FieldNpcRegistryBinding
must retain its backing owner/facade. Actor-context aliases need the real
source root publication, not a guessed name or renderer model. Event native
operand services already own evtGetValue/evtGetFloat/evtSetFloat. Provide
stfs rounding and fctiwz/fused/division/remainder domains explicitly where
needed. The left/right leaf additionally requires the actual camera4+114
owner; leave it pending when that mapping is unavailable.

These callbacks are instantaneous original requests, but pre-admission may
hold a native call before effects. If supporting mid-call missing providers,
retain evaluated arguments, target pointer and exact partial publication
phase so retry cannot reevaluate or repeat earlier stores. Original source
has no such cursor; this is suspension at a required native provider boundary.
Do not reuse map-registration battle/pose readiness as blanket admission for
these actor, VM destination, association or camera owners.

Fresh complete listings live under ignored artifacts/npc-event-leaves/,
produced by tools/inspect_dol_function.py and artifacts/reference/us_symbols.csv
from the read-only ISO. Constants and strings were read at actual DOL virtual
addresses; r2=80427440 is established by original startup. No code/build/tests
or original runtime observations are supplied by this contract.

| Body | Address / hex size | SHA-256 original body |
|---|---|---|
|evtNpcNameToPtr|8004f5d8/168|897b33ff5f3bc5224ef209829aabb8ef787ae2b531e1bcf8e9f7c29072f2f19e|
|evt_npc_flag_onoff|800500b0/9c|7d370debe1299ae3549b037ca682eaeef9cba5e08e31d133e5ed9012ce737d68|
|evt_npc_set_position|80053158/e4|126064a0d8867aab27e8f5bd7733ce2bb8c15e2a409955a77b2d8eec4a85abc6|
|evt_npc_get_position|80052e98/84|bb0fa6930f496b671a37daec8b97d0995c9158c130b7d9af663776c12f7d5dae|
|evt_npc_set_ry|80050988/64|b0f39cb5444417fb656687594fbd4732ef956722c170a0fbec73f8960cb8434d|
|evt_npc_set_ry_lr|8005088c/fc|44ff885cf6588c797616a88598ebfc3099972afe0328e98aefd5b8db7bebfbfa|
|reviseAngle|80007328/60|e44664bf511be1e4b10e4f0b3e40004fbd1d412c40ce6b34fee14987db320ed5|
