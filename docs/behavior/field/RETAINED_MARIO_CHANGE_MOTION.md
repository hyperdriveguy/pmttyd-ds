# Retained original Mario motion transition

`SourceMarioChangeMotion` implements whole original `marioChgMot8009455c/c0`.
The earlier force-wrapper API describes its typed child as SetMotion; its actual
original symbol is ChgMot. `SourceMarioStayPost` implements whole original
`mot_stay_post800a6b10/ac`. They use real source-owned words/halfwords and actual
post-child providers without inventing a full Mario allocation or guest stack.

## Source order

ChgMot captures the actual Mario pointer from 8041e900 and reads main flags.
Main bit10000000 returns without further reads or writes. Otherwise it reads
previous motion +30. Only previous16/17 freshly reads extra flags +4, clears
bit08000000 if set, freshly reads again and clears bit4 if set. It reads current
motion +2e, copies it to +30 and writes the low halfword of requested motion to
+2e. It then reads +30 afresh and selects the real post function from
80310030 + 12*previous +8. A nonzero child receives the actual table-entry
address, exactly as the source r3 operand at 800945f4. After that child completes,
it reads original captured actor flags+c afresh and stores flags|1.

Original table records0/1/2 respectively have post functions800a6b10/0/0.
Thus leaving ordinary walk/dash reaches no post callback. Leaving stay reaches
the whole StayPost owner; a missing callback cannot be acknowledged just because
the motion halfword has already changed. Other actual table post functions stay
typed reached children; no generic successful handler is supplied.

StayPost first captures Mario and reads the new motion. New motion19/20 selects
only sound+27c; other motions select +27c then +280. Each sound captures the
Mario pointer independently, reads the actual handle, and skips only exact
ffffffff. Otherwise actual psndSFXOff800d95f8(handle) must complete before the
original captured actor's sound field receives ffffffff. If Off changes the global
pointer, the clear still targets the original sound actor; the next sound uses
the fresh pointer. Off source callsites are800a6b50/74 or special800a6b9c.

## Actual native field binding

Root owns the canonical finite field view: real Game main/extra/dirty flags,
motion/history and actual source-constructor-derived sound slots. Whole original
constructor stores ffffffff at Mario+27c/+280 at8005b608..610; an admitted current
`NativeMarioConstructor.ui()->word(actualMarioAddress+offset)` can provide that
real initialization history. Do not guess those slots from absent playback.
Keep subsequent changes canonical; arbitrary original post children or unknown
handles remain unavailable until their genuine owners exist.

For WholeMovPos5, bind its motion request to one retained ChgMot owner, using the
actual requested action. Dispatch table-selected800a6b10 to one retained StayPost
owner, then dispatch actual reached800d95f8 to the real public sound registry
service. Preserve engaged negative/stale-handle original Off semantics where the
actual provider supports them. No timer or presentation pose completes a missing
source child. Reset a completed owner only for the next distinct source call.

## Lifetime and validation

Service/user/callback/full shared-owner identity and requested full word are
fixed across holds. `current` guards every source step; loss poisons the owner.
Reentry/reset during a hold reject without effects. Missing callbacks hold at
their actual use boundaries; suppress-change and null-post domains do not need
unreached services. A false store means no effects; child providers retain their
own partial work. This is a single-threaded stationary-callback contract.

Independent reference tests compare full read/write/call traces with each
external boundary held twice, including nested whole StayPost composition.
They cover previous16/17 fresh flags, fresh table-index mutation, low-halfword
request storage, captured versus fresh actor identities, sound zero/ffffffff,
callback/control-block replacement, reentry, suppress flags and source loss.
Strict host, ASan/UBSan and ARM syntax use only these source modules. A read-only
US ISO verifier pins both complete bodies, GetPtr and actual initial table
records. No Game routing, normal field activation, whole MarioMain or complete
motion-handler graph is claimed.
