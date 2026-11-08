# Retained battle voice and public pending sound

The new source owner implements the whole original voice callback80104508/d8,
unit-position wrapper8012619c/5c and positional sound child80126088/114. It is
separate from audio playback. Completing original On means allocating a genuine
original public **pending slot**, or returning its original rejection handle;
a sound-name vector or sample queue cannot substitute for that ownership.

## Original order

Voice captures the event argument pointer and actual battle global before
resolving selector/mode, then translates the actor using its genuine fresh
lookup and obtains the unit through the captured battle. Modes other than1/2
return EVT2 after those lookups and consume no RNG or sound. Mode1 draws5 and
reads the original five-key table80416ad0; mode2 draws10 and reads80416ae4.
Actual cold register initialization80005420/424 sets r13=8041cf20, independently
pinned. Neither table is inferred from a palette address or converted atlas.

The unit wrapper captures its operands, executes actual GetPos80128c7c in XYZ
order from unit+3c/40/44, then calls the positional child. It checks status11,
conditionally gets its signed strength and computes `(2*S-1)*-341`, narrowing
signed16. It independently checks status10 and conditionally overwrites the
bias with `(2*S+1)*-341`. A second irand41 adds caller bias and subtracts20,
then narrows signed16. Caller bias is a sound operand; it is **not** a read of
unit alliance. The original voice path supplies bias0.

The child invokes actual psndSFXOn_3D, applies pitch to its returned raw handle,
and selects volume. Full-word `(rawVolume+0x0ee70000) mod2^32 == 0x4d80` is the
only preserve-volume condition. Thus the original voice raw f1194d80 reads and
reapplies current volume. All other words use low8. The existing older contract's
masked-word/range interpretation and reversed status formulas are superseded by
[the corrected source contract](BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md).

Each accepted provider result is retained before advancing. Holds do not repeat
prior RNG, On, pitch or volume. Event identity/callsite, stable service identity
and strong supplied lifetime remain pinned; replacement/retirement/reentry
cannot acknowledge a held call. The cursor does not silently reset for a new
invocation. Its supplied current predicate must represent the actual owner.

## Native finite binding

`NativeBattleSound` composes those source requests over actual EventNativeRuntime
memory, actual actor/status/value readers, the supplied BattleRandom and the same
process PositionalSfxRegistry. The native path pre-admits all required immutable
name/table bytes, actual GP story/rate and original milliseconds scalar before
calling the existing nonretained `startPositionalSfx` once. This admission is a
serialized native port boundary, not a claim of additional original CPU reads.
No missing byte, GP history, sample, seed, volume or readiness is fabricated.

The admitted registry domain contains free or pending/nonstarted slots. Unknown
started-engine stopping/stealing requires its real backend and holds before On
mutations. All reached provider results are captured and total during the
serialized On call. An unexpected violated admission after entry poisons this
native owner rather than replaying its partially published prefix. Constructor
binding/registry identity and the actual process lifetime are retained.

Pitch on a pending slot writes its original low16 field. Existing raw handle-1
and empty-slot semantics remain no-ops. A started stream descriptor retains the
original suppression; other started pitch paths await actual SoundEfxSetPitch.
Volume uses the existing original public-slot accessors. No source psndMain,
macro/voice interpreter, native hardware channel, cooldown tick, audible sample
or finished-voice receipt is inferred from these pending handles.

The menu child800d98bc uses the same actual registry and standard original
nonpositional defaults. Numeric keys2520/2521/2523 have original assigned rows;
key2522 is genuinely empty (descriptor0) and returns original-1. An engaged-1
completes that source child. Unavailable is a disengaged optional, never0 or-1
invented to bypass a missing provider. Actual string-pointer keys use the
original name-table lookup as well.

## Production hook journal

The unapplied journal binds Game.publicSfx after a genuine Harbor begin, keeps it
through the actual Harbor runtime, routes Packed.invoke80104508 into the retained
owner and exposes startPublicMenuSound for the source UI's typed800d98bc child.
Rebinding a different registry is refused while that owner exists. It adds no
source-ready mutation, sample vector acknowledgement or physical-input remap.
The Harbor/Packed header change requires a coherent full core rebuild.

## Evidence and validation

`tools/verify_source_battle_voice.py` opens the US ISO read-only and pins eight
whole original bodies, actual r13 initializer, both voice-key tables (15keys total),
status arithmetic/volume branch bytes and the four original menu rows. Portable
unit tests exercise every provider boundary with two holds, modes, signed status
precedence, caller-bias wrap, both volume paths, service replacement and owner
retirement. Native tests use real matching VM source data, RNG, public rows and
slot owner, including genuine full-bank rejection without invented allocation.
Fixtures supply explicit process/unit/arguments/GP history; they do not prove an
ordinary battle schedule, audible output, physical timing or original hardware
parity. Parent integration owns complete make test/all and native tutorial gates.
