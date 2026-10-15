#include "animation_group_request.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 std::map<uint32_t,uint32_t> words{{0x80415dc0,0x1000},{0x1100,0x2000},{0x20a0,0x3000},{0x3004,128},{0x3000,0x4000}};
 std::map<uint32_t,std::string> texts{{0x4000,"other"},{0x4020,"model"},{0x4040,"texture"},{0x4060,"model"},{0x4080,"later"},{0x5000,"model"},{0x8041fb30,""}};
 std::vector<uint32_t> archive,types,names,completions;uint32_t model=7,pixels=9;unsigned modelCalls=0,textureCalls=0,compares=0;bool holdModel=false,holdTexture=false,holdCompare=false;
 AnimationGroupRequestServices services(){
  AnimationGroupRequestServices s;s.user=this;
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);auto i=f.words.find(a);return i==f.words.end()?std::nullopt:std::optional<uint32_t>(i->second);};
  s.archive=[](void* p,uint32_t mode){static_cast<Fixture*>(p)->archive.push_back(mode);return true;};
  s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(f.holdCompare)return {};++f.compares;return f.texts.at(a)==f.texts.at(b)?0:1;};
  s.request=[](void* p,uint32_t type,uint32_t c,uint32_t format,uint32_t prefix,uint32_t name)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(prefix==0x8041fb3c);if(type==5){assert(format==0x8041fb34 && name==0x5000);if(f.holdModel)return {};++f.modelCalls;}else{assert(type==4 && format==0x802c1068 && !c);if(f.holdTexture)return {};++f.textureCalls;}f.types.push_back(type);f.names.push_back(name);f.completions.push_back(c);return type==5?f.model:f.pixels;};return s;
 }
};
void combinations(){
 for(uint32_t mode:{0u,1u,2u,3u,0xffffffffu})for(uint32_t model:{0u,7u,0xffffffffu})for(uint32_t texture:{0u,9u,0xffffffffu}){
  Fixture f;f.model=model;f.pixels=texture;auto s=f.services();AnimationGroupRequestCursor c;uint32_t v=99;assert(requestAnimationGroup(0x5000,mode,0x123,s,c,v));assert(!c.phase && v==(model==0xffffffff?1:model==0?0:texture!=0));assert(f.types==std::vector<uint32_t>({5,4}) && f.names==std::vector<uint32_t>({0x5000,0x4040}) && f.completions==std::vector<uint32_t>({0x123,0}));assert(f.compares==2);assert(f.archive==(mode<3?std::vector<uint32_t>{mode==0?1u:mode==1?2u:0u}:std::vector<uint32_t>{}));
 }
 for(uint32_t count:{0u,0xffffffffu}){Fixture f;f.words[0x3004]=count;auto s=f.services();AnimationGroupRequestCursor c;uint32_t v=99;assert(requestAnimationGroup(0x5000,0,0,s,c,v) && v==1 && f.names.back()==0x8041fb30 && !f.compares);}
 {Fixture f;f.texts[0x5000]="absent";auto s=f.services();AnimationGroupRequestCursor c;uint32_t v=99;assert(requestAnimationGroup(0x5000,0,0,s,c,v) && f.compares==4 && f.names.back()==0x8041fb30);}
}
void holds(){
 for(unsigned hold=0;hold<3;++hold){Fixture f;f.holdModel=hold==0;f.holdCompare=hold==1;f.holdTexture=hold==2;auto s=f.services();AnimationGroupRequestCursor c;uint32_t result=99;assert(!requestAnimationGroup(0x5000,1,0,s,c,result));const auto phase=c.phase;const auto compares=f.compares;assert(!requestAnimationGroup(0x5000,0,0,s,c,result) && c.phase==phase && result==99);assert(!requestAnimationGroup(0x5000,1,0,s,c,result) && f.compares==compares && f.archive.size()==1);f.holdModel=f.holdTexture=f.holdCompare=false;assert(requestAnimationGroup(0x5000,1,0,s,c,result) && result==1 && f.modelCalls==1 && f.textureCalls==1 && f.archive.size()==1);}
 Fixture f;f.model=0;auto s=f.services();AnimationGroupRequestCursor c;uint32_t result=99;assert(requestAnimationGroup(0x5000,0,0,s,c,result) && result==0 && !c.phase);f.model=7;assert(requestAnimationGroup(0x5000,2,0,s,c,result) && result==1 && f.modelCalls==2 && f.archive==std::vector<uint32_t>({1,0}));
}
}
int main(){combinations();holds();}
