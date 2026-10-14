#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
struct DisplayRecord {uint8_t camera=0,drawClass=0;uint16_t padding=0;float key=0;uint32_t callback=0,userdata=0;};
static_assert(sizeof(DisplayRecord)==16);
enum class DisplayAdmission {Registered,RejectedNaN,Unavailable};
enum class DisplayGXState {ColorUpdate,AlphaUpdate,ZScaleOffset,BlendMode,ZCompareLocation,AlphaCompare,ZMode};
struct DisplayStateRequest {DisplayGXState kind;std::array<int32_t,5> values{};std::array<float,2> floats{};};
struct DisplayDispatchServices {
    void* user=nullptr;
    bool (*state)(void*,const DisplayStateRequest&)=nullptr;
    bool (*callback)(void*,uint32_t camera,uint32_t identity,uint32_t borrowed)=nullptr;
};
struct DisplayDispatchCursor {uint32_t index=0,camera=0;uint8_t stage=0;bool active=false,invoking=false;uint32_t callback=0,userdata=0;};
// Native record references adapt source pointers; userdata remains borrowed.
class DisplayQueue {
public:
    DisplayAdmission enter(uint32_t camera,int32_t drawClass,uint32_t callback,uint32_t userdata,float priority);
    void reinitialize(){count_=0;}
    void sort();
    bool dispatch(uint32_t camera,const DisplayDispatchServices&,DisplayDispatchCursor&);
    uint32_t count() const{return count_;}
    const DisplayRecord* record(uint32_t index) const{return index<records_.size()?&records_[index]:nullptr;}
    const DisplayRecord* ordered(uint32_t index) const{return index<count_?record(order_[index]):nullptr;}
    const DisplayRecord* current() const{return current_?record(*current_):nullptr;}
private:
    std::array<DisplayRecord,1024> records_{};
    std::array<uint16_t,1024> order_{};
    uint32_t count_=0;std::optional<uint16_t> current_;
};
}
