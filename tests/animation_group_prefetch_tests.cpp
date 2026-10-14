#include "animation_group_prefetch.hpp"
#include "field_area_event.hpp"
#include "field_map_flags.hpp"
#include "field_bero_info.hpp"
#include "field_bero_exec.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
using namespace ttyd;
struct Trace {
 std::vector<uint8_t> table=std::vector<uint8_t>(96);std::vector<std::string> paths;int32_t group=0,pixels=0;unsigned archives=0;
 AnimationGroupAsyncServices services(){AnimationGroupAsyncServices s;s.user=this;s.archive=[](void* p,uint32_t a){assert(a==1);++static_cast<Trace*>(p)->archives;};
 s.request=[](void* p,uint32_t t,uint32_t f,const std::string& path){auto& r=*static_cast<Trace*>(p);assert(f==0);r.paths.push_back(path);if(t==5){std::memcpy(r.table.data()+32,"changed",8);return r.group;}assert(t==4);return r.pixels;};return s;}
};
int main(){
 // Real ordering: group request can mutate the table before alias lookup.
 for(int32_t g:{-1,0,7})for(int32_t p:{-1,0,9}){Trace t;t.group=g;t.pixels=p;std::memcpy(t.table.data(),"group",6);const bool result=animationGroupBaseAsync("group",t.table,t.services());assert(result==(g==-1 || (g!=0 && p!=0)));assert(t.archives==1 && t.paths==std::vector<std::string>({"a/group","a/changed-"}));}
 {Trace t;t.group=7;t.pixels=9;assert(animationGroupBaseAsync("absent",t.table,t.services()));assert(t.paths[1]=="a/-");}
 struct Phase {std::vector<std::string> names;bool ready=false;} phase;
 AnimationGroupPollServices ps;ps.user=&phase;ps.poll=[](void* p,const char* n){auto& t=*static_cast<Phase*>(p);t.names.emplace_back(n);return t.ready;};
 EventEntry missingProvider;missingProvider.waitCount=100;assert(fieldBeroReadMarioPera(missingProvider,true,{})==0 && missingProvider.waitCount==0);
 assert(fieldBeroReadMarioPera(missingProvider,false,ps)==0 && missingProvider.waitCount==0);
 phase.names.clear();
 EventEntry e;e.waitCount=77;e.locals[0]=123;e.locals[15]=456;
 assert(fieldBeroReadMarioPera(e,true,ps)==0 && e.waitCount==0);phase.ready=true;
 assert(fieldBeroReadMarioPera(e,false,ps)==0 && e.waitCount==1 && phase.names.size()==2);
 assert(fieldBeroReadMarioPera(e,false,ps)==2 && e.waitCount==100 && phase.names.back()=="p_dokan_y");
 assert(fieldBeroReadMarioPera(e,false,ps)==2 && phase.names.size()==3 && e.locals[0]==123 && e.locals[15]==456);
 e.waitCount=77;assert(fieldBeroReadMarioPera(e,false,ps)==0 && phase.names.size()==3);
 // Real original files, shared texture cache and bounded first-open/poll behavior.
 AnimationGroupPrefetch cache;assert(!cache.open("romfs/field-prefetch",0));assert(cache.open("romfs/field-prefetch",4096));assert(!cache.open("romfs/field-prefetch"));
 assert(!cache.poll("p_dokan_x") && cache.requestCount()==2 && !cache.resident("a/p_dokan_x"));
 unsigned polls=0;while(!cache.poll("p_dokan_x")){assert(++polls<20);}assert(polls>=9 && !cache.failed());
 const auto* shared=cache.resident("a/p_slit-");assert(shared && shared->size()==22592);assert(cache.resident("a/p_dokan_x")->size()==41756);
 assert(!cache.poll("p_dokan_y") && cache.requestCount()==3);polls=0;while(!cache.poll("p_dokan_y"))assert(++polls<10);
 assert(cache.resident("a/p_slit-")==shared && cache.resident("a/p_dokan_y")->size()==18576 && !cache.failed());
 for(const auto* name:{"a/p_dokan_x","a/p_dokan_y","a/p_slit-"}){std::ifstream file(std::string("romfs/field-prefetch/")+name,std::ios::binary);std::vector<uint8_t> original((std::istreambuf_iterator<char>(file)),{});assert(original==*cache.resident(name));}
 AnimationGroupPrefetch missing;assert(missing.open("romfs/field-prefetch"));for(unsigned n=0;n<3;++n)assert(!missing.poll("missing"));assert(missing.failed() && !missing.resident("a/missing"));
 // Genuine parent/child now finishes prefetch and exact system-flag publication.
 assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 FieldAreaEvent area;assert(area.begin(2,0));FieldAreaGpState state{};state.systemFlags=0x40000000;
 std::memcpy(state.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);state.bytes[0x176-FieldAreaGpState::Begin]=1;state.bytes[0x177-FieldAreaGpState::Begin]=146;assert(area.appendAreaInit(state));
 FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);FieldBeroExec exec;
 struct Capture {uint32_t fn=0;std::array<int32_t,16> bank{};} capture;
 FieldAreaEventServices s;s.prefetch=&cache;s.map=&map;s.beroInfo=&info;s.beroExec=&exec;s.user=&capture;
 s.call=[](void* p,FieldAreaNativeCall& r){auto& c=*static_cast<Capture*>(p);c.fn=r.function;c.bank=r.event.locals;return 0;};
 for(unsigned tick=1;tick<12 && !capture.fn;++tick)area.tick(tick*17,s);
 assert(!area.failed && capture.fn==0x800e7474 && exec.read()==1 && capture.bank[0]==0 && area.liveEventCount()==2);
}
