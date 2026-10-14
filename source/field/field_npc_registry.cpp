#include "field_npc_registry.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t Registry=0x8041e8b4,GpPointer=0x80415fd0;
int32_t signedWord(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
uint32_t selectedRegistry(const EventMemoryServices& m){const auto gp=m.word(m.user,GpPointer);return Registry+(m.word(m.user,gp+0x14)?0x14u:0u);}
}
void initializePackedNpcRegistry(const PackedNpcRegistryServices& s){
 const auto& m=s.memory;assert(m.word && m.storeWord && m.storeHalf && s.allocate && s.clear);
 m.storeWord(m.user,Registry+4,36);
 auto array=s.allocate(s.user,0,36*0x340);auto capacity=m.word(m.user,Registry+4);
 m.storeWord(m.user,Registry+0xc,array);s.clear(s.user,array,capacity*0x340u);
 capacity=m.word(m.user,Registry+4);const auto auxiliary=s.allocate(s.user,0,capacity*0x94u);
 m.storeWord(m.user,0x8041e8b0,auxiliary);m.storeWord(m.user,Registry+0x18,16);
 array=s.allocate(s.user,0,16*0x340);capacity=m.word(m.user,Registry+0x18);
 m.storeWord(m.user,Registry+0x20,array);s.clear(s.user,array,capacity*0x340u);
 const auto field=s.allocate(s.user,0,0x580);
 auto gp=m.word(m.user,GpPointer);m.storeWord(m.user,gp+0x168,field);
 gp=m.word(m.user,GpPointer);s.clear(s.user,m.word(m.user,gp+0x168),0x580);
 m.storeHalf(m.user,field,0);m.storeWord(m.user,0x8041e8ac,0);
}
uint32_t packedNpcNameToPointer(uint32_t name,PackedNpcLookup lookup,const PackedNpcRegistryServices& s){
 const auto& m=s.memory;assert(m.word && s.compare);const auto registry=selectedRegistry(m);
 const auto count=signedWord(m.word(m.user,registry+4));auto actor=m.word(m.user,registry+0xc);int32_t index=0;
 for(;index<count;++index,actor+=0x340)if((m.word(m.user,actor)&1) && s.compare(s.user,actor+8,name)==0)break;
 return lookup==PackedNpcLookup::NoAssert && index>=count?0:actor;
}
int32_t enterPackedNpc(uint32_t name,uint32_t model,const PackedNpcRegistryServices& s){
 const auto& m=s.memory;assert(m.word && m.storeWord && m.storeByte && s.clear && s.compare && s.copyString && s.poseEntry && s.paperOn && s.heightBits && s.radiusBits && s.vivianType && s.lightFlagOn);
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};
 const auto registry=selectedRegistry(m);const auto count=signedWord(word(registry+4));auto actor=word(registry+0xc);
 for(int32_t i=0;i<count;++i,actor+=0x340)if((word(actor)&1) && s.compare(s.user,actor+8,name)==0)break;
 actor=word(registry+0xc);int32_t index=0;
 for(;index<count;++index,actor+=0x340)if(!(word(actor)&1))break;
 assert(index<count); // Native pre-admission excludes the source overrun domain.
 s.clear(s.user,actor,0x340);store(actor,3);s.copyString(s.user,actor+8,name);
 auto selector=[&]{return word(word(GpPointer)+0x14)?1:0;};
 store(actor+0x104,uint32_t(s.poseEntry(s.user,model,selector())));
 if(signedWord(word(actor+0x104))==-2)store(actor+0x104,uint32_t(s.poseEntry(s.user,0x8041fd6c,selector())));
 s.paperOn(s.user,signedWord(word(actor+0x104)));
 auto x=word(0x802c11f8),y=word(0x802c11fc);store(actor+0x8c,x);store(actor+0x90,y);store(actor+0x94,word(0x802c1200));
 x=word(actor+0x8c);y=word(actor+0x90);store(actor+0x98,x);store(actor+0x9c,y);store(actor+0xa0,word(actor+0x94));
 store(actor+0x14c,s.heightBits(s.user,signedWord(word(actor+0x104))));
 store(actor+0x150,s.radiusBits(s.user,signedWord(word(actor+0x104))));
 const auto one=word(0x8041fc94);x=word(0x802c1204);y=word(0x802c1208);const auto zero=word(0x8041fc7c);
 store(actor+0xe0,x);store(actor+0xe4,y);store(actor+0xe8,word(0x802c120c));
 store(actor+0x154,one);store(actor+0x108,0);store(actor+0x10c,0x8041fd74);
 const auto gp=word(GpPointer);x=word(gp+0x38);y=word(gp+0x3c);store(actor+0x19c,y);store(actor+0x198,x);
 store(actor+0x18c,0);store(actor+0x188,0);store(actor+0x1cc,one);store(actor+0x1d0,zero);store(actor+0x1c8,one);
 store(actor+0x110,0xffffffff);store(actor+0x114,word(0x8041fc6c));store(actor+0x1f4,4);store(actor+0x2f8,0);m.storeByte(m.user,actor+0x317,1);
 if(s.vivianType(s.user,signedWord(word(actor+0x104))))store(actor,word(actor)|0x04000000);
 if(s.compare(s.user,model,0x802c1468)==0)store(actor,word(actor)|0x00800000);
 s.lightFlagOn(s.user,signedWord(word(actor+0x104)),2);store(registry,word(registry)+1);
 return index;
}
}
