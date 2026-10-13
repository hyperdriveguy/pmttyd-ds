#include "battle_drop_material.hpp"
#include <cassert>
namespace ttyd {
namespace {
uint32_t word(const BattleDropMaterialServices& s,uint32_t a){return s.actors.memory.word(s.actors.memory.user,a);}
int32_t signedWord(uint32_t v){return v<0x80000000?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t half(const BattleDropMaterialServices& s,uint32_t a){const auto v=s.actors.memory.half(s.actors.memory.user,a);return v<0x8000?int32_t(v):int32_t(v)-65536;}
void store(const BattleDropMaterialServices& s,uint32_t a,uint32_t v){s.actors.memory.storeWord(s.actors.memory.user,a,v);}
int32_t random(const BattleDropMaterialServices& s,int32_t bound){return s.random(s.user,bound);}
void resourceDrops(const BattleDropMaterialServices& s,uint32_t table,uint32_t output,int32_t current,int32_t maximum){
    assert(maximum!=0);
    int32_t ratio=current*100/maximum;if(ratio>100)ratio=100;
    store(s,output,0);
    for(;;table+=16){const auto threshold=signedWord(word(s,table));if(!threshold)return;if(ratio>threshold)continue;
        const int32_t draw=random(s,100),chance=signedWord(word(s,table+4));
        int32_t count=signedWord(word(s,table+8));
        if(draw<chance){while(count!=0){if(random(s,100)<signedWord(word(s,table+12)))store(s,output,word(s,output)+1);--count;}}
        return;
    }
}
}
void battleInformationSetDropMaterial(uint32_t info,const BattleDropMaterialServices& s){
    assert(s.actors.memory.word && s.actors.memory.half && s.actors.memory.storeWord && s.random && s.story && s.equippedBadgeCount);
    const auto setup=word(s,info+0xc),battle=currentBattleAddress(s.actors),group=word(s,setup+0x18);
    const auto heartTable=word(s,group+0x14),flowerTable=word(s,group+0x18),mario=battleMarioUnit(battle,s.actors);
    resourceDrops(s,heartTable,setup+0x10,half(s,mario+0x10c),half(s,mario+0x108));
    resourceDrops(s,flowerTable,setup+0x14,half(s,mario+0x112),half(s,mario+0x10e));
    uint32_t held=word(s,group+8),back=word(s,group+0xc),none=word(s,group+0x10);
    if(s.story(s.user)<=19){held=0;back=0;none=100;}
    const auto badges=s.equippedBadgeCount(s.user,314);
    if(badges>=1){held*=uint32_t(badges)+1;back*=uint32_t(badges)+1;}
    int32_t remaining=signedWord(uint32_t(random(s,signedWord(held+none+back)))-back);
    int32_t mode=0;if(remaining<0)mode=1;else if(signedWord(uint32_t(remaining)-held)<0)mode=2;
    store(s,setup+0xc,0);
    if(mode==1){
        auto table=word(s,word(s,group+4)+0x2c);
        if(table){uint32_t sum=0;for(auto row=table;;row+=8){const auto weight=half(s,row+6);if(weight<=0 && !word(s,row))break;sum+=uint32_t(weight);}
            if(signedWord(sum)>0){remaining=random(s,signedWord(sum));for(;;table+=8){remaining=signedWord(uint32_t(remaining)-uint32_t(half(s,table+6)));if(remaining<0){store(s,setup+0xc,word(s,table));break;}}}
        }
    }else if(mode==2){
        int32_t count=0;for(unsigned i=0;i<8;++i)if(word(s,setup+0x1c+4*i))++count;
        if(count>0){remaining=random(s,count);for(unsigned i=0;i<8;++i){const auto item=word(s,setup+0x1c+4*i);if(item && --remaining<0){store(s,setup+0xc,item);break;}}}
    }
    if(!word(s,setup+0xc) && s.equippedBadgeCount(s.user,314)>=1){
        // Original stack copy of 25 source words precedes this RNG request.
        uint32_t items[25];for(unsigned i=0;i<25;++i)items[i]=word(s,0x802eea40+4*i);
        store(s,setup+0xc,items[random(s,25)]);
    }
}
}
