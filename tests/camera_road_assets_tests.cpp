#include "camera_road_assets.hpp"
#include "camera_road_curve.hpp"
#include "camera_road_zone.hpp"
#include "camera_road_state.hpp"
#include "camera_road_view.hpp"
#include "game.hpp"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace ttyd;
#define CHECK(x) do {if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
void put(std::vector<uint8_t>& b,size_t offset,uint32_t value){for(int n=0;n<4;++n)b[offset+n]=value>>(24-n*8);}
void number(std::vector<uint8_t>& b,size_t offset,float value){uint32_t bits;std::memcpy(&bits,&value,4);put(b,offset,bits);}
std::vector<uint8_t> fixture() {
    std::vector<uint8_t> raw(0x1d0);put(raw,0,raw.size());
    std::memcpy(raw.data()+4,"MarioSt_CameraRoadExport",24);std::memcpy(raw.data()+0x44,"1.01",5);
    std::memcpy(raw.data()+0x84,"parser fixture",15);
    for(int n=0;n<9;++n)put(raw,0xe8+n*4,0xffbf3a10);
    put(raw,0xc4,1);put(raw,0xe8,0x10c);put(raw,0xc8,1);put(raw,0xec,0x118);
    number(raw,0x10c,25);number(raw,0x110,.01f);number(raw,0x114,1000);
    std::memcpy(raw.data()+0x118,"C_fixture",10);number(raw,0x118+0x58,42.5f);
    // Uninterpreted scalar words are retained, not synthesized/converted.
    put(raw,0x118+0x80,0x89abcdef);return raw;
}
std::vector<uint8_t> straightFixture() {
    auto raw=fixture();raw.resize(0x1d0+144);put(raw,0,raw.size());
    put(raw,0xd4,12);put(raw,0xf8,0x1d0);put(raw,0x118+0xa4,12);
    put(raw,0x118+0x40,2);put(raw,0x118+0x44,3);put(raw,0x118+0x84,1);
    number(raw,0x118+0x48,.5f);number(raw,0x118+0x4c,1);
    for(int n=0;n<6;++n) {
        number(raw,0x1d0+n*12,n);number(raw,0x1d0+n*12+4,10+n);
        number(raw,0x1d0+(6+n)*12,1);number(raw,0x1d0+(6+n)*12+4,n);
    }
    return raw;
}
void curveCases() {
    CameraRoadAsset asset;CHECK(asset.decode(straightFixture()));CHECK(supportsStraightCameraPath(asset,0));
    CameraCurveOutput out;out.untouchedWord=0x12345678;
    CHECK(solveStraightCameraPath(asset,0,{2.5f,999,2},out)==CameraPathStatus::Found);
    CHECK(out.parameter==2.5f && out.anchor.x==2.5f && out.anchor.z==1 && out.segmentStartY==12);
    CHECK(out.segment==2 && out.flags==2 && out.untouchedWord==0x12345678 && out.tangent.y==0);
    CHECK(solveStraightCameraPath(asset,0,{2.5f,0,-2},out)==CameraPathStatus::Found);
    CHECK(out.anchor.z==-.5f && out.flags==2);
    CHECK(solveStraightCameraPath(asset,0,{2.5f,0,-.5f},out)==CameraPathStatus::Found);
    CHECK(out.anchor.z==-.5f && out.flags==0); // Equality is not side-capped.
    CHECK(solveStraightCameraPath(asset,0,{1,0,2},out)==CameraPathStatus::Found);
    CHECK(out.parameter==2 && out.anchor.x==2 && out.anchor.z==1 && out.segment==2 && out.flags==3);
    CHECK(out.segmentStartY==10); // Shared endpoint chooses the earlier uncapped segment.
    CHECK(solveStraightCameraPath(asset,0,{4,0,0},out)==CameraPathStatus::Found);
    CHECK(out.parameter==3 && out.anchor.x==3 && out.segment==3 && out.flags==1);
    CHECK(solveStraightCameraPath(asset,0,{2,0,0},out)==CameraPathStatus::Found);
    CHECK(out.parameter==2 && out.anchor.x==2 && out.segment==1 && out.flags==0);
    const auto previous=out;
    CHECK(solveStraightCameraPath(asset,0,{-1,0,0},out)==CameraPathStatus::NoHit);
    CHECK(out.parameter==previous.parameter && out.untouchedWord==previous.untouchedWord);
    auto duplicate=straightFixture();number(duplicate,0x1d0+3*12,2);
    number(duplicate,0x1d0+9*12+4,2);CHECK(asset.decode(duplicate));
    CHECK(supportsStraightCameraPath(asset,0));
    CHECK(solveStraightCameraPath(asset,0,{2,0,0},out)==CameraPathStatus::Found);
    CHECK(out.segment==1 && out.flags==0); // Duplicate retains earlier equal-distance winner.
    number(duplicate,0x1d0+9*12+4,3);CHECK(asset.decode(duplicate));
    CHECK(!supportsStraightCameraPath(asset,0)); // Fraction-dependent zero chord is outside the finite proof.
    CHECK(solveStraightCameraPath(asset,0,{2,0,0},out)==CameraPathStatus::Unsupported);
    CHECK(out.parameter==previous.parameter);
    auto curved=straightFixture();number(curved,0x1d0+7*12+8,.25f);CHECK(asset.decode(curved));
    CHECK(!supportsStraightCameraPath(asset,0));
    auto near=straightFixture();number(near,0x1d0+9*12+8,.005f);CHECK(asset.decode(near));
    CHECK(supportsStraightCameraPath(asset,0));
    CHECK(solveStraightCameraPath(asset,0,{2.5f,0,.25f},out)==CameraPathStatus::Found);
    CHECK(out.parameter==2.5f && out.tangent.z==.0025f && out.anchor.z==.25f);
    auto skew=straightFixture();
    for(int n=0;n<6;++n)number(skew,0x1d0+(6+n)*12+8,.005f);
    CHECK(asset.decode(skew));CHECK(supportsStraightCameraPath(asset,0));
    CHECK(solveStraightCameraPath(asset,0,{-.001f,0,1},out)==CameraPathStatus::Found);
    CHECK(out.segmentStartY==10 && out.parameter==2 && out.flags==1);
    CHECK(out.anchor.x==1.995f && out.anchor.z==1 && out.tangent.z==.005f);
}
void zoneCases() {
    const CameraPoint a{0,0,0},b{0,0,2},c{2,0,0};float best=-100000000;
    CHECK(cameraZoneTriangle({.5f,1,.5f},a,b,c,best) && best==0);
    CHECK(cameraZoneTriangle({.5f,1,.5f},a,c,b,best) && best==0); // Equal-height replacement, either winding above.
    CHECK(!cameraZoneTriangle({.5f,0,.5f},a,b,c,best));
    CHECK(cameraZoneTriangle({.5f,0,.5f},a,c,b,best)); // Coplanar admission is winding-dependent.
    CHECK(!cameraZoneTriangle({.5f,-1,.5f},a,b,c,best));
    CHECK(!cameraZoneTriangle({2,1,2},a,b,c,best));
    CHECK(cameraZoneTriangle({1,1,1},a,b,c,best)); // Inclusive edge.
    CHECK(!cameraZoneTriangle({0,1,0},a,a,a,best));
    CHECK(!cameraZoneTriangle({0,1,0},a,{0,1,0},b,best)); // Vertical plane, nonzero 3D normal.
    CHECK(best==0);
    CHECK(cameraZoneTriangle({.5f,2,.5f},a,b,{2,2,0},best) && best==.5f);
    CHECK(!cameraZoneTriangle({.5f,2,.5f},a,b,c,best) && best==.5f);
    CameraRoadAsset asset;auto raw=fixture();number(raw,0x118+0x8c,2);
    CHECK(asset.decode(raw));auto selected=selectCameraRoadZone(asset,{0,2,0});
    CHECK(selected && selected->road==0 && selected->height==2);
    CHECK(!selectCameraRoadZone(asset,{0,1,0}));
    put(raw,0x118+0x28,1);CHECK(asset.decode(raw));CHECK(!selectCameraRoadZone(asset,{0,3,0}));
}
void stateCases() {
    auto raw=straightFixture();raw.insert(raw.begin()+0x1d0,184,0);
    put(raw,0,raw.size());put(raw,0xc8,2);put(raw,0xf8,0x288);
    std::memcpy(raw.data()+0x1d0,raw.data()+0x118,184);
    number(raw,0x118+0x58,10);number(raw,0x1d0+0x58,20);
    number(raw,0x1d0+0x8c,2);number(raw,0x118+0x7c,100);number(raw,0x1d0+0x7c,100);
    CameraRoadAsset asset;CHECK(asset.decode(raw));CameraRoadWork work;
    auto inactive=cameraRoadFrame(work.current);CHECK(inactive && inactive->eye.y==10 && inactive->eye.z==10 && inactive->fov==25);
    CameraRoadState viewState;viewState.active=true;viewState.anchor={20,3,0};viewState.direction={0,0,1};viewState.distance=50;
    auto frame=cameraRoadFrame(viewState);CHECK(frame && frame->eye.x==2 && frame->eye.z==5 && frame->at.x==2 && frame->at.z==0 && frame->yaw==0);
    auto screen=projectCameraRoot(frame->at,frame->view,frame->projection,frame->viewport);
    CHECK(screen.valid && screen.x==304 && screen.y==240);
    CHECK(frame->nearPlane==.01f && frame->farPlane==327.68f);
    viewState.secondary=90;frame=cameraRoadFrame(viewState);
    CHECK(frame && std::fabs(frame->at.y+4.7f)<.0001f && std::fabs(frame->at.z-5)<.0001f);
    viewState.secondary=0;viewState.direction={1,0,0};frame=cameraRoadFrame(viewState);
    CHECK(frame && frame->yaw==270);
    CHECK(work.update(asset,{25,10,0},{25,10,0},0,16));
    CHECK(work.current.active && work.current.road==0 && !work.transitioning);
    CHECK(work.current.anchor.x==2.5f && work.current.anchor.y==1 && work.current.distance==10);
    CHECK(work.current.milliseconds==16 && work.current.shiftRate==100);
    CHECK(work.update(asset,{25,30,0},{25,30,0},0,33));
    CHECK(work.transitioning && work.progress==0 && work.current.road==0);
    CHECK(work.origin.road==0 && work.destination.road==1 && work.changeMask==1);
    CHECK(std::fabs(work.rate-1.f/30)<.000001f && work.anchorBlend);
    CHECK(work.update(asset,{25,30,0},{25,30,0},0,50));
    CHECK(work.progress>0 && work.current.distance>10 && work.current.distance<20 && work.current.road==0);
    CHECK(work.update(asset,{25,10,0},{25,10,0},0,66));
    CHECK(work.transitioning && work.progress==0 && work.origin.distance>10 && work.origin.distance<20);
    CHECK(work.origin.road==0 && work.destination.road==0); // Restart from displayed geometry, not old origin.
    CHECK(work.update(asset,{25,10,0},{25,10,0},0,2000));
    CHECK(!work.transitioning && work.current.distance==10 && work.current.milliseconds==2000);
    float rate=77;auto a=work.current,b=a;b.anchor.x+=100;b.distance+=100;
    CHECK(compareCameraRoad(asset,a,b,rate)==0 && rate==77); // Same-record early gate ignores geometry.
    b.zone=false;CHECK(compareCameraRoad(asset,a,b,rate)==2 && rate==1.f/90);
    b=a;b.road=1;CHECK(compareCameraRoad(asset,a,b,rate)==0); // Mask alone cannot trigger tiny cost.
    b.distance=20;CHECK(compareCameraRoad(asset,a,b,rate)==1 && std::fabs(rate-1.f/30)<.000001f);
    b.active=false;rate=77;CHECK(compareCameraRoad(asset,a,b,rate)==0 && rate==77);
    CameraRoadState blended=a;blended.flags=123;blended.milliseconds=999;
    b=a;b.anchor.x=4;b.direction={0,0,-1};b.distance=30;
    blendCameraRoad(true,a,b,blended,.5f);
    CHECK(blended.road==a.road && blended.flags==123 && blended.milliseconds==999);
    CHECK(blended.distance>20 && blended.distance<20.001f); // Literal easing scale exceeds .5.
    blendCameraRoad(false,a,b,blended,0);CHECK(blended.anchor.x==4);
    CameraRoadWork moving;CHECK(moving.update(asset,{25,10,20},{25,10,20},0,16));
    CHECK(moving.destination.flags==2 && moving.destination.anchor.z==1);
    CHECK(moving.update(asset,{25,10,0},{25,10,0},1,33));
    CHECK(moving.candidate.flags==0 && moving.normalWeight==.008f && moving.candidate.anchor.z==1);
    CHECK(moving.candidate.segment==2 && moving.candidate.shift==1);
    CHECK(moving.update(asset,{25,10,0},{25,-10,0},1,50));
    CHECK(!moving.candidate.zone && moving.candidate.anchor.x==2.5f && moving.candidate.anchor.z==0);
    const auto before=moving;CHECK(!moving.update(asset,{25,10,0},{25,10,0},1,0));
    CHECK(moving.current.anchor.z==before.current.anchor.z && moving.normalWeight==before.normalWeight);
}
int main(int argc,char**) {
    if(argc>1) {
        CHECK(loadCameraRoadAssets("romfs"));
        const uint32_t expected[]={11,3,14,14,2};uint32_t total=0,eligible=0,triangles=0,regularTriangles=0;
        for(uint32_t room=0;room<5;++room) {
            const auto* asset=cameraRoadAsset(room);CHECK(asset && asset->collections()[1].count==expected[room]);
            CHECK(asset->versionCode()==101 && asset->version()=="1.01");
            const auto lens=*asset->lensMetadata();CHECK(lens[0]==25 && lens[1]==.01f && lens[2]==1000);
            CHECK(asset->collections()[8].count==0 && asset->collections()[8].offset==0xffbf3a10);
            for(uint32_t n=0;n<expected[room];++n){CHECK(!asset->roadName(n)->empty());CHECK(std::isfinite(*asset->roadFloat(n,0x58)));}
            for(uint32_t n=0;n<asset->collections()[4].count;++n){const auto p=*asset->pathVector(n);CHECK(std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z));}
            for(uint32_t n=0;n<expected[room];++n)if(supportsStraightCameraPath(*asset,n)) {
                ++eligible;const auto start=*asset->roadWord(n,0xa0);const auto point=*asset->pathVector(start+2);
                CameraCurveOutput output;output.untouchedWord=0xabcdef12;
                CHECK(solveStraightCameraPath(*asset,n,point,output)==CameraPathStatus::Found);
                CHECK(output.flags==0 && std::fabs(output.anchor.x-point.x)<.0001f && std::fabs(output.anchor.z-point.z)<.0001f);
                CHECK(output.untouchedWord==0xabcdef12);
            }
            CameraRoadWork originalWork;
            for(uint32_t n=0;n<expected[room];++n) {
                const auto point=*asset->pathVector(*asset->roadWord(n,0xa0)+2);
                CHECK(originalWork.update(*asset,{point.x*10,(point.y+100)*10,point.z*10},
                                          {point.x*10,(point.y+100)*10,point.z*10},1,(n+1)*17));
            }
            total+=expected[room];
            for(uint32_t z=0;z<asset->collections()[2].count;++z) {
                const auto base=*asset->zoneWord(z,0x58),first=*asset->zoneWord(z,0x60),count=*asset->zoneWord(z,0x64);
                for(uint32_t p=first;p<first+count;++p) {
                    const auto polygon=*asset->polygon(p);if(polygon[1]<3)continue;
                    const auto a=*asset->zoneVertex(base+*asset->vertexIndex(polygon[0]));
                    for(uint32_t i=1;i+1<polygon[1];++i) {
                        const auto b=*asset->zoneVertex(base+*asset->vertexIndex(polygon[0]+i));
                        const auto c=*asset->zoneVertex(base+*asset->vertexIndex(polygon[0]+i+1));
                        ++triangles;const float ny=(b.z-a.z)*(c.x-a.x)-(b.x-a.x)*(c.z-a.z);
                        float best=-100000000;const CameraPoint centroid{(a.x+b.x+c.x)/3,(a.y+b.y+c.y)/3+1,(a.z+b.z+c.z)/3};
                        const bool hit=cameraZoneTriangle(centroid,a,b,c,best);
                        // The exported slivers can be narrower than the f32
                        // centroid rounding error. This conditioning guard is
                        // test-only; production retains strict source edges.
                        const float extent=std::fabs(b.x-a.x)+std::fabs(b.z-a.z)+std::fabs(c.x-a.x)+std::fabs(c.z-a.z);
                        if(std::fabs(ny)>.00001f*extent*extent){++regularTriangles;CHECK(hit);CHECK(std::fabs(best-(a.y+b.y+c.y)/3)<.001f);}
                        else if(hit)CHECK(std::isfinite(best));
                        else CHECK(best==-100000000);
                    }
                }
            }
        }
        CHECK(eligible==44);CHECK(!cameraRoadAsset(5));CHECK(!loadCameraRoadAssets("/missing-camera-directory"));
        CHECK(cameraRoadAsset(0)->collections()[1].count==11);
        CHECK(loadCollision("romfs/map.walls") && loadGround("romfs/map.ground"));
        Game game;game.restore(game.progress);game.tick({});game.tick({});
        CHECK(game.fieldCameraAuthored && game.fieldCameraFrame.fov==25);
        CHECK(game.cameraRoadWork.current.active && game.fieldCameraCandidateActive);
        CHECK(game.fieldCameraCandidateShiftRate==game.cameraRoadWork.candidate.shiftRate);
        CHECK(game.fieldCameraFrame.eye.z!=game.cameraZ+6.5f);
        const auto clock=game.fieldCameraShift.previousMilliseconds;game.restore(game.progress);
        CHECK(!game.cameraRoadWork.current.active && !game.fieldCameraCandidateActive && game.fieldCameraShift.previousMilliseconds==clock);
        game.tick({});CHECK(game.fieldCameraAuthored);clearCollision();
        CHECK(triangles==494 && regularTriangles>400);
        std::cout<<"Original camera assets passed: "<<total<<" authored roads, "<<triangles<<" zone triangles in five rooms\n";return 0;
    }
    curveCases();zoneCases();stateCases();CameraRoadAsset asset;auto raw=fixture();CHECK(asset.decode(raw));CHECK(asset.bytes()==raw);
    CHECK(asset.roadName(0)=="C_fixture" && asset.roadFloat(0,0x58)==42.5f && asset.roadWord(0,0x80)==0x89abcdef);
    CHECK(asset.collections()[8].offset==0xffbf3a10 && asset.collections()[8].count==0);
    CHECK((*asset.lensMetadata())[0]==25);
    const auto prior=asset.bytes();
    auto reject=[&](std::vector<uint8_t> malformed){CHECK(!asset.decode(std::move(malformed)));CHECK(asset.bytes()==prior);};
    auto bad=raw;bad.pop_back();reject(bad);bad=raw;bad[4]='x';reject(bad);
    bad=raw;bad[0x47]='2';reject(bad);bad=raw;put(bad,0xec,0xfffffff0);reject(bad);
    bad=raw;put(bad,0xc8,0x80000000);reject(bad);
    bad=raw;put(bad,0xe4,1);reject(bad); // Unclosed108 stride has no invented reader.
    bad=raw;put(bad,0x118+0xa4,1);reject(bad); // Nonempty path needs actual vector data.
    bad=raw;std::memset(bad.data()+0x118,'X',32);reject(bad);
    CHECK(!asset.roadWord(1,0));CHECK(!asset.pathVector(0));
    CHECK(!asset.roadWord(0,183));
    CHECK(!asset.load("/missing-camera"));CHECK(asset.bytes()==prior);
    std::cout<<"Camera container preservation, bounds and rollback passed\n";
}
