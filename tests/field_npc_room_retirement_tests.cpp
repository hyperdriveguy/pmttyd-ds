#include "field_area_event.hpp"
#include "event_native_runtime.hpp"
#include "game.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70,Map=0x8100010c;
FieldAreaGpState state(uint32_t field=0){FieldAreaGpState s{};std::memcpy(s.bytes.data()+0x10c-s.Begin,"gor_02",7);std::memcpy(s.bytes.data()+0x12c-s.Begin,"gor",4);std::memcpy(s.bytes.data()+0x11c-s.Begin,"w_bero",7);for(unsigned n=0;n<4;++n)s.bytes[0x168-s.Begin+n]=uint8_t(field>>(24-8*n));return s;}
struct Release {
 unsigned admissions=0,limit=0;std::vector<uint32_t> calls;FieldAreaEvent* area=nullptr;
 static bool admit(void* raw,EventAddressSpace&){auto& r=*static_cast<Release*>(raw);++r.admissions;if(r.area)assert(!r.area->retireMapNpcs(r.services()));return true;}
 static bool step(void* raw,uint32_t function,EventAddressSpace& memory){auto& r=*static_cast<Release*>(raw);if(r.area)assert(!r.area->retireMapNpcs(r.services()));uint32_t flags=1,count=1,field=0;assert(memory.readWord(Base,flags) && flags==0 && memory.readWord(0x8041e8b4,count) && count==0 && memory.readWord(0x81000168,field));uint32_t mask=0;assert(memory.readWord(field+0x5c,mask) && mask==0xdeadbeef);r.calls.push_back(function);return r.calls.size()<=r.limit;}
 FieldAreaNpcReleaseServices services(){return {this,admit,step};}
};
void retainedRetirement(){
 assert(loadFieldAreaEventAssets("romfs"));FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin"));auto game=std::make_unique<Game>();FieldAreaEvent area;auto image=std::make_unique<FieldAreaGpState>(state());assert(area.begin(2,0) && area.prepareMapNpcs(*game,*image,tags,Inputs,Area,Map,Base));
 for(const char* name:{"c_deale","c_kuri_h","c_joho_b","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_f_mura","c_luigi"}){const auto base=std::string("romfs/field-native-")+name;assert(game->originalFieldNpcs->poses().load((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str()));}
 assert(area.advanceMapNpcs({123,234}));const auto root=area.appendAreaInit(*image);assert(root);
 const auto field=game->originalFieldNpcs->fieldWorkAddress();auto inspect=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});auto binding=FieldNpcRegistryBinding::bind(game->originalFieldNpcs,inspect->memory);assert(binding && inspect->memory.writeWord(field+0x5c,0xdeadbeef));
 // Observe a real blocked root and child before teardown. No source child
 // completion/copyback or normal retirement hook is fabricated by ReInit.
 struct Trace {FieldAreaEvent* area;Release* release;unsigned calls=0;EventContext* context=nullptr;EventRuntimeGlobals* globals=nullptr;};Release release;release.area=&area;Trace trace{&area,&release};FieldAreaEventServices s;s.user=&trace;
 s.call=[](void* raw,FieldAreaNativeCall& call)->int32_t{auto& t=*static_cast<Trace*>(raw);++t.calls;t.context=call.creation.manager.context(call.creation.manager.user,false);t.globals=call.creation.runtime(call.creation.user);t.context->work[7]=0x12345678;t.context->flags[1]=0xabcdef01;t.context->field94=0x98765432;t.globals->nextId=0x1234;assert(!t.area->retireMapNpcs(t.release->services()));return 0;};area.tick(17,s);assert(trace.calls && release.admissions==0 && area.eventAlive(*root));
 assert(!area.retireMapNpcs({}) && binding->activeActors()->size()==15);
 assert(!area.retireMapNpcs(release.services()) && release.admissions==1 && binding->activeActors()->empty() && area.eventAlive(*root));assert(area.pendingFunction==0x800d6b7c && area.pendingEventId==0);
 const auto priorCalls=trace.calls;area.tick(34,s);assert(trace.calls==priorCalls && !area.advanceMapNpcs({}) && !area.mapNpcsReady() && !area.appendAreaInit(*image) && !area.appendDoorEntry(0,{}));
 // Admission and reset occur once. Changing provider identity cannot resume.
 Release replacement;assert(!area.retireMapNpcs(replacement.services()) && replacement.admissions==0 && replacement.calls.empty());
 release.limit=3;assert(!area.retireMapNpcs(release.services()) && release.admissions==1 && area.pendingFunction==0x80073394);
 release.limit=100;assert(area.retireMapNpcs(release.services()) && !area.started && !area.eventAlive(*root) && area.liveEventCount()==0 && area.cases() && area.completed().empty());
 assert((release.calls==std::vector<uint32_t>{0x800d6b7c,0x800d6b7c,0x800612f0,0x80073394,0x80073394,0x80071828,0x8005ecd0,0x8003b028}));
 assert(trace.context->work[7]==0x12345678 && trace.context->flags[1]==0xabcdef01 && trace.context->field94==0x98765432 && trace.globals->nextId==0x1234 && trace.globals->allocationCount==0);
 for(unsigned n=0;n<FieldCasePool::Capacity;++n)assert(!area.cases()->active(n));
 assert(!area.appendAreaInit(*image) && !area.appendDoorEntry(0,{}));
 // The provider above is a sequencing fixture, not actual pose auto-release.
 // Source reset and native facade retirement do not secretly release its poses.
 assert(game->originalFieldNpcs->poses().poses().pose(0));uint32_t mask=0;assert(inspect->memory.readWord(field+0x5c,mask) && mask==0xdeadbeef);
 // New facade uses the retained original owner/F and actual history. No boot
 // initialization or replacement allocation can erase those bytes.
 *image=state(field);assert(!area.begin(999,50));assert(area.begin(2,50) && area.prepareMapNpcs(*game,*image,tags,Inputs,Area,Map,Base) && binding->activeActors()->empty());assert(inspect->memory.readWord(field+0x5c,mask) && mask==0xdeadbeef);
 assert(area.advanceMapNpcs({456,567}));const auto nextRoot=area.appendAreaInit(*image);assert(nextRoot && *nextRoot==0x1234 && trace.context->work[7]==0x12345678 && trace.context->flags[1]==0xabcdef01 && trace.context->field94==0x98765432);
}
}
int main(){retainedRetirement();}
