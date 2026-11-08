# Retained original unit coin event

This owner implements whole btlevtcmd_GetCoin8010d914/6c and
BtlUnit_GetCoin80126ca8/78, and whole StoreCoin8010d8d8/3c +BattleStoreCoin800f7c0c/14,
including the complete BattleGetUnitPtr800f88cc/3c
registry lookup. `tools/verify_battle_unit_coin.py` verifies the five complete
read-only original US revision-zero DOL bodies. No original binary payload is
embedded in product code or added to source history.

The event reads argument0, resolves it with original evtGetValue, then captures
raw argument1 before BattleTransID. Only after translation does it load the
current global Battle pointer at8041eb90. Unit IDs outside0..63 produce an
executed null-unit result; valid IDs read the actual registry at B+20+4*id.
A missing memory/provider read remains unavailable and cannot become unit0.
The original translation remains a required whole typed child, bound to the
existing actual native BattleTransID helper.

Null unit returns zero coins without a kind or RNG access. A real unit first
loads kind from unit+10 and captures kind+12 as its unsigned-byte base coin
count. Every loop condition reloads kind and kind+10 as the unsigned-byte trial
count. Each reached trial calls actual irand(100), then freshly reloads kind
and kind+11 as probability; a signed random result below probability increments
the captured base. Kind mutation during RNG is therefore visible in the same
trial. Later kind/base changes cannot replace the original captured base.
At most255 trials follow from the source byte domain; the maximum sum is510.

The raw output destination is passed to the whole original setter exactly once,
then the parent returns original EVT result2. Failed atomic setters have no
acknowledged write; retries retain completed reads and RNG results. Missing
providers and source result zero are distinct. There is no pouch coin award,
drop animation, victory, death completion or field-return fabrication here.

The whole StoreCoin parent separately reads/resolves argument0, then loads the
fresh Battle global and actual Battle-information pointer B+2738. It reads the
information accumulator at+14, performs original wrapping32-bit addition and
writes that same actual cell exactly once. The captured read/add survives a
held atomic word store. This is an original Battle-information accumulator,
not a direct pouch award. Missing information memory holds; it is never
replaced with scratch storage or a guessed pointer. Getter and storage cursor
modes cannot be interchanged during a held invocation.

The cursor retains actual event identity, command/argument address, full service
and shared-owner identity, every reached memory/getter/translation/RNG result,
and the completed output. Provider replacement and recursive entry reject
before calling the replaced provider. The outer owner must retain borrowed
userdata/resources throughout the invocation; the Harbor adapter keeps the
cursor inside the actual Runtime and admits calls only while the actual live
Battle work matches that Runtime. It uses actual VM reads, original operand
getters/setter, actual BattleTransID and actual packed BattleRandom. No cursor
or borrowed Runtime userdata escapes that lifetime.

Focused strict host, ASan/UBSan and strict ARM tests cover every optional read,
translation, RNG and setter held twice, exact read prefix, all invalid unit-ID
branches, null/zero-trial branches,255-trial bounds, signed comparisons, kind
mutation across RNG, capture mutation, stale currentness, provider replacement
and recursive entry. Store tests cover every read/write held twice, negative
amounts,32-bit wrap, missing actual information and mode replacement. New code has no warning demotions; only pre-existing
Harbor caller indentation warnings are demoted in its ARM integration check.
The actual lethal-source CPU fixture uses coherent15584 headers/core plus the
frozen smoke package. Its smoke GPU-residency predicate is explicitly TESTONLY;
source scripts and RNG remain actual. Native pixels and complete normal battle
progress still require root fresh make test/make all and emulator validation.

The actual lethal fixture completes both GetCoin and StoreCoin against its real
Battle-information allocation and reaches original GetExp8010d9bc at95544f2c
with Crump0/Mario4, turn4 and no result. That is the next genuine unbound child;
no defeated-unit removal or victory is inferred from HP.
