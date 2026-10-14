#include "field_item_registry.hpp"
#include "message_icons.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
bool near(float a,float b){return std::fabs(a-b)<.00002f;}
int main() {
 FieldItemRegistry r;auto* item=r.enter95("irai_item",{0,-1000,0});assert(item==&r.items()[0]);
 assert(item->flags==1 && item->mode==16 && item->item==95 && item->secondary==-1 && item->callback==0 && item->sequence==7 && item->substate==0);
 assert(item->scale==1 && item->alpha==255 && item->position.y==-1000);
 auto* icon=r.findIcon("irai_item");assert(icon && icon->id==307 && icon->flags==1 && icon->position.y==-1000 && icon->color==0xffffffff);
 r.flagOn("irai_item",2);assert(item->flags==3 && icon->flags==1); // Different flag owners.
 item->status=0x10001;item->counter78=12;item->counter7c=7;
 assert(r.forceGet("irai_item") && item->sequence==2 && item->substate==0 && item->flags==0x1003 && item->status==1 && !item->counter78 && !item->counter7c);
 assert(r.pollGet("irai_item")==0 && !r.forceGet("missing") && r.pollGet("missing")==2);
 item->status|=2;assert(r.pollGet("irai_item")==2);item->status&=~2u;
 item->position={100,50,-7};icon->flags|=0x180;r.synchronizeIcon(*item);
 assert(icon->camera()==5 && icon->visible(true) && !icon->visible(false) && icon->position.y==50);
 assert(loadMessageIcons("romfs/message-icons.bin"));const auto* asset=messageIcon(307);assert(asset && asset->frames.size()==1);
 const auto& frame=asset->frame(0);assert(frame.width==40 && frame.height==40);
 FieldItemIconDraw draw;assert(item95IconDraw(*icon,0,frame.width,frame.height,draw));
 assert(draw.vertices[0][0]==-12 && draw.vertices[0][1]==24 && draw.vertices[2][0]==12 && draw.vertices[2][1]==0 && draw.depthWrite && !draw.blend);
 const auto corner=draw.model.point(draw.vertices[0]);assert(near(corner[0],88) && near(corner[1],74) && near(corner[2],-7));
 assert(item95IconDraw(*icon,90,40,40,draw));const auto turned=draw.model.point(draw.vertices[0]);assert(near(turned[0],100) && near(turned[2],-19));
 icon->flags|=2;assert(!icon->visible(true) && !item95IconDraw(*icon,0,40,40,draw));icon->flags&=~2u;
 icon->color=0xffffff80;assert(item95IconDraw(*icon,0,40,40,draw) && draw.blend && !draw.depthWrite);
 icon->flags|=0x40;assert(!item95IconDraw(*icon,0,40,40,draw));icon->flags&=~0x40u;
 assert(!item95IconDraw(*icon,0,32,40,draw));
 r.deleteNamed("irai_item");assert(!(item->flags&1) && r.find("irai_item")==item && !r.findIcon("irai_item") && r.pollGet("irai_item")==2);
 const auto retained=icon->position;item->position={99,99,99};r.synchronizeIcon(*item);assert(icon->position.y==retained.y); // Second active gate.
 FieldItemRegistry duplicates;auto* first=duplicates.enter95("same",{1,2,3});auto* second=duplicates.enter95("same",{4,5,6});
 assert(first!=second && duplicates.find("same")==first && (first->flags&1) && (second->flags&1));
 assert(duplicates.icons()[0].position.y==5 && duplicates.icons()[1].position.y==0); // First-active icon setters, new icon remains cleared XYZ.
 first->position={1,11,1};second->position={2,22,2};second->alpha=119;duplicates.synchronizeIcons();
 assert(duplicates.icons()[0].position.y==22 && (duplicates.icons()[0].color&255)==119 && duplicates.icons()[1].position.y==0);
 first->flags&=~1u;assert(duplicates.find("same")==first && duplicates.pollGet("same")==2); // Inactive name shadows later active duplicate.
 duplicates.deleteNamed("same");assert(!duplicates.findIcon("same") && !(second->flags&1));
 FieldItemRegistry full;
 for(unsigned i=0;i<FieldItemRegistry::Capacity;++i){const auto name="i"+std::to_string(i);assert(full.enter95(name.c_str(),{0,-1000,0}));}
 assert(!full.enter95("overflow",{0,0,0}));assert(full.items()[0].name=="i0" && full.items()[31].name=="i31");
 // Independent source pools: inactive item records do not free active icons.
 for(auto& e:full.items())e.flags&=~1u;
 assert(!full.enter95("no-icon-slot",{}));assert(full.items()[0].name=="i0" && !(full.items()[0].flags&1));
 FieldItemRegistry blended;assert(blended.enter95("blend",{},true,true));assert(blended.findIcon("blend")->flags==0x41);assert(!item95IconDraw(*blended.findIcon("blend"),0,40,40,draw));
 FieldItemRegistry deferred;assert(deferred.enter95("pending",{},false));assert(deferred.findIcon("pending")->flags==0x2001);
 assert(!item95IconDraw(*deferred.findIcon("pending"),0,40,40,draw));
 std::cout<<"Original field item/icon pools, stale-name/duplicate semantics, forced wait and icon geometry passed\n";
}
