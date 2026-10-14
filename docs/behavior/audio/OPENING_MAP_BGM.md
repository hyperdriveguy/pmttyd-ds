# Opening map BGM group512 handoff

The used BGM table gives house80, opening86, sailing78. Group512 is ordinary group0, source mode512 has no fade-in bit. Its map off request starts a120tick integer volume fade. A subsequent same-group `BGM_OPENING1` On replaces group ownership; the old active house SoundSS stream receives the original default750ms floating tail, from its volume at replacement. It does not remain a second group0 integer fade. `OpeningMapBgm` retains this limited branch, and the new Audio overload uses the existing three fixed streaming owners. Title/demo ownership and native effect channels are unchanged. Later unrelated Off modes and all-group SDK behavior are outside this contract.


Read-only bodies and table rows are pinned by [verify_opening_map_display.py](../../../tools/verify_opening_map_display.py). Source map event order and native display admission live in the [story contract](../story/OPENING_MAP_DISPLAY.md). The simulation clock adapts original streaming volume updates to60Hz; this does not assert audible DSP parity.

The producer additionally accepts the original later boat off request6000ms
(360simulation ticks) through `OpeningSequence::requestBgmOff`. Its native Audio
overload retains the existing owned group through phases with no new On request,
so heading/Wake admission cannot erase an active source fade. A duplicate Off
while already fading leaves its original duration/remaining state intact. On
completion the published primary track becomes absent, retiring its native
stream while retaining the logical group identity against repeated route polls.
The root boat owner invokes this request at acceptedTick+240, source33c58;
this worktree's older Voyage fixture does not fabricate the newer boat callback.

An explicit original `requestBgmOn(track)` can restart a same-ID group after
its Off, matching `psndBGMOn_f_d`'s same-ID/state2 branch800db484..4ac. Repeated
native route polls do not create such a request. This distinction lets the root
bind later actual source On requests (including any used arrival prefix) without
turning a finished fade into an implicit On every simulation tick.
