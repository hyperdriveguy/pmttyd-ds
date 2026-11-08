#pragma once
#include "field_map_source.hpp"
namespace ttyd {
namespace MapMaterialSource {
constexpr uint32_t Material=0x80020e54,Light=0x8002493c,Ambient=0x8001ba60;
constexpr uint32_t Name=0x8001bcf8,Near=0x8001bad0,ActiveGroup=0x8002b430;
}
struct MapMaterialSourceCall {
 uint32_t function=0;
 std::array<uint32_t,8> arguments{};
 // Raw binary64 f1..f8, including all six attenuation inputs. Keep the
 // unsigned LOD conversion's exact double until the source rounds it.
 std::array<uint64_t,8> reals{};
 std::optional<uint32_t> result;
 std::optional<uint64_t> realResult;
};
struct MapMaterialSourceServices {
 FieldMapSourceServices memory; // Its call member is unused.
 bool (*call)(void*,MapMaterialSourceCall&)=nullptr; // memory.user
};
struct MapMaterialSourceCursor {
 MapMaterialSourceServices owner;
 bool active=false,queued=false,carry=false;
 uint32_t function=0,pc=0,lr=0,ctr=0,step=0,transfer=0,stackTop=0;
 uint32_t visitsConsumed=0;
 std::array<uint32_t,5> arguments{};
 std::array<uint32_t,32> registers{},paired{};
 std::array<uint64_t,32> floats{};
 std::array<bool,32> condition{};
 MapMaterialSourceCall request;
};
// Whole original source bodies in a fixed native graph. Memory callbacks own
// provider/lifetime validation and retained primitive writes. False keeps the
// precise instruction or queued child; only its explicit acknowledgment advances.
// Caller-owned scratch and private zero-seeded ABI registers must stay alive.
// No GX/FIFO/capture success or source map readiness is implied by a request.
std::optional<uint32_t> resumeMapMaterialSource(uint32_t function,
 const std::array<uint32_t,5>& arguments,uint32_t stackTop,
 const MapMaterialSourceServices&,MapMaterialSourceCursor&,unsigned visits=4096);
}
