#include "native_harbor_star_point_presentation.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
namespace {
bool near(float a,float b){return std::fabs(a-b)<.00001f;}
BattleSceneMatrix identity{{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}},{{0,0,0,1}}}};
}
int main(){
 assert(loadMessageIcons("romfs/message-icons.bin"));const auto*original=messageIcon(404);assert(original);
 const auto icon=*original;assert(validNativeHarborStarPointIcon(icon,512,1024));
 assert(!validNativeHarborStarPointIcon(icon,512,512));
 auto wrong=icon;wrong.frames[0].delay=0;assert(!validNativeHarborStarPointIcon(wrong,512,1024));
 wrong=icon;wrong.id=403;assert(!validNativeHarborStarPointIcon(wrong,512,1024));
 std::array<NativeHarborStarPointVisual::Instance,NativeHarborStarPointVisual::Capacity>instances{};
 auto&i=instances[0];i.id=1;i.request={0,3,{100,50,-20}};i.particles.resize(3);
 i.particles[0].position={10,20,30};i.particles[0].alpha=255;
 i.particles[1]=i.particles[0];i.particles[1].alpha=1;i.particles[1].delay=900;
 // Source Disp skips zero alpha before reading relative XYZ.
 i.particles[2].position[0]=std::numeric_limits<float>::quiet_NaN();
 const auto q=nativeHarborStarPointGeometry(instances,icon,0,0x102030ff,identity);assert(q&&q->vertices.size()==12&&q->ranges[0][1]==12);
 const auto&v=q->vertices[0];assert(near(v.x,.98f)&&near(v.y,.94f)&&near(v.z,.1f));
 assert(near(q->vertices[1].x-v.x,.24f)&&near(v.y-q->vertices[2].y,.24f));
 assert(v.color==q->vertices[6].color&&near(v.color[3],1)); // alpha1 is visible at full icon opacity.
 assert(v.u==icon.frame(0).u0&&v.v==icon.frame(0).v0);
 const auto next=nativeHarborStarPointGeometry(instances,icon,6,0xffffffff,identity);assert(next&&next->vertices[0].u==icon.frame(6).u0);
 const auto loop=nativeHarborStarPointGeometry(instances,icon,24,0xffffffff,identity);assert(loop&&loop->vertices[0].u==icon.frame(0).u0);
 i.particles[1].alpha=-1;assert(nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,identity)->vertices.size()==12);
 i.particles[0].alpha=0;i.particles[1].alpha=0;assert(nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,identity)->vertices.empty());
 i.particles[0].alpha=1;i.request.amount=2;assert(!nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,identity));i.request.amount=3;
 i.request.position[0]=std::numeric_limits<float>::max();i.particles[0].position[0]=std::numeric_limits<float>::max();assert(!nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,identity));
 i.request.position[0]=200;i.particles[0].position[0]=10;auto overflow=identity;overflow[0][0]=std::numeric_limits<float>::max();assert(!nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,overflow));
 // Finite offscreen/singular transforms remain admitted; no invented culling.
 BattleSceneMatrix singular{};assert(nativeHarborStarPointGeometry(instances,icon,0,0xffffffff,singular));
 wrong=icon;wrong.frames[0].u0=std::numeric_limits<float>::infinity();assert(!nativeHarborStarPointGeometry(instances,wrong,0,0xffffffff,identity));
}
