#pragma once
#include <cstdint>
#include <cstring>
#include <functional>
namespace ttyd {
struct FieldIdleServices {
    std::function<int()> hp,rate;
    std::function<int(int)> millisecondsToFrames;
    std::function<bool()> key;
    std::function<const char*()> currentPose;
    std::function<void(const char*,bool)> requestPose; // true selects effect pose.
    std::function<void(int)> sound;
    std::function<void()> adjustMoveDirection;
};
// Signed16 breath/refresh counters wrap explicitly. Early HP/breath work
// precedes ground command dispatch; late sleep work runs only if idle survives.
struct FieldIdleWork {
    int32_t phase=0,count=0,displayFlags=0,mainFlags=0;
    int16_t breathe=0,refresh=0,startFrame=0;
    int32_t sleepElapsed=0;
    static int16_t increment(int16_t value) {
        const uint16_t narrowed=uint16_t(uint16_t(value)+1);
        return int16_t(narrowed>=0x8000?int32_t(narrowed)-65536:int32_t(narrowed));
    }
    void ordinary(const FieldIdleServices& s,const char* name) {
        s.requestPose(name,false);
    }
    // HP<=5 uses M_S_3 breathing; HP0 M_I_S and cue 371. Main 80000000
    // special branch retains ordinary pose; main 2000 selects M_U_2/frame 10.
    // IDs are original sound-service tokens, not native PCM slot indices.
    void enter(const FieldIdleServices& s) {
        phase=count=0;refresh=0;
        if(uint32_t(mainFlags)&0x80000000u){ordinary(s,"M_S_1");return;}
        if(mainFlags&0x2000){s.requestPose("M_U_2",false);startFrame=10;return;}
        if(s.hp()==0){ordinary(s,"M_I_S");s.sound(371);}
        else if(s.hp()<=5 && s.key()) {displayFlags|=4;ordinary(s,"M_S_3");breathe=0;}
        else if(std::strcmp(s.currentPose(),"M_I_Y"))ordinary(s,"M_S_1");
    }
    // Early HP/counter owner, before source ground commands can leave idle.
    // Refresh threshold 1000ms, breath cycle4*rate with cues225/226 at
    // phase 0/2*rate. Refreshing a pose must not queue an animation restart.
    void early(const FieldIdleServices& s) {
        if(uint32_t(mainFlags)&0x80000000u)return;
        const int threshold=s.millisecondsToFrames(1000);
        refresh=increment(refresh);
        if(refresh>threshold && !(mainFlags&0x2000) && s.key()) {
            if(s.hp()<=5) {
                displayFlags|=4;if(std::strcmp(s.currentPose(),"M_S_3"))breathe=0;
                ordinary(s,"M_S_3");
            } else if(phase==0 && refresh!=0) {
                refresh=0;if(std::strcmp(s.currentPose(),"M_I_Y"))ordinary(s,"M_S_1");
            }
        }
        if(!std::strcmp(s.currentPose(),"M_S_3") && s.key()) {
            breathe=increment(breathe);
            if(breathe>=s.rate()*4){breathe=0;startFrame=0;} // No animation restart queue.
            if(breathe==0)s.sound(225);
            else if(breathe==s.rate()*2)s.sound(226);
        }
    }
    // Late owner only for callbacks that survive source command/motion gates.
    // Idle phases: 0 waits50000ms, 1 requests M_I_5, 2 holds50 visits,
    // 3 holds100, 4 sleeps with10000ms elapsed cap/cue 228 every2*rate.
    // Phase1 falls through and decrements50 immediately; do not add a tick.
    void late(const FieldIdleServices& s) {
        if((uint32_t(mainFlags)&0x80000000u) || (mainFlags&0x2000))return;
        if((mainFlags&8) || (uint32_t(displayFlags)&0x40000000u)) {
            if(phase<=0 || !std::strcmp(s.currentPose(),"M_S_1") || !std::strcmp(s.currentPose(),"M_S_1R"))return;
            ordinary(s,"M_S_1");phase=0;return;
        }
        if(s.hp()<=5)return;
        switch(phase) {
        case 0:if(++count>=s.millisecondsToFrames(50000))phase=1;return;
        case 1:s.requestPose("M_I_5",true);s.adjustMoveDirection();s.sound(239);count=50;phase=2;[[fallthrough]];
        case 2:if(--count<0){ordinary(s,"M_S_1");count=100;phase=3;}return;
        case 3:
            if(--count<0){s.requestPose("M_N_5A",true);startFrame=0;count=0;phase=4;sleepElapsed=0;s.sound(228);}return;
        case 4:
            ++sleepElapsed;
            {const int limit=s.millisecondsToFrames(10000);if(sleepElapsed>limit)sleepElapsed=limit;}
            if(++count>=s.rate()*2){count=0;s.sound(228);}
            if((mainFlags&8) || s.hp()<=5){ordinary(s,"M_S_1");phase=0;}return;
        default:return;
        }
    }
};
}
