#include "file_model_relocation.hpp"
#include <cassert>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Op {bool store;uint32_t address,value;};
struct Memory {
 std::map<uint32_t,uint32_t> words;
 std::vector<Op> trace;
 std::function<void(uint32_t,uint32_t)> stored;
 EventMemoryServices services(){EventMemoryServices s;s.user=this;
  s.word=[](void* p,uint32_t a){auto& m=*static_cast<Memory*>(p);assert(m.words.count(a));auto v=m.words.at(a);m.trace.push_back({false,a,v});return v;};
  s.storeWord=[](void* p,uint32_t a,uint32_t v){auto& m=*static_cast<Memory*>(p);m.words[a]=v;m.trace.push_back({true,a,v});if(m.stored)m.stored(a,v);};return s;}
};
constexpr uint32_t Base=0x82000000,Table=Base+0x300,Child=Base+0x500;
Memory graph(unsigned count=2){Memory m;m.words[Base+0x148]=count;
 for(unsigned o=0x14c;o<=0x1ac;o+=4)m.words[Base+o]=o==0x1ac?0x300:0x200;
 for(unsigned i=0;i<count;++i)m.words[Table+0x40*i+0x3c]=0x500;
 for(unsigned o=0x24;o<=0x40;o+=4)m.words[Child+o]=o;
 return m;}
void authored(){
 for(uint32_t value:std::initializer_list<uint32_t>{Base,Base+1,0xffffffffu}){Memory m;m.words[Base+0x14c]=value;setPackedFileModelPointers(Base,m.services());assert(m.trace.size()==1);}
 {Memory m;m.words[Base+0x14c]=Base-1;clearPackedFileModelPointers(Base,m.services());assert(m.trace.size()==1);}
 for(uint32_t count:{0u,0x80000000u,0xffffffffu}){auto m=graph(0);m.words[Base+0x148]=count;auto old=m.words;
  setPackedFileModelPointers(Base,m.services());assert(m.trace.size()==51 && m.words[Base+0x1ac]==Table);m.trace.clear();clearPackedFileModelPointers(Base,m.services());assert(m.trace.size()==52 && m.words==old);}
 {auto m=graph();const auto old=m.words;setPackedFileModelPointers(Base,m.services());assert(m.words[Table+0x3c]==Child && m.words[Table+0x7c]==Child && m.words[Child+0x24]==Child+0x24);
  // Root publication completes before captured COUNT and parent traversal.
  assert(m.trace[49].store && m.trace[49].address==Base+0x1ac && !m.trace[50].store && m.trace[50].address==Base+0x148);
  m.trace.clear();clearPackedFileModelPointers(Base,m.services());assert(m.words==old);
  assert(m.trace[4].store && m.trace[4].address==Table+0x3c && !m.trace[5].store && m.trace[5].address==Child+0x24);
  assert(m.trace.back().store && m.trace.back().address==Base+0x1ac);}
 {auto m=graph();m.stored=[&](uint32_t a,uint32_t){if(a==Table+0x3c)m.words[Base+0x148]=0;};
  setPackedFileModelPointers(Base,m.services());assert(m.words[Table+0x7c]==Child);m.stored={};m.words[Base+0x148]=2;
  m.stored=[&](uint32_t a,uint32_t){if(a==Table+0x3c)m.words[Base+0x148]=0xffffffff;};
  clearPackedFileModelPointers(Base,m.services());assert(m.words[Table+0x7c]==0x500);}
 // Set reloads TABLE/CHILD after publishing the old table's parent link.
 {auto m=graph(1);const uint32_t otherTable=Base+0x700,otherChild=Base+0x900;m.words[otherTable+0x3c]=otherChild;
  for(unsigned o=0x24;o<=0x40;o+=4)m.words[otherChild+o]=0;
  m.stored=[&](uint32_t a,uint32_t){if(a==Table+0x3c)m.words[Base+0x1ac]=otherTable;};
  setPackedFileModelPointers(Base,m.services());assert(m.words[Child+0x24]==0x24 && m.words[otherChild+0x24]==otherChild && m.words[Table+0x3c]==Child);}
 // Clear instead retains C, even when the parent store changes the table.
 {auto m=graph(1);setPackedFileModelPointers(Base,m.services());const uint32_t otherTable=Base+0x700;
  m.stored=[&](uint32_t a,uint32_t){if(a==Table+0x3c)m.words[Base+0x1ac]=otherTable;};
  clearPackedFileModelPointers(Base,m.services());assert(m.words[Child+0x24]==0x24 && m.words[Base+0x1ac]==0x700);}
 // Child/root alias changes first root field; Clear must reread it at the end.
 {auto m=graph(1);setPackedFileModelPointers(Base,m.services());const uint32_t alias=Base+0x128;m.words[Table+0x3c]=alias;m.words[Base+0x14c]=alias;
  clearPackedFileModelPointers(Base,m.services());assert(m.words[Base+0x14c]==uint32_t(0-Base));}
 // Root arithmetic wraps; no zero-pointer preservation or null-child shortcut.
 {auto m=graph(0);m.words[Base+0x150]=0xffffffff;setPackedFileModelPointers(Base,m.services());assert(m.words[Base+0x150]==Base-1);}
 {auto m=graph(1);m.words[Table+0x3c]=0;for(unsigned o=0x24;o<=0x40;o+=4)m.words[Base+o]=0;setPackedFileModelPointers(Base,m.services());assert(m.words[Table+0x3c]==Base && m.words[Base+0x24]==Base);}
 // Equality in the child guard skips Set but admits Clear subtraction.
 {auto m=graph(1);m.words[Child+0x24]=Child;setPackedFileModelPointers(Base,m.services());assert(m.words[Child+0x24]==Child);
  for(unsigned o=0x28;o<=0x40;o+=4)m.words[Child+o]=Child;
  clearPackedFileModelPointers(Base,m.services());assert(m.words[Child+0x24]==0 && m.words[Child+0x40]==0);}
}
uint32_t read(const std::vector<uint8_t>& b,uint32_t o){assert(uint64_t(o)+4<=b.size());return uint32_t(b[o])<<24|uint32_t(b[o+1])<<16|uint32_t(b[o+2])<<8|b[o+3];}
void original(const char* root,const char* name){
 const auto path=std::string(root)+"/"+name+".draw-model.bin";FILE* f=std::fopen(path.c_str(),"rb");assert(f);assert(!std::fseek(f,0,SEEK_END));auto length=std::ftell(f);assert(length>0);std::rewind(f);
 std::vector<uint8_t> bytes(size_t(length),0);assert(std::fread(bytes.data(),1,bytes.size(),f)==bytes.size());std::fclose(f);
 // Full payload stays byte-addressable, as in the real guest RAM owner.
 // Avoid allocating one native tree node per word in the strict ARM fixture.
 struct ModelMemory {
  std::vector<uint8_t> bytes;unsigned reads=0,writes=0;
  uint32_t word(uint32_t a){++reads;return read(bytes,a-Base);}
  EventMemoryServices services(){EventMemoryServices m;m.user=this;
   m.word=[](void* p,uint32_t a){return static_cast<ModelMemory*>(p)->word(a);};
   m.storeWord=[](void* p,uint32_t a,uint32_t v){auto& x=*static_cast<ModelMemory*>(p);auto o=a-Base;assert(uint64_t(o)+4<=x.bytes.size());++x.writes;for(unsigned i=0;i<4;++i)x.bytes[o+i]=uint8_t(v>>(24-8*i));};return m;}
 } m{bytes};
 const auto count=read(bytes,0x148),table=read(bytes,0x1ac);assert(count && count<256 && table<bytes.size());
 setPackedFileModelPointers(Base,m.services());
 for(unsigned o=0x14c;o<=0x1ac;o+=4)assert(m.word(Base+o)==Base+read(bytes,o));
 for(unsigned i=0;i<count;++i){const auto relative=read(bytes,table+64*i+0x3c),child=Base+relative;assert(m.word(Base+table+64*i+0x3c)==child);
  for(unsigned o=0x24;o<=0x40;o+=4)assert(m.word(child+o)==child+read(bytes,relative+o));}
 const auto relocated=m.bytes;m.reads=m.writes=0;setPackedFileModelPointers(Base,m.services());assert(m.reads==1 && !m.writes && m.bytes==relocated);
 clearPackedFileModelPointers(Base,m.services());assert(m.bytes==bytes);m.reads=m.writes=0;clearPackedFileModelPointers(Base,m.services());assert(m.reads==1 && !m.writes && m.bytes==bytes);
}
}
void runFileModelRelocationFixture(const char* root){authored();original(root,"field-player-c_pkr");original(root,"field-player-a_mario");}
int main(){runFileModelRelocationFixture("romfs");}
