#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
struct TroubleQuestMenuWork {
 std::array<int32_t,30> quests{};
 int32_t count=0,state=0,cursor=0,top=0,previous=0,counter94=0;
 float pointerX=0,pointerY=0,scroll=0;
};
TroubleQuestMenuWork originalTroubleQuestList(int32_t story);
struct TroubleQuestMenuInput {uint32_t buttons=0,directionRepeat=0,directionTrigger=0;};
struct TroubleQuestMenuServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 std::optional<int32_t> (*activeQuest)(void*)=nullptr;
 std::optional<bool> (*flag)(void*,int32_t)=nullptr;
 void (*iconEnter)(void*,const char*,int32_t)=nullptr;
 void (*iconFlags)(void*,const char*,uint32_t on,uint32_t off)=nullptr;
 void (*iconScale)(void*,const char*,float)=nullptr;
 void (*iconPosition)(void*,const char*,float,float,float)=nullptr;
 void (*iconDelete)(void*,const char*)=nullptr;
 void (*sound)(void*,int32_t)=nullptr;
 void (*publish)(void*,int32_t questId)=nullptr; // Source event LW0; one-based or-1.
 void (*display)(void*,TroubleQuestMenuWork&)=nullptr; // dispEntry(8,0,...,300).
};
// Source irai_init_func list and irai_main_func retained producer. The original
// window drawing/lecture/confirmation/acceptance caller remain separate owners.
class TroubleQuestMenu {
public:
 bool begin(int32_t story,const TroubleQuestMenuServices&);
 std::optional<int32_t> invoke(TroubleQuestMenuInput);
 const TroubleQuestMenuWork& work() const{return work_;}
 bool canAdvance() const{return owner_==this;}
private:
 const TroubleQuestMenu* owner_=nullptr;
 TroubleQuestMenuWork work_;
 TroubleQuestMenuServices services_;
 void present();
};
}
