#include "animation_fan.hpp"
#include <cmath>
namespace ttyd {
namespace {
int binding(uint32_t attribute){return attribute>=9 && attribute<=11?int(attribute-9):attribute==13?3:-1;}
bool finite(const ActorVector& v){for(float x:v)if(!std::isfinite(x))return false;return true;}
}
// CPU staging owns complete source fans, not live GPU Begin state. Attribute
// 9/10/11/13 maps position/normal/color 0/UV0; descriptor 3=INDEX16.
// Zero/one texture slot is admitted; multitexture requires distinct real owners.
bool AnimationFanStream::request(const AnimationShapeCall& r,const AnimationFanTexture& texture,const AnimationFanSubmit& submit){
    if(!r.pose || !r.pose->resource || r.resource!=r.pose->resource->draw || !r.resource)return false;
    const auto& raw=*r.resource;
    if(r.function==0x802b4520){
        if(active_ || r.arguments!=std::array<uint32_t,7>{} || !raw.span(r.draw,1,108))return false;
        const auto draws=raw.word(0x19c);
        if(r.draw<draws || (r.draw-draws)%108 || (r.draw-draws)/108>=raw.word(0x138))return false;
        const uint32_t slots=raw.word(r.draw);if(slots>1)return false; // Other samplers need real GPU owners.
        bindings_={};resource_=r.pose->resource;pose_=r.pose;draw_=r.draw;slots_=slots;used_=count_=0;ready_=false;return true;
    }
    if(resource_!=r.pose->resource || pose_!=r.pose || draw_!=r.draw)return false;
    if(r.function==0x802b3c08 || r.function==0x802b4558 || r.function==0x802b4db0){
        if(active_)return false;
        const uint32_t attribute=r.arguments[r.function==0x802b4558?1:0];const int slot=binding(attribute);
        if(slot<0 || (slot==3 && !slots_))return false;
        auto& b=bindings_[unsigned(slot)];
        if(r.function==0x802b3c08){if(r.arguments!=std::array<uint32_t,7>{attribute,3})return false;b.descriptor=true;return true;}
        if(r.function==0x802b4558){
            if(r.arguments!=std::array<uint32_t,7>{0,attribute,slot==1?0u:1u,slot==2?5u:4u,0})return false;
            b.format=true;return true;
        }
        const uint32_t stride=slot==2?4:slot==3?8:12;
        if(r.arguments!=std::array<uint32_t,7>{attribute,r.arguments[1],stride})return false;
        const auto base=r.arguments[1];
        if(slot<2){const auto& output=slot==0?r.pose->shape.positions:r.pose->shape.normals;
            if(output.size()!=raw.word(slot==0?0xf0:0xf8) || base>output.size())return false;
        }else {
            const auto table=raw.word(slot==2?0x164:0x18c),count=raw.word(slot==2?0x100:0x128);
            if(base<table || (base-table)%stride || (base-table)/stride>count || !raw.span(base,0,stride))return false;
        }
        b.base=base;b.array=true;return true;
    }
    // GXBegin modea0(hex)=fan, vat0, count bounded by native Capacity64.
    // Generator30 uses retained texture matrix, 60 identity; atlas scales1/2/4/8
    // and cells preserve actual resource identity across a suspended fan.
    if(r.function==0x802b5a6c){
        if(active_ || r.arguments!=std::array<uint32_t,7>{0xa0,0,r.arguments[2]} || r.arguments[2]>Capacity)return false;
        for(unsigned i=0;i<3+slots_;++i){const auto& b=bindings_[i];if(!b.descriptor || !b.format || !b.array)return false;}
        if(slots_){
            if(texture.resource!=resource_ || (texture.uvScale!=1 && texture.uvScale!=2 && texture.uvScale!=4 && texture.uvScale!=8) || texture.cell>=64/texture.uvScale || (texture.generator!=30 && texture.generator!=60))return false;
            if(texture.generator==30){if(!texture.matrix)return false;for(float x:*texture.matrix)if(!std::isfinite(x))return false;}
        }
        count_=r.arguments[2];used_=0;ready_=false;texture_=texture;active_=count_!=0;return true;
    }
    if(r.function!=0xcc008000 || !active_ || r.arguments!=std::array<uint32_t,7>{3+slots_})return false;
    // Only identical final FIFO indices may retry a held complete fan. Earlier
    // vertices and bound arrays must not replay or change underneath it.
    if(ready_){if(r.indices!=lastIndices_)return false;}
    else {
        if(used_>=count_)return false;
        const uint64_t p=uint64_t(bindings_[0].base)+r.indices[0],n=uint64_t(bindings_[1].base)+r.indices[1];
        if(p>=r.pose->shape.positions.size() || n>=r.pose->shape.normals.size())return false;
        AnimationFanVertex v;v.position=r.pose->shape.positions[size_t(p)];v.normal=r.pose->shape.normals[size_t(n)];
        if(!finite(v.position) || !finite(v.normal))return false;
        const uint32_t colorTable=raw.word(0x164),colorCount=raw.word(0x100);const uint64_t color=(bindings_[2].base-colorTable)/4+uint64_t(r.indices[2]);
        if(color>=colorCount)return false;
        const auto co=colorTable+uint32_t(4*color);for(unsigned i=0;i<4;++i)v.color[i]=raw.byte(co+i);
        if(slots_){
            const uint32_t uvTable=raw.word(0x18c),uvCount=raw.word(0x128);const uint64_t index=(bindings_[3].base-uvTable)/8+uint64_t(r.indices[3]);if(index>=uvCount)return false;
            const auto offset=uvTable+uint32_t(8*index);const float u=raw.real(offset),t=raw.real(offset+4);if(!std::isfinite(u) || !std::isfinite(t))return false;
            float s=u,q=t;
            if(texture_.generator==30){const auto& m=*texture_.matrix;s=((m[0]*u+m[1]*t)+m[2])+m[3];q=((m[4]*u+m[5]*t)+m[6])+m[7];} // TEX0 input AB11.
            // Atlas clamp needs subdivision outside this domain; never sample a
            // neighboring cell or silently clamp only vertices of such a fan.
            if(!std::isfinite(s) || !std::isfinite(q) || s<0 || s>1 || q<0 || q>1)return false;
            // Logical1024 atlas: 8 columns, 128 stride, 2px gutter, 124 content.
            // Vertical crop scale converts selected package, then native V flips.
            v.uv={(128.f*(texture_.cell%8)+2.f+124.f*s)/1024.f,1.f-(128.f*(texture_.cell/8)+2.f+124.f*q)*texture_.uvScale/1024.f};
        }
        vertices_[used_++]=v;
        if(used_<count_)return true;
        lastIndices_=r.indices;ready_=true;
    }
    // Fewer than3 vertices acknowledge without a primitive. Counters advance
    // only after whole-fan submission; failed submit retains ready geometry.
    if(count_>=3){
        const AnimationFanBatch batch{vertices_.data(),count_,draw_,pose_,resource_,texture_,slots_};
        if(!submit.submit || !submit.submit(submit.user,batch))return false;
        ++submittedFans_;submittedVertices_+=count_;
    }
    active_=ready_=false;return true;
}
}
