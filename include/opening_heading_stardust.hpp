#pragma once
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>
namespace ttyd {
struct OpeningHeadingStardust {
    struct Vertex {float x,y,z,s,t;std::array<uint8_t,4> rgba;};
    struct Particle {float x=0,y=0,vx=0,vy=0,scale=1;int32_t life=0,frame=-1;uint32_t scaleFrame=0;};
    struct Group {std::array<Particle,4> particles{};float x=0,y=0;uint32_t age=0;bool started=false;};
    std::array<std::array<Vertex,4>,8> quads{};
    std::array<uint8_t,36> palette{};std::array<uint8_t,7> scaleCurve{};
    // Source constants: acceleration, damping, scale unit, relaxation, tau, radiusY limit, fixed-point unit.
    std::array<float,7> constants{};
    std::array<Group,8> groups{};
    // Explicit native presentation seed; original global RNG history is unbound.
    uint32_t randomState=1;bool ready=false;
    bool load(const char* path){
        FILE* f=std::fopen(path,"rb");if(!f)return false;OpeningHeadingStardust next;char magic[8];
        bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMHSTAR1",8) && std::fread(next.quads.data(),sizeof(next.quads),1,f)==1 && std::fread(next.palette.data(),36,1,f)==1 && std::fread(next.scaleCurve.data(),7,1,f)==1 && std::fread(next.constants.data(),sizeof(next.constants),1,f)==1 && std::fgetc(f)==EOF;
        std::fclose(f);if(!ok)return false;
        for(const auto& q:next.quads)for(const auto& v:q)if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z)||!std::isfinite(v.s)||!std::isfinite(v.t))return false;
        const std::array<float,7> expected={-.05f,.96f,.01f,.1f,6.2831854820251465f,8.f,1000.f};if(next.constants!=expected)return false;
        next.ready=true;*this=next;return true;
    }
    uint32_t random(){randomState=randomState*1103515245u+12345u;return (randomState>>16)&32767u;}
    bool spawn(unsigned index,float x,float y){
        if(!ready || index>=groups.size() || groups[index].started)return false;
        auto& g=groups[index];g={};g.started=true;g.x=x;g.y=y;
        for(unsigned n=0;n<4;++n){auto& p=g.particles[n];const float radiusX=float(random()%uint32_t(constants[6]*40.f))/constants[6];const float radiusY=float(random()%uint32_t(constants[6]*constants[5]))/constants[6];const float angle=std::fma(360.f/4,float(n+1),float(random()%360));
            const float radians=(constants[4]*angle)/360.f;random(); // Original second draw; (rand%50)/1000 truncates to0.
            p.x=radiusX*float(std::cos(double(radians)));p.y=radiusY*float(std::sin(double(radians)));p.life=20+int32_t(n)*2;
        }return true;
    }
    void step(){if(!ready)return;for(auto& g:groups)if(g.started){++g.age;
        for(unsigned n=0;n<4;++n){auto& p=g.particles[n];p.frame=int32_t((g.age+n)&7);
            p.vx=p.vx*constants[1];p.vy=p.vy+constants[0];p.x=p.x+p.vx;p.y=p.y+p.vy;--p.life;
            if(p.life>20 || p.life<0){p.frame=-1;continue;}
            if(p.scaleFrame<7)p.scale=constants[2]*scaleCurve[p.scaleFrame++];
            else p.scale=std::fma(constants[3],constants[3]-p.scale,p.scale);
        }
    }}
    std::array<uint8_t,4> color(const Group& g,unsigned n)const{
        const unsigned at=((g.age-1)*3+(n+1)*3)%36;return {palette[at],palette[(at+1)%36],palette[(at+2)%36],255};
    }
};
static_assert(sizeof(OpeningHeadingStardust::Vertex)==24,"Stardust exported vertex layout");
}
