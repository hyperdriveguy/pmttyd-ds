# Retained original Mario key and control queries

`field_mario_key_continuation` owns the complete original US
`marioChkKey8005c47c/13c` decision body. `originalMarioChkCtrl` owns the complete
`marioChkCtrl8005c464/18` leaf. The source verifier reads the supplied ISO
read-only, pins every byte of both bodies, and decodes all eleven linked child
calls. No original assets or extracted code are distributed by this module.

The key body calls PartyId80141050, PartyPtr800bf1b4, Sequence8002e210, reads
fresh Mario signed byte39, calls Ship800a4224 twice at distinct sites, then
Roll800a16dc and Slit800a68e8. Sequence must equal2, byte39 must equal0,
first Ship must differ from1, second must differ from2, Roll/Slit must be nonzero.
Only then does a captured null party return1. A nonnull party calls
Christine800c5744 (reject1), Bomhei800c0068 (reject nonzero), Yoshi800b6e94
(reject nonzero), reads fresh word0, conditionally reads fresh signed byte31,
and calls held-item800d0874. Word0 bit100 with byte31 equal7/5/6 rejects;
held-item zero admits. Byte31 is the original raw actor field, not a copied
presentation motion. No guest writes occur. The control leaf returns1 exactly
when byte38 equals0, including signed negative byte values returning0.

## Retention and native ownership

An unavailable query/read returns nullopt and retains the exact source stage.
Each callback owns any internal partial work; false/unavailable does not permit
its caller to replay already completed child work. Successful siblings never
repeat. Party lookup is typed separately because its output is a native actor
pointer and actual lifetime pins, not an invented guest address or stack cell.
An engaged null actor is a real lookup result; absent capture is unavailable.
The capture must retain the actual registry and, for a nonnull actor, its actual
incarnation owner. Neither an address, a hash of bytes, nor a token invented on
lookup constitutes an incarnation owner. `partyCurrent` must validate the exact
captured owners and reject retirement or replacement, including slot ABA.

Every service identity and user pointer is fixed until explicit reset. Reentry
and reset during a callback are rejected. The actual context must remain alive
through holds. The caller must run this owner serially with mutations of its
borrowed source owners. The `current` callback is an ownership predicate, not a
source game-state query. Guards run before and after external calls. Loss of
ownership observed inside the retained body poisons that invocation; it cannot
resume a successful child even if a later provider reports current again.
Source party flags/kind and Mario byte39 are read only at their actual use sites.
The Ctrl leaf is synchronous, holds no borrowed actor, and guards its byte read.

## Available runtime bindings and remaining frontiers

Trio already borrows the actual Game `FieldMarioBattleState` and registry at
entry and retains the actual `OriginalSequence`. The exact journal binds only
Ctrl to its actual byte38 view through the new leaf. It does not replace the
whole letterbox provider or change Bero/sequence gates. This borrowing requires
the existing Game/Trio call context to remain alive during the synchronous leaf.

A future Key binding may query the actual `OriginalSequence::state.current`
and actual `FieldMarioBattleState::keyDepth`; `fieldSequence` and a synthesized
follower1 are not substitutes. `FieldPartyRegistry::current(mario)` and
`pointer(index)` expose actual selection/storage, but the registry's unique_ptr
actors currently expose no immutable incarnation pin for arbitrary held Key
queries. Existing native Entry/Removal weak leases serialize their own source
work; simply observing those leases or a stable slot pointer does not protect
another retained reader from retirement/reuse. A real borrower lease integrated
with all assignment/restore/removal boundaries, or actual shared actor lifetime,
is required before binding nonnull capture. Game's fieldShipChanging,
fieldRollEnabled, fieldSlitEnabled and partner ability/status integers are
explicit harness adapters, not original constructor/history owners; they are
not used by this retained owner or the journal. Missing source getters remain
unavailable rather than returning their harness defaults.

The production-route native probe separately stops earlier at original
BeroEXEC8007ac40: actual checkpoint restore marks unsaved Bero history unknown.
This module does not establish that history or claim source Main activation,
visible class rendering, complete input ownership, or normal battle completion.
Physical input and the original menu style are unchanged.

## Validation

Focused host tests compare 1,024 gate combinations and all successful child
call traces with the existing synchronous source predicate. Each successful-path
child/read holds twice without replay; the conditional partner-byte hold also
retains its earlier flags decision. Tests cover both Ship callsites, null
partner early completion, all256 partner kinds with bit100, all256 Ctrl bytes,
late actor mutation, callback replacement, actor retirement/revival and reentry.
Strict host and ARM syntax checks, read-only whole-body/source-call pins and
ASan/UBSan pass. LeakSanitizer itself cannot run under the sandbox ptrace setup;
ASan/UBSan are run with detect_leaks=0. Full builds/registrations belong to the
root integration batch, not this isolated new-file delivery.
