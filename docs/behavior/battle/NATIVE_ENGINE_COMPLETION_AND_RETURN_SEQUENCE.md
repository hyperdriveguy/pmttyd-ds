# Native engine completion and portable return sequence

The portable engine adapter implements the [original eleven-service order](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md)
and [lower wrapper](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md).
Upper Main captures its battle owner once, requests every service, then tests
bank0. Exact4 returns0; other values read the original GP flags, query bank0
again when both masks1000/2000 are set and preserve source forced-end writes.
Lower Main captures independently, skips null and ORs80000000 into its retained
owner only for an actual engine result0. It never clears an existing end bit.

The adapter's boolean request result means a provider is unavailable. It is
not an original service return gate. A false result retains explicit native
suspension and gives no engine result/end publication. Complete original
providers execute all eleven requests without a readiness shortcut. Requests
carry retained owner context, not invented ABI arguments for every callee.

## Native ending binding and corrected sequence target

Ending1f now invokes the original selector0 setter with value4, writing B+4.
The previous literal write to B+0 was wrong: it changed the two header
halfwords while leaving sequence bank0 at3. Read-only source confirms the call
at80216630 and setter dispatch at800f8a28; see the exact correction in
[service owners](ENDING_TOP_BANK4_SERVICE_OWNERS_AND_IDLE_DOMAINS.md).

On the next native tick, the new engine wrapper runs the existing broken-unit
adapter, four general pad updates with separately recaptured global owners,
source sequence manager's exact bank4 default and private pad/action adapter.
Native channel0 uses actual 3DS input; other channels use explicitly inactive
native samples. This is not evidence of original GameCube port state.
The first missing provider is StageObjectMain80149798, whose actual five map
objects cannot be replaced by an empty bank. That request suspends before
completion testing; stage/icon/spotlight/help/audience/slot/draw remain their
own real services. The source request trace retains this five-call prefix.

This completion pass follows the prototype's tick/input/event cadence; it does
not claim the original whole main loop or complete ActionCommandManager/GX/DSP
semantics. Existing field result/control hold, story11 and black fade remain
until the full service pass and original exit chain are bound.

## Portable sequence and incoming fade

The [sequence source contract](FIELD_RETURN_SEQUENCE_COUNTER_AND_INCOMING_FADE_ORDER.md)
now has portable implementations for battle states5/6 and field state0:

* State5 polls the actual wrapper end flag, clears counter, increments the
  current sequence state and only then reads GP+c to select state11. Its
  camera/listener tail runs even while blocked, through a required provider.
* State6 increments a wrapping32-bit counter first and compares signed>30.
  Both GP masks select(5,1,0); other values select(2,1,0). It has no listener
  tail and state5 does not enter it in the same pass.
* Incoming field fade reads retained32-bit ordinary presets or requests the
  special18/900 fade and original event before freshly clearing GP0 mask10.
  Every reset of f8/fc/108/10c recaptures current GP; the sequence increment
  rereads its current word. No state1 work runs through fallthrough.

Sequence transition/exit/init, retained original clock and fade/event services
remain caller-owned. These portable branches are not yet the native Game
sequence owner. They cannot authorize field result publication or substitute
for BattleEnd/map/context/NPC/event recovery.

## Validation and limits

Regression fixtures exercise exact request order, late-service flag mutations,
null/changed owners, fresh bank reads, unavailable services, all GP gate
combinations, signed counter wrap, 30/31 boundary, GP+c alternate branch,
ordinary/special fade presets and callbacks changing GP/state owners. Native
trio tests retain the original header, verify bank0=4, ordered prefix and no
end flag before the real stage-object provider. Full make test/make all results
are recorded in ../../PROGRESS.md. No new asset, original execution, emulator
capture, hardware performance or complete field return claim.
