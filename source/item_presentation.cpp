#include "item_presentation.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <utility>
namespace ttyd {
namespace {
ItemPresentationCatalog catalog;
uint16_t half(const std::vector<uint8_t>& b,size_t p){return uint16_t(b[p])|uint16_t(b[p+1])<<8;}
uint32_t word(const std::vector<uint8_t>& b,size_t p){return uint32_t(half(b,p))|uint32_t(half(b,p+2))<<16;}
bool key(const std::vector<uint8_t>& b,size_t& p,uint16_t length,uint32_t address,std::string& out) {
    if(!address)return length==0;
    if(!length || length>256 || length>b.size()-p || b[p+length-1]!=0)return false;
    // Exported source keys are ASCII and contain exactly one trailing NUL.
    for(size_t i=0;i+1<length;++i)if(b[p+i]==0 || b[p+i]>127)return false;
    out.assign(reinterpret_cast<const char*>(b.data()+p),length-1);p+=length;return true;
}
}
bool ItemPresentationCatalog::assign(const std::vector<uint8_t>& b) {
    if(b.size()<20 || std::memcmp(b.data(),"PMITEM01",8) || word(b,8)!=340 ||
       word(b,12)!=0x803108a8 || word(b,16)!=40)return false;
    std::vector<ItemPresentation> next;next.reserve(340);size_t p=20;
    for(unsigned i=0;i<340;++i) {
        if(b.size()-p<16)return false;
        ItemPresentation item;item.nameAddress=word(b,p);item.descriptionAddress=word(b,p+4);
        item.icon=half(b,p+8);const auto nl=half(b,p+10),dl=half(b,p+12);
        if(half(b,p+14)!=0)return false;
        p+=16;
        if(!key(b,p,nl,item.nameAddress,item.nameKey) || !key(b,p,dl,item.descriptionAddress,item.descriptionKey))return false;
        next.push_back(std::move(item));
    }
    if(p!=b.size())return false;
    items_=std::move(next);return true;
}
bool ItemPresentationCatalog::load(const char* path) {
    if(!path)return false;
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    if(std::fseek(f,0,SEEK_END)!=0){std::fclose(f);return false;}
    const long length=std::ftell(f);
    // Native pack safety bound; not an original allocator or ID-domain rule.
    if(length<20 || length>200000 || std::fseek(f,0,SEEK_SET)!=0){std::fclose(f);return false;}
    std::vector<uint8_t> bytes(static_cast<size_t>(length));
    const auto read=std::fread(bytes.data(),1,bytes.size(),f);std::fclose(f);
    return read==bytes.size() && assign(bytes);
}
const ItemPresentation* ItemPresentationCatalog::find(uint32_t id) const {return id<items_.size()?&items_[id]:nullptr;}
bool loadItemPresentations(const char* path){return catalog.load(path);}
const ItemPresentation* itemPresentation(uint32_t id){return catalog.find(id);}
}
