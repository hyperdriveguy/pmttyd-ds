#include "battle_weapon_cost_continuation.hpp"
#include "battle_weapon_cost.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint64_t bits(double v){uint64_t n;std::memcpy(&n,&v,8);return n;}
struct Op{uint32_t kind,address;uint64_t a=0,b=0;bool operator==(const Op&o)const{return kind==o.kind&&address==o.address&&a==o.a&&b==o.b;}};
struct Fixture {
 static constexpr uint32_t Unit=0x1000,Weapon=0x2000;
 std::shared_ptr<const void>owner=std::make_shared<int>(1);PackedWeaponCostCursor cursor;
 std::map<uint32_t,uint32_t>words{{Weapon+8,0},{0x802f00b8,0x43300000},{0x802f00bc,0x80000000},{0x802f00d0,0x40000000},{0x802f00d4,0}};
 std::map<uint32_t,uint8_t>bytes{{Weapon+0x11,0},{Unit+0x2f5,0}};
 std::vector<Op>trace;std::optional<Op>pending;int hold=-1;unsigned fails=0,count=0,powers=0,conversions=0;uint32_t copies=1;bool live=true,reentry=false;
 std::function<void(Fixture&,const Op&)>effect;
 bool gate(Op op){if(!pending){pending=op;trace.push_back(op);if(effect)effect(*this,op);}else assert(*pending==op);if(reentry){assert(!cursor.reset());assert(!run());}if(int(trace.size()-1)==hold&&fails++<2)return false;pending.reset();return true;}
 PackedWeaponCostServices services(){PackedWeaponCostServices s;s.user=this;s.lifetime=owner;s.current=[](void*p){return static_cast<Fixture*>(p)->live;};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate({1,at}))return {};auto i=f.words.find(at);return i==f.words.end()?std::nullopt:std::optional<uint32_t>(i->second);};
  s.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate({2,at}))return {};auto i=f.bytes.find(at);return i==f.bytes.end()?std::nullopt:std::optional<uint8_t>(i->second);};
  s.equippedCount=[](void*p,uint32_t item)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate({3,0x800d3300,item}))return {};++f.count;return f.copies;};
  s.power=[](void*p,double base,double exp)->std::optional<double>{auto&f=*static_cast<Fixture*>(p);if(!f.gate({4,0x80272620,bits(base),bits(exp)}))return {};++f.powers;return std::pow(base,exp);};
  s.truncate=[](void*p,double v)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate({5,0x80126a70,bits(v)}))return {};if(!std::isfinite(v)||v<-2147483648.0||v>=2147483648.0)return {};++f.conversions;return int32_t(v);};return s;}
 std::optional<uint32_t>run(){return resumePackedWeaponCost(Unit,Weapon,services(),cursor);}
};
void retained(uint32_t item,uint8_t base,uint32_t copies,uint8_t reduction){Fixture reference;reference.words[Fixture::Weapon+8]=item;reference.bytes[Fixture::Weapon+0x11]=base;reference.copies=copies;reference.bytes[Fixture::Unit+0x2f5]=reduction;auto expected=reference.run();assert(expected);
 for(size_t at=0;at<reference.trace.size();++at){Fixture f;f.words=reference.words;f.bytes=reference.bytes;f.copies=copies;f.hold=int(at);f.reentry=true;assert(!f.run());const auto trace=f.trace;auto bad=f.services();bad.power=nullptr;assert(!resumePackedWeaponCost(Fixture::Unit,Fixture::Weapon,bad,f.cursor));assert(!resumePackedWeaponCost(Fixture::Unit+1,Fixture::Weapon,f.services(),f.cursor));assert(!f.run()&&f.trace==trace);assert(f.run()==expected&&f.trace==reference.trace&&f.count==reference.count&&f.powers==reference.powers&&f.conversions==reference.conversions);}
}
}
int main(){
 for(uint32_t id:{0u,239u,240u,243u,244u,246u,247u,249u,250u,251u,252u,253u,254u,255u,256u,257u,258u,331u,332u,334u,335u,336u,337u,338u,339u,0xffffffffu})for(uint32_t copies:{0u,1u,2u,4u})for(uint8_t base:{uint8_t(0),uint8_t(1),uint8_t(7),uint8_t(255)})for(uint8_t reduction:{uint8_t(0),uint8_t(1),uint8_t(255)}){
  Fixture f;f.words[Fixture::Weapon+8]=id;f.bytes[Fixture::Weapon+0x11]=base;f.copies=copies;f.bytes[Fixture::Unit+0x2f5]=reduction;auto got=f.run();DamageUnit u;u.fpCostReduction=reduction;DamageWeapon w;w.itemId=int32_t(id);w.fpCost=base;WeaponCostServices s;s.user=&copies;s.equippedCount=[](void*p,int32_t){return int32_t(*static_cast<uint32_t*>(p));};s.pow=[](void*,double a,double b){return std::pow(a,b);};s.truncate=[](void*,double v){assert(v>=-2147483648.0&&v<2147483648.0);return int32_t(v);};assert(got&&*got==uint32_t(weaponFpCost(u,w,s)));
 }
 retained(0,0,0,0);retained(0,7,1,2);retained(240,7,0,2);retained(244,255,4,0);retained(250,7,9,1);
 Fixture free;free.bytes.erase(Fixture::Unit+0x2f5);auto s=free.services();s.equippedCount=nullptr;s.power=nullptr;s.truncate=nullptr;auto result=resumePackedWeaponCost(0,Fixture::Weapon,s,free.cursor);assert(result&&*result==0&&free.trace.size()==2&&free.count==0);
 Fixture reload;reload.words[Fixture::Weapon+8]=240;reload.bytes[Fixture::Weapon+0x11]=5;reload.copies=3;reload.effect=[](auto&f,const Op&op){if(op.kind==3){f.words[Fixture::Weapon+8]=244;f.bytes[Fixture::Weapon+0x11]=99;}};assert(reload.run()==15&&reload.powers==0&&reload.count==1);
 Fixture constants;constants.words[Fixture::Weapon+8]=240;constants.bytes[Fixture::Weapon+0x11]=5;constants.copies=2;constants.effect=[](auto&f,const Op&op){if(op.kind==4)f.words[0x802f00bc]=0x80000001;};assert(constants.run()==8); // Bias reload after pow changes conversion of captured base5 to4.
 Fixture missing;missing.words[Fixture::Weapon+8]=244;missing.bytes[Fixture::Weapon+0x11]=7;missing.bytes.erase(Fixture::Unit+0x2f5);assert(!missing.run()&&missing.cursor.pending()==0x80126a8c&&missing.count==1);missing.bytes[Fixture::Unit+0x2f5]=2;assert(missing.run()==5&&missing.count==1);
 Fixture unavailable;unavailable.words[Fixture::Weapon+8]=240;auto service=unavailable.services();service.power=nullptr;assert(!resumePackedWeaponCost(Fixture::Unit,Fixture::Weapon,service,unavailable.cursor)&&unavailable.cursor.pending()==0x80272620&&unavailable.count==1);
 Fixture stale;stale.hold=0;assert(!stale.run());stale.live=false;assert(!stale.run()&&stale.cursor.stale());auto trace=stale.trace;stale.live=true;assert(!stale.run()&&stale.trace==trace);
 std::cout<<"Packed original weapon costs, source zero branch and retained children PASS\n";
}
