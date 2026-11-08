# Retained battle unit constructor

`resumePackedBattleUnitEntry` owns the complete parent
[BtlUnit_Entry80128fe0](BATTLE_UNIT_ENTRY_COMPLETE.md). The input BattleWork is
the caller's captured actual source global. Every reached source call has a
retained request carrying its original callsite, integer operands and floating
arguments. Engaged zero is the original early return; unavailable children
retain the request, local source values and next instruction without replaying
allocation, publication, stores or event creation. Each cursor owns one
invocation. Changed inputs/providers, reentry and sticky retirement reject it.

The first-free scan and checked unit allocation precede clear and registry
publication. Status, vector, gauge, linked part, separate first-body ID/part
lookups, both pose queries, display, special allocation, pouch import and root
event creation retain the original order. Publication remains early; no rollback
or deferred registration is added. Source triplet copies preserve the first two
loads before either store. Position inputs reload for each source setter;
held inputs stay captured until that child completes. The part loop preserves
the original entered-once behavior, including zero count's signed decrement;
its unchecked allocation/table domain is the owner's admission responsibility.

Resource providers must own their own continuations. This parent does not turn
an incomplete synchronous display constructor into a resumable child. In
particular, a completed catalog lookup grants no GPU readiness or pose handle.
The final event provider returns the actual EventEntry, whose source owner and
ID are then written once. No script execution, battle conclusion or field
result is inferred from constructor completion.

Tests compare complete unit/part storage and legacy child effects for six kind
branches, with/without init events. Each reached child is held twice, preserving
its identity/operands and all already-published storage. They also exercise
registry exhaustion, original allocation zero, first-versus-last body identity,
reentry, idempotent completion, input mismatch and retirement. Synthetic memory
fixtures are CPU ownership checks, not an original game trace or playthrough.
The original complete body and linked numeric/storage spans remain pinned by
`tools/verify_battle_unit_entry_complete.py` against the read-only US ISO.

The same module also retains whole `btlDispEntAnime801011fc`: null table/bank
stores its original minus-one handle; otherwise search(type28), actual string
copy and source Entry(mode1) each retain independent requests. It writes each
actual completed handle once, then reloads the next part pointer. The display
tests cover three distinct completed handles after repeated waits, original
null-table/null-bank paths, cursor reentry and retirement. This closes the
parent's replay hazard while leaving resource/heap/animation providers explicit.
