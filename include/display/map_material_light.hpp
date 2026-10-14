#pragma once
#include "shadow_display.hpp"
namespace ttyd {
struct MapLightRecord {
    std::array<uint8_t,96> bytes{};
    uint16_t flags() const {return uint16_t(bytes[0])<<8|bytes[1];}
    uint32_t color() const {return uint32_t(bytes[0x48])<<24|uint32_t(bytes[0x49])<<16|uint32_t(bytes[0x4a])<<8|bytes[0x4b];}
};
struct MapLightBank {bool known=false;int32_t count=32;std::array<MapLightRecord,32> records{};};
struct MapMaterialFog {uint32_t enabled=0,type=0,rgba=0;float start=0,end=0;};
struct MapMaterialLightWork {
    std::array<MapLightBank,2> banks{};uint32_t selector=0;
    uint32_t paperAmbient=0,craftAmbient=0;bool battleLoaded=false;
    std::array<MapMaterialFog,2> battleFog{};
    // Scoped source lightInit:32 allocated/zeroed records per bank. Field map
    // entries are not bound here; their bank remains explicitly unavailable.
    MapMaterialLightWork(){banks[1].known=true;}
    bool loadBattle(const char*);
    void resetSelected(); // Source lightReInit: preserve count, clear records.
};
struct MapLightQuery {
    uint32_t function=0,mask=0;int32_t maximum=0;std::array<float,3> origin{};
    std::array<const MapLightRecord*,8> output{};int32_t count=0;
    const MapLightRecord* record=nullptr;bool found=false;
};
// Exact bank scan/order/filter for <=8 active records. A larger census holds
// at source sort dependency; presets require their separate live producer.
bool mapLightBankQuery(const MapMaterialLightWork&,MapLightQuery&,uint32_t& pending);
struct MapMaterialLightServices {
    void* user=nullptr;
    bool (*query)(void*,MapLightQuery&,uint32_t& pending)=nullptr;
    bool (*light)(void*,unsigned slot,const MapLightRecord*,uint32_t& attenuation,uint32_t& pending)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
};
struct MapMaterialLightCursor {
    bool active=false,queued=false;uint32_t phase=0,pending=0,flags=0,step=0,mask=0,attenuation=2,ambient=0x000000ff;
    std::array<float,3> origin{};std::array<const MapLightRecord*,8> selected{};int32_t count=0,index=0;
    MapLightQuery query;ShadowGXRequest request;
};
bool resumeMapMaterialLight(const MapMaterialLightWork&,uint32_t flags,std::array<float,3>,const MapMaterialLightServices&,MapMaterialLightCursor&);
}
