# Physical input producer, global edges and repeat counters

US G8ME01 disc 0 revision 0, documentation agent ID 1, recovered 2026-10-12.
Evidence B: complete local bodies makeKey (0x80005ff8,size0x504), DEMOPadRead
(0x8002f0c4,size0xc0) and DEMOPadCopy (0x8002f184,size0x1b8), with original
reset-mask data and downstream battle snapshot reads. PADRead/PADClamp/PADReset/
PADControlMotor are hardware-service dependencies, not fully specified here.
No original input trace, hardware capture or Switch equivalence is claimed.

The available pinned Japanese source assisted names. In particular, its makeKey
source omits the US input-clear chord block and uses unscaled repeat-delay
literals. The US instructions below, not that source, define this contract.
No reference implementation has been copied into native code.

## Raw read, channel status and reset requests

DEMOPadRead requests PADRead at0x802a74f8 into four12-byte raw records starting
0x8041e7d0, then PADClamp at0x802a67f8 on the same buffer. Their effects and actual
poll/retrace cadence remain open. Next clear the word at0x8041e800, then process
channels0..3 in order. Raw record stride is12; destination DemoPad stride is30
bytes, starting0x803ca398. DemoPad is BSS, not initialized asset bytes.

Read signed error byte raw+0xa. Error0 or-3 increments the count word. Error-1
instead ORs that channel's reset mask into a local pending word. Other errors
neither increment nor request a reset locally. The reset masks at0x804151a8 are
0x80000000,0x40000000,0x20000000,0x10000000 in channel order. Every channel,
regardless of error, is passed to DEMOPadCopy. After all four, if pending mask is
nonzero call PADReset(mask) at0x802a7184. Do not reset midway through copying or
infer that the count proves a fresh successful sample: error-3 is also counted.

## DemoPad copy and retained sample route

DEMOPadCopy(source,destination) first checks signed source error byte+0xa. For
error-3, clear destination halfwords+0xc,+0xe,+0x12,+0x14,+0x16,+0x18,+0x1a,+0x1c
and return. It retains all12 bytes of the previous raw snapshot at+0..+0xb and
the current direction halfword+0x10. Therefore this route suppresses newly
pressed/released/delta outputs but preserves held input and analog coordinates.
It must not be replaced by a zero-input disconnect sample or fresh raw copy.
For every other error value, the ordinary copy route runs without a local
success-only filter; PADRead/PADClamp determine the incoming data's meaning.

Ordinary route computes direction halfword D from **signed** stick bytes:

| Source raw axis | Strict comparison | Mask in D |
| --- | --- | --- |
| Main X at+2 | <-48 / >48 | 0x4000 / 0x8000 |
| Main Y at+3 | <-48 / >48 | 0x2000 / 0x1000 |
| C X at+4 | <-48 / >48 | 0x0400 / 0x0800 |
| C Y at+5 | <-48 / >48 | 0x0200 / 0x0100 |

Exactly-48 and+48 do not activate those directions. These are distinct from the
battle pad's **inclusive30** virtual-button thresholds and different mask values.
Do not apply one shared direction threshold/mask to every consumer.

Using destination's old D at+0x10, write D&~oldD to+0x12, oldD&~D to+0x14,
then D to+0x10. Read new physical button halfword B from source+0; use the old
button halfword in destination+0 to write B&~oldB to+0xc and oldB&~B to+0xe.
Compute four signed16 axis deltas **new signed byte minus old signed byte** for
main X/Y and C X/Y, storing+0x16,+0x18,+0x1a,+0x1c respectively. Finally copy
three words from source+0,+4,+8 into destination+0,+4,+8 in order. The copy
includes raw error/padding bytes; it does not overwrite the derived region.
All derived edges/deltas use the previous raw snapshot before that final copy.
No pointer, channel or overlap validation is added locally.

## makeKey global field bindings and ordering

makeKey first calls DEMOPadRead. Let GP be the pointer at0x80415fd0. For channel
c0..3, word fields below are at offset+4*c; analog arrays are bytes at offset+c:

| GP offset | Meaning from original reads/writes |
| --- | --- |
| +4 | Signed frame-rate parameter used for repeat scaling |
| +0x1328 | Current physical button mask, sourced from DemoPad raw halfword+0 |
| +0x1338 | Newly pressed physical mask |
| +0x1348 | Physical repeat output |
| +0x1358 | Physical repeat countdown word |
| +0x1368 | Released physical mask, copied from DemoPad halfword+0xe |
| +0x1378 | Current direction mask, sourced from DemoPad halfword+0x10 |
| +0x1388 | Newly pressed direction mask |
| +0x1398 | Direction repeat output |
| +0x13a8 | Direction repeat countdown word |
| +0x13b8,+0x13bc | Main X/Y byte arrays |
| +0x13c0,+0x13c4 | C X/Y byte arrays |
| +0x13c8,+0x13cc | Left/right trigger byte arrays |

The function processes **all four direction channels first**, then all four
physical-button channels, then copies six analog arrays for all channels. The
analog loop is unrolled in pairs but still visits channels0,1,2,3 in that order.
It does not copy the raw analog A/B bytes or raw error byte into additional
GP input fields on this path.

For either mask group, let M be the new unsigned halfword and O the prior GP
word. Write new = M&(M^O), and initialize repeat output to new. If M is nonzero
and equals O, decrement its countdown word with32-bit arithmetic. If the new
count is zero, replace repeat output with the **entire held M** and reload the
short delay. Otherwise repeat remains new (zero for an unchanged normal mask).
If M is zero or different from O, reload the long delay. Finally store M as
current. The physical group also copies the released mask after its current
store. Countdown is per whole mask/channel, not per button/direction.

Adding a button restarts the long countdown and emits only newly added bits.
Removing a button also restarts it but emits no new bits for those still held.
A continuously held input does not create a new press edge on each call.
Because the error-3 copy route retains M, its held repeat countdown can continue
and emit a repeat even though that route supplied no new sample or release.

## Original frame-rate-scaled physical/direction repeats

The long delay is computed from frameRate*24 and the short delay from
frameRate*6. The US signed multiplication/high-word/shift sequence implements
truncation toward zero of each product divided by60 on the ordinary supported
positive frame-rate domain. The multiply itself is word arithmetic; arbitrary
out-of-domain overflow must not be specified as unlimited mathematical integers.

| Frame-rate parameter | Long delay | Short delay |
| --- | --- | --- |
| 60 | 24 | 6 |
| 50 | 20 | 5 |
| 30 | 12 | 3 |

At parameter60, a changed nonzero mask at update0 emits its press and sets24.
Updates1..23 with that unchanged mask do not repeat;update24 emits the held mask
and sets6;update30 emits it again. These are counts of makeKey calls, not a
measured wall-clock repeat rate. There is no local minimum1 when scaling yields0:
on the unchanged path a zero countdown becomes-1, rather than repeating every
call. Repeat flags are recomputed each call, not accumulated until consumed.

The battle pad separately repeats virtual directions at16/8 **pad updates**.
Its combined-mask change rules can reset that phase when physical buttons change,
but physical repeat bits still come from this global24/6-scaled producer. See
[battle pad local rules](BATTLE_INPUT_AND_DEFENCE.md) and
[update order](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md).

## US input-clear chord and motor requests

After global analog copying, process all four channels again. If
`(currentPhysicalMask & 0x1c00) == 0x1c00`, then clear for that channel:
current/new/repeat physical and direction words, physical released word, and
all six analog bytes. Reload both repeat countdowns with the long scaled delay.
The test permits other buttons alongside those masked bits. Clearing happens
**after** edges/repeats/copy were computed, before later consumers read GP.
It does not clear or rewrite the raw DemoPad record, so the chord can suppress
these GP outputs again on a later call. This block has no local reset/reboot
request; do not infer a complete system reset policy from this mask.

Next, if GP word+0x1294 is nonzero, visit four motor channels:

- If byte GP+0x1310+c is nonzero, request PADControlMotor(c,2), without updating
  the ordinary previous-mode latch. This can request mode2 again next call.
- Otherwise compare current mode byte+0x13d0+c to previous byte+0x13d4+c.
  Equality does nothing. On change, modes0/1/2 request the matching integer mode;
  every other byte makes no local motor call. Then copy current to previous,
  including unrecognized modes.

The SDK entry is0x802a77f8 (annotated U_PADControlMotor). No mode requests occur
when the outer word is zero, and no latch update is performed there. Finally
store word1 at GP+0x1324 regardless of that motor gate. The producer's input-clear
and rumble control/state writers still require caller/lifetime recovery.

## Implementable rules and validation obligations

Preserve raw-versus-GP-versus-battle ownership, signed axes and the error-3 retained
snapshot route. Physical masks/repeats can be modeled separately from native
hardware adaptation, with explicit original frame-rate scaling. Do not fabricate
hardware error/reset semantics beyond these observed requests.

Derived acceptance cases: test both sides of each strict48 threshold including
exact48;one press/hold/release;add/remove a second held button;repeat at24/30 for
frameRate60 and20/25 for50;error-3 clears derived edges but retains held/analog;
the0x1c00 chord clears the complete GP outputs while retaining DemoPad. Motor
cases must distinguish forced-mode2 from change-only latch updates.

Original observation must additionally record PADRead/PADClamp results,raw error,
DemoPad,GP counters/masks,configured frame rate and call identity. Native keyboard/
Circle Pad adaptation or host fixtures cannot substitute for that trace. The
[dedicated verifier](../../tools/verify_input_cadence_contracts.py) checks containing
bodies and selected instructions/data; it does not execute hardware services.
