# Native general rule judgment and audience storage

2026-10-13 binding of [general rules](BATTLE_GENERAL_RULE_JUDGMENT.md),
[threshold/query ownership](BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md) and
[audience bank](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md).

Portable battle_general_rule implements all68 selectors. It captures current
battle/descriptor, queries explicit-B Mario then party, and only then reads
the rule. Ignored rules still execute that prefix. Scalar branches retain
ordered unsigned byte loads/full word sums and signed Mario HP, short-circuit
threshold reads and helper global recapture. Scalar helpers write only through
the caller's captured descriptor; zero-sum/end helpers write their own newly
captured descriptor with no copy-back. End and turn zero helpers share their
byte-identical source algorithm without combining their calling cadence.
Direct turn rules retain unsigned comparisons and no255 bypass. Rules54/55
preserve their original repeated4b/4d fields;66/67 preserve differing last
39/38 loads. No counter reset, RNG, event or invented result value.

Native trio ending calls this real service after field partner restoration.
Fresh descriptor rule0 now writes its byte19=6 through actual packed memory,
then holds at AudienceSoundStop8019fd68. Original Mario/party searches use the
still-live battle registry, independently of the newly created field registry.
Story11, sequence4, field result0, control depth and fade remain retained;
Rule status is not a field battle outcome or permission to finish the event.

The audience base is now inline BattleWork+2858, matching original getter
801a86a0. Removed the synthetic descriptor-area pointer Work+400 and changed
all native idle/phase/turn-end/ending audience consumers consistently. Existing
Battle RAM19100 covers original19098; source audience zero span13914 and
24*24hex sound bank fit, including slot1 atB+15c78. Arena zeroing supplies
backing initial bytes, not complete Audience_Init or applause history. Current
phase tests read source-relative audience flags/state/counters. This changes
layout only; full original allocation/cache ABI remains a native adaptation.

Clap(-1,0) is still retained twice as an original request. Initial record0
does not prove final silence or justify skipping those producers; actual
count/type/existence/state and positional SFX backend providers remain open.
Full Audience_Init/Noise2, SoundMain, volume interpolation, stop/play lifecycle,
crowd render and original runtime/hardware behavior are not claimed complete.

Validation covers unconditional query/read ordering, ignored selectors, signed
negative HP/null Mario, widened sums, scalar helper recapture versus output
ownership, end-helper separate output ownership, no255 turn bypass and54/55/
66/67 field differences. Source verifier pins8 bodies/390 instruction anchors/
2 spans/60 direct sites. Separate selector audit matches all57 scalar/HP modes
and ordered offsets to the source-hash-verified contract. Native battle tests
retain audience phase/RNG-counter behavior at corrected addresses and observe
actual Rule0 status6 for joined/unjoined endings. This is not an executed
original-runtime rule comparison, full teardown or playable field return.
