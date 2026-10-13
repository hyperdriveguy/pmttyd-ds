#include "actor_animation.hpp"
#include "partner_presentation.hpp"
#include <cassert>
#include <iostream>
#include <cstdlib>
using namespace ttyd;
int main(int argc,char** argv) {
    if(argc==3 || argc==4) {
        const float tolerance=argc==4?std::strtof(argv[3],nullptr):.0002f;
        assert(std::isfinite(tolerance) && tolerance>0 && tolerance<=.001f);
        ActorAnimationPack pack;assert(pack.load(argv[1]));
        FILE* file=std::fopen(argv[2],"rb");assert(file);char magic[8];uint32_t cases;
        assert(std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMREF1\0",8));
        assert(std::fread(&cases,4,1,file)==1 && cases>0 && cases<10000);float maxError=0;
        for(uint32_t i=0;i<cases;++i){
            char name[32];float frame;uint32_t count;
            assert(std::fread(name,32,1,file)==1 && std::memchr(name,0,32));
            assert(std::fread(&frame,4,1,file)==1 && std::fread(&count,4,1,file)==1);
            auto* pose=pack.pose(name);assert(pose);ActorAnimationSample sample;
            assert(pose->evaluate(frame,sample) && sample.vertices.size()==count);
            for(uint32_t n=0;n<count;++n){ActorMeshVertex expected;assert(std::fread(&expected,sizeof(expected),1,file)==1);
                const auto& actual=sample.vertices[n];const float error=std::max({std::fabs(actual.x-expected.x),std::fabs(actual.y-expected.y),std::fabs(actual.z-expected.z)});
                maxError=std::max(maxError,error);assert(error<tolerance && std::fabs(actual.u-expected.u)<.0000002f && std::fabs(actual.v-expected.v)<.0000002f);
            }
        }
        assert(std::fgetc(file)==EOF);std::fclose(file);
        std::cout<<cases<<" original visibility/geometry samples passed, max error "<<maxError<<"\n";return 0;
    }
    ActorAnimationPose p;p.name="visibility";p.end=4;p.positions={{1,2,3}};p.nodes.resize(24);
    p.nodes[3]=p.nodes[4]=p.nodes[5]=1;
    // Root 3: hidden owner0 hides child1, but sibling2 remains independently visible.
    p.groups={{2,1,0,0,0},{-1,-1,0,0,1},{-1,-1,0,0,2},{-1,0,0,0,3}};
    p.visibility={0,1,255,1};p.bindings={{0,0,0,0,0},{1,0,0,0,0},{2,0,0,0,0}};
    ActorTextureBinding texture{};texture.cells.fill(0);p.textures={texture};
    ActorAnimationKey zero,finish;finish.time=4;finish.visibility={{0,1,{0,0,0}},{2,1,{0,0,0}}};p.keys={zero,finish};
    ActorAnimationSample sample;assert(p.evaluate(0,sample) && sample.vertices.size()==1 && sample.visibility[2]==255);
    assert(p.evaluate(3.999f,sample) && sample.vertices.size()==1); // No fractional visibility.
    assert(p.evaluate(4,sample) && sample.vertices.size()==2 && sample.visibility[2]==0);
    assert(p.evaluate(1,sample) && sample.vertices.size()==1 && sample.visibility[0]==0); // Rewind rebuilds bases.
    p.loop=1;assert(p.evaluate(4,sample) && sample.vertices.size()==1);
    p.groups[1].child=0;assert(!p.evaluate(0,sample)); // Hidden cycles rejected too.
    p.groups[1].child=-1;p.loop=0;
    auto fixture=[&](int scenario){
        FILE* file=std::tmpfile();assert(file);
        auto write=[&](const auto& v){assert(std::fwrite(&v,sizeof(v),1,file)==1);};
        const char magic[8]="PMANIM3",name[32]="visibility";const uint32_t count=1;
        write(magic);write(count);write(name);write(p.loop);write(p.start);write(p.end);
        const uint32_t sizes[]={1,24,4,3,1,uint32_t(p.keys.size()),0,uint32_t(scenario==1?0:4)};write(sizes);
        for(const auto& v:p.positions)write(v);for(float v:p.nodes)write(v);
        if(scenario!=1)for(uint8_t v:p.visibility)write(v);
        for(size_t i=0;i<p.groups.size();++i){auto g=p.groups[i];if(scenario==2 && i==0)g.visible=4;write(g);}
        for(const auto& b:p.bindings)write(b);write(texture);
        for(const auto& k:p.keys){write(k.time);const uint32_t counts[]={0,0,0,0,uint32_t(k.visibility.size())};write(counts);
            for(auto r:k.visibility){if(scenario==3)r.index=4;write(r);}
        }
        if(scenario==4){const uint8_t extra=0;write(extra);}
        std::rewind(file);return file;
    };
    ActorAnimationPack parsed;FILE* file=fixture(0);assert(parsed.read(file));std::fclose(file);
    assert(parsed.pose("visibility")->evaluate(4,sample) && sample.vertices.size()==2);
    for(int scenario=1;scenario<=4;++scenario){file=fixture(scenario);assert(!parsed.read(file));std::fclose(file);
        assert(parsed.pose("visibility")->evaluate(4,sample) && sample.vertices.size()==2);}
    p.end=0;p.keys.resize(1);
    assert(p.evaluate(100,sample) && sample.selectedTime==0 && sample.vertices.size()==1);
    file=fixture(0);assert(parsed.read(file));std::fclose(file);
    assert(parsed.pose("visibility")->evaluate(100,sample) && sample.vertices.size()==1);
    assert(partnerMovingPose(.8f,1.f,2)==2 && partnerMovingPose(.85f,1.f,2)==3);
    assert(partnerMovingPose(2.25f,1.f,1)==2 && partnerMovingPose(2.25f,1.f,2)==3);
    assert(partnerMovingPose(1.99f,.5f,2)==2 && partnerMovingPose(2.f,.5f,1)==3);
    assert(partnerMovingPose(0,1.f,1,true)==3);
    PartnerPresentation partner;partner.update(.01f,0,1,10,0);
    assert(partner.poseId==2 && partner.frame()==0 && partner.target==180 && partner.display==27.5f);
    partner.update(.01f,0,1,11,0);assert(partner.frame()==1);
    partner.update(-.0225f,0,2,12,10);assert(partner.poseId==3 && partner.frame()==0 && partner.target==0);
    partner.update(0,0,2,13,10);assert(partner.poseId==1 && partner.frame()==0);
    for(uint64_t tick=14;tick<74;++tick)partner.update(.0225f,0,2,tick,0);
    assert(partner.display==180 && partner.reflectZ() && partner.yaw()==180); // Original c_pkr faces travel +X.
    for(uint64_t tick=74;tick<134;++tick)partner.update(-.0225f,0,2,tick,0);
    assert(partner.display==0 && !partner.reflectZ() && partner.yaw()==0); // Travel -X.
    std::cout<<"visibility byte wrapping, stepping, hidden descendants/siblings and rewind passed\n";
}
