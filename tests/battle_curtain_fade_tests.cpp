#include "battle_curtain_fade.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
using namespace ttyd;
struct Provider {bool ready=false;float before=0,after=0;bool advanced=false;std::vector<std::string> calls;uint32_t color=0;};
int main(int argc,char** argv){
 if(argc==2){ActorAnimationPack assets;assert(assets.load(argv[1]));const auto* pose=assets.pose("A_2");assert(pose && pose->start==0 && pose->end==45 && !pose->loop && pose->keys.size()==14 && pose->bindings.size()==648);ActorAnimationSample first,last;assert(pose->evaluate(0,first) && pose->evaluate(45,last));assert(first.vertices.size()==648 && last.vertices.size()==648);bool changed=false;for(size_t i=0;i<648;++i){changed|=first.vertices[i].x!=last.vertices[i].x || first.vertices[i].y!=last.vertices[i].y;}assert(changed);return 0;}
 Provider p;BattleCurtainPoseServices s;s.user=&p;
 s.ready=[](void* u,const char* name,int32_t context){auto& p=*static_cast<Provider*>(u);assert(!std::strcmp(name,"OFF_d_maku_left_right") && context==2);p.calls.push_back("ready");return p.ready;};
 s.entry=[](void* u,const char*,int32_t){static_cast<Provider*>(u)->calls.push_back("entry");return 7;};
 s.release=[](void* u,int32_t id){assert(id==7);static_cast<Provider*>(u)->calls.push_back("release");};
 s.flagsOn=[](void* u,int32_t id,uint32_t flag){assert(id==7);static_cast<Provider*>(u)->calls.push_back(flag==0x1800?"on1800":"on40");};
 s.flagsOff=[](void* u,int32_t id,uint32_t flag){assert(id==7 && flag==0x40);static_cast<Provider*>(u)->calls.push_back("off40");};
 s.animation=[](void* u,int32_t id,const char* pose,int32_t reset){assert(id==7 && reset==1 && !std::strcmp(pose,"A_2"));static_cast<Provider*>(u)->calls.push_back("A2");};
 s.color=[](void* u,int32_t id,uint32_t c){assert(id==7);auto& p=*static_cast<Provider*>(u);p.color=c;p.calls.push_back("color");};
 s.loopTimes=[](void* u,int32_t id){assert(id==7);auto& p=*static_cast<Provider*>(u);p.calls.push_back("loop");return p.advanced?p.after:p.before;};
 s.main=[](void* u,int32_t id){assert(id==7);auto& p=*static_cast<Provider*>(u);p.calls.push_back("main");p.advanced=true;};
 BattleCurtainFade f;assert(f.finished() && !f.enter(22,200,255));assert(f.enter(23,200,255));assert(!f.finished() && f.entryColor==255 && f.rawDuration==200);
 for(int i=0;i<1000;++i)f.tick(s);assert(f.step==0 && !f.finished());p.calls.clear();p.ready=true;f.tick(s);
 assert((p.calls==std::vector<std::string>{"ready","entry","on1800","A2","off40","loop","main","loop"}));assert(!f.finished());
 p.calls.clear();p.before=.5f;p.after=.99f;p.advanced=false;f.tick(s);assert(p.color==0xf9f9f9ff && !f.finished());assert((p.calls==std::vector<std::string>{"loop","color","on40","main","loop"}));
 p.calls.clear();p.before=.99f;p.after=1.f;p.advanced=false;f.tick(s);assert(f.finished() && f.flags==3 && f.step==2 && f.pose==-1 && p.color==0xf4f4f4ff);assert(p.calls.back()=="release");
 p.calls.clear();f.tick(s);assert(p.calls.empty());
 assert(f.enter(23,0,0x12345678));p.ready=false;f.tick(s);assert(!f.finished() && f.step==0);
 p.ready=true;p.before=p.after=0;p.advanced=false;f.tick(s);assert(f.pose==7);assert(f.enter(23,200,255) && f.pose==7);p.calls.clear();p.advanced=false;f.tick(s);assert(p.calls[1]=="release" && p.calls[2]=="entry");
}
