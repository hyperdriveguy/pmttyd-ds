# Field BGM global percentage initialization and slew

US G8ME01 disc0 revision0; readonly DOL recovery2026-10-14.
Finite fresh global-volume admission for
[plaza row67](ROGUEPORT_PLAZA_INIT_BGM_AND_ITEMGET_RESUME_OWNER.md).

## Fresh source value100 is an explicit initializer write

Original psndInit800dc4e0/170 reaches sound record803df6e8 through its
retained root+80. Source800dc600..618 clears soundhalf50/54/56 and
cooldownbyte5c,then stores literal100 in byte5a,byte59,**byte58**,in that
order. Current BGM percentagebyte58 is address803df740. It is not an SD
sound-enabled setting, inferred speaker volume or a default from the BGM row.
The initializer is the actual source producer for fresh100.

The verified dynamic byte58 writer is psndBGMMain800db7cc prefix:

1. Set local target100.
2. Read soundhalf56; if mask20 set AND mask40 clear,target65 instead.
3. Query seqGetSeq8002e210. Sequence3 skips percentage mutation.
4. Otherwise read GPword1c; mask1 clear skips percentage mutation.
5. If admitted,read byte58,sign-extend; if current<target,increment its raw
   unsignedbyte and storelow8.
6. Reload byte58,sign-extend; if current>target,decrement its rawbyte and
   storelow8.

The second comparison reloads the result of the first write. There is no
single snapshot,min/maxclamp or immediate target assignment. In ordinary
0..100 domain this moves at most1 toward target per admitted invocation.
Raw signedbyte domains/wrap and provider mutation retain source semantics;
no interpretation as an unsigned0..255 fader is established.

## Flag setters affect target, not the current percentage directly

psndSetFlag800dc110 ORs requested mask into soundhalf56; psndClearFlag
800dc0f8 AND-clears requested bits there. Neither writes percentagebyte58.
Setting20 with40 clear changes the later BGMMain target to65; setting40
can keep target100 even while20 remains set. Those flag owners/lifetimes
are separate from the currentbyte,which can retain an intermediate value.
This verified local producer graph does not assert a complete indirect/REL
writer census for arbitrary later game states.

Item-get's soundflag80 is distinct from20/40 and does not alter this target.
In the finite genuinely fresh source domain (completed psndInit, no subsequent
20-without40 attenuation, currentbyte100), BGMMain preserves100 whether its
sequence/GP gate permits or skips slew. Thus original plaza row67 may use
G100 legitimately under those source conditions. An imported/save/runtime
owner must carry its actual percentage/flags; it must not silently restore100
just because a native sound-toggle bool is true.

Row67 bypassbyte0 consumes signed currentbyte58 for globalpercentage.
Fanfare row217 bypassbyte1 initializes groupbyte10ff,so its group usesG100
independently of this sharedbyte while that bypass remains active. Group0
attenuation/resume retains its original sharedpercentage domain throughout
Dialogue/item acquisition. See
[gain arithmetic](FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md).

## Evidence and limits

Fresh ignored source inspection psndinit.txt and bgmmain.txt under
artifacts/reference/goomba3-services pins stores800dc610..618 and complete
prefix800db7f0..860. Original seqGetSeq symbol/address verified against US
symbols. No nativecode,sharedindex/progress,assetcopy,profiles or saves edited.
Next bind actual source-initialized percentage/flag ownership to retained
fieldgroup0; native/hardware audio validation and complete later writer/flag
lifetimes remain open.
