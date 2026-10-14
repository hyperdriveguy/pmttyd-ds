# GX channel control and partial color-register setters

US G8ME01 disc0 revision0, 2026-10-14. Complete original SDK bodies recovered
read-only for [material-light selection](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md).
The source instruction graph and primary GX interface documentation are separate
evidence: register writes alone do not establish rendered-image equivalence.
No external implementation code or table is incorporated here.

## RGB, alpha and combined control dispatch

GXSetChanCtrl802b6f78/b0 takes channel,enable,ambientSource,materialSource,
lightMask,diffuseFunction,attenuationFunction in r3..r9. For ordinary documented
enums, source bit packing is:

| Control word bits | Source |
| --- | --- |
| 0 | materialSource low1 |
| 1 | enable low1 |
| 2..5 | lightMask low4 |
| 6 | ambientSource low1 |
| 7..8 | diffuseFunction low2, forced0 when attenuationFunction==0 |
| 9 | attenuationFunction!=2 |
| 10 | attenuationFunction!=0 |
| 11..14 | lightMask bits4..7 |

The original boolean sequences for bits9/10 are equality/non-equality tests;
do not extend an alternative enum-range formula to arbitrary invalid inputs.
The body does not locally validate channels or enum values. Initial XF index
is100e+(channel&3), followed by the special combined-channel duplicate below.

| Channel | Meaning | XF control register writes |
| --- | --- | --- |
| 0 | RGB0 | 100e |
| 1 | RGB1 | 100f |
| 2 | alpha0 | 1010 |
| 3 | alpha1 | 1011 |
| 4 | combined RGB0/alpha0 | 100e, then1010 with the same word |
| 5 | combined RGB1/alpha1 | 100f, then1011 with the same word |

Each XF write sends command byte10 followed by register word and packed control
word to FIFO cc008000. After all admitted writes the body sets SDK work
HALF+2=1. It does not replace both controls for a channel0 or channel2 request.
Source channel4 is a combined dispatch, not a separate raster-color bank.

For source requests(0,0,0,1,0,2,2) and(2,0,0,1,0,2,2), both packed words are
501hex, written separately to100e and1010. The disabled register-material
variant with materialSource0 instead packs500. Therefore an earlier combined4
shadow request using materialSource0 is not the same state as the material-light
helper's separate vertex-material requests.

## Ambient and material RGB/alpha partial updates

GXSetChanAmbColor802b6d6c/e8 takes channel and an RGBA pointer. In the valid
0..5 channel domain it combines the requested components with retained SDK
color-register state, then emits one XF write and updates that retained word:

| Channel | Color bank | Replaced components |
| --- | --- | --- |
| 0 | 0 | RGB from input word high24; preserve retained alpha |
| 1 | 1 | RGB from input word high24; preserve retained alpha |
| 2 | 0 | Alpha from input byte+3; preserve retained RGB |
| 3 | 1 | Alpha from input byte+3; preserve retained RGB |
| 4 | 0 | Complete input RGBA word |
| 5 | 1 | Complete input RGBA word |

Packed words are RRGGBBAA in the original big-endian address domain. Ambient
registers are XF100a/100b and SDK work words+a8/+ac. For example, requesting
channel0 RGB cannot implicitly reset the prior alpha; requesting channel2 alpha
does not change prior RGB. Invalid channel branches return without a local write.
For each valid call: FIFO command10, XF register100a+bank, packed word; set
SDK HALF+2=1; then store the packed word in the SDK cache.

GXSetChanMatColor802b6e54/e8 uses exactly the same component-preservation
selection. Its XF registers are100c/100d and cache words+b0/+b4. Ambient and
material caches are distinct. The caller's raw RGBA pointer is not modified.
These helpers do not clamp or convert component values to floating point.

GXSetNumChans802b6f3c/3c puts the input's low3 bits into SDK work word204
bits4..6, sends XF1009 the low8 input, then ORs mask4 into work dirty word5ac.
There is no local0..2 range guard. Correct public use still follows its defined
channel-count domain; invalid XF/cache values are not normalized here.

## Disabled channel output and the flags0 native domain

Primary [devkitPro GX documentation](https://github.com/devkitPro/libogc/blob/master/gc/ogc/gx.h#L1723-L1743)
defines source0 as register and source1 as vertex color. With lighting disabled,
the selected material source passes through as the channel output. RGB0 and
alpha0 are separately controlled but rasterized together as COLOR0A0;
NumChans1 selects this combined output. This explains hardware meaning rather
than inferring it solely from the original SDK bit writer.

For actual material flags0 and the source-proven inactive battle light bank,
mapSetMaterialLight's final request order remains:

1. GXSetFog(0,ffffffff,0,0,0,0).
2. GXSetNumChans(1).
3. GXSetChanAmbColor(4,000000ff), from the real fallback/ambient query result.
4. GXSetChanCtrl(0,0,0,1,0,2,2).
5. GXSetChanCtrl(2,0,0,1,0,2,2).

There is no GXSetChanMatColor request in this F&4-clear path. Retained ambient
black does not multiply disabled vertex-source output. Native vertex-RGBA
passthrough is justified after both actual control writes, channel-count state
and downstream TEV selection have been applied. It does not establish enabled
lighting, preset-light processing, register-material source behavior, another
channel count, or the whole material shader. Preserve separate RGB and alpha
material-source owners even when their current controls happen to coincide.

[Bank/preset lifecycle](BATTLE_MAP_LIGHT_BANK_AND_PRESET_LIFECYCLE.md) supplies
the real32-record map bank, reset/history and fallback ambient owners. Native
flags0 final-request binding now tracks ambient4 and separate RGB/alpha vertex
sources; per-light SDK requests, presets and the greater-than8 light sorter stay
provider boundaries. The new map count/fog/fallback bytes are original resource
data, not a replacement assumption that all scene lighting is disabled.

## Exact evidence and limits

| Complete original body / span(hex) | SHA256 of original bytes |
| --- | --- |
| GXSetChanCtrl802b6f78/b0 | 83190841ab3f1c3a70ec6c924462b803c5861bc1361a71281a3b4f3fc290935c |
| GXSetChanAmbColor802b6d6c/e8 | ee0d1a8673e88737cf53ed390b3377205098b739f6001877a05b0124d3ee8fde |
| GXSetChanMatColor802b6e54/e8 | dc7d4cca70e0c4087eed298e0f59ec56c86f9ddc575086527291984d3eb6b27b |
| GXSetNumChans802b6f3c/3c | 3b4fd527d07e6e55f7fe8e8409cec203f4aefad641f81a3e27e604095d9bbd3a |

Corrected private disassembly/ELF evidence resides under
`artifacts/battle-display-queue-evidence/`, named after each function. The
combined `GXChannelColorSetters.dis` spans802b6d6c/1d0 with SHA256
9ada459eea8c930309ec02da5d719108adb0dad5d5a28a8f6413dd4d4e3a8256.
Original FIFO execution, full GX lighting/TEV raster numerics, PICA image
comparison, live enabled-light owners and full prologue parity remain open.
