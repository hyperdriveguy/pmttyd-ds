#include "opening_house_scene.hpp"
#include "field_camera_events.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
void OpeningHouseScene::appendProgram() {
    auto wait=[&](uint64_t frames){requests.push_back({Kind::Wait,0,{},{},frames});};
    auto ms=[&](unsigned value){wait((uint64_t(value)*60+999)/1000);};
    auto set=[&](unsigned actor,std::array<float,3> p){requests.push_back({Kind::Set,actor,p});};
    auto move=[&](unsigned actor,float x,float z,bool parallel=false){requests.push_back({parallel?Kind::ParallelMove:Kind::Move,actor,{x,0,z}});};
    auto glide=[&](std::array<float,3> p){requests.push_back({Kind::Move,2,p});};
    auto pose=[&](unsigned actor,const char* p,const char* talk=nullptr){Request r{Kind::Pose,actor};r.pose=p;r.talk=talk;requests.push_back(r);};
    auto yaw=[&](unsigned actor,float value){requests.push_back({Kind::Yaw,actor,{value,0,0}});};
    auto door=[&](float value){requests.push_back({Kind::Door,0,{value,0,0}});};
    auto shake=[&](){for(unsigned i=0;i<3;++i){requests.push_back({Kind::Postbox,0,{2,0,0}});wait(2);requests.push_back({Kind::Postbox});wait(2);}};
    auto message=[&](unsigned index){requests.push_back({Kind::Message,index});};
    // Original root: camera request followed by WAIT_MSEC4000. The initial
    // BGM WAIT1500 belongs to an independent inline, not this root's deadline.
    Request camera{Kind::Camera};camera.point={145,122,717};camera.aim={145,108,-180};camera.duration=300;requests.push_back(camera);
    ms(4000);set(2,{500,500,0});glide({210,25,-30});yaw(2,180);ms(500);shake();yaw(2,0);ms(500);message(0);
    glide({0,300,-100});requests.push_back({Kind::Visible,2,{0,0,0}});
    set(1,{20,30,-150});yaw(1,90);door(180);ms(500);move(1,63,-110);door(0);
    move(1,195,0);move(1,225,-75);ms(500);shake();ms(340);pose(1,"L_5");ms(500);
    pose(1,"L_11");move(1,195,0);move(1,63,-110);pose(1,"L_5");door(180);ms(500);
    pose(1,"L_6");move(1,25,-155,true);ms(500);
    camera.point={-345,154,96};camera.aim={-32,75,-217};camera.duration=0;requests.push_back(camera);wait(1);
    requests.push_back({Kind::Interior});wait(1);door(0);ms(500);pose(1,"L_5","L_7");message(1);
    camera.point={-376,154,65};camera.aim={-63,75,-247};camera.duration=120;requests.push_back(camera);
    pose(1,"L_6");move(1,23,-156);pose(1,"L_5");
    set(0,{-155,30,-320});move(0,-125,-290,true);
    // Mario's second waypoint is scheduled when his first move completes.
    // Luigi's blocking return proceeds independently, as in the source inline.
    pose(1,"L_6");move(1,-52,-275);pose(1,"L_5","L_7");ms(500);message(2);
    ms(300);pose(1,"L_8","L_9");message(3);ms(300);
}
void OpeningHouseScene::begin(uint64_t simulationTick) {
    *this=OpeningHouseScene{};tick=beginTick=simulationTick;
    actors[0].position={-150,30,-330};actors[0].pose="M_S_1";actors[0].yaw=180;actors[0].visible=true;
    actors[1].pose="L_5";actors[1].position={20,30,-150};
    actors[2].pose="S_1";actors[2].position={500,500,0};
    eye={145,122,884};at={145,108,-180};appendProgram();step();
}
void OpeningHouseScene::startMove(unsigned actor,const std::array<float,3>& target) {
    auto& a=actors[actor];auto& m=motion[actor];m.start=a.position;m.target=target;
    if(actor!=2)m.target[1]=a.position[1];
    const float dx=m.target[0]-a.position[0],dy=m.target[1]-a.position[1],dz=m.target[2]-a.position[2];
    // Source operand is80. Native bridge uses constant80 engine units/second;
    // original glide/heading equations are deliberately not claimed here.
    const float distance=std::sqrt(dx*dx+dy*dy+dz*dz);
    m.begin=tick;m.end=tick+std::max<uint64_t>(1,uint64_t(std::ceil(distance*60/80)));m.active=true;a.moving=true;
    if(actor<2 && (dx || dz))a.yaw=std::atan2(dx,dz)*180/3.14159265358979323846f;
    if(actor==0){a.pose="M_W_1";a.poseTick=tick;}
}
void OpeningHouseScene::updateMotion() {
    auto advance=[&](Motion& m,std::array<float,3>& p){if(!m.active)return;const float t=std::min(1.f,float(tick-m.begin)/float(m.end-m.begin));for(unsigned i=0;i<3;++i)p[i]=m.start[i]+(m.target[i]-m.start[i])*t;if(t==1)m.active=false;};
    for(unsigned i=0;i<3;++i){const bool moving=motion[i].active;advance(motion[i],actors[i].position);actors[i].moving=motion[i].active;
        if(i==0 && moving && !motion[i].active){
            if(motion[i].target[0]==-125 && motion[i].target[2]==-290)startMove(0,{-85,30,-310});
            else {actors[0].pose="M_S_1";actors[0].poseTick=tick;actors[0].yaw=90;}
        }
    }
    auto curve=[&](Motion& m,std::array<float,3>& p) {
        if(!m.active)return;
        const auto elapsed=std::min(tick-m.begin,m.end-m.begin);
        for(unsigned k=0;k<3;++k)p[k]=*cameraInterpolation(11,int32_t(elapsed),int32_t(m.end-m.begin),m.start[k],m.target[k]);
        if(tick>=m.end){p=m.target;m.active=false;}
    };
    curve(cameraEye,eye);curve(cameraAt,at);
    std::array<float,3> door{doorAngle,0,0};curve(doorMotion,door);
    // evt_sub_intpl_msec_get_value publishes through source evtSetFloat LW0.
    doorAngle=std::trunc(door[0]*1024.f)/1024.f;
}
void OpeningHouseScene::advance(uint64_t simulationTick) {
    if(simulationTick<tick || complete)return;
    // Advance missed simulation ticks too. Neither movement nor explicit
    // WAIT_FRAME counts may accelerate with render calls or stall on a gap.
    while(tick<simulationTick && !complete){++tick;step();}
    step();
}
void OpeningHouseScene::step() {
    musicStarted=tick-beginTick>=90;updateMotion();
    const auto elapsed=std::min<uint64_t>(tick-beginTick,240);
    const float treeValue=*cameraInterpolation(11,int32_t(elapsed),240,0,1500);
    // evt_sub publishes through evtSetFloat; DIV float publishes again.
    const float published=std::trunc(treeValue*1024.f)/1024.f;
    treeX=std::trunc((published/10.f)*1024.f)/1024.f;
    while(cursor<requests.size()) {
        const auto& r=requests[cursor];
        if(!started){started=true;
            switch(r.kind){
            case Kind::Wait:deadline=tick+r.duration;break;
            case Kind::Set:actors[r.actor].position=r.point;actors[r.actor].visible=true;break;
            case Kind::Move:case Kind::ParallelMove:startMove(r.actor,r.point);break;
            case Kind::Pose:actors[r.actor].pose=r.pose;actors[r.actor].talkPose=r.talk;actors[r.actor].poseTick=tick;break;
            case Kind::Yaw:actors[r.actor].yaw=r.point[0];break;
            case Kind::Visible:actors[r.actor].visible=r.point[0]!=0;break;
            case Kind::Camera:if(r.duration){cameraEye={eye,r.point,tick,tick+r.duration,true};cameraAt={at,r.aim,tick,tick+r.duration,true};}else{eye=r.point;at=r.aim;cameraEye.active=cameraAt.active=false;}break;
            case Kind::Interior:interior=true;break;
            case Kind::Door:doorMotion={{doorAngle,0,0},r.point,tick,tick+30,true};break;
            case Kind::Postbox:postboxX=r.point[0];break;
            case Kind::Message:waitingMessage=true;nextMessage=r.actor;break;
            }
        }
        if((r.kind==Kind::Wait && tick<deadline) || (r.kind==Kind::Move && motion[r.actor].active) || (r.kind==Kind::Message && waitingMessage))return;
        ++cursor;started=false;
    }
    complete=true;
}
bool OpeningHouseScene::messageReady(unsigned index) const {return waitingMessage && nextMessage==index;}
void OpeningHouseScene::messageComplete(unsigned index) {if(messageReady(index))waitingMessage=false;}
const char* OpeningHouseScene::actorPose(unsigned actor,bool talking) const {const auto& a=actors[actor];return talking && a.talkPose?a.talkPose:a.pose;}
}
