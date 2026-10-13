# Original cache syscall vector and installation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SystemCallVector8029c5bc/20,
__OSInitSystemCall8029c5dc/64, ICInvalidateRange80296b0c/34 and
memcpy8000519c/50hex. OSInit80295364/4e0 is linked only, with direct
installer call802954ec anchored. Addresses/offsets hexadecimal; counts/shifts
and byte strides decimal. Read [DC cache ranges](GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
and [DSP initialization](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md).
Static source installation/barrier/copy behavior; original boot/cache/DSP execution
and exception environment unmeasured.

## Complete vector and copied-length boundary

Original vector instructions execute in this order:

1. Read SPR1008 (HID0) into r9.
2. r10=r9 OR8; write r10 to SPR1008.
3. isync, then sync.
4. Restore SPR1008 from retained r9.
5. rfi.

There is a trailing nop at8029c5d8, but installer source end is exactly that
address: the nop is NOT part of the copied executable sequence. Full declared
vector size20hex versus actual copy length1chex (28 bytes,seven instructions).
No stack/register save, argument/syscall-number dispatch, memory-copy call,
conditional branch, scalar status or callback. r9/r10 are clobbered; the saved
SPR value is restored before rfi. Restoring that value does not establish that
other callers/interrupts cannot mutate it or that a trap permits this sequence
to finish. Do not replace rfi with an ordinary function return.

The vector explains the LOCAL barrier instruction sequence intended behind the
cache services' sc. Actual exception prefix/vector address, privilege/SRR state,
SPR bit effects, handler residency/patching and synchronization outcome require
boot/processor/runtime evidence. Pinning these source bytes is not evidence that
a particular runtime syscall reached this installed vector.

## Complete installer publication order

__OSInitSystemCall retains destination80000c00 in r31. Form source8029c5bc and
sourceEnd8029c5d8; request memcpy(destination,source,sourceEnd-source) with
length1c. Ignore returned pointer. Then:

- Request DCFlushRangeNoSync(destination,100).
- Execute explicit sync AFTER that child returns.
- Request ICInvalidateRange(destination,100).
- Restore ABI/linkage and return without independently assigned status.

Both cache lengths are100hex (256 bytes), exceeding the28-byte copied sequence;
they are not reduced to the source body size or rounded source length here.
The data request uses dcbf through the already-recovered NoSync child, not
DCStoreRangeNoSync or an sc-emitting DC variant. Instruction invalidation's own
sync/isync follows its loop. No allocator, IRQ disable/restore, range/vector-
ownership gate, pointer/result validation, checksum, copy retry or rollback.
Other memory in the publication range is not initialized by this installer.

OSInit contains the direct installer call at802954ec. Whole caller identity and
site are pinned, but its complete boot/admission/initialization semantics are not
closed by that one binding; do not infer the vector is always freshly installed
or cannot be replaced later. The installer requests do not exercise an sc.

## Complete instruction-cache invalidation range

ICInvalidateRange(P,N) uses the same unsigned range/count structure as the
[DC cache leaves](GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md).
N0 returns immediately, skipping loop AND barriers. Nonzero computes
K=wrap32(wrap32(N+(P&1f))+31) logical>>5; load CTR=K. Execute icbi with raw
unaligned P, add32 with wrap32, then bdnz. There is no K0 gate before first op.
After loop execute sync, then isync, then blr. No sc or child call.

Ordinary nonwrapping counts cover ceil((N+low5(P))/32) stride32 operands.
K0 from a nonzero wrapped length entails2^32 iterations if no trap/control change;
no such large loop was executed during validation. Actual cache line/address/
privilege/instruction-fetch behavior is separate from emitted operand sequence.
No normalized/aligned pointer argument, source-type or memory-ownership checks,
IRQ request or scalar status. r3 advances with iterations under ordinary return.

## Complete memcpy byte-direction and overlap order

memcpy(D,S,N) first compares S/D UNSIGNED. S>=D selects forward, including exact
same pointer: form S-1 and local D-1 with wrap32, counter=N+1; decrement before
each iteration, exit on0; otherwise load byte from preincremented source, then
store to preincremented local destination. Source load precedes destination store.

S<D selects backward even for disjoint ranges: form wrapped S+N and local D+N,
counter=N+1; decrement before iteration; nonzero predecrements source/destination
and copies one byte. Thus ordinary length N copies N bytes, last-to-first on
that branch. There is no overlap test beyond address ordering or vector/block
fast path; both paths handle N0 through the counter gate without a memory access.

Original r3 remains D and is returned unchanged. No alignment/null/region/overflow/
residency/alias/IRQ/type validation, cache instruction, syscall or child request.
Word arithmetic wraps, including N+1; counter decrements depend on zero rather
than signed-positive tests. No host memcpy undefined-overlap assumption should
replace this body's explicit original byte order. Installer's original source
address is greater than80000c00, so its requested28-byte copy takes forward path,
assuming addresses/contents retain the source binding.

## Evidence and remaining source/runtime

[Manifest](../../../tools/gekko_cache_syscall_vector_installation_evidence.json)
pins four complete bodies/two linked fingerprints: six spans,67 whole-body/caller
anchors/four direct calls (three installer/one OSInit). Copy source range/destination/
length hash and both cache lengths retained as numeric bindings. Four SPR save/
restore, six finite overlap/copy and six IC range/barrier vectors are deductions,
not original installation/syscall/cache/processor execution. Full fresh ELFs/
listings remain `/tmp`; no copied vector/bodies/binary assets committed.

Remaining: complete boot/vector/handler residency/patch owners, original exception/
MSR/SRR/HID0/address/privilege/cache/IRQ/ordering/trap/coherency domains, actual
copy/caller ranges/alias/lifetime and DSP command consumers, original syscall/
cache/voice/audio traces and native binding. Four local bodies/direct installer
site answered; full runtime/audio and EVT-01 remain OPEN.
