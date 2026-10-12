#include "camera_road_assets.hpp"
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
int main(int argc,char**) {
    if(argc>1) {
        CHECK(loadCameraRoadAssets("romfs"));
        const uint32_t expected[]={11,3,14,14,2};uint32_t total=0;
        for(uint32_t room=0;room<5;++room) {
            const auto* asset=cameraRoadAsset(room);CHECK(asset && asset->collections()[1].count==expected[room]);
            CHECK(asset->versionCode()==101 && asset->version()=="1.01");
            const auto lens=*asset->lensMetadata();CHECK(lens[0]==25 && lens[1]==.01f && lens[2]==1000);
            CHECK(asset->collections()[8].count==0 && asset->collections()[8].offset==0xffbf3a10);
            for(uint32_t n=0;n<expected[room];++n){CHECK(!asset->roadName(n)->empty());CHECK(std::isfinite(*asset->roadFloat(n,0x58)));}
            for(uint32_t n=0;n<asset->collections()[4].count;++n){const auto p=*asset->pathVector(n);CHECK(std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z));}
            total+=expected[room];
        }
        CHECK(!cameraRoadAsset(5));CHECK(!loadCameraRoadAssets("/missing-camera-directory"));
        CHECK(cameraRoadAsset(0)->collections()[1].count==11);
        std::cout<<"Original camera assets passed: "<<total<<" authored roads in five rooms\n";return 0;
    }
    CameraRoadAsset asset;auto raw=fixture();CHECK(asset.decode(raw));CHECK(asset.bytes()==raw);
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
