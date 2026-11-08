# Retained original unit EXP events

This owner implements whole GetExp8010d9bc/6c, BtlUnit_GetExp80126d20/e0,
StoreExp8010d980/3c and BattleStoreExp800f7bfc/10. Source helpers include the
complete BattleGetUnitPtr800f88cc/3c registry lookup and pouchGetPtr800d5eb0/8.
The read-only verifier pins all six complete bodies plus the original signed
conversion bias802f00b8/8 and factor block804226a8/1c. Original binary payload
is not embedded in product source or added to history.

## Source read and math order

The parent reads/resolves argument0, captures raw argument1, translates the unit
ID, then reads the current Battle global and actual unit registry. IDs outside
0..63 resolve to the source null unit. The getter has no null-unit early-zero
branch: it still reloads Battle globally, obtains actual pouch pointer8041eb00,
reads signed player level at pouch+8a, then the unsigned enemy level at unit+d.
An unavailable unit+d read must hold; it cannot be fabricated as a zero result.

The getter reads Battle-information B+2738 before comparing levels. A higher
player level returns an executed zero without setup/type/constants/math reads,
but the earlier information-pointer read remains required. Otherwise it follows
information+c to setup, setup+18 to group and reads signed group type. Only
0..2 select factor1; type3 selects original1.1, type4 selects1.3, and every
other signed type, including negative values, selects1.5. These are actual
source constants at804226a8/b4/b8/bc, not default native multipliers.

After the selected factor, source reads the signed double bias, captures the
unit kind pointer and reads multiplier804226c0 before its first arithmetic
boundary. It forms the original private4330/xoris conversion bits internally,
without a fabricated guest stack pointer. The binary32 subtraction of the
converted double and source bias occurs before the late kind+f base-EXP byte.
Two separate binary32 multiplies follow, then original fctiwz, then wrapping
integer addition of the captured base. Neither a generic level formula nor a
fused multiply/add replaces that sequence.

The native adapter delegates double subtraction/float rounding, multiplication
and truncation to the actual VM platform providers. Its truncation adapter
admits only finite signed32 inputs supported by that existing host provider;
unknown/out-of-domain SDK/FPSCR behavior holds. The portable owner itself does
not clamp or manufacture a result. Source memory and optional math results are
retained in operation order, including equal-address repeated reads.

## Writes, ownership and integration

GetExp calls the original setter once with the captured raw output destination
and returns EVT result2 only after that whole setter succeeds. StoreExp is a
distinct whole source parent: resolve argument0, read fresh Battle, read B+f00,
perform wrapping32 addition and write that same actual accumulator once. There
is no direct pouch Star Point award, result, victory, EXP animation, defeated
unit removal or field-return substitution.

Each invocation freezes actual event/id/command/argument addresses, full service
function/user/shared-owner identities and getter-versus-store mode. A held
operation resumes without replaying any prior successful memory or math call.
False atomic stores acknowledge no write. Provider replacement rejects before
calling the replacement; recursive entry rejects. The outer caller retains all
borrowed userdata/resources. Native Harbor cursors live inside the actual
Runtime, guard its actual live Battle allocation and use its original VM
memory/getters/setter/translation/platform services. No borrowed Runtime cursor
escapes its actual lifetime.

## Validation and remaining boundary

Strict host and ASan/UBSan tests cover every read/getter/translation/math/store
held twice, exact source prefetch order, signed player-level/type matrices,
positive/zero paths, source base mutation after arithmetic, invalid units,
missing required early-zero memory, currentness/reentry, provider replacement,
mode replacement and wrapping stores. New source/test ARM checks are strict;
only existing Harbor caller indentation warnings are demoted for its integration
syntax check. The source verifier reads the original ISO without modification.

The actual lethal CPU fixture uses coherent15584 headers/core with frozen
Smoke12/Coin5 and this source closure. Its GPU-residency predicate for smoke is
explicitly TESTONLY; original scripts/RNG/platform math remain actual. Normal
native pixels and complete battle progression still require the root's fresh
make test/make all and isolated emulator checks. No HP-driven conclusion is
introduced by this package.

The positive actual lethal fixture completes both getter and Battle+f00 storage,
then reaches original _add_star_point_disp_offset80113f88 at95544f78 with
Crump0/Mario4, turn4 and no result. That is the next unbound source child;
no victory, unit removal or fabricated point count is acknowledged.
