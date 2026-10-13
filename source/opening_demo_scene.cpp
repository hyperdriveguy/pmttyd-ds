#include "opening_demo_scene.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace ttyd {
void OpeningDemoScene::program() {
    auto wait=[&](unsigned ms){requests.push_back({Kind::Wait,0,{},{},(uint64_t(ms)*60+999)/1000});};
    auto camera=[&](std::array<float,3> e,std::array<float,3> a,unsigned ms){Request r{Kind::Camera};r.point=e;r.aim=a;r.duration=(uint64_t(ms)*60+999)/1000;requests.push_back(r);};
    auto move=[&](unsigned actor,float x,float z,float speed){requests.push_back({Kind::Move,actor,{x,speed,z}});};
    auto message=[&](unsigned n){requests.push_back({Kind::Message,n});};
    auto tribe=[&](unsigned n){requests.push_back({Kind::Tribe,n});};
    auto pose=[&](unsigned a,const char* p,const char* talk=nullptr){Request r{Kind::Pose,a};r.pose=p;r.talk=talk;requests.push_back(r);};
    auto reverse=[&](){requests.push_back({Kind::Reverse,0});};
    // Source adjust0..1/3000ms occurs before label100's1000ms wait.
    wait(3000);wait(1000);move(0,0,450,60);wait(1000);message(0);
    wait(300);camera({0,100,834},{0,50,437},800);wait(1000);message(1);
    tribe(0);wait(300);camera({0,200,837},{0,200,437},4000);wait(300);
    for(unsigned i=0;i<4;++i){reverse();wait(500);}message(2);
    wait(300);wait(1000);message(3);wait(300);camera({0,89,739},{0,44,442},0);
    requests.push_back({Kind::Wait,0,{},{},1});wait(300);pose(0,"P_B_1");wait(300);
    pose(0,"P_B_2");wait(300);reverse();wait(800);pose(0,"P_B_2","P_T_7");message(4);
    wait(300);camera({-100,89,739},{-100,44,442},1000);wait(1000);tribe(0);move(0,-55,450,60);message(5);
    wait(300);wait(300);message(6);wait(300);tribe(3);wait(1000);pose(0,"P_H_1");wait(1500);
    pose(0,"P_S_7");tribe(1);wait(500);camera({-120,89,739},{-120,44,442},1000);
    move(0,-80,450,60);wait(500);message(7);wait(300);wait(500);
    requests.push_back({Kind::Lift,2,{},{},60});message(8);wait(300);pose(1,"A_1");wait(700);pose(1,"S_1");tribe(3);message(9);
    wait(1000);camera({-450,250,1030},{-135,100,482},0);tribe(1);requests.push_back({Kind::Walls});wait(300);message(10);
    wait(300);wait(500);message(11);wait(300);requests.push_back({Kind::Handoff});
    move(1,-125,450,40);wait(300);wait(300);move(1,-145,450,40);wait(300);
    requests.push_back({Kind::Reverse,2,{225,0,0}});camera({-90,89,739},{-90,44,442},0);
    requests.push_back({Kind::Wait,0,{},{},1});message(12);wait(300);
    camera({-85,50,590},{-85,28,442},15000);wait(1000);pose(2,"A_1");wait(1000);
    requests.push_back({Kind::Open});wait(2000);
}
void OpeningDemoScene::pose(unsigned i,const char* p,const char* talk) {actors[i].pose=p;actors[i].talkPose=talk;actors[i].poseTick=tick;}
void OpeningDemoScene::tribe(unsigned n) {
    static const char* idles[]={"P_S_1","P_S_7","P_S_8","P_S_4"};
    static const char* talks[]={"P_T_1","P_T_13","P_T_14","P_T_4"};
    static const char* walks[]={"P_W_1","P_W_7","P_W_8","P_W_4"};
    idle[0]=idles[n];walk[0]=walks[n];pose(0,idles[n],talks[n]);
}
void OpeningDemoScene::begin(uint64_t simulationTick) {
    *this=OpeningDemoScene{};tick=beginTick=simulationTick;active=true;
    actors[0].position={-2,0,550};actors[0].yaw=90;actors[0].visible=true;tribe(1);
    actors[1].position={-145,0,450};actors[1].yaw=90;actors[1].visible=true;pose(1,"S_1","T_1");
    actors[2].position={-125,0,450};actors[2].visible=true;pose(2,"S_1");program();step();
}
void OpeningDemoScene::move(unsigned i,std::array<float,3> target,float speed) {
    auto& a=actors[i];auto& m=motion[i];m.start=a.position;m.target=target;m.target[1]=a.position[1];
    const float dx=target[0]-a.position[0],dz=target[2]-a.position[2];
    const float distance=std::hypot(dx,dz);m.begin=tick;m.end=tick+std::max<uint64_t>(1,uint64_t(std::ceil(distance*60/speed)));m.active=true;a.moving=true;
    a.yaw=std::atan2(dx,-dz)*180/3.14159265358979323846f;if(a.yaw<0)a.yaw+=360;pose(i,walk[i],a.talkPose);
}
void OpeningDemoScene::update() {
    auto advance=[&](Motion& m,std::array<float,3>& p){if(!m.active)return;const float t=std::min(1.f,float(tick-m.begin)/float(m.end-m.begin));for(unsigned i=0;i<3;++i)p[i]=m.start[i]+(m.target[i]-m.start[i])*t;if(t==1)m.active=false;};
    for(unsigned i=0;i<3;++i){const bool moving=motion[i].active;advance(motion[i],actors[i].position);actors[i].moving=motion[i].active;
        if(moving && !motion[i].active && i<2)pose(i,idle[i],actors[i].talkPose);
    }
    advance(cameraEye,eye);advance(cameraAt,at);revealProgress=std::min(1.f,float(tick-beginTick)/180);musicStarted=tick-beginTick>=240;
    if(handoff){
        if(handoffStage==0 && tick>=handoffTick+18){move(0,{-100,0,450},40);handoffStage=1;}
        if(handoffStage<2)actors[2].position[0]=actors[1].position[0]+chestOffset;
        if(handoffStage==1 && !motion[0].active){handoffStage=2;handoffSteps=0;}
        if(handoffStage==2){
            // Original sixty WAIT_FRAME1 iterations, ADD_F Q1024 operands.
            actors[2].position[0]+=.0830078125f;actors[2].position[1]+=.166015625f;
            if(++handoffSteps==60){tribe(2);handoffStage=3;handoffTick=tick;chestOffset=actors[2].position[0]-actors[0].position[0];}
        } else if(handoffStage>=3){
            actors[2].position[0]=actors[0].position[0]+chestOffset;
            if(handoffStage==3 && tick>=handoffTick+18){move(0,{-80,0,450},40);handoffStage=4;}
        }
    }
    if(opened){
        if(tick>=openTick+18 && tick<openTick+36 && std::strcmp(actors[0].pose,"P_B_3"))pose(0,"P_B_3");
        if(tick>=openTick+36 && std::strcmp(actors[0].pose,"P_B_4"))pose(0,"P_B_4");
        if(tick>=openTick+30)radiationRequested=true;
    }
}
void OpeningDemoScene::advance(uint64_t simulationTick) {if(!active || simulationTick<tick)return;while(tick<simulationTick){++tick;step();}step();}
void OpeningDemoScene::step() {
    if(!updated || updateTick!=tick){updated=true;updateTick=tick;update();}
    while(cursor<requests.size()){
        const auto& r=requests[cursor];
        if(!started){started=true;
            switch(r.kind){
            case Kind::Wait:deadline=tick+r.duration;break;
            case Kind::Move:move(r.actor,r.point,r.point[1]);break;
            case Kind::Camera:if(r.duration){cameraEye={eye,r.point,tick,tick+r.duration,true};cameraAt={at,r.aim,tick,tick+r.duration,true};}else{eye=r.point;at=r.aim;cameraEye.active=cameraAt.active=false;}break;
            case Kind::Tribe:tribe(r.actor);break;
            case Kind::Pose:pose(r.actor,r.pose,r.talk);break;
            case Kind::Reverse:actors[r.actor].yaw=r.actor==2?r.point[0]:std::fmod(actors[r.actor].yaw+180,360);break;
            case Kind::Message:waitingMessage=true;nextMessage=r.actor;break;
            case Kind::Lift:motion[2]={actors[2].position,{actors[2].position[0]-60*.099609375f,10,actors[2].position[2]},tick,tick+r.duration,true};break;
            case Kind::Handoff:handoff=true;handoffTick=tick;chestOffset=actors[2].position[0]-actors[1].position[0];break;
            case Kind::Walls:wallsVisible=true;break;
            case Kind::Open:opened=true;openTick=tick;crystalRequested=true;break;
            }
        }
        if((r.kind==Kind::Wait && tick<deadline) || (r.kind==Kind::Move && motion[r.actor].active) || (r.kind==Kind::Lift && motion[2].active) || (r.kind==Kind::Message && waitingMessage))return;
        ++cursor;started=false;
    }
    waitingForTitle=true;
}
bool OpeningDemoScene::messageReady(unsigned index) const {return waitingMessage && nextMessage==index;}
void OpeningDemoScene::messageComplete(unsigned index) {if(messageReady(index))waitingMessage=false;}
const char* OpeningDemoScene::actorPose(unsigned i,bool talking) const {
    const auto* requested=talking && actors[i].talkPose?actors[i].talkPose:actors[i].pose;
    // Original merchant tribe requests W_1, but c_nazo has only Z_1/S_1/T_1/
    // A_1. Name lookup falls back to original descriptor0; retain the request
    // in actors[].pose while exposing its actual renderable original pose.
    return i==1 && !std::strcmp(requested,"W_1")?"Z_1":requested;
}
}
