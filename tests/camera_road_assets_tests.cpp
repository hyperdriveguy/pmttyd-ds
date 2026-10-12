#include "camera_road_assets.hpp"
#include "camera_road_curve.hpp"
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
    auto duplicate=straightFixture();number(duplicate,0x1d0+12,0);CHECK(asset.decode(duplicate));
    CHECK(!supportsStraightCameraPath(asset,0));
    CHECK(solveStraightCameraPath(asset,0,{2,0,0},out)==CameraPathStatus::Unsupported);
    CHECK(out.parameter==previous.parameter);
    auto curved=straightFixture();number(curved,0x1d0+7*12+8,.25f);CHECK(asset.decode(curved));
    CHECK(!supportsStraightCameraPath(asset,0));
}
int main(int argc,char**) {
    if(argc>1) {
        CHECK(loadCameraRoadAssets("romfs"));
        const uint32_t expected[]={11,3,14,14,2};uint32_t total=0,eligible=0;
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
                CHECK(output.flags==0 && output.segment==1 && output.anchor.x==point.x && output.anchor.z==point.z);
                CHECK(output.untouchedWord==0xabcdef12);
            }
            total+=expected[room];
        }
        CHECK(eligible==19);CHECK(!cameraRoadAsset(5));CHECK(!loadCameraRoadAssets("/missing-camera-directory"));
        CHECK(cameraRoadAsset(0)->collections()[1].count==11);
        std::cout<<"Original camera assets passed: "<<total<<" authored roads in five rooms\n";return 0;
    }
    curveCases();CameraRoadAsset asset;auto raw=fixture();CHECK(asset.decode(raw));CHECK(asset.bytes()==raw);
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
