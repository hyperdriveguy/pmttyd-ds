# Retained original Bero initial-position event

Source closure: [original initial-position event](../overworld/BERO_RETAINED_INITIAL_POSITION_EVENT_CLOSURE.md).
Dependencies: [ordinary root bank and leaf centers](HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md),
[position callbacks](NATIVE_POSITION_CALLBACKS_AND_ENTRY_CAMERA_RESET.md) and
[normal placement](NATIVE_BERO_POSITION_AND_START_PUBLICATION.md).

The converter retains unchanged US DOL8030f060..8030f2d8:632 bytes,45 commands,
SHA256214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519.
It includes the actual adjacent original word at8030f2d8 solely as the label
scanner's END-prefetch context. That word is not part of the claimed script.
The descriptor remains PMBRIO02/148 bytes; bero-info.pmevt grows to2496 bytes.
No original executable PPC code is linked. Mutable globals are excluded from
immutable imported data.

FieldBeroInitialEvent executes those original words through EventNativeRuntime.
It admits registered known ROAD kind0 only, seeds the supplied real16-word bank,
and runs a standalone priority1 root as a bounded native scheduling adaptation.
It does not claim the original attached parent, full area-init ordering, pipe
branches, door children, or shared field scheduler. Ordinary native root zeros
and selected into_info outputs have source provenance; arbitrary provisional
house05 snapshots are not substituted.

The runtime owns a FieldBeroInfo snapshot at begin, including incoming-name
storage and immutable catalog ownership. Later caller mutation/destruction does
not redirect a held entry. This is an explicit native admission boundary;
concurrent original GP/registration changes during the held event are not modeled.
The event itself cannot be copied/moved. Game handles may observe shared state,
but use_count!=1 holds advancement of a pending VM, preventing two Games/handles
from advancing one retained entry against different world providers.

Original callbacks query into_info and length, perform source normal placement,
READF mutable starting globals into LW10/11/12, set Mario, set raw party0 then1,
and reset camera4 road/shift state. The normal callback captures Mario before
operand resolution, publishes six IEEE32 words in SX,SY,SZ,EX,EY,EZ order at
8041e9f8/f4/f0/ec/e8/e4, then obtains Mario again for recovery+2e4/8/c stores.
Recovery is final F.X, single(37+F.Y), F.Z. Only afterward it reads motionU16
from the originally captured Mario+2e; motion25 obtains Mario again and writes
reload+2e0=1. Other motions preserve the unknown/previous reload word.
Global readback is unavailable until the actual helper publication; allocated
zero RAM is not certified as a known startup value.

Position/camera services retain their recovered operand/store order. Native
recovery words retain actual IEEE32 writes and reload is optional until written.
The native void/reload consumer remains unbound: recording the motion25 store
is not a complete map reload. Native root scaling, libm/FMA and finite conversion
admission retain the numerical boundaries described in the dependencies; no
PowerPC SDK/FPSCR or native-to-engine round-trip equivalence is claimed.

Missing floor/world providers and unadmitted callbacks hold the actual event.
Completed earlier callbacks are not replayed while a later callback is held.
The last pinned camera callback captures actual locals before the remaining
RETURN/END retire the entry; finished requires real allocationCount0 and that
captured bank. The LW7 output script is not run by this original root.
Native arrival movement waits for completion and then consumes the actual
encoded start/final destination. Save restoration reconstructs unsaved entry
state. The public legacy no-catalog adapter remains only for existing host
harnesses; native boot requires the original catalog.

A retained entry allocates a512KiB guest heap and source event contexts once per
road entry. It adds no display callback or per-render-frame geometry work. This
is not a hardware performance measurement. Future work: share the actual field
parent/case/area lifetime, execute preceding door children, and feed this real
returned bank into original house05/case initialization before acknowledging
complete area init. Full original traces and additional areas remain open.
