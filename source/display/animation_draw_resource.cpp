#include "animation_draw_resource.hpp"
#include <cstring>
#include <cassert>
namespace ttyd {
uint32_t AnimationDrawResource::word(uint32_t o) const{
    assert(span(o,1,4));uint32_t v;std::memcpy(&v,data_.data()+o,4);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(v); // ARMv6 REV; no relocated host pointers.
#else
    return v;
#endif
}
float AnimationDrawResource::real(uint32_t o) const{auto b=word(o);float v;std::memcpy(&v,&b,4);return v;}
bool AnimationDrawResource::span(uint32_t o,uint32_t n,uint32_t stride) const{return o<=data_.size() && (!n || (stride && n<=(data_.size()-o)/stride));}
// MODEL fields are BE file-relative offsets. Group stride 88; tree links/shape
// at+40/+44/+48(hex), visibility/node/joint at+4c/+50/+54. -1 terminates links.
AnimationDrawGroup AnimationDrawResource::group(uint32_t i) const{
    const auto o=word(0x1a8)+88*i;
    return {int32_t(word(o+0x40)),int32_t(word(o+0x44)),int32_t(word(o+0x48)),word(o+0x4c),word(o+0x50),word(o+0x54)};
}
std::string AnimationDrawResource::descriptor(uint32_t i) const{
    assert(i<word(0x148));const auto o=word(0x1ac)+64*i;
    const auto* first=reinterpret_cast<const char*>(data_.data()+o);
    const auto* end=static_cast<const char*>(std::memchr(first,0,60));return end?std::string(first,end):std::string{};
}
std::shared_ptr<const AnimationDrawResource> AnimationDrawResource::load(const char* path){
    FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return {};
    auto r=std::make_shared<AnimationDrawResource>();const bool ok=r->read(f);std::fclose(f);return ok?r:nullptr;
}
// read resets this object and may leave partial bytes on failure; callers must
// honor false. load instead returns no shared owner on failed admission.
// Model/texture names occupy64-byte fields at 4/44(hex); minimum header 1b0.
bool AnimationDrawResource::read(FILE* f){
    data_.clear();model_.clear();texture_.clear();
    if(!f || std::fseek(f,0,SEEK_END))return false;
    const auto length=std::ftell(f);
    // Native import cap, not an original MODEL guard.
    if(length<0x1b0 || length>16*1024*1024 || std::fseek(f,0,SEEK_SET))return false;
    data_.resize(size_t(length));if(std::fread(data_.data(),data_.size(),1,f)!=1)return false;
    auto name=[&](uint32_t o,std::string& out){const auto* first=reinterpret_cast<const char*>(data_.data()+o);const auto* end=static_cast<const char*>(std::memchr(first,0,64));if(!end || end==first)return false;for(auto p=first;p!=end;++p)if(uint8_t(*p)>127)return false;out.assign(first,end);return true;};
    if(!name(4,model_) || !name(0x44,texture_))return false;
    // Rows:{count-field, pointer-field, record stride}, all offsets hex below.
    // e8 shapes 168; ec runs 8; f0/f8 positions/normals12; f4/fc indices 4;
    // 100 colors 4/104 indices 4; 128 UVs8; 12c surfaces24; 130 tex slots 8;
    // 134 tex descriptors64; 138 draws 108; 13c visibility1; 140 node floats4;
    // 144 groups 88; 148 pose directory 64. Eight UV index streams follow108/16c.
    constexpr uint32_t tables[][3]{{0xe8,0x14c,168},{0xec,0x150,8},{0xf0,0x154,12},{0xf4,0x158,4},{0xf8,0x15c,12},{0xfc,0x160,4},{0x100,0x164,4},{0x104,0x168,4},{0x128,0x18c,8},{0x12c,0x190,24},{0x130,0x194,8},{0x134,0x198,64},{0x138,0x19c,108},{0x13c,0x1a0,1},{0x140,0x1a4,4},{0x144,0x1a8,88},{0x148,0x1ac,64}};
    for(const auto& t:tables)if(!span(word(t[1]),word(t[0]),t[2]))return false;
    for(unsigned k=0;k<8;++k)if(!span(word(0x16c+4*k),word(0x108+4*k),4))return false;
    if(!groups() || groups()>65536 || !word(0x148))return false;
    const auto validGroup=[&](int32_t i){return i==-1 || (i>=0 && uint32_t(i)<groups());};
    for(uint32_t i=0;i<groups();++i){const auto g=group(i);
        if(!validGroup(g.child) || !validGroup(g.sibling) || g.shape< -1 || (g.shape>=0 && uint32_t(g.shape)>=word(0xe8)) || g.visibility>=word(0x13c) || g.node>word(0x140) || word(0x140)-g.node<24)return false;
    }
    for(uint32_t i=0;i<word(0xe8);++i){const auto h=word(0x14c)+168*i;
        const auto first=word(h+0x98),count=word(h+0x9c);if(first>word(0x138) || count>word(0x138)-first)return false;
        if(word(h+0x40)>word(0xf0) || word(h+0x48)>word(0xf8) || word(h+0x50)>word(0x100))return false;
        for(unsigned k=0;k<8;++k)if(word(h+0x58+8*k)>word(0x128))return false;
    }
    for(uint32_t i=0;i<word(0x138);++i){const auto d=word(0x19c)+108*i;
        const auto first=word(d+0x38),count=word(d+0x3c);if(first>word(0xec) || count>word(0xec)-first)return false;
    }
    // Native import admission for the reachable source graph; no runtime
    // cycle/depth fallback is substituted for original traversal.
    // Root is LAST group. Depth31 leaves one base entry in a32-matrix stack.
    // Native import rejects reachable cycles/shared revisits before traversal;
    // these are safety bounds, not measured original resource limits.
    std::vector<uint8_t> seen(groups());std::vector<std::pair<int32_t,unsigned>> pending{{int32_t(groups())-1,1}};
    while(!pending.empty()){const auto entry=pending.back();pending.pop_back();const auto i=entry.first;if(i==-1)continue;
        if(seen[unsigned(i)] || entry.second>31)return false;seen[unsigned(i)]=1;
        const auto g=group(unsigned(i));pending.emplace_back(g.sibling,entry.second);pending.emplace_back(g.child,entry.second+1);
    }
    for(uint32_t i=0;i<word(0x148);++i)if(descriptor(i).empty())return false;
    // Serialized index streams occupy32-bit words but source FIFO consumes
    // their low 16 bits. Add shape-local base only after narrowing; use64-bit
    // intermediate bounds so malformed sums cannot wrap into valid tables.
    auto indices=[&](uint32_t countField,uint32_t pointerField,uint32_t first,uint32_t offset,uint32_t count,uint32_t base,uint32_t whole){
        const uint64_t begin=uint64_t(first)+offset;if(begin+count>word(countField))return false;
        for(uint32_t v=0;v<count;++v)if(uint64_t(word(word(pointerField)+uint32_t(4*(begin+v)))&0xffff)+base>=whole)return false;
        return true;
    };
    for(uint32_t shape=0;shape<word(0xe8);++shape){const auto h=word(0x14c)+168*shape;if(word(h+0xa4)>3)return false;
        for(uint32_t i=0;i<word(h+0x9c);++i){const auto d=word(0x19c)+108*(word(h+0x98)+i),slots=word(d);if(slots>8)return false;
            for(uint32_t run=0;run<word(d+0x3c);++run){const auto r=word(0x150)+8*(word(d+0x38)+run),first=word(r),count=word(r+4)&0xffff;
                if(!indices(0xf4,0x158,first,word(d+0x40),count,word(h+0x40),word(0xf0)) ||
                   !indices(0xfc,0x160,first,word(d+0x44),count,word(h+0x48),word(0xf8)) ||
                   !indices(0x104,0x168,first,word(d+0x48),count,word(h+0x50),word(0x100)))return false;
                for(uint32_t slot=0;slot<slots;++slot){const auto k=int8_t(byte(d+0x30+(slots==1?0:slots-slot-1)));if(k<0 || k>=8)return false;
                    if(!indices(0x108+4*k,0x16c+4*k,first,word(d+0x4c+4*k),count,word(h+0x58+8*k),word(0x128)))return false;
                }
            }
        }
    }
    return true;
}
}
