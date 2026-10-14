#pragma once
#include "trouble_quest_menu.hpp"
#include "message_assets.hpp"
#include "window_font.hpp"
#include <memory>
namespace ttyd {
struct TroubleWindowClip {uint32_t x=0,y=0,width=608,height=480;};
enum class TroubleWindowOp {FogOff,Frame,FontStart,Color,Scale,Edge,Scissor,ScaleVec,MatrixString,String};
struct TroubleWindowRequest {
 TroubleWindowOp op=TroubleWindowOp::FogOff;
 uint32_t color=0;
 float x=0,y=0,width=0,height=0,scaleX=1,scaleY=1,scaleZ=1;
 const char* text=nullptr;
 TroubleWindowClip clip;
 bool edge=false;
};
struct TroubleWindowServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 bool (*emit)(void*,const TroubleWindowRequest&)=nullptr;
 const char* (*header)(void*,unsigned)=nullptr;
 const char* (*client)(void*,int32_t quest)=nullptr;
 const char* (*contents)(void*,int32_t quest)=nullptr;
 uint32_t (*width)(void*,const char*)=nullptr;
 std::optional<bool> (*flag)(void*,int32_t)=nullptr;
 std::optional<int32_t> (*activeQuest)(void*)=nullptr;
 TroubleWindowClip (*scissor)(void*)=nullptr;
};
bool drawTroubleQuestWindow(TroubleQuestMenuWork&,const TroubleWindowServices&);
// Exact original US strcpy caches with retained room resource/header pointers.
// Native font widths are prepared once for the immutable supplied font/spacing2.
class TroubleQuestWindowCache {
public:
 bool prepare(const char* roomResource,const MessageFont&);
 bool ready() const{return bool(messages_);}
 const char* header(unsigned i) const{return headers_.at(i);}
 const char* client(int32_t quest) const{return clients_.at(quest-1).data();}
 const char* contents(int32_t quest) const{return contents_.at(quest-1).data();}
 uint16_t width(const char*) const;
private:
 std::shared_ptr<const MessageResource> messages_;
 std::array<const char*,4> headers_{};
 std::array<std::array<char,21>,30> clients_{};
 std::array<std::array<char,31>,30> contents_{};
 std::array<uint16_t,64> widths_{};
};
}
