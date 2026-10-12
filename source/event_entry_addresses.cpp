#include "event_entry_addresses.hpp"
#include <cstring>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t floatWord(const float& f) {uint32_t v;std::memcpy(&v,&f,4);return v;}
void floatStore(float& f,uint32_t v) {std::memcpy(&f,&v,4);}
EventEntry*& link(EventEntry& e,unsigned i) {return i==0?e.waitingParent:i==1?e.waitingChild:e.brotherOwner;}
bool rawIndex(uint32_t off,unsigned& index) {
    if(off>=0x10 && off<0x14)index=off-0x10;
    else if(off>=0x84 && off<0x98)index=4+off-0x84;
    else if(off>=0x174 && off<0x178)index=24+off-0x174;
    else if(off>=0x194 && off<0x1a0)index=28+off-0x194;
    else if(off>=0x1ac && off<0x1b0)index=40+off-0x1ac;
    else return false;
    return true;
}
uint8_t* byteField(EventEntry& e,uint32_t off) {
    switch(off) {case 8:return &e.flags;case 9:return &e.argumentCount;case 10:return &e.opcode;case 11:return &e.priority;case 12:return &e.type;case 13:return &e.initialized;case 14:return &e.loopIndex;case 15:return &e.switchIndex;default:break;}
    if(off>=0x1c && off<0x2c)return &e.labelIds[off-0x1c];
    if(off>=0x128 && off<0x130)return &e.switchStates[off-0x128];
    unsigned index;if(rawIndex(off,index))return &e.rawBytes[index];return nullptr;
}
}
EventEntryAddresses::Position EventEntryAddresses::find(uint32_t address) const {
    for(const auto& r:regions_)if(address>=r.base && uint64_t(address)<uint64_t(r.base)+uint64_t(r.count)*stride) {
        const uint32_t delta=address-r.base;return {r.entries+delta/stride,delta%stride};
    }
    return {};
}
bool EventEntryAddresses::registerEntries(uint32_t base,EventEntry* entries,uint32_t count) {
    const uint64_t end=uint64_t(base)+uint64_t(count)*stride;if(!base || !entries || !count || end>0x100000000ULL)return false;
    for(const auto& r:regions_) {
        if(uint64_t(base)<uint64_t(r.base)+uint64_t(r.count)*stride && uint64_t(r.base)<end)return false;
        for(uint32_t i=0;i<count;++i)for(uint32_t j=0;j<r.count;++j)if(entries+i==r.entries+j)return false;
    }
    regions_.push_back({base,count,entries});
    for(const auto& r:regions_)for(uint32_t i=0;i<r.count;++i) {
        auto& e=r.entries[i];for(unsigned j=0;j<3;++j)if(e.unresolvedLinkMask&(1u<<j))if(auto* mapped=entryAt(e.unresolvedLinkWords[j])) {link(e,j)=mapped;e.unresolvedLinkMask&=~uint8_t(1u<<j);}
    }
    return true;
}
bool EventEntryAddresses::addressOf(const EventEntry* e,uint32_t& address) const {
    if(!e) {address=0;return true;}
    for(const auto& r:regions_)for(uint32_t i=0;i<r.count;++i)if(e==r.entries+i) {address=r.base+i*stride;return true;}
    return false;
}
EventEntry* EventEntryAddresses::entryAt(uint32_t address) const {auto p=find(address);return p.offset==0?p.entry:nullptr;}
bool EventEntryAddresses::linksResolved(const EventEntry& e) const {
    for(unsigned i=0;i<3;++i) {auto* p=i==0?e.waitingParent:i==1?e.waitingChild:e.brotherOwner;uint32_t word;if(p) {if(!addressOf(p,word))return false;}else if(e.unresolvedLinkMask&(1u<<i))return false;}
    return true;
}
bool EventEntryAddresses::readFieldWord(EventEntry& e,uint32_t off,uint32_t& v) const {
    if(off>=0x2c && off<0x6c)v=e.labelAddresses[(off-0x2c)/4];
    else if(off>=0x6c && off<0x78) {const unsigned i=(off-0x6c)/4;if(link(e,i))return addressOf(link(e,i),v);v=e.unresolvedLinkMask&(1u<<i)?e.unresolvedLinkWords[i]:0;}
    else if(off>=0x9c && off<0xdc)v=static_cast<uint32_t>(e.locals[(off-0x9c)/4]);
    else if(off>=0xdc && off<0xe8)v=e.localFlags[(off-0xdc)/4];
    else if(off>=0xe8 && off<0x108)v=e.loopBodies[(off-0xe8)/4];
    else if(off>=0x108 && off<0x128)v=static_cast<uint32_t>(e.loopCounts[(off-0x108)/4]);
    else if(off>=0x130 && off<0x150)v=static_cast<uint32_t>(e.switchValues[(off-0x130)/4]);
    else if(off>=0x178 && off<0x194)v=e.messageFields[(off-0x178)/4];
    else switch(off) {
    case 0:v=static_cast<uint32_t>(e.lifetime>>32);break;case 4:v=static_cast<uint32_t>(e.lifetime);break;
    case 0x14:v=e.nextCommand;break;case 0x18:v=e.arguments;break;case 0x78:v=static_cast<uint32_t>(e.waitCount);break;
    case 0x7c:v=static_cast<uint32_t>(e.waitStart>>32);break;case 0x80:v=static_cast<uint32_t>(e.waitStart);break;
    case 0x98:v=e.cachedFunction;break;case 0x150:v=e.field150;break;case 0x154:v=e.userWork;break;case 0x158:v=e.userFlags;break;
    case 0x15c:v=e.id;break;case 0x160:v=static_cast<uint32_t>(e.owner);break;case 0x164:v=floatWord(e.timescale);break;case 0x168:v=floatWord(e.credit);break;
    case 0x16c:v=static_cast<uint32_t>(e.field16c);break;case 0x170:v=e.field170;break;case 0x1a0:v=e.restartScript;break;case 0x1a4:v=e.field1a4;break;case 0x1a8:v=e.currentCommand;break;
    default:return false;
    }
    return true;
}
bool EventEntryAddresses::writeFieldWord(EventEntry& e,uint32_t off,uint32_t v) {
    if(off>=0x2c && off<0x6c)e.labelAddresses[(off-0x2c)/4]=v;
    else if(off>=0x6c && off<0x78) {
        const unsigned i=(off-0x6c)/4;auto* p=entryAt(v);link(e,i)=p;e.unresolvedLinkWords[i]=v;
        if(v!=0 && !p)e.unresolvedLinkMask|=uint8_t(1u<<i);else e.unresolvedLinkMask&=~uint8_t(1u<<i);
    } else if(off>=0x9c && off<0xdc)e.locals[(off-0x9c)/4]=sw(v);
    else if(off>=0xdc && off<0xe8)e.localFlags[(off-0xdc)/4]=v;
    else if(off>=0xe8 && off<0x108)e.loopBodies[(off-0xe8)/4]=v;
    else if(off>=0x108 && off<0x128)e.loopCounts[(off-0x108)/4]=sw(v);
    else if(off>=0x130 && off<0x150)e.switchValues[(off-0x130)/4]=sw(v);
    else if(off>=0x178 && off<0x194)e.messageFields[(off-0x178)/4]=v;
    else switch(off) {
    case 0:e.lifetime=(uint64_t(v)<<32)|(e.lifetime&0xffffffffu);break;case 4:e.lifetime=(e.lifetime&0xffffffff00000000ULL)|v;break;
    case 0x14:e.nextCommand=v;break;case 0x18:e.arguments=v;break;case 0x78:e.waitCount=sw(v);break;
    case 0x7c:e.waitStart=(uint64_t(v)<<32)|(e.waitStart&0xffffffffu);break;case 0x80:e.waitStart=(e.waitStart&0xffffffff00000000ULL)|v;break;
    case 0x98:e.cachedFunction=v;break;case 0x150:e.field150=v;break;case 0x154:e.userWork=v;break;case 0x158:e.userFlags=v;break;
    case 0x15c:e.id=v;break;case 0x160:e.owner=sw(v);break;case 0x164:floatStore(e.timescale,v);break;case 0x168:floatStore(e.credit,v);break;
    case 0x16c:e.field16c=sw(v);break;case 0x170:e.field170=v;break;case 0x1a0:e.restartScript=v;break;case 0x1a4:e.field1a4=v;break;case 0x1a8:e.currentCommand=v;break;
    default:return false;
    }
    return true;
}
bool EventEntryAddresses::readByte(uint32_t address,uint8_t& v) const {
    auto p=find(address);if(!p.entry)return false;if(auto* b=byteField(*p.entry,p.offset)) {v=*b;return true;}
    uint32_t word;if(!readFieldWord(*p.entry,p.offset&~3u,word))return false;v=uint8_t(word>>(24-8*(p.offset&3)));return true;
}
bool EventEntryAddresses::writeByte(uint32_t address,uint8_t v) {
    auto p=find(address);if(!p.entry)return false;if(auto* b=byteField(*p.entry,p.offset)) {*b=v;return true;}
    uint32_t word;if(!readFieldWord(*p.entry,p.offset&~3u,word))return false;const unsigned shift=24-8*(p.offset&3);
    return writeFieldWord(*p.entry,p.offset&~3u,(word&~(0xffu<<shift))|(uint32_t(v)<<shift));
}
bool EventEntryAddresses::readWord(uint32_t address,uint32_t& v) const {
    if(uint64_t(address)+4>0x100000000ULL)return false;
    auto p=find(address);
    if(p.entry && (p.offset&3)==0 && readFieldWord(*p.entry,p.offset,v))return true;
    uint32_t result=0;
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(address+i,b))return false;result=result<<8|b;}
    v=result;return true;
}
bool EventEntryAddresses::writeWord(uint32_t address,uint32_t v) {
    if(uint64_t(address)+4>0x100000000ULL)return false;
    auto p=find(address);
    if(p.entry && (p.offset&3)==0 && writeFieldWord(*p.entry,p.offset,v))return true;
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(address+i,b))return false;}
    for(unsigned i=0;i<4;++i)if(!writeByte(address+i,uint8_t(v>>(24-8*i))))return false;
    return true;
}
}
