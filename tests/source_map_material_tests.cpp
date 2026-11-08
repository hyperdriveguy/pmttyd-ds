#include "source_map_material.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Object=0x10000,Material=0x11000,Work=0x20000,Joint=0x30000,Control=0x31000;
constexpr uint32_t Bank=0x40000,OtherBank=0x42000,Output=0x50000,Names=0x51000,TextureNames=0x52000;
constexpr uint32_t TextureDescriptors=0x53000,TextureNameRefs=0x54000,Palette=0x55000,Images=0x56000,Headers=0x57000;
constexpr uint32_t Camera=0x60000,StackTop=0x90000,RecordBytes=96;
uint32_t bits(float v){uint32_t out;std::memcpy(&out,&v,4);return out;}
uint64_t realBits(double v){uint64_t out;std::memcpy(&out,&v,8);return out;}
double real(uint64_t v){double out;std::memcpy(&out,&v,8);return out;}
using Trace=std::tuple<unsigned,uint32_t,uint32_t>;
struct Bus {
 std::map<uint32_t,uint8_t> bytes;
 std::vector<Trace> trace;
 std::vector<MapMaterialSourceCall> requests;
 std::vector<unsigned> requestGates;
 unsigned gates=0,hold=0;uint32_t unavailable=0;
 bool gate(){return ++gates!=hold;}
 void put(uint32_t at,uint32_t value,unsigned size=4){for(unsigned i=0;i<size;++i)bytes[at+i]=uint8_t(value>>(8*(size-i-1)));}
 void zero(uint32_t at,unsigned size){for(unsigned i=0;i<size;++i)bytes[at+i]=0;}
 uint32_t get(uint32_t at,unsigned size=4)const{uint32_t value=0;for(unsigned i=0;i<size;++i){auto it=bytes.find(at+i);if(it==bytes.end()){std::cerr<<"missing "<<std::hex<<at+i<<'\n';std::abort();}value=value<<8|it->second;}return value;}
 std::string text(uint32_t at)const{std::string out;while(get(at,1))out+=char(get(at++,1));return out;}
 void text(uint32_t at,const std::string& value){for(auto c:value)put(at++,uint8_t(c),1);put(at,0,1);}
 void record(unsigned index,uint16_t flags){auto at=Bank+RecordBytes*index;zero(at,RecordBytes);put(at,flags,2);text(at+2,"light"+std::to_string(index));put(at+0x48,0x7394ffff);for(unsigned i=0;i<4;++i)put(at+0x50+4*i,bits(float(i+2)));}
 MapMaterialSourceServices services(){MapMaterialSourceServices s;auto& m=s.memory;m.user=this;
  m.word=[](void* u,uint32_t at,uint32_t& v){return static_cast<Bus*>(u)->read(at,v,4);};
  m.half=[](void* u,uint32_t at,uint16_t& v){uint32_t x=0;if(!static_cast<Bus*>(u)->read(at,x,2))return false;v=uint16_t(x);return true;};
  m.byte=[](void* u,uint32_t at,uint8_t& v){uint32_t x=0;if(!static_cast<Bus*>(u)->read(at,x,1))return false;v=uint8_t(x);return true;};
  m.store=[](void* u,uint32_t at,uint32_t v){return static_cast<Bus*>(u)->write(at,v,4);};
  m.storeHalf=[](void* u,uint32_t at,uint16_t v){return static_cast<Bus*>(u)->write(at,v,2);};
  m.storeByte=[](void* u,uint32_t at,uint8_t v){return static_cast<Bus*>(u)->write(at,v,1);};
  s.call=[](void* u,MapMaterialSourceCall& q){return static_cast<Bus*>(u)->call(q);};return s;
 }
 bool read(uint32_t at,uint32_t& v,unsigned size){if(!gate())return false;for(unsigned i=0;i<size;++i)if(!bytes.count(at+i))return false;v=get(at,size);trace.emplace_back(size,at,v);return true;}
 bool write(uint32_t at,uint32_t v,unsigned size){if(!gate())return false;put(at,v,size);trace.emplace_back(8+size,at,v);return true;}
 bool call(MapMaterialSourceCall& q){if(!gate()||q.function==unavailable)return false;requests.push_back(q);requestGates.push_back(gates);const auto& a=q.arguments;
  switch(q.function){
  case 0x8026d464:q.result=uint32_t(text(a[0]).compare(text(a[1])));break;
  case 0x8029e6a4:case 0x8029e6f8: // Fixture vector result; no SDK/GPU claim.
   for(unsigned i=0;i<3;++i)put(a[2]+4*i,bits(float(i+1)));break;
  case 0x8029dff0:zero(a[0],48);break;
  case 0x8029dd64:zero(a[2],48);break;
  case 0x8029ebc8:break;
  case 0x8029eb80:for(unsigned i=0;i<3;++i)put(a[2]+4*i,bits(float(i+4)));break;
  case 0x8029eeb4:q.realResult=realBits(double(32-(a[1]-Bank-0x24)/RecordBytes));break;
  case 0x8000519c:{std::vector<uint8_t> copy;for(unsigned i=0;i<a[2];++i)copy.push_back(uint8_t(get(a[1]+i,1)));for(unsigned i=0;i<a[2];++i)put(a[0]+i,copy[i],1);q.result=a[0];break;}
  default:assert(q.function==0x802b6cb8||q.function==0x802b6cc8||q.function==0x802b6ce4||q.function==0x802b6a3c||q.function==0x802b6cf0||q.function==0x802b6a58||q.function==0x802b6be8||q.function==0x802b6f3c||q.function==0x802b6d6c||q.function==0x802b6e54||q.function==0x802b6f78||q.function==0x802b72b0||q.function==0x802b7544||q.function==0x802b7880||q.function==0x80021b80);break;
  }
  trace.emplace_back(16,q.function,q.result.value_or(0));return true;
 }
};
Bus fixture(unsigned textures=0,unsigned policy=1){Bus b;b.zero(StackTop-2048,2064);b.zero(Object,0x134);b.zero(Material,0x114);b.zero(Joint,0x60);b.zero(Control,8);b.zero(Output,64);
 b.put(Object,0x12345678);b.put(Object+8,Joint);b.put(Joint+0x58,Control);b.put(FieldMapSource::Work,Work);b.put(FieldMapSource::ActiveGroup,0);
 b.put(0x8041e650,1);b.put(0x8041e654,Bank);b.put(0x8041e658,1);b.put(0x8041e65c,OtherBank);b.record(0,0x8001);b.zero(OtherBank,RecordBytes);b.put(OtherBank,0x8001,2);b.put(OtherBank+0x48,0x11223344);
 b.put(Material+4,0xccccccff);b.put(Material+8,policy,1);b.put(Material+0xb,textures,1);
 b.put(0x8041e608,4);b.put(0x803c95f8+16,Camera);
 EventScriptAssets literals;assert(literals.load("romfs/field-map-material.pmevt"));for(const auto& segment:literals.segments())for(unsigned i=0;i<segment.size;++i){uint8_t v=0;assert(literals.readByte(segment.address+i,v));b.put(segment.address+i,v,1);}
 b.put(Work+0x94,TextureNames);b.put(Work+0x84,Palette);b.put(Palette+8,Images);b.put(TextureNames,textures);
 for(unsigned i=0;i<textures;++i){const auto name=Names+i*32,desc=TextureDescriptors+i*32,ref=TextureNameRefs+i*8,header=Headers+i*64;
  b.text(name,"texture"+std::to_string(i));b.put(TextureNames+4+4*i,name);b.put(Material+0xc+4*i,desc);b.put(desc,ref);b.put(ref,name);b.put(desc+8,i%3,1);b.put(desc+9,(i+1)%3,1);
  b.put(Images+8*i,header);b.zero(header,0x24);b.put(header,256,2);b.put(header+2,16,2);b.put(header+4,14);b.put(header+8,0x70000+0x1000*i);b.put(header+0x1c,bits(-0.25f));b.put(header+0x20,1,1);b.put(header+0x21,128+i,1);b.put(header+0x22,255-i,1);
 }
 return b;
}
std::optional<uint32_t> run(Bus& b,MapMaterialSourceCursor& c,uint32_t fn,const std::array<uint32_t,5>& args,unsigned budget=4096){return resumeMapMaterialSource(fn,args,StackTop,b.services(),c,budget);}
void finish(Bus& b,MapMaterialSourceCursor& c,uint32_t fn,const std::array<uint32_t,5>& args,unsigned budget=4096){for(unsigned i=0;i<100000;++i)if(run(b,c,fn,args,budget))return;std::cerr<<"held "<<std::hex<<c.pc<<" child "<<c.request.function<<'\n';std::abort();}
std::vector<MapMaterialSourceCall> requests(const Bus& b,uint32_t fn){std::vector<MapMaterialSourceCall> out;for(const auto& q:b.requests)if(q.function==fn)out.push_back(q);return out;}
void same(const Bus& actual,const Bus& golden){
 assert(actual.trace==golden.trace&&actual.bytes==golden.bytes&&actual.requests.size()==golden.requests.size());
 for(unsigned i=0;i<actual.requests.size();++i){const auto& a=actual.requests[i];const auto& b=golden.requests[i];assert(a.function==b.function&&a.arguments==b.arguments&&a.reals==b.reals&&a.result==b.result&&a.realResult==b.realResult);}
}
void heldChildren(const Bus& original,uint32_t fn,const std::array<uint32_t,5>& args){
 Bus golden=original;MapMaterialSourceCursor c;finish(golden,c,fn,args);
 for(auto gate:golden.requestGates){Bus actual=original;actual.hold=gate;assert(!run(actual,c,fn,args));const auto pending=c.request;assert(c.queued);assert(c.request.arguments==pending.arguments&&c.request.reals==pending.reals);finish(actual,c,fn,args);same(actual,golden);}
}
void channelsAndTextures(){
 for(unsigned policy:{0u,1u,2u,255u}){Bus b=fixture(2,policy);MapMaterialSourceCursor c;finish(b,c,MapMaterialSource::Material,{Object,Material});
  auto channels=requests(b,0x802b6f78);assert(channels.size()==(policy<2?2:0));if(policy<2){assert((channels[0].arguments==std::array<uint32_t,8>{0,0,0,policy,0,2,2,0}));assert((channels[1].arguments==std::array<uint32_t,8>{2,0,0,policy,0,0,2,0}));}
  auto init=requests(b,0x802b72b0),lod=requests(b,0x802b7544),load=requests(b,0x802b7880);assert(init.size()==2&&lod.size()==2&&load.size()==2);
  assert(init[0].arguments[1]==0x71000&&init[1].arguments[1]==0x70000);assert(init[0].arguments[2]==16&&init[0].arguments[3]==256&&init[0].arguments[4]==14);assert(init[0].arguments[5]==1&&init[0].arguments[6]==2&&init[0].arguments[7]==1);
  assert(real(lod[0].reals[0])==129&&real(lod[0].reals[1])==254&&real(lod[0].reals[2])==-0.25);assert(real(lod[1].reals[0])==128&&real(lod[1].reals[1])==255);assert(load[0].arguments[1]==0&&load[1].arguments[1]==1);
  assert(b.requests.back().function==0x80021b80);assert(b.requests.back().arguments[0]==2&&b.requests.back().arguments[1]==0&&b.requests.back().arguments[2]==0x12345678&&b.requests.back().arguments[3]==Object+0x1c);
  assert(b.get(StackTop-432+0xc)==0x7394ffff&&b.get(StackTop-432+8)==0xccccccff);
 }
 // Every unsigned source LOD byte must survive the lfd/subtract conversion.
 for(unsigned value=0;value<256;++value){Bus b=fixture(1);b.put(Headers+0x21,value,1);b.put(Headers+0x22,255-value,1);MapMaterialSourceCursor c;finish(b,c,MapMaterialSource::Material,{Object,Material});auto lod=requests(b,0x802b7544);assert(real(lod[0].reals[0])==value&&real(lod[0].reals[1])==255-value);}
}
void exactHolds(){const std::array<uint32_t,5> args{Object,Material};Bus original=fixture(2),golden=original;MapMaterialSourceCursor done;finish(golden,done,MapMaterialSource::Material,args);
 for(unsigned gate=1;gate<=golden.gates;++gate){Bus b=original;b.hold=gate;MapMaterialSourceCursor c;assert(!run(b,c,MapMaterialSource::Material,args));const auto pc=c.pc;const auto trace=b.trace;
  auto changed=args;changed[0]+=4;assert(!run(b,c,MapMaterialSource::Material,changed));assert(c.pc==pc&&b.trace==trace);
  finish(b,c,MapMaterialSource::Material,args);same(b,golden);
 }
 Bus budget=original;MapMaterialSourceCursor c;finish(budget,c,MapMaterialSource::Material,args,1);same(budget,golden);
 Bus held=fixture();held.unavailable=0x802b6f3c;MapMaterialSourceCursor h;assert(!run(held,h,MapMaterialSource::Material,args));assert(h.queued&&h.pc==0x800211e8);const auto trace=held.trace;
 auto changed=held.services();changed.memory.user=&golden;const auto pc=h.pc;assert(!resumeMapMaterialSource(MapMaterialSource::Material,args,StackTop,changed,h));assert(h.pc==pc&&held.trace==trace);assert(!resumeMapMaterialSource(MapMaterialSource::Material,args,StackTop+32,held.services(),h));assert(h.pc==pc&&held.trace==trace);
 assert(held.get(StackTop-432+0x20)==0x7394ffff);assert(!run(held,h,MapMaterialSource::Material,args));assert(held.trace==trace);held.unavailable=0;finish(held,h,MapMaterialSource::Material,args);
}
void lightSelection(){
 Bus b=fixture();MapMaterialSourceCursor c;assert(run(b,c,MapMaterialSource::Ambient,{})==Bank);b.put(Bank,0x8081,2);assert(run(b,c,MapMaterialSource::Ambient,{})==0);b.put(FieldMapSource::ActiveGroup,1);assert(run(b,c,MapMaterialSource::Ambient,{})==OtherBank);b.put(FieldMapSource::ActiveGroup,0);
 b.text(Names,"light0");assert(run(b,c,MapMaterialSource::Name,{Names})==Bank); // Name accepts scene80.
 b.put(Control+4,3);b.put(Work+0x98,TextureNames);b.put(TextureNames+4,TextureNameRefs);b.put(TextureNameRefs,Names);b.put(TextureNames+8,TextureNameRefs);b.put(Bank,0x1081,2); // Named same record twice, no character80 exclusion.
 assert(run(b,c,MapMaterialSource::Light,{Object,Output})==2);assert(b.get(Output)==Bank&&b.get(Output+4)==Bank);assert(b.get(Output+8)==0);
 b.put(Bank,0x8001,2);assert(run(b,c,MapMaterialSource::Light,{Object,Output})==0);assert(b.get(Output)==Bank); // Named-only tail remains untouched.
 b.text(Names,"absent");assert(!run(b,c,MapMaterialSource::Light,{Object,Output}));assert(c.pc==0x80024ab8); // Original null dereference stays unfulfilled; fixture read abort avoided separately.
}
void attenuation(){
 for(auto flags:{0x1001u,0x2001u,0x4001u}){Bus b=fixture();b.record(1,uint16_t(flags));b.put(0x8041e650,2);b.put(Control+4,1);b.put(Work+0x98,TextureNames);b.put(TextureNames+4,TextureNameRefs);b.put(TextureNameRefs,Names);b.text(Names,"light1");heldChildren(b,MapMaterialSource::Material,{Object,Material});MapMaterialSourceCursor c;finish(b,c,MapMaterialSource::Material,{Object,Material});auto atten=requests(b,0x802b6a3c);if(flags==0x1001){assert(atten.size()==1);for(unsigned i=0;i<6;++i)assert(real(atten[0].reals[i])==(i==0?2:i<3?0:i+0));}else if(flags==0x2001){assert(atten.size()==1);assert(real(atten[0].reals[0])==2&&real(atten[0].reals[5])==1);}else{assert(atten.empty());assert(requests(b,0x802b6a58).size()==1&&requests(b,0x802b6be8).size()==1);}auto ctrl=requests(b,0x802b6f78);assert(ctrl[0].arguments[1]==1&&ctrl[0].arguments[4]==1&&ctrl[0].arguments[6]==(flags==0x2001?2u:1u));}
}
void nearSort(){
 Bus b=fixture();b.put(0x8041e650,10);for(unsigned i=0;i<10;++i)b.record(i,1);b.put(Bank,0x8001,2);b.zero(0x803c5360,0x3100);b.zero(0x8041e584,24);heldChildren(b,MapMaterialSource::Near,{Object+0x10,Output,8,0});MapMaterialSourceCursor c;finish(b,c,MapMaterialSource::Near,{Object+0x10,Output,8,0},1);assert(b.get(Output)==Bank+9*RecordBytes&&b.get(Output+7*4)==Bank+2*RecordBytes);assert(requests(b,0x8029eeb4).size()==10);assert(!requests(b,0x8000519c).empty());
 Bus small=fixture();small.put(0x8041e650,3);small.record(1,1);small.record(2,0x81);finish(small,c,MapMaterialSource::Near,{Object+0x10,Output,8,0x80});assert(small.get(Output)==Bank+2*RecordBytes&&small.get(Output+4)==0);assert(requests(small,0x8029eeb4).empty());
 small.zero(Output,32);assert(run(small,c,MapMaterialSource::Near,{Object+0x10,Output,0,0})==1);assert(small.get(Output)==Bank+RecordBytes);
 small.zero(Output,32);assert(run(small,c,MapMaterialSource::Near,{Object+0x10,Output,0xffffffff,0})==1);assert(small.get(Output)==Bank+RecordBytes);
}
}
int main(){channelsAndTextures();exactHolds();lightSelection();attenuation();nearSort();}
