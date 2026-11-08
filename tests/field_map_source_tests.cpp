#include "field_map_source.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
using namespace FieldMapSource;
constexpr uint32_t WorkAddress=0x10000,StackTop=0x90000,JointRoot=0x2000,Array=0x3000,Name=0x4000;
struct Bus {
 std::map<uint32_t,uint8_t> bytes;
 std::vector<std::tuple<unsigned,uint32_t,uint32_t>> trace;
 unsigned calls=0,hold=0;
 bool gate(){return ++calls!=hold;}
 void put(uint32_t a,uint32_t w){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(w>>(24-8*i));}
 uint32_t get(uint32_t a){uint32_t w=0;for(unsigned i=0;i<4;++i){assert(bytes.count(a+i));w=w<<8|bytes[a+i];}return w;}
 void text(uint32_t a,const char* s){do{bytes[a++]=uint8_t(*s);}while(*s++);}
 std::string text(uint32_t a){std::string s;while(bytes.at(a))s+=char(bytes.at(a++));return s;}
 FieldMapSourceServices services(){FieldMapSourceServices s;s.user=this;
  s.word=[](void* u,uint32_t a,uint32_t& w){auto& b=*static_cast<Bus*>(u);if(!b.gate())return false;w=b.get(a);b.trace.emplace_back(0,a,w);return true;};
  s.store=[](void* u,uint32_t a,uint32_t w){auto& b=*static_cast<Bus*>(u);if(!b.gate())return false;b.put(a,w);b.trace.emplace_back(1,a,w);return true;};
  s.call=[](void* u,FieldMapSourceCall& q){auto& b=*static_cast<Bus*>(u);if(!b.gate())return false;assert(q.function==0x8026d464);q.result=uint32_t(b.text(q.arguments[0]).compare(b.text(q.arguments[1])));b.trace.emplace_back(2,q.arguments[0],q.arguments[1]);return true;};return s;
 }
};
Bus tree(){Bus b;for(unsigned i=0;i<5;++i){b.put(JointRoot+i*0x100+0x0c,0);b.put(JointRoot+i*0x100+0x10,0);}b.put(JointRoot+0x0c,JointRoot+0x100);b.put(JointRoot+0x10,JointRoot+0x400);b.put(JointRoot+0x100+0x0c,JointRoot+0x200);b.put(JointRoot+0x100+0x10,JointRoot+0x300);return b;}
}
void mapSource(){
 Bus expected=tree();FieldMapSourceCursor cursor;const auto result=resumeFieldMapSource(Count,{JointRoot},StackTop,expected.services(),cursor);assert(result && *result==4 && !cursor.active);const unsigned calls=expected.calls;
 for(unsigned hold=1;hold<=calls;++hold){Bus actual=tree();actual.hold=hold;FieldMapSourceCursor c;assert(!resumeFieldMapSource(Count,{JointRoot},StackTop,actual.services(),c));assert(c.active);auto completed=resumeFieldMapSource(Count,{0},1,actual.services(),c);assert(completed && *completed==4);assert(actual.trace==expected.trace && actual.bytes==expected.bytes);}
 Bus empty;FieldMapSourceCursor zero;assert(resumeFieldMapSource(Count,{0},StackTop,empty.services(),zero)==0);
 Bus lookup;lookup.put(Work,WorkAddress);lookup.put(ActiveGroup,2);const auto group=WorkAddress+2*GroupBytes;lookup.put(group,1);lookup.put(group+ObjectCount,3);lookup.put(group+ObjectArray,Array);
 for(unsigned i=0;i<3;++i){lookup.put(Array+i*ObjectBytes+Joint,JointRoot+i*0x100);lookup.put(JointRoot+i*0x100,Name+i*0x100);lookup.text(Name+i*0x100,i==1?"wanted":"other");}lookup.text(Name+0x500,"wanted");
 Bus golden=lookup;FieldMapSourceCursor find;assert(resumeFieldMapSource(Find,{Name+0x500},StackTop,golden.services(),find)==Array+ObjectBytes);
 for(unsigned hold=1;hold<=golden.calls;++hold){Bus actual=lookup;actual.hold=hold;FieldMapSourceCursor c;assert(!resumeFieldMapSource(Find,{Name+0x500},StackTop,actual.services(),c));auto done=resumeFieldMapSource(Find,{0},1,actual.services(),c);assert(done==Array+ObjectBytes && actual.trace==golden.trace && actual.bytes==golden.bytes);}
 FieldMapSourceCursor null;lookup.trace.clear();assert(resumeFieldMapSource(Find,{0},StackTop,lookup.services(),null)==0);std::vector<uint32_t> reads;for(const auto& op:lookup.trace)if(std::get<0>(op)==0)reads.push_back(std::get<1>(op));assert(reads[0]==ActiveGroup && reads[1]==Work);
}
int main(){mapSource();}
