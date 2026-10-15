#include "map_material_light.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
namespace ttyd {
// PMLTSC01 fixed64 bytes: 8-byte magic, two24-byte map records, then
// paper/craft ambient RGBA words at 56/60. Each map has source zero light
// count and20-byte fog. Validate candidates before publishing map work.
bool MapMaterialLightWork::loadBattle(const char* path){
    FILE* file=path?std::fopen(path,"rb"):nullptr;if(!file)return false;
    std::array<uint8_t,64> data{};const bool valid=std::fread(data.data(),data.size(),1,file)==1 && std::fgetc(file)==EOF;std::fclose(file);
    if(!valid || std::memcmp(data.data(),"PMLTSC01",8))return false;
    auto word=[&](unsigned o){return uint32_t(data[o])<<24|uint32_t(data[o+1])<<16|uint32_t(data[o+2])<<8|data[o+3];};
    auto real=[&](unsigned o){const auto bits=word(o);float out;std::memcpy(&out,&bits,4);return out;};
    std::array<MapMaterialFog,2> fog;
    for(unsigned i=0;i<2;++i){const auto o=8+24*i;if(word(o)!=0)return false; // Other authored light entries require their real importer.
        fog[i]={word(o+4),word(o+8),word(o+20),real(o+12),real(o+16)};
        if(fog[i].type>4 || !std::isfinite(fog[i].start) || !std::isfinite(fog[i].end))return false;}
    if(!banks[1].known || banks[1].count!=32)return false;
    // bmapLoad selects/clears map work, not the light bank. Never erase lights
    // here: first initialization or explicit unload/reset owns their lifetime.
    selector=1;battleFog=fog;paperAmbient=word(56);craftAmbient=word(60);battleLoaded=true;return true;
}
void MapMaterialLightWork::resetSelected(){
    auto& b=banks[selector==1?1:0];if(b.count<0 || b.count>32){b.known=false;return;}
    for(int32_t i=0;i<b.count;++i)b.records[i]={};
}
// Select bank1 only when selector==1, otherwise field bank0. Record flags
// 81/8081(hex) find ordinary/ambient candidates; 1 is active, 8000 is ambient.
// More than8 active records needs source sort service800064fc, not truncation.
bool mapLightBankQuery(const MapMaterialLightWork& w,MapLightQuery& q,uint32_t& pending){
    pending=q.function;const auto& b=w.banks[w.selector==1?1:0];if(!b.known || b.count<0 || b.count>32)return false;
    if(q.function==0x8001b984 || q.function==0x8001b9f0){
        const uint16_t mask=q.function==0x8001b984?0x81:0x8081;const MapLightRecord* found=nullptr;
        for(int32_t i=0;i<b.count;++i)if((b.records[i].flags()&mask)==mask){found=&b.records[i];break;}
        q.record=found;q.found=found!=nullptr;pending=0;return true;
    }
    if(q.function!=0x8001bad0)return false;
    unsigned active=0;for(int32_t i=0;i<b.count;++i)if(b.records[i].flags()&1)++active;
    if(active>8){pending=0x800064fc;return false;}
    // Native caller capacity8; source M<=0 still writes one eligible pointer.
    if(q.maximum>8)return false;
    // Preserve source write-before-limit check: maximum<=0 still emits one
    // eligible pointer. Caller capacity 8 is a native bound, not a source guard.
    q.output={};q.count=0;
    for(int32_t i=0;i<b.count;++i){const auto& r=b.records[i];const auto flags=r.flags();
        if(!(flags&1) || (flags&0x8000) || (uint16_t(q.mask) && !(flags&uint16_t(q.mask))))continue;
        q.output[unsigned(q.count++)]=&r;if(q.count>=q.maximum)break;
    }
    pending=0;return true;
}
// Source flags 1/8/10(hex) request paper/craft/special presets; 2 adds scene
// lights; 4 selects white register material instead of vertex material.
// Preset queries need real producers; selected records stay borrowed across holds.
bool resumeMapMaterialLight(const MapMaterialLightWork& w,uint32_t flags,std::array<float,3> origin,const MapMaterialLightServices& s,MapMaterialLightCursor& c){
    if(!c.active){c={};c.active=true;c.flags=flags;c.origin=origin;}
    auto query=[&](uint32_t function,int32_t maximum=0){
        if(!c.queued){c.query={};c.query.function=function;c.query.origin=c.origin;c.query.maximum=maximum;c.query.mask=0x80;c.queued=true;}
        c.pending=function;return s.query && s.query(s.user,c.query,c.pending);
    };
    auto next=[&](uint32_t p){c.queued=false;c.phase=p;};
    for(;;)switch(c.phase){
    case 0:if(c.flags&1){if(!query(0x8001b944))return false;if(!c.query.record)return false;c.selected[c.count++]=c.query.record;}next(1);break;
    case 1:if(c.flags&8){if(!query(0x8001b938))return false;if(!c.query.record)return false;c.selected[c.count++]=c.query.record;}next(2);break;
    case 2:if(c.flags&0x10){if(!query(0x8001b944))return false;if(!c.query.record)return false;c.selected[c.count++]=c.query.record;}next(3);break;
    case 3:if(c.flags&2){if(!query(0x8001b984))return false;const bool append=c.query.found && c.count<8;next(append?4:5);}else next(5);break;
    case 4:if(!query(0x8001bad0,8-c.count))return false;
        if(c.query.count<0 || c.query.count>8-c.count)return false;
        for(int32_t i=0;i<c.query.count;++i)c.selected[c.count+i]=c.query.output[i];
        c.count+=c.query.count;next(5);break;
    case 5:
        if(c.flags&0x19){if(c.count==1){c.ambient=(c.flags&1)?w.paperAmbient:(c.flags&8)?w.craftAmbient:0x000000ff;next(7);}else next(6);}else next(7);break;
    case 6:if(!query(0x8001bad0,8))return false;if(c.query.count<0 || c.query.count>8)return false;c.selected=c.query.output;c.count=c.query.count;next(7);break;
    // Record7000(hex) requires actual light setup/attenuation. Each completed
    // slot sets one mask bit; no fake light success may advance index.
    case 7:
        if(c.index>=c.count){next(8);break;}
        if(!c.selected[c.index]){c.pending=0x800208c0;return false;}
        if(c.selected[c.index]->flags()&0x7000){c.pending=0x800208c0;if(!s.light || !s.light(s.user,unsigned(c.index),c.selected[c.index],c.attenuation,c.pending))return false;}
        c.mask|=1u<<unsigned(c.index++);break;
    case 8:if(!query(0x8001b9f0))return false;if(c.query.record)c.ambient=c.query.record->color();next(9);break;
    case 9:{
        if(!c.queued){c.request={};auto& r=c.request;
            switch(c.step){
            case 0:r.kind=ShadowGX::Fog;r.color=0xffffffff;break;
            case 1:r.kind=ShadowGX::NumChans;r.values[0]=1;break;
            case 2:r.kind=static_cast<ShadowGX>(0x802b6d6c);r.values[0]=4;r.color=c.ambient;break;
            case 3:if(c.flags&4){r.kind=ShadowGX::ChanMaterial;r.values[0]=4;r.color=0xffffffff;break;}++c.step;[[fallthrough]];
            case 4:r.kind=ShadowGX::ChanCtrl;r.values={0,c.count!=0,0,(c.flags&4)?0:1,int32_t(c.mask),2,int32_t(c.attenuation)};break;
            case 5:r.kind=ShadowGX::ChanCtrl;r.values={2,0,0,(c.flags&4)?0:1,0,2,2};break;
            default:c.active=false;c.pending=0;return true;
            }c.queued=true;
        }
        c.pending=uint32_t(c.request.kind);if(!s.gx || !s.gx(s.user,c.request))return false;c.queued=false;++c.step;break;}
    default:c.pending=0x800208c0;return false;
    }
}
}
