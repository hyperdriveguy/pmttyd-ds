#include <memory>
#include "battle_present_item_native.hpp"
#include "battle_present_item.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
struct Fixture {
 static constexpr uint32_t B=0x100000,B2=0x200000,U=0x300000,P=0x400000;
 std::map<uint32_t,uint8_t> bytes;
 std::vector<std::tuple<uint32_t,uint32_t,unsigned>> writes;
 std::vector<std::pair<uint32_t,unsigned>> reads;
 std::vector<unsigned> calls;unsigned attempts[4]{};int hold=-1;bool alive=true,probe=false;
 std::function<void(Fixture&,uint32_t)> onRead,onWrite;
 std::function<void(Fixture&,unsigned)> onCall;
 std::array<float,3> output{};const std::array<float,3>* heldOutput=nullptr;uint32_t kind=99;
 BattlePresentItemNativeCursor cursor;BattlePresentItemCursor oldCursor;
 static uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}
 static uint64_t bits(double v){uint64_t n;std::memcpy(&n,&v,8);return n;}
 Fixture(){put(0x8041eb90,B,4);put(B+0x20,U,4);put(0x802efe78,0x43300000,4);put(0x802efe7c,0x80000000,4);real(U+0x114,2);real(0x804225f8,5);real(U+0x3c,1);real(U+0x40,2);real(U+0x44,3);put(U+0xce,10,2);put(U+0x108,10,2);put(U+0x10c,5,2);put(U+0x10e,10,2);put(U+0x112,5,2);}
 uint32_t get(uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;++i)v=(v<<8)|bytes[a+i];if(a<P||a>=P+12)reads.emplace_back(a,n);if(onRead)onRead(*this,a);return v;}
 void put(uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;++i)bytes[a+n-1-i]=uint8_t(v>>(8*i));if(a<P||a>=P+12)writes.emplace_back(a,v,n);if(onWrite)onWrite(*this,a);}
 void real(uint32_t a,float n){put(a,bits(n),4);}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
 a.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->get(a,4);};a.memory.half=[](void*p,uint32_t a){return uint16_t(static_cast<Fixture*>(p)->get(a,2));};
 a.byte=[](void*p,uint32_t a){return uint8_t(static_cast<Fixture*>(p)->get(a,1));};
 a.memory.storeWord=[](void*p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->put(a,n,4);};a.memory.storeHalf=[](void*p,uint32_t a,uint16_t n){static_cast<Fixture*>(p)->put(a,n,2);};a.memory.storeByte=[](void*p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->put(a,n,1);};return a;}
 bool admit(unsigned op){++attempts[op];if(probe){assert(!cursor.reset());assert(!resumeNativeBattlePresentItem(native(),cursor));}
  if(int(op)==hold&&attempts[op]<=2)return false;
  calls.push_back(op);if(onCall)onCall(*this,op);return true;}
 BattlePresentItemNativeServices native(){BattlePresentItemNativeServices s;s.actors=actors();s.user=this;s.allocationCurrent=[](void*p){return static_cast<Fixture*>(p)->alive;};
 s.subtractDoubleSingle=[](void*p,double a,double b)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0))return {};return float(a-b);};
 s.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(1))return {};return std::fma(a,b,c);};
 s.add=[](void*p,float a,float b)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(2))return {};return a+b;};
 s.effect=[](void*p,uint32_t kind,int32_t n,const std::array<float,3>&pos){auto&f=*static_cast<Fixture*>(p);assert(n==1);if(f.heldOutput)assert(f.heldOutput==&pos);else f.heldOutput=&pos;if(!f.admit(3))return false;f.kind=kind;f.output=pos;return true;};return s;}
 BattlePresentItemServices legacy(){auto n=native();BattlePresentItemServices s;s.actors=n.actors;s.user=n.user;s.privatePosition=P;s.subtractDoubleSingle=n.subtractDoubleSingle;s.fma=n.fma;s.add=n.add;s.effect=n.effect;return s;}
 std::optional<int32_t> run(){return resumeNativeBattlePresentItem(native(),cursor);}
 void clear(){reads.clear();writes.clear();calls.clear();}
 void omitPrivate(){for(unsigned i=0;i<12;++i)bytes.erase(P+i);}
};
using Setup=std::function<void(Fixture&)>;
void compare(int32_t item,int32_t target=0,const Setup& setup={}){
 Fixture reference;reference.put(Fixture::B+0x1603c,uint32_t(item),4);reference.put(Fixture::B+0x16044,uint32_t(target),4);if(setup)setup(reference);reference.clear();auto result=resumePackedBattlePresentItem(reference.legacy(),reference.oldCursor);assert(result);reference.omitPrivate();
 Fixture actual;actual.put(Fixture::B+0x1603c,uint32_t(item),4);actual.put(Fixture::B+0x16044,uint32_t(target),4);if(setup)setup(actual);actual.clear();assert(actual.run()==result&&!actual.cursor.active());actual.omitPrivate();assert(actual.bytes==reference.bytes&&actual.reads==reference.reads&&actual.writes==reference.writes&&actual.calls==reference.calls&&actual.output==reference.output&&actual.kind==reference.kind);
 if(item!=123&&item!=124)return;
 for(int gate=0;gate<4;++gate){Fixture delayed;delayed.put(Fixture::B+0x1603c,uint32_t(item),4);delayed.put(Fixture::B+0x16044,uint32_t(target),4);if(setup)setup(delayed);delayed.clear();delayed.hold=gate;delayed.probe=true;
  assert(!delayed.run()&&delayed.cursor.active());auto writes=delayed.writes;auto reads=delayed.reads;auto bytes=delayed.bytes;
  auto bad=delayed.native();bad.user=&actual;assert(!resumeNativeBattlePresentItem(bad,delayed.cursor));bad=delayed.native();bad.actors.audienceLifecycle.user=&actual;assert(!resumeNativeBattlePresentItem(bad,delayed.cursor));bad=delayed.native();bad.fma=nullptr;assert(!resumeNativeBattlePresentItem(bad,delayed.cursor));
  assert(!delayed.run()&&delayed.writes==writes&&delayed.reads==reads&&delayed.bytes==bytes);
  assert(delayed.run()==result&&!delayed.cursor.active());delayed.omitPrivate();assert(delayed.bytes==actual.bytes&&delayed.reads==actual.reads&&delayed.writes==actual.writes&&delayed.calls==actual.calls&&delayed.output==actual.output&&delayed.kind==actual.kind);
 }
}
int main(){
 for(int32_t item:{-1,0,1,122,123,124,125,338,339,1000})for(int32_t target:{-2,-1,0,63,64}){
  if((item==123||item==124)&&target!=0)continue;
  compare(item,target);
 }
 for(uint32_t state:{0u,12u,13u,29u})compare(1,0,[=](auto&f){f.put(Fixture::B+0x171c,state,4);f.put(Fixture::B+0x1720,0xffffffff,4);f.put(Fixture::B+0x1c5c,7,4);});
 compare(123,0,[](auto&f){f.onRead=[](auto&f,uint32_t a){if(a==Fixture::U+0x3c)f.real(Fixture::U+0x40,10);if(a==Fixture::U+0x40)f.real(Fixture::U+0x44,20);};f.onCall=[](auto&f,unsigned op){if(op==0){f.real(Fixture::U+0x114,100);f.real(0x804225f8,100);}if(op==3)f.put(0x8041eb90,Fixture::B2,4);};});
 for(uint16_t height:{uint16_t(0x8000),uint16_t(0xffff),uint16_t(0),uint16_t(32767)})compare(123,0,[=](auto&f){f.put(Fixture::U+0xce,height,2);});
 compare(124,0,[](auto&f){f.put(Fixture::U+0x112,0x7fff,2);});
 compare(123,0,[](auto&f){f.put(Fixture::U+0xce,0xfffd,2);f.put(Fixture::U+0x114,0x3fd55555,4);f.real(Fixture::U+0x40,0);});
 compare(123,0,[](auto&f){f.put(Fixture::U+0x10c,10,2);f.onWrite=[](auto&f,uint32_t a){if(a==Fixture::U+0x10c)f.put(Fixture::U+0x108,4,2);};});
 compare(124,0,[](auto&f){
  constexpr uint32_t mario=Fixture::U+0x1000,replacement=Fixture::U+0x2000;
  f.put(Fixture::U+8,224,4);f.put(Fixture::B+0x24,mario,4);
  for(uint32_t u:{mario,replacement}){f.put(u+8,222,4);f.put(u+0x10e,10,2);f.put(u+0x112,5,2);}
  f.onRead=[](auto&f,uint32_t a){if(a==Fixture::U+0x1000+0x10e)f.put(Fixture::B+0x24,Fixture::U+0x2000,4);};
 });
 // No actual lifetime provider cannot start or touch source memory.
 Fixture absent;auto missing=absent.native();missing.allocationCurrent=nullptr;absent.clear();assert(!resumeNativeBattlePresentItem(missing,absent.cursor)&&!absent.cursor.active()&&absent.reads.empty());
 for(unsigned boundary=0;boundary<4;++boundary){Fixture stale;stale.put(Fixture::B+0x1603c,123,4);stale.clear();stale.onCall=[=](auto&f,unsigned op){if(op==boundary)f.alive=false;};assert(!stale.run()&&stale.cursor.stale());auto reads=stale.reads;auto writes=stale.writes;auto calls=stale.calls;stale.alive=true;assert(!stale.run()&&stale.reads==reads&&stale.writes==writes&&stale.calls==calls);assert(stale.cursor.reset());}
 Fixture missingEffect;missingEffect.put(Fixture::B+0x1603c,123,4);auto noEffect=missingEffect.native();noEffect.effect=nullptr;assert(!resumeNativeBattlePresentItem(noEffect,missingEffect.cursor)&&missingEffect.cursor.pending()==0x8014335c);auto writes=missingEffect.writes;assert(!resumeNativeBattlePresentItem(noEffect,missingEffect.cursor)&&missingEffect.writes==writes&&missingEffect.get(Fixture::U+0x10c,2)==6&&missingEffect.get(Fixture::B+0x1603c,4)==123);
 std::cout<<"Typed native whole PresentItem source parity, exact XYZ ownership, retained math/effect and allocation poisoning passed\n";
}
