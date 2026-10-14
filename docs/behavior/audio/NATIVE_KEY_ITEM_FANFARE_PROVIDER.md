# Native original key-item fanfare provider

Finite fresh group1 resource/playback provider for original item95 acquisition.
Source [routing](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md),
[container loop/retirement](FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md)
and [stereo gain/fades](FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md).
This is a provider, not the complete Goomba3 event or source BGM manager.

`field_item_get_assets.py` extracts original `sound/stream/ff_get2_32k.stm`
read-only, pins its original SHA256 and exports
`romfs/field-item-get-key-fanfare.wav`. Original32000Hz/stereo/83888frames,
PCM16, COMMON one-shot policy, coefficients/initial history and final-frame
extent are retained. Manifest retains original BGM row217/name, volume80/pan64,
ISO provenance and PCM hash. Converter fingerprint participates in cache
admission; a stale loop sidecar forces regeneration/removal. Packs are ignored
and carry existing original Nintendo/Intelligent Systems local-use restrictions.
Both soundtrack modes select this original asset via native track18.

`Audio::keyItemFanfareReady` validates the exact PCM header/length and absence
of loop sidecar once per audio lifetime. Playback requires working native DSP.
Resource readiness is separate from free-slot/playback admission.
`beginKeyItemFanfare` serializes with the refill worker, rejects an existing
fanfare owner and uses the first unoccupied slot in the existing three music
streams. It does not evict another field group/tail. Source forced replacement,
full-pool behavior and group arbitration are not claimed by this fresh-owner
admission. Failure does not invent an assigned group or successful presentation.

The shared3x3 stereo4096-frame ring uses the existing147456-byte linear allocation,
with no additional full-fanfare PCM residency. Native channels1/0/18 remain
partitioned by slot. Mixer publication precedes first NDSP buffer submission.
Source row217/type2 pan64 gives volume80 to both authored channels, hard-panned
left/right; native gain is single(80*source constant3c010204), with no additional
.65 multiplier for this track. Mono/output-mode/surround/aux and complete MusyX
hardware loudness equivalence remain separate owners.

`keyItemFanfareCheck` returns source-shaped -1 for vacant and0 for assigned.
Native retirement requires producer EOF **and** no QUEUED/PLAYING NDSP buffer;
then clear queue/close file/release slot before publishing vacancy. No elapsed
simulation/render timer or assumed2.6215s clock drives it. The refill worker
and query share musicMutex; buffer storage is not freed or reassigned while
NDSP retains a submission. `releaseKeyItemFanfare` explicitly clears the backend
queue and frees its slot; it is **not** the original Off201/1000ms group fade.
Shutdown disables callback, stops/joins worker and stops streams before freeing
linear storage/NDSP. Resource admission cache resets at shutdown.

Native drain is an explicit platform lifetime adapter. Original producer stop,
SoundSSMain clearing flags and psndBGMMain vacancy can precede final audible
sample; exact prefetch/device scheduling remains unverified. Ordinary native
track changes can preempt slots; future source event integration must retain
its group/tail ownership and distinguish preemption from natural completion.

Unbound: original Off201 integer group fade/detached100ms tail, group0 quantized
attenuation/resume and retained cursor, source slot replacement and global flags,
complete item-main dispatch/control/window/camera/pouch event binding. Explicit
backend release must not be wired as an equivalent Off201 request. Original A/B
acknowledgment may precede fanfare EOF; it must not wait for natural vacancy.
Provider validation does not claim a playable acquisition or physical3DS parity.
