# Goomba3 coin fanfare row219 resource and nonloop retirement

US G8ME01 revision0; actual read-only ISO resource recovery2026-10-14.
Asset-specific follow-up to [coin20 service](../field/GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md)
and [shared stream retirement](FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md).
Addresses/offsets/masks hexadecimal; sizes/frame counts decimal.
This confirms the coin fanfare's own bytes rather than inheriting another
track's loop policy. No original runtime/native audible-parity claim.

## Exact original row and filename resolution

BGM table219 at80316878 is four original words:

    802ce248 80000000 802ce25c 50400001

Name BGM_FF_GET_ITEM1,pathsound/stream/ff_get4_32k,volume80,pan64,
surround0,globalpercentagebypass1. Coin state0 requests On201(name);
state100 requests On(a0,NULL) BEFORE Off201. The source callback does not
wait for row219 vacancy before granting/restarting caller quest instructions.

Original SoundSS path producer obtains root `.` through getMarioStDvdRoot,
formats `%s/%s%s` with `.stm`,then opens the same filename for two records.
Thus original lookup reaches **./sound/stream/ff_get4_32k.stm**.
[Filename/open ownership](GAME_STREAM_PLAY_WRAPPERS_REPLACE_SLOT_AND_HEADER_READ.md)
pins allocation/admission/open failures; no renamed key-item resource is used.

| Source resource fact | Value |
| --- | --- |
| ISO path | sound/stream/ff_get4_32k.stm |
| ISO absolute byte offset | 1337188296 |
| File bytes | 107744 |
| SHA256 | 9abc00c564a50d6b3dadc5a76ff11ab428a0556152a7d1550502bf03451d38de |
| Format/rate COMMON0 | 02007d00:format2,32000Hz |
| Channels/type COMMON4 | 2 |
| Decoded sample frames/channel | 65352 |
| Authored PCM extent/rate | 2.04225seconds |

The duration is file metadata, **not an equivalent source timer** for retiring
BGM ownership or a measured last-audible-sample time.

## Actual COMMON versus DSP loop fields

| COMMON byte offset | Original value | Meaning in recovered header owner |
| --- | --- | --- |
| 8 | 91e0=37344 | Common encoded total/channel |
| c | ffffffff | Nonloop sentinel |
| 10 /14 | 91e0/91e0 | Primary/secondary encoded end |
| 18 /1c | 0/0 | Primary/secondary encoded restart |

DSP subheaders64 and160(decimal) both contain samplecount0000ff48=65352,
nibblecount000123c0,rate00007d00,loop halfword1,format halfword0,
startnibble2,endnibble123c2,currentnibble2. Both initial histories are0/0.
They retain each channel's actual16 signed coefficients at header+28;
no replacement coefficients or audio resampling is authorized by this policy.

37344=ceil(65352/14)*8; here65352 is an exact multiple14. Endnibble equals
2*COMMONtotal+2. Source format2 coefficient transfer and loop admission are
[header-pinned](GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md).
Matched header callback copies COMMONc to rowrestart20; exactlyffffffff
normalizes restart0 without setting streamflags200. Fresh stream construction
writes flags1 first,clearing prior200. Therefore this actual fanfare is
**nonlooping on its admitted fresh row**, despite both DSP loop flags1.
A reused row not reset by the fresh producer retains its own flag semantics;
header sentinel alone does not clear a preexisting200.

Original secondary plane offset uses strict32 advance
T=COMMONsecondaryEnd+32-(COMMONsecondaryEnd&31)=37376. With format2
header256, primary encoded plane begins256 and secondary begins37632.
[Initial-read consumer](GAME_STREAM_HEADER_INITIAL_READS_AND_PENDING_PUBLICATION.md)
closes that source offset, not merely converter convention. File trailing
padding does not enlarge decoded65352frames. Original coefficient/history and
complete valid final-frame ownership remain source inputs.

Native PCM export for this verified resource preserves stereo32000Hz,
65352frames/channel and COMMON sentinel **no loop**; no .loop sidecar should
be emitted. DSPflag1 must not create an infinite fanfare. Generic validated
non-sentinel restart mapping is already linked in the shared contract; this
resource does not require a fanfare-specific decoder exception.

## EOS publication, stop and vacancy are owned transitions

The shared admitted format2 producer reaches COMMON rowtotal1c,loads rowrestart20,
and with flags200 clear ORs pending-stop4. This is a producer-span publication,
not proof every device-queued sample has drained. Original SoundSSMain admits
initial8/!2 and gives pending4 priority over fades; it clears captured4,
recaptures row, stores flags0 before closing/freeing original stream records.
[Encoded producer](GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md)
and [ordered stop](GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md) pin these
shared original owners, applicable to the actual fresh nonloop219 request.

SoundSSGetFlagCh returns currentflags; psndBGMMain sees0 and stores group1
index−1, after which BGMChk(1) is−1. Coin callback does not query that vacancy:
state100 can instead start group1's1000ms authored fade while the source
fanfare is still assigned; the detached SoundSS tail remains its separate
100ms owner. [Routing/envelope wrappers](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md)
apply the shared source flags/durations, not the keyitem row/name.

Native stream progression/refill/device drain and original prefetch can shift
producer EOS relative to last audible frame. Full physical timing is unproven.
Retiring from authored cursor/backend ownership can be implemented; starting
a2.04225-second wall timer or publishing vacancy at grant is not source evidence.

## Verification and remaining boundary

Resource was opened with rb through the ISO FST; all COMMON/DSP fields, row219,
literal names and hash were reread. Existing complete source contracts provide
filename/header/producer/stop/BGM composition; no original binary executed.
No runtime/shared index/progress/request edits or extracted asset commits.

Remaining native work: actual row219 PCM/NDSP resource binding, stream owner
retirement/refill/drain adaptation, source fade/global retained-state integration
and New2DS XL performance/audio checks. Original IRQ/callback/MusyX arithmetic
and last-audible-frame timing remain separate parity boundaries.
