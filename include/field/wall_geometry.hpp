#pragma once
#include <algorithm>
#include <array>
#include <cmath>
namespace ttyd {
using WallTriangle=std::array<float,9>;
// Intersect the original triangle with a horizontal ray-height plane, then
// measure clearance against its actual XZ intersection rather than a bbox.
inline bool wallSliceBlocks(const WallTriangle& triangle,float y,float x,float z,float radius) {
    std::array<std::array<float,2>,6> points{};int count=0;
    for(int edge=0;edge<3;++edge) {
        const int a=edge*3,b=((edge+1)%3)*3;
        float da=triangle[a+1]-y,db=triangle[b+1]-y;
        if(std::fabs(da)<1e-6f)points[count++]={triangle[a],triangle[a+2]};
        if((da<0 && db>0) || (da>0 && db<0)) {
            float t=da/(da-db);
            points[count++]={triangle[a]+t*(triangle[b]-triangle[a]),triangle[a+2]+t*(triangle[b+2]-triangle[a+2])};
        }
    }
    if(!count)return false;
    for(int a=0;a<count;++a)for(int b=a;b<count;++b) {
        float dx=points[b][0]-points[a][0],dz=points[b][1]-points[a][1],den=dx*dx+dz*dz;
        float t=den>1e-12f?std::clamp(((x-points[a][0])*dx+(z-points[a][1])*dz)/den,0.f,1.f):0;
        float px=x-points[a][0]-t*dx,pz=z-points[a][1]-t*dz;
        if(px*px+pz*pz<radius*radius)return true;
    }
    return false;
}
}
