# Retained original NPC clock section

This implements the complete clock region of original US npcMain,
80049b88..80049dc0 exclusive. The [complete parent graph](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md)
and [wander producer/consumer contract](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
remain authoritative. Source earlier active/query gates, pose flag, state10
setup and timer decrement, and later movement/detection/display/history/tail
remain caller work. This clock subsection returning true does not complete
npcMain or activate the original Area world in normal Game.

## Actual owners and captured memory

FieldNpcClockBinding accepts only the same initialized FieldNpcRegistryOwner,
registry facade and EventNativeRuntime. It retains the actual owner and admits
only its exact ordinary actor-slot addresses. It writes original actor bytes
through that VM facade; another facade bound to the same owner sees them
immediately. There is no semantic actor copy or copyback. Source Main must
perform its own current-bank selection, source record order, active/query gates
and pose/state/timer work before calling this subsection. The binding does not
invent those prerequisites or decide whether a slot should run.

The live GP pointer at80415fd0 and its timestamp38/3c supply time. The same
explicit VM platform bus-clock callback used by EVT supplies800000f8; this is
not the host/3DS CPU frequency, a cached binding-time value, a fixed timestep or
a render-frame timer. The original float literal1000000 at8041fd50 is extracted
unchanged into the existing private original data pack and read through the
facade after each conversion. No new packed format, dialogue, art or sound.

The cursor retains exact actor and service identity. Empty/false primitive
results have no effects. Each successful read, store, division and conversion
advances once; retry never rereads captured lanes, replays an earlier store or
restarts a partially published64-bit update. Successful services may mutate
admitted memory; later source reads observe those mutations. Retain the VM,
binding, mappings and actor lifetime through a hold. Main/room retirement must
discard the cursor before an actor is recycled; no automatic rollback/reset is
provided by this subsection.

## Source clock operations

State10 clear refreshes only the anchor: fresh GP, read high38 then low3c,
store low19c then high198. Delta, accumulators and all float outputs stay stale.
No numeric service is reached on this path.

State10 set captures GP, actor anchor low19c, GP low3c, GP high38, actor anchor
high198. Wrapped64 subtraction stores low194 then high190. Read bus and fresh
delta high/low. Dms=high32(10624dd3*(bus>>2))>>6; signed division truncates toward
zero. Only signed quotient>500 replaces delta with low32(Dms<<4),high0, in
low-then-high store order. Dms remains captured across that numeric request.

Fresh mainflag20000 chooses accumulated addition or low/high zeroing at17c/178.
The addition reads accumulated low, delta low, accumulated high, delta high
before low/high stores. Elapsed188/18c always uses the same low-first capture
and store sequence. Both additions wrap64. Fresh GP high38 then low3c refreshes
the anchor low19c then high198.

Four separate conversions publish elapsed188→1a0, accumulated178→180,
delta190→1a4 and anchor198→1a8. Each freshly reads bus, source high then low;
forms wrapped64(source<<3); computes Dus=high32(431cde83*(bus>>2))>>15;
requests signed division by positive32-bit Dus, then source signed64 conversion;
reads actual1000000 literal; performs binary32 division; stores its result.
Only then does the next conversion read bus/source. Negative shifted values
and negative delta retain signed behavior; no saturation or millisecond
rounding is introduced. Constants are recovered source values, not timings
measured on original or New2DSXL hardware.

## Numeric adapter and ARM compilation

Native division explicitly rejects divisor0 and otherwise preserves signed64
truncation, including INT64_MIN with a positive divisor. The native conversion
uses a volatile binary64 intermediate before binary32: original
__cvt_sll_flt80264e70 first rounds to double and then executes frsp. For
N=2^54+2^30+1, this yields float2^54; a direct integer-to-float conversion rounds
upward instead. Original helper listing and full39-child ABI review are ignored
under artifacts/npc-main-child-abi-audit/.

Compiler-generated ARM multiplication uses UMULL for the source high-product
clock divisors; signed division uses the devkitARM ABI helper. No cursor-owned
allocation or additional scan/GPU submission is introduced. This is compile
inspection, not a hardware performance measurement or proof of all Gekko
floating-point exception/rounding-mode behavior. Parent/provider scheduling,
original cadence and full NPC rendering still require integration and matched
runtime checks.

## Regression scope

The host test has an independent expected original memory/numeric trace,
fractional-millisecond precision, inclusive500ms boundary/501ms cap, negative
and wrapping values, stale stopped-clock dt, disabled accumulator, fresh bus
and post-conversion literal reads, and the signed conversion midpoint case.
Every reached primitive can hold independently on ordinary, stopped and capped/
accumulator-clear paths. Completed effects and captured reads must match the
uninterrupted source trace exactly. Separate tests reject swapped actor/service
identity and missing numeric/literal providers.

An actual original-data actor is entered through the initialized native registry,
advanced against the same live GP/facade, and read through another real facade
bound to that owner. These are explicit input fixtures, not an observation that
normal Game already invokes the complete original Main loop. Missing resource,
clock, Mario, numeric/audio/query/display services keep the overall parent and
EVT-01 open. See [progress](../../PROGRESS.md) for exact build and emulator validation.
