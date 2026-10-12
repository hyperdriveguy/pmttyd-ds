# Complete event float routing: API asymmetry,quantization and old-value return

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtGetFloat80030840/4bc and evtSetFloat800304bc/384.
Read [integer routing](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[saved storage](EVENT_SAVED_SWITCH_STORAGE.md) and
[badge presentation](../battle/BATTLE_BADGE_RECOVERY_PRESENTATION.md).
Verify: `python3 tools/verify_event_operand_namespaces.py`.
Static local bodies;original numerical extremes and execution remain open.

Use integer document's E,C,C32(w),D(w),bit q/r/PPC_slw and wrapped address
arithmetic. Both APIs call evtGetWork FIRST,even for pass-through/no-store.
Comparisons on tag/word values are signed32. Fixed constants1024/1/1024 are
binary32 bits44800000/3a800000;bias is4330000080000000. Every fsubs/fmuls shown
rounds single independently. No IEEE-bit reinterpretation or unbounded host sum.

## Complete evtGetFloat routing

| Signed operand interval | Action / return |
| --- | --- |
| <=-270000000 | C32(raw operand) |
| (-270000000,-250000000] | C32(raw operand) |
| (-250000000,-220000000] | D(raw operand) |
| (-220000000,-180000000] | Read word(word(E+154)+4*(operand+190000000)) |
| (-180000000,-160000000] | swByteGet(operand+170000000) |
| (-160000000,-140000000] | _swByteGet(operand+150000000) |
| (-140000000,-80000000] | GF bit in C+84 indexed by operand+90000000;return binary32 0.0/1.0 |
| (-80000000,-60000000] | LF bit in E+dc indexed by operand+70000000;return0.0/1.0 |
| (-60000000,-40000000] | GW word C+4+4*(operand+50000000) |
| (-40000000,-20000000] | LW word E+9c+4*(operand+30000000) |
| >-20000000 | C32(raw operand) |

For EVERY word/saved-byte read row above,decode fetched w with D(w) when
w<=-220000000;otherwise C32(w). There is NO lower--270 bound here,unlike
GetValue's fetched-word rule. This means a very negative ordinary integer can
be decoded differently by integer and float APIs. Readers do not recursively
resolve a fetched tag as a second variable namespace.

There is no UF float branch:the whole integer UF+UW band becomes UW addressing.
There is no saved GSF/LSF float branch:those integer bands become GF addressing.
Such calls can use negative/out-of-bounds indices;the original has no validation
fallback. Do not unify the APIs with one semantic tag decoder or pretend unsupported
calls return0. Valid script/domain assumptions must remain explicit.

## Complete evtSetFloat(E,destination,x)

| Signed destination interval | Action |
| --- | --- |
| <=-270000000 | No storage;return supplied floating x |
| (-270000000,-220000000] | No storage;return x |
| (-220000000,-180000000] | UW word at user pointer154,index destination+190000000 |
| (-180000000,-80000000] | GF bit in C+84,index destination+90000000 |
| (-80000000,-60000000] | LF bit in E+dc,index destination+70000000 |
| (-60000000,-40000000] | GW word C+4,index destination+50000000 |
| (-40000000,-20000000] | LW word E+9c,index destination+30000000 |
| >-20000000 | No storage;return x |

No UF or saved GSW/LSW/GSF/LSF setter exists;their integer tag ranges are covered
by the broader UW/GF rows above. For GF/LF,compare supplied x unordered against
binary32+0.0:equal (including±0) clears the bit;all other values,including NaN,
set it. Return supplied x unchanged. Original bit addressing/PPC shift semantics
apply;no float-to-int truncation determines truth.

For UW/GW/LW:compute single(x*1024),read old word BEFORE fctiwz/store,convert
product with fctiwz,store wrap32(result-230000000). The stored word is fixed-point
encoded,not raw x bits. Return D(old) if old<=-220000000,otherwise C32(old).
This old-value return is independent of the newly stored x and has no lower
--270 check. Product/storage overflow,NaN/infinity/fctiwz/FPSCR behavior is not
locally clamped. Original tag ranges do not provide array/index validation.

## Derived ordinary cases and implementation tests

Scale1.5 stores-229998464 and reads1.5 as float,1 as integer. Setting1.5 over
old integer7 returns float7,not1.5. Setting NaN to a flag sets it despite integer
fctiwz being undefined/out-of-domain. A stored-270000000 is raw in GetValue but
D(-270000000) in GetFloat/SetFloat's old-value return. These are instruction-
derived cases,not runtime observations. Exact leading signed boundaries and
float-setter broad GF band are part of the original local behavior.

Acceptance cases:each boundary/base,integer versus float route asymmetry,
1024 quantization/intermediate rounding,old-value read-before-store/return,
negative/encoded words,flag±0/NaN and negative bit indexing. Full pointer/storage
source and valid numerical domains,persistence and original traces remain open.
This replaces the earlier scoped-local caveat for these complete four API bodies;
it does not complete all variable/control opcodes or event/runtime systems.
