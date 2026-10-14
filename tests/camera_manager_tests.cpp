#include "camera_manager.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
using namespace ttyd;
struct MainHarness {
    std::vector<uint32_t> callbacks;
    unsigned looks=0,zooms=0;bool viewReady=false,callbackReady=true,zoomReady=false;
    static bool callback(void* p,uint32_t fn,uint32_t index,CameraState& c){
        auto& h=*static_cast<MainHarness*>(p);h.callbacks.push_back(index);
        assert(fn==0x8000ea68 || fn==0x8000e974 || fn==0x8000ea30);
        if(!h.callbackReady){c.callback=0;return false;}
        return true;
    }
    static std::optional<CameraViewMatrix> look(void* p,const CameraState& c){
        auto& h=*static_cast<MainHarness*>(p);++h.looks;
        if(!h.viewReady)return {};
        return cameraLookAtAdapter(c.eye,c.at,c.up);
    }
    static bool zoom(void* p,CameraState& c){
        auto& h=*static_cast<MainHarness*>(p);++h.zooms;if(!h.zoomReady)return false;
        c.matrices.view[0][3]=123;c.matrices.projection[0][0]=3;return true;
    }
    CameraMainServices services(){return {this,callback,look,zoom};}
};
struct DrawHarness {
    CameraManager& cameras;std::vector<std::string> calls;
    bool letterboxReady=false,nearReady=false,callbackReady=false,dummyReady=false;
    unsigned letters=0,nearAttempts=0;uint32_t borrowed=0;
    static bool letterbox(void* p,DisplayQueue& q){
        auto& h=*static_cast<DrawHarness*>(p);++h.letters;if(!h.letterboxReady)return false;
        h.calls.push_back("letterbox");return q.enter(8,3,0x8000e564,0,0)==DisplayAdmission::Registered;
    }
    static bool nearFar(void* p,CameraState& c){
        auto& h=*static_cast<DrawHarness*>(p);++h.nearAttempts;
        c.publication.flags&=~0x40u; // Changing flags does not uncapture the admitted source call.
        if(!h.nearReady)return false;c.nearPlane=2;c.farPlane=200;h.calls.push_back("near");return true;
    }
    static bool request(void* p,const CameraDrawRequest& r){
        auto& h=*static_cast<DrawHarness*>(p);assert(h.cameras.currentIndex()==r.index);
        assert(r.camera==h.cameras.current());
        const char* names[]{"viewport","scissor","projection","dummy"};
        if(r.kind==CameraDrawRequestKind::DummyDraw && !h.dummyReady)return false;
        h.calls.push_back(std::string(names[unsigned(r.kind)])+std::to_string(r.index));return true;
    }
    static bool state(void*,const DisplayStateRequest&){return true;}
    static bool callback(void* p,uint32_t camera,uint32_t fn,uint32_t borrowed){
        auto& h=*static_cast<DrawHarness*>(p);assert(h.cameras.currentIndex()==camera);
        h.borrowed=borrowed;if(!h.callbackReady)return false;
        h.calls.push_back("callback"+std::to_string(camera));assert(fn==123 || fn==0x8000e564);return true;
    }
    CameraDrawServices services(){return {this,letterbox,nearFar,request,{this,state,callback}};}
};
int main(){
    CameraManager bank;bank.initializeBoot(608,480);
    assert(!bank.camera(13) && !bank.current());
    for(unsigned i=0;i<13;++i){const auto& c=*bank.camera(i);assert(c.publication.flags==0 && c.mode==0 && c.drawCamera==8 && c.distance==1000);
        assert((c.scissor==std::array<uint16_t,4>({0,0,608,480})));
        assert(c.matrices.view==CameraViewMatrix{} && c.publication.extra==CameraProjectionMatrix{});
    }
    assert(std::string(bank.camera(4)->name)=="3d" && bank.camera(4)->nearPlane==1);
    assert(bank.camera(8)->nearPlane==100 && bank.camera(8)->viewport[4]==.1f);
    assert(bank.camera(6)->eye[2]!=1000 && bank.camera(6)->distance==1000);
    assert(bank.camera(0)->matrices.projection[1][1]>0 && bank.camera(2)->matrices.projection[1][1]<0);
    assert(bank.camera(2)->projectionKind==1 && bank.camera(8)->projectionKind==0);
    // Callback completes once before a missing LookAt, including a draw-disabled slot.
    bank.camera(0)->callback=0x8000ea68;bank.camera(0)->publication.flags=1;
    MainHarness main;CameraMainCursor mc;
    assert(!bank.main(false,main.services(),mc) && mc.index==0 && mc.stage==1);
    assert(!bank.main(false,main.services(),mc) && main.callbacks.size()==1);
    main.viewReady=true;assert(bank.main(false,main.services(),mc));
    assert(main.callbacks.size()==5 && !bank.current());
    // Capture the indirect callback identity even if the provider changes the live field.
    CameraManager captured;captured.initializeBoot(608,480);MainHarness indirect;indirect.viewReady=true;indirect.callbackReady=false;
    assert(!captured.main(false,indirect.services(),mc) && mc.index==4);
    indirect.callbackReady=true;assert(captured.main(false,indirect.services(),mc));assert(indirect.callbacks.size()==5);
    // Field zoom comes after view transforms, before one-use extra. No repeat on hold.
    auto& c4=*bank.camera(4);c4.publication.flags|=8;c4.publication.extra={};
    for(unsigned i=0;i<4;++i)c4.publication.extra[i][i]=1;c4.publication.extra[0][0]=2;
    const auto callbacks=main.callbacks.size();assert(!bank.main(true,main.services(),mc) && mc.index==4 && mc.stage==2);
    assert(c4.publication.flags&8);const auto beforeCallbacks=main.callbacks.size();const auto beforeLooks=main.looks;
    assert(!bank.main(true,main.services(),mc));assert(main.callbacks.size()==beforeCallbacks && main.looks==beforeLooks);
    main.zoomReady=true;assert(bank.main(true,main.services(),mc));
    assert(c4.matrices.view[0][3]==123 && c4.matrices.projection[0][0]==6 && !(c4.publication.flags&8));
    assert(main.callbacks.size()==callbacks+5);
    // Main never published current index. Draw registers letterbox before sorting.
    CameraManager draw;draw.initializeBoot(608,480);draw.camera(0)->publication.flags=1;
    draw.camera(1)->projectionKind=0;draw.camera(1)->publication.flags=0x40;
    DisplayQueue q;assert(q.enter(1,1,123,0xabcdef,0)==DisplayAdmission::Registered);
    DrawHarness h{draw};CameraDrawCursor dc;
    assert(!draw.draw(q,h.services(),dc) && q.count()==1 && !draw.current());
    h.letterboxReady=true;assert(!draw.draw(q,h.services(),dc));
    assert(q.count()==2 && dc.index==1 && dc.stage==6 && draw.currentIndex()==1);
    assert(h.calls==std::vector<std::string>({"letterbox","viewport1","scissor1"}));
    h.nearReady=true;assert(!draw.draw(q,h.services(),dc) && h.nearAttempts==2 && dc.stage==8);
    assert(h.borrowed==0xabcdef && q.current()->userdata==h.borrowed);
    h.callbackReady=true;assert(!draw.draw(q,h.services(),dc) && dc.stage==9 && q.count()==2);
    const auto prefix=h.calls;assert(!draw.draw(q,h.services(),dc) && h.calls==prefix); // Dummy hold does not replay callbacks.
    h.dummyReady=true;assert(draw.draw(q,h.services(),dc));assert(q.count()==0 && draw.currentIndex()==12);
    assert(h.letters==2); // Initial unavailable letterbox plus one completed call, never repeated later.
    for(unsigned i=0;i<13;++i)draw.camera(i)->publication.flags|=1;
    assert(draw.draw(q,h.services(),dc) && q.count()==0 && draw.currentIndex()==12);
    assert(h.letters==3); // All disabled still register, sort and reset; keep last current index.
    // All13 published matrices remain distinct: overlays are never coerced to camera4.
    assert(bank.camera(2)->matrices.projection!=bank.camera(4)->matrices.projection);
}
