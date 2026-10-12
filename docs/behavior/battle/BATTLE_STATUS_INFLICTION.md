# Weapon status parameters, immunity gates and chance resolution

US G8ME01 revision0, recovered 2026-10-12. B evidence is original instruction
and jump-table inspection; friendly status names are R labels from
[status storage](BATTLE_STATUS_STORAGE.md). Identity/source boundaries are in
[ORIGINAL_BEHAVIOR.md](../../ORIGINAL_BEHAVIOR.md). No original execution or Switch
equivalence is claimed. This documents the weapon wrapper/helpers and the
admission/mutation prefix of BattleSetStatusDamage. The remaining local branches are documented in
[status presentation and completion](BATTLE_STATUS_PRESENTATION.md).

| Routine | US address / size |
| --- | --- |
| BattleSetStatusDamageFromWeapon | 0x800fbd8c /0x14c |
| BattleSetStatusDamage | 0x800fbed8 /0x107c |
| _getRegistStatus | 0x800fcf54 /0xdc |
| _getSickStatusParam | 0x800fd030 /0x460 |
| _getSickStatusRate | 0x800fd490 /0x1f0 |

Ignored artifacts under `artifacts/reference/behavior/` contain disassembly.
Case mappings were inspected at US tables: wrapper8034c750 (17 entries),
removal classifier8034c994 (25), part immunity8034c924 (28), badge
immunity8034c8b4 (28), result bits8034c868 (19), susceptibility8034c9f8 (28),
weapon parameters8034ca68 (28), rate8034cad8 (28). Addresses are hexadecimal.

## Weapon iteration and argument binding (B)

`BattleSetStatusDamageFromWeapon(A,D,P,W,F)` initializes an output-bit word
to zero. If F has0x20000000, return zero before inspecting any status.
Otherwise iterate IDs0 through27 in ascending order. For each:

1. Obtain duration/strength bytes via `_getSickStatusParam(A,W,id,&T,&S)`.
2. Obtain `_getSickStatusRate(W,id)` and interpret its low byte as **signed8**
   for the application rate. Initialize extra chance adjustment X=0.
3. For IDs10,12,14 only, skip the application call if both T and S are zero.
   Opposite IDs11,13,15 do not have this wrapper skip.
4. For ID26 only, if the signed rate is positive, set
   `X=U8(A+0x0d)-U8(D+0x0d)`.
5. Call `BattleSetStatusDamage(&bits,D,P,U32(W+0x74),id,rate,X,T,S)`.
   T/S arrive as byte values; the callee uses signed-byte tests where specified.
6. If the call returns nonzero and id18, call `btlDispPoseAnime(P)`.

Return the accumulated bits after all28 IDs. The loop does not stop after a
successful status. Earlier mutations can influence later checks. F is only
used for the wrapper bypass here; the callee receives W's special flags.

## Base weapon fields (B)

Offsets below are hexadecimal relative to W. Parameter fields are interpreted
as signed bytes. `same` means T and S both start with the listed byte.
For paired positive/negative statuses, parameters are zero on the wrong sign
side, including zero strength. Rate has a subtly different zero-sign rule.

| ID | Base T / S | Base rate |
| --- | --- | --- |
| 0 | 9d /same | U8(9c) |
| 1 | 81 /same | U8(80) |
| 2 | 83 /same | U8(82) |
| 3 | 85 /same | U8(84) |
| 4 | 87 /88 | U8(86) |
| 5 | 8a /same | U8(89) |
| 6 | 8c /same | U8(8b) |
| 7 | 8e /same | U8(8d) |
| 8 | 90 /same | U8(8f) |
| 9 | 92 /same | U8(91) |
| 10 | 94 /95 when S8(95)>0; otherwise0/0 | U8(93) if S8(95)>0, else0 |
| 11 | 94 /95 when S8(95)<0; otherwise0/0 | U8(93) if S8(95)≤0, else0 |
| 12 | 97 /98 when S8(98)>0; otherwise0/0 | U8(96) if S8(98)>0, else0 |
| 13 | 97 /98 when S8(98)<0; otherwise0/0 | U8(96) if S8(98)≤0, else0 |
| 14 | 9a /9b when S8(9b)>0; otherwise0/0 | U8(99) if S8(9b)>0, else0 |
| 15 | 9a /9b when S8(9b)<0; otherwise0/0 | U8(99) if S8(9b)≤0, else0 |
| 16 | 9f /same | 100 if S8(9f)≠0, else0 |
| 17 | 0 /0 | 0 |
| 18 | a9 /same | U8(a8) |
| 19 | a1 /same | U8(a0) |
| 20 | a3 /same | U8(a2) |
| 21 | a6 /same | 100 if S8(a6)≠0, else0 |
| 22 | a7 /same | 100 if S8(a7)>0, else0 |
| 23 | aa /ab | 100 if S8(aa)>0, else0 |
| 24 | ac /ad | 100 if S8(ac)>0, else0 |
| 25 | 0 /0 | U8(a4) |
| 26 | 0 /0 | U8(a5) |
| 27 | 9e /same | U8(9e) |

Out-of-range IDs produce base0/0 and rate0. The parameter helper continues
through later weapon/item logic even for an out-of-range ID. Both output
pointers are assumed valid. Direct helper rate returns are unsigned loads or
0/100; the wrapper's signed-byte conversion means byte128–255 becomes a
nonpositive application rate, rather than a greater-than-100 percentage.

## Equipped-badge parameter adjustments (B)

After choosing base T/S, read signed word itemID at W+8. For IDs240–338
inclusive, call `pouchEquipCheckBadge(itemID)` to obtain n; otherwise n=0.
Only if n>0 apply these special cases:

| ItemID / status | Adjustment |
| --- | --- |
| 244 /11 | T +=2*(n−1) |
| 245 /1 | T +=2*(n−1) |
| 246 /15 | T +=2*(n−1) |
| 251 /5 | T +=2*(n−1) |
| 253 /9 | T +=2*(n−1) |
| 335 or336 /14 | If U8(A+0x2fe)≥2, S +=U8(A+0x2fe)−1 |

No other item/status combination changes values in this stage. These numeric
item bindings must be preserved; equipment-count helper semantics remain a
separate inventory dependency. Arithmetic is word-sized until final byte stores.

For status16, additional logic uses **weapon pointer identity**, not itemID:

- W=0x803550a8 or0x80355168: query badge256 if A kind+8 is222, badge257
  otherwise. If count≥2, add count−1 to S.
- W=0x80355228 or0x803552e8: query badge337 for kind222, badge338 otherwise.
  If count≥2, add2*(count−1) to S.
- For every status16 call, if S≥10 set S=9, including weapons not matching
  those four pointers. No lower clamp is applied.

Finally store T and S as bytes. A relocated port needs stable weapon identities
for these four records; replacing the tests with item-ID equality is unproven.
There is no general duration clamp here; large adjusted T narrows modulo256.

## Application entry and removal classification (B)

`BattleSetStatusDamage(bits,D,P,special,id,rate,X,T,S)` first obtains old Charge
strength via GetStatus(D,16,null,&oldCharge). That read precedes the gates.
It classifies a request as removal-like using this exact table:

| IDs | Removal-like condition |
| --- | --- |
| 0,19,20,21 | S8(T)≤0 |
| 1–9 | S8(T)=0 |
| 10–15 | S8(S)=0 |
| Other | false |

This is a gate classification, not proof the request will remove a status.
There is no universal null D/P or HP guard in this prefix. Evaluate rejection
conditions in this order; each returns0 before the chance draw:

1. P attributes at+0x1ac have0x20000000 or0x80000000: reject unconditionally.
2. D has status0 Allergic and request is not removal-like: reject.
3. P attributes have0x40000000, request is not removal-like, and ID belongs
   to {0,1,2,3,4,5,8,9,11,13,15,20,25,26,27}: reject.
4. U8(D+0x2f6) is nonzero (R: Feeling Fine) and ID is in
   {1,2,3,4,5,6,11,13,15,27}: reject. This gate has no removal exception.
5. ID8 and U8(D+0x301) nonzero (R: Ice Power): reject, again without a
   removal exception.
6. Signed word rate≤0: reject.

Do not merge part immunity, Allergic and badge gates into one common set:
their covered statuses and treatment of removal requests differ.

## Susceptibility and exact random draw (B)

`_getRegistStatus(D,id)` reads pointer D+0x144 and returns an unsigned byte
for these IDs. Other IDs return100. It does not check the data pointer for null.

| Status ID | Offset in susceptibility data |
| --- | --- |
| 1,2,3,4,5,6 | 0,1,2,3,4,5 respectively |
| 8,9,10,11,12,13,14,15,0 | 6,7,8,9,10,11,12,13,14 respectively |
| 25,26,19,20,7,18,27 | 15,16,17,18,19,20,21 respectively |

Let r be this unsigned byte, p the positive input rate, X the extra adjustment,
and b be whether special has0x100. At `0x800fc078`–`0x800fc0a8`:

```text
if not b or r == 0:
    p = signed_trunc_toward_zero((p*r) / 100)
    if not b and r > 0:
        p += X
# otherwise p remains the input rate and X is not added
draw = irand(100)
if draw >= p: return 0
```

Word multiplication/division preserves original 32-bit arithmetic; no general
clamp to0–100 occurs. Special bit0x100 bypasses nonzero susceptibility scaling
but does not override zero susceptibility. A positive input rate reduced to
zero still consumes one draw. Even final p≥100 consumes a draw. Negative X
can yield a nonpositive final chance, also after consuming that draw. Earlier
rejection gates consume none in this function.

## Mutation prefix and accumulated result bits (B)

On passing the draw, initialize a local setter result to0.
For id25, OR4 into *bits; for id26, OR8 into *bits. Neither calls SetStatus.
For all other IDs, first OR0x20 for id1 or0x10 for id27, then call
`BtlUnit_SetStatus(D,S8(id),T,S)` and retain its return value.

Next, IDs {6,7,10,12,14,16,18,19,21,22,23,24} OR2 into *bits.
For every other ID, OR1 when S8(T)>0, otherwise OR2. These mutations happen
even when the setter returns0, for example opposite-status cancellation.
The local setter return then gates a large effects block beginning at
0x800fc198. Consequently the accumulated bits are not simply a list of
statuses still active after the call.

This document's detailed application-prefix coverage ends at0x800fc194.
The remaining effect creation/cleanup, announcements, sound selection and
completion are specified in [the tail contract](BATTLE_STATUS_PRESENTATION.md).
After reaching that tail the whole function returns1, including when the setter
returned0 after cancellation. Do not substitute the local setter return for the whole function return
when implementing the wrapper's id18 pose update. The numeric setters themselves
are documented in [BATTLE_STATUS_STORAGE.md](BATTLE_STATUS_STORAGE.md).

## Verification obligations and remaining recovery (P)

Derived cases: positive/negative pair rate at exactly zero strength; signed
weapon rates127/128; per-item duration stacking with final byte narrowing;
pointer-specific Charge bonus and9 cap; removal exceptions vs Feeling Fine;
special bit0x100 with susceptibility0/1/100; zero final chance consuming a draw;
Gale Force X adjustment; setters returning0 while result bits are added;
ascending multi-status application whose earlier state affects later gates.

These are proposed differential cases, not original execution tests. Still
required: all weapon data bindings, equipment-count semantics, effect/announcement service internals,
orchestration callers, status turn
schedule and periodic effects. No full infliction or combat equivalence claim
follows from the recovered prefix alone.
