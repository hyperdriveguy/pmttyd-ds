#include "field_map_file_owner.hpp"
#include "field_map_source_owner.hpp"
#include "field_map_sdk.hpp"
#include "event_native_runtime.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
using namespace ttyd;
namespace {
using namespace FieldMapSource;
constexpr uint32_t ProcessWork=0x86000000,MapStack=0x87000000,Heap=0x88000000;
constexpr uint32_t FileTable=0x89000000,WorkerStack=0x8b000000,Inputs=0x8c000000;
constexpr uint32_t Archives=0x803dbff0,ArchiveBytes=4*0x28,OriginalPayloadBytes=975520;
constexpr uint32_t Distance=0x8029eeb4,SceneRelative=0xd2b00,CollisionRelative=0xd02d8;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t w=0;assert(m.readWord(a,w));return w;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t w=0;assert(m.readHalf(a,w));return w;}
void text(EventAddressSpace& m,uint32_t a,const char* s){do{assert(m.writeByte(a++,uint8_t(*s)));}while(*s++);}
std::string text(EventAddressSpace& m,uint32_t a){std::string s;for(unsigned i=0;i<256;++i){uint8_t b=0;assert(m.readByte(a+i,b));if(!b)return s;s+=char(b);}assert(false);return {};}
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};
 std::vector<uint8_t> heap=std::vector<uint8_t>(8*1024*1024);
 std::unique_ptr<FieldMapFileOwner> files;
 std::unique_ptr<FieldMapSourceOwner> maps;
 bool enableEstimate=false;unsigned estimates=0;
 FieldMapSdk sdk{vm.memory,[this]{return maps && maps->current();},[this](float squared)->std::optional<double>{if(!enableEstimate)return {}; ++estimates;return 1.0/std::sqrt(double(squared));}}; // Explicit native approximation, not Gekko seed parity.
 bool unloadReceipts=false;std::vector<uint32_t> resets;
 bool drawSync=false,invalidate=false;unsigned syncs=0,invalidates=0;
 uint32_t frontier=0;std::array<uint32_t,8> frontierArguments{};
 explicit Fixture(const char* root){
  auto& m=vm.memory;EventScriptAssets original,literals;
  assert(original.load((std::string(root)+"/field-harbor-map-source.pmevt").c_str()));
  assert(literals.load((std::string(root)+"/field-harbor-loader-literals.pmevt").c_str()));
  EventScriptAssets hit;assert(hit.load((std::string(root)+"/field-hit-literals.pmevt").c_str()));
  assert(original.merge(literals) && original.merge(hit) && m.importScripts(original)); // Shared delimiter bytes agree.
  assert(m.registerExternalRam(Heap,heap.data(),uint32_t(heap.size())) && m.registerRam(PrimaryHeap,{0x88,0,0,0}));
  assert(m.writeWord(Heap,0) && m.writeWord(Heap+4,uint32_t(heap.size())) && m.writeHalf(Heap+8,0));
  assert(m.registerRam(Archives,std::vector<uint8_t>(ArchiveBytes)) && m.registerRam(Inputs,std::vector<uint8_t>(512)));
  files=FieldMapFileOwner::bind(m,FileTable,WorkerStack,root);assert(files);
  FieldMapHeapOwner owner;owner.user=this;owner.address=Heap;owner.bytes=uint32_t(heap.size());owner.backing=heap.data();
  owner.flushProcess=[](void*,uint8_t*,uint32_t){return true;};owner.flushBlock=[](void*,uint32_t){};owner.systemCall=[](void*){};
  owner.sdk=[](void* u,FieldMapSourceCall& q){return static_cast<Fixture*>(u)->child(q);};
  maps=FieldMapSourceOwner::bind(m,ProcessWork,MapStack,owner);assert(maps);
 }
 bool child(FieldMapSourceCall& q){
  const auto& a=q.arguments;auto& m=vm.memory;
  if(FieldMapFileOwner::accepts(q.function))return files->service(q);
  switch(q.function){
  // Explicit fixture-only GPU/cache boundary receipts. Production source owner
  // has no such provider. They isolate acquisition/relocation/scene construction.
  case 0x80005ce4:++syncs;return drawSync;
  case 0x802b4df4:++invalidates;return invalidate;
  case 0x80296a54:return true;
  case 0x80018df8:case 0x8001c1d8:case 0x8019e058:case 0x8000f310:case 0x8000e8c0:
   if(!unloadReceipts){frontier=q.function;return false;}
   resets.push_back(q.function);return true; // Explicit source-composition fixture.
  case 0x8026d5b8:{const auto s=text(m,a[1]);for(unsigned i=0;i<a[2];++i)assert(m.writeByte(a[0]+i,i<s.size()?uint8_t(s[i]):0));q.result=a[0];return true;}
  case 0x8026a0b8:{const auto format=text(m,a[1]);std::string result;
   if(format=="./m/%s/d")result="./m/"+text(m,a[2])+"/d";
   else if(format=="./m/%s/t")result="./m/"+text(m,a[2])+"/t";
   else{assert(format=="%s/%s");result=text(m,a[2])+"/"+text(m,a[3]);}
   text(m,a[0],result.c_str());q.result=uint32_t(result.size());return true;}
  default:
   if(sdk.advance(q))return true;
   frontier=q.function;frontierArguments=q.arguments;return false;
  }
 }
 std::optional<uint32_t> run(FieldMapRequest& request,uint32_t f,const std::array<uint32_t,5>& args={}){
  for(unsigned i=0;i<100000;++i){auto result=maps->invoke(request,f,args);if(result)return result;}assert(false);return {};
 }
};
}
void originalMapLoadPrefix(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapRequest request,other;
 assert(f.run(request,Init));text(m,Inputs,"gor_00");assert(m.writeWord(ProcessWork,77));
 assert(!f.maps->invoke(request,Load,{Inputs}));assert(f.syncs==1 && word(m,ProcessWork)==77);
 assert(!f.maps->invoke(other,Load,{Inputs}) && f.syncs==1);
 f.drawSync=true;
 for(unsigned i=0;!f.invalidates && i<100000;++i)assert(!f.maps->invoke(request,Load,{Inputs}));
 assert(f.invalidates && !f.frontier && word(m,ProcessWork)==0 && f.maps->pending());
 const auto d=word(m,ProcessWork+0x7c),t=word(m,ProcessWork+0x84);
 assert(d>=Heap && t>=Heap && d!=t && word(m,d)==1039618 && word(m,ProcessWork+0x80)==1039618);
 assert(word(m,t)==0x0020af30 && word(m,t+8)>0x80000000u && word(m,ProcessWork+0x88)==525792);
 // All original fixups are still unrelocated at the vertex-cache child.
 const auto fixups=d+0x20+OriginalPayloadBytes;assert(word(m,d+8)==15974);
 std::vector<std::array<uint32_t,2>> original;
 for(unsigned i=0;i<15974;++i){const auto site=word(m,fixups+4*i);original.push_back({site,word(m,d+0x20+site)});}
 f.invalidate=true;
 for(unsigned i=0;!f.frontier && i<100000;++i)assert(!f.maps->invoke(request,Load,{0}));
 assert(f.frontier==Distance && word(m,ProcessWork+HitCount)==98);
 const auto hits=word(m,ProcessWork+HitArray);assert(hits>=Heap && word(m,hits+8)==d+0x20+CollisionRelative);
 assert(half(m,hits+0xaa)==0 && word(m,ProcessWork)==0 && word(m,ProcessWork+ObjectCount)==540);
 const auto payload=d+0x20;assert(word(m,ProcessWork+0x58)==payload && word(m,ProcessWork+0x78)==d);
 for(const auto& item:original)assert(word(m,payload+item[0])==payload+item[1]);
 const auto array=word(m,ProcessWork+ObjectArray);assert(word(m,ProcessWork+Root)==array && word(m,array+Joint)==payload+SceneRelative);
 assert(text(m,word(m,word(m,array+52*ObjectBytes+Joint)))=="S_fune_b");
 // Neither missing collision nor repeated retries can publish loaded count.
 const auto pc=f.maps->pendingInstruction();for(unsigned i=0;i<4;++i)assert(!f.maps->invoke(request,Load,{Inputs}));
 assert(pc==f.maps->pendingInstruction() && word(m,ProcessWork)==0);
 // Resume the genuine original constructor with an explicit native estimate.
 // Its first bbox expansion must not replay while the seed child is held.
 const auto firstJoint=word(m,hits+8),expanded=word(m,firstJoint+0x48);
 f.enableEstimate=true;f.frontier=0;
 for(unsigned i=0;!f.frontier && i<100000;++i)assert(!f.maps->invoke(request,Load,{0}));
 assert(f.frontier==0x8001bd98 && word(m,firstJoint+0x48)==expanded);
 assert(f.estimates==98 && word(m,ProcessWork)==0);
 unsigned objects=0,triangles=0,payloadBytes=0;
 for(unsigned i=0;i<98;++i){const auto hit=hits+i*HitObjectBytes;assert(word(m,hit+8));++objects;const auto n=half(m,hit+0xa8);triangles+=n;if(n){assert(word(m,hit+0xac)>=Heap);payloadBytes+=n*HitTriangleBytes;}}
 assert(objects==98 && triangles>0 && triangles<=1604 && payloadBytes<=134736);
 // Close is queued by the parent; only real source Main retires the final slot.
 assert(half(m,FileTable+FieldMapFileOwner::SlotBytes+0x90)&8);
 for(unsigned i=0;!f.files->pump() && i<100000;++i){}
 assert(!half(m,FileTable+0x90) && !half(m,FileTable+FieldMapFileOwner::SlotBytes+0x90));
}
void fileOwnerIdentityAndClose(const char* root){
 Fixture f(root);auto& m=f.vm.memory;const auto table=f.files->tableAddress();
 std::vector<uint8_t> replacement(FieldMapFileOwner::TableBytes);
 assert(m.unregisterExternalRam(table) && m.registerExternalRam(table,replacement.data(),uint32_t(replacement.size())));
 assert(!f.files->current() && !f.files->pump());FieldMapSourceCall q;q.function=0x8029fdbc;assert(!f.files->service(q));
 f.files.reset();assert(m.unregisterExternalRam(table,replacement.data()));
}

namespace {
constexpr uint32_t Elements=0x8d000000,Comparator=0x90000000,Palette=Elements+0x800;
struct GraphBus {
 Fixture& fixture;unsigned operations=0,hold=0;int32_t readResult=0;
 std::vector<std::tuple<unsigned,uint32_t,uint32_t>> trace;
 std::vector<std::array<uint32_t,8>> reads,callbacks;
 bool gate(){return ++operations!=hold;}
 FieldMapSourceServices services(){FieldMapSourceServices s;s.user=this;
  s.word=[](void* u,uint32_t a,uint32_t& v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.readWord(a,v));b.trace.emplace_back(0,a,v);return true;};
  s.half=[](void* u,uint32_t a,uint16_t& v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.readHalf(a,v));b.trace.emplace_back(1,a,v);return true;};
  s.byte=[](void* u,uint32_t a,uint8_t& v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.readByte(a,v));b.trace.emplace_back(2,a,v);return true;};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.writeWord(a,v));b.trace.emplace_back(3,a,v);return true;};
  s.storeHalf=[](void* u,uint32_t a,uint16_t v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.writeHalf(a,v));b.trace.emplace_back(4,a,v);return true;};
  s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;assert(b.fixture.vm.memory.writeByte(a,v));b.trace.emplace_back(5,a,v);return true;};
  s.call=[](void* u,FieldMapSourceCall& q){auto& b=*static_cast<GraphBus*>(u);if(!b.gate())return false;b.trace.emplace_back(6,q.function,q.arguments[0]);auto& m=b.fixture.vm.memory;
   if(q.function==Comparator){const auto a=word(m,q.arguments[0]),c=word(m,q.arguments[1]);q.result=a==c?0u:a<c?UINT32_MAX:1u;return true;}
   // Explicit portable source-composition fixtures for SDK transfer/status and
   // callback ABIs. They are never registered as native physical file handles.
   if(q.function==0x802a03c0){b.reads.push_back(q.arguments);q.result=uint32_t(b.readResult);return true;}
   if(q.function==0x90000004){b.callbacks.push_back(q.arguments);return true;}
   if(q.function==0x802a0178){q.result=1;return true;}
   return b.fixture.files->service(q) || b.fixture.sdk.advance(q);
  };return s;
 }
 uint32_t run(uint32_t function,const std::array<uint32_t,5>& args={},unsigned budget=4096){
  FieldMapSourceCursor c;for(unsigned i=0;i<1000000;++i){auto result=resumeFieldMapSource(function,args,MapStack-32,services(),c,budget);if(result)return *result;}assert(false);return 0;
 }
};
void preparePalette(EventAddressSpace& m){
 for(unsigned i=0;i<128;++i)assert(m.writeByte(Palette+i,0));
 assert(m.writeWord(Palette,0x0020af30) && m.writeWord(Palette+4,1) && m.writeWord(Palette+8,12));
 assert(m.writeWord(Palette+12,20) && m.writeWord(Palette+16,64));
 assert(m.writeWord(Palette+28,96) && m.writeWord(Palette+72,112));
}
}
void mapSourceBoundaryContracts(const char* root){
 Fixture f(root);auto& m=f.vm.memory;assert(m.registerRam(Elements,std::vector<uint8_t>(4096)) && m.registerRam(MapStack-0x10000,std::vector<uint8_t>(0x10000)));
 // Complete original qqsort/fsort, including recursive dynamic comparator
 // calls, carry/division arithmetic, duplicate keys and retained instruction budgets.
 for(unsigned count:std::array<unsigned,9>{{0,1,2,7,16,17,31,127,256}}){
  std::vector<uint32_t> sorted;for(unsigned i=0;i<count;++i){const auto value=(i*37+11)%19;sorted.push_back(value);assert(m.writeWord(Elements+4*i,value));}
  GraphBus bus{f};bus.run(Sort,{Elements,count,4,Comparator},17);std::sort(sorted.begin(),sorted.end());
  for(unsigned i=0;i<count;++i)assert(word(m,Elements+4*i)==sorted[i]);
 }
 preparePalette(m);GraphBus expected{f};expected.run(UnpackPalette,{Palette});const auto golden=expected.trace;
 const auto operations=expected.operations;assert(word(m,Palette+8)==Palette+12 && word(m,Palette+12)==Palette+20 && word(m,Palette+16)==Palette+64 && word(m,Palette+28)==Palette+96 && word(m,Palette+72)==Palette+112);
 for(unsigned hold=1;hold<=operations;++hold){preparePalette(m);GraphBus actual{f};actual.hold=hold;actual.run(UnpackPalette,{Palette});assert(actual.trace==golden);}
 // Unpacking an already relocated table skips all image/palette repairs.
 assert(m.writeByte(Palette+20+35,0));GraphBus repeat{f};repeat.run(UnpackPalette,{Palette});uint8_t unpacked=1;assert(m.readByte(Palette+20+35,unpacked) && !unpacked);
 // Worker success counts the requested chunk, even if SDK returns fewer bytes.
 // Cancellation completes without counters; other errors preserve pending bit.
 for(const int32_t result:std::array<int32_t,3>{{7,-3,-1}}){
  for(unsigned i=0;i<FieldMapFileOwner::TableBytes;++i)assert(m.writeByte(FileTable+i,0));
  assert(m.writeHalf(FileTable+0x90,0x8001) && m.writeHalf(FileTable+0x92,2) && m.writeWord(FileTable+0x7c,Elements));
  assert(m.writeWord(FileTable+0x80,64) && m.writeWord(FileTable+0x84,8) && m.writeWord(FileTable+0x8c,0x90000004));
  GraphBus worker{f};worker.readResult=result;worker.run(FileWorker,{},23);assert(worker.reads.size()==1 && worker.reads[0][2]==64 && worker.reads[0][3]==8);
  assert(word(m,FileTable+0x80)==(result>=0?0u:64u) && word(m,FileTable+0x88)==(result>=0?64u:0u));
  assert(word(m,FileTable+0x7c)==Elements+(result>=0?64u:0u));
  assert(half(m,FileTable+0x90)==(result>=0 || result==-3?0x8002:0x80a1));
  assert(worker.callbacks.size()==(result==-1?0u:1u));if(result!=-1)assert(worker.callbacks[0][0]==uint32_t(result) && worker.callbacks[0][1]==FileTable+0x40);
 }
 // Source Close queues bit08 without erasing/releasing the slot. Worker owns
 // cancellation/close and only then the exact98-byte slot clear.
 GraphBus closing{f};closing.run(FileClose,{FileTable});assert(half(m,FileTable+0x90)==0x80a9);
 closing.run(FileWorker);assert(!half(m,FileTable+0x90));
 // Nonzero full16-bit mode caps chunks, although sort compares only low8.
 assert(m.writeHalf(FileTable+0x90,0x8001) && m.writeHalf(FileTable+0x92,0x100));
 assert(m.writeWord(FileTable+0x7c,Elements) && m.writeWord(FileTable+0x80,0x40020));GraphBus capped{f};capped.run(FileWorker);assert(capped.reads.size()==1 && capped.reads[0][2]==0x40000 && word(m,FileTable+0x80)==32 && half(m,FileTable+0x90)==0x8001);
}


void mapUnloadOwnership(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapRequest request;assert(f.run(request,Init));
 std::vector<uint32_t> allocations;
 for(unsigned i=0;i<6;++i){const auto p=f.run(request,AllocateTail,{Heap,64});assert(p && *p);allocations.push_back(*p);}
 const auto work=f.maps->workAddress();assert(m.writeWord(work,1));
 for(const auto pair:std::array<std::array<uint32_t,2>,6>{{{{0x154,allocations[0]}},{{0x164,allocations[1]}},{{0x16c,allocations[2]}},{{0x7c,allocations[3]}},{{0x84,allocations[4]}},{{0x8c,allocations[5]}}}})assert(m.writeWord(work+pair[0],pair[1]));
 assert(m.writeWord(work+0x150,1) && m.writeWord(work+0x160,1) && m.writeWord(work+0x168,64));
 // No flag requirement in archive membership, and the range end is inclusive.
 // Borrowed T must survive unload; descriptor1 alone is then deleted.
 assert(m.writeWord(Archives+0x28+0x20,allocations[4]-16) && m.writeWord(Archives+0x28+0x24,16));
 assert(m.writeWord(Archives,0x11223344) && m.writeWord(Archives+2*0x28,0x55667788));
 f.drawSync=true;for(unsigned i=0;!f.frontier && i<1000;++i)assert(!f.maps->invoke(request,Unload));
 assert(f.frontier==0x80018df8 && word(m,work)==1 && !word(m,work+0x154) && !word(m,work+0x164) && !word(m,work+0x16c));
 assert(word(m,work+0x7c)==allocations[3] && half(m,allocations[3]-24)==1);
 f.unloadReceipts=true;assert(f.run(request,Unload));
 assert((f.resets==std::vector<uint32_t>{0x80018df8,0x8001c1d8,0x8019e058,0x8000f310,0x8000e8c0}));
 for(unsigned i=0;i<GroupBytes;++i){uint8_t b=1;assert(m.readByte(work+i,b) && !b);}
 assert(half(m,allocations[4]-24)==1); // Original borrowed archive allocation.
 assert(word(m,Archives)==0x11223344 && word(m,Archives+2*0x28)==0x55667788);
 for(unsigned i=0;i<0x28;++i){uint8_t b=1;assert(m.readByte(Archives+0x28+i,b) && !b);}
 assert(f.run(request,Free,{Heap,allocations[4]}));assert(!word(m,Heap) && !half(m,Heap+8) && word(m,Heap+4)==f.heap.size());
}

namespace {
uint32_t floatBits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
void hitTriangleContracts(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapRequest init;assert(f.run(init,Init));
 assert(m.registerRam(Elements,std::vector<uint8_t>(4096)) && m.registerRam(MapStack-0x10000,std::vector<uint8_t>(0x10000)));
 constexpr uint32_t Hit=Elements,Joint=Elements+0x100,Mesh=Elements+0x200,Stream=Elements+0x300;
 constexpr uint32_t Positions=Elements+0x320,Strip=Elements+0x500,Output=Elements+0x600,Vcd=Elements+0x800;
 auto prepare=[&](bool packed,bool degenerate,unsigned quantization){
  for(unsigned i=0;i<4096;++i)assert(m.writeByte(Elements+i,0));
  assert(m.writeWord(Hit+8,Joint) && m.writeWord(Hit+0xac,Output));
  for(unsigned i=0;i<12;++i)assert(m.writeWord(Hit+0xc+4*i,floatBits(i%5==0?1.0f:0.0f)));
  assert(m.writeWord(Joint+0x5c,1) && m.writeWord(Joint+0x64,Mesh));
  assert(m.writeByte(Mesh+3,packed) && m.writeWord(Mesh+4,1) && m.writeWord(Mesh+8,1));
  assert(m.writeWord(Mesh+0xc,Stream) && m.writeWord(Stream,Positions) && m.writeWord(Mesh+0x10,Strip));
  assert(m.writeWord(ProcessWork+0xa4,Vcd) && m.writeWord(Vcd+0x44,quantization));
  const std::array<std::array<int16_t,3>,4> values{{{{0,0,0}},{{256,0,0}},{{0,256,0}},{{0,int16_t(degenerate?256:0),0}}}};
  if(packed){assert(m.writeHalf(Strip+1,4));for(unsigned i=0;i<4;++i){assert(m.writeHalf(Strip+3+2*i,uint16_t(i)));for(unsigned j=0;j<3;++j)assert(m.writeHalf(Positions+4+6*i+2*j,uint16_t(values[i][j])));}}
  else {assert(m.writeWord(Strip,4));for(unsigned i=0;i<4;++i){assert(m.writeHalf(Strip+4+0x18*i,uint16_t(i)));for(unsigned j=0;j<3;++j)assert(m.writeWord(Positions+4+12*i+4*j,floatBits(float(values[i][j])/256)));}}
 };
 for(bool packed:{false,true})for(bool degenerate:{false,true}){
  prepare(packed,degenerate,8);GraphBus expected{f};const auto count=expected.run(HitVertices,{Hit},19);assert(count==(packed && degenerate?1u:2u));
  const std::array<float,21> triangle{{0,0,0,0,1,0,1,0,0,1,0,0,0,-1,0,-1,1,0,0,0,-1}};
  for(unsigned i=0;i<triangle.size();++i){const float value=triangle[i];const auto actual=word(m,Output+4*i);assert((actual&0x7fffffff)==(floatBits(value)&0x7fffffff));}
  const auto trace=expected.trace;for(unsigned hold=1;hold<=expected.operations;++hold){prepare(packed,degenerate,8);GraphBus actual{f};actual.hold=hold;assert(actual.run(HitVertices,{Hit},7)==count && actual.trace==trace);}
 }
 // Signed six-bit load scale: encoded32 means multiply by2^32.
 prepare(true,false,32);GraphBus signedScale{f};assert(signedScale.run(HitVertices,{Hit})==2);assert(word(m,Output+0x18)==floatBits(std::ldexp(256.0f,32)));
 // Empty triangle pointer still executes scalar/pair stack save and restore.
 assert(m.writeWord(Hit+0xac,0));GraphBus empty{f};assert(empty.run(HitVertices,{Hit})==0);
}
void distanceServiceContracts(const char* root){
 Fixture f(root);auto& m=f.vm.memory;constexpr uint32_t Left=Inputs+0x80,Right=Inputs+0xa0;
 for(unsigned i=0;i<3;++i)assert(m.writeWord(Left+4*i,floatBits(i==0?1.0f:0.0f)) && m.writeWord(Right+4*i,0));
 bool deliver=false;unsigned calls=0;float captured=0;
 FieldMapSdk sdk{m,[]{return true;},[&](float q)->std::optional<double>{++calls;captured=q;if(!deliver)return {};return 0.99981689453125;}};
 FieldMapSourceCall q;q.function=Distance;q.arguments[0]=Left;q.arguments[1]=Right;
 assert(!sdk.advance(q) && calls==1 && captured==1 && !q.floatingResult);
 auto foreign=q;foreign.arguments[0]+=4;assert(!sdk.advance(foreign) && calls==1);
 assert(m.writeWord(Left,floatBits(9)));deliver=true;
 assert(sdk.advance(q) && calls==2 && captured==1 && q.floatingResult==0x3f7fffff);
 // Cross retains fused cancellation and source final negative zero.
 FieldMapSourceCall cross;cross.function=0x8029eca4;cross.arguments[0]=Left;cross.arguments[1]=Right;cross.arguments[2]=Inputs+0xc0;
 const std::array<float,3> a{{0,4097,4096}},b{{0,4098,4097}};
 for(unsigned i=0;i<3;++i)assert(m.writeWord(Left+4*i,floatBits(a[i])) && m.writeWord(Right+4*i,floatBits(b[i])));
 assert(sdk.advance(cross) && word(m,Inputs+0xc0)==0x3f800000);
 for(unsigned i=0;i<3;++i)assert(m.writeWord(Left+4*i,0) && m.writeWord(Right+4*i,0));
 assert(sdk.advance(cross) && word(m,Inputs+0xc0)==0 && word(m,Inputs+0xc4)==0x80000000 && word(m,Inputs+0xc8)==0x80000000);
 // Fresh zero distance returns exact squared zero without requesting a seed.
 assert(m.writeWord(Left,0));q.floatingResult.reset();assert(sdk.advance(q) && q.floatingResult==0 && calls==2);
}
}

int main(){hitTriangleContracts("romfs");distanceServiceContracts("romfs");mapSourceBoundaryContracts("romfs");originalMapLoadPrefix("romfs");mapUnloadOwnership("romfs");fileOwnerIdentityAndClose("romfs");}
