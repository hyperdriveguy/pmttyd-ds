#include "pouch_item_count.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Pouch=0x1000,OtherPouch=0x2000,Global=0x8041eb00;
struct F{std::map<uint32_t,uint16_t>memory;std::vector<uint32_t>reads;uint32_t current=Pouch;bool swap=false;EventMemoryServices services(){EventMemoryServices m;m.user=this;m.word=[](void*p,uint32_t a){auto&f=*static_cast<F*>(p);assert(a==Global);f.reads.push_back(a);return f.current;};m.half=[](void*p,uint32_t a){auto&f=*static_cast<F*>(p);f.reads.push_back(a);if(f.swap)f.current=OtherPouch;auto i=f.memory.find(a);return i==f.memory.end()?uint16_t(0):i->second;};return m;}};
void range(int32_t item,uint32_t offset,uint32_t count){F f;for(uint32_t i=0;i<count;++i)f.memory[Pouch+offset+2*i]=uint16_t(item);assert(packedPouchItemCount(item,f.services())==int32_t(count));assert(f.reads.size()==count+1&&f.reads[0]==Global);for(uint32_t i=0;i<count;++i)assert(f.reads[i+1]==Pouch+offset+2*i);f.reads.clear();f.memory[Pouch+offset]=0xffff;assert(packedPouchItemCount(item,f.services())==int32_t(count-1));}
}
int main(){for(auto item:{1,120})range(item,0xa0,121);for(auto item:{121,151,235})range(item,0x192,20);for(auto item:{240,338})range(item,0x1fa,200);for(auto item:std::vector<int32_t>{std::numeric_limits<int32_t>::min(),-1,0,236,239,339,std::numeric_limits<int32_t>::max()}){F f;assert(!packedPouchItemCount(item,f.services())&&f.reads.empty());}F f;f.swap=true;f.memory[Pouch+0x192]=151;f.memory[Pouch+0x192+2*19]=151;f.memory[OtherPouch+0x192]=151;assert(packedPouchItemCount(151,f.services())==2&&f.current==OtherPouch&&f.reads.back()==Pouch+0x192+38);f.reads.clear();assert(packedPouchItemCount(151,f.services())==1&&f.reads[1]==OtherPouch+0x192);std::cout<<"whole source pouchCheckItem ranges/ordered20-121-200/capture tests PASS\n";}
