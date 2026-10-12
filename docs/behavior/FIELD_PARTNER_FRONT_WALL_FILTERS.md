# Partner wall filters,hidden block names and global Yoshi status

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B: complete
chkfilter0x800c4a9c,size0xe4;chkfilterNokotaro0x800c48cc,size0xe4;
chkfilterYoshi0x800c49b0,size0xec;yoshiGetStatus0x800b6e94,size0xcc;
mobjHitObjPtrToPtr0x8005f7c4,size0x1c. They support the
[front-wall query](FIELD_PARTNER_FRONT_WALL_QUERY.md),not a complete ray backend.

## Callback ABI and priority

Each callback captures candidate hit node H from its **second integer
argument**,calls marioGetPtr with an unused returned pointer,and reads attribute
word H+4. There is no null H guard. Return1 admits,return0 rejects,as confirmed
by the backend's callback/result test. The first callback argument does not
select a partner object in these bodies.

Apply tests strictly in the listed order;overlapping attributes must retain
priority rather than being reduced to one unordered mask expression:

| Callback | Ordered rules before ownership/name handling |
| --- | --- |
| chkfilter |Attr&0x00080080→1;otherwise attr&0x00800004→0;otherwise high bit clear→1 |
| chkfilterNokotaro |Attr&0x00800005→0;otherwise attr&0x00080080→1;otherwise high bit clear→1 |
| chkfilterYoshi |Attr&0x00080080→1;otherwise attr&0x00800000→1;otherwise attr&4→0;otherwise high bit clear→1 |

Here high bit is0x80000000. Generic and Yoshi can admit an overlapping
priority attribute before a later rejection;Nokotaro checks its rejection mask
first. No surface-kind meaning is inferred solely from a numeric attribute.

## Dynamic ownership and original resource names

Only after the earlier rules,high-bit nodes request mobjHitObjPtrToPtr(H).
Its complete leaf checks H+4 high bit and returns **word H+0xd0** when
set,otherwise null. The filter then compares the returned owner's C string at
**owner+0x15** against,in order:

1. MOBJ_HiddenHatenaBlock,original string0x802cb8a0.
2. MOBJ_HiddenBadgeBlock,original string0x802cb8b8.
3. MOBJ_Hidden10CountBlock,original string0x802cb8d0.

Any strcmp match rejects0 and skips later comparisons. On a match,the filter
requests mobjHitObjPtrToPtr(H) **again** before returning0,ignoring that second
return value. No match admits1. There is no null-owner guard before the first
name read. These strings are exact resource identifiers,not story dialogue or
guessed English attribution.

The callback does not treat every high-bit node as hidden,mutate attributes,
rewrite names or produce a stable name from an owner index. Native dynamic
providers need actual ownership/name evidence;an index-only/static mesh is not
proof of the original hidden-object policy.

## Global yoshiGetStatus selection

yoshiGetStatus ignores its incoming argument,requests marioGetPartyId then
partyGetPtr. Null,an unsigned kind byte other than4,or object P+0 mask0x100
clear returns0. For admitted kind4/mask-set object,the unsigned byte+0x39
selects a nonzero result:

| State byte | Return |
| --- | --- |
| 10..13 |1 |
| 20..22 |2 |
| 100..101 |5 |
| 60..65 |3 |
| 50..52 |4 |
| Other admitted values |5 |

The ranges are checked in that original order. No state0 rejection occurs
after kind/flag admission;it returns5 by default. Consequently wall filter
choice is driven by this **global party query**,not simply the front-wall
input object's kind. The two ray passes independently call the service.
Pointer/slot selection and state writers remain separate dependencies.

## Height leaf and verification

partyGetHeight0x800be4c4,size0x30 returns float+0xf0 normally. Unsigned
kind4 with word+0 mask0x100 set adds original18.5 through fadds. It does not
read the global Yoshi status code or share its full state-range test;do not
merge these two admission rules.

tools/verify_field_partner_front_wall.py pins these complete bodies,front-wall
selection/projection and selected backend layout regions,62 instruction
anchors,16 constants and four data spans (direction seed and three name strings).
Original executable/name payloads are not distributed;authored identifiers and
hash metadata are versioned. Inspection listings/reports remain ignored.

Next:ownership/status/slot writers and supported dynamic-object providers;
complete ray backend,mesh/numeric behavior and original named-object/collision
observations. Static filter closure does not complete partner collision or the game.
