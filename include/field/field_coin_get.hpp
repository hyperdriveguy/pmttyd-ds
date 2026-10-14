#pragma once
#include "field_item_get.hpp"
#include <array>
#include <optional>
namespace ttyd {
struct FieldCoinClock {uint32_t high=0,low=0,hardwareFrequency=0;};
enum class FieldCoinRequest {GetMotion,BgmOff3800,FanfareOn201,StayMotion,BgmResumeA0,BgmOff201};
struct FieldCoinGetServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 ItemGetPoint (*mario)(void*)=nullptr;
 float (*heading)(void*)=nullptr;
 FieldCoinClock (*clock)(void*)=nullptr;
 uintptr_t (*effect)(void*,int32_t,ItemGetPoint)=nullptr;
 void (*effectPayload)(void*,uintptr_t,int32_t)=nullptr;
 void (*softDelete)(void*,uintptr_t)=nullptr;
 void (*iconEnter)(void*,const char*,uint32_t,ItemGetPoint)=nullptr;
 void (*iconFlags)(void*,const char*,uint16_t on,uint16_t off)=nullptr;
 void (*iconDelete)(void*,const char*)=nullptr;
 void (*request)(void*,FieldCoinRequest)=nullptr;
 int32_t (*window)(void*,uint32_t,const int32_t*)=nullptr;
 void (*windowClose)(void*,int32_t)=nullptr;
 int32_t (*windowAction)(void*,int32_t)=nullptr;
 int32_t (*addCoins)(void*,int32_t)=nullptr;
};
struct FieldCoinPlacement {ItemGetPoint effect,icon;};
FieldCoinPlacement fieldCoinPlacement(ItemGetPoint mario,float cameraHeading);
// Finite source callback. Stable in-owner parameter storage replaces its map
// allocation; return2 at100 does not retire the window's borrowed parameter.
class FieldCoinGet {
public:
 bool bind(const FieldCoinGetServices&);
 std::optional<int32_t> invoke(int32_t resolvedAmount,bool firstCall,uint32_t triggers);
 bool retained() const{return retained_;}
 bool canAdvance() const{return owner_==this;}
 int32_t state() const{return state_;}
 const std::array<int32_t,2>& parameter() const{return parameter_;}
private:
 const FieldCoinGet* owner_=nullptr;
 FieldCoinGetServices services_;
 std::array<int32_t,2> parameter_{};
 FieldCoinClock saved_;
 uintptr_t effect_=0;
 int32_t state_=0,window_=-1;
 bool retained_=false;
};
}
