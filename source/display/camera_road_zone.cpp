#include "camera_road_zone.hpp"
#include <cmath>
namespace ttyd {
namespace {
CameraPoint sub(CameraPoint a,CameraPoint b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
CameraPoint cross(CameraPoint a,CameraPoint b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
float dot(CameraPoint a,CameraPoint b){return a.x*b.x+a.y*b.y+a.z*b.z;}
bool finite(CameraPoint p){return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z);}
}
// Source downward zone ray accepts either winding with its ordered side tests.
// Equal-height triangles replace best; no epsilon/normalization is invented.
// Query/triangle coordinates are authored engine/10 units.
bool cameraZoneTriangle(CameraPoint q,CameraPoint a,CameraPoint b,CameraPoint c,float& best) {
    if(!finite(q) || !finite(a) || !finite(b) || !finite(c) || !std::isfinite(best))return false;
    const auto e0=sub(a,b),e1=sub(c,a),e2=sub(b,c),n=cross(e0,e2);
    if(!finite(n) || (n.x==0 && n.y==0 && n.z==0))return false;
    const float s=dot(n,sub(q,a)),v=-n.y;
    if(!std::isfinite(s) || (s>0?v>=0:v<=0))return false;
    constexpr CameraPoint down{0,-1,0};
    for(float k:{dot(cross(e1,down),sub(q,a)),dot(cross(e0,down),sub(q,b)),dot(cross(e2,down),sub(q,c))})
        if(!std::isfinite(k) || (s>0?k>0:k<0))return false;
    const float height=q.y+s/v;
    if(!std::isfinite(height) || height<best)return false;
    best=height;return true;
}
std::optional<CameraZoneSelection> selectCameraRoadZone(const CameraRoadAsset& asset,CameraPoint look) {
    if(!finite(look))return std::nullopt;
    // Source no-hit height sentinel. Road+28 disables zone selection in v1.01;
    // road+a8/ac is zone range,road+8c fallback height when range is empty.
    // Empty-zone height requires strict >best; triangle equality can replace.
    float best=-100000000;std::optional<CameraZoneSelection> selected;
    for(uint32_t road=0;road<asset.collections()[1].count;++road) {
        if(asset.versionCode()==101 && *asset.roadWord(road,0x28)!=0)continue;
        const auto first=*asset.roadWord(road,0xa8),count=*asset.roadWord(road,0xac);
        if(!count) {
            const float height=*asset.roadFloat(road,0x8c);
            if(height<=look.y && height>best){best=height;selected=CameraZoneSelection{road,best};}
            continue;
        }
        for(uint32_t zone=first;zone<first+count;++zone) {
            const auto base=*asset.zoneWord(zone,0x58),polygonFirst=*asset.zoneWord(zone,0x60),polygonCount=*asset.zoneWord(zone,0x64);
            for(uint32_t p=polygonFirst;p<polygonFirst+polygonCount;++p) {
                const auto polygon=*asset.polygon(p);if(polygon[1]<3)continue;
                const auto a=*asset.zoneVertex(base+*asset.vertexIndex(polygon[0]));
                for(uint32_t i=1;i+1<polygon[1];++i) {
                    const auto b=*asset.zoneVertex(base+*asset.vertexIndex(polygon[0]+i));
                    const auto c=*asset.zoneVertex(base+*asset.vertexIndex(polygon[0]+i+1));
                    if(cameraZoneTriangle(look,a,b,c,best))selected=CameraZoneSelection{road,best};
                }
            }
        }
    }
    return selected;
}
}
