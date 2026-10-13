#include "message_playback.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace ttyd {
bool MessageFont::load(const char* path) {
    if(!path)return false;
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[8];uint32_t count=0;std::array<uint16_t,256> map{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFONT1",8) && std::fread(&count,4,1,f)==1 && count && count<=1024 && std::fread(map.data(),sizeof(map),1,f)==1;
    std::vector<FontGlyph> pending;
    if(ok){pending.resize(count);ok=std::fread(pending.data(),8,count,f)==count && std::fgetc(f)==EOF;}
    std::fclose(f);if(!ok)return false;
    for(auto g:map)if(g>=count)return false;
    for(const auto& g:pending)if(!g.width || !g.height || g.width>24 || g.height>24 || g.x+g.width>512 || g.y+g.height>256)return false;
    mapping=map;glyphs=std::move(pending);return true;
}
namespace {
bool number(const std::string& s,float& f){char* end=nullptr;f=std::strtof(s.c_str(),&end);return end!=s.c_str() && *end==0 && std::isfinite(f);}
int16_t half(int32_t v){const uint16_t w=uint16_t(v);return w<=32767?int16_t(w):int16_t(int32_t(w)-65536);}
}
bool MessagePlayback::open(const char* key,const char* text,const MessageFont& font,uint64_t now) {
    if(!key || !text || font.glyphs.empty())return false;
    MessagePlayback next;next.identity=key;next.raw=text;
    if(next.raw.size()>65536)return false;
    uint32_t t=0,speed=25,style=0;int16_t x=0,y=0,k=0,s=0,baseline=0;float scale=1,verticalScale=1,dynamic=1,lineScale=0;
    auto record=[&](uint16_t code){MessageRecord r;r.flags=style;r.code=code;r.x=x;r.y=y;r.schedule=t;r.scale=scale;r.value=dynamic;return r;};
    for(size_t at=0;at<next.raw.size();) {
        const uint8_t c=uint8_t(next.raw[at++]);
        if(c=='<') {
            const size_t end=next.raw.find('>',at);if(end==std::string::npos || end-at>=128)return false;
            const auto control=next.raw.substr(at,end-at);const auto space=control.find(' ');
            const auto name=control.substr(0,space);const auto args=space==std::string::npos?std::string{}:control.substr(space+1);float value=0;
            if(name=="k" || name=="key") {auto r=record(0xfffe);r.x=k;next.records.push_back(r);t+=100;k=half(k+1);}
            else if(name=="p" || name=="scrl_auto") {if(s){auto r=record(0xfffd);r.y=half(s-3);next.records.push_back(r);baseline=half(y-6);y=half(y-3);s=0;t+=50;}}
            else if(name=="wait" || name=="speed") {if(!number(args,value) || double(value)<-2147483648. || double(value)>=2147483648.)return false;if(name=="wait")t+=uint32_t(int32_t(value));else speed=uint32_t(int32_t(value));}
            else if(name=="/speed")speed=25;
            else if(name=="scale" || name=="scaleX") {if(!number(args,scale) || scale<=0 || scale>8)return false;if(name=="scale"){verticalScale=scale;style&=~0x10000u;}else style|=0x10000;}
            else if(name=="/scale"){scale=verticalScale=1;style&=~0x10000u;}
            else if(name=="pos") {int a=0,b=0;char extra=0;if(std::sscanf(args.c_str(),"%d%d %c",&a,&b,&extra)!=2)return false;x=half(a);if(b!=999){y=half(baseline-b);s=half(-b);}}
            else if(name=="col" || name=="/col") {unsigned color=255;char extra=0;if(name=="col" && std::sscanf(args.c_str(),"%x %c",&color,&extra)!=1)return false;auto r=record(0xfff7);r.flags=color;next.records.push_back(r);}
            else if(name=="wave")style|=4;
            else if(name=="/wave")style&=~4u;
            else if(name=="small"){next.windowType=1;next.geometry.width=232*.9f;next.geometry.height=64*.9f;}
            else if(name=="clear")next.windowType=11;
            else if(name=="wpos") {int a,b,c,d;char extra;if(std::sscanf(args.c_str(),"%d%d%d%d %c",&a,&b,&c,&d,&extra)!=4 || c<=0 || d<=0 || c>32767 || d>32767)return false;next.geometry={float(half(a)),float(half(b)),float(c),float(d)};}
            else if(name=="dynamic") {if(!number(args,dynamic))return false;style|=1;}
            else if(name=="/dynamic")style&=~1u;
            else if(name=="shake")style|=2;
            else if(name=="/shake")style&=~2u;
            else return false;
            at=end+1;if(at<next.raw.size() && next.raw[at]=='\n')++at;
        } else if(c=='\n') {
            next.records.push_back(record(0xfffb));const float delta=(next.windowType==1?.7f:1.f)*32*(lineScale==0?1:lineScale);
            y=half(int32_t(float(y)-delta));s=half(int32_t(float(s)-delta));x=0;lineScale=0;t+=speed;
        } else {
            const uint16_t glyph=font.mapping[c];if(glyph>=font.glyphs.size())return false;
            auto r=record(glyph);r.y=half(y-int32_t(10*verticalScale));if(c!=' ')r.flags|=8;next.records.push_back(r);
            x=half(x+int32_t(scale*(font.glyphs[glyph].width+2)));t+=speed;lineScale=std::max(lineScale,verticalScale);
        }
        if(next.records.size()>=2560)return false;
    }
    next.records.push_back(record(0xffff));next.state=5;next.alpha=0;next.lastTime=next.animationTime=now;
    static uint64_t serial=0;next.serial=++serial;
    *this=std::move(next);return true;
}
int MessagePlayback::play(uint64_t now,uint32_t input) {
    flags&=~2u;
    const uint32_t delta=uint32_t(now-lastTime);
    auto accelerate=[&](){if(!(flags&0x80) && (input&0x300))flags|=4;};
    if(scrollTarget>scroll) {
        scroll=int32_t(std::fma(.36000001430511475f,float(delta),float(scroll)));accelerate();
        if(flags&4)scroll=int32_t(std::fma(.36000001430511475f,float(delta),float(scroll)));
        scroll=std::min(scroll,scrollTarget);lastTime=now;return 3;
    }
    if(wait==1)return 2;
    if(wait==2){++acceptedKey;wait=0;elapsed+=100;}
    else {accelerate();elapsed+=delta*((flags&4)?100u:1u);}
    lastTime=now;
    while(cursor<records.size() && records[cursor].schedule<=elapsed) {
        const auto& r=records[cursor];
        if(r.code==0xffff)return 0;
        if(r.code==0xfffe && int32_t(r.x)==acceptedKey){wait=1;elapsed=r.schedule;flags&=~4u;}
        else if(r.code==0xfffd){scrollTarget-=r.y;elapsed=r.schedule;}
        ++cursor;
    }
    return wait?2:3;
}
void MessagePlayback::tick(uint64_t now,uint32_t input) {
    if(!state || now<lastTime)return;
    if(state==5){animationTime=now;state=6;}
    if(state==6){const uint64_t a=(now-animationTime)*255/200;alpha=uint8_t(std::min<uint64_t>(a,255));if(a>255){state=1;lastTime=now;}return;}
    if(state==7){animationTime=now;state=8;return;}
    if(state==8){if(alpha==0){state=0;return;}const uint64_t a=(now-animationTime)*255/200;alpha=uint8_t(a>=255?0:255-a);return;}
    if(state==3){if(input&0x300){state=1;wait=2;if((input&0x200) && !(flags&0x80))flags|=4;}return;}
    if(state==1){const int result=play(now,input);if(result==0)state=7;else if(result==2)state=3;}
}
}
