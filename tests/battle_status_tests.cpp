#include "battle_status.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)
void gettersAndSetters() {
    for(int id=0;id<=27;++id) {
        BattleStatusUnit u;const int strength=id>=10 && id<=15 && id%2?-3:3;
        CHECK(setStatus(u,id,2,strength)==1);
        const auto value=getStatus(u,id);
        if(id==25 || id==26) {CHECK(value.duration==0 && value.strength==0);continue;}
        CHECK(value.duration==(id==16?3:2));
        CHECK(value.strength==uint8_t((id==4 || (id>=10 && id<=16) || id==23 || id==24)?strength:2));
        CHECK(checkStatus(&u,id));
    }
    BattleStatusUnit u;
    CHECK(!checkStatus(nullptr,6));u.zapTap=2;
    CHECK(checkStatus(&u,6) && getStatus(u,6).duration==0 && getStatus(u,6).strength==0);
    CHECK(checkRecoveryStatus(&u,6)==0 && u.zapTap==2);
    u.hp=10;CHECK(setStatus(u,0,2,0)==1 && getStatus(u,256).duration==2);
    const auto before=u.statusBytes;
    for(int id:{-1,28,127,128,255}) {
        CHECK(getStatus(u,id).duration==0 && getStatus(u,id).strength==0);
        CHECK(setStatus(u,id,7,7)==1 && u.statusBytes==before);
    }
    CHECK(setStatus(u,256,0,0)==0 && getStatus(u,0).duration==0);
    CHECK(setStatus(u,21,128,0)==0 && u.statusByte(0x12d)==0);
    CHECK(setStatus(u,21,257,0)==1 && u.statusByte(0x12d)==1);
    u.statusByte(0x119)=100;CHECK(setStatus(u,1,2,0)==1 && u.statusByte(0x119)==0);
    CHECK(setStatus(u,2,255,0)==1 && checkStatus(&u,2)); // Negative duration is present.
}
void pairsAndCancellation() {
    BattleStatusUnit u;
    CHECK(setStatus(u,9,-1,0)==1);
    CHECK(setStatus(u,8,0,0)==0 && u.statusByte(0x122)==0 && u.statusByte(0x121)==0);
    CHECK(setStatus(u,8,3,0)==1);
    CHECK(setStatus(u,9,3,0)==0 && !checkStatus(&u,8) && !checkStatus(&u,9));
    for(int positive:{10,12}) {
        CHECK(setStatus(u,positive,3,4)==1 && checkStatus(&u,positive));
        CHECK(setStatus(u,positive+1,5,-2)==1 && !checkStatus(&u,positive));
        CHECK(getStatus(u,positive+1).duration==5 && getStatus(u,positive+1).strength==254);
        CHECK(setStatus(u,positive,0,0)==0 && checkStatus(&u,positive+1)); // Opposite zero clear leaves it.
        CHECK(setStatus(u,positive+1,0,0)==0 && !checkStatus(&u,positive+1));
        CHECK(setStatus(u,positive,0,-4)==1); // Requested ID does not force strength sign.
        CHECK(getStatus(u,positive+1).duration==0 && getStatus(u,positive+1).strength==252);
        CHECK(!checkStatus(&u,positive+1));
    }
    CHECK(setStatus(u,14,3,4)==1);
    CHECK(setStatus(u,15,5,-2)==0 && !checkStatus(&u,14) && !checkStatus(&u,15));
    CHECK(setStatus(u,15,5,-2)==1);
    CHECK(setStatus(u,14,0,0)==0 && checkStatus(&u,15));
    CHECK(setStatus(u,14,3,4)==0 && !checkStatus(&u,15));
    u.statusByte(0x129)=99;
    CHECK(setStatus(u,16,50,29)==0 && u.statusByte(0x129)==0); // Byte128 clears, not99 clamp.
    CHECK(setStatus(u,16,0,120)==1 && u.statusByte(0x129)==99);
    CHECK(setStatus(u,16,0,-1)==1 && u.statusByte(0x129)==98);
    CHECK(setStatus(u,16,0,258)==1 && u.statusByte(0x129)==99); // Low-byte arithmetic before clamp.
}
void recovery() {
    BattleStatusUnit u;u.hp=10;
    CHECK(checkRecoveryStatus(nullptr,1)==0);
    setStatus(u,1,3,0);u.statusByte(0x119)=100;
    CHECK(checkRecoveryStatus(&u,1)==0 && getStatus(u,1).duration==2 && u.statusByte(0x119)==100);
    CHECK(checkRecoveryStatus(&u,1)==0 && getStatus(u,1).duration==1);
    CHECK(checkRecoveryStatus(&u,1)==1 && getStatus(u,1).duration==0 && u.statusByte(0x119)==100);
    setStatus(u,1,-3,0);CHECK(checkRecoveryStatus(&u,1)==1 && getStatus(u,1).duration==0);
    setStatus(u,4,2,-3);CHECK(checkRecoveryStatus(&u,4)==0 && getStatus(u,4).strength==253);
    CHECK(checkRecoveryStatus(&u,4)==1 && getStatus(u,4).strength==0);
    setStatus(u,12,-2,3);CHECK(checkRecoveryStatus(&u,12)==0 && getStatus(u,12).duration==255);
    CHECK(checkRecoveryStatus(&u,12)==1 && getStatus(u,12).strength==0);
    setStatus(u,23,2,5);CHECK(checkRecoveryStatus(&u,23)==0 && getStatus(u,23).strength==5);
    CHECK(checkRecoveryStatus(&u,23)==1 && getStatus(u,23).strength==0);
    setStatus(u,16,0,3);setStatus(u,17,3,0);
    CHECK(checkRecoveryStatus(&u,16)==0 && getStatus(u,16).duration==3);
    CHECK(checkRecoveryStatus(&u,17)==0 && getStatus(u,17).duration==3);
    setStatus(u,1,1,0);u.statusByte(0x119)=100;u.hp=0;
    CHECK(checkRecoveryStatus(&u,1)==0 && getStatus(u,1).duration==1);
    u.hp=10;setStatus(u,27,1,0);
    CHECK(checkRecoveryStatus(&u,1)==0 && getStatus(u,1).duration==1 && u.statusByte(0x119)==100);
    setStatus(u,27,0,0);CHECK(checkRecoveryStatus(&u,1)==1);
    u.statusByte(0x12a)=254; // Negative Allergic duration cleared by its setter, but local T!=0.
    CHECK(checkRecoveryStatus(&u,0)==0 && !checkStatus(&u,0));
    u.statusByte(0x121)=u.statusByte(0x122)=2; // Getter/step/setter ordering on externally written state.
    CHECK(checkRecoveryStatus(&u,8)==0 && u.statusByte(0x121)==2 && u.statusByte(0x122)==0);
    CHECK(checkRecoveryStatus(&u,8)==0 && u.statusByte(0x121)==1);
    CHECK(checkRecoveryStatus(&u,8)==1 && u.statusByte(0x121)==0);
}
void moveCounts() {
    BattleStatusUnit u;u.moveBytes={99,3,0,0,0};resetMoveStatus(u);
    CHECK(u.moveBytes[0]==0 && u.moveBytes[2]==3 && u.moveBytes[3]==3);
    CHECK(setStatus(u,19,3,0)==1 && u.moveBytes[2]==6 && u.moveBytes[3]==6);
    CHECK(setStatus(u,20,3,0)==0 && !checkStatus(&u,19) && !checkStatus(&u,20));
    CHECK(u.moveBytes[2]==3 && u.moveBytes[3]==3);
    u.moveBytes[3]=2;CHECK(setStatus(u,19,3,0)==1 && u.moveBytes[2]==3 && u.moveBytes[3]==2);
    u.flags=0x1000;u.moveBytes[3]=3;CHECK(setStatus(u,19,0,0)==1 && u.moveBytes[2]==3);
    u.flags=0;u.statusBytes={};u.moveBytes={0,1,1,1,1};setStatus(u,20,3,0);
    CHECK(u.moveBytes[2]==0 && u.moveBytes[3]==0 && u.moveBytes[0]==10);
    u.moveBytes[4]=2;resetMoveStatus(u);CHECK(u.moveBytes[2]==1 && u.moveBytes[0]==0);
    u.moveBytes[1]=0;u.moveBytes[4]=3;resetMoveStatus(u);CHECK(u.moveBytes[2]==0);
    u.moveBytes[4]=4;resetMoveStatus(u);CHECK(u.moveBytes[2]==1);
    u.statusByte(0x12f)=1;u.moveBytes[1]=64;u.moveBytes[4]=1;resetMoveStatus(u);
    CHECK(u.moveBytes[2]==0); // Fast wraps to signed-128, Slow takes low-count branch.
    u.statusByte(0x130)=0;resetMoveStatus(u);CHECK(u.moveBytes[2]==128 && u.moveBytes[0]==0);
    u.moveBytes={7,3,2,1,9};setMaxMoveCount(u,5);
    CHECK(u.moveBytes[1]==5 && u.moveBytes[2]==2 && u.moveBytes[3]==1 && u.moveBytes[0]==7);
    setMaxMoveCount(u,1);CHECK(u.moveBytes[2]==1 && u.moveBytes[3]==1);
    setMaxMoveCount(u,128);CHECK(u.moveBytes[1]==128 && u.moveBytes[2]==128);
    u.statusBytes={};u.moveBytes={0,1,1,1,1};u.statusByte(0x130)=1;
    CHECK(checkRecoveryStatus(&u,20)==0); // HP0 prevents a step.
    u.hp=10;CHECK(checkRecoveryStatus(&u,20)==1 && u.moveBytes[2]==1);
}
void eligibility() {
    CHECK(canActStatus(nullptr) && canGuardStatus(nullptr));
    BattleStatusUnit u;CHECK(canActStatus(&u) && actionCommandPossible(u)); // No HP/count check.
    for(int id:{1,2,9,27}) {
        setStatus(u,id,1,0);CHECK(!canActStatus(&u) && !canGuardStatus(&u) && !actionCommandPossible(u));
        setStatus(u,id,0,0);
    }
    u.statusFlags=1;CHECK(!canActStatus(&u));u.statusFlags=0;
    u.belong=-1;CHECK(canActStatus(&u) && !actionCommandPossible(u));
    u.belong=0;u.kind=223;CHECK(!actionCommandPossible(u));u.kind=222;CHECK(actionCommandPossible(u));
}
int main() {
    try {gettersAndSetters();pairsAndCancellation();recovery();moveCounts();eligibility();std::puts("Status/move-count contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}
