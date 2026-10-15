# Native outer Main continuation and shared input owner

The complete original [outer update order](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
is implemented by `GameOuterMainCursor`/`updateGameOuterMain` for8006f904.
Private read-only evidence is `artifacts/outer-main-after-camera-evidence/`.
This parent does not certify its33 subsystem callees or complete field return.

Read current GP pointer and GP.word10 before any subsystem. Nonzero requests
OSYieldThread8029ccf4, then reloads the pointer and readiness word after the
child acknowledges. A held yield never falls through or clears GP. Ready runs
all33 original calls in order, beginning makeKey80005ff8 then seqMain8002e228.
The cursor retains each acknowledged prefix and nested pending identity. A
sequence wait does not rerun makeKey, repoll HID, restart Init or execute later
battle/camera/event/world owners. Finishing the parent holds at next outer
OSGetTick8029d864; profiling/end-render/repeating-frame ownership is separate.

The idle tail captures GP once after33calls. Clear word0 mask1000 returns
without clock/baseline work. Set scans four channels, each held-direction word,
held-button word and six analog bytes; first nonzero stops the whole scan.
Active input refreshes baseline low then high. All idle calls OSGetTime, reads
baseline low, hardware clock, then baseline high, requests signed wrapping-delta
division by clock>>2 and compares strictly>120. There is no zero-rate fallback.
On success first sequence query2 or fresh second query4 allows a fresh GP flag
read/OR2000/store. Child time/division/query/store waits retain prior operands.

`EventGpAddresses::bindPhysicalInput` exposes a stable Game-owned producer:
ready1324; nine four-channel word arrays1328..13b7; six analog byte arrays
13b8..13cf; force-motor1310..1313; mode/previous13d0..13d7. Big-endian byte,
half and word writes mutate the same output/countdown/motor state. Raw DemoPad
history stays separate. GP profiling1314 is not a motor byte. Retirement copies
the live aliased bytes. One region accepts the same owner repeatedly but rejects
a different owner. The backing must outlive its registered GP facade.

Native Game binds its retained producer when the trio begins. At makeKey it
uses current GP rate and motor-enable1294, supplied HID-to-PAD samples and the
existing source portable input producer. Native PADRead/clamp results and
hardware reset/motor requests remain declared platform adapters; New2DS XL has
no motor hardware. This does not execute original SDK pad services or recover
all process-global raw PAD/count storage. Four native channel samples follow
the existing native adapter policy, not measured GameCube connectivity.

After the finishing camera pass, Game enters this continuation before its
legacy field/event/presentation scope. Main supplies actual GPU-sync and field
Exit services to the shared OriginalSequence. Existing out-of-band sequence
polling and renderer field/icon updates are suppressed during this ownership.
Original current/requested/work/callback history is retained. The native
sequence can still be suspended at actual battleInit80073f20; no late Init,
forced state5/6, completion-bit write, sequence2 import or fabricated field
restoration acknowledges that callback. Native GP boot/disc readiness history,
remaining33-callee services, idle hardware clock and full outer loop remain open.

Host fixtures cover GP reloading after yields,33-call order, retained nested
Init wait without repeated input, middle-call wait, first-nonzero idle scans,
strict120/negative signed deltas, both sequence query routes, baseline order and
mutable GP input aliases including signed countdown/axis bytes and retirement.
Runtime evidence is recorded in [PROGRESS](../../PROGRESS.md). Source lifecycle
and [EVT-01](../../requests/EVT-01.md) remain the integration requirements.

Native150s ending evidence retains outer phase3/index1 and nested80073f20;
actual input is admitted once at live rate60 and sourceGP1324=1. The shared
sequence remains current/requested4,previous2,work[4,0,0,0,0,0,0,0]; live
Battle.ef4 already has80000000. These are native fixtures, not original runtime
measurements. Authentic entry must precede Trio admission: recovered Init has
real audio/control/event effects and Main0..4 perform resource loading before
battle_init clears GLOBALB and rebuilds ownership. Late replay after victory
would break lifecycle order. Private complete17-body recovery is retained in
artifacts/battle-sequence-init-ownership-evidence/; no raw payload committed.
