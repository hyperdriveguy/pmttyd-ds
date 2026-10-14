# Bero length float leaf

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `bero_length` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 8007a76c, `.elf` 436 bytes, 67-line `.disasm`, 3 `bl`
(`evtGetValue` 0x80031260, `evtSetFloat` 0x800304bc ×2 — both complete in
the [integer](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md) and [float
](../event/EVENT_FLOAT_OPERAND_ROUTING.md) operand docs). No JSON row. The
[road doc](OVERWORLD_ROGUEPORT_ROADS.md) cites this address as
`evt_bero_get_info_length` ("length-1 resolves 30 original units").

## Body

Event callback, returns 2. `evtGetValue(arg0)` → index; entry = word at
[0x803DC110 + 4·index] (absolute table base from `lis` 0x803E0000 − 16112).
Destination throughout is the raw second event word (no routing on it).

- If word at [entry+24] != −1: flip its high bit (`xoris` 0x8000), convert
  to float with the 0x43300000-bias double pair against 0x802C3058 — which
  reads 2^52 + 2^31 (0x4330000080000000) from the ISO, the signed int→float bias — and
  `evtSetFloat(entry, dest, float)`.
- Else: take low 12 bits (`lhz` + `clrlwi`) of [entry+4]; select SDA
  constant 30.0 (r2−28456, read from the ISO) when the value is 0, 3, or
  3840, else 60.0 (r2−28448); `evtSetFloat(entry, dest, K)`.

Signed comparisons throughout (`cmpwi`); the −1 test is signed equality.

## Not established

Table contents/length semantics beyond the road doc's account, operand
provenance, cadence, and any on-screen effect.
