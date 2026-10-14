# Audience holding, throwing, damage and escape controllers

US G8ME01 revision0, read-only DOL reconstruction; not Dolphin measurement.
Whole local bodies in
[`battle_audience_item_control.cpp`](../../../source/battle/battle_audience_item_control.cpp)
and [`battle_audience_sound_start.cpp`](../../../source/battle/battle_audience_sound_start.cpp).
Native binding extends the existing member-state provider; it does not insert
new audience calls into outer Main or acknowledge item allocation/flight.

| Original | Address / bytes | SHA256 |
| --- | --- | --- |
| KinopioItemOn | 80191a48 / 168 | 943e85e194f2c6d576eed37379df869feacd8f02d96686d003174b69e6c4f5f5 |
| KinopioItemThrow | 80191980 / c8 | 4199a48922b69373563e38292e1e8424c8b0f60efaa8e4833c80e0487498e379 |
| KinopioDamage | 8019173c / 148 | 70393f8f1106c014a1d519f2e31760160c5c7d162ebab5d4216a9d39bfdb796f |
| KinopioEscape | 80191290 / 1e0 | a25e3b31cd9cfb6cfef345787e824d4ef828465bf0dd485e1caceca329046139 |
| ItemGetPtr | 801a8624 / 40 | 7761387b8d9f71a7e88dbc500f1461f49ecafd0c8ad71aaa24b4ea43cb4a0213 |
| GetEscapeChangeOK | 801a27e8 / fc | d32196a3a80c20cd1ae7cde5c5fb6f8463ff40a48bac62c860819d65a8d638cc |
| GetSysCtrl | 801a2960 / 60 | c925063fca01bf9adf87713d9ebfd6f20d2951189ddfc799332aa03ba11d6da5 |
| SoundItemThrow | 801a044c / f0 | 7b5136a0626bcf3d753c495d90b0eca0cf52876204cafb3386850bed45a26899 |

All counters/pointer arithmetic retain32-bit wrapping; scalar instructions round
separately to single. Missing linked target/RNG/sound services return false
with earlier stores retained. Unsupported controller addresses return false
without stores. These bodies are not general resumable cursors: native pending
state stops the surrounding bridge at an unavailable service.

## Correct record ownership and interruption penalty

ItemGetPtr(index) returns fresh audience A+f25c+wrapped(index*48), without a
range clamp. It never returns the battle-unit table. Actual source item slots
are0..99; the arithmetic itself preserves signed-index wrapping.

Interruption penalty reads **audience item0's kind at+10**, independently of the
held item index. If kind>=236, it reobtains item0 before comparing kind<240;
236..239 suppress adjustment. Otherwise fresh audience baseline13778 multiplied
by original80424570 (-.14999999105930328) is passed to whole AddTargetNum with
bias0. The initial idle port's battle-unit interpretation was incorrect and is
fixed separately; opposing table-kind regression tests prevent its return.
This original item0 quirk is not replaced by the held item's kind.

## Holding (actor state7)

Capture B, audience A, then actor R. Select animation0 for front low-byte1,
otherwise5. If actor flags40 is set: capture ItemGetPtr(signed R1c), clear actor
flags60, write R1c=-1, clear that item's flags word, call whole ChangeStatus10,
then apply the item0 penalty. This arm precedes all suppression/countdown gates.

Otherwise actor flags20 returns. Captured B flagsEF4 must contain80. Resolve the
held item before testing captured A flags40000; that global bit suppresses the
countdown. Decrement item word8 even when its active flag is clear. Only when
item flags1, signed timer<=0 and item state word4==1: ChangeStatus8, then
SoundItemThrow(0,slot). No item-state/flight write is invented here.

## Throwing (actor state8)

Select animation6, call original AnimationEnded and, on result1, ChangeStatus0.
Then reload actor flags. Flags40 still overrides this reset: clear60,
ChangeStatus10 and apply the same item0 penalty. Thus an animation-complete and
interrupted call performs state0 then state10 in source order. Without flags40,
the final source flags20 test has no subsequent stores or calls. It is not an
extra suppression or item-unlink operation.

## Damage (actor state10)

Capture base then actor; valid phase word4 values are0/1. Other values return.
Phase0 selects animation9, writes height velocitydc=8, acceleratione4=-1,
timer30=0 and facinge8=270, increments phase and falls through. Phase1 resets
scale64=1 and timer30=0 when signed timer>=8, then subtracts source.1 through
timer4 or adds it afterwards; increment timer30 before AnimationEnded.

On animation completion: scale64=1,timer30=0; GetEscapeChangeOK decides between
whole ChangeStatus0 and ChangeStatus12 followed by psndSFXOn_3D(key802f9698,
captured R48 position). There is no synthetic animation-end timer.

GetEscapeChangeOK captures A and R, rejects A flags4000, requires fresh
GetSysCtrl (GetExist flags1 with102 clear, then captured actor flags10 clear),
rejects captured R flags200 and states12/13/14/15/17/18/21/22. Other unsigned
state bytes are allowed, including values above the member dispatch range.

## Escape (actor state12)

Always select animation7. Type11 has a separate path. Phase0 decrements count8
and waits while signed>0; otherwise writes velocity78/7c/80=(0,-1,0), increments
phase and returns. Phase1 deletes at positionY<=source-100 and increments the
captured phase even after Delete. There is no raw RNG or escape sound here.

Other types, phase0: decrement count8; wait while signed>0. Call raw rand once,
convert signed result to single, then preserve the source operation sequence:

    vx = ((2*float(rand))/32767 - 1) - (positionX/120)
    vy = 0; vz = 8; heightVelocity = 6; heightAcceleration = -.5

Increment phase, then request psndSFXOn_3D(key802f9684,R48); return without
phase1 fallthrough. Phase1 advances to2 only when height acceleration==0, again
without fallthrough. Phase2 deletes when positionZ>=source600. No angle, RNG
range, movement or output clamp is added. Member position/height integration and
negative-height clearance remain in the dispatcher's later motion tail, which
also runs on the call that deletes the member. Whole Delete preserves its
existing paired-seat behavior.

## Original throw-sound record and native binding

SoundItemThrow first gets actor position, selects sound slot10 when X<=source0,
otherwise11 (unordered takes11), captures that record, then stops the existing
slot through the real stop provider. Kind0 selects key802fa43c; nonzero selects
802fa458. Sound2 obtains a fresh actor position after the stop. Store the returned
public handle into captured record+4. Actual -1 rejection returns without
installing active/lifetime/volume fields; unavailable is not substituted by -1.

For an actual handle: set flags1, lifetime+c=-1, fade+10=0; query public volume,
store bytes1d then1c, zero words18 then14, and set byte1e=100. Missing volume
preserves the handle/active/lifetime stores and leaves prior volume fields.

Native sound start setup is shared with the existing clap/noise path. It uses
original key/row lookup, original positions, shared public registry and existing
volume/stop providers. Damage/escape ignore an actual failed sound return as
the source does. Public pending handles are real registry requests; this is
not a claim that source sound macros or NDSP voices are playing.

## Tests, scope and remaining owners

Tests distinguish audience items from battle units and held-item kind from
item0 kind. Holding checks cover front-byte animation, all countdown gates,
active/state requirements, signed underflow and partial target/sound stores.
Throwing checks simultaneous completion/interruption order. All256 escape-state
bytes and flags are checked; damage wobble/end/eligibility and both retreat
branches run through their boundaries. Coupled member motion drives both types
to deletion. Sound tests cover lane capture before stop versus fresh position,
real handle initialization, -1 rejection, missing volume, X0 and unordered X.
Pack tests pin all additional float constants.

Item allocation/selection at801a5aac, live item Main/flight/collision and item
rendering remain separate providers. Actor Enter11 and special states13..20,
idle low-ratio caller-register provenance, full normal Main/camera cadence and
real voice playback remain open. These checks do not prove playable throws,
visual telegraphs, successful audience taps or a full battle. Physical X and
optional touch interruption must feed the recovered input/selection owners;
no touch-only flow or fabricated target is introduced. Original menu/HUD art,
dialogue/story, action-command timing and lower-screen scene replay are unchanged.
