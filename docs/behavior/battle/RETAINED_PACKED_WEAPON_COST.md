# Retained packed weapon cost

`battle_weapon_cost_continuation` implements whole US
BtlUnit_GetWeaponCost80126968/158 against actual packed unit/weapon addresses.
The native weapon UI can call this owner directly; no temporary DamageUnit,
display estimate or default equipment snapshot is substituted. The separate
portable `weaponFpCost` remains a semantic reference for admitted typed inputs.

The source captures item at weapon+8 and base FP byte at weapon+11. If the item
is zero it skips pouch lookup. If the captured base is also zero, it returns
zero without reading any unit bytes, constants or numerical providers. This
is an original reached branch, not an unknown-state fallback. It lets genuine
free ordinary weapons remain free when unrelated badge/pouch state is unknown.

Nonzero item calls actual pouchEquipCheckBadge800d3300 with the captured item,
then reloads item from packed memory. Exact source ID memberships select linear
or exponential scaling. Linear multiplication wraps32 bits. Exponential scaling
reads the original signed-double bias and power base, converts copies-1 with
word wrap, calls actual pow80272620, then **reloads** the bias for conversion of
the captured base cost. Double multiplication and actual fctiwz remain distinct.
The source does not reload base FP after the pouch child.

A nonzero resulting cost reads the actual unit+2f5 reduction byte, subtracts
with word wrap and clamps signed nonpositive values to1. Zero skips that byte.
Signed costs>=1000 cap999. These are original source values, not measured timing
constants or native saturation policy. Genuine item counts/numerical services
are optional only on unreached branches; missing reached children hold.

The retained cursor captures each successful memory/child result, identity and
pending operands. Optional engaged zero is a real result. Unknown reads do not
become zero. Count/pow/fctiwz waits cannot replay earlier successful operations;
source/current loss is sticky, changed inputs/services reject, and request/reset
reentry cannot discard the prefix. Callback objects/backing must remain alive
and current through invocation. This owner grants no source frame/queue/GPU or
allocation readiness and never writes a cost to guest memory itself.

Evidence: `tools/verify_packed_weapon_cost.py` pins whole cost, actual pouch count
and pow-wrapper bodies, their linked calls, and both initialized double spans
from the read-only US ISO. Child pins do not mean those children are implemented
here. Tests compare boundary item groups/copies/base/reduction against the
independent typed reference; hold every read and child twice; test source item
mutation/base capture, post-pow bias reload, source free branch with absent unit
mapping, late reduction repair, stale lifetime and reentry. Focused test numeric
providers model the admitted finite arithmetic domain; original FPSCR/hardware
or executed-PPC parity is not claimed. Real native source memory/pouch/numeric
binding and whole weapon callback activation remain separate.
