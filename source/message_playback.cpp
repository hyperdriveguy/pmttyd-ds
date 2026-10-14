#include "message_playback.hpp"
#include "message_icons.hpp"
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
    bool smallSpacing=false;uint32_t choiceMax=0;
    auto record=[&](uint16_t code){MessageRecord r;r.flags=style;r.code=code;r.x=x;r.y=y;r.schedule=t;r.scale=scale;r.value=dynamic;return r;};
    for(size_t at=0;at<next.raw.size();) {
        const uint8_t c=uint8_t(next.raw[at++]);
        if(c=='<') {
            const size_t end=next.raw.find('>',at);if(end==std::string::npos || end-at>=128)return false;
            const auto control=next.raw.substr(at,end-at);const auto space=control.find(' ');
            const auto name=control.substr(0,space);const auto args=space==std::string::npos?std::string{}:control.substr(space+1);float value=0;
            if(name=="k" || name=="key") {auto r=record(0xfffe);r.x=k;next.records.push_back(r);t+=100;k=half(k+1);}
            else if(name=="o" || name=="once_stop") {next.records.push_back(record(0xfffc));t+=50;}
            else if(name=="dkey" || name=="/dkey") {auto r=record(0xfff5);r.flags=name=="dkey"?1u:0u;next.records.push_back(r);}
            else if(name=="p" || name=="scrl_auto") {if(s){auto r=record(0xfffd);r.y=half(s-3);next.records.push_back(r);baseline=half(y-6);y=half(y-3);s=0;t+=50;}}
            else if(name=="wait" || name=="speed") {if(!number(args,value) || double(value)<-2147483648. || double(value)>=2147483648.)return false;if(name=="wait")t+=uint32_t(int32_t(value));else speed=uint32_t(int32_t(value));}
            else if(name=="/speed")speed=25;
            else if(name=="scale" || name=="scaleX") {if(!number(args,scale) || scale<=0 || scale>8)return false;if(name=="scale"){verticalScale=scale;style&=~0x10000u;}else style|=0x10000;}
            else if(name=="/scale"){scale=verticalScale=1;style&=~0x10000u;}
            else if(name=="pos") {int a=0,b=0;char extra=0;if(std::sscanf(args.c_str(),"%d%d %c",&a,&b,&extra)!=2)return false;x=half(a);if(b!=999){y=half(baseline-b);s=half(-b);}}
            else if(name=="col" || name=="/col") {unsigned color=255;char extra=0;if(name=="col" && std::sscanf(args.c_str(),"%x %c",&color,&extra)!=1)return false;auto r=record(0xfff7);r.flags=color;next.records.push_back(r);}
            else if(name=="wave")style|=4;
            else if(name=="/wave")style&=~4u;
            else if(name=="small"){next.windowType=1;smallSpacing=true;}
            else if(name=="system") {next.windowType=3;auto r=record(0xfff7);r.flags=0xdcc8dcff;next.records.push_back(r);}
            else if(name=="select") {
                int a,b,c,d;char extra;
                if(std::sscanf(args.c_str(),"%d%d%d%d %c",&a,&b,&c,&d,&extra)!=4 || a<-32768 || a>32767 || b<-32768 || b>32767 || c<-32768 || c>32767 || d<-32768 || d>32767)return false;
                auto byte=[](int v){const uint8_t n=uint8_t(v);return n<128?int8_t(n):int8_t(int(n)-256);};
                next.initialChoice=byte(a);next.cancelChoice=byte(b);next.choiceCount=1;next.windowType=5;
                next.choiceWidth=int16_t(c);next.choiceY=int16_t(d);
                next.flags|=0x20;style|=0x100;speed=0;
            }
            else if(name=="clear")next.windowType=11;
            else if(name=="icon") {
                char id[32],extra;float q;int offset,aux,advance;
                if(std::sscanf(args.c_str(),"%31s%f%d%d%d %c",id,&q,&offset,&aux,&advance,&extra)!=5 || !std::isfinite(q) || q<=0 || q>8)return false;
                uint32_t identifier=0;
                if(!std::strcmp(id,"STICK"))identifier=0x81;
                else if(!std::strcmp(id,"PAD_Y_ON"))identifier=0x73;
                else {char* end=nullptr;identifier=std::strtoul(id,&end,10);if(*end || end==id)return false;}
                const auto* icon=messageIcon(identifier);if(!icon)return false;
                const auto& frame=icon->frames[0];auto r=record(0xfff8);r.flags=identifier;r.scale=q;r.extra=uint16_t(aux);
                r.x=half(int32_t(std::fma(.5f*frame.width,q,float(x))+offset));
                r.y=half(int32_t(std::fma(-float(frame.height),q,float(y))));
                next.records.push_back(r);x=half(x+advance+40);t+=speed;
            }
            else if(name=="diary")next.windowType=7;
            else if(name=="wpos") {int a,b,c,d;char extra;if(std::sscanf(args.c_str(),"%d%d%d%d %c",&a,&b,&c,&d,&extra)!=4 || c<=0 || d<=0 || c>32767 || d>32767)return false;next.geometry={float(half(a)),float(half(b)),float(c),float(d)};}
            else if(name=="dynamic") {if(!number(args,dynamic))return false;style|=1;}
            else if(name=="/dynamic")style&=~1u;
            else if(name=="shake")style|=2;
            else if(name=="/shake")style&=~2u;
            else return false;
            at=end+1;if(at<next.raw.size() && next.raw[at]=='\n')++at;
        } else if(c=='\n') {
            if(next.flags&0x20){
                if(next.choiceCount>=127)return false;
                style=(style&~0xff00u)|(next.choiceCount<24?0x100u<<next.choiceCount:0u);
                ++next.choiceCount;
                if(next.choiceWidth==0)choiceMax=std::max(choiceMax,uint32_t(int32_t(x)));
            }
            next.records.push_back(record(0xfffb));const float delta=(smallSpacing?.7f:1.f)*32*(lineScale==0?1:lineScale);
            y=half(int32_t(float(y)-delta));s=half(int32_t(float(s)-delta));x=0;lineScale=0;t+=speed;
        } else {
            const uint16_t glyph=font.mapping[c];if(glyph>=font.glyphs.size())return false;
            auto r=record(glyph);r.y=half(y-int32_t(10*verticalScale));if(c!=' ')r.flags|=8;next.records.push_back(r);
            x=half(x+int32_t(scale*(font.glyphs[glyph].width+2)));t+=speed;lineScale=std::max(lineScale,verticalScale);
        }
        if(next.records.size()>=2560)return false;
    }
    if(next.windowType==1){next.geometry.width=232*.9f;next.geometry.height=64*.9f;}
    if(next.windowType==5) {
        if(next.choiceWidth==0)next.choiceWidth=half(int32_t(std::max(choiceMax,uint32_t(int32_t(x)))+80));
        if(next.choiceWidth<=0)return false;
        next.geometry={float(-int(next.choiceWidth)/2),float(next.choiceY),float(next.choiceWidth),float(half(36*next.choiceCount+40))};
    }
    next.records.push_back(record(0xffff));next.state=5;next.windowFlags=3;next.alpha=0;next.lastTime=next.animationTime=now;
    next.openedAt=now;
    static uint64_t serial=0;next.serial=++serial;
    *this=std::move(next);return true;
}
bool MessagePlayback::append(const char* text,const MessageFont& font,uint64_t now) {
    if(!text || !active())return false;
    MessagePlayback parsed;
    if(!parsed.open(identity.c_str(),(raw+text).c_str(),font,now))return false;
    // Reparse the accumulated original bytes to reproduce retained parser
    // T/V/style/coordinates and overwrite the prior terminal slot. Playback
    // ownership and the existing window identity/anchor remain unchanged.
    records=std::move(parsed.records);raw=std::move(parsed.raw);
    state=1;windowFlags|=2;lastTime=now;return true;
}
bool MessagePlayback::resume(uint64_t now) {
    if(!active())return false;
    state=1;windowFlags|=2;lastTime=now;return true;
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
        if(r.code==0xffff) {
            if(requireTerminalInput) {
                // A source key following the final glyph already gates this
                // page. Only a payload lacking that gate needs the adaptation.
                for(size_t i=cursor;i>0;--i) {
                    const auto code=records[i-1].code;
                    if(code==0xfffe)break;
                    if(code<0xfff0)return 4;
                }
            }
            return 0;
        }
        if(r.code==0xfffc){++cursor;elapsed=r.schedule;flags&=~4u;return 1;}
        if(r.code==0xfffe && int32_t(r.x)==acceptedKey){wait=1;elapsed=r.schedule;flags&=~4u;}
        else if(r.code==0xfffd){scrollTarget-=r.y;elapsed=r.schedule;}
        else if(r.code==0xfff5) {
            if(r.flags){flags|=0x80;flags&=~4u;elapsed=r.schedule;}
            else flags&=~0x80u;
        }
        // Generic-record talk publication follows the source bit0x10 priority.
        if(r.code<0xfff0 && !(r.flags&0x10) && (r.flags&8) && windowType!=3)talkCountdown=15;
        ++cursor;
    }
    return wait?2:3;
}
void MessagePlayback::tick(uint64_t now,uint32_t input) {
    if(!state || now<lastTime)return;
    auto requestPose=[&](PoseRequest request){poseRequest=request;++poseRequestSerial;poseRequestedAt=now;};
    if(windowType!=5 && talkCountdown) {
        if(!talkLatched){talking=true;talkLatched=true;requestPose(PoseRequest::Talk);}
        if(--talkCountdown==0){talking=false;talkLatched=false;requestPose(PoseRequest::Stay);}
    }
    if(state==5){animationTime=now;state=6;}
    if(state==6){windowFlags|=2;const uint64_t a=(now-animationTime)*255/200;alpha=uint8_t(std::min<uint64_t>(a,255));if(a>255){state=1;lastTime=now;}return;}
    if(state==7){animationTime=now;state=8;}
    if(state==8){if(alpha==0){state=0;windowFlags=0;return;}const uint64_t a=(now-animationTime)*255/200;alpha=uint8_t(a>=255?0:255-a);return;}
    if(state==9){if(input&0x300)state=7;return;}
    if(state==3){if(input&0x300){state=1;wait=2;if((input&0x200) && !(flags&0x80))flags|=4;}return;}
    if(state==1){const int result=play(now,input);if(result==0){state=7;talking=false;requestPose(PoseRequest::Stay);}else if(result==1){state=2;windowFlags&=~2u;talking=false;requestPose(PoseRequest::Stay);}else if(result==2)state=3;else if(result==4){state=9;talking=false;requestPose(PoseRequest::Stay);}}
}
}
