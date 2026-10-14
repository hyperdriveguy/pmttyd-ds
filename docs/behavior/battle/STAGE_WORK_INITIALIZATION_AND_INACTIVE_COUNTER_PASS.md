# Stage work initialization and inactive/counter Main pass

US G8ME01 disc0 revision0, 2026-10-13 read-only complete
BattleStageInit80139780/e4 and BattleStageMain80139150/630 listings.
Next used provider after [spotlight Main](STAGE_SPOTLIGHT_WORK_INIT_AND_MAIN_ADMISSION.md).
This closes initializer scalar writes, inactive-entry cleanup and four global
counter requests. It does not fabricate empty state for active stage entries.

## Work owner and complete local initialization

BattleStageInit captures global B8041eb90 once at80139798, retains W=B163fc
in r31, then memset(W,0,b3c) at801397a8. No separately allocated W handle.
W0=0 at801397b8. For i0..11, E=W+4+i*e8: store WORD E0=0 and WORD Ee4=0
at801397cc/d0. These redundant explicit writes follow the fullmemset and
establish the original entry/scratch initialization, not new allocator ownership.

Local writes then initialize separate ambient/background records. Unlisted
bytes below retain actual memset0; they are not uninitialized defaults.

| Site | Width / W offset | Value |
| --- | --- | --- |
| 801397e0 | WORD ae4 | 1 |
| 801397ec/f4/fc/80139804 | BYTE ae9/ae8/aea/aec | 0, in this order |
| 80139808/0c | WORD af0/af8 | 0 |
| 80139810 | WORD b04 | 1 |
| 80139814/18 | BYTE b09/b08 | 140, in this order |
| 8013981c/20 | BYTE b0a/b0c | 0 |
| 80139824/28 | WORD b10/b18 | 0 |
| 8013982c | FLOAT b24 | 290 |
| 80139830 | FLOAT b28 | 200 |
| 80139834 | FLOAT b2c | 90 |
| 80139838/3c | Reload FLOAT b24; store FLOAT b30 | actual reloaded b24 (ordinary290) |
| 80139840 | FLOAT b34 | -50 |
| 80139844 | FLOAT b38 | 200 |

Float literals are80422b3c/40/44/48=(290,200,90,-50). The b28=200 initial
field is the actual escape_wall Y provider used by StageObjectInit; do not
confuse it with spotlight Work8 or invented groundlevel0.

After ALL writes, call StageObjectInit80149bd0 at80139848, then
batSpotInit80138068 at8013984c. No local failure branch or teardown occurs.
[Original seven-record initialization and map requests](SETUP4_STAGE_OBJECT_CACHE_AND_NAMED_MAP_REQUESTS.md)
remain genuine child services. A native initializer's scalar storage is a
valid original projection while child provider readiness remains distinct.
The12 E entries initially inactive do NOT mean whole W is zero/empty:
Wae4/Wb04 are explicitly1 and retain the above material/colour/position inputs.

## Main: retained owner, required spotlight then twelve entries

Main recaptures global B once at80139160, retains W=B163fc in r31, then
ALWAYS calls batSpotMain8013800c at8013916c. It does not check ending/topbank,
SP/pouch8c or entry count before that request.

It walks exactly12 E=W+4+i*e8 in ascending order. At80139180/84 reads WORD E0
and tests mask1. Clear jumps to inactive cleanup80139584:

1. Read WORD Ee4 at80139584.
2. Exactly0 skips request/store, then advances to next entry.
3. Nonzero calls BattleFree800f8908(pointer) at80139590.
4. AFTER callback returns, store WORD Ee4=0 at80139598, through the retained
   E pointer, then advance. If callback changes registry/global B, this local
   field owner is not recaptured. Do not pre-clear before callback.

Other entry bytes/counters are untouched by inactive cleanup. The pointer is
not an eventID or texture handle: original mapheap BattleFree domain applies.
Counter/lifetime outputs elsewhere do not skip this cleanup.

Set E0 bit1 selects ACTIVE graph. Its colour, motion, interpolation, effects
and scratch graph must remain a real provider or explicit boundary. Native
may use predicate-gated inactive pass only from actual imported/retained flags,
never force active E0=0 because ending now wants to return. Activecolour
completion can clear flags and skip its remaining work; activeposition branch
also owns a separate Ee4 free at801394f4 before its scale interpolation.
Those conditional active producers are outside this inactive closure.

## Four ordered global counters and interpolation ABI

After the12 entries, Main executes ambient/background counters in exact order:
Waf0, Waf4, Wb10, Wb14. For EACH: read signedWORD counter; <=0 skips ALL
writes/requests/output mutation for that counter. Positive:

* Compute wrapping WORD remaining=old-1 and store it FIRST.
* Load start/end as unsignedBYTE→float; mode as signedBYTE (extsb).
* Read original duration as signedWORD and REREAD stored remaining.
* elapsed=duration-remaining with wrapping WORD subtraction.
* intplGetValue80006c8c(mode,elapsed,duration,startFloat,endFloat).
* Convert returnedfloat with source fctiwz; store low8 of integer into output
  BYTE. There is no output clamp to0..255 or HALFWORD store.

Actual operands/sites:

| Counter / decrement store | Start / end bytes | Mode signedbyte | DurationWORD | Call / outputbyte store |
| --- | --- | --- | --- | --- |
| af0 /801395c0 | aea /aec | b00 | af8 | 80139608 /ae8 at80139618 |
| af4 /80139630 | aeb /aed | b01 | afc | 80139678 /ae9 at80139688 |
| b10 /801396a0 | b0a /b0c | b20 | b18 | 801396e8 /b08 at801396f8 |
| b14 /80139710 | b0b /b0d | b21 | b1c | 80139758 /b09 at80139768 |

ABI is r3=mode,r4=elapsed,r5=duration,f1=start,f2=end. Arguments are fresh
for each request; output from an earlier counter can be mutated by callbacks
before later reads. Positivecounter1 stores0 before the interpolation request
and ordinarily submits elapsed=duration. Counter0/negative never executes
interpolation and remains unchanged, including negative sentinel values.
Fresh exact Init countersall0 therefore skip these four numeric providers
without changing ambientcurrent0/backgroundcurrent140. This domain is
determined by actual counters, not by their active ambient/backgroundWORD1.

intplGetValue is the FLOAT system helper, not intpl_sub integer-square helper.
Original mode-specific curve/validduration/fctiwz nonfinite domains remain
providers when positive counters are admitted. Existing native source-float
interpolation can be reused only within its recovered mode/admission domain;
no whole-callback success or guessed endpoint assignment is supplied here.

## Minimal native readiness and explicit boundaries

A faithful bounded pass needs actual StageWorkW fields initialized above,
real persistent spotlight Work/bank provider, twelve retained entry flags and
scratch ownership, mapheap free callback for inactive nonnull scratch, and
four counter readers/writers with original interpolation requests onpositive.
Allinactive/nullscratch/allfourcounter<=0 is a closed scalar no-change domain
AFTER actual spotlight Main, while Wambient/background remain active original
records. Changed counters and active entries are not silently discarded.
Full source StageMain active graph, stage/effect render callbacks and map/GPU
provider integration remain separate implementation work.

## Evidence

Complete original raw DOL SHA256:

| Body | Address/size(hex) | SHA256 |
| --- | --- | --- |
| StageInit | 80139780/e4 | c692d3c176e90df5800a44f8416eb8f1ad5772a59318f60756544db5af0e6bf2 |
| StageMain | 80139150/630 | f61083bae0f1b5fcb95e3490e3fa3216a3f34ebe9838dd5bf1440a1ee11f62e5 |

Complete listings under ignored artifacts/recovered-temp-evidence/ending-top4-
BattleStageInit/Main.dis. Checked initializer order/values, active/inactive
branch separation, callback-before-clear owner and allfour typedcounter/call/
byte-store sites. Documentation only; no native/shared docs edits, build rerun
or commit. Scoped diff check passed; original active stage readiness remains
an explicit service domain rather than an inferred false-empty bank.
