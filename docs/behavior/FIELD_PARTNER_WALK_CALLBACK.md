# Ordinary partner walk callback, initialization and speed adjustment

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyMoveWalk at0x800ca134,size0x420. This closes its local request graph,
history initialization and numeric speed writes;called pose/status/distance and
final movement engines remain dependencies. Addresses are DOL virtual addresses.

[driveParty](FIELD_PARTY_DRIVER_DISPATCH.md) modes0 and2 invoke kind record+8.
Pinned kind1/8 records bind that pointer to this callback. Mode1 instead calls
the [substate dispatcher](FIELD_PARTNER_MODE1_SUBSTATES.md). These are distinct
routes,not interchangeable wrappers around a guessed follower algorithm.

## Pending-change initialization and record layout

Let P be the party object. If word P+8 mask8 is set,consume it,clear P+0
mask0x100,load owner M=P+0x160 and OR2 into P+0. Select the history bank
using **P+0 mask8**,not P's slot byte:

| Bank | Record base | Read-index word | Write-index word |
| --- | --- | --- | --- |
| 0,mask8 clear |0x803ec2c8|0x8041eae8|0x8041eaf0|
| 1,mask8 set |0x803ec7c8|0x8041eaec|0x8041eaf4|

Each bank holds80 records of16 bytes. Record+0 is a flag byte,+4/+8/+0x0c
are X/Y/Z words interpreted as floats by readers. Initialization writes only
the flag byte and coordinate words;bytes+1..+3 remain untouched. The loop has
ten iterations,each writing eight records and advancing128 bytes.

Bank0 seeds every record from a freshly queried global Mario position
+0x8c/+0x90/+0x94,with flag0. Bank1 queries marioGetPartyId at0x80141050,
passes its result to partyGetPtr,and calls nokonokoGetStatus on a nonnull
returned object Q. Seed from global Mario position when Q is null,status
equals3,or PSVECDistance(global Mario position,Q+0x58 vector) is
ordered>=**150**. Otherwise seed from Q+0x58/+0x5c/+0x60. The service
call pipeline is explicit;no partner kind or story meaning is inferred from
the getter's name. Reset only the selected bank's read/write index words to0.

Then store byte0 at P+0x3d,word16 at+0x44,float50 at+0x40,byte0 at+0x3e,
float **0x3fe66666** (approximately1.8) at+0x118,and float0 at+0x114.
The owner M was captured earlier;the seed's global pointer reads do not
replace that owner pointer in the following test.

If M+0 mask0x00400000 is set,skip the rest of initialization. Otherwise:

1. Request partyChgPoseId(P,1).
2. If unsigned P+0x3b equals6,clear P+0 mask0x8000.
3. Store byte1 at+0x3b and OR4 into word+8.
4. If unsigned P+0x36 equals3,clear masks0x70,0x03000000 and0x04000000
   in P+0 through three ordered load/write operations.
5. Clear byte+0x36 and OR2 into word+8.

No pending marker skips all these initialization writes and proceeds directly
to the speed/admission section. Mode0's driver can already consume mask8
before this callback;do not initialize histories on every mode0 callback merely
because the field event requested stop. The called partyChgMot/pose services
may affect subsequent state and require their own contracts.

## Speed update admission and target selection

Skip the speed computation if signed halfword P+0x28 is **nonzero**,including
negative,or if unsigned byte P+0x36 is2,3 or6. Regardless of that admission,
the callback later calls [partyWalkMain](FIELD_PARTNER_HISTORY_WRITER.md).

Otherwise load M=P+0x160 and current speed s=P+0x104. If M+0 mask0x8000
is set,target t is float M+0x180 directly. Otherwise:

```
t = R(R(P+0x108 * M+0x228) * factor)
factor = 0.75 if P+0 mask0x80 is set, otherwise 1.0
```

R denotes each single-precision instruction result. The factor1 multiply is
still present in the original. A negative target is not clamped before its
branch test. There is no local elapsed-time or rendering-frame multiplier.

## Rounded adjustment and clamps

If target is ordered>0:

```
u = R(0.5 + t)
v = R(u - s)
next = F(0x3d4ccccd, v, s)
if ordered(next >= t): next = t
P+0x104 = next
```

F denotes the single-precision **fused multiply-add**. The coefficient's exact
binary32 word0x3d4ccccd is approximately0.05;substituting a host double literal
or two separately rounded operations is not a demonstrated equivalent.

Otherwise,including an unordered target comparison:

```
v = R(-1.0 - s)
next = F(0x3dcccccd, v, s)
if ordered(next <= 0): next = +0.0
P+0x104 = next
```

The coefficient is approximately0.1. Both clamp comparisons use CR OR of
the relevant ordered inequality and equality;unordered results do not clamp.
There is no generic epsilon,finite-value validation or maximum speed clamp.
FPSCR/denormal/exception-domain behavior remains a platform dependency.

Finally request partyWalkMain(P) at0x800c9b9c and return. That routine writes
follower history and delegates actual movement;this callback's speed field does
not itself prove distance traveled or original tick duration.

## Verification and open composition

tools/verify_field_party_follow.py checks seven complete local bodies,66
instructions,ten constants,two kind-record hashes and two used lecture imports.
The scope includes this callback,the mode1 dispatcher,the history writer,two
read helpers,anotherPartyGetPtr and hitGetAttr. Reports and corrected Gekko
listings remain ignored;no original gameplay or measured movement is claimed.

Next:walkMain/terrain/collision/pose engines,seed getter/status/distance semantics,
history read-index producers,owner/substate/speed writers and original partner
following/stop/run traces. Full partner locomotion and game equivalence remain open.
