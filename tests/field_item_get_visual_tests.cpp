#include "field_item_get_effect.hpp"
#include "item_get_text.hpp"
#include "message_assets.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
bool close(float a,float b){return std::fabs(a-b)<.00002f;}
int main() {
 FieldItemGetEffect6 effect;assert(!effect.active() && !effect.tick());effect.begin({100,82,-7});effect.layer(5);
 const float curve[]={.18f,.38f,.59f,.78f,.96f,1.10f,1.20f,1.21f,1.14f,1.05f,1.f};
 for(float expected:curve){assert(effect.tick() && close(effect.scale(),expected) && effect.alpha()==255 && effect.lifetime()==1000);}
 for(int i=0;i<1200;++i)effect.tick();assert(effect.age()==1000 && effect.scale()==1 && effect.layer()==5);
 auto draw=effect.draw(0,0);assert(draw.textures[0]==44 && draw.textures[1]==42 && draw.colors[1]==0xeb7d06ff);
 assert(draw.vertices[0][0]==-32 && draw.vertices[1][0]==32 && draw.vertices[0][1]==32);
 const auto base=draw.passes[1].model.point({0,0,0}),shadow=draw.passes[0].model.point({0,0,0});
 assert(close(base[0],100) && close(base[1],82) && close(base[2],-9));
 assert(close(shadow[0]-base[0],1.8f) && close(shadow[1]-base[1],-1.8f));
 assert(draw.passes[0].color==0x00000080 && draw.passes[1].color==0xffffffff);
 const auto rotated=effect.draw(90,0).passes[1].model.point({0,0,0});assert(close(rotated[0],102) && close(rotated[2],-7));
 const auto unit=effect.draw(90,0).passes[1].model.point({1,0,0});
 assert(close(unit[0]-rotated[0],0) && close(unit[2]-rotated[2],.9f));
 effect.softDelete();const int before=effect.age();
 for(int life=15;life>=0;--life){assert(effect.tick() && effect.lifetime()==life && effect.alpha()==life*16);}
 assert(!effect.tick() && !effect.active() && effect.age()==before); // Age was already capped.
 FieldItemGetEffect6 fresh;fresh.begin({0,0,0});fresh.softDelete();
 for(int i=0;i<16;++i)assert(fresh.tick());
 assert(fresh.age()==16 && fresh.alpha()==0 && !fresh.tick() && fresh.age()==16);
 const auto idleAge=effect.age();for(int i=0;i<10;++i)effect.draw(90,0);assert(effect.age()==idleAge);
 assert(expandUsItemGetText("<AN><ITEM> <AN><ITEM>","Apple")=="an Apple <AN><ITEM>");
 assert(expandUsItemGetText("<AN><ITEM>","Box")=="a Box");
 assert(expandUsItemGetText("<AN><ITEM>","<col>Apple")=="a <col>Apple"); // Raw first byte, no token stripping.
 MessageAssets messages;assert(messages.loadGlobal("romfs/messages/global.txt"));
 const auto* templ=messages.find("msg_window_item_get");const auto* name=messages.find("in_irai_butsu");
 assert(templ && name && expandUsItemGetText(templ,name)=="You got a <col c00000ff>Box</col>!");
 ActorAnimationPack pose;assert(pose.load("romfs/field-item-get-mario.pmanim"));
 const auto* original=pose.pose("M_I_U");assert(original && !original->loop && original->start==0 && original->end==0);
 ActorAnimationSample sample;assert(original->evaluate(0,sample) && !sample.vertices.empty());
}
