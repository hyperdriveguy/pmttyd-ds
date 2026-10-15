#include "hit_n64_effect.hpp"
#include <cstring>
namespace ttyd {
namespace {
// Original byte tables:kind lifetime(count table),age animation/radius. Root
// and each particle stride44hex;root kind0,XYZ4,scale28,life30,age34,progress38,
// colors3c..41. Root alpha2c is deliberately UNINITIALIZED by source Entry.
constexpr uint32_t CountTable=0x80419888,Anim=0x8041988c,Radius=0x8041989c;
constexpr uint32_t Zero=0x80425468,One=0x8042547c,Factor=0x80425478;
int32_t signedWord(uint32_t n){return n<0x80000000?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}
bool sameMemory(const EventMemoryServices& a,const EventMemoryServices& b){return a.user==b.user && a.word==b.word && a.half==b.half && a.storeWord==b.storeWord && a.storeHalf==b.storeHalf && a.storeByte==b.storeByte && a.flushBlock==b.flushBlock && a.systemCall==b.systemCall;}
bool same(const HitN64Services& a,const HitN64Services& b){return sameMemory(a.pool.memory,b.pool.memory) && a.pool.user==b.pool.user && a.pool.heapAllocate==b.pool.heapAllocate && a.pool.clear==b.pool.clear && a.pool.heapFree==b.pool.heapFree && a.pool.update==b.pool.update && a.pool.fileFree==b.pool.fileFree && a.pool.fileAsync==b.pool.fileAsync && a.pool.fileAllocate==b.pool.fileAllocate && a.user==b.user && a.byte==b.byte && a.randomRaw==b.randomRaw && a.effectDelete==b.effectDelete && a.depth==b.depth && a.display==b.display;}
struct Guard{bool& running;explicit Guard(bool& f):running(f){running=true;}~Guard(){running=false;}};
struct Memory {
    HitN64Services services;
    uint32_t word(uint32_t a)const{return services.pool.memory.word(services.pool.memory.user,a);}
    void put(uint32_t a,uint32_t n)const{services.pool.memory.storeWord(services.pool.memory.user,a,n);}
    uint8_t byte(uint32_t a)const{return services.byte(services.user,a);}
    void putByte(uint32_t a,uint8_t n)const{services.pool.memory.storeByte(services.pool.memory.user,a,n);}
    float real(uint32_t a)const{return scalar(word(a));}
    void real(uint32_t a,float f)const{put(a,bits(f));}
    uint64_t wide(uint32_t a)const{const auto high=word(a),low=word(a+4);return uint64_t(high)<<32|low;}
};
// Source4330 signed/unsigned bias conversion rounds double difference to SP.
// RNG signed remainder360 and wrapped direction numerator are preserved;no
// arbitrary nonnegative RNG normalization or guessed radian input here.
float convert(uint32_t n,bool sign,uint64_t bias){const uint64_t pattern=0x4330000000000000ULL|(sign?n^0x80000000u:n);double d,b;std::memcpy(&d,&pattern,8);std::memcpy(&b,&bias,8);return float(d-b);}
bool valid(const HitN64Services& s,bool entry){return s.pool.memory.word && s.pool.memory.storeWord && (!entry || s.pool.memory.storeByte) && s.byte;}
}
struct HitN64EntryCursor::State:Memory {
    uint32_t phase=0,pending=0,effect=0,payload=0,particle=0,angle=360,index=1;
    int32_t kind=0,amount=0,count=0;
    std::array<float,3> position{};float scale=0,zero=0;uint64_t bias=0;
};
HitN64EntryCursor::HitN64EntryCursor()=default;HitN64EntryCursor::~HitN64EntryCursor()=default;
bool HitN64EntryCursor::active()const{return bool(state_);}uint32_t HitN64EntryCursor::pending()const{return state_?state_->pending:0;}
bool HitN64EntryCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Reserve real pool record BEFORE heap3 payload:count=trunc_signed(wrap32
//(amount*5+20)/3)+1,bytes=wrap32(count*44hex). No clamp/null-allocation repair.
// Root name2fb0c8/callback1c61bc are original identities,not native handles.
std::optional<uint32_t> resumeHitN64Entry(int32_t kind,int32_t amount,const std::array<float,3>& pos,float scale,const HitN64Services& s,HitN64EntryCursor& cursor){
    if(cursor.invoking_ || !valid(s,true))return {};
    if(cursor.state_ && (!same(cursor.state_->services,s) || cursor.state_->kind!=kind || cursor.state_->amount!=amount || std::memcmp(cursor.state_->position.data(),pos.data(),12) || bits(cursor.state_->scale)!=bits(scale)))return {};
    Guard guard(cursor.invoking_);
    if(!cursor.state_){cursor.state_=std::make_unique<HitN64EntryCursor::State>();auto& c=*cursor.state_;c.services=s;c.kind=kind;c.amount=amount;c.position=pos;c.scale=scale;}
    auto& c=*cursor.state_;
    for(;;)switch(c.phase){
    case 0:{
        c.pending=0x8005ec68;const auto effect=effectPoolEntry(s.pool);if(!effect)return {};
        c.effect=*effect;
        const auto numerator=signedWord(uint32_t(amount)*5u+20u);c.count=numerator/3+1;
        c.put(c.effect+0x14,0x802fb0c8);c.put(c.effect+8,uint32_t(c.count));c.phase=1;break;}
    case 1:{
        c.pending=0x800300f0;if(!s.pool.heapAllocate)return {};
        const auto payload=s.pool.heapAllocate(s.pool.user,3,uint32_t(c.count)*0x44u);if(!payload)return {};
        c.payload=*payload;c.put(c.effect+0xc,c.payload);c.put(c.effect+0x10,0x801c61bc);
        const auto zero=c.real(Zero);
        c.put(c.payload,uint32_t(kind));for(unsigned i=0;i<3;++i)c.real(c.payload+4+4*i,pos[i]);
        c.put(c.payload+0x30,c.byte(CountTable+uint32_t(kind)));c.put(c.payload+0x34,0);c.real(c.payload+0x38,zero);
        std::array<uint8_t,6> colors{};
        if(kind==0)colors={255,255,255,255,255,255};
        else if(kind==1)colors={255,255,0,255,0,0};
        else if(kind==2)colors={255,255,0,200,255,230};
        else colors={0,255,255,0,0,230};
        for(unsigned i=0;i<6;++i)c.putByte(c.payload+0x3c+i,colors[i]);
        c.real(c.payload+0x28,scale);c.particle=c.payload+0x44;
        c.bias=c.wide(0x802fb0b8);c.zero=c.real(Zero);c.phase=2;break;}
    // Particle slots1..count-1 only,one raw RNG call per slot. Successive360
    // numerators divide by count-1;loop skip for count<=1 avoids that divide.
    // Preserve source duplicate zero stores and child alpha255;do not fill root2c.
    case 2:{
        if(int32_t(c.index)>=c.count){const auto effect=c.effect;cursor.state_.reset();return effect;}
        c.pending=0x8026c324;if(!s.randomRaw)return {};
        const auto draw=s.randomRaw(s.user);if(!draw)return {};
        const auto direction=signedWord(c.angle)/(c.count-1);
        c.real(c.particle+4,c.zero);c.real(c.particle+8,c.zero);
        c.angle+=360;c.real(c.particle+0xc,c.zero);++c.index;
        c.real(c.particle+0x10,c.zero);c.real(c.particle+0x14,c.zero);
        c.real(c.particle+0x20,c.zero);c.put(c.particle,0);
        c.real(c.particle+0x1c,convert(uint32_t(direction),true,c.bias));
        c.real(c.particle+0x24,convert(uint32_t(*draw%360),true,c.bias));
        c.real(c.particle+0x10,c.zero);c.put(c.particle+0x2c,255);
        c.particle+=0x44;break;}
    default:return {};
    }
}
struct HitN64MainCursor::State:Memory {uint32_t phase=0,pending=0,effect=0,payload=0;std::array<float,3> position{};float depth=0;};
HitN64MainCursor::HitN64MainCursor()=default;HitN64MainCursor::~HitN64MainCursor()=default;
bool HitN64MainCursor::active()const{return bool(state_);}uint32_t HitN64MainCursor::pending()const{return state_?state_->pending:0;}
bool HitN64MainCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Copy root XYZ,then decrement life/increment age before expiry test. Negative
// life holds at real Delete5ea30. Otherwise source byte animation*1/256 and
// radius update particles with fresh record count;no render-driven second tick.
// Depth1086c uses captured XYZ,then queue camera4/class2/callback1c5d6c.
bool resumeHitN64Main(uint32_t effect,const HitN64Services& s,HitN64MainCursor& cursor){
    if(cursor.invoking_ || !valid(s,false))return false;
    if(cursor.state_ && (cursor.state_->effect!=effect || !same(cursor.state_->services,s)))return false;
    Guard guard(cursor.invoking_);
    if(!cursor.state_){cursor.state_=std::make_unique<HitN64MainCursor::State>();cursor.state_->services=s;cursor.state_->effect=effect;}
    auto& c=*cursor.state_;const auto finish=[&](){cursor.state_.reset();return true;};
    for(;;)switch(c.phase){
    case 0:{
        c.payload=c.word(effect+0xc);for(unsigned i=0;i<3;++i)(void)c.word(0x802fb0a0+4*i);
        for(unsigned i=0;i<3;++i)c.position[i]=c.real(c.payload+4+4*i);
        const auto remaining=c.word(c.payload+0x30),kind=c.word(c.payload);
        c.put(c.payload+0x30,remaining-1);c.put(c.payload+0x34,c.word(c.payload+0x34)+1);
        if(signedWord(c.word(c.payload+0x30))<0){c.phase=1;break;}
        const auto age=c.word(c.payload+0x34);
        if(signedWord(age)<=c.byte(CountTable+kind)){
            const auto animation=c.byte(Anim+age-1);const auto bias=c.wide(0x802fb0c0);const auto factor=c.real(Factor);
            const auto value=convert(animation,false,bias);volatile float progress=value*factor;c.real(c.payload+0x38,progress);
        }else c.real(c.payload+0x38,c.real(One));
        const auto zero=c.real(Zero);const auto bias=c.wide(0x802fb0c0);
        for(uint32_t i=1;signedWord(i)<signedWord(c.word(effect+8));++i){
            const auto row=signedWord(age)>c.byte(CountTable+kind)?12u:age-1;
            const auto radius=c.byte(Radius+row);const auto particle=c.payload+0x44*i;
            c.real(particle+4,convert(radius,false,bias));c.real(particle+8,zero);c.real(particle+0xc,zero);
        }
        c.phase=2;break;}
    case 1:
        c.pending=0x8005ea30;if(!s.effectDelete || !s.effectDelete(s.user,effect))return false;return finish();
    case 2:{
        c.pending=0x8001086c;if(!s.depth)return false;const auto depth=s.depth(s.user,c.position);if(!depth)return false;
        c.depth=*depth;c.phase=3;break;}
    case 3:
        c.pending=0x80010bc4;if(!s.display || !s.display(s.user,4,2,0x801c5d6c,effect,c.depth))return false;return finish();
    default:return false;
    }
}
}
