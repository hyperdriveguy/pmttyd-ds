#include "field_item_get.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
using namespace ttyd;
struct Harness {
 std::vector<std::string> calls;
 bool ready=true,silent=false,vacant=false,received=true;
 int action=1,grants=0,deleted=0;
 ItemGetPoint effectPosition{};
 ProloguePouch pouch;
};
FieldItemGetServices services(Harness& h) {
 FieldItemGetServices s;s.user=&h;
 s.ready=[](void* p){return static_cast<Harness*>(p)->ready;};
 s.mario=[](void*){return ItemGetPoint{3,20,-7};};
 s.request=[](void* p,ItemGetRequest r){static_cast<Harness*>(p)->calls.push_back("r"+std::to_string(int(r)));};
 s.effect=[](void* p,int32_t type,ItemGetPoint xyz){auto& h=*static_cast<Harness*>(p);assert(type==6);h.effectPosition=xyz;h.calls.push_back("effect6");return uintptr_t(0x1234);};
 s.window=[](void* p,uint32_t definition,const char* name){auto& h=*static_cast<Harness*>(p);assert(std::string(name)=="irai_item");h.calls.push_back("w"+std::to_string(definition));return definition==0x80313dc8?20:definition==0x80313df0?21:22;};
 s.windowAction=[](void* p,int32_t w){assert(w==20);auto& h=*static_cast<Harness*>(p);h.calls.push_back("action");return h.action;};
 s.bgmSilent=[](void* p){auto& h=*static_cast<Harness*>(p);h.calls.push_back("silent");return h.silent;};
 s.bgmCheck=[](void* p,int32_t category){assert(category==1);auto& h=*static_cast<Harness*>(p);h.calls.push_back("vacant");return h.vacant?int32_t(-1):int32_t(0);};
 s.receive=[](void* p,int32_t item){auto& h=*static_cast<Harness*>(p);assert(item==95);++h.grants;h.calls.push_back("grant95");return h.received?h.pouch.insertKeyItem(item):0;};
 s.removeNamedEntries=[](void* p,const char* name){auto& h=*static_cast<Harness*>(p);assert(std::string(name)=="irai_item");++h.deleted;h.calls.push_back("delete-all-matching");};
 return s;
}
std::string request(ItemGetRequest r){return "r"+std::to_string(int(r));}
size_t at(const Harness& h,const std::string& s){for(size_t i=0;i<h.calls.size();++i)if(h.calls[i]==s)return i;assert(false);return 0;}
int main() {
 for(bool silent:{false,true})for(bool vacant:{false,true}) {
  Harness h;h.silent=silent;h.vacant=vacant;FieldItem95Get get;auto s=services(h);
  h.pouch.half(0xa0,107);h.pouch.half(0xa4,9);h.pouch.half(0x70,3);h.pouch.half(0x74,1);
  assert(get.begin("irai_item",s) && get.flags()==0x1003 && !get.begin("irai_item",s));assert(h.grants==0);
  assert(get.tick(0x300)==ItemGetStep::Running && get.substate()==2 && h.grants==1 && h.deleted==0);
  assert(get.itemPosition().y==70 && h.effectPosition.y==82 && get.effectWork()==0x1234);
  assert(get.mainWindow()==20 && get.secondaryWindow()==21 && (get.status()&1));
  assert(h.pouch.half(0xa0)==95 && h.pouch.half(0xa2)==107 && h.pouch.half(0xa4)==0 && h.pouch.half(0xa6)==9);
  assert(h.pouch.half(0x70)==3 && h.pouch.half(0x74)==1);
  std::vector<std::string> expected={request(ItemGetRequest::Camera8FlagOn),request(ItemGetRequest::KeyOff),request(ItemGetRequest::SystemLevel2),request(ItemGetRequest::SoundFlag80On),request(ItemGetRequest::ClearMarioMotion),request(ItemGetRequest::GetItemMotion),"effect6",request(ItemGetRequest::EffectPayload5),request(ItemGetRequest::IconFlag100On),"silent"};
  if(!silent)expected.push_back(request(ItemGetRequest::BgmOff3800));
  expected.insert(expected.end(),{request(ItemGetRequest::KeyItemFanfare),"w"+std::to_string(0x80313dc8u),"w"+std::to_string(0x80313df0u),"grant95",request(ItemGetRequest::IconFlag80On)});
  assert(h.calls==expected);
  assert(at(h,request(ItemGetRequest::KeyItemFanfare))<at(h,"grant95"));
  assert(at(h,request(ItemGetRequest::KeyOff))<at(h,"effect6"));
  assert(at(h,"w"+std::to_string(0x80313dc8u))<at(h,"grant95") && at(h,"w"+std::to_string(0x80313df0u))<at(h,"grant95"));
  assert(get.tick(0x100)==ItemGetStep::Running && get.substate()==3 && (get.status()&0x01000000));
  assert(get.tick(0x300)==ItemGetStep::Running && h.grants==1 && h.deleted==0); // Window is still moving.
  h.action=0;assert(get.tick(0)==ItemGetStep::Running);assert(get.tick(0x400)==ItemGetStep::Running);
  assert(get.tick(0x200)==ItemGetStep::Complete && h.deleted==1 && h.grants==1 && !(get.flags()&1) && !(get.status()&0x01000000));
  assert(at(h,request(ItemGetRequest::WindowCloseMain))<at(h,request(ItemGetRequest::ControlOn)));
  assert(at(h,request(ItemGetRequest::Camera8FlagOff))<at(h,"delete-all-matching"));
  assert(std::count(h.calls.begin(),h.calls.end(),request(ItemGetRequest::BgmOff201))==1);
  assert(std::count(h.calls.begin(),h.calls.end(),request(ItemGetRequest::BgmResumeA0))==(silent?0:1));
  if(vacant)assert(at(h,request(ItemGetRequest::BgmOff201))<at(h,request(ItemGetRequest::WindowCloseMain)));
  else assert(at(h,request(ItemGetRequest::WindowCloseSecondary))<at(h,request(ItemGetRequest::BgmOff201)));
  const auto count=h.calls.size();assert(get.tick(0x300)==ItemGetStep::Complete && h.calls.size()==count);
 }
 // Assigned fanfare group returns0; completion/vacancy changes query to-1.
 Harness music;music.action=0;FieldItem95Get musicGet;assert(musicGet.begin("irai_item",services(music)));
 musicGet.tick(0);musicGet.tick(0);assert(!(musicGet.flags()&0x2000));
 assert(std::count(music.calls.begin(),music.calls.end(),request(ItemGetRequest::BgmOff201))==0);
 music.vacant=true;assert(musicGet.tick(0)==ItemGetStep::Running && (musicGet.flags()&0x2000));
 const auto checks=std::count(music.calls.begin(),music.calls.end(),"vacant");musicGet.tick(0);
 assert(std::count(music.calls.begin(),music.calls.end(),"vacant")==checks);
 assert(musicGet.tick(0x100)==ItemGetStep::Complete);
 assert(std::count(music.calls.begin(),music.calls.end(),request(ItemGetRequest::BgmOff201))==1);
 assert(std::count(music.calls.begin(),music.calls.end(),request(ItemGetRequest::BgmResumeA0))==1);
 Harness failed;failed.received=false;failed.action=0;FieldItem95Get get;
 assert(get.begin("irai_item",services(failed)));assert(get.tick(0)==ItemGetStep::Running && !(get.status()&1));
 assert(at(failed,"grant95")<at(failed,"w"+std::to_string(0x80313e18u)));
 assert(get.tick(0x100)==ItemGetStep::DiscardPending && get.substate()==5 && failed.deleted==0 && (get.status()&0x01000000));
 assert(at(failed,request(ItemGetRequest::WindowCloseFailure))<at(failed,request(ItemGetRequest::StayMotion)));
 assert(at(failed,request(ItemGetRequest::StayMotion))<at(failed,request(ItemGetRequest::IconFlag2On)));
 assert(get.tick(0x300)==ItemGetStep::DiscardPending && failed.grants==1 && !get.begin("irai_item",services(failed)));
 Harness selected;selected.received=false;selected.action=0;auto selection=services(selected);
 selection.discardSelection=[](void* p,int32_t kind,int32_t item,int32_t mode){assert(kind==1 && item==95 && mode==1);static_cast<Harness*>(p)->calls.push_back("discard-selection");return uintptr_t(77);};
 FieldItem95Get discard;assert(discard.begin("irai_item",selection));discard.tick(0);discard.tick(0x100);
 assert(discard.state()==ItemGetStep::DiscardPending && discard.discardSelection()==77 && selected.calls.back()=="discard-selection");
 Harness absent;absent.ready=false;FieldItem95Get unopened;assert(!unopened.begin("irai_item",services(absent)) && absent.calls.empty());
 auto missing=services(absent);missing.receive=nullptr;assert(!unopened.begin("irai_item",missing));
 ProloguePouch full;for(unsigned i=0;i<121;++i)full.half(0xa0+2*i,int16_t(i+1));
 const auto before=full.bytes;assert(full.insertKeyItem(95)==0 && full.bytes==before);
 full.half(0xa0,0);assert(full.insertKeyItem(95)==1 && full.half(0xa0)==95 && full.half(0xa2)==2);
 full.half(0xa0,0);full.half(0xa0+120*2,0);assert(full.insertKeyItem(95)==1 && full.half(0xa0)==95 && full.half(0xa2)==0);
}
