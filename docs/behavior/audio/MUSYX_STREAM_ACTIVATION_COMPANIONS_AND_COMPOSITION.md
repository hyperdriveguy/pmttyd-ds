# MusyX stream activation companions and whole-body composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamActivate region
8027cca0..8027d0e0, end exclusive,272 words. Together with
[primary admission/returns/ABI](MUSYX_STREAM_ACTIVATION_PRIMARY_ADMISSION.md),
this closes the local body8027cab4/658hex. Addresses/offsets hexadecimal;
stage/count values decimal. Static source deductions, not original chain/audio
execution. Read [voice block](MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md),
[ID lookup](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Three companion stages after the primary row

Primary word0 other thanffffffff supplies companion ID. This continuation
processes three more IDs before making an actual recursive call. Each stage
initializes result holder r31=0 and requests hwDisableIrq; then searches for the
current raw companion ID among64 streamInfo803f98e8 rows, stride68hex. The first
two stages inline eight iterations of eight state/ID checks. The third calls
GetPrivateIndex. All select the first nonzero-state exact fullword ID match;
no sentinel input rejection, live-count restriction or generation decode.

| Companion stage | Disable | Lookup | voiceBlock |
| --- | --- | --- | --- |
| 1 | 8027cca4 | Inline starting8027ccb8 | 8027ce0c |
| 2 | 8027ce64 | Inline starting8027ce74 | 8027cfc8 |
| 3 | 8027d020 | GetPrivateIndex at8027d028 | 8027d058 |

Missing ID preserves result0 and unwinds this stage's enable and all enclosing
stages. Found row freshly reads state+c; exactly3 requests voiceBlock(U8 byte58).
After child STORE raw returned voice at word4c before checkingffffffff. Failure
returns0 through the stage-specific unwind below, without clearing state/freeing
buffer/restoring earlier rows. Any other word stores cursor20=0 then state+c=1.
Other nonzero states bypass voice/cursor/state writes. No post-child ID/state/
identity recapture, active hardware check or allocation-result range validation.

After each admitted row read companion word0 FRESH. Sentinelffffffff chooses
result1 and unwinds; otherwise the next stage consumes that raw ID. Callback/
voice/shared-cell mutations can affect this new read. Retained ordinal/state-base
still owns this stage's writes; there is no chain snapshot or duplicate suppression.

## Recursive continuation after four rows

After companion stage3, fresh word0 non-sentinel requests sndStreamActivate(ID)
at8027d0ac. This is the only direct recursive site in this body. Retain raw r3
return in r31; do not normalize or combine it with earlier row outcomes. Then
unwind all four levels in the current invocation. Primary plus three companion
stages therefore handles up to four IDs before recursion, without invoking the
public function separately for the first two inline companion stages.

No visited-ID set, recursion/chain-depth limit, cycle check, all-row reservation,
transactional rollback or hardware-start request. Even when a previous row has
become state1, revisiting it still follows its companion word. Self/cyclic active
chains can keep reaching recursion absent external mutation/exception; this is
not a claim that valid original providers create such chains. Stack/lifetime/
IRQ/ownership and acyclic-chain domains remain open.

## Exact enable/unwind paths

The primary's common final enable is8027d0e4. Companion success/miss/recursive
returns use the following suffix sequence; each call has no restore token:

| Deepest companion reached | Ordinary enable sites, in order |
| --- | --- |
| 1 | 8027d0d4,8027d0e4 |
| 2 | 8027d0c8,8027d0d4,8027d0e4 |
| 3 | 8027d0bc,8027d0c8,8027d0d4,8027d0e4 |

Companion voice failure replaces its own first ordinary enable with an earlier
call: stage1 at8027ce2c then directly8027d0e4; stage2 at8027cfe8 then8027d0d4/
8027d0e4; stage3 at8027d078 then8027d0c8/8027d0d4/8027d0e4. Failure assigns0;
companion sentinel assigns1. The outer result is moved r31→r28 then r28→r3.
Primary voice failure remains its separate enable/direct-epilogue route in the
prior contract.

Thus one finite invocation's local request counts pair with the number of stages
entered. These counts do NOT prove restoration of preexisting hardware IRQ state
or safe nested critical sections. Recursive/voice children have their own calls;
do not merge their effects into an assumed saved-state restore. Earlier successful
voice/cursor/state writes survive a later missing-ID/voice failure.

## Full local coverage and source checks

Prior two regions contain120 words; this continuation contains272 disjoint
words, covering8027cacc..8027d0ec without gaps. Fourteen separately anchored
prefix/epilogue words complete406 instructions/658hex. The register-save word
inside the primary region remains counted there, not duplicated as external ABI.
The prior manifest is reverified; anchor union covers every body instruction.
Whole local sndStreamActivate is now answered, including its recursive request;
this does not establish runtime chain termination or transitive audio fidelity.

[Manifest](../../../tools/musyx_stream_activation_companion_composition_evidence.json)
pins six spans,272 anchors,fourteen calls. Fifteen finite acyclic-chain models
cover lengths1..6, failures at each inline stage, missing links and already-active
rows; request counts/partial writes/recursive results checked. Models operate
on stable valid table examples and exclude child internals; they are not original
execution or proof of concurrent chain domains. Generic verifier checks hashes/
anchors/calls, not those semantic deductions. Fresh full ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed.

Remaining: companion writers/acyclicity/ID/state/priority/voice/IRQ/providers and
valid index/extent/value/alias/lifetime/concurrency/stack/ABI domains, stream
free/deactivate/frequency/mix lifecycle, transitive ARAM/DSP and original chained
activation/failure/audio traces. EVT-01 and full native/full-game binding OPEN.
