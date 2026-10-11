#include "visibility.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
Point3 sub(Point3 a,Point3 b) {return {a.x-b.x,a.y-b.y,a.z-b.z};}
Point3 cross(Point3 a,Point3 b) {return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
float dot(Point3 a,Point3 b) {return a.x*b.x+a.y*b.y+a.z*b.z;}
bool finite(Point3 p) {return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z);}
}
bool segmentHitsBounds(Point3 from,Point3 to,const Bounds3& b) {
    if(!finite(from) || !finite(to) || !finite(b.min) || !finite(b.max))return false;
    float origin[]={from.x,from.y,from.z},end[]={to.x,to.y,to.z};
    float lo[]={b.min.x,b.min.y,b.min.z},hi[]={b.max.x,b.max.y,b.max.z};
    float first=0,last=1;
    for(int n=0;n<3;++n) {
        if(lo[n]>hi[n])return false;
        float d=end[n]-origin[n];
        if(std::fabs(d)<1e-8f) {if(origin[n]<lo[n] || origin[n]>hi[n])return false;continue;}
        float a=(lo[n]-origin[n])/d,c=(hi[n]-origin[n])/d;if(a>c)std::swap(a,c);
        first=std::max(first,a);last=std::min(last,c);if(first>last)return false;
    }
    return true;
}
bool segmentHitsTriangle(Point3 from,Point3 to,Point3 a,Point3 b,Point3 c) {
    if(!finite(from) || !finite(to) || !finite(a) || !finite(b) || !finite(c))return false;
    Point3 direction=sub(to,from),ab=sub(b,a),ac=sub(c,a),p=cross(direction,ac);
    float determinant=dot(ab,p);if(std::fabs(determinant)<1e-7f)return false;
    float inverse=1/determinant;Point3 offset=sub(from,a);
    float u=dot(offset,p)*inverse;if(u<-.00001f || u>1.00001f)return false;
    Point3 q=cross(offset,ab);float v=dot(direction,q)*inverse;
    if(v<-.00001f || u+v>1.00001f)return false;
    float t=dot(ac,q)*inverse;
    // Geometry touching the camera or actor is not a foreground occluder.
    return t>.0001f && t<.9999f;
}
}
