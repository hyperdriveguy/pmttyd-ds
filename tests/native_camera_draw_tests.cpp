#include "native_camera_draw.hpp"
#include <cassert>
#include <limits>
#include <vector>
using namespace ttyd;
struct Sink {std::vector<NativeCameraPacket> packets;bool ready=true;
    NativeCameraSubmit services(){return {this,[](void* p,const NativeCameraPacket& packet){auto& s=*static_cast<Sink*>(p);if(!s.ready)return false;s.packets.push_back(packet);return true;}};}
};
int main(){
    auto full=nativeCameraRect(0,0,608,480);assert(full && full->x==0 && full->y==0 && full->width==240 && full->height==400);
    auto center=nativeCameraRect(152,120,304,240);assert(center && center->x==60 && center->y==100 && center->width==120 && center->height==200);
    auto corner=nativeCameraRect(0,0,152,120);assert(corner && corner->x==180 && corner->y==300);
    assert(!nativeCameraRect(1,0,607,480) && !nativeCameraRect(0,0,0,480));
    assert(!nativeCameraRect(-1,0,608,480) && !nativeCameraRect(0,0,609,480));
    assert(!nativeCameraRect(0,std::numeric_limits<float>::quiet_NaN(),608,480));
    CameraProjectionMatrix p{{{2,0,3,4},{0,5,0,6},{0,0,8,9},{1,2,3,4}}};
    CameraViewMatrix v{{{1,0,0,10},{0,1,0,20},{0,0,1,30}}};
    auto tilted=nativeCameraTransform(p,v);
    assert((tilted[0]==std::array<float,4>({0,5,0,106})));
    assert((tilted[1]==std::array<float,4>({-2,0,-3,-114})));
    assert(tilted[2][3]==249 && tilted[3][3]==144);
    CameraState c;c.viewport={0,0,608,480,.1f,1};c.scissor={0,0,608,480};c.matrices={v,p};
    NativeCameraDraw backend;Sink sink;sink.ready=false;
    assert(!backend.request({CameraDrawRequestKind::Viewport,0,&c},sink.services()));
    sink.ready=true;assert(backend.restore(sink.services()) && sink.packets.empty());
    assert(!backend.request({CameraDrawRequestKind::DummyDraw,0,&c},sink.services()));
    for(auto kind:{CameraDrawRequestKind::Viewport,CameraDrawRequestKind::Scissor,CameraDrawRequestKind::Projection,CameraDrawRequestKind::DummyDraw})assert(backend.request({kind,0,&c},sink.services()));
    assert(sink.packets.size()==4 && sink.packets[0].depthScale==.1f-1 && sink.packets[0].depthOffset==0);
    assert(sink.packets.back().transform==tilted);
    c.matrices={}; // Acknowledged backend snapshots do not reread the live bank.
    assert(backend.restore(sink.services()) && sink.packets.size()==8);
    assert(sink.packets[6].kind==NativeCameraCommand::Projection && sink.packets[6].transform==tilted);
    assert(sink.packets[7].kind==NativeCameraCommand::DummyStateRestore && sink.packets[7].transform==tilted);
    unsigned draws=0;for(const auto& packet:sink.packets)draws+=packet.kind==NativeCameraCommand::DummyDraw;
    assert(draws==1);
}
