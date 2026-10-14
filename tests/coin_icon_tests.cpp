#include "field_item_registry.hpp"
#include "message_icons.hpp"
#include "game.hpp"
#include "camera_road_view.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
 FieldItemRegistry registry;auto* icon=registry.enterCoin403("iraiCoin",{3,52,-7});
 assert(icon && icon->id==403 && icon->flags==0x1001 && icon->frameCount==4 && icon->frameIndex==0 && icon->texture==414 && icon->delay==6);
 registry.iconFlags("iraiCoin",0x102,0);assert(icon->flags==0x1103 && icon->camera()==5 && !icon->visible(true));
 registry.advanceIcons(false);assert(icon->delay==6);
 for(unsigned visit=1;visit<=24;++visit) {
  registry.advanceIcons(true);
  assert(icon->frameIndex==(visit/6)%4 && icon->texture==414+(visit/6)%4 && icon->delay==6-visit%6);
  assert(!icon->visible(true)); // Hidden icon cycles before display admission.
 }
 for(unsigned i=0;i<8;++i)registry.advanceIcons(true);
 const auto frame=icon->frameIndex,delay=icon->delay;
 registry.iconFlags("iraiCoin",0,2);assert(icon->visible(true) && icon->frameIndex==frame && icon->delay==delay);
 assert(loadMessageIcons("romfs/message-icons.bin"));const auto* original=messageIcon(403);assert(original && original->frames.size()==4 && original->loop==1);
 for(const auto& f:original->frames)assert(f.width==40 && f.height==40 && f.delay==6);
 FieldItemIconDraw draw;assert(fieldItemIconDraw(*icon,0,40,40,draw));
 assert(draw.vertices[0][0]==-12 && draw.vertices[0][1]==24 && draw.vertices[2][0]==12 && draw.vertices[2][1]==0);
 assert(draw.color==0xffffffff && draw.depthWrite && !draw.blend && !item95IconDraw(*icon,0,40,40,draw));
 const auto corner=draw.model.point(draw.vertices[0]);assert(corner[0]==-9 && corner[1]==76 && corner[2]==-7);
 icon->flags|=0x40;assert(!fieldItemIconDraw(*icon,0,40,40,draw));icon->flags&=~uint16_t(0x40);
 registry.deleteIcon("iraiCoin");assert(!registry.findIcon("iraiCoin"));registry.advanceIcons(true);
 assert(icon->frameIndex==frame && icon->delay==delay);
 // Both constructor and named setters preserve source duplicate semantics.
 FieldItemRegistry duplicate;auto* first=duplicate.enterCoin403("same",{1,2,3});auto* second=duplicate.enterCoin403("same",{4,5,6});
 assert(first!=second && first->position.y==5 && second->position.y==0);
 duplicate.iconFlags("same",2,0);assert((first->flags&2) && !(second->flags&2));
 duplicate.deleteIcon("same");assert(duplicate.findIcon("same")==second);
 FieldItemRegistry queued;auto* waiting=queued.enterCoin403("pending",{},false);assert(waiting->flags==0x2001 && !waiting->frameCount);
 queued.advanceIcons(false);assert(waiting->flags==0x2001);
 queued.advanceIcons(true);assert(waiting->flags==0x1001 && waiting->texture==414 && waiting->delay==5);
 FieldItemRegistry full;for(unsigned i=0;i<32;++i)assert(full.enterCoin403(("c"+std::to_string(i)).c_str(),{}));
 assert(!full.enterCoin403("overflow",{}) && !full.enter95("no-icon",{}));
 // Camera5 is published by simulation before later camera/Mario writes. Draw
 // consumes this snapshot instead of rebuilding from a changed live camera4.
 Game game;game.fieldCameraFlags=2;game.fieldCameraFrame=*fieldCameraFrameFromView({0,5,10},{0,1,0},40,1,100);
 game.tick({});assert(game.fieldItemCamera5Frame && game.fieldItemCamera5Frame->fov==25);
 const auto copied=game.fieldItemCamera5Frame->eye;game.fieldCameraFrame.eye.x=99;
 assert(game.fieldItemCamera5Frame->eye.x==copied.x && game.fieldItemCamera5Frame->eye.z==copied.z);
}
