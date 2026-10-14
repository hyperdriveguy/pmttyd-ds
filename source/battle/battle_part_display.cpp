#include "battle_part_display.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
enum Slot {Uminus,Uplus,Udisplay,Uworld,Urx,Ury,Urz,Uscale,Temp,Unit,Pminus,Pplus,Prx,Pry,Prz,Pworld,Pdisplay,Pscale,Draw,History,Depth};
constexpr unsigned rotationOrders[6][3]{{1,2,0},{1,0,2},{0,1,2},{0,2,1},{2,0,1},{2,1,0}};
float real(const BattleActorServices& a,uint32_t p){auto b=a.memory.word(a.memory.user,p);float v;std::memcpy(&v,&b,4);return v;}
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
std::array<float,3> xyz(const BattleActorServices& a,uint32_t p){return {real(a,p),real(a,p+4),real(a,p+8)};}
}
float nativeBattlePartPhaseOffset(uint16_t phase,bool floating){
    const int32_t degrees=(int32_t(phase)*(floating?4:3)+270)%360;
    const float turns=float(float(degrees)*6.2831854820251465f);
    const float radians=turns/360.f; // Source GetSin fmuls then fdivs before double sin.
    const float sine=float(std::sin(double(radians)));
    const float amplitude=floating?2.f:4.f;
    return std::fma(amplitude,sine,amplitude);
}
bool resumePackedBattlePartDisplay(uint32_t part,const BattlePartDisplayServices& s,BattlePartDisplayCursor& c){
    const auto& a=s.actors;const auto& m=a.memory;
    if(!c.active){
        if(!m.word || !m.storeWord || !m.storeByte || !a.byte)return false;
        c={};c.active=true;c.part=part;
        const int32_t handle=int32_t(m.word(m.user,part+0x1c0));
        c.battle=currentBattleAddress(a);c.unit=m.word(m.user,part+0x4ec);
        if(handle<0){c.active=false;return true;}
        c.phase=1;
    }
    if(c.part!=part)return false;
    auto scalar=[&](uint32_t fn,float value=0,std::array<float,4> coordinates=std::array<float,4>{})->std::optional<float>{
        if(!c.requestPending){c.scalar={fn,part,value,coordinates};c.requestPending=true;}
        c.pending=fn;const auto out=s.scalar?s.scalar(s.user,c.scalar):std::nullopt;
        if(out)c.requestPending=false;
        return out;
    };
    auto matrix=[&](auto prepare)->bool{
        if(!c.requestPending){c.matrix={};prepare();c.requestPending=true;}
        c.pending=c.matrix.function;if(!s.matrix || !s.matrix(s.user,c.matrix))return false;
        c.matrices[c.target]=c.matrix.output;c.requestPending=false;return true;
    };
    auto trans=[&](unsigned target,std::array<float,3> v){c.target=target;c.matrix.function=0x8029e230;c.matrix.xyz=v;};
    auto concat=[&](unsigned target,unsigned left,unsigned right){c.target=target;c.matrix.function=0x8029dd64;c.matrix.left=c.matrices[left];c.matrix.right=c.matrices[right];};
    auto identity=[&](){c.target=Temp;c.matrix.function=0x8029dd04;};
    auto scaleUnit=[&](bool negative){c.target=Uscale;c.matrix.function=0x8029e2b0;
        const float sharedScale=real(a,c.unit+0x114);
        c.matrix.xyz={sharedScale*(real(a,c.unit+0x9c)*real(a,c.unit+0x90)),sharedScale*(real(a,c.unit+0xa0)*real(a,c.unit+0x94)),real(a,c.unit+0xa4)*real(a,c.unit+0x98)};
        if(negative)c.matrix.xyz[2]=real(a,0x80422260)*c.matrix.xyz[2];};
    auto rotation=[&](unsigned target,uint32_t owner,unsigned base,unsigned current,unsigned axis,float correction=0){
        c.target=target;c.matrix.function=0x8029dff0;c.matrix.axis=axis;
        const float sum=real(a,owner+current)+real(a,owner+base);
        c.matrix.radians=real(a,0x8042223c)*(target==Pry?correction+sum:sum);
    };
    auto camera=[&](){c.pending=0x8000f2e4;if(!s.currentCamera || !(c.camera=s.currentCamera(s.user)))return false;return true;};
    auto coordinates=[&](){return std::array<float,4>{c.camera->eye[0],c.camera->eye[2],c.camera->at[0],c.camera->at[2]};};
    while(c.active){switch(c.phase){
    case 1:
        while(c.step<16){
            if(!matrix([&](){switch(c.step){
            case 0:{auto v=xyz(a,c.unit+0x78);for(auto& x:v)x=-x;trans(Uminus,v);break;}
            case 1:trans(Uplus,xyz(a,c.unit+0x78));break;
            case 2:trans(Udisplay,xyz(a,c.unit+0x54));break;
            case 3:trans(Uworld,xyz(a,c.unit+0x3c));break;
            case 4:rotation(Urx,c.unit,0x60,0x6c,120);break;
            case 5:rotation(Ury,c.unit,0x64,0x70,121);break;
            case 6:rotation(Urz,c.unit,0x68,0x74,122);break;
            case 7:scaleUnit(false);break;
            case 8:identity();break;
            case 9:concat(Temp,Uscale,Temp);break;
            case 10:concat(Temp,Uminus,Temp);break;
            case 11:concat(Temp,Ury,Temp);break;
            case 12:concat(Temp,Urz,Temp);break;
            case 13:concat(Temp,Urx,Temp);break;
            case 14:concat(Unit,Uplus,Temp);break;
            case 15:concat(Unit,Udisplay,Unit);break;
        }}))return false;
            ++c.step;}
        c.phase=2;break;
    case 2:{
        if(m.word(m.user,part+0x1ac)&0x20000000){c.phase=99;break;}
        c.direction=int8_t(a.byte(a.user,c.unit+0x189))<0?real(a,0x80422258):real(a,0x8042225c);
        const auto side=int8_t(a.byte(a.user,part+0xbd));
        const float sign=side>0?real(a,0x8042224c):side<0?real(a,0x80422260):float(int8_t(a.byte(a.user,c.battle+uint32_t(8*int8_t(a.byte(a.user,c.unit+0xc)))+0xa)));
        c.world[0]=std::fma(real(a,part+0x24),sign,real(a,c.unit+0x3c));c.phase=3;break;}
    case 3:{auto v=scalar(0x80100ed0);if(!v)return false;c.floatOffset=*v;c.phase=4;break;}
    case 4:{auto v=scalar(0x80100ffc);if(!v)return false;c.gravityOffset=*v;
        c.world[1]=((real(a,c.unit+0x40)+real(a,part+0x28))+c.gravityOffset)+c.floatOffset;
        c.world[2]=real(a,c.unit+0x44)+real(a,part+0x2c);c.correction=real(a,0x80422240);
        c.phase=m.word(m.user,m.word(m.user,part+0x4ec)+0x104)&0x04000000?5:8;c.step=0;break;}
    case 5:if(!camera())return false;c.phase=6;break;
    case 6:{auto v=scalar(0x8000703c,0,coordinates());if(!v)return false;c.q=*v;c.phase=7;break;}
    case 7:{auto v=scalar(0x8000703c,0,{c.camera->eye[0],c.camera->eye[2],c.world[0],c.world[2]});if(!v)return false;c.correction=c.q-*v;c.phase=70;break;}
    case 70:{auto v=scalar(0x80007328,c.correction);if(!v)return false;c.correction=*v;c.phase=8;c.step=0;break;}
    case 8:
        while(c.step<7){
            if(!matrix([&](){switch(c.step){
            case 0:{auto v=xyz(a,part+0x54);for(auto& x:v)x=-x;trans(Pminus,v);break;}
            case 1:trans(Pplus,xyz(a,part+0x54));break;
            case 2:rotation(Prx,part,0x3c,0x48,120);break;
            case 3:rotation(Pry,part,0x40,0x4c,121,c.correction);break;
            case 4:rotation(Prz,part,0x44,0x50,122);break;
            case 5:trans(Pworld,c.world);break;
            case 6:trans(Pdisplay,xyz(a,part+0x30));break;
        }}))return false;
            ++c.step;}
        c.phase=9;break;
    case 9:{auto v=scalar(0x80007328,std::fma(real(a,0x80422260),c.direction,-real(a,0x8042225c)));if(!v)return false;c.angle=*v;
        c.phase=m.word(m.user,m.word(m.user,part+0x4ec)+0x1c)&0x01000000?14:10;c.step=0;break;}
    case 10:{const float q=(((c.angle+real(a,c.unit+0x70))+real(a,part+0x4c))+real(a,c.unit+0x64))+real(a,part+0x40);
        auto v=scalar(0x80007328,q);if(!v)return false;c.q=*v;c.phase=11;break;}
    case 11:if(!camera())return false;c.phase=12;break;
    case 12:{auto v=scalar(0x8000703c,0,coordinates());if(!v)return false;c.correction=*v;c.phase=13;break;}
    case 13:{auto v=scalar(0x80007328,c.q+c.correction);if(!v)return false;c.phase=*v>=real(a,0x8042225c) && *v<=real(a,0x80422258)?130:14;c.step=0;break;}
    case 130:
        while(c.step<9){
            if(c.step==4 && !c.requestPending)c.order=uint8_t(a.byte(a.user,part+0x4fc));
            if(c.step>=4 && c.step<=6 && c.order>=6){c.step=7;continue;}
            if(!matrix([&](){switch(c.step){
                case 0:scaleUnit(true);break;case 1:identity();break;case 2:concat(Temp,Uscale,Temp);break;case 3:concat(Temp,Uminus,Temp);break;
                case 4:case 5:case 6:concat(Temp,Urx+rotationOrders[c.order][c.step-4],Temp);break;
                case 7:concat(Unit,Uplus,Temp);break;case 8:concat(Unit,Udisplay,Unit);break;
            }}))return false;
            ++c.step;
        }
        c.phase=14;c.step=0;break;
    case 14:
        while(c.step<14){
            if(c.step==5 && !c.requestPending)c.order=uint8_t(a.byte(a.user,part+0x4fc));
            if(c.step>=5 && c.step<=7 && c.order>=6){c.step=8;continue;}
            if(!matrix([&](){switch(c.step){
                case 0:c.target=Pscale;c.matrix.function=0x8029e2b0;c.matrix.xyz={real(a,part+0x6c)*real(a,part+0x60),real(a,part+0x70)*real(a,part+0x64),real(a,0x8042224c)*(real(a,part+0x74)*real(a,part+0x68))};break;
                case 1:identity();break;case 2:concat(Temp,Unit,Temp);break;case 3:concat(Temp,Pscale,Temp);break;case 4:concat(Temp,Pminus,Temp);break;
                case 5:case 6:case 7:concat(Temp,Prx+rotationOrders[c.order][c.step-5],Temp);break;
                case 8:concat(Temp,Pplus,Temp);break;case 9:concat(Draw,Pworld,Temp);break;case 10:concat(Draw,Pdisplay,Draw);break;
                case 11:c.target=History;c.matrix.function=0x8029dd30;c.matrix.left=c.matrices[Draw];break;
                case 12:{const float zero=real(a,0x80422240);trans(Depth,{zero,zero,real(a,0x80422268)});break;}
                case 13:concat(History,Depth,History);break;
            }}))return false;
            ++c.step;
        }
        c.phase=15;break;
    case 15:
        m.storeWord(m.user,part+0x218,m.word(m.user,part+0x4f4));
        for(auto mask:{0x10u,0x20u})if(m.word(m.user,part+0x204)&mask)for(unsigned i=0;i<3;++i)m.storeByte(m.user,part+0x218+i,0);
        c.white=a.byte(a.user,part+0x218)==255 && a.byte(a.user,part+0x219)==255 && a.byte(a.user,part+0x21a)==255;
        c.phase=c.white?16:18;break;
    case 16:{c.pending=0x80127814;const auto status=s.status?s.status(s.user,c.unit,4):std::nullopt;if(!status)return false;
        if(*status){m.storeByte(m.user,part+0x218,uint8_t(unsigned(a.byte(a.user,part+0x218))*160/255));m.storeByte(m.user,part+0x21a,uint8_t(unsigned(a.byte(a.user,part+0x218))*160/255));}
        c.phase=17;break;}
    case 17:for(unsigned i=0;i<3;++i)m.storeByte(m.user,part+0x218+i,uint8_t(unsigned(a.byte(a.user,part+0x218+i))*a.byte(a.user,c.unit+0x312)/255));c.phase=18;break;
    case 18:case 19:case 22:{
        if(!c.requestPending){c.material={c.phase==18?0x800406b4u:c.phase==19 || c.status16?0x80040730u:0x80040710u,int32_t(m.word(m.user,part+0x1c0)),c.phase==18?m.word(m.user,part+0x218):c.phase==19?0x40u:0x800000u};c.requestPending=true;}
        c.pending=c.material.function;if(!s.material || !s.material(s.user,c.material))return false;c.requestPending=false;
        c.phase=c.phase==18?19:c.phase==19?20:23;c.step=1;break;}
    case 20:case 24:
        if(!c.requestPending){c.callback=m.word(m.user,part+0x210);c.requestPending=true;}
        if(c.callback){c.pending=c.callback;if(!s.callback || !s.callback(s.user,c.callback,part,c.phase==24?1:0))return false;}
        c.requestPending=false;c.phase=c.phase==20?21:25;break;
    case 21:{c.pending=0x80127814;const auto v=s.status?s.status(s.user,c.unit,16):std::nullopt;if(!v)return false;c.status16=*v;c.phase=22;break;}
    case 23:
        while(c.step<=3){if(!c.requestPending){c.draw={int32_t(m.word(m.user,part+0x1c0)),c.step,c.matrices[Draw],c.angle,real(a,0x80422250)};c.requestPending=true;}
            c.pending=0x8003b4d8;uint32_t nested=0;if(!s.draw || !s.draw(s.user,c.draw,nested)){if(nested)c.pending=nested;return false;}
            c.requestPending=false;++c.step;}
        c.phase=24;break;
    case 25:
        c.color=m.word(m.user,part+0x218);c.historyMode=(m.word(m.user,part+0x1ac)&0x04000000)?1:0;c.phase=26;break;
    case 26:{const uint32_t h=part+0x21c;
        for(unsigned destination=9;destination>0;--destination){const uint32_t src=h+0xc+(destination-1)*0x44,dst=src+0x44;
            for(unsigned pair=0;pair<8;++pair){const auto x=m.word(m.user,src+8*pair),y=m.word(m.user,src+8*pair+4);m.storeWord(m.user,dst+8*pair,x);m.storeWord(m.user,dst+8*pair+4,y);}
            m.storeWord(m.user,dst+64,m.word(m.user,src+64));}
        m.storeWord(m.user,h+0xc,c.historyMode);c.phase=27;break;}
    case 27:{if(!matrix([&](){c.target=History;c.matrix.function=0x8029dd30;c.matrix.left=c.matrices[History];}))return false;
        const auto h=part+0x21c;for(unsigned pair=0;pair<6;++pair){const auto x=bits(c.matrix.output[2*pair]),y=bits(c.matrix.output[2*pair+1]);m.storeWord(m.user,h+0x10+8*pair,x);m.storeWord(m.user,h+0x14+8*pair,y);}
        m.storeWord(m.user,h+0x40,bits(c.angle));m.storeWord(m.user,h+0x44,c.color);m.storeWord(m.user,h+0x48,m.word(m.user,h+4));m.storeWord(m.user,h+0x4c,m.word(m.user,h+8));
        if((m.word(m.user,h)&1) && (m.word(m.user,h+0x50)&1))m.storeWord(m.user,h+0xc,m.word(m.user,h+0xc)&~1u);
        c.active=false;c.pending=0;return true;}
    default:c.pending=0x80101774;return false;
    }}
    return true;
}
}
