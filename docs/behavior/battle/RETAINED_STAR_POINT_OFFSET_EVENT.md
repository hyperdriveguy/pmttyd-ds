# Retained original star-point offset event

This owner implements the complete original US `_add_star_point_disp_offset`
80113f88/174 and its complete BattleGetUnitPtr800f88cc/3c child. The read-only
verifier also pins signed conversion bias802eea38/8. Original code/data payloads
remain outside source history.

The parent captures Battle before resolving argument0 and translating the unit
ID. Valid IDs0..63 read the actual registry; other IDs produce the original
null unit. There is no invented null-unit success branch. Each unavailable
required operand holds. Raw X/Y/Z output destinations are captured in original
order before any setter; equal destinations still receive three writes.

The source reads bias X, captures raw Y, converts X, resolves Y, reads bias Y,
captures raw Z, converts Y and resolves Z. It then captures unit kind, reads
kind+90/+94 offsets, adds X, reads bias Z, adds Y and truncates X. Only then does
it read kind+98, convert Z and add Z. Setter X precedes truncation Y; setter Y
precedes truncation Z. The native adapter uses actual VM double subtraction,
float rounding, float addition, truncation and original EVT getters/setters.
Private4330/xoris double conversion bits are typed values, never fake guest SP
addresses. The native truncation provider admits its actual finite signed32
host domain; unknown FPSCR behavior remains unavailable.

The cursor retains every successful read, math result and setter separately.
Later holds do not replay an earlier source write. Borrowed event identity,
command, arguments and all service/user/shared-owner identities remain fixed;
replacement and recursive invocation reject. An actual currentness predicate is
required before source service operations. Native Harbor keeps cursors inside
the actual Runtime and checks its actual Battle allocation through the global
pointer. No callback escapes the owning Runtime.

Strict host tests exercise every optional operation held twice, exact read/math/
write order, live source mutations, aliased destinations, float rounding,
missing actual operands, provider replacement, retirement and reentry. ASan/
UBSan and strict ARM source/test syntax checks pass. Existing Harbor caller
syntax only demotes its baseline misleading-indentation warnings.

The actual lethal CPU fixture uses coherent15584 headers/core plus frozen
Smoke12/Coin5/EXP5 and this journal. Its smoke GPU proof and presentation receipts
are explicitly TESTONLY. Original death scripts, RNG and math are actual. This
leaf advances past80113f88 to the second evt_eff8006d970 request at95544f90,
name `star_point`; it does not complete that effect, award pouch rewards,
publish a result, finish the original end controller or return to the field.
Root integration requires fresh make test/make all and native validation.
