#include "camera_road_assets.hpp"
#include "camera_road_curve.hpp"
#include "camera_road_zone.hpp"
#include "camera_road_state.hpp"
#include "camera_road_view.hpp"
#include "game.hpp"
#include "actor_mesh.hpp"
#include "actor_animation.hpp"
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
void actorPackCases() {
    auto point=actorMeshPoint({10,-2,3,0,0},{1,2,3},0,false);
    CHECK(std::fabs(point[0]-1.2f)<.00001f && std::fabs(point[1]-1.96f)<.00001f && std::fabs(point[2]-3.06f)<.00001f);
    point=actorMeshPoint({10,-2,3,0,0},{1,2,3},90,true);
    CHECK(std::fabs(point[0]-.94f)<.00001f && std::fabs(point[2]-2.8f)<.00001f); // Z reflection precedes yaw; no X/UV mirror.
    ActorMeshPack pack;FILE* file=std::tmpfile();CHECK(file);
    const char magic[8]="PMCHAR1",name[32]="fixture";uint32_t count=1,size=3;
    const ActorMeshVertex vertices[]={{-1,-2,0,0,0},{1,-2,0,1,0},{0,2,0,.5f,1}};
    CHECK(std::fwrite(magic,8,1,file)==1 && std::fwrite(&count,4,1,file)==1);
    CHECK(std::fwrite(name,32,1,file)==1 && std::fwrite(&size,4,1,file)==1 && std::fwrite(vertices,sizeof(vertices),1,file)==1);
    std::rewind(file);CHECK(pack.read(file));CHECK(pack.pose("fixture") && pack.pose("fixture")->vertices[0].y==-2);
    CHECK(!pack.pose("unknown"));
    std::fseek(file,8,SEEK_SET);count=0;CHECK(std::fwrite(&count,4,1,file)==1);
    std::rewind(file);CHECK(!pack.read(file) && pack.pose("fixture"));
    std::fclose(file);CHECK(!pack.load("/missing-actor-pack") && pack.pose("fixture"));
}
void actorAnimationCases() {
    ActorAnimationPose p;p.name="fixture";p.end=4;
    p.positions={{0,0,0},{1,0,0},{0,1,0}};p.normals={{0,1,0}};p.nodes.resize(24);
    p.nodes[3]=p.nodes[4]=p.nodes[5]=1;p.groups={{-1,-1,0,0,1}};
    p.bindings={{0,0,0,0,0},{0,1,0,1,0},{0,2,0,0,1}};
    ActorTextureBinding texture{};texture.cells.fill(0xffff);texture.cells[0]=0;texture.cells[1]=1;p.textures={texture};
    ActorAnimationKey zero,finish;finish.time=4;
    finish.positions={{0,16,0,0,0}};finish.normals={{0,-16,8,32,0}};finish.nodes={{1,16,0,0,0},{2,16,0,90,0}};finish.textures={{0,1,{0,0,0}}};
    p.keys={zero,finish};ActorAnimationSample sample;
    CHECK(p.evaluate(1,sample));CHECK(sample.positions[0][0]==.25f && sample.nodes[1]==.15625f);
    CHECK(sample.nodes[2]==0 && sample.textures[0]==0 && sample.fraction==.25f);
    CHECK(sample.normals[0][0]==-.25f && sample.normals[0][1]==1.125f && sample.normals[0][2]==.5f);
    CHECK(sample.vertices[0].x==.25f && sample.vertices[0].y==.15625f);
    CHECK(p.evaluate(4,sample));CHECK(sample.positions[0][0]==1 && sample.nodes[1]==1 && sample.nodes[2]==1);
    CHECK(sample.textures[0]==1 && sample.vertices[0].u==130.f/1024 && sample.next==sample.last);
    CHECK(sample.normals[0][0]==-1 && sample.normals[0][1]==1.5f && sample.normals[0][2]==2);
    p.loop=1;CHECK(p.evaluate(4,sample));CHECK(sample.selectedTime==0 && sample.textures[0]==0 && sample.nodes[1]==0);
    CHECK(p.evaluate(5,sample));CHECK(sample.positions[0][0]==.25f && sample.nodes[1]==.15625f);
    CHECK(p.evaluate(.5f,sample));CHECK(sample.nodes[1]==.04296875f); // Fractional rewind rebuilds full cache.
    CHECK(p.evaluate(-1,sample) && sample.selectedTime==0);
    CHECK(!p.evaluate(std::numeric_limits<float>::infinity(),sample));
    const ActorSparseUpdate tangent{0,16,45,0,0};
    CHECK(std::fabs(actorNodeDelta(tangent,.25f,4)-.71875f)<.00001f);
    float n[24]={};n[3]=n[4]=1;n[5]=2;const float ones[]={1,1,1};
    CHECK(actorNodeMatrix(n,true,ones).v[10]==1 && actorNodeMatrix(n,false,ones).v[10]==2);
    n[5]=1;n[18]=5;CHECK(actorNodeMatrix(n,false,ones).v[3]==0);n[12]=10;
    CHECK(actorNodeMatrix(n,false,ones).v[3]==5);CHECK(actorNodeMatrix(n,true,ones).v[3]==0);
    const float parentZero[]={2,0,4};CHECK(actorNodeMatrix(n,true,parentZero).v[0]==1);
    MarioPresentation presentation;presentation.update(0,0,0,10,1000);
    CHECK(presentation.animationFrame()==0);presentation.update(0,0,0,10,2000);
    CHECK(presentation.animationFrame()==60);CHECK(presentation.animationFrame()==60);
    presentation.update(11,0,0,10,2500);presentation.update(0,0,0,10,3000);
    CHECK(presentation.animationFrame()==0); // Front request queues a reset of the same final rear name.
    presentation.facing=90;presentation.update(0,0,0,10,4000);CHECK(presentation.animationFrame()==0);
    presentation.update(11,0,0,10,4500);presentation.update(0,0,0,10,5000);
    CHECK(presentation.animationFrame()==60); // Same current front request does not queue.

    // Local binary fixture verifies parsing, rollback and malformed graph rejection.
    auto fixture=[&](bool cycle,bool version2=false,bool badNormal=false) {
        FILE* file=std::tmpfile();CHECK(file);
        auto write=[&](const auto& value){CHECK(std::fwrite(&value,sizeof(value),1,file)==1);};
        const char magic[8]="PMANIM1",name[32]="fixture";uint32_t count=1;if(version2){const char magic2[8]="PMANIM2";write(magic2);}else write(magic);write(count);write(name);
        write(p.loop);write(p.start);write(p.end);const uint32_t sizes[]={3,24,1,3,1,2};write(sizes);
        if(version2){uint32_t normals=1;write(normals);}
        if(badNormal)p.keys[1].normals[0].index=1;
        for(const auto& v:p.positions)write(v);if(version2)for(const auto& v:p.normals)write(v);for(auto v:p.nodes)write(v);
        auto g=p.groups[0];if(cycle)g.sibling=0;write(g);
        for(const auto& b:p.bindings)write(b);write(texture);
        for(const auto& key:p.keys) {
            write(key.time);const uint32_t counts[]={uint32_t(key.positions.size()),uint32_t(key.nodes.size()),uint32_t(key.textures.size())};write(counts);if(version2){uint32_t normals=key.normals.size();write(normals);}
            for(const auto& r:key.positions)write(r);if(version2)for(const auto& r:key.normals)write(r);for(const auto& r:key.nodes)write(r);for(const auto& r:key.textures)write(r);
        }
        std::rewind(file);return file;
    };
    ActorAnimationPack pack;auto file=fixture(false);CHECK(pack.read(file));std::fclose(file);
    CHECK(pack.pose("fixture") && !pack.pose("missing"));file=fixture(true);CHECK(!pack.read(file));std::fclose(file);
    CHECK(pack.pose("fixture") && !pack.load("/missing-animation-pack"));
    file=fixture(false,true);CHECK(pack.read(file));std::fclose(file);
    CHECK(pack.pose("fixture")->evaluate(1,sample) && sample.normals[0][2]==.5f);
    file=fixture(false,true,true);CHECK(!pack.read(file));std::fclose(file);
    CHECK(pack.pose("fixture")->evaluate(1,sample) && sample.normals[0][2]==.5f);
}
int main(int argc,char** argv) {
    if(argc>1) {
        CHECK(loadCameraRoadAssets("romfs"));
        ActorMeshPack mesh;CHECK(mesh.load("romfs/characters.pmmesh"));
        ActorAnimationPack animations;CHECK(animations.load("romfs/characters.pmanim"));
        for(const auto name:{"M_S_1","M_W_1","M_R_1","M_J_1B","M_J_1C","M_S_3","M_I_S","M_I_Y","M_I_5","M_N_5A","M_F_1","M_D_2","M_U_3","M_S_1R","M_W_1R","M_R_1R","M_J_1BR","M_J_1CR"}) {
            const auto* pose=mesh.pose(name);CHECK(pose);
            if(std::strcmp(name,"M_I_5") && std::strcmp(name,"M_N_5A") && std::strcmp(name,"M_F_1"))
                CHECK(pose->vertices.size()==(name[std::strlen(name)-1]=='R'?186u:306u));
            const auto* animation=animations.pose(name);CHECK(animation);ActorAnimationSample sample;
            CHECK(animation->evaluate(0,sample) && sample.vertices.size()==pose->vertices.size());
            for(size_t i=0;i<sample.vertices.size();++i) {
                const auto& a=sample.vertices[i];const auto& b=pose->vertices[i];
                CHECK(std::fabs(a.x-b.x)<.0001f && std::fabs(a.y-b.y)<.0001f && std::fabs(a.z-b.z)<.0001f);
                CHECK(std::fabs(a.u-b.u)<.0000002f && std::fabs(a.v-b.v)<.0000002f);
            }
            const auto base=sample.vertices;bool changed=false;
            for(float frame:{.25f,1.5f,animation->end*.5f,animation->end-.25f,animation->end,animation->end+.25f}) {
                CHECK(animation->evaluate(frame,sample));
                for(size_t i=0;i<sample.vertices.size();++i) {
                    const auto& v=sample.vertices[i];CHECK(std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z));
                    changed|=v.x!=base[i].x || v.y!=base[i].y || v.z!=base[i].z || v.u!=base[i].u || v.v!=base[i].v;
                }
            }
            // Some authored rear jump streams are static after key0; do not
            // invent movement for those source poses. Walk/run must advance.
            if(std::strncmp(name,"M_W_",4)==0 || std::strncmp(name,"M_R_",4)==0)CHECK(changed);
        }
        if(argc>2) {
            FILE* reference=std::fopen(argv[2],"rb");CHECK(reference);
            char magic[8];uint32_t cases=0;
            CHECK(std::fread(magic,8,1,reference)==1 && std::memcmp(magic,"PMREF1\0",8)==0);
            CHECK(std::fread(&cases,4,1,reference)==1 && cases>0 && cases<10000);
            float error=0;
            for(uint32_t c=0;c<cases;++c) {
                char name[32];float frame=0;uint32_t count=0;
                CHECK(std::fread(name,32,1,reference)==1 && std::memchr(name,0,32));
                CHECK(std::fread(&frame,4,1,reference)==1 && std::fread(&count,4,1,reference)==1);
                const auto* pose=animations.pose(name);CHECK(pose);ActorAnimationSample sample;
                CHECK(pose->evaluate(frame,sample) && count==sample.vertices.size());
                for(uint32_t v=0;v<count;++v) {
                    ActorMeshVertex expected;CHECK(std::fread(&expected,sizeof(expected),1,reference)==1);
                    const auto& actual=sample.vertices[v];
                    const float local=std::max({std::fabs(actual.x-expected.x),std::fabs(actual.y-expected.y),std::fabs(actual.z-expected.z)});
                    error=std::max(error,local);CHECK(local<.0002f);
                    CHECK(std::fabs(actual.u-expected.u)<.0000002f && std::fabs(actual.v-expected.v)<.0000002f);
                }
            }
            CHECK(std::fgetc(reference)==EOF);std::fclose(reference);
            std::cout<<cases<<" independent animation samples passed; max host model error "<<error<<"\n";
        }
        const auto* idle=mesh.pose("M_S_1");float low=1000,high=-1000;
        for(const auto& v:idle->vertices){low=std::min(low,v.y);high=std::max(high,v.y);}
        CHECK(std::fabs(low*.02f+.03316393f)<.00001f && std::fabs(high*.02f-.43603596f)<.00001f);

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
        const auto roadStamp=game.cameraRoadWork.candidate.milliseconds;
        CHECK(game.setCameraView({400,100,600},{400,0,0},1000,11));
        const auto currentEye=game.fieldCameraFrame.eye;
        game.tick({});CHECK(game.fieldCameraEvents.view.state==3);
        CHECK(game.cameraRoadWork.candidate.milliseconds==roadStamp); // Preset branch bypasses road/shift-main.
        CHECK(game.fieldCameraShift.previousMilliseconds>roadStamp); // Post still runs.
        CHECK(game.fieldCameraFrame.eye.x>currentEye.x && game.fieldCameraFrame.eye.x<4);
        CHECK(game.setCameraTarget({300,0,100},25,1000,11));
        const auto targetStart=game.fieldCameraEvents.target.start;
        game.fieldCameraEvents.nearPlane=2;game.fieldCameraEvents.farPlane=10000;
        game.fieldCameraFlags|=0x10;const auto retainedView=game.fieldCameraFrame.eye;
        const auto clock=game.fieldCameraShift.previousMilliseconds;game.restore(game.progress);
        CHECK(!(game.fieldCameraFlags&0x10) && game.fieldCameraEvents.nearPlane==2 && game.fieldCameraEvents.farPlane==10000);
        CHECK(game.fieldCameraFrame.eye.x==retainedView.x && game.fieldCameraFrame.eye.z==retainedView.z);
        CHECK(!game.cameraRoadWork.current.active && !game.fieldCameraCandidateActive && game.fieldCameraShift.previousMilliseconds==clock);
        CHECK(game.fieldCameraEvents.target.state==3 && game.fieldCameraEvents.target.start==targetStart && game.fieldCameraEvents.endPercent==25);
        CHECK(game.fieldCameraEvents.view.state==0 && game.fieldCameraEvents.storedEye.x==400);
        game.tick({});CHECK(game.fieldCameraAuthored);clearCollision();
        CHECK(triangles==494 && regularTriangles>400);
        std::cout<<"Original camera assets passed: "<<total<<" authored roads, "<<triangles<<" zone triangles in five rooms\n";return 0;
    }
    curveCases();zoneCases();stateCases();actorPackCases();actorAnimationCases();CameraRoadAsset asset;auto raw=fixture();CHECK(asset.decode(raw));CHECK(asset.bytes()==raw);
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
