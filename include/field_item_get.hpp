#pragma once
#include <cstdint>
#include <string>
namespace ttyd {
struct ItemGetPoint {float x=0,y=0,z=0;};
// Typed requests preserve source service order. Render/control/audio providers
// own the actual original resources; these are not story message substitutes.
enum class ItemGetRequest {
 Camera8FlagOn,KeyOff,SystemLevel2,SoundFlag80On,ClearMarioMotion,
 GetItemMotion,EffectPayload5,IconFlag100On,BgmOff3800,KeyItemFanfare,
 BgmOff201,BgmResumeA0,WindowCloseMain,WindowCloseSecondary,EffectSoftDelete,
 Camera4,IconFlag100Off,IconFlag80On,IconFlag80Off,SystemLevel0,
 SoundFlag80Off,ControlOff,ControlOn,KeyOn,StayMotion,Camera8FlagOff,
 WindowCloseFailure,IconFlag2On
};
struct FieldItemGetServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 ItemGetPoint (*mario)(void*)=nullptr;
 void (*request)(void*,ItemGetRequest)=nullptr;
 uintptr_t (*effect)(void*,int32_t type,ItemGetPoint)=nullptr;
 int32_t (*window)(void*,uint32_t originalDefinition,const char* entryName)=nullptr;
 int32_t (*windowAction)(void*,int32_t mainWindow)=nullptr;
 bool (*bgmSilent)(void*)=nullptr;
 bool (*bgmPlaying)(void*,int32_t)=nullptr;
 int32_t (*receive)(void*,int32_t item)=nullptr; // Low-level pouchGetItem, no revision.
 void (*removeNamedEntries)(void*,const char*)=nullptr;
 uintptr_t (*discardSelection)(void*,int32_t kind,int32_t item,int32_t mode)=nullptr;
};
enum class ItemGetStep {Idle,Running,Complete,DiscardPending};
// Original finite forced item95/mainflags1003 path; itemMain cadence is supplied
// by the caller. Full discard selection/other modes need their own providers.
class FieldItem95Get {
public:
 bool begin(const char* sourceName,const FieldItemGetServices&);
 ItemGetStep tick(uint32_t triggeredButtons);
 ItemGetStep state() const{return state_;}
 uint16_t substate() const{return substate_;}
 uint16_t flags() const{return flags_;}
 uint32_t status() const{return status_;}
 ItemGetPoint itemPosition() const{return position_;}
 uintptr_t effectWork() const{return effect_;}
 int32_t mainWindow() const{return windows_[0];}
 int32_t secondaryWindow() const{return windows_[1];}
 uintptr_t discardSelection() const{return discard_;}
private:
 FieldItemGetServices services_;std::string name_;
 ItemGetStep state_=ItemGetStep::Idle;
 uint16_t substate_=0,flags_=0;
 uint32_t status_=0;
 ItemGetPoint position_{};uintptr_t effect_=0;
 int32_t windows_[3]{-1,-1,-1};
 uintptr_t discard_=0;
 void request(ItemGetRequest r){services_.request(services_.user,r);}
};
}
