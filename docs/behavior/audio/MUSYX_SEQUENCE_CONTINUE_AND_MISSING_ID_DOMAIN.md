# MusyX sequence continue and missing-ID wrapped-write domain

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete seqContinue80273b84/108
and sndSeqContinue80279e1c/38hex. Read
[lookup/stop](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md) and
[pause/mute](MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md).
Offsets/addresses hexadecimal; states decimal. Static source behavior only;
original reachable ID/list/audio runtime remains open.

## Inlined lookup with NO miss guard

Core retains input external word X. Inlines active-root8041ee5c then fresh
paused-root8041ee58 traversal; compare RAW node.wordc to X&7fffffff. First match
returns node.byte9 with original X bit80000000 inserted. Total miss yields
wordffffffff, as in the separate lookup contract. No initial sentinel/active/
IRQ/member/cycle/key-generation validation.

Unlike Stop/Pause/Mute, this body does NOT test the lookup result for-1. It
immediately branches on result bit80000000, so lookup miss follows the tagged
field-write branch below. A port must not claim a recovered graceful miss or
silence fallback. Valid lookup/private-ID invariants require source evidence.

## Untagged state2 paused-to-active transition

Untagged lookup result R selects Q=803ebdc0+wrap32(R*1888hex). No bounds check
against the original eight-instance array. Read Q.stateByte8; EXACT2 admits,
every other byte returns without further mutation.

For admitted Q, writes occur in exact order:

1. Read Q.previous(word4); nonnull writes previous.next=Q.next. Null writes
   seqPausedRoot=Q.next.
2. Read Q.next FRESH; nonnull writes next.previous=Q.previous.
3. Capture activeRoot; store Q.next=capturedRoot. Nonnull root.previous=Q.
4. Store Q.previous=0, seqActiveRoot=Q, then Q.stateByte8=1.

Own links are overwritten only by insertion; no whole-instance clear, note
reconstruction, voice allocation/restart, key/private-ID generation write,
project/resource reload or job/controller/hardware request. Pausing previously
killed/recycled note lists; this body does not restore them. Actual sequencer
track/note resume behavior belongs to its consumers. No identity recheck after
array selection or general unknown-state repair. No independent success code;
successful move leaves r3=0 from its final constant store, not a status protocol.

## Tagged branch and exact missing-ID address

For tagged result R, strip only bit31; compute
Q=803ebdc0+wrap32((R&7fffffff)*1888hex). Read Q.byteefa, clear bit8hex,
store it. No Q.state guard, list/voice request or retained high-bit status.
Other bits survive; the deferred byte consumer determines eventual behavior.

Total miss R=ffffffff therefore uses index7fffffff. Exact32-bit arithmetic:

`wrap32(7fffffff*1888)=ffffe778` (negative1888hex displacement).

`Q=803ea538; Q+efa=803eb432`.

Thus miss reads and rewrites byte803eb432, BEFORE seqInstance803ebdc0. It is
not a valid selected instance; no statement about its runtime contents or
original executed corruption is made. This is a derived address consequence of
the recovered source, not execution evidence. No null/array/ID guard prevents
it. Source callers must establish lookup success before faithful native binding;
a checked adapter rejection remains explicit adapter policy.

## Complete public wrapper

sndSeqContinue retains RAW X, requests hwDisableIrq, seqContinue(X), then
hwEnableIrq. No sndActive/validity/ID-1 precheck, normalization, fallback or
separate success assignment. Core return is not retained across Enable.
The wrapper therefore does not repair the missing-ID branch. IRQ membership/
thread ownership and direct frame-critical context remain separate contracts.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_sequence_continue_evidence.json) pins two complete
bodies and two linked IRQ wrappers: four spans, 53 instruction anchors and
three resolved direct calls. Seven ID/tag/address, five flag-clear and five
state-route vectors are derived checks, including exact miss addresses. Full
fresh ELFs/listings stay `/tmp/inbox-seqcontinue.elf` and publicseqcontinue file.
No original code executes/body/asset is exported to history.

Remaining: actual caller lookup-success/private-ID/source invariants, state/list/
track/note/deferred-byte consumers and valid member/cycle/alias/IRQ/concurrency
policy, original resume/missing-ID/audio traces. Two local bodies are answered;
full native/source/runtime/hardware equivalence remains OPEN.
