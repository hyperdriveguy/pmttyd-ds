# Action-class table: admission, storage, and cursor publication

This closes the complete local `_btlcmd_MakeActClassTable` body at
`80124dc8`, size `0x5b8`, for the native menu-domain request in
[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
Evidence is static US G8ME01 disc0 revision0 DOL disassembly, not an original
execution trace. [The verifier](../../../tools/verify_action_class_table.py)
pins all 366 instructions, the complete body, three linked service identities,
and seven original NUL-terminated message keys. Per-class enable checks and
presentation requests are in [the companion contract](ACTION_CLASS_TABLE_ELIGIBILITY_AND_MENU_KEYS.md).

## Captured owners and mandatory entry order

The sole argument is battle address `B`. Retain it; load `WORD(B+0x420)` and
request `BattleGetUnitPtr(B, actorId)` at `80124de8`, retaining returned `U`.
Compute and retain `M = wrap32(B+0x171c)`. Call `pouchGetPtr` at `80124df4`,
retaining `P`. Load `S = WORD(B+0x2738)`, then `D = WORD(S+0x0c)` at
`80124e04`. Only afterward does the branch at `80124e08` honor the comparison
`M == 0`. There is no prior actor, pouch, setup, or nested-definition null guard.
The wrapped menu-address test is not a test of `B == 0`.

On the ordinary nonzero-M path, read `K = WORD(U+0x08)`. Exactly `K == 222`
selects class cursor `C=M+0x498`; every other value selects `C=M+0x4a4`.
This retained cursor does not switch when later kind reads change. Start local
count `N=0`. Use that same first kind read to admit the initial Jump row.
Party, Hammer, Item, and Special each read kind again at their own admission
points. Item reads it once more for its Mario-versus-other restriction branch.
All message lookups use retained owners; an implementation cannot replace this
sequence with one snapshot of every input before issuing the lookups.

## Row representation and ordered admission

For the current local ordinal `N`, let `R=wrap32(M+20*N)`. Writes are:

| Raw address | Width | Meaning established by this producer |
| --- | --- | --- |
| `R+0x08` | word | class/menu type |
| `R+0x0c` | word | enabled value, 0 or 1 |
| `R+0x10` | word | address returned by `msgSearch` |
| `R+0x14` | halfword | icon number |
| `R+0x18` | word | reason value, 0 or 3 |

Equivalently, each descriptor begins at `M+8+20*N`, with field offsets
0, 4, 8, 12, and 16. Its bytes 14..15 are untouched padding. Neither the
menu's leading eight bytes nor unused descriptors are cleared. Do not memset
the entire table or assume old rows beyond the final count disappear.
Disabled admitted rows remain in the table and count.

Admission proceeds in this order; all integer kind comparisons are signed:

| Stage / instruction range | Admission read | Emitted type |
| --- | --- | --- |
| Jump `80124e24..80124f34` | retained entry kind equals 222 | 0 |
| Party `80124f38..80124fe4` | fresh kind >=224 and <231 | 6 |
| Hammer `80124fe8..80125118` | fresh kind equals 222, then signed byte `P+0x99` >=1 | 1 |
| Item `8012511c..80125204` | fresh kind equals 222 or is 224..230 | 2 |
| Special `80125208..8012528c` | fresh kind equals 222, then unsigned halfword `P+0x8c` !=0 | 4 |
| Operations `80125290..801252ec` | fresh `WORD(B+0x0ef4) & 0x08000000` equals 0 | 5 |
| Lecture end `801252f0..8012534c` | another fresh `WORD(B+0x0ef4) & 0x08000000` !=0 | 13 |

The final two stages are independent tests. When flags remain stable, precisely
one emits. If the Operations message lookup changes bit `0x08000000` from
clear to set, both emit; if its initial test sees set and the next read sees
clear, neither emits. A single `if/else` implementation loses this behavior.
Neither final stage has an actor-kind admission guard.

With stable kind and flags, Mario's sequence is Jump, optional Hammer, Item,
optional Special, then Operations or Lecture end (at most five). Party kinds
224..230 emit Party, Item, then the final class (three). Other stable kinds
emit only the final class. These are conditional summaries, not runtime
observations or global count bounds under changing service-visible inputs.

Each emitted stage increments N exactly once. Jump sets N=1 after its icon.
Party, Item, and Operations increment before the final icon halfword store;
Hammer and Special increment after the icon; Lecture increments after its
post-message restriction check. Preserve the underlying store/service order
when supporting observable shared memory or callback mutation.

## Count and index publication

At `80125350`, write `WORD(C+8)=N`. Load the current signed `WORD(C+0)`
(index), then the current signed `WORD(C+8)` (count), and compare them. If
`index >= count`, write `WORD(C+0)=wrap32(count-1)`; otherwise leave index
unchanged. Negative index values are not clamped to zero. A nonnegative index
with count zero becomes -1. `C+4` remains untouched. There is no explicit
menu-type (`M+0x540`) publication in this body.

The ordinary path leaves r3 containing the freshly loaded count. The early
wrapped-M-zero path leaves the pouch getter's return in r3. There is no
explicit success/status assignment or event completion value 2. Preserve
this distinction if exposing a raw ABI return; derive a portable interface
only at a documented caller boundary.

## Native port requirements and remaining source proof

Implement the descriptor builder with address-backed fields, typed signedness,
retained U/P/D/C owners, ordered service requests, partial writes, independent
final flag tests, and the literal index correction. Reuse the original keys,
text, and icon assets; UI adaptation must not invent menu labels or dialogue.

This proves what class records can be produced, including type13. It does
not prove which row a player or confusion selector chooses, when its type is
copied to `M+0x540`, or whether a weapon query can reach an unsupported type.
[Setup/decision](WEAPON_MENU_SETUP_AND_DECISION.md) and
[type/cursor reuse](WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md) remain the
linked consumers. Complete ActMenuMain, confusion, Input, class setup,
message/asset services, event reachability, and original execution remain open.
