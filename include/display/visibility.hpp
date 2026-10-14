#pragma once
namespace ttyd {
// Native world-space cutaway geometry. Boolean intersection does not change
// collision, source visibility flags or actor position; renderer owns hide hold.
struct Point3 {float x,y,z;};
struct Bounds3 {Point3 min,max;};
bool segmentHitsBounds(Point3 from,Point3 to,const Bounds3& bounds);
bool segmentHitsTriangle(Point3 from,Point3 to,Point3 a,Point3 b,Point3 c);
}
