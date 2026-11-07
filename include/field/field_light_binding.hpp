#pragma once
#include "event_manager.hpp"
#include <optional>

namespace ttyd {
namespace FieldLightSource {
constexpr uint32_t FindLight = 0x8001bcf8;
constexpr uint32_t FindMapObject = 0x8001f878;
constexpr uint32_t MatrixVector = 0x8029e6a4;
constexpr uint32_t VectorSubtract = 0x8029eba4;
constexpr uint32_t TranslationMatrix = 0x8029e230;
constexpr uint32_t VectorAdd = 0x8029eb80;
constexpr uint32_t InitialOrigin = 0x802f3748;
constexpr uint32_t CurrentOrigin = 0x802f3754;
constexpr uint32_t MapBaseMatrix = 0x1c;
constexpr uint32_t MapDynamicMatrix = 0xac;
constexpr uint32_t MapTranslationX = 0xb8;
constexpr uint32_t MatrixRowBytes = 0x10;
constexpr uint32_t LightReferencePosition = 0x3c;
constexpr uint32_t LightCurrentPosition = 0x24;
constexpr uint32_t LocalMatrix = 0;
constexpr uint32_t DynamicMode = 0;
constexpr uint32_t TranslationMode = 1;
}

// SDK children own any partial memory effects across a false result. A zero
// matrix token selects localMatrix; source/destination identify guest storage.
struct FieldLightSdkRequest {
    uint32_t function=0, matrix=0, source=0, destination=0;
    std::array<uint32_t,3> vector{}, other{}, output{};
    std::array<uint32_t,12> localMatrix{}, outputMatrix{};
};
struct FieldLightBindServices {
    void* user=nullptr;
    bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
    bool (*value)(void*,EventEntry&,int32_t,int32_t&)=nullptr;
    bool (*lookup)(void*,uint32_t function,uint32_t name,uint32_t&)=nullptr;
    bool (*sdk)(void*,FieldLightSdkRequest&)=nullptr;
};
enum class FieldLightBindPhase : uint8_t {
    Idle, ReadLightArgument, ResolveLightArgument, ReadMapArgument,
    ResolveMapArgument, ReadModeArgument, ResolveModeArgument, FindLight,
    FindMap, ReadInitialOrigin, TransformInitialOrigin, SubtractReference,
    StoreScratch, ReadScratch, ReadTranslation, BuildTranslation,
    TransformOffset, ReadCurrentOrigin, TransformCurrentOrigin, PublishPosition
};
struct FieldLightBindCursor {
    FieldLightBindServices owner;
    EventEntry* event=nullptr;
    uint32_t id=0, args=0, raw=0, index=0;
    uint32_t lightName=0, mapName=0, mode=0, light=0, map=0;
    FieldLightBindPhase phase=FieldLightBindPhase::Idle;
    bool first=false, queued=false;
    std::array<uint32_t,3> vector{}, origin{}, translation{};
    std::array<uint32_t,12> matrix{};
    FieldLightSdkRequest request;
    bool active() const {return phase!=FieldLightBindPhase::Idle;}
};
// Complete original evt_light_bind_mapobj80136158. Engaged0 ends one source
// poll; empty retains a native child, including the original nonzero first flag.
// Every completed poll re-resolves names/mode; E78..80 holds persistent raw XYZ.
// The context/event identity stays fixed. Missing callbacks may be installed
// once; an installed callback cannot change while this invocation is retained.
std::optional<int32_t> fieldLightBindEvent(EventEntry&,int32_t first,
    const FieldLightBindServices&,FieldLightBindCursor&);
}
