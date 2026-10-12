# Visible position writer and body-ID event capture

US G8ME01 disc0 revision0;static read-only-ISO inspection2026-10-12.
Read [position writer leaf](BATTLE_UNIT_SET_POS.md),
[body-ID leaf](BTL_UNIT_GET_BODY_PARTS_ID.md) and
[position queries](BATTLE_POSITION_EVENT_CAPTURE_ORDER.md).
No original execution or unit/part producer/lifetime claim.

SetPos80112fcc/174 retains E/A,resolves/translates actor,then reads/resolves X,Y,Z
independently through GetValue,NOT GetFloat. Each signed word converts to single
precision. Only after resolutions capture current battle and lookup retained ID.
Always GetPos into three private temporaries,even with no sentinel operands.
An axis uses its prior coordinate iff the RAW operand isf1194d80;resolved equality
alone does not preserve it. Call the unconditional SetPos leaf with all three
selected floats,storing X,Y,Z in order;return2. No firstCall/null/-1/range clamp.
Numeric rounding/exceptional/FPSCR domain remains a separate platform boundary.

Anchors:actor80113000/10;X/Y/Z GetValue80113024/54/84;late battle8011309c;
lookup801130b4;old position801130c8;raw sentinel tests801130cc/dc/ec;leaf8011310c.
An encoded3.5 source passed through GetValue supplies integer3 before float
conversion;changing this caller to GetFloat changes its behavior.

GetBodyId8010ead8/74 instead captures battle BEFORE actor resolution/translation.
Capture RAW output AFTER translation but BEFORE lookup using retained battle.
Call GetBodyPartsId,then SetValue and return2. No wrapper ID-1 gate:the callee
itself accepts null unit and returns-1. Anchors:battle8010eaf0,resolve8010eaf8,
translate8010eb08,output8010eb0c,lookup8010eb18,body8010eb1c.

GetBodyPartsId80128c98/40 reads unit14 head;scan part.next0 while nonnull.
First part whose attributes1ac has numeric mask01 selects definition=part.word4;
return signed word definition0. It does NOT return part.next0 or a constant1.
No match/null unit/head returns-1. Matching part with null definition is not
guarded. First match wins;no HP/visibility/part-ID range filter or mutation.

Inspection-only ELFs remain `/tmp/pm-native-set-pos.elf` and
`pm-native-get-body-id.elf`,outside history. Reproduce with original US symbol
index and `tools/inspect_dol_function.py`. Native tests preserve captured/fresh
battle families,raw versus resolved sentinel,ordered full stores,template ID,
first body match and original-address callback/event/unit/part memory. Source
unit/part initialization,full motion/render/Jump/Game and runtime traces remain
separate requirements.
