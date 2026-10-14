#include "battle_audience_population.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
uint32_t record(int32_t i,const BattleActorServices& a){return base(a)+0x1bc+uint32_t(i)*0x134;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
uint32_t stage(const BattleActorServices& a){return currentBattleAddress(a)+0x163fc;}
float scalar(uint32_t address,const BattleActorServices& a){return number(a.memory.word(a.memory.user,address));}
void copyVector(uint32_t from,uint32_t to,const BattleActorServices& a){
    const auto& m=a.memory;const auto x=m.word(m.user,from),y=m.word(m.user,from+4);
    m.storeWord(m.user,to,x);m.storeWord(m.user,to+4,y);
    m.storeWord(m.user,to+8,m.word(m.user,from+8));
}
bool freeSeat(int32_t slot,const BattleActorServices& a){
    const auto& m=a.memory;
    if(m.word(m.user,record(slot,a))&1)return false;
    return !(m.word(m.user,record(slot,a))&0x100);
}
}
std::optional<int32_t> packedBattleAudienceFreeSeat(bool front,const BattleAudiencePopulationServices& s){
    const auto& a=s.actors;const auto& m=a.memory;assert(m.word && m.half);
    (void)base(a);const auto p=m.word(m.user,0x8041eb00);const auto rank=int16_t(m.half(m.user,p+0x88));
    std::array<int32_t,10> rowWeight{};std::array<int32_t,20> columnWeight{};
    for(unsigned i=0;i<10;++i)rowWeight[i]=integer(m.word(m.user,(front?0x802fa1ac:0x802fa134)+4*i));
    for(unsigned i=0;i<20;++i)columnWeight[i]=integer(m.word(m.user,(front?0x802fa1d4:0x802fa15c)+4*i));
    const uint32_t table=(front?0x8039cc20u:0x8039cb80u)+uint32_t(int32_t(rank))*40;
    std::array<int32_t,200> seats{},cumulative{};unsigned count=0;uint32_t total=0;
    for(int32_t row=0;row<10;++row){
        const auto width=integer(m.word(m.user,table+4*row));
        if(width<0 || width>20)return {}; // Outside the original seating-table domain.
        int32_t column=(20-width)/2;
        for(int32_t i=0;i<integer(m.word(m.user,table+4*row));++i,++column){
            const auto slot=column+20*row;
            if(freeSeat(slot,a)){
                const auto weight=uint32_t(rowWeight[row])*uint32_t(columnWeight[column])+1u;
                total+=weight;seats[count]=slot;cumulative[count]=integer(total);++count;
            }
        }
    }
    if(!s.randomRaw || !s.truncate)return {};
    const auto raw=s.randomRaw(s.user);if(!raw)return {};
    const float product=float(integer(total))*float(*raw);
    const auto draw=s.truncate(s.user,product/scalar(0x804249e4,a));if(!draw)return {};
    for(unsigned i=0;i<count;++i)if(*draw<cumulative[i])return seats[i];
    return -1; // rand()==32767 can select the exclusive cumulative endpoint.
}
std::optional<int32_t> packedBattleAudienceFreeFrontThenBack(const BattleAudiencePopulationServices& s){
    const auto& m=s.actors.memory;
    if(integer(m.word(m.user,base(s.actors)+0x13784))>=20)return packedBattleAudienceFreeSeat(false,s);
    const auto first=packedBattleAudienceFreeSeat(true,s);if(!first || *first!=-1)return first;
    return packedBattleAudienceFreeSeat(false,s);
}
std::optional<int32_t> packedBattleAudienceFreeSeatUniform(const BattleAudiencePopulationServices& s){
    const auto& a=s.actors;const auto& m=a.memory;assert(m.word && m.half && s.random);
    (void)base(a);const auto p=m.word(m.user,0x8041eb00);const auto rank=int16_t(m.half(m.user,p+0x88));
    const uint32_t table=0x8039cb80u+uint32_t(int32_t(rank))*40;
    std::array<int32_t,200> seats{};unsigned count=0;
    for(int32_t row=0;row<10;++row){
        const auto width=integer(m.word(m.user,table+4*row));if(width<0 || width>20)return {};
        int32_t column=(20-width)/2;
        for(int32_t i=0;i<integer(m.word(m.user,table+4*row));++i,++column)
            if(freeSeat(column+20*row,a))seats[count++]=column+20*row;
    }
    if(!count)return -1;
    return seats[s.random(s.user,int32_t(count))];
}
void packedBattleAudienceSetAnimation(int32_t slot,int32_t animation,int32_t force,const BattleActorServices& a){
    const auto& m=a.memory;(void)base(a);const auto r=record(slot,a),flags=m.word(m.user,r);
    if(!(flags&1) || (flags&0x100))return;
    const auto old=m.word(m.user,r+0x24),selected=m.word(m.user,m.word(m.user,r+0x20)+uint32_t(animation)*4);
    if(old && old==selected && force==0)return;
    m.storeWord(m.user,r+0x24,selected);m.storeWord(m.user,r+0x28,selected);m.storeWord(m.user,r+0x2c,0);
    for(const auto offset:{0x34u,0x38u,0x3cu})m.storeWord(m.user,r+offset,0);
    m.storeByte(m.user,r+0x40,0);
}
bool packedBattleAudienceEntrySub(int32_t slot,uint8_t type,int32_t appearance,const BattleAudiencePopulationServices& s){
    const auto& a=s.actors;const auto& m=a.memory;assert(m.word && m.storeWord && m.storeHalf && m.storeByte && a.byte && s.fma);
    (void)base(a);const auto st=stage(a),r=record(slot,a);
    m.storeWord(m.user,r+4,0);m.storeWord(m.user,r+8,0);
    m.storeByte(m.user,r+0x19,uint8_t(appearance));m.storeByte(m.user,r+0x1b,type);
    m.storeHalf(m.user,r+0x1c,0xffff);m.storeWord(m.user,r+0x20,m.word(m.user,0x803b4de8+uint32_t(type)*32));
    m.storeWord(m.user,r+0x30,0);packedBattleAudienceSetAnimation(slot,1,1,a);
    for(const auto offset:{0x40u,0x43u,0x42u})m.storeByte(m.user,r+offset,0);
    const auto depthStep=scalar(0x804249e8,a);
    m.storeByte(m.user,r+0x41,0);m.storeByte(m.user,r+0x44,255);
    const auto row=slot/20,column=slot-row*20;const auto y=scalar(st+0xb34,a);
    m.storeWord(m.user,r+0x4c,bits(y));m.storeWord(m.user,r+0xb8,bits(y));
    const auto z=s.fma(s.user,depthStep,float(column),float(integer(uint32_t(row)*30u+115u)));
    m.storeWord(m.user,r+0x50,bits(z));m.storeWord(m.user,r+0xbc,bits(z));
    if(a.byte(a.user,r+0x1b)==11){
        m.storeWord(m.user,r+0xbc,bits(scalar(r+0xbc,a)+scalar(0x804249ec,a)));
        m.storeWord(m.user,r+0x50,bits(scalar(r+0x50,a)+scalar(0x804249ec,a)));
    }
    if(a.byte(a.user,r+0x19)==11)m.storeWord(m.user,r+0x4c,m.word(m.user,0x804249f0));
    const auto flip=m.word(m.user,0x804249f4);
    copyVector(0x802fa0b0,r+0x84,a);copyVector(r+0x84,r+0x78,a);
    for(const auto pair:{std::pair<uint32_t,uint32_t>{0x802fa0bc,0x90},{0x802fa0c8,0x9c},
        {0x802fa0d4,0xa8},{0x802fa0e0,0x54},{0x802fa0ec,0x60},{0x802fa0f8,0x6c}})copyVector(pair.first,r+pair.second,a);
    m.storeWord(m.user,r+0xe8,flip);
    if((slot>=4 && slot<=15) || (slot>=24 && slot<=35) || (slot>=44 && slot<=55))m.storeWord(m.user,r,m.word(m.user,r)|8);
    return true;
}
std::optional<int32_t> packedBattleAudienceEntry(int32_t slot,int32_t type,int32_t appearance,const BattleAudiencePopulationServices& s){
    const auto& a=s.actors;const auto& m=a.memory;assert(m.word && m.storeWord && a.byte);
    (void)base(a);(void)stage(a);const auto r=record(slot,a);
    const bool paired=(a.byte(a.user,0x803b4de4+(uint32_t(type)&255u)*32)&1)!=0;
    if(paired && slot%2==1)return 0; // Source signed remainder rejects positive odd slots only.
    const auto flags=m.word(m.user,r);if(flags&1)return 0;if(flags&0x100)return 0;
    m.storeWord(m.user,r,paired?0x85:5);
    if(!s.randomRaw)return {};const auto raw=s.randomRaw(s.user);if(!raw)return {};
    const int32_t column=slot-(slot/20)*20,jitter=integer(uint32_t(*raw)*10)/32767;
    float x;
    if(!paired)x=float(column*25+jitter-235);
    else{const float offset=scalar(0x804249f8,a)+float(column*25-230);const float withJitter=offset+float(jitter);x=withJitter-scalar(0x8042499c,a);}
    m.storeWord(m.user,r+0x48,bits(x));m.storeWord(m.user,r+0xb4,bits(x));
    if(!packedBattleAudienceEntrySub(slot,uint8_t(type),appearance,s))return {};
    if(paired)m.storeWord(m.user,r+0x134,0x101);
    return 1;
}
void packedBattleAudienceChangeStatus(int32_t slot,int32_t state,const BattleActorServices& a){
    const auto& m=a.memory;(void)base(a);const auto r=record(slot,a),flags=m.word(m.user,r);
    if(!(flags&1))return;
    if(flags&0x100){if(slot>=1)packedBattleAudienceChangeStatus(slot-1,state,a);return;}
    m.storeByte(m.user,r+0x43,0);const auto zero=m.word(m.user,0x80424988);
    for(const auto offset:{0x42u,0x41u})m.storeByte(m.user,r+offset,0);
    m.storeByte(m.user,r+0x44,255);
    for(const auto pair:{std::pair<uint32_t,uint32_t>{0x802fa104,0x9c},{0x802fa110,0x6c},
        {0x802fa11c,0x60},{0x802fa128,0x78}})copyVector(pair.first,r+pair.second,a);
    for(const auto offset:{0x30u,4u,8u})m.storeWord(m.user,r+offset,0);
    for(const auto offset:{0x14u,0x10u,0xcu})m.storeWord(m.user,r+offset,zero);
    m.storeByte(m.user,r+0x19,uint8_t(state));
}
void packedBattleAudienceDelete(int32_t slot,const BattleActorServices& a){
    const auto& m=a.memory;(void)base(a);const auto r=record(slot,a);
    if(m.word(m.user,r)&0x80)m.storeWord(m.user,r+0x134,0);
    m.storeWord(m.user,r,0);
}
int32_t packedBattleAudienceAnimationEnded(int32_t slot,const BattleActorServices& a){
    const auto& m=a.memory;const auto r=record(slot,a);
    if(integer(m.word(m.user,m.word(m.user,r+0x28)))!=-1)return 0;
    const auto timer=m.word(m.user,r+0x2c);return timer==0 || (timer&0x80000000u)?1:0;
}
bool initializePackedBattleAudiencePopulation(const BattleAudiencePopulationServices& s){
    const auto& a=s.actors;const auto& m=a.memory;
    assert(m.word && m.half && m.storeWord && a.byte && s.random && s.fma && s.truncate);
    const auto battle=currentBattleAddress(a),owner=base(a);
    const auto setup=m.word(m.user,m.word(m.user,currentBattleAddress(a)+0x2738)+0xc)+0xac;
    const auto pouch=m.word(m.user,0x8041eb00);
    std::array<uint32_t,65> levels{};for(unsigned i=0;i<65;++i)levels[i]=m.word(m.user,0x802f9eac+4*i);
    const auto rank=int16_t(m.half(m.user,pouch+0x88));
    if(rank>=0 && rank<=3)m.storeWord(m.user,owner+0x13790,50u*(rank+1));
    if(m.half(m.user,m.word(m.user,0x8041eb00)+0x8c)==0)m.storeWord(m.user,owner+0x13790,0);
    const auto level=int16_t(m.half(m.user,pouch+0x8a));
    if(level<1)return false; // Original negative local-table index is outside this typed domain.
    m.storeWord(m.user,owner+0x13778,levels[std::min<int32_t>(level-1,64)]);
    if(m.word(m.user,battle+0x19078)&0x10000){
        m.storeWord(m.user,owner+0x1377c,bits(float(integer(m.word(m.user,battle+0x19084)))));
        const auto cap=integer(m.word(m.user,owner+0x13790));
        if(scalar(owner+0x1377c,a)>float(cap))m.storeWord(m.user,owner+0x1377c,bits(float(cap)));
    }else{
        const auto q0=scalar(m.word(m.user,0x8041eb00)+0x84,a);
        const auto first=s.random(s.user,10),second=s.random(s.user,10);
        const auto q1=scalar(m.word(m.user,0x8041eb00)+0x84,a);
        const auto delta=q1-scalar(owner+0x13778,a);const float product=delta*float(first+second-13);
        m.storeWord(m.user,owner+0x1377c,bits(s.fma(s.user,scalar(0x80424998,a),product,q0)));
    }
    const auto mode=integer(m.word(m.user,setup));
    if(mode==0 || mode==1)m.storeWord(m.user,owner+0x13780,m.word(m.user,0x80424988));
    else if(mode==4)m.storeWord(m.user,owner+0x13780,bits(scalar(0x80424a78,a)*scalar(owner+0x1377c,a)));
    else if(mode>=2 && mode<=6)m.storeWord(m.user,owner+0x13780,bits(float(integer(m.word(m.user,owner+0x13790)))-scalar(owner+0x1377c,a)));
    std::array<int32_t,12> weight{},order{};bool guest=false;
    const auto paired=[&](int32_t type){return (a.byte(a.user,0x803b4de4+uint32_t(type)*32)&1)!=0;};
    const auto weightMode=integer(m.word(m.user,setup));
    if(weightMode==2){weight[1]=weight[7]=1;}
    else if(weightMode==5){weight[1]=1;}
    else if(weightMode==6){for(int32_t type=0;type<12;++type)weight[type]=paired(type)?2:1;guest=true;}
    else{
        std::array<int32_t,11> thresholds{};
        for(unsigned i=0;i<11;++i)thresholds[i]=integer(m.word(m.user,0x802f9fb0+4*i));
        const auto draw=s.random(s.user,100);int32_t cardinality=12;uint32_t cumulative=0;
        for(unsigned i=0;i<11;++i){cumulative+=uint32_t(thresholds[i]);if(draw<integer(cumulative)){cardinality=i+1;break;}}
        for(int32_t i=0;i<12;++i)order[i]=i;
        for(unsigned i=0;i<24;++i){const auto first=s.random(s.user,12),second=s.random(s.user,12);std::swap(order[first],order[second]);}
        int32_t admitted=0,lastType=0;
        for(const auto type:order){
            lastType=type;
            if(admitted>=cardinality)continue;
            const auto byte=a.byte(a.user,setup+4+type);if(byte==0)continue;
            const auto multiplier=s.random(s.user,10)+1;
            const auto currentByte=a.byte(a.user,setup+4+type);
            const auto signedWeight=currentByte<128?int32_t(currentByte):int32_t(currentByte)-256;
            weight[type]=integer(uint32_t(signedWeight)*uint32_t(multiplier));
            if(paired(type))weight[type]=integer(uint32_t(weight[type])*2u);
            ++admitted;
        }
        const auto override=m.word(m.user,battle+0x19088);
        if(override){
            if(override<=12){
                // Source jump-table0 preserves r28; >12 likewise retains the last scanned type.
                constexpr int32_t choices[]{0,0,10,8,4,7,2,1,3,5,6,9,11};
                lastType=choices[override];
            }
            weight.fill(0);weight[lastType]=1;
        }
        guest=true;
    }
    std::array<int32_t,13> allocation{};
    const auto converted=s.truncate(s.user,scalar(owner+0x1377c,a)+scalar(owner+0x13780,a));if(!converted)return false;
    int32_t target=*converted;if(target<0)target=0;
    const auto capacity=integer(m.word(m.user,owner+0x13790));if(target>capacity)target=capacity;
    uint32_t total=0;for(const auto value:weight)total+=uint32_t(value);
    const int32_t divisor=integer(total);int32_t allocated=0;
    for(int32_t type=0;type<12;++type){
        const auto numerator=integer(uint32_t(target)*uint32_t(weight[type]));
        if(divisor==0 || (numerator==INT32_MIN && divisor==-1))return false;
        const bool pair=paired(type);allocation[type]=numerator/divisor;
        if(pair){allocation[type]/=2;allocated=integer(uint32_t(allocated)+uint32_t(allocation[type])*2u);}
        else allocated=integer(uint32_t(allocated)+uint32_t(allocation[type]));
    }
    for(int32_t i=0;i<12;++i)order[i]=i;
    for(unsigned i=0;i<11;++i)for(unsigned j=i+1;j<12;++j){
        if(weight[i]==weight[j]){if(s.random(s.user,2)==0){std::swap(order[i],order[j]);continue;}}
        if(weight[i]<weight[j])std::swap(order[i],order[j]);
    }
    int32_t cursor=0,failures=0;
    while(allocated<target){
        const auto type=order[cursor];
        if(weight[type]>0){
            if(paired(type)){
                if(allocated<=target-2){++allocation[type];allocated+=2;failures=0;}
                else if(++failures>=12)break;
            }else{++allocation[type];++allocated;}
        }
        cursor=(cursor+1)%12;
    }
    if(guest && allocated!=0){
        const auto draw=s.random(s.user,20);
        if(draw==0 || (m.word(m.user,battle+0x19078)&0x40000)){
            for(int32_t i=0;i<12;++i)order[i]=i;
            for(unsigned i=0;i<24;++i){const auto first=s.random(s.user,12),second=s.random(s.user,12);std::swap(order[first],order[second]);}
            for(const auto type:order)if(allocation[type]>0){--allocation[type];++allocation[12];break;}
        }
    }
    if(integer(m.word(m.user,setup))==2){
        for(;;){const auto slot=packedBattleAudienceFreeSeatUniform(s);if(!slot)return false;if(*slot==-1)break;
            if(!packedBattleAudienceEntry(*slot,*slot%20<=9?7:1,0,s))return false;}
    }else{
        std::array<int32_t,200> types{};unsigned length=0;
        for(int32_t type=0;type<=12;++type)for(int32_t i=0;i<allocation[type];++i){
            if(length>=types.size())return false; // Original local seat-list capacity, no invented overflow.
            types[length++]=type;
        }
        for(unsigned i=0;i<150;++i){const auto first=s.random(s.user,int32_t(length)),second=s.random(s.user,int32_t(length));
            // Original L0 still makes300 RNG requests; unwritten local swaps have no later read.
            if(length)std::swap(types[first],types[second]);}
        for(unsigned i=0;i<length;++i){
            const auto slot=packedBattleAudienceFreeFrontThenBack(s);if(!slot)return false;if(*slot==-1)break;
            if(!packedBattleAudienceEntry(*slot,uint8_t(types[i]),0,s))return false;
        }
    }
    for(unsigned i=0;i<12;++i)m.storeWord(m.user,owner+0x13794+4*i,uint32_t(weight[i]));
    return true;
}
}
