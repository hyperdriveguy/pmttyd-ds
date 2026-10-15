#include "field_map_npc_history.hpp"
#include "event_address_space.hpp"
#include <array>
#include <cassert>
#include <string>
namespace ttyd {
uint32_t packedMapNpcMapDefinition(uint32_t map,const PackedMapNpcHistoryServices& s){
 for(uint32_t p=0x8030f668;;p+=12){const auto name=s.memory.word(s.memory.user,p);if(!name || !s.compare(s.user,name,map))return p;}
}
std::optional<PackedMapNpcHistoryGate> preparePackedMapNpcHistory(uint32_t field,uint32_t map,const PackedMapNpcHistoryServices& s){
 const auto& m=s.memory;assert(m.word && m.storeWord && s.compare && s.copyString && s.clear);
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};
 const auto definition=packedMapNpcMapDefinition(map,s),head=field+0x4c;
 unsigned index=0;
 for(;index<64;++index){const auto slot=head+20*index;
  if(!s.compare(s.user,slot,0x804207a8)){s.clear(s.user,slot,20);s.copyString(s.user,slot,map);break;}
  if(!s.compare(s.user,slot,map))break;
 }
 if(index==64)return {}; // Native bound; no fabricated eviction or tail record.
 if(index){std::array<uint32_t,5> captured{};for(unsigned n=0;n<5;++n)captured[n]=word(head+20*index+4*n);
  for(unsigned n=index;n;--n){const auto from=head+20*(n-1),to=from+20;
   auto a=word(from),b=word(from+4);store(to,a);store(to+4,b);
   a=word(from+8);b=word(from+12);store(to+8,a);store(to+12,b);store(to+16,word(from+16));
  }
  for(unsigned n=0;n<5;++n)store(head+4*n,captured[n]);
 }
 if(word(definition+8)&1){const auto current=packedMapNpcMapDefinition(map,s),previous=packedMapNpcMapDefinition(field+0x60,s);
  const auto currentGroup=word(current+4),previousGroup=word(previous+4);
  if(currentGroup!=previousGroup){s.clear(s.user,head,0x500);s.copyString(s.user,head,map);}
  else {const auto flags=word(definition+8);if(!(flags&2) && (flags&4))s.clear(s.user,field+0x74,0x4d8);}
 }
 return PackedMapNpcHistoryGate{head,(word(definition+8)&1)!=0};
}
uint32_t packedMapNpcOrdinalMask(uint32_t ordinal){const auto shift=uint8_t(ordinal)&63;return shift<32?1u<<shift:0;}
bool skipPackedMapNpc(const PackedMapNpcHistoryGate& gate,uint32_t ordinal,const EventMemoryServices& m){return gate.enabled && (m.word(m.user,gate.head+16)&packedMapNpcOrdinalMask(ordinal));}
namespace {
bool text(EventAddressSpace& m,uint32_t address,std::string& out,unsigned limit){
 out.clear();for(unsigned n=0;n<limit;++n){uint8_t b=0;if(uint64_t(address)+n>UINT32_MAX || !m.readByte(address+n,b))return false;if(!b)return true;out.push_back(char(b));}return false;
}
PackedMapNpcHistoryServices services(EventAddressSpace& memory){
 PackedMapNpcHistoryServices s;s.user=s.memory.user=&memory;
 s.memory.word=[](void* p,uint32_t a){uint32_t v=0;const bool ok=static_cast<EventAddressSpace*>(p)->readWord(a,v);assert(ok);return v;};
 s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<EventAddressSpace*>(p)->writeWord(a,v);assert(ok);};
 s.compare=[](void* p,uint32_t a,uint32_t b)->int32_t{std::string x,y;auto& m=*static_cast<EventAddressSpace*>(p);const bool ok=text(m,a,x,256) && text(m,b,y,256);assert(ok);return x<y?-1:x>y?1:0;};
 s.copyString=[](void* p,uint32_t a,uint32_t b){auto& m=*static_cast<EventAddressSpace*>(p);for(;;++a,++b){uint8_t v=0;bool ok=m.readByte(b,v);assert(ok);ok=m.writeByte(a,v);assert(ok);if(!v)break;}};
 s.clear=[](void* p,uint32_t a,uint32_t size){for(unsigned n=0;n<size;++n){const bool ok=static_cast<EventAddressSpace*>(p)->writeByte(a+n,0);assert(ok);}};return s;
}
}
std::optional<PackedMapNpcHistoryGate> prepareFieldMapNpcHistory(EventAddressSpace& m,uint32_t field,uint32_t map){
 // All field bytes are retained writable RAM under this adapter's contract.
 // Reject overlap with input strings/table: stores must not invalidate admission.
 if(uint64_t(field)+0x580>0x100000000ULL)return {};
 auto overlaps=[&](uint32_t a,uint32_t size){return uint64_t(a)<uint64_t(field)+0x580 && uint64_t(field)<uint64_t(a)+size;};
 std::string name,empty;if(!text(m,map,name,16) || overlaps(map,unsigned(name.size()+1)) || !text(m,0x804207a8,empty,1))return {};
 uint32_t value=0;for(unsigned n=0;n<0x580;n+=4)if(!m.readWord(field+n,value))return {};
 bool terminated=false;
 for(unsigned n=0;n<=198;++n){const auto row=0x8030f668+12*n;uint32_t pointer=0;
  if(overlaps(row,12) || !m.readWord(row,pointer) || !m.readWord(row+4,value) || !m.readWord(row+8,value))return {};
  if(!pointer){terminated=true;break;}std::string label;if(!text(m,pointer,label,256) || overlaps(pointer,unsigned(label.size()+1)))return {};
 }
 if(!terminated || overlaps(0x804207a8,1))return {};
 for(unsigned n=0;n<64;++n){std::string label;if(!text(m,field+0x4c+20*n,label,16))return {};if(label.empty() || label==name)break;if(n==63)return {};}
 // Group comparison can read the second slot even when selecting head0.
 std::string previous;if(!text(m,field+0x60,previous,16))return {};
 return preparePackedMapNpcHistory(field,map,services(m));
}
std::optional<bool> skipFieldMapNpc(EventAddressSpace& m,const PackedMapNpcHistoryGate& gate,uint32_t ordinal){
 if(!gate.enabled)return false;
 uint32_t mask=0;if(!m.readWord(gate.head+16,mask))return {};return (mask&packedMapNpcOrdinalMask(ordinal))!=0;
}
}
