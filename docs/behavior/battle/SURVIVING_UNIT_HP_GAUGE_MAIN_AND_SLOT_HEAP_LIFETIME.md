# Surviving-unit HP gauge Main and slot heap lifetime

2026-10-14 US G8ME01 disc0 revision0. Read-only original DOL/ISO inspection,
continuing [ending slot/display admission](ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md).
Owned new subsystem documentation only; no native/shared documentation edits
or commit. Source Main already delivered is not retested or reclassified here.

## Complete HpGaugeInit and persistent fields

Ordinary r3=unit U, BtlUnit_HpGaugeInit80125fd8/b0:

1. HALF U200=0, HALF U204=0, HALF U1fc=0, in that order.
2. WORD U208=separately read signed HALF U10c currentHP.
3. WORD U20c=another signed HALF U10c read.
4. Fresh signed U10c/currentHP and U108/maxHP convert to single via double
   bias802f00b8; compute single current/max; FLOAT U210=ratio, U214=same ratio.
5. Reload unsigned U1fc: exact1 selects HALF U202=40, then reload signed U202
   and HALF U206=U202+30. Otherwise U202=20,U206=U202+30 (50 ordinarily).

The same body just stored mode0 with no callbacks, so ordinary serial Init
always selects20/50. It does not preserve caller mode1. U1fe is not written
here; original unit allocation memset supplies initial0, later producers own
the latch. No whole-gauge-bank reset occurs after damage or at ending entry.
The complete constructor ordering is already in
[unit registry/geometry initialization](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md).
Gauge initialization precedes later source resource import; do not reseed its
before/after ratios from live pouch during battle ending.

| Field (hex) | Width / meaning in these bodies |
| --- | --- |
| 1fc | unsigned HALF mode |
| 1fe | unsigned HALF flags; bit1 auto-hide latch |
| 200 | signed HALF interpolation remaining timer |
| 202 | signed HALF interpolation duration |
| 204 | signed HALF hide remaining timer |
| 206 | signed HALF hide duration |
| 208 | signed WORD pre-change HP |
| 20c | signed WORD post-change HP |
| 210,214 | FLOAT gauge ratios |
| 108,10c | signed HALF max/current HP |
| 1c | WORD unitflags; bit1 gauge visibility owner |

These unit offsets are distinct from bodypart P204 animation flags.

## Complete HpGaugeMain80125d88/250

No global/BattleWork getter, allocation, render queue or sound request appears.
The only transitive service is intplGetValue80006c8c in two active branches.
Let f round each source single arithmetic; i32 convert signed WORD and
i16 convert signed HALF. Stores/reloads remain in source order:

```
if i16(U200) > 0:
    U200 = LOW16(i16(U200)-1)
    if i16(U200) <= 0:
        U208 = U20c                         // raw word copy
        ratio = f(float(i16(U10c))/float(i16(U108)))
        U214 = ratio
        U210 = ratio
    else if U16(U1fc) == 1:
        U210 = f(float(i16(U10c))/float(i16(U108)))
        curve = intplGetValue(2,i16(U200),i16(U202),0,1)
        difference = wrappingSigned32(i32(U208)-i32(U20c))
        contribution = f(f(float(difference)*curve)/float(i16(U108)))
        U214 = f(U210+contribution)
    else:
        U214 = +0
        capturedMax = i16(U108)             // captured BEFORE interpolation
        curve = intplGetValue(0,i16(U200),i16(U202),0,1)
        difference = wrappingSigned32(i32(U208)-i32(U20c))
        capturedCurrent = i16(U10c)         // read AFTER interpolation
        contribution = f(f(float(difference)*curve)/float(capturedMax))
        ratio = f(float(capturedCurrent)/float(capturedMax))
        U210 = f(ratio+contribution)

if i16(U204) > 0:
    U204 = LOW16(i16(U204)-1)
if (U1c & 1) and i16(U204)<=0 and (U16(U1fe)&1):
    U1c = U1c & ~1                         // word store FIRST
    U1fe = U16(U1fe) & ~1                   // fresh halfword read/store SECOND
```

Mode1 obtains max before interpolation only for its first ratio calculation,
then reloads signed U108 for contribution after interpolation. Default mode
captures max before the helper and uses that retained value for both ratios.
In the active formulas convert signed WORD difference to single before
single multiply; preserve independent current/max reloads. Do not write U20c,
U202,U206,U1fc or reset countdowns in Main. A timer0 leaves both ratios alone,
but the independently admitted visibility/latch tail can still mutate flags.

Original curve arguments use REMAINING step, not duration-minus-remaining.
Mode0 source entry80006d24 gives a+(step*(b-a))/duration; mode2 entry80006d54
gives a+((step*(step*step))*(b-a))/(duration*(duration*duration)), with single
rounding at every instruction. Duration0 returns end before arithmetic.
Both start0/end1 words are original804226a4=00000000/804226a8=3f800000.
Mode2 is cubic, not the squared integer helper or generic camera easing.

Source maxHP0 has no guard in Init or active Main. Keep source numerical
provider/IEEE domain rather than inventing1. A no-HP scripted actor can retain
an initialized exceptional ratio while timer0 prevents recomputation; timer0
does not prove its stored ratio was1 or0. Actual Mario/Goombella maxHP10 closes
the ordinary finite used interpolation arithmetic. Full live timers and flags
must come from source damage/recovery history rather than a fresh ending reset.

## Actual direct-damage timer/latch producers matter

Original BattleDamageDirect800fdfd4 local HP application at800fe750 requires
unit U104 mask40000000 clear and part P1ac mask00800000 clear. Then it captures
signed oldHP U10c→WORD U208, subtracts signed BYTE U270 appliedHP, stores/clamps
currentHP to max then lower0, and stores signed newHP→WORD U20c.

If signed appliedHP U270!=0, ordered stores800fe7c4/cc copy signed HALF U202
to HALF U200 and signed HALF U206 to HALF U204. Following this, source retained
caller flags at stack8 mask100 set ORs U1fe bit1; clear clears that bit, even
when appliedHP0 (but only within the admitted HP application block). This does
not establish every other gauge-visibility flag producer or recovery path;
it closes the ordinary direct-damage timer/latch writes used by trio impacts.

Read-only native source/trio_attack.cpp current writeDamage exports HP,
WORD U208/U20c accounting, but its local producer does not export these gauge
timer/latch writes. Existing actual shared BtlUnit_Entry provider requests
HpGaugeInit, whereas native trio's scoped registry construction must preserve
its own initializer history explicitly. Binding Main with fabricated timer0
would hide this missing producer; import the source writes/lifecycle instead.
Portable damage/registry/gauge owners may be reused, but their scope must be
verified against the current direct-damage applications and packed export.

## SlotInit uses fresh nested map arena, not current map work or SDK heap ID

BattleAlloc800f8934/28 captures raw caller size to r4, then freshly loads
WORD8041e80c (r13+18ec) to r3 at800f8944; _mapAlloc8002ff58 at800f8948 returns
the pointer directly. No current BattleWork, GP heap selector, mapGetWork,
current map index, root validation, size normalization or fallback appears.
The root is the initialized nested battle map arena, paired with the already
recovered [map heap allocation/free authority](../event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md)
and ending allocation-bootstrap contracts. Each of SlotInit's three calls
loads this root afresh. Reel pointer retained across an allocation owns its
R14 returned-pointer store and following copy; do not requery/rebind that reel
after the allocator service. No meaningful ordinary source failure recovery
exists before SlotInit's memcpy of the returned pointers.

BattleFree800f8908/2c captures raw pointer to r4. Null returns without root
lookup; nonnull freshly loads SAME WORD8041e80c and calls _mapFree8002fdc4.
It does not receive or capture the allocating root for later comparison. A
changed arena between allocation/free remains a real lifecycle error/domain,
not a reason to silently free through the prior retained root.

## Slot idle cleanup versus actual Slot_End

Idle Main only clears each reelflags bit1 and writes BYTE58=ff. It does not
free arrays or zero pointers/counts/controller. This is already the complete
used idle path; no additional cleanup is implied by its return.

BattleBreakSlot_End80203688/4c is the exact later free service:

1. ReelGetPtr(0) → read WORD14 → BattleFree.
2. ReelGetPtr(1) → read WORD14 → BattleFree.
3. ReelGetPtr(2) → read WORD14 → BattleFree.

Each getter uses current global BattleWork; each nonnull Free separately reads
current battle heap root. No nulling of R14, count/ordinal/state/flag reset,
controller/event deletion or secondary memory free appears in this body.
After free, pointer words retain their old raw values until their containing
BattleWork lifetime ends or original next Init overwrites them. Do not invoke
Slot_End as idle cleanup or twice against retained nonnull freed pointers.

Actual BattleEnd800f8da4 teardown calls StageEnd→Audience_End→Slot_End after
its earlier actor/icon/menu cleanup, then frees B1c78 if nonnull and outer
heap1 BattleWork; [ending teardown ordering](ENDING_HEAP_FREE_AND_REMAINING_UNIT_TEARDOWN.md)
remains its authority. Slot_End is later engine teardown, not ending1f/Main.

## Evidence and validation

Ignored new listings slot-next-* under artifacts/recovered-temp-evidence/.
Complete original body SHA256:

| Body/address/size(hex) | SHA256 |
| --- | --- |
| HpGaugeMain80125d88/250 | a74ffc565bdd14fedaa2e30d545ebda0ce41c1c49b3c93930751e6f584b11747 |
| HpGaugeInit80125fd8/b0 | 74b1d30e4935777ca580656691a14f8127761a461c6010aa5b8eb29f355b4632 |
| BattleAlloc800f8934/28 | 5225f27b27e3e163d13c50a2bca3b38c4667e379e2c66eb8527316b279d81492 |
| BattleFree800f8908/2c | 4513c22a59f470372e6b7fe43710f4dc607a88f21d53631b14dcb90dac3ada4d |
| Slot_End80203688/4c | 46ca6506f71c89eb969ba7be3286394477fb523cd3aed9e93c8a80a32be12f7e |

Read-only whole-body inspection and bounded damage-producer store tracing;
scoped documentation diff check only. Native checks remain implementation-owned.
