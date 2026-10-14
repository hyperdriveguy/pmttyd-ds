#include "opening_harbor_scene.hpp"
#include "opening_ship_objects.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
int main(int argc,char** argv){
 OpeningHarborScene s;s.begin(100);s.advance(280);assert(s.x==-600 && !s.marioVisible);s.advance(400);assert(s.x==-270);s.advance(460);assert(std::fabs(s.x+123.75f)<.001f);s.advance(520);assert(s.x==-75 && s.marioVisible && s.eye.x==2 && s.at.z==243);
 s.advance(579);assert(!s.messageReady(0));s.advance(580);assert(s.messageReady(0));s.messageComplete(0);s.advance(699);assert(!s.messageReady(1));s.advance(700);assert(s.messageReady(1));s.messageComplete(1);
 s.advance(783);assert(s.jumpStarted && s.jump.active && s.y>0 && !s.paperCapturePending);s.advance(784);assert(s.paperCapturePending && !s.paperLive);s.captureComplete();assert(s.paperLive && s.paperFrame()==0);s.advance(844);assert(s.paperFrame()==60 && s.paperLive && !s.paperDone);s.presented(59);s.advance(845);assert(!s.paperDone);s.presented(60);s.advance(846);assert(s.paperDone && s.yaw==0);
 s.advance(868);assert(!s.jump.active && s.jump.result==2 && s.y==0);s.advance(988);assert(s.departureAt==989);s.advance(1049);assert(s.x==-123.75f && s.wakeX==-475);s.advance(1109);assert(s.x==-270 && !s.storyPublished);s.advance(1169);assert(s.storyPublished);s.advance(1468);assert(s.visible && !s.complete);s.advance(1469);assert(!s.visible && s.complete && s.x==-1260);
 OpeningHarborScene resolved;resolved.begin(0,1.75f);resolved.advance(480);resolved.messageComplete(0);resolved.advance(600);resolved.messageComplete(1);resolved.advance(660);assert(resolved.jump.start[1]==1.75f && resolved.jump.target[1]==1);
 resolved.advance(714);assert(resolved.work8>100);resolved.advance(768);assert(resolved.work8==1 && resolved.jump.result==2);
 OpeningHarborScene dense,sparse;dense.begin(0);sparse.begin(0);for(unsigned tick=0;tick<=600;++tick)dense.advance(tick);sparse.advance(600);assert(dense.x==sparse.x && dense.parts==sparse.parts && dense.rock==sparse.rock);
 if(argc==3){FILE* f=std::fopen(argv[2],"rb");assert(f);assert(!std::fseek(f,0,SEEK_END));const long size=std::ftell(f);std::fclose(f);OpeningShipObjects objects;assert(objects.load(argv[1],size/36));unsigned writes=0;objects.apply(-75,0,-75,180,0,{},[&](uint32_t i,const OpeningShipObjects::Point& p){assert(i<unsigned(size/36));for(float v:p)assert(std::isfinite(v));++writes;});assert(writes>1000);}
 std::puts("Original harbor approach, message gates, actual paper presentation, departure and dense/sparse clock passed");
}
