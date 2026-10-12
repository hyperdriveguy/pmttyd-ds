# Saved-switch storage leaves used by event integer namespaces

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete swByteGet8008582c/24,swByteSet80085850/24,
_swByteGet80085744/10,_swByteSet80085754/10;
swGet800858b4/48,swSet800858fc/40,swClear80085874/40;
_swGet800857a4/48,_swSet800857ec/40,_swClear80085764/40.
Read [integer](EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[float](EVENT_FLOAT_OPERAND_ROUTING.md) API routing.
Verify: `python3 tools/verify_event_operand_namespaces.py`.
These RAM leaves do not establish on-disk save commits or script lifecycle.

Each call reloads GP through global pointer80415fd0. No context selection,
index/null/bounds check,dirty-bit update,disk write,event callback or scalar
setter return contract occurs in these bodies. All index/address additions wrap32.

## Byte families and the GSW index0 exception

swByteGet(index0) returns WORD(GP+174),not a byte. swByteSet(index0,value)
stores the complete value WORD at GP+174. Every nonzero index (including negative)
uses byte at GP+578+index:reader zero-extends U8,writer stores low8(value).
Thus GSW0 and GSW1 do not simply form one homogeneous byte array;0's storage
is physically separate. Naming this a saved-byte API does not change its word0
exception. Story/initialization consumers require their own caller bindings.

_swByteGet(index) always returns U8(GP+db8+index);_swByteSet always storeslow8
there,including index0. Event SetValue's LSW route explicitly narrows toU8 before
calling this writer;GSW passes its raw word to preserve index0 behavior.

## Flag families

For signed index i compute q=truncSigned(i/32),r=i-32*q as original srawi/addze/
rotate/sign arithmetic. mask=PPC_slw(1,r),which returns0 when low6 shift count
hasbit5 set and otherwise1<<(count&31). For nonnegative index,usual word/bit
quotient/remainder applies. Negative remainder is not normalized to0..31.

| Family | Word address |
| --- | --- |
| swGet/Set/Clear (GSF) | GP+178+4*q |
| _swGet/Set/Clear (LSF) | GP+d78+4*q |

Get reads word&mask,normalizes to integer0/1. Set reads word,ORs mask,stores
word. Clear reads word,ANDs complement mask,stores. Set/Clear do not take a
value operand;evtSetValue chooses one after first reading oldbool via Get.
Each separate service can reload a different GP under a mutating adapter;
there is no API-side pointer identity enforcement. No persistence guarantee
follows from names GSF/LSF or a byte-store instruction.

Acceptance cases:GSW0 full-word versus nonzero low8,LSW0 byte,negative index
arithmetic,word boundaries31/32,flags read bool,set/clear preserve other bits,
zero PPC mask still reads/stores unchanged word,GP reload between old-read and
write. Save/load/disk ownership,region/story source bindings and valid RAM domains
remain separate work. The event interpreter should expose these exact original
service operations rather than substituting native save-map semantics.
