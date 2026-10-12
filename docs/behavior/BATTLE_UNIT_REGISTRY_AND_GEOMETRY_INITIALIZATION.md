# Unit registry,belong/height/body getters and complete geometry initialization leaves

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete named local bodies below,called by
[full unit entry](BATTLE_UNIT_ENTRY_COMPLETE.md). Verify:
`python3 tools/verify_battle_unit_entry_complete.py`.
Read [part world/hit positions](BATTLE_UNIT_PART_POSITION_QUERIES.md),
[direct damage](BATTLE_DAMAGE_APPLICATION.md),
[map/battle pool](MAP_BUFFER_FREE_AND_BATTLE_POOL.md) and
[Mario numeric wrapper](BATTLE_MARIO_JUMP_NUMERICS.md).
Static original DOL evidence;no original geometry/stat/source/execution trace.

## Registry and BattleAlloc

BattleGetUnitPtr800f88cc/3c takes explicit B and SIGNED ID. ID<0 or ID>=64
returns0;otherwise return word(B+20+4*ID). The special initial ID==-1 check
returns0 too. No B null/activity/kind/entry-ID check. BattleSetUnitPtr800f88b4/18
only checks SIGNED ID<64;negative IDs pass and store at B+20+4*ID using wrapped
address arithmetic. It does NOT share the getter's lower-bound guard. Setter
has no scalar contract. Unit entry itself uses only its checked0..63 free slot.

BattleAlloc800f8934/28 loads nested battle root8041e80c and forwards requested
byte word to _mapAlloc,returning its pointer. No size/root/failure normalization,
heap0 allocation or extra clear;map allocator already rounds/zeroes/flushes its
normal successful payload. Unit/part allocation later failure policy belongs
to each caller and differs,as full entry specifies.

## Belong getter widths and enemy mapping

BtlUnit_GetBelong80128144/8 returns UNSIGNED byte U.c through lbz. It does not
sign-extend,translate a team ID,look up battle context or validate U.
BtlUnit_GetEnemyBelong80127454/2c reads SAME byte but sign-extends it FIRST:
S8 value0→return1,value1→return0,EVERY other value→return that signed value.
It is not boolean NOT or an always-clamped two-team result.

Concrete width example:storedff gives GetBelong255,EnemyBelong-1. Stored02
returns2 in both. Some consumers explicitly narrow the unsigned getter result:
BattleDamageDirect at800fe1ec/428/620/704 calls GetBelong,then extsb at the NEXT
instruction before its zero test. Its effect/direction policy therefore uses
S8(GetBelong(U));the getter itself is not signed. GetHitPos801289ec instead
reads/sign-extends U.c directly before side-table lookup,as its contract says.
Do not change those consumer widths merely because the shared getter is U8.

Full entry copies raw setup.byte4 into U.c and separately uses S8(U.c) to fetch
side byte at captured B+8*belong+a for U189 and each part.bd. No local0/1 bounds
check in those side accesses. Original setup/side/source domain remains required.

## Height and body lookup

BtlUnit_SetHeight8012840c/8 stores low16 of supplied integer at U.ce without
clamp/scale/query;no scalar result contract. BtlUnit_GetHeight80128414/8 returns
SIGNED16 U.ce;no null guard. Full entry initializes it from signed kind.half18.
This is distinct from current/base vector Y scale and scalar U114.

BtlUnit_GetBodyPartsId80128c98/40 accepts null U and returns-1. Otherwise scan
linked parts from U14 until null. For EACH P read word P1ac;first bit1 set reads
P.source4,returns its word0 ID. Other nodes advance via P.next0. No positive-ID
validation,kind-count limit,cycle guard,part-source-null guard or activity gate.
Empty/no flagged node returns-1. It selects FIRST body-flagged part;entry's
U2d4 write inside every flagged part retains LAST flagged source ID. Those two
contracts must not be silently unified. PartsPtr lookup/negative-ID policy is
in the [part contract](UNIT_PART_DEFENSE_AND_LIFETIME.md).

## Complete position and sentinel triplet setters

Each plain setter writes f1,f2,f3 through stfs in X/Y/Z order,without getter,
clamp or scalar result:

| Body | Targets |
| --- | --- |
| SetPos80128c6c/10 | U3c,40,44 |
| SetHomePos801289a0/10 | U30,34,38 |
| SetPartsBaseRotate80128654/10 | P3c,40,44 |
| SetPartsRotate801287b4/10 | P48,4c,50 |

The following0x34-byte bodies independently compare each incoming axis via fcmpu
to float sentinel bit pattern0xcd6e6b28 (numeric-250000000). Exact equality
preserves that field;every other value—including NaN—stores via stfs. Reload
the sentinel before EACH axis. No common “all-or-nothing” gate or normalized
operand decoding. Different zero signs still store;sentinel is not zero.

| Body | Targets |
| --- | --- |
| SetOffsetPos801283d8 | U48,4c,50 |
| SetScale80128524 | U90,94,98 |
| SetBaseScale80128574 | U9c,a0,a4 |
| SetRotateOffset80128604 | U78,7c,80 |
| SetPartsScale80128470 | P60,64,68 |
| SetPartsBaseScale801284a4 | P6c,70,74 |
| SetPartsRotateOffset801285d0 | P54,58,5c |

Constructor inputs0/1 avoid sentinel and fill each relevant triplet. No direct
unit scalar114 update occurs in these vector setters. stfs and comparison/FPSCR
exception domains remain original numeric/platform requirements.

## Unit angle setters and complete reviseAngle wrapper

SetBaseRotate80128680/60 calls reviseAngle on each axis in order and stores its
returned values at U60,64,68. Retain Y/Z input values across earlier calls.
No sentinel checks or second360 gate in this setter.

SetRotate801288b4/b4 likewise calls reviseAngle for X/Y/Z,retaining other inputs;
after EACH result,fcmpo against float360 then CR EQ=GT OR EQ. If that OR bit is
SET,replace the result with positive0. Otherwise preserve it. Store U6c,70,74,
respectively. Thus the local extra gate is >=360 for ordered inputs;unordered
result preserves itself. Do not substitute an unconditional host modulo.

reviseAngle80007328/60 calls original fmod(input,DOUBLE360 at802bf240),then
rounds returned double to single. If the single is unordered with itself,
replace with positive0. Compare to positive0:if negative,add single360;if that
sum>=360,replace with positive0. Otherwise return. Negative zero survives the
nonnegative path. Underlying fmod/FPSCR/exception/errno behavior is separate;
its complete local wrapper is also documented in the linked Mario numerics.
Constructor uses positive0,so local normal outputs initialize both angle banks0.

## Complete HpGaugeInit80125fd8/b0

Store half0 at U200,U204,U1fc,in that order. Read signed U10c separately for
word stores U208 then U20c. Reload signed currentHP10c and maxHP108;convert
both using double bias802f00b8 then fsubs(single);fdivs(current,max);store SAME
float result U210 then U214. No max0/negative clamp or resource mutation.

Reload UNSIGNED U1fc. If exact1,set U202=40;reload it SIGNED,add30 and store
U206. Otherwise set U202=20;reload SIGNED,add30 and store U206. Ordinary serial
execution has just stored U1fc0,so that local branch takes20/50;do not retain a
caller's old mode1. No helper callback between its store/reload. Concurrent
alias/domain behavior is not normalized. Getter/stat/FP/status/pouch calls later
in entry may change resources;gauge ratio is computed BEFORE that later import.
No actual display/animation/death/recovery event request in this gauge leaf.

Acceptance:registry64 and negative setter asymmetry,nested map allocator;
U8 getter versus S8 consumer/enemy/high-byte/side cases,kind heightS16/low16 store,
first body/last cache/missing body;all triplet target banks,sentinel one-axis and
NaN writes,angle call/clamp/rounding/negative zero;gauge order/current-max0
exception boundary/mode reset/pre-pouch timing. Kind/part/side/scalar114/pointer
144 source lifetimes and original geometry/damage/spawn traces remain open.
