# Surviving-unit status effects, icon admission, and part frontier

2026-10-14 US G8ME01 disc0 revision0. Read-only source recovery after
[HP gauge Main](SURVIVING_UNIT_HP_GAUGE_MAIN_AND_SLOT_HEAP_LIFETIME.md).
This closes ordinary inactive status-effect leaves and the icon display gate;
it does not erase live status/effect producers or claim a whole empty display.
Owned new documentation only; no native/shared documentation edit or commit.

## Ordered progression after HpGaugeMain

For every surviving ordinary unit U, btlDispMain801024f0 calls HpGaugeMain
at8010275c, then:

1. FLOAT U114=1 from8042224c=3f800000.
2. GetStatus80127f34(U,11,&duration,&strength). If signed duration>0 and
   signed strength<0, factor=singleFMA(.5,strength,1), lower-clamp factor.5;
   U114=single(U114*factor). Original.5=80422234.
3. GetStatus(U,10,the same two temporary bytes). Positive duration and
   positive strength give factor=single(1+strength), upper-clamp factor2
   (80422250), then U114=single(U114*factor).
4. BattleStatusEffectMain801966f0(U) at8010284c.
5. BattleStatusIconMain80253ca8(U) at80102854.
6. Read U14 first linked part; test for null BEFORE first part body. Per part,
   P1c0==-1 skips to next linked P0. Otherwise P204 animation flags and actual
   status/pose data admit the original part graph. First original service with
   P204&2 clear is _GetStatusPoseType80103084(U) at80102880; it is not another
   gauge/whole-unit empty check. P204&2 set uses its existing animation-name
   owner branch. Retain this next real provider when part pose admission is live.

No-status duration/strength outputs0 keep U114=1 but do not skip steps4/5/6.
The source numeric/status scalar is separate from the native rendering model
scale and cannot be replaced with an unconditional unit scale reset.

## Complete ordered inactive StatusEffectMain

Ordinary r3=U, Main801966f0/4c always calls these five leaves in order:

| Leaf | Predicate | Existing handle field | Inactive delete leaf |
| --- | --- | --- | --- |
| BSE_Sleep80196538/16c | CheckStatus(U,1) | WORD324 | 801964f8/40 |
| BSE_Biribiri801963ac/14c | CheckStatus(U,6) | WORD330 | 8019636c/40 |
| BSE_Fire80196240/12c | CheckStatus(U,8) | WORD328 | 80196200/40 |
| BSE_Freeze801960ac/154 | CheckStatus(U,9) | WORD32c | 8019606c/40 |
| BSE_Kagegakure80195f8c/e0 | current Bef8&1 AND U104&10 | WORD334 | 80195f48/44 |

CheckStatus80127814 is the existing original getter wrapper, not simply a
local duration array check. Status6 has its extra unsigned BYTE U2f7 producer:
nonzero can admit Biribiri even when its ordinary status-duration lane is0.
Null/negative/count semantics remain [the status-storage authority](BATTLE_STATUS_STORAGE.md).

Every inactive leaf STILL calls its delete wrapper. For handleWORD0:
each wrapper returns with no external request or field write. With a nonzero
handle, Sleep/Biribiri/Fire call effSoftDelete8005ea30(handle), then store
their corresponding handle field0. Freeze calls effDelete8005e9c0(handle),
then handle0. Kagegakure calls iconDelete8001ae08(handle+18), then handle0.
All are ordinary C calls; no event result/readiness value is defined.

Therefore the bounded complete inactive pass requires actual predicatefalse
AND each actual old handle0. A false current status alone is not a no-op if
an effect survived a previous frame. Do not zero that handle instead of the
source delete request or turn effect creation failure into fabricated handle0.

## Initializer and active producers

BattleStatusEffectInit8019673c is ONLY blr: it does not clear any of these five
handle fields. Original BtlUnit_Entry allocated-unit memset establishes their
initial0; live reused/saved units need their real history. No source status
clear helper automatically proves handle0. Source active leaf creates when
its stored handle0 and later updates the same owner; creation providers are:

- Sleep→effSleepEntry802594c0, publishes U324.
- Biribiri→effSparkEntry80193680, publishes U330.
- Fire→effFireEntry8018eb6c, publishes U328.
- Freeze→effIceEntry80196db8, publishes U32c.
- Kagegakure→iconEntry8001b094/formatted source name, then iconNameToPtr80019754,
  publishes U334. Its returned icon owner is not an effect handle.

Function addresses are original direct-call targets; exact effect API names
below are source-symbol names where verified. Their model/position/effect
backend admission remains active-provider work. Actual fresh trio ordinary
no-status path needs current CheckStatus1/6/8/9=0, U2f7=0, Kage predicatefalse,
and real absent handles. The original Frankly attributes12000000 do not set
its Kage mask10; Mario/Goombella still use their actual current attribute words.

## StatusIconMain gate: initial writes remain even when inactive

StatusIconInit80253e84/2c calls memset(U+348,0,7a0), including final
counter HALF Uadc and icon animation accumulators Uad8/Uade/Uae0/Uae2/Uae4/Uae6.
No queue/resource allocation occurs. Full unit entry invokes it before later
stat/status/geometry initialization; do not rerun this clear at ending entry.

Main80253ca8/1dc captures U and current B, then ALWAYS stores HALF Uadc=0
at80253cc8. If captured Bef4 mask10000 clear AND Bef8 bit1 clear, return
before position services, scalar writes, callbacks or queue registration.
This is an exact admitted inactive domain, not a pouch/zero-status guess.

If either battle flag admits it, retain full original graph:

1. BtlUnit_GetPos80128c7c(U,&U348,&U34c,&U350).
2. FLOAT U348=singleFMA(float(S16 Ud0),U114,U348);
   FLOAT U34c=singleFMA(float(S16 Ud2),freshU114,U34c).
3. FLOAT U354=13.440000534 (804287d0); U35c=0 (804287b4);
   WORD U364=0. If U104&2 clear, U360=25.600000381 (804287d4),
   U358=19.200000763 (804287d8); otherwise U360=25.600000381,U358=0.
4. For each entry freshly read GP WORD16c:2 selects table803b96e8;
   3/4 selects803b9858;5 selects803b99c8;otherwise803b9578.
   Rows16bytes contain{statusID,callback,iconID,option}. Up to23 rows;
   first statusWORD-1 stops before callback.
5. Compute output record U348+20+signed(HALF Uadc)*44hex.
   Invoke callback(U,rowPointer,outputRecord). Nonzero increments HALF Uadc;
   return0 retains the current output address for the next row.
6. Positive signed Uadc registers dispEntry(camera8,class1,
   BattleStatusIconDisp80253938, parameterU, priority901 at804287dc).

The23rd row is the terminator;22 real callback invocations are possible.
No retained table pointer across callbacks: source reloads GP16c each iteration,
which matters if service-visible globals change.

## A no-status unit can still produce the first icon

All four tables have17 default rows in order:
status1,4,21,2,8,9,19,20,14,15,7,10,11,5,12,13,18
→_bsi_default_turn802537c8. Then special rows3→_bsi_guruguru80253694,
6→_bsi_biribiri8025337c,0→_bsi_allergy80253564,
23/24→_bsi_regeneration802530fc. Only regeneration icon IDs differ by table.

_bsi_default_turn freshly reads current Bef4. When10000 clear it calls
CheckStatus(unit, signed8 rowstatus); false returns0 without output writes.
When10000 set, rowstatus1 **bypasses CheckStatus**, admits kind1..215,
222..223,224..230, then GetStatus and outputs an icon even with duration0.
The other16 default rows still require CheckStatus before kind eligibility.
Thus actual surviving Mario224, Goombella225 and Frankly180 can each produce
row1 when this real battle flag admits it. Zero status storage is not proof
of no icons or no eventual queued draw. Do not clear10000 early to suppress it;
btlDispMain's own clear happens after its complete footer.

False special checks execute real reset stores before return0:
guruguru→FLOAT Uad8=+0; Biribiri→HALF Uae4=0 thenUae6=0;
allergy→HALF Uade=0; regeneration23→HALF Uae0=0,
regeneration24→HALF Uae2=0. Biribiri retains CheckStatus6's U2f7 extension.
These helper-local resets do not erase the icon bank or change battle status.
Positive predicates remain their original record-animation/draw providers.

## Concrete next native providers and validation scope

The smallest source-backed progression can bind the five ordered inactive
effect leaves with actual packed predicates/handle history; handle nonzero
retains its appropriate delete address. Bind icon counter reset and real battle
flag gate. A flag-admitted pass must retain the actual position/callback/record
graph; its mandatory row1 may be the next provider even with status0. Once
these services complete, first valid linkedpart P1c0 reaches original status
pose selection/animation Main rather than a registry-empty shortcut.

Complete ignored listings display-status-* under artifacts/recovered-temp-evidence/.
Main/init body hashes:

| Body/address/size(hex) | SHA256 |
| --- | --- |
| EffectMain801966f0/4c | d802d92ebdf2da2c6d87d5ecc5c6267747c067c2949ea00a25e49cf9f0d04130 |
| EffectInit8019673c/4 | f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e |
| IconMain80253ca8/1dc | 61eb625bfb00fd1a5e47ee3ec687383420c0b4ba69ce7297f0ec9b5a7e6b1480 |
| IconInit80253e84/2c | e2badf5a7bff305272a988a25640dd84e4cb5907b1c3493b14d375ecae814708 |
| Sleep80196538/16c | c70a5a809f4e54578f88eb58eba90a5fa99634abbd582e115986fb22df7e3966 |
| Biribiri801963ac/14c | ed5f7eb03030b59d0e1d3239ee62a3fc88c49f526106561101c0766457962a49 |
| Fire80196240/12c | 49c209aecd5125efc24e8883982e4b299ca11d87416e29e4f210834cd9bbe2db |
| Freeze801960ac/154 | 8fbaf97225ef79746189ec1e382d27bd78277d9bcea6759dc88e539acfc35ea1 |
| Kage80195f8c/e0 | ccea0e6b5e92a260e6f7b163ccf1d4c3af3df3f0f46e119620de0844a7428170 |

Whole-body source/static table inspection and scoped documentation diff check.
No native tests/build/emulator or complete display/backend claim.
