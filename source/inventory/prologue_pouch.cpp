#include "prologue_pouch.hpp"
#include "battle_packed_pouch.hpp"
#include "message_assets.hpp"
#include "key_item_inventory.hpp"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <string>
namespace ttyd {
namespace {
std::string originalYoshiName;bool assetsReady=false;
constexpr uint32_t Address=0x85000000;
struct Provider {ProloguePouch pouch;const char* name;bool allocated=false;uint32_t published=0;};
}
int16_t ProloguePouch::half(unsigned at) const {
    assert(at+1<bytes.size());const auto value=uint16_t(uint16_t(bytes[at])<<8|bytes[at+1]);return value<0x8000?int16_t(value):int16_t(int32_t(value)-65536);
}
void ProloguePouch::half(unsigned at,int16_t value){assert(at+1<bytes.size());const auto bits=uint16_t(value);bytes[at]=bits>>8;bytes[at+1]=bits;}
int32_t ProloguePouch::addCoins(int32_t amount) {
    const uint16_t bits=uint16_t(uint32_t(int32_t(half(0x78)))+uint32_t(amount));
    half(0x78,int16_t(bits));
    const int32_t stored=half(0x78);
    if(stored<0)half(0x78,0);
    else if(stored>999)half(0x78,999);
    return half(0x78);
}
int32_t ProloguePouch::keyItemCount(int32_t item) const {
    EventMemoryServices m;m.user=const_cast<ProloguePouch*>(this);
    m.half=[](void* p,uint32_t at){return uint16_t(static_cast<const ProloguePouch*>(p)->half(at));};
    return packedKeyItemCount(0,item,m);
}
int32_t ProloguePouch::removeKeyItem(int32_t item) {
    EventMemoryServices m;m.user=this;
    m.half=[](void* p,uint32_t at){return uint16_t(static_cast<ProloguePouch*>(p)->half(at));};
    m.storeHalf=[](void* p,uint32_t at,uint16_t value){static_cast<ProloguePouch*>(p)->half(at,int16_t(value));};
    return removePackedKeyItem(0,item,m);
}
int32_t ProloguePouch::insertKeyItem(int32_t item) {
    EventMemoryServices m;m.user=this;
    m.half=[](void* p,uint32_t at){return uint16_t(static_cast<ProloguePouch*>(p)->half(at));};
    m.storeHalf=[](void* p,uint32_t at,uint16_t value){static_cast<ProloguePouch*>(p)->half(at,int16_t(value));};
    return insertPackedKeyItem(0,item,m);
}
ProloguePouch initializeProloguePouch(const char* name){
    assert(name);Provider provider{{},name};PackedPouchInitializationServices s;s.user=&provider;s.memory.user=&provider;
    s.allocate=[](void* p,int32_t heap,uint32_t size){auto& v=*static_cast<Provider*>(p);assert(!v.allocated && heap==0 && size==v.pouch.bytes.size());v.allocated=true;return Address;};
    s.message=[](void* p,const char* key){assert(!std::strcmp(key,"name_party3"));return static_cast<Provider*>(p)->name;};
    s.memory.storeByte=[](void* p,uint32_t at,uint8_t value){auto& v=*static_cast<Provider*>(p);assert(at>=Address && at-Address<v.pouch.bytes.size());v.pouch.bytes[at-Address]=value;};
    s.memory.storeHalf=[](void* p,uint32_t at,uint16_t value){auto& v=*static_cast<Provider*>(p);assert(at>=Address);v.pouch.half(at-Address,int16_t(value));};
    s.memory.storeWord=[](void* p,uint32_t at,uint32_t value){auto& v=*static_cast<Provider*>(p);if(at==0x8041eb00){v.published=value;return;}assert(at>=Address && at-Address+3<v.pouch.bytes.size());for(unsigned i=0;i<4;++i)v.pouch.bytes[at-Address+i]=uint8_t(value>>(24-8*i));};
    const auto initialized=initializePackedPouch(s);
    if(initialized!=Address || provider.published!=Address)std::abort();
    auto& pouch=provider.pouch;
    for(int16_t item:{6,0x32,9}){
        // These source scalar writes precede the source key-item insertion.
        if(item==6)pouch.bytes[0x98]=1;else if(item==9)pouch.bytes[0x99]=1;
        int slot=120;while(slot>=0 && pouch.half(0xa0+2*unsigned(slot))!=0)--slot;assert(slot>=0);
        for(;slot>0;--slot)pouch.half(0xa0+2*unsigned(slot),pouch.half(0xa0+2*unsigned(slot-1)));
        pouch.half(0xa0,item);
    }
    pouch.half(0x78,100);
    // Only the source fresh zero-equipment domain is admitted here. This runs
    // once at explicit new-game creation, never as a general revision/heal.
    for(unsigned i=0;i<200;++i)assert(pouch.half(0x38a+2*i)==0);
    pouch.half(0x72,pouch.half(0x8e));pouch.half(0x76,pouch.half(0x90));
    if(pouch.half(0x70)>pouch.half(0x72))pouch.half(0x70,pouch.half(0x72));
    if(pouch.half(0x74)>pouch.half(0x76))pouch.half(0x74,pouch.half(0x76));
    pouch.half(0x92,pouch.half(0x94));return pouch;
}
bool loadProloguePouchAssets(const char* root){
    if(!root)return false;
    MessageResource next;if(!next.load((std::string(root)+"/messages/global.txt").c_str()))return false;
    const auto name=next.find("name_party3");if(!name || std::strlen(name)>=20)return false;
    // Retain only the original localized name after indexing the global bank.
    // Field ownership needs no second resident copy of the complete resource.
    originalYoshiName=name;assetsReady=true;return true;
}
std::shared_ptr<ProloguePouch> createProloguePouch(){return assetsReady?std::make_shared<ProloguePouch>(initializeProloguePouch(originalYoshiName.c_str())):nullptr;}
}
