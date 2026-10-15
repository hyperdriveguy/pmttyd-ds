#include "field_npc_creation_event.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Entry=0x800536e4,Tribe=0x8004e5c0;
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
bool same(const FieldNpcCreationEventServices& a,const FieldNpcCreationEventServices& b){return a.user==b.user && a.word==b.word && a.half==b.half && a.value==b.value && a.resolve==b.resolve && a.tribe==b.tribe && a.async==b.async && a.enter==b.enter && a.copy==b.copy && a.animation==b.animation && a.store==b.store && a.storeHalf==b.storeHalf;}
}
bool fieldNpcCreationEventFunction(uint32_t f){return f==Entry || f==Tribe;}
std::optional<int32_t> fieldNpcCreationEvent(uint32_t function,EventEntry& e,const FieldNpcCreationEventServices& s,FieldNpcCreationEventCursor& c){
 if(!fieldNpcCreationEventFunction(function) || !s.word || !s.value || !s.store || !s.storeHalf)return {};
 if(c.phase && (c.event!=&e || c.id!=e.id || c.function!=function || !same(c.owner,s)))return {};
 if(!c.phase){c.owner=s;c.event=&e;c.id=e.id;c.function=function;c.args=e.arguments;c.phase=1;}
 auto finish=[&](int32_t v)->std::optional<int32_t>{c={};return v;};
 for(;;)switch(c.phase){
 case 1:if(!s.word(s.user,c.args,c.raw))return {};c.phase=2;break;
 case 2:{int32_t v=0;if(!s.value(s.user,e,signedWord(c.raw),v))return {};c.name=uint32_t(v);c.phase=3;break;}
 case 3:if(!s.word(s.user,c.args+4,c.raw))return {};c.phase=4;break;
 case 4:{int32_t v=0;if(!s.value(s.user,e,signedWord(c.raw),v))return {};c.modelOrTribe=uint32_t(v);c.phase=function==Entry?5:10;break;}
 case 5:if(!s.word(s.user,0x80415fd0,c.gp))return {};c.phase=6;break;
 case 6:if(!s.word(s.user,c.gp+0x14,c.mode))return {};c.mode=c.mode?1:0;c.phase=7;break;
 case 7:{if(!s.async)return {};auto r=s.async(s.user,c.modelOrTribe,c.mode);if(!r)return {};if(!*r)return finish(0);c.phase=8;break;}
 case 8:if(!s.enter || !s.enter(s.user,c.name,c.modelOrTribe))return {};return finish(2);
 case 10:{if(!s.resolve)return {};auto p=s.resolve(s.user,e,c.name);if(!p)return {};c.actor=*p;c.phase=11;break;}
 case 11:{if(!s.tribe)return {};auto d=s.tribe(s.user,c.modelOrTribe);if(!d)return {};c.descriptor=*d;c.phase=12;break;}
 case 12:s.store(s.user,c.actor+0x28,c.descriptor);c.phase=13;break;
 case 13:if(!s.word(s.user,c.descriptor+0x28,c.raw))return {};c.phase=14;break;
 case 14:s.store(s.user,c.actor+0x14c,c.raw);c.phase=15;break;
 case 15:if(!s.word(s.user,c.descriptor+0x2c,c.raw))return {};c.phase=16;break;
 case 16:s.store(s.user,c.actor+0x150,c.raw);c.phase=17;break;
 case 17:if(!s.word(s.user,c.descriptor+0x10,c.pointer))return {};c.phase=c.pointer?18:19;break;
 case 18:if(!s.copy || !s.copy(s.user,c.actor+0x4c,c.pointer))return {};c.phase=19;break;
 case 19:if(!s.word(s.user,c.descriptor+0x14,c.pointer))return {};c.phase=c.pointer?20:21;break;
 case 20:if(!s.copy || !s.copy(s.user,c.actor+0x6c,c.pointer))return {};c.phase=21;break;
 case 21:if(!s.word(s.user,c.descriptor+8,c.pointer))return {};c.phase=c.pointer?22:25;break;
 case 22:if(!s.copy || !s.copy(s.user,c.actor+0x2c,c.pointer))return {};c.phase=23;break;
 case 23:if(!s.word(s.user,c.actor+0x104,c.handle))return {};c.phase=24;break;
 case 24:if(!s.animation || !s.animation(s.user,c.handle,c.actor+0x2c,1))return {};c.phase=25;break;
 case 25:if(!s.word(s.user,c.descriptor+0x40,c.metadata))return {};c.phase=26;break;
 case 26:if(!s.word(s.user,0x8041fd84,c.zero))return {};c.phase=27;break;
 case 27:s.store(s.user,c.actor+0x1e0,c.metadata);c.phase=28;break;
 case 28:if(!s.word(s.user,c.descriptor+0x44,c.raw))return {};c.phase=29;break;
 case 29:s.store(s.user,c.actor+0x1e4,c.raw);c.phase=30;break;
 case 30:if(!s.word(s.user,c.descriptor+0x4c,c.raw))return {};c.phase=31;break;
 case 31:s.store(s.user,c.actor+0x1ec,c.raw);c.phase=32;break;
 case 32:if(!s.word(s.user,c.descriptor+0x50,c.raw))return {};c.phase=33;break;
 case 33:s.store(s.user,c.actor+0x1f0,c.raw);c.phase=34;break;
 case 34:if(!s.half || !s.half(s.user,c.descriptor+0x48,c.half))return {};c.phase=35;break;
 case 35:s.storeHalf(s.user,c.actor+0x1e8,c.half);c.phase=36;break;
 case 36:s.storeHalf(s.user,c.actor+0x1d8,1);c.phase=37;break;
 case 37:s.store(s.user,c.actor+0x1dc,c.zero);return finish(1);
 default:return {};
 }
}
}
