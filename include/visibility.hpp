#pragma once
namespace ttyd {
struct Point3 {float x,y,z;};
struct Bounds3 {Point3 min,max;};
bool segmentHitsBounds(Point3 from,Point3 to,const Bounds3& bounds);
bool segmentHitsTriangle(Point3 from,Point3 to,Point3 a,Point3 b,Point3 c);
}
