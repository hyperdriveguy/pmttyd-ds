#include "field_npc_registry.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <string>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t R=0x8041e8b4,G=0x80415fd0;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};
 std::vector<std::pair<uint32_t,uint32_t>> allocations,clears;
 std::vector<std::tuple<uint32_t,uint32_t>> comparisons;
 std::vector<std::pair<uint32_t,int32_t>> poses;
 std::vector<int32_t> paper,heights,radii,vivians,lights;
 std::function<void(Fixture&,unsigned)> onAllocate;
 std::function<void(Fixture&,uint32_t,uint32_t)> onStore,onCompare;
 std::function<void(Fixture&)> onCopy,onHeight,onRadius,onLight;
 std::function<int32_t(Fixture&,unsigned)> onPose;
 bool vivian=false;
 Fixture(){for(auto p:{std::pair<uint32_t,uint32_t>{0x1000,0x1000},{R-8,0x30},{0x802c11f8,24},{0x802c1468,8},{0x8041fc6c,0x2c},{0x8041fd6c,16},{0x83000000,0x1000}})assert(vm.memory.registerRam(p.first,std::vector<uint8_t>(p.second)));
  for(auto a:{0x802c1204u,0x802c1208u,0x802c120cu,0x8041fc94u})w(a,0x3f800000);
  w(0x8041fc6c,0xffffffff);text(0x802c1468,"c_luigi");text(0x8041fd6c,"hoshi");text(0x8041fd74,"M_I_2");text(0x1000,"actor");
 }
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v=0;assert(vm.memory.readWord(a,v));return v;}
 uint8_t b(uint32_t a){uint8_t v=0;assert(vm.memory.readByte(a,v));return v;}
 void text(uint32_t a,const char* s){do{assert(vm.memory.writeByte(a++,uint8_t(*s)));}while(*s++);}
 PackedNpcRegistryServices services(){PackedNpcRegistryServices s;s.user=this;auto& m=s.memory;m.user=this;
  m.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->w(a);};
  m.storeWord=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.w(a,v);if(f.onStore)f.onStore(f,a,v);};
  m.storeHalf=[](void* p,uint32_t a,uint16_t v){assert(static_cast<Fixture*>(p)->vm.memory.writeHalf(a,v));};
  m.storeByte=[](void* p,uint32_t a,uint8_t v){assert(static_cast<Fixture*>(p)->vm.memory.writeByte(a,v));};
  s.allocate=[](void* p,int32_t heap,uint32_t bytes){auto& f=*static_cast<Fixture*>(p);assert(heap==0);const uint32_t a=0x85000000+uint32_t(f.allocations.size())*0x10000;assert(f.vm.memory.registerRam(a,std::vector<uint8_t>(bytes,0xa5)));f.allocations.emplace_back(a,bytes);if(f.onAllocate)f.onAllocate(f,unsigned(f.allocations.size()));return a;};
  s.clear=[](void* p,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(p);f.clears.emplace_back(a,n);for(unsigned i=0;i<n;++i)assert(f.vm.memory.writeByte(a+i,0));};
  s.compare=[](void* p,uint32_t a,uint32_t b){auto& f=*static_cast<Fixture*>(p);f.comparisons.emplace_back(a,b);if(f.onCompare)f.onCompare(f,a,b);for(;;++a,++b){const auto x=f.b(a),y=f.b(b);if(x!=y || !x)return int32_t(x)-int32_t(y);}};
  s.copyString=[](void* p,uint32_t a,uint32_t b){auto& f=*static_cast<Fixture*>(p);uint8_t v;do{v=f.b(b++);assert(f.vm.memory.writeByte(a++,v));}while(v);if(f.onCopy)f.onCopy(f);};
  s.poseEntry=[](void* p,uint32_t model,int32_t selector){auto& f=*static_cast<Fixture*>(p);f.poses.emplace_back(model,selector);return f.onPose?f.onPose(f,unsigned(f.poses.size())):7;};
  s.paperOn=[](void* p,int32_t pose){static_cast<Fixture*>(p)->paper.push_back(pose);};
  s.heightBits=[](void* p,int32_t pose)->uint32_t{auto& f=*static_cast<Fixture*>(p);f.heights.push_back(pose);if(f.onHeight)f.onHeight(f);return 0x42000000;};
  s.radiusBits=[](void* p,int32_t pose)->uint32_t{auto& f=*static_cast<Fixture*>(p);f.radii.push_back(pose);if(f.onRadius)f.onRadius(f);return 0x41200000;};
  s.vivianType=[](void* p,int32_t pose){auto& f=*static_cast<Fixture*>(p);f.vivians.push_back(pose);return f.vivian?1:0;};
  s.lightFlagOn=[](void* p,int32_t pose,uint32_t flags){auto& f=*static_cast<Fixture*>(p);assert(flags==2);f.lights.push_back(pose);if(f.onLight)f.onLight(f);};return s;
 }
 void initialize(){initializePackedNpcRegistry(services());}
};
void initialization(){Fixture f;f.w(R,9);f.w(R+0x14,10);f.initialize();assert((f.allocations==std::vector<std::pair<uint32_t,uint32_t>>{{0x85000000,36*0x340},{0x85010000,36*0x94},{0x85020000,16*0x340},{0x85030000,0x580}}));assert(f.clears.size()==3 && f.clears[0]==f.allocations[0] && f.clears[1]==f.allocations[2] && f.clears[2]==f.allocations[3]);assert(f.w(R)==9 && f.w(R+0x14)==10 && f.w(R+4)==36 && f.w(R+0x18)==16 && f.w(R+0xc)==0x85000000 && f.w(R+0x20)==0x85020000 && f.w(R-4)==0x85010000 && f.w(R-8)==0 && f.w(0x81000168)==0x85030000 && f.w(0x85010000)==0xa5a5a5a5 && f.w(0x85030000)==0);
 Fixture alias;assert(alias.vm.memory.registerRam(0x86000000,std::vector<uint8_t>(0x580,0xcc)));alias.w(0x83000168,0x86000000);alias.onAllocate=[](Fixture& f,unsigned n){if(n==1)f.w(R+4,2);if(n==3)f.w(R+0x18,1);};alias.onStore=[](Fixture& f,uint32_t a,uint32_t){if(a==0x81000168)f.w(G,0x83000000);};alias.initialize();assert(alias.clears[0].second==2*0x340 && alias.allocations[1].second==2*0x94 && alias.clears[1].second==0x340 && alias.clears[2].first==0x86000000 && alias.w(0x85030000)==0x0000a5a5 && alias.w(0x86000000)==0);alias.w(G,0x81000000);
}
void constructionAndNames(){Fixture f;f.initialize();const auto a=f.w(R+0xc);for(unsigned i=0;i<0x340;++i)assert(f.vm.memory.writeByte(a+i,0xcc));f.w(a,0xcccccccc);f.w(0x81000038,0x11223344);f.w(0x8100003c,0x55667788);auto s=f.services();assert(enterPackedNpc(0x1000,0x1000,s)==0);assert(f.w(a)==3 && f.w(a+0x104)==7 && f.w(a+0x28)==0 && f.w(a+0x14c)==0x42000000 && f.w(a+0x150)==0x41200000 && f.w(a+0x154)==0x3f800000 && f.w(a+0xe8)==0x3f800000 && f.w(a+0x110)==0xffffffff && f.w(a+0x114)==0xffffffff && f.w(a+0x198)==0x11223344 && f.w(a+0x19c)==0x55667788 && f.b(a+0x317)==1 && f.w(a+0x330)==0 && f.w(a+0x33c)==0 && f.w(R)==1);assert((f.paper==std::vector<int32_t>{7}) && f.heights==f.paper && f.radii==f.paper && f.vivians==f.paper && f.lights==f.paper);
 assert(enterPackedNpc(0x1000,0x1000,s)==1 && f.w(R)==2);assert(packedNpcNameToPointer(0x1000,PackedNpcLookup::NoAssert,s)==a);f.w(a,0x801);assert(packedNpcNameToPointer(0x1000,PackedNpcLookup::Release,s)==a);f.w(a,0);assert(packedNpcNameToPointer(0x1000,PackedNpcLookup::NoAssert,s)==a+0x340);f.text(0x1020,"missing");assert(packedNpcNameToPointer(0x1020,PackedNpcLookup::NoAssert,s)==0 && packedNpcNameToPointer(0x1020,PackedNpcLookup::Release,s)==a+36*0x340);
 f.w(0x81000014,7);assert(enterPackedNpc(0x1000,0x1000,s)==0 && packedNpcNameToPointer(0x1000,PackedNpcLookup::NoAssert,s)==f.w(R+0x20));f.w(R+0x18,0);assert(packedNpcNameToPointer(0x1000,PackedNpcLookup::Release,s)==f.w(R+0x20) && packedNpcNameToPointer(0x1000,PackedNpcLookup::NoAssert,s)==0);
}
void serviceBoundaries(){Fixture f;f.initialize();const auto a=f.w(R+0xc);f.vivian=true;f.onCopy=[](Fixture& f){f.w(0x81000014,1);};f.onPose=[](Fixture& f,unsigned n){if(n==1){f.w(0x81000014,0);return -2;}return 11;};f.onHeight=[a](Fixture& f){f.w(a+0x104,12);};f.onLight=[](Fixture& f){f.w(R,99);};assert(enterPackedNpc(0x1000,0x802c1468,f.services())==0);assert((f.poses==std::vector<std::pair<uint32_t,int32_t>>{{0x802c1468,1},{0x8041fd6c,0}}) && f.paper[0]==11 && f.heights[0]==11 && f.radii[0]==12 && f.vivians[0]==12 && f.lights[0]==12 && f.w(a)==0x04800003 && f.w(R)==100);
 Fixture stored;stored.initialize();const auto b=stored.w(R+0xc);stored.onPose=[](Fixture&,unsigned){return -2;};stored.onStore=[b](Fixture& f,uint32_t a,uint32_t v){if(a==b+0x104 && v==0xfffffffe)f.w(a,19);};enterPackedNpc(0x1000,0x1000,stored.services());assert(stored.poses.size()==1 && stored.paper[0]==19);
 Fixture moved;moved.initialize();auto s=moved.services();enterPackedNpc(0x1000,0x1000,s);assert(moved.vm.memory.registerRam(0x86010000,std::vector<uint8_t>(36*0x340)));moved.onCompare=[](Fixture& f,uint32_t left,uint32_t){if(left==0x85000008){f.w(R+0xc,0x86010000);f.w(R+4,0);}};assert(enterPackedNpc(0x1000,0x1000,s)==0 && moved.w(0x86010000)==3 && moved.w(0x85000340)==0);
}
void wordBoundaries(){Fixture f;f.initialize();const auto a=f.w(R+0xc);f.w(0x802c11f8,0x11);f.w(0x802c11fc,0x22);f.w(0x802c1200,0x33);f.w(0x81000038,0x1234);f.w(0x8100003c,0x5678);std::vector<uint32_t> timestampStores;
 f.onStore=[a,&timestampStores](Fixture& f,uint32_t address,uint32_t){if(address==a+0x98)f.w(a+0x94,0x99);if(address==a+0xe0)f.w(0x802c120c,0x44);if(address==a+0x154)f.w(0x8041fc94,0);if(address==a+0x19c){f.w(0x81000038,0xabcd);timestampStores.push_back(address);}if(address==a+0x198)timestampStores.push_back(address);if(address==a+0x110)f.w(0x8041fc6c,0xdeadbeef);};enterPackedNpc(0x1000,0x1000,f.services());assert(f.w(a+0x98)==0x11 && f.w(a+0x9c)==0x22 && f.w(a+0xa0)==0x99 && f.w(a+0xe8)==0x44 && f.w(a+0x1cc)==0x3f800000 && f.w(a+0x1c8)==0x3f800000 && f.w(a+0x114)==0xdeadbeef && f.w(a+0x198)==0x1234 && (timestampStores==std::vector<uint32_t>{a+0x19c,a+0x198}));
}
}
int main(){initialization();constructionAndNames();serviceBoundaries();wordBoundaries();}
